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

enum class ModelType : int8_t
{
    MODEL_TYPE_NORMAL = 0,
    MODEL_TYPE_ARGMAX = 1,
    MODEL_TYPE_PPU = 2,
    MODEL_TYPE_PPCPU = 3,
};


} // namespace dxrt
