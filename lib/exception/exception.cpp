
/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "../include/dxrt/exception/exception.h"

#include <iostream>
#ifdef __linux__
#include <execinfo.h>
#endif
#include <cstdlib>
#include <array>

namespace dxrt {

Exception::Exception(const std::string& msg, ERROR_CODE code)
{
    setMessage(msg);
    setCode(code);
}

void Exception::setMessage(const std::string& msg)
{
    _message = "[dxrt-exception] " + msg;
}

void Exception::setCode(ERROR_CODE code)
{
    _errorCode = code;
}

void Exception::printTrace() const
{
#ifdef __linux__
    std::array<void*, 100> buffer;
    int nptrs = backtrace(buffer.data(), 100);
    char** symbols = backtrace_symbols(buffer.data(), nptrs);
    for (int i = 0; i < nptrs; ++i) {
        LOG_DXRT_ERR(symbols[i]);
    }
    free(symbols);
#else
    // not implemented
#endif
}

std::string Exception::codeStr() const noexcept {
    switch (_errorCode) {
        case ERROR_CODE::DEFAULT:
            return "DEFAULT";
        case ERROR_CODE::FILE_NOT_FOUND:
            return "FILE_NOT_FOUND";
        case ERROR_CODE::NULL_POINTER:
            return "NULL_POINTER";
        case ERROR_CODE::FILE_IO:
            return "FILE_IO";
        case ERROR_CODE::INVALID_ARGUMENT:
            return "INVALID_ARGUMENT";
        case ERROR_CODE::INVALID_OPERATION:
            return "INVALID_OPERATION";
        case ERROR_CODE::INVALID_MODEL:
            return "INVALID_MODEL";
        case ERROR_CODE::MODEL_PARSING:
            return "MODEL_PARSING";
        case ERROR_CODE::SERVICE_IO:
            return "SERVICE_IO";
        case ERROR_CODE::DEVICE_IO:
            return "DEVICE_IO";
        default:
            return std::to_string(static_cast<int>(_errorCode));
    }
}


FileNotFoundException::FileNotFoundException(const std::string& msg)
{
    setMessage("File not found exception {" + msg + "}");
    setCode(ERROR_CODE::FILE_NOT_FOUND);
}

NullPointerException::NullPointerException(const std::string& msg)
{
    setMessage("Null pointer exception {" + msg + "}");
    setCode(ERROR_CODE::NULL_POINTER);
}

FileIOException::FileIOException(const std::string& msg)
{
    setMessage("File input or output exception {" + msg + "}");
    setCode(ERROR_CODE::FILE_IO);
}

InvalidArgumentException::InvalidArgumentException(const std::string& msg)
{
    setMessage("Invalid argument exception {" + msg + "}");
    setCode(ERROR_CODE::INVALID_ARGUMENT);
}

InvalidOperationException::InvalidOperationException(const std::string& msg)
{
    setMessage("Invalid operation exception {" + msg + "}");
    setCode(ERROR_CODE::INVALID_OPERATION);
}

InvalidModelException::InvalidModelException(const std::string& msg)
{
    setMessage("Invalid model exception {" + msg + "}");
    setCode(ERROR_CODE::INVALID_MODEL);
}

ModelParsingException::ModelParsingException(const std::string& msg)
{
    setMessage("Model parsing exception {" + msg + "}");
    setCode(ERROR_CODE::MODEL_PARSING);
}

ServiceIOException::ServiceIOException(const std::string& msg)
{
    setMessage("Service input & output exception {" + msg + "}");
    setCode(ERROR_CODE::SERVICE_IO);
}

DeviceIOException::DeviceIOException(const std::string& msg)
{
    setMessage("Device input & output exception {" + msg + "}");
    setCode(ERROR_CODE::DEVICE_IO);
}


}  // namespace dxrt
