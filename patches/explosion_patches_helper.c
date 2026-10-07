#include "patches.h"
#include "misc_funcs.h"
#include "gfx_tagging.h"
#include <PR/sched.h>
#include <hd_code/macros.h>
#include <hd_code/functions.h>
#include <hd_code/variables.h>

Gfx* explosion_begin(Gfx* entry, s32 slotIndex) {
    gEXMatrixGroupDecomposedNormal(entry++, TAG_EXPLOSION(slotIndex), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    return entry;
}

Gfx* explosion_end(Gfx* entry) {
    gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);
    return entry;
}
