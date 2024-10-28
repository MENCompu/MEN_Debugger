#ifndef DSV_OS_H
#define DSV_OS_H

#include "Registers.h"

// Processes
#ifdef _WIN32
    typedef HANDLE Process_ID;
    typedef HANDLE Thread_ID;
#if 0
    typedef struct {
        HANDLE handle;
        HANDLE ID;
    } Process_ID;
    typedef struct {
        HANDLE handle;
        HANDLE ID;
    } Thread_ID;
#endif
#else // UNIX
    typedef pid_t  Process_ID;
    typedef pid_t  Thread_ID;
#endif

Array_Ptr(Array_Thread_ID, Thread_ID);

typedef struct {
    Process_ID procID;
    Array_Thread_ID threadIDs;
} Process_Info;

#define MACRO_WriteProcMem(name) b32 name(Process_ID procID, void *localMem, void *procMem, u64 size)
MACRO_WriteProcMem(WriteProcMem);
#define WriteProcMemType(procID, localMem, procMem) WriteProcMem(procID, localMem, procMem, sizeof(*localMem))

#define MACRO_WriteProcCode(name) b32 name(Process_ID procID, void *localMem, void *procMem, u64 size)
MACRO_WriteProcMem(WriteProcCode);
#define WriteProcCodeType(procID, localMem, procMem) WriteProcCode(procID, localMem, procMem, sizeof(*localMem))

#define MACRO_ReadProcMem(name) b32 name(Process_ID procID, void *localMem, void *procMem, u64 size)
MACRO_ReadProcMem(ReadProcMem);
#define ReadProcMemType(procID, localMem, procMem) ReadProcMem(procID, localMem, procMem, sizeof(*localMem))

#define MACRO_GetPC(name) u64 name(Thread_ID threadID);
MACRO_GetPC(GetPC);

#define MACRO_SetPC(name) void name(Thread_ID threadID, u64 value)
MACRO_SetPC(SetPC);

#define MACRO_GetRbp(name) u64 name(Thread_ID threadID);
MACRO_GetRbp(GetRbp);

#define MACRO_SingleStep(name) void name(Process_Info *procInfo)
MACRO_SingleStep(SingleStep);

#define MACRO_ContinueProcess(name) b32 name(Process_Info *procInfo)
MACRO_ContinueProcess(ContinueProcess);

#define MACRO_GetRegister(name) u64 name(Thread_ID threadID, Register_Enum reg)
MACRO_GetRegister(GetRegister);

#define MACRO_SetRegister(name) void name(Thread_ID threadID, Register_Enum reg, u64 value)
MACRO_SetRegister(SetRegister);

#define MACRO_LoadRegisters(name) b32 name(Thread_ID threadID, Registers *regs)
MACRO_LoadRegisters(LoadRegisters);

// File I/O
typedef struct {
    WordSize size;
    void *memory;
} File;

#define MACRO_ReadEntireFile(name) b32 name(File *file, char *fileName)
//typedef MACRO_ReadEntireFile(FuncPtr_ReadEntireFile);
MACRO_ReadEntireFile(ReadEntireFile);

#define MACRO_WriteNewFile(name) b32 name(char *newFilePath, void *fileMemory, u32 fileSize)
//typedef MACRO_WriteNewFile(FuncPtr_WriteNewFile);
MACRO_WriteNewFile(WriteNewFile);

#define MACRO_FreeFileMemory(name) void name(File *file)
//typedef MACRO_FreeFileMemory(FuncPtr_FreeFileMemory);
MACRO_FreeFileMemory(FreeFileMemory);

#endif //DSV_OS_H
