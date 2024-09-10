// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/datatype.h"
#include "dxrt/task.h"
#include "dxrt/device.h"
#include "dxrt/util.h"
#include "dxrt/request.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/inference_engine.h"
#include "dxrt/filesys_support.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#ifdef __linux__
    #include <cxxabi.h>
#elif _WIN32
#endif
#include <regex>

#define PRINT_ALL_INFERENCE_ENGINE

using namespace std;

namespace dxrt
{
InferenceEngine::InferenceEngine(const string &path_, InferenceOption &option_)
:_modelFile(path_), _option(option_), _latency(30), _infTime(30)
{
    _modelDir = getParentPath(getAbsolutePath(_modelFile));

    LOG_DXRT_DBG <<_modelFile << endl;
    LOG_DXRT_DBG << getAbsolutePath(_modelFile) << endl;
    LOG_DXRT_DBG << _modelDir << endl;

    /* check files */
    if(dxrt::fileExists(_modelFile))
    {
        _modelFile = string(getAbsolutePath(_modelFile));
        _name = _modelFile;        
        _modelData = LoadModelParam(_modelFile);
        _taskOrder = _modelData.deepx_graph.topoSort_order();
        // topoSort_order = {"npu_1"};// for debug

        //version check



        if(_taskOrder.empty())
        {
            _taskOrder.push_back(
                _modelData.deepx_binary.rmap_info(0).name()
            );
            // cout << modelData.deepx_binary.rmap_info(0).name() << endl;
        }
        // auto graphs = _modelData.deepx_graph.m_graph();
        // for(auto &order : topoSort_order )
        // {
        //     for(auto &graph : graphs)
        //     {
        //         cout << graph.name() << endl;


        //     }
        // }
        // exit(1);
        for(auto &order : _taskOrder )
        {
            vector<dxrt::rmapinfo> rmapInfos;
            vector<vector<uint8_t>> data;
            bool found = false;
            auto graphs = _modelData.deepx_graph.m_graph();
            for(auto &graph:graphs)
            {
                if(order==graph.name())
                {
                    _graphMap[graph.name()] = graph;
                    break;
                }
            }
            // for(int j=0; j<_modelData.deepx_graph.m_graph().size(); j++)
            // {
            //     auto &graph = _modelData.deepx_graph.m_graph(j);
            //     // auto graphs = modelData.deepx_graph.m_graph();
            //     // for(auto &graph:graphs)
            //     {
            //         if(order == graph.name())
            //         {
            //             cout << order << endl;
            //         }
            //         // cout << graph.name() << endl;
            //     }
            // }
            // cout << "check " << order << endl;
            for(size_t j=0; j<_modelData.deepx_binary.rmap_info().size(); j++)
            {
                if(order == _modelData.deepx_binary.rmap_info(j).name())
                {
                    // cout << "found " << order << endl;
                    auto rmapInfo = _modelData.deepx_rmap.m_rmap(j);
#define MIN_CG_VERSION "2.20.0"
                    // version check
                    string version_str = rmapInfo.version().rmapinfo();
                    DXRT_ASSERT(isSupporterModelVersion(version_str),
                       (string("Not Supported model compiler version ")+version_str).c_str());


                    if(_graphMap.find(order)!=_graphMap.end())
                        rmapInfo.input().memory().name() = _graphMap[order].inputs().front().key(); // Temp. workaround : rmapinfo doesn't have input tensor name
                    rmapInfos.emplace_back(rmapInfo);
                    data.emplace_back(move(vector<uint8_t>(rmapInfo.memorys().memory(0).size(), 0)) );
                    memcpy(data.back().data(), _modelData.deepx_binary.rmap(j).buffer(), data.back().size());
                    DXRT_ASSERT(0 < data.back().size(), "invalid model");
                    data.emplace_back(move(vector<uint8_t>(rmapInfo.memorys().memory(1).size(), 0)) );
                    memcpy(data.back().data(), _modelData.deepx_binary.weight(j).buffer(), data.back().size());
                    DXRT_ASSERT(0 < data.back().size(), "invalid model");
                    found = true;
                }
            }
#ifdef USE_ORT
            if(!found)
            {
                for(size_t j=0; j<_modelData.deepx_binary.cpu_models().size(); j++)
                {
                    if(order == _modelData.deepx_binary.cpu_models(j).name())
                    {
// #ifndef USE_ORT
//                         DXRT_ASSERT(false, "Multi-task model is not supported if USE_ORT is disabled in dx-rt.");
// #endif
                        // cout << "found " << order << endl;
                        data.emplace_back(move(vector<uint8_t>(_modelData.deepx_binary.cpu_models(j).size(), 0)) );
                        memcpy(data.back().data(), (void*)_modelData.deepx_binary.cpu_models(j).buffer(), data.back().size());
                        found = true;
                        break;
                    }
                }
            }
#endif
            // DXRT_ASSERT(found==true, "invalid graph info in model");
            if(found)
            {
                _tasks.emplace_back( make_shared<Task>(order, rmapInfos, move(data)) );
                _taskMap[_tasks.back()->name()] = _tasks.back();
#ifndef USE_ORT
                break; // force single task
#endif
            }
        }
        _head = _tasks.front();
        _tail = _tasks.back();
        _head->is_head() = true;
        _tail->is_tail() = true;
        // exit(1);
    }
    else
    {
        DXRT_ASSERT(false, "Can't find " + _modelFile);
    }
    // task chain
    for(auto it = _tasks.begin(); it!=_tasks.end(); ++it)
    {
        auto elem = *it;
        if(next(it)!=_tasks.end())
            elem->next() = *(next(it));
        else
            elem->next() = nullptr;
    }

    for(auto &task : _tasks)
    {
        auto &graph = _graphMap[task->name()];
        auto &inputs = graph.inputs();
        auto &outputs = graph.outputs();
        auto &input_index = task->input_index();
        auto &output_index = task->output_index();
        // cout << graph.name() << endl;
        std::ignore = inputs;// for(auto &v:inputs) cout << v.key() << ", " << v.val() << endl;
        std::ignore = outputs;// for(auto &v:outputs) cout << v.key() << ", " << v.val() << endl;
        if(task != _tail) 
        {
            auto &nexts = task->nexts();
            int idx = 0;

            for (auto &node : graph.outputs()) {
                string tensorName = node.key();
                string nextTaskName = node.val();
                int nextTaskId;
                auto target = _taskMap.find(nextTaskName);

                if (target != _taskMap.end()) {

                    nextTaskId = target->second->id();
                    auto it = find(nexts.begin(), nexts.end(), target->second);

                    if (it == nexts.end()) {
                        nexts.emplace_back(target->second);
                    }

                    output_index[nextTaskId].push_back(idx);
                    LOG_DBG("[OUTPUT][" + task->name() + "] Tensorname : " + tensorName + " / nextTaskName : " + nextTaskName +", id : " +std::to_string(nextTaskId) + " / idx : " + std::to_string(idx));
                    idx++;
                    
                } else {
                    nextTaskId = -1;
                }
            }
        }

        if (task != _head) 
        {
            auto &prevs = task->prevs();
            //for (size_t i = 0; i < task->inputs().size(); ++i) {
            //    cout <<task->name() << "->inputs (" <<to_string(i) <<") " <<task->inputs()[i].name() << endl;
            //}
            std::vector<string> input_name_order;

            for (auto &node : inputs) {

                string tensorName = node.key();
                string prevTaskName = node.val();
                
                input_name_order.push_back(tensorName);

                int prevTaskId = _taskMap[prevTaskName]->id();
                auto target = _taskMap[prevTaskName];
                auto it = find(prevs.begin(), prevs.end(), target);

                if (it == prevs.end()) {
                    prevs.emplace_back(target);
                }
                if(input_index.find(prevTaskId) == input_index.end()){

                    std::map<string,int> position_in_outputs;
                    for (size_t i = 0; i < target->outputs().size(); ++i){
                        position_in_outputs[target->outputs()[i].name()] = i;
                    }

                    std::vector<int> relative_positions;
                    for (size_t i = 0; i < task->inputs().size(); ++i) {
                        if (position_in_outputs.find( task->inputs()[i].name()) != position_in_outputs.end()) {
                            relative_positions.push_back(position_in_outputs[task->inputs()[i].name()]);
                        }
                    }

                    std::vector<int> sorted_positions = relative_positions;
                    std::sort(sorted_positions.begin(), sorted_positions.end());

                    std::map<int, int> index_map;
                    for (size_t i = 0; i < sorted_positions.size(); ++i) {
                        index_map[sorted_positions[i]] = i;
                    }

                    std::vector<int> normalized_positions;
                    for (int pos : relative_positions) {
                        normalized_positions.push_back(index_map[pos]);
                    }
                    input_index[prevTaskId] = normalized_positions;

                }
                LOG_DBG("[INPUT][" + task->name() + "] Tensorname : " + tensorName + " / prevTaskName : " + prevTaskName +", id : " +std::to_string(prevTaskId) );
            }
            task->input_name_order(input_name_order);

            task->input_tensor_queue_set(task->prevs().size());//JG

            for (auto i = 0u; i < task->prevs().size(); ++i) {
                task->input_tensor_queue_idx()[task->prevs()[i]->name()] = static_cast<int>(i);
            }
            /*
            std::cout << "Input Index:\n";
            for (const auto& pair : input_index) {
                int prevTaskId = pair.first;
                const std::vector<int>& positions = pair.second;
                
                std::cout << "PrevTaskId " << prevTaskId << ": ";
                for (int pos : positions) {
                    std::cout << pos << " ";
                }
                std::cout << "\n";
            }*/
        }
        task->inference_engine() = this;
    }
    _numTails = 0;
    for(auto &task : _tasks)
    {
        if(task->nexts().empty())
        {
            _numTails++;
        }
    }
    LOG_DBG("_numTails : "+_numTails)
    DXRT_ASSERT(_numTails==1, "Invalid Graph : check the number of tail task \
                                (The final inference output aggregation for multi-tail tasks is not yet supported.)");

#ifdef PRINT_ALL_INFERENCE_ENGINE
    for(auto &task : _tasks)
    {
        cout << "[ ";
        for(auto &prev:task->prevs())
        {
            cout << prev->name() << ", ";
        }
        cout << " ] -> " << task->name() << " -> [";
        for(auto &next:task->nexts())
        {
            cout << next->name() << ", ";
        }
        cout << " ]" << endl;
    }
    cout << *this << endl;
#endif
    LOG_DBG("InferenceEngine created.");
}
InferenceEngine::~InferenceEngine(void)
{
    LOG_DXRT_DBG << endl;
    for(auto &task : _tasks)
    {
        // cout << *task << endl;
        // LOG_VALUE(task.use_count());
        task->prevs().clear();
        task->nexts().clear();
    }
    _tasks.clear();
    _taskMap.clear();
    // usleep(500);
}

TensorPtrs InferenceEngine::Run(void *inputPtr, void *userArg, void *outputPtr)
{
    auto req = Request::Create(_head.get(), inputPtr, outputPtr, userArg, outputPtr);
    req->requestor() = nullptr;
    req->SetStatus(Request::Status::REQ_BUSY);
    _head->InferenceRequest(req);
    req->Wait();
    TensorPtrs ret;
    for(auto &tensor:req->outputs())
    {
        ret.emplace_back(
            make_shared<Tensor>(tensor)
        );
    }
    return ret;
    // return req->outputs();
}
// void Model::Run(Tensors &inputs, Tensors &outputs)
// {    
//     /* TODO */
//     // RequestPtr req = Request::Create(_head.get(), inputs, outputs);
//     // _head->InferenceRequest(req);
// }
int InferenceEngine::RunAsync(void *inputPtr, void *userArg, void *outputPtr)
{
    auto req = Request::Create(_head.get(), inputPtr, outputPtr, userArg, outputPtr);
    req->requestor() = nullptr;
    req->SetStatus(Request::Status::REQ_BUSY);
    _head->InferenceRequest(req);
    return req->id();
}
void InferenceEngine::RegisterCallBack(function<int(TensorPtrs &outputs, void *userArg)> f)
{
    LOG_DXRT_DBG << endl;
    _tail->RegisterCallBack(f);;
}
float InferenceEngine::RunBenchMark(int num)
{
    float sum = 0.;
    auto& profiler = dxrt::Profiler::GetInstance();
    vector<float> fps;
    int todo = max(30, num);
    while(todo>0)
    {
        uint64_t infTime = 0;
        int infCnt = min(todo, REQUEST_ID_MAX_VALUE);
        _tail->inference_count() = 0;
        profiler.Start("benchmark");
        // profiler.Start("req");
        for(int i=0 ; i<infCnt ; i++)
        {
            RunAsync(nullptr);
        }
        // profiler.End("req");
        while(_tail->inference_count() < infCnt);
        profiler.End("benchmark");
        infTime = profiler.Get("benchmark");
        todo -= infCnt;
        LOG_VALUE(infTime);
        LOG_VALUE(infCnt);
        fps.emplace_back(1000000.0 * infCnt/infTime);
    }
    profiler.Erase("benchmark");
    for(auto &val:fps)
    {
        sum += val;
        cout << "fps: " << val << endl;
    }
    return sum / fps.size();
}
TensorPtrs InferenceEngine::ValidateDevice(void *inputPtr, int deviceId)
{
    DXRT_ASSERT(_head->processor()==Processor::NPU, "invalid model for device validation");
    DXRT_ASSERT(_tasks.size()==1, "invalid model for device validation");
    auto devices = dxrt::CheckDevices();
    DXRT_ASSERT(static_cast<size_t>(deviceId) < devices.size(), "invalid device id");
    auto req = Request::Create(_head.get(), inputPtr, nullptr, nullptr, nullptr);
    req->requestor() = nullptr;
    req->SetStatus(Request::Status::REQ_BUSY);
    return devices[deviceId]->Validate(req);
}
TensorPtrs InferenceEngine::Wait(int reqId)
{
    LOG_DXRT_DBG << reqId << endl;
    auto req = Request::GetById(reqId);
    while(req->status()== Request::Status::REQ_BUSY);
    LOG_DXRT_DBG << reqId << " done" << endl;
    // return req->outputs();
    TensorPtrs ret;
    for(auto &tensor:req->outputs())
    {
        ret.emplace_back(
            make_shared<Tensor>(tensor)
        );
    }
    return ret;
}
void InferenceEngine::PushLatency(int latency)
{
    _latency.Push(latency);
}
void InferenceEngine::PushInferenceTime(uint32_t infTime)
{
    _infTime.Push(infTime);
}
Tensors InferenceEngine::inputs(void *ptr, uint64_t phyAddr)
{
    return _head->inputs(ptr, phyAddr);
}
Tensors InferenceEngine::outputs(void *ptr, uint64_t phyAddr)
{
    return _tail->outputs(ptr, phyAddr);
}
vector<Tensors> InferenceEngine::inputs(int devId)
{
    auto devices = dxrt::CheckDevices();
    if(devices.empty()) return {};
    auto device = devices[devId];
    return device->inputs(_head->id());
}
uint64_t InferenceEngine::input_size()
{
    return _head->input_size();
}

uint64_t InferenceEngine::output_size()
{
    return _tail->output_size();
}
string InferenceEngine::name()
{
    return _name;
}
vector<string> InferenceEngine::task_order()
{
    return _taskOrder;
}
int InferenceEngine::latency()
{
    LOG_DXRT_DBG << endl;
#if 0
    int sum = 0;
    for(auto &task:_tasks)
    {
        sum += task->latency();
    }
    return sum;
#else
    if(_latency.IsEmpty())
        return 0;
    return _latency.Get();
#endif
}
uint32_t InferenceEngine::inference_time()
{
    LOG_DXRT_DBG << endl;
#if 0
    uint32_t sum = 0;
    for(auto &task:_tasks)
    {
        sum += task->inference_time();
    }
    return sum;
#else
    if(_infTime.IsEmpty())
        return 0;
    return _infTime.Get();
#endif
}
vector<TensorPtrs> InferenceEngine::GetOutputs()
{
    LOG_DXRT_DBG << "Collecting outputs from all tasks in order." << endl;
    vector<TensorPtrs> all_outputs;

    // Iterate through each task in the task order
    for (const auto& task_name : _taskOrder)
    {
        // Find the corresponding task in the task map
        auto it = _taskMap.find(task_name);
        if (it != _taskMap.end())
        {
            // Get the outputs of the task
            auto task = it->second;
            TensorPtrs task_outputs;
            for (auto& tensor : task->outputs())
            {
                task_outputs.emplace_back(make_shared<Tensor>(tensor));
            }
            // Add the outputs to the list
            all_outputs.push_back(task_outputs);
        }
        #ifdef USE_ORT
        else
        {
            cout << "Task " << task_name << " not found in task map." << endl;
        }
        #endif
    }
    return all_outputs;
}
vector<uint8_t> InferenceEngine::bitmatch_mask(int index)
{
    vector<uint8_t> data(_modelData.deepx_binary.bitmatch_mask(index).buffer(), _modelData.deepx_binary.bitmatch_mask(index).buffer() + _modelData.deepx_binary.bitmatch_mask(index).size());
    return data;
}

ostream& operator<<(ostream& os, const InferenceEngine& ie)
{
    os << " InferenceEngine " << ie._name << endl;
    // for(auto &task:ie._tasks)
    // {
    //     os << *task << endl;
    // }
    for(auto &pair:ie._taskMap)
    {
        os << *pair.second << endl;
    }
    return os;
}

} // namespace dxrt
