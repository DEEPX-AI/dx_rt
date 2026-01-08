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

bool DXRT_API isDxrtServiceRunning();

// Add these declarations for Windows
#ifdef _WIN32
HANDLE DXRT_API createServiceMutex();
void DXRT_API releaseServiceMutex(HANDLE hMutex);
#endif

}  // namespace dxrt
