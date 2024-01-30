// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/task.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/inference_engine.h"
#include "dxrt/cpu_handle.h"
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
        for(auto &pair:_map)
        {
            auto &stats = pair.second;
            cout << "Task" << stats.id << ": latency " << stats.latency_us << "us, inference time " << stats.inference_time_us << "us" << endl;
        }
    }
    std::unordered_map<int, TaskStats> _map;
};
TaskStatsInstances taskStatsInstances;
TaskStats &TaskStats::GetInstance(int id)
{
    return taskStatsInstances._map[id];
}

Task::Task(string name_, vector<rmapinfo> rmapInfos_, std::vector<std::vector<uint8_t>>&& data_)
: _name(name_), _infos(rmapInfos_), _data(move(data_)), _latency(CircularBuffer<int>(30)), _infTime(CircularBuffer<uint32_t>(30))
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
        LOG_DXRT_DBG << "NPU Task: imported output shapes" << endl;
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
        for(int i=0; i<_infos.size(); i++)
        {
            auto &info = _infos[i];
            auto cmdSize = info.memorys().memory(0).size();
            auto weightSize = info.memorys().memory(1).size();
            dxrt_model_t model = {            
                .type = 0,
                .cmds = info.counts().cmd(),
                .cmd = {
                    .data = reinterpret_cast<uint64_t>( _data[ 2*i + 0 ].data() ),
                    .base = 0, // decided in device
                    .offset = 0, // defined in device
                    .size = cmdSize,
                },
                .weight = {
                    .data = reinterpret_cast<uint64_t>( _data[ 2*i + 1 ].data() ),
                    .base = 0, // decided in device
                    .offset = 0, // defined in device
                    .size = weightSize,
                },
                .output_all_offset = info.outputs().memory().offset(),
                .output_all_size = info.outputs().memory().size(),
                .last_output_offset = info.outputs().outputlist().output(0).memory().offset() - info.outputs().memory().offset(),
                .last_output_size = _outputSize,
            };
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
        _cpuHandle = make_shared<CpuHandle>( _data.front().data(), _data.front().size() );
        // cout << *_cpuHandle << endl;
        _numInputs = _cpuHandle->_numInputs;
        _numOutputs = _cpuHandle->_numOutputs;
        _inputSize = _cpuHandle->_inputSize;
        _outputSize = _cpuHandle->_outputSize;
        _outputMemSize = _outputSize;
        _memUsage = _outputSize;
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
    Request::Init();
}

Task::Task()
: _latency(CircularBuffer<int>(30)), _infTime(CircularBuffer<uint32_t>(30))
{
    _id = nextId++;
    LOG_DBG("Task created.");
}

Task::~Task(void)
{
    // LOG_DXRT << endl;
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
    if(_processor==Processor::NPU)
    {
        req->inference_time() = response->inf_time;
    }
    else
    {
        req->inference_time() = req->latency();
    }
    _latency.Push(req->latency());
    _infTime.Push(req->inference_time());
    if(_next)
    {
        auto nextReq= Request::Create(_next.get(), req->outputs().front().data(), nullptr, req->user_arg(), req->last_output_ptr());
        nextReq->head() = req->head();
        nextReq->SetStatus(Request::Status::REQ_BUSY);
        _next->InferenceRequest(nextReq);
    }
    else
    {        
        LOG_DXRT_DBG << "tail task done: " << _id << endl;
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
        if(_isTail)
        {
            req->head()->outputs() = req->outputs();
            req->head()->SetStatus(Request::Status::REQ_DONE);
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
            t.data() = ptr + _inputOffsets[i];
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
            t.data() = ptr + _outputOffsets[i];
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

ostream& operator<<(ostream& os, const Task& task)
{
    os << dec << "  Task[" << task._id << "] " << task._name << ", " << task._processor 
        << ", " << task._memUsage << "bytes (input " << task._inputSize << ", output " << task._outputSize << ")" << endl;
    os << "    inputs" << endl;
    for(auto &tensor : task._inputTensors) os << "      " << tensor << endl;
    os << "    outputs" << endl;
    for(auto &tensor : task._outputTensors) os << "      " << tensor << endl;    
    for(auto &device : task._devices)
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
    return os;
}

/* TODO : Devices */
/* TODO : Create Task MAP */

} // namespace dxrt
