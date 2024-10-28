#ifndef TE_MEMORY_H
#define TE_MEMORY_H

#include "DataTypes.h"
#include <stdio.h>

#define UNDETERMINED_SIZE 0
#define TEMP_ARENA_COUNT  1024

typedef struct {
    WordSize size;
    WordSize used;
    void *base;
} Memory_Arena;

typedef struct {
    b32 allocated;
    Memory_Arena arena;
} Temp_Arena;

Array_Ptr(Array_Temp_Arena, Temp_Arena);

typedef struct {
    Array_Temp_Arena tempArenas;
    Memory_Arena arena;
} Temp_Mem;

typedef struct {
    struct {
        Memory_Arena dwarf;
        Memory_Arena strings;
    };
    Memory_Arena arena;
} Perma_Mem;

typedef struct {
    void *baseAddress;

    Temp_Mem  temp;
    Perma_Mem perma;
} Mem;

Mem globalMem;

#define ArenaPushArray(arena, type, size) (type *)ArenaPush(arena, sizeof(type) * (size))
#define ArenaPushMem(arena, size) (byte *)ArenaPush(arena, sizeof(byte) * (size))
#define ArenaPushType(arena, type) (type *)ArenaPush(arena, sizeof(type))

void InitializeMem(void *baseAlloc);
void InitializeArena(Memory_Arena *arena, void *base, u32 size);
void *ArenaPush(Memory_Arena *arena, WordSize size);
Memory_Arena *AllocTempArena(u32 size);
void FreeTempArena(Memory_Arena *arena);

#endif //TE_MEMORY_H
