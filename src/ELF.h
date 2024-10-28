#ifndef DSV_ELF_H
#define DSV_ELF_H

#define SHT_STRTAB 0x03

#include "ELFDef.h"

#if 0
Inter b32 LoadELFFile(char *fileName, File *ELFFile) {
    b32 ret = 1;

    int fd = open(fileName, 0);

    if (fd == -1) {
        printERR("Couldn't open ELF file: %s\n", strerror(errno));
        return 0;
    }

    WordSize fileSize = (WordSize)lseek(fd, 0, SEEK_END);

    if (fileSize == -1) {
        printERR("Couldn't get ELF file size: %s\n", strerror(errno));
        defer(0);
    }

    void *memory = mmap(0, fileSize, PROT_READ, MAP_SHARED, fd, 0);

    if (memory == MAP_FAILED) {
        printERR("Couldn't map the ELF file to memory: %s\n", strerror(errno));
        defer(0);
    }

    ELFFile->memory = memory;
    ELFFile->size   = fileSize;

defer:
    close(fd);
    return ret;
}
#endif

Inter char *GetSectionStbTbl32(ELF_Header32 *ELFHdr) {
    Section_Header32 *firstSectionHdr = (Section_Header32 *)((byte *)ELFHdr + ELFHdr->e_shoff);
    Section_Header32 *sectionStrTbl = firstSectionHdr + ELFHdr->e_shstrndx;
    return (char *)ELFHdr + sectionStrTbl->sh_offset;
}

Inter char *GetSectionStbTbl64(ELF_Header64 *ELFHdr) {
    Section_Header64 *firstSectionHdr = (Section_Header64 *)((byte *)ELFHdr + ELFHdr->e_shoff);
    Section_Header64 *sectionStrTbl = firstSectionHdr + ELFHdr->e_shstrndx;
    return (char *)ELFHdr + sectionStrTbl->sh_offset;
}

Inter ELF_Image ParseELF(void *memoryFile, WordSize sizeFile) {
    ELF_Image ret;

    ELF_Identifier *identifier = (ELF_Identifier *)memoryFile;

    if (identifier->format == ELF_BITS_32) {
        ret.ELFHeader32      = (ELF_Header32 *)memoryFile;
        ret.programHeaders32 = (Program_Header32 *)((byte *)ret.ELFHeader32 + ret.ELFHeader32->e_phoff);
        ret.sectionHeaders32 = (Section_Header32 *)((byte *)ret.ELFHeader32 + ret.ELFHeader32->e_shoff);
        ret.sectionStrTbl    = GetSectionStbTbl32(ret.ELFHeader32);
    } else {
        ret.ELFHeader64      = (ELF_Header64 *)memoryFile;
        ret.programHeaders64 = (Program_Header64 *)((byte *)ret.ELFHeader64 + ret.ELFHeader64->e_phoff);
        ret.sectionHeaders64 = (Section_Header64 *)((byte *)ret.ELFHeader64 + ret.ELFHeader64->e_shoff);
        ret.sectionStrTbl    = GetSectionStbTbl64(ret.ELFHeader64);
    }

    return ret;
}

Inter void PrintSectionHeader32(Section_Header32 *sectionHdr, char *stringTable) {
    printf("sh_name = %s\n", stringTable + sectionHdr->sh_name);
    printf("sh_type = %d\n", sectionHdr->sh_type);
    printf("sh_flags = %d\n", sectionHdr->sh_flags);
    printf("sh_addr = %d\n", sectionHdr->sh_addr);
    printf("sh_offset = %d\n", sectionHdr->sh_offset);
    printf("sh_size = %d\n", sectionHdr->sh_size);
    printf("sh_link = %d\n", sectionHdr->sh_link);
    printf("sh_info = %d\n", sectionHdr->sh_info);
    printf("sh_addralign = %d\n", sectionHdr->sh_addralign);
    printf("sh_entsize = %d\n", sectionHdr->sh_entsize);
}

Inter void PrintSectionHeader64(Section_Header64 *sectionHdr, char *stringTable) {
    printf("sh_name = %s\n", (char *)(stringTable + sectionHdr->sh_name));
    printf("sh_type = %d\n", sectionHdr->sh_type);
    printf("sh_flags = %llu\n", sectionHdr->sh_flags);
    printf("sh_addr = %llu\n", sectionHdr->sh_addr);
    printf("sh_offset = %llu\n", sectionHdr->sh_offset);
    printf("sh_size = %llu\n", sectionHdr->sh_size);
    printf("sh_link = %d\n", sectionHdr->sh_link);
    printf("sh_info = %d\n", sectionHdr->sh_info);
    printf("sh_addralign = %llu\n", sectionHdr->sh_addralign);
    printf("sh_entsize = %llu\n", sectionHdr->sh_entsize);
}

Inter void PrintProgramHeader32(Program_Header32 *programHdr) {
    printf("p_type = %d\n", programHdr->p_type);
    printf("p_offset = %d\n", programHdr->p_offset);
    printf("p_vaddr = %d\n", programHdr->p_vaddr);
    printf("p_paddr = %d\n", programHdr->p_paddr);
    printf("p_filesz = %d\n", programHdr->p_filesz);
    printf("p_memsz = %d\n", programHdr->p_memsz);
    printf("p_flags = %d\n", programHdr->p_flags);
    printf("p_align = %d\n", programHdr->p_align);
}

Inter void PrintProgramHeader64(Program_Header64 *programHdr) {
    printf("p_type = %d\n", programHdr->p_type);
    printf("p_flags = %d\n", programHdr->p_flags);
    printf("p_offset = %llu\n", programHdr->p_offset);
    printf("p_vaddr = %llu\n", programHdr->p_vaddr);
    printf("p_paddr = %llu\n", programHdr->p_paddr);
    printf("p_filesz = %llu\n", programHdr->p_filesz);
    printf("p_memsz = %llu\n", programHdr->p_memsz);
    printf("p_align = %llu\n", programHdr->p_align);
}

Inter void PrintIdentifier(ELF_Identifier *identifier) {
    printf("magic = ");
    for (u32 i = 0; i < 4; ++i) {
        printf("%c", identifier->magic[i]);
    }
    printf("\n");

    printf("format = %d\n", identifier->format);
    printf("endianness = %d\n", identifier->endianness);
    printf("version = %d\n", identifier->version);
    printf("OSABI = %d\n", identifier->OSABI);
    printf("ABIVersion = %d\n", identifier->ABIVersion);
}

Inter void PrintELFHeader32(ELF_Header32 *elfhdr32) {
    ELF_Identifier *identifier = &elfhdr32->e_ident;
    PrintIdentifier(identifier);

    printf("e_type = %d\n", elfhdr32->e_type);
    printf("e_machine = %d\n", elfhdr32->e_machine);
    printf("e_version = %d\n", elfhdr32->e_version);
    printf("e_entry = %d\n", elfhdr32->e_entry);
    printf("e_phoff = %d\n", elfhdr32->e_phoff);
    printf("e_shoff = %d\n", elfhdr32->e_shoff);
    printf("e_flags = %d\n", elfhdr32->e_flags);
    printf("e_ehsize = %d\n", elfhdr32->e_ehsize);
    printf("e_phentsize = %d\n", elfhdr32->e_phentsize);
    printf("e_phnum = %d\n", elfhdr32->e_phnum);
    printf("e_shentsize = %d\n", elfhdr32->e_shentsize);
    printf("e_shnum = %d\n", elfhdr32->e_shnum);
    printf("e_shstrndx = %d\n", elfhdr32->e_shstrndx);
}

Inter void PrintELFHeader64(ELF_Header64 *elfhdr64) {
    ELF_Identifier *identifier = &elfhdr64->e_ident;
    PrintIdentifier(identifier);

    printf("e_type = %d\n", elfhdr64->e_type);
    printf("e_machine = %d\n", elfhdr64->e_machine);
    printf("e_version = %d\n", elfhdr64->e_version);
    printf("e_entry = %llu\n", elfhdr64->e_entry);
    printf("e_phoff = %llu\n", elfhdr64->e_phoff);
    printf("e_shoff = %llu\n", elfhdr64->e_shoff);
    printf("e_flags = %d\n", elfhdr64->e_flags);
    printf("e_ehsize = %d\n", elfhdr64->e_ehsize);
    printf("e_phentsize = %d\n", elfhdr64->e_phentsize);
    printf("e_phnum = %d\n", elfhdr64->e_phnum);
    printf("e_shentsize = %d\n", elfhdr64->e_shentsize);
    printf("e_shnum = %d\n", elfhdr64->e_shnum);
    printf("e_shstrndx = %d\n", elfhdr64->e_shstrndx);
}

Inter void PrintAllProgramHeaders(ELF_Image *ELFImage) {
    if (ELFImage->format == ELF_BITS_32) {
        ELF_Header32 *ELFHeader32 = ELFImage->ELFHeader32;
        Program_Header32 *programHdr = ELFImage->programHeaders32;
        for (u32 counter = 0; counter < ELFHeader32->e_phnum; ++programHdr, ++counter) {
            printf("\n");
            PrintProgramHeader32(programHdr);
        }
    } else {
        ELF_Header64 *ELFHeader64 = ELFImage->ELFHeader64;
        Program_Header64 *programHdr = ELFImage->programHeaders64;
        for (u64 counter = 0; counter < ELFHeader64->e_phnum; ++programHdr, ++counter) {
            printf("\n");
            PrintProgramHeader64(programHdr);
        }
    }
}

Inter void PrintAllSectionHeaders(ELF_Image *ELFImage) {
    if (ELFImage->format == ELF_BITS_32) {
        ELF_Header32 *ELFHeader32 = ELFImage->ELFHeader32;
        Section_Header32 *sectionHdr = ELFImage->sectionHeaders32;
        for (u32 counter = 0; counter < ELFHeader32->e_shnum; ++sectionHdr, ++counter) {
            printf("\n");
            PrintSectionHeader32(sectionHdr, ELFImage->sectionStrTbl);
        }
    } else {
        ELF_Header64 *ELFHeader64 = ELFImage->ELFHeader64;
        Section_Header64 *sectionHdr = ELFImage->sectionHeaders64;
        for (u64 counter = 0; counter < ELFHeader64->e_shnum; ++sectionHdr, ++counter) {
            printf("\n");
            PrintSectionHeader64(sectionHdr, ELFImage->sectionStrTbl);
        }
    }
}

#endif //DSV_ELF_H
