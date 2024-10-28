#ifndef DSV_DWARF_H
#define DSV_DWARF_H

#define STD_OPCODE_LENGTHS_SIZE 15
#define DWARF_32_BITS 1
#define DWARF_64_BITS 2

#include "DwarfDef.h"
#include "DwarfExpr.h"

GlobalVariable Dwarf_Sections dwarfSections = {0};

#define MoveDataCursorOrCrash(DIE, attrib)              \
    do {                                                \
        if (!MoveDataCursor(DIE, attrib)) {             \
            Assert(false && "attrib does not exists."); \
        }                                               \
    } while (0)

#define CheckDataCursor(DIE, attrib)                              \
    do {                                                          \
        if (DIE->abbrev->def.A[DIE->cursorData].name != attrib) { \
            MoveDataCursorOrCrash(DIE, attrib);                   \
        }                                                         \
    } while(0)

#define GetDIEFlag(funcName, attrib)                    \
    Inter inline b32 GetDIE##funcName(Dwarf_DIE *DIE) { \
        CheckDataCursor(DIE, attrib);                   \
        return DIE->data[DIE->cursorData].flag;         \
    }

#define GetDIEString(funcName, attrib)                     \
    Inter inline String GetDIE##funcName(Dwarf_DIE *DIE) { \
        CheckDataCursor(DIE, attrib);                      \
        return DIE->data[DIE->cursorData].string;          \
    }

#define GetDIEAddress(funcName, attrib)                 \
    Inter inline u64 GetDIE##funcName(Dwarf_DIE *DIE) { \
        CheckDataCursor(DIE, attrib);                   \
        return DIE->data[DIE->cursorData].address;      \
    }

#define GetDIERangelist(funcName, attrib)                 \
    Inter inline byte *GetDIE##funcName(Dwarf_DIE *DIE) { \
        CheckDataCursor(DIE, attrib);                     \
        return DIE->data[DIE->cursorData].rnglist;        \
    }

#define GetDIEExprloc(funcName, attrib)                       \
    Inter inline Mem_Block GetDIE##funcName(Dwarf_DIE *DIE) { \
        CheckDataCursor(DIE, attrib);                         \
        return DIE->data[DIE->cursorData].exprloc;            \
    }

typedef enum {
    DLSF_isStmt        = (1 << 0),
    DLSF_basicBlock    = (1 << 1),
    DLSF_endSequence   = (1 << 2),
    DLSF_prologeEnd    = (1 << 3),
    DLSF_epilogueBegin = (1 << 4)
} Dwarf_Line_State_Flags;

typedef struct {
    u64 address;
    u64 opIndex;
    u32 file;
    u64 line;
    u64 column;
    u8  flags;
    u32 isa;
    u32 discriminator;
} Dwarf_Line_State;

Array_Ptr(Array_Dwarf_Line_State, Dwarf_Line_State);
Array(Array_Std_Opcode_lengths, STD_OPCODE_LENGTHS_SIZE, u8);

typedef struct {
    byte *baseAddr;
    u8  format;
    u64 unitLength;
    u32 version;
    u32 addressSize;
    u32 segmentSelectorSize;
    u64 headerLength;
    u32 minimumInstructionLength;
    u32 maximumOperationsInstructions;
    u32 defaultIsStmt;
    s32 lineBase;
    u32 lineRange;
    u32 opcodeBase;

    Array_Std_Opcode_lengths stdOpcodeLengths;
    Array_u32 projectFilesIdsIndices;

    byte *lineNumberProgram;
} Line_Header;

typedef struct {
    Line_Header header;
    b32 lineTableCreated;
    Memory_Arena *lineArena;
    Array_Dwarf_Line_State lineTable;
} Dwarf_Line;

typedef struct {
    u32 code;
    Array_s64 implConst;
} Code_ImplConst;

Array_Ptr(Array_Code_ImplConst, Code_ImplConst);

typedef struct {
    u16 name;
    u16 form;
} AbbrevDefEntry, Abbrev_Def_Entry;

Array_Ptr(Array_Abbrev_Def_Entry, Abbrev_Def_Entry);

typedef struct {
    Dwarf_Tag tag;
    b32 hasChildren;
    Array_Abbrev_Def_Entry def;
} Dwarf_Abbrev_Entry;

Array_Ptr(Array_Dwarf_Abbrev_Entry, Dwarf_Abbrev_Entry);

struct Dwarf_Compile_Unit;

typedef struct {
    Dwarf_Class dwarfClass;

    union {
        u64 address;
        Mem_Block block;
        u64 uConst;
        s64 sConst;
        String string;
        b32 flag;
        u64 reference;
        Mem_Block exprloc;
        byte *loclist;
        byte *rnglist;
        byte *addr;
        byte *line;
        byte *mac;
        byte *strOffsets;
    };
} DIE_Value;

#if 0
    DW_CLASS_address,
    DW_CLASS_block,

    //DW_CLASS_constant,

    DW_CLASS_uconstant,
    DW_CLASS_sconstant,
    DW_CLASS_string,
    DW_CLASS_flag,
    DW_CLASS_reference,
    DW_CLASS_loclist,
    DW_CLASS_exprloc,
    DW_CLASS_rnglist,
    DW_CLASS_addrptr,
    DW_CLASS_lineptr,
    //DW_CLASS_loclistsptr,
    DW_CLASS_macptr,
    //DW_CLASS_rnglistsptr,
    //DW_CLASS_stroffsetsptr,
#endif

#define MAX_DIE_DATA 40

// TODO(JENH): Maybe implement cursor or offset/pointer array or data array with polimorfism.
typedef struct {
    u32 code;
    byte *dataRaw;
    Dwarf_Abbrev_Entry *abbrev;
    struct Dwarf_Compile_Unit *CU;

    u32 cursorAbbrev; // NOTE(JENH): It's just an index.
    byte *cursorDataRaw; // NOTE(JENH): It's just an address.
    u32 treeDepth;
    b32 prevHasChildren;

    u32 cursorData; // NOTE(JENH): It's just an index.
    DIE_Value data[MAX_DIE_DATA];
} Dwarf_DIE;

typedef struct {
    Array_Dwarf_Abbrev_Entry table;
    Array_Code_ImplConst codeImplConst;
} Dwarf_Abbrev;

typedef struct {
    u32 format;
    u64 unitLength;
    u32 version;
    u32 unitType;
    u32 addressSize;

    Dwarf_Abbrev abbrev;
} Compile_Unit_Header;

typedef struct Dwarf_Compile_Unit {
    Compile_Unit_Header header;

    String name;
    String compDir;
    String producer;
    Dwarf_Lang language;
    Dwarf_Line *line;
    u64 defaultBaseAddress;

    byte *baseAddr;
    byte *DIEChilds;
    Dwarf_DIE DIE;
} Dwarf_Compile_Unit;

Array_Ptr(Array_Dwarf_Compile_Unit, Dwarf_Compile_Unit);

typedef struct {
    byte *secBaseAddress;
    u32 format;
    u64 unitLength;
    u32 version;
    u32 addressSize;
    u32 segmentSelectorSize;
    union {
        Array_u32 offsets32;
        Array_u64 offsets64;
    };
    Array_byte rangeListsAddress;
} Dwarf_Range_List_Header;

GlobalVariable Dwarf_Range_List_Header rnglistHeader;

Inter void PrintTag(Dwarf_Tag tag, CString format) {
    if (!format) { format = "%s"; }

    switch(tag) {
        #define X(enumName, enumVal) case enumName: { printf(format, #enumName); } break;
        LIST_DWARF_TAG
        #undef X

        default:{
            printf("\033[0;31m");
            printf("DW_TAG 0x%x", tag);
            printf("\033[0m");
        } break;
    }
}

Inter void PrintAttrib(u16 name, CString format) {
    if (!format) { format = "%s"; }

    switch(name) {
        #define X(enumName, enumVal) case enumName: { printf(format, #enumName); } break;
        LIST_DWARF_AT
        #undef X

        default: {
            printf("\033[0;31m");
            printf("DW_AT: 0x%x", name);
            printf("\033[0m");
        } break;
    }
}

Inter void PrintForm(u16 form, CString format) {
    if (!format) { format = "%s"; }

    switch(form) {
        #define X(enumName, enumVal) case enumName: { printf(format, #enumName); } break;
        LIST_DWARF_FORM
        #undef X

        default: {
            printf("\033[0;31m");
            printf("DW_FORM: 0x%x", form);
            printf("\033[0m");
        } break;
    }
}

Inter b32 MoveDataCursor(Dwarf_DIE *DIE, u16 name) {
    for (u32 defIndex = 0; defIndex < DIE->abbrev->def.size; ++defIndex) {
        Abbrev_Def_Entry *defEntry = &DIE->abbrev->def.A[defIndex];
        //PrintAttrib(defEntry->name, "%s\n");

        if (defEntry->name == name) {
            DIE->cursorData = defIndex;
            return true;
        }
    }

    return false;
}

GetDIEString(Name, DW_AT_name)
GetDIEAddress(LowPC, DW_AT_low_pc)
GetDIERangelist(Ranges, DW_AT_ranges)
GetDIEFlag(Prototyped, DW_AT_prototyped)
GetDIEExprloc(FrameBase, DW_AT_frame_base)
GetDIEExprloc(Location, DW_AT_location)

Inter u64 GetDIEHighPC(Dwarf_DIE *DIE) {
    DIE_Value *DIEValue = &DIE->data[DIE->cursorData];

    switch (DIEValue->dwarfClass) {
        case DW_CLASS_uconstant: {
            if (MoveDataCursor(DIE, DW_AT_low_pc)) {
                u64 offset = DIEValue->uConst;
                return GetDIELowPC(DIE) + offset;
            }
            Assert(false);
        } break;

        case DW_CLASS_address: {
            return DIEValue->address;
        } break;
    }

    PrintClass(DIEValue->dwarfClass, "%s\n");
    printf("Something went worng.\n");
    return 0;
}

Inter inline void UpdateDIE(Dwarf_DIE *DIE) {
    Assert(DIE->CU      != 0);
    Assert(DIE->dataRaw != 0);

    while (INFINITE_LOOP) {
        DIE->code = ULEB128AndAdvance(&DIE->dataRaw);
        if (DIE->code != 0 ) { break; }
        DIE->prevHasChildren = false;
        if (DIE->treeDepth == 0) { break; }
        --DIE->treeDepth;
    }

    DIE->abbrev = &DIE->CU->header.abbrev.table.A[DIE->code];
    if (DIE->prevHasChildren) { ++DIE->treeDepth; }
    DIE->prevHasChildren = DIE->abbrev->hasChildren;

    DIE->cursorDataRaw = DIE->dataRaw;
    DIE->cursorAbbrev  = 0;
}

Inter inline void InitializeDIE(Dwarf_DIE *DIE, byte *dataRaw, Dwarf_Compile_Unit *CU) {
    DIE->CU = CU;
    DIE->dataRaw = dataRaw;
    DIE->treeDepth = 1;
    DIE->prevHasChildren = false;
    UpdateDIE(DIE);
}

#include "DwarfForms.h"
#include "RangeList.h"
#include "Frame.h"

Inter b32 IsInRange(Dwarf_DIE *DIE, u64 givenAddress) {
    if (MoveDataCursor(DIE, DW_AT_ranges)) {
        byte *ranges = GetDIERanges(DIE);
        //PrintRangeListEntry(&rnglistHeader, ranges, DIE->CU->defaultBaseAddress);
        return IsInRangeRnglist(&rnglistHeader, ranges, DIE->CU->defaultBaseAddress, givenAddress);
    } if (MoveDataCursor(DIE, DW_AT_low_pc)) {
        u64 lowPC  = GetDIELowPC(DIE);
        u64 highPC = lowPC;

        if (MoveDataCursor(DIE, DW_AT_high_pc)) {
            highPC = GetDIEHighPC(DIE);
        }

        //printf("%llx <= %llx <= %llx\n", lowPC, givenAddress, highPC);
        return (b32)(lowPC <= givenAddress && givenAddress <= highPC);
    } else {
        //printf("Doesn't have range associated attirbutes.\n");
        return false;
    }

    return false;
}

Inter b32 GetFunctionByAddress(Dwarf_DIE *DIE, Array_Dwarf_Compile_Unit *CUs, u64 address) {
    foreach (Dwarf_Compile_Unit, CU, *CUs) {
        if (!IsInRange(&CU->DIE, address)) { continue; }

        for (InitializeDIE(DIE, CU->DIEChilds, CU); DIE->treeDepth != 0; NextDIE(DIE)) {
            if (DIE->abbrev->tag == DW_TAG_subprogram) {
                ParseDIE(DIE);
                if (IsInRange(DIE, address)) {
                    return true;
                }
            }
        }
    }

    return false;
}

Inter Dwarf_Compile_Unit *GetCUByAddress(Array_Dwarf_Compile_Unit *CUs, u64 PC) {
    foreach (Dwarf_Compile_Unit, CU, *CUs) {
        if (IsInRange(&CU->DIE, PC)) { return CU; }
    }
    return 0;
}

Inter void GetFunctionByName(Dwarf_DIE *DIE, Array_Dwarf_Compile_Unit *CUs, String name) {
    foreach (Dwarf_Compile_Unit, CU, *CUs) {
        for (InitializeDIE(DIE, CU->DIEChilds, CU); DIE->treeDepth != 0; NextDIE(DIE)) {
            if (DIE->abbrev->tag == DW_TAG_subprogram) {
                if (MoveDataCursor(DIE, DW_AT_prototyped) && GetDIEPrototyped(DIE)) {
                    continue;
                }

                ParseDIE(DIE);
                if (MoveDataCursor(DIE, DW_AT_name)) {
                    String funcName = GetDIEName(DIE);
                    //printf("funcName = %s\n", funcName.str);
                    if (CompStr(funcName, name) == 0) {
                        return;
                    }
                } else { Assert(false && "should have a name."); }
            }
        }
    }
}

#include "Dwarf_line.h"

Inter void InitializeDwarfSections() {
    ZeroType(&dwarfSections);
    #define X(enumName, structField, nameLit)                    \
	    dwarfSections.structField.name.str    = nameLit;         \
	    dwarfSections.structField.name.length = strlen(nameLit);
    LIST_DWARF_DEBUG_SECTIONS
    #undef X
}

//TODO(JENH): attrib look ups clould be faster by changing this to an SoA to use SIMD.
Inter void PrintDwarfSections() {
    for (Dwarf_Section *dwarfSection = dwarfSections.A; dwarfSection->name.str; ++dwarfSection) {
        if (dwarfSection->ptr) {
	        printf("%s: Ptr = %p | Size = %d.\n", dwarfSection->name.str, dwarfSection->ptr, dwarfSection->size);
	    }
    }
}

#if 0
Inter u32 AdvanceScanToDIEAttrib(Array_Abbrev_Def_Entry *abbrevDef, byte **scan, Dwarf_Compile_Unit *CU, u16 name) {
    //for (AbbrevDefEntry *defEntry = abbrevDef; defEntry->name && abbrevDefEntry->form; ++abbrevDefEntry) {
    foreach (Abbrev_Def_Entry, defEntry, *abbrevDef) {
        if (defEntry->name == name) {
            // TODO(JENH): Should resolve DW_FORM_indirect.
            return defEntry->form;
        }

        NextDIEValue(scan, defEntry->name, defEntry->form, 1, abbrevDef, CU);
    }

    return DW_FORM_nul;
}
#endif

Inter void PrintAbbrevDef(Dwarf_Abbrev_Entry *abbrevEntry) {
    PrintTag(abbrevEntry->tag, "%-25s ");
    printf((abbrevEntry->hasChildren) ? "[has children]\n" : "[no children]\n");

    foreach (Abbrev_Def_Entry, defEntry, abbrevEntry->def) {
        printf("      ");

        PrintAttrib(defEntry->name, "%-18s ");
        PrintForm(defEntry->form, 0);

        printf("\n");
    }
}

Inter void PrintAbbrev(Array_Dwarf_Compile_Unit *CUs) {
    foreach (Dwarf_Compile_Unit, CU, *CUs) {
        Dwarf_Abbrev *abbrev = &CU->header.abbrev;
        for (u32 code = 1; code < abbrev->table.size; ++code) {
            Dwarf_Abbrev_Entry *abbrevEntry = &abbrev->table.A[code];

            printf("% 2d ", code);

            PrintTag(abbrevEntry->tag, "%-25s ");
            printf((abbrevEntry->hasChildren) ? "[has children]\n" : "[no children]\n");

            foreach (Abbrev_Def_Entry, defEntry, abbrevEntry->def) {
                printf("      ");

                PrintAttrib(defEntry->name, "%-18s ");
                PrintForm(defEntry->form, 0);

                if (defEntry->form == DW_FORM_implicit_const) {
                    s64 implicitConst = GetImplicitConst(&abbrev->codeImplConst, &abbrevEntry->def,
                                                         code, defEntry->name);
                    printf(": %lld", implicitConst);
                }

                printf("\n");
            }
        }
    }
}

Inter u32 GetimplConstSize(byte *abbrevDef) {
    byte *scan = abbrevDef;
    u32 implConstSize = 0;

    while (INFINITE_LOOP) {
        u16 name = ULEB128AndAdvance(&scan);
        u16 form = ULEB128AndAdvance(&scan);

        if (name == DW_AT_nul && form == DW_FORM_nul) {
            return implConstSize;
        }

        if (form == DW_FORM_implicit_const) {
            s64 implConst = SLEB128AndAdvance(&scan);
            ++implConstSize;
        }
    }
}

Inter u32 GetAbbrevDefSize(byte *abbrevDef) {
    byte *scan = abbrevDef;
    u32 abbrevDefSize = 0;

    while (INFINITE_LOOP) {
        u16 name = ULEB128AndAdvance(&scan);
        u16 form = ULEB128AndAdvance(&scan);

        if (name == DW_AT_nul && form == DW_FORM_nul) { return abbrevDefSize; }

        if (form == DW_FORM_implicit_const) {
            s64 implConst = SLEB128AndAdvance(&scan);
        }

        ++abbrevDefSize;
    }
}

Inter void ParseDwarfAbbrev(Dwarf_Abbrev *abbrev, Memory_Arena *dwarfArena, byte *abbrevData) {
    // TODO(JENH): The temp arena size should be better.
    Memory_Arena *abbrevTableArena = AllocTempArena(dwarfSections.abbrev.size);
    Array_Dwarf_Abbrev_Entry tempAbbrevTable = {0};
    tempAbbrevTable.A = ArenaPushArray(abbrevTableArena, Dwarf_Abbrev_Entry, UNDETERMINED_SIZE);

    Memory_Arena *codeImplConstArena = AllocTempArena(dwarfSections.abbrev.size);
    Array_Code_ImplConst tempCodeImplConst = {0};
    tempCodeImplConst.A = ArenaPushArray(codeImplConstArena, Code_ImplConst, UNDETERMINED_SIZE);

    for (byte *dataScan = abbrevData;;) {
        u32 code = (u64)ULEB128AndAdvance(&dataScan);
        //printf("code = %u\n", code);

        if (code == 0) { break; }

        // NOTE(JENH): Just in case that, for some reason, the extracted code is greater than the
        //             previous code + 1. So the table is updated correctly.
        if (tempAbbrevTable.size <= code) {
            ArenaPushArray(abbrevTableArena, Dwarf_Abbrev_Entry, code - tempAbbrevTable.size + 1);
            tempAbbrevTable.size = code + 1;
        }

        Dwarf_Abbrev_Entry *abbrevEntry = &tempAbbrevTable.A[code];

        abbrevEntry->tag = (Dwarf_Tag)ULEB128AndAdvance(&dataScan);
        abbrevEntry->hasChildren = (b32)*dataScan++;
        ArrayAlloc(abbrevEntry->def, GetAbbrevDefSize(dataScan), dwarfArena);

	    //printf("%2u ", code);
        //PrintTag(abbrevEntry->tag, "%-25s ");
        //printf((abbrevEntry->hasChildren) ? "[has children]\n" : "[no children]\n");

        Code_ImplConst *codeImplConst = 0;
        u32 implConstSize = GetimplConstSize(dataScan);

        if (implConstSize != 0) {
            codeImplConst = ArenaPushType(codeImplConstArena, Code_ImplConst);
            ++tempCodeImplConst.size;
            codeImplConst->code = code;
            ArrayAlloc(codeImplConst->implConst, implConstSize, dwarfArena);
        }

        u32 indexImplConst = 0;

        foreach (Abbrev_Def_Entry, defEntry, abbrevEntry->def) {
            defEntry->name = ULEB128AndAdvance(&dataScan);
            defEntry->form = ULEB128AndAdvance(&dataScan);

            //PrintAttrib(defEntry->name, "%-18s ");
            //PrintForm(defEntry->form, 0);

            if (defEntry->form == DW_FORM_implicit_const) {
                codeImplConst->implConst.A[indexImplConst++] = SLEB128AndAdvance(&dataScan);
                //printf("%lld", codeImplConst->implConst.A[indexImplConst - 1]);
            }

            //printf("\n");
        }
        //printf("\n");

        // TODO(JENH): abbrevDef.size + 1?
#if 0
        AbbrevDefEntry *nul = ArenaPushType(abbrevDef, AbbrevDefEntry);
        ZeroMem(nul, sizeof(nul));
#endif
        dataScan += 2;
    }
    //printf("hola 1\n");

    // TODO(JENH): Push the constents of the temp arena into the dwarfArena one.
    ArrayAlloc(abbrev->table, tempAbbrevTable.size, dwarfArena);
    ArrayCopy(abbrev->table.A, tempAbbrevTable.A, tempAbbrevTable.size);
    //CopyCStrForward((char *)abbrev->table.A, (char *)tempAbbrevTable.A, tempAbbrevTable.size * sizeof(Dwarf_Abbrev_Entry));

    ArrayAlloc(abbrev->codeImplConst, tempCodeImplConst.size, dwarfArena);
    ArrayCopy(abbrev->codeImplConst.A, tempCodeImplConst.A, tempCodeImplConst.size);
    //CopyCStrForward((char *)abbrev->codeImplConst.A, (char *)tempAbbrevTable.A, tempAbbrevTable.size * sizeof(Dwarf_Abbrev_Entry));

    FreeTempArena(abbrevTableArena);
    FreeTempArena(codeImplConstArena);
}

Inter u32 GetCompileUnitCount(Dwarf_Section *info) {
    byte *scanData = info->ptr;
    u32 CUCount = 0;

    while (INFINITE_LOOP) {
        u64 unitLength;

        if (((u64)*scanData & 0xFFFFFFFF) == 0xFFFFFFFF) { // format == DWARF_64_BITS
            unitLength = (*(u64 *)scanData) >> 16;
            scanData += 8;
        } else { // format == DWARF_32_BITS
            unitLength = *(u32 *)scanData;
            scanData += 4;
        }

        if (unitLength == 0) { return CUCount;}

        scanData += unitLength;
        ++CUCount;
    }
}

Inter void InitializeCompileUnits(Memory_Arena *dwarfArena, Memory_Arena *stringArena, Array_Dwarf_Compile_Unit *CUs,
                                  Array_File_Identifier *projectFilesIds, Dwarf_Section *info) {
    byte *scanData = info->ptr;

    ArrayAlloc(*CUs, GetCompileUnitCount(info), dwarfArena);

    foreach (Dwarf_Compile_Unit, compileUnit, *CUs) {
        ZeroType(compileUnit);
        compileUnit->baseAddr = scanData;

        if (((u64)*scanData & 0xffffffff) == 0xffffffff) { // format == DWARF_64_BITS
            compileUnit->header.format = DWARF_64_BITS;
            compileUnit->header.unitLength = (*(u64 *)scanData) >> 16;
            scanData += 8;
        } else { // format == DWARF_32_BITS
            compileUnit->header.format = DWARF_32_BITS;
            compileUnit->header.unitLength = *(u32 *)scanData;
            scanData += 4;
        }

        compileUnit->header.version = (u32)*(u16 *)scanData;
        scanData += 2;

        compileUnit->header.unitType = (u32)*(u8 *)scanData;
        scanData += 1;

        compileUnit->header.addressSize = (u32)*(u8 *)scanData;
        scanData += 1;

        u64 abbrevOffset = GetValueArchFormatAndAdvance(&scanData, compileUnit->header.format);

        ParseDwarfAbbrev(&compileUnit->header.abbrev, dwarfArena, dwarfSections.abbrev.ptr + abbrevOffset);
        //PrintAbbrev(&compileUnit->header.abbrev);

        // DIE
        InitializeDIE(&compileUnit->DIE, scanData, compileUnit);

        //PrintTag(abbrevEntry->tag, "(%s)\n");
        Assert(compileUnit->DIE.abbrev->tag == DW_TAG_compile_unit);

#if 1
        for (; compileUnit->DIE.cursorAbbrev < compileUnit->DIE.abbrev->def.size; ++compileUnit->DIE.cursorAbbrev) {
            Abbrev_Def_Entry *defEntry = &compileUnit->DIE.abbrev->def.A[compileUnit->DIE.cursorAbbrev];
            //PrintAttrib(defEntry->name, "%s\n");

            switch (defEntry->name) {
                case DW_AT_stmt_list: {
                    u64 lineOffset = GetValueArchFormatAndAdvance(&compileUnit->DIE.cursorDataRaw, compileUnit->header.format);

                    compileUnit->line = ArenaPushType(dwarfArena, Dwarf_Line);
                    compileUnit->line->lineArena = dwarfArena;
                    ParseLineHeader(&compileUnit->line->header, projectFilesIds, stringArena, dwarfArena,
                                    dwarfSections.line.ptr + lineOffset);
                    //PrintLineHeader(&compileUnit->line->header);

                    compileUnit->line->lineTableCreated = false;
                    ZeroType(&compileUnit->line->lineTable);
                } break;

                case DW_AT_macro_info: {
                    Assert(false && "need to advance");
                    //DIEValueEntry->addr = FormToAddrPtr(defEntry->form, &compileUnit->DIE.cursorDataRaw,
                    //                                    DIE->CU->header.format);
                } break;

                case DW_AT_str_offsets_base: {
                    Assert(false && "need to advance");
                } break;

                case DW_AT_addr_base: {
                    Assert(false && "need to advance");
                } break;

                case DW_AT_rnglists_base: {
                    Assert(false && "need to advance");
                } break;

                case DW_AT_macros: {
                    Assert(false && "need to advance");
                } break;

                case DW_AT_loclists_base: {
                    Assert(false && "need to advance");
                } break;

                default: {
                    NextDIEValue(&compileUnit->DIE);
                } break;
            }
        }

        compileUnit->DIEChilds = compileUnit->DIE.cursorDataRaw;
#endif

        if (compileUnit->header.format == DWARF_32_BITS) {
            scanData = compileUnit->baseAddr + compileUnit->header.unitLength + 4;
        } else { // format == DWARF_64_BITS
            scanData = compileUnit->baseAddr + compileUnit->header.unitLength + 8;
        }
    }
}

Inter void PrintDIE(Dwarf_DIE *DIE) {
    printf("<%2u><0x%08llx>: Abbrev number %u ", DIE->treeDepth, (u64)(DIE->dataRaw - DIE->CU->baseAddr), DIE->code);
    PrintTag(DIE->abbrev->tag, "(%s)\n");

    for (u32 dataIndex = 0; dataIndex < DIE->abbrev->def.size; ++dataIndex) {
        printf("    ");
        PrintAttrib(DIE->abbrev->def.A[dataIndex].name, "%-18s : ");

        PrintDIEValue(&DIE->data[dataIndex]);
        printf("\n");
    }
}

#if 1
Inter void PrintInfo(Array_Dwarf_Compile_Unit *CUs) {
    foreach (Dwarf_Compile_Unit, CU, *CUs) {
        Dwarf_DIE DIE;
        for (InitializeDIE(&DIE, CU->DIEChilds, CU); DIE.treeDepth != 0; NextDIE(&DIE)) {
            ParseDIE(&DIE);
            PrintDIE(&DIE);
        }

        printf("\n\n");
    }
#if 0
    for (u32 code = ULEB128AndAdvance(&scanData); treeDepth != 0;
         code = NextDIE(&scanData, code, &CU->header.abbrev.table.A[code].def, CU)) {
        printf("<%2u><0x%08llx>: Abbrev number %u ", treeDepth, (u64)(scanData - base), code);

        if (code == 0) {
            printf("\n");
            --treeDepth;
            continue;
        }

        Dwarf_Abbrev_Entry *abbrevEntry = &CU->header.abbrev.table.A[code];

        if (abbrevEntry->hasChildren) { ++treeDepth; }

        PrintTag(abbrevEntry->tag, "(%s)");
        printf("\n");

        byte *scan = scanData;
        for (AbbrevDefEntry *defEntry = abbrevEntry->def.A;
             defEntry->name; ++defEntry) {
            printf("    ");
            PrintAttrib(defEntry->name, "%-18s : ");

            PrintDieValue(&scan, code, &abbrevEntry->def, defEntry->form, defEntry->name, CU);
            printf("\n");
        }
    }
#endif
}
#endif

Inter u32 GetDwarfSectionsSize() {
    u32 totalSize = 0;

    for (Dwarf_Section *dwarfSection = dwarfSections.A; dwarfSection->name.str; ++dwarfSection) {
        totalSize += dwarfSection->size;
    }

    return totalSize;
}

Inter b32 DwarfSectionsToFile(char *fileName) {
    b32 ret = true;

    Memory_Arena *tempArena = AllocTempArena(GetDwarfSectionsSize() + KiB(16));

    u32 sizeInHexDumpLine = 16;

    for (Dwarf_Section *dwarfSection = dwarfSections.A; dwarfSection->name.str; ++dwarfSection) {
    void *memBlock;

        if (dwarfSection->ptr) {
            memBlock = ArenaPushMem(tempArena, ((dwarfSection->name.length / sizeInHexDumpLine) + 2) * sizeInHexDumpLine);
            CopyCStrForward((char *)memBlock, dwarfSection->name.str, dwarfSection->name.length);

            memBlock = ArenaPushMem(tempArena, ((dwarfSection->size / sizeInHexDumpLine) + 2) * sizeInHexDumpLine);
            CopyCStrForward((char *)memBlock, (char *)dwarfSection->ptr, dwarfSection->size);
        }
    }

    if (!WriteNewFile(fileName, tempArena->base, tempArena->used)) {
        ret = false;
    }

    FreeTempArena(tempArena);
    return ret;
}

#ifdef WIN32 // PE
Inter void DwarfSectionsFromPE(PE_Image *PE) {
    InitializeDwarfSections();

    for (u32 sectionIndex = 0; sectionIndex < PE->sectionHeaders.size; ++sectionIndex) {
        Section_Header *sectionHeader = &PE->sectionHeaders.A[sectionIndex];

        for (Dwarf_Section *dwarfSection = dwarfSections.A; dwarfSection->name.str; ++dwarfSection) {
            if (CompCStr(GetSectionName(sectionHeader, PE->strTable), dwarfSection->name.str) == 0) {
                // NOTE(JENH): In memory.
                //dwarfSection->ptr  = (byte *)PE->baseAddrImage + sectionHeader->VirtualAddress;
                //dwarfSection->size = sectionHeader->Misc.VirtualSize;

                dwarfSection->ptr  = (byte *)PE->baseAddrImage + sectionHeader->PointerToRawData;
                dwarfSection->size = sectionHeader->SizeOfRawData;
            }
        }
    }
}
#else // ELF
Inter void DwarfSectionsFromELF(ELF_Image *ELFImage) {
    InitializeDwarfSections();

    if (ELFImage->format == ELF_BITS_32) {
        ELF_Header32 *ELFHeader = ELFImage->ELFHeader32;
        Section_Header32 *sectionHeader = ELFImage->sectionHeaders32;

        for (u32 sectionIndex = 0; sectionIndex < ELFHeader->e_shnum; ++sectionIndex, ++sectionHeader) {
            CString sectionName = (CString)((byte *)ELFImage->sectionStrTbl + sectionHeader->sh_name);

	        for (Dwarf_Section *dwarfSection = dwarfSections.array; dwarfSection->name.str; ++dwarfSection) {
		        if (CompCStr(sectionName, dwarfSection->name.str) == 0) {
                    dwarfSection->ptr  = (byte *)ELFImage->ELFHeader32 + sectionHeader->sh_offset;
                    dwarfSection->size = sectionHeader->sh_size;
		        }
	        }
        }
    } else { // ELFImage->format == ELF_BITS_64
        ELF_Header64 *ELFHeader = ELFImage->ELFHeader64;
        Section_Header64 *sectionHeader = ELFImage->sectionHeaders64;

        for (u32 sectionIndex = 0; sectionIndex < ELFHeader->e_shnum; ++sectionIndex, ++sectionHeader) {
            CString sectionName = (CString)((byte *)ELFImage->sectionStrTbl + sectionHeader->sh_name);

	        for (Dwarf_Section *dwarfSection = dwarfSections.array; dwarfSection->name.str; ++dwarfSection) {
		        if (CompCStr(sectionName, dwarfSection->name.str) == 0) {
                    dwarfSection->ptr  = (byte *)ELFImage->ELFHeader64 + sectionHeader->sh_offset;
                    dwarfSection->size = sectionHeader->sh_size;
		        }
	        }
        }
    }
}
#endif // WIN32

#endif //DSV_DWARF_H
