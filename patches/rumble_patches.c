#include "patches.h"
#include "misc_funcs.h"
#include "gfx_tagging.h"
#include "../lib/blastcorps/game/src/hd_code/hd.h"

#include <PR/sched.h>
#include <hd_code/macros.h>
#include <hd_code/functions.h>
#include <hd_code/variables.h>

extern u8 D_hd_code_802E8BE4;

#define IN_GAME_GAMESTATES (1 << 2)

RECOMP_PATCH void func_hd_code_8025B2B8(void) {

    // @recomp: enable rumble when shaky camera is active
    if (D_hd_code_802E8BE4 > 5 && g_currentGameState & IN_GAME_GAMESTATES) {
        recomp_start_rumble();
    } else {
        recomp_stop_rumble();
    }


    s32 i;
    for (i = 0; i < 0x50; i++) {
        if (D_hd_code_80365360[i] != 0) {
            D_hd_code_80365360[i]--;
        }
    }
}