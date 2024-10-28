#ifndef DSV_DATATYPES_H
#define DSV_DATATYPES_H

#include <stdint.h>

#define LocalPersistant static
#define GlobalVariable static
#define Inter static

#define MAX_VALUE_u64 0xFFFFFFFFFFFFFFFF
#define MAX_VALUE_PTR (WordSize)(0xFFFFFFFFFFFFFFFF)

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef union {
    struct {
        u64 l64;
        u64 h64;
    };
    u32 l32;
    u16 l16;
    u8  l8;
    u8 B[16];
} u128;

typedef size_t WordSize;

typedef uint8_t byte;

typedef float  f32;
typedef double f64;

typedef s32 b32;

#define MAX_u8  (u8 )(0xff)
#define MAX_u16 (u16)(0xffff)
#define MAX_u32 (u32)(0xffffffff)
#define MAX_u64 (u64)(0xffffffffffffffff)

#define MAX_i8  (i8 )(0x7f)
#define MAX_i16 (i16)(0x7fff)
#define MAX_i32 (i32)(0x7fffffff)
#define MAX_i64 (i64)(0x7fffffffffffffff)

#define false 0
#define true 1

#include "DataStructures.h"

#endif //DSV_DATATYPES_H
