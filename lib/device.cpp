// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/device.h"
#include "dxrt/memory.h"
#include "dxrt/task.h"
#include "dxrt/worker.h"
#include "dxrt/buffer.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/filesys_support.h"
#include "dxrt/device_version.h"
#include "dxrt/fw.h"
#include "dxrt/multiprocess_memory.h"
#include "dxrt/driver_adapter/linux_driver_adapter.h"
#include "dxrt/exception/exception.h"
#include "dxrt/objects_pool.h"

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
#include <sstream>
#include <iomanip>

using namespace std;

#define DEVICE_FILE "dxrt"
// #define DEVICE_POLL_LIMIT_MS 1000
// #define DEVICE_POLL_LIMIT_MS 3*1000
#define DEVICE_POLL_LIMIT_MS 3*1000*1000
#define DEVICE_NUM_BUF 2
//#define ACC_DEVICE_BUFFER_SIZE 128*1024*1024
//#define ACC_DEVICE_BUFFER_SIZE 64*1024*1024

namespace dxrt {

#ifdef USE_SERVICE
shared_ptr<MultiprocessMemory> Device::_sMulti_mems;
#endif
std::atomic<bool> Device::_sNpuValidateOpt = {false}; 

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
}

Device::~Device(void)
{
    _stop = true;
    if ((_type == DeviceType::ACC_TYPE) && (_skip == SkipMode::NONE))
    {

        if (_inputWorker)
        _inputWorker->Stop();
        if (_outputWorker)
        _outputWorker->Stop();
        if (_eventWorker)
            _eventWorker->Stop();
        Terminate();
    }
#ifdef __linux__
    _driverAdapter = nullptr;
#elif _WIN32
    CloseHandle(_devHandle);
#endif
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
    return _load;
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

int Device::fd()
{
    return _devFd;
}

dxrt_device_status_t Device::status()
{
    Process(dxrt::dxrt_cmd_t::DXRT_CMD_GET_STATUS, &_status);
    return _status;
}

int Device::Process(dxrt_cmd_t cmd, void *data, uint32_t size, uint32_t sub_cmd)
{
    int ret = 0;
#ifdef __linux__
    ret = _driverAdapter->IOControl(cmd, data, size, sub_cmd);
    if (ret < 0)
        ret = errno*(-1);
#elif _WIN32
    DWORD bytesReturned;
    BOOL success = DeviceIoControl(
        (HANDLE)_devFd,
        static_cast<DWORD>(dxrt::dxrt_ioctl_t::DXRT_IOCTL_MESSAGE),
        &msg,
        sizeof(msg),
        NULL,
        0,
        &bytesReturned,
        NULL);
    if (!success) {
        ret = GetLastError() * (-1);
    }
    else
    {
        ret = bytesReturned;
    }
#endif
    return ret;
}

int Device::InferenceRequest(RequestData* req, npu_bound_op boundOp)
{
    // cout << "InferenceRequest, load:" << _load << endl;
    // while (_load >= 6)
    // {
    //     continue;
    // }

    if(_type == DeviceType::ACC_TYPE)
    {
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
                memcpy(dest, reqInputPtr, task->_inputSize);
                Process(dxrt::dxrt_cmd_t::DXRT_CMD_CPU_CACHE_FLUSH, reinterpret_cast<void*>(&inferences[pick].input));
            }
        }
        auto npu_inference = inferences[pick];
        npu_inference.req_id = req->requestId;
        // req->outputs() = _outputTensors[taskId][pick];
        {
            UniqueLock lk2(requestsLock);
            _ongoingRequestsStd[req->requestId] = npu_inference;
        }
        LOG_DXRT_DBG << "Device " << _id << " Request : " << inferences[pick] << endl;
#ifdef __linux__
        // ret = write(_devFd, inference, sizeof(dxrt_request_t));
        ret = _driverAdapter->Write(&inferences[pick], sizeof(dxrt_request_t));
#elif _WIN32
        DWORD bytesWritten;
        BOOL success = WriteFile(_devHandle, &req->npu_inference(), sizeof(dxrt_request_t), &bytesWritten, NULL);
        if (!success) {
            ret = -1;
        }
        else {
            ret = bytesWritten;
        }
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
    
    bufId = _bufIdx[taskId];
    (++_bufIdx[taskId]) %= DEVICE_NUM_BUF;

    void* reqInputPtr = nullptr;
    if (req->inputs.size() > 0)
        reqInputPtr = req->inputs.front().data();

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
                    data_align(task->_inputSize, 64) + task->_outputMemSize
                );
            }
        }
        //npu_inference_acc.output.data = reinterpret_cast<uint64_t>(_buffer->Get(task->_outputSize));
        if(Device::_sNpuValidateOpt)
        {
            dxrt::RequestPtr reqPtr = Request::GetById(req->requestId);
            req->output_ptr = reqPtr->task()->GetOutputBuffer();
        }
        npu_inference_acc.output.data = reinterpret_cast<uint64_t>(req->output_ptr); // device buffer -> task buffer 

        // auto outputOffset =  _sMulti_mems->Allocate(id(), ask->output_mem_size()); // Allocate(task->output_mem_size());
        // auto outputOffset = inferenceAcc.input.offset + model.front().output_all_offset;
        auto outputOffset = npu_inference_acc.input.offset;
        if (model.front().output_all_offset == 0)
            outputOffset += data_align(task->_inputSize, 64);
        else outputOffset += model.front().output_all_offset;

        npu_inference_acc.output.offset = outputOffset + model.front().last_output_offset;
        npu_inference_acc.arg0 = outputOffset + model.back().last_output_offset;
        npu_inference_acc.status = 0;
        npu_inference_acc.proc_id = getpid();
        npu_inference_acc.bound = boundOp;
        req->outputs = task->outputs(reinterpret_cast<void*>(npu_inference_acc.output.data));
        {
            UniqueLock lk2(requestsLock);
            _ongoingRequestsAcc[req->requestId] = npu_inference_acc;
            if(Device::_sNpuValidateOpt)
            {
                Request::GetById(req->requestId)->setNpuInferenceAcc(npu_inference_acc);
            }
        }
        LOG_DXRT_DBG << "Device " << _id << " Request : " << npu_inference_acc << "Bound:" << boundOp << endl;
        // req->dev_arg() = (void*)(&_npuInferenceAcc[taskId][bufId]);
        //DXRT_ASSERT(npu_inference_acc.input.offset != 0, "wrong inferenceRequest_ACC");

        ret = _inputWorker->request(req->requestId);
        Profiler::GetInstance().Start(_name);

        LOG_DXRT_DBG << "request to input worker returned " << ret << endl;
    }

    // exit(1);
    return 0;
}

TensorPtrs Device::Validate(RequestPtr req, bool skipInference)
{
    TensorPtrs ret;
    auto task = req->task();
    if (skipInference == false)
    {
        // req->CheckTimePoint(0);
        InferenceRequest(req->getData());
        req->Wait();
    }
    if (_type == DeviceType::STD_TYPE)
    {
        /* TODO */
        auto model = _npuModel[task->id()];
        void* ptr = _outputValidateBuffers[task->id()].data();
        ret.emplace_back(
            make_shared<Tensor>("output", vector<int64_t>{model.front().output_all_size},
            DataType::INT8, ptr));
    }
    else
    {
        auto inferenceAcc = req->npu_inference_acc();
        auto model = _npuModel[task->id()];
        auto memInfo = dxrt_meminfo_t(inferenceAcc.output);
        void* ptr = _outputValidateBuffers[task->id()].data();
        memInfo.data = reinterpret_cast<uint64_t>(ptr);
        memInfo.offset -= model.front().last_output_offset;
        memInfo.size = model.front().output_all_size;
        ret.emplace_back(
            make_shared<Tensor>("output", vector<int64_t>{memInfo.size},
            DataType::INT8, ptr));
        //cout << *ret.back() << endl;
        //DXRT_ASSERT( Read(memInfo)==0, "Fail to read device");

        if (memInfo.size == 0) memInfo = inferenceAcc.output; //temporary solution for zero size argmax model

        if(Read(memInfo)!=0){
            LOG_DXRT_DBG << "Validate output is empty." << endl;
            ret.clear();
        }
        req->task()->FreeOutputBuffer(req->output_ptr());
    }
    return ret;
}

int Device::Release(TaskData *task)
{
    int taskId = task->id();
    for (auto &model : _npuModel[taskId])
    {
        Deallocate(model.cmd.offset);
        Deallocate(model.weight.offset);
    }
    for (auto &inf : _npuInference[taskId])
    {
        Deallocate(inf);
    }
    _outputValidateBuffers.erase(taskId);
    if (_npuMemoryCacheManager.canGetCache(taskId))
    {
        _npuMemoryCacheManager.unRegisterMemorgCache(taskId);
    }
    return 0;
}

int Device::Response(dxrt_response_t &response)
{
    int ret;
#ifdef __linux__
    ret = _driverAdapter->Read(&response, sizeof(dxrt_response_t));
    if (ret != sizeof(response))
    {
        return -1;
    }
#elif _WIN32
    DWORD bytesRead;
    BOOL success = ReadFile(_devHandle, &response, sizeof(dxrt_response_t), &bytesRead, NULL);
    if (!success || bytesRead != sizeof(dxrt_response_t))
    {
        return -1;
    }
    ret = bytesRead;
#endif
    LOG_DXRT_DBG << "Device " << _id << " Response : " << response.req_id << endl;
    return 0;
}

int Device::Write(dxrt_meminfo_t &meminfo)
{
    int ch = _writeChannel;
    _writeChannel = (_writeChannel+1)%3;
    return Write(meminfo, ch);
}

int Device::Write(dxrt_meminfo_t &meminfo, int ch)
{
    LOG_DXRT_DBG << "Device " << _id << " Write : " << meminfo << endl;
    dxrt_req_meminfo_t mem_info_req;
    mem_info_req.data = meminfo.data;
    mem_info_req.base = meminfo.base;
    mem_info_req.offset = meminfo.offset;
    mem_info_req.size = meminfo.size;
    mem_info_req.ch = ch;

    int ret;
    Profiler::GetInstance().Start("Write");
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_MEM, static_cast<void*>(&mem_info_req));
    Profiler::GetInstance().End("Write");
    if (ret < 0)return ret;
    return 0;
}

int Device::Read(dxrt_meminfo_t &meminfo)
{
    int ch = _readChannel;
    _readChannel = (_readChannel+1)%3;
    return Read(meminfo, ch);
}

int Device::Read(_dxrt_meminfo_t &meminfo, int ch)
{
    LOG_DXRT_DBG << "Device " << _id << " Read : " << meminfo << endl;
    dxrt_req_meminfo_t mem_info_req;
    mem_info_req.data = meminfo.data;
    mem_info_req.base = meminfo.base;
    mem_info_req.offset = meminfo.offset;
    mem_info_req.size = meminfo.size;
    mem_info_req.ch = ch;

    Profiler::GetInstance().Start("Read");

    //cout << "Read" << mem_info_req.ch << endl;

    int ret;
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_READ_MEM, static_cast<void*>(&mem_info_req));
    Profiler::GetInstance().End("Read");

    if (ret < 0)return ret;
    return 0;
}

int Device::Wait(void)
{
    LOG_DXRT_DBG << "Device " << _id << " Wait" << endl;
    int ret;
#ifdef __linux__
    ret = _driverAdapter->Poll();
    LOG_DXRT_DBG << "Device " << _id << " Wakeup" << endl;
    if (ret < 0)
    {
        cout << "Error: Device " << _id << "poll fail." << endl;
        return -1;
    }
#elif _WIN32
    DWORD waitResult = WaitForSingleObject(_devHandle, DEVICE_POLL_LIMIT_MS);
    if (waitResult == WAIT_FAILED)
    {
        cout << "Error: Device " << _id << " WaitForSingleObject fail." << endl;
        return -1;
    }
    else if (waitResult == WAIT_TIMEOUT)
    {
        // Timeout occurred, you might want to handle this case
        cout << "Warning: Device " << _id << " wait timeout." << endl;
        return -1;
    }
#endif
    return 0;
}

void Device::BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp)
{
    int ret;
    if(subCmd == dxrt_sche_sub_cmd_t::DX_SCHED_ADD)
    {
        _isBoundOptionSet = true;
        _setBoundOption = boundOp;
    }
    else
    {
        if(_isBoundOptionSet == false)
        {
            return;
        }
        _isBoundOptionSet = false;
        _setBoundOption = boundOp;
    }
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_SCHEDULE, reinterpret_cast<void*>(&boundOp), sizeof(dxrt_sche_sub_cmd_t), subCmd);
    DXRT_ASSERT(ret==0, "failed to apply bound option to device");
}

void Device::Identify(int id_, SkipMode skip, uint32_t subCmd)
{
    LOG_DXRT_DBG << "Device " << _id << " Identify" << endl;
    int ret;
    _id = id_;
#ifdef __linux__
    _driverAdapter = make_shared<LinuxDriverAdapter>(_file.c_str());
    _devFd = _driverAdapter->GetFd();

#elif _WIN32
    _devHandle = CreateFile(_file.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (_devHandle == INVALID_HANDLE_VALUE)
    {
        cout << "Error: Can't open " << _file << endl;
        return;
    }
#endif
#ifdef USE_SERVICE
    if (_sMulti_mems == nullptr)
    {
        _sMulti_mems = make_shared<MultiprocessMemory>();
    }
#endif
    _info = dxrt_device_info_t();
    _info.type = 0;
    _skip = skip;
    if (skip == SkipMode::IDENTIFY_SKIP) return;
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, reinterpret_cast<void*>(&_info), 0, subCmd);
    DXRT_ASSERT(ret == 0, "failed to identify device "+ to_string(id_));

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
    HANDLE fileMapping = CreateFileMapping(_devHandle, NULL, PAGE_READWRITE, 0, _info.mem_size, NULL);
    void* _mem = nullptr;
    if (fileMapping != NULL)
    {
        _mem = MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, _info.mem_size);
        CloseHandle(fileMapping);
    }
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
    if(_skip == SkipMode::NONE)
    {
        if (_type == DeviceType::ACC_TYPE)
        {
            int num_ch = _info.num_dma_ch;
            if (_info.interface == DEVICE_INTERFACE_ASIC)
            {
                _inputWorker = DeviceInputWorker::Create(_name + "_input", num_ch, this);
                _outputWorker = DeviceOutputWorker::Create(_name + "_output", num_ch, this);
            }
            if (_info.interface == DEVICE_INTERFACE_ASIC)
                _eventWorker = DeviceEventWorker::Create(_name + "event", this);

            //_buffer = make_shared<Buffer>(ACC_DEVICE_BUFFER_SIZE);
        }
        else
        {
            _thread = thread(&Device::ThreadImpl, this);
        }
    }
}

void Device::Terminate()
{
    LOG_DXRT_DBG << "Device " << _id << " terminate" << endl;

    uint32_t i;

    for(i=0;i<_info.num_dma_ch;i++)
    {
        dxrt_response_t data;
        data.req_id = i;
        int ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE, &data);
        std::ignore = ret;
    }
}

void Device::Reset(int opt)
{
    DisplayCountdown(2, "Please wait until the device reset is complete.");
#ifdef USE_SERVICE
    _sMulti_mems->SignalDeviceReset(_id);
#endif
    Process(dxrt::dxrt_cmd_t::DXRT_CMD_RESET, &opt, 4);
    LOG_DXRT << "Device reset is complete!" << endl;
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

int64_t Device::Allocate(uint64_t size)
{
    LOG_DXRT_DBG << "Device " << _id << " allocate: " << showbase << hex << "+" << size << endl;
    unique_lock<mutex> lk(_lock);

#ifdef USE_SERVICE
    if (_type == DeviceType::ACC_TYPE)
    {
        return _sMulti_mems->Allocate(id(), size);
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
    if (_type == DeviceType::ACC_TYPE)
    {
        _sMulti_mems->Deallocate(_id, addr);
        return;
    }
#else
    _memory->Deallocate(addr);
#endif
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
        _sMulti_mems->Deallocate(_id, addr);
#else
        _memory->Deallocate(addr);
#endif
    }
}

void Device::Deallocate(dxrt_request_t &inference)
{
    LOG_DXRT_DBG << "Device " << _id << " deallocate: " << inference << endl;
    _memory->Deallocate(inference);
}

void Device::ThreadImpl(void)
{
    int ret;
    LOG_DXRT_DBG << "Device " << _id << " thread start. " << endl;
    while (true)
    {
        if (_stop) break;
        dxrt_response_t response;
        response.req_id = 0;
        LOG_DXRT_DBG << "Device " << _id << " wait. " << endl;
        ret = Wait();
        // cout << "Device " << _id << " wakeup : " << ret << endl;
        if (_stop) break;
        // LOG_VALUE(ret);
        _profiler.End(_name);
        ret = Response(response);
        if (_stop) break;
        LOG_DXRT_DBG << "Device " << _id << " got response " << response.req_id << endl;
        if (ret == 0 && response.req_id > 0)
        {
            // cout << "response " << response.req_id << ", inf time " << response.inf_time << ", load " << load() << endl;
            auto req = Request::GetById(response.req_id);
            // LOG_VALUE(req.use_count());
            if (req != nullptr)
            {
                req->task()->ProcessResponse(req, &response);
                CallBack();
            }
        }
    }
    LOG_DXRT_DBG << "Device " << _id << " thread end. " << endl;
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
    vector<dxrt_model_t> models = task->_npuModel;
    _npuModel[id] = vector<dxrt_model_t>();
    _npuInference[id] = vector<dxrt_request_t>();
    _npuInferenceAcc[id] = vector<dxrt_request_acc_t>();

    DXRT_ASSERT(task->input_size() > 0, "Input size is 0");
    DXRT_ASSERT(task->output_size() > 0, "Output size is 0");

    /* Prepare embedded parameters, tensors */
    for (size_t i = 0; i < models.size(); i++)
    {
        auto model = models[i];
        model.cmd.base = _memory->start();
        model.weight.base = _memory->start();

        if (_type == DeviceType::STD_TYPE)
        {
            model.cmd.offset = Allocate(model.cmd.size);
            model.weight.offset = Allocate(model.weight.size);
            if (model.cmd.offset > model.weight.offset)
                model.cmd.offset = Allocate(model.cmd.size);
        }

        _npuModel[id].emplace_back(model);
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
            inference.cmd_offset = model.cmd.offset;
            inference.weight_offset = model.weight.offset;
            inference.last_output_offset = model.last_output_offset;

            if (_memory->data() == 0)
            {
                {
                    vector<uint8_t> buf(model.output_all_size);
                    _outputValidateBuffers[id] = move(buf);
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
                _outputValidateBuffers[id] = vector<uint8_t>(static_cast<uint8_t*>(start), static_cast<uint8_t*>(end));
                // LOG_VALUE_HEX(inference.last_output_offset);
            }

            _npuInference[id].emplace_back(inference);
        }
        DXRT_ASSERT(Write(model.cmd) == 0, "failed to write model parameters");
        DXRT_ASSERT(Write(model.weight) == 0, "failed to write model parameters");
        // cout << "write done" << endl;
    }

    /* Write model parameters to device */
    for (size_t i = 0;  i < models.size(); i++)
    {
        auto model = _npuModel[id][i];
        vector<vector<uint8_t>> readData;
        readData.emplace_back(vector<uint8_t>(model.cmd.size));
        readData.emplace_back(vector<uint8_t>(model.weight.size));
        dxrt_meminfo_t cmd(model.cmd);
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

    for (auto &v : _inputTensors[id])
        for (auto &tensor : v)
            cout << tensor << endl;
    for (auto &v : _outputTensors[id])
        for (auto &tensor : v)
            cout << tensor << endl;

    return ret;
}

int Device::RegisterTask_ACC(TaskData* task)
{
    LOG_DXRT_DBG << "Device " << _id << endl;
    int ret = 0;
    int id = task->id();
    _bufIdx[id] = 0;
    vector<dxrt_model_t> models = task->_npuModel;
    _npuModel[id] = vector<dxrt_model_t>();
    _npuInference[id] = vector<dxrt_request_t>();
    _npuInferenceAcc[id] = vector<dxrt_request_acc_t>();

    DXRT_ASSERT(task->input_size() > 0, "Input size is 0");
    DXRT_ASSERT(task->output_size() > 0, "Output size is 0");

    /* Prepare embedded parameters, tensors */
    for (size_t i = 0; i < models.size(); i++)
    {
        auto model = models[i];
        model.cmd.base = _memory->start();
        model.weight.base = _memory->start();


        {
#ifdef USE_SERVICE
            model.cmd.offset = _sMulti_mems->BackwardAllocate(_id, model.cmd.size);
            model.weight.offset = _sMulti_mems->BackwardAllocate(_id, model.weight.size);
            if (model.cmd.offset > model.weight.offset)
                model.cmd.offset = _sMulti_mems->BackwardAllocate(_id, model.cmd.size);
#else
            model.cmd.offset = _memory->BackwardAllocate(model.cmd.size);
            model.weight.offset = _memory->BackwardAllocate(model.weight.size);
            if (model.cmd.offset > model.weight.offset)
                model.cmd.offset = _memory->BackwardAllocate(model.cmd.size);
#endif
        }

        _npuModel[id].emplace_back(model);
        if (_type == DeviceType::ACC_TYPE)
        {
            uint32_t modelS = model.cmd.size+model.weight.size;
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
            inferenceAcc.req_id = 0;
            inferenceAcc.input.data = 0;
            inferenceAcc.input.base = _memory->start();
            inferenceAcc.input.offset = inference_offset;
            inferenceAcc.input.size = task->input_size();
            inferenceAcc.output.data = 0;
            inferenceAcc.output.base = _memory->start();

            {
                inferenceAcc.output.offset = 0;
                inferenceAcc.output.size = task->output_size();
            }

            // inference.output .size = task->output_size();
            inferenceAcc.model_type = static_cast<uint32_t>(model.type);
            inferenceAcc.model_format = static_cast<uint32_t>(model.format);
            inferenceAcc.model_cmds = static_cast<uint32_t>(model.cmds);
            inferenceAcc.cmd_offset = model.cmd.offset;
            inferenceAcc.weight_offset = model.weight.offset;
            //LOG_VALUE(_memory->data())
            if (_memory->data() == 0)
            {
                {
                    vector<uint8_t> buf(model.output_all_size);
                    _outputValidateBuffers[id] = move(buf);
                }
            }
            else
            {
                inferenceAcc.input.data = _memory->data() + inferenceAcc.input.offset;
                inferenceAcc.output.data = _memory->data() + inferenceAcc.output.offset;// + inferenceAcc.last_output_offset;
                void *start = static_cast<void*>(reinterpret_cast<uint8_t*>(_memory->data()) + inferenceAcc.output.offset);
                void *end = static_cast<void*>(static_cast<uint8_t*>(start) + model.output_all_size);
                // LOG_VALUE_HEX(start);
                // LOG_VALUE_HEX(end);
                // _outputValidateBuffers[id] = vector<uint8_t>((uint8_t*)(_memory->data()) + inference.output.offset, (uint8_t*)(_memory->data()) + model.output_all_size);
                _outputValidateBuffers[id] = vector<uint8_t>(static_cast<uint8_t*>(start), static_cast<uint8_t*>(end));
                // LOG_VALUE_HEX(inference.last_output_offset);
            }
            inferenceAcc.model_cmds2 = inferenceAcc.model_cmds;
            inferenceAcc.cmd_offset2 = inferenceAcc.cmd_offset;
            inferenceAcc.weight_offset2 = inferenceAcc.weight_offset;
            {
                std::unique_lock<std::mutex> lk(_npuInferenceLock);
                _npuInferenceAcc[id].emplace_back(inferenceAcc);
            }
        }
        int ret = Write(model.cmd);
        DXRT_ASSERT(ret == 0, "failed to write model cmd parameters"+ std::to_string(ret));
        ret = Write(model.weight);
        DXRT_ASSERT(ret== 0, "failed to write model weight parameters"+ std::to_string(ret));
        // cout << "write done" << endl;
    }

    /* Write model parameters to device */
    for (size_t i = 0;  i < models.size(); i++)
    {
        auto model = _npuModel[id][i];
        vector<vector<uint8_t>> readData;
        readData.emplace_back(vector<uint8_t>(model.cmd.size));
        readData.emplace_back(vector<uint8_t>(model.weight.size));
        dxrt_meminfo_t cmd(model.cmd);
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
    }
    DXRT_ASSERT(ret == 0, "failed to check data integrity of model parameters");
    // cout << "memcmp done" << endl;
    int block_size = data_align(task->input_size(), 64) + task->_outputMemSize;
    
    _npuMemoryCacheManager.registerMemoryCache(task->id(), block_size, DXRT_ASYNC_LOAD_THRE);
    return ret;
}

void Device::CallBack()
{
    unique_lock<mutex> _lk(_lock);
    _load--;
    _inferenceCnt++;
#ifdef USE_SERVICE
    _sMulti_mems->SignalEndJobs(_id);
#endif
}

vector<dxrt_model_t> Device::npu_model(int taskId)
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

dxrt_request_acc_t* Device::peekInferenceAcc(int requestId)
{
    SharedLock lk(requestsLock);
    auto it = _ongoingRequestsAcc.find(requestId);
    if (it == _ongoingRequestsAcc.end())
    {
        return nullptr;
    }
    return &it->second;
}

dxrt_request_t* Device::peekInferenceStd(int requestId)
{
    SharedLock lk(requestsLock);
    auto it = _ongoingRequestsStd.find(requestId);
    if (it == _ongoingRequestsStd.end())
    {
        return nullptr;
    }
    return &it->second;
}

void Device::popInferenceStruct(int requestId)
{
    UniqueLock lk(requestsLock);
    _ongoingRequestsAcc.erase(requestId);
    _ongoingRequestsStd.erase(requestId);
}

ostream& operator<<(ostream &os, const Device& device)
{
    os << "    Device[" << dec << device._id << "] " << device._name << ", load " << device._load
        << ", type " << ((device._type == DeviceType::STD_TYPE)? "STD" : "ACC")
        << hex << ", variant " << device._info.variant
        << ", @ " << device._info.mem_addr << " ~ " << device._info.mem_addr + device._info.mem_size << dec << endl;
    os << *device._memory;

    return os;
}

vector<shared_ptr<Device>> CheckDevices(SkipMode skip, uint32_t subCmd)
{
    LOG_DXRT_DBG << endl;
    
    auto inst = ObjectsPool::GetInstance();
    inst->InitDevices(skip,subCmd);
    return inst->CheckDevices();

}

void WaitDeviceResponses(vector<shared_ptr<Device>> &devices_) // temp.
{
    LOG_DXRT_DBG << endl;
    for(auto &device:devices_)
    {
        while(device->load()>0)
        {
            cout << *device << endl;
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
        << "dvfs [" << status.dvfs_enable << ", " << status.dvfs_maxfreq << "], "
        << "cnt [" << status.count[0] << ", " << status.count[1] << ", " << status.count[2] << ", " << status.count[3] << "], "
        << "boot_state " << status.boot_state ;
    return os;
}

void Device::signalToWorker(int ch)
{
    std::ignore = ch;
    _inputWorker->signalToWorker();
}

void Device::signalToDevice(npu_bound_op boundOp)
{
#ifdef USE_SERVICE
    _sMulti_mems->SignalDeviceInit(_id, boundOp);
#else
    (void)boundOp;
#endif
}
#ifdef USE_SERVICE

void Device::SignalToService(dxrt_request_acc_t* npu_inference_acc)
{
    _sMulti_mems->SignalScheduller(_id, *npu_inference_acc);
}

void Device::ProcessResponseFromService(const dxrt_response_t& resp)
{
    _outputWorker->PushWork(resp);
}
#endif

}  // namespace dxrt
