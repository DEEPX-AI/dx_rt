/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <atomic>
#include <string>
#include "dxrt/common.h"
#include "dxrt/driver.h"

namespace dxrt {

class Device;
class Request;

// NFH Input work structure
struct DXRT_API NfhInputWork
{
    uint32_t requestId;
    std::shared_ptr<Request> req;
    int threadId;
    npu_bound_op boundOp;  // Original boundOp parameter preserved
    
    NfhInputWork()
    : requestId(0), req(nullptr), threadId(0), boundOp(N_BOUND_NORMAL)
    {
        
    }
    
    NfhInputWork(uint32_t reqId, std::shared_ptr<Request> request, int tId, npu_bound_op bound = N_BOUND_NORMAL)
    : requestId(reqId), req(request), threadId(tId), boundOp(bound)
    {
        
    }
};

// NFH Output work structure
struct DXRT_API NfhOutputWork
{
    uint32_t requestId;
    dxrt_response_t response;
    std::shared_ptr<Request> req;
    int threadId;
    
    NfhOutputWork()
    : requestId(0), response{}, req(nullptr), threadId(0)
    {
        
    }
    
    NfhOutputWork(uint32_t reqId, const dxrt_response_t& resp, std::shared_ptr<Request> request, int tId)
    : requestId(reqId), response(resp), req(request), threadId(tId)
    {
        
    }
};

// --- NFH Worker Classes ---

// NFH Input Worker class
class DXRT_API NfhInputWorker
{
public:
    NfhInputWorker(const std::string& name, int numThreads, Device* device);
    virtual ~NfhInputWorker();
    
    static std::shared_ptr<NfhInputWorker> Create(const std::string& name, int numThreads, Device* device);
    
    int EnqueueWork(const NfhInputWork& work);
    
    void Start();
    void Stop();
    bool isStopped() const;
    
    size_t QueueSize();
    void UpdateQueueStats(int queueSize);
    float GetAverageLoad();
    
private:
    void initializeThreads();
    void workerThreadMain(int threadId);
    int processInputNfh(const NfhInputWork& work, int threadId);
    
private:
    std::string _name;
    int _numThreads;
    Device* _device;
    
    std::vector<std::thread> _threads;
    std::atomic<bool> _stop{false};
    std::atomic<unsigned int> _stopCount{0};
    
    std::queue<NfhInputWork> _workQueue;
    std::mutex _queueMutex;
    std::condition_variable _queueCV;
    
    std::mutex _statsLock;
    std::atomic<int> _checkQueueCnt{0};
    std::atomic<int> _accumulatedQueueSize{0};
};

// NFH Output Worker class
class DXRT_API NfhOutputWorker
{
public:
    NfhOutputWorker(const std::string& name, int numThreads, Device* device);
    virtual ~NfhOutputWorker();
    
    static std::shared_ptr<NfhOutputWorker> Create(const std::string& name, int numThreads, Device* device);
    
    int EnqueueWork(const NfhOutputWork& work);
    
    void Start();
    void Stop();
    bool isStopped() const;
    
    size_t QueueSize();
    void UpdateQueueStats(int queueSize);
    float GetAverageLoad();
    
private:
    void initializeThreads();
    void workerThreadMain(int threadId);
    int processOutputNfh(const NfhOutputWork& work, int threadId);
    
private:
    std::string _name;
    int _numThreads;
    Device* _device;
    
    std::vector<std::thread> _threads;
    std::atomic<bool> _stop{false};
    std::atomic<unsigned int> _stopCount{0};
    
    std::queue<NfhOutputWork> _workQueue;
    std::mutex _queueMutex;
    std::condition_variable _queueCV;
    
    std::mutex _statsLock;
    std::atomic<int> _checkQueueCnt{0};
    std::atomic<int> _accumulatedQueueSize{0};
};

} // namespace dxrt
