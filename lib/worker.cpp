// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/request.h"
#include "dxrt/device.h"
#include "dxrt/task.h"
#include "dxrt/util.h"
#include "dxrt/cpu_handle.h"

using namespace std;

namespace dxrt {

Worker::Worker(string name_, Type type_, int numThreads, Device *device_, CpuHandle *cpuHandle_)
: _name(name_), _type(type_), _threads(numThreads), _device(device_), _cpuHandle(cpuHandle_)
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
        case Worker::Type::DEVICE_ERROR:
            for(int i=0;i<numThreads;i++) _threads[i] = std::thread(&Worker::DeviceErrorThread, this);
            break;
        case Worker::Type::CPU_HANDLE:
            for(int i=0;i<numThreads;i++) _threads[i] = std::thread(&Worker::CpuHandleThread, this);
            break;
        default:
            break;
    }
    LOG_DXRT_DBG << _name << " created." << endl;
    {
        const char* env = getenv("DXRT_DEBUG_DATA");
        if(env)
        {
            try {
                _debugData = std::stoi(env);
            } catch (const invalid_argument& e) {
                std::cerr << "Environment variable DXRT_DEBUG_DATA is invalid: " << e.what() << endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Environment variable DXRT_DEBUG_DATA is out of range: " << e.what() << endl;
            }
        }
    }
}
Worker::Worker()
{
    LOG_DXRT_DBG << endl;
}
Worker::~Worker()
{
    Stop();
    LOG_DXRT_DBG << "Destroying " << _name << endl;
    for(size_t i=0;i<_threads.size();i++) _threads[i].join();
    LOG_DXRT_DBG << "Destroying " << _name << " done, " << _threads.size() << endl;
    _threads.clear();
}
shared_ptr<Worker> Worker::Create(string name_, Worker::Type type_, int numThreads, Device *device_, CpuHandle *cpuHandle_)
{
    shared_ptr<Worker> ret = make_shared<Worker>(name_, type_, numThreads, device_, cpuHandle_);
    return ret;
}
int Worker::request(std::shared_ptr<Request> req)
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
    string threadName = _name + to_string(id);
    int loopCnt = 0;//int processCnt = 0;
    LOG_DXRT_DBG << _name << " : Entry" << endl;
    //int devId = _device->id();
    int ret;//int err;
    uint32_t type = _device->info().type;
#ifdef WORKER_USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif
    dxrt_cmd_t cmd = static_cast<dxrt_cmd_t>(static_cast<int>(dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_INPUT_DMA_CH0) + id);
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
#ifdef WORKER_USE_PROFILER
            profiler.Start(threadName);
#endif
            auto &inferenceAcc = req->npu_inference_acc();
            inferenceAcc.dma_ch = id;
            // cout << inferenceAcc << endl; // for debug.
            while(1)
            {
                // cout << inferenceAcc.req_id << endl;
                ret = _device->Process(cmd, &inferenceAcc);
                if(ret==0 || _stop)
                {
                    if(_debugData>0)
                    {
                        DataDumpBin(req->task()->name() + "_input.bin", req->inputs());
                    }
                    // processCnt++;
                    break;
                }
                if(ret!=-EBUSY) // write done, but failed to enqueue
                {
                    inferenceAcc.input.data = 0;
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
            auto &inference = req->npu_inference();
            // cout << inference << endl; // for debug.
            ret = _device->Process(cmd, &inference);
#ifdef WORKER_USE_PROFILER
            profiler.End(threadName);
#endif
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}
void Worker::DeviceOutputThread(int id)
{
    string threadName = _name + to_string(id);
    int loopCnt = 0, ret=0;
    LOG_DXRT_DBG << _name << " : Entry" << endl;
    //int devId = _device->id();
#ifdef WORKER_USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif
    dxrt_cmd_t cmd = static_cast<dxrt_cmd_t>(static_cast<int>(dxrt::dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH0) + id);
    while(1)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        if(_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt_response_t response;
        response.req_id = static_cast<uint32_t>(id);
#ifdef WORKER_USE_PROFILER
        if(loopCnt>0) profiler.Start(threadName);
#endif
        ret = _device->Process(cmd, &response);
        // cout << response << endl; // for debug.
#ifdef WORKER_USE_PROFILER
        profiler.End(_device->name());
#endif
        if(ret==0 && !_stop)
        {
            // DXRT_ASSERT(response.status==0, "Error Detected: " + ErrTable((dxrt_error_t)response.status));            
            if(response.status!=0)
            {
                LOG_VALUE(response.status);
                string _dumpFile = "dxrt.dump.bin." + to_string(_device->id());
                cout << "Error Detected: " + ErrTable(static_cast<dxrt_error_t>(response.status)) << endl;
                cout << "    Device " << _device->id() << " dump to file " << _dumpFile << endl;
                 vector<uint32_t> dump(1000, 0);
                _device->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DUMP, dump.data());
                for(size_t i=0;i<dump.size(); i+=2)
                {
                    if(dump[i]==0xFFFFFFFF) break;
                    // cout << hex << dump[i] << " : " << dump[i+1] << endl;
                }
                DataDumpBin(_dumpFile, dump.data(), dump.size());            
                DataDumpTxt(_dumpFile+".txt", static_cast<uint32_t*>(dump.data()), 1, dump.size()/2, 2, true);
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
                        // cout << threadName << " : " << id << ", " << reqId << ", " << req->latency() << ", " << response.inf_time << ", " << _device->load() << endl;
                        if(_debugData>0)
                        {
                            if(_debugData==1)
                            {
                                dxrt::TensorPtrs outputs = _device->Validate(req, true);
                                if(!outputs.empty()){
                                    DataDumpBin(req->task()->name() + "_output.bin", outputs);
                                }
                            }
                            else
                            {
                                DataDumpBin(req->task()->name() + "_output.bin", req->outputs());
                            }
                        }
                        if (req->model_type() == 1)
                        {
                            // LOG_VALUE(response->argmax);
                            *((uint16_t *)(req->outputs().front().data())) = response.argmax;
                            if(_debugData>0)
                                DataDumpBin(req->task()->name() + "_output.argmax.bin", req->outputs());
                        }
                        else if (req->model_type() == 2)
                        {
                            // LOG_VALUE(response->ppu_filter_num);
                            vector<int64_t> shape{response.ppu_filter_num};
                            req->outputs().front().shape() = shape;
                            if(_debugData>0)
                                DataDumpBin(req->task()->name() + "_output.ppu.bin", req->outputs());
                        }      
                        req->task()->ProcessResponse(req, &response);
                        _device->CallBack();
                        // processCnt++;
                    }
                }
            }
        }
        loopCnt++;
#ifdef WORKER_USE_PROFILER
        profiler.End(threadName);
#endif
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}
void Worker::DeviceErrorThread()
{
    string threadName = _name;

    int loopCnt = 0;// ret;
    LOG_DXRT_DBG << _name << " : Entry" << endl;
    //int devId = _device->id();
#ifdef WORKER_USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_ERROR;
    while(1)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        if(_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt::dxrt_error_t err;
        _device->Process(cmd, &err);
        // LOG_VALUE((int)err);
        DXRT_ASSERT(err==dxrt::dxrt_error_t::ERR_NONE, "Error Detected in device " + to_string(_device->id()) +": " + ErrTable(err));
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}
void Worker::CpuHandleThread()
{
    string threadName = _name;
    int loopCnt = 0;
    LOG_DXRT_DBG << _name << " : Entry" << endl;
#ifdef WORKER_USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif
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
        if(_debugData>0)
        {
            DataDumpBin(req->task()->name() + "_input.bin", req->inputs());
        }
        _cpuHandle->Run(req);
        if(_debugData>0)
        {
            DataDumpBin(req->task()->name() + "_output.bin", req->outputs());
            DataDumpBin(req->task()->name() + "_output_done.bin", &loopCnt, 1);           
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}
} // namespace dxrt