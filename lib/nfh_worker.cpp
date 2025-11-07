/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/nfh_worker.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/request_data.h"
#include "dxrt/task_data.h"
#include "dxrt/npu_format_handler.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/driver.h"
#include <iostream>
#include <algorithm>

using std::string;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::unique_lock;
using std::mutex;

namespace dxrt {

// ==================== NfhInputWorker Implementation ====================

NfhInputWorker::NfhInputWorker(const string& name, int numThreads, Device* device)
: _name(name), _numThreads(numThreads), _device(device)
{
    LOG_DXRT_DBG << _name << " will be created with " << _numThreads << " threads." << endl;
}

NfhInputWorker::~NfhInputWorker()
{
    LOG_DXRT_DBG << "Destroying " << _name << endl;
    Stop();
    
    for (auto& thread : _threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

shared_ptr<NfhInputWorker> NfhInputWorker::Create(const string& name, int numThreads, Device* device)
{
    auto worker = make_shared<NfhInputWorker>(name, numThreads, device);
    worker->Start();
    return worker;
}

void NfhInputWorker::Start()
{
    initializeThreads();
    LOG_DXRT_DBG << _name << " started with " << _numThreads << " threads." << endl;
}

void NfhInputWorker::Stop()
{
    LOG_DXRT_DBG << "Stopping " << _name << endl;
    _stop.store(true);
    
    {
        unique_lock<mutex> lock(_queueMutex);
        _queueCV.notify_all();
    }
}

bool NfhInputWorker::isStopped() const
{
    return (_stopCount.load() >= static_cast<unsigned int>(_numThreads));
}

int NfhInputWorker::EnqueueWork(const NfhInputWork& work)
{
    if (_stop.load())
    {
        return -1;
    }
    
    {
        unique_lock<mutex> lock(_queueMutex);
        
        // No queue size limit (consistent with other workers like DeviceInputWorker, CpuHandleWorker)
        
        _workQueue.push(work);
        _queueCV.notify_one();
    }
    
    return 0;
}

size_t NfhInputWorker::QueueSize()
{
    unique_lock<mutex> lock(_queueMutex);
    return _workQueue.size();
}

void NfhInputWorker::UpdateQueueStats(int queueSize)
{
    unique_lock<mutex> lock(_statsLock);
    _checkQueueCnt++;
    _accumulatedQueueSize.fetch_add(queueSize);
}

float NfhInputWorker::GetAverageLoad()
{
    unique_lock<mutex> lock(_statsLock);
    return (_checkQueueCnt.load() > 0) ? 
        static_cast<float>(_accumulatedQueueSize.load()) / _checkQueueCnt.load() : 0.0f;
}

void NfhInputWorker::initializeThreads()
{
    _threads.reserve(_numThreads);
    for (int i = 0; i < _numThreads; ++i)
    {
        _threads.emplace_back(&NfhInputWorker::workerThreadMain, this, i);
    }
}

void NfhInputWorker::workerThreadMain(int threadId)
{
    string threadName = _name + "_t" + std::to_string(threadId);
    LOG_DXRT_DBG << threadName << " started." << endl;
    
    try
    {
        while (!_stop.load(std::memory_order_acquire))
        {
            NfhInputWork work;
            int queueLoad = 0;
            
            // Get work from work queue
            {
                unique_lock<mutex> lock(_queueMutex);
                _queueCV.wait(lock, [this] {
                    return !_workQueue.empty() || _stop.load(std::memory_order_acquire);
                });
                
                if (_stop.load(std::memory_order_acquire))
                {
                    break;
                }
                
                if (_workQueue.empty())
                {
                    continue;
                }
                
                // Update load statistics when actually processing (like other workers)
                queueLoad = _workQueue.size();
                UpdateQueueStats(queueLoad);
                
                work = _workQueue.front();
                _workQueue.pop();
            }
            
            // NFH input processing
            try
            {
                int result = processInputNfh(work, threadId);
                if (result != 0)
                {
                    LOG_DXRT_ERR("Failed to process input NFH for request " << work.requestId);
                }
                // InferenceRequest_ACC trigger
                if (work.req)
                {
                    auto reqData = work.req->getData();
                    if (reqData && _device)
                    {
                        int inferenceResult = _device->ProcessInferenceRequestACC(reqData, work.boundOp);
                        if (inferenceResult != 0)
                        {
                            LOG_DXRT_ERR("Failed to process InferenceRequest_ACC after NFH for request " << work.requestId);
                        }
                    }
                    else
                    {
                        LOG_DXRT_ERR("Request data or device not available for request " << work.requestId);
                    }
                }
            }
            catch (const std::exception& e)
            {
                LOG_DXRT_ERR("Exception in NFH input processing: " << e.what());
            }
        }
    }
    catch (const std::exception& e)
    {
        LOG_DXRT_ERR("Worker thread error in " << threadName << ": " << e.what());
    }
    
    // Output load statistics on thread termination (like other workers)
    if (threadId == 0)
    {
        double avgLoad = GetAverageLoad();
        if (avgLoad > 2 || SHOW_PROFILE || Configuration::GetInstance().GetEnable(Configuration::ITEM::SHOW_PROFILE))
        {
            double loadPercent = 0.0;
            if (avgLoad > 1 && DXRT_TASK_MAX_LOAD > 1) {
                loadPercent = (avgLoad - 1) / (DXRT_TASK_MAX_LOAD - 1) * 100;
            }
            
            // Thread-safe output with consistent formatting
            {
                std::lock_guard<std::mutex> lock(_statsLock);
                LOG << "NPU DEVICE [" << _device->id() << "] Input Format Handler - Average Input Queue Load : " << loadPercent << "%" 
                    << " (Threads: " << NFH_INPUT_WORKER_THREADS << ")"
                    << (avgLoad > 2 ? " - Consider increasing NFH_INPUT_WORKER_THREADS for better performance" : "")
                    << endl;
            }
        }
    }
    
    _stopCount++;
    LOG_DXRT_DBG << threadName << " stopped." << endl;
}

int NfhInputWorker::processInputNfh(const NfhInputWork& work, int threadId)
{
    if (!work.req || !_device)
    {
        LOG_DXRT_ERR("Invalid work or device in processInputNfh");
        return -1;
    }
    
    auto reqData = work.req->getData();
    if (!reqData || !reqData->taskData)
    {
        LOG_DXRT_ERR("Invalid request data in processInputNfh");
        return -1;
    }
    
    // common encoding utility
    int enc = npu_format_handler::NpuFormatHandler::EncodeInputs(reqData, threadId);
    if (enc != 0) return enc;
    
    return 0;
}

// ==================== NfhOutputWorker Implementation ====================

NfhOutputWorker::NfhOutputWorker(const string& name, int numThreads, Device* device)
: _name(name), _numThreads(numThreads), _device(device)
{
    LOG_DXRT_DBG << _name << " will be created with " << _numThreads << " threads." << endl;
}

NfhOutputWorker::~NfhOutputWorker()
{
    LOG_DXRT_DBG << "Destroying " << _name << endl;
    Stop();
    
    for (auto& thread : _threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

shared_ptr<NfhOutputWorker> NfhOutputWorker::Create(const string& name, int numThreads, Device* device)
{
    auto worker = make_shared<NfhOutputWorker>(name, numThreads, device);
    worker->Start();
    return worker;
}

void NfhOutputWorker::Start()
{
    initializeThreads();
    LOG_DXRT_DBG << _name << " started with " << _numThreads << " threads." << endl;
}

void NfhOutputWorker::Stop()
{
    LOG_DXRT_DBG << "Stopping " << _name << endl;
    _stop.store(true);
    
    {
        unique_lock<mutex> lock(_queueMutex);
        _queueCV.notify_all();
    }
}

bool NfhOutputWorker::isStopped() const
{
    return (_stopCount.load() >= static_cast<unsigned int>(_numThreads));
}

int NfhOutputWorker::EnqueueWork(const NfhOutputWork& work)
{
    if (_stop.load())
    {
        return -1;
    }
    
    {
        unique_lock<mutex> lock(_queueMutex);
        
        // No queue size limit (consistent with other workers like DeviceInputWorker, CpuHandleWorker)
        
        _workQueue.push(work);
        _queueCV.notify_one();
    }
    
    return 0;
}

size_t NfhOutputWorker::QueueSize()
{
    unique_lock<mutex> lock(_queueMutex);
    return _workQueue.size();
}

void NfhOutputWorker::UpdateQueueStats(int queueSize)
{
    unique_lock<mutex> lock(_statsLock);
    _checkQueueCnt++;
    _accumulatedQueueSize.fetch_add(queueSize);
}

float NfhOutputWorker::GetAverageLoad()
{
    unique_lock<mutex> lock(_statsLock);
    return (_checkQueueCnt.load() > 0) ? 
        static_cast<float>(_accumulatedQueueSize.load()) / _checkQueueCnt.load() : 0.0f;
}

void NfhOutputWorker::initializeThreads()
{
    _threads.reserve(_numThreads);
    for (int i = 0; i < _numThreads; ++i)
    {
        _threads.emplace_back(&NfhOutputWorker::workerThreadMain, this, i);
    }
}

void NfhOutputWorker::workerThreadMain(int threadId)
{
    string threadName = _name + "_t" + std::to_string(threadId);
    LOG_DXRT_DBG << threadName << " started." << endl;
    
    try
    {
        while (!_stop.load(std::memory_order_acquire))
        {
            NfhOutputWork work;
            int queueLoad = 0;
            
            // Get work from work queue
            {
                unique_lock<mutex> lock(_queueMutex);
                _queueCV.wait(lock, [this] {
                    return !_workQueue.empty() || _stop.load(std::memory_order_acquire);
                });
                
                if (_stop.load(std::memory_order_acquire))
                {
                    break;
                }
                
                if (_workQueue.empty())
                {
                    continue;
                }
                
                // Update load statistics when actually processing (like other workers)
                queueLoad = _workQueue.size();
                UpdateQueueStats(queueLoad);
                
                work = _workQueue.front();
                _workQueue.pop();
            }
            
            // NFH output processing
                try
                {
                    int result = processOutputNfh(work, threadId);
                    if (result != 0)
                    {
                        LOG_DXRT_ERR("Failed to process output NFH for request " << work.requestId);
                    }
                    else
                    {
                        // NFH processing completed, proceed to direct subsequent processing (prevent circular calls)
                        if (work.req && _device)
                        {
                            try
                            {
                                auto request_acc = _device->peekInferenceAcc(work.requestId);
                                
                                // direct subsequent processing
                                TASK_FLOW("[" + std::to_string(work.req->job_id()) + "]" + 
                                         work.req->taskData()->name() + " NFH output completed, load :" + 
                                         std::to_string(_device->load()));

                                _device->Deallocate_npuBuf(request_acc.input.offset, work.req->taskData()->id());
                                ProcessResponse(work.req, const_cast<dxrt_response_t*>(&work.response), 0);
                                _device->popInferenceStruct(work.requestId);
                                
                                LOG_DXRT_DBG << "NFH Output processing completed for request " << work.requestId << endl;
                            }
                            catch (const std::exception& e)
                            {
                                LOG_DXRT_ERR("Exception in NFH output completion: " << e.what());
                            }
                        }
                    }
                }
                catch (const std::exception& e)
                {
                    LOG_DXRT_ERR("Exception in NFH output processing: " << e.what());
                }
        }
    }
    catch (const std::exception& e)
    {
        LOG_DXRT_ERR("Worker thread error in " << threadName << ": " << e.what());
    }
    
    // Output load statistics on thread termination (like other workers)
    if (threadId == 0)
    {
        double avgLoad = GetAverageLoad();
        if (avgLoad > 2 || SHOW_PROFILE || Configuration::GetInstance().GetEnable(Configuration::ITEM::SHOW_PROFILE))
        {
            double loadPercent = 0.0;
            if (avgLoad > 1 && DXRT_TASK_MAX_LOAD > 1) {
                loadPercent = (avgLoad - 1) / (DXRT_TASK_MAX_LOAD - 1) * 100;
            }
            
            // Thread-safe output with consistent formatting
            {
                std::lock_guard<std::mutex> lock(_statsLock);
                LOG << "NPU DEVICE [" << _device->id() << "] Output Format Handler - Average Input Queue Load : " << loadPercent << "%" 
                    << " (Threads: " << NFH_OUTPUT_WORKER_THREADS << ")"
                    << (avgLoad > 2 ? " - Consider increasing NFH_OUTPUT_WORKER_THREADS for better performance" : "")
                    << endl;
            }
        }
    }
    
    _stopCount++;
    LOG_DXRT_DBG << threadName << " stopped." << endl;
}

int NfhOutputWorker::processOutputNfh(const NfhOutputWork& work, int threadId)
{
    if (!work.req || !_device)
    {
        LOG_DXRT_ERR("Invalid work or device in processOutputNfh");
        return -1;
    }

    // common decoding utility
    int dec = npu_format_handler::NpuFormatHandler::DecodeOutputs(&work.req, &work.response, threadId);
    if (dec != 0) return dec;

    return 0;
}

} // namespace dxrt
