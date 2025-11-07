/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/device.h"
#include "dxrt/memory.h"
#include "dxrt/model.h"
#include "dxrt/task.h"
#include "dxrt/worker.h"
#include "dxrt/buffer.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/filesys_support.h"
#include "dxrt/device_version.h"
#include "dxrt/fw.h"
#include "dxrt/multiprocess_memory.h"
#include "dxrt/npu_format_handler.h"
#ifdef __linux__
#include "dxrt/driver_adapter/linux_driver_adapter.h"
#include "dxrt/driver_adapter/network_driver_adapter.h"
#include "dxrt/driver_net.h"
#else
#include "dxrt/driver_adapter/windows_driver_adapter.h"
#endif

#include "dxrt/exception/exception.h"
#include "dxrt/objects_pool.h"
#include "dxrt/configuration.h"
#include "dxrt/npu_format_handler.h"
#include "dxrt/device_struct.h"
#include "dxrt/device_struct_operators.h"
#include "resource/log_messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#ifdef __linux__
    #include <unistd.h>
#endif
#include <errno.h>
#include <string.h>
#ifdef __linux__
    #include <sys/mman.h>
    #include <sys/ioctl.h>
#endif
#include <sys/types.h>
#include <limits>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::hex;
using std::dec;
using std::showbase;
using std::make_shared;
using std::unique_lock;
using std::mutex;


namespace dxrt {

std::atomic<bool> Device::_sNpuValidateOpt{false};

static SharedMutex requestsLock;
Device::Device(const string &file_)
: _file(file_), _profiler(Profiler::GetInstance()), _npuMemoryCacheManager(this)
{
    _name = string(_file);  // temp.
    LOG_DXRT_DBG << "Device created from " << _name << endl;
#ifdef __linux__
#elif _WIN32
    // _driverAdapter = make_shared<WindowsDriverAdapter>(_file);
#endif
    _status = dxrt_device_status_t{};
    _info = dxrt_device_info_t{};
    _devInfo = dxrt_dev_info_t{};
}

Device::~Device(void)
{
    LOG_DXRT_DBG << "Device " << _id << " start to destruction." << endl;
    _stop.store(true);
#if DXRT_USB_NETWORK_DRIVER
    {
        Process(dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE, NULL, 0);
    }
#endif
    if ((_type == DeviceType::ACC_TYPE) && (_skip == SkipMode::NONE))
    {

        if (_inputWorker)
            _inputWorker->Stop();
        if (_outputWorker)
            _outputWorker->Stop();
        if (_eventWorker)
            _eventWorker->Stop();
        // NFH workers cleanup
        if (_nfhInputWorker)
            _nfhInputWorker->Stop();
        if (_nfhOutputWorker)
            _nfhOutputWorker->Stop();
        Terminate();  // To wake up the event thread waiting in the kernel.
    }
    if (( _type == DeviceType::STD_TYPE) && (_skip == SkipMode::NONE))
    {
        Terminate();
    }

    _driverAdapter = nullptr;
    LOG_DXRT_DBG << "Device " << _id << " released." << endl;
    if (( _type == DeviceType::STD_TYPE) && (_skip == SkipMode::NONE))
    {
        _thread.join();
    }
    LOG_DXRT_DBG << "Device " << _id << " thread released. total inference count:" << _inferenceCnt  << endl;

    // Request::ShowAll();
}
/*
void *Device::input_buf(int taskId, int bufId)
{
    return _inputTensorBuffers[taskId][bufId].data();
}
*/
int Device::load()
{
    unique_lock<mutex> lk(_lock);
    return _load.load();
}

void Device::pick()
{
    unique_lock<mutex> lk(_lock);
    ++_load;
}

int Device::infCnt()
{
    unique_lock<mutex> lk(_lock);
    return _inferenceCnt;
}
#ifdef __linux__
int Device::fd()
{
    return _devFd;
}
#elif _WIN32
HANDLE Device::fd()
{
    return _devHandle;
}
#endif
dxrt_device_status_t Device::status()
{
    _status = dxrt_device_status_t{};
    Process(dxrt::dxrt_cmd_t::DXRT_CMD_GET_STATUS, &_status);
    return _status;
}

int Device::Process(dxrt_cmd_t cmd, void *data, uint32_t size, uint32_t sub_cmd, uint64_t address)
{
    int ret = 0;
#ifdef __linux__
  #if DXRT_USB_NETWORK_DRIVER
    ret = _driverAdapter->NetControl(cmd, data, size, sub_cmd, address);
  #else
    std::ignore = address;
    ret = _driverAdapter->IOControl(cmd, data, size, sub_cmd);
  #endif
    if (ret < 0)
        ret = errno*(-1);
#elif _WIN32
    ret = _driverAdapter->IOControl(cmd, data, size, sub_cmd);
#endif
    return ret;
}

int Device::InferenceRequest(RequestData* req, npu_bound_op boundOp)
{

    if (_type == DeviceType::ACC_TYPE)
    {
        // NFH Input Worker async processing (step-by-step testing)
        if (ENABLE_ASYNC_NFH_INPUT && _nfhInputWorker && req)
        {
            auto request = Request::GetById(req->requestId);
            if (request)
            {
                // NFH processing enqueued asynchronously (delegate entire processing)
                NfhInputWork nfhWork(req->requestId, request, 0, boundOp);
                int result = _nfhInputWorker->EnqueueWork(nfhWork);
                if (result != 0)
                {
                    LOG_DXRT_ERR("Failed to enqueue NFH input work for request " << req->requestId << ", falling back to sync");
                    // Fallback: proceed to synchronous processing below
                }
                else
                {
                    // NFH Worker will process, so we can exit here (prevent circular calls)
                    LOG_DXRT_DBG << "NFH Input work successfully enqueued for request " << req->requestId << endl;
                    return 0;
                }
            }
        }

        // Input Format Encoding (synchronous path also uses common utility)
        int enc = npu_format_handler::NpuFormatHandler::EncodeInputs(req, -1);
        if (enc != 0) return enc;
        return InferenceRequest_ACC(req, boundOp);
    }
    else if (_type == DeviceType::STD_TYPE)
    {
        return InferenceRequest_STD(req, boundOp);
    }

    DXRT_ASSERT(false, "Invalid Device Type");

    return -1;
}

int Device::InferenceRequest_STD(RequestData* req, npu_bound_op boundOp)
{
    std::ignore = boundOp;
    LOG_DXRT_DBG << "Device " << _id << " inference request" << endl;
    int ret = 0;
    int bufId = 0;
    auto task = req->taskData;
    int taskId = task->id();
    unique_lock<mutex> lk(_lock);
    bufId = _bufIdx[taskId];
    (++_bufIdx[taskId]) %= DEVICE_NUM_BUF;

    void* reqInputPtr = nullptr;
    if (req->inputs.size() > 0)
        reqInputPtr = req->inputs.front().data();

    {
        /* standalone device: check to memcpy & cache flush */
        auto &inferences = _npuInference[taskId];
        int pick = -1;
        // dxrt_request_t *pick = nullptr;
        for (size_t i = 0; i < inferences.size(); i++)
        {
            if (reinterpret_cast<void*>(inferences[i].input.data) == reqInputPtr)
            {
                pick = static_cast<int>(i);
                // pick = &inferences[i];
                req->outputs = _outputTensors[taskId][i];
                break;
            }
        }
        if (pick == -1)
        {
            pick = bufId;
            void *dest = reinterpret_cast<void*>(inferences[pick].input.data);
            if (reqInputPtr == nullptr)
            {
                reqInputPtr = dest;
            }
            else
            {
                LOG_DXRT_DBG << hex << "memcpy " << reqInputPtr << "-> " << dest << dec << endl;
#ifdef USE_PROFILER

                // Start profiling for overall NPU task (input preprocess + PCIe + NPU execution + output postprocess)
                auto& profiler = dxrt::Profiler::GetInstance();
                std::string profile_name = "STD Memcpy[device "+std::to_string(id())+" pick" + std::to_string(pick) + "]";
                profiler.Start(profile_name);
#endif
                memcpy(dest, reqInputPtr, task->_encodedInputSize);
                //Process(dxrt::dxrt_cmd_t::DXRT_CMD_CPU_CACHE_FLUSH, reinterpret_cast<void*>(&inferences[pick].input));
#ifdef USE_PROFILER

                profiler.End(profile_name);

#endif
            }
            req->outputs = _outputTensors[taskId][pick];
        }
        auto npu_inference = inferences[pick];
        npu_inference.req_id = req->requestId;
        // req->outputs() = _outputTensors[taskId][pick];
        {
            UniqueLock lk2(requestsLock);
            _ongoingRequestsStd[req->requestId] = npu_inference;
        }
        LOG_DXRT_DBG << "Device " << _id << " Request : " << inferences[pick] << endl;
#ifdef USE_PROFILER

        // Start profiling for overall NPU task (input preprocess + PCIe + NPU execution + output postprocess)
        auto& profiler = dxrt::Profiler::GetInstance();
        std::string profile_name_write = "STD Write[device "+std::to_string(id())+" pick" + std::to_string(pick) + "]";
        profiler.Start(profile_name_write);
#endif
#ifdef __linux__
        // ret = write(_devFd, inference, sizeof(dxrt_request_t));
        ret = _driverAdapter->Write(&npu_inference, sizeof(dxrt_request_t));
#elif _WIN32
        ret = _driverAdapter->Write(&npu_inference, sizeof(dxrt_request_t));
#endif
#ifdef USE_PROFILER
        profiler.End(profile_name_write);
#endif
        LOG_DXRT_DBG << "written " << ret << endl;
    }
    return 0;
}

int Device::InferenceRequest_ACC(RequestData* req, npu_bound_op boundOp)
{
    LOG_DXRT_DBG << "Device " << _id << " inference request" << endl;
    int ret = 0;
    int bufId = 0;
    auto task = req->taskData;
    int taskId = task->id();
    int requestId = req->requestId;

    bufId = _bufIdx[taskId];
    (++_bufIdx[taskId]) %= DEVICE_NUM_BUF;

    void* reqInputPtr = nullptr;
    if (req->inputs.size() > 0)
        reqInputPtr = req->encoded_inputs_ptr;

    {
        /* accelerator device: runtime allocation */
        dxrt_request_acc_t npu_inference_acc;
        {
            std::unique_lock<std::mutex> lock(_npuInferenceLock);
            npu_inference_acc = _npuInferenceAcc[taskId][bufId];
        }
        auto model = task->_npuModel;

        npu_inference_acc.req_id = req->requestId;
        if (reqInputPtr == nullptr)
        {
            LOG_DXRT_ERR("Device::InferenceRequest_ACC - reqInputPtr is nullptr");
            reqInputPtr = reinterpret_cast<void*>(npu_inference_acc.input.data);
        }
        else
        {
            npu_inference_acc.input.data = reinterpret_cast<uint64_t>(reqInputPtr);
        }
        {
            if (_npuMemoryCacheManager.canGetCache(task->id()))
            {
                npu_inference_acc.input.offset = _npuMemoryCacheManager.getNpuMemoryCache(task->id());
            }
            else
            {
                npu_inference_acc.input.offset = Allocate(
                    data_align(task->_encodedInputSize, 64) + task->_outputMemSize
                );
            }
        }
        // npu_inference_acc.output.data = reinterpret_cast<uint64_t>(_buffer->Get(task->_outputSize));
        if (Device::_sNpuValidateOpt.load())
        {
            _load++;
            // dxrt::RequestPtr reqPtr = Request::GetById(req->requestId);
            // req->outputs_ptr = reqPtr->task()->GetOutputBuffer();
        }
        npu_inference_acc.output.data = reinterpret_cast<uint64_t>(req->encoded_outputs_ptr);  // device buffer -> task buffer

        // auto outputOffset = sMulti_mems->Allocate(id(), ask->output_mem_size()); // Allocate(task->output_mem_size());
        // auto outputOffset = inferenceAcc.input.offset + model.output_all_offset;
        auto outputOffset = npu_inference_acc.input.offset;
        if (model.output_all_offset == 0)
            outputOffset += data_align(task->_encodedInputSize, 64);
        else outputOffset += model.output_all_offset;

        npu_inference_acc.output.offset = outputOffset + model.last_output_offset;
        npu_inference_acc.proc_id = getpid();
        npu_inference_acc.bound = boundOp;
        {
            ObjectsPool::GetInstance().GetRequestById(requestId)->setOutputs(
                task->outputs(reinterpret_cast<void*>(npu_inference_acc.output.data)));
        }
        req->outputs = task->outputs(reinterpret_cast<void*>(req->output_buffer_base));
        {
            UniqueLock lk2(requestsLock);
            _ongoingRequestsAcc[req->requestId] = npu_inference_acc;
            if (Device::_sNpuValidateOpt.load())
            {
                Request::GetById(req->requestId)->setNpuInferenceAcc(npu_inference_acc);
                auto memInfo = dxrt_meminfo_t(npu_inference_acc.output);
                LOG_DXRT_DBG << "    data: 0x" << hex << memInfo.data << endl;
                LOG_DXRT_DBG << "    base: 0x" << hex << memInfo.base << endl;
                LOG_DXRT_DBG << "    offset: 0x" << hex << memInfo.offset << endl;
                LOG_DXRT_DBG << "    size: " << dec << memInfo.size << " bytes" << endl;
            }
        }
        LOG_DXRT_DBG << "Device " << _id << " Request : " << npu_inference_acc << "Bound:" << boundOp << endl;
        // req->dev_arg() = (void*)(&_npuInferenceAcc[taskId][bufId]);
        // DXRT_ASSERT(npu_inference_acc.input.offset != 0, "wrong inferenceRequest_ACC");
        DXRT_ASSERT(ObjectsPool::GetInstance().GetRequestById(requestId)->id() == requestId, "Request ID Mismatch");
        ret = _inputWorker->request(req->requestId);

        LOG_DXRT_DBG << "request to input worker returned " << ret << endl;
    }

    // exit(1);
    return 0;
}

TensorPtrs Device::Validate(RequestPtr req, bool skipInference)
{
    LOG_DXRT_DBG << "Device::Validate - Request ID: " << req->id() << endl;
    LOG_DXRT_DBG << "  Task Name: " << req->taskData()->name() << endl;
    LOG_DXRT_DBG << "  Model Type: " << req->model_type() << endl;

    TensorPtrs ret;
    auto task = req->task();
    if (skipInference == false)
    {
        if (req->getData()->output_buffer_base == nullptr)
        {
            req->getData()->output_buffer_base = req->task()->GetOutputBuffer();
        }
        req->getData()->encoded_inputs_ptr = req->getData()->inputs.front().data();
        req->getData()->encoded_outputs_ptr = req->task()->GetEncodedOutputBuffer();

        req->getData()->BuildEncodedInputPtrs(req->taskData()->_encodedInputOffsets);
        req->getData()->BuildEncodedOutputPtrs(req->taskData()->_encodedOutputOffsets);

        InferenceRequest(req->getData());
        auto inferenceAcc = req->npu_inference_acc();
        auto memInfo = dxrt_meminfo_t(inferenceAcc.output);
        LOG_DXRT_DBG << "    data: 0x" << hex << memInfo.data << endl;
        LOG_DXRT_DBG << "    base: 0x" << hex << memInfo.base << endl;
        LOG_DXRT_DBG << "    offset: 0x" << hex << memInfo.offset << endl;
        LOG_DXRT_DBG << "    size: " << dec << memInfo.size << " bytes" << endl;
        req->Wait();
    }
    if (_type == DeviceType::STD_TYPE)
    {
        /* TODO */
        auto model = _npuModel[task->id()];
        void* ptr = _outputValidateBuffers[task->id()].data();
        ret.emplace_back(
            make_shared<Tensor>("output", vector<int64_t>{model.output_all_size},
            DataType::INT8, ptr));
    }
    else
    {
        auto inferenceAcc = req->npu_inference_acc();
        auto model = _npuModel[task->id()];
        auto memInfo = dxrt_meminfo_t(inferenceAcc.output);
        void* ptr = _outputValidateBuffers[task->id()].data();
        LOG_DXRT_DBG << "    model.last_output_offset: 0x" << hex << model.last_output_offset << endl;
        LOG_DXRT_DBG << "    memInfo.offset: 0x" << hex << memInfo.offset << endl;
        memInfo.data = reinterpret_cast<uint64_t>(ptr);
        memInfo.offset -= model.last_output_offset;
        memInfo.size = model.output_all_size;
        ret.emplace_back(
            make_shared<Tensor>("output", vector<int64_t>{memInfo.size},
            DataType::INT8, ptr));
        LOG_DXRT_DBG << *ret.back() << endl;
        DXRT_ASSERT(Read(memInfo) == 0, "Fail to read device");
        LOG_DXRT_DBG << "  Output Memory Info:" << endl;
        LOG_DXRT_DBG << "    data: 0x" << hex << memInfo.data << endl;
        LOG_DXRT_DBG << "    base: 0x" << hex << memInfo.base << endl;
        LOG_DXRT_DBG << "    offset: 0x" << hex << memInfo.offset << endl;
        LOG_DXRT_DBG << "    size: " << dec << memInfo.size << " bytes" << endl;

        LOG_DXRT_DBG << "  Encoded Input Size: " << req->taskData()->encoded_input_size() << " bytes" << endl;
        LOG_DXRT_DBG << "  Encoded Output Size: " << req->taskData()->encoded_output_size() << " bytes" << endl;
        LOG_DXRT_DBG << "  Validate Buffer size: " <<  _outputValidateBuffers[task->id()].size() << " bytes" << endl;

        if (memInfo.size == 0) memInfo = inferenceAcc.output;  // temporary solution for zero size argmax model

        if (Read(memInfo) != 0) {
            LOG_DXRT_DBG << "Validate output is empty." << endl;
            ret.clear();
        }
        req->task()->ReleaseOutputBuffer(req->getData()->output_buffer_base);
        req->task()->ReleaseEncodedOutputBuffer(req->getData()->encoded_outputs_ptr);
    }
    return ret;
}

int Device::Release(TaskData* task)
{
    int taskId = task->id();

#ifndef USE_SERVICE
    auto &model = _npuModel[taskId];
    Deallocate(model.rmap.offset);
    Deallocate(model.weight.offset);
#endif

    for (auto &inf : _npuInference[taskId])
    {
        Deallocate(inf);
    }
    _outputValidateBuffers.erase(taskId);
    if (_npuMemoryCacheManager.canGetCache(taskId))
    {
        _npuMemoryCacheManager.unRegisterMemoryCache(taskId);
    }
    return 0;
}

int Device::Response(dxrt_response_t &response)
{
    int ret;
#ifdef __linux__
    ret = _driverAdapter->Read(&response, sizeof(dxrt_response_t));
    if (ret != 0)  // if (ret != sizeof(response))
    {
        return -1;
    }
#elif _WIN32
    ret = _driverAdapter->Read(&response, sizeof(dxrt_response_t));
#endif
    LOG_DXRT_DBG << "Device " << _id << " Response : " << response.req_id << endl;
    return 0;
}

int Device::Write(dxrt_meminfo_t &meminfo)
{
#if DXRT_USB_NETWORK_DRIVER == 0
    int ch = _writeChannel.load();
    // _writeChannel = (_writeChannel+1)%3;
    _writeChannel.store((ch + 1) % 3);
    return Write(meminfo, ch);
#else
    {
        net_control_info info;
        info.address = meminfo.base + meminfo.offset;
        info.size = meminfo.size;
        info.type = 2;
        _driverAdapter->Write(&info, sizeof(info));
        _driverAdapter->Write(reinterpret_cast<void *>(meminfo.data), meminfo.size);
    }
    return 0;
#endif
}
int Device::Write(dxrt_meminfo_t &meminfo, int ch)
{
    LOG_DXRT_DBG << "Device " << _id << " Write : " << meminfo << endl;
    int ret;
    //Profiler::GetInstance().Start("Write");
#if DXRT_USB_NETWORK_DRIVER == 0
    dxrt_req_meminfo_t mem_info_req;
    mem_info_req.data = meminfo.data;
    mem_info_req.base = meminfo.base;
    mem_info_req.offset = meminfo.offset;
    mem_info_req.size = meminfo.size;
    mem_info_req.ch = ch;
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_MEM, static_cast<void*>(&mem_info_req));
#else
    ignore = ch;
    ret = _driverAdapter->NetControl(
        DXRT_CMD_WRITE_MEM,
        reinterpret_cast<void *>(meminfo.data),
        meminfo.size,
        0,
        meminfo.base + meminfo.offset);
#endif
    //Profiler::GetInstance().End("Write");
    if (ret < 0)return ret;
    return 0;
}

int Device::Read(dxrt_meminfo_t &meminfo)
{
    int ch = _readChannel.load();
    // _readChannel = (_readChannel+1)%3;
    _readChannel.store((ch + 1) % 3);
    return Read(meminfo, ch);
}

int Device::Read(_dxrt_meminfo_t &meminfo, int ch, bool ctrlCmd)
{
    LOG_DXRT_DBG << "Device " << _id << " Read : " << meminfo << endl;
    int ret;
#if DXRT_USB_NETWORK_DRIVER == 0
    dxrt_req_meminfo_t mem_info_req;
    mem_info_req.data = meminfo.data;
    mem_info_req.base = meminfo.base;
    mem_info_req.offset = meminfo.offset;
    mem_info_req.size = meminfo.size;
    mem_info_req.ch = ch;

    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_READ_MEM, static_cast<void*>(&mem_info_req));
    std::ignore = ctrlCmd;
#else
    std::ignore = ch;
    ret = _driverAdapter->NetControl(
        DXRT_CMD_READ_MEM,
        reinterpret_cast<void *>(meminfo.data),
        meminfo.size,
        0,
        meminfo.base + meminfo.offset,
        ctrlCmd);
#endif
    //Profiler::GetInstance().End("Read");

    if (ret < 0)return ret;
    return 0;
}

int Device::Wait(void)
{
    LOG_DXRT_DBG << "Device " << _id << " Wait" << endl;
    int ret = 0;
#ifdef __linux__
    ret = _driverAdapter->Poll();
    LOG_DXRT_DBG << "Device " << _id << " Wakeup" << endl;
    if (ret < 0)
    {
        LOG_DXRT << "Error: Device " << _id << "poll fail." << endl;
        return -1;
    }
#elif _WIN32
    ret = _driverAdapter->Poll();  // unused in windows
#endif
    std::ignore = ret;
    return 0;
}

void Device::BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp)
{
#if DXRT_USB_NETWORK_DRIVER == 0
    int ret;
    if (subCmd == dxrt_sche_sub_cmd_t::DX_SCHED_ADD)
    {
        _isBoundOptionSet = true;
        _setBoundOption = boundOp;
    }
    else
    {
        if (_isBoundOptionSet == false)
        {
            return;
        }
        _isBoundOptionSet = false;
        _setBoundOption = boundOp;
    }
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_SCHEDULE, reinterpret_cast<void*>(&boundOp), sizeof(dxrt_sche_sub_cmd_t), subCmd);
    DXRT_ASSERT(ret == 0, "failed to apply bound option to device");
#else
    std::ignore = subCmd;
    std::ignore = boundOp;
#endif
}
#ifdef __linux__
void Device::SelectDriver()
{
  #if DXRT_USB_NETWORK_DRIVER == 0
    {
        _driverAdapter = make_shared<LinuxDriverAdapter>(_file.c_str());
        _devFd = _driverAdapter->GetFd();
    }
  #else
    {
        _driverAdapter = make_shared<NetworkDriverAdapter>();
    }
  #endif
}
#endif

void Device::Identify(int id_, SkipMode skip, uint32_t subCmd)
{
    LOG_DXRT_DBG << "Device " << _id << " Identify" << endl;
    int ret;
    _id = id_;
#ifdef __linux__
    SelectDriver();

#elif _WIN32
    _driverAdapter = make_shared<WindowsDriverAdapter>(_file.c_str());
    _devHandle = (HANDLE)_driverAdapter->GetFd();
    if (_devHandle == INVALID_HANDLE_VALUE) {
        LOG_DXRT << "Error: Can't open " << _file << endl;
        return;
    }
#endif
    _info = dxrt_device_info_t{};
    _info.type = 0;
    _skip = skip;
    if (skip == SkipMode::IDENTIFY_SKIP) return;
#if DXRT_USB_NETWORK_DRIVER == 0
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, reinterpret_cast<void*>(&_info), 0, subCmd);
#else
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, reinterpret_cast<void*>(&_info), sizeof(_info), subCmd, true);
#endif

    if (ret != 0)
    {
        LOG_DXRT << "failed to identify device " << id_ << endl;
        _isBlocked = true;
        //return;
        throw InvalidOperationException(EXCEPTION_MESSAGE(LogMessages::Device_FailToInitialize(id_)));
    }
/*
#if DXRT_USB_NETWORK_DRIVER == 0
    {
#ifdef __linux__
        DxDeviceVersion dxVer(this, _info.fw_ver, _info.type, _info.interface, _info.variant);
#elif _WIN32
        DxDeviceVersion dxVer(this, _info.fw_ver, _info.type, _info.interface_value, _info.variant);
#endif
        _devInfo = dxVer.GetVersion();
        if ((_skip != VERSION_CHECK) && (_skip !=COMMON_SKIP))
        {
            dxVer.CheckVersion();
        }
    }
#endif
*/
    LOG_DXRT_DBG << _name << ": device info : type " << _info.type
        << hex << ", variant " << _info.variant
        << ", mem_addr " << _info.mem_addr
        << ", mem_size " << _info.mem_size
        << dec << ", num_dma_ch " << _info.num_dma_ch << endl;
    DXRT_ASSERT(_info.mem_size > 0, "invalid device memory size");
    _type = static_cast<DeviceType>(_info.type);
    _variant = _info.variant;
#ifdef __linux__
    void *_mem = _driverAdapter->MemoryMap(0, _info.mem_size, 0);
    if (reinterpret_cast<int64_t>(_mem) == -1)
    {
        _mem = nullptr;
    }
#elif _WIN32
    void* _mem = nullptr;  // unused in windows
#endif
    _memory = make_shared<Memory>(_info, _mem);
    dxrt_device_info_t featureMemInfo = _info;
    dxrt_device_info_t modelMemInfo = _info;
    featureMemInfo.mem_size = 3*(_info.mem_size / 4);
    modelMemInfo.mem_addr = featureMemInfo.mem_addr + featureMemInfo.mem_size;
    modelMemInfo.mem_size = 1*(_info.mem_size / 4);
    _featureMem = make_shared<Memory>(featureMemInfo, nullptr);
    _modelMem = make_shared<Memory>(modelMemInfo, nullptr);

    LOG_DXRT_DBG << "    Device " << _id << ": " << _info << endl;
    if (_skip == SkipMode::NONE)
    {
        if (_type == DeviceType::ACC_TYPE)
        {
            int num_ch = _info.num_dma_ch;

#ifdef __linux__
            if (_info.interface == DEVICE_INTERFACE_ASIC)
#elif _WIN32
            if (_info.interface_value == DEVICE_INTERFACE_ASIC)
#endif
            {
                int output_worker_count = num_ch;
#ifdef USE_SERVICE
                if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
                {
                    output_worker_count = DEVICE_OUTPUT_WORKER_NUM;
                }
#endif
                _inputWorker = DeviceInputWorker::Create(_name + "_input", num_ch, this);
                _outputWorker = DeviceOutputWorker::Create(_name + "_output", output_worker_count, this);

                // NFH workers created (only if enabled)
                if (ENABLE_ASYNC_NFH_INPUT)
                {
                    int nfh_input_worker_threads=GetNfhInputWorkerThreads();
                    _nfhInputWorker = NfhInputWorker::Create(_name + "_nfh_input", nfh_input_worker_threads, this);
                    LOG_DXRT_DBG << "NFH Input Worker created with " << nfh_input_worker_threads << " threads" << endl;
                }

                // NFH Output Worker enabled (prevent circular calls)
                if (ENABLE_ASYNC_NFH_OUTPUT)
                {
                    int nfh_output_worker_threads=GetNfhOutputWorkerThreads();
                    _nfhOutputWorker = NfhOutputWorker::Create(_name + "_nfh_output", nfh_output_worker_threads, this);
                    LOG_DXRT_DBG << "NFH Output Worker created with " << nfh_output_worker_threads << " threads" << endl;
                }
            }
#if DXRT_USB_NETWORK_DRIVER == 0
  #ifdef __linux__
            if (_info.interface == DEVICE_INTERFACE_ASIC)
  #elif _WIN32
            if (_info.interface_value == DEVICE_INTERFACE_ASIC)
  #endif
                _eventWorker = DeviceEventWorker::Create(_name + "event", this);
#endif

            // _buffer = make_shared<Buffer>(ACC_DEVICE_BUFFER_SIZE);

#if DXRT_USB_NETWORK_DRIVER == 0
            int data = 1;
            DoPcieCommand(&data, dxrt_pcie_sub_cmd_t::DX_CLEAR_ERR_STAT,sizeof(int));
#endif
        }
        else
        {
            _thread = std::thread(&Device::ThreadImpl, this);
        }
    }
}

void Device::Terminate()
{
    // LOG_DXRT_DBG << "Device " << _id << " terminate" << endl;

    uint32_t i;
    if (_type == DeviceType::ACC_TYPE)
    {
        if (_eventWorker == nullptr)
        {
            return;
        }

        do
        {
            for (i = 0; i < _info.num_dma_ch; i++)
            {
                dxrt_response_t data;
                data.req_id = i;
                int ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE_EVENT, &data);
                std::ignore = ret;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
#ifdef __linux__
        while (_eventWorker->isStopped() == false);
#elif _WIN32
        while (false);
#endif

    }
    else
    {
        for (i = 0; i < _info.num_dma_ch; i++)
        {
            dxrt_response_t data;
            data.req_id = i;
            int ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE_EVENT, &data);
            std::ignore = ret;
        }
    }
}

void Device::Reset(int opt)
{
    DisplayCountdown(2, "Please wait until the device reset is complete.");
#ifdef USE_SERVICE
    if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
    {
        ObjectsPool::GetInstance().GetMultiProcessMemory()->SignalDeviceReset(_id);
    }
#endif
    Process(dxrt::dxrt_cmd_t::DXRT_CMD_RESET, &opt, 4);
    LOG_DXRT << "Device reset is complete!" << endl;
}

void Device::StartDev(uint32_t option)
{
    std::ignore = option;

    uint32_t start = 1;
    Process(dxrt::dxrt_cmd_t::DXRT_CMD_START, &start, sizeof(start));
    unblock();
}

void Device::ResetBuffer(int opt)
{
    std::ignore = opt;
    _modelMem->ResetBuffer();
}

int Device::UpdateFw(string fwFile, int subCmd)
{
    DXRT_ASSERT(fileExists(fwFile), fwFile + " doesn't exist.");
    vector<uint8_t> buf(getFileSize(fwFile));
    DataFromFile(fwFile, buf.data());
    return Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_FIRMWARE, buf.data(), buf.size(), subCmd);
}

int Device::UploadFw(string fwFile, int subCmd)
{
    DXRT_ASSERT(fileExists(fwFile), fwFile + " doesn't exist.");
    vector<uint8_t> buf(getFileSize(fwFile));
    DataFromFile(fwFile, buf.data());
    return Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPLOAD_FIRMWARE, buf.data(), buf.size(), subCmd);
}

int Device::UpdateFwConfig(string jsonFile)
{
    DXRT_ASSERT(fileExists(jsonFile), jsonFile + " doesn't exist.");
    vector<uint8_t> buf(getFileSize(jsonFile));
    DataFromFile(jsonFile, buf.data());
    Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_CONFIG_JSON, buf.data(), buf.size());
    return buf[0];
}

uint32_t Device::UploadModel(string filePath, uint64_t base_addr)
{
    dxrt_meminfo_t mem;
    DXRT_ASSERT(fileExists(filePath), filePath + " doesn't exist.");
    vector<uint8_t> buf(getFileSize(filePath));
    DataFromFile(filePath, buf.data());
    mem.data = reinterpret_cast<uint64_t>(buf.data());
    mem.size = buf.size();
    mem.offset = 0;
    mem.base = base_addr;

    int ret = Write(mem);
    if (ret < 0) return 0;
    return mem.size;
}

void Device::DoPcieCommand(void *data, uint32_t subCmd, uint32_t size)
{
    dxrt_pcie_sub_cmd_t sCmd = static_cast<dxrt_pcie_sub_cmd_t>(subCmd);
    if (data == nullptr)
    {
        LOG_DXRT_ERR("Null data pointer received");
        return;
    }
    switch (sCmd) {
        case DX_GET_PCIE_INFO:
        {
            dxrt_pcie_info_t *info = static_cast<dxrt_pcie_info_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_PCIE,
                    info,
                    sizeof(dxrt_pcie_info_t),
                    sCmd);
            break;
        }
        case DX_CLEAR_ERR_STAT:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_PCIE,
                    data,
                    size,
                    sCmd);
            break;
        }
        default:
        {
            LOG_DXRT_ERR("Unknown Command");
            break;
        }
    }
}


void Device::DoCustomCommand(void *data, uint32_t subCmd, uint32_t size)
{
    dxrt_custom_sub_cmt_t sCmd = static_cast<dxrt_custom_sub_cmt_t>(subCmd);
    if (data == nullptr)
    {
        LOG_DXRT_ERR("Null data pointer received");
        return;
    }

    switch (sCmd)
    {
        case DX_SET_DDR_FREQ:
        {
            uint32_t freq = *static_cast<uint32_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    &freq,
                    sizeof(uint32_t),
                    sCmd);
            break;
        }
        case DX_GET_OTP:
        {
            otp_info_t *info = static_cast<otp_info_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    info,
                    sizeof(otp_info_t),
                    sCmd);
            break;
        }
        case DX_SET_OTP:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    size,
                    sCmd);
            break;
        }
        case DX_SET_LED:
        {
            uint32_t ledVal = *static_cast<uint32_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    &ledVal,
                    sizeof(uint32_t),
                    sCmd);
            break;
        }
        case DX_UPLOAD_MODEL:
        {
            uint32_t *model_info = static_cast<uint32_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    model_info,
                    sizeof(uint32_t)*3,
                    sCmd);
            break;
        }
        case DX_INTERNAL_TESTCASE:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    size,
                    sCmd);
            break;
        }
        //GET FCT RESULT
        case DX_GET_FCT_TESTCASE_RESULT:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    size,
                    sCmd);
            break;
        }
        case DX_RUN_FCT_TESTCASE:
        {
            uint32_t type = *static_cast<uint32_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    &type,
                    sizeof(uint32_t),
                    sCmd);
            break;
        }
        default:
            LOG_DXRT_ERR("Unknown sub command: " << sCmd);
            break;
    }
}

int64_t Device::Allocate(uint64_t size)
{
    LOG_DXRT_DBG << "Device " << _id << " allocate: " << showbase << hex << "+" << size << endl;
    unique_lock<mutex> lk(_lock);

#ifdef USE_SERVICE
    if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
    {
        if (_type == DeviceType::ACC_TYPE)
        {
            return ObjectsPool::GetInstance().GetMultiProcessMemory()->Allocate(id(), size);
        }
    }
#endif
    return _memory->Allocate(size);

}

int64_t Device::Allocate(dxrt_request_t &inference)
{
    LOG_DXRT_DBG << "Device " << _id << " allocate: " << inference << endl;
    return _memory->Allocate(inference);
}

void Device::Deallocate(uint64_t addr)
{
    LOG_DXRT_DBG << "Device " << _id << " deallocate: " << showbase << hex << addr << dec << endl;
    unique_lock<mutex> lk(_lock);

#ifdef USE_SERVICE
    if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
    {
        if (_type == DeviceType::ACC_TYPE)
        {
            ObjectsPool::GetInstance().GetMultiProcessMemory()->Deallocate(_id, addr);
            return;
        }
    }
#endif
    _memory->Deallocate(addr);

}

void Device::Deallocate_npuBuf(int64_t addr, int taskId)
{
    LOG_DXRT_DBG << "Device " << _id << " deallocate: " << showbase << hex << addr << dec << endl;

    if (_npuMemoryCacheManager.canGetCache(taskId))
    {
        _npuMemoryCacheManager.returnNpuMemoryCache(taskId, addr);
    }
    else
    {
#ifdef USE_SERVICE
        if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
        {
            ObjectsPool::GetInstance().GetMultiProcessMemory()->Deallocate(_id, addr);
        }
        else
#endif
        {
            _memory->Deallocate(addr);
        }
    }
}

void Device::Deallocate(dxrt_request_t &inference)
{
    LOG_DXRT_DBG << "Device " << _id << " deallocate: " << inference << endl;
    _memory->Deallocate(inference);
}

void Device::ThreadImpl(void)
{
    int ret = 0;
    LOG_DXRT_DBG << "Device " << _id << " thread start. " << endl;
    while (true)
    {
        if (_stop.load()) break;
        dxrt_response_t response;
        response.req_id = 0;
        LOG_DXRT_DBG << "Device " << _id << " wait. " << endl;
#ifdef USE_PROFILER
        auto& profiler = dxrt::Profiler::GetInstance();
        std::string profile_name_wait = "ThreadImpl Wait[device "+std::to_string(id())+"]";
        profiler.Start(profile_name_wait);
#endif
        ret = Wait();
#ifdef USE_PROFILER
        profiler.End(profile_name_wait);
#endif
        // cout << "Device " << _id << " wakeup : " << ret << endl;
        if (_stop.load()) break;
        // LOG_VALUE(ret);
        _profiler.End(_name);
        ret = Response(response);
        if (_stop.load()) break;
        LOG_DXRT_DBG << "Device " << _id << " got response " << response.req_id << endl;
		if ((ret == 0) & (response.req_id != 0xFFFFFFFF)) // 0xFFFFFFFF: clear value
        {
            // cout << "response " << response.req_id << ", inf time " << response.inf_time << ", load " << load() << endl;
            auto req = Request::GetById(response.req_id);
            // LOG_VALUE(req.use_count());
            if (req != nullptr)
            {
                // LOG_VALUE(req->model_type());
                if (req->model_type() == 1)
                {
                    // LOG_VALUE(resp.argmax);
                    *(static_cast<uint16_t *>(req->getData()->outputs.front().data())) = response.argmax;
                    // if (DEBUG_DATA > 0)
                    //    DataDumpBin(req->taskData()->name() + "_output.argmax.bin", req->outputs());
                }
                else if (req->model_type() == 2)
                {
                    // LOG_VALUE(resp.ppu_filter_num);

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

                    req->task()->getData()->_outputSize = req->getData()->outputs.front().shape()[1]*32;//task->output_size() setting

                    //if (DEBUG_DATA > 0)
                    //    DataDumpBin(req->taskData()->name() + "_output.ppu.bin", req->outputs());
                }

                ProcessResponse(req, &response, 1);
                CallBack();
            }
        }
    }
    LOG_DXRT_DBG << "Device " << _id << " thread end. ret:"<< ret << endl;
}

int Device::RegisterTask(TaskData* task)
{
    if (_type == DeviceType::ACC_TYPE)
    {
        return RegisterTask_ACC(task);
    }
    else if (_type == DeviceType::STD_TYPE)
    {
        return RegisterTask_STD(task);
    }
    DXRT_ASSERT(false, "Invalid Device Type");
    return -1;
}

int Device::RegisterTask_STD(TaskData* task)
{
    LOG_DXRT_DBG << "Device " << _id << endl;
    int ret = 0;
    int id = task->id();
    _bufIdx[id] = 0;
    dxrt_model_t model = task->_npuModel;
    _npuModel[id] = model;
    _npuInference[id] = vector<dxrt_request_t>();
    _npuInferenceAcc[id] = vector<dxrt_request_acc_t>();

    DXRT_ASSERT(task->input_size() > 0, "Input size is 0");
    DXRT_ASSERT(task->output_size() > 0, "Output size is 0");


    model.rmap.base = _memory->start();
    model.weight.base = _memory->start();

    if (_type == DeviceType::STD_TYPE)
    {
        model.rmap.offset = Allocate(model.rmap.size);
        model.weight.offset = Allocate(model.weight.size);
        if (model.rmap.offset > model.weight.offset)
            model.rmap.offset = Allocate(model.rmap.size);
    }

    for (int j = 0; j < DEVICE_NUM_BUF; j++)
    {
        uint32_t inference_offset = 0;
        if (_type == DeviceType::STD_TYPE){
            uint64_t allocate_size = 0;
            if (model.output_all_offset == 0)
                allocate_size = data_align(task->input_size(), 64);
            else allocate_size = model.output_all_offset;
            inference_offset = static_cast<uint32_t>(Allocate(allocate_size));
        }
        dxrt_request_t inference;
        inference.req_id = 0;
        inference.input.data = 0;
        inference.input.base = _memory->start();
        inference.input.offset = inference_offset;
        inference.input.size = task->input_size();
        inference.output.data = 0;
        inference.output.base = _memory->start();

        if (_type == DeviceType::STD_TYPE){
            inference.output.offset = static_cast<uint32_t>(Allocate(model.output_all_size));
            inference.output.size = model.output_all_size;
        }
        else
        {
            inference.output.offset = 0;
            inference.output.size = task->output_size();
        }

        // inference.output .size = task->output_size();
        inference.model_type = static_cast<uint32_t>(model.type);
        inference.model_format = static_cast<uint32_t>(model.format);
        inference.model_cmds = static_cast<uint32_t>(model.cmds);
        inference.cmd_offset = model.rmap.offset;
        inference.weight_offset = model.weight.offset;
        
        inference.last_output_offset = model.last_output_offset;

        if (_memory->data() == 0)
        {
            {
                vector<uint8_t> buf(model.output_all_size);
                _outputValidateBuffers[id] = std::move(buf);
            }
        }
        else
        {
            inference.input.data = _memory->data() + inference.input.offset;
            inference.output.data = _memory->data() + inference.output.offset + inference.last_output_offset;
            void *start = static_cast<void*>(reinterpret_cast<uint8_t*>(_memory->data()) + inference.output.offset);
            void *end = static_cast<void*>(static_cast<uint8_t*>(start) + model.output_all_size);
            // LOG_VALUE_HEX(start);
            // LOG_VALUE_HEX(end);
            // _outputValidateBuffers[id] = vector<uint8_t>((uint8_t*)(_memory->data()) + inference.output.offset, (uint8_t*)(_memory->data()) + model.output_all_size);
            if (model.output_all_size == 0) {
                LOG_DXRT_WARN << "Task " << id << " output_all_size is 0, allocating minimum buffer" << endl;
                _outputValidateBuffers[id] = vector<uint8_t>(1);  // Prevent empty vector
            } else {
                _outputValidateBuffers[id] = vector<uint8_t>(static_cast<uint8_t*>(start), static_cast<uint8_t*>(end));
            }
            // LOG_VALUE_HEX(inference.last_output_offset);
        }

        // Write RMAP (with PPU if PPCPU type)
        if (task->_isPPCPU && task->_data != nullptr && task->_data->size() >= 3) {
            // PPCPU type: combine RMAP + PPU with alignment
            uint64_t alignedRmapSize = GetAlign(model.rmap.size);
            size_t paddingSize = alignedRmapSize - model.rmap.size;
            size_t ppuSize = (*task->_data)[2].size();
            
            std::vector<uint8_t> combinedBuffer(alignedRmapSize + ppuSize, 0);
            memcpy(combinedBuffer.data(), reinterpret_cast<void*>(model.rmap.data), model.rmap.size);
            // padding is already zero-filled by vector initialization
            memcpy(combinedBuffer.data() + alignedRmapSize, (*task->_data)[2].data(), ppuSize);
            
            dxrt_meminfo_t combinedMemInfo;
            combinedMemInfo.data = reinterpret_cast<uint64_t>(combinedBuffer.data());
            combinedMemInfo.base = model.rmap.base;
            combinedMemInfo.offset = model.rmap.offset;
            combinedMemInfo.size = static_cast<uint32_t>(combinedBuffer.size());
            
            LOG_DXRT_DBG << "PPCPU: Writing combined RMAP+PPU - RMAP: " << model.rmap.size 
                         << ", padding: " << paddingSize << ", PPU: " << ppuSize 
                         << ", total: " << combinedBuffer.size() << std::endl;
            
            DXRT_ASSERT(Write(combinedMemInfo) == 0, "failed to write PPCPU combined RMAP+PPU");

            //inference.custom_offset = alignedRmapSize; // [TO DO] custom offset for V8 PPCPU
        } else {
            // Normal type or legacy PPU: write RMAP only
            DXRT_ASSERT(Write(model.rmap) == 0, "failed to write model parameters(rmap)");
            //inference.custom_offset = 0; // [TO DO] custom offset for V8 PPCPU
        }
        
        DXRT_ASSERT(Write(model.weight) == 0, "failed to write model parameters(weight)");
        // cout << "write done" << endl;
        
        _npuInference[id].emplace_back(inference);

    }

    /* Write model parameters to device */

    vector<vector<uint8_t>> readData;
    readData.emplace_back(vector<uint8_t>(model.rmap.size));
    readData.emplace_back(vector<uint8_t>(model.weight.size));
    dxrt_meminfo_t cmd(model.rmap);
    dxrt_meminfo_t weight(model.weight);
    cmd.data = reinterpret_cast<uint64_t>(readData[0].data());
    weight.data = reinterpret_cast<uint64_t>(readData[1].data());
    if (Read(cmd) == 0 )
    {
        ret += memcmp(reinterpret_cast<void*>(cmd.data), readData[0].data(), cmd.size);
    }
    if (Read(weight) == 0 )
    {
        ret += memcmp(reinterpret_cast<void*>(weight.data), readData[1].data(), weight.size);
    }

    DXRT_ASSERT(ret == 0, "failed to check data integrity of model parameters");
    // cout << "memcmp done" << endl;

    /* Embedded tensor for standalone device */
    for (auto &inf : _npuInference[id])
    {
        _inputTensors[id].emplace_back(task->inputs(reinterpret_cast<void*>(inf.input.data),
            inf.input.base + inf.input.offset));
        _outputTensors[id].emplace_back(task->outputs(reinterpret_cast<void*>(inf.output.data),
            inf.output.base + inf.output.offset));
    }

    for (const auto &v : _inputTensors[id])
        for (const auto &tensor : v)
            LOG_DXRT << tensor << endl;
    for (const auto &v : _outputTensors[id])
        for (const auto &tensor : v)
            LOG_DXRT << tensor << endl;

    return ret;
}

int Device::RegisterTask_ACC(TaskData* task)
{
    SharedLock lk(_registerTaskMutex);
    LOG_DXRT_DBG << "Device " << _id << endl;
    int ret = 0;
    int id = task->id();
    _bufIdx[id] = 0;
    dxrt_model_t model = task->_npuModel;
    _npuModel[id] = model;
    _npuInference[id] = vector<dxrt_request_t>();
    _npuInferenceAcc[id] = vector<dxrt_request_acc_t>();

    DXRT_ASSERT(task->input_size() > 0, "Input size is 0");
    DXRT_ASSERT(task->output_size() > 0, "Output size is 0");

    /* Prepare embedded parameters, tensors */

    model.rmap.base = _memory->start();
    model.weight.base = _memory->start();

#ifdef USE_SERVICE
    if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
    {
        const auto sMulti_mems = ObjectsPool::GetInstance().GetMultiProcessMemory();

        // Check total memory requirement first
        uint64_t totalRequired = model.weight.size + model.rmap.size;
        LOG_DXRT_DBG << "Task " << id << " requires " << totalRequired << " bytes total (weight: "
                     << model.weight.size << ", rmap: " << model.rmap.size << ")" << endl;

        model.weight.offset = sMulti_mems->BackwardAllocateForTask(_id, id, model.weight.size);
        if (model.weight.offset == static_cast<uint32_t>(-1)) {
            LOG_DXRT_ERR("Failed to allocate weight memory for Task " + std::to_string(id));
            throw InvalidModelException(EXCEPTION_MESSAGE("Failed to allocate weight memory for task"));
        }

        model.rmap.offset = sMulti_mems->BackwardAllocateForTask(_id, id, model.rmap.size);
        if (model.rmap.offset == static_cast<uint32_t>(-1)) {
            LOG_DXRT_ERR("Failed to allocate rmap memory for Task " + std::to_string(id));
            throw InvalidModelException(EXCEPTION_MESSAGE("Failed to allocate rmap memory for task"));
        }

        if (model.rmap.offset > model.weight.offset)
        {
            uint32_t temp_addr = model.rmap.offset;
            model.rmap.offset = sMulti_mems->BackwardAllocateForTask(_id, id, model.rmap.size);
            if (model.rmap.offset == static_cast<uint32_t>(-1)) {
                LOG_DXRT_ERR("Failed to reallocate rmap memory for Task " + std::to_string(id));
                throw InvalidModelException(EXCEPTION_MESSAGE("Failed to reallocate rmap memory for task"));
            }
            sMulti_mems->Deallocate(_id, temp_addr);
        }
    }
    else
#endif
    {
        model.weight.offset = _memory->BackwardAllocate(model.weight.size);
        model.rmap.offset = _memory->BackwardAllocate(model.rmap.size);
        if (model.rmap.offset > model.weight.offset)
        {
            uint32_t temp_addr = model.rmap.offset;
            model.rmap.offset = _memory->BackwardAllocate(model.rmap.size);
            _memory->Deallocate(temp_addr);
        }
    }

    if (_type == DeviceType::ACC_TYPE)
    {
        uint32_t modelS = model.rmap.size+model.weight.size;
        string msg = "Model Memory size is not enough(" +
            int_to_hex(modelS) + "/" +
            int_to_hex(_modelMem->size()) + ")";
        DXRT_ASSERT(_modelMem->size() > modelS, msg);
    }
    for (int j = 0; j < DEVICE_NUM_BUF; j++)
    {
        uint32_t inference_offset = 0;
        // dxrt_request_t inference;
        dxrt_request_acc_t inferenceAcc;
        memset(static_cast<void *>(&inferenceAcc), 0x00, sizeof(dxrt_request_acc_t));
        inferenceAcc.task_id = id;
        inferenceAcc.req_id = 0;
        inferenceAcc.input.data = 0;
        inferenceAcc.input.base = _memory->start();
        inferenceAcc.input.offset = inference_offset;
        inferenceAcc.input.size = task->encoded_input_size();
        inferenceAcc.output.data = 0;
        inferenceAcc.output.base = _memory->start();

        {
            // inferenceAcc.output.offset = 0;//V6
            // inferenceAcc.output.size = task->output_size();  // V6
            inferenceAcc.output.offset = model.last_output_offset;  // V7 (will be overwritten in release type inference)
            inferenceAcc.output.size = model.last_output_size;  // V7
        }
        // inference.output .size = task->output_size();
        inferenceAcc.model_type = static_cast<uint32_t>(model.type);
        inferenceAcc.model_format = static_cast<uint32_t>(model.format);
        inferenceAcc.model_cmds = static_cast<uint32_t>(model.cmds);
        inferenceAcc.cmd_offset = model.rmap.offset;
        inferenceAcc.weight_offset = model.weight.offset;
        // LOG_VALUE(_memory->data())
        if (_memory->data() == 0)
        {
            {
                vector<uint8_t> buf(model.output_all_size);
                _outputValidateBuffers[id] =  std::move(buf);
            }
        }
        else
        {
            inferenceAcc.input.data = _memory->data() + inferenceAcc.input.offset;
            inferenceAcc.output.data = _memory->data() + inferenceAcc.output.offset;
            void *start = static_cast<void*>(reinterpret_cast<uint8_t*>(_memory->data()));  // + inferenceAcc.output.offset);
            void *end = static_cast<void*>(static_cast<uint8_t*>(start) + model.output_all_size);
            // LOG_VALUE_HEX(start);
            // LOG_VALUE_HEX(end);
            // _outputValidateBuffers[id] = vector<uint8_t>((uint8_t*)(_memory->data()) + inference.output.offset, (uint8_t*)(_memory->data()) + model.output_all_size);
            if (model.output_all_size == 0) {
                LOG_DXRT_WARN << "Task " << id << " output_all_size is 0, allocating minimum buffer" << endl;
                _outputValidateBuffers[id] = vector<uint8_t>(1);  // Prevent empty vector
            } else {
                _outputValidateBuffers[id] = vector<uint8_t>(static_cast<uint8_t*>(start), static_cast<uint8_t*>(end));
            }
            // LOG_VALUE_HEX(inference.last_output_offset);
        }
        inferenceAcc.op_mode = model.op_mode;
        for (int i = 0; i < MAX_CHECKPOINT_COUNT; i++)
            inferenceAcc.datas[i] = model.checkpoints[i];
        {
            std::unique_lock<std::mutex> lk(_npuInferenceLock);
            _npuInferenceAcc[id].emplace_back(inferenceAcc);
        }
    }
    
    // Write RMAP (with PPU if PPCPU type) - ACC Type
    if (task->_isPPCPU && task->_data != nullptr && task->_data->size() >= 3) {
        // PPCPU type: combine RMAP + PPU with alignment
        uint64_t alignedRmapSize = GetAlign(model.rmap.size);
        size_t paddingSize = alignedRmapSize - model.rmap.size;
        size_t ppuSize = (*task->_data)[2].size();
        
        std::vector<uint8_t> combinedBuffer(alignedRmapSize + ppuSize, 0);
        memcpy(combinedBuffer.data(), reinterpret_cast<void*>(model.rmap.data), model.rmap.size);
        // padding is already zero-filled by vector initialization
        memcpy(combinedBuffer.data() + alignedRmapSize, (*task->_data)[2].data(), ppuSize);
        
        dxrt_meminfo_t combinedMemInfo;
        combinedMemInfo.data = reinterpret_cast<uint64_t>(combinedBuffer.data());
        combinedMemInfo.base = model.rmap.base;
        combinedMemInfo.offset = model.rmap.offset;
        combinedMemInfo.size = static_cast<uint32_t>(combinedBuffer.size());
        
        LOG_DXRT_DBG << "PPCPU ACC: Writing combined RMAP+PPU - RMAP: " << model.rmap.size 
                     << ", padding: " << paddingSize << ", PPU: " << ppuSize 
                     << ", total: " << combinedBuffer.size() << std::endl;
        
        ret = Write(combinedMemInfo);
        DXRT_ASSERT(ret == 0, "failed to write PPCPU combined RMAP+PPU"+ std::to_string(ret));
        
        // Set custom_offset for all buffers (PPCPU type)
        for (auto& inferenceAcc : _npuInferenceAcc[task->id()]) {
            inferenceAcc.custom_offset = alignedRmapSize;
        }
    } else {
        // Normal type or legacy PPU: write RMAP only
        ret = Write(model.rmap);
        DXRT_ASSERT(ret == 0, "failed to write model rmap parameters"+ std::to_string(ret));
        
        // Set custom_offset to 0 for all buffers (non-PPCPU type)
        for (auto& inferenceAcc : _npuInferenceAcc[task->id()]) {
            inferenceAcc.custom_offset = 0;
        }
    }
    
    ret = Write(model.weight);
    DXRT_ASSERT(ret== 0, "failed to write model weight parameters"+ std::to_string(ret));
    // cout << "write done" << endl;


    /* Write model parameters to device */

    vector<vector<uint8_t>> readData;
    readData.emplace_back(vector<uint8_t>(model.rmap.size));
    readData.emplace_back(vector<uint8_t>(model.weight.size));
    dxrt_meminfo_t cmd(model.rmap);
    dxrt_meminfo_t weight(model.weight);
    cmd.data = reinterpret_cast<uint64_t>(readData[0].data());
    weight.data = reinterpret_cast<uint64_t>(readData[1].data());
    if (Read(cmd) == 0 )
    {
        ret += memcmp(reinterpret_cast<void*>(cmd.data), readData[0].data(), cmd.size);
    }
    if (Read(weight) == 0 )
    {
        ret += memcmp(reinterpret_cast<void*>(weight.data), readData[1].data(), weight.size);
    }

    DXRT_ASSERT(ret == 0, "failed to check data integrity of model parameters");
    // cout << "memcmp done" << endl;
    int block_size = data_align(task->encoded_input_size(), 64) + task->_outputMemSize;

    LOG_DXRT_DBG << "Task " << id << " memory allocation summary:" << endl;
    LOG_DXRT_DBG << "  Model memory (RMAP + Weight): " << ((model.rmap.size + model.weight.size) / (1024*1024)) << "MB" << endl;
    LOG_DXRT_DBG << "  Input buffer size: " << (task->encoded_input_size() / (1024*1024)) << "MB" << endl;
    LOG_DXRT_DBG << "  Output buffer size: " << (task->_outputMemSize / (1024*1024)) << "MB" << endl;
    LOG_DXRT_DBG << "  Total per inference buffer: " << (block_size / (1024*1024)) << "MB" << endl;
    LOG_DXRT_DBG << "  Total with " << DEVICE_NUM_BUF << " buffers: " << ((block_size * DEVICE_NUM_BUF + model.rmap.size + model.weight.size) / (1024*1024)) << "MB" << endl;

    int npu_cache_count = DXRT_TASK_MAX_LOAD;
    while (npu_cache_count > 0)
    {
        if (_npuMemoryCacheManager.registerMemoryCache(task->id(), block_size, npu_cache_count) == false)
        {
            npu_cache_count--;
        }
        else
        {
            break;
        }
    }
    if (npu_cache_count < 1)
    {
        LOG_DXRT_ERR("Failed to register memory cache for task " + std::to_string(task->id()));
        ret = -1;
    }

#if DXRT_USB_NETWORK_DRIVER
    _inputWorker->UnHold();
    _outputWorker->UnHold();
#endif
    return ret;
}

void Device::CallBack()
{
    unique_lock<mutex> _lk(_lock);
    _load--;
    _inferenceCnt++;
#ifdef USE_SERVICE
    if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
        ObjectsPool::GetInstance().GetMultiProcessMemory()->SignalEndJobs(_id);
#endif

    // notify all
    _lk.unlock();
    ObjectsPool::GetInstance().AwakeDevice(_id);
}

dxrt_model_t Device::npu_model(int taskId)
{
    if (_npuModel.find(taskId) == _npuModel.end())
    {
        return {};
    }
    return _npuModel[taskId];
}

vector<Tensors> Device::inputs(int taskId)
{
    return _inputTensors[taskId];
}

dxrt_request_acc_t Device::peekInferenceAcc(uint32_t requestId)
{
    SharedLock lk(requestsLock);
    auto it = _ongoingRequestsAcc.find(requestId);
    if (it == _ongoingRequestsAcc.end())
    {
        DXRT_ASSERT(false, "peekInferenceAcc Failed "+std::to_string(requestId));
    }
    return it->second;
}

dxrt_request_t* Device::peekInferenceStd(uint32_t requestId)
{
    SharedLock lk(requestsLock);
    auto it = _ongoingRequestsStd.find(requestId);
    if (it == _ongoingRequestsStd.end())
    {
        return nullptr;
    }
    return &it->second;
}

void Device::popInferenceStruct(uint32_t requestId)
{
    UniqueLock lk(requestsLock);
    _ongoingRequestsAcc.erase(requestId);
    _ongoingRequestsStd.erase(requestId);
}

std::ostream& operator<<(std::ostream &os, const Device& device)
{
    os << "    Device[" << dec << device._id << "] " << device._name << ", load " << device._load.load()
        << ", type " << ((device._type == DeviceType::STD_TYPE)? "STD" : "ACC")
        << hex << ", variant " << device._info.variant
        << ", @ " << device._info.mem_addr << " ~ " << device._info.mem_addr + device._info.mem_size << dec << endl;
    os << *device._memory;

    return os;
}

vector<shared_ptr<Device>>& CheckDevices(SkipMode skip, uint32_t subCmd)
{
    LOG_DXRT_DBG << endl;

    auto& inst = ObjectsPool::GetInstance();
    inst.InitDevices(skip, subCmd);
    return inst.CheckDevices();

}

void WaitDeviceResponses(vector<shared_ptr<Device>> &devices_)  // temp.
{
    LOG_DXRT_DBG << endl;
    for (auto &device : devices_)
    {
        while (device->load() > 0)
        {
            LOG_DXRT << *device << endl;
            // LOG_VALUE(device->load());
        }
    }
}

std::ostream& operator<<(std::ostream& os, const dxrt_device_status_t& status)
{
    os << showbase << dec
        << "voltage [" << status.voltage[0] << ", " << status.voltage[1] << ", " << status.voltage[2] << ", " << status.voltage[3] << "], "
        << "clock [" << status.clock[0] << ", " << status.clock[1] << ", " << status.clock[2] << ", " << status.clock[3] << "], "
        << "temperature [" << status.temperature[0] << ", " << status.temperature[1] << ", " << status.temperature[2] << ", " << status.temperature[3] << "], "
        << "cnt [" << status.count[0] << ", " << status.count[1] << ", " << status.count[2] << ", " << status.count[3] << "], ";
    return os;
}

void Device::signalToWorker(int ch)
{
    std::ignore = ch;
    _inputWorker->signalToWorker();
}

void Device::signalToDevice(npu_bound_op boundOp, int taskId, uint32_t checksum)
{
#ifdef USE_SERVICE
    if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
    {
        LOG_DXRT_DBG << "WARNING: signalToDevice() is deprecated. Use Task::InitializeTaskWithService() for proper Task-based initialization." << endl;
        // Legacy support - but this should be replaced by Task::InitializeTaskWithService()
        int weightSize = _npuModel[taskId].weight.size;
        int weightOffset = _npuModel[taskId].weight.offset;
        ObjectsPool::GetInstance().GetMultiProcessMemory()->SignalDeviceInit(_id, boundOp, weightSize, weightOffset, checksum);
    }
#else
    (void)boundOp;
    (void)taskId;
    (void)checksum;
#endif
}

void Device::signalToDeviceDeInit(npu_bound_op boundOp, int taskId, uint32_t checksum)
{
#ifdef USE_SERVICE
    if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
    {
        LOG_DXRT_DBG << "WARNING: signalToDeviceDeInit() is deprecated. Use Task::CleanupTaskFromService() for proper Task-based cleanup." << endl;
        // Legacy support - but this should be replaced by Task::CleanupTaskFromService()
        int weightSize = _npuModel[taskId].weight.size;
        int weightOffset = _npuModel[taskId].weight.offset;
        ObjectsPool::GetInstance().GetMultiProcessMemory()->SignalDeviceDeInit(_id, boundOp, weightSize, weightOffset, checksum);
    }
#else
    (void)boundOp;
    (void)taskId;
    (void)checksum;
#endif
}

#ifdef USE_SERVICE
void Device::SignalToService(dxrt_request_acc_t* npu_inference_acc)
{
    ObjectsPool::GetInstance().GetMultiProcessMemory()->SignalScheduller(_id, *npu_inference_acc);
}

void Device::ProcessResponseFromService(const dxrt_response_t& resp)
{
    _outputWorker->PushWork(resp);
}
void Device::ProcessErrorFromService(dxrt_server_err_t err, int value)
{
    cout << "============================================================" << endl;
    cout << "error occured in device " << id() << endl;
    cout << " ** Reason : " <<  err <<
        "(value: " << value << ")" << endl;
    cout << " ** Take error message from server" << endl;
    cout << " ** Please restart daemon and applications" << endl;
    cout << "============================================================" << endl;
    ShowPCIEDetails();
    block();
}
#endif


void Device::ShowPCIEDetails(std::ostream& os)
{
    // check fw version
    static constexpr int MIN_PCIE_VERSION = 1700;
    static constexpr int MIN_FW_VERSION = 211;
    bool unsupportedVersion = false;
    if (_devInfo.rt_drv_ver.driver_version < MIN_PCIE_VERSION)
    {
        os << "Device " << id() << ":PCIE status is not supported due to low RT driver version "<< endl
         << LogMessages::NotSupported_DeviceDriverVersion(_devInfo.rt_drv_ver.driver_version, MIN_PCIE_VERSION) << endl;
        unsupportedVersion = true;
    }
    if (_info.fw_ver < MIN_FW_VERSION)
    {
        os << "Device " << id() << ":PCIE status is not supported due to low fw version" << endl
         << LogMessages::NotSupported_FirmwareVersion(_info.fw_ver, MIN_FW_VERSION) << endl;
        unsupportedVersion = true;
    }
    if (unsupportedVersion == true)
    {
        return;
    }

    dxrt_pcie_info_t pcieInfo;
    memset(&pcieInfo, 0, sizeof(dxrt_pcie_info_t));
    DoPcieCommand(&pcieInfo, DX_GET_PCIE_INFO, sizeof(dxrt_pcie_info_t));
    os << "Device "<< id() << " pcie status:" << endl;

    dxrt_device_status_t status_data = status();

    os << pcieInfo << dec << endl;
    os << "DDR Memory Error information";
    for (int i = 0; i < 4; i++)
    {
        os << " ch" << i << ":";
        uint32_t sbe = status_data.ddr_sbe_cnt[i];
        uint32_t dbe = status_data.ddr_dbe_cnt[i];
#if 1
        if ((sbe == 0) && (dbe == 0))
        {
            os << "None";
        }
        else
#endif
        {
            os << "SBE " << sbe
              << ",DBE " << dbe;
        }
    }
    os << endl;
}
void Device::ShowPCIEDetails()
{
    ShowPCIEDetails(cout);
}

}  // namespace dxrt
