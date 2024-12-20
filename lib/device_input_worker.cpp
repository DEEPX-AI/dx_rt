// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/device.h"
#include "dxrt/task_data.h"

namespace dxrt {

DeviceInputWorker::DeviceInputWorker(string name_, int numThreads, Device *device_)
: Worker(name_, Type::DEVICE_INPUT, numThreads, device_, nullptr)
{
    InitializeThread();
}
DeviceInputWorker::~DeviceInputWorker()
{
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
    string threadName = getName() + to_string(id);
    int loopCnt = 0;  // int processCnt = 0;
    LOG_DXRT_DBG << getName() << " : Entry" << endl;

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
        LOG_DXRT_DBG << threadName << " : wake up. (" << _queue.size() << ") " << endl;
        if (_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
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
            if (SKIP_INFERENCE_IO != 1)
            {
                _device->Write(inferenceAcc->input, id);
            }
#ifdef USE_SERVICE
            std::ignore = ret;
            _device->SignalToService(inferenceAcc);
        
#else
       
            while (true)
            {
                 ret = _device->Process(cmd, inferenceAcc);
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
#endif
            
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
