#ifndef DSV_MEMORY_UTILS_H
#define DSV_MEMORY_UTILS_H

#define ShiftMemRight(mem, size, shifts) CopyBackward(((byte *)(mem) + (shifts)), mem, size)
#define ShiftMemRightPtr(mem, ptr, shifts) \
    do { \
        Assert((byte *)(ptr) <= (byte *)(mem)); \
        ShiftMemRight(mem, (u32)((byte *)(mem) - (byte *)(ptr) + 1), shifts); \
    } while (0)

Inter void CopyBackward(void *dest, void *src, u32 size) {
    byte *scanSrc  = (byte *)src;
    byte *scanDest = (byte *)dest;

    while (size--) { *scanDest-- = *scanSrc--; }
}

#define ShiftMemLeft(mem, size, shifts) CopyForward((mem) - (shifts), mem, size)
#define ShiftMemLeftPtr(mem, ptr, shifts) \
    do { \
        Assert((byte *)(ptr) >= (byte *)(mem)); \
        ShiftMemLeft(mem, (u32)((byte *)(mem) - (byte *)(ptr) + 1), shifts); \
    } while (0)

#define ArrayCopy(dest, src, size) CopyForward(dest, src, size * sizeof((src)[0]))
Inter void CopyForward(void *dest, void *src, u32 size) {
    byte *scanSrc  = (byte *)src;
    byte *scanDest = (byte *)dest;

    while (size--) { *scanDest++ = *scanSrc++; }
}

Inter void *FindAnyDiffByteForward(void *mem, void *end, void *bytesToFind, u32 sizeBytes, u32 timesToIgnore) {
    u32 timesLeft = timesToIgnore;

    byte *scan = (byte *)mem;
    byte *lastByteFind = 0;
    byte *bytes = (byte *)bytesToFind;

    while (scan <= (byte *)end) {
        for (u32 i = 0; i < sizeBytes; ++i) {
            //printf("%u == %u\n", (u32)*scan, (u32)bytes[i]);
            if (*scan == bytes[i]) { goto skip_byte; }
        }

        if (timesLeft-- == 0) { return scan; }
        lastByteFind = scan;

skip_byte:
        ++scan;
    }

    return lastByteFind;
}

Inter void *FindAnyDiffByteBackward(void *mem, void *end, void *bytesToFind, u32 sizeBytes, u32 timesToIgnore) {
    u32 timesLeft = timesToIgnore;

    byte *scan = (byte *)mem;
    byte *lastByteFind = 0;
    byte *bytes = (byte *)bytesToFind;

    while (scan >= (byte *)end) {
        for (u32 i = 0; i < sizeBytes; ++i) {
            if (*scan == bytes[i]) { goto skip_byte; }
        }

        if (timesLeft-- == 0) { return scan; }
        lastByteFind = scan;

skip_byte:
        --scan;
    }

    return lastByteFind;
}

Inter void *FindAnyByteForward(void *mem, void *end, void *bytesToFind, u32 sizeBytes, u32 timesToIgnore) {
    u32 timesLeft = timesToIgnore;

    byte *scan = (byte *)mem;
    byte *lastByteFind = 0;
    byte *bytes = (byte *)bytesToFind;

    while (scan <= (byte *)end) {
        for (u32 i = 0; i < sizeBytes; ++i) {
            if (*scan == bytes[i]) {
                if (timesLeft-- == 0) { return scan; }
                lastByteFind = scan;
                break;
            }
        }

        ++scan;
    }

    return lastByteFind;
}

Inter void *FindAnyByteBackward(void *mem, void *end, void *bytesToFind, u32 sizeBytes, u32 timesToIgnore) {
    u32 timesLeft = timesToIgnore;

    byte *scan = (byte *)mem;
    byte *lastByteFind = 0;
    byte *bytes = (byte *)bytesToFind;

    while (scan >= (byte *)end) {
        for (u32 i = 0; i < sizeBytes; ++i) {
            if (*scan == bytes[i]) {
                if (timesLeft-- == 0) { return scan; }
                lastByteFind = scan;
                break;
            }
        }

        ++scan;
    }

    return lastByteFind;
}

#define FindByteForward(mem, size, byteToFind) FindByteForwardTimes(mem, (byte *)mem + size, byteToFind, 0)
Inter void *FindByteForwardTimes(void *mem, void *end, byte byteToFind, u32 timesToIgnore) {
    u32 timesLeft = timesToIgnore;

    byte *scan = (byte *)mem;
    byte *lastByteFind = 0;

    while (scan <= (byte *)end) {
    	if (*scan == byteToFind) {
            if (timesLeft-- == 0) { return scan; }
            lastByteFind = scan;
	    }

        ++scan;
    }

    return lastByteFind;
}

#define FindByteBackwards(mem, size, byteToFind) FindByteBackwardsTimes(mem, size, byteToFind, 0)
Inter u32 FindByteBackwardsTimes(void* mem, u32 size, byte byteToFind, u32 timesToIgnore) {
    u32 timesLeft = timesToIgnore;

    byte* scan = (byte*)mem;
    u32 lastByteFind = MAX_U32;

    for (u32 index = 0; index < size; ++index) {
    	if (*scan == byteToFind) {
            if (timesLeft-- == 0) { return index; }
            lastByteFind = index;
	    }

        --scan;
    }

    return lastByteFind;
}

#define ZeroMem(mem, size) FillMemWithByte(mem, size, 0)
#define ZeroType(type) ZeroMem(type, sizeof(*(type)))
Inter void FillMemWithByte(void *mem, u32 size, byte value) {
    byte *scan = (byte *)mem;

    while (size--) { *scan++ = value; }
}

Inter s32 CompMem(void *mem1, void *mem2, u32 size) {
    byte *scan1 = (byte *)mem1;
    byte *scan2 = (byte *)mem2;

    while (size--) {
        if (*scan1 != *scan2) {
            return (*scan1 < *scan2) ? 1 : -1;
        }

        scan1++;
        scan2++;
    }

    return 0;
}

#endif //DSV_MEMORY_UTILS_H
