// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/device.h"
#include "dxrt/util.h"
#include "dxrt/request.h"
#include "dxrt/task_data.h"
#include "dxrt/profiler.h"

namespace dxrt {


DeviceOutputWorker::DeviceOutputWorker(string name_, int numThreads, Device *device_)
: Worker(name_, Type::DEVICE_OUTPUT, numThreads, device_, nullptr)
{
    InitializeThread();
}
DeviceOutputWorker::~DeviceOutputWorker()
{
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

void DeviceOutputWorker::ThreadWork(int id)
{
    std::string threadName = getName() + to_string(id);

    LOG_DXRT_DBG << getName() << " : Entry" << endl;
    //int devId = _device->id();
#ifdef WORKER_USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif

    while (true)
    {
        unique_lock<mutex> lk(_lock);
        _cv.wait(
            lk, [this] {
                return _queue.size() || _stop;
            }
        );
        if (_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt_response_t resp = _queue.front();
        _queue.pop();
        int reqId = resp.req_id;
        dxrt_request_acc_t* request_acc = _device->peekInferenceAcc(reqId);
        auto req = Request::GetById(reqId);
        if (request_acc != nullptr)
        {
            dxrt_meminfo_t output = request_acc->output;
            DXRT_ASSERT(_device->Read(output, id) == 0, "Failed to read output");
            
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
                *(static_cast<uint16_t *>(req->outputs().front().data())) = resp.argmax;
                if (DEBUG_DATA > 0)
                    DataDumpBin(req->taskData()->name() + "_output.argmax.bin", req->outputs());
            }
            else if (req->model_type() == 2)
            {
                //LOG_VALUE(resp.ppu_filter_num);
                vector<int64_t> shape{resp.ppu_filter_num};
                req->outputs().front().shape() = shape;
                if (DEBUG_DATA > 0)
                    DataDumpBin(req->taskData()->name() + "_output.ppu.bin", req->outputs());
            }

            _device->CallBack();
            TASK_FLOW("["+to_string(req->job_id())+"]"+req->taskData()->name()+" output is ready, load :"+to_string(_device->load()));
            Profiler::GetInstance().End(_device->name());

            _device->Deallocate_npuBuf(request_acc->input.offset, req->taskData()->id());
            req->task()->ProcessResponse(req, &resp);


            _device->popInferenceStruct(reqId);
        }
        else
        {
            LOG_DXRT_I_ERR(": Error request acc is NULL");
            _stop = true;
        }
    }
}
#else
void DeviceOutputWorker::ThreadWork(int id)
{
    string threadName = getName() + to_string(id);
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
        if (_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt_response_t response;
        response.req_id = static_cast<uint32_t>(id);
#ifdef WORKER_USE_PROFILER
        if (loopCnt > 0)  profiler.Start(threadName);
#endif

        ret = _device->Process(cmd, &response);
        // cout << response << endl; // for debug.
#ifdef WORKER_USE_PROFILER
        profiler.End(_device->name());
#endif
        if (ret == 0 && !_stop)
        {
            // DXRT_ASSERT(response.status==0, "Error Detected: " + ErrTable((dxrt_error_t)response.status));
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
            else
            {
                int reqId = response.req_id;
                dxrt_request_acc_t* request_acc = _device->peekInferenceAcc(reqId);
                //DXRT_ASSERT(request_acc != nullptr, "no request found inferenceAcc map");
                LOG_DXRT_DBG << threadName << " : wake up by response " << reqId << endl;
                // cout << threadName << ": " << id << ", " << reqId << ", " << response.inf_time << ", " << _device->load() << endl;
                if ((request_acc != nullptr)&&(reqId >= 0))
                {
                    auto req = Request::GetById(reqId);
                    // LOG_VALUE(req.use_count());
                    if (req != nullptr)
                    {
                        // LOG_DXRT_DBG << threadName << " : " << id << ", " << reqId << ", " << req->latency() << ", " << response.inf_time << ", " << _device->load() << endl;
                        //output
                        dxrt_meminfo_t output = request_acc->output;
                        _device->Read(output);
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
                            // LOG_VALUE(response->argmax);
                            *(static_cast<uint16_t *>(req->outputs().front().data())) = response.argmax;
                            if (DEBUG_DATA > 0)
                                DataDumpBin(req->taskData()->name() + "_output.argmax.bin", req->outputs());
                        }
                        else if (req->model_type() == 2)
                        {
                            // LOG_VALUE(response->ppu_filter_num);
                            vector<int64_t> shape{response.ppu_filter_num};
                            req->outputs().front().shape() = shape;
                            if (DEBUG_DATA > 0)
                                DataDumpBin(req->taskData()->name() + "_output.ppu.bin", req->outputs());
                        }


                        _device->Deallocate_npuBuf(request_acc->input.offset, req->taskData()->id());
                        
                        req->task()->ProcessResponse(req, &response);
                        _device->CallBack();
                        // processCnt++;
                    }
                }
                _device->popInferenceStruct(reqId);
            }
            
        }
        loopCnt++;
#ifdef WORKER_USE_PROFILER
        profiler.End(threadName);
#endif
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}
#endif
}  // namespace dxrt
