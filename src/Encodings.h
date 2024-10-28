#ifndef DSV_ENCODINGS_H
#define DSV_ENCODINGS_H

Inter u64 ULEB128(byte *valueRaw) {
    u64 ret = 0;
    u32 shift = 0;
    byte *scan = valueRaw - 1;

    do {
        ret |= (*(++scan) & 0b01111111) << shift;
        shift += 7;
    } while (*scan & 0b10000000);

    //*bytesCount = scan - valueRaw + 1;
    return ret;
}

Inter inline u64 ULEB128AndAdvance(byte **valueRawPtr) {
    u64 ret = 0;
    u32 shift = 0;
    --(*valueRawPtr);

    do {
        ret |= (*(++(*valueRawPtr)) & 0b01111111) << shift;
        shift += 7;
    } while ((**valueRawPtr) & 0b10000000);

    ++(*valueRawPtr);
    return ret;
}

Inter inline s64 SLEB128AndAdvance(byte **valueRawPtr) {
    s64 ret = 0;
    u32 shift = 0;
    --(*valueRawPtr);

    do {
        ret |= (*(++(*valueRawPtr)) & 0b01111111) << shift;
        shift += 7;
    } while ((**valueRawPtr) & 0b10000000);

    Assert(shift < (sizeof(ret) * 8));

    if (((**valueRawPtr) & 0b01000000) != 0) {
        ret |= ~(u64)0 << shift;
    }

    ++(*valueRawPtr);
    return ret;
}

#endif //DSV_ENCODINGS_H
