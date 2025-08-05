// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/task.h"

#include <algorithm>
#include <future>
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/exception/exception.h"
#include "dxrt/objects_pool.h"
#include "dxrt/fixed_size_buffer.h"
#include "dxrt/configuration.h"
#ifdef USE_SERVICE
#include "dxrt/multiprocess_memory.h"
#endif

using std::endl;


namespace dxrt {

int Task::nextId = 0;
std::mutex Task::_nextIdLock;

struct TaskStatsInstances
{
    ~TaskStatsInstances()
    {
        LOG_DXRT_DBG << endl;
        for (const auto& pair : _map)
        {
            const auto& stats = pair.second;
            std::cout << "Task" << stats.id << " , " << stats.name << " : latency " << stats.latency_us
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

Task::Task(string name_, rmapinfo rmapInfo_, std::vector<std::vector<uint8_t>>&& data_, npu_bound_op boundOp)
: Task(name_, rmapInfo_, std::move(data_), boundOp, CheckDevices())
{
}
Task::Task(std::string name_, rmapinfo rmapInfo_, std::vector<std::vector<uint8_t>>&& data_,
    npu_bound_op boundOp, std::vector<DevicePtr>& devices_)
: _taskData(getNextId(), name_, rmapInfo_), _data(std::move(data_)), _boundOp(boundOp)
{
    _inferenceCnt.store(0);
    if (_taskData._info.is_initialized())
    {
        _taskData._processor = Processor::NPU;
        // DXRT_ASSERT(_data.size() == 2 || _data.size() == 4,
        //    "invalid npu task " + name() + ": " + to_string(data_.size()));
        if (_data.size() != 2 && _data.size() != 4 )
            throw InvalidModelException(EXCEPTION_MESSAGE(
                "invalid npu task " + name() + ": " + std::to_string(data_.size())));
        _taskData.set_from_npu(_data);
        LOG_DXRT_DBG << "NPU Task: imported npu parameters" << endl;
        SetEncodedInputBuffer(devices_.size() * DXRT_TASK_MAX_LOAD);
        SetOutputBuffer(devices_.size() * DXRT_TASK_MAX_LOAD);
        for (auto device : devices_)
        {
            if (device->isBlocked()) continue;
            _device_ids.push_back(device->id());
        }
        LOG_DXRT_DBG << "NPU Task: checked devices" << endl;
        for (auto &device : devices_)
        {
            if (device->isBlocked()) continue;
            // DXRT_ASSERT(device->RegisterTask(getData()) == 0, "failed to register task");
            if ( device->RegisterTask(getData()) != 0 )
                throw InvalidModelException(EXCEPTION_MESSAGE("failed to register task"));

#ifdef USE_SERVICE
            if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
            {
                // Unified Task-based initialization
                InitializeTaskWithService(device->id());
            }
            else
#endif
            {
                // Direct device initialization (non-service mode)
                device->BoundOption(DX_SCHED_ADD, static_cast<npu_bound_op>(_boundOp));
            }
        }
        LOG_DXRT_DBG << "NPU Task created" << endl;
    }
    else
    {
        _taskData._processor = Processor::CPU;
        _cpuHandle = std::make_shared<CpuHandle>(_data.front().data(), _data.front().size(), _taskData._name, devices_.size());
        // cout << *_cpuHandle << endl;
        _taskData.set_from_cpu(_cpuHandle);
        SetOutputBuffer(devices_.size() * DXRT_TASK_MAX_LOAD);
        _cpuHandle->Start();
        LOG_DXRT_DBG << "CPU Task created" << endl;
    }
}

Task::Task()
: _taskData(getNextId(), "EMPTY", {})
{
    LOG_DBG("Task created.");
}

Task::~Task(void)
{
    LOG_DXRT_DBG << "Task " << id() << " (" << name() << ") destruction started" << endl;

    if (_cpuHandle)
    {
        _cpuHandle = nullptr;
        LOG_DXRT_DBG << "Task " << id() << " Done (CPU)" << endl;
    }
    else if (!_device_ids.empty())
    {
        // NPU Task cleanup - unified Task-based approach
        for (int device_id : _device_ids)
        {
            auto device = ObjectsPool::GetInstance().GetDevice(device_id);

#ifdef USE_SERVICE
            if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
            {
                // New unified Task-based cleanup
                CleanupTaskFromService(device_id);
            }
            else
#endif
            {
                // Direct device cleanup (non-service mode)
                device->BoundOption(DX_SCHED_DELETE, static_cast<npu_bound_op>(_boundOp));
            }

            // Release device-local resources
            device->Release(getData());
        }

        LOG_DXRT_DBG << "Task " << id() << " Done (NPU)" << endl;
    }

    LOG_DXRT_DBG << "Task " << id() << " destruction completed" << endl;
}

void Task::RegisterCallBack(std::function<int(TensorPtrs &outputs, void *userArg)> f)
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
std::map<int, std::vector<int>> &Task::input_index()
{
    return _inputTensorIdx;
}
std::map<int, std::vector<int>> &Task::output_index()
{
    return _outputTensorIdx;
}
void Task::input_name_order(const std::vector<string>& order) {
    _inputNameOrder = order;
}

const std::vector<string>& Task::input_name_order() const {
    return _inputNameOrder;
}
std::atomic<int> &Task::inference_count()
{
    return _inferenceCnt;
}

rmapinfo Task::npu_param()
{
    return _taskData._info;
}
dxrt_model_t Task::npu_model()
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

void Task::InitializeTaskWithService(int device_id)
{
    LOG_DXRT_DBG << "Task " << id() << " initialization with service on device " << device_id << endl;
#ifdef USE_SERVICE
    auto multiProcessMemory = ObjectsPool::GetInstance().GetMultiProcessMemory();
    uint64_t modelMemSize = _taskData._npuModel.rmap.size + _taskData._npuModel.weight.size;

    // 1. Signal Task Init (Register Task metadata)
    multiProcessMemory->SignalTaskInit(device_id, _taskData._id, static_cast<npu_bound_op>(_boundOp), modelMemSize);

    LOG_DXRT_DBG << "Task " << id() << " service initialization completed for device " << device_id << endl;
#endif
}

void Task::CleanupTaskFromService(int device_id)
{
    LOG_DXRT_DBG << "Task " << id() << " cleanup from service on device " << device_id << endl;
#ifdef USE_SERVICE
    auto multiProcessMemory = ObjectsPool::GetInstance().GetMultiProcessMemory();

    // 1. Signal Task DeInit (Task metadata cleanup + device binding release)
    multiProcessMemory->SignalTaskDeInit(device_id, _taskData._id, static_cast<npu_bound_op>(_boundOp));

    // 2. Deallocate Task Memory (Task-based memory deallocation)
    multiProcessMemory->DeallocateTaskMemory(device_id, _taskData._id);

    LOG_DXRT_DBG << "Task " << id() << " service cleanup completed for device " << device_id << endl;
#endif
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
    std::unique_lock<std::mutex> lk(_completeCntLock);
    return _completeCnt;
}
void Task::IncrementCompleteCount()
{
    std::unique_lock<std::mutex> lk(_completeCntLock);
    _completeCnt++;
}
void Task::SetInferenceEngineTimer(InferenceTimer* ie)
{
    _inferenceEngineTimer = ie;
}
void Task::SetEncodedInputBuffer(int size)
{
    std::lock_guard<std::mutex> lock(_bufferMutex);
    if (_taskData._processor == Processor::NPU)
    {
        LOG_DXRT_DBG << "Task "<< id() <<" Encoded Input Buffer Count : " << size << std::endl;
        _taskEncodedInputBuffer = std::make_shared<FixedSizeBuffer>(_taskData.encoded_input_size(), size);
    }
    else
        LOG_DXRT_DBG << "CPU Task "<< id() <<" does not have a buffer"<< std::endl;
}
void* Task::GetEncodedInputBuffer()
{
    std::shared_ptr<FixedSizeBuffer> buffer;
    {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        if (_taskData._processor != Processor::NPU)
        {
            LOG_DXRT_DBG << "CPU Task "<< id() <<" does not have a buffer"<< std::endl;
            return nullptr;
        }
        buffer = _taskEncodedInputBuffer;
    }

    if (buffer) {
        LOG_DXRT_DBG << "Task " << id() << " Encoded Input Buffer GET " << std::endl;
        return buffer->getBuffer();
    }
    return nullptr;
}
void Task::ReleaseEncodedInputBuffer(void* ptr)
{
    std::shared_ptr<FixedSizeBuffer> buffer;
    {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        if (_taskData._processor != Processor::NPU) {
            LOG_DXRT_DBG << "CPU Task "<< id() <<" does not have a buffer"<< std::endl;
            return;
        }
        buffer = _taskEncodedInputBuffer;
    }

    if (buffer) {
        LOG_DXRT_DBG << "Task "<< id() <<" Encoded Input Buffer RELEASE " << std::endl;
        buffer->releaseBuffer(ptr);
    }
}
void Task::ClearEncodedInputBuffer()
{
    std::lock_guard<std::mutex> lock(_bufferMutex);
    for (size_t i = 0; i < _taskData._inputTensors.size(); i++)
        _taskData._inputTensors[i].data() = nullptr;
    if (_taskData._processor == Processor::NPU)
    {
        LOG_DXRT_DBG << "Task "<< id() <<" Encoded Input Buffer CLEAR " << std::endl;
        _taskEncodedInputBuffer = nullptr;
    }
    else
    {
        LOG_DXRT_DBG << "CPU Task "<< id() <<" does not have a buffer"<< std::endl;
    }
}

void Task::SetOutputBuffer(int size)
{
    std::lock_guard<std::mutex> lock(_bufferMutex);
    LOG_DXRT_DBG << "Task "<< id() <<" Output Buffer Count : " << size << std::endl;
    if (_taskData._processor == Processor::NPU)
    {
        _taskOutputBuffer = std::make_shared<FixedSizeBuffer>(_taskData.output_size(), size);
        _taskEncodedOutputBuffer = std::make_shared<FixedSizeBuffer>(_taskData.encoded_output_size(), size);
        LOG_DXRT_DBG << "Task "<< id() <<" Output Buffer Size : " << _taskData.output_size() << std::endl;
        LOG_DXRT_DBG << "Task "<< id() <<" Encoded Output Buffer Size : " << _taskData.encoded_output_size() << std::endl;
    }
    else
    {
        _taskOutputBuffer = std::make_shared<FixedSizeBuffer>(_taskData.output_size(), size);
    }
}

void* Task::GetOutputBuffer()
{
    std::shared_ptr<FixedSizeBuffer> buffer;
    {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        buffer = _taskOutputBuffer;
    }

    if (buffer) {
        LOG_DXRT_DBG << "Task " << id() << " Output Buffer GET " << std::endl;
        return buffer->getBuffer();
    }
    return nullptr;
}

void* Task::GetEncodedOutputBuffer()
{
    std::shared_ptr<FixedSizeBuffer> buffer;
    {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        if (_taskData._processor != Processor::NPU) {
            LOG_DXRT_DBG << "CPU Task "<< id() <<" does not have a decoded output buffer"<< std::endl;
            return nullptr;
        }
        buffer = _taskEncodedOutputBuffer;
    }

    if (buffer) {
        LOG_DXRT_DBG << "Task " << id() << " Encoded Output Buffer GET " << std::endl;
        return buffer->getBuffer();
    }
    return nullptr;
}

void Task::ReleaseOutputBuffer(void* ptr)
{
    std::shared_ptr<FixedSizeBuffer> buffer;
    {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        buffer = _taskOutputBuffer;
    }

    if (buffer) {
        LOG_DXRT_DBG << "Task "<< id() <<" Output Buffer RELEASE " << std::endl;
        buffer->releaseBuffer(ptr); 
    }
}

void Task::ReleaseEncodedOutputBuffer(void* ptr)
{
    std::shared_ptr<FixedSizeBuffer> buffer;
    {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        if (_taskData._processor != Processor::NPU) {
            LOG_DXRT_DBG << "CPU Task "<< id() <<" does not have a decoded output buffer"<< std::endl;
            return;
        }
        buffer = _taskEncodedOutputBuffer;
    }

    if (buffer) {
        LOG_DXRT_DBG << "Task "<< id() <<" Encoded Output Buffer RELEASE " << std::endl;
        buffer->releaseBuffer(ptr);
    }
}

void Task::ClearOutputBuffer()
{
    std::lock_guard<std::mutex> lock(_bufferMutex);
    if (_taskData._processor == Processor::NPU && _taskEncodedOutputBuffer != nullptr)
    {
        LOG_DXRT_DBG << "Task "<< id() <<" Encoded Output Buffer CLEAR " << std::endl;
        _taskEncodedOutputBuffer = nullptr;
    }

    LOG_DXRT_DBG << "Task "<< id() <<" Output Buffer CLEAR " << std::endl;
    for (size_t i = 0; i < _taskData._outputTensors.size(); i++)
    {
        _taskData._outputTensors[i].data() = nullptr;
    }
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
    std::lock_guard<std::mutex> lock(_bufferMutex);
    _lastOutput = t;
}
Tensors Task::getLastOutput()
{
    std::lock_guard<std::mutex> lock(_bufferMutex);
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
int Task::getNextId()
{
    std::lock_guard<std::mutex> lk(_nextIdLock);
    return nextId++;
}

std::ostream& operator<<(std::ostream& os, const Task& task)
{
    if (task._taskData._processor == Processor::NPU)
    {
        int64_t mem_usage = 0;
        if (task._taskData._processor == Processor::NPU)
        {
            mem_usage += task._taskData._memUsage;
            mem_usage += (task._taskData._inputSize * DXRT_TASK_MAX_LOAD);
            mem_usage += (task._taskData._outputMemSize * DXRT_TASK_MAX_LOAD);
        }

        os << std::dec << "  Task[" << task._taskData._id << "] "
           << task._taskData._name << ", " << task._taskData._processor
           << ", NPU memory usage " << format_number_with_commas(task._taskData._memUsage)
           << " bytes (input " << format_number_with_commas(task._taskData._inputSize)
           << " bytes, output " << format_number_with_commas(task._taskData._outputSize)
           << " bytes)" << std::endl;


        os << "  Inputs" << std::endl;
        for (const auto& tensor : task._taskData._inputTensors) os << "     -  " << tensor << std::endl;
        os << "  Outputs" << std::endl;
        for (const auto& tensor : task._taskData._outputTensors) os << "    -  " << tensor << std::endl;
    }
    else if (task._taskData._processor == Processor::CPU)
    {
        os << std::dec << "  Task[" << task._taskData._id << "] "
           << task._taskData._name << ", " << task._taskData._processor
           << ", input " << format_number_with_commas(task._taskData._inputSize)
           << " bytes, output " << format_number_with_commas(task._taskData._outputSize)
           << " bytes" << std::endl;

        os << "  Inputs" << std::endl;
        for (const auto& tensor : task._taskData._inputTensors) os << "     -  " << tensor << std::endl;
        os << "  Outputs" << std::endl;
        for (const auto& tensor : task._taskData._outputTensors) os << "     -  " << tensor << std::endl;
    }
    return os;
}

BufferSet Task::AcquireAllBuffers()
{
    BufferSet buffers;

    try {
        // allocate buffers in a consistent order: encoded_input -> output -> encoded_output
        // If not a head task, the input buffer is not allocated because it reuses the output from the previous task
        if (_taskData._processor == Processor::NPU) {
            buffers.encoded_input = GetEncodedInputBuffer();
            LOG_DXRT_DBG << "Task " << id() << " (HEAD): allocated encoded_input buffer" << std::endl;
        }
        buffers.output = GetOutputBuffer();
        LOG_DXRT_DBG << "Task " << id() << ": allocated output buffer" << std::endl;

        if (_taskData._processor == Processor::NPU) {
            buffers.encoded_output = GetEncodedOutputBuffer();
            LOG_DXRT_DBG << "Task " << id() << ": allocated encoded_output buffer" << std::endl;
        }
        return buffers;
    }
    catch (...) {
        // free already allocated buffers on failure
        ReleaseAllBuffers(buffers);
        throw;
    }
}

void Task::ReleaseAllBuffers(const BufferSet& buffers)
{
    // Release in reverse order with nullptr checks to prevent double release
    if (buffers.encoded_output != nullptr) {
        ReleaseEncodedOutputBuffer(buffers.encoded_output);
    }
    if (buffers.output != nullptr) {
        ReleaseOutputBuffer(buffers.output);
    }
    if (buffers.encoded_input != nullptr) {
        ReleaseEncodedInputBuffer(buffers.encoded_input);
    }
}

}  // namespace dxrt
