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

#include <exception>
#include <string>

namespace dxrt {

enum ERROR_CODE {  // NOSONAR:S3642
    DEFAULT = 0x0100,
    FILE_NOT_FOUND,
    NULL_POINTER,
    FILE_IO,
    INVALID_ARGUMENT,
    INVALID_OPERATION,
    INVALID_MODEL,
    MODEL_PARSING,
    SERVICE_IO,
    DEVICE_IO
};

#define EXCEPTION_MESSAGE(msg) ("\"" + std::string(msg) + "\":" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + ":" + __func__)

class DXRT_API Exception  //: public std::exception
{

    std::string _message = "";
    ERROR_CODE _errorCode = ERROR_CODE::DEFAULT;

 protected:
    Exception() = default;
    virtual ~Exception() = default;

 public:
    Exception(const std::string& msg, ERROR_CODE code);

    virtual const char* what() const noexcept {
        return _message.c_str();
    }

    ERROR_CODE code() const noexcept {
        return _errorCode;
    }

    void setMessage(const std::string& msg);

    void setCode(ERROR_CODE code);

    void printTrace() const;

    std::string codeStr() const noexcept;
};

class DXRT_API FileNotFoundException : public Exception
{
public:
    explicit FileNotFoundException(const std::string& msg = "");
    ~FileNotFoundException() override = default;

};

class DXRT_API NullPointerException : public Exception
{
 public:
    explicit NullPointerException(const std::string& msg = "");
    ~NullPointerException() override = default;

};

class DXRT_API FileIOException : public Exception
{
 public:
    explicit FileIOException(const std::string& msg = "");
    ~FileIOException() override = default;

};

class DXRT_API InvalidArgumentException : public Exception
{
 public:
    explicit InvalidArgumentException(const std::string& msg = "");
    ~InvalidArgumentException() override = default;

};

class DXRT_API InvalidOperationException : public Exception
{
 public:
    explicit InvalidOperationException(const std::string& msg = "");
    ~InvalidOperationException() override = default;

};

class DXRT_API InvalidModelException : public Exception
{
 public:
    explicit InvalidModelException(const std::string& msg = "");
    ~InvalidModelException() override = default;

};

class DXRT_API ModelParsingException : public Exception
{
 public:
    explicit ModelParsingException(const std::string& msg = "");
    ~ModelParsingException() override = default;

};


class DXRT_API ServiceIOException : public Exception
{
 public:
    explicit ServiceIOException(const std::string& msg = "");
    ~ServiceIOException() override = default;

};

class DXRT_API DeviceIOException : public Exception
{
 public:
    explicit DeviceIOException(const std::string& msg = "");
    ~DeviceIOException() override = default;

};


}  // namespace dxrt
