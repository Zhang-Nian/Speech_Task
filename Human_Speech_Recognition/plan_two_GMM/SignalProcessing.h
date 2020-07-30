#ifndef SIGNAL_PROCESSING_H_
#define SIGNAL_PROCESSING_H_ 

#include <string>
#include <iostream>
#include "Public.h"

class SignalProcessing {
public:
    static SignalProcessing* GetInstance();

    WebRtc_Word32 WebRtcSpl_Energy(WebRtc_Word16* vector, int vector_length, int* scale_factor);

    int WebRtcSpl_GetScalingSquare(WebRtc_Word16 *in_vector, int in_vector_length, int times);

    WebRtc_UWord32 WebRtcSpl_DivU32U16(WebRtc_UWord32 num, WebRtc_UWord16 den);
    WebRtc_Word32 WebRtcSpl_DivW32W16(WebRtc_Word32 num, WebRtc_Word16 den);
    WebRtc_Word16 WebRtcSpl_DivW32W16ResW16(WebRtc_Word32 num, WebRtc_Word16 den);
    WebRtc_Word32 WebRtcSpl_DivResultInQ31(WebRtc_Word32 num, WebRtc_Word32 den);
    WebRtc_Word32 WebRtcSpl_DivW32HiLow(WebRtc_Word32 num, WebRtc_Word16 den_hi, WebRtc_Word16 den_low);

private:
    SignalProcessing();
    ~SignalProcessing();

    static SignalProcessing* m_instance;
};

#endif  

