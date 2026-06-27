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

namespace dxrt {

enum class MemoryType : int32_t
{
    Normal = 0,
    Model_rmap = 1,
    Model_weight = 2,
    Input_output = 3,
    Model_ppu_binary = 4,
};

}  // namespace dxrt
