
#ifndef WEBRTC_COMMON_AUDIO_VAD_VAD_SP_H_
#define WEBRTC_COMMON_AUDIO_VAD_VAD_SP_H_

#include "typedefs.h"
#include "Public.h"

class WebRtcVadSP {
public:
    WebRtcVadSP();
    ~WebRtcVadSP();

    // Downsamples the signal by a factor 2, eg. 32->16 or 16->8.
    //
    // Inputs:
    //      - signal_in     : Input signal.
    //      - in_length     : Length of input signal in samples.
    //
    // Input & Output:
    //      - filter_state  : Current filter states of the two all-pass filters. The
    //                        |filter_state| is updated after all samples have been
    //                        processed.
    //
    // Output:
    //      - signal_out    : Downsampled signal (of length |in_length| / 2).
    void WebRtcVad_Downsampling(int16_t* signal_in,
                                int16_t* signal_out,
                                int32_t* filter_state,
                                int in_length);

    // Updates and returns the smoothed feature minimum. As minimum we use the
    // median of the five smallest feature values in a 100 frames long window.
    //
    // Inputs:
    //      - feature_value : New feature value to update with.
    //      - channel       : Channel number.
    //
    // Input & Output:
    //      - handle        : State information of the VAD.
    //
    // Returns:
    //                      : Smoothed minimum value for a moving window.
    int16_t WebRtcVad_FindMinimum(VadInstT* handle,
                                  int16_t feature_value,
                                  int channel);

};
#endif  // WEBRTC_COMMON_AUDIO_VAD_VAD_SP_H_