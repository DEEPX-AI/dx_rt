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
    RequestPtr req = Request::GetById(requestId); //for DEBUG
    _queue.push(requestId);
    //if(requestId%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || requestId%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
    //{
    //    cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<requestId<<"][Dev_"<<_device->id()<<"][Buffer] Input Notify all"<<endl;
    //}
    _cv.notify_all();

    return 0;
}

void DeviceInputWorker::ThreadWork(int id)
{
    string threadName = getName() +"_t"+ to_string(id);
    //thread::id thisId = this_thread::get_id();
    int loopCnt = 0;  // int processCnt = 0;
    LOG_DXRT_DBG << getName() << " : Entry" << endl;
    int load;
    int ret;
    uint32_t type = _device->info().type;
    int deviceId = _device->id();
    int dma_ch = _device->info().num_dma_ch;
    bool cycle_ch = ((static_cast<size_t>(dma_ch) > _threads.size()) && (dma_ch > 1));
#ifdef USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif
    dxrt_cmd_t cmd = //static_cast<dxrt_cmd_t>(static_cast<int>(dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_INPUT_DMA_CH0)+id);
        dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_REQ;
    while (_stop.load(memory_order_acquire) == false)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        unique_lock<mutex> lk(_lock);
        _cv.wait(
            lk, [this] {
                return _queue.size() || _stop.load(std::memory_order_acquire);
            }
        );
        if (_stop.load(std::memory_order_acquire))
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            while (!_queue.empty()) {
                _queue.pop();
            }

            if (id == 0)
            {
                if (SHOW_PROFILE)
                    LOG << "NPU DEVICE [" << deviceId << "] Average Input Queue Load : " << ((GetAverageLoad()-1)/(DXRT_TASK_MAX_LOAD-1)*100) <<"%"<< endl;
                else
                    LOG_DXRT_DBG << "NPU DEVICE [" << deviceId << "] Average Input Queue Load : " << ((GetAverageLoad()-1)/(DXRT_TASK_MAX_LOAD-1)*100) <<"%"<< endl;
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
            auto inferenceAcc = _device->peekInferenceAcc(requestId);
            int channel = id;
            
            if (cycle_ch)
            {
                channel = loopCnt % dma_ch; 
            }
            inferenceAcc.dma_ch = channel;
            RequestPtr req = Request::GetById(requestId);
            if (SKIP_INFERENCE_IO != 1)
            {
                TASK_FLOW("["+to_string(req->job_id())+"]"+req->taskData()->name()+" write input, load: "+to_string(load));
#ifdef USE_PROFILER
                profiler.Start("PCIe Write(" + to_string(inferenceAcc.dma_ch)+")");
#endif
                _device->Write(inferenceAcc.input, channel);
                //if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
                //{
                //    cout<<"[    IN_W     ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<deviceId<<"][Buffer] INPUT2DEV"<<endl;
                //}
#ifdef USE_PROFILER
                profiler.End("PCIe Write(" + to_string(inferenceAcc.dma_ch)+")");
#endif
            }
#ifdef USE_SERVICE
            if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
            {
                if (DEBUG_DATA > 0)
                {
                    DataDumpBin(req->taskData()->name() + "_input.bin", req->inputs());
                }
                std::ignore = ret;
                TASK_FLOW("["+to_string(req->job_id())+"]"+req->taskData()->name()+" signal to service input");

                _device->SignalToService(&inferenceAcc);
                //if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
                //{
                //    cout<<"[    IN_W     ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<deviceId<<"][Buffer] SIG2SVC"<<endl;
                //}
            }
            else
#endif
            {
                while (_stop.load(memory_order_acquire) == false)
                {
                    ret = _device->Process(cmd, &inferenceAcc);
                    LOG_DXRT_DBG << "Input signalled " << id << " " << inferenceAcc.req_id<< endl;
                    if (ret == 0 || _stop.load())
                    {
                        if (DEBUG_DATA > 0)
                        {
                            RequestPtr req = Request::GetById(requestId);
                            DataDumpBin(req->taskData()->name() + "_input.bin", req->inputs());
                        }
                        // processCnt++;
                        break;
                    }
#ifdef __linux__
                    if (ret != -EBUSY)  // write done, but failed to enqueue
#elif _WIN32
                    if (ret != ERROR_BUSY)
#endif
                    {
                        inferenceAcc.input.data = 0;
                    }
                }
            }
        }
        else
        {
#ifdef USE_PROFILER
            profiler.Start("Input Reqeust");
#endif
            auto inference = _device->peekInferenceStd(requestId); 
            LOG_DXRT_DBG << inference << endl; // for debug.
            ret = _device->Process(cmd, inference);
#ifdef USE_PROFILER
            profiler.End("Input Request");
#endif
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End, loopCount:" << loopCnt << endl;
}


void DeviceInputWorker::signalToWorker()
{
    std::unique_lock<std::mutex> lock (_lock);
    _cv.notify_all();
}

}  // namespace dxrt
