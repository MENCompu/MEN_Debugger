#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "DataTypes.h"

#include <windows.h>

#ifdef false
    #undef false
#endif
#ifdef true
    #undef true
#endif

#define Public static

typedef enum {
    false,
    true,
} b8;
#define MAX_U32 0xffffffff;

#include "Math.h"
#include "Memory_Managment.h"
#include "Memory_Managment.c"
#include "Utils.h"
#include "String.h"
#include "OS.h"
#include <winnt.h>

typedef HANDLE File_Handle;

#define OS_FILE_SEP '/'

Public String Get_Dir_Path(String filePath) {
    return Str_Skip_End(filePath, Str_Find_Char_Backward(filePath, OS_FILE_SEP));
}

Public void Build_Absolute_File_Path(String* out, u32 capacity, String path) {
    char* dummy = 0;
    (void)GetFullPathNameA(path.str, capacity, out->str, &dummy);
    out->size = CStrLen(out->str);
}

Public File_Handle File_Open(CString inPath) {
    // OPEN_ALWAYS
    File_Handle retHandle = (File_Handle)CreateFileA((char*)inPath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                                                     0, OPEN_EXISTING, 0, 0);
    return retHandle;
}

Public void File_Close(File_Handle inHandle) {
    CloseHandle((HANDLE)inHandle);
}

Public b8 File_Exists(CString path) {
    File_Handle file = File_Open(path);

    if ( file != INVALID_HANDLE_VALUE ) {
        File_Close(file);
        return true;
    }

    return false;
}

Public u32 File_Get_Size(File_Handle inHandle) {
    HANDLE handleOS = (HANDLE)inHandle;

    u32 ret;

	LARGE_INTEGER size;
	if (!GetFileSizeEx(handleOS, &size)) {
        printf("Couldn't get file size\n");
        Assert(false);
        return MAX_U32;
    }

    ret = (u32)size.QuadPart;

    return ret;
}

Public b8 File_Read(File_Handle handle, u32 bytesToRead, void* mem) {
    HANDLE handleOS = (HANDLE)handle;

    DWORD bytesRead;
    if (!ReadFile(handleOS, mem, (DWORD)bytesToRead, &bytesRead, 0) || bytesRead != bytesToRead) {
        printf("Failed to read the file at the amount requested\n");
        Assert(false);
        return false;
    }

    return true;
}

Public void* File_Read_All(String path, Memory_Arena* arena, u32* size) {
    u32 dummy = 0;
    if ( !size ) { size = &dummy; }

    void* retData;

    File_Handle file = File_Open(path.str);
    if ( file == INVALID_HANDLE_VALUE ) {
        return 0;
    }

    *size = File_Get_Size(file);
    retData = ArenaPushMem(arena, *size);

    Assert( File_Read(file, *size, retData) > 0 );

    File_Close(file);

    return retData;
}

u32 procID2   = 0;
u32 threadID2 = 0;

#include "PE.h"
#include "Debugger.h"

#define INVALID_REG_NAME 0xFFFFFFFF

void FreeFileMemory(File *file) {
    VirtualFree(file->memory, 0, MEM_RELEASE);
    file->size = 0;
}

b32 ReadEntireFile(File *file, char *path) {
    b32 ret = true;

    HANDLE fileHandle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

    if (fileHandle == INVALID_HANDLE_VALUE) {
        ERR("Couldn't open the file \"%s\".\n", path);
    }

    LARGE_INTEGER fileSizeLI;

    if (!GetFileSizeEx(fileHandle, &fileSizeLI)) {
        ERR("Couldn't get size of the specified file \"%s\".\n", path);
	    defer(false);
    }

    WordSize fileSize = (WordSize)fileSizeLI.QuadPart;

    void *memory = VirtualAlloc(0, fileSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

    if (!memory) {
        ERR("Couldn't allocate memory for the entire file \"%s\".\n", path);
	    defer(false);
    }

    DWORD bytesRead;

    if (!ReadFile(fileHandle, memory, fileSize, &bytesRead, 0) || bytesRead != fileSize) {
        ERR("Problems at reading the file \"%s\".\n", path);
        FreeFileMemory(memory);
	    defer(false);
    }

    file->size   = fileSize;
    file->memory = memory;

    return ret;

defer:
    CloseHandle(fileHandle);
    return ret;
}

b32 WriteNewFile(char *newFilePath, void *fileMemory, u32 fileSize) {
    b32 ret = false;

    HANDLE fileHandle = CreateFileA(newFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

    if (fileHandle == INVALID_HANDLE_VALUE) {
        ERR("Couldn't Create a file Handle \"%s\".\n", newFilePath);
	    return false;
    }

    DWORD bytesWritten;

    if (WriteFile(fileHandle, fileMemory, fileSize, &bytesWritten, 0)) {
        ret = bytesWritten == fileSize;
    } else {
        ERR("Couldn't write to the file \"%s\".\n", newFilePath);
	    ret = false;
    }

    CloseHandle(fileHandle);

    return ret;
}

#include "Dwarf.h"

void WaitChildProc(Process_ID procID) {
    while (INFINITE_LOOP) {
        DEBUG_EVENT event = {0};
        WaitForDebugEvent(&event, INFINITE);

        ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE);
    }
}

Inter b32 CreateAndTraceProcess(Process_Info *procInfo, CString binName, char *cmd) {
    STARTUPINFOA startupInfo = {0};

    startupInfo.cb = sizeof(STARTUPINFO);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    PROCESS_INFORMATION processInfo;

    //CREATE_SUSPENDED|DEBUG_PROCESS|DEBUG_ONLY_THIS_PROCESS
    b32 ret = (b32)CreateProcessA(binName, cmd, 0, 0, FALSE, DEBUG_ONLY_THIS_PROCESS, 0, 0, &startupInfo, &processInfo);
    procInfo->procID = processInfo.hProcess;
    procInfo->threadIDs.size = 1;
    procInfo->threadIDs.A[0] = processInfo.hThread;

    return ret;
}

void ContinueThread(Process_Info *ProcInfo) {
    //b32 ret = ContinueDebugEvent(event.dwProcessId, event.dwThreadId, status);
}

b32 WriteProcMem(Process_ID procID, void *localMem, void *procMem, u64 size) {
    WordSize bytesWritten;
    if (!WriteProcessMemory(procID, procMem, localMem, size, &bytesWritten)) {
        ERR("Couldn't write process memory.\n");
        return false;
    }

    b32 ret = (bytesWritten == size);
    return ret;
}

b32 WriteProcCode(Process_ID procID, void *localMem, void *procMem, u64 size) {
    b32 ret1 = WriteProcMem(procID, localMem, procMem, size);
    b32 ret2 = FlushInstructionCache(procID, procMem, size);
    return ret1;
}

b32 ReadProcMem(Process_ID procID, void *localMem, void *procMem, u64 size) {
    u64 bytesRead = 0;
    if (!ReadProcessMemory(procID, procMem, localMem, size, &bytesRead)) {
        ERR("Couldn't read process memory.\n");
        return false;
    }

    b32 ret = (bytesRead == size);
    return ret;
}

u64 GetPC(Thread_ID threadID) {
    CONTEXT context = {0};
    context.ContextFlags = CONTEXT_ALL;
    b32 ret = GetThreadContext(threadID, &context);
    return context.Rip;
}

void SetPC(Thread_ID threadID, u64 value) {
    CONTEXT context = {0};
    context.ContextFlags = CONTEXT_ALL;
    b32 ret1 = GetThreadContext(threadID, &context);

    context.Rip = value;

    b32 ret2 =  SetThreadContext(threadID, &context);
}

u64 GetRbp(Thread_ID threadID) {
    CONTEXT context = {0};
    context.ContextFlags = CONTEXT_ALL;
    b32 ret = GetThreadContext(threadID, &context);
    return context.Rbp;
}

u64 GetRegister(Thread_ID threadID, Register_Enum reg) {
    CONTEXT context = {0};
    context.ContextFlags = CONTEXT_ALL;
    b32 ret = GetThreadContext(threadID, &context);

    switch (reg) {
        case Reg_rax:    { return context.Rax;    } break;
        case Reg_rdx:    { return context.Rdx;    } break;
        case Reg_rcx:    { return context.Rcx;    } break;
        case Reg_rbx:    { return context.Rbx;    } break;
        case Reg_rsi:    { return context.Rsi;    } break;
        case Reg_rdi:    { return context.Rdi;    } break;
        case Reg_rbp:    { return context.Rbp;    } break;
        case Reg_rsp:    { return context.Rsp;    } break;
        case Reg_r8:     { return context.R8;     } break;
        case Reg_r9:     { return context.R9;     } break;
        case Reg_r10:    { return context.R10;    } break;
        case Reg_r11:    { return context.R11;    } break;
        case Reg_r12:    { return context.R12;    } break;
        case Reg_r13:    { return context.R13;    } break;
        case Reg_r14:    { return context.R14;    } break;
        case Reg_r15:    { return context.R15;    } break;
        case Reg_rip:    { return context.Rip;    } break;
        case Reg_rFLAGS: { return context.EFlags; } break;
        case Reg_es:     { return context.SegEs;  } break;
        case Reg_cs:     { return context.SegCs;  } break;
        case Reg_ss:     { return context.SegSs;  } break;
        case Reg_ds:     { return context.SegDs;  } break;
        case Reg_fs:     { return context.SegFs;  } break;
        case Reg_gs:     { return context.SegGs;  } break;

        case Reg_mxcsr: { return context.MxCsr; } break;

        NO_DEFAULT
    }

    return 0;
}

void SetRegister(Thread_ID threadID, Register_Enum reg, u64 value) {
    CONTEXT context = {0};
    context.ContextFlags = CONTEXT_ALL;
    b32 ret1 = GetThreadContext(threadID, &context);

    switch (reg) {
        case Reg_rax:    { context.Rax    = value; } break;
        case Reg_rdx:    { context.Rdx    = value; } break;
        case Reg_rcx:    { context.Rcx    = value; } break;
        case Reg_rbx:    { context.Rbx    = value; } break;
        case Reg_rsi:    { context.Rsi    = value; } break;
        case Reg_rdi:    { context.Rdi    = value; } break;
        case Reg_rbp:    { context.Rbp    = value; } break;
        case Reg_rsp:    { context.Rsp    = value; } break;
        case Reg_r8:     { context.R8     = value; } break;
        case Reg_r9:     { context.R9     = value; } break;
        case Reg_r10:    { context.R10    = value; } break;
        case Reg_r11:    { context.R11    = value; } break;
        case Reg_r12:    { context.R12    = value; } break;
        case Reg_r13:    { context.R13    = value; } break;
        case Reg_r14:    { context.R14    = value; } break;
        case Reg_r15:    { context.R15    = value; } break;
        case Reg_rip:    { context.Rip    = value; } break;
        case Reg_rFLAGS: { context.EFlags = value; } break;
        case Reg_es:     { context.SegEs  = value; } break;
        case Reg_cs:     { context.SegCs  = value; } break;
        case Reg_ss:     { context.SegSs  = value; } break;
        case Reg_ds:     { context.SegDs  = value; } break;
        case Reg_fs:     { context.SegFs  = value; } break;
        case Reg_gs:     { context.SegGs  = value; } break;
        case Reg_mxcsr:  { context.MxCsr  = value; } break;

        NO_DEFAULT
    }

    b32 ret2 = SetThreadContext(threadID, &context);
}

b32 LoadRegisters(Thread_ID threadID, Registers *regs) {
    CONTEXT context = {0};
    context.ContextFlags = CONTEXT_ALL;
    b32 ret = GetThreadContext(threadID, &context);

    regs->rax = context.Rax;
    regs->rdx = context.Rdx;
    regs->rcx = context.Rcx;
    regs->rbx = context.Rbx;
    regs->rsi = context.Rsi;
    regs->rdi = context.Rdi;
    regs->rbp = context.Rbp;
    regs->rsp = context.Rsp;
    regs->r8 = context.R8;
    regs->r9 = context.R9;
    regs->r10 = context.R10;
    regs->r11 = context.R11;
    regs->r12 = context.R12;
    regs->r13 = context.R13;
    regs->r14 = context.R14;
    regs->r15 = context.R15;
    regs->rip = context.Rip;
    regs->rFLAGS = context.EFlags;
    regs->es = context.SegEs;
    regs->cs = context.SegCs;
    regs->ss = context.SegSs;
    regs->ds = context.SegDs;
    regs->fs = context.SegFs;
    regs->gs = context.SegGs;

    return 0;
}

b32 PrintDebugEvent(DEBUG_EVENT *event) {
    switch (event->dwDebugEventCode) {
        case EXCEPTION_DEBUG_EVENT: {
            switch(event->u.Exception.ExceptionRecord.ExceptionCode) {
                case EXCEPTION_ACCESS_VIOLATION: { printf("EXCEPTION_ACCESS_VIOLATION\n"); } break;
                case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: { printf("EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n"); } break;
                case EXCEPTION_BREAKPOINT: { printf("EXCEPTION_BREAKPOINT\n"); } break;
                case EXCEPTION_DATATYPE_MISALIGNMENT: { printf("EXCEPTION_DATATYPE_MISALIGNMENT\n"); } break;
                case EXCEPTION_FLT_DENORMAL_OPERAND: { printf("EXCEPTION_FLT_DENORMAL_OPERAND\n"); } break;
                case EXCEPTION_FLT_DIVIDE_BY_ZERO: { printf("EXCEPTION_FLT_DIVIDE_BY_ZERO\n"); } break;
                case EXCEPTION_FLT_INEXACT_RESULT: { printf("EXCEPTION_FLT_INEXACT_RESULT\n"); } break;
                case EXCEPTION_FLT_INVALID_OPERATION: { printf("EXCEPTION_FLT_INVALID_OPERATION\n"); } break;
                case EXCEPTION_FLT_OVERFLOW: { printf("EXCEPTION_FLT_OVERFLOW\n"); } break;
                case EXCEPTION_FLT_STACK_CHECK: { printf("EXCEPTION_FLT_STACK_CHECK\n"); } break;
                case EXCEPTION_FLT_UNDERFLOW: { printf("EXCEPTION_FLT_UNDERFLOW\n"); } break;
                case EXCEPTION_ILLEGAL_INSTRUCTION: { printf("EXCEPTION_ILLEGAL_INSTRUCTION\n"); } break;
                case EXCEPTION_IN_PAGE_ERROR: { printf("EXCEPTION_IN_PAGE_ERROR\n"); } break;
                case EXCEPTION_INT_DIVIDE_BY_ZERO: { printf("EXCEPTION_INT_DIVIDE_BY_ZERO\n"); } break;
                case EXCEPTION_INT_OVERFLOW: { printf("EXCEPTION_INT_OVERFLOW\n"); } break;
                case EXCEPTION_INVALID_DISPOSITION: { printf("EXCEPTION_INVALID_DISPOSITION\n"); } break;
                case EXCEPTION_NONCONTINUABLE_EXCEPTION: { printf("EXCEPTION_NONCONTINUABLE_EXCEPTION\n"); } break;
                case EXCEPTION_PRIV_INSTRUCTION: { printf("EXCEPTION_PRIV_INSTRUCTION\n"); } break;
                case EXCEPTION_SINGLE_STEP: { printf("EXCEPTION_SINGLE_STEP\n"); } break;
                case EXCEPTION_STACK_OVERFLOW: { printf("EXCEPTION_STACK_OVERFLOW\n"); } break;
                default: { printf("default exception\n"); } break;
            }
        } break;

        case CREATE_THREAD_DEBUG_EVENT: { printf("CREATE_THREAD_DEBUG_EVENT\n"); } break;
        case CREATE_PROCESS_DEBUG_EVENT: { printf("CREATE_PROCESS_DEBUG_EVENT\n"); } break;
        case EXIT_THREAD_DEBUG_EVENT: { printf("EXIT_THREAD_DEBUG_EVENT\n"); } break;
        case EXIT_PROCESS_DEBUG_EVENT: { printf("EXIT_PROCESS_DEBUG_EVENT\n"); return true;} break;
        case LOAD_DLL_DEBUG_EVENT: { printf("LOAD_DLL_DEBUG_EVENT\n"); } break;
        case UNLOAD_DLL_DEBUG_EVENT: { printf("UNLOAD_DLL_DEBUG_EVENT\n"); } break;
        case OUTPUT_DEBUG_STRING_EVENT: { printf("OUTPUT_DEBUG_STRING_EVENT\n"); } break;
        case RIP_EVENT: { printf("RIP_EVENT\n"); } break;
        default: { printf("default event\n"); } break;
    }

    return false;
}

#define Trap_Flag 0x100

void SingleStep(Process_Info *procInfo) {
    CONTEXT context = {0};
    context.ContextFlags = CONTEXT_ALL;

    GetThreadContext(procInfo->threadIDs.A[0], &context);
    context.EFlags |= Trap_Flag;
    SetThreadContext(procInfo->threadIDs.A[0], &context);

    u32 status = DBG_CONTINUE;

    while (INFINITE_LOOP) {
        b32 ret = ContinueDebugEvent(procID2, threadID2, status);

        DEBUG_EVENT event = {0};
        WaitForDebugEvent(&event, INFINITE);

        procID2   = event.dwProcessId;
        threadID2 = event.dwThreadId;

        if (event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT &&
            event.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_SINGLE_STEP) {
            return;
        } else {
            PrintDebugEvent(&event);
        }
    }
}

b32 ContinueProcess(Process_Info *procInfo) {
    u32 evenrStatus = DBG_CONTINUE;

    while (INFINITE_LOOP) {
        ContinueDebugEvent(procID2 ,threadID2, evenrStatus);

        DEBUG_EVENT event = {0};
        WaitForDebugEvent(&event, INFINITE);

        procID2   = event.dwProcessId;
        threadID2 = event.dwThreadId;

        switch (event.dwDebugEventCode) {
            case EXCEPTION_DEBUG_EVENT: {
                switch(event.u.Exception.ExceptionRecord.ExceptionCode) {
                    case EXCEPTION_ACCESS_VIOLATION: { printf("EXCEPTION_ACCESS_VIOLATION\n"); } break;
                    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: { printf("EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n"); } break;

                    case EXCEPTION_BREAKPOINT: {
                        //printf("EXCEPTION_BREAKPOINT\n");
                        SetPC(procInfo->threadIDs.A[0], GetPC(procInfo->threadIDs.A[0]) - 1);
                        return false;
                    } break;

                    case EXCEPTION_DATATYPE_MISALIGNMENT: { printf("EXCEPTION_DATATYPE_MISALIGNMENT\n"); } break;
                    case EXCEPTION_FLT_DENORMAL_OPERAND: { printf("EXCEPTION_FLT_DENORMAL_OPERAND\n"); } break;
                    case EXCEPTION_FLT_DIVIDE_BY_ZERO: { printf("EXCEPTION_FLT_DIVIDE_BY_ZERO\n"); } break;
                    case EXCEPTION_FLT_INEXACT_RESULT: { printf("EXCEPTION_FLT_INEXACT_RESULT\n"); } break;
                    case EXCEPTION_FLT_INVALID_OPERATION: { printf("EXCEPTION_FLT_INVALID_OPERATION\n"); } break;
                    case EXCEPTION_FLT_OVERFLOW: { printf("EXCEPTION_FLT_OVERFLOW\n"); } break;
                    case EXCEPTION_FLT_STACK_CHECK: { printf("EXCEPTION_FLT_STACK_CHECK\n"); } break;
                    case EXCEPTION_FLT_UNDERFLOW: { printf("EXCEPTION_FLT_UNDERFLOW\n"); } break;
                    case EXCEPTION_ILLEGAL_INSTRUCTION: { printf("EXCEPTION_ILLEGAL_INSTRUCTION\n"); } break;
                    case EXCEPTION_IN_PAGE_ERROR: { printf("EXCEPTION_IN_PAGE_ERROR\n"); } break;
                    case EXCEPTION_INT_DIVIDE_BY_ZERO: { printf("EXCEPTION_INT_DIVIDE_BY_ZERO\n"); } break;
                    case EXCEPTION_INT_OVERFLOW: { printf("EXCEPTION_INT_OVERFLOW\n"); } break;
                    case EXCEPTION_INVALID_DISPOSITION: { printf("EXCEPTION_INVALID_DISPOSITION\n"); } break;
                    case EXCEPTION_NONCONTINUABLE_EXCEPTION: { printf("EXCEPTION_NONCONTINUABLE_EXCEPTION\n"); } break;
                    case EXCEPTION_PRIV_INSTRUCTION: { printf("EXCEPTION_PRIV_INSTRUCTION\n"); } break;
                    case EXCEPTION_SINGLE_STEP: { printf("EXCEPTION_SINGLE_STEP\n"); } break;
                    case EXCEPTION_STACK_OVERFLOW: { printf("EXCEPTION_STACK_OVERFLOW\n"); } break;
                    default: { printf("default exception\n"); } break;
                }
            } break;

            case CREATE_THREAD_DEBUG_EVENT: { printf("CREATE_THREAD_DEBUG_EVENT\n"); } break;
            case CREATE_PROCESS_DEBUG_EVENT: { printf("CREATE_PROCESS_DEBUG_EVENT\n"); } break;
            case EXIT_THREAD_DEBUG_EVENT: { printf("EXIT_THREAD_DEBUG_EVENT\n"); } break;

            case EXIT_PROCESS_DEBUG_EVENT: {
                printf("EXIT_PROCESS_DEBUG_EVENT\n");
                return true;
            } break;

            case LOAD_DLL_DEBUG_EVENT: { printf("LOAD_DLL_DEBUG_EVENT\n"); } break;
            case UNLOAD_DLL_DEBUG_EVENT: { printf("UNLOAD_DLL_DEBUG_EVENT\n"); } break;
            case OUTPUT_DEBUG_STRING_EVENT: { printf("OUTPUT_DEBUG_STRING_EVENT\n"); } break;
            case RIP_EVENT: { printf("RIP_EVENT\n"); } break;
            default: { printf("default event\n"); } break;
        }
    }

    return 0;
}

void ProcStartup(State *state, Process_Info *procInfo) {
    b32 isKernelBreakpoint = true;
    Breakpoint kernelBreakpoint = {0};

    void *startAddress = 0;

    while (INFINITE_LOOP) {
        DEBUG_EVENT event = {0};
        WaitForDebugEvent(&event, INFINITE);

        procID2   = event.dwProcessId;
        threadID2 = event.dwThreadId;

        if (event.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT) {
            startAddress = (void *)event.u.CreateProcessInfo.lpStartAddress;
            state->binImagePos.baseAddress = (u64)event.u.CreateProcessInfo.lpBaseOfImage;
        } else if (event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT &&
                   event.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT) {
            if (isKernelBreakpoint) {
                Assert(startAddress != 0);

                SetBreakpoint(&kernelBreakpoint, procInfo, startAddress);

                isKernelBreakpoint = false;
            } else {
                SetPC(procInfo->threadIDs.A[0], GetPC(procInfo->threadIDs.A[0]) - 1);
                RemoveBreakpoint(&kernelBreakpoint, procInfo);
                return;
            }
        } else {
            if (PrintDebugEvent(&event)) { break; }
        }

        ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE);
    }
}

int main(int argc, char* argv[]) {
    Array_CString args = { .size = argc, .A = argv };

    Mem* mem = {0};
    InitializeMem((void*)0x2000000000);
    mem = &globalMem;

    State *state = ArenaPushType(&mem->perma.arena, State);
    state->projectFilesIds.size = 0;
    state->projectFilesIds.capacity = PFI_SIZE;

    if ( args.size < 2 ) {
        ERR("The .exe file path is missing.\n");
        return 1;
    }

    gBinPath = (String){ .size = CStrLen(args.A[1]), .str = args.A[1] };
    if ( !File_Exists(gBinPath.str) ) {
        ERR("Couldn't access the file: %s.\n", gBinPath.str);
        return 1;
    }

    CString cmd = 0;
    Process_Info targetProcInfo;

    Thread_ID threadIdBuffer[256];
    targetProcInfo.threadIDs.A = threadIdBuffer;

    // "testBin\\win\\test.exe"
    if (!CreateAndTraceProcess(&targetProcInfo, gBinPath.str, cmd)) {
        ERR("Couldn't load the executable into memory.\n");
        return 1;
    }

    ProcStartup(state, &targetProcInfo);

    b32 ret = DebuggerLoop(mem, state, &targetProcInfo, gBinPath.str);

    printf("FIN DEL PROGRAMA.\n");
    return ret;
}
