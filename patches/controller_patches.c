#include "patches.h"
#include "misc_funcs.h"
#include "config.h"
#include <PR/sched.h>
#include <PR/os.h>
#include <hd_code/macros.h>
#include <hd_code/hd.h>
#include <levels.h>
#include <hd_code/functions.h>
#include <hd_code/variables.h>

struct LevelData {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ char* name;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 padC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ s8 unk18[0x1C - 0x18];
    /* 0x1C */ s8 unk1C[0x24 - 0x1C];
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
}; /* Size: 0x30 */

extern u8 D_hd_front_end_8021A904;
extern u8 D_hd_front_end_8021A905;
extern s8 D_hd_front_end_8021A906;
extern s8 D_hd_front_end_8021A907;
extern f32 D_hd_front_end_8021A90C;
extern f32 D_hd_front_end_8021A910;
extern f32 D_hd_front_end_8021A914;
extern f32 D_hd_front_end_8021A918;
extern f32 D_hd_front_end_8021A91C;
extern f32 D_hd_front_end_8021A920;
extern u16 D_hd_front_end_8021A924;
extern u16 D_hd_front_end_8021A926;
extern u8 D_hd_front_end_8021A930;
extern f32 D_hd_front_end_8021A934;
extern f32 D_hd_front_end_8021A938;
extern s32 D_hd_front_end_80217B6C;
extern f32 D_hd_front_end_8020E3D8;
extern u8 D_hd_front_end_8021AB21;
extern u32 D_hd_front_end_8021AB24;
extern f32 D_hd_front_end_8021AB28;
extern s16 D_hd_front_end_8021AB2C;
extern u8 D_hd_front_end_8021AB2E;
extern struct S_80367C04* D_hd_front_end_8021AB34;
extern s32 D_hd_front_end_8021AB38;
extern s32 D_hd_front_end_8021AB58;
extern s32 D_hd_front_end_8021AB5C;
extern f32 D_hd_front_end_8021AB60;
extern f32 D_hd_front_end_8021AB64;
extern Gfx* D_hd_front_end_8021AB68;
extern Gfx* D_hd_front_end_8021AB6C;
extern struct LevelData D_hd_front_end_8020D810[];
extern s32 D_hd_code_80358078;
extern ALBank* D_hd_code_80367738;
extern OSMesgQueue D_hd_code_80315180;

void func_hd_front_end_801ECB18(void);
void func_hd_front_end_801F885C(s32);
f32 func_hd_front_end_801FD6B8(f32, f32, f32);
void func_hd_front_end_801FD484(f32*, f32*, f32*, f32*, f32*, f32);
Gfx* func_hd_front_end_801F9258(Gfx*, struct FrameContext*, s32*);
Gfx* func_hd_front_end_801F9820(Gfx*, struct FrameContext*, s32*);
Gfx* func_hd_front_end_801F9B84(Gfx*, struct FrameContext*, s32*);
void func_hd_front_end_801FD748(void);
f32 func_hd_code_8028BBF4(s16, s16, s16, s16);
u64 func_hd_code_80299FE8(s32);
Gfx* func_hd_front_end_801FA180(Gfx*, struct FrameContext*, f32, s8*);

RECOMP_PATCH void func_hd_front_end_801F8980(void) {
    s16 sp46;
    s16 sp44;
    s32 sp40;
    u32 sp3C;
    struct FrameContext* sp38;

    nextdma = 0;
    no_palette_dmas = 0;
    func_hd_code_802A5720();
    func_hd_code_8028A3E4();
    if (g_frameCount != 0) {
        gfxSubmitTask(D_hd_code_803156F8[D_hd_code_8035805C].dp, D_hd_code_80358078, 2, 0, 1234, 0);
        gfxSubmitTask(D_hd_front_end_8021AB68, D_hd_front_end_8021AB58, 0, 0, 1234, 0);
        gfxSubmitTask(D_hd_front_end_8021AB6C, D_hd_front_end_8021AB5C, 1, 1, 1234, 0);
    } else {
        gfxSubmitTask(D_hd_code_803156F8[D_hd_code_8035805C].dp, D_hd_code_80358078, 1, 1, 1234, 0);
    }
    D_hd_code_8035805C ^= 1;
    sp38 = &D_hd_code_803156F8[D_hd_code_8035805C];
    controllerUpdateInput();
    if ((D_hd_front_end_8021AB2E != 0) ||
        ((areWeFading() == 0) && (D_hd_front_end_8021A924 == 1) && (g_currentButtons & (A_BUTTON | START_BUTTON)) &&
         !(g_previousButtons & (A_BUTTON | START_BUTTON)))) {
        if (D_hd_front_end_8021A926 == 0) {
            sndDeactivateAllSfxByFlag_3();
            sndPlaySfx((struct ALBankAlt_s*) D_hd_code_80367738, 0x1E, NULL);
            D_hd_front_end_8021A924 = 2;
            rmonPrintf("selected level %d\n", D_hd_front_end_8021A905);
            g_currentLevel = D_hd_front_end_8021A905;
            func_hd_front_end_801ECB18();
            D_hd_front_end_8021AB2E = 0;
        } else {
            D_hd_front_end_8021AB2E = 1;
        }
    }
    if ((g_currentButtons & B_BUTTON) && !(g_previousButtons & B_BUTTON) && (areWeFading() == 0) &&
        (D_hd_front_end_8021A924 == 1)) {
        D_hd_code_80364A87 = 0;
        sndPlaySfx((struct ALBankAlt_s*) D_hd_code_80367738, 0xDE, NULL);
        func_hd_code_80275390(0x4000000000000000);
        g_currentLevel = D_hd_front_end_8021A905;
    }
    sp3C = (D_hd_code_80370C2C * D_hd_code_80370C2C) + (D_hd_code_80370C2D * D_hd_code_80370C2D);
    if (sp3C < 0x5DC) {
        D_hd_front_end_8021A930 = 1;
    }
    if (sp3C != 0) {
        D_hd_front_end_8020E3D8 = func_hd_code_8028BBF4(0, 0, D_hd_code_80370C2C, -D_hd_code_80370C2D);

        // @recomp: determine the target level AFTER calculating the angle
        static Gfx tmp[1024]; // was about max. 400 gfx commands when testing, so 1024 is enough room for dumping the commands
        func_hd_front_end_801FA180(tmp, sp38, D_hd_front_end_8020E3D8, &D_hd_front_end_8021A907);
    }
    if ((sp3C >= 0x5DD) && (D_hd_front_end_8021A930 != 0) && (D_hd_front_end_8021A907 >= 0) &&
        (D_hd_front_end_8021AB2C == 0xFF) && (D_hd_front_end_8021A924 == 1) && (D_hd_code_8036BB1C == 1)) {
        D_hd_front_end_8021A930 = 0;
        D_hd_front_end_8021A904 = D_hd_front_end_8021A905;
        func_hd_front_end_801F885C(D_hd_front_end_8021A907);
        D_hd_front_end_8021A926 = 1;
        D_hd_front_end_8021AB28 = 1.0f;
        if (D_hd_front_end_8021AB38 != 0) {
            sndDeactivate(D_hd_front_end_8021AB38);
        }
    }
    switch (D_hd_front_end_8021A924) { /* irregular */
        case 0:
            if ((D_hd_front_end_8021AB21 + 0xA) >= 0x100) {
                D_hd_front_end_8021AB21 = -1;
            } else {
                D_hd_front_end_8021AB21 = D_hd_front_end_8021AB21 + 0xA;
            }
            D_hd_front_end_8021A918 -= 200.0f;
            if (D_hd_front_end_8021A918 <= 925.0) {
                D_hd_front_end_8021A924 = 1;
                D_hd_front_end_8021A918 = 925.0f;
                D_hd_front_end_8021AB24 = D_hd_code_803156C4;
            }
            break;
        case 2:
            D_hd_front_end_8021A918 -= 50.0f;
            if ((D_hd_front_end_8021AB21 - 0x19) < 0) {
                D_hd_front_end_8021AB21 = 0;
            } else {
                D_hd_front_end_8021AB21 = D_hd_front_end_8021AB21 - 0x19;
            }
            if ((D_hd_front_end_8021AB2C - 0x40) < 0) {
                D_hd_front_end_8021AB2C = 0;
            } else {
                D_hd_front_end_8021AB2C -= 0x40;
            }
            if (D_hd_front_end_8021A918 <= 300.0) {
                g_nextGameState = func_hd_code_80299FE8(g_currentLevel);
            }
            break;
        default:
            D_hd_front_end_8021AB21 = 0xFF;
            if (D_hd_front_end_8021A926 == 0) {
                if ((D_hd_front_end_8021AB2C + 0x20) >= 0x100) {
                    D_hd_front_end_8021AB2C = 0xFF;
                } else {
                    D_hd_front_end_8021AB2C += 0x20;
                }
            }
            break;
    }
    if (D_hd_front_end_80217B6C != 3) {
        D_hd_front_end_8021AB21 = 0;
    }
    sp46 = (s16) (s32) D_hd_front_end_8021A90C;
    sp44 = (s16) (s32) D_hd_front_end_8021A910;
    func_hd_front_end_801FD484(&D_hd_front_end_8021A920, &D_hd_front_end_8021A91C, &D_hd_front_end_8021A90C,
                               &D_hd_front_end_8021A910, &D_hd_front_end_8021A914, 925.0f);
    D_hd_front_end_8021A934 = D_hd_front_end_8021A90C - (f32) sp46;
    D_hd_front_end_8021A938 = D_hd_front_end_8021A910 - (f32) sp44;
    D_hd_front_end_8021AB60 = D_hd_front_end_8020D810[D_hd_front_end_8021A905].unk14;
    D_hd_front_end_8021AB64 = D_hd_front_end_8020D810[D_hd_front_end_8021A905].unk10;
    D_hd_front_end_8021A934 = (func_hd_front_end_801FD6B8(D_hd_front_end_8021AB60, D_hd_front_end_8021A91C, 180.0f) *
                               MAX(0x1F4U, (sp3C >> 2))) /
                              10000.0f;
    D_hd_front_end_8021A938 = (func_hd_front_end_801FD6B8(D_hd_front_end_8021AB64, D_hd_front_end_8021A920, 180.0f) *
                               MAX(0x1F4U, (sp3C >> 2))) /
                              10000.0f;
    D_hd_front_end_8021A91C -= D_hd_front_end_8021A934;
    D_hd_front_end_8021A920 -= D_hd_front_end_8021A938;
    D_hd_front_end_8021AB68 = func_hd_front_end_801F9258(sp38->dp, sp38, &D_hd_code_80358078);
    D_hd_front_end_8021AB6C = func_hd_front_end_801F9820(D_hd_front_end_8021AB68, sp38, &D_hd_front_end_8021AB58);
    func_hd_front_end_801F9B84(D_hd_front_end_8021AB6C, sp38, &D_hd_front_end_8021AB5C);
    func_hd_front_end_801FD748();

    for (sp40 = 0; sp40 < nextdma; sp40++) {
        osRecvMesg(&D_hd_code_80315180, NULL, 1);
    }
    for (sp40 = 0; sp40 < (nextdma - no_palette_dmas); sp40++) {
        func_hd_code_802A57AC();
    }
}
