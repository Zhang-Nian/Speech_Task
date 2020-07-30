
#include "SignalProcessing.h"

#define PATH_LENGTH 1024

SignalProcessing *SignalProcessing::m_instance = 0;

SignalProcessing *SignalProcessing::GetInstance() {
    if (m_instance == NULL) {
        m_instance = new SignalProcessing();
    }

    return m_instance;
}

SignalProcessing::SignalProcessing() {
}

SignalProcessing::~SignalProcessing() {
}

WebRtc_Word32 SignalProcessing::WebRtcSpl_Energy(WebRtc_Word16* vector, int vector_length, int* scale_factor)
{
    WebRtc_Word32 en = 0;
    int i;
    int scaling = WebRtcSpl_GetScalingSquare(vector, vector_length, vector_length);
    int looptimes = vector_length;
    WebRtc_Word16 *vectorptr = vector;

    for (i = 0; i < looptimes; i++)
    {
        en += WEBRTC_SPL_MUL_16_16_RSFT(*vectorptr, *vectorptr, scaling);
        vectorptr++;
    }
    *scale_factor = scaling;

    return en;
}

int SignalProcessing::WebRtcSpl_GetScalingSquare(WebRtc_Word16 *in_vector, int in_vector_length, int times)
{
    int nbits = WebRtcSpl_GetSizeInBits(times);
    int i;
    WebRtc_Word16 smax = -1;
    WebRtc_Word16 sabs;
    WebRtc_Word16 *sptr = in_vector;
    int t;
    int looptimes = in_vector_length;

    for (i = looptimes; i > 0; i--)
    {
        sabs = (*sptr > 0 ? *sptr++ : -*sptr++);
        smax = (sabs > smax ? sabs : smax);
    }
    t = WebRtcSpl_NormW32(WEBRTC_SPL_MUL(smax, smax));

    if (smax == 0)
    {
        return 0; // Since norm(0) returns 0
    } else
    {
        return (t > nbits) ? 0 : nbits - t;
    }
}

WebRtc_UWord32 SignalProcessing::WebRtcSpl_DivU32U16(WebRtc_UWord32 num, WebRtc_UWord16 den)
{
    // Guard against division with 0
    if (den != 0)
    {
        return (WebRtc_UWord32)(num / den);
    } else
    {
        return (WebRtc_UWord32)0xFFFFFFFF;
    }
}

WebRtc_Word32 SignalProcessing::WebRtcSpl_DivW32W16(WebRtc_Word32 num, WebRtc_Word16 den)
{
    // Guard against division with 0
    if (den != 0) {
        return (WebRtc_Word32)(num / den);
    } else {
        return (WebRtc_Word32)0x7FFFFFFF;
    }
}

WebRtc_Word16 SignalProcessing::WebRtcSpl_DivW32W16ResW16(WebRtc_Word32 num, WebRtc_Word16 den)
{
    // Guard against division with 0
    if (den != 0) {
        return (WebRtc_Word16)(num / den);
    } else {
        return (WebRtc_Word16)0x7FFF;
    }
}

WebRtc_Word32 SignalProcessing::WebRtcSpl_DivResultInQ31(WebRtc_Word32 num, WebRtc_Word32 den)
{
    WebRtc_Word32 L_num = num;
    WebRtc_Word32 L_den = den;
    WebRtc_Word32 div = 0;
    int k = 31;
    int change_sign = 0;

    if (num == 0)
        return 0;

    if (num < 0)
    {
        change_sign++;
        L_num = -num;
    }
    if (den < 0)
    {
        change_sign++;
        L_den = -den;
    }
    while (k--)
    {
        div <<= 1;
        L_num <<= 1;
        if (L_num >= L_den)
        {
            L_num -= L_den;
            div++;
        }
    }
    if (change_sign == 1)
    {
        div = -div;
    }
    return div;
}

WebRtc_Word32 SignalProcessing::WebRtcSpl_DivW32HiLow(WebRtc_Word32 num, WebRtc_Word16 den_hi, WebRtc_Word16 den_low)
{
    WebRtc_Word16 approx, tmp_hi, tmp_low, num_hi, num_low;
    WebRtc_Word32 tmpW32;

    approx = (WebRtc_Word16)WebRtcSpl_DivW32W16((WebRtc_Word32)0x1FFFFFFF, den_hi);
    // result in Q14 (Note: 3FFFFFFF = 0.5 in Q30)

    // tmpW32 = 1/den = approx * (2.0 - den * approx) (in Q30)
    tmpW32 = (WEBRTC_SPL_MUL_16_16(den_hi, approx) << 1)
            + ((WEBRTC_SPL_MUL_16_16(den_low, approx) >> 15) << 1);
    // tmpW32 = den * approx

    tmpW32 = (WebRtc_Word32)0x7fffffffL - tmpW32; // result in Q30 (tmpW32 = 2.0-(den*approx))

    // Store tmpW32 in hi and low format
    tmp_hi = (WebRtc_Word16)WEBRTC_SPL_RSHIFT_W32(tmpW32, 16);
    tmp_low = (WebRtc_Word16)WEBRTC_SPL_RSHIFT_W32((tmpW32
            - WEBRTC_SPL_LSHIFT_W32((WebRtc_Word32)tmp_hi, 16)), 1);

    // tmpW32 = 1/den in Q29
    tmpW32 = ((WEBRTC_SPL_MUL_16_16(tmp_hi, approx) + (WEBRTC_SPL_MUL_16_16(tmp_low, approx)
            >> 15)) << 1);

    // 1/den in hi and low format
    tmp_hi = (WebRtc_Word16)WEBRTC_SPL_RSHIFT_W32(tmpW32, 16);
    tmp_low = (WebRtc_Word16)WEBRTC_SPL_RSHIFT_W32((tmpW32
            - WEBRTC_SPL_LSHIFT_W32((WebRtc_Word32)tmp_hi, 16)), 1);

    // Store num in hi and low format
    num_hi = (WebRtc_Word16)WEBRTC_SPL_RSHIFT_W32(num, 16);
    num_low = (WebRtc_Word16)WEBRTC_SPL_RSHIFT_W32((num
            - WEBRTC_SPL_LSHIFT_W32((WebRtc_Word32)num_hi, 16)), 1);

    // num * (1/den) by 32 bit multiplication (result in Q28)

    tmpW32 = (WEBRTC_SPL_MUL_16_16(num_hi, tmp_hi) + (WEBRTC_SPL_MUL_16_16(num_hi, tmp_low)
            >> 15) + (WEBRTC_SPL_MUL_16_16(num_low, tmp_hi) >> 15));

    // Put result in Q31 (convert from Q28)
    tmpW32 = WEBRTC_SPL_LSHIFT_W32(tmpW32, 3);

    return tmpW32;
}


