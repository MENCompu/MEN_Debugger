#ifndef DSV_DATASTRUCTURES_H
#define DSV_DATASTRUCTURES_H

#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))

#define Array_Ptr(name, type) \
    typedef struct {          \
        u32 size;             \
        type *A;              \
    } name

#define NulArray_Ptr(name, type) typedef type *name;

#define Array(name, sizeArray, type) \
    typedef struct {           \
        u32 capacity;          \
        u32 size;              \
        type A[sizeArray];     \
    } name

#define ArrayAlloc(array, arraySize, arena)                             \
    (array).A = ArenaPush((arena), sizeof((array).A[0]) * (arraySize)); \
    (array).size = arraySize;

Array_Ptr(Array_byte_Ptr, byte *);
Array_Ptr(Array_byte, byte);
Array_Ptr(Array_u64, u64);
Array_Ptr(Array_u32, u32);
Array_Ptr(Array_s64, s64);

#define AddFlags(value, flags) ((value) |= (flags))
#define DelFlags(value, flags) ((value) &= ~(flags))
#define SwitchFlags(value, flags) ((value) ^= (flags))
#define HasFlags(value, flags) (((value) & (flags)) == (flags))
#define HasAnyFlag(value, flags) (((value) & (flags)) != 0)

#define Bit_Flags(name, bitsCount)              \
    typedef struct {                            \
        u32 W[((bitsCount) - 1) / sizeof(u32)]; \
    } name

#define ToBits(bytes) ((bytes) * 8)

#define AtWordBitFlag(bitFlags, bit)  ((bitFlags)->W[bit / ToBits(sizeof((bitFlags)->W[0]))])
#define BitMaskBitFlag(bitFlags, bit) (1 << (bit % ToBits(sizeof((bitFlags)->W[0]))))
#define SetBitFlag(bitFlags, bit)     (AtWordBitFlag(bitFlags, bit) |=  BitMaskBitFlag(bitFlags, bit))
#define UnsetBitFlag(bitFlags, bit)   (AtWordBitFlag(bitFlags, bit) &= ~BitMaskBitFlag(bitFlags, bit))
#define SwitchBitFlag(bitFlags, bit)  (AtWordBitFlag(bitFlags, bit) ^=  BitMaskBitFlag(bitFlags, bit))
#define IsBitFlagSet(bitFlags, bit)   ((AtWordBitFlag(bitFlags, bit) & BitMaskBitFlag(bitFlags, bit)) != 0)

#define Stack(name, type, capacity, funcPopName, funcPushName, funcTopIndexName) \
    typedef struct { \
        u32 size; \
        type S[capacity]; \
    } name; \
\
    Inter type *funcPopName(name *stack) { \
        if (stack->size == 0) { return 0; } \
        return &stack->S[--stack->size]; \
    } \
\
    Inter type *funcPushName(name *stack) { \
        if (stack->size == capacity) { return 0; } \
        return &stack->S[stack->size++]; \
    } \
\
    Inter type *funcTopIndexName(name *stack, u32 index) { \
        if ((stack->size - 1 - index) >= 0) { return 0; } \
        return &stack->S[(stack->size - 1) - index]; \
    } \

#endif //DSV_DATASTRUCTURES_H
