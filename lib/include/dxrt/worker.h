// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <signal.h>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <memory>
#include <string>
#include <array>
#include "dxrt/common.h"
#include "dxrt/profiler.h"
#include "dxrt/driver.h"

namespace dxrt {
class Tensor;
class Task;
class Device;
class Request;
struct CpuHandle;

using std::shared_ptr;
using std::string;

class DXRT_API Worker
{
public:
    enum Type
    {
        DEVICE_INPUT,
        DEVICE_OUTPUT,
        DEVICE_EVENT,
        CPU_HANDLE,
    };
    Worker(std::string name_, Type type_, int numThreads = 1, Device *device_ = nullptr, CpuHandle *cpuHandle_ = nullptr);
    Worker();
    virtual ~Worker();
    static std::shared_ptr<Worker> Create(std::string name_, Type type_, int numThreads = 1, Device *device_ = nullptr, CpuHandle *cpuHandle_ = nullptr);
    void Stop();
protected:
    const std::string& getName() const {return _name;}
    Device *_device = nullptr;
    CpuHandle *_cpuHandle = nullptr;
    std::mutex _lock;
    std::condition_variable _cv;
    std::atomic<bool> _stop {false};
    void InitializeThread();


    virtual void ThreadWork(int id) = 0;
private:
    void DoThread(int id);
    std::string _name;
    Type _type;
    int _queueMaxSize = 1000;

    //std::queue<std::shared_ptr<Request>> _queue;

    std::vector<std::thread> _threads;
};

class DeviceInputWorker : public Worker
{
public:
    DeviceInputWorker(string name_, int numThreads, Device *device_);
    virtual ~DeviceInputWorker();
    static shared_ptr<DeviceInputWorker> Create(string name_, int numThreads, Device *device_);
    int request(int requestId);
    size_t load(){ std::unique_lock<std::mutex> lk(_lock); return _queue.size();}
    void signalToWorker();
private:
    std::queue<int> _queue;
    void ThreadWork(int id) override;
    std::condition_variable _cv;
};
class DeviceOutputWorker : public Worker
{
public:
    DeviceOutputWorker(string name_, int numThreads, Device *device_);
    virtual ~DeviceOutputWorker();
    static shared_ptr<DeviceOutputWorker> Create(string name_, int numThreads, Device *device_);
#ifdef USE_SERVICE
    void PushWork(const dxrt_response_t& resp);
#endif
private:
    void ThreadWork(int id) override;
#ifdef USE_SERVICE
    std::queue<dxrt_response_t> _queue;
    std::condition_variable _cv;
#endif

};
class DeviceEventWorker : public Worker
{
public:
    DeviceEventWorker(string name_, Device *device_);
    virtual ~DeviceEventWorker();
    static shared_ptr<DeviceEventWorker> Create(string name_, Device *device_);

private:
    void ThreadWork(int id) override;
};
class CpuHandleWorker : public Worker
{
public:
    CpuHandleWorker(string name_, int numThreads, CpuHandle *cpuHandle_);
    virtual ~CpuHandleWorker();
    static shared_ptr<CpuHandleWorker> Create(string name_, int numThreads, CpuHandle *cpuHandle_);
    int request(std::shared_ptr<Request> req);

private:
    std::queue<std::shared_ptr<Request>> _queue;
    void ThreadWork(int id) override;
};
} // namespace dxrt