/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

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
#include "dxrt/device_task_layer.h"
#ifdef USE_SERVICE
#include "dxrt/multiprocess_memory.h"
#endif
#include "dxrt/device_pool.h"

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
            LOG_DXRT << "Task" << stats.id << " , " << stats.name << " : latency " << stats.latency_us
                << " us, inference time " << stats.inference_time_us << " us" << endl;
        }
    }
    std::unordered_map<int, TaskStats> _map;
    TaskStatsInstances() = default;
    TaskStatsInstances(const TaskStatsInstances&) = delete;
    TaskStatsInstances& operator=(const TaskStatsInstances&) = delete;
    TaskStatsInstances(TaskStatsInstances&&) = delete;
    TaskStatsInstances& operator=(TaskStatsInstances&&) = delete;
};
TaskStatsInstances taskStatsInstances;  // NOSONAR:S5421
[[deprecated("will be removed because no usage")]]
TaskStats &TaskStats::GetInstance(int id)
{
    return taskStatsInstances._map[id];
}

static std::vector<int> makeList(int n)
{
    std::vector<int> vec(n);
    std::iota(vec.begin(), vec.end(), 0);
    return vec;
}

// Constructor 1: Default devices + hasPpuBinary
Task::Task(const std::string& name_, const rmapinfo& rmapInfo_ , int bufferCount_, std::vector<std::vector<uint8_t>>&& data_, npu_bound_op boundOp, bool hasPpuBinary)
: Task(name_, rmapInfo_, bufferCount_, std::move(data_), boundOp, makeList(static_cast<int>(DevicePool::GetInstance().GetDeviceCount())), hasPpuBinary)
{
}

// Constructor 2: Specific devices + hasPpuBinary
Task::Task(const std::string& name_, const rmapinfo& rmapInfo_, int bufferCount_, std::vector<std::vector<uint8_t>>&& data_,
    npu_bound_op boundOp, const std::vector<int>& deviceIds, bool hasPpuBinary)
: _taskData(getNextId(), name_, rmapInfo_, bufferCount_),  _device_ids(deviceIds), _data(std::move(data_)), _boundOp(boundOp)
{
    _inferenceCnt.store(0);
    auto device_id_count = static_cast<int>(_device_ids.size());
    if (_taskData._info.is_initialized())
    {
        _taskData._processor = Processor::NPU;
        // v6/v7: data.size() == 2 (rmap, weight) or 4 (with bitmatch)
        // v8 PPCPU: data.size() == 3 (rmap, weight, ppu) or 5 (with bitmatch + ppu)
        if (_data.size() != 2 && _data.size() != 3 && _data.size() != 4 && _data.size() != 5)
            throw InvalidModelException(EXCEPTION_MESSAGE(
                "invalid npu task " + name() + ": data size = " + std::to_string(data_.size())));

        // Set data reference for device memory write
        _taskData._data = &_data;

        _taskData.set_from_npu(_data, hasPpuBinary);
        LOG_DXRT_DBG << "NPU Task: imported npu parameters" << endl;
        SetEncodedInputBuffer(device_id_count * _taskData.get_buffer_count());
        SetOutputBuffer(device_id_count * _taskData.get_buffer_count());
        LOG_DXRT_DBG << "NPU Task: checked devices" << endl;
        for (auto deviceId : _device_ids)
        {
            auto devicePtr = DevicePool::GetInstance().GetDeviceTaskLayer(deviceId);
            if (devicePtr->isBlocked())
                continue;
            if ( devicePtr->RegisterTask(getData()) != 0 )
                throw InvalidModelException(EXCEPTION_MESSAGE("failed to register task"));



            InitializeTaskWithService(deviceId);

        }
        LOG_DXRT_DBG << "NPU Task created" << endl;
    }
    else
    {
        _taskData._processor = Processor::CPU;
        _cpuHandle = std::make_shared<CpuHandle>(_data.front().data(), _data.front().size(), _taskData._name, _device_ids.size(), _taskData.get_buffer_count());
        _taskData.set_from_cpu(_cpuHandle);
        SetOutputBuffer(static_cast<int>(_device_ids.size() * _taskData.get_buffer_count())); // DXRT_TASK_MAX_LOAD)
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
    else if (_device_ids.empty())
    {
        LOG_DXRT_DBG << "Task " << id() << " has no associated devices. Skipping cleanup." << endl;
    }
    else
    {
        // NPU Task cleanup - unified Task-based approach
        for (int device_id : _device_ids)
        {
            auto device = DevicePool::GetInstance().GetDeviceTaskLayer(device_id);


            // New unified Task-based cleanup
            try
            {
                CleanupTaskFromService(device_id);
            }
            catch (const dxrt::Exception& e)
            {
                LOG_DXRT_ERR("Task cleanup with service failed on device " + std::to_string(device_id)
                    + " [dxrt::Exception]: " + e.what());
            }
            catch (const std::exception& e)
            {
                LOG_DXRT_ERR("Task cleanup with service failed on device " + std::to_string(device_id)
                    + ": " + e.what());
            }
            catch (...)
            {
                LOG_DXRT_ERR("Task cleanup with service failed on device " + std::to_string(device_id)
                    + ": unknown exception");
            }


            // Release device-local resources
            try
            {
                device->Release(getData());
            }
            catch (const dxrt::Exception& e)
            {
                LOG_DXRT_ERR("Task device resource release failed on device " + std::to_string(device_id)
                    + " [dxrt::Exception]: " + e.what());
            }
            catch (const std::exception& e)
            {
                LOG_DXRT_ERR("Task device resource release failed on device " + std::to_string(device_id)
                    + ": " + e.what());
            }
            catch (...)
            {
                LOG_DXRT_ERR("Task device resource release failed on device " + std::to_string(device_id)
                    + ": unknown exception");
            }
        }

        LOG_DXRT_DBG << "Task " << id() << " Done (NPU)" << endl;
    }

    LOG_DXRT_DBG << "Task " << id() << " destruction completed" << endl;
}

void Task::RegisterCallBack(const std::function<int(TensorPtrs &outputs, void *userArg)>& f)
{
    LOG_DXRT_DBG << endl;
    _callBack = f;
}

int Task::id() const
{
    return _taskData.id();
}
std::string Task::name() const
{
    return _taskData.name();
}



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

Processor Task::processor() const
{
    return _taskData._processor;
}

uint32_t Task::input_size() const
{
    return _taskData._inputSize;
}

uint32_t Task::output_size() const
{
    return _taskData._outputSize;
}
uint32_t Task::output_mem_size() const
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
void Task::input_name_order(const std::vector<std::string>& order) {
    _inputNameOrder = order;
}

const std::vector<std::string>& Task::input_name_order() const {
    return _inputNameOrder;
}
std::atomic<int> &Task::inference_count()
{
    return _inferenceCnt;
}

rmapinfo Task::npu_param() const
{
    return _taskData._info;
}
dxrt_model_t Task::npu_model() const
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
std::function<int(TensorPtrs&, void*)> Task::callback() const
{
    return _callBack;
}

void Task::InitializeTaskWithService(int device_id) const
{
    LOG_DXRT_DBG << "Task " << id() << " initialization with service on device " << device_id << endl;

    uint64_t modelMemSize = _taskData._npuModel.rmap.size + _taskData._npuModel.weight.size;

    // 1. Signal Task Init (Register Task metadata)
    DevicePool::GetInstance().GetServiceLayer()->SignalTaskInit(device_id,
        _taskData._id, static_cast<npu_bound_op>(_boundOp), modelMemSize);

    LOG_DXRT_DBG << "Task " << id() << " service initialization completed for device " << device_id << endl;

}

void Task::CleanupTaskFromService(int device_id) const
{
    LOG_DXRT_DBG << "Task " << id() << " cleanup from service on device " << device_id << endl;


    DevicePool::GetInstance().GetServiceLayer()->SignalTaskDeInit(device_id,
        _taskData._id, static_cast<npu_bound_op>(_boundOp));


    LOG_DXRT_DBG << "Task " << id() << " service cleanup completed for device " << device_id << endl;
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
#ifndef USE_VNPU
        _taskEncodedInputBuffer = std::make_shared<FixedSizeBuffer>(_taskData.encoded_input_size(), size);
#else
        _taskEncodedInputBuffer = std::make_shared<FixedSizeBuffer>(

        _taskData.encoded_input_size(), size, BufferAllocType::CMA_DMA, BufferDirection::INPUT);
#endif // USE_VNPU
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
    for (auto& input :  _taskData._inputTensors)
    {
        input.data() = nullptr;
    }
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
#ifndef USE_VNPU
        _taskOutputBuffer = std::make_shared<FixedSizeBuffer>(_taskData.output_size(), size);
        _taskEncodedOutputBuffer = std::make_shared<FixedSizeBuffer>(_taskData.encoded_output_size(), size);
#else
        _taskOutputBuffer = std::make_shared<FixedSizeBuffer>(
            _taskData.output_size(), size, BufferAllocType::CMA_DMA, BufferDirection::OUTPUT);
        _taskEncodedOutputBuffer = std::make_shared<FixedSizeBuffer>(
            _taskData.encoded_output_size(), size, BufferAllocType::CMA_DMA, BufferDirection::OUTPUT);
#endif // USE_VNPU
        LOG_DXRT_DBG << "Task "<< id() <<" Output Buffer Size : " << _taskData.output_size() << std::endl;
        LOG_DXRT_DBG << "Task "<< id() <<" Encoded Output Buffer Size : " << _taskData.encoded_output_size() << std::endl;
    }
    else
    {
#ifndef USE_VNPU
        _taskOutputBuffer = std::make_shared<FixedSizeBuffer>(_taskData.output_size(), size);
#else
        _taskOutputBuffer = std::make_shared<FixedSizeBuffer>(
        _taskData.output_size(), size, BufferAllocType::HEAP);
#endif // USE_VNPU
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

#ifdef USE_VNPU
void Task::FlushEncodedOutputCache(void* ptr, uint32_t size, bool invalidate)
{
    std::lock_guard<std::mutex> lock(_bufferMutex);
    if (_taskData._processor != Processor::NPU) {
        return;
    }

    if (_taskEncodedOutputBuffer) {
        _taskEncodedOutputBuffer->flushCache(ptr, size, invalidate);
    }
}
#endif // USE_VNPU

void Task::ClearOutputBuffer()
{
    std::lock_guard<std::mutex> lock(_bufferMutex);
    if (_taskData._processor == Processor::NPU && _taskEncodedOutputBuffer != nullptr)
    {
        LOG_DXRT_DBG << "Task "<< id() <<" Encoded Output Buffer CLEAR " << std::endl;
        _taskEncodedOutputBuffer = nullptr;
    }

    LOG_DXRT_DBG << "Task "<< id() <<" Output Buffer CLEAR " << std::endl;
    for (auto& output : _taskData._outputTensors)
    {
        output.data() = nullptr;
    }
    _taskOutputBuffer = nullptr;
}


const std::vector<int>& Task::getDeviceIds() const
{
    return _device_ids;
}
CpuHandle* Task::getCpuHandle()
{
    return _cpuHandle.get();
}
int Task::getNpuBoundOp() const
{
    return _boundOp;
}
void Task::setLastOutput(Tensors t) // NOSONAR:S1238 due to legacy code, will be refactored in the future
{
     std::lock_guard<std::mutex> lock(_bufferMutex);
     _lastOutput = t;
}
Tensors Task::getLastOutput()
{
    std::lock_guard<std::mutex> lock(_bufferMutex);
    return _lastOutput;
}
bool Task::has_next() const
{
    return _nextTasks.empty() == false;
}
void Task::setTailOffset(int64_t n)
{
    _tailOffset = n;
}
int64_t Task::getTailOffset() const
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
            mem_usage += (task._taskData._inputSize * task._taskData.get_buffer_count());
            mem_usage += (task._taskData._outputMemSize * task._taskData.get_buffer_count());
        }

        os << std::dec << "  Task[" << task._taskData._id << "] "
           << task._taskData._name << ", " << task._taskData._processor
           << ", NPU memory usage " << format_number_with_commas(task._taskData._memUsage)
           << " bytes (input " << format_number_with_commas(task._taskData._inputSize)
           << " bytes, output " << format_number_with_commas(task._taskData._outputSize)
           << " bytes), mem_usage:" << format_number_with_commas(mem_usage) << " bytes" << std::endl;


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
    else
    {
        os << "  Task[" << task._taskData._id << "] "
           << task._taskData._name << ", Processor: UNKNOWN (" << static_cast<int>(task._taskData._processor) << ")" << std::endl;
    }
    return os;
}

BufferSet Task::AcquireAllBuffers()
{
    BufferSet buffers;

    try {
        // allocate buffers in a consistent order: encoded_input -> output -> encoded_output
        // If not a head task, the input buffer is not allocated because it reuses the output from the previous task
        if (_taskData._processor == Processor::NPU)
        {
            buffers.encoded_input = GetEncodedInputBuffer();
#ifndef USE_VNPU
            LOG_DXRT_DBG << "Task " << id() << " (HEAD): allocated encoded_input buffer" << std::endl;
#else
            buffers.encoded_input_phy = _taskEncodedInputBuffer->getPhysicalAddress(buffers.encoded_input);
            LOG_DXRT_DBG << "Task " << id() << " allocated encoded_input: vaddr=" << buffers.encoded_input
                     << ", phy=0x" << std::hex << buffers.encoded_input_phy << std::dec << std::endl;
#endif // USE_VNPU
        }
        buffers.output = GetOutputBuffer();
#ifndef USE_VNPU
        LOG_DXRT_DBG << "Task " << id() << ": allocated output buffer" << std::endl;

        if (_taskData._processor == Processor::NPU)
        {
            buffers.encoded_output = GetEncodedOutputBuffer();
            LOG_DXRT_DBG << "Task " << id() << ": allocated encoded_output buffer" << std::endl;
        }
#else
        buffers.output_phy = _taskOutputBuffer->getPhysicalAddress(buffers.output);
        LOG_DXRT_DBG << "Task " << id() << " allocated output: vaddr=" << buffers.output
                 << ", phy=0x" << std::hex << buffers.output_phy << std::dec << std::endl;
        if (_taskData._processor == Processor::NPU)
        {
            buffers.encoded_output = GetEncodedOutputBuffer();
            buffers.encoded_output_phy = _taskEncodedOutputBuffer->getPhysicalAddress(buffers.encoded_output);
            LOG_DXRT_DBG << "Task " << id() << " allocated encoded_output: vaddr=" << buffers.encoded_output
                     << ", phy=0x" << std::hex << buffers.encoded_output_phy << std::dec << std::endl;
        }
#endif // USE_VNPU
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
