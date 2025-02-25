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
    int loopCnt = 0, ret=0;
    LOG_DXRT_DBG << getName() << " : Entry" << endl;
    //int devId = _device->id();
#ifdef WORKER_USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif
    dxrt_cmd_t cmd = //static_cast<dxrt_cmd_t>(static_cast<int>(dxrt::dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH0)+id);
        dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP;
    while (true)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        dxrt_response_t response;
#ifdef USE_SERVICE
        if (Configuration::GetInstance()->GetEnable(Configuration::ITEM::SERVICE))
        {
            unique_lock<mutex> lk(_lock);
            _cv.wait(
                lk, [this] {
                    return _queue.size() || _stop;
                }
            );
            LOG_DXRT_DBG << threadName << " : wake up. " << endl;
#ifdef WORKER_USE_PROFILER
            profiler.Start(threadName);
#endif
            if (_stop)
            {
                LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
                while (!_queue.empty()) {
                    _queue.pop();
                }
                auto it = find_if(_threads.begin(), _threads.end(),
                    [this_id](thread& t) { return t.get_id() == this_id; });
                if (it != _threads.end()) {
                    LOG_DXRT_DBG<<threadName<<" Detaching..."<<endl;
                    it->detach();
                    _threads.erase(it);
                    LOG_DXRT_DBG << threadName << " : removed itself from output worker threads. Remaining: " 
                        << _threads.size() <<endl;
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
#ifdef WORKER_USE_PROFILER
            if (loopCnt > 0)  profiler.Start(threadName);
#endif

            ret = _device->Process(cmd, &response);

            if (ret != 0)
            {
                continue;
            }
            if (response.status != 0)
            {
                LOG_VALUE(response.status);
                string _dumpFile = "dxrt.dump.bin." + to_string(_device->id());
                cout << "Error Detected: " + ErrTable(static_cast<dxrt_error_t>(response.status)) << endl;
                cout << "    Device " << _device->id() << " dump to file " << _dumpFile << endl;
                vector<uint32_t> dump(1000, 0);
                _device->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DUMP, dump.data());
                for (size_t i = 0; i < dump.size(); i+=2)
                {
                    if (dump[i] == 0xFFFFFFFF) break;
                    // cout << hex << dump[i] << " : " << dump[i+1] << endl;
                }
                DataDumpBin(_dumpFile, dump.data(), dump.size());
                DataDumpTxt(_dumpFile+".txt", dump.data(), 1, dump.size()/2, 2, true);
                _stop = true;
                DXRT_ASSERT(false, "");
            }
        }
        if (_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
#ifdef USE_SERVICE
            while (!_queue.empty()) {
                _queue.pop();
            }
#endif
            auto it = find_if(_threads.begin(), _threads.end(),
                [this_id](thread& t) { return t.get_id() == this_id; });
            if (it != _threads.end()) {
                LOG_DXRT_DBG<<threadName<<" Detaching..."<<endl;
                it->detach();
                _threads.erase(it);
                LOG_DXRT_DBG << threadName << " : removed itself from output worker threads. Remaining: " 
                    << _threads.size() <<endl;
            }
            break;
        }
        if (response.proc_id == 0)
        {
            continue;
        }
        int reqId = response.req_id;
        dxrt_request_acc_t* request_acc = _device->peekInferenceAcc(reqId);
        auto req = Request::GetById(reqId);
        req->set_processed_unit("NPU_"+to_string(_device->id()),response.dma_ch);

        if ((request_acc != nullptr) && (req != nullptr))
        {
            dxrt_meminfo_t output = request_acc->output;
            if (SKIP_INFERENCE_IO != 1 || req->model_type() != 1)
            {
                DXRT_ASSERT(_device->Read(output, id) == 0, "Failed to read output");
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
                *(static_cast<uint16_t *>(req->outputs().front().data())) = response.argmax;
                if (DEBUG_DATA > 0)
                    DataDumpBin(req->taskData()->name() + "_output.argmax.bin", req->outputs());
            }
            else if (req->model_type() == 2)
            {
                //LOG_VALUE(resp.ppu_filter_num);
                vector<int64_t> shape{1, response.ppu_filter_num};
                req->outputs().front().shape() = shape;
                if (DEBUG_DATA > 0)
                    DataDumpBin(req->taskData()->name() + "_output.ppu.bin", req->outputs());
            }
#ifdef WORKER_USE_PROFILER
            profiler.End(threadName);
#endif
            _device->CallBack();
            TASK_FLOW("["+to_string(req->job_id())+"]"+req->taskData()->name()+" output is ready, load :"+to_string(_device->load()));
            Profiler::GetInstance().End(_device->name());

            _device->Deallocate_npuBuf(request_acc->input.offset, req->taskData()->id());
            ProcessResponse(req, &response);


            _device->popInferenceStruct(reqId);
        }
        else
        {
#ifdef WORKER_USE_PROFILER
            profiler.End(threadName);
#endif
           // cout << "ERRORs" << reqId << endl;
            DXRT_ASSERT(false, "FAILED");

            
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;

}

}  // namespace dxrt
