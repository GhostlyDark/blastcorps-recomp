#pragma once

#define TAG_MISSILE_ARROW 0x100
#define TAG_FADE_SCREEN 0x200
#define TAG_SKY_QUAD 0x300
#define TAG_ARROWS 0x400
#define TAG_PILLARBOX_START 0x400
#define TAG_PILLARBOX_END 0x40F
#define TAG_PILLARBOX_PROJECTION 0x410

#define TAG_VEHICLE(addr, arg1) ((u32)(addr) | (arg1) << 24)