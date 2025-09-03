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
#include<string>

namespace dxrt{
    DXRT_API std::string getPath(const std::string& path);
    DXRT_API std::string getCurrentPath();
    DXRT_API std::string getAbsolutePath(const std::string& path);
    DXRT_API std::string getParentPath(const std::string& path);
    DXRT_API int getFileSize(const std::string& filename);
    DXRT_API bool fileExists(const std::string& path);
    DXRT_API std::string getExtension(const std::string& path);
}