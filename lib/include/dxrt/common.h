// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ifdef _WIN32
    #ifdef DXRT_STATIC
        #define DXRT_API
    #else
        #ifdef DXRT_EXPORTS
            #define DXRT_API __declspec(dllexport)
        #else
            #define DXRT_API __declspec(dllimport)
        #endif
    #endif
#else
    #define DXRT_API
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <queue>
#ifdef __linux__
    #include <dirent.h>
#endif
#include <cmath>
#ifdef __linux__
    #include <unistd.h>
#endif
#include <memory>
#include <functional>
#include <assert.h>
#include <numeric>
#include "dxrt/gen.h"

#if DEBUG_DXRT
#define LOG_DBG(x) std::cout<<"[DXRT] "<< x << std::endl;
#else
#define LOG_DBG(x)
#endif
#define LOG_DXRT     std::cout<<"[DXRT]["<< __func__ << "] "
#define LOG_DXRT_DBG if(DEBUG_DXRT) std::cout<<"[DXRT]["<< __func__ << "] " 

#define LOG_DXRT_ERR(x) std::cout<<"[DXRT][Error] "<< x << std::endl;
#define DXRT_STR(a) __DXRT_STR(a)
#define __DXRT_STR(a) #a
#define QUOTE_VALUE(val) DXRT_STR(val) << ": " << val
#define LOG_VALUE(val) std::cout << DXRT_STR(val) << ": " << val << std::endl;
#define LOG_VALUE_HEX(val) std::cout << DXRT_STR(val) << ": " << std::showbase << std::hex << val << std::dec << std::endl;

#ifdef NDEBUG
#define DXRT_ASSERT(cond, msg) do {\
        if(!(cond)) {LOG_DXRT_ERR(msg);\
        std::abort();}\
    }while(0);
#else
#define DXRT_ASSERT(cond, msg) do {\
        if(!(cond)) {LOG_DXRT_ERR(msg);\
        assert((cond));}\
    }while(0);
#endif

#ifdef __aarch64__
#define IOMEM_BARRIER() asm volatile("isb");asm volatile("dsb sy");
#else
#define IOMEM_BARRIER() 
#endif

// #define RMAPINFO_ALIAS (using rmapinfo = deepx_rmapinfo::RegisterInfoDatabase)

namespace dxrt {
/** @brief Processors 
 * @headerfile "dxrt/dxrt_api.h"
*/
enum DXRT_API Processor
{
    NPU, ///< Neural Processing Unit(dxnn)
    CPU, ///< Central Processing Unit(ONNX)
    NONE_PROCESSOR,
};
DXRT_API std::ostream& operator<<(std::ostream&, const Processor&);

/* \brief Inference modes 
 * \headerfile "dxrt/dxrt_api.h"
*/
enum DXRT_API InferenceMode
{
    SYNC, // Synchronous inference (wait for device response after inference request)
    ASYNC,// Asynchronous inference (don't wait for device response after inference request)
};
/** @brief CPU cache flush API (Platform-dependent)
 * @headerfile "dxrt/dxrt_api.h"
*/
DXRT_API int CleanMemIf();

DXRT_API std::string DeviceVariant();

template <typename T>
T vectorProduct(const std::vector<T>& v)
{
    return accumulate(v.begin(), v.end(), 1, std::multiplies<T>());
}
} // namespace dxrt