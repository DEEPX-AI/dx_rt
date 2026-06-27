/*
 * Backward-compatibility shim — Exception classes for SDK users.
 * Prefer #include "dxrt/dxrt_api.h" and catch dxrt::Exception for new code.
 */
#pragma once

/* ── ODR guard ───────────────────────────────────────────────────
 * See dxrt_cxx_api.h / wrapper/inference_engine.h — this header
 * also declares dxrt::Exception, and must not be mixed with the
 * cxx_api Exception in the same translation unit.
 */
#ifndef DXRT_WRAPPER_HEADERS_INCLUDED
# define DXRT_WRAPPER_HEADERS_INCLUDED
#endif
#ifdef DXRT_CXX_API_H_INCLUDED
# error "dxrt/wrapper/*.h and dxrt_cxx_api.h cannot be included in the same translation unit (two dxrt::Exception classes — ODR violation risk). Use one or the other."
#endif

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

class Exception
{
    std::string _message;
    ERROR_CODE _errorCode = ERROR_CODE::DEFAULT;

public:
    Exception() = default;
    virtual ~Exception() = default;

    Exception(const std::string& msg, ERROR_CODE code)
        : _message(msg), _errorCode(code) {}

    explicit Exception(const std::string& msg)
        : _message(msg), _errorCode(DEFAULT) {}

    virtual const char* what() const noexcept {
        return _message.c_str();
    }

    ERROR_CODE code() const noexcept {
        return _errorCode;
    }
};

class FileNotFoundException : public Exception
{
public:
    explicit FileNotFoundException(const std::string& msg = "")
        : Exception(msg, FILE_NOT_FOUND) {}
};

class NullPointerException : public Exception
{
public:
    explicit NullPointerException(const std::string& msg = "")
        : Exception(msg, NULL_POINTER) {}
};

class FileIOException : public Exception
{
public:
    explicit FileIOException(const std::string& msg = "")
        : Exception(msg, FILE_IO) {}
};

class InvalidArgumentException : public Exception
{
public:
    explicit InvalidArgumentException(const std::string& msg = "")
        : Exception(msg, INVALID_ARGUMENT) {}
};

class InvalidOperationException : public Exception
{
public:
    explicit InvalidOperationException(const std::string& msg = "")
        : Exception(msg, INVALID_OPERATION) {}
};

class InvalidModelException : public Exception
{
public:
    explicit InvalidModelException(const std::string& msg = "")
        : Exception(msg, INVALID_MODEL) {}
};

class ModelParsingException : public Exception
{
public:
    explicit ModelParsingException(const std::string& msg = "")
        : Exception(msg, MODEL_PARSING) {}
};

class ServiceIOException : public Exception
{
public:
    explicit ServiceIOException(const std::string& msg = "")
        : Exception(msg, SERVICE_IO) {}
};

class DeviceIOException : public Exception
{
public:
    explicit DeviceIOException(const std::string& msg = "")
        : Exception(msg, DEVICE_IO) {}
};

}  // namespace dxrt
