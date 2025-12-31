
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

    void Exception::printTrace()
    {
#ifdef __linux__
        void* buffer[100];
        int nptrs = backtrace(buffer, 100);
        char** symbols = backtrace_symbols(buffer, nptrs);
        for (int i = 0; i < nptrs; ++i) {
            //std::cout << symbols[i] << std::endl;
            LOG_DXRT_ERR(symbols[i]);
        }
        free(symbols);
#else
        // not implemented        
#endif
    }

    

       
    FileNotFoundException::FileNotFoundException(const std::string& msg)
    {
        setMessage("File not found exception {" + msg + "}");
        setCode(ERROR_CODE::FILE_NOT_FOUND);
    }

    /*FileNotFoundException::FileNotFoundException(const std::string& className, const std::string& funcName, const std::string& msg)
    {
        setMessage("File not found exception {" + className + ":" + funcName + ":" + msg + "}");
        setCode(ERROR_CODE::FILE_NOT_FOUND);
    }*/

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


} // namespace dxrt