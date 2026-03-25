/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 *
 * This file uses ONNX Runtime (MIT License) - Copyright (c) Microsoft Corporation.
 */

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include "dxrt/cpu_handle.h"
#include "dxrt/util.h"
#include "dxrt/request.h"
#include "dxrt/task.h"
#include "dxrt/profiler.h"
#include "dxrt/device.h"
#include "dxrt/exception/exception.h"
#include "dxrt/request_response_class.h"
#include "dxrt/inference_job.h"



using std::endl;
using std::memory_order_acquire;
using std::to_string;
using std::unique_lock;
using std::mutex;


namespace dxrt {

CpuHandleWorker::CpuHandleWorker(const string& name_, int bufferCount, int numThreads, int initDynamicThreads, CpuHandle *cpuHandle_, size_t device_num)
: Worker(name_, Type::CPU_HANDLE, bufferCount, numThreads, nullptr, cpuHandle_),
  _device_num(device_num), _numThreads(numThreads), _initDynamicThreads(initDynamicThreads)
{
    InitializeThread();

    if (CpuHandle::_dynamicCpuThread)
    {
        for (int i = 0; i < _initDynamicThreads; i++)
        {
            StartDynamicThread(i);
            LOG_DXRT_DBG <<getName()<< " Added a new thread, current number of threads: " << static_cast<int>(_dynamicThreads.size()) + _numThreads << endl;
        }
    }
}

CpuHandleWorker::~CpuHandleWorker()
{
    LOG_DXRT_DBG << endl;
    if (CpuHandle::_dynamicCpuThread)
    {
        {
            std::unique_lock<std::mutex> lk(getLock());
            if (_dynamicThreads.empty()) {
                LOG_DXRT_DBG << " _dynamicThreads is empty - return" << endl;
                return;
            }
            _dynamicStopCnt.store(static_cast<int>(_dynamicThreads.size()));
            LOG_DXRT_DBG << " _dynamicStopCnt is set to " << _dynamicStopCnt.load() << ", notify_all" << endl;
            getConditionVariable().notify_all();
        }

        for (auto &t : _dynamicThreads) {
            if (t.joinable()) {
                LOG_DXRT_DBG << "Joining a dynamic thread: " << t.get_id() << endl;
                t.join();
            }
        }
        LOG_DXRT_DBG << "_dynamicThreads all joined." << endl;

        std::unique_lock<std::mutex> lk(getLock());
        _dynamicThreads.clear();
        LOG_DXRT_DBG << "_dynamicThreads.clear() done." << endl;

    }
    LOG_DXRT_DBG << " DONE" << endl;
}

std::shared_ptr<CpuHandleWorker> CpuHandleWorker::Create(const string& name_, int buffer_count_, int numThreads, int initDynamicThreads, CpuHandle *cpuHandle_, size_t device_num)
{
    auto ret = std::make_shared<CpuHandleWorker>(name_, buffer_count_, numThreads, initDynamicThreads, cpuHandle_, device_num);
    return ret;
}

void CpuHandleWorker::ThreadWork(int id)
{
    ThreadWorkImpl(id);
}

int CpuHandleWorker::MakeDynamicThreadId(int dynamicIndex) const
{
    return dynamicIndex + static_cast<int>(_numThreads);
}

void CpuHandleWorker::StartDynamicThread(int dynamicIndex)
{
    _dynamicThreads.emplace_back([this, dynamicIndex]()
    {
        ThreadWorkImpl(MakeDynamicThreadId(dynamicIndex));
    });
}

void CpuHandleWorker::ThreadWorkImpl(int id)
{
    string threadName = getName() + "_t" + to_string(id);
    int loopCnt = 0;
    size_t load;
    bool isDynamic = (static_cast<size_t>(id) >= _numThreads);
    LOG_DXRT_DBG << threadName << " : Entry ( dynamic : " << isDynamic <<")" << endl;

#ifdef USE_ORT
    // Hybrid approach: Use shared session with worker-level synchronization
    std::shared_ptr<Ort::Session> workerSession = getCpuHandle()->_session;

    if (CpuHandle::_dynamicCpuThread) {
        // Even in dynamic mode, use shared session for better performance
        // ONNX Runtime's Session::Run() is thread-safe
        // note: if you need workerSession, you can get it from CreateWorkerSession method with no args
        LOG_DXRT_DBG << threadName << " : Using shared session in dynamic mode" << endl;
    }

#endif


    bool dynamicStop = false;
    while (getStopFlag().load(memory_order_acquire) == false)
    {
        bool shouldExit = false;
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        std::unique_lock<std::mutex> lk(getLock());

        getConditionVariable().wait(lk, [this, &isDynamic, &dynamicStop] {
            if (isDynamic && (_dynamicStopCnt.load()> 0)) {
                _dynamicStopCnt--;
                dynamicStop = true;
                return true;

            }
            return !_queue.empty() || getStopFlag().load();
        });
        if (isDynamic && dynamicStop)
        {
            // Check if this dynamic thread is requested to stop
            LOG_DXRT_DBG << threadName << " : requested to dynamic stop thread." << endl;
            CpuHandle::_totalNumThreads--;
            LOG_DXRT_DBG << threadName << " : dynamic thread exiting." << endl;
            shouldExit = true;
        }
        else if (isDynamic)
        {
            LOG_DXRT_DBG << threadName << " : spurious wakeup or stop signal, continue waiting." << endl;
        }
        else
        {
            //not dynamic thread, check _stop flag
            if (getStopFlag().load(memory_order_acquire))
            {
                LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
                while (!_queue.empty()) {
                    _queue.pop();
                }
                LOG_DXRT_DBG << "Queue is flushed" << endl;
                CpuHandle::_totalNumThreads--;
                if (id == 0 &&
                        (GetAverageLoad() > 2 || SHOW_PROFILE || Configuration::GetInstance().GetEnable(Configuration::ITEM::SHOW_PROFILE) ))
                {
                    double avgLoad = GetAverageLoad();
                    double loadPercent = 0.0;

                    if (avgLoad > 1) {
                        loadPercent = (avgLoad - 1.0) / (static_cast<double>(getBufferCount()*_device_num) - 1.0) * 100.0;
                    }
                    LOG << "CPU TASK [" << getName() << "] Inference Worker - Average Input Queue Load : " << loadPercent
                    << "%  (DXRT_DYNAMIC_CPU_THREAD: " << (CpuHandle::_dynamicCpuThread ? "ON" : "OFF") << ")"
                    << (avgLoad > 2 && (CpuHandle::_dynamicCpuThread == false) ? " - To improve FPS, set: \'export DXRT_DYNAMIC_CPU_THREAD=ON\'" : "")
                    << endl;
                }
#if 0
                if (id == 0)
                    LOG << "CPU TASK [" << getName() << "] Average Wait Load : " << ((GetAverageLoad()-1)/(getBufferCount()-1)*100)
                    << "%  (DXRT_DYNAMIC_CPU_THREAD: " << (CpuHandle::_dynamicCpuThread ? "ON" : "OFF") << ")"
                    << (GetAverageLoad() > 2 && (CpuHandle::_dynamicCpuThread== false) ? " - Enable \'DXRT_DYNAMIC_CPU_THREAD\' for higher FPS." : "")
                    << endl;
#endif
                shouldExit = true;
            }
        }

        if (!shouldExit)
        {
            load = _queue.size();
            LOG_DXRT_DBG<< threadName <<" wakeup, load: " << to_string(static_cast<int>(load))
                << ", isDynamic : " << to_string(isDynamic) << " _dynamicStopCnt: " << to_string(_dynamicStopCnt.load()) << endl;
            UpdateQueueStats(static_cast<int>(load));

            if (!_queue.empty()) {
                auto req = _queue.front();
                req->set_processed_unit(getName(), 0, id);
                TASK_FLOW("["+to_string(req->job_id())+"] cpu worker "+to_string(id) +" wakeup, load: "+to_string(load));
                _queue.pop();
#ifdef USE_PROFILER

                auto& profiler = dxrt::Profiler::GetInstance();
                std::string queue_wait_name =
                    "CPU Task Queue Wait[Job_" + std::to_string(req->job_id()) + "][" +
                    req->task()->name() + "][Req_" +
                    std::to_string(req->id()) + "]";
                profiler.End(queue_wait_name);

#endif
                if (DEBUG_DATA > 0)
                {
                    DataDumpBin(req->task()->name() + "_input.bin", req->inputs());
                }
                TASK_FLOW_START("["+to_string(req->job_id())+"]"+req->task()->name() +" thread "+to_string(id)+" run");
                lk.unlock();


                dxrt_response_t response;
                response.req_id = -1;
                try
                {
#ifdef USE_ORT
                    if (CpuHandle::_dynamicCpuThread && workerSession)
                    {
                        getCpuHandle()->RunWithSession(req, workerSession);
                    }
                    else
                    {
                        getCpuHandle()->Run(req);
                    }
#else
                    getCpuHandle()->Run(req);
#endif
                    TASK_FLOW_FINISH("["+to_string(req->job_id())+"]"+req->task()->name() +" thread "+to_string(id)+" run");
#ifdef USE_VNPU
                    // User Input Early Release: CPU execution complete, user input buffer can be released
                    // This handles the case where CPU task is the head (first task in the graph)
                    auto inferenceJob = req->inferenceJob();
                    if (inferenceJob)
                    {
                        LOG_DXRT_DBG << "[CpuWorker] CPU task '" << req->task()->name()
                                 << "' completed, triggering user input release for job " << req->job_id() << std::endl;
                        inferenceJob->TriggerUserInputRelease();
                    }
#endif // USE_VNPU
                    RequestResponse::ProcessResponse(req, response, -1);
                }
                catch (const Exception &e)
                {
                    // print error message
                    LOG_DXRT_ERR(e.what());
                    TASK_FLOW_FINISH("["+to_string(req->job_id())+"]"+req->task()->name() +" thread "+to_string(id)+" run");
                    RequestResponse::ProcessResponse(req, response, -1);
                    shouldExit = true;
                }
                catch (const std::exception &e)
                {
                    LOG_DXRT_ERR(std::string("std::exception: ") + e.what());
                    TASK_FLOW_FINISH("["+to_string(req->job_id())+"]"+req->task()->name() +" thread "+to_string(id)+" run");
                    RequestResponse::ProcessResponse(req, response, -1);
                    shouldExit = true;
                }
                catch (...)  // NOSONAR:S2738
                {
                    LOG_DXRT_ERR("Unknown exception in CpuHandleWorker");
                    TASK_FLOW_FINISH("["+to_string(req->job_id())+"]"+req->task()->name() +" thread "+to_string(id)+" run");
                    RequestResponse::ProcessResponse(req, response, -1);
                    shouldExit = true;
                }

            }
            else
            {
                LOG_DXRT_DBG << "Warning: Attempted to pop from an empty queue!" << endl;
            }
            loopCnt++;
        }

        if (shouldExit)
        {
            break;
        }
    }
    LOG_DXRT_DBG << threadName << " : End, loopCount" << loopCnt << endl;
}

int CpuHandleWorker::request(shared_ptr<Request> req)
{
    if (getStopFlag().load()) {
        LOG_DXRT_DBG << "Thread stopped. Ignoring request for job_id: " << req->job_id() << endl;
        return -1;
    }
    TASK_FLOW("["+std::to_string(req->job_id())+"] cpu worker request");


    if (!CpuHandle::_dynamicCpuThread)
    {
        std::unique_lock<std::mutex> lk(getLock());
        _queue.push(req);
        getConditionVariable().notify_one();
        return 0;
    }

    auto now = std::chrono::steady_clock::now();

    std::unique_lock<std::mutex> lk(getLock());

    auto timeSinceLastAdd = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastThreadControlTime);

    size_t load = _queue.size();
    _loadHistory.push_back(load);
    _slidingSum += load;

    if (_loadHistory.size() > getBufferCount()*_device_num) {
        _slidingSum -= _loadHistory.front();
        _loadHistory.pop_front();
    }

    size_t avgLoad = _slidingSum / _loadHistory.size();
    if (timeSinceLastAdd >= _threadControlInterval && _loadHistory.size() == getBufferCount()*_device_num)
    {
        auto dynamicThreads = static_cast<int>(_dynamicThreads.size());

        if (avgLoad > dynamicThreads + _numThreads)
        {
            if (dynamicThreads + _numThreads < _maxThreads)
            {
                StartDynamicThread(dynamicThreads);

                CpuHandle::_totalNumThreads++;
                LOG_DXRT_DBG << getName() << " Added a new thread, current threads: "
                    << _dynamicThreads.size() + _numThreads << "(total: " << CpuHandle::_totalNumThreads.load()
                    << "), avgLoad: " << avgLoad << endl;
                _threadControlInterval = std::chrono::milliseconds(10);
                _lastThreadControlTime = std::chrono::steady_clock::now();
            }
        }
        else if (avgLoad == 0)
        {
            if (_idleStartTime == std::chrono::steady_clock::time_point()) {
                _idleStartTime = std::chrono::steady_clock::now();
            }
            auto timeSinceLastIdle = std::chrono::duration_cast<std::chrono::milliseconds>(now - _idleStartTime);

            bool isIdleIntervalExceeded = timeSinceLastIdle > _idleInterval;
            bool canRemoveThread = (dynamicThreads > 0) && (dynamicThreads + _numThreads > _minThreads);
            bool canStopThread = isIdleIntervalExceeded && canRemoveThread;

            if (canStopThread)
            {
                if (dynamicThreads > _dynamicStopCnt.load())
                {
                    _dynamicStopCnt++;
                }
                LOG_DXRT_DBG << getName() << " Remove one unnecessary thread. Remaining: "
                    << dynamicThreads <<" + "<< _numThreads << ", avgLoad: " << avgLoad << ", dynamicStopCnt: " << _dynamicStopCnt.load() << endl;

                getConditionVariable().notify_all();
                lk.unlock();
                std::this_thread::yield();
                lk.lock();
                _idleStartTime = std::chrono::steady_clock::time_point();
                _threadControlInterval = std::chrono::milliseconds(10);
            }
        }
        else
        {
            _threadControlInterval = std::chrono::milliseconds(50);
        }
    }
    _queue.push(req);
    getConditionVariable().notify_one();
    return 0;
}

}  // namespace dxrt
