#ifndef DSV_DWARF_LINE_H
#define DSV_DWARF_LINE_H

#include "DwarfForms.h"

typedef enum {
    DW_LNS_copy               = 0x01,
    DW_LNS_advance_pc         = 0x02,
    DW_LNS_advance_line       = 0x03,
    DW_LNS_set_file           = 0x04,
    DW_LNS_set_column         = 0x05,
    DW_LNS_negate_stmt        = 0x06,
    DW_LNS_set_basic_block    = 0x07,
    DW_LNS_const_add_pc       = 0x08,
    DW_LNS_fixed_advance_pc   = 0x09,
    DW_LNS_set_prologue_end   = 0x0a,
    DW_LNS_set_epilogue_begin = 0x0b,
    DW_LNS_set_isa            = 0x0c
} Dwarf_LNS_Opcodes;

typedef enum {
    DW_LNE_end_sequence      = 0x01,
    DW_LNE_set_address       = 0x02,
    //Reserved               = 0x03, Version 4
    DW_LNE_set_discriminator = 0x04,
    DW_LNE_lo_user           = 0x80,
    DW_LNE_hi_user           = 0xff
} Dwarf_LNE_Opcodes;

typedef enum {
    DW_LNCT_path            = 0x1,
    DW_LNCT_directory_index = 0x2,
    DW_LNCT_timestamp       = 0x3,
    DW_LNCT_size            = 0x4,
    DW_LNCT_MD5             = 0x5,
    DW_LNCT_lo_user         = 0x2000,
    DW_LNCT_hi_user         = 0x3fff
} Dwarf_LNCT;

typedef struct {
    String name;
    String dir;
} File_Identifier;

typedef struct {
    u64 line;
    u64 column;
    u32 fileIDIndex;
    u64 address;
} Source_Position;

#define PFI_SIZE 256
Array(Array_File_Identifier, PFI_SIZE, File_Identifier);

#define INVALID_FILE_ID_INDEX MAX_u32

//TODO(MENC): This is just a quick and smelly fix just to push this code to github and have it working correctly. GIVE ME A JOB!!!!!!.
String gBinPath;

Inter void CreateFilePath(File_Identifier *fileID, char *dest) {
    CopyCStrForward(dest, fileID->dir.str, fileID->dir.length);
    dest[fileID->dir.length] = '/';
    CopyCStrForward(dest + fileID->dir.length + 1, fileID->name.str, fileID->name.length);
    dest[fileID->dir.length + 1 + fileID->name.length] = '\0';
}

Inter void PrintSourcePosition(Source_Position *srcPos, Array_File_Identifier *fileIDs) {
    Local_Str(filePath, KiB(4));
    CreateFilePath(&fileIDs->A[srcPos->fileIDIndex], filePath.str);
    filePath.size = CStrLen(filePath.str);

    Local_Str(fullPath, KiB(4));
    CatStr(&fullPath, Get_Dir_Path(gBinPath), filePath);

    Local_Str(absPath, KiB(4));
    Build_Absolute_File_Path(&absPath, KiB(4), fullPath);

    printf("\"%s\":%llu:%llu (0x%llx)\n", absPath.str, srcPos->line, srcPos->column, srcPos->address);

    Memory_Arena* tempArena = AllocTempArena(MiB(1));

    String file;
    file.str = File_Read_All(absPath, tempArena, &file.size);
    if ( !file.str ) {
        FreeTempArena(tempArena);
        return;
    }

    String line = { 0 };
    u32 lineCounter = 1;
    for (char* scan = file.str; scan < &file.str[file.size]; ++scan) {
        if ( *scan == '\n' ) {
            if ( !line.str ) {
                ++lineCounter;
                if ( lineCounter == srcPos->line ) {
                    line.str = scan + 1;
                }
            } else {
                line.size = (u32)( scan - line.str );
                line.str[line.size] = '\0';
                break;
            }
        }
    }

    printf("%s\n", line.str);

    FreeTempArena(tempArena);
}

#define printf(...)

Inter void CreateLineTable(Dwarf_Line *line) {
    Line_Header *lineHeader = &line->header;
    byte *endLineProgram;

    if (lineHeader->format == DWARF_32_BITS) {
        endLineProgram = lineHeader->baseAddr + lineHeader->unitLength + 4;
    } else {
        endLineProgram = lineHeader->baseAddr + lineHeader->unitLength + 8;
    }

    byte *scanData = line->header.lineNumberProgram;

    line->lineTable.A = ArenaPushArray(line->lineArena, Dwarf_Line_State, UNDETERMINED_SIZE);

    Dwarf_Line_State state = {0};
    // NOTE(JENH): Transform 'local' file indices to 'global' file indices.
    state.file = line->header.projectFilesIdsIndices.A[1];
    state.line = 1;
    AddFlags(state.flags, (line->header.defaultIsStmt) ? DLSF_isStmt : 0);

    printf("\nLine Number Statements:\n");

    while (scanData < endLineProgram) {
        printf("  [0x%08llx] ", (u64)(scanData - line->header.lineNumberProgram));

        u8 opcode = *(u8 *)scanData++;

        if (opcode == 0) { // Extended opcode
            u64 size = ULEB128AndAdvance(&scanData);
            byte *scanSave = scanData;
            opcode = *(u8 *)scanData++;

            printf("Extended opcode %u: ", opcode);

            switch (opcode) {
                case DW_LNE_end_sequence: {
                    AddFlags(state.flags, DLSF_endSequence);

                    printf("\n end line number program.\n\n");

                    Dwarf_Line_State *newFileEntry = ArenaPushType(line->lineArena, Dwarf_Line_State);
                    ++line->lineTable.size;
                    *newFileEntry = state;

                    ZeroType(&state);
                    state.file = line->header.projectFilesIdsIndices.A[1];
                    state.line = 1;
                    AddFlags(state.flags, (line->header.defaultIsStmt) ? DLSF_isStmt : 0);
                } break;

                case DW_LNE_set_address: {
                    state.address  = *(u64 *)scanData;
                    state.opIndex = 0;
                    printf("Set address to 0x%08llx.\n", state.address);
                } break;

                //NOTE(JENH): Version 4 and prior.
#if 0
                case DW_LNE_define_file: {
                    printf("\n\nDW_LNE_define_file\n\n");
                } break;
#endif

                case DW_LNE_set_discriminator: {
                    state.discriminator = ULEB128AndAdvance(&scanData);
                    printf("Set discriminator to %u.\n", state.discriminator);
                } break;

                case DW_LNE_lo_user: {
                    Assert(false && "DW_LNE_lo_user");
                    //printf("\n\nDW_LNE_lo_user\n\n");
                } break;

                case DW_LNE_hi_user: {
                    Assert(false && "DW_LNE_hi_user");
                    //printf("\n\nDW_LNE_hi_user\n\n");
                } break;

                NO_DEFAULT
            }

            scanData = scanSave + size;
        } else if (opcode >= lineHeader->opcodeBase) { // Special opcode
            u32 adjusted_opcode = opcode - lineHeader->opcodeBase;
            u32 operationAdvance = adjusted_opcode / lineHeader->lineRange;

            u32 advance_address = lineHeader->minimumInstructionLength * ((state.opIndex + operationAdvance) /
                                                                           lineHeader->maximumOperationsInstructions);
            s32 advance_line = lineHeader->lineBase + (adjusted_opcode % lineHeader->lineRange);
            state.opIndex = (state.opIndex + operationAdvance) % lineHeader->maximumOperationsInstructions;

            state.address += advance_address;
            state.line    += advance_line;

            Dwarf_Line_State *newFileEntry = ArenaPushType(line->lineArena, Dwarf_Line_State);
            ++line->lineTable.size;
            *newFileEntry = state;

            DelFlags(state.flags, DLSF_basicBlock|DLSF_prologeEnd|DLSF_epilogueBegin);
            state.discriminator = 0;

            printf("Special opcode %u: advance Address by %u to 0x%llx and Line by %d to %lld.\n",
                   adjusted_opcode, advance_address, state.address, advance_line, state.line);
        } else { // Standar opcode
            printf("Standar opcode %u: ", opcode);

            switch (opcode) {
                case DW_LNS_copy: {
                    Dwarf_Line_State *newFileEntry = ArenaPushType(line->lineArena, Dwarf_Line_State);
                    ++line->lineTable.size;
                    *newFileEntry = state;

                    DelFlags(state.flags, DLSF_basicBlock|DLSF_prologeEnd|DLSF_epilogueBegin);
                    state.discriminator = 0;
                    printf("Copy.\n");
                } break;

                case DW_LNS_advance_pc: {
                    u32 operationAdvance = ULEB128AndAdvance(&scanData);
                    u32 advanceAddress   = lineHeader->minimumInstructionLength *
                                           ((state.opIndex + operationAdvance) / lineHeader->maximumOperationsInstructions);
                    state.opIndex = (state.opIndex + operationAdvance) % lineHeader->maximumOperationsInstructions;
                    state.address += advanceAddress;
                    printf("Advance PC by constant %u to 0x%llx.\n", advanceAddress, state.address);
                } break;

                case DW_LNS_advance_line: {
                    s64 advance_line = SLEB128AndAdvance(&scanData);
                    state.line += advance_line;
                    printf("Advance Line by %lld to %lld.\n", advance_line, state.line);
                } break;

                case DW_LNS_set_file: {
                    state.file = ULEB128AndAdvance(&scanData);
                    printf("Set File name to entry %u in the File Name Table.\n", state.file);
                    // NOTE(JENH): Transform 'local' file indices to 'global' file indices.
                    state.file = line->header.projectFilesIdsIndices.A[state.file];
                } break;

                case DW_LNS_set_column: {
                    state.column = ULEB128AndAdvance(&scanData);
                    printf("Set column to %llu.\n", state.column);
                } break;

                case DW_LNS_negate_stmt: {
                    SwitchFlags(state.flags, DLSF_isStmt);
                    printf("Set is_stmt to %u.\n", HasFlags(state.flags, DLSF_isStmt));
                } break;

                case DW_LNS_set_basic_block: {
                    AddFlags(state.flags, DLSF_basicBlock);
                    printf("Set basic_block to %u.\n", HasFlags(state.flags, DLSF_basicBlock));
                    Assert(false && "DW_LNS_set_basic_block");
                } break;

                case DW_LNS_const_add_pc: {
                    u32 adjustedOpcode   = (u32)0xff - lineHeader->opcodeBase;
                    u32 operationAdvance = adjustedOpcode / lineHeader->lineRange;
                    u32 advanceAddress   = lineHeader->minimumInstructionLength *
                                          ((state.opIndex + operationAdvance) / lineHeader->maximumOperationsInstructions);
                    state.opIndex = (state.opIndex + operationAdvance) % lineHeader->maximumOperationsInstructions;
                    state.address += advanceAddress;
                    printf("Advance PC by constant %u to 0x%llx.\n", advanceAddress, state.address);
                } break;

                case DW_LNS_fixed_advance_pc: {
                    u32 offset = *(u16 *)scanData;
                    scanData += sizeof(u16);
                    state.opIndex = 0;
                    printf("Add offset to %u to address 0x%llx to get 0x%llu.\n",
                           offset, state.address, state.address + offset);
                    state.address += offset;
                    Assert(false && "DW_LNS_fixed_advance_pc");
                } break;

                case DW_LNS_set_prologue_end: {
                    AddFlags(state.flags, DLSF_prologeEnd);
                    printf("Set prologue_end to %u.\n", HasFlags(state.flags, DLSF_prologeEnd));
                    Assert(false && "DW_LNS_set_prologue_end");
                } break;

                case DW_LNS_set_epilogue_begin: {
                    AddFlags(state.flags, DLSF_epilogueBegin);
                    printf("Set epilogue_begin to %u.\n", HasFlags(state.flags, DLSF_epilogueBegin));
                    Assert(false && "DW_LNS_set_epilogue_begin");
                } break;

                case DW_LNS_set_isa: {
                    state.isa = ULEB128AndAdvance(&scanData);
                    printf("Set isa to %u.\n", state.isa);
                    Assert(false && "DW_LNS_set_isa");
                } break;

                NO_DEFAULT
            }
        }
    }

    line->lineTableCreated = true;
}
#undef printf

Inter inline Array_Dwarf_Line_State *GetLineTable(Dwarf_Line *line) {
    if (!line->lineTableCreated) {
        CreateLineTable(line);
    }
    return &line->lineTable;
}

Inter void ParseLineHeader(Line_Header *lineHeader, Array_File_Identifier *projectFilesIds, Memory_Arena *lineArena,
                           Memory_Arena *stringArena, byte *lineData) {
    byte *scanData = lineData;
    lineHeader->baseAddr = scanData;

    lineHeader->unitLength = GetInitialLengthAndAdvance(&scanData, &lineHeader->format);

    lineHeader->version = *(u16 *)scanData;
    scanData += sizeof(u16);

    lineHeader->addressSize = *(u8 *)scanData;
    scanData += sizeof(u8);

    lineHeader->segmentSelectorSize = *(u8 *)scanData;
    scanData += sizeof(u8);

    lineHeader->headerLength = GetValueArchFormatAndAdvance(&scanData, lineHeader->format);

    lineHeader->minimumInstructionLength = *(u8 *)scanData;
    scanData += sizeof(u8);

    lineHeader->maximumOperationsInstructions = *(u8 *)scanData;
    scanData += sizeof(u8);

    lineHeader->defaultIsStmt = *(u8 *)scanData;
    scanData += sizeof(u8);

    lineHeader->lineBase = *(s8 *)scanData;
    scanData += sizeof(s8);

    lineHeader->lineRange = *(u8 *)scanData;
    scanData += sizeof(u8);

    lineHeader->opcodeBase = *(u8 *)scanData;
    scanData += sizeof(u8);

    lineHeader->stdOpcodeLengths.size = lineHeader->opcodeBase;

    for (u32 opcodeIndex = 1; opcodeIndex < lineHeader->stdOpcodeLengths.size; ++opcodeIndex) {
        lineHeader->stdOpcodeLengths.A[opcodeIndex] = *(u8 *)scanData;
        scanData += sizeof(u8);
    }

    //NOTE(JENH): Support prev Dwarf versions (page. 157).

    u64 columns  = ULEB128AndAdvance(&scanData);
    u32 typeCode = ULEB128AndAdvance(&scanData);
    u32 formCode = ULEB128AndAdvance(&scanData);

    Assert(typeCode == DW_LNCT_path);

    u32 directoryEntriesCount = *(u8 *)scanData;
    scanData += sizeof(u8);

#define MAX_DIRECTORY_ENTRIES 256
    String directoryEntries[MAX_DIRECTORY_ENTRIES];

    for (u32 dirEntryIndex = 0; dirEntryIndex < directoryEntriesCount; ++dirEntryIndex) {
        String *dir = &directoryEntries[dirEntryIndex];

        *dir = FormToString(formCode, &scanData, lineHeader->format);
    }

    AbbrevDefEntry fileTableDef[64];

    u64 fileTableDefSize = ULEB128AndAdvance(&scanData);

    for (u32 defIndex = 0; defIndex < fileTableDefSize; ++defIndex) {
        fileTableDef[defIndex].name = ULEB128AndAdvance(&scanData);
        fileTableDef[defIndex].form = ULEB128AndAdvance(&scanData);
    }

    lineHeader->projectFilesIdsIndices.size = *(u8 *)scanData;
    scanData += sizeof(u8);

    lineHeader->projectFilesIdsIndices.A = ArenaPushArray(lineArena, u32, lineHeader->projectFilesIdsIndices.size);

    for (u32 indicesIndex = 0; indicesIndex < lineHeader->projectFilesIdsIndices.size; ++indicesIndex) {
        //printf("% 5d ", dirIndex);

        String fileDir  = {0};
        String fileName = {0};

        for (u32 defIndex = 0; defIndex < fileTableDefSize; ++defIndex) {
            AbbrevDefEntry *defEntry = &fileTableDef[defIndex];

            switch (defEntry->name) {
                case DW_LNCT_path: {
                    fileName = FormToString(defEntry->form, &scanData, lineHeader->format);
                    //printf("%s", fileNameEntry->fileName);
                } break;

                case DW_LNCT_directory_index: {
                    u32 dirCode = (u32)FormToUConst(defEntry->form, &scanData);
                    fileDir = directoryEntries[dirCode];
                    //printf("  %lu", fileNameEntry->dirCode);
                } break;

#if 0
                case DW_LNCT_timestamp: {
                } break;

                case DW_LNCT_size: {
                } break;

                case DW_LNCT_MD5: {
                } break;
#endif

                NO_DEFAULT
            }
        }

        //foreach (File_Identifier, fileIdFake, *projectFilesIds) {
        for (u32 fileIDIndex = 1; fileIDIndex < projectFilesIds->size; ++fileIDIndex) {
            File_Identifier fileId = projectFilesIds->A[fileIDIndex];

            if ((CompStr(fileDir, fileId.dir) == 0) && (CompStr(fileName, fileId.name) == 0)) {
                //Assert(fileDir.str == fileId.dir.str && fileName.str == fileId.name.str);
                lineHeader->projectFilesIdsIndices.A[indicesIndex] = fileIDIndex;
                goto already_in;
            }
        }

        // TODO(JENH): See this.
        u32 newFileIDIndex = projectFilesIds->size++;
        File_Identifier *newFileID = &projectFilesIds->A[newFileIDIndex];
        newFileID->name = fileName;
        newFileID->dir  = fileDir;
        lineHeader->projectFilesIdsIndices.A[indicesIndex] = newFileIDIndex;

        already_in:;
    }

    lineHeader->lineNumberProgram = scanData;
}

Inter void PrintLineHeader(Line_Header *lineHeader) {
    printf("unit_length: %llu ", lineHeader->unitLength);
    if (lineHeader->format == DWARF_32_BITS) {
        printf("[32 bits]\n");
    } else { // format == DWARF_64_BITS
        printf("[64 bits]\n");
    }
    printf("version = %u\n", lineHeader->version);
    printf("address_size = %u\n", lineHeader->addressSize);
    printf("segment_selector_size = %u\n", lineHeader->segmentSelectorSize);
    printf("header_length = %llu\n", lineHeader->headerLength);
    printf("minimum_instruction_length = %u\n", lineHeader->minimumInstructionLength);
    printf("maximum_operations_instructions = %u\n", lineHeader->maximumOperationsInstructions);
    (lineHeader->defaultIsStmt) ? printf("default_is_stmt = true\n") : printf("default_is_stmt = false\n");
    printf("line_base = %d\n", lineHeader->lineBase);
    printf("line_range = %d\n", lineHeader->lineRange);
    printf("opcode_base = %d\n", lineHeader->opcodeBase);

    printf("\n");

    for (u32 opcodeIndex = 1; opcodeIndex < lineHeader->stdOpcodeLengths.size; ++opcodeIndex) {
        printf("Opcode %u has %u args\n", opcodeIndex, lineHeader->stdOpcodeLengths.A[opcodeIndex]);
    }

    printf("address number program = %p\n", lineHeader->lineNumberProgram);

#if 0
    printf("\ndirectories count = %d\n", lineHeader->directoryEntries.size);

    printf("\nEntry Name\n");

    for (u32 defIndex = 0; defIndex < lineHeader->directoryEntries.size; ++defIndex) {
        printf("% 5d %s\n", defIndex, lineHeader->directoryEntries.A[defIndex]);
    }

    printf("\nfile names count = %d\n", lineHeader->fileNameTable.size);
#endif
}

Inter void PrintLineEntry(Dwarf_Line_State *lineEntry) {
    printf("0x%08llx  [%5lld, %3llu] ", lineEntry->address, lineEntry->line, lineEntry->column);
    if (HasFlags(lineEntry->flags, DLSF_isStmt))        { printf("NS "); }
    if (HasFlags(lineEntry->flags, DLSF_basicBlock))    { printf("BB "); }
    if (HasFlags(lineEntry->flags, DLSF_endSequence))   { printf("ET "); }
    if (HasFlags(lineEntry->flags, DLSF_prologeEnd))    { printf("PE "); }
    if (HasFlags(lineEntry->flags, DLSF_epilogueBegin)) { printf("EB "); }
    if (lineEntry->isa)           { printf("IS= %u", lineEntry->isa); }
    if (lineEntry->discriminator) { printf("DI= %u", lineEntry->discriminator); }
}

Inter void PrintLineTable(Dwarf_Line *line, Array_File_Identifier *projectFilesIds) {
    PrintLineHeader(&line->header);

    Array_Dwarf_Line_State *lineTable = GetLineTable(line);

    char pathBuffer[4096];

    u32 actualFileIndex = MAX_u32;

    printf("<pc>        [ lno, col] NS BB ET PE EB IS= DI= uri: \"filepath\"\n");

    for (u32 lineEntryIndex = 0; lineEntryIndex < lineTable->size; ++lineEntryIndex) {
        Dwarf_Line_State *lineEntry = &lineTable->A[lineEntryIndex];

        PrintLineEntry(lineEntry);

        if (actualFileIndex != lineEntry->file) {
            actualFileIndex = lineEntry->file;
            File_Identifier *file = &projectFilesIds->A[lineEntry->file];
            CreateFilePath(file, pathBuffer);
            printf("rui: \"%s\" ", pathBuffer);
        }

        printf("\n");
    }
}

Inter void NextSrcPos(Source_Position *srcPos, Array_Dwarf_Compile_Unit *CUs) {
    foreach (Dwarf_Compile_Unit, CU, *CUs) {
        if (IsInRange(&CU->DIE, srcPos->address)) {
            Array_Dwarf_Line_State *lineTable = GetLineTable(CU->line);

            u32 lineTableIndex = 0;

            for (; lineTableIndex < lineTable->size; ++lineTableIndex) {
                Dwarf_Line_State *lineTableEntry = &lineTable->A[lineTableIndex];
                if (lineTableEntry->address == srcPos->address &&
                    lineTableEntry->line    ==  srcPos->line) {
                    break;
                }
            }

            for (++lineTableIndex; lineTableIndex < lineTable->size; ++lineTableIndex) {
                Dwarf_Line_State *lineTableEntry = &lineTable->A[lineTableIndex];
                if (srcPos->line != lineTableEntry->line &&
                    srcPos->address != lineTableEntry->address &&
                    HasFlags(lineTableEntry->flags, DLSF_isStmt)) {
                    srcPos->line = lineTableEntry->line;
                    srcPos->column = lineTableEntry->column;
                    srcPos->fileIDIndex = lineTableEntry->file;
                    srcPos->address = lineTableEntry->address;
                    return;
                }
            }
        }
    }

    Assert(false && "there's no more lines.");
}

Inter void GetSrcPosByAddress(Source_Position *srcPos, Array_Dwarf_Compile_Unit *CUs, u64 address) {
    foreach (Dwarf_Compile_Unit, CU, *CUs) {
        if (IsInRange(&CU->DIE, address)) {
            Array_Dwarf_Line_State *lineTable = GetLineTable(CU->line);
            u64 minAddressDif = MAX_u64;
            Dwarf_Line_State *nearestLTEntry = 0;

            foreach (Dwarf_Line_State, lineTableEntry, *lineTable) {
                u64 addressDif = address - lineTableEntry->address;
                if (addressDif < minAddressDif &&
                    HasFlags(lineTableEntry->flags, DLSF_isStmt)) {
                    if (!nearestLTEntry || nearestLTEntry->line != lineTableEntry->line) {
                        minAddressDif = address;
                        nearestLTEntry = lineTableEntry;
                    }
                }
            }

            srcPos->line = nearestLTEntry->line;
            srcPos->column = nearestLTEntry->column;
            srcPos->fileIDIndex = nearestLTEntry->file;
            srcPos->address = nearestLTEntry->address;
            return;
        }
    }

    Assert(false && "The address doesn't have an associated line.");
}

Inter u64 GetPCByLine(Array_Dwarf_Compile_Unit *CUs, Array_File_Identifier *fileIDs, u32 fileIDIndex, u64 lineNumber) {
    foreach (Dwarf_Compile_Unit, CU, *CUs) {
        if (!CU->line) { continue; }

        for (u32 i = 0; i < CU->line->header.projectFilesIdsIndices.size; ++i) {
            u32 lineFileIDIndex = CU->line->header.projectFilesIdsIndices.A[i];

            if (lineFileIDIndex != fileIDIndex) { continue; }

            Array_Dwarf_Line_State *lineTable = GetLineTable(CU->line);

            foreach (Dwarf_Line_State, lineEntry, *lineTable) {
                if (lineEntry->line == lineNumber && lineEntry->file == fileIDIndex &&
                    HasFlags(lineEntry->flags, DLSF_isStmt)) {
                    PrintLineEntry(lineEntry);
                    printf("\n");
                    return lineEntry->address;
                }
            }
        }
    }

    printf("la linea no existe.\n");
    return MAX_u64;
}

#endif //DSV_DWARF_LINE_H
