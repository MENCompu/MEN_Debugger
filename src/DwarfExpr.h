#ifndef DSV_DWARFEXPR_H
#define DSV_DWARFEXPR_H

// TODO(JENH): It should consider target address size.
#define targetAddress u64 *

typedef enum {
    DW_OP_addr                = 0x03,
    DW_OP_deref               = 0x06,
    DW_OP_const1u             = 0x08,
    DW_OP_const1s             = 0x09,
    DW_OP_const2u             = 0x0a,
    DW_OP_const2s             = 0x0b,
    DW_OP_const4u             = 0x0c,
    DW_OP_const4s             = 0x0d,
    DW_OP_const8u             = 0x0e,
    DW_OP_const8s             = 0x0f,
    DW_OP_constu              = 0x10,
    DW_OP_consts              = 0x11,
    DW_OP_dup                 = 0x12,
    DW_OP_drop                = 0x13,
    DW_OP_over                = 0x14,
    DW_OP_pick                = 0x15,
    DW_OP_swap                = 0x16,
    DW_OP_rot                 = 0x17,
    DW_OP_xderef              = 0x18,
    DW_OP_abs                 = 0x19,
    DW_OP_and                 = 0x1a,
    DW_OP_div                 = 0x1b,
    DW_OP_minus               = 0x1c,
    DW_OP_mod                 = 0x1d,
    DW_OP_mul                 = 0x1e,
    DW_OP_neg                 = 0x1f,
    DW_OP_not                 = 0x20,
    DW_OP_or                  = 0x21,
    DW_OP_plus                = 0x22,
    DW_OP_plus_uconst         = 0x23,
    DW_OP_shl                 = 0x24,
    DW_OP_shr                 = 0x25,
    DW_OP_shra                = 0x26,
    DW_OP_xor                 = 0x27,
    DW_OP_bra                 = 0x28,
    DW_OP_eq                  = 0x29,
    DW_OP_ge                  = 0x2a,
    DW_OP_gt                  = 0x2b,
    DW_OP_le                  = 0x2c,
    DW_OP_lt                  = 0x2d,
    DW_OP_ne                  = 0x2e,
    DW_OP_skip                = 0x2f,
    DW_OP_lit0                = 0x30,
    DW_OP_lit1                = 0x31,
    DW_OP_lit2                = 0x32,
    DW_OP_lit3                = 0x33,
    DW_OP_lit4                = 0x34,
    DW_OP_lit5                = 0x35,
    DW_OP_lit6                = 0x36,
    DW_OP_lit7                = 0x37,
    DW_OP_lit8                = 0x38,
    DW_OP_lit9                = 0x39,
    DW_OP_lit10               = 0x3a,
    DW_OP_lit11               = 0x3b,
    DW_OP_lit12               = 0x3c,
    DW_OP_lit13               = 0x3d,
    DW_OP_lit14               = 0x3e,
    DW_OP_lit15               = 0x3f,
    DW_OP_lit16               = 0x40,
    DW_OP_lit17               = 0x41,
    DW_OP_lit18               = 0x42,
    DW_OP_lit19               = 0x43,
    DW_OP_lit20               = 0x44,
    DW_OP_lit21               = 0x45,
    DW_OP_lit22               = 0x46,
    DW_OP_lit23               = 0x47,
    DW_OP_lit24               = 0x48,
    DW_OP_lit25               = 0x49,
    DW_OP_lit26               = 0x4a,
    DW_OP_lit27               = 0x4b,
    DW_OP_lit28               = 0x4c,
    DW_OP_lit29               = 0x4d,
    DW_OP_lit30               = 0x4e,
    DW_OP_lit31               = 0x4f,
    DW_OP_reg0                = 0x50,
    DW_OP_reg1                = 0x51,
    DW_OP_reg2                = 0x52,
    DW_OP_reg3                = 0x53,
    DW_OP_reg4                = 0x54,
    DW_OP_reg5                = 0x55,
    DW_OP_reg6                = 0x56,
    DW_OP_reg7                = 0x57,
    DW_OP_reg8                = 0x58,
    DW_OP_reg9                = 0x59,
    DW_OP_reg10               = 0x5a,
    DW_OP_reg11               = 0x5b,
    DW_OP_reg12               = 0x5c,
    DW_OP_reg13               = 0x5d,
    DW_OP_reg14               = 0x5e,
    DW_OP_reg15               = 0x5f,
    DW_OP_reg16               = 0x60,
    DW_OP_reg17               = 0x61,
    DW_OP_reg18               = 0x62,
    DW_OP_reg19               = 0x63,
    DW_OP_reg20               = 0x64,
    DW_OP_reg21               = 0x65,
    DW_OP_reg22               = 0x66,
    DW_OP_reg23               = 0x67,
    DW_OP_reg24               = 0x68,
    DW_OP_reg25               = 0x69,
    DW_OP_reg26               = 0x6a,
    DW_OP_reg27               = 0x6b,
    DW_OP_reg28               = 0x6c,
    DW_OP_reg29               = 0x6d,
    DW_OP_reg30               = 0x6e,
    DW_OP_reg31               = 0x6f,
    DW_OP_breg0               = 0x70,
    DW_OP_breg1               = 0x71,
    DW_OP_breg2               = 0x72,
    DW_OP_breg3               = 0x73,
    DW_OP_breg4               = 0x74,
    DW_OP_breg5               = 0x75,
    DW_OP_breg6               = 0x76,
    DW_OP_breg7               = 0x77,
    DW_OP_breg8               = 0x78,
    DW_OP_breg9               = 0x79,
    DW_OP_breg10              = 0x7a,
    DW_OP_breg11              = 0x7b,
    DW_OP_breg12              = 0x7c,
    DW_OP_breg13              = 0x7d,
    DW_OP_breg14              = 0x7e,
    DW_OP_breg15              = 0x7f,
    DW_OP_breg16              = 0x80,
    DW_OP_breg17              = 0x81,
    DW_OP_breg18              = 0x82,
    DW_OP_breg19              = 0x83,
    DW_OP_breg20              = 0x84,
    DW_OP_breg21              = 0x85,
    DW_OP_breg22              = 0x86,
    DW_OP_breg23              = 0x87,
    DW_OP_breg24              = 0x88,
    DW_OP_breg25              = 0x89,
    DW_OP_breg26              = 0x8a,
    DW_OP_breg27              = 0x8b,
    DW_OP_breg28              = 0x8c,
    DW_OP_breg29              = 0x8d,
    DW_OP_breg30              = 0x8e,
    DW_OP_breg31              = 0x8f,
    DW_OP_regx                = 0x90,
    DW_OP_fbreg               = 0x91,
    DW_OP_bregx               = 0x92,
    DW_OP_piece               = 0x93,
    DW_OP_deref_size          = 0x94,
    DW_OP_xderef_size         = 0x95,
    DW_OP_nop                 = 0x96,
    DW_OP_push_object_address = 0x97,
    DW_OP_call2               = 0x98,
    DW_OP_call4               = 0x99,
    DW_OP_call_ref            = 0x9a,
    DW_OP_form_tls_address    = 0x9b,
    DW_OP_call_frame_cfa      = 0x9c,
    DW_OP_bit_piece           = 0x9d,
    DW_OP_implicit_value      = 0x9e,
    DW_OP_stack_value         = 0x9f,
    DW_OP_implicit_pointer    = 0xa0,
    DW_OP_addrx               = 0xa1,
    DW_OP_constx              = 0xa2,
    DW_OP_entry_value         = 0xa3,
    DW_OP_const_type          = 0xa4,
    DW_OP_regval_type         = 0xa5,
    DW_OP_deref_type          = 0xa6,
    DW_OP_xderef_type         = 0xa7,
    DW_OP_convert             = 0xa8,
    DW_OP_reinterpret         = 0xa9,
    DW_OP_lo_user             = 0xe0,
    DW_OP_hi_user             = 0xff
} Expr_Opcode;

typedef enum {
    ELT_generic,
    ELT_integral,
    ELT_loc_reg,
    ELT_implicit,
    ELT_stack_value,
} Expr_Element_Type;

typedef struct {
    Expr_Element_Type type;
    union {
        u64 stackValue;
        Mem_Block implicit;
        u64 reg;
        u64 generic;
        u64 sValue;
    };
} Expr_Element;

Stack(Stack_Expr, Expr_Element, 256, StackExprPop, StackExprPush, StackExprTopIndex)

#define printf(...)
Inter Expr_Element EvaluateExpression(Mem_Block expression, Registers *regs, Process_ID procID, u64 frameBase) {
    byte *scan = expression.ptr;
    byte *end  = expression.ptr + expression.size;

    Stack_Expr stack;
    b32 retIsSet = false;
    Expr_Element ret;

    while (scan < end) {
        u8 opcode = GetTypeAndAdvance(u8, &scan);

        switch (opcode) {
            case DW_OP_addr: {
                u64 address = GetTypeAndAdvance(u64, &scan);
            } break;

            case DW_OP_deref: {
                Expr_Element *address = StackExprPop(&stack);
                Expr_Element *elem = StackExprPush(&stack);
                elem->type = ELT_integral;
                ReadProcMemType(procID, (targetAddress)&elem->generic, (targetAddress)address->generic);
            } break;

            case DW_OP_const1u: {
                Expr_Element *uConst = StackExprPush(&stack);
                uConst->type = ELT_generic;
                uConst->generic = GetTypeAndAdvance(u8, &scan);
            } break;

            case DW_OP_const1s: {
                Expr_Element *sConst = StackExprPush(&stack);
                sConst->type = ELT_generic;
                sConst->generic = GetTypeAndAdvance(s8, &scan);
            } break;

            case DW_OP_const2u: {
                Expr_Element *uConst = StackExprPush(&stack);
                uConst->type = ELT_generic;
                uConst->generic = GetTypeAndAdvance(u16, &scan);
            } break;

            case DW_OP_const2s: {
                Expr_Element *sConst = StackExprPush(&stack);
                sConst->type = ELT_generic;
                sConst->generic = GetTypeAndAdvance(s16, &scan);
            } break;

            case DW_OP_const4u: {
                Expr_Element *uConst = StackExprPush(&stack);
                uConst->type = ELT_generic;
                uConst->generic = GetTypeAndAdvance(u32, &scan);
            } break;

            case DW_OP_const4s: {
                Expr_Element *sConst = StackExprPush(&stack);
                sConst->type = ELT_generic;
                sConst->generic = GetTypeAndAdvance(s32, &scan);
            } break;

            case DW_OP_const8u: {
                Expr_Element *uConst = StackExprPush(&stack);
                uConst->type = ELT_generic;
                uConst->generic = GetTypeAndAdvance(u64, &scan);
            } break;

            case DW_OP_const8s: {
                Expr_Element *sConst = StackExprPush(&stack);
                sConst->type = ELT_generic;
                sConst->generic = GetTypeAndAdvance(s64, &scan);
            } break;

            case DW_OP_constu: {
                Expr_Element *uConst = StackExprPush(&stack);
                uConst->type = ELT_generic;
                uConst->generic = ULEB128AndAdvance(&scan);
            } break;

            case DW_OP_consts: {
                Expr_Element *sConst = StackExprPush(&stack);
                sConst->type = ELT_generic;
                sConst->generic = SLEB128AndAdvance(&scan);
            } break;

            case DW_OP_dup: {
                Expr_Element *elem = StackExprPush(&stack);
                *elem = *StackExprTopIndex(&stack, 0);
            } break;

            case DW_OP_drop: {
                (void)StackExprPop(&stack);
            } break;

            case DW_OP_over: {
                Expr_Element *elem = StackExprPush(&stack);
                *elem = *StackExprTopIndex(&stack, 1);
            } break;

            case DW_OP_pick: {
                u8 index = GetTypeAndAdvance(u8, &scan);
                Expr_Element *elem = StackExprPush(&stack);
                *elem = *StackExprTopIndex(&stack, index);
            } break;

            case DW_OP_swap: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *addrElem2 = StackExprPush(&stack);
                *addrElem2 = elem1;
                Expr_Element *addrElem1 = StackExprPush(&stack);
                *addrElem1 = elem2;
            } break;

            case DW_OP_rot: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element elem3 = *StackExprPop(&stack);
                Expr_Element *addrElem3 = StackExprPush(&stack);
                *addrElem3 = elem1;
                Expr_Element *addrElem2 = StackExprPush(&stack);
                *addrElem2 = elem3;
                Expr_Element *addrElem1 = StackExprPush(&stack);
                *addrElem1 = elem2;
            } break;

            case DW_OP_xderef: {
                //TODO(JENH): uhhhh.
            } break;

            case DW_OP_abs: {
                Expr_Element elem = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem.type;
                pushed->generic = llabs(elem.sValue);
            } break;

            case DW_OP_and: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->generic = elem1.generic & elem2.generic;
            } break;

            case DW_OP_div: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->generic = elem1.generic & elem2.generic;
            } break;

            case DW_OP_minus: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->sValue = elem2.sValue / elem1.sValue;
            } break;

            case DW_OP_mod: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->sValue = elem2.sValue % elem1.sValue;
            } break;

            case DW_OP_mul: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->sValue = elem2.sValue * elem1.sValue;
            } break;

            case DW_OP_neg: {
                Expr_Element elem = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem.type;
                pushed->sValue = elem.sValue * -1;
            } break;

            case DW_OP_not: {
                Expr_Element elem = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem.type;
                pushed->sValue = ~elem.generic;
            } break;

            case DW_OP_or: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->generic = elem2.generic | elem1.generic;
            } break;

            case DW_OP_plus: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->sValue = elem2.sValue + elem1.sValue;
            } break;

            case DW_OP_plus_uconst: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->generic = elem2.generic + elem1.generic + ULEB128AndAdvance(&scan);
            } break;

            case DW_OP_shl: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->generic = elem2.generic << elem1.generic;
            } break;

            case DW_OP_shr: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->generic = elem2.generic >> elem1.generic;
            } break;

            case DW_OP_shra: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->sValue = elem2.sValue / (2 * elem1.generic);
            } break;

            case DW_OP_xor: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = elem1.type;
                pushed->generic = elem2.generic ^ elem1.generic;
            } break;

            case DW_OP_bra: {
                Expr_Element *elem = StackExprPop(&stack);
                s16 jumpOffset = GetTypeAndAdvance(s16, &scan);
                if (elem->generic != 0) {
                    scan += jumpOffset;
                }
            } break;

            case DW_OP_eq: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed  = StackExprPush(&stack);
                Assert(elem1.type == elem2.type);
                pushed->type = ELT_generic;
                pushed->generic = (elem2.sValue == elem1.sValue) ? 1 : 0;
            } break;

            case DW_OP_ge: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                Assert(elem1.type == elem2.type);
                pushed->type = ELT_generic;
                pushed->generic = (elem2.sValue >= elem1.sValue) ? 1 : 0;
            } break;

            case DW_OP_gt: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                Assert(elem1.type == elem2.type);
                pushed->type = ELT_generic;
                pushed->generic = (elem2.sValue > elem1.sValue) ? 1 : 0;
            } break;

            case DW_OP_le: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                Assert(elem1.type == elem2.type);
                pushed->type = ELT_generic;
                pushed->generic = (elem2.sValue <= elem1.sValue) ? 1 : 0;
            } break;

            case DW_OP_lt: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                Assert(elem1.type == elem2.type);
                pushed->type = ELT_generic;
                pushed->generic = (elem2.sValue < elem1.sValue) ? 1 : 0;
            } break;

            case DW_OP_ne: {
                Expr_Element elem1 = *StackExprPop(&stack);
                Expr_Element elem2 = *StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);
                Assert(elem1.type == elem2.type);
                pushed->type = ELT_generic;
                pushed->generic = (elem2.sValue != elem1.sValue) ? 1 : 0;
            } break;

            case DW_OP_skip: {
                s16 jumpOffset = GetTypeAndAdvance(s16, &scan);
                scan += jumpOffset;
            } break;

            case DW_OP_lit0:
            case DW_OP_lit1:
            case DW_OP_lit2:
            case DW_OP_lit3:
            case DW_OP_lit4:
            case DW_OP_lit5:
            case DW_OP_lit6:
            case DW_OP_lit7:
            case DW_OP_lit8:
            case DW_OP_lit9:
            case DW_OP_lit10:
            case DW_OP_lit11:
            case DW_OP_lit12:
            case DW_OP_lit13:
            case DW_OP_lit14:
            case DW_OP_lit15:
            case DW_OP_lit16:
            case DW_OP_lit17:
            case DW_OP_lit18:
            case DW_OP_lit19:
            case DW_OP_lit20:
            case DW_OP_lit21:
            case DW_OP_lit22:
            case DW_OP_lit23:
            case DW_OP_lit24:
            case DW_OP_lit25:
            case DW_OP_lit26:
            case DW_OP_lit27:
            case DW_OP_lit28:
            case DW_OP_lit29:
            case DW_OP_lit30:
            case DW_OP_lit31: {
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = ELT_generic;
                pushed->generic = ((u64)opcode - DW_OP_lit0);
            } break;

            case DW_OP_reg0:
            case DW_OP_reg1:
            case DW_OP_reg2:
            case DW_OP_reg3:
            case DW_OP_reg4:
            case DW_OP_reg5:
            case DW_OP_reg6:
            case DW_OP_reg7:
            case DW_OP_reg8:
            case DW_OP_reg9:
            case DW_OP_reg10:
            case DW_OP_reg11:
            case DW_OP_reg12:
            case DW_OP_reg13:
            case DW_OP_reg14:
            case DW_OP_reg15:
            case DW_OP_reg16:
            case DW_OP_reg17:
            case DW_OP_reg18:
            case DW_OP_reg19:
            case DW_OP_reg20:
            case DW_OP_reg21:
            case DW_OP_reg22:
            case DW_OP_reg23:
            case DW_OP_reg24:
            case DW_OP_reg25:
            case DW_OP_reg26:
            case DW_OP_reg27:
            case DW_OP_reg28:
            case DW_OP_reg29:
            case DW_OP_reg30:
            case DW_OP_reg31: {
                retIsSet = true;
                ret.type = ELT_loc_reg;
                ret.reg  = (opcode - DW_OP_reg0);
            } break;

            case DW_OP_breg0:
            case DW_OP_breg1:
            case DW_OP_breg2:
            case DW_OP_breg3:
            case DW_OP_breg4:
            case DW_OP_breg5:
            case DW_OP_breg6:
            case DW_OP_breg7:
            case DW_OP_breg8:
            case DW_OP_breg9:
            case DW_OP_breg10:
            case DW_OP_breg11:
            case DW_OP_breg12:
            case DW_OP_breg13:
            case DW_OP_breg14:
            case DW_OP_breg15:
            case DW_OP_breg16:
            case DW_OP_breg17:
            case DW_OP_breg18:
            case DW_OP_breg19:
            case DW_OP_breg20:
            case DW_OP_breg21:
            case DW_OP_breg22:
            case DW_OP_breg23:
            case DW_OP_breg24:
            case DW_OP_breg25:
            case DW_OP_breg26:
            case DW_OP_breg27:
            case DW_OP_breg28:
            case DW_OP_breg29:
            case DW_OP_breg30:
            case DW_OP_breg31: {
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = ELT_generic;
                pushed->generic = GetLoadedRegister(regs, opcode - DW_OP_breg0) + SLEB128AndAdvance(&scan);
            } break;

            case DW_OP_regx: {
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = ELT_loc_reg;
                pushed->reg  = ULEB128AndAdvance(&scan);
            } break;

            case DW_OP_fbreg: {
                Expr_Element *pushed = StackExprPush(&stack);
                s64 offset = SLEB128AndAdvance(&scan);
                pushed->type = ELT_generic;
                pushed->generic = frameBase + offset;
                printf("DW_OP_fbreg(%lld)\n", offset);
            } break;

            case DW_OP_bregx: {
                Expr_Element *pushed = StackExprPush(&stack);
                pushed->type = ELT_generic;
                pushed->generic = GetLoadedRegister(regs, ULEB128AndAdvance(&scan)) + SLEB128AndAdvance(&scan);
            } break;

            case DW_OP_piece: {
                Assert(false && "must implement.");
            } break;

            case DW_OP_deref_size: {
                u8 size = GetTypeAndAdvance(u8, &scan);
                Assert(size <= sizeof(targetAddress));

                Expr_Element *address = StackExprPop(&stack);
                Expr_Element *pushed = StackExprPush(&stack);

                pushed->type = ELT_generic;
                ReadProcMem(procID, &pushed->generic, (targetAddress)address->generic, size);
            } break;

            case DW_OP_xderef_size: {
                Assert(false && "must implement.");
            } break;

            case DW_OP_nop: {
                Assert(false && "shouldn't be here... i think.");
            } break;

            case DW_OP_push_object_address: {
            } break;

            case DW_OP_call2: {
            } break;

            case DW_OP_call4: {
            } break;

            case DW_OP_call_ref: {
            } break;

            case DW_OP_form_tls_address: {
            } break;

            case DW_OP_call_frame_cfa: break;

            case DW_OP_bit_piece: {
            } break;

            case DW_OP_implicit_value: {
                retIsSet = true;
                ret.type = ELT_implicit;
                ret.implicit.size = ULEB128AndAdvance(&scan);
                ret.implicit.ptr  = scan;
                scan += ret.implicit.size;
            } break;

            case DW_OP_stack_value: {
                retIsSet = true;
                ret = *StackExprPush(&stack);
                ret.type = ELT_stack_value;
            } break;

            case DW_OP_implicit_pointer: {
                Assert(false && "to implement.");
            } break;

            case DW_OP_addrx: {
            } break;

            case DW_OP_constx: {
            } break;

            case DW_OP_entry_value: {
            } break;

            case DW_OP_const_type: {
            } break;

            case DW_OP_regval_type: {
            } break;

            case DW_OP_deref_type: {
            } break;

            case DW_OP_xderef_type: {
            } break;

            case DW_OP_convert: {
            } break;

            case DW_OP_reinterpret: {
            } break;

            NO_DEFAULT
        }
    }

    if (!retIsSet) {
        ret = *StackExprPop(&stack);
    }

    return ret;
}
#undef printf

Inter Expr_Element EvalExprFrameBase(Mem_Block expression, Registers *regs, Process_ID procID, u64 CFA) {
    Expr_Element ret;

    byte *scan = expression.ptr;
    u8 firstOpcode = *(u8 *)scan;

    if (firstOpcode == DW_OP_call_frame_cfa) {
        printf("DW_OP_call_frame_cfa\n");
        ret.type = ELT_generic;
        ret.generic = CFA;
        return ret;
    }

    return EvaluateExpression(expression, regs, procID, 0);
}

#endif //DSV_DWARFEXPR_H
