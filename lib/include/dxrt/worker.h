/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <signal.h>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <memory>
#include <string>
#include <array>
#include <unordered_map>
#include <chrono>
#include <ostream>
#include "dxrt/common.h"
#include "dxrt/driver.h"

namespace dxrt {
class Tensor;

class Device;
class Request;
class CpuHandle;

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
    Worker(std::string name_, Type type_, int bufferCount, int numThreads = 1, Device *device_ = nullptr, CpuHandle *cpuHandle_ = nullptr);
    Worker();
    virtual ~Worker();
    static std::shared_ptr<Worker> Create(std::string name_, Type type_, int bufferCount, int numThreads = 1, Device *device_ = nullptr, CpuHandle *cpuHandle_ = nullptr);
    virtual void Stop();
    void UpdateQueueStats(int queueSize);
    bool isStopped();
    void UnHold();

protected:
    const std::string& getName() const {return _name;}
    Device *_device = nullptr;
    CpuHandle *_cpuHandle = nullptr;
    std::mutex _lock;
    std::mutex _statsLock;
    std::condition_variable _cv;
    std::atomic<bool> _stop {false};
    std::vector<std::thread> _threads;
    bool _useSystemCall = false;;
    std::atomic<bool> _hold {true};

    void InitializeThread();
    float GetAverageLoad();
    virtual void ThreadWork(int id) = 0;
    std::atomic<unsigned int> _stopCount {0};
private:
    void DoThread(int id);
    std::string _name;
    Type _type;
    std::atomic<int> _checkQueueCnt{0};
    std::atomic<int> _accumulatedQueueSize{0};
    //std::queue<std::shared_ptr<Request>> _queue;

protected:
    int _bufferCount = DXRT_TASK_MAX_LOAD_VALUE;

};


class CpuHandleWorker : public Worker
{
public:
    CpuHandleWorker(string name_, int buffer_count_, int numThreads, int initDynamicThreads, CpuHandle *cpuHandle_, size_t device_num);
    virtual ~CpuHandleWorker();
    static shared_ptr<CpuHandleWorker> Create(string name_, int buffer_count_, int numThreads, int initDynamicThreads, CpuHandle *cpuHandle_, size_t device_num);
    int request(std::shared_ptr<Request> req);

private:
    std::queue<std::shared_ptr<Request>> _queue;
    void ThreadWork(int id) override;

    size_t _device_num;
    size_t _numThreads;
    size_t _minThreads;
    size_t _maxThreads;

    int _initDynamicThreads;

    std::deque<size_t> _loadHistory;
    size_t _slidingSum = 0;

    std::chrono::steady_clock::time_point _lastThreadControlTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds _threadControlInterval = std::chrono::milliseconds(200);
    std::chrono::steady_clock::time_point _idleStartTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds _idleInterval = std::chrono::milliseconds(500);

    std::vector<std::thread> _dynamicThreads;
    std::atomic<int> _dynamicStopCnt{0};

};
} // namespace dxrt
