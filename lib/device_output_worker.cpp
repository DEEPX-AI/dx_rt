/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

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
#include "dxrt/npu_format_handler.h"
#include <iostream>
#include <memory>
using std::endl;
using std::to_string;


namespace dxrt {

DeviceOutputWorker::DeviceOutputWorker(string name_, int numThreads, Device *device_)
: Worker(name_, Type::DEVICE_OUTPUT, numThreads, device_, nullptr)
{
    InitializeThread();
}
DeviceOutputWorker::~DeviceOutputWorker()
{
    LOG_DXRT_DBG << endl;
#ifdef USE_SERVICE
    _cv.notify_all();
#endif
}
shared_ptr<DeviceOutputWorker> DeviceOutputWorker::Create(string name_, int numThreads, Device *device_)
{
    std::shared_ptr<DeviceOutputWorker> ret = std::make_shared<DeviceOutputWorker>(name_, numThreads, device_);
    return ret;
}

#ifdef USE_SERVICE
void DeviceOutputWorker::PushWork(const dxrt_response_t& resp)
{
    std::unique_lock<std::mutex> lk(_lock);
    _queue.push(resp);
    _cv.notify_all();

}
#endif

void DeviceOutputWorker::ThreadWork(int id)
{
    std::string threadName = getName() +"_t"+ std::to_string(id);
    std::thread::id this_id = std::this_thread::get_id();
    std::ignore = this_id;
    // int dma_ch = _device->info().num_dma_ch;
    // bool cycle_ch = ((static_cast<size_t>(dma_ch) > _threads.size()) && (dma_ch > 1));

    int loopCnt = 0, ret = 0;
    std::shared_ptr<TimePoint> tp = nullptr;
    LOG_DXRT_DBG << getName() << " : Entry" << endl;
    int deviceId = _device->id();
#ifdef USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif
    dxrt_cmd_t cmd =  // static_cast<dxrt_cmd_t>(static_cast<int>(dxrt::dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH0)+id);
        dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP;

#if DXRT_USB_NETWORK_DRIVER
    do {
        ;
    } while (_hold);
#endif

#ifdef USE_SERVICE
    if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE) == false)
#endif
    _useSystemCall = true;


    while (_stop.load(std::memory_order_acquire) == false)
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
#if DXRT_USB_NETWORK_DRIVER
            ret = _device->Process(cmd, &response, sizeof(response));
#else
            ret = _device->Process(cmd, &response);
#endif
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
                string _dumpFile = "dxrt.dump.bin." + std::to_string(deviceId);
                LOG_DXRT << "Error Detected: " + ErrTable(static_cast<dxrt_error_t>(response.status)) << endl;
                LOG_DXRT << "    Device " << deviceId << " dump to file " << _dumpFile << endl;
                std::vector<uint32_t> dump(1000, 0);
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
            LOG_DXRT << "response from other process reqid: " << response.req_id
              << ", pid:" << response.proc_id << endl;
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
            req->set_processed_unit("NPU_"+std::to_string(deviceId), deviceId, response.dma_ch);
            dxrt_meminfo_t output = request_acc.output;
            if (SKIP_INFERENCE_IO != 1 || req->model_type() != 1)
            {
#ifdef USE_PROFILER
                if (response.wait_start_time > 0 && response.wait_end_time > response.wait_start_time) {
                    uint64_t inf_time_ns = static_cast<uint64_t>(response.inf_time) * 1000;
                    uint64_t wait_window = response.wait_end_time - response.wait_start_time;

                    if (wait_window - inf_time_ns > 1000000000ULL) {
                        uint64_t npu_start_ns = response.wait_end_time - inf_time_ns;
                        uint64_t npu_end_ns   = response.wait_end_time;
                        auto npu_tp = std::make_shared<TimePoint>();
                        npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(npu_start_ns));
                        npu_tp->end   = ProfilerClock::time_point(std::chrono::nanoseconds(npu_end_ns));
                        profiler.AddTimePoint("NPU Core[Job_" + to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + to_string(req->id()) + "]_" + to_string(response.dma_ch), npu_tp);
                    } else {
                        uint64_t center_ns = (response.wait_start_time + response.wait_end_time) / 2;
                        uint64_t npu_start_ns = center_ns - (inf_time_ns / 2);
                        uint64_t npu_end_ns   = center_ns + (inf_time_ns / 2);
                        auto npu_tp = std::make_shared<TimePoint>();
                        npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(npu_start_ns));
                        npu_tp->end   = ProfilerClock::time_point(std::chrono::nanoseconds(npu_end_ns));
                        profiler.AddTimePoint("NPU Core[Job_" + to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + to_string(req->id()) + "]_" + to_string(response.dma_ch), npu_tp);
                    }
                } else {
                    tp->start = tp->end - std::chrono::microseconds(response.inf_time);
                    profiler.AddTimePoint("NPU Core[Job_" + to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + to_string(req->id()) + "]_" + to_string(response.dma_ch), tp);
                }

                if (response.wait_timestamp > 0) {
                    auto wait_tp = std::make_shared<TimePoint>();
                    wait_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response.wait_start_time));
                    wait_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(response.wait_end_time));
                    profiler.AddTimePoint("Service Process Wait[Job_" + to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + to_string(req->id()) + "]_" + to_string(response.dma_ch), wait_tp);
                }
                profiler.Start("PCIe Read[Job_" + to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + to_string(req->id()) + "](" + to_string(id)+")");
                // profiler.Start("PCIe Read(" + to_string(response.dma_ch)+")");
                
#endif
                int read_ch = id;
                // if (cycle_ch)
                // {
                //     read_ch = loopCnt % dma_ch;
                //     // read_ch = loopCnt % 4;
                // }
                memset(reinterpret_cast<void*>(output.data),0, output.size );
#if DXRT_USB_NETWORK_DRIVER
                int ret2 = _device->Read(output, read_ch, false);
#else
                int ret2 = _device->Read(output, read_ch);
#endif
#ifdef USE_PROFILER
                profiler.End("PCIe Read[Job_" + to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + to_string(req->id()) + "](" + to_string(id)+")");
                // profiler.End("PCIe Read(" + to_string(response.dma_ch)+")");
                
#endif
                DXRT_ASSERT(ret2 == 0, "Failed to read output, errno="+ std::to_string(ret2) +
                    ", reqId=" + std::to_string(reqId) + ",ch:" + std::to_string(id));

            }
            _device->CallBack();
#ifdef USE_PROFILER
            {
                auto& profiler = dxrt::Profiler::GetInstance();
                std::string profile_name = "NPU Output Format Handler[Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "](" + to_string(id)+")";
                profiler.Start(profile_name);
            }
#endif
            if (DEBUG_DATA > 0)
            {
                DataDumpBin(req->taskData()->name() + "_output.bin",
                    req->encoded_outputs_ptr(), req->taskData()->encoded_output_size());
            }

            //Normal
            if (req->model_type() == 0)
            {
                RequestData* req_data = req->getData();
                // Output Format Decoding
                if (Device::_sNpuValidateOpt == false)
                {
                    for (size_t i = 0; i < req_data->outputs.size(); i++)
                    {
                        Tensor& output_tensor = req_data->outputs[i];
                        deepx_rmapinfo::TensorInfo tensor_info = req_data->taskData->_npuOutputTensorInfos[i];
                        int shape_dims = tensor_info.shape_encoded().size();
                        npu_format_handler::Bytes encoded_output = {static_cast<uint32_t>(req_data->taskData->_encodedOutputSizes[i]), static_cast<uint8_t*>(req_data->encoded_output_ptrs[i])};
                        npu_format_handler::Bytes decoded_output = {static_cast<uint32_t>(output_tensor.size_in_bytes()), static_cast<uint8_t*>(output_tensor.data())};

                        // dummy decoder
                        if (tensor_info.layout() == deepx_rmapinfo::Layout::ALIGNED)
                        {
                            // transpose
                            if (tensor_info.transpose() == deepx_rmapinfo::Transpose::TRANSPOSE_NONE)
                            {
                                LOG_DXRT_DBG <<"Output Transpose (TRANSPOSE_NONE) ["<<i<<"]" << endl;
                                npu_format_handler::NpuFormatHandler::decode_aligned(encoded_output,
                                                                                     decoded_output,
                                                                                     tensor_info.shape_encoded()[shape_dims - 1],
                                                                                     static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded())
                                                                                    );
                                LOG_DXRT_DBG <<"Output format is decoded (ALIGNED) ["<<i<<"] encoded_output size : "<<encoded_output.size<<", decoded_output size : "<<decoded_output.size<< endl;
                            }
                            else if (tensor_info.transpose() == deepx_rmapinfo::Transpose::CHANNEL_LAST_TO_FIRST)
                            {
                                npu_format_handler::NpuFormatHandler::decode_aligned(encoded_output,
                                                                                     decoded_output,
                                                                                     tensor_info.shape_encoded()[shape_dims - 1],
                                                                                     static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded())
                                                                                    );
                                LOG_DXRT_DBG << "Output format is decoded (ALIGNED) [" << i
                                  << "] encoded_output size : " << encoded_output.size
                                  << ", decoded_output size : "<<decoded_output.size << endl;
                                npu_format_handler::Bytes transposed_output = {encoded_output.size, decoded_output.data};
                                ///*
                                int col = tensor_info.shape_encoded()[shape_dims - 1];
                                int row = 1;
                                for (int j = 0; j < shape_dims - 1; j++)
                                {
                                    row *= tensor_info.shape_encoded()[j];
                                }
                                int elem_size = GetDataSize_rmapinfo_datatype(static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded()));
                                //*/
                                npu_format_handler::NpuFormatHandler::bidirectional_transpose(transposed_output.data, decoded_output.data, row, col, elem_size);

                                /*
                                npu_format_handler::NpuFormatHandler::decode_aligned_transposed(encoded_output,
                                                                                                decoded_output,
                                                                                                tensor_info.shape_encoded()[shape_dims - 1],
                                                                                                static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded()),
                                                                                                tensor_info.shape_encoded(),
                                                                                                tensor_info.transpose()
                                                                                                );
                                */
                                LOG_DXRT_DBG <<"Output format is decoded (ALIGNED+CHANNEL_LAST_TO_FIRST) [" << i
                                  << "] encoded_output size : " << encoded_output.size
                                  << ", decoded_output size : " << decoded_output.size<< endl;
                            }
                            else
                            {
                                LOG_DXRT_ERR("Invalid transpose type");
                                memcpy(static_cast<void*>(decoded_output.data),
                                        static_cast<const void*>(encoded_output.data),
                                        encoded_output.size);
                            }
                        }
                        else
                        {
                            memcpy(static_cast<void*>(decoded_output.data),
                                    static_cast<const void*>(encoded_output.data),
                                    encoded_output.size);
                        }
                    }
                }
                else
                {
                    for (size_t i = 0; i < req_data->outputs.size(); i++)
                        req_data->outputs[i].data() = req_data->encoded_output_ptrs[i];
                }
                if (DEBUG_DATA > 0)
                {
                    DataDumpBin(req->taskData()->name() + "_decoder_output.bin", req->outputs());
                }
            }

            //Argmax
            else if (req->model_type() == 1)
            {
                LOG_DXRT_DBG << "response.argmax : " << response.argmax << endl;
                *(static_cast<uint16_t *>(req->outputs().front().data())) = response.argmax;
                if (DEBUG_DATA > 0)
                    DataDumpBin(req->taskData()->name() + "_output.argmax.bin", req->outputs());
            }

            //PPU
            else if (req->model_type() == 2)
            {
                LOG_DXRT_DBG << "response.ppu_filter_num : " << response.ppu_filter_num << endl;
                RequestData* req_data = req->getData();
                if (!req_data->outputs.empty())
                {
                    memcpy(static_cast<void*>(req_data->outputs[0].data()),
                           static_cast<const void*>(req_data->encoded_output_ptrs[0]),
                           128 * 1024);
                    req_data->outputs[0].shape() = {1, response.ppu_filter_num};
                }

                DXRT_ASSERT(req_data->outputs.front().shape()[1] == response.ppu_filter_num, "PPU MODEL OUTPUT NOT VALID SET");

                if (DEBUG_DATA > 0)
                    DataDumpBin(req->taskData()->name() + "_output.ppu.bin", req->outputs());
            }
            else
            {
                DXRT_ASSERT(false, "Invalid model type (normal, argmax, ppu)");
            }
#ifdef USE_PROFILER
            {
                auto& profiler = dxrt::Profiler::GetInstance();
                std::string profile_name = "NPU Output Format Handler[Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "](" + to_string(id)+")";
                profiler.End(profile_name);
            }
#endif

            //if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
            //{
            //    cout<<"[        OUT_W][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"](---_"<<deviceId<<")[Buffer] Device Released"<<endl;
            //}
            TASK_FLOW("["+std::to_string(req->job_id())+"]"+req->taskData()->name()+" output is ready, load :"+std::to_string(_device->load()));

            _device->Deallocate_npuBuf(request_acc.input.offset, req->taskData()->id());
            ProcessResponse(req, &response, 0);

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
