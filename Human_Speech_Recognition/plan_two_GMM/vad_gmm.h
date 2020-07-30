
#ifndef WEBRTC_COMMON_AUDIO_VAD_VAD_GMM_H_
#define WEBRTC_COMMON_AUDIO_VAD_VAD_GMM_H_

#include "typedefs.h"

class WebRtcVadGMM {
public:
    WebRtcVadGMM();
    ~WebRtcVadGMM();

    // Calculates the probability for |input|, given that |input| comes from a
    // normal distribution with mean and standard deviation (|mean|, |std|).
    //
    // Inputs:
    //      - input         : input sample in Q4.
    //      - mean          : mean input in the statistical model, Q7.
    //      - std           : standard deviation, Q7.
    //
    // Output:
    //
    //      - delta         : input used when updating the model, Q11.
    //                        |delta| = (|input| - |mean|) / |std|^2.
    //
    // Return:
    //   (probability for |input|) =
    //    1 / |std| * exp(-(|input| - |mean|)^2 / (2 * |std|^2));
    int32_t WebRtcVad_GaussianProbability(int16_t input,
                                          int16_t mean,
                                          int16_t std,
                                          int16_t* delta);
};

#endif  // WEBRTC_COMMON_AUDIO_VAD_VAD_GMM_H_
