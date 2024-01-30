// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/request.h"
#include "dxrt/device.h"
#include "dxrt/task.h"
#include "dxrt/util.h"

using namespace std;

namespace dxrt {

Worker::Worker(string name_, Type type_, int numThreads, Device *device_)
: _name(name_), _type(type_), _threads(numThreads), _device(device_)
{
    // cout << name << " will be created." << endl;
    switch(_type)
    {
        case Worker::Type::DEVICE_INPUT:
            for(int i=0;i<numThreads;i++) _threads[i] = std::thread(&Worker::DeviceInputThread, this, i);
            break;
        case Worker::Type::DEVICE_OUTPUT:
            for(int i=0;i<numThreads;i++) _threads[i] = std::thread(&Worker::DeviceOutputThread, this, i);
            // for(int i=0;i<numThreads;i++) _threads[i] = std::thread(&Device::ThreadImpl, _device);
            break;
        default:
            break;
    }
    LOG_DXRT_DBG << _name << " created." << endl;
}
Worker::Worker()
{
    LOG_DXRT_DBG << endl;
}
Worker::~Worker()
{
    Stop();
    LOG_DXRT_DBG << "Destroying " << _name << endl;
    for(int i=0;i<_threads.size();i++) _threads[i].join();
    LOG_DXRT_DBG << "Destroying " << _name << " done, " << _threads.size() << endl;
    _threads.clear();
}
shared_ptr<Worker> Worker::Create(string name_, Worker::Type type_, int numThreads, Device *device_)
{
    shared_ptr<Worker> ret = make_shared<Worker>(name_, type_, numThreads, device_);
    return ret;
}
int Worker::request(RequestPtr req)
{
    unique_lock<mutex> lk(_lock);
    _queue.push(req);
    _cv.notify_all();
    return 0;
}
void Worker::Stop(void)
{
    if(_threads.empty()) return;
    _stop = true;
    _cv.notify_all();
}

void Worker::DeviceInputThread(int id)
{
    string threadName = _name;
    int loopCnt = 0;
    LOG_DXRT_DBG << _name << " : Entry" << endl;
    int devId = _device->id();
    int ret, err;
    uint32_t type = _device->info().type;
    auto& profiler = dxrt::Profiler::GetInstance();
    dxrt_cmd_t cmd = (dxrt_cmd_t)((int)DXRT_CMD_WRITE_INPUT_DMA_CH0 + id);
    while(1)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        unique_lock<mutex> lk(_lock);
        _cv.wait(
            lk, [&] {
                return _queue.size()||_stop;
            }
        );
        LOG_DXRT_DBG << threadName << " : wake up. (" << _queue.size() << ") " << endl;
        if(_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        auto req = _queue.front();
        _queue.pop();
        lk.unlock();
        if(type==0)
        {
            // profiler.Start(threadName);
            auto &inferenceAcc = req->npu_inference_acc();
            inferenceAcc.dma_ch = id;
            // cout << inferenceAcc << endl; // for debug.
            while(1)
            {
                // cout << inferenceAcc.req_id << endl;
                ret = _device->Process(cmd, &inferenceAcc);
                if(ret==0) break;
                if(ret!=-EBUSY) // write done, but failed to enqueue
                {
                    inferenceAcc.input.data = 0;
                }
            }
            // profiler.End(threadName);
        }
        else
        {
            // profiler.Start(threadName);
            auto &inference = req->npu_inference();
            // cout << inference << endl; // for debug.
            ret = _device->Process(cmd, &inference);
            // profiler.End(threadName);
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}
void Worker::DeviceOutputThread(int id)
{
    string threadName = _name;
    int loopCnt = 0, ret;
    LOG_DXRT_DBG << _name << " : Entry" << endl;
    int devId = _device->id();
    auto& profiler = dxrt::Profiler::GetInstance();
    dxrt_cmd_t cmd = (dxrt_cmd_t)((int)DXRT_CMD_READ_OUTPUT_DMA_CH0 + id);
    while(1)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        if(_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt_response_t response{id};
        if(loopCnt>0) profiler.Start(threadName);
        ret = _device->Process(cmd, &response);
        // cout << response << endl; // for debug.
        // profiler.End(_device->name());
        if(ret==0 && !_stop)
        {
            // DXRT_ASSERT(response.status==0, "Error Detected: " + ErrTable((dxrt_error_t)response.status));            
            if(response.status!=0)
            {
                LOG_VALUE(response.status);
                string _dumpFile = "dxrt.dump.bin." + to_string(_device->id());
                cout << "Error Detected: " + ErrTable((dxrt_error_t)response.status) << endl;
                cout << "    Device " << _device->id() << " dump to file " << _dumpFile << endl;
                auto dump = _device->Dump();
                for(int i=0;i<dump.size(); i+=2)
                {
                    if(dump[i]==0xFFFFFFFF) break;
                    // cout << hex << dump[i] << " : " << dump[i+1] << endl;
                }
                dxrt::DataDumpBin(_dumpFile, dump.data(), dump.size());            
                dxrt::DataDumpTxt(_dumpFile+".txt", (uint32_t*)dump.data(), 1, dump.size()/2, 2, true);
                _stop = true;
                DXRT_ASSERT(false, "");
            }
            else
            {
                uint32_t reqId = response.req_id;
                LOG_DXRT_DBG << threadName << " : wake up by response " << reqId << endl;                
                // cout << threadName << ": " << id << ", " << reqId << ", " << response.inf_time << ", " << _device->load() << endl;
                if(reqId>0)
                {
                    auto req = Request::GetById(reqId);
                    // LOG_VALUE(req.use_count());
                    if(req!=nullptr)
                    {
                        req->task()->ProcessResponse(req, &response);
                        _device->CallBack();
                        // cout << threadName << " : " << id << ", " << reqId << ", " << req->latency() << ", " << response.inf_time << ", " << _device->load() << endl;
                    }
                }
            }            
        }
        loopCnt++;
        profiler.End(threadName);
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}

} // namespace dxrt
