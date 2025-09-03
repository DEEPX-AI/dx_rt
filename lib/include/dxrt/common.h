/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#ifdef _WIN32
    #include <windows.h>
	typedef int pid_t;
	#if 0
	void usleep(int us)
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = us; // micro seconds
		select(0, NULL, NULL, NULL, &tv); // sleep 10 us
	}
	#endif
	// usleep(us) this_thread::sleep_for(chrono::microseconds(us));
	#pragma warning(disable : 4244)
	#pragma warning(disable : 4251)
	#pragma warning(disable : 4267)
	#pragma warning(disable : 4312)
	#pragma warning(disable : 4819)
	#pragma warning(disable : 4996)
	#pragma warning(disable : 5208)
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
#include <cstdlib> 
#include "dxrt/gen.h"
#include <mutex>
#if __cplusplus >= 201402L
#include <shared_mutex>
#endif

#ifndef DEBUG_DXRT
#define DEBUG_DXRT 0
#endif

#define DXRT_TASK_MAX_LOAD_DEFAULT    (6)

#if DEBUG_DXRT
#define LOG_DBG(x) std::cout<<"[DXRT] "<< x << std::endl;
#else
#define LOG_DBG(x)
#endif

#if SHOW_TASK_FLOW
#define TASK_FLOW(x) std::cout<<"[TASK_FLOW] "<< x <<std::endl;
#define TASK_FLOW_START(x) std::cout<<"[TASK_FLOW] "<< x << " START" <<std::endl;
#define TASK_FLOW_FINISH(x) std::cout<<"[TASK_FLOW] "<< x << " FINISH" <<std::endl;
#else
#define TASK_FLOW(x)
#define TASK_FLOW_START(x)
#define TASK_FLOW_FINISH(x)
#endif

#define LOG     std::cout<<"[DXRT] "
#define LOG_DXRT     std::cout<<"[DXRT]["<< __func__ << "] "
#define LOG_DXRT_DBG if(DEBUG_DXRT) std::cout<<"[DXRT]["<< __func__ << "] " 
#define LOG_DXRT_ERR(x) std::cout<<"[DXRT][Error] "<< x << std::endl;
#define LOG_DXRT_WARN(x) std::cout<<"[DXRT][Warning] "<< x << std::endl;
#define LOG_DXRT_INFO(x) std::cout<<"[DXRT][Info] "<< x << std::endl;

#define LOG_DXRT_S        std::cout<<"[DXRT_SVC]["<< __func__ << "] "
#define LOG_DXRT_S_DBG    if(DEBUG_DXRT) std::cout<<"[DXRT_SVC]["<< __func__ << "] " 
#define LOG_DXRT_S_ERR(x) std::cout<<"[DXRT_SVC][Error] "<< x << std::endl;

#define LOG_DXRT_I        std::cout<<"[DXRT_IPC]["<< __func__ << "] "
#define LOG_DXRT_I_DBG    if(DEBUG_DXRT) std::cout<<"[DXRT_IPC]["<< __func__ << "] " 
#define LOG_DXRT_I_ERR(x) std::cout<<"[DXRT_IPC][Error] "<< x << std::endl;

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

#if __cplusplus >= 201402L // C++14 or later
    using SharedMutex = std::shared_timed_mutex;
    using SharedLock = std::shared_lock<std::shared_timed_mutex>;
    using UniqueLock = std::unique_lock<std::shared_timed_mutex>;
#else // C++11
    using SharedMutex = std::mutex; // Use std::mutex as a substitute
    using SharedLock = std::unique_lock<std::mutex>; // Substitute with a write lock
    using UniqueLock = std::unique_lock<std::mutex>;
#endif


inline int GetTaskMaxLoad() {
    static int cached_value = -1;
    if (cached_value == -1) {
        const char* env_value = std::getenv("DXRT_TASK_MAX_LOAD");
        if (env_value != nullptr) {
            int env_int = std::atoi(env_value);
            if (env_int > 0 && env_int <= 100) { 
                cached_value = env_int;
                std::cout << "[DXRT] Using DXRT_TASK_MAX_LOAD=" << cached_value << " from environment" << std::endl;
            } else {
                cached_value = DXRT_TASK_MAX_LOAD_DEFAULT;
                std::cout << "[DXRT] Invalid DXRT_TASK_MAX_LOAD value, using default=" << cached_value << std::endl;
            }
        } else {
            cached_value = DXRT_TASK_MAX_LOAD_DEFAULT;
        }
    }
    return cached_value;
}

#define DXRT_TASK_MAX_LOAD GetTaskMaxLoad()

} // namespace dxrt