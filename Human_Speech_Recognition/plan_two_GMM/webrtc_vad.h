
#ifndef WEBRTC_VAD_WEBRTC_VAD_H_
#define WEBRTC_VAD_WEBRTC_VAD_H_

#include "typedefs.h"
#include "vad_core.h"

typedef struct WebRtcVadInst VadInst;

#define MAX_FRAMES_CNT_ONESPEECH  100*30 //max length of one speech : 30s
#define MAX_BREAK_MID_SPEECH   50 //1s
#define MIN_SPEECH_LEN         50  //500ms

typedef struct speechBuffer{
    short buffer[MAX_FRAMES_CNT_ONESPEECH][160];
    short currFrameIndex;
    short speech_len;
    short ns_tail_len;
}SpeechBuffer;

class WebRtcVad {
public:
    WebRtcVad();
    ~WebRtcVad();

    /****************************************************************************
    * WebRtcVad_get_version(...)
    *
    * This function returns the version number of the code.
    *
    * Output:
    *      - version       : Pointer to a buffer where the version info will
    *                        be stored.
    * Input:
    *      - size_bytes    : Size of the buffer.
    *
    */
    WebRtc_Word16 WebRtcVad_get_version(char *version, size_t size_bytes);

    /****************************************************************************
    * WebRtcVad_AssignSize(...) 
    *
    * This functions get the size needed for storing the instance for encoder
    * and decoder, respectively
    *
    * Input/Output:
    *      - size_in_bytes : Pointer to integer where the size is returned
    *
    * Return value         : 0
    */
    WebRtc_Word16 WebRtcVad_AssignSize(int *size_in_bytes);

    /****************************************************************************
    * WebRtcVad_Assign(...) 
    *
    * This functions Assigns memory for the instances.
    *
    * Input:
    *        - vad_inst_addr :  Address to where to assign memory
    * Output:
    *        - vad_inst      :  Pointer to the instance that should be created
    *
    * Return value           :  0 - Ok
    *                          -1 - Error
    */
    WebRtc_Word16 WebRtcVad_Assign(VadInst **vad_inst, void *vad_inst_addr);

    /****************************************************************************
    * WebRtcVad_Create(...)
    *
    * This function creates an instance to the VAD structure
    *
    * Input:
    *      - vad_inst      : Pointer to VAD instance that should be created
    *
    * Output:
    *      - vad_inst      : Pointer to created VAD instance
    *
    * Return value         :  0 - Ok
    *                        -1 - Error
    */
    WebRtc_Word16 WebRtcVad_Create(VadInst **vad_inst);

    /****************************************************************************
    * WebRtcVad_Free(...)
    *
    * This function frees the dynamic memory of a specified VAD instance
    *
    * Input:
    *      - vad_inst      : Pointer to VAD instance that should be freed
    *
    * Return value         :  0 - Ok
    *                        -1 - Error
    */
    WebRtc_Word16 WebRtcVad_Free(VadInst *vad_inst);

    /****************************************************************************
    * WebRtcVad_Init(...)
    *
    * This function initializes a VAD instance
    *
    * Input:
    *      - vad_inst      : Instance that should be initialized
    *
    * Output:
    *      - vad_inst      : Initialized instance
    * 
    * Return value         :  0 - Ok
    *                        -1 - Error
    */
    WebRtc_Word16 WebRtcVad_Init(VadInst *vad_inst);

    /****************************************************************************
    * WebRtcVad_set_mode(...)
    *
    * This function initializes a VAD instance
    *
    * Input:
    *      - vad_inst      : VAD instance
    *      - mode          : Aggressiveness setting (0, 1, 2, or 3) 
    *
    * Output:
    *      - vad_inst      : Initialized instance
    *
    * Return value         :  0 - Ok
    *                        -1 - Error
    */
    WebRtc_Word16 WebRtcVad_set_mode(VadInst *vad_inst, WebRtc_Word16 mode);

    /****************************************************************************
    * WebRtcVad_Process(...)
    * 
    * This functions does a VAD for the inserted speech frame
    *
    * Input
    *        - vad_inst     : VAD Instance. Needs to be initiated before call.
    *        - fs           : sampling frequency (Hz): 8000, 16000, or 32000
    *        - speech_frame : Pointer to speech frame buffer
    *        - frame_length : Length of speech frame buffer in number of samples
    *
    * Output:
    *        - vad_inst     : Updated VAD instance
    *
    * Return value          :  1 - Active Voice
    *                          0 - Non-active Voice
    *                         -1 - Error
    */
    WebRtc_Word16 WebRtcVad_Process(VadInst *vad_inst,
                                    WebRtc_Word16 fs,
                                    WebRtc_Word16 *speech_frame,
                                    WebRtc_Word16 frame_length);

    int HumanSpeechRecognition(short* wav_data, int length);

private:
    int ClearSpeechBuffer(SpeechBuffer *stBuffer);
    int DataInSpeechBuffer(SpeechBuffer *stBuffer,short* Inframe,int is_speech);
    int QuerySpeechBufferIsEmpty(SpeechBuffer *stBuffer);

    size_t GetPartWaveData(short* wav_data, int length, int start_index, short* part_data);

    WebRtcVadCore* m_pVadCore;

};
#endif // WEBRTC_VAD_WEBRTC_VAD_H_
