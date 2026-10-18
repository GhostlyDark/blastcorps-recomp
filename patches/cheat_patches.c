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

s32 func_hd_front_end_801F75A4(u8*, s32);  /* extern */
s32 func_hd_front_end_801F76E4(u8*, s32);  /* extern */
s32 func_hd_front_end_801F6BD0(u8, u64*);  /* extern */

extern s32 D_hd_code_802FA264;
extern OSPfs D_hd_code_8039B630;
extern struct S_8039B698 D_hd_code_8039B698;
extern u8 D_hd_front_end_8020BEE0[256];
extern u8 D_hd_front_end_8020C000[0x14];
extern u8 D_hd_front_end_8020C014[8];
extern u8 D_hd_code_80365060[];

RECOMP_PATCH s32 func_hd_front_end_801F6264(u8 arg0, u8 write) {
    s32 sp3C = 0;
    s32 pad38;
    u32 sp34;
    s32 pad30;
    s32 sp2C;
    UnknownData8024C414* sp28;
    u64 sp20;

    sp28 = &players[arg0];

    for (sp34 = 0; (sp34 < 0x100) && ((u8) write == 1); sp34++) {
        rmonPrintf("0x%x, ", ((u8*) sp28)[sp34]);
    }
    rmonPrintf("\n");

    if (0) {
        rmonPrintf(ASSERT_MESSAGE, "sizeof(playerInfo)<=512", "pfsHandler.c", 0x123);
    }

    if (write == 1) {
        func_hd_front_end_801F75A4(sp28, 0x100);
    }

    if ((D_hd_code_802E8BF8 != 0) || (g_currentGameState == 0x0040000000000000)) {
        if ((D_hd_code_8039C4B4 == 0) || D_hd_code_802FA264) {
            for (sp34 = 0; sp34 < 0x100; sp34++) {
                if ((u8) write == 1) {
                    D_hd_code_8039B698.data[sp34] = ((u8*) sp28)[sp34];
                    D_hd_front_end_8020BEE0[sp34] = ((u8*) sp28)[sp34];
                } else {
                    D_hd_code_8039B698.data[sp34] = D_hd_front_end_8020BEE0[sp34];
                    ((u8*) sp28)[sp34] = D_hd_code_8039B698.data[sp34];
                }
            }
        } else if ((u8) write == 1) {
            osEepromLongWrite(&D_hd_code_80370BF8, 0U, (u8*) sp28, 0x100);
        } else {
            osEepromLongRead(&D_hd_code_80370BF8, 0U, (u8*) sp28, 0x100);
        }
    } else {
        sp2C = 0;
        do {
            sp3C = osPfsFindFile(&D_hd_code_8039B630, 0x3031U, 0x4E424345U, D_hd_front_end_8020C000,
                                 D_hd_front_end_8020C014, &D_hd_code_8039B698.fileNo[arg0]);
            sp2C++;
        } while ((sp3C != 0) && (sp2C < 3));

        if (sp3C == 0) {
            sp2C = 0;
            do {
                sp3C = osPfsReadWriteFile(&D_hd_code_8039B630, D_hd_code_8039B698.fileNo[arg0], write, 0, 0x100,
                                          (u8*) sp28);
                sp2C++;
            } while ((sp3C != 0) && (sp2C < 3));
        }
    }

    if ((sp3C == 0) && ((u8) write == 0)) {
        sp3C = func_hd_front_end_801F76E4(sp28, 0x100);
    }

    if ((sp3C == 0) && ((u8) write == 0) && ((func_hd_front_end_801F6BD0(arg0, &sp20), (sp20 != 0x87569AB6CD076AEC)))) {
        sp3C = 0x6E382;
    }

    #if UNLOCK_ALL_LEVELS
    s32 i;
    char* name = (char*) &sp28->unk0;
    *(name++) = 'M';
    *(name++) = 'A';
    *(name++) = 'S';
    *(name++) = 'T';
    *(name++) = 'E';
    *(name++) = 'R';
    *(name++) = 0;
    
    sp28->unk91 = 13; // Highest carreer level

    for (i = 0; i < LEVEL_MAX; i++) {
        sp28->unk18[i] = 4; // Medal (1 - bronze, 2 - silver, 3 - gold, 4 - platinum?)
        sp28->unk54[i] = 0xFF; // Line connections?
    }
    // mark slot as initialized properly
    D_hd_code_80365060[arg0] = 1;

    sp3C = 0;
    #endif


    return sp3C;
}

extern u16 D_hd_code_80364F70[];
extern u16 D_hd_code_80364EF0[][0x10];
RECOMP_PATCH s32 func_hd_front_end_801F67E4(u8 pn, u8 level, u8 arg2) {
    s32 sp3C = 0;
    s32 sp38;
    u8 sp37;
    u16* sp30;

    sp37 = level * 2;
    sp30 = &D_hd_code_80364F70[sp37 & ~3];

    if (D_hd_code_802E8BF8 != 0) {
        if (!(pn == D_hd_code_80364AEA)) {
            rmonPrintf(ASSERT_MESSAGE, "pn==playerNumberAtStart", "pfsHandler.c", 0x25C);
        }
        if ((u8) arg2 == 1) {
            D_hd_code_80364F70[sp37] = D_hd_code_80364EF0[pn][D_hd_code_802E8C44[players[pn].unk92[level]]];
            D_hd_code_80364F70[sp37 + 1] = D_hd_code_80364F70[sp37] ^ 0x55AA;
            rmonPrintf("%d %d EEWRITE %x %x\n", level, D_hd_code_80364F70[sp37], (u32) ((sp37 * 2) + 0x100) >> 3, sp30);
            osEepromWrite(&D_hd_code_80370BF8, (u8) ((u32) ((sp37 * 2) + 0x100) >> 3), (u8*) sp30);
        } else {
            osEepromRead(&D_hd_code_80370BF8, (u8) ((u32) ((sp37 * 2) + 0x100) >> 3), (u8*) sp30);
            for (sp38 = 0; sp38 < 2; sp38++, level++) {
                if (((players[pn].unk18[level] > 0) && (players[pn].unk18[level] < 6) ? 1 : 0) != 0 &&
                    (level != 0x31) && (level != 0x2F) && (level != 0x26)) {
                    D_hd_code_80364EF0[pn][D_hd_code_802E8C44[players[pn].unk92[level]]] =
                        D_hd_code_80364F70[level * 2];
                    rmonPrintf("%d EETIMES: %d %d\n", level, D_hd_code_80364F70[level * 2],
                               D_hd_code_80364F70[level * 2 + 1] ^ 0x55AA);
                    if (D_hd_code_80364F70[level * 2] != (D_hd_code_80364F70[level * 2 + 1] ^ 0x55AA)) {
                        sp3C = 0x6E382;
                    }
                }
            }
        }
    }
#if UNLOCK_ALL_LEVELS
    sp3C = 0;
#endif
    return sp3C;
}