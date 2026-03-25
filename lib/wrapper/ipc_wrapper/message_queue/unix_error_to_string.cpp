/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include <cstdio>
#include <errno.h>
#include <cstring>
#include <string>
#include <array>
#include "dxrt/common.h"

#if __linux__
DXRT_API std::string getErrorString(int error_code)
{
    std::array<char, 256> buffer;
    memset(buffer.data(), 0, buffer.size());
    std::string error = "Error no " + std::to_string(error_code);
    const char* str = strerror_r(error_code, buffer.data(), buffer.size());
    if (str != nullptr)
    {
        error += "(";
        error += std::string(str);
        error += ")";
    }
    else
    {
        error += "(strerror_r notfound "+ std::to_string(errno)+")";
    }
    return error;
}
#else  // _WIN32
#include <windows.h>
DXRT_API std::string getErrorString(int error_code)
{
    LPVOID msgBuffer;
    std::string error = "Error no " + std::to_string(error_code);

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&msgBuffer,
        0, NULL);

    if (msgBuffer != NULL)
    {
        error += " (";
        error += std::string(static_cast<char*>(msgBuffer));
        error += ")";
        LocalFree(msgBuffer);
    }
    else
    {
        error += "(FormatMessage failed "+ std::to_string(GetLastError())+")";
    }
    return error;
}
#endif

DXRT_API std::string getString()
{
    return getErrorString(errno);
}
