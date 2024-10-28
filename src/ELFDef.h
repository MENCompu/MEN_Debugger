#ifndef DSV_ELFDEF_H
#define DSV_ELFDEF_H

#define ELF_BITS_32 1
#define ELF_BITS_64 2

typedef struct {
    byte magic[4];
    byte format;
    byte endianness;
    byte version;
    byte OSABI;
    byte ABIVersion;
    byte Paddingp[7];
} ELF_Identifier;

typedef struct {
    ELF_Identifier e_ident;
    u16 e_type;
    u16 e_machine;
    u32 e_version;
    u32 e_entry;
    u32 e_phoff;
    u32 e_shoff;
    u32 e_flags;
    u16 e_ehsize;
    u16 e_phentsize;
    u16 e_phnum;
    u16 e_shentsize;
    u16 e_shnum;
    u16 e_shstrndx;
} ELF_Header32;

typedef struct {
    ELF_Identifier e_ident;
    u16 e_type;
    u16 e_machine;
    u32 e_version;
    u64 e_entry;
    u64 e_phoff;
    u64 e_shoff;
    u32 e_flags;
    u16 e_ehsize;
    u16 e_phentsize;
    u16 e_phnum;
    u16 e_shentsize;
    u16 e_shnum;
    u16 e_shstrndx;
} ELF_Header64;

typedef struct {
    u32 p_type;
    u32 p_offset;
    u32 p_vaddr;
    u32 p_paddr;
    u32 p_filesz;
    u32 p_memsz;
    u32 p_flags;
    u32 p_align;
} Program_Header32;

typedef struct {
    u32 p_type;
    u32 p_flags;
    u64 p_offset;
    u64 p_vaddr;
    u64 p_paddr;
    u64 p_filesz;
    u64 p_memsz;
    u64 p_align;
} Program_Header64;

typedef struct {
    u32 sh_name;
    u32 sh_type;
    u32 sh_flags;
    u32 sh_addr;
    u32 sh_offset;
    u32 sh_size;
    u32 sh_link;
    u32 sh_info;
    u32 sh_addralign;
    u32 sh_entsize;
} Section_Header32;

typedef struct {
    u32 sh_name;
    u32 sh_type;
    u64 sh_flags;
    u64 sh_addr;
    u64 sh_offset;
    u64 sh_size;
    u32 sh_link;
    u32 sh_info;
    u64 sh_addralign;
    u64 sh_entsize;
} Section_Header64;

typedef struct {
    u32 format;
    u32 ELFBaseAddress;
    union{
        struct {
            ELF_Header32     *ELFHeader32;
            Program_Header32 *programHeaders32;
            Section_Header32 *sectionHeaders32;
        };
        struct {
            ELF_Header64     *ELFHeader64;
            Program_Header64 *programHeaders64;
            Section_Header64 *sectionHeaders64;
        };
    };

    char *sectionStrTbl;
} ELF_Image;

#endif //DSV_ELFDEF_H
