#include "patches.h"

// TODO: put these into the decomp
#define VEHICLE_ID_SIDESWIPE 1
#define VEHICLE_ID_SKYFALL 3
#define VEHICLE_ID_TRUCK 5
#define VEHICLE_ID_BALLISTA 10
#define VEHICLE_ID_MISSILE_CARRIER 0xFF

/*
Vehicle ids:
- Sideswipe: 1 (has wheel patch)
- Thunderfist: 2
- Skyfall: 3 (has wheel patch)
- Ramdozer: 4
- Backlash: 5 (has wheel patch)
- Train: 7
- J-Bomb: 9
- Ballista: 10 (has wheel patch)
- Police Car: 13
- Cyclone Suit: 16
- Missile Carrier: 0xFF (has wheel patch)

Unknown:
Starsky & Hutch Car
The Dukes of Hazard Car
The A-Team Van
Ferry
Crane
*/

// cannot use Mtx. different storage format. the assembly code transforms this into Mtx later
typedef struct {
    s32 m[4][4];
} FixedMtx;

typedef struct {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ u32 unk4;
    /* 0x08 */ FixedMtx mtx[];
} VehicleMtxTable;

typedef struct {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ u32 vtxOffset;
    /* 0x18 */ u32 mtxTableOffset;
} VehicleData;

typedef struct {
    u32 offset;
    u32 count;
} VtxRange;

typedef struct {
    u32 baseMtxOffset; // offset of pivot matrix, which puts the vertex to the center of the previous matrix
    VtxRange vtx[2]; // vertices drawn with the wheel matrix
} WheelDesc;

static const WheelDesc sTruckWheels[] = {
    { 0x080, { { 0x140, 41 } } }, // front left wheel, joint matrix 0x480
    { 0x180, { { 0x410, 42 } } }, // front right wheel, joint matrix 0x500
    { 0x200, { { 0x6B0, 92 } } }, // rear axle wheels (seem to be combined), joint matrix 0x540
};

static const WheelDesc sSkyfallWheels[] = {
    { 0x180, { { 0x1500, 40 } } }, // front left wheel, joint matrix 0x600
    { 0x300, { { 0x1780, 40 } } }, // front right wheel, joint matrix 0x6C0
    { 0x400, { { 0x1A00, 45 } } }, // rear left wheel, joint matrix 0x740
    { 0x480, { { 0x1CD0, 45 } } }, // rear right wheel, joint matrix 0x780
};

static const WheelDesc sSideswipeWheels[] = {
    { 0x100, { { 0x1450, 86 }, { 0x3800, 86 } } }, // front axle wheels, joint matrix 0x540
    { 0x180, { { 0x19B0, 86 }, { 0x3D60, 86 } } }, // rear axle wheels, joint matrix 0x580
};

static const WheelDesc sBallistaWheels[] = {
    { 0x100, { { 0x7C0, 52 }, { 0x1940, 52 } } }, // rear wheel, joint matrix 0x4C0
    { 0x280, { { 0xC60, 52 }, { 0x1DE0, 52 } } }, // front wheel, joint matrix 0x580
};

static const WheelDesc sMissileCarrierWheels[] = {
    { 0x000, { { 0x1270, 48 }, { 0x3210, 48 } } }, // front axle wheels, joint matrix 0x340
    { 0x080, { { 0x1570, 52 }, { 0x3510, 52 } } }, // middle axle wheels, joint matrix 0x380
    { 0x100, { { 0x18B0, 48 }, { 0x3850, 48 } } }, // rear axle wheels, joint matrix 0x3C0
};

static void recenterWheel(VehicleData* data, const WheelDesc* wheel) {
    VehicleMtxTable* mtxTable = (VehicleMtxTable*) ((u8*) data + data->mtxTableOffset);
    FixedMtx* mtx = &mtxTable->mtx[wheel->baseMtxOffset / sizeof(FixedMtx)];
    FixedMtx* invMtx = mtx + 1;
    s32 pivot[3];
    u32 r;
    u32 i;
    u32 j;

    for (j = 0; j < 3; j++) {
        // extract translation from inverse matrix
        // the inverse matrix puts the wheel back into its place after rotation
        pivot[j] = invMtx->m[3][j] >> 16;
    }

    // Move every vertex of the wheel to the center
    for (r = 0; r < sizeof(wheel->vtx) / sizeof(wheel->vtx[0]); r++) {
        Vtx* vtx = (Vtx*) ((u8*) data + data->vtxOffset + wheel->vtx[r].offset);
        for (i = 0; i < wheel->vtx[r].count; i++) {
            for (j = 0; j < 3; j++) {
                vtx[i].v.ob[j] -= pivot[j];
            }
        }
    }

    for (j = 0; j < 3; j++) {
        mtx->m[3][j] += pivot[0] * mtx->m[0][j] + pivot[1] * mtx->m[1][j] + pivot[2] * mtx->m[2][j];
    }
}

static void recenterWheels(VehicleData* data, const WheelDesc* wheels, u32 count) {
    u32 i;

    for (i = 0; i < count; i++) {
        recenterWheel(data, &wheels[i]);
    }
}

// called from LoadLevelVehicles and LoadLevelMissileCarrier after the vehicle data has been loaded
void patchVehicle(u32 vehicleId, VehicleData* data) {
    switch (vehicleId) {
        case VEHICLE_ID_SIDESWIPE:
            recenterWheels(data, sSideswipeWheels, sizeof(sSideswipeWheels) / sizeof(sSideswipeWheels[0]));
            break;
        case VEHICLE_ID_SKYFALL:
            recenterWheels(data, sSkyfallWheels, sizeof(sSkyfallWheels) / sizeof(sSkyfallWheels[0]));
            break;
        case VEHICLE_ID_TRUCK:
            recenterWheels(data, sTruckWheels, sizeof(sTruckWheels) / sizeof(sTruckWheels[0]));
            break;
        case VEHICLE_ID_BALLISTA:
            recenterWheels(data, sBallistaWheels, sizeof(sBallistaWheels) / sizeof(sBallistaWheels[0]));
            break;
        case VEHICLE_ID_MISSILE_CARRIER:
            recenterWheels(data, sMissileCarrierWheels, sizeof(sMissileCarrierWheels) / sizeof(sMissileCarrierWheels[0]));
            break;
        default:
            break;
    }
}
