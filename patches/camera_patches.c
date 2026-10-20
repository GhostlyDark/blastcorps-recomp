#include "patches.h"
#include "camera_patches.h"
#include <hd_code/macros.h>
#include <hd_code/functions.h>
#include <hd_code/variables.h>

// cam eye position (16.16 fixed point)
extern s32 D_hd_code_803643F8;
extern s32 D_hd_code_803643FC;
extern s32 D_hd_code_80364400;
// cam look at position
extern f32 D_hd_code_80365084;
extern f32 D_hd_code_80365088;
extern f32 D_hd_code_8036508C;

static f32 sPrevEye[3];
static f32 sPrevAt[3];
static f32 sEyeVelocity[3];
static f32 sAtVelocity[3];
static bool sCameraInterpolationSkipped = FALSE;

bool camera_should_interpolate(f32 eye[3], f32 at[3]) {
    f32 predicted_eye[3];
    f32 predicted_at[3];
    f32 diff[3];

    predicted_eye[0] = sPrevEye[0] + sEyeVelocity[0];
    predicted_eye[1] = sPrevEye[1] + sEyeVelocity[1];
    predicted_eye[2] = sPrevEye[2] + sEyeVelocity[2];
    predicted_at[0] = sPrevAt[0] + sAtVelocity[0];
    predicted_at[1] = sPrevAt[1] + sAtVelocity[1];
    predicted_at[2] = sPrevAt[2] + sAtVelocity[2];

    sEyeVelocity[0] = eye[0] - sPrevEye[0];
    sEyeVelocity[1] = eye[1] - sPrevEye[1];
    sEyeVelocity[2] = eye[2] - sPrevEye[2];
    sAtVelocity[0] = at[0] - sPrevAt[0];
    sAtVelocity[1] = at[1] - sPrevAt[1];
    sAtVelocity[2] = at[2] - sPrevAt[2];

    diff[0] = eye[0] - predicted_eye[0];
    diff[1] = eye[1] - predicted_eye[1];
    diff[2] = eye[2] - predicted_eye[2];
    f32 eye_dist = sqrtf(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);

    diff[0] = at[0] - predicted_at[0];
    diff[1] = at[1] - predicted_at[1];
    diff[2] = at[2] - predicted_at[2];
    f32 at_dist = sqrtf(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);

    diff[0] = sAtVelocity[0] - sEyeVelocity[0];
    diff[1] = sAtVelocity[1] - sEyeVelocity[1];
    diff[2] = sAtVelocity[2] - sEyeVelocity[2];
    f32 velocity_diff = sqrtf(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);

    sPrevEye[0] = eye[0];
    sPrevEye[1] = eye[1];
    sPrevEye[2] = eye[2];
    sPrevAt[0] = at[0];
    sPrevAt[1] = at[1];
    sPrevAt[2] = at[2];

    // TODO: from MK64 recomp, need tuning for Blast Corps
    if (velocity_diff <= 3.0f && eye_dist <= 100.0f && at_dist <= 100.0f) {
        return TRUE;
    }

    if (velocity_diff > 50.0f || at_dist > 50.0f || eye_dist > 300.0f) {
        sEyeVelocity[0] = sEyeVelocity[1] = sEyeVelocity[2] = 0.0f;
        sAtVelocity[0] = sAtVelocity[1] = sAtVelocity[2] = 0.0f;
        return FALSE;
    }

    return TRUE;
}

bool camera_update_interpolation(void) {
    f32 eye[3] = {
        D_hd_code_803643F8 / 65536.0f,
        D_hd_code_803643FC / 65536.0f,
        D_hd_code_80364400 / 65536.0f,
    };
    f32 at[3] = { D_hd_code_80365084, D_hd_code_80365088, D_hd_code_8036508C };

    sCameraInterpolationSkipped = !camera_should_interpolate(eye, at);
    return !sCameraInterpolationSkipped;
}

bool perspective_interpolation_skipped() {
    return sCameraInterpolationSkipped;
}
