#ifndef TE_MATH_H
#define TE_MATH_H

#include "DataTypes.h"

#define KB(x) (  (x) * 1000)
#define MB(x) (KB(x) * 1000)
#define GB(x) (MB(x) * 1000)
#define TB(x) (GB(x) * 1000)

#define KiB(x) (   (x) * 1024)
#define MiB(x) (KiB(x) * 1024)
#define GiB(x) (MiB(x) * 1024)
#define TiB(x) (GiB(x) * 1024)

#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))

inline s32 TruncateI64ToS32(s64 value) {
    s32 result = value & 0xFFFFFFFF;
    return result;
}

inline s32 TruncateF32ToS32(f32 value) {
    s32 result = (s32)(value);
    return result;
}

inline u32 TruncateF32ToU32(f32 value) {
    u32 result = (u32)(value);
    return result;
}

inline s32 RoundF32ToS32(f32 value) {
    s32 result = (s32)(value + 0.5f);
    return result;
}

inline u32 RoundF32ToU32(f32 value) {
    u32 result = (u32)(value + 0.5f);
    return result;
}

inline s32 CeilF32ToS32(f32 value) {
    s32 result = TruncateF32ToU32(value);
    result++;
    return result;
}

inline u32 CeilF32ToU32(f32 value) {
    u32 result = TruncateF32ToU32(value);
    result++;
    return result;
}

#define GetIntegerDigits(value) Log10S32((value))

inline s32 Log10S32(s32 value) {
    s32 result = 0;

    while (value >= 10) {
        value /= 10;
	    ++result;
    }

    return result;
}

#endif //TE_MATH_H
