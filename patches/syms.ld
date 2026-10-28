__start = 0x80000000;

/* Dummy addresses that get recompiled into function calls */
recomp_puts = 0x8F000000;
recomp_exit = 0x8F000004;
recomp_load_overlays = 0x8F000008;
osInvalDCache_recomp = 0x8F00000C;
osInvalICache_recomp = 0x8F000010;
osWritebackDCache_recomp = 0x8F000014;
osWritebackDCacheAll_recomp = 0x8F000018;
bzero_recomp = 0x8F00001C;
recomp_load_overlays_by_rom = 0x8F000020;
osSendMesg_recomp = 0x8F000024;
osGetCount_recomp = 0x8F000028;
osCreateMesgQueue_recomp = 0x8F00002C;
osRecvMesg_recomp = 0x8F000030;
osDpSetStatus_recomp = 0x8F000034;
recomp_error = 0x8F000060;
memcpy_recomp = 0x8F000064;
__divdi3_recomp = 0x8F000068;
__udivdi3_recomp = 0x8F00006C;
__umoddi3_recomp = 0x8F000070;
strlen_recomp = 0x8F000074;
osVirtualToPhysical_recomp = 0x8F000078;
osPiStartDma_recomp = 0x8F00007C;
recomp_abort = 0x8F000080;
recomp_get_target_aspect_ratio = 0x8F000084;
osExQueueDisplaylistEvent_recomp = 0x8F000088;

recomp_get_bgm_volume = 0x8F000090;
__sinf_recomp = 0x8F000094;
__cosf_recomp = 0x8F000098;
recomp_get_right_analog_inputs = 0x8F00009C;
recomp_get_analog_cam_enabled = 0x8F0000A0;
recomp_get_analog_inverted_axes = 0x8F0000A4;
recomp_set_right_analog_suppressed = 0x8F0000A8;
osContGetReadData_recomp = 0x8F0000AC;
bcopy_recomp = 0x8F0000B0;



recomp_get_analog_cam_sensitivity = 0x8F0000C4;
recomp_run_ui_callbacks = 0x8F0000C8;

osViGetCurrentFramebuffer_recomp = 0x8F0000D0;
recomp_set_multiplayer_max_two_players = 0x8F0000D4;
recomp_set_multiplayer_max_four_players = 0x8F0000D8;
osMotorInit_recomp = 0x8F0000DC;
osMotorStop_recomp = 0x8F0001E0;
osMotorStart_recomp = 0x8F0001E4;
osSetEventMesg_recomp = 0x8F0001E8;
osContStartQuery_recomp = 0x8F0001EC;
osContGetQuery_recomp = 0x8F0001F0;
osPfsIsPlug_recomp = 0x8F0001F4;
recomp_get_sfx_volume = 0x8F000200;
osCreatePiManager_recomp = 0x8F00020C;
osDpGetStatus_recomp = 0x8F000210;
boot_osPiRawStartDma = 0x8F000214;
osCreateThread_recomp = 0x8F000218;
osStartThread_recomp = 0x8F00021C;
osSetThreadPri_recomp = 0x8F000220;
osAiGetLength_recomp = 0x8F000224;
osAiSetNextBuffer_recomp = 0x8F000228;
yield_self = 0x8F00022C;
osViGetNextFramebuffer_recomp = 0x8F000230;
osViGetCurrentFramebuffer_recomp = 0x8F000234;
osViSwapBuffer_recomp = 0x8F000238;
osGetTime_recomp = 0x8F00023C;
osPfsFindFile_recomp = 0x8F000240;
osPfsReadWriteFile_recomp = 0x8F000244;
osEepromLongWrite_recomp = 0x8F000248;
osEepromLongRead_recomp = 0x8F00024C;
osEepromWrite_recomp = 0x8F000250;
osEepromRead_recomp = 0x8F000254;
recomp_start_rumble = 0x8F000258;
recomp_stop_rumble = 0x8F00025C;
