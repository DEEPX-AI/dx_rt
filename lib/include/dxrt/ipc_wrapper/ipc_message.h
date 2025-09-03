/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <stdint.h>
#include <cstdint>
#include <map>
#include <chrono>
#include <cstring>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

namespace dxrt 
{
    enum class IPC_TYPE : int {
        //SOCKET_SYNC = 1,        // socket sync read/write
        //SOCKET_CB = 2,          // socket read callback & write sync
        MESSAE_QUEUE = 3,       // message queue (FIFO)
        //MSG_QUEUE = 4,          // message queue (POSIX)
        WIN_PIPE = 5            // windows named pipe
    };
    IPC_TYPE inline IPCDefaultType()
    {
#ifdef __linux__
    	return IPC_TYPE::MESSAE_QUEUE ;
#elif _WIN32
    	return IPC_TYPE::WIN_PIPE ;
#endif
    }

    enum class MEMORY_REQUEST_CODE : int {
        REGISTESR_PROCESS = 0,      //set msg to pid
        GET_MEMORY = 1,             //set msg to size
        FREE_MEMORY = 2,            //set msg to value returned by GET_MEMORY

    };

    enum class MEMORY_ERROR_CODE : int {
        MEMORY_OK = 0,              //msg is allocated memory if GET_MEMORY, for REGISTER_PROCESS, it is start
        NOT_ENOUGH_MEMORY = 1,
        NOT_ALLOCATED = 2,
    };

    enum class REQUEST_CODE : uint32_t {
        REGISTESR_PROCESS = 0,      //set msg to pid
        GET_MEMORY = 1,             //set msg to size
        FREE_MEMORY = 2,            //set msg to value returned by GET_MEMORY
        GET_MEMORY_FOR_MODEL = 3, //memory from backwards;
        DEVICE_INIT = 4,
        DEVICE_RESET = 5,
        DEVICE_DEINIT = 6,
        TASK_INIT = 7,
        TASK_DEINIT = 8,
        DEALLOCATE_TASK_MEMORY = 9,
        PROCESS_DEINIT = 10,        //process cleanup
        VIEW_FREE_MEMORY = 11, 
        VIEW_USED_MEMORY = 12,
        VIEW_AVAILABLE_DEVICE = 15,
        GET_USAGE = 17, 
    
        MEMORY_ALLOCATION_AND_TRANSFER_MODEL = 100,
        COMPLETE_TRANSFER_MODEL = 101,
        MEMORY_ALLOCATION_INPUT_AND_OUTPUT = 102,
        TRANSFER_INPUT_AND_RUN = 103,
        COMPLETE_TRANSFER_AND_RUN = 104,
        COMPLETE_TRNASFER_OUTPUT = 105,
        REQUEST_SCHEDULE_INFERENCE = 301,
        INFERENCE_COMPLETED = 302,
        CLOSE = 1001
    };
    std::ostream& operator<< (std::ostream& os, REQUEST_CODE code);

    enum class RESPONSE_CODE : uint32_t {
        VIEW_FREE_MEMORY_RESULT = 13,
        VIEW_USED_MEMORY_RESULT = 14,
        VIEW_AVAILABLE_DEVICE_RESULT = 16,
        GET_USAGE_RESULT = 18,
        CONFIRM_MEMORY_ALLOCATION_AND_TRANSFER_MODEL = 200,
        CONFIRM_MEMORY_ALLOCATION = 201,
        CONFIRM_TRANSFER_INPUT_AND_RUN = 202,
        CONFIRM_MEMORY_FREE = 203,
        DO_SCHEDULED_INFERENCE_CH0 = 400,
        DO_SCHEDULED_INFERENCE_CH1 = 401,
        DO_SCHEDULED_INFERENCE_CH2 = 402,
        ERROR_REPORT = 900,
        CLOSE = 1001,
        INVALID_REQUEST_CODE = 1234,
    };
    std::ostream& operator<< (std::ostream& os, RESPONSE_CODE code);
    std::string to_string(dxrt::REQUEST_CODE code);
#pragma pack(push, 1)

    struct IPCClientMessage
    {
        REQUEST_CODE code;
        uint32_t deviceId;
        uint64_t data;
        pid_t pid;
        long msgType; // for message queue
        int seqId;
        dxrt::dxrt_request_acc_t npu_acc;
        
        int taskId;
        uint64_t modelMemorySize;
        
        IPCClientMessage()
        : code(REQUEST_CODE::REGISTESR_PROCESS), deviceId(0), data(0), pid(0), msgType(0), seqId(0), taskId(-1), modelMemorySize(0)
        {
            npu_acc = dxrt::dxrt_request_acc_t{};
        }
    };

    struct IPCServerMessage
    {
        RESPONSE_CODE code;
        uint32_t deviceId;
        uint32_t result;
        uint64_t data;
        long msgType; // for message queue
        int seqId;
        dxrt::dxrt_response_t npu_resp;
        IPCServerMessage()
        : code(RESPONSE_CODE::CLOSE), deviceId(0), result(0), data(0), msgType(0), seqId(0)
        {
            npu_resp = dxrt::dxrt_response_t{};
        }
    };

    struct IPCRegisterTask
    {
        RESPONSE_CODE code;
        uint32_t deviceId;
        int taskId;
        pid_t pid;
        int8_t    model_type;
        int8_t    model_format;
        uint32_t  model_cmds;
        uint32_t  cmd_offset;
        uint32_t  weight_offset;
    };

    struct IPCRequestInference
    {
        RESPONSE_CODE code;
        uint32_t deviceId;
        int taskId;
        int requestId;
        long msgType;
        pid_t pid;
        uint64_t input_base = 0;
        uint32_t input_offset = 0;
        uint32_t input_size = 0;
        uint64_t output_base = 0;
        uint32_t output_offset = 0;
        uint32_t output_size = 0;
    };


#pragma pack(pop)

    inline DXRT_API std::ostream& operator<<(std::ostream& os, const IPCClientMessage& clientMessage)
    {
        os << "client-message code=" << clientMessage.code;
        return os;
    }

    inline DXRT_API std::ostream& operator<<(std::ostream& os, const IPCServerMessage& serverMessage)
    {
        os << "server-message code=" << serverMessage.code;
        return os;
    }

    // for tracing
    inline DXRT_API std::string _s(dxrt::REQUEST_CODE c)
    {
        static std::map<dxrt::REQUEST_CODE, std::string> m;
        if (m.size() == 0) {
            m[dxrt::REQUEST_CODE::REGISTESR_PROCESS] = "REGISTESR_PROCESS";
            m[dxrt::REQUEST_CODE::GET_MEMORY] = "GET_MEMORY";
            m[dxrt::REQUEST_CODE::FREE_MEMORY] = "FREE_MEMORY";
            m[dxrt::REQUEST_CODE::GET_MEMORY_FOR_MODEL] = "GET_MEMORY_FOR_MODEL";
            m[dxrt::REQUEST_CODE::DEVICE_INIT] = "DEVICE_INIT";
            m[dxrt::REQUEST_CODE::DEVICE_RESET] = "DEVICE_RESET";
            m[dxrt::REQUEST_CODE::DEVICE_DEINIT] = "DEVICE_DEINIT";
            
            m[dxrt::REQUEST_CODE::TASK_INIT] = "TASK_INIT";
            m[dxrt::REQUEST_CODE::TASK_DEINIT] = "TASK_DEINIT";
            m[dxrt::REQUEST_CODE::DEALLOCATE_TASK_MEMORY] = "DEALLOCATE_TASK_MEMORY";
            m[dxrt::REQUEST_CODE::PROCESS_DEINIT] = "PROCESS_DEINIT";
            
            m[dxrt::REQUEST_CODE::VIEW_FREE_MEMORY] = "VIEW_FREE_MEMORY";
            m[dxrt::REQUEST_CODE::VIEW_USED_MEMORY] = "VIEW_USED_MEMORY";
            m[dxrt::REQUEST_CODE::VIEW_AVAILABLE_DEVICE] = "VIEW_AVAILABLE_DEVICE"; 
            m[dxrt::REQUEST_CODE::GET_USAGE] = "GET_USAGE";  
            
            m[dxrt::REQUEST_CODE::MEMORY_ALLOCATION_AND_TRANSFER_MODEL] = "MEMORY_ALLOCATION_AND_TRANSFER_MODEL";
            m[dxrt::REQUEST_CODE::COMPLETE_TRANSFER_MODEL] = "COMPLETE_TRANSFER_MODEL";
            m[dxrt::REQUEST_CODE::MEMORY_ALLOCATION_INPUT_AND_OUTPUT] = "MEMORY_ALLOCATION_INPUT_AND_OUTPUT";
            m[dxrt::REQUEST_CODE::TRANSFER_INPUT_AND_RUN] = "TRANSFER_INPUT_AND_RUN";
            m[dxrt::REQUEST_CODE::COMPLETE_TRANSFER_AND_RUN] = "COMPLETE_TRANSFER_AND_RUN";
            m[dxrt::REQUEST_CODE::COMPLETE_TRNASFER_OUTPUT] = "COMPLETE_TRNASFER_OUTPUT";
            m[dxrt::REQUEST_CODE::REQUEST_SCHEDULE_INFERENCE] = "REQUEST_SCHEDULE_INFERENCE";
            m[dxrt::REQUEST_CODE::INFERENCE_COMPLETED] = "INFERENCE_COMPLETED";
            m[dxrt::REQUEST_CODE::CLOSE] = "CLOSE";
        }
        return m.find(c) == m.end() ? "REQUEST_Unknown" : m[c];
    }
    inline DXRT_API std::string _s(dxrt::RESPONSE_CODE c)
    {
        static std::map<dxrt::RESPONSE_CODE, std::string> m;
        if (m.size() == 0) {
            m[dxrt::RESPONSE_CODE::VIEW_FREE_MEMORY_RESULT] = "VIEW_FREE_MEMORY_RESULT";
            m[dxrt::RESPONSE_CODE::VIEW_USED_MEMORY_RESULT] = "VIEW_USED_MEMORY_RESULT";
            m[dxrt::RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION_AND_TRANSFER_MODEL] = "CONFIRM_MEMORY_ALLOCATION_AND_TRANSFER_MODEL";
            m[dxrt::RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION] = "CONFIRM_MEMORY_ALLOCATION";
            m[dxrt::RESPONSE_CODE::CONFIRM_TRANSFER_INPUT_AND_RUN] = "CONFIRM_TRANSFER_INPUT_AND_RUN";
            m[dxrt::RESPONSE_CODE::CONFIRM_MEMORY_FREE] = "CONFIRM_MEMORY_FREE";
            m[dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0] = "DO_SCHEDULED_INFERENCE_CH0";
            m[dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH1] = "DO_SCHEDULED_INFERENCE_CH1";
            m[dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH2] = "DO_SCHEDULED_INFERENCE_CH2";
            m[dxrt::RESPONSE_CODE::ERROR_REPORT] = "ERROR_REPORT";
            m[dxrt::RESPONSE_CODE::CLOSE] = "CLOSE";
            m[dxrt::RESPONSE_CODE::INVALID_REQUEST_CODE] = "INVALID_REQUEST_CODE";
        }
        return m.find(c) == m.end() ? "RESPONSE_Unknown" : m[c];
    }


#ifdef _WIN32
    // usage
	// static auto start = std::chrono::high_resolution_clock::now();
	// ...
	// start = durationPrint(start, "IPCPipeWindows::SendOL :");
    inline DXRT_API std::chrono::steady_clock::time_point durationPrint(std::chrono::steady_clock::time_point start, const char* msg)
    {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        double total_time = duration.count();
        double avg_latency = total_time / 1;
        if (avg_latency > 100)
            LOG_DXRT_I_DBG << msg << avg_latency << " ms" << std::endl;
        return end;
    }

#endif // _WIN32



}  // namespace dxrt
