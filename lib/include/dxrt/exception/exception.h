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

    enum ERROR_CODE {
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

    class DXRT_API Exception //: public std::exception
    {    

        std::string _message = "";
        ERROR_CODE _errorCode = ERROR_CODE::DEFAULT;

    protected:
        Exception() {}

    public:
        Exception(const std::string& msg, ERROR_CODE code);

        //virtual const char* what() const noexcept override {
        //    return _message.c_str();
        //}
        virtual const char* what() const noexcept {
            return _message.c_str();
        }

        ERROR_CODE code() const noexcept {
            return _errorCode;
        }

        void setMessage(const std::string& msg);

        void setCode(ERROR_CODE code);

        void printTrace();

    };

    class DXRT_API FileNotFoundException : public Exception
    {
    public:
        FileNotFoundException(const std::string& msg = "");
        //FileNotFoundException(const std::string& msg, const std::string& funcName, const std::string& className);

    };

    class DXRT_API NullPointerException : public Exception
    {
    public:
        NullPointerException(const std::string& msg = "");

    };

    class DXRT_API FileIOException : public Exception
    {
    public:
        FileIOException(const std::string& msg = "");

    };

    class DXRT_API InvalidArgumentException : public Exception
    {
    public:
        InvalidArgumentException(const std::string& msg = "");

    };

    class DXRT_API InvalidOperationException : public Exception
    {
    public:
        InvalidOperationException(const std::string& msg = "");

    };

    class DXRT_API InvalidModelException : public Exception
    {
    public:
        InvalidModelException(const std::string& msg = "");

    };

    class DXRT_API ModelParsingException : public Exception
    {
    public:
        ModelParsingException(const std::string& msg = "");

    };
    

    class DXRT_API ServiceIOException : public Exception
    {
    public:
        ServiceIOException(const std::string& msg = "");

    };

    class DXRT_API DeviceIOException : public Exception
    {
    public:
        DeviceIOException(const std::string& msg = "");

    };


}
