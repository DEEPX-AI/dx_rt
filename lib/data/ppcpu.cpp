/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ppcpu.h"
#include "dxrt/common.h"
#include <iostream>


extern unsigned char ppcpu_bin[];
extern unsigned int ppcpu_bin_len;

namespace dxrt {

uint8_t *PPCPUDataLoader::GetData(size_t &size)
{
    size = static_cast<size_t>(ppcpu_bin_len);
    return GetData();
}

uint8_t *PPCPUDataLoader::GetData()
{
    return ppcpu_bin;
}

int PPCPUDataLoader::GetDataSize()
{
    return static_cast<int>(ppcpu_bin_len);
}

dx_ppcpu_image_header_t PPCPUDataLoader::GetHeader()
{
    dx_ppcpu_image_header_t header;
    dx_ppcpu_image_header_t* ptr = static_cast<dx_ppcpu_image_header_t*>(static_cast<void*>(ppcpu_bin));
    header = *ptr;
    return header;
}
void PPCPUDataLoader::PrintHeader()
{
    dx_ppcpu_image_header_t header = GetHeader();
    std::cout << "PPCPU Firmware Header:" << std::endl;
    std::cout << "  Firmware Version: " << header.fw_ver << std::endl;
    std::cout << std::endl;
    std::cout << "  Header CRC: 0x" << std::hex << header.header_crc << std::dec << std::endl;
    std::cout << "  Image CRC: 0x" << std::hex << header.image_crc << std::dec << std::endl;
    std::cout << "  Image Size: " << header.image_size << " bytes" << std::endl;
}

}  // namespace dxrt
