#pragma once

#include "patches.h"

bool camera_should_interpolate(f32 eye[3], f32 at[3]);
bool camera_update_interpolation(void);
