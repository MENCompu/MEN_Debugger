#ifndef TE_STRING_H
#define TE_STRING_H

#include "Memory_Utils.h"

#define P(a, b) a##b

#define Local_Str(strName, bufSize) Local_Str_(strName, bufSize, __LINE__)
#define Local_Str_(strName, bufSize, counter) Local_Str__(strName, bufSize, counter)
#define Local_Str__(strName, bufSize, counter) \
    u8 P(dummyBufferName, counter) [bufSize]; \
    String strName; \
    strName.size = 0; \
    strName.str = P(dummyBufferName, counter); \
    ZeroMem(P(dummyBufferName, counter), bufSize)

typedef struct {
    u32 length;
} LengthBaseString;

#define BACKWARDSTRING(address, length) ((char *)(address) - (length))

typedef struct {
    u32 length;
    char str[0];
} Len_Preix_String;

typedef struct {
    union {
        u32 len;
        u32 length;
        u32 size;
    };
    char *str;
} String;

typedef char *CString;
Array_Ptr(Array_CString, CString);

#define LitToStr(strLit) Str((char *)strLit, ArrayCount(strLit))
Inter String Str(char *str, u32 size) {
    String ret;
    ret.size = size;
    ret.str = str;
    return ret;
}

Inter u32 Str_Find_Char_Backward(String string, u8 charToFind) {
    return FindByteBackwards(string.str + string.size - 1, string.size, (byte)charToFind);
}

#define CapTop(cap, val) Min(cap, val)
Inter String Str_Skip_End(String string, u32 skip) {
    u32 cappedSkip = CapTop(string.size, skip);
    String ret = (String){ .str = string.str, .size = (string.size - cappedSkip) };
    return ret;
}

#define CopyCStrBackwardToPointer(dest, src, end)         \
    CopyCStrBackward(dest, src, (u32)((src) - (end) + 1))

#define CopyCStrForward(dest, src, size) CopyForward(dest, src, size * sizeof((src)[0]))
#define AllocAndCopyCStrForward(arena, dest, src, size) \
    (dest) = ArenaPush(arena, sizeof((src)[0]) * size); \
    CopyCStrForward(dest, src, size)

#define CopyCStrBackward(dest, src, size) CopyBackward(dest, src, size * sizeof((src)[0]))
#define AllocAndCopyCStrBackward(arena, dest, src, size) \
    (dest) = ArenaPush(arena, sizeof((src)[0]) * size);  \
    CopyCStrBackward(dest, src, size)

#define CopyStrForward(dest, src) CopyForward(dest.str, src.str, src.size * sizeof((src).str[0]))
#define AllocAndCopyStrForward(arena, dest, src)                                \
    do { \
        (dest).str = (char *)ArenaPush(arena, sizeof((src).str[0]) * ((src).size + 1)); \
        CopyStrForward(dest, src); \
        (dest).size = (src).size; \
    } while (0)

#define CopyStrBackward(dest, src) CopyBackward(dest.str, src.str, src.size * sizeof((src).str[0]))
#define AllocAndCopyStrBackward(arena, dest, src)                     \
    (dest).str = ArenaPush(arena, sizeof((src).str[0]) * (src).size); \
    CopyStrBackward(dest, src)

#define FindAnyDiffCharForward(mem, end, chars, timesToIgnore) \
    (char *)FindAnyDiffByteForward(mem, end, chars.str, chars.size, timesToIgnore)

#define FindAnyDiffCharBackward(mem, end, chars, sizeChars, timesToIgnore) \
    (char *)FindAnyDiffByteBackward(mem, end, chars, sizeChars, timesToIgnore)

#define FindAnyCharForward(mem, end, chars, timesToIgnore) \
    (char *)FindAnyByteForward(mem, end, chars.str, chars.size, timesToIgnore)

#define FindAnyCharBackward(mem, end, chars, sizeChars, timesToIgnore) \
    (char *)FindAnyByteBackward(mem, end, chars, sizeChars, timesToIgnore)

#define FindCharForwardTimes(mem, end, charToFind, timesToIgnore) \
    (char *)FindByteForwardTimes(mem, end, charToFind, timesToIgnore)

#define FindCharBackwardsTimes(mem, end, charToFind, timesToIgnore) \
    (char *)FindByteBackwardsTimes(mem, end, charToFind, timesToIgnore)

#define FindCharForward(string, charToFind) (char *)FindByteForward((string).str, (string).size, charToFind)
Inter inline u32 FindCharForwardX(String string, char charToFind) {
    char *ptr = FindCharForward(string, charToFind);
    return (ptr == 0) ? MAX_u32 : (u32)(ptr - string.str);
}

#define CompCStrSize(str1, str2, size) CompMem(str1, str2, size)

Inter s32 CompCStr(CString str1, CString str2) {
    char *scan1 = str1;
    char *scan2 = str2;

    while (*scan1 != '\0') {
        if (*scan1 != *scan2) {
            return (*scan1 > *scan2) ? 1 : -1;
        }

        ++scan1;
        ++scan2;
    }

    return (*scan2 == '\0') ? 0 : -1;
}

Inter u32 CStrLen(CString str) {
    char *scan = &str[0];

    while (*scan++ != '\0');

    u32 len = (u32)(scan - &str[0]);
    return len;
}

Inter inline char DigitToChar(u8 value) {
    return (char)(value + 48);
}

Inter inline u8 CharToDigit(char digitChar) {
    return (u8)(digitChar - 48);
}

Inter void S32ToCStr(CString str, s32 value, u32 digitsCount) {
    char *scan = str + digitsCount - 1;

    while (digitsCount--) {
        u8 mod = (u8)value % 10;
        value /= 10;
        char digitChar = DigitToChar(mod);
        *scan-- = digitChar;
    }
}

Inter s32 StrToS32(String string) {
    s32 ret = 0;

    for (u32 i = 0; i < string.size; ++i) {
        ret = (10 * ret) + CharToDigit(string.str[i]);
    }

    return ret;
}

Inter s32 CompStr(String str1, String str2) {
    if (str1.size != str2.size) {
        return (str1.size > str2.size) ? 1 : -1;
    }

    return CompCStrSize(str1.str, str2.str, str1.size);
}

Inter void CatStr(String *strOut, String str1, String str2) {
    CopyCStrForward(strOut->str, str1.str, str1.size);
    CopyCStrForward(strOut->str + str1.size, str2.str, str2.size);
    strOut->size = str1.size + str2.size;
}

Inter void ParseCommands(CString input, CString *commands, u32 *commandsCount) {
    char *garbage = (char *)MAX_VALUE_PTR;

    char *scan = input;

    while (INFINITE_LOOP) {
	    ++(*commandsCount);

        scan = FindAnyDiffCharForward(scan, garbage, LitToStr(" \t\0"), 0);

	    if (*scan == '\"') {
	        *commands++ = ++scan;

	        char charsLookingFor2[] = "\"\0";

            scan = FindAnyCharForward(scan, garbage, LitToStr("\"\0"), 0);

	        *scan++ = '\0';

	        if (*scan == '\0') { return; }
	    } else {
	        *commands++ = scan;

            scan = FindAnyCharForward(scan, garbage, LitToStr(" \t\0"), 0);

            if (*scan == '\0') { return; }

            *scan++ = '\0';
        }
    }
}

#if 1
//TODO(JENH): End variable shouldn't be neccesary.
Inter char *GetCharLocationInLineByHorSpatialValue(char *string, char *end, u32 *horSpatialValue) {
    char *scanner = string;
    u32 lineWidth = 0;

    while (*scanner != '\r' && *scanner != '\n' && scanner < end) {
        if (lineWidth >= *horSpatialValue) { break; }
        lineWidth += (*scanner++ == '\t') ? 4 : 1;
    }

    if (*scanner == '\r') { ++scanner; }

    *horSpatialValue = lineWidth;
    return scanner;
}

Inter u32 GetSpatialLineWidthBackward(char *string) {
    char *scanner = string;
    u32 result = 0;

    while (*scanner != '\n') {
	    result += (*scanner-- == '\t') ? 4 : 1;
    }

    return result;
}

#if 0
Inter s32x2 GetCursorDisplacementForward(char *cursorInText, char *end, u32 tabToChars, u32 cursorPositionX) {
    s32x2 result {0, 0};

    char *scanner = cursorInText;

    u32 tabCharCount = tabToChars - (cursorPositionX % tabToChars);

    while (scanner < end) {
	if (*scanner != '\r') {
	    if (*scanner == '\t') {
	        result.x += tabCharCount;
            tabCharCount = 1;
	    } else if (*scanner == '\n') {
	        ++result.y;
	        result.x = 0;
            tabCharCount = 1;
	    } else {
	        ++result.x;
	    }

	    if (!--tabCharCount) {
	        tabCharCount = tabToChars;
	    }
	}

        scanner++;
    }

    return result;
}

Inter char *GetCursorPositionTillCharForward(char *string, char *end, char charLookingFor, u32 tabToChars,
		                               u32 occurrenceToFind, u32 *cursorPositionX, u32 *cursorPositionY) {
    char *scanner = string;
    char *lastCharFind = 0;

    u32 occurrencesLeft = occurrenceToFind;

    u32 tabCharCount = tabToChars - ((*cursorPositionX % tabToChars) + 1);

    while (scanner < end) {
        if (*scanner == charLookingFor) {
            lastCharFind = scanner;
            occurrencesLeft--;
        }

	    if (!occurrencesLeft) { break; }

        if (*scanner != '\r') {
            if (*scanner == '\t') {
                (*cursorPositionX) += tabCharCount;
                tabCharCount = 1;
            } else if (*scanner == '\n') {
                (*cursorPositionY)++;
                (*cursorPositionX) = 0;
                tabCharCount = 1;
            } else {
                (*cursorPositionX)++;
            }

            if (!--tabCharCount) {
                tabCharCount = tabToChars;
            }
        }

        scanner++;
    }

    return lastCharFind;
}

Inter void GetCursorPositionForward(char *string, char *end, u32 tabToChars, u32 *cursorPositionX,
		                       u32 *cursorPositionY) {
    char *scanner = string;

    u32 tabCharCount = tabToChars - (*cursorPositionX % tabToChars);

    while (scanner < end) {
        if (*scanner != '\r') {
            if (*scanner == '\t') {
                (*cursorPositionX) += tabCharCount;
                tabCharCount = 1;
            } else if (*scanner == '\n') {
                ++(*cursorPositionY);
                (*cursorPositionX) = 0;
                tabCharCount = 1;
            } else {
                ++(*cursorPositionX);
            }

            if (!--tabCharCount) {
                tabCharCount = tabToChars;
            }
        }

        scanner++;
    }
}
#endif
#endif

#endif //TE_STRING_H
