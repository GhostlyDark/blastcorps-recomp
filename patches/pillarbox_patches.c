#include "patches.h"
#include "misc_funcs.h"
#include "gfx_tagging.h"
#include <PR/sched.h>
#include <hd_code/macros.h>
#include <hd_code/functions.h>
#include <hd_code/variables.h>

#define DEFAULT_FRAMEBUFFER_WIDTH 320
#define DEFAULT_FRAMEBUFFER_HEIGHT 240

extern u8 D_hd_code_8035805C;
extern u64 g_currentGameState;
static Vtx sVertices[2][200];
static Mtx sMatrices[2][50];

float identity_matrix[4][4] = {
    { 1.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }
};

// TODO: put them into the decomp, then use it here
#define GS_ATTRACT_TEAM_MEMBERS 0x1000000000000ULL // shows all characters
#define GS_PRELEVEL_STATS 0x80ULL
#define GS_PROMOTION_SCREEN 0x40000000ULL
#define GS_POSTLEVEL_STATS 0x8000000ULL
#define GS_VEHICLE_SELECTION 0x20000000ULL

#define NEEDS_PILLARBOX \
    (GS_PRELEVEL_STATS | GS_POSTLEVEL_STATS | GS_ATTRACT_TEAM_MEMBERS | GS_VEHICLE_SELECTION | GS_PROMOTION_SCREEN)

void pillarbox_draw(Gfx** gfx) {
    if (!(g_currentGameState & NEEDS_PILLARBOX)) {
        // This is only activated for the stats screen
        return;
    }

    Vtx* currentVertex = &sVertices[D_hd_code_8035805C][0]; 
    Vtx** vtx = &currentVertex;

    Mtx* currentMatrix = &sMatrices[D_hd_code_8035805C][0];
    Mtx** mtx = &currentMatrix;

    float pillarbox_scale = 1.0f;
    float original_aspect = (float) DEFAULT_FRAMEBUFFER_WIDTH / DEFAULT_FRAMEBUFFER_HEIGHT;
    float target_aspect = recomp_get_target_aspect_ratio(original_aspect);
    float target_width = DEFAULT_FRAMEBUFFER_WIDTH;
    float wide_width = DEFAULT_FRAMEBUFFER_HEIGHT * target_aspect;
    float pillar_width = wide_width - target_width * pillarbox_scale;
    if (pillar_width >= 0.0f) {
        gEXEnable((*gfx)++);
        // @recomp Use a high precision version of the projection matrix instead of the fixed precision one. This fixes
        // accuracy issues when rendering text using the orthographic projection.
        guOrthoF(*mtx, -(2 * (f32) DEFAULT_FRAMEBUFFER_WIDTH), (2 * (f32) DEFAULT_FRAMEBUFFER_WIDTH),
                 -(2 * (f32) DEFAULT_FRAMEBUFFER_HEIGHT), (2 * (f32) DEFAULT_FRAMEBUFFER_HEIGHT), -256.0f, 256.0f,
                 1.0f);
        gEXMatrixFloat((*gfx)++, OS_K0_TO_PHYSICAL((*mtx)++), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

        guTranslate(*mtx, 0.0f, 0.0f, 0.0f);
        gSPMatrix((*gfx)++, OS_K0_TO_PHYSICAL((*mtx)++), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        // @recomp Set an identity view matrix.
        gEXSetViewMatrixFloat((*gfx)++, identity_matrix);
        gEXMatrixGroupSimpleNormal((*gfx)++, TAG_PILLARBOX_PROJECTION, G_EX_PUSH, G_MTX_PROJECTION, G_EX_EDIT_NONE);

        Vtx* verts = *vtx;
        for (s32 ix = -1; ix < 2; ix += 2) {
            for (s32 iy = 0; iy < 2; iy++) {
                (*vtx)->v.ob[0] = ix * pillar_width * 2;
                (*vtx)->v.ob[1] = iy * DEFAULT_FRAMEBUFFER_HEIGHT * 4 - DEFAULT_FRAMEBUFFER_HEIGHT * 2;
                (*vtx)->v.ob[2] = 1;
                (*vtx)++;
            }
        }

        gEXPushOtherMode((*gfx)++);
        gEXPushCombineMode((*gfx)++);
        gEXPushGeometryMode((*gfx)++);
        gSPClearGeometryMode((*gfx)++, G_CULL_BOTH);
        gDPSetRenderMode((*gfx)++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCycleType((*gfx)++, G_CYC_1CYCLE);
        gDPSetCombineLERP((*gfx)++, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

        u32 id = TAG_PILLARBOX_START;
        for (s32 ix = -1; ix < 2; ix += 2) {
            guTranslate(*mtx, ix * wide_width * 2, 0.0f, 0.0f);
            gSPMatrix((*gfx)++, OS_K0_TO_PHYSICAL(*mtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            (*mtx)++;

            gEXMatrixGroupSimpleVerts((*gfx)++, id++, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
            gSPVertex((*gfx)++, verts, 4, 0);
            gSP1Triangle((*gfx)++, 0, 1, 3, 0);
            gSP1Triangle((*gfx)++, 0, 2, 3, 0);

            gEXPopMatrixGroup((*gfx)++, G_MTX_MODELVIEW);
        }

        gEXPopOtherMode((*gfx)++);
        gEXPopCombineMode((*gfx)++);
        gEXPopGeometryMode((*gfx)++);
        gEXPopMatrixGroup((*gfx)++, G_MTX_PROJECTION);
    }
}