#include "patches.h"
#include "misc_funcs.h"
#include "sound.h"
#include <PR/sched.h>
#include <hd_code/macros.h>
#include <hd_code/functions.h>
#include <hd_code/variables.h>
#include <hd_code/music.h>
#include <hd_code/snd.h>
#include <hd_code/hd.h>

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

void amClearDmaBuffers(void);
extern s32 g_FrameSize;
extern u32 g_MinFrameSize;
extern AudioManager g_AudioManager;
extern u32 g_CurrentAcmdList;
extern s32 g_CommandLength;
extern OSScClient g_AudioClient;

ALSoundState* sndSetupSound(ALBank* soundBank, ALSound* sound);
extern ALSndPlayer* g_sndPlayerPtr;

extern s16 D_hd_code_802E8D00[66];
extern u8 D_hd_code_802E8D84;
extern f32 D_hd_code_802E8D88;
extern s32 D_hd_code_80367408[0x42];
extern void* D_hd_code_80367510;
extern ALSeqFile* D_hd_code_80367514;
extern ALCSeq D_hd_code_80367518[2];
extern u8 D_hd_code_80367708;
extern f32 D_hd_code_8036770C;

// @recomp: base volume of the current tune
static f32 music_tune_volume(void) {
    return D_hd_code_802E8D00[D_hd_code_80367708] * D_hd_code_8036770C;
}

RECOMP_PATCH void musicPlayTune(u8 sequenceId, f32 sequenceVolume) {
    u8* sp24;

    D_hd_code_802E8D84 ^= 1;
    alCSPStop(g_musicPlayer);

    D_hd_code_8036772A = 0;
    D_hd_code_8036770C = sequenceVolume;
    D_hd_code_80367708 = sequenceId;

    sp24 = D_hd_code_80367514->seqArray[sequenceId].offset;

    INITIATE_DMA(sp24, D_hd_code_80367510, &D_hd_code_80367408[sequenceId], 0U, 0, 0);
    alCSeqNew(&D_hd_code_80367518[D_hd_code_802E8D84], D_hd_code_80367510);
    alCSPSetSeq(g_musicPlayer, &D_hd_code_80367518[D_hd_code_802E8D84]);
    alCSPPlay(g_musicPlayer);

    // @recomp: apply bgm volume
    alCSPSetVol(g_musicPlayer, music_tune_volume() * D_hd_code_802E8D88 * recomp_get_bgm_volume());
}

RECOMP_PATCH void musicSetMasterVolume(f32 volume) {
    D_hd_code_802E8D88 = volume;

    // @recomp: apply bgm volume
    alCSPSetVol(g_musicPlayer, (s32) (music_tune_volume() * volume * recomp_get_bgm_volume()));
}

RECOMP_PATCH void func_hd_code_802613C8(void) {
    f32 sp2C;
    f32 sp28;
    s16 sp26;

    sp2C = alCSPGetVol(g_musicPlayer);

    // @recomp: apply bgm volume
    sp28 = D_hd_code_802E8D00[D_hd_code_80367708] * D_hd_code_802E8D88 * recomp_get_bgm_volume();

    sp26 = sp2C + (sp28 * D_hd_code_8036770C - sp2C) * 0.075;
    if ((ABS(sp26 - sp28 * D_hd_code_8036770C)) < 10.0f) {
        sp26 = sp28 * D_hd_code_8036770C;
        D_hd_code_8036772A = 0;
    }
    alCSPSetVol(g_musicPlayer, (s32) sp26);
}

// @recomp: queries for updated bgm volume
static void config_update_bgm_volume(void) {
    static f32 last_bgm_volume = -1.0f;
    f32 bgm_volume = recomp_get_bgm_volume();

    if (bgm_volume == last_bgm_volume) {
        return;
    }

    if (last_bgm_volume >= 0.0f && g_musicPlayer != NULL) {
        // volume changed, activate fade mode
        D_hd_code_8036772A = 1;
    }
    last_bgm_volume = bgm_volume;
}

// @recomp: patch static volume with configurable sfx volume
RECOMP_PATCH ALSoundState* sndPlaySfx(ALBank* soundBank, s16 sndId, ALSoundState* pendingState) {
    ALSoundState* newState;
    ALSoundState* nextState;
    ALKeyMap* keyMap;
    ALSound* sound;
    s16 eventSndId;
    ALMicroTime playSfxDelta;
    ALMicroTime deltaLoop;
    ALMicroTime deltaTotal;

    nextState = NULL;
    eventSndId = 0;
    deltaTotal = 0;

    if (sndId == 0 || (g_frameCount == 0 && sndId == 0xC) ||
        ((sndId == 0x14 || sndId == 0x15) && (g_currentLevel == 0x26 || g_currentLevel == 0x31))) {
        return 0;
    }

    do {
        sound = soundBank->instArray[0]->soundArray[sndId - 1];

        newState = sndSetupSound(soundBank, sound);

        if (newState != NULL) {
            ALSndpEvent playEvent;

            // @recomp: scale the initial volume by sfx volume
            newState->vol = (u16) (newState->vol * recomp_get_sfx_volume());

            g_sndPlayerPtr->target = (s32) newState;
            playEvent.common.type = AL_SNDP_PLAY_EVT;
            playEvent.common.state = newState;
            deltaLoop = sound->keyMap->velocityMax * DELTA_33_MS;

            if (newState->unk3e & 0x10) {
                newState->unk3e &= ~(s16) 0x10;
                alEvtqPostEvent(&g_sndPlayerPtr->evtq, (ALEvent*) &playEvent, deltaTotal + 1);
                playSfxDelta = deltaLoop + 1;
                eventSndId = sndId;
            } else {
                alEvtqPostEvent(&g_sndPlayerPtr->evtq, (ALEvent*) &playEvent, deltaLoop + 1);
            }

            nextState = newState;
        } else {
            rmonPrintf("Sound state allocate failed - sndId %d\n", sndId);
        }

        deltaTotal += deltaLoop;

        keyMap = sound->keyMap;
        sndId = (s16) ((s32) keyMap->velocityMin + (s32) (keyMap->keyMin & 0xC0) * 4);
    } while (sndId != 0 && newState != NULL);

    if (nextState != NULL) {
        nextState->unk3e |= 0x1;
        nextState->state = pendingState;

        if (eventSndId != 0) {
            ALSndpEvent playSfxEvent;

            nextState->unk3e |= 0x10;

            playSfxEvent.playSfx.type = AL_SNDP_PLAY_SFX_EVT;
            playSfxEvent.playSfx.state = nextState;
            playSfxEvent.playSfx.soundIndex = eventSndId;
            playSfxEvent.playSfx.soundBank = soundBank;

            alEvtqPostEvent(&g_sndPlayerPtr->evtq, (ALEvent*) &playSfxEvent, playSfxDelta);
        }
    }

    if (pendingState != NULL) {
        pendingState->link.next = (void*) nextState;
    }

    return nextState;
}

#define MAX_ACMD_SIZE 2750
#define EXTRA_SAMPLES 0x25
RECOMP_PATCH void amHandleFrameMessage(AudioInfo* info, AudioInfo* lastInfo) {
    s16* outBuffer;
    Acmd* cmdlp;
    u32 sp2C;
    OSScTask* task;

    // @recomp: apply bgm changes
    config_update_bgm_volume();

    sp2C = 0;
    amClearDmaBuffers();
    outBuffer = (s16*) osVirtualToPhysical(info->data);
    if (lastInfo != NULL) {
        osAiSetNextBuffer(lastInfo->data, lastInfo->frameSamples * 4);
    }
    sp2C = osAiGetLength() >> 2;

    // @recomp: osAiGetLength() could be larger than the frame size, then neededSamples get below zero, which caused
    // issues with unsigned casting
    // TODO: better skip when neededSamples is below zero?
    s32 neededSamples = g_FrameSize - sp2C;
    if (neededSamples < 1)
        neededSamples = 1;
    info->frameSamples = ((neededSamples) + 0x10 + EXTRA_SAMPLES) & ~0xF;
    if ((u32) info->frameSamples < (u32) g_MinFrameSize) {
        info->frameSamples = (s16) g_MinFrameSize;
    }
    cmdlp = alAudioFrame(g_AudioManager.cmdList[g_CurrentAcmdList], &g_CommandLength, outBuffer, info->frameSamples);
    if (g_CommandLength > MAX_ACMD_SIZE) {
        rmonPrintf(ASSERT_MESSAGE, "cmdLen <= MAX_RSP_CMDS", "audio.c", 0x150);
    }
    task = &info->task;
    info->task.next = NULL;
    task->msgQ = &g_AudioManager.replyMessageQueue;
    task->msg = (OSMesg) info;
    task->flags = OS_SC_NEEDS_RDP;
    task->client = &g_AudioClient;
    task->list.t.data_ptr = (u64*) g_AudioManager.cmdList[g_CurrentAcmdList];
    task->list.t.data_size = ((s32) ((s32) cmdlp - (s32) g_AudioManager.cmdList[g_CurrentAcmdList]) >> 3) * 8;
    task->list.t.type = M_AUDTASK;
    task->list.t.ucode_boot = (u64*) rspbootTextStart;
    task->list.t.ucode_boot_size = (s32) aspMainTextStart - (s32) rspbootTextStart;
    task->list.t.flags = 0;
    task->list.t.ucode = (u64*) aspMainTextStart;
    task->list.t.ucode_data = (u64*) aspMainDataStart;
    task->list.t.ucode_data_size = 0x800;
    task->list.t.yield_data_ptr = NULL;
    task->list.t.yield_data_size = 0;

    if (osSendMesg(&sc.cmdQ, (OSMesg) task, OS_MESG_NOBLOCK) == -1) {
        rmonPrintf(ASSERT_MESSAGE, "osSendMesg(osScGetCmdQ(&sc), (OSMesg) t, OS_MESG_NOBLOCK)!=-1", "audio.c", 0x169);
    }
    g_CurrentAcmdList ^= 1;
}
