#include "patches.h"
#include "misc_funcs.h"
#include "gfx.h"
#include "config.h"
#include <PR/sched.h>
#include <hd_code/macros.h>
#include <hd_code/functions.h>
#include <hd_code/variables.h>

#define VIDEO_MSG 0
#define RSP_DONE_MSG 1
#define RDP_DONE_MSG 2
#define PRE_NMI_MSG 3
#define RSP_STATE_SUSPENDED 3

#define EXEC_IS_AUDIO 0
#define EXEC_IS_GFX 1

void __scHandleGfxTask(OSSched*, void*);
void __scExecAudioIfIdle(OSSched*);
void __scExec(OSSched*, s32); /* extern */
void __scRspDone(OSSched*);
void __scRdpDone(OSSched*);
void __scAppendList(OSSched*, OSScTask*); /* extern */
s32 __scSendMesg(OSMesgQueue* messageQueue, OSMesg message, s32 flags); /* extern */

extern s32 D_hd_code_8036BF10;
extern u32 D_hd_code_8036BFB8;
extern OSTime D_hd_code_8036BEF8;
extern OSTime D_hd_code_8036BF00;
extern s32 D_hd_code_8036BF08;
extern s32 D_hd_code_8036BF0C;
extern OSTimer D_hd_code_8036BF78;
extern s32 g_nextRetrace;
extern s32 D_hd_code_8036BF18;
extern OSScTask* g_currentRdpTask;
extern u8 g_gfxTaskSlotUsed[5];

typedef struct AudioInfo_s {
    s16* data;
    s16 frameSamples;
    OSScTask task;
} AudioInfo;

typedef struct AudioManager_s {
    void* cmdList[2];  // @recomp: stubbed
    AudioInfo* audioInfo[3];
    u32 numberOutputBuffers;
    u8 audioThread[0x1C8 - 0x18]; // @recomp: stubbed
    OSMesgQueue frameMessageQueue;
    OSMesg frameMessageBuffer[8];
    OSMesgQueue replyMessageQueue;
    // @recomp: rest of the fields not included here
} AudioManager;

extern AudioManager g_AudioManager;

RECOMP_PATCH void __scRetraceDone(OSSched* scheduler) {
    OSScTask* rspTask;
    OSScClient* client;
    s32 sp3C;
    s32 sp38;

    scheduler->retraceCount++;
    if (D_hd_code_802E8BD0 == 0) {
        scheduler->unk803156C0++;
    }

    D_hd_code_8036BF38 = osGetTime();
    if (g_currentRdpTask != NULL) {
        osViSwapBuffer(g_currentRdpTask->framebuffer);
        D_hd_code_8036BF18 = g_nextRetrace;
        g_nextRetrace = scheduler->retraceCount + 1;
        osDpSetStatus(DPC_SET_FREEZE);
        if (g_currentRdpTask->msgQ != NULL) {
            __scSendMesg(g_currentRdpTask->msgQ, g_currentRdpTask->msg, OS_MESG_NOBLOCK);
        }
        g_currentRdpTask = NULL;
    } else {
        if (osViGetCurrentFramebuffer() == osViGetNextFramebuffer() && (osDpGetStatus() & 2)) {
            scheduler->unk803156C8 = osGetTime();
            osDpSetStatus(DPC_CLR_FREEZE);
        }
    }


    for (sp38 = scheduler->cmdQ.validCount, sp3C = 0; sp3C < sp38; sp3C++) {
        if (osRecvMesg(&scheduler->cmdQ, (OSMesg*) &rspTask, OS_MESG_NOBLOCK) == -1) {
            rmonPrintf(ASSERT_MESSAGE, "osRecvMesg(&sc->cmdQ, (OSMesg *)&rspTask, OS_MESG_NOBLOCK) != -1", "sched.c",
                       0x1BD);
        }

        // @recomp: Ignore empty audio tasks and answer them directly as completed. Potentially not needed anymore. TODO: Need to recheck
        if (rspTask->list.t.data_size == 0) {
            rmonPrintf("WARNING: empty audio task received!\n");
            osSendMesg(rspTask->msgQ, rspTask->msg, OS_MESG_NOBLOCK);
            continue;
        }

        // @recomp: append audio task to the audio task list
        __scAppendList(scheduler, rspTask);
    }

    for(client = scheduler->clientList; client != NULL; client = client->next) {
        if (client->unkC == 3) {
            // This is needed for unblocking hd_front_end pak things
            osSendMesg(client->msgQ, (void* )0x29A, OS_MESG_NOBLOCK);
        }
    }
}

static s32 sPendingDpDone = 0;
static s32 sAudioFrame = 0;

RECOMP_PATCH void __scMain(void* params) {
    OSMesg msg;
    OSSched* scheduler;
    OSScClient* client;

    scheduler = (OSSched*)params;
    while (1) {
        osRecvMesg(&scheduler->interruptQ, &msg, OS_MESG_BLOCK);

        switch ((s32) msg - 0x29A) {
            case VIDEO_MSG:
                D_hd_code_8036BFB8++;
                if ((D_hd_code_8036BFB8 % 480U) == 0) {
                    D_hd_code_8036BEF8 = D_hd_code_8036BF00;
                    D_hd_code_8036BF08 = D_hd_code_8036BF0C;
                }
                // @recomp: retrace first so it can clear the freeze, then handle the held back RDP done
                __scRetraceDone(scheduler);
                if (sPendingDpDone && !(osDpGetStatus() & DPC_STATUS_FREEZE)) {
                    sPendingDpDone = 0;
                    __scRdpDone(scheduler);
                }

                // @recomp: For every second retrace we tell the audio thread to prepare an audio task
                if (sAudioFrame % 2 == 0) {
                    osSendMesg(&g_AudioManager.frameMessageQueue, (OSMesg) 5, OS_MESG_NOBLOCK);

                }
                sAudioFrame++;

                // @recomp: If there is audio pending, run it. curRSP should be null
                if (scheduler->audioListHead != NULL) {
                    __scExecAudioIfIdle(scheduler);
                }

                break;
            case 4:
                // __scExecAudioIfIdle(scheduler);
                break;
            case RSP_DONE_MSG:
                __scRspDone(scheduler);
                break;
            case RDP_DONE_MSG:
                // @recomp: the runtime doesn't honor DPC_SET_FREEZE, hold the RDP done while frozen
                if (osDpGetStatus() & DPC_STATUS_FREEZE) {
                    sPendingDpDone = 1;
                } else {
                    __scRdpDone(scheduler);
                }
                break;
            case 5:
                osSendMesg(D_hd_code_8036BF78.mq, D_hd_code_8036BF78.msg, 1);
                break;
            case PRE_NMI_MSG:
                for (client = scheduler->clientList; client != NULL; client = client->next) {
                    osSendMesg(client->msgQ, (void*) 0x29D, 0);
                }
                D_hd_code_8036BF10 = 1;
                osViBlack(TRUE);
                rmonPrintf("%x\n", osDpGetStatus());
                osDpSetStatus(DPC_CLR_FREEZE);
                rmonPrintf("%x\n", osDpGetStatus());
                while (1)
                    ;
            case 6:
                rmonPrintf(" *** CPU FAULT *** - UNFREEZING RDP?\n");
                while (osViGetCurrentFramebuffer() != osViGetNextFramebuffer()) {}
                osDpSetStatus(DPC_CLR_FREEZE);
                while (1)
                    ;
            default:
                __scHandleGfxTask(scheduler, (OSScTask*) msg);
                break;
        }
    }
}

// @recomp: no yielding here. Perhaps not needed anymore
RECOMP_PATCH void __scExecAudioIfIdle(OSSched* scheduler) {
    if (scheduler->curRSPTask == NULL) {
        D_hd_code_8036BF00 = 0;
        __scExec(scheduler, EXEC_IS_AUDIO);
    }
}

extern OSScTask g_gfxTasks[5][2];
extern void* g_gfxMicrocodeText[6];
extern void* g_gfxMicrocodeData[5];
extern u64 D_hd_code_80367750;
extern u64 D_hd_code_8036AFB0;

RECOMP_PATCH void gfxSubmitTask(Gfx* displayList, s32 displayListEntries, u8 arg2, s32 arg3, s32 gfxTaskId, s32 arg5) {
    // @recomp: if displayList contains special marker word, use extended dl instead
    if (displayList->words.w0 == 0xCAFEBABE) {
        displayList->words.w0 = 0x0;

        displayList = sExtendedGfx[D_hd_code_8035805C];
    }
        

    OSScTask* gfxTask;
    s32 displayListSize;

    displayListSize = displayListEntries * sizeof(Gfx);
    gfxTask = &g_gfxTasks[arg2][D_hd_code_8035805C];
    g_gfxTaskSlotUsed[(u8) arg2] = 1;
    gfxTask->list.t.type = M_GFXTASK;
    if ((u8) arg2 == 4) {
        gfxTask->list.t.flags = OS_TASK_DP_WAIT;
    } else {
        gfxTask->list.t.flags = 0;
    }
    gfxTask->list.t.ucode_boot = (u64*) rspbootTextStart;
    gfxTask->list.t.ucode_boot_size = (u32) aspMainTextStart - (u32) rspbootTextStart;
    gfxTask->list.t.ucode = (u64*) g_gfxMicrocodeText[(u8) arg2];
    gfxTask->list.t.ucode_data = (u64*) g_gfxMicrocodeData[(u8) arg2];
    gfxTask->list.t.ucode_size = 0x1000;
    gfxTask->list.t.ucode_data_size = 0x800;
    gfxTask->list.t.dram_stack = &D_hd_code_80367750;
    gfxTask->list.t.dram_stack_size = 0x400;
    gfxTask->list.t.output_buff = (u64*) g_gfxTaskOutputBuffer;
    gfxTask->list.t.output_buff_size = (u64*) (g_gfxTaskOutputBuffer + 0x1400);
    gfxTask->list.t.data_ptr = (u64*) displayList;
    gfxTask->list.t.data_size = (u32) displayListSize;
    gfxTask->list.t.yield_data_ptr = &D_hd_code_8036AFB0;
    gfxTask->list.t.yield_data_size = 0x900;
    gfxTask->next = NULL;
    gfxTask->msgQ = &D_hd_code_803153D8;
    gfxTask->msg = (OSMesg) ((arg2 << 0x10) | gfxTaskId);
    gfxTask->flags = OS_SC_NEEDS_RDP | OS_SC_NEEDS_RSP;
    if ((u8) arg3 != 0) {
        gfxTask->flags |= OS_SC_SWAPBUFFER;
    }
    gfxTask->framebuffer = D_80000400[D_hd_code_8035805C];
    gfxTask->client = &g_gfxClient;

    // @recomp: removed osWritebackDCache calls
    osSendMesg(&sc.interruptQ, gfxTask, OS_MESG_BLOCK);
}

// Do not remove this. N64Recomp needs at least one global variable that is exposed in the .data
// section. else it will crash
int sJumptableFix = 1;
