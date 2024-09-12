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
#define ACC_DEVICE_BUFFER_SIZE 128*1024*1024

namespace dxrt {
static vector<shared_ptr<Device>> devices;

Device::Device(const string &file_)
: _file(file_), _profiler(Profiler::GetInstance())
{
    _name = string(_file); // temp.
    LOG_DXRT_DBG << "Device created from " << _name << endl;
}
Device::~Device(void)
{
    _stop = true;
    if(_type==0)
    {
        _inputWorker->Stop();
        _outputWorker->Stop();
        _errorWorker->Stop();
    }    
    Terminate();
#ifdef __linux__
    close(_devFd);
#elif _WIN32
    CloseHandle(_devHandle);
#endif
    LOG_DXRT_DBG << "Device " << _id << " released." << endl;
    if(_type==1)
    {
        _thread.join();
    }
    LOG_DXRT_DBG << "Device " << _id << " thread released. total inference count:" << _inferenceCnt  << endl;

    // Request::ShowAll();
}
void *Device::input_buf(int taskId, int bufId)
{
    return _inputTensorBuffers[taskId][bufId].data();
}
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
    int ret;
    dxrt_message_t msg;
    msg.cmd = static_cast<int32_t>(cmd);
    msg.sub_cmd = static_cast<int32_t>(sub_cmd),
    msg.data = data;
    msg.size = size;

#ifdef __linux__
    ret = ioctl(_devFd, static_cast<unsigned long>(dxrt::dxrt_ioctl_t::DXRT_IOCTL_MESSAGE), &msg);
    if(ret<0)
        return errno*(-1);
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
        NULL
    );
    if (!success) {
        ret = GetLastError() * (-1);
    }
    else {
        ret = bytesReturned;
    }
#endif
    return ret;
}

int Device::InferenceRequest(RequestPtr req)
{
    LOG_DXRT_DBG << "Device " << _id << " inference request" << endl;
    int ret;
    int bufId;
    auto task = req->task();
    int taskId = req->task()->id();
    unique_lock<mutex> lk(_lock);
    bufId = _bufIdx[taskId];
    (++_bufIdx[taskId]) %= DEVICE_NUM_BUF;
    // if(_load>1)
    // {
    //     req->latency_valid() = false;
    // }
    // dxrt_request_t *inference = nullptr;
    // LOG_VALUE_HEX(req->input_ptr());
    auto reqInputPtr = req->input_ptr();
    if(_type==1)
    {
        /* standalone device: check to memcpy & cache flush */
        auto &inferences = _npuInference[taskId];
        int pick = -1;
        // dxrt_request_t *pick = nullptr;
        for(size_t i=0;i<inferences.size();i++)
        {
            if(reinterpret_cast<void*>(inferences[i].input.data) == reqInputPtr)
            {
                pick = static_cast<int>(i);
                // pick = &inferences[i];
                // req->outputs() = _outputTensors[taskId][i];
                break;
            }
        }
        if(pick==-1)
        {
            pick = bufId;
            void *dest = reinterpret_cast<void*>(inferences[pick].input.data);
            if(reqInputPtr==nullptr)
            {
                reqInputPtr = dest;
            }
            else
            {
                LOG_DXRT_DBG << hex << "memcpy " << reqInputPtr << "-> " << dest << dec << endl;
                memcpy(dest, reqInputPtr, task->input_size());
                Process(dxrt::dxrt_cmd_t::DXRT_CMD_CPU_CACHE_FLUSH, reinterpret_cast<void*>(&inferences[pick].input));
            }
        }
        req->npu_inference() = inferences[pick];
        req->npu_inference().req_id = req->id();
        req->outputs() = _outputTensors[taskId][pick];
        // for(auto &candidate : _npuInference[taskId])
        // {
        //     if(reinterpret_cast<void*>(candidate.input.data) == reqInputPtr)
        //     {
        //         pick = &candidate;
        //         break;
        //     }
        // }
        // if(pick==nullptr)
        // {
        //     pick = &_npuInference[taskId][bufId];
        //     if(reqInputPtr!=nullptr)
        //     {
        //         void *dest = reinterpret_cast<void*>(pick->input.data);
        //         LOG_DXRT_DBG << hex << "memcpy " << reqInputPtr << "-> " << dest << dec << endl;
        //         memcpy(dest, reqInputPtr, task->input_size());
        //         Process(dxrt::dxrt_cmd_t::DXRT_CMD_CPU_CACHE_FLUSH, reinterpret_cast<void*>(&pick->input));
        //     }
        //     else
        //     {
        //         reqInputPtr = reinterpret_cast<void*>(pick->input.data);
        //     }
        // }
        // DXRT_ASSERT(pick!=nullptr, "invalid inference request to device");
        // if(req->outputs().empty())
        //     req->outputs() = _outputTensors[taskId][bufId];
        // // req->npu_inference_ptr() = inference;
        // req->npu_inference() = *pick;
        // req->npu_inference().req_id = req->id();
    }
    else
    {
#if 0
        /* accelerator device: bypass current buf. */
        inferenceAcc = &_npuInferenceAcc[taskId][bufId];
        DXRT_ASSERT(inferenceAcc!=nullptr, "invalid inference request to device");
        inferenceAcc->req_id = req->id();
        inferenceAcc->input.data = reinterpret_cast<uint64_t>(reqInputPtr);
        if(req->outputs().empty())
            req->outputs() = _outputTensors[taskId][bufId];
#else
        /* accelerator device: runtime allocation */
        req->npu_inference_acc() = _npuInferenceAcc[taskId][0];
        auto model = task->npu_model();
        auto &inferenceAcc = req->npu_inference_acc();        
        inferenceAcc.req_id = req->id();        
        if(reqInputPtr==nullptr)
        {
            reqInputPtr = reinterpret_cast<void*>(inferenceAcc.input.data);
        }
        else
        {
            inferenceAcc.input.data = reinterpret_cast<uint64_t>(reqInputPtr);
        }
        inferenceAcc.input.offset = _featureMem->GetBufferAsOffset(
            data_align(task->input_size(), 64) + task->output_mem_size()
        );
        inferenceAcc.output.data = reinterpret_cast<uint64_t>(_buffer->Get(task->output_size()));
        // auto outputOffset = _featureMem->GetBufferAsOffset(task->output_mem_size()); // Allocate(task->output_mem_size());
        // auto outputOffset = inferenceAcc.input.offset + model.front().output_all_offset;
        auto outputOffset = inferenceAcc.input.offset + (model.front().output_all_offset==0?data_align(task->input_size(), 64):model.front().output_all_offset);
        inferenceAcc.output.offset = outputOffset + model.front().last_output_offset;
        inferenceAcc.arg0 = outputOffset + model.back().last_output_offset;
        inferenceAcc.status = 0;
        req->outputs() = task->outputs(reinterpret_cast<void*>(inferenceAcc.output.data));
#endif
    }
    if(_type==1)
    {
        LOG_DXRT_DBG << "Device " << _id << " Request : " << req->npu_inference() << endl;
#ifdef __linux__
        // ret = write(_devFd, inference, sizeof(dxrt_request_t));
        ret = write(_devFd, &req->npu_inference(), sizeof(dxrt_request_t));
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
    else
    {
        LOG_DXRT_DBG << "Device " << _id << " Request : " << req->npu_inference_acc() << endl;
        // req->dev_arg() = (void*)(&_npuInferenceAcc[taskId][bufId]);
        ret = _inputWorker->request(req);
        LOG_DXRT_DBG << "request to input worker returned " << ret << endl;
    }
    // _profiler.Start(_name);
    // exit(1);
    return 0;
}
TensorPtrs Device::Validate(RequestPtr req, bool skipInference)
{
    TensorPtrs ret;
    auto task = req->task();
    if(!skipInference)
    {
        // req->CheckTimePoint(0);
        InferenceRequest(req);
        req->Wait();
    }
    if(_type==1)
    {
        /* TODO */
        auto model = _npuModel[task->id()];
        void* ptr = _outputValidateBuffers[task->id()].data();
        ret.emplace_back(
            make_shared<Tensor>("output", vector<int64_t>{model.front().output_all_size}, DataType::INT8, ptr)
        );
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
        // cout << memInfo << endl;
        // LOG_VALUE(model.front().output_all_size);
        ret.emplace_back(
            make_shared<Tensor>("output", vector<int64_t>{memInfo.size}, DataType::INT8, ptr)
        );
        // cout << *ret.back() << endl;
        //DXRT_ASSERT( Read(memInfo)==0, "Fail to read device");

        if (memInfo.size == 0) memInfo = inferenceAcc.output; //temporary solution for zero size argmax model

        if(Read(memInfo)!=0){
            LOG_DXRT_DBG << "Validate output is empty." << endl;
            ret.clear();
        }
    }
    return ret;
}
int Device::Release(Task *task)
{
    int taskId = task->id();
    for(auto &model : _npuModel[taskId])
    {
        Deallocate(model.cmd.offset);
        Deallocate(model.weight.offset);
    }
    for(auto &inf : _npuInference[taskId])
    {
        Deallocate(inf);
    }
    _outputValidateBuffers.erase(taskId);
    return 0;
}
int Device::Response(dxrt_response_t &response)
{    
    int ret;
#ifdef __linux__
    ret = read(_devFd, &response, sizeof(dxrt_response_t));
    if(ret!=sizeof(response))
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
    LOG_DXRT_DBG << "Device " << _id << " Write : " << meminfo << endl;
    int ret;
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_MEM, reinterpret_cast<void*>(&meminfo));
    if (ret<0)return ret;
    return 0;
}
int Device::Read(dxrt_meminfo_t &meminfo)
{
    LOG_DXRT_DBG << "Device " << _id << " Read : " << meminfo << endl;
    int ret;
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_READ_MEM, reinterpret_cast<void*>(&meminfo));
    if (ret<0)return ret;
    return 0;
}
int Device::Wait(void)
{
    LOG_DXRT_DBG << "Device " << _id << " Wait" << endl;
    int ret;
#ifdef __linux__
    ret = poll(&_devPollFd, 1, DEVICE_POLL_LIMIT_MS);
    LOG_DXRT_DBG << "Device " << _id << " Wakeup" << endl;
    if(ret<0)
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
void Device::BoundOption(dxrt_sche_sub_cmd_t subCmd)
{
    int ret;
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_SCHEDULE, reinterpret_cast<void*>(&_boundOp), sizeof(dxrt_sche_sub_cmd_t), subCmd);
    DXRT_ASSERT(ret==0, "failed to apply bound option to device");
}
void Device::Identify(int id_, SkipMode skip, uint32_t subCmd)
{
    LOG_DXRT_DBG << "Device " << _id << " Identify" << endl;
    int ret;
    _id = id_;    
#ifdef __linux__
    _devFd = open(_file.c_str(), O_RDWR|O_SYNC);
    if(_devFd<0)
    {
        cout << "Error: Can't open " << _file << endl;
        return;
    }
    _devPollFd = {
        .fd = _devFd,
        .events = POLLIN,
        // .events = POLLIN|POLLHUP,
        .revents = 0,
    };
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
    _info = dxrt_device_info_t();
    _info.type = 0;
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, reinterpret_cast<void*>(&_info), 0, subCmd);
    DXRT_ASSERT(ret==0, "failed to identify device");

    _skip = skip;
    if(_skip != VERSION_CHECK)
    {
#ifdef __linux__
        DxDeviceVersion dxVer(this, _info.fw_ver, _info.type, _info.interface, _info.variant);
#elif _WIN32
        DxDeviceVersion dxVer(this, _info.fw_ver, _info.type, _info.interface_value, _info.variant);
#endif
        dxVer.CheckVersion();
    }

    LOG_DXRT_DBG << _name << ": device info : type " << _info.type
        << hex << ", variant " << _info.variant
        << ", mem_addr " << _info.mem_addr
        << ", mem_size " << _info.mem_size 
        << dec << ", num_dma_ch " << _info.num_dma_ch << endl;
    DXRT_ASSERT(_info.mem_size>0, "invalid device memory size");
    _type = _info.type;
    _variant = _info.variant;
#ifdef __linux__
    void *_mem = mmap(0, _info.mem_size, PROT_READ|PROT_WRITE, MAP_SHARED, _devFd, 0);
    if(reinterpret_cast<int64_t>(_mem)==-1)
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
    if(_type==0)
    {
        // cout << featureMemInfo << endl;
        // cout << modelMemInfo << endl;
        // cout << *_featureMem << endl;
        // cout << *_modelMem << endl;
    }

    LOG_DXRT_DBG << "    Device " << _id << ": " << _info << endl;
    if((_variant == 202) && (_skip != COMMON_SKIP))
    {
        _boundOp = N_BOUND_NORMAL;
        BoundOption(DX_SCHED_ADD);
    }

    if(_type==0)
    {
        _inputWorker = Worker::Create(_name + "_input", Worker::Type::DEVICE_INPUT, _info.num_dma_ch, this);
        _outputWorker = Worker::Create(_name + "_output", Worker::Type::DEVICE_OUTPUT, _info.num_dma_ch, this);
        _buffer = make_shared<Buffer>(ACC_DEVICE_BUFFER_SIZE);
        _errorWorker = Worker::Create( _name + "_error", Worker::Type::DEVICE_ERROR, 1, this);
    }
    else
    {
        _thread = thread(&Device::ThreadImpl, this);
    }      
}
void Device::Terminate()
{
    LOG_DXRT_DBG << "Device " << _id << " terminate" << endl;

    uint32_t i;
    if((_variant == 202) && (_skip != COMMON_SKIP))
        BoundOption(DX_SCHED_DELETE);
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
    Process(dxrt::dxrt_cmd_t::DXRT_CMD_RESET, &opt, 4);
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

int64_t Device::Allocate(uint64_t size)
{
    LOG_DXRT_DBG << "Device " << _id << " allocate: " << showbase << hex << "+" << size << endl;
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
    _memory->Deallocate(addr);
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
    while(1)
    {
        if(_stop) break;
        dxrt_response_t response;
        response.req_id = 0;
        LOG_DXRT_DBG << "Device " << _id << " wait. " << endl;
        ret = Wait();
        // cout << "Device " << _id << " wakeup : " << ret << endl;
        if(_stop) break;
        // LOG_VALUE(ret);
        _profiler.End(_name);
        ret = Response(response);
        if(_stop) break;
        LOG_DXRT_DBG << "Device " << _id << " got response " << response.req_id << endl;
        if(ret==0 && response.req_id>0)
        {
            // cout << "response " << response.req_id << ", inf time " << response.inf_time << ", load " << load() << endl;
            auto req = Request::GetById(response.req_id);
            // LOG_VALUE(req.use_count());
            if(req!=nullptr)
            {
                req->task()->ProcessResponse(req, &response);
                CallBack();
            }
        }
    }
    LOG_DXRT_DBG << "Device " << _id << " thread end. " << endl;
}
int Device::RegisterTask(Task* task)
{
    LOG_DXRT_DBG << "Device " << _id << endl;
    int ret = 0;
    int id = task->id();
    _bufIdx[id] = 0;
    vector<dxrt_model_t> models = task->npu_model();
    _npuModel[id] = vector<dxrt_model_t>();
    _npuInference[id] = vector<dxrt_request_t>();
    _npuInferenceAcc[id] = vector<dxrt_request_acc_t>();

    DXRT_ASSERT(task->input_size()>0, "Input size is 0");
    DXRT_ASSERT(task->output_size()>0, "Output size is 0");

    /* Prepare embedded parameters, tensors */
    for(size_t i=0; i<models.size(); i++)
    {
        auto model = models[i];
        model.cmd.base = _memory->start();
        model.cmd.offset = _type==1?Allocate(model.cmd.size):(_modelMem->start() - _featureMem->start() + _modelMem->GetBufferAsOffset(model.cmd.size));
        model.weight.base = _memory->start();
        model.weight.offset = _type==1?Allocate(model.weight.size):(_modelMem->start() - _featureMem->start() + _modelMem->GetBufferAsOffset(model.weight.size));
        if (model.cmd.offset > model.weight.offset)
            model.cmd.offset = _type==1?Allocate(model.cmd.size):(_modelMem->start() - _featureMem->start() + _modelMem->GetBufferAsOffset(model.cmd.size));
        _npuModel[id].emplace_back(model);
        if (_type == 0)
        {
            uint32_t modelS = model.cmd.size+model.weight.size;
            string msg = "Model Memory size is not enough(" +
                int_to_hex(modelS) + "/" +
                int_to_hex(_modelMem->size()) + ")";
            DXRT_ASSERT(_modelMem->size() > modelS, msg);
        }
        for(int j=0; j<DEVICE_NUM_BUF; j++)
        {
            uint32_t inference_offset = 0;
            if(_type ==1){
            	inference_offset = static_cast<uint32_t>(Allocate(model.output_all_offset==0?data_align(task->input_size(), 64):model.output_all_offset));
            }
            dxrt_request_t inference; 
            inference.req_id = 0;
            inference.input.data = 0;
            inference.input.base = _memory->start();
            inference.input.offset = inference_offset;
            inference.input.size = task->input_size();
            inference.output.data = 0;
            inference.output.base = _memory->start();
            // inference.output .offset = _type==1?(model.last_output_offset + Allocate(model.output_all_size)):0;
            inference.output.offset = _type==1?static_cast<uint32_t>(Allocate(model.output_all_size)):0;
            inference.output.size = _type==1?model.output_all_size:task->output_size();
            // inference.output .size = task->output_size();
            inference.model_type = static_cast<uint32_t>(model.type);
            inference.model_format = static_cast<uint32_t>(model.format);
            inference.model_cmds = static_cast<uint32_t>(model.cmds);
            inference.cmd_offset = model.cmd.offset;
            inference.weight_offset = model.weight.offset;
            inference.last_output_offset = model.last_output_offset;

            if(_memory->data()==0)
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
            dxrt_request_acc_t inferenceAcc;
            memset(static_cast<void *>(&inferenceAcc), 0x00, sizeof(dxrt_request_acc_t));
            inferenceAcc.req_id = inference.req_id;
            inferenceAcc.task_id = static_cast<uint32_t>(task->id());
            inferenceAcc.input.data = inference.input.data;
            inferenceAcc.input.base = inference.input.base;
            inferenceAcc.input.offset = inference.input.offset;
            inferenceAcc.input.size = inference.input.size;
            inferenceAcc.output.data = inference.output.data;
            inferenceAcc.output.base = inference.output.base;
            inferenceAcc.output.offset = inference.output.offset;
            inferenceAcc.output.size = inference.output.size;
            inferenceAcc.model_type = static_cast<int16_t>(inference.model_type);
            inferenceAcc.model_format = static_cast<int16_t>(inference.model_format);
            inferenceAcc.bound = _boundOp;

            _npuInference[id].emplace_back(inference);
            _npuInferenceAcc[id].emplace_back(inferenceAcc);
        }
        DXRT_ASSERT(Write(model.cmd)==0, "failed to write model parameters");
        DXRT_ASSERT(Write(model.weight)==0, "failed to write model parameters");
        // cout << "write done" << endl;
    }
    for(auto &inf:_npuInferenceAcc[id])
    {
        inf.npu_id = models.front().npu_id;
        inf.model_cmds = _npuInference[id].front().model_cmds;
        inf.cmd_offset = _npuInference[id].front().cmd_offset;
        inf.weight_offset = _npuInference[id].front().weight_offset;
        inf.model_cmds2 = _npuInference[id].back().model_cmds;
        inf.cmd_offset2 = _npuInference[id].back().cmd_offset;
        inf.weight_offset2 = _npuInference[id].back().weight_offset;
    }

    /* Write model parameters to device */
    for(size_t i=0; i<models.size(); i++)
    {
        auto model = _npuModel[id][i];
        vector<vector<uint8_t>> readData;
        readData.emplace_back(vector<uint8_t>(model.cmd.size));
        readData.emplace_back(vector<uint8_t>(model.weight.size));
        dxrt_meminfo_t cmd(model.cmd);
        dxrt_meminfo_t weight(model.weight);
        cmd.data = reinterpret_cast<uint64_t>(readData[0].data());
        weight.data = reinterpret_cast<uint64_t>(readData[1].data());
        if( Read(cmd) == 0 )
        {
            ret += memcmp(reinterpret_cast<void*>(cmd.data), readData[0].data(), cmd.size);
        }
        if( Read(weight) == 0 )
        {
            ret += memcmp(reinterpret_cast<void*>(weight.data), readData[1].data(), weight.size);
        }
    }
    DXRT_ASSERT(ret==0, "failed to check data integrity of model parameters");
    // cout << "memcmp done" << endl;

    /* Embedded tensor for standalone device */
    for(auto &inf:_npuInference[id])
    {
        _inputTensors[id].emplace_back(task->inputs(reinterpret_cast<void*>(inf.input.data), inf.input.base + inf.input.offset));
        _outputTensors[id].emplace_back(task->outputs(reinterpret_cast<void*>(inf.output.data), inf.output.base + inf.output.offset));
    }
    if(_type==1)
    {
        for(auto &v:_inputTensors[id])
            for(auto &tensor:v)
                cout << tensor << endl;
        for(auto &v:_outputTensors[id])
            for(auto &tensor:v)
                cout << tensor << endl;
    }

    return ret;
}
void Device::CallBack()
{    
    unique_lock<mutex> _lk(_lock);
    _load--;
    _inferenceCnt++;
}
vector<dxrt_model_t> Device::npu_model(int taskId)
{
    if(_npuModel.find(taskId) == _npuModel.end())
    {
        return {};
    }    
    return _npuModel[taskId];
}
vector<Tensors> Device::inputs(int taskId)
{
    return _inputTensors[taskId];
}

ostream& operator<<(ostream &os, const Device& device)
{
    os << "    Device[" << dec << device._id << "] " << device._name << ", load " << device._load << ", type " << device._type
        << hex << ", variant " << device._info.variant
        << ", @ " << device._info.mem_addr << " ~ " << device._info.mem_addr + device._info.mem_size << dec << endl;
    os << *device._memory;
    // for(auto &pair:device._npuModel)
    // {        
    //     for(auto &model:pair.second)
    //     {
    //         os << "      model " << model << endl;
    //     }
    // }
    // if(device._info.type==1)
    // {
    //     for(auto &pair:device._npuInference)
    //     {
    //         for(auto &inf:pair.second)
    //         {
    //             os << "      inference " << inf << endl;
    //         }
    //     }
    // }
    // if(device._info.type==0)
    // {
    //     for(auto &pair:device._npuInferenceAcc)
    //     {
    //         for(auto &inf:pair.second)
    //         {
    //             os << "      inferenceAcc " << inf << endl;
    //         }
    //     }
    // }
    // int i=0;
    // for(auto &pair : device._inputTensors)
    // {
    //     int taskId = pair.first;
    //     for(int i=0; i<DEVICE_NUM_BUF; i++)
    //     {
    //         for(auto &tensor:device._inputTensors.at(taskId).at(i))
    //             os << tensor << endl;
    //         for(auto &tensor:device._outputTensors.at(taskId).at(i))
    //             os << tensor << endl;
    //     }
    // }
    return os;
}

static mutex _devicesLock;
//static int curDevIdx = 0;
shared_ptr<Device> PickOneDevice(vector<shared_ptr<Device>> &devices_)
{
#if 1
    LOG_DXRT_DBG << endl;
    unique_lock<mutex> lk(_devicesLock);
    shared_ptr<Device> pick = nullptr;
    int load = numeric_limits<int>::max();
    int curDeviceLoad;
    while(1)
    {
        for(auto &device:devices_)
        {
            curDeviceLoad = device->load();
            if(curDeviceLoad < 10 && curDeviceLoad < load)
            // if(curDeviceLoad < load)
            {
                load = curDeviceLoad;
                pick = device;
            }
        }
        if(pick!=nullptr)
            break;
    }
    pick->pick();
    // cout << "dev " << pick->id() << ", " << curDeviceLoad << endl;
    LOG_DXRT_DBG << " pick device " << pick->id() << endl;
    return pick;
#else
    unique_lock<mutex> lk(_devicesLock);
    shared_ptr<Device> pick = nullptr;
    pick = devices_[(curDevIdx++)%devices_.size()];
    pick->pick();
    while(pick->load()>10);
    // cout << "dev " << pick->id() << ", " << pick->load() << endl;
    return pick;
#endif
}
vector<shared_ptr<Device>> CheckDevices(SkipMode skip, uint32_t subCmd)
{
    LOG_DXRT_DBG << endl;    
    const char* forceNumDevStr = getenv("DXRT_FORCE_NUM_DEV");
    const char* forceDevIdStr = getenv("DXRT_FORCE_DEVICE_ID");
    int forceNumDev = forceNumDevStr?stoi(forceNumDevStr):0;
    int forceDevId = forceDevIdStr?stoi(forceDevIdStr):-1;

    if(devices.empty())
    {
        cout << "DXRT " DXRT_VERSION << endl;
        devices.clear();
        int cnt = 0;
        while(1)
        {
            string devFile("/dev/" + string(DEVICE_FILE) + to_string(cnt));
            if(fileExists(devFile))
            {
                if(forceNumDev>0 && cnt>=forceNumDev) break;
                if(forceDevId!=-1 && cnt!=forceDevId)
                {
                    cnt++;
                    continue;
                }

                LOG_DBG("Found " + devFile);
                shared_ptr<Device> device = make_shared<Device>(devFile);
                device->Identify(cnt, skip, subCmd);
                devices.emplace_back(move(device));
            }
            else
            {
                break;
            }
            cnt++;
        }
        DXRT_ASSERT(cnt>0, "Device not found.");
    }
    return devices;
    // vector<shared_ptr<Device>> ret = vector<shared_ptr<Device>>(devices);
    // return ret;
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


} // namespace dxrt
