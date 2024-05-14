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
: _name(name_), _infos(rmapInfos_), _data(move(data_)), _latency(30), _infTime(30)
{
    _id = nextId++;    
    _inferenceCnt = 0;
    if(!_infos.empty())
    {
        _processor = Processor::NPU;
        DXRT_ASSERT(_data.size()==2 || _data.size()==4, "invalid npu task " + _name + ": " + to_string(data_.size()));
        for(auto &info:_infos)
        {
            _memUsage += info.size();
        }
        auto &info = _infos.front();
        _numOutputs = info.outputs().outputlist().output_size();
        {
            vector<int64_t> shapes;
            for(int i=0; i<info.input().shapes().shape_size();i++)
            {        
                shapes.emplace_back(info.input().shapes().shape(i));
            }
            _inputShape.emplace_back(shapes);
            _inputNames.emplace_back(info.input().memory().name());
        }
        LOG_DXRT_DBG << "NPU Task: imported input shapes" << endl;
        {        
            for(int i=0; i<info.outputs().outputlist().output_size(); i++)
            {
                auto &node = info.outputs().outputlist().output(i);
                vector<int64_t> shapes;
                for(int j=0; j<node.shapes().shape_size(); j++)
                {
                    shapes.emplace_back(node.shapes().shape(j));
                }
                if(shapes.size()==4)
                {
                    shapes[3] = GetAlign64(shapes[3]);
                }
                _outputShape.emplace_back(shapes);
                _outputOffsets.emplace_back(info.outputs().outputlist().output(i).memory().offset() - info.outputs().outputlist().output(0).memory().offset());
                _outputSize += info.outputs().outputlist().output(i).memory().size();
                _outputNames.emplace_back(node.name());
            }
        }
        LOG_DXRT_DBG << "NPU Task: imported output shapes"<< endl;
        _inputSize = info.input().memory().size();
        _inputDataType = (DataType)info.input().type();
        if(info.outputs().outputlist().output_size()>0)
        {
            _outputDataType = (DataType)info.outputs().outputlist().output(0).type();
        }
        else
        {
            _outputDataType = DataType::NONE_TYPE;
        }
        LOG_DXRT_DBG << "NPU Task: imported data types" << endl;

        for(int i=0;i<_numInputs;i++)
        {
            _inputTensors.emplace_back(Tensor(_inputNames[i], _inputShape[i], _inputDataType, nullptr));
        }
        for(int i=0;i<_numOutputs;i++)
        {
            _outputTensors.emplace_back(Tensor(_outputNames[i], _outputShape[i], _outputDataType, nullptr));
        }
        LOG_DXRT_DBG << "NPU Task: imported tensors" << endl;
        for(size_t i=0; i<_infos.size(); i++)
        {
            auto &info = _infos[i];
            auto cmdSize = info.memorys().memory(0).size();
            auto weightSize = info.memorys().memory(1).size();
            dxrt_model_t model;          
            model.npu_id = 0;
            model.type = 0;
            model.cmds = static_cast<int32_t>(info.counts().cmd());
            model.cmd.data = reinterpret_cast<uint64_t>( _data[ 2*i + 0 ].data() );
            model.cmd.base = 0; // decided in device
            model.cmd.offset = 0; // defined in device
            model.cmd.size = static_cast<uint32_t>(cmdSize);
            model.weight.data = reinterpret_cast<uint64_t>( _data[ 2*i + 1 ].data() );
            model.weight.base = 0; // decided in device
            model.weight.offset = 0; // defined in device
            model.weight.size = static_cast<uint32_t>(weightSize);
            model.output_all_offset = static_cast<uint32_t>(info.outputs().memory().offset());
            model.output_all_size = static_cast<uint32_t>(info.outputs().memory().size());
            model.last_output_offset = static_cast<uint32_t>(info.outputs().outputlist().output(0).memory().offset() - info.outputs().memory().offset());
            model.last_output_size = _outputSize;

            if(info.outputs().outputlist().output(0).memory().type()==deepx_rmapinfo::MemoryType::ARGMAX)
            {
                model.type = 1;
                // model.last_output_offset = model.output_all_size;
                // model.last_output_size = 64;
                // model.output_all_size += 64;
                // _outputSize = 64;
                model.last_output_size = 2;
                _outputSize = 2;
                _isArgMax = true;
            }
            else if(info.outputs().outputlist().output(0).memory().type()==deepx_rmapinfo::MemoryType::PPU)
            {
                if(info.outputs().outputlist().output(0).format()>=deepx_rmapinfo::DataFormat::PPU_YOLO
                    && info.outputs().outputlist().output(0).format()<=deepx_rmapinfo::DataFormat::PPU_POSE )
                {
                    model.type = 2;
                    _outputTensors.clear();
                    int dataType = DataType::BBOX + info.outputs().outputlist().output(0).format() - deepx_rmapinfo::DataFormat::PPU_YOLO;    
                    _outputTensors.emplace_back(
                        Tensor(_outputNames[i], _outputShape[i], (DataType)dataType, nullptr)
                    );
                    model.last_output_offset = model.output_all_size;
                    model.last_output_size = 128*1024;
                    model.output_all_size += 128*1024;
                    _outputSize = 128*1024;
                }
            }
            if(_infos.size()>1)
            {
                model.npu_id = -1;
            }
            else
            {
                if(info.version().npu()=="M1_8K")
                {
                    model.npu_id = 1;
                }
                else
                {
                    model.npu_id = 0;
                }
            }
            _npuModel.emplace_back(model);
        }
        _outputMemSize = 0;
        for(auto &model:_npuModel)
        {
            _outputMemSize = max(_outputMemSize, model.output_all_size);
        }
        LOG_DXRT_DBG << "NPU Task: imported npu parameters" << endl;
        _devices = CheckDevices();
        LOG_DXRT_DBG << "NPU Task: checked devices" << endl;
        for(auto &device:_devices)
        {
            DXRT_ASSERT( device->RegisterTask(this)==0, "failed to register task" );
        }
        LOG_DXRT_DBG << "NPU Task created" << endl;
    }
    else
    {
        _processor = Processor::CPU;
        _cpuHandle = make_shared<CpuHandle>( _data.front().data(), _data.front().size(), _name );
        // cout << *_cpuHandle << endl;
        _numInputs = _cpuHandle->_numInputs;
        _numOutputs = _cpuHandle->_numOutputs;
        _inputSize = _cpuHandle->_inputSize;
        _outputSize = _cpuHandle->_outputSize;
        _outputMemSize = _outputSize;
        _memUsage = _inputSize + _outputSize;
        _inputDataType = _cpuHandle->_inputDataType;
        _outputDataType = _cpuHandle->_outputDataType;
        _inputNames = _cpuHandle->_inputNames;
        _outputNames = _cpuHandle->_outputNames;
        _inputShape = _cpuHandle->_inputShape;
        _outputShape = _cpuHandle->_outputShape;
        _inputOffsets = _cpuHandle->_inputOffsets;
        _outputOffsets = _cpuHandle->_outputOffsets;
        for(int i=0;i<_numInputs;i++)
        {
            _inputTensors.emplace_back(Tensor(_inputNames[i], _inputShape[i], _inputDataType, nullptr));
        }
        for(int i=0;i<_numOutputs;i++)
        {
            _outputTensors.emplace_back(Tensor(_outputNames[i], _outputShape[i], _outputDataType, nullptr));
        }
    }
    _inputTensorQueue = vector<queue<Tensor>>(_numInputs);
    Request::Init();
}

Task::Task()
: _latency(30), _infTime(30)
{
    _id = nextId++;
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
    // cout << "Req " << req->id() << ": " << (req->requestor()?req->requestor()->name():"") << " -> " << _name << ", " << _inputTensorQueue.size() << endl;
    if(_prevTasks.size()>1)
    {
        unique_lock<mutex> lk(_inputTensorQueueLock);        
        auto &task = req->task();
        auto &requestor = req->requestor();
        if(requestor)
        {
            //bool multiInputReady = true;
            auto requestorId = requestor->id();
            auto indices = task->input_index()[requestorId];
            int cnt = 0;
            for(auto &index : indices)
            {
                // cout << "push " << index << endl;
                _inputTensorQueue[index].push( req->inputs()[cnt] );
                cnt++;
            }
            for(auto &q : _inputTensorQueue)
            {
                if(q.empty())
                {
                    // cout << "        multi input NOT Ready" << endl;
                    return req->id();
                }
            }
            Tensors inputs;
            for(auto &q : _inputTensorQueue)
            {
                inputs.emplace_back(q.front());
                q.pop();
            }
            req->inputs() = move(inputs);
            // cout << "        multi input Ready" << endl;
        }
    }    
    if(_processor==Processor::NPU)
    {
        auto device = PickOneDevice(_devices);
        req->CheckTimePoint(0);
        req->model_type() = _npuModel.front().type;
        device->InferenceRequest(req);
    }
    else
    {
        req->CheckTimePoint(0);
        _cpuHandle->InferenceRequest(req);
    }
    return req->id();
}
int Task::ProcessResponse(RequestPtr req, dxrt_response_t *response)
{    
    _inferenceCnt++;
    req->CheckTimePoint(1);
    // cout << "    Response : " << req->id() << ", " << (req->requestor()?req->requestor()->name():"") << " -> " << req->task()->name() << ", " << req->latency() << endl;
    if(_processor==Processor::NPU)
    {
        req->inference_time() = response->inf_time;
    }
    else
    {
        req->inference_time() = req->latency();
    }
    if(!_nextTasks.empty())
    {
        // LOG_VALUE(_nextTasks.size());
        for(auto &next:_nextTasks)
        {            
            auto outputs = SelectElements(req->outputs(), _outputTensorIdx[next->id()] );
            auto nextReq = Request::Create(next.get(), outputs, {}, req->user_arg(), req->last_output_ptr());
            
            nextReq->head() = req->head();
            nextReq->requestor() = req->task();
            nextReq->SetStatus(Request::Status::REQ_BUSY);
            // cout << *nextReq << endl;
            // cout << "Req " << nextReq->id() << ": " << _name << " -> " << next->name() << endl;
            next->InferenceRequest(nextReq);
            // cout << "Req " << nextReq->id() << ": " << _name << " -> " << next->name() << " done" << endl;
            // cout << "inf. request done" << endl;
        }
    }
    else
    {
        LOG_DXRT_DBG << "tail task done: " << _id << ", " << req->id() << endl;
        if(_processor==Processor::NPU)
        {
            if(req->model_type()==1)
            {
                // LOG_VALUE(response->argmax);
                *((uint16_t*)(req->outputs().front().data())) = response->argmax;

            }
            else if(req->model_type()==2)
            {
                // LOG_VALUE(response->ppu_filter_num);
                vector<int64_t> shape{response->ppu_filter_num};
                req->outputs().front().shape() = shape;
            }
        }
        auto &head = req->head();
        if(head->task()!=this)
        {
            head->outputs() = req->outputs();
            head->NotifyCompletion();
            head->SetStatus(Request::Status::REQ_DONE);
            if(head->complete_cnt()==_completeCnt)
            {
                // cout << "tail task done: task" << _id << ", " << req->id() << endl;
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
        if(_callBack!=nullptr)
        {
            LOG_DXRT_DBG << "task callback" << endl;
            future<void> result = async(launch::async, [&](void){
                TensorPtrs ret;
                for(auto &tensor:req->outputs())
                {
                    ret.emplace_back(
                        make_shared<Tensor>(tensor)
                    );
                }
                _callBack(ret, req->user_arg());
                // _callBack(req->outputs(), req->user_arg());
            } );
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
    return _id;
}
string Task::name()
{
    return _name;
}

void *Task::input_buf(int deviceId, int bufId)
{
    return _devices[deviceId]->input_buf(_id, bufId);
}

Tensors Task::inputs(void* ptr, uint64_t phyAddr)
{
    if(ptr==nullptr)
    {
        return _inputTensors;
    }
    else
    {
        Tensors ret(_inputTensors);
        int i=0;
        for(auto &t:ret)
        {
            t.data() = static_cast<void*>(static_cast<uint8_t*>(ptr) + _inputOffsets[i]);
            t.phy_addr() = phyAddr + _inputOffsets[i];
            i++;
        }
        return ret;
    }
    return _inputTensors;
}

Tensors Task::outputs(void* ptr, uint64_t phyAddr)
{
    if(ptr==nullptr)
    {
        return _outputTensors;
    }
    else
    {
        Tensors ret(_outputTensors);
        int i=0;
        for(auto &t:ret)
        {
            t.data() = static_cast<void*>(static_cast<uint8_t*>(ptr) + _outputOffsets[i]);
            t.phy_addr() = phyAddr + _outputOffsets[i];
            i++;
        }
        return ret;
    }
    return _outputTensors;
}

Processor Task::processor()
{
    return _processor;
}

uint32_t Task::input_size()
{
    return _inputSize;
}

uint32_t Task::output_size()
{
    return _outputSize;
}
uint32_t Task::output_mem_size()
{
    return _outputMemSize;
}
map<int, vector<int>> &Task::input_index()
{
    return _inputTensorIdx;
}
map<int, vector<int>> &Task::output_index()
{
    return _outputTensorIdx;
}
atomic<int> &Task::inference_count()
{
    return _inferenceCnt;
}

vector<rmapinfo> Task::npu_param()
{
    return _infos;
}
vector<dxrt_model_t> Task::npu_model()
{
    return _npuModel;
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
    return _isArgMax;
}
std::function<int(TensorPtrs&, void*)> Task::callback()
{
    return _callBack;
}
int Task::latency()
{
    if(_latency.IsEmpty())
        return 0;
    return _latency.Get();
}
uint32_t Task::inference_time()
{
    if(_infTime.IsEmpty())
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
    os << dec << "  Task[" << task._id << "] " << task._name << ", " << task._processor 
        << ", " << task._memUsage << "bytes (input " << task._inputSize << ", output " << task._outputSize << ")" << endl;
    os << "    inputs" << endl;
    for(auto &tensor : task._inputTensors) os << "      " << tensor << endl;
    os << "    outputs" << endl;
    for(auto &tensor : task._outputTensors) os << "      " << tensor << endl;    
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

} // namespace dxrt
