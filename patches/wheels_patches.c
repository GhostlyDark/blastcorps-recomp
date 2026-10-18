#include "patches.h"

#define VEHICLE_ID_TRUCK 5

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
    u32 baseMtxOffset; // offset of pivot matrix, which puts the vertex to the center of the previous matrix
    u32 vtxOffset;
    u32 vtxCount;
} WheelDesc;

static const WheelDesc sTruckWheels[] = {
    { 0x080, 0x140, 41 }, // front left wheel, joint matrix 0x480
    { 0x180, 0x410, 42 }, // front right wheel, joint matrix 0x500
    { 0x200, 0x6B0, 92 }, // rear axle wheels (seem to be combined), joint matrix 0x540
};

static void recenterWheel(VehicleData* data, const WheelDesc* wheel) {
    Vtx* vtx = (Vtx*) ((u8*) data + data->vtxOffset + wheel->vtxOffset);
    VehicleMtxTable* mtxTable = (VehicleMtxTable*) ((u8*) data + data->mtxTableOffset);
    FixedMtx* mtx = &mtxTable->mtx[wheel->baseMtxOffset / sizeof(FixedMtx)];
    FixedMtx* invMtx = mtx + 1;
    s32 pivot[3];
    u32 i;
    u32 j;

    for (j = 0; j < 3; j++) {
        // extract translation from inverse matrix
        // the inverse matrix puts the wheel back into its place after rotation
        pivot[j] = invMtx->m[3][j] >> 16;
    }

    // Move every vertex of the wheel to the center
    for (i = 0; i < wheel->vtxCount; i++) {
        for (j = 0; j < 3; j++) {
            vtx[i].v.ob[j] -= pivot[j];
        }
    }

    for (j = 0; j < 3; j++) {
        mtx->m[3][j] += pivot[0] * mtx->m[0][j] + pivot[1] * mtx->m[1][j] + pivot[2] * mtx->m[2][j];
    }
}

static void patchTruckVehicle(VehicleData* data) {
    u32 i;

    for (i = 0; i < sizeof(sTruckWheels) / sizeof(sTruckWheels[0]); i++) {
        recenterWheel(data, &sTruckWheels[i]);
    }
}

// called from LoadLevelVehicles after the vehicle data has been loaded
void patchVehicle(u32 vehicleId, VehicleData* data) {
    switch (vehicleId) {
        case VEHICLE_ID_TRUCK:
            patchTruckVehicle(data);
            break;
        default:
            break;
    }
}
