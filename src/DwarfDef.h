#ifndef DSV_DWARFDEF_H
#define DSV_DWARFDEF_H

#define DW_CHILDREN_no  0x00
#define DW_CHILDREN_yes 0x01

#define LIST_DWARF_TAG                          \
    X(DW_TAG_nul                      , 0x00  ) \
    X(DW_TAG_array_type               , 0x01  ) \
    X(DW_TAG_class_type               , 0x02  ) \
    X(DW_TAG_entry_point              , 0x03  ) \
    X(DW_TAG_enumeration_type         , 0x04  ) \
    X(DW_TAG_formal_parameter         , 0x05  ) \
    X(DW_TAG_imported_declaration     , 0x08  ) \
    X(DW_TAG_label                    , 0x0a  ) \
    X(DW_TAG_lexical_block            , 0x0b  ) \
    X(DW_TAG_member                   , 0x0d  ) \
    X(DW_TAG_pointer_type             , 0x0f  ) \
    X(DW_TAG_reference_type           , 0x10  ) \
    X(DW_TAG_compile_unit             , 0x11  ) \
    X(DW_TAG_string_type              , 0x12  ) \
    X(DW_TAG_structure_type           , 0x13  ) \
    X(DW_TAG_subroutine_type          , 0x15  ) \
    X(DW_TAG_typedef                  , 0x16  ) \
    X(DW_TAG_union_type               , 0x17  ) \
    X(DW_TAG_unspecified_parameters   , 0x18  ) \
    X(DW_TAG_variant                  , 0x19  ) \
    X(DW_TAG_common_block             , 0x1a  ) \
    X(DW_TAG_common_inclusion         , 0x1b  ) \
    X(DW_TAG_inheritance              , 0x1c  ) \
    X(DW_TAG_inlined_subroutine       , 0x1d  ) \
    X(DW_TAG_module                   , 0x1e  ) \
    X(DW_TAG_ptr_to_member_type       , 0x1f  ) \
    X(DW_TAG_set_type                 , 0x20  ) \
    X(DW_TAG_subrange_type            , 0x21  ) \
    X(DW_TAG_with_stmt                , 0x22  ) \
    X(DW_TAG_access_declaration       , 0x23  ) \
    X(DW_TAG_base_type                , 0x24  ) \
    X(DW_TAG_catch_block              , 0x25  ) \
    X(DW_TAG_const_type               , 0x26  ) \
    X(DW_TAG_constant                 , 0x27  ) \
    X(DW_TAG_enumerator               , 0x28  ) \
    X(DW_TAG_file_type                , 0x29  ) \
    X(DW_TAG_friend                   , 0x2a  ) \
    X(DW_TAG_namelist                 , 0x2b  ) \
    X(DW_TAG_namelist_item            , 0x2c  ) \
    X(DW_TAG_packed_type              , 0x2d  ) \
    X(DW_TAG_subprogram               , 0x2e  ) \
    X(DW_TAG_template_type_parameter  , 0x2f  ) \
    X(DW_TAG_template_value_parameter , 0x30  ) \
    X(DW_TAG_thrown_type              , 0x31  ) \
    X(DW_TAG_try_block                , 0x32  ) \
    X(DW_TAG_variant_part             , 0x33  ) \
    X(DW_TAG_variable                 , 0x34  ) \
    X(DW_TAG_volatile_type            , 0x35  ) \
    X(DW_TAG_dwarf_procedure          , 0x36  ) \
    X(DW_TAG_restrict_type            , 0x37  ) \
    X(DW_TAG_interface_type           , 0x38  ) \
    X(DW_TAG_namespace                , 0x39  ) \
    X(DW_TAG_imported_module          , 0x3a  ) \
    X(DW_TAG_unspecified_type         , 0x3b  ) \
    X(DW_TAG_partial_unit             , 0x3c  ) \
    X(DW_TAG_imported_unit            , 0x3d  ) \
    X(DW_TAG_condition                , 0x3f  ) \
    X(DW_TAG_shared_type              , 0x40  ) \
    X(DW_TAG_type_unit                , 0x41  ) \
    X(DW_TAG_rvalue_reference_type    , 0x42  ) \
    X(DW_TAG_template_alias           , 0x43  ) \
    X(DW_TAG_coarray_type             , 0x44  ) \
    X(DW_TAG_generic_subrange         , 0x45  ) \
    X(DW_TAG_dynamic_type             , 0x46  ) \
    X(DW_TAG_atomic_type              , 0x47  ) \
    X(DW_TAG_call_site                , 0x48  ) \
    X(DW_TAG_call_site_parameter      , 0x49  ) \
    X(DW_TAG_skeleton_unit            , 0x4a  ) \
    X(DW_TAG_immutable_type           , 0x4b  ) \
    X(DW_TAG_lo_user                  , 0x4080) \
    X(DW_TAG_hi_user                  , 0xffff) \

#define LIST_DWARF_AT                          \
    X(DW_AT_nul                     , 0x00   ) \
    X(DW_AT_sibling                 , 0x01   ) \
    X(DW_AT_location                , 0x02   ) \
    X(DW_AT_name                    , 0x03   ) \
    X(DW_AT_ordering                , 0x09   ) \
    X(DW_AT_byte_size               , 0x0b   ) \
    X(DW_AT_bit_size                , 0x0d   ) \
    X(DW_AT_stmt_list               , 0x10   ) \
    X(DW_AT_low_pc                  , 0x11   ) \
    X(DW_AT_high_pc                 , 0x12   ) \
    X(DW_AT_language                , 0x13   ) \
    X(DW_AT_discr                   , 0x15   ) \
    X(DW_AT_discr_value             , 0x16   ) \
    X(DW_AT_visibility              , 0x17   ) \
    X(DW_AT_import                  , 0x18   ) \
    X(DW_AT_string_length           , 0x19   ) \
    X(DW_AT_common_reference        , 0x1a   ) \
    X(DW_AT_comp_dir                , 0x1b   ) \
    X(DW_AT_const_value             , 0x1c   ) \
    X(DW_AT_containing_type         , 0x1d   ) \
    X(DW_AT_default_value           , 0x1e   ) \
    X(DW_AT_inline                  , 0x20   ) \
    X(DW_AT_is_optional             , 0x21   ) \
    X(DW_AT_lower_bound             , 0x22   ) \
    X(DW_AT_producer                , 0x25   ) \
    X(DW_AT_prototyped              , 0x27   ) \
    X(DW_AT_return_addr             , 0x2a   ) \
    X(DW_AT_start_scope             , 0x2c   ) \
    X(DW_AT_bit_stride              , 0x2e   ) \
    X(DW_AT_upper_bound             , 0x2f   ) \
    X(DW_AT_abstract_origin         , 0x31   ) \
    X(DW_AT_accessibility           , 0x32   ) \
    X(DW_AT_address_class           , 0x33   ) \
    X(DW_AT_artificial              , 0x34   ) \
    X(DW_AT_base_types              , 0x35   ) \
    X(DW_AT_calling_convention      , 0x36   ) \
    X(DW_AT_count                   , 0x37   ) \
    X(DW_AT_data_member_location    , 0x38   ) \
    X(DW_AT_decl_column             , 0x39   ) \
    X(DW_AT_decl_file               , 0x3a   ) \
    X(DW_AT_decl_line               , 0x3b   ) \
    X(DW_AT_declaration             , 0x3c   ) \
    X(DW_AT_discr_list              , 0x3d   ) \
    X(DW_AT_encoding                , 0x3e   ) \
    X(DW_AT_external                , 0x3f   ) \
    X(DW_AT_frame_base              , 0x40   ) \
    X(DW_AT_friend                  , 0x41   ) \
    X(DW_AT_identifier_case         , 0x42   ) \
    X(DW_AT_macro_info              , 0x43   ) \
    X(DW_AT_namelist_item           , 0x44   ) \
    X(DW_AT_priority                , 0x45   ) \
    X(DW_AT_segment                 , 0x46   ) \
    X(DW_AT_specification           , 0x47   ) \
    X(DW_AT_static_link             , 0x48   ) \
    X(DW_AT_type                    , 0x49   ) \
    X(DW_AT_use_location            , 0x4a   ) \
    X(DW_AT_variable_parameter      , 0x4b   ) \
    X(DW_AT_virtuality              , 0x4c   ) \
    X(DW_AT_vtable_elem_location    , 0x4d   ) \
    X(DW_AT_allocated               , 0x4e   ) \
    X(DW_AT_associated              , 0x4f   ) \
    X(DW_AT_data_location           , 0x50   ) \
    X(DW_AT_byte_stride             , 0x51   ) \
    X(DW_AT_entry_pc                , 0x52   ) \
    X(DW_AT_use_UTF8                , 0x53   ) \
    X(DW_AT_extension               , 0x54   ) \
    X(DW_AT_ranges                  , 0x55   ) \
    X(DW_AT_trampoline              , 0x56   ) \
    X(DW_AT_call_column             , 0x57   ) \
    X(DW_AT_call_file               , 0x58   ) \
    X(DW_AT_call_line               , 0x59   ) \
    X(DW_AT_description             , 0x5a   ) \
    X(DW_AT_binary_scale            , 0x5b   ) \
    X(DW_AT_decimal_scale           , 0x5c   ) \
    X(DW_AT_small                   , 0x5d   ) \
    X(DW_AT_decimal_sign            , 0x5e   ) \
    X(DW_AT_digit_count             , 0x5f   ) \
    X(DW_AT_picture_string          , 0x60   ) \
    X(DW_AT_mutable                 , 0x61   ) \
    X(DW_AT_threads_scaled          , 0x62   ) \
    X(DW_AT_explicit                , 0x63   ) \
    X(DW_AT_object_pointer          , 0x64   ) \
    X(DW_AT_endianity               , 0x65   ) \
    X(DW_AT_elemental               , 0x66   ) \
    X(DW_AT_pure                    , 0x67   ) \
    X(DW_AT_recursive               , 0x68   ) \
    X(DW_AT_signature               , 0x69   ) \
    X(DW_AT_main_subprogram         , 0x6a   ) \
    X(DW_AT_data_bit_offset         , 0x6b   ) \
    X(DW_AT_const_expr              , 0x6c   ) \
    X(DW_AT_enum_class              , 0x6d   ) \
    X(DW_AT_linkage_name            , 0x6e   ) \
    X(DW_AT_string_length_bit_size  , 0x6f   ) \
    X(DW_AT_string_length_byte_size , 0x70   ) \
    X(DW_AT_rank                    , 0x71   ) \
    X(DW_AT_str_offsets_base        , 0x72   ) \
    X(DW_AT_addr_base               , 0x73   ) \
    X(DW_AT_rnglists_base           , 0x74   ) \
    X(DW_AT_dwo_name                , 0x76   ) \
    X(DW_AT_reference               , 0x77   ) \
    X(DW_AT_rvalue_reference        , 0x78   ) \
    X(DW_AT_macros                  , 0x79   ) \
    X(DW_AT_call_all_calls          , 0x7a   ) \
    X(DW_AT_call_all_source_calls   , 0x7b   ) \
    X(DW_AT_call_all_tail_calls     , 0x7c   ) \
    X(DW_AT_call_return_pc          , 0x7d   ) \
    X(DW_AT_call_value              , 0x7e   ) \
    X(DW_AT_call_origin             , 0x7f   ) \
    X(DW_AT_call_parameter          , 0x80   ) \
    X(DW_AT_call_pc                 , 0x81   ) \
    X(DW_AT_call_tail_call          , 0x82   ) \
    X(DW_AT_call_target             , 0x83   ) \
    X(DW_AT_call_target_clobbered   , 0x84   ) \
    X(DW_AT_call_data_location      , 0x85   ) \
    X(DW_AT_call_data_value         , 0x86   ) \
    X(DW_AT_noreturn                , 0x87   ) \
    X(DW_AT_alignment               , 0x88   ) \
    X(DW_AT_export_symbols          , 0x89   ) \
    X(DW_AT_deleted                 , 0x8a   ) \
    X(DW_AT_defaulted               , 0x8b   ) \
    X(DW_AT_loclists_base           , 0x8c   ) \
    X(DW_AT_lo_user                 , 0x2000 ) \
    X(DW_AT_hi_user                 , 0x3fff ) \
    X(DW_AT_GNU_locviews            , 0x2137 ) \
    X(DW_AT_GNU_entry_view          , 0x2138 )

#define LIST_DWARF_FORM                \
    X(DW_FORM_nul            , 0x00  ) \
    X(DW_FORM_addr           , 0x01  ) \
    X(DW_FORM_block2         , 0x03  ) \
    X(DW_FORM_block4         , 0x04  ) \
    X(DW_FORM_data2          , 0x05  ) \
    X(DW_FORM_data4          , 0x06  ) \
    X(DW_FORM_data8          , 0x07  ) \
    X(DW_FORM_string         , 0x08  ) \
    X(DW_FORM_block          , 0x09  ) \
    X(DW_FORM_block1         , 0x0a  ) \
    X(DW_FORM_data1          , 0x0b  ) \
    X(DW_FORM_flag           , 0x0c  ) \
    X(DW_FORM_sdata          , 0x0d  ) \
    X(DW_FORM_strp           , 0x0e  ) \
    X(DW_FORM_udata          , 0x0f  ) \
    X(DW_FORM_ref_addr       , 0x10  ) \
    X(DW_FORM_ref1           , 0x11  ) \
    X(DW_FORM_ref2           , 0x12  ) \
    X(DW_FORM_ref4           , 0x13  ) \
    X(DW_FORM_ref8           , 0x14  ) \
    X(DW_FORM_ref_udata      , 0x15  ) \
    X(DW_FORM_indirect       , 0x16  ) \
    X(DW_FORM_sec_offset     , 0x17  ) \
    X(DW_FORM_exprloc        , 0x18  ) \
    X(DW_FORM_flag_present   , 0x19  ) \
    X(DW_FORM_strx           , 0x1a  ) \
    X(DW_FORM_addrx          , 0x1b  ) \
    X(DW_FORM_ref_sup4       , 0x1c  ) \
    X(DW_FORM_strp_sup       , 0x1d  ) \
    X(DW_FORM_data16         , 0x1e  ) \
    X(DW_FORM_line_strp      , 0x1f  ) \
    X(DW_FORM_ref_sig8       , 0x20  ) \
    X(DW_FORM_implicit_const , 0x21  ) \
    X(DW_FORM_loclistx       , 0x22  ) \
    X(DW_FORM_rnglistx       , 0x23  ) \
    X(DW_FORM_ref_sup8       , 0x24  ) \
    X(DW_FORM_strx1          , 0x25  ) \
    X(DW_FORM_strx2          , 0x26  ) \
    X(DW_FORM_strx3          , 0x27  ) \
    X(DW_FORM_strx4          , 0x28  ) \
    X(DW_FORM_addrx1         , 0x29  ) \
    X(DW_FORM_addrx2         , 0x2a  ) \
    X(DW_FORM_addrx3         , 0x2b  ) \
    X(DW_FORM_addrx4         , 0x2c  ) \

#define LIST_DWARF_DEBUG_SECTIONS                            \
    X(DSI_abbrevRaw     , abbrev     , ".debug_abbrev")      \
    X(DSI_addrRaw       , addr       , ".debug_addr")        \
    X(DSI_arangesRaw    , aranges    , ".debug_aranges")     \
    X(DSI_frameRaw      , frame      , ".debug_frame")       \
    X(DSI_infoRaw       , info       , ".debug_info")        \
    X(DSI_lineRaw       , line       , ".debug_line")        \
    X(DSI_line_strRaw   , line_str   , ".debug_line_str")    \
    X(DSI_locRaw        , loc        , ".debug_loc")         \
    X(DSI_loclistsRaw   , loclists   , ".debug_loclists")    \
    X(DSI_macinfoRaw    , macinfo    , ".debug_macinfo")     \
    X(DSI_macroRaw      , macro      , ".debug_macro")       \
    X(DSI_namesRaw      , names      , ".debug_names")       \
    X(DSI_pubnamesRaw   , pubnames   , ".debug_pubnames")    \
    X(DSI_pubtypesRaw   , pubtypes   , ".debug_pubtypes")    \
    X(DSI_rangesRaw     , ranges     , ".debug_ranges")      \
    X(DSI_rnglistsRaw   , rnglists   , ".debug_rnglists")    \
    X(DSI_strRaw        , str        , ".debug_str")         \
    X(DSI_str_offsetsRaw, str_offsets, ".debug_str_offsets") \
    X(DSI_supRaw        , sup        , ".debug_sup")         \
    X(DSI_typesRaw      , types      , ".debug_types")

typedef enum {
    #define X(enumName, enumVal) enumName = enumVal,
    LIST_DWARF_AT
    #undef X
} Dwarf_Attrib;

typedef enum {
    #define X(enumName, enumVal) enumName = enumVal,
    LIST_DWARF_TAG
    #undef X
} Dwarf_Tag;

typedef enum {
    #define X(enumName, enumVal) enumName = enumVal,
    LIST_DWARF_FORM
    #undef X
} Dwarf_Form;

typedef enum {
    DW_CLASS_null,
    DW_CLASS_address,
    DW_CLASS_block,
    DW_CLASS_constant,
    DW_CLASS_uconstant,
    DW_CLASS_sconstant,
    DW_CLASS_string,
    DW_CLASS_flag,
    DW_CLASS_reference,
    DW_CLASS_exprloc,
    DW_CLASS_loclist,
    DW_CLASS_rnglist,
    DW_CLASS_addrptr,
    DW_CLASS_lineptr,
    DW_CLASS_loclistsptr,
    DW_CLASS_macptr,
    DW_CLASS_rnglistsptr,
    DW_CLASS_stroffsetsptr,
} Dwarf_Class;

typedef enum {
    DW_LANG_C89            = 0x0001,
    DW_LANG_C              = 0x0002,
    DW_LANG_Ada83          = 0x0003,
    DW_LANG_C_plus_plus    = 0x0004,
    DW_LANG_Cobol74        = 0x0005,
    DW_LANG_Cobol85        = 0x0006,
    DW_LANG_Fortran77      = 0x0007,
    DW_LANG_Fortran90      = 0x0008,
    DW_LANG_Pascal83       = 0x0009,
    DW_LANG_Modula2        = 0x000a,
    DW_LANG_Java           = 0x000b,
    DW_LANG_C99            = 0x000c,
    DW_LANG_Ada95          = 0x000d,
    DW_LANG_Fortran95      = 0x000e,
    DW_LANG_PLI            = 0x000f,
    DW_LANG_ObjC           = 0x0010,
    DW_LANG_ObjC_plus_plus = 0x0011,
    DW_LANG_UPC            = 0x0012,
    DW_LANG_D              = 0x0013,
    DW_LANG_Python         = 0x0014,
    DW_LANG_OpenCL         = 0x0015,
    DW_LANG_Go             = 0x0016,
    DW_LANG_Modula3        = 0x0017,
    DW_LANG_Haskell        = 0x0018,
    DW_LANG_C_plus_plus_03 = 0x0019,
    DW_LANG_C_plus_plus_11 = 0x001a,
    DW_LANG_OCaml          = 0x001b,
    DW_LANG_Rust           = 0x001c,
    DW_LANG_C11            = 0x001d,
    DW_LANG_Swift          = 0x001e,
    DW_LANG_Julia          = 0x001f,
    DW_LANG_Dylan          = 0x0020,
    DW_LANG_C_plus_plus_14 = 0x0021,
    DW_LANG_Fortran03      = 0x0022,
    DW_LANG_Fortran08      = 0x0023,
    DW_LANG_RenderScript   = 0x0024,
    DW_LANG_BLISS          = 0x0025,
    DW_LANG_lo_user        = 0x8000,
    DW_LANG_hi_user        = 0xffff
} Dwarf_Lang;

typedef enum {
    #define X(enumName, structField, name) enumName,
    LIST_DWARF_DEBUG_SECTIONS
    #undef X
} Debug_Section_Index;

typedef struct {
    String name;
    byte *ptr;
    u32  size;
} Dwarf_Section;

typedef union {
    struct {
        #define X(enumName, structField, name) Dwarf_Section structField;
        LIST_DWARF_DEBUG_SECTIONS
        #undef X
    };
    Dwarf_Section A[21];
} Dwarf_Sections;

#if 1
Inter Dwarf_Class GetDwarfClass(u32 version, Dwarf_Attrib name, Dwarf_Form form) {
    switch (form) {
        case DW_FORM_addr:
        case DW_FORM_addrx:
        case DW_FORM_addrx1:
        case DW_FORM_addrx3:
        case DW_FORM_addrx4:
		    return DW_CLASS_address;

        case DW_FORM_block :
        case DW_FORM_block1:
        case DW_FORM_block2:
        case DW_FORM_block4: {
            if (version <= 3) {
                switch (name) {
                    case DW_AT_location:
                    case DW_AT_byte_size:
                    case DW_AT_data_bit_offset:
                    case DW_AT_bit_size:
                    case DW_AT_string_length:
                    case DW_AT_lower_bound:
                    case DW_AT_return_addr:
                    case DW_AT_bit_stride:
                    case DW_AT_upper_bound:
                    case DW_AT_count:
                    case DW_AT_data_member_location:
                    case DW_AT_frame_base:
                    case DW_AT_segment:
                    case DW_AT_static_link:
                    case DW_AT_use_location:
                    case DW_AT_vtable_elem_location:
                    case DW_AT_allocated:
                    case DW_AT_associated:
                    case DW_AT_data_location:
                    case DW_AT_byte_stride:
			            return DW_CLASS_exprloc;
                    default:
                        return DW_CLASS_block;
                }
            } else {
                return DW_CLASS_block;
	        }
	    } break;

        case DW_FORM_data4:
        case DW_FORM_data8:
	        if (version <= 3) {
                switch (name) {
                    case DW_AT_location:
                    case DW_AT_stmt_list:
                    case DW_AT_string_length:
                    case DW_AT_return_addr:
                    case DW_AT_start_scope:
                    case DW_AT_data_member_location:
                    case DW_AT_frame_base:
                    case DW_AT_macro_info:
                    case DW_AT_segment:
                    case DW_AT_static_link:
                    case DW_AT_use_location:
                    case DW_AT_vtable_elem_location:
                    case DW_AT_ranges:
                        goto sec_offset;
                }
	        } else {
                switch (name) {
                    case DW_AT_high_pc:
                        return DW_CLASS_uconstant;
                }
                return DW_CLASS_constant;
	        }

        case DW_FORM_data1:
        case DW_FORM_data2:
        case DW_FORM_data16:
            switch (name) {
                case DW_AT_language:
                case DW_AT_upper_bound: // TODO(JENH): 2.19 Static and Dynamic Values of Attributes (page 55)
                case DW_AT_const_value: // TODO(JENH): This shouldn't be here.
                case DW_AT_bit_size:
                case DW_AT_data_bit_offset:
                case DW_AT_encoding:
                case DW_AT_call_file:
                case DW_AT_call_line:
                case DW_AT_call_column:
                case DW_AT_GNU_entry_view:
                case DW_AT_data_member_location:
                case DW_AT_decimal_sign:
                case DW_AT_endianity:
                case DW_AT_alignment:
                case DW_AT_accessibility:
                case DW_AT_visibility:
                case DW_AT_virtuality:
                case DW_AT_identifier_case:
                case DW_AT_calling_convention:
                case DW_AT_inline:
                case DW_AT_ordering:
                case DW_AT_discr_list:
                case DW_AT_defaulted:
                case DW_AT_byte_size:
                case DW_AT_decl_file:
                case DW_AT_decl_line:
                case DW_AT_decl_column: {
                    return DW_CLASS_uconstant;
                }
            }
            return DW_CLASS_constant;

        case DW_FORM_udata:
            return DW_CLASS_uconstant;

        case DW_FORM_sdata:
        case DW_FORM_implicit_const:
            return DW_CLASS_sconstant;

        case DW_FORM_exprloc:
            return DW_CLASS_exprloc;

        case DW_FORM_flag:
        case DW_FORM_flag_present:
            return DW_CLASS_flag;

        case DW_FORM_ref1:
        case DW_FORM_ref2:
        case DW_FORM_ref4:
        case DW_FORM_ref8:
        case DW_FORM_ref_addr:
        case DW_FORM_ref_sig8:
        case DW_FORM_ref_udata:
	    case DW_FORM_ref_sup4:
        case DW_FORM_ref_sup8:
            return DW_CLASS_reference;

        case DW_FORM_string:
        case DW_FORM_strp:
	    case DW_FORM_strp_sup:
        case DW_FORM_line_strp:
        case DW_FORM_strx1:
        case DW_FORM_strx2:
        case DW_FORM_strx3:
        case DW_FORM_strx4:
            return DW_CLASS_string;

	    case DW_FORM_loclistx:
	        return DW_CLASS_loclist;

	    case DW_FORM_rnglistx:
	        return DW_CLASS_rnglist;

        case DW_FORM_indirect:
            return DW_CLASS_null;

	    case DW_FORM_sec_offset: {
sec_offset:
            switch (name) {
                case DW_AT_location:
                case DW_AT_string_length:
                case DW_AT_return_addr:
                case DW_AT_data_member_location:
                case DW_AT_frame_base:
                case DW_AT_segment:
                case DW_AT_static_link:
                case DW_AT_use_location:
                case DW_AT_vtable_elem_location:
                case DW_AT_GNU_locviews:
                    return DW_CLASS_loclist;

                case DW_AT_start_scope:
                case DW_AT_ranges:
                    return DW_CLASS_rnglist;

                case DW_AT_stmt_list:
                    return DW_CLASS_null; // DW_CLASS_lineptr

                case DW_AT_loclists_base:
                    return DW_CLASS_null; // DW_CLASS_loclistsptr

                case DW_AT_macro_info: // ???
                case DW_AT_macros: // DW_CLASS_macptr
                    return DW_CLASS_null;

                case DW_AT_str_offsets_base: // DW_CLASS_stroffsetsptr
                    return DW_CLASS_null;

                case DW_AT_addr_base: // DW_CLASS_addrptr
                    return DW_CLASS_null;

                case DW_AT_rnglists_base: // DW_CLASS_rnglistsptr
                    return DW_CLASS_null;

                default:
		            return DW_CLASS_null;
            }
	    } break;
    }

    return DW_CLASS_null;
}

Inter void PrintDwarfLang(Dwarf_Lang lang) {
    switch (lang) {
        case DW_LANG_C89: printf("DW_LANG_C89"); break;
        case DW_LANG_C: printf("DW_LANG_C"); break;
        case DW_LANG_Ada83: printf("DW_LANG_Ada83"); break;
        case DW_LANG_C_plus_plus: printf("DW_LANG_C_plus_plus"); break;
        case DW_LANG_Cobol74: printf("DW_LANG_Cobol74"); break;
        case DW_LANG_Cobol85: printf("DW_LANG_Cobol85"); break;
        case DW_LANG_Fortran77: printf("DW_LANG_Fortran77"); break;
        case DW_LANG_Fortran90: printf("DW_LANG_Fortran90"); break;
        case DW_LANG_Pascal83: printf("DW_LANG_Pascal83"); break;
        case DW_LANG_Modula2: printf("DW_LANG_Modula2"); break;
        case DW_LANG_Java: printf("DW_LANG_Java"); break;
        case DW_LANG_C99: printf("DW_LANG_C99"); break;
        case DW_LANG_Ada95: printf("DW_LANG_Ada95"); break;
        case DW_LANG_Fortran95: printf("DW_LANG_Fortran95"); break;
        case DW_LANG_PLI: printf("DW_LANG_PLI"); break;
        case DW_LANG_ObjC: printf("DW_LANG_ObjC"); break;
        case DW_LANG_ObjC_plus_plus: printf("DW_LANG_ObjC_plus_plus"); break;
        case DW_LANG_UPC: printf("DW_LANG_UPC"); break;
        case DW_LANG_D: printf("DW_LANG_D"); break;
        case DW_LANG_Python: printf("DW_LANG_Python"); break;
        case DW_LANG_OpenCL: printf("DW_LANG_OpenCL"); break;
        case DW_LANG_Go: printf("DW_LANG_Go"); break;
        case DW_LANG_Modula3: printf("DW_LANG_Modula3"); break;
        case DW_LANG_Haskell: printf("DW_LANG_Haskell"); break;
        case DW_LANG_C_plus_plus_03: printf("DW_LANG_C_plus_plus_03"); break;
        case DW_LANG_C_plus_plus_11: printf("DW_LANG_C_plus_plus_11"); break;
        case DW_LANG_OCaml: printf("DW_LANG_OCaml"); break;
        case DW_LANG_Rust: printf("DW_LANG_Rust"); break;
        case DW_LANG_C11: printf("DW_LANG_C11"); break;
        case DW_LANG_Swift: printf("DW_LANG_Swift"); break;
        case DW_LANG_Julia: printf("DW_LANG_Julia"); break;
        case DW_LANG_Dylan: printf("DW_LANG_Dylan"); break;
        case DW_LANG_C_plus_plus_14: printf("DW_LANG_C_plus_plus_14"); break;
        case DW_LANG_Fortran03: printf("DW_LANG_Fortran03"); break;
        case DW_LANG_Fortran08: printf("DW_LANG_Fortran08"); break;
        case DW_LANG_RenderScript: printf("DW_LANG_RenderScript"); break;
        case DW_LANG_BLISS: printf("DW_LANG_BLISS"); break;
        case DW_LANG_lo_user: printf("DW_LANG_lo_user"); break;
        case DW_LANG_hi_user: printf("DW_LANG_hi_user"); break;

        default: {
            printf("Languge does not exist.\n");
        } break;
    }
}

Inter void PrintClass(Dwarf_Class dwarfClass, CString format) {
    switch (dwarfClass) {
        case DW_CLASS_null: { printf(format, "DW_CLASS_null"); } break;
        case DW_CLASS_address: { printf(format, "DW_CLASS_address"); } break;
        case DW_CLASS_block: { printf(format, "DW_CLASS_block"); } break;
        case DW_CLASS_constant: { printf(format, "DW_CLASS_constant"); } break;
        case DW_CLASS_uconstant: { printf(format, "DW_CLASS_uconstant"); } break;
        case DW_CLASS_sconstant: { printf(format, "DW_CLASS_sconstant"); } break;
        case DW_CLASS_string: { printf(format, "DW_CLASS_string"); } break;
        case DW_CLASS_flag: { printf(format, "DW_CLASS_flag"); } break;
        case DW_CLASS_reference: { printf(format, "DW_CLASS_reference"); } break;
        case DW_CLASS_exprloc: { printf(format, "DW_CLASS_exprloc"); } break;
        case DW_CLASS_loclist: { printf(format, "DW_CLASS_loclist"); } break;
        case DW_CLASS_rnglist: { printf(format, "DW_CLASS_rnglist"); } break;
        case DW_CLASS_addrptr: { printf(format, "DW_CLASS_addrptr"); } break;
        case DW_CLASS_lineptr: { printf(format, "DW_CLASS_lineptr"); } break;
        case DW_CLASS_loclistsptr: { printf(format, "DW_CLASS_loclistsptr"); } break;
        case DW_CLASS_macptr: { printf(format, "DW_CLASS_macptr"); } break;
        case DW_CLASS_rnglistsptr: { printf(format, "DW_CLASS_rnglistsptr"); } break;
        case DW_CLASS_stroffsetsptr: { printf(format, "DW_CLASS_stroffsetsptr"); } break;
    }
}

#endif //GetDwarfClass

#endif //DSV_DWARFDEF_H
