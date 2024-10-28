#ifndef DSV_FRAME_H
#define DSV_FRAME_H

#define PRIMARY_OPCODE_MASK 0b11000000

typedef enum {
    DW_CFA_advance_loc        = 0b01000000,
    DW_CFA_offset             = 0b10000000,
    DW_CFA_restore            = 0b11000000,
    DW_CFA_nop                = 0x00,
    DW_CFA_set_loc            = 0x01,
    DW_CFA_advance_loc1       = 0x02,
    DW_CFA_advance_loc2       = 0x03,
    DW_CFA_advance_loc4       = 0x04,
    DW_CFA_offset_extended    = 0x05,
    DW_CFA_restore_extended   = 0x06,
    DW_CFA_undefined          = 0x07,
    DW_CFA_same_value         = 0x08,
    DW_CFA_register           = 0x09,
    DW_CFA_remember_state     = 0x0a,
    DW_CFA_restore_state      = 0x0b,
    DW_CFA_def_cfa            = 0x0c,
    DW_CFA_def_cfa_register   = 0x0d,
    DW_CFA_def_cfa_offset     = 0x0e,
    DW_CFA_def_cfa_expression = 0x0f,
    DW_CFA_expression         = 0x10,
    DW_CFA_offset_extended_sf = 0x11,
    DW_CFA_def_cfa_sf         = 0x12,
    DW_CFA_def_cfa_offset_sf  = 0x13,
    DW_CFA_val_offset         = 0x14,
    DW_CFA_val_offset_sf      = 0x15,
    DW_CFA_val_expression     = 0x16,
    DW_CFA_lo_user            = 0x1c,
    DW_CFA_hi_user            = 0x3f
} DW_CFA;

typedef enum {
    Reg_Rule_undefined      = 0x0,
    Reg_Rule_same_value     = 0x1,
    Reg_Rule_offset         = 0x2,
    Reg_Rule_val_offset     = 0x3,
    Reg_Rule_register       = 0x4,
    Reg_Rule_expression     = 0x5,
    Reg_Rule_val_expression = 0x6,
    Reg_Rule_architectural  = 0x7
} Register_Rule;

typedef struct {
    Register_Rule rule;
    union {
        s64 offset;
        u64 regNumber;
        Mem_Block expression;
    };
} Reg_Unwinding_Rule;

typedef enum {
    CFA_Rule_undefined       = 0x0,
    CFA_Rule_register_offset = 0x1,
    CFA_Rule_expression      = 0x2,
} CFA_Rule;

typedef struct {
    CFA_Rule rule;
    union {
        struct {
            u32 reg;
            s64 offset;
        };
        Mem_Block expression;
    };
} CFA_Unwinding_Rule;

#define REGISTER_COUNT 256

Bit_Flags(Bit_Flags_Registers, REGISTER_COUNT);

typedef struct {
    Bit_Flags_Registers regsWithRules;
    u64 PC;
    CFA_Unwinding_Rule CFADef;
    Reg_Unwinding_Rule registers[REGISTER_COUNT];
} Frame_Row;

typedef struct {
    u8  format;
    u64 length;
    u64 CIEID;
    u8  version;
    u8  augmentation;
    u8  addressSize;
    u8  segmentSelectorSize;
    u16 codeAlignmentFactor;
    s16 dataAlignmentFactor;
    u16 returnAddressRegister;
    byte *initialInstructions;
} Dwarf_CIE;

typedef struct {
    u8  format;
    u64 length;
    u64 CIEPtr;
    Dwarf_CIE CIE;
    u64 initialLocation; //TODO(JENH): This should be target address.
    u64 addressRange; //TODO(JENH): This should be target address.
    byte *instructions;
} Dwarf_FDE;

// NOTE(JENH): SoA.
typedef struct {
    u32 size;
    byte **FDEAddresses;
    u64 *PCs;
} FDE_PC_Map;

Inter void ParseCIE(Dwarf_CIE *CIE, byte *scan) {
    CIE->length = GetInitialLengthAndAdvance(&scan, &CIE->format);
    CIE->CIEID = GetValueArchFormatAndAdvance(&scan, CIE->format);

    CIE->version = GetTypeAndAdvance(u8, &scan);

    CIE->augmentation = GetTypeAndAdvance(u8, &scan);
    Assert(CIE->augmentation == 0x00);

#if 0
    CIE->addressSize = *(u8 *)scan;
    scan += sizeof(u8);
    printf("address size = %u\n", CIE->addressSize);

    CIE->segmentSelectorSize = *(u8 *)scan;
    scan += sizeof(u8);
    printf("segment selector size = %u\n", CIE->segmentSelectorSize);

    //Assert(CIE->segmentSelectorSize == 0);
#endif

    CIE->codeAlignmentFactor = ULEB128AndAdvance(&scan);

    CIE->dataAlignmentFactor = SLEB128AndAdvance(&scan);

    CIE->returnAddressRegister = ULEB128AndAdvance(&scan);

    CIE->initialInstructions = scan;
}

Inter void PrintCIE(Dwarf_CIE *CIE) {
    printf("version = %u\n", CIE->version);
    printf("augmentation = %u\n", CIE->augmentation);
    printf("codeAlignmentFactor = %u\n", CIE->codeAlignmentFactor);
    printf("dataAlignmentFactor = %d\n", CIE->dataAlignmentFactor);
    printf("returnAddressRegister = %u\n", CIE->returnAddressRegister);
}

Inter void ParseFDE(Dwarf_FDE *FDE, byte *scan) {
    FDE->length = GetInitialLengthAndAdvance(&scan, &FDE->format);

    u64 CIEPtr = GetValueArchFormatAndAdvance(&scan, FDE->format);

    FDE->CIEPtr = CIEPtr;
    ParseCIE(&FDE->CIE, dwarfSections.frame.ptr + CIEPtr);

    FDE->initialLocation = GetTypeAndAdvance(u64, &scan);
    FDE->addressRange = GetTypeAndAdvance(u64, &scan);
    FDE->instructions = scan;
}

#define printf(...)

Inter void FrameInstructionsUntilPC(Frame_Row *frameRow, Dwarf_CIE *CIE, byte *instructions, u64 PC) {
    u32 indexRegStack = 0;
    Reg_Unwinding_Rule regStack[64][REGISTER_COUNT];

    byte *scan = instructions;

    while (*(u8 *)scan != DW_CFA_nop) {
        u8 opcode = GetTypeAndAdvance(u8, &scan);

        u8 primaryOpcode = opcode & PRIMARY_OPCODE_MASK;

        switch (primaryOpcode) {
            case DW_CFA_advance_loc: {
                u8 delta = opcode & ~PRIMARY_OPCODE_MASK;
                printf("DW_CFA_advance_loc(%u): PC = 0x%llx\n", delta, frameRow->PC + delta);
                if (PC < (frameRow->PC + delta)) { return; }
                frameRow->PC += delta;
            } continue;

            case DW_CFA_offset: {
                u8 regNumber = opcode & ~PRIMARY_OPCODE_MASK;
                u64 factoredOffset = ULEB128AndAdvance(&scan);
                SetBitFlag(&frameRow->regsWithRules, regNumber);
                frameRow->registers[regNumber].rule = Reg_Rule_offset;
                frameRow->registers[regNumber].offset = factoredOffset * CIE->dataAlignmentFactor;
                printf("DW_CFA_offset(%s, %lld)\n", regsMetadata.names[regNumber].str, frameRow->registers[regNumber].offset);
            } continue;

            case DW_CFA_restore: {
                u32 regNumber = opcode & ~PRIMARY_OPCODE_MASK;
                //frameRow->registers[regNumber].rule = ;
                printf("DW_CFA_restore(%s)\n", regsMetadata.names[regNumber].str);
            } continue;
        }

        switch (opcode) {
            case DW_CFA_set_loc: {
                u64 address = GetTypeAndAdvance(u64, &scan);
                printf("DW_CFA_set_loc(0x%llx): PC = 0x%llx\n", address, address);
                if (PC < address) { return; }
                frameRow->PC = address;
            } break;

            case DW_CFA_advance_loc1: {
                u8 delta = GetTypeAndAdvance(u8, &scan);
                printf("DW_CFA_advance_loc1(%u): PC = 0x%llx\n", delta, frameRow->PC + delta);
                if (PC < (frameRow->PC + delta)) { return; }
                frameRow->PC += delta;
            } break;

            case DW_CFA_advance_loc2: {
                u16 delta = GetTypeAndAdvance(u16, &scan);
                printf("DW_CFA_advance_loc2(%u): PC = 0x%llx\n", delta, frameRow->PC + delta);
                if (PC < (frameRow->PC + delta)) { return; }
                frameRow->PC += delta;
            } break;

            case DW_CFA_advance_loc4: {
                u32 delta = GetTypeAndAdvance(u32, &scan);
                printf("DW_CFA_advance_loc4(%u): PC = 0x%llx\n", delta, frameRow->PC + delta);
                if (PC < (frameRow->PC + delta)) { return; }
                frameRow->PC += delta;
            } break;

            case DW_CFA_offset_extended: {
                u32 regNumber = ULEB128AndAdvance(&scan);
                u64 factoredOffset = ULEB128AndAdvance(&scan);
                SetBitFlag(&frameRow->regsWithRules, regNumber);
                frameRow->registers[regNumber].rule = Reg_Rule_offset;
                frameRow->registers[regNumber].offset = factoredOffset * CIE->dataAlignmentFactor;
                printf("DW_CFA_offset_extended(%s, %llu)\n", regsMetadata.names[regNumber].str,
                       frameRow->registers[regNumber].offset);
            } break;

            case DW_CFA_restore_extended: {
                u32 regNumber = ULEB128AndAdvance(&scan);
                //frameRow->registers[regNumber].rule = ;
                printf("DW_CFA_offset_extended(%s)\n", regsMetadata.names[regNumber].str);
            } break;

            case DW_CFA_undefined: {
                u32 regNumber = ULEB128AndAdvance(&scan);
                UnsetBitFlag(&frameRow->regsWithRules, regNumber);
                frameRow->registers[regNumber].rule = Reg_Rule_undefined;
                printf("DW_CFA_undefined(%s)\n", regsMetadata.names[regNumber].str);
            } break;

            case DW_CFA_same_value: {
                u32 regNumber = ULEB128AndAdvance(&scan);
                SetBitFlag(&frameRow->regsWithRules, regNumber);
                frameRow->registers[regNumber].rule = Reg_Rule_same_value;
                printf("DW_CFA_same_value(%s)\n", regsMetadata.names[regNumber].str);
            } break;

            case DW_CFA_register: {
                u32 regNumber = ULEB128AndAdvance(&scan);
                SetBitFlag(&frameRow->regsWithRules, regNumber);
                frameRow->registers[regNumber].rule = Reg_Rule_register;
                frameRow->registers[regNumber].regNumber = ULEB128AndAdvance(&scan);
                printf("DW_CFA_register(%s, %llu)\n", regsMetadata.names[regNumber].str,
                       frameRow->registers[regNumber].regNumber);
            } break;

            case DW_CFA_remember_state: {
                // TODO(JENH): Should save bit flags too.
                Assert(indexRegStack < 64);

                //ArrayCopy(&regStack[0][0], &frameRow->registers, 1);
                CopyForward(&regStack[indexRegStack++], &frameRow->registers, REGISTER_COUNT * sizeof(Reg_Unwinding_Rule));
                printf("DW_CFA_remember_state\n");
            } break;

            case DW_CFA_restore_state: {
                // TODO(JENH): Should save bit flags too.

                //ArrayCopy(&frameRow->registers, &regStack[--indexRegStack], REGISTER_COUNT);
                CopyForward(&frameRow->registers, &regStack[--indexRegStack], REGISTER_COUNT * sizeof(Reg_Unwinding_Rule));
                printf("DW_CFA_restore_state\n");
            } break;

            case DW_CFA_def_cfa: {
                frameRow->CFADef.rule = CFA_Rule_register_offset;
                frameRow->CFADef.reg = ULEB128AndAdvance(&scan);
                frameRow->CFADef.offset = ULEB128AndAdvance(&scan);
                printf("DW_CFA_def_cfa(%s, %lld)\n", regsMetadata.names[frameRow->CFADef.reg].str, frameRow->CFADef.offset);
            } break;

            case DW_CFA_def_cfa_register: {
                Assert(frameRow->CFADef.rule == CFA_Rule_register_offset);
                frameRow->CFADef.reg  = ULEB128AndAdvance(&scan);
                printf("DW_CFA_def_cfa_register(%s)\n", regsMetadata.names[frameRow->CFADef.reg].str);
            } break;

            case DW_CFA_def_cfa_offset: {
                Assert(frameRow->CFADef.rule == CFA_Rule_register_offset);
                frameRow->CFADef.offset = ULEB128AndAdvance(&scan);
                printf("DW_CFA_def_cfa_offset(%lld)\n", frameRow->CFADef.offset);
            } break;

            case DW_CFA_def_cfa_expression: {
                frameRow->CFADef.rule = CFA_Rule_expression;
                frameRow->CFADef.expression = FormToExprloc(DW_FORM_exprloc, &scan);
                //printf("DW_CFA_def_cfa_offset(%llu)\n", frameRow->CFADef.offset);
            } break;

            case DW_CFA_expression: {
                u32 regNumber = ULEB128AndAdvance(&scan);
                SetBitFlag(&frameRow->regsWithRules, regNumber);
                frameRow->registers[regNumber].rule = Reg_Rule_expression;
                frameRow->registers[regNumber].expression = FormToMemBlock(DW_FORM_block, &scan);
                //printf("DW_CFA_def_cfa_offset(%llu)\n", frameRow->CFADef.offset);
            } break;

            case DW_CFA_offset_extended_sf: {
                u32 regNumber = ULEB128AndAdvance(&scan);
                s64 factoredOffset = SLEB128AndAdvance(&scan);
                SetBitFlag(&frameRow->regsWithRules, regNumber);
                frameRow->registers[regNumber].rule = Reg_Rule_offset;
                frameRow->registers[regNumber].offset = factoredOffset * CIE->dataAlignmentFactor;
                printf("DW_CFA_offset_extended_sf(%s, %lld)\n", regsMetadata.names[regNumber].str,
                       frameRow->registers[regNumber].offset);
            } break;

            case DW_CFA_def_cfa_sf: {
                frameRow->CFADef.rule = CFA_Rule_register_offset;
                frameRow->CFADef.reg = ULEB128AndAdvance(&scan);
                s64 factoredOffset = SLEB128AndAdvance(&scan);
                frameRow->CFADef.offset = factoredOffset * CIE->dataAlignmentFactor;
                printf("DW_CFA_def_cfa_sf(%s, %lld)\n", regsMetadata.names[frameRow->CFADef.reg].str,
                       frameRow->CFADef.offset);
            } break;

            case DW_CFA_def_cfa_offset_sf: {
                Assert(frameRow->CFADef.rule == CFA_Rule_register_offset);
                s64 factoredOffset = SLEB128AndAdvance(&scan);
                frameRow->CFADef.offset = factoredOffset * CIE->dataAlignmentFactor;
                printf("DW_CFA_def_cfa_sf(%lld)\n", frameRow->CFADef.offset);
            } break;

            case DW_CFA_val_offset: {
                u32 regNumber = ULEB128AndAdvance(&scan);
                u64 factoredOffset = ULEB128AndAdvance(&scan);
                SetBitFlag(&frameRow->regsWithRules, regNumber);
                frameRow->registers[regNumber].rule = Reg_Rule_val_offset;
                frameRow->registers[regNumber].offset = factoredOffset * CIE->dataAlignmentFactor;
                printf("DW_CFA_val_offset(%s, %lld)\n", regsMetadata.names[regNumber].str,
                       frameRow->registers[regNumber].offset);
            } break;

            case DW_CFA_val_offset_sf: {
                u32 regNumber = ULEB128AndAdvance(&scan);
                s64 factoredOffset = SLEB128AndAdvance(&scan);
                SetBitFlag(&frameRow->regsWithRules, regNumber);
                frameRow->registers[regNumber].rule = Reg_Rule_val_offset;
                frameRow->registers[regNumber].offset = factoredOffset * CIE->dataAlignmentFactor;
                printf("DW_CFA_val_offset_sf(%s, %lld)\n", regsMetadata.names[regNumber].str,
                       frameRow->registers[regNumber].offset);
            } break;

            case DW_CFA_val_expression: {
                u32 regNumber = ULEB128AndAdvance(&scan);
                SetBitFlag(&frameRow->regsWithRules, regNumber);
                frameRow->registers[regNumber].rule = Reg_Rule_val_expression;
                frameRow->registers[regNumber].expression = FormToMemBlock(DW_FORM_block, &scan);
                //printf("DW_CFA_val_offset_sf(%s, %lld)\n", regsMetadata.names[regNumber], factoredOffset);
            } break;
        }
    }

    printf("end\n");
}

#undef printf

Inter u64 GetCFA(CFA_Unwinding_Rule *CFADef, Registers *regs) {
    switch (CFADef->rule) {
        case CFA_Rule_undefined: {
            return 0;
        } break;

        case CFA_Rule_register_offset: {
            return GetLoadedRegister(regs, CFADef->reg) + CFADef->offset;
        } break;

        case CFA_Rule_expression: {
            Assert(false && "Need support for espressions.");
        } break;

        NO_DEFAULT
    }

    Assert(false && "Shouldn't be here.");
    return 0;
}

Inter u64 GetRegInFrameRow(Frame_Row *frameRow, Process_ID procID, u64 CFA, u32 regNumber) {
    Reg_Unwinding_Rule *reg = &frameRow->registers[regNumber];

    switch (reg->rule) {
        case Reg_Rule_undefined: {
            printf(":(\n");
        } break;

        case Reg_Rule_same_value: {
            return 0; //GetRegister(threadID, regNumber);
        } break;

        case Reg_Rule_offset: {
            u64 value;
            ReadProcMemType(procID, &value, (void *)(CFA + reg->offset));
            return value;
        } break;

        case Reg_Rule_val_offset: {
            return (u64)(CFA + reg->offset);
        } break;

        case Reg_Rule_register: {
            return GetRegInFrameRow(frameRow, procID, CFA, reg->regNumber);
        } break;

        case Reg_Rule_expression: {
            Assert(false && "Need support for espressions.");
        } break;

        case Reg_Rule_val_expression: {
            Assert(false && "Need support for espressions.");
        } break;

        case Reg_Rule_architectural: {
            Assert(false && "uhhh");
        } break;

        NO_DEFAULT
    }

    Assert(false && "Shouldn't be here.");
    return 0;
}

Inter byte *FindFDE(FDE_PC_Map *frameMap, u64 pc) {
    u32 low  = 0;
    u32 high = frameMap->size;

    while (low <= high) {
        u32 index = ((low + high) / 2);

        if (frameMap->PCs[index] <= pc && pc <= frameMap->PCs[index + 1]) {
            return frameMap->FDEAddresses[index];
        }

        if (frameMap->PCs[index] <= pc) {
            low  = index + 1;
        } else {
            high = index - 1;
        }
    }

    Assert(false);
    return 0;
}

Inter void GetFrameRowAtPC(Frame_Row *frameRow, Dwarf_FDE *FDE, u64 PC) {
    frameRow->PC = FDE->initialLocation;

    FrameInstructionsUntilPC(frameRow, &FDE->CIE, FDE->CIE.initialInstructions, 0xffffffff);
    FrameInstructionsUntilPC(frameRow, &FDE->CIE, FDE->instructions, PC);
}

Inter void CreateFDEPCMap(Memory_Arena *dwarfArena, FDE_PC_Map *frameMap, Dwarf_Section *frame) {
    Memory_Arena *tempAddressesArena = AllocTempArena(frame->size / 2);
    byte **FDEAddresses = ArenaPushArray(tempAddressesArena, byte *, UNDETERMINED_SIZE);

    Memory_Arena *tempPCsArena = AllocTempArena(frame->size / 2);
    u64 *PCs = ArenaPushArray(tempPCsArena, u64, UNDETERMINED_SIZE);

    frameMap->size = 0;

    byte *scan = frame->ptr;

    while (INFINITE_LOOP) {
        byte *baseFrameObject = scan;
        byte *baseToJump;

        u8  format;
        u64 length;
        u64 CIEID;

        length = GetInitialLengthAndAdvance(&scan, &format);
        baseToJump = scan;

        if (length == 0) { break; }

        CIEID = GetValueArchFormatAndAdvance(&scan, format);

        if ((format == DWARF_32_BITS && CIEID != 0xffffffff)        ||
            (format == DWARF_64_BITS && CIEID != 0xffffffffffffffff)) {
            Assert(frameMap->size < tempPCsArena->size);

            FDEAddresses[frameMap->size] = baseFrameObject;
            PCs[frameMap->size] = *(u64 *)scan;

            // NOTE(JENH): Add map entry sorted.
            for (s32 index = frameMap->size - 1; index > 0; --index) {
                if (PCs[index] > PCs[index + 1]) {
                    SWAP(PCs[index + 1], PCs[index], u64);
                    SWAP(FDEAddresses[index + 1], FDEAddresses[index], byte *);
                }
            }

            ++frameMap->size;
        }

        scan = baseToJump + length;
    }

    // NOTE(JENH): Add MAX_u64 to make binary search to work at the last element.
    FDEAddresses[frameMap->size] = (byte *)MAX_u64;
    PCs[frameMap->size] = MAX_u64;
    ++frameMap->size;

    frameMap->FDEAddresses = ArenaPushArray(dwarfArena, byte *, frameMap->size);
    ArrayCopy(frameMap->FDEAddresses, FDEAddresses, frameMap->size);

    frameMap->PCs = ArenaPushArray(dwarfArena, u64, frameMap->size);
    ArrayCopy(frameMap->PCs, PCs, frameMap->size);
}

Inter void PrintMap(FDE_PC_Map *frameMap) {
    for (u32 mapIndex = 0; mapIndex < frameMap->size; ++mapIndex) {
        printf("%u. 0x%llx 0x%llx\n", mapIndex, (u64)frameMap->FDEAddresses[mapIndex], frameMap->PCs[mapIndex]);
    }
}

#endif //DSV_FRAME_H
