#include <stdlib.h>
#include <string.h>
#include "webrtc_vad.h"

#define SPEECH_LENGTH_BOUNDARY 50

static const int kInitCheck = 42;

WebRtcVad::WebRtcVad()
{
    m_pVadCore = new WebRtcVadCore();
}

WebRtcVad::~WebRtcVad()
{
    if(m_pVadCore) {
        delete m_pVadCore;
        m_pVadCore = NULL;
    }
}

WebRtc_Word16 WebRtcVad::WebRtcVad_get_version(char *version, size_t size_bytes)
{
    const char my_version[] = "VAD 1.2.0";
    if (version == NULL) {
        return -1;
    }

    if (size_bytes < sizeof(my_version)) {
        return -1;
    }

    memcpy(version, my_version, sizeof(my_version));
    return 0;
}

WebRtc_Word16 WebRtcVad::WebRtcVad_AssignSize(int *size_in_bytes)
{
    *size_in_bytes = sizeof(VadInstT) * 2 / sizeof(WebRtc_Word16);
    return 0;
}

WebRtc_Word16 WebRtcVad::WebRtcVad_Assign(VadInst **vad_inst, void *vad_inst_addr)
{
    if (vad_inst == NULL) {
        return -1;
    }

    if (vad_inst_addr != NULL) {
        *vad_inst = (VadInst*)vad_inst_addr;
        return 0;
    } else {
        return -1;
    }
}

WebRtc_Word16 WebRtcVad::WebRtcVad_Create(VadInst **vad_inst)
{
    VadInstT *vad_ptr = NULL;

    if (vad_inst == NULL) {
        return -1;
    }

    *vad_inst = NULL;

    vad_ptr = (VadInstT *)malloc(sizeof(VadInstT));
    *vad_inst = (VadInst *)vad_ptr;

    if (vad_ptr == NULL) {
        return -1;
    }

    vad_ptr->init_flag = 0;
    return 0;
}

WebRtc_Word16 WebRtcVad::WebRtcVad_Free(VadInst *vad_inst)
{
    if (vad_inst == NULL) {
        return -1;
    }

    free(vad_inst);
    return 0;
}

WebRtc_Word16 WebRtcVad::WebRtcVad_Init(VadInst *vad_inst)
{
    short mode = 0; // Default high quality

    if (vad_inst == NULL) {
        return -1;
    }

    return m_pVadCore->WebRtcVad_InitCore((VadInstT*)vad_inst, mode);
}

WebRtc_Word16 WebRtcVad::WebRtcVad_set_mode(VadInst *vad_inst, WebRtc_Word16 mode)
{
    VadInstT* vad_ptr;

    if (vad_inst == NULL) {
        return -1;
    }

    vad_ptr = (VadInstT*)vad_inst;
    if (vad_ptr->init_flag != kInitCheck) {
        return -1;
    }

    return m_pVadCore->WebRtcVad_set_mode_core((VadInstT*)vad_inst, mode);
}

WebRtc_Word16 WebRtcVad::WebRtcVad_Process(VadInst *vad_inst,
                                WebRtc_Word16 fs,
                                WebRtc_Word16 *speech_frame,
                                WebRtc_Word16 frame_length)
{
    WebRtc_Word16 vad;
    VadInstT* vad_ptr;

    if (vad_inst == NULL) {
        return -1;
    }

    vad_ptr = (VadInstT*)vad_inst;
    if (vad_ptr->init_flag != kInitCheck) {
        return -1;
    }

    if (speech_frame == NULL) {
        return -1;
    }

    if (fs == 32000) {
        if ((frame_length != 320) && (frame_length != 640) && (frame_length != 960)) {
            return -1;
        }
        vad = m_pVadCore->WebRtcVad_CalcVad32khz((VadInstT*)vad_inst, speech_frame, frame_length);

    } else if (fs == 16000) {
        if ((frame_length != 160) && (frame_length != 320) && (frame_length != 480)) {
            return -1;
        }
        vad = m_pVadCore->WebRtcVad_CalcVad16khz((VadInstT*)vad_inst, speech_frame, frame_length);

    } else if (fs == 8000) {
        if ((frame_length != 80) && (frame_length != 160) && (frame_length != 240)) {
            return -1;
        }
        vad = m_pVadCore->WebRtcVad_CalcVad8khz((VadInstT*)vad_inst, speech_frame, frame_length);
    } else {
        return -1; // Not a supported sampling frequency
    }

    if (vad > 0) {
        return 1;
    } else if (vad == 0) {
        return 0;
    } else {
        return -1;
    }
}

size_t WebRtcVad::GetPartWaveData(short* wav_data, int length, int start_index, short* part_data)
{
    size_t count = 0;
    if((length - start_index -1) >= 160)
        count = 160;
    else
        count = length - start_index - 1;

    for(int i=0; i<count; i++) {
        part_data[i] = wav_data[start_index + i];
    }
    return count;
}

int WebRtcVad::HumanSpeechRecognition(short* wav_data, int length)
{
    int nMode = 3;
    if(wav_data == NULL) {
        return -1;
    }

    VadInst* pVad = NULL;
    if(WebRtcVad_Create(&pVad)) {
        perror("WebRtcVad_Create failed!");
        return -1;
    }

    if(WebRtcVad_Init(pVad)) {
        perror("WebRtcVad_Init failed!");
        return -1;
    }

    if(WebRtcVad_set_mode(pVad, nMode)) {
        perror("WebRtcVad_set_mode failed!");
        return -1;
    }

    SpeechBuffer my_buffer;
    ClearSpeechBuffer(&my_buffer);

    int have_speech = 0;
    
    short shBufferIn[160] = {0};
    short* temp = wav_data;
    int start_index = 22;
    while(1) {
        if(160 != GetPartWaveData(temp, length, start_index, shBufferIn)){
            printf("reach the end of file.\n");
            if(my_buffer.speech_len >= MIN_SPEECH_LEN){
                //goto Save_Speech;
            }
            break;
        }

        start_index = start_index + 160;
        int nRet = WebRtcVad_Process(pVad, 16000, shBufferIn, 160);
        
        if(0 == nRet && 1 == QuerySpeechBufferIsEmpty(&my_buffer)){
            //printf("discard this frame.\n");
            continue;
        }
        DataInSpeechBuffer(&my_buffer, shBufferIn, nRet);
        
        if(my_buffer.speech_len > SPEECH_LENGTH_BOUNDARY){ 
            have_speech = 1;
            //break;
        }
    }
    return have_speech;
}

int WebRtcVad::ClearSpeechBuffer(SpeechBuffer *stBuffer){
    if(stBuffer){
        memset(stBuffer->buffer, 0, sizeof(short)*MAX_FRAMES_CNT_ONESPEECH*160);
        stBuffer->currFrameIndex = 0;
        stBuffer->speech_len = 0;
        stBuffer->ns_tail_len = 0;
    }
    return 0;
}

int WebRtcVad::DataInSpeechBuffer(SpeechBuffer *stBuffer,short* Inframe,int is_speech)
{
    if(stBuffer->currFrameIndex >= MAX_FRAMES_CNT_ONESPEECH){
        return -2;//SpeechBuffer is already full
    }
   
    if(stBuffer && Inframe){
        memcpy(stBuffer->buffer[stBuffer->currFrameIndex], Inframe,160*sizeof(short));    //2 byte* 160 frames once
        stBuffer->currFrameIndex++;

        if(1==is_speech){
            stBuffer->speech_len = stBuffer->currFrameIndex;
            stBuffer->ns_tail_len = 0;

        }else{
            stBuffer->ns_tail_len++;

        }
        return 0;
    }else{
        return -1;
    }
}

int WebRtcVad::QuerySpeechBufferIsEmpty(SpeechBuffer *stBuffer)
{  
    if(stBuffer && stBuffer->currFrameIndex > 0){
        return 0;
    }
    return 1;
}

