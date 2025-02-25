// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/util.h"
#include "dxrt/request.h"
#include "dxrt/task.h"
#include "dxrt/profiler.h"
#include "dxrt/device.h"

#define MIN_EACH_CPU_TASK_THREADS 1
#define MAX_EACH_CPU_TASK_THREADS 5
#define MAX_TOTAL_CPU_TASK_THREADS 10
namespace dxrt {

CpuHandleWorker::CpuHandleWorker(string name_, int numThreads, int initDynamicThreads, CpuHandle *cpuHandle_)
: Worker(name_, Type::CPU_HANDLE, numThreads, nullptr, cpuHandle_),
  _minThreads(MIN_EACH_CPU_TASK_THREADS), _maxThreads(MAX_EACH_CPU_TASK_THREADS)
{
    _numThreads = numThreads;
    _initDynamicThreads = initDynamicThreads;
    InitializeThread();

    if (CpuHandle::_multiThreadEnv ) {
        // Add a new thread (if the excess load persists for 'threshold' seconds)
        for(int i=0; i < _initDynamicThreads; i++)
        {                
            _dynamicThreads.emplace_back([this]() {
                this->ThreadWork(_dynamicThreads.size() + _numThreads);
            });
            LOG_DXRT_DBG <<getName()<< " Added a new thread, current number of threads: " << _dynamicThreads.size() + _numThreads <<endl;
        }
    }
}

CpuHandleWorker::~CpuHandleWorker()
{
    LOG_DXRT_DBG<<endl;
    if (_dynamicThreads.empty())
    {
        return;
    }

    {
        unique_lock<mutex> lk(_lock);
        _dynamicStopCnt=_dynamicThreads.size();
        _cv.notify_all();
    }

    for (auto &t : _dynamicThreads)
        t.detach();
    _dynamicThreads.clear();
}

shared_ptr<CpuHandleWorker> CpuHandleWorker::Create(string name_, int numThreads, int initDynamicThreads, CpuHandle *cpuHandle_)
{
    shared_ptr<CpuHandleWorker> ret = make_shared<CpuHandleWorker>(name_, numThreads, initDynamicThreads, cpuHandle_);
    return ret;
}

void CpuHandleWorker::ThreadWork(int id)
{
    string threadName = getName() + "_t" + to_string(id);
    int loopCnt = 0;
    size_t load;
    LOG_DXRT_DBG << threadName << " : Entry" << endl;
    bool isDynamic = (static_cast<size_t>(id) >= _numThreads);
    thread::id this_id = this_thread::get_id();
    bool dynamicStop = false;
    while (true)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        unique_lock<mutex> lk(_lock);

        _cv.wait(lk, [this, &isDynamic, &dynamicStop] {
            if (isDynamic) {
                if (_dynamicStopCnt > 0) 
                { 
                    _dynamicStopCnt--; 
                    dynamicStop = true;
                    return true; 
                }
            }
            return !_queue.empty() || _stop;
        });
        if (_stop || (isDynamic && dynamicStop))
        {
            if (_stop)
            {
                LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
                while (!_queue.empty()) {
                    _queue.pop();
                }
                LOG_DXRT_DBG<<"Queue is flushed"<<endl;
                CpuHandle::_totalNumThreads--;
                break;
            }
            if (isDynamic && dynamicStop) {
                auto it = find_if(_dynamicThreads.begin(), _dynamicThreads.end(),
                    [this_id](thread& t) { return t.get_id() == this_id; });
                if (it != _dynamicThreads.end()) {
                    LOG_DXRT_DBG<<"Dynamic Stop - target thread is found"<<endl;
                    it->detach();
                    _dynamicThreads.erase(it);
                    CpuHandle::_totalNumThreads--;
                    cout << threadName << " : removed itself from dynamic threads. Remaining: " 
                        << _dynamicThreads.size() + _numThreads <<"(total : "<<CpuHandle::_totalNumThreads<<"), _loadCheckTerm : "<<_loadCheckTerm<<endl;
                }
                dynamicStop=false;
            }
            LOG_DXRT_DBG << threadName << " : End" << endl;
            lk.unlock(); 
            break;
        }
        load = _queue.size();
        LOG_DXRT_DBG<< threadName <<" wakeup, load: "<<to_string(load)<<"isDynamic : "<<to_string(isDynamic)<<" _dynamicStopCnt: "<<to_string(_dynamicStopCnt)<<endl;;
        UpdateQueueStats(load);

        auto req = _queue.front();
        req->set_processed_unit(getName(),id);
        TASK_FLOW("["+to_string(req->job_id())+"] cpu worker "+to_string(id) +" wakeup, load: "+to_string(load));

        _queue.pop();

        if (DEBUG_DATA > 0)
        {
            DataDumpBin(req->task()->name() + "_input.bin", req->inputs());
        }
        TASK_FLOW_START("["+to_string(req->job_id())+"]"+req->task()->name() +" thread "+to_string(id)+" run");
        lk.unlock();

        _cpuHandle->Run(req);


        TASK_FLOW_FINISH("["+to_string(req->job_id())+"]"+req->task()->name() +" thread "+to_string(id)+" run");
        ProcessResponse(req, nullptr);

        /*
        if (DEBUG_DATA > 0)
        {
            cout<<"TEST : "<<req->outputs()[0]<<endl;
            DataDumpBin(req->task()->name() + "_output.bin", req->outputs());
            DataDumpBin(req->task()->name() + "_output_done.bin", &loopCnt, 1);
        }
        */
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}

int CpuHandleWorker::request(shared_ptr<Request> req)
{
    if (_stop) {
        LOG_DXRT_DBG << "Thread stopped. Ignoring request for job_id: " << req->job_id() << endl;
        return -1; 
    }
    TASK_FLOW("["+to_string(req->job_id())+"] cpu worker request")
    unique_lock<mutex> lk(_lock);


    if(CpuHandle::_multiThreadEnv)
    {
        // Load Check
        _loadCheck++;
        if(_loadCheck>_loadCheckTerm)
        {
            _loadCheck = 0;
            size_t load = _queue.size();
            size_t dynamicThreads = _dynamicThreads.size();
            if(load >= _numThreads + dynamicThreads 
                && dynamicThreads + _numThreads < _maxThreads)
            {
                _stableLoad += load;
                if (_stableLoad >= _threshold) {
                    // Add a new thread (if the excess load persists for 'threshold' seconds)
                    LOG_DXRT_DBG <<getName()<< " Add a new thread, current number of threads: " << dynamicThreads + _numThreads << " load : "<<load<<endl;
                    for(int i=0; i < _stableLoad - _threshold; i++)
                    {   
                        if(CpuHandle::_totalNumThreads<MAX_TOTAL_CPU_TASK_THREADS)
                        {
                            _dynamicThreads.emplace_back([this, &dynamicThreads]() {
                                this->ThreadWork(dynamicThreads + _numThreads);
                            });
                            CpuHandle::_totalNumThreads++;
                            //LOG_DXRT_DBG << "Added a new thread, current number of threads: " << dynamicThreads << endl;
                            cout <<getName()<< " Added a new thread, current threads: " << dynamicThreads + _numThreads <<"(total: "
                                <<CpuHandle::_totalNumThreads<< "), load : "<<load<<" _loadCheckTerm : "<<_loadCheckTerm <<endl;
                        }
                        else
                        {
                            break;
                        }
                    }
                    _stableLoad = 0; // Reset stable state after adding a thread
                    _loadCheckTerm--;
                }
                else
                {
                    _loadCheckTerm++;
                }
            }
            else if (load == 0 && dynamicThreads + _numThreads > _minThreads) 
            {
                _loadZeroCount += dynamicThreads;
                if (_loadZeroCount >= _idleLimit) {
                    _dynamicStopCnt += (_loadZeroCount - _idleLimit);
                    LOG_DXRT_DBG << getName() << " Remove one unnecessary thread. Remaining: " 
                        << dynamicThreads + _numThreads << " load : "<<load << endl;
                    _loadZeroCount = 0;
                    _cv.notify_all();
                    lk.unlock();
                    this_thread::yield();
                    lk.lock();
                    _loadCheckTerm--;
                }
                else
                {
                    _loadCheckTerm++;
                }
            }
            else
            {
                _loadCheckTerm++;
            }
        }
    }
    _queue.push(req);
    _cv.notify_one();
    //_cv.notify_all();

    return 0;
}

}  // namespace dxrt
