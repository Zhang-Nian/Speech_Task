
#ifndef WEBRTC_VAD_CORE_H_
#define WEBRTC_VAD_CORE_H_

#include <iostream>
#include "Public.h"

#include "vad_sp.h"
#include "vad_filterbank.h"
#include "vad_gmm.h"

class WebRtcVadCore {
public:
    WebRtcVadCore();
    ~WebRtcVadCore();

    /****************************************************************************
    * WebRtcVad_InitCore(...)
    *
    * This function initializes a VAD instance
    *
    * Input:
    *      - inst      : Instance that should be initialized
    *      - mode      : Aggressiveness degree
    *                    0 (High quality) - 3 (Highly aggressive)
    *
    * Output:
    *      - inst      : Initialized instance
    *
    * Return value     :  0 - Ok
    *                    -1 - Error
    */
    int WebRtcVad_InitCore(VadInstT* inst, short mode);

    /****************************************************************************
    * WebRtcVad_set_mode_core(...)
    *
    * This function changes the VAD settings
    *
    * Input:
    *      - inst      : VAD instance
    *      - mode      : Aggressiveness degree
    *                    0 (High quality) - 3 (Highly aggressive)
    *
    * Output:
    *      - inst      : Changed  instance
    *
    * Return value     :  0 - Ok
    *                    -1 - Error
    */

    int WebRtcVad_set_mode_core(VadInstT* inst, short mode);

    /****************************************************************************
    * WebRtcVad_CalcVad32khz(...) 
    * WebRtcVad_CalcVad16khz(...) 
    * WebRtcVad_CalcVad8khz(...) 
    *
    * Calculate probability for active speech and make VAD decision.
    *
    * Input:
    *      - inst          : Instance that should be initialized
    *      - speech_frame  : Input speech frame
    *      - frame_length  : Number of input samples
    *
    * Output:
    *      - inst          : Updated filter states etc.
    *
    * Return value         : VAD decision
    *                        0 - No active speech
    *                        1-6 - Active speech
    */
    WebRtc_Word16 WebRtcVad_CalcVad32khz(VadInstT* inst, WebRtc_Word16* speech_frame,
                                     int frame_length);
    WebRtc_Word16 WebRtcVad_CalcVad16khz(VadInstT* inst, WebRtc_Word16* speech_frame,
                                     int frame_length);
    WebRtc_Word16 WebRtcVad_CalcVad8khz(VadInstT* inst, WebRtc_Word16* speech_frame,
                                    int frame_length);

    /****************************************************************************
    * WebRtcVad_GmmProbability(...)
    *
    * This function calculates the probabilities for background noise and
    * speech using Gaussian Mixture Models. A hypothesis-test is performed to decide
    * which type of signal is most probable.
    *
    * Input:
    *      - inst              : Pointer to VAD instance
    *      - feature_vector    : Feature vector = log10(energy in frequency band)
    *      - total_power       : Total power in frame.
    *      - frame_length      : Number of input samples
    *
    * Output:
    *      VAD decision        : 0 - noise, 1 - speech
    *    
    */
    WebRtc_Word16 WebRtcVad_GmmProbability(VadInstT* inst, WebRtc_Word16* feature_vector,
                                       WebRtc_Word16 total_power, int frame_length);

private:
    WebRtcVadSP* m_pVadSP;
    WebRtcVadFilterBank* m_pVadFilterBank;
    WebRtcVadGMM* m_pVadGMM;
};
#endif // WEBRTC_VAD_CORE_H_
