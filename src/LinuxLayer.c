#ifndef DSV_LINUXLAYER_H
#define DSV_LINUXLAYER_H value

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <sys/personality.h>

#include "DataTypes.h"

#include "../../SourceCode/Memory.h"
#include "../../SourceCode/Math.h"
#include "../../SourceCode/String.h"
#include "OS.h"

#include "ELF.h"
#include "Debugger.h"

Inter void FileIdentifierFromDirAndName(File_Identifier *fileId, Memory_Arena *StrArena,
                                        String name, String dir) {
    fileId->name.length = name.length;
    fileId->dir.length  = dir.length + 1; // NOTE(JENH): space for Last forward slash.
    fileId->path.length = fileId->name.length + fileId->dir.length + 1;

    fileId->path.str = ArenaPushArray(StrArena, char, fileId->path.length);

    fileId->dir.str = fileId->path.str;
    CopyCStrForward(fileId->dir.str, dir.str, fileId->dir.length - 1);
    *(fileId->dir.str + fileId->dir.length - 1) = '/';

    fileId->name.str = fileId->path.str + fileId->dir.length;
    CopyCStrForward(fileId->name.str, name.str, fileId->name.length);

    // NOTE(JENH): Just a dumb null character.
    *(fileId->path.str + fileId->path.length - 1) = 0;
}

Inter b32 ReadEntireFile(File *file, CString fileName) {
    b32 ret = true;

    int fd = open(fileName, 0);

    if (fd == -1) {
        printERR("Couldn't open file \"%s\": %s\n", fileName, strerror(errno));
        return false;
    }

    WordSize fileSize = (WordSize)lseek(fd, 0, SEEK_END);

    if (fileSize == -1) {
        printERR("Couldn't get file size \"%s\": %s\n", fileName, strerror(errno));
        defer(false);
    }

    void *memory = mmap(0, fileSize, PROT_READ, MAP_SHARED, fd, 0);

    if (memory == MAP_FAILED) {
        printERR("Couldn't map the file \"%s\" to memory: %s\n", fileName, strerror(errno));
        defer(false);
    }

    file->memory = memory;
    file->size   = fileSize;

defer:
    close(fd);
    return ret;
}

Inter b32 WriteNewFile(CString fileName, void *memory, WordSize size) {
    b32 ret = 1;

    int fdNewFile = creat(fileName, S_IRWXU);

    if (fdNewFile == ERROR_CODE) {
        printERR("Couldn't create new file: %s\n", strerror(errno));
        return 0;
    }

    ssize_t bytesWritten = write(fdNewFile, memory, size);

    if (bytesWritten == ERROR_CODE) {
        printERR("Couldn't write to the new file: %s\n", strerror(errno));
        ret = 0;
    }

    int resClose = close(fdNewFile);

    if (resClose == ERROR_CODE) {
        printERR("Couldn't close new file: %s\n", strerror(errno));
        //ret = 0;  ?????
    }

    return ret;
}

Inter void SeeTags(byte *base, Dwarf_Compile_Unit *CU) {
    u32 treeDepth = 1;
    byte *scanData = base;

    for (u64 code = ULEB128AndAdvance(&scanData); treeDepth != 0;
         code = NextDIE(&scanData, code, CU->header.abbrev.table.A[code].definition, CU)) {
        printf("<%2u><0x%08lx>: Abbrev number %lu ", treeDepth, (u64)(scanData - base), code);

        if (code == 0) {
            printf("\n");
            --treeDepth;
            continue;
        }

        Dwarf_Abbrev_Entry *abbrev = &CU->header.abbrev.table.A[code];

        if (abbrev->hasChildren) { ++treeDepth; }

        PrintTag(abbrev->tag, "(%s)");
    }
}

void WaitChildProc(s32 procID) {
    s32 status;
    waitpid(procID, &status, 0);
}

Inter Proc_ID CreateAndTraceProcess(char **cmd) {
    s32 procID = (s32)fork();

    if (procID == -1) {
        printERR("Could not fork child process: %s\n", strerror(errno));
        return 1;
    }

    if (procID == 0) {
        personality(ADDR_NO_RANDOMIZE);
        ptrace(PTRACE_TRACEME, 0, 0, 0); //PTRACE_MODE_ATTACH_REALCREDS

        if (execvp(cmd[0], cmd) < 0) {
            printERR("Could not exec child process: %s\n", strerror(errno));
            exit(1);
        }
    }

    WaitChildProc(procID);

    return procID;
}

#define WriteProcMemType(procID, localMem, procMem) WriteProcMem(procID, localMem, procMem, sizeof(*localMem))
b32 WriteProcMem(Proc_ID procID, void *localMem, void *procMem, u64 size) {
    struct iovec local = { .iov_base = localMem, .iov_len  = size };
    struct iovec proc  = { .iov_base = procMem , .iov_len  = size };

    //s64 ret = ptrace(PTRACE_POKETEXT, procID, address, value);
    s64 bytesWritten = process_vm_writev(procID, &local, 1, &proc, 1, 0);
    b32 success = (ret != -1);
    return success;
}

#define ReadProcMemType(procID, localMem, procMem) ReadProcMem(procID, localMem, procMem, sizeof(*localMem))
b32 ReadProcMem(Proc_ID procID, void *localMem, void *procMem, u64 size) {
    struct iovec local = { .iov_base = localMem, .iov_len  = size };
    struct iovec proc  = { .iov_base = procMem , .iov_len  = size };

    //u64 ret = (u64)ptrace(PTRACE_PEEKTEXT, procID, address, 0);
    s64 bytesRead = process_vm_readv(procID, &local, 1, &proc, 1, 0);
    b32 ret = (bytesRead == size);
    return ret;
}

u64 GetRegister(Thread_ID threadID, Register reg) {
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, threadID, 0, &regs);

    u64 *regBase = (u64 *)&regs;
    u64 ret = *(regBase + (u32)reg);

    return ret;
}

void SetRegister(Thread_ID threadID, Register reg, u64 value) {
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, threadID, 0, &regs);

    u64 *regBase = (u64 *)&regs;
    *(regBase + (u32)reg) = value;

    ptrace(PTRACE_SETREGS, threadID, 0, &regs);
}

void SingleStep(Process_Info *ProcInfo) {
    ptrace(PTRACE_SINGLESTEP, ProcInfo->threadIDs.A[0], 0, 0);
    WaitChildProcess(ProcInfo->procID);
}

void ContinueThread(Process_Info *ProcInfo) {
    ptrace(PTRACE_CONT, ProcInfo->threadIDs.A[0], 0, 0);
    WaitChildProcess(ProcInfo->procID);
}

int main() {
    char *cmdNewProcess[] = {"testBin/test.bin", 0};
    s32 processID = CreateAndTraceProcess(cmdNewProcess);

    ptrace(PTRACE_SETOPTIONS, processID, 0, PTRACE_O_EXITKILL);

    File ELFFile = {0};
    if (!LoadELFFile(argvNewProcess[0], &ELFFile)) {
        printERR("Could not load the ELF file into memory: %s\n", strerror(errno));
        return 1;
    }

    Mem mem = {0};
    CreateMemory(&mem);
    globalMem = &mem;

    State *state = ArenaPushType(mem->permaMem.arena);
    state->projectFilesIds.size = 0;
    state->projectFilesIds.capacity = PFI_SIZE;

    WriteNewFile("newFile.bin", ELFFile.memory, ELFFile.size);

    ELF_Image ELFImage = ParseELF(ELFFile.memory, ELFFile.size);

    DwarfSectionsFromELF(&ELFImage);

    if (!DwarfSectionsToFile("elf_dwarf_sections")) {
        return 1;
    }

    ParseRangeListHeader(&rnglistHeader, dwarfSections.rnglists.ptr);

    InitializeCompileUnits(&mem.perma.dwarf, &mem.perma.strings, &state->CUs, &state->projectFilesIds, &dwarfSections.info);

    char input[256];
    CString commands[64];
    u32 commandsCount = 0;
    CString command;

    do {
        scanf("%[^\n]%*c", input);
        ParseCommands(input, commands, &commandsCount);

        command = commands[0];

        if (CompCStr(command, "continue") == 0) {
            u64 pcPrevInstruction = GetRegister(processID, Reg_rip) - 1;

            Breakpoint_Entry *breakpoint = GetBreakpointByAddress(state->breakpoints, (byte *)pcPrevInstruction);

            if (breakpoint) {
                SingleStepFromBreakpoint(processID, breakpoint);
            }

            ptrace(PTRACE_CONT, processID, 0, 0);

            WaitChildProcess(processID);
        } else if (CompCStr(command, "register") == 0) {
            CString action = commands[1];
            Register reg = (Register)FindName(registerNames, commands[2]);

            if (reg != INVALID_REG_NAME) {
                if (CompCStr(action, "get") == 0) {
                    u64 regValue = GetRegister(processID, reg);
                    printf("register value = 0x%lX\n", regValue);
                } else if (CompCStr(action, "set") == 0) {
                    u64 value = strtol(commands[3], 0, 0);
                    SetRegister(processID, reg, value);
                }
            }
        } else if (CompCStr(command, "breakpoint") == 0) {
            CString action = commands[1];

            if (CompCStr(action, "set") == 0) {
                if (CompCStr(commands[2], "line") == 0) {
                    u32 lineNumber = atoi(commands[3]);
                    byte *address  = (byte *)GetPCByLine(state->CUs.A[0].line, 0, lineNumber, &state->projectFilesIds);
                    byte *fixAddress = (byte *)(0x555555554000 + address);
                    printf("address breakpoint = %p\n", fixAddress);

                    SetBreakpoint(GetEmptyBreakpoint(state->breakpoints), processID, fixAddress);
                } else {
                    byte *address = (byte *)(0x555555554000 + strtol(commands[2], 0, 0));
                    printf("address breakpoint = %p\n", address);

                    SetBreakpoint(GetEmptyBreakpoint(state->breakpoints), processID, address);
                }
            } else if (CompCStr(action, "list") == 0) {
                ListBreakpoints(state->breakpoints, BP_SIZE);
            }
        } else if (CompCStr(command, "elf") == 0) {
            CString action = commands[1];

            if (CompCStr(action, "which") == 0) {
                CString secBundleName = commands[2];
                if (CompCStr(secBundleName, "dwarf") == 0) {
                    PrintDwarfSections();
                }
            } else if (CompCStr(action, "abbrev") == 0) {
                PrintAbbrev(&state->CUs.A[0].header.abbrev);
            } else if (CompCStr(action, "info") == 0) {
                //SeeTags(state->CUs.A[0].DIEChilds, state->CUs.A);
                PrintInfo(state->CUs.A[0].DIEChilds, &state->CUs.A[0]);
            } else if (CompCStr(action, "line") == 0) {
                PrintLineTable(state->CUs.A[0].line, &state->projectFilesIds);
            } else if (CompCStr(action, "ranges") == 0) {
                byte *scan = state->CUs.A[0].DIE;
                u32 code = ULEB128AndAdvance(&scan);
                if (HasAttrib(state->CUs.A[0].header.abbrev.table.A[code].definition, DW_AT_ranges)) {
                    byte *rnglist = GetDIERanges(&state->CUs.A[0], state->CUs.A[0].header.abbrev.table.A[code].definition,
                                                 scan);
                    PrintRangeListEntry(&rnglistHeader, rnglist, state->CUs.A[0].defaultBaseAddress);
                }
            }
        } else if (CompCStr(command, "range") == 0) {
            u64 givenAddress = strtol(commands[1], 0, 0);

            byte *scan = state->CUs.A[0].DIE;
            u32 code = ULEB128AndAdvance(&scan);

            if (IsInRange(state->CUs.A[0].header.abbrev.table.A[code].definition, scan, givenAddress, &state->CUs.A[0])) {
                printf("it is in range.\n");
            } else {
                printf("Is not in range.\n");
            }
        } else if (CompCStr(command, "pid") == 0) {
            printf("target process ID: %d\n", processID);
        }
    } while(CompCStr(command, "exit"));

    return 0;
}

#endif //ifndef DSV_LINUXLAYER_H
