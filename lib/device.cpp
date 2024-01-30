// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/device.h"
#include "dxrt/memory.h"
#include "dxrt/task.h"
#include "dxrt/worker.h"
#include "dxrt/buffer.h"
#include "dxrt/profiler.h"
#include "dxrt/fwlog.h"
#include "dxrt/util.h"
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
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
    }
    Terminate();
    close(_devFd);
    LOG_DXRT_DBG << "Device " << _id << " released." << endl;
    if(_type==1)
    {
        _thread.join();
    }
    LOG_DXRT_DBG << "Device " << _id << " thread released." << endl;
    cout << "Device " << _id << " : " << _inferenceCnt << endl;
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
    Process(DXRT_CMD_GET_STATUS, &_status);
    return _status;
}

int Device::Process(dxrt_cmd_t cmd, void *data, uint32_t size)
{
    int ret;
    dxrt_message_t msg{cmd, data, size};
    ret = ioctl(_devFd, DXRT_IOCTL_MESSAGE, &msg);
    if(ret<0)
        return errno*(-1);
    else
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
    if(_load>1)
    {
        req->latency_valid() = false;
    }
    // dxrt_request_t *inference = nullptr;        
    auto reqInputPtr = req->input_ptr();
    if(_type==1)
    {
        /* standalone device: check to memcpy & cache flush */
        auto &inferences = _npuInference[taskId];
        int pick = -1;
        // dxrt_request_t *pick = nullptr;
        for(int i=0;i<inferences.size();i++)
        {
            if(reinterpret_cast<void*>(inferences[i].input.data) == reqInputPtr)
            {
                pick = i;
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
                Process(DXRT_CMD_CPU_CACHE_FLUSH, reinterpret_cast<void*>(&inferences[pick].input));
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
        //         Process(DXRT_CMD_CPU_CACHE_FLUSH, reinterpret_cast<void*>(&pick->input));
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
        auto outputOffset = inferenceAcc.input.offset + task->input_size();
        inferenceAcc.output.offset = outputOffset + model.front().last_output_offset;
        inferenceAcc.arg0 = outputOffset + model.back().last_output_offset;
        inferenceAcc.status = 0;
        req->outputs() = task->outputs(reinterpret_cast<void*>(inferenceAcc.output.data));
#endif
    }
    if(_type==1)
    {
        LOG_DXRT_DBG << "Device " << _id << " Request : " << req->npu_inference() << endl;
        // ret = write(_devFd, inference, sizeof(dxrt_request_t));
        ret = write(_devFd, &req->npu_inference(), sizeof(dxrt_request_t));
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
    return 0;
}
int Device::Response(dxrt_response_t &response)
{    
    int ret;
    ret = read(_devFd, &response, sizeof(dxrt_response_t));
    if(ret!=sizeof(response))
    {
        return -1;
    }
    LOG_DXRT_DBG << "Device " << _id << " Response : " << response.req_id << endl;
    return 0;
}
int Device::Write(dxrt_meminfo_t &meminfo)
{
    LOG_DXRT_DBG << "Device " << _id << " Write : " << meminfo << endl;
    int ret;
    ret = Process(DXRT_CMD_WRITE_MEM, reinterpret_cast<void*>(&meminfo));
    return 0;
}
int Device::Read(dxrt_meminfo_t &meminfo)
{
    LOG_DXRT_DBG << "Device " << _id << " Read : " << meminfo << endl;
    int ret;
    ret = Process(DXRT_CMD_READ_MEM, reinterpret_cast<void*>(&meminfo));
    return 0;
}
int Device::Wait(void)
{
    LOG_DXRT_DBG << "Device " << _id << " Wait" << endl;
    int ret;
    ret = poll(&_devPollFd, 1, DEVICE_POLL_LIMIT_MS);
    LOG_DXRT_DBG << "Device " << _id << " Wakeup" << endl;
    if(ret<0)
    {
        cout << "Error: Device " << _id << "poll fail." << endl;
        return -1;
    }
    return 0;
}
void Device::Identify(int id_)
{
    LOG_DXRT_DBG << "Device " << _id << " Identify" << endl;
    int ret;
    _id = id_;    
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
    };
    ret = Process(DXRT_CMD_IDENTIFY_DEVICE, reinterpret_cast<void*>(&_info));
    DXRT_ASSERT(ret==0, "failed to identify device");
    if(_info.type==0)
    {
        DXRT_ASSERT(_info.fw_ver>=113, "Invalid firmware version");
    }
    LOG_DXRT_DBG << _name << ": device info : type " << _info.type
        << hex << ", variant " << _info.variant
        << ", mem_addr " << _info.mem_addr
        << ", mem_size " << _info.mem_size 
        << dec << ", num_dma_ch " << _info.num_dma_ch << endl;
    DXRT_ASSERT(_info.mem_size>0, "invalid device memory size");
    _type = _info.type;
    void *_mem = mmap(0, _info.mem_size, PROT_READ|PROT_WRITE, MAP_SHARED, _devFd, 0);
    if(_mem==(void*)-1)
    {
        _mem = nullptr;
    }
    _memory = make_shared<Memory>(_info, _mem);
    auto featureMemInfo = dxrt_device_info_t(_info);
    auto modelMemInfo = dxrt_device_info_t(_info);
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
    cout << "    Device " << _id << ": " << _info << endl;
#if 1
    if(_type==0)
    {
        _inputWorker = Worker::Create(_name + "_input", Worker::Type::DEVICE_INPUT, _info.num_dma_ch, this);
        _outputWorker = Worker::Create(_name + "_output", Worker::Type::DEVICE_OUTPUT, _info.num_dma_ch, this);
        _buffer = make_shared<Buffer>(ACC_DEVICE_BUFFER_SIZE);
    }
    else
    {
        _thread = thread(&Device::ThreadImpl, this);
    }
#else
    _inputWorker = Worker::Create(_name + "_input", Worker::Type::DEVICE_INPUT, _info.num_dma_ch, this);
    _outputWorker = Worker::Create(_name + "_output", Worker::Type::DEVICE_OUTPUT, _info.num_dma_ch, this);
    if(_type==0)
    {
        _buffer = make_shared<Buffer>(ACC_DEVICE_BUFFER_SIZE);
    }
#endif    
}
void Device::Terminate()
{
    LOG_DXRT_DBG << "Device " << _id << " terminate" << endl;
    int i;
    for(i=0;i<_info.num_dma_ch;i++)
    {
        dxrt_response_t data{i};
        int ret = Process(DXRT_CMD_TERMINATE, &data);
    }
}
void Device::Reset(int opt)
{
    Process(DXRT_CMD_RESET, &opt, 4);
}
void Device::ResetBuffer(int opt)
{
    _modelMem->ResetBuffer();
}
vector<uint32_t> Device::Dump()
{
    vector<uint32_t> dump(1000, 0);    
    Process(DXRT_CMD_DUMP, dump.data());
    return dump;
}
void Device::UpdateFwConfig(vector<uint32_t> cfg)
{
    int size = sizeof(uint32_t)*cfg.size();
    Process(DXRT_CMD_UPDATE_CONFIG, cfg.data(), size);
}
int Device::UpdateFw(string fwFile)
{
    int ret;
    DXRT_ASSERT(filesystem::exists(fwFile), fwFile + " doesn't exist.");
    vector<uint8_t> buf(filesystem::file_size(fwFile));
    DataFromFile(fwFile, buf.data());
    return Process(DXRT_CMD_UPDATE_FIRMWARE, buf.data(), buf.size());
}
shared_ptr<FwLog> Device::GetFwLog()
{    
    vector<dxrt_device_log_t> logBuf(512, {0});
    Process(DXRT_CMD_GET_LOG, logBuf.data());
    auto fwlog = make_shared<FwLog>(logBuf);
    return fwlog;
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
        dxrt_response_t response{0};
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
    for(int i=0; i<models.size(); i++)
    {
        auto model = models[i];
        model.cmd.base = _memory->start();
        model.cmd.offset = _type==1?Allocate(model.cmd.size):(_modelMem->start() - _featureMem->start() + _modelMem->GetBufferAsOffset(model.cmd.size));
        model.weight.base = _memory->start();
        model.weight.offset = _type==1?Allocate(model.weight.size):(_modelMem->start() - _featureMem->start() + _modelMem->GetBufferAsOffset(model.weight.size));
        _npuModel[id].emplace_back(model);
        for(int j=0; j<DEVICE_NUM_BUF; j++)
        {
            dxrt_request_t inference = {
                .req_id = 0,
                .input = {
                    .data = 0,
                    .base = _memory->start(),
                    .offset = _type==1?Allocate(task->input_size()):0,
                    .size = task->input_size(),
                },
                .output = {
                    .data = 0,
                    .base = _memory->start(),
                    // .offset = _type==1?(model.last_output_offset + Allocate(model.output_all_size)):0,
                    .offset = _type==1?Allocate(model.output_all_size):0,
                    .size = _type==1?model.output_all_size:task->output_size(),
                    // .size = task->output_size(),
                },
                .model_type = model.type,
                .model_cmds = model.cmds,
                .cmd_offset = model.cmd.offset,
                .weight_offset = model.weight.offset,
                .last_output_offset = model.last_output_offset,
            };
            if(_memory->data()==0)
            {
                {
                    vector<uint8_t> buf(task->input_size());
                    _inputTensorBuffers[id].emplace_back( move(buf) );
                }
                {
                    vector<uint8_t> buf(task->output_size());
                    _outputTensorBuffers[id].emplace_back( move(buf) );
                }
                inference.input.data = reinterpret_cast<uint64_t>(_inputTensorBuffers[id].back().data());
                inference.output.data = reinterpret_cast<uint64_t>(_outputTensorBuffers[id].back().data());
            }
            else
            {
                inference.input.data = _memory->data() + inference.input.offset;
                inference.output.data = _memory->data() + inference.output.offset + inference.last_output_offset;
                // LOG_VALUE_HEX(inference.last_output_offset);
            }
            dxrt_request_acc_t inferenceAcc = {
                .req_id = inference.req_id,
                .task_id = task->id(),
                .input = {
                    .data = inference.input.data,
                    .base = inference.input.base,
                    .offset = inference.input.offset,
                    .size = inference.input.size,
                },
                .output = {
                    .data = inference.output.data,
                    .base = inference.output.base,
                    .offset = inference.output.offset,
                    .size = inference.output.size,
                },
                .model_type = inference.model_type,
            };
            _npuInference[id].emplace_back(inference);
            _npuInferenceAcc[id].emplace_back(inferenceAcc);
        }
        cout << "model cmd: " << model.cmd << endl;
        cout << "model weight: " << model.weight << endl;
        DXRT_ASSERT(Write(model.cmd)==0, "failed to write model parameters");
        DXRT_ASSERT(Write(model.weight)==0, "failed to write model parameters");
        {
            int opt = 2;
            Reset(opt);
        }
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
    for(int i=0; i<models.size(); i++)
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
            ret += memcmp((void*)cmd.data, readData[0].data(), cmd.size);
        }
        if( Read(weight) == 0 )
        {
            ret += memcmp((void*)weight.data, readData[1].data(), weight.size);
        }
    }
    DXRT_ASSERT(ret==0, "failed to check data integrity of model parameters");
    // cout << "memcmp done" << endl;

    /* Embedded tensor for standalone device */
    for(auto &inf:_npuInference[id])
    {
        _inputTensors[id].emplace_back(task->inputs((void*)inf.input.data, inf.input.base + inf.input.offset));
        _outputTensors[id].emplace_back(task->outputs((void*)inf.output.data, inf.output.base + inf.output.offset));
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
static int curDevIdx = 0;
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
vector<shared_ptr<Device>> CheckDevices()
{
    LOG_DXRT_DBG << endl;    
    const char* forceNumDevStr = getenv("DXRT_FORCE_NUM_DEV");
    const char* forceDevIdStr = getenv("DXRT_FORCE_DEVICE_ID");
    int forceNumDev = forceNumDevStr?stoi(forceNumDevStr):0;
    int forceDevId = forceDevIdStr?stoi(forceDevIdStr):-1;

    if(devices.empty())
    {
        cout << "DXRT "DXRT_VERSION << endl;
        devices.clear();
        int cnt = 0;
        while(1)
        {
            string devFile("/dev/" + string(DEVICE_FILE) + to_string(cnt));
            if(filesystem::exists(devFile))
            {
                if(forceNumDev>0 && cnt>=forceNumDev) break;
                if(forceDevId!=-1 && cnt!=forceDevId)
                {
                    cnt++;
                    continue;
                }

                LOG_DBG("Found " + devFile);
                shared_ptr<Device> device = make_shared<Device>(devFile);
                device->Identify(cnt);
                devices.emplace_back(move(device));
            }
            else
            {
                break;
            }
            cnt++;
        }
        
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
