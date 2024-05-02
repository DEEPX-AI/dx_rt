// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/profiler.h"
#include <mutex>
#include <atomic>
#include <signal.h>
#include <thread>
#include <condition_variable>

namespace dxrt {
class Tensor;
class Task;
class Device;
class Request;
class CpuHandle;

class Worker
{
public:
    enum Type
    {
        DEVICE_INPUT,
        DEVICE_OUTPUT,
        DEVICE_ERROR,
        CPU_HANDLE,
    };
    Worker(std::string name_, Type type_, int numThreads = 1, Device *device_ = nullptr, CpuHandle *cpuHandle_ = nullptr);
    Worker();
    ~Worker();
    static std::shared_ptr<Worker> Create(std::string name_, Type type_, int numThreads = 1, Device *device_ = nullptr, CpuHandle *cpuHandle_ = nullptr);
    int request(std::shared_ptr<Request> req);
    int load(){ std::unique_lock<std::mutex> lk(_lock); return _queue.size();}
    void Stop();
private:
    std::string _name;
    Type _type;
    int _queueMaxSize = 1000;
    int _debugData = 0;
    std::queue<std::shared_ptr<Request>> _queue;
    std::mutex _lock;
    std::condition_variable _cv;
    std::vector<std::thread> _threads;
    std::atomic<bool> _stop {false};
    void DeviceInputThread(int id);
    void DeviceOutputThread(int id);
    void DeviceErrorThread();
    void CpuHandleThread();
    Device *_device = nullptr;
    CpuHandle *_cpuHandle = nullptr;
};

} // namespace dxrt