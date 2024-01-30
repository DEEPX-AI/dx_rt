// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/request.h"
#include "dxrt/driver.h"
#include <mutex>
#include <atomic>
#include <signal.h>
#include <thread>
#include <condition_variable>
#include <poll.h>

namespace dxrt {
using DevicePtr = std::shared_ptr<Device>;
typedef struct device_status
{
    uint32_t voltage[4];
    uint32_t clock[4];
    uint32_t temperature[4];
    uint32_t dvfs_enable;
    uint32_t dvfs_maxfreq;
    uint32_t count[4];
    uint32_t boot_state;
} dxrt_device_status_t;
typedef struct
{
    uint64_t timestamp;
    uint32_t cmd;
    uint32_t args[5];
} dxrt_device_log_t;
class Worker;
class Memory;
class InferenceOption;
class Task;
class Profiler;
class Buffer;
class FwLog;
class Device
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
    int Process(dxrt_cmd_t, void*, uint32_t size=0);
    int InferenceRequest(Task *task, Tensors inputs, Tensors outputs);
    int InferenceRequest(Tensors);
    int InferenceRequest(RequestPtr req);
    int Release(Task *task);
    int Response(dxrt_response_t &response);
    int Write(dxrt_meminfo_t &);
    int Read(dxrt_meminfo_t &);
    int Wait();
    void Identify(int id_);
    void Terminate();
    void Reset(int opt);
    void ResetBuffer(int opt);
    std::vector<uint32_t> Dump();
    void UpdateFwConfig(std::vector<uint32_t> cfg);
    int UpdateFw(std::string fwFile);
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
    friend std::ostream& operator<<(std::ostream &, const Device&);
protected:
    int _id = 0;
    uint32_t _type = 0; /* 0: ACC type, 1: STD type */
    int _devFd = -1;
    struct pollfd _devPollFd;
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
    std::atomic<bool> _stop = false;
    std::unordered_map<int, std::vector< dxrt_model_t >> _npuModel;
    std::unordered_map<int, std::vector< dxrt_request_t >> _npuInference;
    std::unordered_map<int, std::vector< dxrt_request_acc_t >> _npuInferenceAcc;
    std::unordered_map<int, std::vector<Tensors>> _inputTensors;
    std::unordered_map<int, std::vector<Tensors>> _outputTensors;
    std::unordered_map<int, std::vector<std::vector<uint8_t>>> _inputTensorBuffers;
    std::unordered_map<int, std::vector<std::vector<uint8_t>>> _outputTensorBuffers;
    std::unordered_map<int, int> _bufIdx;
    std::shared_ptr<Worker> _inputWorker=nullptr;
    std::shared_ptr<Worker> _outputWorker=nullptr;
    std::shared_ptr<Buffer> _buffer=nullptr;
};

extern std::shared_ptr<Device> PickOneDevice(std::vector<std::shared_ptr<Device>> &devices_);
extern std::vector<std::shared_ptr<Device>> CheckDevices();
extern void WaitDeviceResponses(std::vector<std::shared_ptr<Device>> &devices_); // temp.
std::ostream& operator<<(std::ostream&, const dxrt_device_status_t&);

} // namespace dxrt