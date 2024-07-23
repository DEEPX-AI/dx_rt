// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/request.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include <mutex>
#include <atomic>
#include <signal.h>
#include <thread>
#include <condition_variable>
#ifdef __linux__
    #include <poll.h>
#elif _WIN32
    #include <windows.h>
    struct pollfd {
        HANDLE fd;
        short events;
        short revents;
    };
#endif

namespace dxrt {
using DevicePtr = std::shared_ptr<Device>;

enum SkipMode
{
    NONE            = 0,
    VERSION_CHECK,
    COMMON_SKIP
};

class Worker;
class Memory;
#ifdef __linux__
class InferenceOption;
#elif _WIN32
// TODO: cpu_handle.h������ struct�� ����Ǿ� ����.
struct InferenceOption;
#endif
class Task;
class Profiler;
class Buffer;
class FwLog;
class DXRT_API Device
{
public:
    const static std::string DeviceStatusTable[];
    Device(const std::string &);
    ~Device(void);
    std::string name() { return _name; }
    int id() { return _id; }    
    void *input_buf(int taskId, int bufId);
    int load();
    void pick();
    int infCnt();
    int fd();
    dxrt_device_info_t info() { return _info;}
    dxrt_device_status_t status();
    int Process(dxrt_cmd_t, void*, uint32_t size=0, uint32_t sub_cmd=0);
    int InferenceRequest(Task *task, Tensors inputs, Tensors outputs);
    int InferenceRequest(Tensors);
    int InferenceRequest(RequestPtr req);
    TensorPtrs Validate(RequestPtr req, bool skipInference=false);
    int Release(Task *task);
    int Response(dxrt_response_t &response);
    int Write(dxrt_meminfo_t &);
    int Read(dxrt_meminfo_t &);
    int Wait();
    void Identify(int id_, SkipMode skip = NONE);
    void BoundOption(dxrt_sche_sub_cmd_t subCmd);
    void Terminate();
    void Reset(int opt);
    void ResetBuffer(int opt);
    std::vector<uint32_t> Dump();
    void UpdateFwConfig(std::vector<uint32_t> cfg);
    int UpdateFw(std::string fwFile, int subCmd = 0);
    std::shared_ptr<FwLog> GetFwLog();
    int64_t Allocate(uint64_t size);
    int64_t Allocate(dxrt_request_t &inference);
    void Deallocate(uint64_t addr);
    void Deallocate(dxrt_request_t &inference);
    void ThreadImpl(void);
    int RegisterTask(Task *task);
    void CallBack();
    std::vector<dxrt_model_t> npu_model(int taskId);
    std::vector<Tensors> inputs(int taskId);
    Tensors outputs(int taskId);
    friend DXRT_API std::ostream& operator<<(std::ostream &, const Device&);
protected:
    int _id = 0;
    uint32_t _type = 0; /* 0: ACC type, 1: STD type */
    SkipMode _skip;
    npu_bound_op _boundOp;
    uint32_t _variant;
    int _devFd = -1;
#ifdef __linux__
    struct pollfd _devPollFd;
#elif _WIN32
    HANDLE _devHandle = INVALID_HANDLE_VALUE;
#endif
    std::string _file;
    std::string _name;
    dxrt_device_info_t _info;
    dxrt_device_status_t _status;
    int _load = 0;
    int _inferenceCnt = 0;
    bool _hasWorkers = false;
    Profiler &_profiler;
    std::shared_ptr<Memory> _memory;
    std::shared_ptr<Memory> _modelMem;
    std::shared_ptr<Memory> _featureMem;
    std::thread _thread;
    std::mutex _lock;
    std::atomic<bool> _stop {false};
    std::unordered_map<int, std::vector< dxrt_model_t >> _npuModel;
    std::unordered_map<int, std::vector< dxrt_request_t >> _npuInference;
    std::unordered_map<int, std::vector< dxrt_request_acc_t >> _npuInferenceAcc;
    std::unordered_map<int, std::vector<Tensors>> _inputTensors;
    std::unordered_map<int, std::vector<Tensors>> _outputTensors;
    std::unordered_map<int, std::vector<std::vector<uint8_t>>> _inputTensorBuffers;
    std::unordered_map<int, std::vector<std::vector<uint8_t>>> _outputTensorBuffers;
    std::unordered_map<int, std::vector<uint8_t>> _outputValidateBuffers;
    std::unordered_map<int, int> _bufIdx;
    std::shared_ptr<Worker> _inputWorker=nullptr;
    std::shared_ptr<Worker> _outputWorker=nullptr;
    std::shared_ptr<Worker> _errorWorker=nullptr;
    std::shared_ptr<Buffer> _buffer=nullptr;
};

extern DXRT_API std::shared_ptr<Device> PickOneDevice(std::vector<std::shared_ptr<Device>> &devices_);
extern DXRT_API std::vector<std::shared_ptr<Device>> CheckDevices(SkipMode skip = NONE);
extern DXRT_API void WaitDeviceResponses(std::vector<std::shared_ptr<Device>> &devices_); // temp.
DXRT_API std::ostream& operator<<(std::ostream&, const dxrt_device_status_t&);
DXRT_API std::ostream& operator<<(std::ostream& os, const dxrt_device_info_t& info);

} // namespace dxrt