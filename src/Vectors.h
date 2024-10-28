#ifndef TE_VECTORS_H
#define TE_VECTORS_H

#include "DataTypes.h"

#define AXIS_HOR  0
#define AXIS_VERT 1

// TODO(JENH): Change vA_blah for blahxA.

typedef union {
    struct {
        s32 x;
        s32 y;
    };
    struct {
        s32 width;
        s32 height;
    };
    s32 E[2];
} s32x2;

typedef union {
    struct {
        u32 x;
        u32 y;
    };
    struct {
        u32 width;
        u32 height;
    };
    u32 E[2];
} u32x2;

typedef union {
    struct {
        f32 x;
        f32 y;
    };
    struct {
        f32 width;
        f32 height;
    };
    f32 E[2];
} f32x2;

typedef union {
    struct {
        s32 x;
        s32 y;
        s32 z;
    };
    struct {
        s32 red;
        s32 green;
        s32 blue;
    };
    struct {
        s32 width;
        s32 height;
        s32 depth;
    };
    s32 E[3];
} s32x3;

typedef union {
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
    struct {
        f32 red;
        f32 green;
        f32 blue;
    };
    struct {
        f32 width;
        f32 height;
        f32 depth;
    };
    f32 E[3];
} f32x3;

typedef union {
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };
    struct {
        f32 red;
        f32 green;
        f32 blue;
        f32 alpha;
    };
    struct {
        f32 width;
        f32 height;
        f32 depth;
    };
    f32x3 xyz;
    f32 E[3];
} f32x4;

Inter s32x2 V2_s32(s32 a, s32 b) {
    s32x2 result;
    result.x = a;
    result.y = b;
    return result;
}

Inter s32x2 operator+(s32x2 a, s32x2 b) {
    s32x2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

Inter s32x2 &operator+=(s32x2 &_this, s32x2 a) {
    _this = _this + a;
    return _this;
}

Inter s32x2 operator-(s32x2 a) {
    s32x2 result;
    result.x = -a.x;
    result.y = -a.y;
    return result;
}

Inter s32x2 operator-(s32x2 a, s32x2 b) {
    s32x2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

Inter s32x2 &operator-=(s32x2 &_this, s32x2 a) {
    _this = _this - a;
    return _this;
}

Inter s32x2 operator*(s32x2 a, f32 b) {
    s32x2 result;
    result.x = (s32)(a.x * b);
    result.y = (s32)(a.y * b);
    return result;
}

Inter s32x2 operator*(f32 a, s32x2 b) {
    s32x2 result;
    result.x = (s32)(a * b.x);
    result.y = (s32)(a * b.y);
    return result;
}

Inter s32x2 &operator*=(s32x2 &_this, f32 a) {
    _this = _this * a;
    return _this;
}

//
// V2 u32
//

Inter u32x2 V2_u32(u32 a, u32 b) {
    u32x2 result;
    result.x = a;
    result.y = b;
    return result;
}

Inter u32x2 operator+(u32x2 a, u32x2 b) {
    u32x2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

Inter u32x2 &operator+=(u32x2 &_this, u32x2 a) {
    _this = _this + a;
    return _this;
}

Inter s32x2 operator-(u32x2 a) {
    s32x2 result;
    result.x = -(s32)a.x;
    result.y = -(s32)a.y;
    return result;
}

Inter u32x2 operator-(u32x2 a, u32x2 b) {
    u32x2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

Inter u32x2 &operator-=(u32x2 &_this, u32x2 a) {
    _this = _this - a;
    return _this;
}

Inter u32x2 operator*(u32x2 a, f32 b) {
    u32x2 result;
    result.x = (u32)(a.x * b);
    result.y = (u32)(a.y * b);
    return result;
}

Inter u32x2 operator*(f32 a, u32x2 b) {
    u32x2 result;
    result.x = (u32)(a * b.x);
    result.y = (u32)(a * b.y);
    return result;
}

Inter u32x2 &operator*=(u32x2 &_this, f32 a) {
    _this = _this * a;
    return _this;
}

//
// V2 f32
//

Inter f32x2 V2_f32(f32 a, f32 b) {
    f32x2 result;
    result.x = a;
    result.y = b;
    return result;
}

Inter f32x2 operator+(f32x2 a, f32x2 b) {
    f32x2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

Inter f32x2 &operator+=(f32x2 &_this, f32x2 a) {
    _this = _this + a;
    return _this;
}

Inter f32x2 operator-(f32x2 a) {
    f32x2 result;
    result.x = -a.x;
    result.y = -a.y;
    return result;
}

Inter f32x2 operator-(f32x2 a, f32x2 b) {
    f32x2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

Inter f32x2 &operator-=(f32x2 &_this, f32x2 a) {
    _this = _this - a;
    return _this;
}

Inter f32x2 operator*(f32x2 a, f32 b) {
    f32x2 result;
    result.x = a.x * b;
    result.y = a.y * b;
    return result;
}

Inter f32x2 operator*(f32 a, f32x2 b) {
    f32x2 result;
    result.x = a * b.x;
    result.y = a * b.y;
    return result;
}

Inter f32x2 &operator*=(f32x2 &_this, f32 a) {
    _this = _this * a;
    return _this;
}

//
// V3
//

Inter f32x3 V3_f32(f32 a, f32 b, f32 c) {
    f32x3 result;
    result.x = a;
    result.y = b;
    result.z = c;
    return result;
}

Inter f32x3 operator+(f32x3 a, f32x3 b) {
    f32x3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Inter f32x3 &operator+=(f32x3 &_this, f32x3 a) {
    _this = _this + a;
    return _this;
}

Inter f32x3 operator-(f32x3 a) {
    f32x3 result;
    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    return result;
}

Inter f32x3 operator-(f32x3 a, f32x3 b) {
    f32x3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

Inter f32x3 &operator-=(f32x3 &_this, f32x3 a) {
    _this = _this - a;
    return _this;
}

Inter f32x3 operator*(f32x3 a, f32 b) {
    f32x3 result;
    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    return result;
}

Inter f32x3 operator*(f32 a, f32x3 b) {
    f32x3 result;
    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;
    return result;
}

Inter f32x3 &operator*=(f32x3 &_this, f32 a) {
    _this = _this * a;
    return _this;
}

//
// V4 F32
//

Inter f32x4 V3_f32(f32 a, f32 b, f32 c, f32 d) {
    f32x4 result;
    result.x = a;
    result.y = b;
    result.z = c;
    result.w = d;
    return result;
}

Inter f32x4 operator+(f32x4 a, f32x4 b) {
    f32x4 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    return result;
}

Inter f32x4 &operator+=(f32x4 &_this, f32x4 a) {
    _this = _this + a;
    return _this;
}

Inter f32x4 operator-(f32x4 a) {
    f32x4 result;
    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    result.w = -a.w;
    return result;
}

Inter f32x4 operator-(f32x4 a, f32x4 b) {
    f32x4 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;
    return result;
}

Inter f32x4 &operator-=(f32x4 &_this, f32x4 a) {
    _this = _this - a;
    return _this;
}

Inter f32x4 operator*(f32x4 a, f32 b) {
    f32x4 result;
    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    result.w = a.w * b;
    return result;
}

Inter f32x4 operator*(f32 a, f32x4 b) {
    f32x4 result;
    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;
    result.w = a * b.w;
    return result;
}

Inter f32x4 &operator*=(f32x4 &_this, f32 a) {
    _this = _this * a;
    return _this;
}
// functions

inline f32x2 HadamardProd(f32x2 a, f32x2 b) {
    return {a.x * b.x, a.y * b.y};
}

inline f32x2 HadamardDiv(f32x2 a, f32x2 b) {
    return {a.x / b.x, a.y / b.y};
}

inline u32x2 HadamardProd(u32x2 a, u32x2 b) {
    return {a.x * b.x, a.y * b.y};
}

inline u32x2 HadamardDiv(u32x2 a, u32x2 b) {
    return {a.x / b.x, a.y / b.y};
}

inline f32x3 HadamardProd(f32x3 a, f32x3 b) {
    return {a.x * b.x, a.y * b.y, a.z * b.z};
}

inline f32x3 HadamardDiv(f32x3 a, f32x3 b) {
    return {a.x / b.x, a.y / b.y, a.z / b.z};
}

inline f32x4 HadamardProd(f32x4 a, f32x4 b) {
    return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

inline f32x4 HadamardDiv(f32x4 a, f32x4 b) {
    return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

inline s32x2 Tos32(f32x2 a) {
    s32x2 result;
    result.x = (s32)a.x;
    result.y = (s32)a.y;
    return result;
}

inline s32x2 Tos32(u32x2 a) {
    s32x2 result;
    result.x = (s32)a.x;
    result.y = (s32)a.y;
    return result;
}

inline s32x3 Tos32(f32x3 a) {
    s32x3 result;
    result.x = (s32)a.x;
    result.y = (s32)a.y;
    result.y = (s32)a.z;
    return result;
}

inline u32x2 Tou32(s32x2 a) {
    u32x2 result;
    result.x = (u32)a.x;
    result.y = (u32)a.y;
    return result;
}

inline u32x2 Tou32(f32x2 a) {
    u32x2 result;
    result.x = (u32)a.x;
    result.y = (u32)a.y;
    return result;
}

inline f32x2 Tof32(s32x2 a) {
    f32x2 result;
    result.x = (f32)a.x;
    result.y = (f32)a.y;
    return result;
}

inline f32x2 Tof32(u32x2 a) {
    f32x2 result;
    result.x = (f32)a.x;
    result.y = (f32)a.y;
    return result;
}

inline f32x3 Tof32(s32x3 a) {
    f32x3 result;
    result.x = (f32)a.x;
    result.y = (f32)a.y;
    result.y = (f32)a.z;
    return result;
}

#endif //TE_VECTORS_H
