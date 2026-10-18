#include "patches.h"
#include "misc_funcs.h"
#include "gfx_tagging.h"
#include "gfx.h"
#include "pillarbox_patches.h"
#include <PR/sched.h>
#include <PR/gbi.h>
#include <hd_code/macros.h>
#include <hd_code/functions.h>
#include <hd_code/variables.h>
#include <hd_code/yoshi.h>
#include <hd_code/music.h>
#include <hd_code/gfx.h>

Gfx sExtendedGfx[2][EXTENDED_DL_SIZE];

#define VIRTUAL_TO_PHYSICAL(addr) ((uintptr_t) (addr) & 0x1FFFFFFF)
extern void* D_hd_code_8035806C;
extern s16 D_hd_code_80364454; // camera pitch, 0..4095; proposed name: cameraPitch
extern s32 D_hd_code_80364458;
extern Gfx* D_hd_code_80358030[2];
extern Gfx* D_hd_code_80358038[2];
extern Gfx* D_hd_code_80358040[2];
extern Gfx* D_hd_code_80358048[2];
extern u8 D_hd_code_803643DC;
extern s32 D_hd_code_803649F4;
extern s32 D_hd_code_80364A44;
extern u8 D_hd_code_80364A48;
extern s16 D_hd_code_80364A4A;
extern s16 D_hd_code_80364A4C;
extern u8 D_hd_code_80364A4E;
extern u8 D_hd_code_80364A86;
extern u8 D_hd_code_80364AC1;
extern u8 D_hd_code_8035805C;

RECOMP_PATCH void func_hd_code_802559F8(Gfx* gfx, s32* length) {
    Gfx* entry = gfx;

    gDPFullSync(entry++);
    gSPEndDisplayList(entry++);

    // @recomp: calculate proper length
    s32 extlen = (s32) ABS(((s32) entry - (s32) &sExtendedGfx[D_hd_code_8035805C]));
    s32 intlen = (s32) ABS(((s32) entry - (s32) &D_hd_code_803156F8[D_hd_code_8035805C]) - 0x48B0);
    *length = MIN(extlen, intlen) >> 3;
    if (*length >= EXTENDED_DL_SIZE) {
        rmonPrintf(ASSERT_MESSAGE, "*length<EXTENDED_DL_SIZE", "patches.c", 0x123);
    }
}

#define HUD_FB_WIDTH 320
#define HUD_FB_HEIGHT 240

s32 gCfbWidth = HUD_FB_WIDTH;
s32 gCfbHeight = HUD_FB_HEIGHT;

Vp gHudViewport = {
    .vp = {
        .vscale = { (HUD_FB_WIDTH / 2) << 2, (HUD_FB_HEIGHT / 2) << 2, G_MAXZ, 0 },
        .vtrans = { (HUD_FB_WIDTH / 2) << 2, (HUD_FB_HEIGHT / 2) << 2, 0, 0 },
    }
};

Gfx* hud_begin(Gfx *entry) {
    gEXPushScissor(entry++);
    gEXSetScissor(entry++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0, gCfbHeight);

    gEXPushViewport(entry++);
    gEXViewport(entry++, G_EX_ORIGIN_LEFT, &gHudViewport);
    return entry;
}

Gfx* hud_align_right(Gfx* entry) {
    gEXSetViewportAlign(entry++, G_EX_ORIGIN_RIGHT, -(gCfbWidth << 2), 0);
    gSPViewport(entry++, &gHudViewport);
    return entry;
}

Gfx* hud_align_left(Gfx* entry) {
    gEXSetViewportAlign(entry++, G_EX_ORIGIN_LEFT, 0, 0);
    gSPViewport(entry++, &gHudViewport);
    return entry;
}

Gfx* hud_align_none(Gfx* entry) {
    gEXSetViewportAlign(entry++, G_EX_ORIGIN_NONE, 0, 0);
    gSPViewport(entry++, &gHudViewport);
    return entry;
}

Gfx* hud_end(Gfx* entry) {
    gEXSetViewportAlign(entry++, G_EX_ORIGIN_NONE, 0, 0);
    gEXPopViewport(entry++);
    gEXPopScissor(entry++);
    return entry;
}

// @recomp: Enable extended RT64 commands in hdGameFrame
RECOMP_PATCH Gfx* func_hd_code_8024C414(struct FrameContext* arg0, s32* arg1) {
    Gfx* entry;
    u8 sp194[16];
    u8 sp184[16];
    s32 sp180;

    // @recomp: use extended dl
    arg0->dp[0].words.w0 = (u32) 0xCAFEBABE;
    entry = &sExtendedGfx[D_hd_code_8035805C][0];

    gEXEnable(entry++);             // @recomp
    gEXSetRefreshRate(entry++, 30); // @recomp

    gSPSegment(entry++, 0x00, 0x00000000);
    gSPSegment(entry++, 0x02, osVirtualToPhysical(arg0));
    gSPSegment(entry++, 0x01, osVirtualToPhysical(D_hd_code_8035806C));
    gSPDisplayList(entry++, D_1000038);
    gSPDisplayList(entry++, D_1000010);
    gDPPipeSync(entry++);
    gDPSetCycleType(entry++, G_CYC_FILL);
    gSPClearGeometryMode(entry++, G_ZBUFFER);
    gDPSetDepthImage(entry++, (s32) D_hd_code_80358058);
    gDPSetColorImage(entry++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, (s32) D_hd_code_80358058);
    gDPSetFillColor(entry++, 0xFFFCFFFC);
    // @recomp: make rectangle fill whole frame
    gDPFillRectangle(entry++, 0, 0, 320, 240);

    guTranslate(&arg0->modelview, 0.f, 0.f, 0.f);
    // @recomp: make both orthos fill the whole frame
    guOrtho(&arg0->mtxOrtho, 0, 320.f, 240.f, 0.0f, -20000.0f, 20000.0f, 1.0f);
    guOrtho(&arg0->mtxOrthoLarge, 0, 1280.f, 960.f, 0.0f, -20000.0f, 20000.0f, 1.0f);
    func_hd_code_802507C8(&arg0->projection2, &arg0->lookAt, &arg0->unk180);

    gDPSetColorImage(entry++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, D_hd_code_80358050[D_hd_code_8035805C]);
    entry = func_hd_code_80271FD0(entry, arg0, g_currentLevel, D_hd_code_80364452, (s32) D_hd_code_80364454, &sp180);

    gDPPipeSync(entry++);
    gDPSetCycleType(entry++, G_CYC_FILL);

    switch (g_currentLevel) {
        case 0xD:
        case 0xE:
        case 0x10:
        case 0x34:
            gDPSetFillColor(entry++, 0xD55FD55F);
            break;
        case 0xF:
            gDPSetFillColor(entry++, 0x10511051);
            break;
        default:
            gDPSetFillColor(entry++, 0x00010001);
            break;
    }
    gDPPipeSync(entry++);

    // @recomp: make rectangle fill whole frame
    gDPFillRectangle(entry++, 0, (sp180 < 1 ? 0 : (sp180 - 1)), 320, 240);
    gDPPipeSync(entry++);

    gSPLookAt(entry++, &D_2000000.lookAt);
    gSPMatrix(entry++, &D_2000000.projection, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(entry++, &D_2000000.projection2, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    gSPMatrix(entry++, &D_2000000.modelview, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPPerspNormalize(entry++, D_hd_code_8035807C);

    switch (g_currentGameState) {
        case 0x40:
        case 0x400:
            guPerspective(&arg0->projection, &D_hd_code_8035807C, D_hd_code_80364438, ASPECT_RATIO, 10.0f, 20000.0f,
                          1.0f);
            break;
        case 0x01:
        case 0x800:
        case 0x1000:
            guPerspective(&arg0->projection, &D_hd_code_8035807C, D_hd_code_80364438, ASPECT_RATIO, 10.0f, 10000.0f,
                          1.0f);
            break;
        default:
            guPerspective(&arg0->projection, &D_hd_code_8035807C, D_hd_code_80364438, ASPECT_RATIO, 10.0f, 10000.0f,
                          1.0f);
            break;
    }

    gDPSetColorDither(entry++, G_CD_MAGICSQ);
    gSPClipRatio(entry++, FRUSTRATIO_3);
    func_hd_code_8027F1F8(&entry, D_hd_code_8035805C, 0);

    gSPSegment(entry++, 0x08, VIRTUAL_TO_PHYSICAL(D_hd_code_80364458));
    gSPClearGeometryMode(entry++, 0xFFFFFFFF);

    gSPDisplayList(entry++, osVirtualToPhysical(D_hd_code_80358030[D_hd_code_8035805C]));
    gSPDisplayList(entry++, (s32) D_hd_code_803BE6E0);
    gSPClearGeometryMode(entry++, 0xFFFFFFFF);
    gSPSetGeometryMode(entry++, G_ZBUFFER);
    gSPDisplayList(entry++, osVirtualToPhysical(D_hd_code_80358038[D_hd_code_8035805C]));
    gSPDisplayList(entry++, (s32) D_hd_code_803BE6E4);

    if ((D_hd_code_803643D6 == 0) && (D_hd_code_803643D7 == 0)) {
        gSPClearGeometryMode(entry++, 0xFFFFFFFF);
        gSPSetGeometryMode(entry++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LOD | G_SHADING_SMOOTH);

        gSPDisplayList(entry++, &D_2000000.unk21410);
        gDPPipeSync(entry++);
    }
    switch (D_hd_code_8035805C) { /* irregular */
        case 0:
            gSPSegment(entry++, 0x0A, osVirtualToPhysical(D_hd_code_803F7820));
            break;
        case 1:
            gSPSegment(entry++, 0x0A, osVirtualToPhysical(D_hd_code_803F7824));
            break;
    }
    gDPPipeSync(entry++);
    gSPSetGeometryMode(entry++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    gDPSetCycleType(entry++, G_CYC_1CYCLE);
    gDPSetCombineMode(entry++, G_CC_SHADE, G_CC_SHADE);
    gSPDisplayList(entry++, &D_2000000.unkA4E0);

    if (!(g_currentGameState & 0x440)) {
        func_hd_code_8027C4C8(&entry, arg0);
    }
    func_hd_code_802502EC();
    func_hd_code_8024E4F4(&entry, arg0, 0);
    func_hd_code_80258B78(&entry, arg0);
    if ((D_hd_code_802E8BD0 == 0) && (D_hd_code_80364AA8 != 0x40) && !(g_currentGameState & 0x440) &&
        (D_hd_code_80364A84 == 0)) {
        func_hd_code_80279EE8(&entry, arg0, D_hd_code_8035805C);
    }
    gSPClearGeometryMode(entry++, 0xFFFFFFFF);
    gSPSetGeometryMode(entry++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LOD | G_SHADING_SMOOTH);
    gSPDisplayList(entry++, &D_2000000.unkA580);

    gDPPipeSync(entry++);

    entry = func_hd_code_802CEEFC(entry, D_hd_code_8035805C, &arg0->unkA3A0, &arg0->unk11C0);
    func_hd_code_8024E4F4(&entry, arg0, 1);
    if (D_hd_code_803643DC != 0) {
        func_hd_code_8024F520(&entry, arg0);
    }
    func_hd_code_802701A8(&entry, arg0);
    func_hd_code_80281E44(&entry);
    func_hd_code_8028E9E4(&entry, arg0);
    func_hd_code_802917B0(&entry, arg0);
    func_hd_code_80292EB8(&entry, arg0);
    func_hd_code_8028CB30(&entry, arg0);
    switch (D_hd_code_8035805C) { /* switch 1; irregular */
        case 0:                   /* switch 1 */
            gSPDisplayList(entry++, osVirtualToPhysical(&D_hd_code_803C5770));
            break;
        case 1: /* switch 1 */
            gSPDisplayList(entry++, osVirtualToPhysical(&D_hd_code_803C6370));
            break;
    }
    func_hd_code_80288DF0(&entry, D_hd_code_8035805C);
    if (!(g_currentGameState & 2) || D_hd_code_802E8BF0 == 0) {
        func_hd_code_8024FC2C(&entry, 0);
    }
    func_hd_code_802976E8(&entry);

    gSPClearGeometryMode(entry++, 0xFFFFFFFF);
    gSPSetGeometryMode(entry++, G_ZBUFFER);
    gSPDisplayList(entry++, osVirtualToPhysical(D_hd_code_80358040[D_hd_code_8035805C]));
    gSPDisplayList(entry++, (s32) D_hd_code_803BE6E8);
    if (!(g_currentGameState & 2) || D_hd_code_802E8BF0 == 0) {
        func_hd_code_8024FC2C(&entry, 1);
    }

    gSPClearGeometryMode(entry++, 0xFFFFFFFF);
    gSPSetGeometryMode(entry++, G_ZBUFFER);
    gSPDisplayList(entry++, osVirtualToPhysical(D_hd_code_80358048[D_hd_code_8035805C]));
    gSPDisplayList(entry++, (s32) D_hd_code_803BE6EC);

    gSPClearGeometryMode(entry++, 0xFFFFFFFF);
    gSPSetGeometryMode(entry++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LOD | G_SHADING_SMOOTH);
    gSPDisplayList(entry++, &D_2000000.unkA918);
    gDPPipeSync(entry++);

    if ((D_hd_code_803643D6 == 0) && (D_hd_code_803643D7 == 0)) {
        gSPClearGeometryMode(entry++, 0xFFFFFFFF);
        gSPSetGeometryMode(entry++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LOD | G_SHADING_SMOOTH);
        gSPDisplayList(entry++, &D_2000000.unk21478);
        gDPPipeSync(entry++);
    }
    if ((!(g_currentGameState & 2)) || (D_hd_code_802E8BF0 == 0)) {
        func_hd_code_8024FC2C(&entry, 2);
    }
    func_hd_code_80295120(&entry, arg0);
    if ((g_currentGameState & 0x2000001000003905)) {
        func_hd_code_80280F34(&entry, D_hd_code_8035805C);
    }
    func_hd_code_8027F1F8(&entry, D_hd_code_8035805C, 1);
    func_hd_code_8024E4F4(&entry, arg0, 2);
    if (D_hd_code_803643DC != 0) {
        func_hd_code_80266248(&entry, arg0);
    }
    switch (D_hd_code_8035805C) { /* switch 2; irregular */
        case 0:                   /* switch 2 */
            gSPDisplayList(entry++, osVirtualToPhysical(&D_hd_code_803C6F70));
            break;
        case 1: /* switch 2 */
            gSPDisplayList(entry++, osVirtualToPhysical(&D_hd_code_803C7B70));
            break;
    }
    if (D_hd_code_803643DB != 0) {
        func_hd_code_8028273C(&entry, D_hd_code_8035805C);
    }

    // @recomp: fix viewport to take aspect ratio into account
    entry = hud_begin(entry);

    if ((D_hd_code_80364A68 != 0) && (g_currentGameState & 0x104)) {
        func_hd_code_80286C60(&entry, arg0, D_hd_code_8035805C, D_hd_code_80364456);
    }
    if ((g_currentGameState & 0x200000000400220C)) {
        // @recomp: align comms to the right if it is rendered on the right
        if (D_hd_code_802FBD50[0].v.ob[0] > 0x50) {
            // comms is on the right side
            entry = hud_align_right(entry);
            func_hd_code_80278324(&entry, arg0, D_hd_code_8035805C);
            entry = hud_align_left(entry);
        } else {
            func_hd_code_80278324(&entry, arg0, D_hd_code_8035805C);
        }
    }
    if ((g_currentGameState == 0x100) && (D_hd_code_803643DB != 0)) {
        func_hd_code_80276E50(&entry, arg0, D_hd_code_8035805C, D_hd_code_803643E0, D_hd_code_803643E4,
                              D_hd_code_803643E8);
    }
    func_hd_code_80259450();
    if ((D_hd_code_80364A6A != 0) && ((g_currentGameState & 0x104))) {
        func_hd_code_80287530(&entry, arg0, D_hd_code_8035805C, D_hd_code_80364456);
    }
    if ((D_hd_code_80364A6C != 0) && ((g_currentGameState & 0x104))) {
        func_hd_code_80287C68(&entry, arg0, D_hd_code_8035805C, D_hd_code_80364456);
    }
    if ((g_currentGameState & 0x104)) {
        func_hd_code_80282224(&entry, D_hd_code_80364456);
    }
    if ((D_hd_code_80364AA8 & 1) && ((g_currentGameState & 0x0400030C))) {
        func_hd_code_8026A378(D_hd_code_803649F4, &sp194[1]);
        sp194[0] = 0x24;
        drawTextSimple(arg0, &sp194, NULL, 1, 0, 0x118, 0x12, 0x14, 0x14, 0, 0xFF, 0xFF, 0xFF,
                              (s32) D_hd_code_80367BD0.unk6);
        if ((D_hd_code_80364A44 != 0) && (D_hd_code_802E8BD0 == 0)) {
            func_hd_code_8026A378(D_hd_code_80364A44, &sp184[1]);
            sp184[0] = 0x24;
            drawTextSimple(arg0, &sp184, NULL, 1, 0, (s32) D_hd_code_80364A4A, (s32) D_hd_code_80364A4C, 0x23,
                                  0x23, (s32) D_hd_code_80364A4E, 0xFF, 0xFF, 0xFF, D_hd_code_80364A48);
        }
    }
    if ((g_currentGameState & 0x2000000000000104) &&
        (!(D_hd_code_80364AA8 & 0x81) || (D_hd_code_803643DB != 0) || (D_hd_code_80364AC1 != 0))) {
        entry = hud_align_none(entry);
        func_hd_code_80275478(arg0, &entry,
                              g_currentGameState & 0x100 || D_hd_code_8036BB18 == 0x4D || D_hd_code_8036BB18 == 0x49);
        entry = hud_align_left(entry);
    }
    if ((g_nextGameState == 0) && (areWeFading() == 0)) {
        if (!(g_currentGameState & 0x200000100400230C) &&
            ((u32) ((u32) (((u32) sc.retraceCount % 50U) * 0x3C) / 60U) >= 0x15U) && (D_hd_code_8036BB1C == 1) &&
            ((!(g_currentGameState & 2)) ||
             ((D_hd_code_802E8BEC != 0) && ((D_hd_code_80366A12 == 3) || (D_hd_code_802E8BEC == 1)))) &&
            ((g_currentGameState != 0x100000000000) || (D_hd_code_803A6B04 != 0)) &&
            ((!(g_currentGameState & 0x1801)) || (players[playerNumber].unk91 != 0)) && (g_currentLevel != 0x2F)) {
            drawTextSimple(arg0, "PRESS START", NULL, 1, 0, 0x5C, 0xC4, 0x1A, 0x1A, 1, 0xFF, 0xFF, 0xFF, 0xFF);
        }
        if ((u32) ((u32) (((u32) sc.retraceCount % 40U) * 60) / 60U) >= 16U) {
            if (D_hd_code_802E8BD0 != 0) {
                if ((g_currentGameState == 0x2000000000000000) && (D_hd_code_8036BB1C == 2)) {
                    drawTextSimple(arg0, "USE Z/R TO TURN PAGES", D_hd_code_8030491C, 0, 0, 0x18, 0x14, 0xF, 0xF,
                                          1, 0xFF, 0xFF, 0xFF, 0xFF);
                } else if ((g_currentGameState == 0x100) && (D_hd_code_8036BB18 == 0) && (D_hd_code_8036BB1C == 2) &&
                           (D_hd_code_803643DB != 0) && !(g_currentButtons & (Z_TRIG | R_TRIG))) {
                    drawTextSimple(arg0, "USE Z/R TO MOVE MAP", D_hd_code_80304938, 0, 0, 0x18, 0x14, 0xF, 0xF,
                                          1, 0xFF, 0xFF, 0xFF, 0xFF);
                }
            } else if ((g_currentGameState == 0x100)) {
                if (D_hd_code_80364AC1 != 0) {
                    drawTextSimple(arg0, "SHUTTLE VIEW", D_hd_code_80304904, 0, 0, 0x18, 0x14, 0xF, 0xF, 1, 0xFF,
                                          0xFF, 0xFF, 0xFF);
                } else {
                    drawTextSimple(arg0, "MISSILE VIEW", D_hd_code_80304910, 0, 0, 0x18, 0x14, 0xF, 0xF, 1, 0xFF,
                                          0xFF, 0xFF, 0xFF);
                }
            }
        }
    }
    if (((g_currentGameState & 0x440) || (g_currentLevel == 0x26)) && (D_hd_code_8036BB1C == 1) &&
        (areWeFading() == 0)) {
        if (((g_currentGameState & 0x440)) && (D_hd_code_80364AA8 != 1)) {
            func_hd_code_80274B40(&entry, arg0, D_hd_code_80365580, 0x108, 0x12);
        } else {
            func_hd_code_80274B40(&entry, arg0, D_hd_code_80365580, 0x18, 0x12);
        }
    }
    if (((g_currentGameState & 0x104)) && (D_hd_code_80364410 != 0)) {
        func_hd_code_80274B40(&entry, arg0, D_hd_code_80364A86, 0x108, 0xBE);
    }
    if (((g_currentGameState & 0x104)) && (D_hd_code_80364AA8 == 1) && (D_hd_code_802E8BD0 == 0)) {
        func_hd_code_80285CC0();
    }
    if ((D_hd_code_803643DB != 0) && (g_currentGameState == 4)) {
        func_hd_code_80282C80(&entry, arg0, D_hd_code_803643E0, D_hd_code_803643E4, D_hd_code_803643E8,
                              D_hd_code_803EF6DC, D_hd_code_803EF6E0, D_hd_code_803EF6E4);
    }
    if ((D_hd_code_802E8F94[g_currentLevel].unk0 & 0x81) && (g_currentGameState == 4)) {
        if (g_currentLevel != 0x32 ||
            ((players[playerNumber].unk18[0x32] > 0 && players[playerNumber].unk18[0x32] < 6) ? 1 : 0)) {
            func_hd_code_8028376C(&entry, arg0, D_hd_code_8035805C, D_hd_code_803643E0, D_hd_code_803643E8,
                                  D_hd_code_803EF6DC, D_hd_code_803EF6E4);
        }
    }
    if ((D_hd_code_803643DB != 0) || (D_hd_code_80364AC1 != 0)) {
        switch (g_currentGameState) {
            case 0x4:
            case 0x100000000000:
            case 0x100:
            case 0x200:
                entry = hud_align_none(entry);
                func_hd_code_8025E2CC(&entry, arg0, D_hd_code_8035805C);
                func_hd_code_8025E67C(&entry, arg0, D_hd_code_8035805C);
                entry = hud_align_left(entry);
            case 0x400:
            case 0x40:
                if (((D_hd_code_803643D6 != 0) || (D_hd_code_803643D7 != 0) || (D_hd_code_803643D9 != 0) ||
                     (D_hd_code_803643DA != 0)) &&
                    ((g_currentGameState & 0x144))) {
                    func_hd_code_802A45D4(0x32);
                    if (g_currentGameState == 0x40) {
                        g_nextGameState = 1024;
                    } else {
                        g_nextGameState = 512;
                    }
                }
                break;
        }
    }
    drawTextOrtho(&entry, arg0);
    // @recomp: end hud drawing
    entry = hud_end(entry);

    // @recomp: use extended dl
    *arg1 = (s32) (((s32) entry - (s32) sExtendedGfx[D_hd_code_8035805C])) >> 3;
    return entry;
}

extern u8 D_hd_code_80364AC1;
extern u8 D_hd_code_803649EC;
extern u8 D_hd_code_803156F5;
// @recomp: Tag Vehicle transforms like wheels. Prevents wobbly wheels
RECOMP_PATCH void func_hd_code_8024FC2C(Gfx** arg0, u8 arg1) {
    Gfx* entry;
    s32 sp60;
    u8 pad;
    u8 sp5E;
    u8 sp5D;

    entry = *arg0;

    sp5D = D_hd_code_803649E8 == 0 && (D_hd_code_803649EC != 0 || g_currentGameState & 0x1801);
    sp60 = 0;
    while (&D_hd_code_80364460[sp60] != D_hd_code_803649D0) {
        if (((D_hd_code_80364460[sp60].unk5C != 0) || (sp5D != 0)) &&
            ((D_hd_code_80364460[sp60].unk5C != 0xFF) || (D_hd_code_803EF6FF == 0)) &&
            ((D_hd_code_80364460[sp60].unk5C == 0xFD) || (D_hd_code_80364A84 == 0) || (D_hd_code_80364AC1 == 0))) {

            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(entry++, TAG_VEHICLE(sp60, arg1), G_EX_PUSH, G_MTX_MODELVIEW,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_SKIP, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_SKIP);
            // @recomp: move resetting the modelview matrix here
            gSPMatrix(entry++, &D_2000000.modelview, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);


            gSPSegment(entry++, 0x06, osVirtualToPhysical((void*) D_hd_code_80364460[sp60].unk0));
            if (((g_currentGameState & 0x1801)) &&
                (((D_hd_code_80364460[sp60].unk5C == 0xFE)) || (D_hd_code_80364460[sp60].unk5C == 0))) {
                sp5E = D_hd_code_8035805C;
            } else {
                sp5E = D_hd_code_803156F5;
            }
            if (sp5E != 0) {
                gSPSegment(entry++, 0x07, osVirtualToPhysical((void*) D_hd_code_80364460[sp60].unk4));
            } else {
                gSPSegment(entry++, 0x07, osVirtualToPhysical((void*) D_hd_code_80364460[sp60].unk8));
            }
            gDPPipeSync(entry++);
            gDPSetEnvColor(entry++, 0x00, 0x00, 0x00, D_hd_code_80364460[sp60].unk60);
            gSPClearGeometryMode(entry++, 0xFFFFFFFF);
            if ((D_hd_code_803643D6 != 0) && !(D_hd_code_80364AA8 & 0x81) &&
                (D_hd_code_80364460[sp60].unk5C == D_hd_code_80364456)) {
                gSPMatrix(entry++, &D_2000000.unk1500, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            }
            if (sp5E != 0) {
                switch (arg1) {
                    case 0:
                        gSPDisplayList(entry++, osVirtualToPhysical((void*) D_hd_code_80364460[sp60].unkC));
                        break;
                    case 1:
                        gSPDisplayList(entry++, osVirtualToPhysical((void*) D_hd_code_80364460[sp60].unk10));
                        break;
                    case 2:
                        gSPDisplayList(entry++, osVirtualToPhysical((void*) D_hd_code_80364460[sp60].unk14));
                        break;
                }
            } else {
                switch (arg1) { /* irregular */
                    case 0:
                        gSPDisplayList(entry++, osVirtualToPhysical((void*) D_hd_code_80364460[sp60].unk30));
                        break;
                    case 1:
                        gSPDisplayList(entry++, osVirtualToPhysical((void*) D_hd_code_80364460[sp60].unk34));
                        break;
                    case 2:
                        gSPDisplayList(entry++, osVirtualToPhysical((void*) D_hd_code_80364460[sp60].unk38));
                        break;
                }
            }
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);

        }
        sp60++;
    }
    *arg0 = entry;
}

#define qu016(n) ((u16) ((n) * 0x10000))
extern s32 D_hd_code_803F7660;
extern u16 D_hd_code_802FCEB0[32 * 32];
extern Vtx D_hd_code_802FD9B8[10];


// @recomp: missile carrier arrow. wobbles in-game. This is fixed with tagging
RECOMP_PATCH void func_hd_code_80282C80(Gfx** gfx, struct FrameContext* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    Gfx* entry = *gfx;
    f32 sp130;
    f32 sp12C;
    u8 sp12B;
    u8 sp12A;
    s16 sp128;
    f32 spE8[4][4];
    f32 spA8[4][4];
    s32 spA4;
    s32 spA0;
    u8 sp9F;

    arg2 >>= 5;
    arg3 >>= 5;
    arg4 >>= 5;
    arg5 >>= 5;
    arg6 >>= 5;
    arg7 >>= 5;

    spA4 = D_hd_code_803F7670 >> 5;
    spA0 = D_hd_code_803F7678 >> 5;
    sp130 = sqrtf((((spA4 - arg2) * (spA4 - arg2)) + ((spA0 - arg4) * (spA0 - arg4))));
    if (sp130 < 1.0) {
        sp130 = 1.0f;
    }
    if ((spA4 >= arg2) && (spA0 >= arg4)) {
        sp12C = (func_hd_code_802AD7D4((s32) (((spA4 - arg2) / sp130) * 65536.0)) >> 4);
    }
    if ((spA4 >= arg2) && (spA0 < arg4)) {
        sp12C = ((func_hd_code_802AD7D4((s32) (((arg4 - spA0) / sp130) * 65536.0)) >> 4) + 0x400);
    }
    if ((spA4 < arg2) && (spA0 < arg4)) {
        sp12C = ((func_hd_code_802AD7D4((s32) (((arg2 - spA4) / sp130) * 65536.0)) >> 4) + 0x800);
    }
    if ((spA4 < arg2) && (spA0 >= arg4)) {
        sp12C = ((func_hd_code_802AD7D4((s32) (((spA0 - arg4) / sp130) * 65536.0)) >> 4) + 0xC00);
    }
    sp12C = (sp12C * 0.08791208791208792);
    sp12C = ((360.0 - sp12C) - 45.0);
    sp12C = (sp12C + (((D_hd_code_80364452 * 360.0) / 4095.0) - 135.0));
    sp130 = sqrtf((((arg5 - spA4) * (arg5 - spA4)) + ((arg7 - spA0) * (arg7 - spA0))));
    if (sp130 > 1500.0f) {
        sp12A = 0xFF;
        sp12B = 0;
    } else if (sp130 < 500.0f) {
        sp12B = 0xFF;
        sp12A = 0;
    } else {
        sp128 = (s16) (s32) (((sp130 - 500.0f) / 1000.0f) * 511.0f);
        if ((s16) sp128 < 0x100) {
            sp12A = sp128, sp12B = 0xFF;
        } else {
            sp12A = 0xFF, sp12B = 0x1FE - sp128;
        }
    }
    if (sp130 < 250.0f) {
        sp9F = 1;
    } else {
        sp9F = 0;
    }

    if ((((D_hd_code_803156C4 % 30U) >= 0x10U) || (sp9F == 0)) && (D_hd_code_803F7660 != 0x98967F)) {
        guAlignF(spE8, 20.0f, 1.0f, 0.0f, 0.0f);
        guAlignF(spA8, -sp12C, 0.0f, 0.0f, 1.0f);
        guMtxCatF(spE8, spA8, spE8);
        guTranslateF(spA8, -150.0f, -230.0f, -800.0f);
        guMtxCatF(spE8, spA8, spE8);
        guMtxF2L(spE8, (Mtx*) arg1->unk1580);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(entry++, TAG_MISSILE_ARROW, G_EX_PUSH, G_MTX_PROJECTION, G_EX_EDIT_NONE);

        gSPMatrix(entry++, (u32) &D_2000000.projection, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        gSPPerspNormalize(entry++, D_hd_code_8035807C);

        gSPMatrix(entry++, D_2000000.unk1580, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPClearGeometryMode(entry++, G_ZBUFFER | G_TEXTURE_ENABLE | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING |
                                          G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH | 0xFFE0CDF8);
        gSPSetGeometryMode(entry++, G_SHADE | G_CULL_FRONT | G_LIGHTING | G_TEXTURE_GEN | G_SHADING_SMOOTH);
        gDPPipeSync(entry++);
        gDPSetCycleType(entry++, G_CYC_1CYCLE);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(entry++, TAG_MISSILE_ARROW + 1, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);

        if (D_hd_code_80367BD6 == 0xFF) {
            gDPSetRenderMode(entry++, G_RM_RA_OPA_SURF, G_RM_RA_OPA_SURF2);
        } else {
            gDPSetRenderMode(entry++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
        }
        gDPSetCombineMode(entry++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

        gDPSetPrimColor(entry++, 0, 0, sp12B, sp12A, 0, D_hd_code_80367BD6);
        gSPTexture(entry++, qu016(0.03028), qu016(0.03028), 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(entry++, OS_PHYSICAL_TO_K0(&D_hd_code_802FCEB0), G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(entry++, osVirtualToPhysical(D_hd_code_802FD9B8), 10, 0);
        gSP1Triangle(entry++, 0, 1, 2, 0);
        gSP1Triangle(entry++, 0, 2, 3, 0);
        gSP1Triangle(entry++, 3, 1, 0, 0);
        gSP1Triangle(entry++, 1, 4, 2, 0);
        gSP1Triangle(entry++, 4, 5, 2, 0);
        gSP1Triangle(entry++, 5, 3, 2, 0);
        gSP1Triangle(entry++, 6, 7, 8, 0);
        gSP1Triangle(entry++, 9, 6, 8, 0);
        gSP1Triangle(entry++, 7, 9, 8, 0);
        gDPPipeSync(entry++);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(entry++, G_MTX_PROJECTION);
    }
    *gfx = entry;
}

// @recomp: white-out fade is not full-screen. fixed this by tagging this
RECOMP_PATCH void func_hd_code_8025E67C(Gfx** arg0, struct FrameContext* arg1, u8 arg2) {
    Gfx* entry;
    u32 sp60;
    u32 sp5C;
    u32 sp58;
    u32 pad54;
    u32 sp50;

    entry = *arg0;
    sp60 = sc.retraceCount;
    if (D_hd_code_803643D6 != 0) {
        if (D_hd_code_803643D8 == 0) {
            sndDeactivateAllSfxByFlag_1();
            func_hd_code_802C1DD0(D_hd_code_802E8F94[g_currentLevel].unk0 == 0x20 ||
                                  D_hd_code_802E8F94[g_currentLevel].unk0 == 0x80);
            switch (g_currentLevel) { /* switch 1; irregular */
                case 49:              /* switch 1 */
                    sndPlaySfx(D_hd_code_80367738, 0x31U, NULL);
                    func_hd_code_80261570(0.0f);
                    break;
                case 50: /* switch 1 */
                    D_hd_code_8036BB1A = -1;
                    yoshiShow(0xA00EU);
                    func_hd_code_80261570(0.0f);
                    break;
                default: /* switch 1 */
                    sndPlaySfx(D_hd_code_80367738, 0x31U, NULL);
                    D_hd_code_802E8BD8 = 1;
                    if ((D_hd_code_8036BB18 != -1) || (func_hd_code_8026B10C() != 0)) {
                        yoshiShow(0x4000U);
                    }
                    D_hd_code_8036BB1A = -1;
                    func_hd_code_80261570(0.0f);
                    break;
            }
            D_hd_code_80366BB8 = sp60;
            D_hd_code_80366BC4.unk1 = 0U;
        }
        sp5C = sp60 - D_hd_code_80366BB8;
        if (sp5C >= 0xB4U) {
            switch (g_currentLevel) { /* switch 2; irregular */
                case 49:              /* switch 2 */
                    if (D_hd_code_80366BC4.unk1 == 0) {
                        sndPlaySfx(D_hd_code_80367738, 0x32U, NULL);
                        D_hd_code_80366BC4.unk1 = 1U;
                    }
                    break;
                case 50: /* switch 2 */
                    if ((D_hd_code_8036BB1C == 1) && (areWeFading() == 0)) {
                        if ((((s32) players[playerNumber].unk18[g_currentLevel] > 0) &&
                                     ((s32) players[playerNumber].unk18[g_currentLevel] < 6)
                                 ? 1
                                 : 0) != 0) {
                            fadeToFast(0x08000000);
                        } else {
                            fadeToFast(0x40);
                        }
                    }
                    break;
                default: /* switch 2 */
                    if (D_hd_code_80366BC4.unk1 == 0) {
                        sndPlaySfx(D_hd_code_80367738, 0x32U, NULL);
                        D_hd_code_80366BC4.unk1 = 1U;
                    }

                    gEXMatrixGroupSkipAllAspect(entry++, TAG_FADE_SCREEN, G_EX_PUSH, G_MTX_PROJECTION, G_EX_EDIT_NONE,
                                                G_EX_ASPECT_STRETCH); // @recomp: force stretching
                    
                    gSPMatrix(entry++, &D_2000000.mtxOrtho, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
                    gSPMatrix(entry++, &D_2000000.modelview, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gDPPipeSync(entry++);
                    gDPSetRenderMode(entry++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
                    gDPSetCombineMode(entry++, G_CC_SHADE, G_CC_SHADE);
                    gSPSetGeometryMode(entry++, G_SHADE | G_SHADING_SMOOTH);
                    gSPTexture(entry++, qu016(0.999985), qu016(0.999985), 0, G_TX_RENDERTILE, G_OFF);
                    gSPVertex(entry++, OS_PHYSICAL_TO_K0(&D_hd_code_802FA8B0[arg2]), 4, 0);
                    gSP1Triangle(entry++, 0, 1, 2, 0);
                    gSP1Triangle(entry++, 0, 2, 3, 0);
                    gDPPipeSync(entry++);
                    gEXPopMatrixGroup(entry++, G_MTX_PROJECTION); // @recomp: reset stretching


                    if (!areWeFading()) {
                        if ((sp60 - D_hd_code_80366BB8) - 0xB4 < 0x5AU) {
                            sp50 = (u32) (((sp60 - D_hd_code_80366BB8) - 0xB4) * 2.8333333333333335);
                            for (sp5C = 0; sp5C < 4U; sp5C++) {
                                for (sp58 = 0; sp58 < 4U; sp58++) {
                                    D_hd_code_802FA8B0[arg2].v[sp5C].v.cn[sp58] = sp50;
                                }
                            }
                        } else if ((u32) ((sp60 - D_hd_code_80366BB8) - 0x10E) >= 0x2EU) {
                            if ((g_currentGameState == 0x100000000000)) {
                                g_nextGameState = 0x200000000000;
                            } else {
                                if ((((s32) players[playerNumber].unk18[g_currentLevel] > 0) &&
                                     ((s32) players[playerNumber].unk18[g_currentLevel] < 6))
                                        ? 1
                                        : 0 != 0) {
                                    fadeToFast(0x08000000);
                                } else {
                                    fadeToFast(0x40);
                                }
                            }
                        } else {
                            for (sp5C = 0; sp5C < 4U; sp5C++) {
                                for (sp58 = 0; sp58 < 4U; sp58++) {
                                    D_hd_code_802FA8B0[arg2].v[sp5C].v.cn[sp58] = 0xFF;
                                }
                            }
                        }
                    }
                    break;
            }
        }
    }
    *arg0 = entry;
}

extern u16 D_hd_code_802FA8A0[2];
extern f32 D_hd_code_8036BFC0;
extern u8 D_hd_code_8036BFC4;
extern u8 D_hd_code_8036BFC5;
extern f32 D_hd_code_8036BFC8;
extern f32 D_hd_code_8036BFCC;
extern f32 D_hd_code_8036BFD0;
extern Vtx D_hd_code_802FA820[2][4];
extern struct S_802FA280 D_hd_code_802FA280[60][2];
#define qu102(n) ((u16) ((n) * 0x0004))
// @recomp: The sky-quad (yes no skybox) is tied to 4:3 ratio. This code fixes it for all aspect ratios
RECOMP_PATCH Gfx* func_hd_code_80271FD0(Gfx* arg0, struct FrameContext* arg1, u16 arg2, s16 arg3, s16 arg4, s32* arg5) {
    Gfx* entry = arg0;
    INIT_FROM_ARRAY(u16 sp78[2], sp78, D_hd_code_802FA8A0);
    s32 sp74;
    struct S_802FA280* sp70;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    f32 sp60;
    f32 sp5C;

    // @recomp: get aspect ratio
    f32 ratio = recomp_get_target_aspect_ratio(4.0f / 3.0f);
    f32 offset = ((ratio * 960.0f) - 1280.0f) / 1280.0f;

    if (D_hd_code_8036BFC4 == 0) {
        *arg5 = 0;
        return entry;
    }
    sp6C = (120.0 - ((f32) arg4 * 0.22)) + D_hd_code_8036BFC0;

    D_hd_code_8036BFCC = MAX(0.0, sp6C);
    D_hd_code_8036BFD0 = MAX(0.0, -sp6C);

    *arg5 = D_hd_code_8036BFCC;
    if (*arg5 <= 0) {
        return entry;
    }
    D_hd_code_8036BFC8 = ((arg3 - 2048.0) * 0.5);

    D_hd_code_802FA820[D_hd_code_8036BFC5][2].v.ob[1] = (D_hd_code_8036BFCC * 4.0) - 1.0;
    D_hd_code_802FA820[D_hd_code_8036BFC5][3].v.ob[1] = (D_hd_code_8036BFCC * 4.0) - 1.0;

    // @recomp: calculate new skybox sizes
    D_hd_code_802FA820[D_hd_code_8036BFC5][0].v.ob[0] = -offset * 1280;
    D_hd_code_802FA820[D_hd_code_8036BFC5][1].v.ob[0] = 1280 + offset * 1280;
    D_hd_code_802FA820[D_hd_code_8036BFC5][2].v.ob[0] = -offset * 1280.0f;
    D_hd_code_802FA820[D_hd_code_8036BFC5][3].v.ob[0] = 1280 + offset * 1280;


    gSPClearGeometryMode(entry++, -1);
    gSPSetGeometryMode(entry++, G_SHADE | G_SHADING_SMOOTH);
    gSPTexture(entry++, qu016(0.5), qu016(0.5), 0, G_TX_RENDERTILE, G_ON);
    gDPPipeSync(entry++);
    gDPSetCombineLERP(entry++, TEXEL1, TEXEL0, TEXEL1_ALPHA, TEXEL0, TEXEL1, TEXEL0, TEXEL0, TEXEL0, 0, 0, 0, COMBINED,
                      0, 0, 0, COMBINED);
    gDPSetCycleType(entry++, G_CYC_2CYCLE);
    gDPSetRenderMode(entry++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetTextureFilter(entry++, G_TF_BILERP);

    for (sp74 = 0; sp74 < 2; sp74++) {
        sp70 = &D_hd_code_802FA280[arg2][sp74];

        gDPSetTextureImage(entry++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, sp70->unk4);
        gDPTileSync(entry++);
        gDPSetTile(entry++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, sp74 << 8, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
                   G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPLoadSync(entry++);
        gDPLoadBlock(entry++, G_TX_LOADTILE, 0, 0, 1023, 256);
    }

    gSPMatrix(entry++, OS_PHYSICAL_TO_K0(&arg1->mtxOrthoLarge), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(entry++, OS_PHYSICAL_TO_K0(&arg1->modelview), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPTileSync(entry++);
    gDPSetTextureLOD(entry++, G_TL_TILE);

    for (sp74 = 0; sp74 < 2; sp74++) {
        sp70 = &D_hd_code_802FA280[arg2][sp74];

        gDPSetTile(entry++, sp78[sp74], G_IM_SIZ_16b, 8, sp74 << 8, sp74, 0, (sp70->unkB & 0x3), 5, sp74,
                   (sp70->unkA & 0x3), 5, sp74);
        gDPSetTileSize(entry++, sp74, 0, 0, qu102(31), qu102(31));
    }

    sp68 = (D_hd_code_8036BFC8 + 16.0f) / (f32) (1 << sp70->unk8);
    sp60 = (f32) (320.0 / (f64) (f32) (1 << sp70->unk8));

    // @recomp Tag the transform.
    // Interpolating texture coordinates
    gEXMatrixGroup(entry++, TAG_SKY_QUAD, G_EX_INTERPOLATE_SIMPLE, G_EX_PUSH, G_MTX_MODELVIEW,
                   G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                   G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                   G_EX_COMPONENT_SKIP, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW, G_EX_ASPECT_STRETCH,
                   G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP);

    // @recomp: adapt texture size in width
    D_hd_code_802FA820[D_hd_code_8036BFC5][0].v.tc[0] = (sp68 - offset * 320) * 32.0;
    D_hd_code_802FA820[D_hd_code_8036BFC5][1].v.tc[0] = (sp68 + sp60 + offset * 320) * 32.0;
    D_hd_code_802FA820[D_hd_code_8036BFC5][2].v.tc[0] = (sp68 - offset * 320) * 32.0;
    D_hd_code_802FA820[D_hd_code_8036BFC5][3].v.tc[0] = (sp68 + sp60 + offset * 320) * 32.0;

    sp64 = ((D_hd_code_8036BFCC - D_hd_code_8036BFC0) - D_hd_code_8036BFD0) / (f32) (1 << sp70->unk9);
    sp5C = (f32) (((f64) D_hd_code_8036BFCC - 1.0) / (f64) (f32) (1 << sp70->unk9));
    D_hd_code_802FA820[D_hd_code_8036BFC5][0].v.tc[1] = sp64 * 32.0;
    D_hd_code_802FA820[D_hd_code_8036BFC5][1].v.tc[1] = sp64 * 32.0;
    D_hd_code_802FA820[D_hd_code_8036BFC5][2].v.tc[1] = (sp64 - sp5C) * 32.0;
    D_hd_code_802FA820[D_hd_code_8036BFC5][3].v.tc[1] = (sp64 - sp5C) * 32.0;

    gSPVertex(entry++, D_hd_code_802FA820[D_hd_code_8036BFC5], 4, 0);
    gDPPipeSync(entry++);
    gSP1Triangle(entry++, 0, 1, 2, 0);
    gSP1Triangle(entry++, 1, 2, 3, 0);

    D_hd_code_8036BFC5 ^= 1;

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);

    return entry;
}

extern u8 D_hd_code_8036C784;
extern s32 D_hd_code_8036C780;
extern u16 D_hd_code_8036C770;
extern u64 D_hd_code_8036C778;
extern f32 D_hd_code_8036C774;
extern f32 D_hd_code_802FA930;

RECOMP_PATCH Gfx* func_hd_code_80274BF0(void* arg0, Gfx* gfx) {
    Gfx* entry;

    entry = gfx;
    if (g_frameCount == 0) {
        if ((g_currentGameState & 0x4055800100040000) || (((g_currentGameState & 0x1801)) && (g_currentLevel == 0x32))) {
            D_hd_code_8036C784 = 0xFF;
            if ((g_currentGameState & 0x51800100040000)) {
                D_hd_code_8036C770 = func_hd_code_8026B10C();
                yoshiShow(0U);
            }
            D_hd_code_8036C780 = D_hd_code_803156C4;
        } else {
            D_hd_code_8036C784 = 0;
        }
    }
    if ((D_hd_code_8036C778 != 0)) {
        D_hd_code_8036C784 = MIN(255.0f, ((D_hd_code_803156C4 - D_hd_code_8036C780) * D_hd_code_8036C774));
        if (D_hd_code_8036C784 == 0xFF) {
            g_nextGameState = D_hd_code_8036C778;
            D_hd_code_8036C778 = 0;
        }
    } else if (D_hd_code_8036C784 != 0) {
        D_hd_code_8036C784 = MAX(0.0f, 255.0f - ((D_hd_code_803156C4 - D_hd_code_8036C780) * D_hd_code_802FA930));
        if (D_hd_code_8036C784 == 0) {
            yoshiShow(D_hd_code_8036C770);
            D_hd_code_8036C770 = 0;
        }
    }
    if (D_hd_code_8036C784 != 0) {
        gDPPipeSync(entry++);
        gDPSetRenderMode(entry++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
        gSPTexture(entry++, qu016(0.999985), qu016(0.999985), 0, G_TX_RENDERTILE, G_OFF);
        gDPSetPrimColor(entry++, 0, 0, 0, 0, 0, D_hd_code_8036C784);
        gDPSetCycleType(entry++, G_CYC_1CYCLE);
        gDPSetCombineMode(entry++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);

        // @recomp: fill the whole screen
        // gDPFillRectangle(entry++, 0, 0, 319, 239);
        gDPFillRectangle(entry++, 0, 0, 320, 240);

        osWritebackDCache(&D_hd_code_802FA8B0[D_hd_code_8035805C], 0x40);
    }

    // @recomp: inject pillarboxes
    // TODO: perhaps we find a better injection point than the fade code
    pillarbox_draw(&entry);

    return entry;
}

static s32 sLastBuilding = 0;
// @recomp: This fixes the arrow of the building that has to be destroyed for the missile carrier path
RECOMP_PATCH void func_hd_code_80275478(struct FrameContext* arg0, Gfx** arg1, s32 arg2) {
    s16 sp76;
    s16 sp74;
    s16 sp72;
    Gfx* entry;
    s16 sp6A;
    s16 sp68;
    s16 sp66;
    s16 sp64;
    s16 sp62;
    s16 sp60;
    s16 sp5E;
    s16 sp5C;
    u8 sp5B;
    u8 sp5A;
    s16 sp58;
    s16* sp54;
    s32 sp50; // Pointer to the nearest building

    sp72 = 0x10;
    entry = *arg1;
    sp62 = 0x7FFF, sp60 = -0x8000, sp5E = 0x7FFF, sp5C = -0x8000;
    sp50 = func_hd_code_802BCE40();
    D_hd_code_8036C7CC = 0;
    sp54 = D_hd_code_8036C790;
    if ((D_hd_code_8036EB98 == 0) && (sp50 == 0) && (D_hd_code_80364AA8 == 1) && (D_hd_code_802E8BD0 == 0)) {
        if (func_hd_code_8026AD30(0x4B) == 0) {
            yoshiShow(0x803DU);
            func_hd_code_80277EDC(3, 1, 2, 0x82);
        }
        D_hd_code_8036EB98 = 1;
    }
    if ((D_hd_code_8036C790 != NULL) && ((func_hd_code_802768A8() == 0) || ((u8) arg2 != 0)) &&
        (D_hd_code_8036C794 == NULL)) {
        for (sp76 = 0; sp76 < 4; sp76++) {
            func_hd_code_8027690C(arg0, D_hd_code_8036C790->x, D_hd_code_8036C790->y, D_hd_code_8036C790->z, &sp6A,
                                  &sp68, 0, 0, 0, 1.0f);
            D_hd_code_8036C790++;
            if (sp6A < sp62) {
                sp62 = sp6A;
            }
            if (sp6A > sp60) {
                sp60 = sp6A;
            }
            if (sp68 < sp5E) {
                sp5E = sp68;
            }
            if (sp68 > sp5C) {
                sp5C = sp68;
            }
        }
        D_hd_code_8036C7CC = 0;
        for (sp74 = 0; sp74 < 4; sp74++) {
            switch (sp74) {
                case 0:
                    sp66 = ((sp60 - sp62) / 2) + sp62;
                    sp64 = sp5E;
                    break;
                case 1:
                    sp66 = ((sp60 - sp62) / 2) + sp62;
                    sp64 = sp5C;
                    break;
                case 2:
                    sp66 = sp62;
                    sp64 = ((sp5C - sp5E) / 2) + sp5E;
                    break;
                case 3:
                    sp66 = sp60;
                    sp64 = ((sp5C - sp5E) / 2) + sp5E;
                    break;
            }
            if ((sp66 < 0x136) && (sp66 >= 0xB) && (sp64 < 0xE6) && (sp64 >= 0xB)) {
                D_hd_code_8036C794 = sp54;
                D_hd_code_8036C798 = sp50;
                D_hd_code_803F7809 = D_hd_code_803F7808;
                D_hd_code_802FAD44 = 0;
            }
        }
    }
    if (D_hd_code_8036C794 != NULL) {
        func_hd_code_802BD10C(D_hd_code_8036C798);
        if ((D_hd_code_8036C7C8 >= 0x5DD) || (D_hd_code_803643DB == 0)) {
            sp5A = 0xFF;
            sp5B = 0;
        } else if (D_hd_code_8036C7C8 < 0x1F4) {
            sp5B = 0xFF;
            sp5A = 0;
        } else {
            sp58 = (((D_hd_code_8036C7C8 - 0x1F4) / 1000.0f) * 511.0f);
            if (sp58 < 0x100) {
                sp5A = sp58, sp5B = 0xFF;
            } else {
                sp5A = 0xFF, sp5B = 0x1FE - sp58;
            }
        }
        sp54 = D_hd_code_8036C794;
        for (sp76 = 0; sp76 < 4; sp76++) {
            func_hd_code_8027690C(arg0, sp54[0], sp54[1], sp54[2], &sp6A, &sp68, 0, 0, 0, 1.0f);
            sp54 += 3;
            if (sp6A < sp62) {
                sp62 = sp6A;
            }
            if (sp6A > sp60) {
                sp60 = sp6A;
            }
            if (sp68 < sp5E) {
                sp5E = sp68;
            }
            if (sp68 > sp5C) {
                sp5C = sp68;
            }
        }

        for (sp74 = 0; sp74 < 4; sp74++) {
            switch (sp74) {
                case 0:
                    sp66 = ((sp60 - sp62) / 2) + sp62, sp64 = sp5E - D_hd_code_802FAD40;
                    break;
                case 1:
                    sp66 = ((sp60 - sp62) / 2) + sp62, sp64 = sp5C + D_hd_code_802FAD40;
                    break;
                case 2:
                    sp66 = sp62 - D_hd_code_802FAD40, sp64 = ((sp5C - sp5E) / 2) + sp5E;
                    break;
                case 3:
                    sp66 = sp60 + D_hd_code_802FAD40, sp64 = ((sp5C - sp5E) / 2) + sp5E;
                    break;
            }
            if ((sp66 < 0x136) && (sp66 >= 0xB) && (sp64 < 0xE6) && (sp64 >= 0xB)) {
                D_hd_code_8036C7CC += 1;
            }
            sp72 = (s32) func_hd_code_80276080(arg0, sp74, sp72, sp66, sp64, 8, 8, sp5B, sp5A, 0, 0xFF);
        }

        if (D_hd_code_802FAD48 == 0) {
            D_hd_code_802FAD40++;
            if (D_hd_code_802FAD40 == 0xA) {
                D_hd_code_802FAD48 = 1;
                D_hd_code_802FAD44 += 1;
            }
        } else {
            D_hd_code_802FAD40--;
            if (D_hd_code_802FAD40 == 0) {
                D_hd_code_802FAD48 = 0;
                D_hd_code_802FAD44 += 1;
            }
        }
        if (D_hd_code_802FAD44 == 5) {
            D_hd_code_8036C794 = NULL;
        }

        bool shouldInterpolate = sp50 == sLastBuilding;

        if (shouldInterpolate) {
            // @recomp Tag the transform.
            // Interpolating texture coordinates
            gEXMatrixGroup(entry++, TAG_ARROWS, G_EX_INTERPOLATE_SIMPLE, G_EX_PUSH, G_MTX_MODELVIEW,
                           G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                           G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                           G_EX_COMPONENT_SKIP, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW, G_EX_ASPECT_AUTO,
                           G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP);
        } else {
            // @recomp Tag the transform.
            gEXMatrixGroupSkipAll(entry++, TAG_ARROWS, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
            // rmonPrintf("INTERPOLATION SKIPPED!\n");
        }

        entry = func_hd_code_80275DA4(entry, 0);
        gSPVertex(entry++, &D_2000000.unk1F00, 16, 0);

        sp72 = 0;
        for (sp74 = 0; sp74 < 4; sp74++) {
            gSP1Triangle(entry++, sp72, sp72 + 1, sp72 + 2, 0);
            gSP1Triangle(entry++, sp72, sp72 + 2, sp72 + 3, 0);

            sp72 += 4;
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);

        sLastBuilding = sp50;
    }
    func_hd_code_8027656C(arg0);
    *arg1 = entry;
}


struct S_80288DF0 {
    u8 unk0;
    s32 unk4;
}; // Size: 0x8

struct S_8036EC30 {
    s16* unk0;
    s16* unk4;
    s16* unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    s8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 pad23;
}; // Size: 0x24+

struct S_8036EC38 {
    u8 unk0;
    u8 pad1;
    u8 pad2;
    u8 pad3;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 unk16;
    u8 unk17;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 pad1E;
}; // Size: 0x20

extern u8 D_hd_code_80370B8C;
extern struct S_8036EC38 D_hd_code_8036EC38[50];
extern struct S_8036EC30* D_hd_code_8036EC30;
extern Mtx D_hd_code_8036F278[][50];
extern Vtx D_hd_code_802FDA80[4];
void func_hd_code_80289EF4(Gfx** gfx);
u32 func_hd_code_8028A0A0(s16 arg0);
void func_hd_code_8028A1D0(struct S_80288DF0* arg0, s32 arg1);

RECOMP_PATCH void func_hd_code_80288DF0(Gfx** gfx, u8 arg1) {
    s32 sp2FC;
    s32 sp2F8;
    Gfx* entry = *gfx;
    f32 sp2B4[4][4];
    f32 sp274[4][4];
    s32 sp270;
    s32 sp26C;
    s32 sp268;
    s32 sp264;
    s32 sp260;
    s32 sp25C;
    struct S_80288DF0 spCC[50];
    s32 spC8 = 0;
    s32 spC4 = -1;

    if (D_hd_code_80370B8C == 0) {
        return;
    }

    func_hd_code_80289EF4(&entry);
    for (sp2FC = 0; sp2FC < 50; sp2FC++) {
        if (D_hd_code_8036EC38[sp2FC].unk0 != 0) {
            switch (D_hd_code_8036EC38[sp2FC].unk16) {
                case 0:
                    sp25C = D_hd_code_8036EC30->unk0[D_hd_code_8036EC38[sp2FC].unk18];
                    break;
                case 1:
                    sp25C = D_hd_code_8036EC30->unk4[D_hd_code_8036EC38[sp2FC].unk18];
                    break;
                case 3:
                    sp25C = D_hd_code_8036EC30->unk8[D_hd_code_8036EC38[sp2FC].unk18];
                    break;
            }
            spCC[spC8].unk0 = sp2FC;
            spCC[spC8].unk4 = func_hd_code_8028A0A0(sp25C);
            spC8 += 1;
        }
    }
    func_hd_code_8028A1D0(spCC, spC8);

    static s16 sLastParticleState[50] = { 0 }; // recomp

    for (sp2F8 = 0; sp2F8 < spC8; sp2F8++) {
        sp2FC = spCC[sp2F8].unk0;

        // @recomp: check if the particle is being used
        if (D_hd_code_8036EC38[sp2FC].unk0 != 0) {
            if (sLastParticleState[sp2FC] != D_hd_code_8036EC38[sp2FC].unk16) {
                D_hd_code_8036EC38[sp2FC].pad1E = 0;
            }
        }

        // @recomp: check if the particle should be interpolated
        if (D_hd_code_8036EC38[sp2FC].pad1E) {
            // @recomp Tag the transform.
            // Interpolating texture coordinates
            gEXMatrixGroup(entry++, (u32) &D_hd_code_8036EC38[sp2FC], G_EX_INTERPOLATE_SIMPLE, G_EX_PUSH,
                           G_MTX_MODELVIEW, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                           G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                           G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW,
                           G_EX_ASPECT_STRETCH, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP);
        } else {
            // @recomp Tag the transform.
            gEXMatrixGroupSkipAll(entry++, (u32) &D_hd_code_8036EC38[sp2FC], G_EX_PUSH, G_MTX_MODELVIEW,
                                  G_EX_EDIT_NONE);
            D_hd_code_8036EC38[sp2FC].pad1E = 1;
            // rmonPrintf("INTERPOLATION SKIPPED FOR PARTICLE: %d !\n", sp2FC);
        }

        if (spCC[sp2F8].unk4 != spC4) {
            spC4 = spCC[sp2F8].unk4;

            gDPPipeSync(entry++);

            switch (D_hd_code_8036EC30->unk22) {
                case G_IM_SIZ_4b:
                    rare_gDPLoadTextureBlock_4b(entry++, OS_PHYSICAL_TO_K0(spC4), D_hd_code_8036EC30->unk21,
                                                D_hd_code_8036EC30->unk16, D_hd_code_8036EC30->unk17, 0,
                                                G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
                                                G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                    break;
                case G_IM_SIZ_8b:
                    gDPLoadTextureBlock(entry++, OS_PHYSICAL_TO_K0(spC4), D_hd_code_8036EC30->unk21, G_IM_SIZ_8b,
                                        D_hd_code_8036EC30->unk16, D_hd_code_8036EC30->unk17, 0,
                                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
                                        G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                    break;
                case G_IM_SIZ_16b:
                    gDPLoadTextureBlock(entry++, OS_PHYSICAL_TO_K0(spC4), D_hd_code_8036EC30->unk21, G_IM_SIZ_16b,
                                        D_hd_code_8036EC30->unk16, D_hd_code_8036EC30->unk17, 0,
                                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
                                        G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                    break;
                case G_IM_SIZ_32b:
                    gDPLoadTextureBlock(entry++, OS_PHYSICAL_TO_K0(spC4), D_hd_code_8036EC30->unk21, G_IM_SIZ_32b,
                                        D_hd_code_8036EC30->unk16, D_hd_code_8036EC30->unk17, 0,
                                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
                                        G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                    break;
            }
        }
        sp270 = func_hd_code_8026A6F0(D_hd_code_803643F8 >> 11, D_hd_code_803643FC >> 11, D_hd_code_80364400 >> 11,
                                      D_hd_code_8036EC38[sp2FC].unk4, D_hd_code_8036EC38[sp2FC].unk8,
                                      D_hd_code_8036EC38[sp2FC].unkC);
        if (sp270 == 0) {
            sp270 = 1;
        }
        sp26C = (D_hd_code_803643FC >> 11) - D_hd_code_8036EC38[sp2FC].unk8;
        sp26C <<= 16;
        sp264 = sp26C / sp270;
        if (sp264 < 0) {
            sp260 = -sp264;
        } else {
            sp260 = sp264;
        }
        sp268 = func_hd_code_802AD7D4(sp260);
        if (sp264 > 0) {
            sp268 = -sp268;
        }
        guAlignF(sp2B4, ((f32) sp268 / 65536.0) * 360.0, 1.0f, 0.0f, 0.0f);
        guAlignF(sp274, ((f32) D_hd_code_80364452 / 4095.0) * 360.0, 0.0f, 1.0f, 0.0f);
        guMtxCatF(sp2B4, sp274, sp2B4);
        guTranslateF(sp274, D_hd_code_8036EC38[sp2FC].unk4 / 32.0f, D_hd_code_8036EC38[sp2FC].unk8 / 32.0f,
                     D_hd_code_8036EC38[sp2FC].unkC / 32.0f);
        guMtxCatF(sp2B4, sp274, sp2B4);
        guMtxF2L(sp2B4, &D_hd_code_8036F278[arg1][sp2FC]);
        gSPMatrix(entry++, osVirtualToPhysical(&D_hd_code_8036F278[arg1][sp2FC]),
                  G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(entry++, osVirtualToPhysical(D_hd_code_802FDA80), 4, 0);
        gSP1Triangle(entry++, 0, 1, 2, 0);
        gSP1Triangle(entry++, 0, 2, 3, 0);
        gSPPopMatrix(entry++, G_MTX_MODELVIEW);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);

        // @recomp: record last state of the particle
        // check if the particle is being used
        if (D_hd_code_8036EC38[sp2FC].unk0 != 0) {
            sLastParticleState[sp2FC] = D_hd_code_8036EC38[sp2FC].unk16;
        }
    }

    *gfx = entry;
}

#if 1
struct S_8020BD30 {
    f32 unk0;
    s32 pad4;
    f32 unk8;
    f32 unkC;
    struct S_8020BD30* unk10;
    struct S_8020BD30* unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 pad1B;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
};
typedef struct {
    s32 unk0;
    Gfx* unk4;
} S_80218270;

extern struct S_8020BD30 D_hd_front_end_8020BD30;
extern struct S_8020BD30 D_hd_front_end_8020BDE4;
extern f32 D_hd_front_end_80217B54;
extern f32 D_hd_front_end_80217B58;
extern f32 D_hd_front_end_80217B5C;
extern Mtx D_hd_front_end_80217B70[7][4];
extern S_80218270 D_hd_front_end_80218270[];

Gfx* func_hd_front_end_801FE238(Gfx*, struct FrameContext*);
Gfx* func_hd_front_end_801F4110(Gfx* gfx, struct FrameContext* arg1, struct S_8020BD30* arg2, f32 arg3);
Gfx* func_hd_front_end_801F3964(Gfx* gfx, struct FrameContext* arg1, struct S_8020BD30* arg2, f32 arg3);

RECOMP_PATCH void func_hd_front_end_801F4878(Gfx* gfx, struct FrameContext* arg1) {
    Gfx* entry = gfx;
    s32 sp68;
    struct S_8020BD30* sp64;
    struct S_8020BD30* sp60;
    Mtx* sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;

    for (sp68 = 0; sp68 < 7; sp68++) {
        sp64 = &D_hd_front_end_8020BD30;
        sp60 = &sp64[((s32) &sp64[sp68] - (s32) sp64) / 0x3C];
        sp5C = D_hd_front_end_80217B70[sp68];
        sp58 = sp60->unk0 / D_hd_front_end_8020BDE4.unk0;
        sp54 = sp60->unk20 - D_hd_front_end_80217B54;
        sp50 = sp60->unk24 - D_hd_front_end_80217B58;
        sp4C = sp60->unk28 - D_hd_front_end_80217B5C;
        sp60->unk38 = sqrtf((sp54 * sp54) + (sp50 * sp50) + (sp4C * sp4C));
        D_hd_front_end_80218270[((s32) sp60 - (s32) &D_hd_front_end_8020BD30) / 0x3C].unk0 =
            ((s32) sp60 - (s32) &D_hd_front_end_8020BD30) / 0x3C;
        D_hd_front_end_80218270[((s32) sp60 - (s32) &D_hd_front_end_8020BD30) / 0x3C].unk4 = entry;
        // @recomp: enable extended commands for each planet. Each planet is put into the lookup table D_hd_front_end_80218270
        gEXEnable(entry++);

        // @recomp: tag planets one by one
        gEXMatrixGroupDecomposedNormal(entry++, TAG_PLANET(sp68), G_EX_PUSH, G_MTX_PROJECTION, G_EX_EDIT_NONE);
        gEXMatrixGroupDecomposedPosRotOnly(entry++, TAG_PLANET(sp68) | 0x10, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_NONE);

        switch (sp68) {
            case 3:

                gSPMatrix(entry++, &arg1->projection, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
                gSPMatrix(entry++, &arg1->projection2, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
                gSPMatrix(entry++, &sp5C[D_hd_code_8035805C], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
                gSPMatrix(entry++, &sp5C[D_hd_code_8035805C + 2], G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
                gSPMatrix(entry++, &arg1->unk1280, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
                entry = func_hd_front_end_801FE238(entry, arg1);
                break;
            case 6:
                gSPMatrix(entry++, &arg1->mtxOrthoLarge, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
                gSPMatrix(entry++, &arg1->modelview, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
                entry = func_hd_front_end_801F4110(entry, arg1, sp60, sp58);
                break;
            default:
                gSPMatrix(entry++, &arg1->mtxOrthoLarge, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
                gSPMatrix(entry++, &arg1->modelview, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
                entry = func_hd_front_end_801F3964(entry, arg1, sp60, sp58);
                break;
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);
        gEXPopMatrixGroup(entry++, G_MTX_PROJECTION);

        gSPEndDisplayList(entry++);
    }
}

#endif


#if 1

extern s32 D_hd_code_8036506C;
extern s16 D_hd_code_80364440;
// @recomp: dynamically generated shadow tagging
RECOMP_PATCH void func_hd_code_8024E4F4(Gfx** arg0, struct FrameContext* arg1, u8 arg2) {
    Gfx* entry; // sp144
    s32 sp140;
    s16 pad;
    s16 sp13C;
    s16 sp13A;
    s16 sp138;
    s16 sp136;
    s16 sp134;
    u8 sp133;
    f32 spF0[4][4];
    f32 spB0[4][4];
    s16 spAE;
    s16 spAC;
    u8 spAB;

    entry = *arg0;
    if (arg2 == 0) {
        D_hd_code_8036506C = 0;
    }
    gDPPipeSync(entry++);
    gDPSetTexturePersp(entry++, G_TP_PERSP);
    gDPSetCycleType(entry++, G_CYC_1CYCLE);
    gSPClearGeometryMode(entry++, 0xFFFFFFFF);
    gSPSetGeometryMode(entry++, G_SHADE | G_CULL_BACK | G_LOD | G_SHADING_SMOOTH);
    gSPTexture(entry++, qu016(0.999985), qu016(0.999985), 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(entry++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetTextureFilter(entry++, G_TF_BILERP);
    gDPSetRenderMode(entry++, G_RM_CLD_SURF, G_RM_CLD_SURF2);

    sp133 = !D_hd_code_803649E8 && (D_hd_code_803649EC || (g_currentGameState & 0x1801));
    sp140 = 0;
    while (&D_hd_code_803643C8.textures[sp140] != D_hd_code_803643C8.end) {
        if (((D_hd_code_803643C8.textures[sp140].unk1022 != 0) || (sp133 != 0)) &&
            (D_hd_code_803643C8.textures[sp140].unk1022 != 0xFE) &&
            ((D_hd_code_803643C8.textures[sp140].unk1022 != 0xFF) || (D_hd_code_803EF6FF == 0)) &&
            ((D_hd_code_803643C8.textures[sp140].unk1022 == 0xFD) || (D_hd_code_80364A84 == 0) ||
             (D_hd_code_80364AC1 == 0)) &&
            D_hd_code_803643C8.textures[sp140].unk1023 == arg2) {
            sp13C = D_hd_code_803643C8.textures[sp140].unk1004 >> 5;
            sp13A = D_hd_code_803643C8.textures[sp140].unk1008 >> 5;
            sp138 = D_hd_code_803643C8.textures[sp140].unk100C >> 5;
            sp136 = D_hd_code_803643C8.textures[sp140].unk1018; // 2D x
            sp134 = D_hd_code_803643C8.textures[sp140].unk101A; // 2D y
            spAB = D_hd_code_803643C8.textures[sp140].unk1022 == D_hd_code_80364456 && D_hd_code_803ED40D == 0x65;
            if (spAB) {
                guAlignF(spB0, ((f32) D_hd_code_80364440 * 360.0) / 4096.0f, 0.0f, 1.0f,
                         0.0f); // Additional rotation around y axis if spAB
            }
            guAlignF(spF0, ((f32) -D_hd_code_803643C8.textures[sp140].unk101E * 360.0) / 4096.0f, 0.0f, 1.0f,
                     0.0f); // rotation around y axis by -unk101E * D_hd_code_803087C0 degrees
            if (spAB) {
                guMtxCatF(spB0, spF0, spF0); // Apply additional rotation
            }
            guAlignF(spB0, (f32) (((f64) (f32) D_hd_code_803643C8.textures[sp140].unk101C * 360.0) / 4096.0), 1.0f, 0,
                     0.0f);
            guMtxCatF(spF0, spB0, spB0);
            guAlignF(spF0, (f32) (((f64) (f32) D_hd_code_803643C8.textures[sp140].unk1020 * 360.0) / 4096.0), 0, 0,
                     1.0f);
            guMtxCatF(spB0, spF0, spF0);
            guAlignF(spB0, (f32) (((f64) (f32) D_hd_code_803643C8.textures[sp140].unk101E * 360.0) / 4096.0), 0, 1.0f,
                     0.0f);
            guMtxCatF(spF0, spB0, spF0);
            guTranslateF(spB0, (f32) sp13C, (f32) sp13A, (f32) sp138);
            guMtxCatF(spF0, spB0, spF0);
            guMtxF2L(spF0, &arg1->unk2C0[sp140]);

            if (spAB) {
                gDPLoadTextureBlock(entry++, OS_PHYSICAL_TO_K0(D_hd_code_802FA940), G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                                    G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                    G_TX_NOLOD, G_TX_NOLOD);
                spAC = spAE = 32;
            } else {
                gDPLoadTextureBlock(entry++, K0_TO_PHYS((s32) (D_hd_code_803643C8.textures[sp140].data)), G_IM_FMT_IA,
                                    G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP,
                                    G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                spAC = spAE = 64;
            }

            SET_COLOR_VERTEX(arg1->unk1580[D_hd_code_8036506C + 4], (s16) -sp136, 0, sp134, (s16) ((spAE - 1) << 5),
                             (s16) ((spAC - 1) << 5), 0xA, 0xA, 0xA, 0x8C);
            D_hd_code_8036506C++;
            SET_COLOR_VERTEX(arg1->unk1580[D_hd_code_8036506C + 4], sp136, 0, sp134, 0, (s16) ((spAC - 1) << 5), 0xA,
                             0xA, 0xA, 0x8C);
            D_hd_code_8036506C++;
            SET_COLOR_VERTEX(arg1->unk1580[D_hd_code_8036506C + 4], sp136, 0, (s16) -sp134, 0, 0, 0xA, 0xA, 0xA, 0x8C);
            D_hd_code_8036506C++;
            SET_COLOR_VERTEX(arg1->unk1580[D_hd_code_8036506C + 4], (s16) -sp136, 0, (s16) -sp134,
                             (s16) ((spAE - 1) << 5), 0, 0xA, 0xA, 0xA, 0x8C);
            D_hd_code_8036506C++;

            // @recomp Tag the shadows
            gEXMatrixGroupDecomposedVertsSkipRot(entry++, TAG_SHADOW(sp140), G_EX_PUSH, G_MTX_MODELVIEW,
                                                 G_EX_EDIT_NONE);


            if ((D_hd_code_803643D6 != 0) && !(D_hd_code_80364AA8 & 0x81) &&
                (D_hd_code_803643C8.textures[sp140].unk1022 == D_hd_code_80364456)) {
                gSPMatrix(entry++, &D_2000000.unk1540, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            }
            gSPMatrix(entry++, &D_2000000.unk2C0[sp140], G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPVertex(entry++, &D_2000000.unk1580[D_hd_code_8036506C], 4, 0);
            gSP1Triangle(entry++, 0, 1, 2, 0);
            gSP1Triangle(entry++, 0, 2, 3, 0);
            gSPPopMatrix(entry++, G_MTX_MODELVIEW);

            if ((D_hd_code_803643D6 != 0) && !(D_hd_code_80364AA8 & 0x81) &&
                (D_hd_code_803643C8.textures[sp140].unk1022 == D_hd_code_80364456)) {
                gSPPopMatrix(entry++, G_MTX_MODELVIEW);
            }
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);

            gDPPipeSync(entry++);
        }
        sp140++;
    }
    *arg0 = entry;
}

#endif

#if 1
extern Vp D_hd_code_802E8C60;
RECOMP_PATCH void func_hd_code_80258544(struct Texture* arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32* arg5, s32* arg6,
                           s32* arg7) {
    Gfx* entry;
    u16 sp9A;
    u32 pad;

    entry = &D_hd_code_803650B0[0];

    // @recomp: enable extended commands for shadow rendering
    gEXEnable(entry++);


    gSPViewport(entry++, VIRTUAL_TO_PHYSICAL(&D_hd_code_802E8C60));
    gSPClearGeometryMode(entry++, G_ZBUFFER | G_TEXTURE_ENABLE | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING |
                                      G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH | 0xFFE0CDF8);
    gSPTexture(entry++, qu016(0.999985), qu016(0.999985), 0, G_TX_RENDERTILE, G_OFF);
    gSPSegment(entry++, 0x00, 0x00000000);
    gSPSegment(entry++, 0x06, VIRTUAL_TO_PHYSICAL(arg6));
    gSPSegment(entry++, 0x07, VIRTUAL_TO_PHYSICAL(arg7));
    gDPPipeSync(entry++);
    gDPSetScissor(entry++, G_SC_NON_INTERLACE, 0, 0, 63, 63);
    gDPSetPrimColor(entry++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetColorDither(entry++, G_CD_DISABLE);
    gDPSetCycleType(entry++, G_CYC_FILL);
    gDPSetColorImage(entry++, G_IM_FMT_CI, G_IM_SIZ_8b, 64, VIRTUAL_TO_PHYSICAL(arg0));
    gDPSetFillColor(entry++, 0x00010001);
    gDPFillRectangle(entry++, 0, 0, 63, 63);
    gDPPipeSync(entry++);
    gDPSetCycleType(entry++, G_CYC_1CYCLE);
    gDPSetRenderMode(entry++, G_RM_OPA_CI, G_RM_OPA_CI2);
    gDPSetCombineMode(entry++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);

    // @recomp Tag the shadows
    s32 sp140 = ((s32) arg0 - (s32) D_hd_code_803643C8.textures) / 0x1040;
    gEXMatrixGroupDecomposedNormal(entry++, TAG_SHADOW(sp140) | 0x100, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    gEXMatrixGroupDecomposedNormal(entry++, TAG_SHADOW(sp140) | 0x200, G_EX_PUSH, G_MTX_PROJECTION, G_EX_EDIT_NONE);



    guPerspective(&D_hd_code_803651F0, &sp9A, 45.0f, 1.0f, 1.0f, 1000.0f, 1.0f);
    gSPMatrix(entry++, VIRTUAL_TO_PHYSICAL(&D_hd_code_803651F0), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPPerspNormalize(entry++, sp9A);

    guTranslate(&D_hd_code_803652F0, 0.0f, 0.0f, -arg4);
    gSPMatrix(entry++, VIRTUAL_TO_PHYSICAL(&D_hd_code_803652F0), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    guAlign(&D_hd_code_80365270, -90.0f, 1.0f, 0.0f, 0.0f);
    gSPMatrix(entry++, VIRTUAL_TO_PHYSICAL(&D_hd_code_80365270), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    guAlign(&D_hd_code_803652B0, 180.0f, 0.0f, 1.0f, 0.0f);
    gSPMatrix(entry++, VIRTUAL_TO_PHYSICAL(&D_hd_code_803652B0), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    guTranslate(&D_hd_code_80365230, -(arg1 / 32.0f), -(arg2 / 32.0f), -(arg3 / 32.0f));
    gSPMatrix(entry++, VIRTUAL_TO_PHYSICAL(&D_hd_code_80365230), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPDisplayList(entry++, VIRTUAL_TO_PHYSICAL(arg5));
    
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(entry++, G_MTX_PROJECTION);
    gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);

    
    gSPEndDisplayList(entry++);

    osWritebackDCache(&D_hd_code_803651F0, 0x140);
    gfxSubmitTask(&D_hd_code_803650B0, entry - D_hd_code_803650B0, 1, 0, 0x61F, 0);
}

#endif


#if 1

extern f32 D_hd_front_end_8020E3D8;
extern s8 D_hd_front_end_8021A907;

Gfx* func_hd_front_end_801FA180(Gfx* arg0, struct FrameContext* arg1, f32 arg2, s8* arg3);

RECOMP_PATCH Gfx* func_hd_front_end_801F9820(Gfx* gfx, struct FrameContext* arg1, s32* arg2) {
    Gfx* entry = gfx;
    gSPSegment(entry++, 0, 0x00000000);
    gSPSegment(entry++, 2, osVirtualToPhysical(arg1));
    gSPSegment(entry++, 1, osVirtualToPhysical(D_hd_code_8035806C));
    gSPDisplayList(entry++, D_1000010);
    gSPDisplayList(entry++, D_1000038);
    gDPSetColorImage(entry++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, D_hd_code_80358050[D_hd_code_8035805C]);
    gDPSetDepthImage(entry++, D_hd_code_80358058);
    gDPPipeSync(entry++);
    gDPSetRenderMode(entry++, G_RM_AA_XLU_LINE, G_RM_AA_XLU_LINE2);
    gSPPerspNormalize(entry++, D_hd_code_8035807C);

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(entry++, TAG_PLANET_LINES, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);

    gSPMatrix(entry++, &arg1->projection, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(entry++, &arg1->projection2, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(entry++, &D_hd_front_end_80217B70[3][D_hd_code_8035805C], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(entry++, &D_hd_front_end_80217B70[3][D_hd_code_8035805C + 2], G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(entry++, &arg1->unk1280, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    entry = func_hd_front_end_801FA180(entry, arg1, D_hd_front_end_8020E3D8, &D_hd_front_end_8021A907);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);

    gSPEndDisplayList(entry++);
    *arg2 = (s32) ((s32) entry - (s32) gfx) >> 3;
    return entry;
}

#endif


#if 1

struct S_8039C960 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u8 unk18;
    s16 unk1A;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    u8 unk28;
    u8 unk29;
    u8 pad2A;
    u8 pad2B;
};  // Size: 0x2C

struct S_802FE3C0 {
    Gfx unk0[80];
    Vtx unk280[50];
    s16 unk5A0;
    s16 pad5A2;
    s32 unk5A4;
    u8 unk5A8;
    u8 unk5A9;
    u8 pad5AA;
    u8 pad5AB;
    s32 unk5AC;
    u8 unk5B0;
    u8 pad5B1;
    u8 pad5B2;
    u8 pad5B3;
    s32 pad5B4;
};  // Size: 0x5B8

extern struct S_8039C960 D_hd_code_8039C960[4];
extern struct S_802FE3C0 D_hd_code_802FE3C0[1];

RECOMP_PATCH void func_hd_code_80292EB8(Gfx** gfx, struct FrameContext* arg1) {
    Gfx* entry;
    s32 spD8;
    f32 sp98[4][4];
    f32 sp58[4][4];

    entry = *gfx;

    gDPPipeSync(entry++);
    gDPSetCycleType(entry++, G_CYC_1CYCLE);
    gDPSetRenderMode(entry++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCombineMode(entry++, G_CC_SHADE, G_CC_SHADE);
    gSPClearGeometryMode(entry++, G_ZBUFFER | G_TEXTURE_ENABLE | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING |
                                      G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH | 0xFFE0CDF8);
    gSPSetGeometryMode(entry++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
    gSPTexture(entry++, qu016(0.999985), qu016(0.999985), 0, G_TX_RENDERTILE, G_OFF);

    for (spD8 = 0; spD8 < 4; spD8++) {
        if ((D_hd_code_8039C960[spD8].unk28 != 0) && (D_hd_code_8039C960[spD8].unk24 >= 2)) {
            guScaleF(sp98, 0.5f, 0.5f, 0.35f);
            guAlignF(sp58, (f32)(((f64)D_hd_code_8039C960[spD8].unk16 / 4095.0) * 360.0), 1.0f, 0.0f, 0.0f);
            guMtxCatF(sp98, sp58, sp98);
            guAlignF(sp58, (f32)(((f64)D_hd_code_8039C960[spD8].unk14 / 4095.0) * 360.0), 0.0f, 1.0f, 0.0f);
            guMtxCatF(sp98, sp58, sp98);
            guTranslateF(sp58, (f32)D_hd_code_8039C960[spD8].unk0 / 32.0f, (f32)D_hd_code_8039C960[spD8].unk4 / 32.0f,
                         (f32)D_hd_code_8039C960[spD8].unk8 / 32.0f);
            guMtxCatF(sp98, sp58, sp98);
            guMtxF2L(sp98, &arg1->unkD00[spD8]);

            // @recomp: tag the transform
            gEXMatrixGroupDecomposedNormal(entry++, TAG_PROJECTILE(spD8), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
            gSPMatrix(entry++, &D_2000000.unkD00[spD8], G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gDPPipeSync(entry++);
            gDPSetPrimColor(entry++, 0, 0, D_hd_code_8039C960[spD8].unk29, 0, 0, 0xFF);
            gSPDisplayList(entry++, osVirtualToPhysical(&D_hd_code_802FE3C0[D_hd_code_8039C960[spD8].unk18].unk0));
            gSPPopMatrix(entry++, G_MTX_MODELVIEW);
            // @recomp: pop the transform
            gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);
        }
    }

    gDPPipeSync(entry++);
    *gfx = entry;
}
#endif