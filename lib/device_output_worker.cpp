// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/device.h"
#include "dxrt/util.h"
#include "dxrt/request.h"
#include "dxrt/task_data.h"
#include "dxrt/profiler.h"
#include "dxrt/task.h"
#include "dxrt/request.h"
#include "dxrt/configuration.h"

namespace dxrt {


DeviceOutputWorker::DeviceOutputWorker(string name_, int numThreads, Device *device_)
: Worker(name_, Type::DEVICE_OUTPUT, numThreads, device_, nullptr)
{
    InitializeThread();
}
DeviceOutputWorker::~DeviceOutputWorker()
{
    LOG_DXRT_DBG<<endl;
#ifdef USE_SERVICE
    _cv.notify_all();
#endif
}
shared_ptr<DeviceOutputWorker> DeviceOutputWorker::Create(string name_, int numThreads, Device *device_)
{
    shared_ptr<DeviceOutputWorker> ret = make_shared<DeviceOutputWorker>(name_, numThreads, device_);
    return ret;
}

#ifdef USE_SERVICE
void DeviceOutputWorker::PushWork(const dxrt_response_t& resp)
{
    unique_lock<mutex> lk(_lock);
    _queue.push(resp);
    _cv.notify_all();

}
#endif

void DeviceOutputWorker::ThreadWork(int id)
{
    std::string threadName = getName() +"_t"+ to_string(id);
    thread::id this_id = this_thread::get_id();
    std::ignore = this_id;
    int dma_ch = _device->info().num_dma_ch;
    bool cycle_ch = ((static_cast<size_t>(dma_ch) > _threads.size()) && (dma_ch > 1));

    int loopCnt = 0, ret=0;
    std::shared_ptr<TimePoint> tp = nullptr;
    LOG_DXRT_DBG << getName() << " : Entry" << endl;
    int deviceId = _device->id();
#ifdef USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif
    dxrt_cmd_t cmd = //static_cast<dxrt_cmd_t>(static_cast<int>(dxrt::dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH0)+id);
        dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP;


#ifdef USE_SERVICE
    if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE) == false)
#endif
    _useSystemCall = true;


    while (_stop.load(memory_order_acquire) == false)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        dxrt_response_t response;
#ifdef USE_SERVICE
        if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
        {
            std::unique_lock<std::mutex> lk(_lock);
            _cv.wait(
                lk, [this] {
                    return _queue.size() || _stop.load(std::memory_order_acquire);
                }
            );
            LOG_DXRT_DBG << threadName << " : wake up. " << endl;
#ifdef USE_PROFILER
            tp = std::make_shared<TimePoint>();
            tp->end = ProfilerClock::now();
#endif
            if (_stop.load(std::memory_order_acquire))
            {
                LOG_DXRT_DBG << threadName << " : requested to stop thread." << this_id << endl;
                while (!_queue.empty()) {
                    _queue.pop();
                }

                break;
            }
            response = _queue.front();
            _queue.pop();
            
        }
        else
#endif
        {
            response.req_id = static_cast<uint32_t>(id);
            ret = _device->Process(cmd, &response);
#ifdef USE_PROFILER
            tp = std::make_shared<TimePoint>();
            tp->end = ProfilerClock::now();
#endif
            if (ret != 0)
            {
                continue;
            }
            if (response.status != 0)
            {
                LOG_VALUE(response.status);
                string _dumpFile = "dxrt.dump.bin." + to_string(deviceId);
                LOG_DXRT << "Error Detected: " + ErrTable(static_cast<dxrt_error_t>(response.status)) << endl;
                LOG_DXRT << "    Device " << deviceId << " dump to file " << _dumpFile << endl;
                vector<uint32_t> dump(1000, 0);
                _device->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DUMP, dump.data());
                for (size_t i = 0; i < dump.size(); i+=2)
                {
                    if (dump[i] == 0xFFFFFFFF) break;
                    // cout << hex << dump[i] << " : " << dump[i+1] << endl;
                }
                DataDumpBin(_dumpFile, dump.data(), dump.size());
                DataDumpTxt(_dumpFile+".txt", dump.data(), 1, dump.size()/2, 2, true);
                _stop.store(true);
                DXRT_ASSERT(false, "");
            }
            if (_stop.load(std::memory_order_acquire))
            {
                LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
                break;
            }
        }
        if (response.proc_id == 0)
        {
            continue;
        }
        if (response.proc_id != static_cast<uint32_t>(getpid()))
        {
            LOG_DXRT << "response from other process reqid: " << response.req_id << ", pid:" << response.proc_id << endl;
            continue;
        }
        uint32_t reqId = response.req_id;
        dxrt_request_acc_t request_acc = _device->peekInferenceAcc(reqId);
        auto req = Request::GetById(reqId);

        if (req == nullptr)
        {
            DXRT_ASSERT(false, "req is nullptr "+std::to_string(reqId));
        }
        if ( (req != nullptr))
        {
            req->set_processed_unit("NPU_"+to_string(deviceId),deviceId,response.dma_ch);
            dxrt_meminfo_t output = request_acc.output;
            if (SKIP_INFERENCE_IO != 1 || req->model_type() != 1)
            {
#ifdef USE_PROFILER
                tp->start = tp->end - std::chrono::microseconds(response.inf_time);
                profiler.AddTimePoint("NPU Core_" + to_string(response.dma_ch), tp);
                profiler.Start("PCIe Read(" + to_string(response.dma_ch)+")");
#endif
                int read_ch = id;
                if (cycle_ch)
                {
                    read_ch = loopCnt % dma_ch;
                }
                memset(reinterpret_cast<void*>(output.data),0, output.size );
                int ret = _device->Read(output, read_ch);
#ifdef USE_PROFILER
                profiler.End("PCIe Read(" + to_string(response.dma_ch)+")");
#endif
                DXRT_ASSERT(ret == 0, "Failed to read output, errno="+ to_string(ret) +
                    ", reqId=" + to_string(reqId) + ",ch:" + to_string(id));

            }
            if (DEBUG_DATA == 1)
            {
                dxrt::TensorPtrs outputs = _device->Validate(req, true);
                if (outputs.empty() == false){
                    DataDumpBin(req->taskData()->name() + "_output.bin", outputs);
                }
            }
            else if (DEBUG_DATA > 1)
            {
                DataDumpBin(req->taskData()->name() + "_output.bin", req->outputs());
            }
            if (req->model_type() == 1)
            {
                //LOG_VALUE(resp.argmax);
                *(static_cast<uint16_t *>(req->getData()->outputs.front().data())) = response.argmax;
                if (DEBUG_DATA > 0)
                    DataDumpBin(req->taskData()->name() + "_output.argmax.bin", req->outputs());
            }
            else if (req->model_type() == 2)
            {
                //LOG_VALUE(resp.ppu_filter_num);
               
                vector<int64_t> shape{1, response.ppu_filter_num};
                Tensors newOutput;
                Tensors oldOutput = req->outputs();
                auto fronts = oldOutput.front();
                newOutput.emplace_back(fronts.name(), shape, fronts.type(), fronts.data());
                for(size_t i = 1; i < oldOutput.size(); i++)
                {
                    newOutput.push_back(oldOutput[i]);
                }
                req->setOutputs(newOutput);
                DXRT_ASSERT(req->getData()->outputs.front().shape()[1] == response.ppu_filter_num, "PPU MODEL OUTPUT NOT VALID SET");
                
                if (DEBUG_DATA > 0)
                    DataDumpBin(req->taskData()->name() + "_output.ppu.bin", req->outputs());
            }
 
            _device->CallBack();
            if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
            {
                cout<<"[        OUT_W][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"](---_"<<deviceId<<")[Buffer] Device Released"<<endl;
            }
            TASK_FLOW("["+to_string(req->job_id())+"]"+req->taskData()->name()+" output is ready, load :"+to_string(_device->load()));
            Profiler::GetInstance().End(_device->name());

            _device->Deallocate_npuBuf(request_acc.input.offset, req->taskData()->id());
            ProcessResponse(req, &response);

            _device->popInferenceStruct(reqId);
        }
        else
        {
           // cout << "ERRORs" << reqId << endl;
            DXRT_ASSERT(false, "FAILED");
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End, Loopcount: " << loopCnt << endl;

}

}  // namespace dxrt
