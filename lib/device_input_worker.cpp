// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/device.h"
#include "dxrt/util.h"
#include "dxrt/task_data.h"
#include "dxrt/profiler.h"
#include "dxrt/request.h"
#include "dxrt/configuration.h"

namespace dxrt {

DeviceInputWorker::DeviceInputWorker(string name_, int numThreads, Device *device_)
: Worker(name_, Type::DEVICE_INPUT, numThreads, device_, nullptr)
{
    InitializeThread();
}
DeviceInputWorker::~DeviceInputWorker()
{
    LOG_DXRT_DBG<<endl;
    _cv.notify_all();
}

shared_ptr<DeviceInputWorker> DeviceInputWorker::Create(string name_, int numThreads, Device *device_)
{
    shared_ptr<DeviceInputWorker> ret = make_shared<DeviceInputWorker>(name_, numThreads, device_);
    return ret;
}

int DeviceInputWorker::request(int requestId)
{
    unique_lock<mutex> lk(_lock);
    _queue.push(requestId);
    signalToWorker();

    return 0;
}

void DeviceInputWorker::ThreadWork(int id)
{
    string threadName = getName() +"_t"+ to_string(id);
    thread::id this_id = this_thread::get_id();
    int loopCnt = 0;  // int processCnt = 0;
    LOG_DXRT_DBG << getName() << " : Entry" << endl;
    int load;
    int ret;
    uint32_t type = _device->info().type;
#ifdef WORKER_USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif
    dxrt_cmd_t cmd = //static_cast<dxrt_cmd_t>(static_cast<int>(dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_INPUT_DMA_CH0)+id);
        dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_REQ;
    while (true)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        unique_lock<mutex> lk(_lock);
        _cv.wait(
            lk, [this] {
                return _queue.size() || _stop;
            }
        );
        if (_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            std::cout << "[" << threadName << "] Average Load: " << GetAverageLoad() << std::endl;
            while (!_queue.empty()) {
                _queue.pop();
            }
            auto it = find_if(_threads.begin(), _threads.end(),
                [this_id](thread& t) { return t.get_id() == this_id; });
            if (it != _threads.end()) {
                LOG_DXRT_DBG<<threadName<<" Detaching..."<<endl;
                it->detach();
                _threads.erase(it);
                LOG_DXRT_DBG << threadName << " : removed itself from input worker threads. Remaining: " 
                    << _threads.size() <<endl;
            }
            break;
        }
        load = _device->load();
        LOG_DXRT_DBG << threadName << " : wake up. (" << load << ") " << endl;
        UpdateQueueStats(load);
        auto requestId = _queue.front();
        _queue.pop();
        lk.unlock();
        if (type == static_cast<uint32_t>(DeviceType::ACC_TYPE))
        {
#ifdef WORKER_USE_PROFILER
            profiler.Start(threadName);
#endif
            auto inferenceAcc = _device->peekInferenceAcc(requestId);
            inferenceAcc->dma_ch = id;
            // cout << inferenceAcc << endl; // for debug.
            RequestPtr req = Request::GetById(requestId);
            if (SKIP_INFERENCE_IO != 1)
            {
                TASK_FLOW("["+to_string(req->job_id())+"]"+req->taskData()->name()+" write input, load: "+to_string(load));
                _device->Write(inferenceAcc->input, id);
            }
#ifdef USE_SERVICE
            if (Configuration::GetInstance()->GetEnable(Configuration::ITEM::SERVICE))
            {
                std::ignore = ret;
                TASK_FLOW("["+to_string(req->job_id())+"]"+req->taskData()->name()+" signal to service input");
                _device->SignalToService(inferenceAcc);
            }
            else
#endif
            {
                while (true)
                {
                    ret = _device->Process(cmd, inferenceAcc);
                    LOG_DXRT_DBG << "Input signalled " << id << " " << inferenceAcc->req_id<< endl;
                    if (ret == 0 || _stop)
                    {
                        if (DEBUG_DATA > 0)
                        {
                            RequestPtr req = Request::GetById(requestId);
                            DataDumpBin(req->taskData()->name() + "_input.bin", req->inputs());
                        }
                        // processCnt++;
                        break;
                    }
                    if (ret != -EBUSY)  // write done, but failed to enqueue
                    {
                        inferenceAcc->input.data = 0;
                    }
                }
            }

            
#ifdef WORKER_USE_PROFILER
            profiler.End(threadName);
#endif
        }
        else
        {
#ifdef WORKER_USE_PROFILER
            profiler.Start(threadName);
#endif
            auto inference = _device->peekInferenceStd(requestId);
            cout << inference << endl; // for debug.
            ret = _device->Process(cmd, inference);
#ifdef WORKER_USE_PROFILER
            profiler.End(threadName);
#endif
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}


void DeviceInputWorker::signalToWorker()
{
    _cv.notify_all();
}

}  // namespace dxrt
