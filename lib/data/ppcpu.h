/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#pragma once
#include "dxrt/common.h"
#include <cstdint>

namespace dxrt {

typedef struct
{
    char fw_ver[16];
    uint32_t header_crc;
    uint32_t image_crc;
    uint32_t image_size;
    uint32_t reserved[9];
} dx_ppcpu_image_header_t;

class PPCPUDataLoader
{
 public:
    static uint8_t *GetData(size_t &size);
    static uint8_t *GetData();
    static int GetDataSize();
    static dx_ppcpu_image_header_t GetHeader();
    static void PrintHeader();
};

}  // namespace dxrt
