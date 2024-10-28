#ifndef DSV_DEBUGGER_H
#define DSV_DEBUGGER_H

#include "Encodings.h"

#define GetTypeAndAdvance(type, scanPtr) \
    *(type *)*(scanPtr);                 \
    *(scanPtr) += sizeof(type);

#define ERROR_CODE -1

typedef struct {
    u64  size;
    byte *ptr;
} Mem_Block;

typedef struct {
    b32 used;
    u8 byteExtracted;
    byte *address;
} Breakpoint;

Array_Ptr(Array_Breakpoint, Breakpoint);

#define BP_SIZE 256

typedef struct {
    u64 baseAddress;
    u64 desiredAddress;
} Bin_Image_Pos;

Inter inline u64 AddressIndepToASLR(Bin_Image_Pos *binImagePos, u64 address) {
    u64 ret = (u64)(address - (u64)binImagePos->desiredAddress) + (u64)binImagePos->baseAddress;
    return ret;
}

Inter inline u64 ASLRToAddressIndep(Bin_Image_Pos *binImagePos, u64 address) {
    u64 ret = (u64)(address - (u64)binImagePos->baseAddress) + (u64)binImagePos->desiredAddress;
    return ret;
}

Inter inline u64 GetPCIndep(Bin_Image_Pos *binImagePos, Thread_ID *threadD) {
    return ASLRToAddressIndep(binImagePos, (u64)GetRegister(threadD, Reg_rip));
}

#include "Dwarf.h"

typedef struct {
    Breakpoint breakpoints[BP_SIZE];

    byte *baseProcAddress;
    u64 procDesiredAddress;
    Bin_Image_Pos binImagePos;
    Array_File_Identifier projectFilesIds;
    u32 currentFile;
    Dwarf_Range_List_Header rnglistHeader;

    Memory_Arena CUsDataArena;

    Array_Dwarf_Compile_Unit CUs;

    FDE_PC_Map frameMap;
} State;

Inter u32 FindFileID(Array_File_Identifier *fileIDs, CString fileDir, CString fileName) {
    for (u32 fileIdIndex = 0; fileIdIndex < fileIDs->size; ++fileIdIndex) {
        File_Identifier *fileID = &fileIDs->A[fileIdIndex];

        //CompCStr(fileDir, fileID->dir) == 0 &&
        if (CompCStr(fileName, fileID->name.str) == 0) {
            return fileIdIndex;
        }
    }

    return INVALID_FILE_ID_INDEX;
}

#if 0
Inter void ParseCommands(CString input, CString *commands, u32 *commandsCount) {
    char *garbage = (char *)MAX_VALUE_PTR;

    char *scan = input;

    while (INFINITE_LOOP) {
	    ++(*commandsCount);

	    char charsLookingFor1[] = " \t\0";

        scan = FindAnyDiffCharForward(scan, garbage, charsLookingFor1, ArrayCount(charsLookingFor1) - 1, 0);

	    if (*scan == '\"') {
	        *commands++ = ++scan;

	        char charsLookingFor2[] = "\"\0";

            scan = FindAnyCharForward(scan, garbage, charsLookingFor2, ArrayCount(charsLookingFor2) - 1, 0);

	        *scan++ = '\0';

	        if (*scan == '\0') { return; }
	    } else {
	        *commands++ = scan;

            scan = FindAnyCharForward(scan, garbage, charsLookingFor1, ArrayCount(charsLookingFor1) - 1, 0);

            if (*scan == '\0') { return; }

            *scan++ = '\0';
        }
    }
}
#endif

Inter u32 FindName(String *names, CString nameToFind) {
    u32 index = 0;
    for (String *name = names; index < Reg_count; ++name, ++index) {
        if (CompCStr(name->str, nameToFind) == 0) {
            u32 ret = index;
            return ret;
        }
    }

    return INVALID_REG_NAME;
}

Inter u64 GetReturnAddresss(Registers *regs, Process_ID procID, FDE_PC_Map *frameMap, u64 PC) {
    byte *FDEAddress = FindFDE(frameMap, PC);

    Dwarf_FDE FDE = {0};
    ParseFDE(&FDE, FDEAddress);

    Frame_Row frameRow = {0};

    GetFrameRowAtPC(&frameRow, &FDE, PC);

    u64 CFA = GetCFA(&frameRow.CFADef, regs);
    printf("CFA: %llx\n", CFA);

    for (u32 regIndex = 0; regIndex < Reg_count; ++regIndex) {
        if (IsBitFlagSet(&frameRow.regsWithRules, regIndex)) {
            SetLoadedRegister(regs, regIndex, GetRegInFrameRow(&frameRow, procID, CFA, regIndex));
            //PrintlnLoadedRegister(regs, regIndex);
        }
    }

    return GetLoadedRegister(regs, FDE.CIE.returnAddressRegister);
}

Inter void PrintCallStack(Bin_Image_Pos *binImagePos, Array_Dwarf_Compile_Unit *CUs, Array_File_Identifier *fileIDs,
                          Process_ID procID, Thread_ID threadID, FDE_PC_Map *frameMap) {
    u64 PC = GetPCIndep(binImagePos, threadID);

    Registers regs = {0};
    LoadRegisters(threadID, &regs);

    do {
        Dwarf_DIE func;
        if (!GetFunctionByAddress(&func, CUs, PC)) { break; }

        MoveDataCursor(&func, DW_AT_name);
        printf("(%s) ", GetDIEName(&func).str);

        //PrintDIE(&func);

        Source_Position srcPos = {0};
        GetSrcPosByAddress(&srcPos, CUs, PC);
        PrintSourcePosition(&srcPos, fileIDs);

        PC = ASLRToAddressIndep(binImagePos, GetReturnAddresss(&regs, procID, frameMap, PC));
        //printf("PC = 0x%llx\n", PC);
    } while (INFINITE_LOOP);
}

Inter void PrintLocalVariables(Process_ID procID, Thread_ID threadID, Bin_Image_Pos *binImagePos,
                               Array_Dwarf_Compile_Unit *CUs, FDE_PC_Map *frameMap) {
    u64 PC = GetPCIndep(binImagePos, threadID);

    Registers regs = {0};
    LoadRegisters(threadID, &regs);

    byte *FDEAddress = FindFDE(frameMap, PC);

    Dwarf_FDE FDE = {0};
    ParseFDE(&FDE, FDEAddress);

    Frame_Row frameRow = {0};

    GetFrameRowAtPC(&frameRow, &FDE, PC);

    u64 CFA = GetCFA(&frameRow.CFADef, &regs);

    Dwarf_DIE func;

    Dwarf_Compile_Unit *CU = GetCUByAddress(CUs, PC);
    GetFunctionByAddress(&func, CUs, PC);
    ParseDIE(&func);

    Expr_Element exprlocFrameBase = EvalExprFrameBase(GetDIEFrameBase(&func), &regs, procID, CFA);
    u64 frameBase = exprlocFrameBase.generic;

    printf("variables in function (%s):\n", GetDIEName(&func).str);
    //PrintDIE(&func);

    Dwarf_DIE funcChild;
    for (InitializeDIE(&funcChild, func.cursorDataRaw, CU); funcChild.treeDepth != 0; NextDIE(&funcChild)) {
        if (funcChild.abbrev->tag == DW_TAG_variable ||
            funcChild.abbrev->tag == DW_TAG_formal_parameter) {
            ParseDIE(&funcChild);
            printf("%s\n", GetDIEName(&funcChild).str);
#if 0
            //PrintDIE(&funcChild);

            Expr_Element location = EvaluateExpression(GetDIELocation(&funcChild), &regs, procID, frameBase);
            printf("    (%llx) ", (u64)location.generic);

            printf(" %s: ", GetDIEName(&funcChild).str);
            if (location.type == ELT_generic) {
                s32 val;
                ReadProcMemType(procID, &val, (void *)location.generic);
                printf("%d.\n", val);
            }
#endif
        }
    }
}

Inter Breakpoint *GetBreakpointByAddress(Breakpoint *breakpoints, byte *address) {
    State state;

    for (u32 breakpointIndex = 0; breakpointIndex < ArrayCount(state.breakpoints); ++breakpointIndex) {
        if (breakpoints[breakpointIndex].address == address) {
            return &breakpoints[breakpointIndex];
        }
    }

    return 0;
}

Inter Breakpoint *GetEmptyBreakpoint(Breakpoint *breakpoints) {
    State state;

    for (u32 breakpointIndex = 0; breakpointIndex < ArrayCount(state.breakpoints); ++breakpointIndex) {
        if (!breakpoints[breakpointIndex].used) {
            return &breakpoints[breakpointIndex];
        }
    }

    return 0;
}

Inter void SetBreakpoint(Breakpoint *breakpoint, Process_Info *procInfo, byte *address) {
    Assert(breakpoint != 0 && breakpoint->used == false);

    ReadProcMemType(procInfo->procID, &breakpoint->byteExtracted, address);
    breakpoint->address = address;
    breakpoint->used = true;

    byte interupt = 0xCC;
    WriteProcCodeType(procInfo->procID, &interupt, address);
}

Inter void RemoveBreakpoint(Breakpoint *breakpoint, Process_Info *procInfo) {
    Assert(breakpoint != 0 && breakpoint->used == true);

    WriteProcCodeType(procInfo->procID, &breakpoint->byteExtracted, breakpoint->address);
    breakpoint->used = false;
}

Inter void ListBreakpoints(Breakpoint *breakpoints, u32 size) {
    for (u32 BPIndex = 0; BPIndex < size; ++BPIndex) {
        Breakpoint *breakpoint = &breakpoints[BPIndex];
        if (breakpoint->used) {
            printf("byte extracted = 0x%x\n", breakpoint->byteExtracted);
            printf("address = %p\n", breakpoint->address);
        }
    }
}

Inter void PrintCUsRanges(Dwarf_Range_List_Header *rnglistHeader, Array_Dwarf_Compile_Unit *CUs) {
    foreach (Dwarf_Compile_Unit, CU, *CUs) {
        String nameCU = GetDIEName(&CU->DIE);
        printf("CU (%s) ranges:\n", nameCU.str);

        if (MoveDataCursor(&CU->DIE, DW_AT_ranges)) {
            byte *rnglist = GetDIERanges(&CU->DIE);
            PrintRangeListEntry(rnglistHeader, rnglist, CU->defaultBaseAddress);
        } else if (MoveDataCursor(&CU->DIE, DW_AT_low_pc)) {
            u64 lowPC  = GetDIELowPC(&CU->DIE);
            u64 highPC = lowPC;

            if (MoveDataCursor(&CU->DIE, DW_AT_high_pc)) {
                highPC = GetDIEHighPC(&CU->DIE);
            }

            printf("    0x%016llx  0x%016llx\n", lowPC, highPC);
        }
        printf("\n");
    }
}

Inter void SingleStepFromBreakpoint(Process_Info *procInfo, Breakpoint *breakpoint) {
    Assert(breakpoint != 0 && breakpoint->used == true);

    WriteProcCodeType(procInfo->procID, &breakpoint->byteExtracted, breakpoint->address);

    SingleStep(procInfo);

    byte interrupt = 0xCC;
    WriteProcCodeType(procInfo->procID, &interrupt, breakpoint->address);
}

Inter void SingleStepInstruction(Process_Info *procInfo, Breakpoint *breakpoints) {
    Breakpoint *breakpoint = GetBreakpointByAddress(breakpoints, (byte *)GetPC(procInfo->threadIDs.A[0]));

    if (breakpoint) {
        SingleStepFromBreakpoint(procInfo, breakpoint);
    } else {
        SingleStep(procInfo);
    }
}

Inter void Continue(Process_Info *procInfo, Breakpoint *breakpoints) {
    Breakpoint *breakpoint = GetBreakpointByAddress(breakpoints, (byte *)GetPC(procInfo->threadIDs.A[0]));

    if (breakpoint) {
        SingleStepFromBreakpoint(procInfo, breakpoint);
    }

    ContinueProcess(procInfo);
}

Inter void StepOut(Process_Info *procInfo, Bin_Image_Pos *binImagePos, FDE_PC_Map *frameMap, Breakpoint *breakpoints) {
    Registers regs = {0};
    LoadRegisters(procInfo->threadIDs.A[0], &regs);
    byte *retAddress = (byte *)GetReturnAddresss(&regs, procInfo->procID, frameMap,
                                                 GetPCIndep(binImagePos, procInfo->threadIDs.A[0]));

    Breakpoint tempBreakpoint = {0};

    SetBreakpoint(&tempBreakpoint, procInfo, retAddress);

    Continue(procInfo, breakpoints);

    RemoveBreakpoint(&tempBreakpoint, procInfo);
}

Inter void StepIn(Process_Info *procInfo, Bin_Image_Pos *binImagePos, Array_Dwarf_Compile_Unit *CUs, Breakpoint *breakpoints) {
    Source_Position currentSrcPos;
    GetSrcPosByAddress(&currentSrcPos, CUs, GetPCIndep(binImagePos, procInfo->threadIDs.A[0]));

    while (INFINITE_LOOP) {
        Source_Position srcPos;
        GetSrcPosByAddress(&srcPos, CUs, GetPCIndep(binImagePos, procInfo->threadIDs.A[0]));
        //PrintSourcePosition(&srcPos, &state->projectFilesIds);

        if (srcPos.line != currentSrcPos.line) { break; }

        SingleStepInstruction(procInfo, breakpoints);
    }
}

Inter void StepOver(Process_Info *procInfo, Bin_Image_Pos *binImagePos,
                    Array_Dwarf_Compile_Unit *CUs, Breakpoint *breakpoints) {
    Dwarf_DIE currentFunc;
    GetFunctionByAddress(&currentFunc, CUs, GetPCIndep(binImagePos, procInfo->threadIDs.A[0]));

    u64 funcBeginAddress;
    if (MoveDataCursor(&currentFunc, DW_AT_low_pc)) {
        funcBeginAddress = GetDIELowPC(&currentFunc);
    } else { Assert(false && "Function should have low pc."); }

    u64 funcEndAddress;
    if (MoveDataCursor(&currentFunc, DW_AT_high_pc)) {
        funcEndAddress = GetDIEHighPC(&currentFunc);
    } else { Assert(false && "Function should have high pc."); }

    Memory_Arena *breakpointArena = AllocTempArena((funcEndAddress - funcBeginAddress) * sizeof(Breakpoint));
    Array_Breakpoint tempBreakpoints = {0};
    tempBreakpoints.A = ArenaPushArray(breakpointArena, Breakpoint, UNDETERMINED_SIZE);

    Source_Position pcSrcPos;
    GetSrcPosByAddress(&pcSrcPos, CUs, GetPCIndep(binImagePos, procInfo->threadIDs.A[0]));

    Source_Position endFuncSrcPos;
    GetSrcPosByAddress(&endFuncSrcPos, CUs, funcEndAddress);

    Source_Position srcPos;
    GetSrcPosByAddress(&srcPos, CUs, funcBeginAddress);

    while (INFINITE_LOOP) {
        if (srcPos.address != pcSrcPos.address) {
            //PrintSourcePosition(&srcPos, &state->projectFilesIds);
            Breakpoint *breakpoint = ArenaPushType(breakpointArena, Breakpoint);
            breakpoint->used = false;
            SetBreakpoint(breakpoint, procInfo, (byte *)AddressIndepToASLR(binImagePos, srcPos.address));
            ++tempBreakpoints.size;
        }

        if (srcPos.line == endFuncSrcPos.line) { break; }

        NextSrcPos(&srcPos, CUs);
    }

#if 1
    byte *frame = (byte *)GetRbp(procInfo->threadIDs.A[0]);
    byte *retAddress;
    ReadProcMemType(procInfo->procID, &retAddress, frame + 8);

    Breakpoint *retBreakpoint = ArenaPushType(breakpointArena, Breakpoint);
    retBreakpoint->used = false;
    SetBreakpoint(retBreakpoint, procInfo, retAddress);
    ++tempBreakpoints.size;
#endif

    Continue(procInfo, breakpoints);

    foreach (Breakpoint, breakpoint, tempBreakpoints) {
        RemoveBreakpoint(breakpoint, procInfo);
    }
}

Inter void PrintFileNames(Array_File_Identifier *projectFilesIds) {
    char pathBuffer[4096];

    foreach (File_Identifier, fileID, *projectFilesIds) {
        CreateFilePath(fileID, pathBuffer);
        printf("\"%s\"\n", pathBuffer);
    }
}

b32 DebuggerLoop(Mem *mem, State *state, Process_Info *procInfo, CString binName) {
    InitializeRegisterMetadata();

    File targetBinFile;
    ReadEntireFile(&targetBinFile, binName);
    WriteNewFile("newFile.bin", targetBinFile.memory, targetBinFile.size);

    PE_Image targetPE;
    ParsePEImage(&targetPE, targetBinFile.memory);

    if (targetPE.format == PE_32) {
        Optional_Header32 *optionalHeader32 = &targetPE.NTHeaders->OptionalHeader32;
        state->binImagePos.desiredAddress = optionalHeader32->ImageBase;
    } else {
        Optional_Header64 *optionalHeader64 = &targetPE.NTHeaders->OptionalHeader64;
        state->binImagePos.desiredAddress = optionalHeader64->ImageBase;
    }

    DwarfSectionsFromPE(&targetPE);

    if (!DwarfSectionsToFile("pe_dwarf_sections.bin")) {
        return false;
    }

    ParseRangeListHeader(&rnglistHeader, dwarfSections.rnglists.ptr);

    InitializeCompileUnits(&mem->perma.dwarf, &mem->perma.strings, &state->CUs, &state->projectFilesIds, &dwarfSections.info);
    CreateFDEPCMap(&mem->perma.dwarf, &state->frameMap, &dwarfSections.frame);

    char input[256];
    CString commands[64];
    u32 commandsCount = 0;
    CString command;

    do {
        scanf("%[^\n]%*c", input);
        ParseCommands(input, commands, &commandsCount);

        printf("\n");
        command = commands[0];

        if (CompCStr(command, "continue") == 0) {
            Continue(procInfo, state->breakpoints);

            Source_Position srcPos;
            GetSrcPosByAddress(&srcPos, &state->CUs, ASLRToAddressIndep(&state->binImagePos, GetPC(procInfo->threadIDs.A[0])));
            PrintSourcePosition(&srcPos, &state->projectFilesIds);
        } else if (CompCStr(command, "stepi") == 0) {
            SingleStepInstruction(procInfo, state->breakpoints);

            Source_Position srcPos;
            GetSrcPosByAddress(&srcPos, &state->CUs, ASLRToAddressIndep(&state->binImagePos, GetPC(procInfo->threadIDs.A[0])));
            PrintSourcePosition(&srcPos, &state->projectFilesIds);
        } else if (CompCStr(command, "out") == 0) {
            StepOut(procInfo, &state->binImagePos, &state->frameMap, state->breakpoints);
            Source_Position srcPos;
            GetSrcPosByAddress(&srcPos, &state->CUs, ASLRToAddressIndep(&state->binImagePos, GetPC(procInfo->threadIDs.A[0])));
            PrintSourcePosition(&srcPos, &state->projectFilesIds);
        } else if (CompCStr(command, "in") == 0) {
            StepIn(procInfo, &state->binImagePos, &state->CUs, state->breakpoints);
            Source_Position srcPos;
            GetSrcPosByAddress(&srcPos, &state->CUs, ASLRToAddressIndep(&state->binImagePos, GetPC(procInfo->threadIDs.A[0])));
            PrintSourcePosition(&srcPos, &state->projectFilesIds);
        } else if (CompCStr(command, "over") == 0) {
            StepOver(procInfo, &state->binImagePos, &state->CUs, state->breakpoints);
            Source_Position srcPos;
            GetSrcPosByAddress(&srcPos, &state->CUs, ASLRToAddressIndep(&state->binImagePos, GetPC(procInfo->threadIDs.A[0])));
            PrintSourcePosition(&srcPos, &state->projectFilesIds);
        } else if (CompCStr(command, "register") == 0) {
            CString action = commands[1];
            Register_Enum reg = (Register_Enum)FindName(regsMetadata.names, commands[2]);

            if (CompCStr(action, "get") == 0) {
                u64 regValue = GetRegister(procInfo->threadIDs.A[0], reg);
                printf("register value = 0x%llx\n", regValue);
            } else if (CompCStr(action, "set") == 0) {
                u64 value = strtol(commands[3], 0, 0);
                SetRegister(procInfo->threadIDs.A[0], reg, value);
            }
        } else if (CompCStr(command, "breakpoint") == 0) {
            CString action = commands[1];

            if (CompCStr(action, "set") == 0) {
                if (CompCStr(commands[2], "line") == 0) {
                    CString fileName = commands[3];
                    u32 lineNumber = atoi(commands[4]);

                    u32 fileIDIndex = FindFileID(&state->projectFilesIds, 0, fileName);

                    if (fileIDIndex != INVALID_FILE_ID_INDEX) {
                        u64 address = GetPCByLine(&state->CUs, &state->projectFilesIds, fileIDIndex, lineNumber);

                        if (address != MAX_u64) {
                            byte *fixAddress = (byte *)AddressIndepToASLR(&state->binImagePos, address);
                            printf("address = %llx\n", address);
                            printf("address breakpoint = %p\n", fixAddress);

                            SetBreakpoint(GetEmptyBreakpoint(state->breakpoints), procInfo, fixAddress);
                        }
                    } else {
                        printf("The file doesn't exists.\n");
                    }
                } else if (CompCStr(commands[2], "func") == 0) {
                    String name = {
                        .str = commands[3],
                        .length = CStrLen(commands[3]) - 1
                    };

                    Dwarf_DIE func;
                    GetFunctionByName(&func, &state->CUs, name);

                    //PrintDIE(&func);

                    u64 funcBegin;
                    if (MoveDataCursor(&func, DW_AT_low_pc)) {
                        funcBegin = GetDIELowPC(&func);
                    } else { Assert(false && "Function should have low pc."); }

                    Source_Position fnPos;
                    GetSrcPosByAddress(&fnPos, &state->CUs, funcBegin);

                    Source_Position srcPos = fnPos;
                    NextSrcPos(&srcPos, &state->CUs);

                    byte *fixAddress = (byte *)AddressIndepToASLR(&state->binImagePos, srcPos.address);

                    printf("address breakpoint = %p\n", fixAddress);
                    PrintSourcePosition(&fnPos, &state->projectFilesIds);
                    //printf("address = %llx\n", srcPos.address);

                    SetBreakpoint(GetEmptyBreakpoint(state->breakpoints), procInfo, fixAddress);
                } else {
                    byte *address = state->baseProcAddress + strtol(commands[2], 0, 0);
                    printf("address breakpoint = %p\n", address);

                    SetBreakpoint(GetEmptyBreakpoint(state->breakpoints), procInfo, address);
                }
            } else if (CompCStr(action, "list") == 0) {
                ListBreakpoints(state->breakpoints, BP_SIZE);
            }
        } else if (CompCStr(command, "callstack") == 0) {
            PrintCallStack(&state->binImagePos, &state->CUs, &state->projectFilesIds,
                           procInfo->procID, procInfo->threadIDs.A[0], &state->frameMap);
        } else if (CompCStr(command, "locals") == 0) {
            PrintLocalVariables(procInfo->procID, procInfo->threadIDs.A[0],
                                &state->binImagePos, &state->CUs, &state->frameMap);
        } else if (CompCStr(command, "pe") == 0) {
            PrintPE(&targetPE);
        } else if (CompCStr(command, "section") == 0) {
            CString action = commands[1];

            if (CompCStr(action, "dwarf") == 0) {
                PrintDwarfSections();
            } else if (CompCStr(action, "abbrev") == 0) {
                PrintAbbrev(&state->CUs);
            } else if (CompCStr(action, "info") == 0) {
                //SeeTags(state->CUs.A[0].DIEChilds, state->CUs.A);
                PrintInfo(&state->CUs);
            } else if (CompCStr(action, "line") == 0) {
                PrintLineTable(state->CUs.A[0].line, &state->projectFilesIds);
            }
        } else if (CompCStr(command, "range") == 0) {
            u64 givenAddress = strtol(commands[1], 0, 0);

            if (IsInRange(&state->CUs.A[0].DIE, givenAddress)) {
                printf("it is in range.\n");
            } else {
                printf("Is not in range.\n");
            }
        } else if (CompCStr(command, "pid") == 0) {
            printf("target process ID: %llu\n", (u64)procInfo->procID);
        } else if (CompCStr(command, "filenames") == 0) {
            PrintFileNames(&state->projectFilesIds);
        } else if (CompCStr(command, "cu") == 0) {
            CString action = commands[1];
            if (CompCStr(action, "ranges") == 0) {
                PrintCUsRanges(&state->rnglistHeader, &state->CUs);
            }
        }
    } while(CompCStr(command, "exit"));

    return 0;
}

#endif //DSV_DEBUGGER_H
