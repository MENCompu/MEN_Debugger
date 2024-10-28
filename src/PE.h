#ifndef PEP_PE_H
#define PEP_PE_H

#include "PEDef.h"
#include "DataTypes.h"

Inter b32 ParsePEImage(PE_Image *PE, byte *binFileRaw) {
    PE->baseAddrImage = binFileRaw;

    PE->DOSHeader = (DOS_Header *)PE->baseAddrImage;
    PE->NTHeaders = (NT_Headers *)(PE->baseAddrImage + PE->DOSHeader->e_lfanew);

         if (PE->NTHeaders->OptionalHeaderMagic == PE_WIN_32 ) { PE->format = PE_32;  }
    else if (PE->NTHeaders->OptionalHeaderMagic == PE_WIN_64 ) { PE->format = PE_64;  }
    else if (PE->NTHeaders->OptionalHeaderMagic == PE_WIN_ROM) { PE->format = PE_ROM; }
    else {
        ERR("Optional Header magic is an invalid value.\n");
        ERR("The bin file may be corrupted.\n");
        return false;
    }

    PE->sectionHeaders.size = PE->NTHeaders->FileHeader.NumberOfSections;
    PE->sectionHeaders.A    = (Section_Header *)(&PE->NTHeaders->firstByteOptionalHeader +
                                                 PE->NTHeaders->FileHeader.SizeOfOptionalHeader);

    u32 symbolTableOffset = (u32)PE->NTHeaders->FileHeader.PointerToSymbolTable;
    byte *symbolTable = PE->baseAddrImage + symbolTableOffset;

    u32 numberOfSymbols = (u32)PE->NTHeaders->FileHeader.NumberOfSymbols;
    PE->strTable = (char *)(symbolTable + (numberOfSymbols * SYMBOL_SIZE));

    return true;
}

Inter CString GetSectionName(Section_Header *sectionHeader, char *strTable) {
    char *scan = sectionHeader->Name;

    if (scan[0] == '/') {
        String offsetStr = { .str = ++scan, .size = 7 };
        offsetStr.size = Min(FindCharForwardX(offsetStr, '\0'), offsetStr.size);
        u32 offset = (u32)StrToS32(offsetStr);
        CString name = strTable + offset;
        return name;
    }

    return sectionHeader->Name;
}

Inter void PrintPEDOSHeader(DOS_Header *DOSHeader) {
    //DOS HEADER
    printf("__DOS HEADER__\n");
    printf("    e_magic    = %04x\n", DOSHeader->e_magic);
    printf("    e_cblp     = %04x\n", DOSHeader->e_cblp);
    printf("    e_cp       = %04x\n", DOSHeader->e_cp);
    printf("    e_crlc     = %04x\n", DOSHeader->e_crlc);
    printf("    e_cparhdr  = %04x\n", DOSHeader->e_cparhdr);
    printf("    e_minalloc = %04x\n", DOSHeader->e_minalloc);
    printf("    e_maxalloc = %04x\n", DOSHeader->e_maxalloc);
    printf("    e_ss       = %04x\n", DOSHeader->e_ss);
    printf("    e_sp       = %04x\n", DOSHeader->e_sp);
    printf("    e_csum     = %04x\n", DOSHeader->e_csum);
    printf("    e_ip       = %04x\n", DOSHeader->e_ip);
    printf("    e_cs       = %04x\n", DOSHeader->e_cs);
    printf("    e_lfarlc   = %04x\n", DOSHeader->e_lfarlc);
    printf("    e_ovno     = %04x\n", DOSHeader->e_ovno);
    //printf("%04x\n",PE->DOSHeader->e_res);
    printf("    e_oemid    = %04x\n", DOSHeader->e_oemid);
    printf("    e_oeminfo  = %04x\n", DOSHeader->e_oeminfo);
    //printf("%04x\n",PE->DOSHeader->e_res2);
    printf("    e_lfanew   = %08x\n\n", (u32)DOSHeader->e_lfanew);
}

Inter void PrintPEFileHeader(File_Header *fileHeader) {
    //printf("    Signature            = %08x\n",   (u32)PE->NTHeaders->Signature);
    printf("__FILE HEADER__\n");
    printf("    Machine              = %04x\n", (u16)fileHeader->Machine);
    printf("    NumberOfSections     = %04x\n", (u16)fileHeader->NumberOfSections);
    printf("    TimeDateStamp        = %08x\n", (u32)fileHeader->TimeDateStamp);
    printf("    PointerToSymbolTable = %08x\n", (u32)fileHeader->PointerToSymbolTable);
    printf("    NumberOfSymbols      = %08x\n", (u32)fileHeader->NumberOfSymbols);
    printf("    SizeOfOptionalHeader = %04x\n", (u16)fileHeader->SizeOfOptionalHeader);
    printf("    Characteristics      = %04x\n", (u16)fileHeader->Characteristics);
    printf("\n");
}

Inter void PrintEPOptionalHeader(void *optionalHeader, u32 format) {
    // TODO(JENH): Do this for 64 format as well.
    Optional_Header32 *optionalHeader32 = (Optional_Header32 *)optionalHeader;

    printf("__OPTIONAL HEADER__\n");
    printf("    Magic                       = %04x\n", optionalHeader32->Magic);
    printf("    MajorLinkerVersion          = %02x\n", optionalHeader32->MajorLinkerVersion);
    printf("    MinorLinkerVersion          = %02x\n", optionalHeader32->MinorLinkerVersion);
    printf("    SizeOfCode                  = %08x\n", optionalHeader32->SizeOfCode);
    printf("    SizeOfInitializedData       = %08x\n", optionalHeader32->SizeOfInitializedData);
    printf("    SizeOfUninitializedData     = %08x\n", optionalHeader32->SizeOfUninitializedData);
    printf("    AddressOfEntryPoint         = %08x\n", optionalHeader32->AddressOfEntryPoint);
    printf("    BaseOfCode                  = %08x\n", optionalHeader32->BaseOfCode);

    if (format == PE_32) {
        printf("    BaseOfData                  = %08x\n", optionalHeader32->BaseOfData);
        printf("    ImageBase                   = %08x\n", optionalHeader32->ImageBase);
        printf("    SectionAlignment            = %08x\n", optionalHeader32->SectionAlignment);
        printf("    FileAlignment               = %08x\n", optionalHeader32->FileAlignment);
        printf("    MajorOperatingSystemVersion = %04x\n", optionalHeader32->MajorOperatingSystemVersion);
        printf("    MinorOperatingSystemVersion = %04x\n", optionalHeader32->MinorOperatingSystemVersion);
        printf("    MajorImageVersion           = %04x\n", optionalHeader32->MajorImageVersion);
        printf("    MinorImageVersion           = %04x\n", optionalHeader32->MinorImageVersion);
        printf("    MajorSubsystemVersion       = %04x\n", optionalHeader32->MajorSubsystemVersion);
        printf("    MinorSubsystemVersion       = %04x\n", optionalHeader32->MinorSubsystemVersion);
        printf("    Win32VersionValue           = %08x\n", optionalHeader32->Win32VersionValue);
        printf("    SizeOfImage                 = %08x\n", optionalHeader32->SizeOfImage);
        printf("    SizeOfHeaders               = %08x\n", optionalHeader32->SizeOfHeaders);
        printf("    CheckSum                    = %08x\n", optionalHeader32->CheckSum);
        printf("    Subsystem                   = %04x\n", optionalHeader32->Subsystem);
        printf("    DllCharacteristics          = %04x\n", optionalHeader32->DllCharacteristics);
        printf("    SizeOfStackReserve          = %08x\n", optionalHeader32->SizeOfStackReserve);
        printf("    SizeOfStackCommit           = %08x\n", optionalHeader32->SizeOfStackCommit);
        printf("    SizeOfHeapReserve           = %08x\n", optionalHeader32->SizeOfHeapReserve);
        printf("    SizeOfHeapCommit            = %08x\n", optionalHeader32->SizeOfHeapCommit);
        printf("    LoaderFlags                 = %08x\n", optionalHeader32->LoaderFlags);
        printf("    NumberOfRvaAndSizes         = %08x\n", optionalHeader32->NumberOfRvaAndSizes);
    } else { // format == PE_64
        Optional_Header64 *optionalHeader64 = (Optional_Header64 *)optionalHeader;

        printf("    ImageBase                   = %016llx\n", optionalHeader64->ImageBase);
        printf("    SectionAlignment            = %08x\n",    optionalHeader64->SectionAlignment);
        printf("    FileAlignment               = %08x\n",    optionalHeader64->FileAlignment);
        printf("    MajorOperatingSystemVersion = %04x\n",    optionalHeader64->MajorOperatingSystemVersion);
        printf("    MinorOperatingSystemVersion = %04x\n",    optionalHeader64->MinorOperatingSystemVersion);
        printf("    MajorImageVersion           = %04x\n",    optionalHeader64->MajorImageVersion);
        printf("    MinorImageVersion           = %04x\n",    optionalHeader64->MinorImageVersion);
        printf("    MajorSubsystemVersion       = %04x\n",    optionalHeader64->MajorSubsystemVersion);
        printf("    MinorSubsystemVersion       = %04x\n",    optionalHeader64->MinorSubsystemVersion);
        printf("    Win32VersionValue           = %08x\n",    optionalHeader64->Win32VersionValue);
        printf("    SizeOfImage                 = %08x\n",    optionalHeader64->SizeOfImage);
        printf("    SizeOfHeaders               = %08x\n",    optionalHeader64->SizeOfHeaders);
        printf("    CheckSum                    = %08x\n",    optionalHeader64->CheckSum);
        printf("    Subsystem                   = %04x\n",    optionalHeader64->Subsystem);
        printf("    DllCharacteristics          = %04x\n",    optionalHeader64->DllCharacteristics);
        printf("    SizeOfStackReserve          = %016llx\n", optionalHeader64->SizeOfStackReserve);
        printf("    SizeOfStackCommit           = %016llx\n", optionalHeader64->SizeOfStackCommit);
        printf("    SizeOfHeapReserve           = %016llx\n", optionalHeader64->SizeOfHeapReserve);
        printf("    SizeOfHeapCommit            = %016llx\n", optionalHeader64->SizeOfHeapCommit);
        printf("    LoaderFlags                 = %08x\n",    optionalHeader64->LoaderFlags);
        printf("    NumberOfRvaAndSizes         = %08x\n",    optionalHeader64->NumberOfRvaAndSizes);
    }
    printf("\n");

    // DATA DIRECTORIES
    printf("__DATA_DIRECTORIES__\n");

    for (s32 index = 0; index < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; ++index) {
        Data_Directory *dataDir = &optionalHeader32->DataDirectory[index];
        printf("    %d. VirtualAddress = %08x | Size = %08x\n", index + 1, (u32)dataDir->VirtualAddress, (u32)dataDir->Size);
    }
}

Inter void PrintSections(Array_Section_Header *sectionHeaders, char *strTable) {
    // Sections
    printf("\n__SECTIONS__\n\n");

    //for (s32 index = 0; index < sectionHeaders.size; ++index) {
    foreach (Section_Header, sectionHeader, *sectionHeaders) {
        printf("Name: \"%s\"\n", GetSectionName(sectionHeader, strTable));
        printf("    VirtualSize          = %08x\n", (u32)sectionHeader->Misc.VirtualSize);
        printf("    VirtualAddress       = %08x\n", (u32)sectionHeader->VirtualAddress);
        printf("    SizeOfRawData        = %08x\n", (u32)sectionHeader->SizeOfRawData);
        printf("    PointerToRawData     = %08x\n", (u32)sectionHeader->PointerToRawData);
        printf("    PointerToRelocations = %08x\n", (u32)sectionHeader->PointerToRelocations);
        printf("    PointerToLinenumbers = %08x\n", (u32)sectionHeader->PointerToLinenumbers);
        printf("    NumberOfRelocations  = %04x\n", (u32)sectionHeader->NumberOfRelocations);
        printf("    NumberOfLinenumbers  = %04x\n", (u32)sectionHeader->NumberOfLinenumbers);
        printf("    Characteristics      = %08x\n", (u32)sectionHeader->Characteristics);
        printf("\n");
    }
    printf("\n");
}

Inter void PrintPE(PE_Image *PE) {
    PrintPEDOSHeader(PE->DOSHeader);
    PrintPEFileHeader(&PE->NTHeaders->FileHeader);
    PrintEPOptionalHeader(&PE->NTHeaders->firstByteOptionalHeader, PE->format);
    PrintSections(&PE->sectionHeaders, PE->strTable);
}

#endif //PEP_PE_H
