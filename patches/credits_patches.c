#include "patches.h"
#include "gfx_tagging.h"
#include <hd_code/macros.h>
#include <hd_code/functions.h>
#include <hd_code/variables.h>
#include <hd_code/yoshi.h>

struct S_80304A90 {
    s32 unk0;
    u8 unk4;
    u8 unk5;
    char unk6[42];
}; // Size: 0x30

extern struct S_80304A90 D_hd_code_80304A90[0x52];
extern s32 D_hd_code_803A6B20; // credits scroll position
extern s32 D_hd_code_803A6B24;
extern u8 D_hd_code_803643D6;
extern u8 D_hd_code_8035805C;
extern u32 g_frameCount;

void drawTextBegin(Gfx** gfx, struct FrameContext* ctx);

// @recomp: credits matrix, double buffered
static Mtx sCreditsScrollMtx[2];

// @recomp: patch the credits. apply scroll position with a matrix
RECOMP_PATCH s32 func_hd_code_8029A518(struct FrameContext* arg0, s32 arg1) {
    Gfx* entry;
    Mtx* scrollMtx;
    struct S_80304A90* sp60;
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    s32 sp50;

    sp54 = 0;
    entry = (Gfx*) arg1;

    for (sp5C = 0; sp5C < 0x52; sp5C++) {
        sp60 = &D_hd_code_80304A90[sp5C];
        if (sp60->unk0 & 1) {
            sp50 = 0xA0;
        } else if (sp60->unk0 & 4) {
            sp50 = 0x6A;
        } else if (sp60->unk0 & 8) {
            sp50 = 0xD5;
        } else {
            sp50 = 0;
        }
        if ((sp54 - D_hd_code_803A6B20 >= -0x31) && (sp54 - D_hd_code_803A6B20 < 0xF0)) {
            // @recomp: queue the text without the scroll position
            func_hd_code_80259DC8(arg0, (u8*) sp60->unk6, 0, 0, sp50, sp58, sp54, sp60->unk5, sp60->unk5, 1,
                                  D_hd_code_802F47B0[sp60->unk4][0], D_hd_code_802F47B0[sp60->unk4][1],
                                  D_hd_code_802F47B0[sp60->unk4][2], D_hd_code_802F47B0[sp60->unk4][3],
                                  D_hd_code_802F47B0[sp60->unk4][4], D_hd_code_802F47B0[sp60->unk4][5],
                                  D_hd_code_802F47B0[sp60->unk4][6], D_hd_code_802F47B0[sp60->unk4][7]);
            /*
            func_hd_code_80259DC8(arg0, sp60->unk6, 0, 0, sp50, sp58, sp54 - D_hd_code_803A6B20, sp60->unk5, sp60->unk5, 1,
                            D_hd_code_802F47B0[sp60->unk4][0], D_hd_code_802F47B0[sp60->unk4][1],
                            D_hd_code_802F47B0[sp60->unk4][2], D_hd_code_802F47B0[sp60->unk4][3],
                            D_hd_code_802F47B0[sp60->unk4][4], D_hd_code_802F47B0[sp60->unk4][5],
                            D_hd_code_802F47B0[sp60->unk4][6], D_hd_code_802F47B0[sp60->unk4][7]);
            */
        }
        if (sp60->unk0 & 0x20) {
            sp54 += 0x26;
        }
        if (sp60->unk0 & 0x10) {
            sp54 += 0x16;
        }
        if (sp60->unk0 & 0x40) {
            sp54 += 0x11;
        }
    }

    // @recomp: flush the credits text with the scroll applied as a matrix
    scrollMtx = &sCreditsScrollMtx[D_hd_code_8035805C];
    guTranslate(scrollMtx, 0.0f, (f32) -D_hd_code_803A6B20, 0.0f);
    gSPMatrix(entry++, (u32) &arg0->mtxOrtho, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(entry++, (u32) &arg0->modelview, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gEXMatrixGroupSimple(entry++, TAG_CREDITS, G_EX_PUSH, G_MTX_MODELVIEW,
                         G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP,
                         G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP,
                         G_EX_ORDER_LINEAR, G_EX_EDIT_NONE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP);
    gSPMatrix(entry++, (u32) scrollMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    drawTextBegin(&entry, arg0);
    drawTextQueued(&entry, arg0);
    gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);
    gSPMatrix(entry++, (u32) &arg0->modelview, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    // @recomp: end

    if (g_frameCount == 0x64) {
        yoshiShow(0x8036U);
    }
    if ((u32) g_frameCount >= 0x18CU) {
        D_hd_code_803A6B20 += 1;
    }
    if (D_hd_code_803643D6 != 0) {
        D_hd_code_803A6B24 += 1;
    }
    if (D_hd_code_803A6B24 == 0x7D) {
        yoshiShow(0x8037U);
        func_hd_code_80260EE0(0x25);
    }
    if ((D_hd_code_803A6B24 >= 0x7E) && (D_hd_code_8036BB1C == 1) && (areWeFading() == 0)) {
        fadeTo(0x200000000000, 0.75f);
    }
    return (s32) entry;
}
