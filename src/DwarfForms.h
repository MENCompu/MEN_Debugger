#ifndef DSV_DWARFFORMS_H
#define DSV_DWARFFORMS_H

Inter u64 GetInitialLengthAndAdvance(byte **scan, u8 *format) {
    u64 ret;

    if ((*(u64 *)*scan & 0xFFFFFFFF) == 0xFFFFFFFF) {
        *format = DWARF_64_BITS;
        ret = (*(u64 *)*scan) >> 16;
        *scan += 8;
    } else {
        *format = DWARF_32_BITS;
        ret = *(u32 *)*scan;
        *scan += 4;
    }

    return ret;
}

Inter u64 GetValueArchFormatAndAdvance(byte **scan, u8 format) {
    u64 ret;

    if (format == DWARF_32_BITS) {
	    ret = *(u32 *)*scan;
	    *scan += 4;
    } else { // format == DWARF_64_BITS
	    ret = *(u64 *)*scan;
	    *scan += 8;
    }

    return ret;
}

Inter u64 AddressByAddressSize(byte *scan, u32 addressSize) {
    u64 ret = *(u64 *)scan& (0xFFFFFFFFFFFFFFFF >> ((sizeof(ret) - addressSize) * 8));
    return ret;
}

Inter s64 GetImplicitConst(Array_Code_ImplConst *codeImplConst, Array_Abbrev_Def_Entry *abbrevDef,
                           u32 code, Dwarf_Attrib name) {
    foreach (Code_ImplConst, ImplConstEntry, *codeImplConst) {
        if (ImplConstEntry->code == code) {
            u32 implConstIndex = 0;

            //for (u32 abbrevDefIndex = 0; abbrevDef[abbrevDefIndex].name; ++abbrevDefIndex) {
            foreach (Abbrev_Def_Entry, abbrevDefEntry, *abbrevDef) {
                if (abbrevDefEntry->name == name) {
                    Assert(implConstIndex < ImplConstEntry->implConst.size);
                    s64 ret = ImplConstEntry->implConst.A[implConstIndex];
                    return ret;
                }

                if (abbrevDefEntry->form == DW_FORM_implicit_const) { ++implConstIndex; }
            }
        }
    }

    printf("There's no implicit constant.\n");
    return 0;
}

Inter byte *FormToMacPtr(Dwarf_Form form, byte **valueScan, b32 format) {
    switch (form) {
        case DW_FORM_sec_offset: {
            u64 offset = GetValueArchFormatAndAdvance(valueScan, format);

            byte *macPtr = (byte *)offset; //macHeader.secBaseAddress + offset;
            return macPtr;
        } break;
    }

    return 0;
}

Inter byte *FormToAddrPtr(Dwarf_Form form, byte **valueScan, b32 format) {
    switch (form) {
        case DW_FORM_sec_offset: {
            u64 offset = GetValueArchFormatAndAdvance(valueScan, format);

            byte *addrPtr = (byte *)offset; //addrHeader.secBaseAddress + offset;
            return addrPtr;
        } break;
    }

    return 0;
}

Inter byte *FormToOffsetStr(Dwarf_Form form, byte **valueScan, b32 format) {
    switch (form) {
        case DW_FORM_sec_offset: {
            u64 offset = GetValueArchFormatAndAdvance(valueScan, format);

            byte *addrPtr = (byte *)offset; //offsetStrHeader.secBaseAddress + offset;
            return addrPtr;
        } break;
    }

    return 0;
}

Inter byte *FormToRnglist(Dwarf_Form form, byte **valueScan, b32 format) {
    switch (form) {
        case DW_FORM_rnglistx: {
            u64 index = ULEB128AndAdvance(valueScan);
            u64 offset;

            if (rnglistHeader.format == DWARF_32_BITS) {
                Assert(index < rnglistHeader.offsets32.size);
                offset = rnglistHeader.offsets32.A[index];
            } else { // format == DWARF_64_BITS
                Assert(index < rnglistHeader.offsets64.size);
                offset = rnglistHeader.offsets64.A[index];
            }

            byte *rnglistAddress = rnglistHeader.secBaseAddress + offset;
            return rnglistAddress;
        } break;

        case DW_FORM_sec_offset: {
            u64 offset = GetValueArchFormatAndAdvance(valueScan, rnglistHeader.format);

            byte *rnglistAddress = rnglistHeader.secBaseAddress + offset;
            return rnglistAddress;
        } break;

        NO_DEFAULT
    }

    return 0;
}

Inter byte *FormToLoclist(Dwarf_Form form, byte **valueScan, b32 format) {
    //Assert(false);

    switch (form) {
        case DW_FORM_loclistx: {
            u64 ret = ULEB128AndAdvance(valueScan);
            return (byte *)ret;
        } break;

        case DW_FORM_sec_offset: {
            u64 ret = GetValueArchFormatAndAdvance(valueScan, format);
            return (byte *)ret;
        } break;

        NO_DEFAULT
    }

    return 0;
}

Inter Mem_Block FormToExprloc(Dwarf_Form form, byte **valueScan) {
    switch (form) {
        case DW_FORM_exprloc: {
            Mem_Block ret;

            ret.size  = ULEB128AndAdvance(valueScan);
            ret.ptr   = *valueScan;

            *valueScan += ret.size;
	        return ret;
        } break;

        NO_DEFAULT
    }

    Mem_Block ret = {0};
    return ret;
}

Inter u64 FormToReference(Dwarf_Form form, byte **valueScan, b32 format) {
    switch (form) {
        case DW_FORM_ref_addr: {
        } break;

        case DW_FORM_ref1: {
            u64 reference = (u64)(*(u8  *)*valueScan);
            *valueScan += sizeof(u8);

            return reference;
        } break;

        case DW_FORM_ref2: {
            u64 reference = (u64)(*(u16 *)*valueScan);
            *valueScan += sizeof(u16);

            return reference;
        } break;

        case DW_FORM_ref4: {
            u64 reference = (u64)(*(u32 *)*valueScan);
            *valueScan += sizeof(u32);

            return reference;
        } break;

        case DW_FORM_ref8: {
            u64 reference = (u64)(*(u64 *)*valueScan);
            *valueScan += sizeof(u64);

            return reference;
        } break;

        case DW_FORM_ref_udata: {
            u64 reference = ULEB128AndAdvance(valueScan);

            return reference;
        } break;

        case DW_FORM_ref_sup4: {
        } break;

        case DW_FORM_ref_sig8: {
        } break;

        case DW_FORM_ref_sup8: {
        } break;

        NO_DEFAULT
    }

    return MAX_VALUE_u64;
}

Inter Mem_Block FormToMemBlock(Dwarf_Form form, u8 **valueScan) {
    switch (form) {
	    case DW_FORM_block1: {
            Mem_Block ret;

	        u8 value = *(u8  *)*valueScan;
            ret.size = (u64)value;
            ret.ptr  = (*valueScan + sizeof(value));

            *valueScan += value + sizeof(value);
	        return ret;
	    } break;

	    case DW_FORM_block2: {
            Mem_Block ret;

            u16 value = *(u16 *)*valueScan;
            ret.size = (u64)value;
            ret.ptr  = (*valueScan + sizeof(value));

            *valueScan += value + sizeof(value);
	        return ret;
	    } break;

	    case DW_FORM_block4: {
            Mem_Block ret;

	        u32 value = *(u32 *)*valueScan;
            ret.size = (u64)value;
            ret.ptr  = (*valueScan + sizeof(value));

            *valueScan += value + sizeof(value);
	        return ret;
	    } break;

	    case DW_FORM_block: {
            Mem_Block ret;

	        u64 value = ULEB128AndAdvance(valueScan);
            ret.size = (u64)value;
            ret.ptr  = *valueScan;

            *valueScan += value;
	        return ret;
	    } break;

	    default: {
	        printf("hola me gusta le pene.\n");

            Mem_Block ret = {0};

	        return ret;
	    } break;
    }
}

Inter b32 DieAttribToFlag(Dwarf_Form form, u8 **valueScan) {
    switch (form) {
        case DW_FORM_flag: {
            b32 value = (b32)**valueScan;
            *valueScan += sizeof(byte);

            return value;
        } break;

        case DW_FORM_flag_present: {
            return true;
        } break;

        default: {
            printf("hola me gusta le pene.\n");
            return 0;
        } break;
    }
}

Inter s64 FormToSConst(Dwarf_Form form, u8 **valueScan, Array_Code_ImplConst *codeImplConst,
                       Array_Abbrev_Def_Entry *abbrevDef, u32 code, Dwarf_Attrib name) {
    switch (form) {
        case DW_FORM_data1: {
            s64 value = (s64)(*(s8  *)*valueScan);
            *valueScan += sizeof(byte);

            return value;
        } break;

        case DW_FORM_data2: {
            s64 value = (s64)(*(s16 *)*valueScan);
            *valueScan += 2;

            return value;
        } break;

        case DW_FORM_data4: {
            s64 value = (s64)(*(s32 *)*valueScan);
            *valueScan += 4;

            return value;
        } break;

        case DW_FORM_data8: {
            s64 value = (s64)(*(s64 *)*valueScan);
            *valueScan += 8;

            return value;
        } break;

        case DW_FORM_sdata: {
            s64 value = SLEB128AndAdvance(valueScan);
            return value;
        } break;

        case DW_FORM_implicit_const: {
            s64 value = GetImplicitConst(codeImplConst, abbrevDef, code, name);
            return value;
        } break;

        default: {
            printf("ta' malo.\n");
            return 0;
        } break;
    }
}

Inter u64 FormToUConst(Dwarf_Form form, u8 **valueScan) {
    switch (form) {
        case DW_FORM_data1: {
            u64 value = (u64)(*(u8  *)*valueScan);
            *valueScan += 1;

            return value;
        } break;

        case DW_FORM_data2: {
            u64 value = (u64)(*(u16 *)*valueScan);
            *valueScan += 2;

            return value;
        } break;

        case DW_FORM_data4: {
            u64 value = (u64)(*(u32 *)*valueScan);
            *valueScan += 4;

            return value;
        } break;

        case DW_FORM_data8: {
            u64 value = (u64)(*(u64 *)*valueScan);
            *valueScan += 8;

            return value;
        } break;

        case DW_FORM_udata: {
            u64 value = ULEB128AndAdvance(valueScan);
            return value;
        } break;

        NO_DEFAULT
    }

    return 0;
}

Inter u64 FormToAddress(Dwarf_Form form, u8 **valueScan, u32 addressSize) {
    switch (form) {
        case DW_FORM_addr: {
            u64 address = AddressByAddressSize(*valueScan, addressSize);
            *valueScan += addressSize;
            return address;
        } break;

        case DW_FORM_addrx1: {
            u64 address = *(u8  *)*valueScan;
            *valueScan += 2;
            return address;
        } break;

        case DW_FORM_addrx2: {
            u64 address = *(u16 *)*valueScan;
            *valueScan += 2;
            return address;
        } break;

        case DW_FORM_addrx3: {
            u64 address = *(u32 *)*valueScan & 0x00FFFFFF;
            *valueScan += 3;
            return address;
        } break;

        case DW_FORM_addrx4: {
            u64 address = *(u32 *)*valueScan;
            *valueScan += 4;
            return address;
        } break;

        case DW_FORM_addrx: {
            u64 address = *(u8  *)*valueScan;
            *valueScan += 1;
            return address;
        } break;

        default: {
            printf("ta malo\n");
            return 0;
        } break;
    }
}

Inter String FormToString(Dwarf_Form form, byte **valueScan, u32 format) {
    u64 strOffset;

    switch (form) {
        case DW_FORM_string: {
            String ret;

            ret.str = (char *)*valueScan;
            ret.length = CStrLen((CString)*valueScan) - 1;

            *valueScan += ret.length + 1;

            return ret;
        } break;

        case DW_FORM_strp_sup: {
        } break;

        case DW_FORM_line_strp: {
                String ret;

                strOffset = GetValueArchFormatAndAdvance(valueScan, format);

                Dwarf_Section *line_str = &dwarfSections.line_str;

                ret.str = (char *)((byte *)line_str->ptr + strOffset);
                ret.length = CStrLen((CString)ret.str) - 1;

            return ret;
        } break;

        case DW_FORM_strx1: {
            u64 firstOffset = *(u8  *)*valueScan;
            *valueScan += 1;

            Dwarf_Section *str_offsets = &dwarfSections.str_offsets;

            byte *ptrOffset = (byte *)str_offsets->ptr + firstOffset;
            strOffset = (format) ? *(u32 *)ptrOffset : *(u64 *)ptrOffset;

            goto str_section;
        } break;

        case DW_FORM_strx2: {
            u64 firstOffset = *(u16 *)*valueScan;
            *valueScan += 2;

            Dwarf_Section *str_offsets = &dwarfSections.str_offsets;

            byte *ptrOffset = (byte *)str_offsets->ptr + firstOffset;
            strOffset = (format) ? *(u32 *)ptrOffset : *(u64 *)ptrOffset;

            goto str_section;
        } break;

        case DW_FORM_strx3: {
            u64 firstOffset = *(u32 *)*valueScan & 0x00FFFFFF;
            *valueScan += 3;

            Dwarf_Section *str_offsets = &dwarfSections.str_offsets;

            byte *ptrOffset = (byte *)str_offsets->ptr + firstOffset;
            strOffset = (format) ? *(u32 *)ptrOffset : *(u64 *)ptrOffset;

            goto str_section;
        } break;

        case DW_FORM_strx4: {
            u64 firstOffset = *(u32 *)*valueScan;
            *valueScan += 4;

            Dwarf_Section *str_offsets = &dwarfSections.str_offsets;

            byte *ptrOffset = (byte *)str_offsets->ptr + firstOffset;
            strOffset = (format) ? *(u32 *)ptrOffset : *(u64 *)ptrOffset;

            goto str_section;
        } break;

        case DW_FORM_strx: {
            u64 firstOffset = *(u8  *)*valueScan;
            *valueScan += 1;

            Dwarf_Section *str_offsets = &dwarfSections.str_offsets;

            byte *ptrOffset = (byte *)str_offsets->ptr + firstOffset;
            strOffset = (format) ? *(u32 *)ptrOffset : *(u64 *)ptrOffset;

            goto str_section;
        } break;

        case DW_FORM_strp: {
            String ret;

            strOffset = GetValueArchFormatAndAdvance(valueScan, format);

str_section:
            Dwarf_Section *str = &dwarfSections.str;

            ret.str = (char *)((byte *)str->ptr + strOffset);
            ret.length = CStrLen((CString)ret.str) - 1;

            return ret;
        } break;

        NO_DEFAULT
    }

    String str = {0};
    return str;
}

Inter void PrintDIEValue(DIE_Value *DIEValue) {
    switch (DIEValue->dwarfClass) {
        case DW_CLASS_address: {
            printf("%llx", DIEValue->address);
        } break;

        case DW_CLASS_block: {
            printf("size = %llu | mem = %p", DIEValue->block.size, DIEValue->block.ptr);
        } break;

        case DW_CLASS_constant: {
            Assert(false && "erase all constant from humanity.");
            //printf("%lld", constant);
        } break;

        case DW_CLASS_sconstant: {
            printf("%lld", DIEValue->sConst);
        } break;

        case DW_CLASS_uconstant: {
            //if (name == DW_AT_language) { PrintDwarfLang(constant); }
            printf("%llu", DIEValue->uConst);
        } break;

        case DW_CLASS_string: {
            printf("%s", DIEValue->string.str);
        } break;

        case DW_CLASS_flag: {
            printf((DIEValue->flag) ? "true" : "false");
        } break;

        case DW_CLASS_reference: {
            printf("<0x%llx>", DIEValue->reference);
        } break;

        case DW_CLASS_exprloc: {
            printf("%llu byte block: ", DIEValue->exprloc.size);
            for (u32 i = 0; i < DIEValue->exprloc.size; ++i) {
                printf("%02x ", DIEValue->exprloc.ptr[i]);
            }
            //goto not_supported;
        } break;

        case DW_CLASS_loclist: {
            printf("0x%p", DIEValue->loclist);
        } break;

        case DW_CLASS_rnglist: {
            u64 offset = rnglistHeader.secBaseAddress - DIEValue->rnglist;
            printf("0x%llx", offset);
        } break;

        case DW_CLASS_addrptr: {
            goto not_supported;
        } break;

        case DW_CLASS_lineptr: {
            u64 lineOffset = dwarfSections.line.ptr - DIEValue->line;
            printf("0x%llx", lineOffset);
        } break;

        case DW_CLASS_loclistsptr: {
            goto not_supported;
        } break;

        case DW_CLASS_macptr: {
            goto not_supported;
        } break;

        case DW_CLASS_rnglistsptr: {
            goto not_supported;
        } break;

        case DW_CLASS_stroffsetsptr: {
            goto not_supported;
        } break;

        default: {
not_supported:;
        } break;
    }
}

Inter void NextDIE(Dwarf_DIE *DIE) {
    for (; DIE->cursorAbbrev < DIE->abbrev->def.size; ++DIE->cursorAbbrev) {
        Abbrev_Def_Entry *defEntry = &DIE->abbrev->def.A[DIE->cursorAbbrev];

        Dwarf_Class dwarfClass = GetDwarfClass(DIE->CU->header.version, defEntry->name, defEntry->form);

        switch (dwarfClass) {
            case DW_CLASS_address: {
                FormToAddress(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.addressSize);
            } break;

            case DW_CLASS_block: {
                FormToMemBlock(defEntry->form, &DIE->cursorDataRaw);
            } break;

            case DW_CLASS_constant: {
                FormToUConst(defEntry->form, &DIE->cursorDataRaw);
            } break;

            case DW_CLASS_sconstant: {
                FormToSConst(defEntry->form, &DIE->cursorDataRaw, &DIE->CU->header.abbrev.codeImplConst,
                             &DIE->abbrev->def, DIE->code, defEntry->name);
            } break;

            case DW_CLASS_uconstant: {
                FormToUConst(defEntry->form, &DIE->cursorDataRaw);
            } break;

            case DW_CLASS_string: {
                FormToString(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.format);
            } break;

            case DW_CLASS_flag: {
                DieAttribToFlag(defEntry->form, &DIE->cursorDataRaw);
            } break;

            case DW_CLASS_reference: {
                FormToReference(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.format);
            } break;

            case DW_CLASS_exprloc: {
                FormToExprloc(defEntry->form, &DIE->cursorDataRaw);
                //goto not_supported;
            } break;

            case DW_CLASS_loclist: {
                FormToLoclist(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.format);
            } break;

            case DW_CLASS_rnglist: {
                FormToRnglist(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.format);
            } break;

#if 0
            case DW_CLASS_addrptr: {
                goto not_supported;
            } break;

            case DW_CLASS_lineptr: {
                Assert(defEntry->form == DW_FORM_sec_offset);
                GetValueArchFormatAndAdvance(scan, CU->header.format);
            } break;

            case DW_CLASS_loclistsptr: {
                goto not_supported;
            } break;

            case DW_CLASS_macptr: {
                goto not_supported;
            } break;

            case DW_CLASS_rnglistsptr: {
                goto not_supported;
            } break;

            case DW_CLASS_stroffsetsptr: {
                goto not_supported;
            } break;
#endif

            default: {
    //not_supported:
                PrintAttrib(defEntry->name, "%-18s ");
                PrintForm(defEntry->form, 0);
            } break;
        }
    }

    DIE->dataRaw = DIE->cursorDataRaw;
    UpdateDIE(DIE);
}

Inter void NextDIEValue(Dwarf_DIE *DIE) {
    Abbrev_Def_Entry *defEntry = &DIE->abbrev->def.A[DIE->cursorAbbrev];

    DIE_Value *DIEValueEntry = &DIE->data[DIE->cursorAbbrev];
    DIEValueEntry->dwarfClass = GetDwarfClass(DIE->CU->header.version, defEntry->name, defEntry->form);

    //PrintAttrib(defEntry->name, "%s\n");
    //PrintForm(defEntry->form, "%s\n");

    switch (DIEValueEntry->dwarfClass) {
        case DW_CLASS_address: {
            DIEValueEntry->address = FormToAddress(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.addressSize);
        } break;

        case DW_CLASS_block: {
            DIEValueEntry->block = FormToMemBlock(defEntry->form, &DIE->cursorDataRaw);
        } break;

        case DW_CLASS_constant: {
            PrintAttrib(defEntry->name, "%s    ");
            PrintForm(defEntry->form, 0);
            Assert(false && "is plain constant");
            DIEValueEntry->uConst = FormToUConst(defEntry->form, &DIE->cursorDataRaw);
        } break;

        case DW_CLASS_sconstant: {
            DIEValueEntry->sConst = FormToSConst(defEntry->form, &DIE->cursorDataRaw, &DIE->CU->header.abbrev.codeImplConst,
                                                 &DIE->abbrev->def, DIE->code, defEntry->name);
        } break;

        case DW_CLASS_uconstant: {
            DIEValueEntry->uConst = FormToUConst(defEntry->form, &DIE->cursorDataRaw);
        } break;

        case DW_CLASS_string: {
            DIEValueEntry->string = FormToString(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.format);
        } break;

        case DW_CLASS_flag: {
            DIEValueEntry->flag = DieAttribToFlag(defEntry->form, &DIE->cursorDataRaw);
        } break;

        case DW_CLASS_reference: {
            DIEValueEntry->reference = FormToReference(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.format);
        } break;

        case DW_CLASS_exprloc: {
            DIEValueEntry->exprloc = FormToExprloc(defEntry->form, &DIE->cursorDataRaw);
            //goto not_supported;
        } break;

        case DW_CLASS_loclist: {
            DIEValueEntry->loclist = FormToLoclist(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.format);
        } break;

        case DW_CLASS_rnglist: {
            DIEValueEntry->rnglist = FormToRnglist(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.format);
        } break;

#if 0
        case DW_CLASS_addrptr: {
            DIEValueEntry->addr = FormToAddrPtr(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.format);
            goto not_supported;
        } break;

        case DW_CLASS_lineptr: {
            Assert(defEntry->form == DW_FORM_sec_offset);
            Assert(false && "need to offset from line table");
            DIEValueEntry->line = (byte *)GetValueArchFormatAndAdvance(&DIE->cursorDataRaw, DIE->CU->header.format);
        } break;

        case DW_CLASS_loclistsptr: {
            Assert(false && "loclistsptr");
            goto not_supported;
        } break;

        case DW_CLASS_macptr: {
            DIEValueEntry->mac = FormToMacPtr(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.format);
            goto not_supported;
        } break;

        case DW_CLASS_rnglistsptr: {
            Assert(false && "rnglistsptr");
            goto not_supported;
        } break;

        case DW_CLASS_stroffsetsptr: {
            DIEValueEntry->strOffsets = FormToOffsetStr(defEntry->form, &DIE->cursorDataRaw, DIE->CU->header.format);
            goto not_supported;
        } break;
#endif

        default: {
//not_supported:
            PrintAttrib(defEntry->name, "%-18s ");
            PrintForm(defEntry->form, 0);
        } break;
    }
}

Inter void ParseDIE(Dwarf_DIE *DIE) {
    for (; DIE->cursorAbbrev < DIE->abbrev->def.size; ++DIE->cursorAbbrev) {
        NextDIEValue(DIE);
    }
}

#endif //DSV_DWARFFORMS_H
