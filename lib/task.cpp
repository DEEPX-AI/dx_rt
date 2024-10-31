// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/task.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/exception_handler.h"
#include <algorithm>
#include <future>

using namespace std;

namespace dxrt {

int Task::nextId = 0;

struct TaskStatsInstances
{
    ~TaskStatsInstances()
    {
        LOG_DXRT_DBG << endl;
        for(auto &pair:_map)
        {
            auto &stats = pair.second;
            cout << "Task" << stats.id << " , " << stats.name << " : latency " << stats.latency_us << " us, inference time " << stats.inference_time_us << " us" << endl;
        }
    }
    std::unordered_map<int, TaskStats> _map;
    // std::map<int, TaskStats> _map;
};
TaskStatsInstances taskStatsInstances;
TaskStats &TaskStats::GetInstance(int id)
{
    return taskStatsInstances._map[id];
}

Task::Task(string name_, vector<rmapinfo> rmapInfos_, std::vector<std::vector<uint8_t>>&& data_, npu_bound_op boundOp)
: Task(name_, rmapInfos_, move(data_), boundOp, CheckDevices())
{

}
Task::Task(std::string name_, vector<rmapinfo> rmapInfos_, std::vector<std::vector<uint8_t>>&& data_, npu_bound_op boundOp, std::vector<DevicePtr> devices_)
: _taskData(nextId, name_, rmapInfos_), _data(move(data_)), _boundOp(boundOp)
{
    nextId++;
    _inferenceCnt = 0;
    if (_taskData._infos.empty() == false)
    {
        DXRT_ASSERT(_data.size() == 2 || _data.size() == 4,
            "invalid npu task " + name() + ": " + to_string(data_.size()));

        _taskData.set_from_npu(_data);
        LOG_DXRT_DBG << "NPU Task: imported npu parameters" << endl;
        SetOutputBuffer(_taskData.output_size()*DXRT_ASYNC_LOAD_THRE);
        _devices = devices_;
        LOG_DXRT_DBG << "NPU Task: checked devices" << endl;
        for (auto &device : _devices)
        {
            DXRT_ASSERT(device->RegisterTask(getData()) == 0, "failed to register task");
            device->BoundOption(DX_SCHED_ADD, static_cast<npu_bound_op>(_boundOp));
        }
        LOG_DXRT_DBG << "NPU Task created" << endl;
    }
    else
    {
        _taskData._processor = Processor::CPU;
        _cpuHandle = make_shared<CpuHandle>(_data.front().data(), _data.front().size(), _taskData._name);
        // cout << *_cpuHandle << endl;
        _taskData.set_from_cpu(_cpuHandle);
        _cpuHandle->Start();
        LOG_DXRT_DBG << "CPU Task created" << endl;
    }
    exceptionHandler.SetTasks(this);
    Request::Init();
}





Task::Task()
: _taskData(nextId, "EMPTY", {})
{
    nextId++;
    LOG_DBG("Task created.");
}

Task::~Task(void)
{
    LOG_DXRT_DBG << endl;
    if(_cpuHandle) _cpuHandle->Terminate();
    //Request::SaveTaskStats(this);
    for(auto &device:_devices)
    {
        device->BoundOption(DX_SCHED_DELETE, static_cast<npu_bound_op>(_boundOp));
        device->Release(getData());
    }
    // Release();

    LOG_DBG("Task released.");
}

int Task::InferenceRequest(RequestPtr req)
{
    LOG_DXRT_DBG << "[" << req->id() << "] " << "- - - - - - - Req " << req->id() << ": "
        << req->requestor_name() << " -> " << name() << std::endl;
    if (processor() == Processor::NPU)
    {
        LOG_DXRT_DBG << "[" << req->id() << "] " << "N) Req " << req->id() << ": "
            << req->requestor_name() << " -> " << name() << std::endl;
        auto device = PickOneDevice(_devices);
        req->CheckTimePoint(0);
        req->model_type() = req->taskData()->_npuModel.front().type;
        device->InferenceRequest(req->getData(), static_cast<npu_bound_op>(_boundOp));
    }
    else
    {
        LOG_DXRT_DBG << "[" << req->id() << "] " << "C) Req " << req->id() << ": "
            << req->requestor_name() << " -> " << name() << std::endl;
        req->CheckTimePoint(0);
        _cpuHandle->InferenceRequest(req);
    }
    return req->id();
}

int Task::ProcessResponse(RequestPtr req, dxrt_response_t *response)
{

    req->CheckTimePoint(1);
    LOG_DXRT_DBG << "[" << req->id() << "] " << "    Response : " << req->id() << ", " << req->requestor_name()<< " -> " << req->task()->name() << ", " << req->latency() << std::endl;
    _taskData._outputTensors = req->outputs(); 
    if (req->task()->processor() == Processor::NPU)
    {
        req->inference_time() = response->inf_time;
    }
    else
    {
        req->inference_time() = req->latency();
    }

    
    req->task()->PushLatency(req->latency());
    req->task()->PushInferenceTime(req->inference_time());
    req->onRequestComplete(req);
    return 0;
}
void Task::RegisterCallBack(function<int(TensorPtrs &outputs, void *userArg)> f)
{
    LOG_DXRT_DBG << endl;
    _callBack = f;
}
// int Task::Release(void)
// {
//     LOG_DXRT_DBG << endl;
//     // cout << "--------Task::Release--------" << endl;
//     int ret = 0;
//     for(const auto &device : _devices)
//     {
//         // LOG_VALUE(device->load());        
//         for(auto &model:_npuModel[device->id()])
//         {
//             device->Deallocate(model.cmd.offset);
//             device->Deallocate(model.weight.offset);
//         }
//         // while(device->load()>0);
// #if DEBUG_DXRT
//         // cout << *device << endl;
// #endif
//     }
//     return ret;
// }

int Task::id()
{
    return _taskData.id();
}
string Task::name()
{
    return _taskData.name();
}

/*
void *Task::input_buf(int deviceId, int bufId)
{
    return _devices[deviceId]->input_buf(_taskData.id(), bufId);
}
*/

Tensors Task::inputs(void* ptr, uint64_t phyAddr)
{
    if (ptr == nullptr)
    {
        return _taskData._inputTensors;
    }
    else
    {
        Tensors ret(_taskData._inputTensors);
        int i = 0;
        for (auto &t : ret)
        {
            t.data() = static_cast<void*>(static_cast<uint8_t*>(ptr) + _taskData._inputOffsets[i]);
            t.phy_addr() = phyAddr + _taskData._inputOffsets[i];
            i++;
        }
        return ret;
    }
    return _taskData._inputTensors;
}

Tensors Task::outputs(void* ptr, uint64_t phyAddr)
{
    if (ptr == nullptr)
    {
        return _taskData._outputTensors;
    }
    else
    {
        Tensors ret(_taskData._outputTensors);
        int i = 0;
        for (auto &t : ret)
        {
            t.data() = static_cast<void*>(static_cast<uint8_t*>(ptr) + _taskData._outputOffsets[i]);
            t.phy_addr() = phyAddr + _taskData._outputOffsets[i];
            i++;
        }
        return ret;
    }
    return _taskData._outputTensors;
}

Processor Task::processor()
{
    return _taskData._processor;
}

uint32_t Task::input_size()
{
    return _taskData._inputSize;
}

uint32_t Task::output_size()
{
    return _taskData._outputSize;
}
uint32_t Task::output_mem_size()
{
    return _taskData._outputMemSize;
}
map<int, vector<int>> &Task::input_index()
{
    return _inputTensorIdx;
}
map<int, vector<int>> &Task::output_index()
{
    return _outputTensorIdx;
}
void Task::input_name_order(const std::vector<string>& order) {
    _inputNameOrder = order;
}

const std::vector<string>& Task::input_name_order() const {
    return _inputNameOrder;
}
atomic<int> &Task::inference_count()
{
    return _inferenceCnt;
}

vector<rmapinfo> Task::npu_param()
{
    return _taskData._infos;
}
vector<dxrt_model_t> Task::npu_model()
{
    return _taskData._npuModel;
}
shared_ptr<Buffer> Task::OutputBuffer()
{
    return _taskData.OutputBuffer();
}
TaskPtr &Task::next()
{
    return _next;
}
TaskPtrs &Task::prevs()
{
    return _prevTasks;
}
TaskPtrs &Task::nexts()
{
    return _nextTasks;
}


bool &Task::is_head()
{
    return _isHead;
}
bool &Task::is_tail()
{
    return _isTail;
}
bool &Task::is_argmax()
{
    return _taskData._isArgMax;
}
std::function<int(TensorPtrs&, void*)> Task::callback()
{
    return _callBack;
}
int Task::latency()
{
    return _inferenceTimer.latency();
}
uint32_t Task::inference_time()
{
    return _inferenceTimer.inference_time();
}
void Task::PushLatency(int latency)
{
    _inferenceTimer.PushLatency(latency);
}
void Task::PushInferenceTime(uint32_t infTime)
{
    _inferenceTimer.PushInferenceTime(infTime);
}
int &Task::complete_cnt()
{
    return _completeCnt;
}
void Task::SetInferenceEngineTimer(InferenceTimer* ie)
{
    _ie = ie;
}
void Task::SetOutputBuffer(int size)
{
    _taskData.SetOutputBuffer(size); 
}
ostream& operator<<(ostream& os, const Task& task)
{
    os << dec << "  Task[" << task._taskData._id << "] " << task._taskData._name << ", " << task._taskData._processor
        << ", " << task._taskData._memUsage << "bytes (input " << task._taskData._inputSize << ", output " << task._taskData._outputSize << ")" << endl;
    os << "    inputs" << endl;
    for (auto &tensor : task._taskData._inputTensors) os << "      " << tensor << endl;
    os << "    outputs" << endl;
    for (auto &tensor : task._taskData._outputTensors) os << "      " << tensor << endl;
    /*for(auto &device : task._devices)
    {
        // os << *device << endl;
        // int id = device->id();
        // auto &models = task._npuModel;
        // auto &inferences = task._npuInference.at(id);
        // for(auto &model : models)
        // {
        //     os << "      npu_model " << model << endl;
        // }
        // for(auto &inference : inferences)
        // {
        //     os << "      npu_inference " << inference << endl;
        // }
    }
    if(task._cpuHandle)
    {
        // os << *task._cpuHandle << endl;
    }*/
    return os;
}

/* TODO : Devices */
/* TODO : Create Task MAP */

}  // namespace dxrt