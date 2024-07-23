// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/task.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/inference_engine.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
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

Task::Task(string name_, vector<rmapinfo> rmapInfos_, std::vector<std::vector<uint8_t>>&& data_)
: _taskData(nextId, name_, rmapInfos_), _data(move(data_)), _latency(30), _infTime(30)
{
    nextId++;
    _inferenceCnt = 0;
    if (_taskData._infos.empty() == false)
    {
        DXRT_ASSERT(_data.size() == 2 || _data.size() == 4,
            "invalid npu task " + name() + ": " + to_string(data_.size()));

        _taskData.set_from_npu(_data);
        LOG_DXRT_DBG << "NPU Task: imported npu parameters" << endl;
        _devices = CheckDevices();
        LOG_DXRT_DBG << "NPU Task: checked devices" << endl;
        for (auto &device : _devices)
        {
            DXRT_ASSERT(device->RegisterTask(this) == 0, "failed to register task");
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
    Request::Init();
}

Task::Task()
: _taskData(nextId, "EMPTY", {}), _latency(30), _infTime(30)
{
    nextId++;
    LOG_DBG("Task created.");
}

Task::~Task(void)
{
    LOG_DXRT_DBG << endl;
    if(_cpuHandle) _cpuHandle->Terminate();
    Request::SaveTaskStats(this);
    for(auto &device:_devices)
    {
        device->Release(this);
    }
    // Release();
    LOG_DBG("Task released.");
}

int Task::InferenceRequest(RequestPtr req)
{
    LOG_DXRT_DBG << "[" << req->id() << "] " << "- - - - - - - Req " << req->id() << ": " << (req->requestor() ? req->requestor()->name() : "") << " -> " << name() << ", " << _inputTensorQueue.size() << std::endl;
    if (_prevTasks.size() > 1)
    {
        unique_lock<mutex> lk(_inputTensorQueueLock);
        //LOG_DXRT_DBG << "[" << req->id() << "] " << "1) Req " << req->id() << ": " << (req->requestor() ? req->requestor()->name() : "") << " -> " << _name << ", " << _inputTensorQueue.size() << std::endl;
        auto &task = req->task();
        auto &requestor = req->requestor();
        if (requestor)
        {
            //bool multiInputReady = true;
            auto requestorId = requestor->id();
            auto indices = task->input_index()[requestorId];
            int cnt = 0;
            int qidx = 0;
            for (auto &index : indices)
            {
                //_inputTensorQueue[index].push( req->inputs()[cnt] );//Origin?
                qidx = _inputTensorQueueIdx[requestor->name()];
                _inputTensorQueue[qidx].push(req->inputs()[index]); //JG
                LOG_DXRT_DBG << "[" << req->id() << "] " << "push inputTensorQueue[" << qidx << "] req->inputs(" << index << ")" << std::endl;
            }
            for (auto &q : _inputTensorQueue)
            {
                if (q.empty())
                {
                    LOG_DXRT_DBG << "        multi input NOT Ready - inputTensorQueue[" << cnt << "] is empty" << std::endl;
                    return req->id();
                }
                cnt++;
            }
            Tensors inputs;
            for (auto &q : _inputTensorQueue)
            {
                while (!q.empty())
                {
                    inputs.emplace_back(std::move(q.front()));
                    q.pop();
                }
            }
            LOG_DXRT_DBG << "        multi input Ready, inputs size : " << inputs.size() << std::endl;
            req->inputs() = move(inputs);
            //LOG_DXRT << "[" << req->id() << "] " << "cnt : " << cnt << std::endl;
        }
    }
    if (processor() == Processor::NPU)
    {
        LOG_DXRT_DBG << "[" << req->id() << "] " << "N) Req " << req->id() << ": " << (req->requestor() ? req->requestor()->name() : "") << " -> " << name() << ", " << _inputTensorQueue.size() << std::endl;
        auto device = PickOneDevice(_devices);
        req->CheckTimePoint(0);
        req->model_type() = _taskData._npuModel.front().type;
        device->InferenceRequest(req);
    }
    else
    {
        LOG_DXRT_DBG << "[" << req->id() << "] " << "C) Req " << req->id() << ": " << (req->requestor() ? req->requestor()->name() : "") << " -> " << name() << ", " << _inputTensorQueue.size() << std::endl;
        req->CheckTimePoint(0);
        _cpuHandle->InferenceRequest(req);
    }
    return req->id();
}

int Task::ProcessResponse(RequestPtr req, dxrt_response_t *response)
{
    _inferenceCnt++;
    req->CheckTimePoint(1);
    LOG_DXRT_DBG << "[" << req->id() << "] " << "    Response : " << req->id() << ", " << (req->requestor() ? req->requestor()->name() : "") << " -> " << req->task()->name() << ", " << req->latency() << std::endl;
    _taskData._outputTensors = req->outputs(); 
    if (processor() == Processor::NPU)
    {
        req->inference_time() = response->inf_time;
    }
    else
    {
        req->inference_time() = req->latency();
    }
    if (!_nextTasks.empty())
    {
        // LOG_VALUE(_nextTasks.size());
        for (auto &next : _nextTasks)
        {
            auto outputs = SelectElements(req->outputs(), _outputTensorIdx[next->id()]);

            auto nextReq = Request::Create(next.get(), outputs, {}, req->user_arg(), req->last_output_ptr());

            nextReq->head() = req->head();
            nextReq->requestor() = req->task();
            nextReq->SetStatus(Request::Status::REQ_BUSY);
            LOG_DXRT_DBG << *nextReq << std::endl;
            LOG_DXRT_DBG << "Req " << nextReq->id() << ": " << name() << " -> " << next->name() << std::endl;
            next->InferenceRequest(nextReq);
            LOG_DXRT_DBG << "Req " << nextReq->id() << ": " << name() << " -> " << next->name() << " done" << std::endl;
        }
    }
    else
    {
        LOG_DXRT_DBG << "tail task done: " << id() << ", " << req->id() << std::endl;
        /* move to DeviceOutputThread
        if (_processor == Processor::NPU)
        {
            if (req->model_type() == 1)
            {
                // LOG_VALUE(response->argmax);
                *((uint16_t *)(req->outputs().front().data())) = response->argmax;
                DataDumpBin(req->task()->name() + "_output.argmax.bin", req->outputs());
                
            }
            else if (req->model_type() == 2)
            {
                // LOG_VALUE(response->ppu_filter_num);
                vector<int64_t> shape{response->ppu_filter_num};
                req->outputs().front().shape() = shape;
            }
        }
        */
        auto &head = req->head();
        if (head->task() != this)
        {
            head->outputs() = req->outputs();
            head->NotifyCompletion();
            head->SetStatus(Request::Status::REQ_DONE);
            if (head->complete_cnt() == _completeCnt)
            {
                auto latency = chrono::duration_cast<chrono::microseconds>(req->time_point()->end - head->time_point()->start).count();
                _ie->PushLatency(latency);
                _ie->PushInferenceTime(latency);
            }
            // LOG_VALUE(req->head()->id());
            // LOG_VALUE(req->head()->latency());
        }
        else
        {
            _ie->PushLatency(req->latency());
            _ie->PushInferenceTime(req->inference_time());
        }
        req->SetStatus(Request::Status::REQ_DONE);
        if (_callBack != nullptr)
        {
            LOG_DXRT_DBG << "task callback" << std::endl;
            future<void> result = async(launch::async, [&](void){
                TensorPtrs ret;
                for (auto &tensor : req->outputs())
                {
                    ret.emplace_back(
                        make_shared<Tensor>(tensor));
                }
                _callBack(ret, req->user_arg());
                // _callBack(req->outputs(), req->user_arg()); });
            });
        }
    }
    _latency.Push(req->latency());
    _infTime.Push(req->inference_time());
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

void *Task::input_buf(int deviceId, int bufId)
{
    return _devices[deviceId]->input_buf(_taskData.id(), bufId);
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
TaskPtr &Task::next()
{
    return _next;
}
TaskPtrs &Task::prevs()
{
    return _prevTasks;
}
map<string, int> &Task::input_tensor_queue_idx()
{
    return _inputTensorQueueIdx;
}
vector<queue<Tensor>> &Task::input_tensor_queue()
{
    return _inputTensorQueue;
}
vector<queue<Tensor>> &Task::input_tensor_queue_set(size_t size)
{
    _inputTensorQueue = vector<queue<Tensor>>(size);
    return _inputTensorQueue;
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
    if (_latency.IsEmpty())
        return 0;
    return _latency.Get();
}
uint32_t Task::inference_time()
{
    if (_infTime.IsEmpty())
        return 0;
    return _infTime.Get();
}
int &Task::complete_cnt()
{
    return _completeCnt;
}
InferenceEngine* &Task::inference_engine()
{
    return _ie;
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
