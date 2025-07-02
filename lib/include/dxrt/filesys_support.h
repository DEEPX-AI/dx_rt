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