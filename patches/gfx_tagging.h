#pragma once

#define TAG_MISSILE_ARROW 0x100
#define TAG_FADE_SCREEN 0x200
#define TAG_SKY_QUAD 0x300
#define TAG_ARROWS 0x400
#define TAG_PILLARBOX_START 0x500
#define TAG_PILLARBOX_END 0x50F
#define TAG_PILLARBOX_PROJECTION 0x510
#define TAG_PLANET_LINES 0x600
#define TAG_VEHICLE(slotid, arg1) (0x700 | (slotid) << 2 | (arg1))
#define TAG_SHADOW(id) (0x1000 | (id))
// 0x1100 is shadow projection
// 0x1200 is shadow modelview
// 0x1300 is free

