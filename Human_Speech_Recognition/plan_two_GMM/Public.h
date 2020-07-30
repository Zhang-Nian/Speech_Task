
#ifndef HUMAN_SPEECH_RECOGNITION_PUBLIC_H_
#define HUMAN_SPEECH_RECOGNITION_PUBLIC_H_

#include "typedefs.h"
#include "vad_defines.h"

// Macros specific for the fixed point implementation
#define WEBRTC_SPL_WORD16_MAX       32767
#define WEBRTC_SPL_WORD16_MIN       -32768
#define WEBRTC_SPL_WORD32_MAX       (WebRtc_Word32)0x7fffffff
#define WEBRTC_SPL_WORD32_MIN       (WebRtc_Word32)0x80000000
#define WEBRTC_SPL_MAX_LPC_ORDER    14
#define WEBRTC_SPL_MAX_SEED_USED    0x80000000L
#define WEBRTC_SPL_MIN(A, B)        (A < B ? A : B) // Get min value
#define WEBRTC_SPL_MAX(A, B)        (A > B ? A : B) // Get max value
#define WEBRTC_SPL_ABS_W16(a) \
    (((WebRtc_Word16)a >= 0) ? ((WebRtc_Word16)a) : -((WebRtc_Word16)a))
#define WEBRTC_SPL_ABS_W32(a) \
    (((WebRtc_Word32)a >= 0) ? ((WebRtc_Word32)a) : -((WebRtc_Word32)a))

#if (defined WEBRTC_TARGET_PC)||(defined __TARGET_XSCALE)
#define WEBRTC_SPL_GET_BYTE(a, nr)  (((WebRtc_Word8 *)a)[nr])
#define WEBRTC_SPL_SET_BYTE(d_ptr, val, index) \
    (((WebRtc_Word8 *)d_ptr)[index] = (val))
#elif defined WEBRTC_BIG_ENDIAN
#define WEBRTC_SPL_GET_BYTE(a, nr) \
    ((((WebRtc_Word16 *)a)[nr >> 1]) >> (((nr + 1) & 0x1) * 8) & 0x00ff)
#define WEBRTC_SPL_SET_BYTE(d_ptr, val, index) \
    ((WebRtc_Word16 *)d_ptr)[index >> 1] = \
    ((((WebRtc_Word16 *)d_ptr)[index >> 1]) \
    & (0x00ff << (8 * ((index) & 0x1)))) | (val << (8 * ((index + 1) & 0x1)))
#else
#define WEBRTC_SPL_GET_BYTE(a,nr) \
    ((((WebRtc_Word16 *)(a))[(nr) >> 1]) >> (((nr) & 0x1) * 8) & 0x00ff)
#define WEBRTC_SPL_SET_BYTE(d_ptr, val, index) \
    ((WebRtc_Word16 *)(d_ptr))[(index) >> 1] = \
    ((((WebRtc_Word16 *)(d_ptr))[(index) >> 1]) \
    & (0x00ff << (8 * (((index) + 1) & 0x1)))) | \
    ((val) << (8 * ((index) & 0x1)))
#endif

#define WEBRTC_SPL_MUL(a, b) \
    ((WebRtc_Word32) ((WebRtc_Word32)(a) * (WebRtc_Word32)(b)))
#define WEBRTC_SPL_UMUL(a, b) \
    ((WebRtc_UWord32) ((WebRtc_UWord32)(a) * (WebRtc_UWord32)(b)))
#define WEBRTC_SPL_UMUL_RSFT16(a, b) \
    ((WebRtc_UWord32) ((WebRtc_UWord32)(a) * (WebRtc_UWord32)(b)) >> 16)
#define WEBRTC_SPL_UMUL_16_16(a, b) \
    ((WebRtc_UWord32) (WebRtc_UWord16)(a) * (WebRtc_UWord16)(b))
#define WEBRTC_SPL_UMUL_16_16_RSFT16(a, b) \
    (((WebRtc_UWord32) (WebRtc_UWord16)(a) * (WebRtc_UWord16)(b)) >> 16)
#define WEBRTC_SPL_UMUL_32_16(a, b) \
    ((WebRtc_UWord32) ((WebRtc_UWord32)(a) * (WebRtc_UWord16)(b)))
#define WEBRTC_SPL_UMUL_32_16_RSFT16(a, b) \
    ((WebRtc_UWord32) ((WebRtc_UWord32)(a) * (WebRtc_UWord16)(b)) >> 16)
#define WEBRTC_SPL_MUL_16_U16(a, b) \
    ((WebRtc_Word32)(WebRtc_Word16)(a) * (WebRtc_UWord16)(b))
#define WEBRTC_SPL_DIV(a, b) \
    ((WebRtc_Word32) ((WebRtc_Word32)(a) / (WebRtc_Word32)(b)))
#define WEBRTC_SPL_UDIV(a, b) \
    ((WebRtc_UWord32) ((WebRtc_UWord32)(a) / (WebRtc_UWord32)(b)))

#ifndef WEBRTC_ARCH_ARM_V7A
// For ARMv7 platforms, these are inline functions in spl_inl_armv7.h
#define WEBRTC_SPL_MUL_16_16(a, b) \
    ((WebRtc_Word32) (((WebRtc_Word16)(a)) * ((WebRtc_Word16)(b))))
#define WEBRTC_SPL_MUL_16_32_RSFT16(a, b) \
    (WEBRTC_SPL_MUL_16_16(a, b >> 16) \
     + ((WEBRTC_SPL_MUL_16_16(a, (b & 0xffff) >> 1) + 0x4000) >> 15))
#define WEBRTC_SPL_MUL_32_32_RSFT32(a32a, a32b, b32) \
    ((WebRtc_Word32)(WEBRTC_SPL_MUL_16_32_RSFT16(a32a, b32) \
    + (WEBRTC_SPL_MUL_16_32_RSFT16(a32b, b32) >> 16)))
#define WEBRTC_SPL_MUL_32_32_RSFT32BI(a32, b32) \
    ((WebRtc_Word32)(WEBRTC_SPL_MUL_16_32_RSFT16(( \
    (WebRtc_Word16)(a32 >> 16)), b32) + \
    (WEBRTC_SPL_MUL_16_32_RSFT16(( \
    (WebRtc_Word16)((a32 & 0x0000FFFF) >> 1)), b32) >> 15)))
#endif

#define WEBRTC_SPL_MUL_16_32_RSFT11(a, b) \
    ((WEBRTC_SPL_MUL_16_16(a, (b) >> 16) << 5) \
    + (((WEBRTC_SPL_MUL_16_U16(a, (WebRtc_UWord16)(b)) >> 1) + 0x0200) >> 10))
#define WEBRTC_SPL_MUL_16_32_RSFT14(a, b) \
    ((WEBRTC_SPL_MUL_16_16(a, (b) >> 16) << 2) \
    + (((WEBRTC_SPL_MUL_16_U16(a, (WebRtc_UWord16)(b)) >> 1) + 0x1000) >> 13))
#define WEBRTC_SPL_MUL_16_32_RSFT15(a, b) \
    ((WEBRTC_SPL_MUL_16_16(a, (b) >> 16) << 1) \
    + (((WEBRTC_SPL_MUL_16_U16(a, (WebRtc_UWord16)(b)) >> 1) + 0x2000) >> 14))

#ifdef ARM_WINM
#define WEBRTC_SPL_MUL_16_16(a, b) \
    _SmulLo_SW_SL((WebRtc_Word16)(a), (WebRtc_Word16)(b))
#endif

#define WEBRTC_SPL_MUL_16_16_RSFT(a, b, c) \
    (WEBRTC_SPL_MUL_16_16(a, b) >> (c))

#define WEBRTC_SPL_MUL_16_16_RSFT_WITH_ROUND(a, b, c) \
    ((WEBRTC_SPL_MUL_16_16(a, b) + ((WebRtc_Word32) \
                                  (((WebRtc_Word32)1) << ((c) - 1)))) >> (c))
#define WEBRTC_SPL_MUL_16_16_RSFT_WITH_FIXROUND(a, b) \
    ((WEBRTC_SPL_MUL_16_16(a, b) + ((WebRtc_Word32) (1 << 14))) >> 15)

// C + the 32 most significant bits of A * B
#define WEBRTC_SPL_SCALEDIFF32(A, B, C) \
    (C + (B >> 16) * A + (((WebRtc_UWord32)(0x0000FFFF & B) * A) >> 16))

#define WEBRTC_SPL_ADD_SAT_W32(a, b)    WebRtcSpl_AddSatW32(a, b)
#define WEBRTC_SPL_SAT(a, b, c)         (b > a ? a : b < c ? c : b)
#define WEBRTC_SPL_MUL_32_16(a, b)      ((a) * (b))

#define WEBRTC_SPL_SUB_SAT_W32(a, b)    WebRtcSpl_SubSatW32(a, b)
#define WEBRTC_SPL_ADD_SAT_W16(a, b)    WebRtcSpl_AddSatW16(a, b)
#define WEBRTC_SPL_SUB_SAT_W16(a, b)    WebRtcSpl_SubSatW16(a, b)

// We cannot do casting here due to signed/unsigned problem
#define WEBRTC_SPL_IS_NEG(a)            ((a) & 0x80000000)
// Shifting with negative numbers allowed Positive means left shift
#define WEBRTC_SPL_SHIFT_W16(x, c) \
    (((c) >= 0) ? ((x) << (c)) : ((x) >> (-(c))))
#define WEBRTC_SPL_SHIFT_W32(x, c) \
    (((c) >= 0) ? ((x) << (c)) : ((x) >> (-(c))))

// Shifting with negative numbers not allowed
// We cannot do casting here due to signed/unsigned problem
#define WEBRTC_SPL_RSHIFT_W16(x, c)     ((x) >> (c))
#define WEBRTC_SPL_LSHIFT_W16(x, c)     ((x) << (c))
#define WEBRTC_SPL_RSHIFT_W32(x, c)     ((x) >> (c))
#define WEBRTC_SPL_LSHIFT_W32(x, c)     ((x) << (c))

#define WEBRTC_SPL_RSHIFT_U16(x, c)     ((WebRtc_UWord16)(x) >> (c))
#define WEBRTC_SPL_LSHIFT_U16(x, c)     ((WebRtc_UWord16)(x) << (c))
#define WEBRTC_SPL_RSHIFT_U32(x, c)     ((WebRtc_UWord32)(x) >> (c))
#define WEBRTC_SPL_LSHIFT_U32(x, c)     ((WebRtc_UWord32)(x) << (c))

#define WEBRTC_SPL_VNEW(t, n)           (t *) malloc (sizeof (t) * (n))
#define WEBRTC_SPL_FREE                 free

#define WEBRTC_SPL_RAND(a) \
    ((WebRtc_Word16)(WEBRTC_SPL_MUL_16_16_RSFT((a), 18816, 7) & 0x00007fff))

#define WEBRTC_SPL_MEMCPY_W8(v1, v2, length) \
   memcpy(v1, v2, (length) * sizeof(char))
#define WEBRTC_SPL_MEMCPY_W16(v1, v2, length) \
   memcpy(v1, v2, (length) * sizeof(WebRtc_Word16))

#define WEBRTC_SPL_MEMMOVE_W16(v1, v2, length) \
   memmove(v1, v2, (length) * sizeof(WebRtc_Word16))

typedef struct VadInstT_
{

    WebRtc_Word16 vad;
    WebRtc_Word32 downsampling_filter_states[4];
    WebRtc_Word16 noise_means[NUM_TABLE_VALUES];
    WebRtc_Word16 speech_means[NUM_TABLE_VALUES];
    WebRtc_Word16 noise_stds[NUM_TABLE_VALUES];
    WebRtc_Word16 speech_stds[NUM_TABLE_VALUES];
    // TODO(bjornv): Change to |frame_count|.
    WebRtc_Word32 frame_counter;
    WebRtc_Word16 over_hang; // Over Hang
    WebRtc_Word16 num_of_speech;
    // TODO(bjornv): Change to |age_vector|.
    WebRtc_Word16 index_vector[16 * NUM_CHANNELS];
    WebRtc_Word16 low_value_vector[16 * NUM_CHANNELS];
    // TODO(bjornv): Change to |median|.
    WebRtc_Word16 mean_value[NUM_CHANNELS];
    WebRtc_Word16 upper_state[5];
    WebRtc_Word16 lower_state[5];
    WebRtc_Word16 hp_filter_state[4];
    WebRtc_Word16 over_hang_max_1[3];
    WebRtc_Word16 over_hang_max_2[3];
    WebRtc_Word16 individual[3];
    WebRtc_Word16 total[3];

    short init_flag;

} VadInstT;



// spl_in.h static function
static __inline WebRtc_Word16 WebRtcSpl_SatW32ToW16(WebRtc_Word32 value32) {
  WebRtc_Word16 out16 = (WebRtc_Word16) value32;

  if (value32 > 32767)
    out16 = 32767;
  else if (value32 < -32768)
    out16 = -32768;

  return out16;
}

static __inline WebRtc_Word16 WebRtcSpl_AddSatW16(WebRtc_Word16 a,
                                                  WebRtc_Word16 b) {
  return WebRtcSpl_SatW32ToW16((WebRtc_Word32) a + (WebRtc_Word32) b);
}

static __inline WebRtc_Word32 WebRtcSpl_AddSatW32(WebRtc_Word32 l_var1,
                                                  WebRtc_Word32 l_var2) {
  WebRtc_Word32 l_sum;

  // perform long addition
  l_sum = l_var1 + l_var2;

  // check for under or overflow
  if (WEBRTC_SPL_IS_NEG(l_var1)) {
    if (WEBRTC_SPL_IS_NEG(l_var2) && !WEBRTC_SPL_IS_NEG(l_sum)) {
        l_sum = (WebRtc_Word32)0x80000000;
    }
  } else {
    if (!WEBRTC_SPL_IS_NEG(l_var2) && WEBRTC_SPL_IS_NEG(l_sum)) {
        l_sum = (WebRtc_Word32)0x7FFFFFFF;
    }
  }

  return l_sum;
}

static __inline WebRtc_Word16 WebRtcSpl_SubSatW16(WebRtc_Word16 var1,
                                                  WebRtc_Word16 var2) {
  return WebRtcSpl_SatW32ToW16((WebRtc_Word32) var1 - (WebRtc_Word32) var2);
}

static __inline WebRtc_Word32 WebRtcSpl_SubSatW32(WebRtc_Word32 l_var1,
                                                  WebRtc_Word32 l_var2) {
  WebRtc_Word32 l_diff;

  // perform subtraction
  l_diff = l_var1 - l_var2;

  // check for underflow
  if ((l_var1 < 0) && (l_var2 > 0) && (l_diff > 0))
    l_diff = (WebRtc_Word32)0x80000000;
  // check for overflow
  if ((l_var1 > 0) && (l_var2 < 0) && (l_diff < 0))
    l_diff = (WebRtc_Word32)0x7FFFFFFF;

  return l_diff;
}

static __inline WebRtc_Word16 WebRtcSpl_GetSizeInBits(WebRtc_UWord32 n) {
  int bits;

  if (0xFFFF0000 & n) {
    bits = 16;
  } else {
    bits = 0;
  }
  if (0x0000FF00 & (n >> bits)) bits += 8;
  if (0x000000F0 & (n >> bits)) bits += 4;
  if (0x0000000C & (n >> bits)) bits += 2;
  if (0x00000002 & (n >> bits)) bits += 1;
  if (0x00000001 & (n >> bits)) bits += 1;

  return bits;
}

static __inline int WebRtcSpl_NormW32(WebRtc_Word32 a) {
  int zeros;

  if (a <= 0) a ^= 0xFFFFFFFF;

  if (!(0xFFFF8000 & a)) {
    zeros = 16;
  } else {
    zeros = 0;
  }
  if (!(0xFF800000 & (a << zeros))) zeros += 8;
  if (!(0xF8000000 & (a << zeros))) zeros += 4;
  if (!(0xE0000000 & (a << zeros))) zeros += 2;
  if (!(0xC0000000 & (a << zeros))) zeros += 1;

  return zeros;
}

static __inline int WebRtcSpl_NormU32(WebRtc_UWord32 a) {
  int zeros;

  if (a == 0) return 0;

  if (!(0xFFFF0000 & a)) {
    zeros = 16;
  } else {
    zeros = 0;
  }
  if (!(0xFF000000 & (a << zeros))) zeros += 8;
  if (!(0xF0000000 & (a << zeros))) zeros += 4;
  if (!(0xC0000000 & (a << zeros))) zeros += 2;
  if (!(0x80000000 & (a << zeros))) zeros += 1;

  return zeros;
}

static __inline int WebRtcSpl_NormW16(WebRtc_Word16 a) {
  int zeros;

  if (a <= 0) a ^= 0xFFFF;

  if (!(0xFF80 & a)) {
    zeros = 8;
  } else {
    zeros = 0;
  }
  if (!(0xF800 & (a << zeros))) zeros += 4;
  if (!(0xE000 & (a << zeros))) zeros += 2;
  if (!(0xC000 & (a << zeros))) zeros += 1;

  return zeros;
}

static __inline int32_t WebRtc_MulAccumW16(int16_t a,
                                          int16_t b,
                                          int32_t c) {
  return (a * b + c);
}


typedef struct wav_header {
    uint32_t riff_id;
    uint32_t riff_sz;
    uint32_t riff_fmt;
    uint32_t fmt_id;
    uint32_t fmt_sz;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint32_t data_id;
    uint32_t data_sz;
}WAV_HEADER;

#endif


