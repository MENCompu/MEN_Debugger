#ifndef DSV_REGISTERS_H
#define DSV_REGISTERS_H

#define LIST_REGISTERS_METADATA                                                      \
    X(FieldOffset(Registers, rax)    , sizeof(u64)  , Reg_rax     , 0  , "rax"     ) \
    X(FieldOffset(Registers, rdx)    , sizeof(u64)  , Reg_rdx     , 1  , "rdx"     ) \
    X(FieldOffset(Registers, rcx)    , sizeof(u64)  , Reg_rcx     , 2  , "rcx"     ) \
    X(FieldOffset(Registers, rbx)    , sizeof(u64)  , Reg_rbx     , 3  , "rbx"     ) \
    X(FieldOffset(Registers, rsi)    , sizeof(u64)  , Reg_rsi     , 4  , "rsi"     ) \
    X(FieldOffset(Registers, rdi)    , sizeof(u64)  , Reg_rdi     , 5  , "rdi"     ) \
    X(FieldOffset(Registers, rbp)    , sizeof(u64)  , Reg_rbp     , 6  , "rbp"     ) \
    X(FieldOffset(Registers, rsp)    , sizeof(u64)  , Reg_rsp     , 7  , "rsp"     ) \
    X(FieldOffset(Registers, r8)     , sizeof(u64)  , Reg_r8      , 8  , "r8"      ) \
    X(FieldOffset(Registers, r9)     , sizeof(u64)  , Reg_r9      , 9  , "r9"      ) \
    X(FieldOffset(Registers, r10)    , sizeof(u64)  , Reg_r10     , 10 , "r10"     ) \
    X(FieldOffset(Registers, r11)    , sizeof(u64)  , Reg_r11     , 11 , "r11"     ) \
    X(FieldOffset(Registers, r12)    , sizeof(u64)  , Reg_r12     , 12 , "r12"     ) \
    X(FieldOffset(Registers, r13)    , sizeof(u64)  , Reg_r13     , 13 , "r13"     ) \
    X(FieldOffset(Registers, r14)    , sizeof(u64)  , Reg_r14     , 14 , "r14"     ) \
    X(FieldOffset(Registers, r15)    , sizeof(u64)  , Reg_r15     , 15 , "r15"     ) \
    X(FieldOffset(Registers, rip)    , sizeof(u64)  , Reg_rip     , 16 , "rip"     ) \
    X(FieldOffset(Registers, xmm0)   , sizeof(u128) , Reg_xmm0    , 17 , "xmm0"    ) \
    X(FieldOffset(Registers, xmm1)   , sizeof(u128) , Reg_xmm1    , 18 , "xmm1"    ) \
    X(FieldOffset(Registers, xmm2)   , sizeof(u128) , Reg_xmm2    , 19 , "xmm2"    ) \
    X(FieldOffset(Registers, xmm3)   , sizeof(u128) , Reg_xmm3    , 20 , "xmm3"    ) \
    X(FieldOffset(Registers, xmm4)   , sizeof(u128) , Reg_xmm4    , 21 , "xmm4"    ) \
    X(FieldOffset(Registers, xmm5)   , sizeof(u128) , Reg_xmm5    , 22 , "xmm5"    ) \
    X(FieldOffset(Registers, xmm6)   , sizeof(u128) , Reg_xmm6    , 23 , "xmm6"    ) \
    X(FieldOffset(Registers, xmm7)   , sizeof(u128) , Reg_xmm7    , 24 , "xmm7"    ) \
    X(FieldOffset(Registers, xmm8)   , sizeof(u128) , Reg_xmm8    , 25 , "xmm8"    ) \
    X(FieldOffset(Registers, xmm9)   , sizeof(u128) , Reg_xmm9    , 26 , "xmm9"    ) \
    X(FieldOffset(Registers, xmm10)  , sizeof(u128) , Reg_xmm10   , 27 , "xmm10"   ) \
    X(FieldOffset(Registers, xmm11)  , sizeof(u128) , Reg_xmm11   , 28 , "xmm11"   ) \
    X(FieldOffset(Registers, xmm12)  , sizeof(u128) , Reg_xmm12   , 29 , "xmm12"   ) \
    X(FieldOffset(Registers, xmm13)  , sizeof(u128) , Reg_xmm13   , 30 , "xmm13"   ) \
    X(FieldOffset(Registers, xmm14)  , sizeof(u128) , Reg_xmm14   , 31 , "xmm14"   ) \
    X(FieldOffset(Registers, xmm15)  , sizeof(u128) , Reg_xmm15   , 32 , "xmm15"   ) \
    X(FieldOffset(Registers, st0)    , sizeof(u64)  , Reg_st0     , 33 , "st0"     ) \
    X(FieldOffset(Registers, st1)    , sizeof(u64)  , Reg_st1     , 34 , "st1"     ) \
    X(FieldOffset(Registers, st2)    , sizeof(u64)  , Reg_st2     , 35 , "st2"     ) \
    X(FieldOffset(Registers, st3)    , sizeof(u64)  , Reg_st3     , 36 , "st3"     ) \
    X(FieldOffset(Registers, st4)    , sizeof(u64)  , Reg_st4     , 37 , "st4"     ) \
    X(FieldOffset(Registers, st5)    , sizeof(u64)  , Reg_st5     , 38 , "st5"     ) \
    X(FieldOffset(Registers, st6)    , sizeof(u64)  , Reg_st6     , 39 , "st6"     ) \
    X(FieldOffset(Registers, st7)    , sizeof(u64)  , Reg_st7     , 40 , "st7"     ) \
    X(FieldOffset(Registers, mm0)    , sizeof(u64)  , Reg_mm0     , 41 , "mm0"     ) \
    X(FieldOffset(Registers, mm1)    , sizeof(u64)  , Reg_mm1     , 42 , "mm1"     ) \
    X(FieldOffset(Registers, mm2)    , sizeof(u64)  , Reg_mm2     , 43 , "mm2"     ) \
    X(FieldOffset(Registers, mm3)    , sizeof(u64)  , Reg_mm3     , 44 , "mm3"     ) \
    X(FieldOffset(Registers, mm4)    , sizeof(u64)  , Reg_mm4     , 45 , "mm4"     ) \
    X(FieldOffset(Registers, mm5)    , sizeof(u64)  , Reg_mm5     , 46 , "mm5"     ) \
    X(FieldOffset(Registers, mm6)    , sizeof(u64)  , Reg_mm6     , 47 , "mm6"     ) \
    X(FieldOffset(Registers, mm7)    , sizeof(u64)  , Reg_mm7     , 48 , "mm7"     ) \
    X(FieldOffset(Registers, rFLAGS) , sizeof(u64)  , Reg_rFLAGS  , 49 , "rFLAGS"  ) \
    X(FieldOffset(Registers, es)     , sizeof(u16)  , Reg_es      , 50 , "es"      ) \
    X(FieldOffset(Registers, cs)     , sizeof(u16)  , Reg_cs      , 51 , "cs"      ) \
    X(FieldOffset(Registers, ss)     , sizeof(u16)  , Reg_ss      , 52 , "ss"      ) \
    X(FieldOffset(Registers, ds)     , sizeof(u16)  , Reg_ds      , 53 , "ds"      ) \
    X(FieldOffset(Registers, fs)     , sizeof(u16)  , Reg_fs      , 54 , "fs"      ) \
    X(FieldOffset(Registers, gs)     , sizeof(u16)  , Reg_gs      , 55 , "gs"      ) \
    X(MAX_u16                        , 0            , Reg_unused1 , 56 , ""        ) \
    X(MAX_u16                        , 0            , Reg_unused2 , 57 , ""        ) \
    X(FieldOffset(Registers, fsBase) , sizeof(u64)  , Reg_fs_base , 58 , "fs.base" ) \
    X(FieldOffset(Registers, gsBase) , sizeof(u64)  , Reg_gs_base , 59 , "gs.base" ) \
    X(MAX_u16                        , 0            , Reg_unused3 , 60 , ""        ) \
    X(MAX_u16                        , 0            , Reg_unused4 , 61 , ""        ) \
    X(FieldOffset(Registers, tr)     , sizeof(u16)  , Reg_tr      , 62 , "tr"      ) \
    X(FieldOffset(Registers, ldtr)   , sizeof(u32)  , Reg_ldtr    , 63 , "tr"      ) \
    X(FieldOffset(Registers, mxcsr)  , sizeof(u32)  , Reg_mxcsr   , 64 , "sr"      ) \
    X(FieldOffset(Registers, fcw)    , sizeof(u16)  , Reg_fcw     , 65 , "cw"      ) \
    X(FieldOffset(Registers, fsw)    , sizeof(u16)  , Reg_fsw     , 66 , "sw"      ) \

typedef enum {
    #define X(offset, size, enumName, enumVal, nameStr) enumName = enumVal,
    LIST_REGISTERS_METADATA
    #undef X
    Reg_count
} Register_Enum;

typedef struct {
    u64  rax;
    u64  rdx;
    u64  rcx;
    u64  rbx;
    u64  rsi;
    u64  rdi;
    u64  rbp;
    u64  rsp;
    u64  r8;
    u64  r9;
    u64  r10;
    u64  r11;
    u64  r12;
    u64  r13;
    u64  r14;
    u64  r15;
    u64  rip;
    u128 xmm0;
    u128 xmm1;
    u128 xmm2;
    u128 xmm3;
    u128 xmm4;
    u128 xmm5;
    u128 xmm6;
    u128 xmm7;
    u128 xmm8;
    u128 xmm9;
    u128 xmm10;
    u128 xmm11;
    u128 xmm12;
    u128 xmm13;
    u128 xmm14;
    u128 xmm15;
    u64  st0;
    u64  st1;
    u64  st2;
    u64  st3;
    u64  st4;
    u64  st5;
    u64  st6;
    u64  st7;
    u64  mm0;
    u64  mm1;
    u64  mm2;
    u64  mm3;
    u64  mm4;
    u64  mm5;
    u64  mm6;
    u64  mm7;
    u64  rFLAGS;
    u16  es;
    u16  cs;
    u16  ss;
    u16  ds;
    u16  fs;
    u16  gs;
    u64  fsBase;
    u64  gsBase;
    u16  tr;
    u32  ldtr;
    u32  mxcsr;
    u16  fcw;
    u16  fsw;
} Registers;

typedef struct {
    struct {
        u8  size;
        u16 offset;
    } structData[Reg_count];
    String names[Reg_count];
} Registers_Metadata;

GlobalVariable Registers_Metadata regsMetadata = {0};

Inter void InitializeRegisterMetadata() {
    #define X(offsetField, sizeField, enumName, enumVal, nameStr) \
        regsMetadata.structData[enumVal].size = sizeField;         \
        regsMetadata.structData[enumVal].offset = offsetField;     \
        regsMetadata.names[enumVal].str = nameStr;                 \
        regsMetadata.names[enumVal].length = sizeof(nameStr);
    LIST_REGISTERS_METADATA
    #undef X
}

#define INVALID_REG_NAME 0xFFFFFFFF

u64 GetLoadedRegister(Registers *regs, Register_Enum reg) {
    switch (reg) {
        case Reg_rax: { return regs->rax; } break;
        case Reg_rdx: { return regs->rdx; } break;
        case Reg_rcx: { return regs->rcx; } break;
        case Reg_rbx: { return regs->rbx; } break;
        case Reg_rsi: { return regs->rsi; } break;
        case Reg_rdi: { return regs->rdi; } break;
        case Reg_rbp: { return regs->rbp; } break;
        case Reg_rsp: { return regs->rsp; } break;
        case Reg_r8:  { return regs->r8;  } break;
        case Reg_r9:  { return regs->r9;  } break;
        case Reg_r10: { return regs->r10; } break;
        case Reg_r11: { return regs->r11; } break;
        case Reg_r12: { return regs->r12; } break;
        case Reg_r13: { return regs->r13; } break;
        case Reg_r14: { return regs->r14; } break;
        case Reg_r15: { return regs->r15; } break;
        case Reg_rip: { return regs->rip; } break;
#if 0
        case Reg_xmm0: { return regs->xmm0; } break;
        case Reg_xmm1: { return regs->xmm1; } break;
        case Reg_xmm2: { return regs->xmm2; } break;
        case Reg_xmm3: { return regs->xmm3; } break;
        case Reg_xmm4: { return regs->xmm4; } break;
        case Reg_xmm5: { return regs->xmm5; } break;
        case Reg_xmm6: { return regs->xmm6; } break;
        case Reg_xmm7: { return regs->xmm7; } break;
        case Reg_xmm8: { return regs->xmm8; } break;
        case Reg_xmm9: { return regs->xmm9; } break;
        case Reg_xmm10: { return regs->xmm10; } break;
        case Reg_xmm11: { return regs->xmm11; } break;
        case Reg_xmm12: { return regs->xmm12; } break;
        case Reg_xmm13: { return regs->xmm13; } break;
        case Reg_xmm14: { return regs->xmm14; } break;
        case Reg_xmm15: { return regs->xmm15; } break;
        case Reg_st0: { return regs->st0; } break;
        case Reg_st1: { return regs->st1; } break;
        case Reg_st2: { return regs->st2; } break;
        case Reg_st3: { return regs->st3; } break;
        case Reg_st4: { return regs->st4; } break;
        case Reg_st5: { return regs->st5; } break;
        case Reg_st6: { return regs->st6; } break;
        case Reg_st7: { return regs->st7; } break;
        case Reg_mm0: { return regs->mm0; } break;
        case Reg_mm1: { return regs->mm1; } break;
        case Reg_mm2: { return regs->mm2; } break;
        case Reg_mm3: { return regs->mm3; } break;
        case Reg_mm4: { return regs->mm4; } break;
        case Reg_mm5: { return regs->mm5; } break;
        case Reg_mm6: { return regs->mm6; } break;
        case Reg_mm7: { return regs->mm7; } break;
#endif
        case Reg_rFLAGS: { return regs->rFLAGS; } break;
        case Reg_es: { return regs->es; } break;
        case Reg_cs: { return regs->cs; } break;
        case Reg_ss: { return regs->ss; } break;
        case Reg_ds: { return regs->ds; } break;
        case Reg_fs: { return regs->fs; } break;
        case Reg_gs: { return regs->gs; } break;
#if 0
        case Reg_fs_base: { return regs->fsBase; } break;
        case Reg_gs_base: { return regs->gsBase; } break;
        case Reg_tr: { return regs->tr; } break;
        case Reg_ldtr: { return regs->ldtr; } break;
        case Reg_mxcsr: { return regs->mxcsr; } break;
        case Reg_fcw: { return regs->fcw; } break;
        case Reg_fsw: { return regs->fsw; } break;
#endif
        NO_DEFAULT
    }

    return 0;
}

void SetLoadedRegister(Registers *regs, Register_Enum reg, u64 value) {
    if (regsMetadata.structData[reg].offset != MAX_u16) {
        *(u64 *)((byte *)regs + regsMetadata.structData[reg].offset) = value;
    }
}

#define PrintlnLoadedRegister(regs, reg) PrintfLoadedRegister(regs, reg, "%s: %llx\n")
void PrintfLoadedRegister(Registers *regs, Register_Enum reg, CString format) {
    printf("offset: %u\n", regsMetadata.structData[reg].offset);
    if (regsMetadata.structData[reg].offset != MAX_u16) {
        printf(format, regsMetadata.names[reg].str, *(u64 *)((byte *)regs + regsMetadata.structData[reg].offset));
    }
}

#endif //DSV_REGISTERS_H
