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
namespace dxrt {

CpuHandleWorker::CpuHandleWorker(string name_, int numThreads, int initDynamicThreads, CpuHandle *cpuHandle_)
: Worker(name_, Type::CPU_HANDLE, numThreads, nullptr, cpuHandle_),
  _minThreads(MIN_EACH_CPU_TASK_THREADS), _maxThreads(MAX_EACH_CPU_TASK_THREADS)
{
    _numThreads = numThreads;
    _initDynamicThreads = initDynamicThreads;
    InitializeThread();

    if (CpuHandle::_dynamicCpuThread ) {
        for(int i=0; i < _initDynamicThreads; i++)
        {                
            _dynamicThreads.emplace_back([this,i]() {
                this->ThreadWork(( i + 1 ) + (_numThreads - 1)); 
            });
            LOG_DXRT_DBG <<getName()<< " Added a new thread, current number of threads: " << _dynamicThreads.size() + _numThreads <<endl;
        }
    }
}

CpuHandleWorker::~CpuHandleWorker()
{
    LOG_DXRT_DBG << endl;
    if (CpuHandle::_dynamicCpuThread) {
        { 
            unique_lock<mutex> lk(_lock);
            if (_dynamicThreads.empty()) {
                LOG_DXRT_DBG << " _dynamicThreads is empty - return" << endl;
                return;
            }
            _dynamicStopCnt.store(_dynamicThreads.size());
            LOG_DXRT_DBG << " _dynamicStopCnt is set to " << _dynamicStopCnt.load() << ", notify_all" << endl;
            _cv.notify_all();
        } 

        for (auto &t : _dynamicThreads) {
            if (t.joinable()) {
                LOG_DXRT_DBG << "Joining a dynamic thread: " << t.get_id() << endl;
                t.join();
            }
        }
        LOG_DXRT_DBG << "_dynamicThreads all joined." << endl;
        
        unique_lock<mutex> lk(_lock); 
        _dynamicThreads.clear();
        LOG_DXRT_DBG << "_dynamicThreads.clear() done." << endl;
    }
    LOG_DXRT_DBG << " DONE" << endl;
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
    bool isDynamic = (static_cast<size_t>(id) >= _numThreads);
    LOG_DXRT_DBG << threadName << " : Entry ( dynamic : " << isDynamic <<")"<<endl;
    //thread::id this_id = this_thread::get_id();
    bool dynamicStop = false;
    while (_stop.load(memory_order_acquire) == false)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        unique_lock<mutex> lk(_lock);

        _cv.wait(lk, [this, &isDynamic, &dynamicStop] {
            if (isDynamic) {
                if (_dynamicStopCnt.load() > 0) 
                { 
                    _dynamicStopCnt--; 
                    dynamicStop = true;
                    return true; 
                }
            }
            return !_queue.empty() || _stop.load();
        });
        if (_stop.load(memory_order_acquire) && !isDynamic)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            while (!_queue.empty()) {
                _queue.pop();
            }
            LOG_DXRT_DBG<<"Queue is flushed"<<endl;
            CpuHandle::_totalNumThreads--; 
            if (id == 0 && 
                    (GetAverageLoad() > 2 || CpuHandle::_dynamicCpuThread || SHOW_PROFILE 
                    || Configuration::GetInstance().GetEnable(Configuration::ITEM::SHOW_PROFILE) ))
            {
                double avgLoad = GetAverageLoad();
                double loadPercent = 0.0;

                if (avgLoad > 1) {
                    loadPercent = (avgLoad - 1) / (DXRT_TASK_MAX_LOAD - 1) * 100;
                }
                LOG << "CPU TASK [" << getName() << "], Average Input Queue Load : " << loadPercent
                << "%  (DXRT_DYNAMIC_CPU_THREAD: " << (CpuHandle::_dynamicCpuThread ? "ON" : "OFF") << ")"
                << (avgLoad > 2 && !(CpuHandle::_dynamicCpuThread) ? " - To improve FPS, set: \'export DXRT_DYNAMIC_CPU_THREAD=ON\'" : "")
                << endl;
            }
            /*
            if (id == 0)
                LOG << "CPU TASK [" << getName() << "] Average Wait Load : " << ((GetAverageLoad()-1)/(DXRT_TASK_MAX_LOAD-1)*100)
                << "%  (DXRT_DYNAMIC_CPU_THREAD: " << (CpuHandle::_dynamicCpuThread ? "ON" : "OFF") << ")"
                << (GetAverageLoad() > 2 && !(CpuHandle::_dynamicCpuThread) ? " - Enable \'DXRT_DYNAMIC_CPU_THREAD\' for higher FPS." : "")
                << endl;
            */
            break;
        }
        else if (isDynamic && dynamicStop) 
        {
            LOG_DXRT_DBG << threadName << " : requested to dynamic stop thread." << endl;
            CpuHandle::_totalNumThreads--;
            LOG_DXRT_DBG << threadName << " : dynamic thread exiting." << endl;
            break;
            /*
            dynamicStop=false;
            auto it = find_if(_dynamicThreads.begin(), _dynamicThreads.end(),
                [this_id](thread& t) { return t.get_id() == this_id; });
            if (it != _dynamicThreads.end()) {
                LOG_DXRT_DBG<<"Dynamic Stop - target thread is found"<<endl;
                it->detach();
                _dynamicThreads.erase(it);
                CpuHandle::_totalNumThreads--;
                LOG_DXRT_DBG << threadName << " : removed itself from dynamic threads. Remaining: " 
                    << _dynamicThreads.size() + _numThreads <<"(total : "<<CpuHandle::_totalNumThreads.load() <<"), _dynamicStopCnt : "<< _dynamicStopCnt.load() << endl;
                break;
            }
            else {
                LOG_DXRT_DBG << "Error: Attempted to remove a non-existent thread!" << endl;
                break;
            }*/
        }
        load = _queue.size();
        LOG_DXRT_DBG<< threadName <<" wakeup, load: "<<to_string(load)<<", isDynamic : "<<to_string(isDynamic)<<" _dynamicStopCnt: "<<to_string(_dynamicStopCnt.load())<<endl;;
        UpdateQueueStats(load);

        if (!_queue.empty()) {
            auto req = _queue.front();
            req->set_processed_unit(getName(), 0, id);
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
        
        } 
        else {
            LOG_DXRT_DBG << "Warning: Attempted to pop from an empty queue!" << endl;
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End, loopCount" << loopCnt << endl;
}

int CpuHandleWorker::request(shared_ptr<Request> req)
{
    if (_stop.load()) {
        LOG_DXRT_DBG << "Thread stopped. Ignoring request for job_id: " << req->job_id() << endl;
        return -1; 
    }
    TASK_FLOW("["+to_string(req->job_id())+"] cpu worker request");


    if (!CpuHandle::_dynamicCpuThread) {
        unique_lock<mutex> lk(_lock);
        _queue.push(req);
        _cv.notify_one();
        return 0;
    }

    auto now = chrono::steady_clock::now();
    
    unique_lock<mutex> lk(_lock);

    auto timeSinceLastAdd = chrono::duration_cast<chrono::milliseconds>(now - _lastThreadControlTime);

    size_t load = _queue.size();
    _loadHistory.push_back(load);
    _slidingSum += load;

    if (_loadHistory.size() > DXRT_TASK_MAX_LOAD) {
        _slidingSum -= _loadHistory.front();
        _loadHistory.pop_front();
    }

    size_t avgLoad = _slidingSum / _loadHistory.size();  
    if (timeSinceLastAdd >= _threadControlInterval && _loadHistory.size()==DXRT_TASK_MAX_LOAD)
    {
        int dynamicThreads = static_cast<int>(_dynamicThreads.size());

        if (avgLoad > dynamicThreads + _numThreads)
        {
            if (dynamicThreads + _numThreads < _maxThreads) 
            {
                _dynamicThreads.emplace_back([this, dynamicThreads]() {
                    this->ThreadWork(dynamicThreads + (_numThreads - 1) + 1);
                });

                CpuHandle::_totalNumThreads++;
                LOG_DXRT_DBG << getName() << " Added a new thread, current threads: " 
                    << _dynamicThreads.size() + _numThreads << "(total: " << CpuHandle::_totalNumThreads.load() 
                    << "), avgLoad: " << avgLoad << endl;
                _threadControlInterval = std::chrono::milliseconds(10);
                _lastThreadControlTime = chrono::steady_clock::now();
            }
        }
        else if (avgLoad == 0)
        {
            if (_idleStartTime == std::chrono::steady_clock::time_point()) {
                _idleStartTime = chrono::steady_clock::now();
            }
            auto timeSinceLastIdle = chrono::duration_cast<chrono::milliseconds>(now - _idleStartTime);

            if (timeSinceLastIdle > _idleInterval){
                if (dynamicThreads > 0 && dynamicThreads + _numThreads > _minThreads) 
                {
                    if (dynamicThreads > _dynamicStopCnt.load())
                    {
                        _dynamicStopCnt ++;
                    }
    
                    LOG_DXRT_DBG << getName() << " Remove one unnecessary thread. Remaining: " 
                        << dynamicThreads <<" + "<< _numThreads << ", avgLoad: " << avgLoad << ", dynamicStopCnt: " << _dynamicStopCnt.load() <<endl;

                    _cv.notify_all();
                    lk.unlock();
                    this_thread::yield();
                    lk.lock();
                    _idleStartTime = std::chrono::steady_clock::time_point();
                    _threadControlInterval = std::chrono::milliseconds(10);
                }
            }
        }
        else
        {
            _threadControlInterval = std::chrono::milliseconds(50);
        }
    }
    _queue.push(req);
    _cv.notify_one();
    return 0;
}

}  // namespace dxrt
