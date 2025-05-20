// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"

#include <algorithm>
#include <future>
#include "dxrt/task.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/exception/exception.h"
#include "dxrt/objects_pool.h"
#include "dxrt/fixed_size_buffer.h"
#include "dxrt/configuration.h"




namespace dxrt {

int Task::nextId = 0;

struct TaskStatsInstances
{
    ~TaskStatsInstances()
    {
        LOG_DXRT_DBG << endl;
        for (const auto& pair : _map)
        {
            const auto& stats = pair.second;
            cout << "Task" << stats.id << " , " << stats.name << " : latency " << stats.latency_us
                << " us, inference time " << stats.inference_time_us << " us" << endl;
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
: Task(name_, rmapInfos_, std::move(data_), boundOp, CheckDevices())
{

}
Task::Task(std::string name_, vector<rmapinfo> rmapInfos_, std::vector<std::vector<uint8_t>>&& data_,
    npu_bound_op boundOp, std::vector<DevicePtr>& devices_)
: _taskData(nextId, name_, rmapInfos_), _data(std::move(data_)), _boundOp(boundOp)
{
    nextId++;
    _inferenceCnt.store(0);
    if (_taskData._infos.empty() == false)
    {
        // DXRT_ASSERT(_data.size() == 2 || _data.size() == 4,
        //    "invalid npu task " + name() + ": " + to_string(data_.size()));
        if (_data.size() != 2 && _data.size() != 4 )
            throw InvalidModelException(EXCEPTION_MESSAGE(
                "invalid npu task " + name() + ": " + to_string(data_.size())));

        _taskData.set_from_npu(_data);
        LOG_DXRT_DBG << "NPU Task: imported npu parameters" << endl;
        SetOutputBuffer(devices_.size() * DXRT_TASK_MAX_LOAD * 2);
        for (auto device : devices_)
        {
            _device_ids.push_back(device->id());
        }
        LOG_DXRT_DBG << "NPU Task: checked devices" << endl;
        for (auto &device : devices_)
        {
            // DXRT_ASSERT(device->RegisterTask(getData()) == 0, "failed to register task");
            if ( device->RegisterTask(getData()) != 0 )
                throw InvalidModelException(EXCEPTION_MESSAGE("failed to register task"));

#ifdef USE_SERVICE
            if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
            {
                device->signalToDevice(static_cast<npu_bound_op>(_boundOp));
            }
            else
#endif
            {
                device->BoundOption(DX_SCHED_ADD, static_cast<npu_bound_op>(_boundOp));
            }
        }
        LOG_DXRT_DBG << "NPU Task created" << endl;
    }
    else
    {
        _taskData._processor = Processor::CPU;
        _cpuHandle = make_shared<CpuHandle>(_data.front().data(), _data.front().size(), _taskData._name);
        // cout << *_cpuHandle << endl;
        _taskData.set_from_cpu(_cpuHandle);
        SetOutputBuffer(DXRT_TASK_MAX_LOAD);
        _cpuHandle->Start();
        LOG_DXRT_DBG << "CPU Task created" << endl;
    }
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
    if (_cpuHandle)
    {
        _cpuHandle->Terminate();
        LOG_DXRT_DBG <<" Done (CPU)"<< endl;
    }
    else
    {
        for (int device_id : _device_ids)
        {
            auto device = ObjectsPool::GetInstance().GetDevice(device_id);
#ifdef USE_SERVICE
            if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
            {
                device->signalToDeviceDeInit(static_cast<npu_bound_op>(_boundOp));
            }
            else
#endif
            {
                device->BoundOption(DX_SCHED_DELETE, static_cast<npu_bound_op>(_boundOp));
            }
            device->Release(getData());
        }
        // Release();
        LOG_DXRT_DBG <<" Done (NPU)"<< endl;
    }
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
void Task::set_head() {
    _isHead = true;
}
void Task::set_tail() {
    _isTail = true;
}
bool &Task::is_head()
{
    return _isHead;
}
bool &Task::is_tail()
{
    return _isTail;
}
bool &Task::is_PPU()
{
    return _taskData._isPPU;
}
bool &Task::is_argmax()
{
    return _taskData._isArgMax;
}
std::function<int(TensorPtrs&, void*)> Task::callback()
{
    return _callBack;
}
InferenceTimer& Task::GetTaskTimer()
{
    return _taskTimer;
}
int Task::GetLatency()
{
    return _taskTimer.latency();
}
uint32_t Task::GetNpuInferenceTime()
{
    return _taskTimer.inference_time();
}
void Task::PushLatency(int latency)
{
    _taskTimer.PushLatency(latency);
}
void Task::PushInferenceTime(uint32_t infTime)
{
    _taskTimer.PushInferenceTime(infTime);
}
int &Task::GetCompleteCnt()
{
    unique_lock<mutex> lk(_completeCntLock);
    return _completeCnt;
}
void Task::IncrementCompleteCount()
{
    unique_lock<mutex> lk(_completeCntLock);
    _completeCnt++;
}
void Task::SetInferenceEngineTimer(InferenceTimer* ie)
{
    _inferenceEngineTimer = ie;
}
void Task::SetOutputBuffer(int size)
{
    LOG_DXRT_DBG << "Task "<< id() <<" Output Buffer Size : " << size << std::endl;
    _taskOutputBuffer = make_shared<FixedSizeBuffer>(_taskData.output_size(), size);
}
void* Task::GetOutputBuffer()
{
    LOG_DXRT_DBG << "Task " << id() << " Output Buffer GET " << std::endl;
    void* retval = _taskOutputBuffer->getBuffer();  // Process as Conditional variable
    return retval;
}
void Task::ReleaseOutputBuffer(void* ptr)
{
    LOG_DXRT_DBG << "Task "<< id() <<" Output Buffer RELEASE " << std::endl;
    _taskOutputBuffer->releaseBuffer(ptr);
}
void Task::ClearOutputBuffer()
{
    LOG_DXRT_DBG << "Task "<< id() <<" Output Buffer CLEAR " << std::endl;
    _taskOutputBuffer = nullptr;
}

const std::vector<int>& Task::getDeviceIds()
{
    return _device_ids;
}
CpuHandle* Task::getCpuHandle()
{
    return _cpuHandle.get();
}
int Task::getNpuBoundOp()
{
    return _boundOp;
}
void Task::setLastOutput(Tensors t)
{
    std::lock_guard<std::mutex> lock(_lastOutputLock);
    _lastOutput = t;
}
Tensors Task::getLastOutput()
{
    std::lock_guard<std::mutex> lock(_lastOutputLock);
    return _lastOutput;
}
bool Task::has_next()
{
    return _nextTasks.empty() == false;
}
void Task::setTailOffset(int64_t n)
{
    _tailOffset = n;
}
int64_t Task::getTailOffset()
{
    return _tailOffset;
}


ostream& operator<<(ostream& os, const Task& task)
{
    os << dec << "  Task[" << task._taskData._id << "] " << task._taskData._name << ", " << task._taskData._processor
        << ", memory-usage " << task._taskData._memUsage << " bytes (input " << task._taskData._inputSize
        << ", output " << task._taskData._outputSize << ")" << endl;
    os << "    inputs" << endl;
    for (const auto& tensor : task._taskData._inputTensors) os << "      " << tensor << endl;
    os << "    outputs" << endl;
    for (const auto& tensor : task._taskData._outputTensors) os << "      " << tensor << endl;
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
