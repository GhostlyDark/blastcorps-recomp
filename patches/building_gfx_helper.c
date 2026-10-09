#include "patches.h"
#include "misc_funcs.h"
#include "gfx_tagging.h"
#include <PR/sched.h>
#include <hd_code/macros.h>
#include <hd_code/functions.h>
#include <hd_code/variables.h>

Gfx* building_part_begin(Gfx* entry, s32 id, s32 reflect) {
    gEXMatrixGroupDecomposedNormal(entry++, TAG_BUILDING(id, reflect), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    return entry;
}

Gfx* building_part_end(Gfx* entry) {
    gEXPopMatrixGroup(entry++, G_MTX_MODELVIEW);
    return entry;
}
