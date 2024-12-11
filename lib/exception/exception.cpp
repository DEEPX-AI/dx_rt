
#include "../include/dxrt/exception/exception.h"

#include <iostream>
#include <execinfo.h>
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
        void* buffer[100];
        int nptrs = backtrace(buffer, 100);
        char** symbols = backtrace_symbols(buffer, nptrs);
        for (int i = 0; i < nptrs; ++i) {
            //std::cout << symbols[i] << std::endl;
            LOG_DXRT_ERR(symbols[i]);
        }
        free(symbols);
    }

    

       
    FileNotFoundException::FileNotFoundException(const std::string& msg)
    {
        setMessage("File not found execption {" + msg + "}");
        setCode(ERROR_CODE::FILE_NOT_FOUND);
    }

    /*FileNotFoundException::FileNotFoundException(const std::string& className, const std::string& funcName, const std::string& msg)
    {
        setMessage("File not found exception {" + className + ":" + funcName + ":" + msg + "}");
        setCode(ERROR_CODE::FILE_NOT_FOUND);
    }*/

    NullPointerException::NullPointerException(const std::string& msg)
    {
        setMessage("Null pointer execption {" + msg + "}");
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

    ModelPasrsingException::ModelPasrsingException(const std::string& msg)
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