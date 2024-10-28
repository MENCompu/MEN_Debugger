#ifndef DSV_RANGE_LIST_H
#define DSV_RANGE_LIST_H

typedef enum {
    DW_RLE_end_of_list   = 0x00,
    DW_RLE_base_addressx = 0x01,
    DW_RLE_startx_endx   = 0x02,
    DW_RLE_startx_length = 0x03,
    DW_RLE_offset_pair   = 0x04,
    DW_RLE_base_address  = 0x05,
    DW_RLE_start_end     = 0x06,
    DW_RLE_start_length  = 0x07
} Range_List_Name;

Inter void ParseRangeListHeader(Dwarf_Range_List_Header *rnglistHeader, byte *rnglistData) {
    byte *scanData = rnglistData;
    rnglistHeader->secBaseAddress = rnglistData;

    if (((u64)*scanData & 0xFFFFFFFF) == 0xFFFFFFFF) {
        rnglistHeader->format = DWARF_64_BITS;
        rnglistHeader->unitLength = (*(u64 *)scanData) >> 16;
        scanData += 8;
    } else {
        rnglistHeader->format = DWARF_32_BITS;
        rnglistHeader->unitLength = *(u32 *)scanData;
        scanData += 4;
    }

    rnglistHeader->version = *(u16 *)scanData;
    scanData += sizeof(u16);

    rnglistHeader->addressSize = *(u8 *)scanData;
    scanData += sizeof(u8);

    rnglistHeader->segmentSelectorSize = *(u8 *)scanData;
    scanData += sizeof(u8);

    if (rnglistHeader->format == DWARF_32_BITS) {
        rnglistHeader->offsets32.size = *(u32 *)scanData;
        scanData += sizeof(u32);
        rnglistHeader->offsets32.A = (u32 *)scanData;
        scanData += rnglistHeader->offsets32.size * sizeof(u32);
        rnglistHeader->rangeListsAddress.size = rnglistHeader->unitLength - (scanData - (rnglistData + 4));
    } else { // format == DWARF_64_BITS
        rnglistHeader->offsets64.size = *(u32 *)scanData;
        scanData += sizeof(u32);
        rnglistHeader->offsets64.A = (u64 *)scanData;
        scanData += rnglistHeader->offsets64.size * sizeof(u64);
        rnglistHeader->rangeListsAddress.size = rnglistHeader->unitLength - (scanData - (rnglistData + 8));
    }

    rnglistHeader->rangeListsAddress.A = scanData;
}

Inter b32 IsInRangeRnglist(Dwarf_Range_List_Header *rnglistHeader, byte *rnglist, u64 defaultBaseAddress, u64 givenAddress) {
    byte *scan = rnglist;
    u64 baseAddress = defaultBaseAddress;

    while (INFINITE_LOOP) {
        Range_List_Name type = *(u8 *)scan;
        scan += sizeof(u8);

        u64 lowAddress  = 0;
        u64 highAddress = 0;

        switch (type) {
            case DW_RLE_end_of_list: {
                return false;
            } break;

            case DW_RLE_base_addressx: {
                u64 baseIndex = ULEB128AndAdvance(&scan);
                u64 base = baseIndex; // .debug_addr
            } break;

            case DW_RLE_startx_endx: {
                u64 startIndex = ULEB128AndAdvance(&scan);
                u64 start = startIndex; // .debug_addr

                u64 endIndex   = ULEB128AndAdvance(&scan);
                u64 end = endIndex; // .debug_addr
            } break;

            case DW_RLE_startx_length: {
                u64 startIndex = ULEB128AndAdvance(&scan);
                u64 start = startIndex; // .debug_addr

                u64 length = ULEB128AndAdvance(&scan);
            } break;

            case DW_RLE_offset_pair: {
                u64 lowOffset  = ULEB128AndAdvance(&scan);
                lowAddress  = baseAddress + lowOffset;

                u64 highOffset = ULEB128AndAdvance(&scan);
                highAddress = baseAddress + highOffset;
            } break;

            case DW_RLE_base_address: {
                baseAddress = AddressByAddressSize(scan, rnglistHeader->addressSize);
                scan += rnglistHeader->addressSize;
            } break;

            case DW_RLE_start_end: {
                lowAddress  = AddressByAddressSize(scan, rnglistHeader->addressSize);
                scan += rnglistHeader->addressSize;

                highAddress = AddressByAddressSize(scan, rnglistHeader->addressSize);
                scan += rnglistHeader->addressSize;
            } break;

            case DW_RLE_start_length: {
                lowAddress  = AddressByAddressSize(scan, rnglistHeader->addressSize);
                scan += rnglistHeader->addressSize;

                u64 length  = ULEB128AndAdvance(&scan);
                highAddress = lowAddress + length;
            } break;

            default: {
                printf("default\n");
            } break;
        }

        //printf("%llx <= %llx <= %llx.\n", lowAddress, givenAddress, highAddress);
        if (lowAddress <= givenAddress && givenAddress <= highAddress) {
            return true;
        }
    }

    return false;
}

Inter void PrintRangeListEntry(Dwarf_Range_List_Header *rnglistHeader, byte *rnglist, u64 defaultBaseAddress) {
    byte *scan = rnglist;
    u64 baseAddress = defaultBaseAddress;

    //for (byte *scan = rnglistHeader->rangeListsAddress.A;
    //     (scan - rnglistHeader->rangeListsAddress.A) < rnglistHeader->rangeListsAddress.size;) {
    while (INFINITE_LOOP) {
        Range_List_Name type = *(u8 *)scan;
        scan += sizeof(u8);

        printf("    ");

        switch (type) {
            case DW_RLE_end_of_list: {
                printf("<End of List>\n");
                return;
            } break;

            case DW_RLE_base_addressx: {
                //printf("DW_RLE_base_addressx\n");

                u64 baseIndex = ULEB128AndAdvance(&scan);
                u64 base = baseIndex; // .debug_addr
                printf("0x%016llx", base);
            } break;

            case DW_RLE_startx_endx: {
                //printf("DW_RLE_startx_endx\n");

                printf("hola\n");
                u64 startIndex = ULEB128AndAdvance(&scan);
                u64 start = startIndex; // .debug_addr
                printf("0x%016llx  ", start);

                u64 endIndex   = ULEB128AndAdvance(&scan);
                u64 end = endIndex; // .debug_addr
                printf("0x%016llx", end);
            } break;

            case DW_RLE_startx_length: {
                //printf("DW_RLE_startx_length\n");

                u64 startIndex = ULEB128AndAdvance(&scan);
                u64 start = startIndex; // .debug_addr
                printf("0x%016llx  ", start);

                u64 length = ULEB128AndAdvance(&scan);
                printf("0x%016llx", start + length);
            } break;

            case DW_RLE_offset_pair: {
                //printf("DW_RLE_offset_pair\n");

                //printf("hola\n");
                u64 startOffset = ULEB128AndAdvance(&scan);
                printf("0x%016llx  ", baseAddress + startOffset);

                u64 endOffset   = ULEB128AndAdvance(&scan);
                printf("0x%016llx", baseAddress + endOffset);
            } break;

            case DW_RLE_base_address: {
                //printf("DW_RLE_base_address\n");

                baseAddress = AddressByAddressSize(scan, rnglistHeader->addressSize);
                scan += rnglistHeader->addressSize;
                printf("0x%016llx", baseAddress);
                printf("  (base address)");
            } break;

            case DW_RLE_start_end: {
                //printf("DW_RLE_start_end\n");

                u64 start = AddressByAddressSize(scan, rnglistHeader->addressSize);
                scan += rnglistHeader->addressSize;
                printf("0x%016llx  ", start);

                u64 end   = AddressByAddressSize(scan, rnglistHeader->addressSize);
                scan += rnglistHeader->addressSize;
                printf("0x%016llx", end);
            } break;

            case DW_RLE_start_length: {
                //printf("DW_RLE_start_length\n");

                u64 address = AddressByAddressSize(scan, rnglistHeader->addressSize);
                scan += rnglistHeader->addressSize;
                printf("0x%016llx  ", address);

                u64 length = ULEB128AndAdvance(&scan);
                printf("0x%016llx", address + length);
            } break;

            default: {
                printf("default\n");
            } break;
        }

        printf("\n");
    }
}

#endif //DSV_RANGE_LIST_H
