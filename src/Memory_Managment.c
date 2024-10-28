#include "Memory_Managment.h"
#include <windows.h>
#include "Utils.h"
#include "Math.h"

Mem globalMem;

void InitializeArena(Memory_Arena *arena, void *base, u32 size) {
    arena->used = 0;
    arena->size = size;
    arena->base = base;
}

void *ArenaPush(Memory_Arena *arena, WordSize size) {
    Assert((arena->used + size) < arena->size);
    void *ret = (byte *)arena->base + arena->used;
    arena->used += size;
    return ret;
}

Memory_Arena *AllocTempArena(u32 size) {
    Assert(globalMem.temp.tempArenas.size < TEMP_ARENA_COUNT);
    Temp_Arena *tempArena = &globalMem.temp.tempArenas.A[globalMem.temp.tempArenas.size++];

    tempArena->allocated = true;

    InitializeArena(&tempArena->arena, ArenaPushMem(&globalMem.temp.arena, size), size);
    return &tempArena->arena;
}

void FreeTempArena(Memory_Arena *arena) {
    Assert((byte *)globalMem.temp.tempArenas.A <= (byte *)arena);

    u32 arenaIndex = (u32)((byte *)arena - (byte *)globalMem.temp.tempArenas.A) / sizeof(Temp_Arena);

    Assert(arenaIndex < globalMem.temp.tempArenas.size);

    globalMem.temp.tempArenas.A[arenaIndex].allocated = false;

    for (s32 i = globalMem.temp.tempArenas.size - 1; i >= 0; --i) {
        Temp_Arena *tempArena = &globalMem.temp.tempArenas.A[i];

        if (tempArena->allocated) { break; }

        --globalMem.temp.tempArenas.size;
        globalMem.temp.arena.used -= tempArena->arena.size;
        tempArena->arena.base = 0;
    }
}

void InitializeMem(void *baseAlloc) {
    u32 dwarf_size   = MiB(16);
    u32 txtEd_size   = MiB(16);
    u32 strings_size = MiB(16);

    u32 temp_size  = MiB(256);
    u32 perma_size = MiB(256);

    globalMem.baseAddress = VirtualAlloc(baseAlloc, perma_size + temp_size + (TEMP_ARENA_COUNT * sizeof(Temp_Arena)),
                                    MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

    InitializeArena(&globalMem.perma.arena, globalMem.baseAddress, perma_size);
    globalMem.temp.tempArenas.A = (Temp_Arena *)((byte *)globalMem.baseAddress + perma_size);
    InitializeArena(&globalMem.temp.arena, (byte *)globalMem.baseAddress + perma_size + (TEMP_ARENA_COUNT * sizeof(Temp_Arena)),
                    temp_size);

    InitializeArena(&globalMem.perma.dwarf, ArenaPushMem(&globalMem.perma.arena, dwarf_size), dwarf_size);
    InitializeArena(&globalMem.perma.strings, ArenaPushMem(&globalMem.perma.arena, strings_size), strings_size);
}
