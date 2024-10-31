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
#include "dxrt/inference_job.h"

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
:_modelFile(path_), _option(option_)
{
    _modelDir = getParentPath(getAbsolutePath(_modelFile));

    LOG_DXRT_DBG <<_modelFile << endl;
    LOG_DXRT_DBG << getAbsolutePath(_modelFile) << endl;
    LOG_DXRT_DBG << _modelDir << endl;

    /* check files */
    if (dxrt::fileExists(_modelFile))
    {
        _modelFile = string(getAbsolutePath(_modelFile));
        _name = _modelFile;
        _modelData = LoadModelParam(_modelFile);
        _taskOrder = _modelData.deepx_graph.topoSort_order();
        // topoSort_order = {"npu_1"};// for debug

        // version check



        if (_taskOrder.empty())
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
        for (auto &order : _taskOrder )
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

                   for (size_t i = 0; i < 2; ++i) {
                        const auto& bufferSource = (i == 0) ? _modelData.deepx_binary.rmap(j).buffer() : _modelData.deepx_binary.weight(j).buffer();
                        
                        data.emplace_back(bufferSource.begin(), bufferSource.end());
                        DXRT_ASSERT(!data.back().empty(), "invalid model");
                    }
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
                        const auto& bufferSource =_modelData.deepx_binary.cpu_models(j).buffer();
                        data.emplace_back(bufferSource.begin(), bufferSource.end());
                        found = true;
                        break;
                    }
                }
            }
#endif
            // DXRT_ASSERT(found==true, "invalid graph info in model");
            if (found)
            {
                std::vector<dxrt::DevicePtr> devices = CheckDevices();
                std::vector<dxrt::DevicePtr> selected_devices = {};
                if(_option.devices.size() == 0)
                {
                    selected_devices = devices;
                }
                else
                {
                    for(int dev_id : _option.devices)
                    {
                        selected_devices.push_back(devices[dev_id]);
                    }
                }
                _tasks.emplace_back( make_shared<Task>(order, rmapInfos, move(data), static_cast<npu_bound_op>(_option.boundOption), selected_devices));
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
    for (auto it = _tasks.begin(); it != _tasks.end(); ++it)
    {
        auto elem = *it;
        if (next(it) != _tasks.end())
            elem->next() = *(next(it));
        else
            elem->next() = nullptr;
    }

    for (auto &task : _tasks)
    {
        auto &graph = _graphMap[task->name()];
        auto &inputs = graph.inputs();
        auto &outputs = graph.outputs();
        auto &input_index = task->input_index();
        auto &output_index = task->output_index();
        // cout << graph.name() << endl;
        std::ignore = inputs;  // for(auto &v:inputs) cout << v.key() << ", " << v.val() << endl;
        std::ignore = outputs;  // for(auto &v:outputs) cout << v.key() << ", " << v.val() << endl;
        if (task != _tail)
        {
            auto &nexts = task->nexts();
            int idx = 0;

            for (auto &node : graph.outputs()) {
                string tensorName = node.key();
                string nextTaskName = node.val();
                int nextTaskId;
                auto target = _taskMap.find(nextTaskName);

                if (target != _taskMap.end())
                {
                    nextTaskId = target->second->id();
                    auto it = find(nexts.begin(), nexts.end(), target->second);

                    if (it == nexts.end()) {
                        nexts.emplace_back(target->second);
                    }

                    output_index[nextTaskId].push_back(idx);
                    LOG_DBG("[OUTPUT][" + task->name() + "] Tensorname : " + tensorName + " / nextTaskName : " + nextTaskName+", id : " +std::to_string(nextTaskId) + " / idx : " + std::to_string(idx));
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
        task->SetInferenceEngineTimer(&_inferenceTimer);
    }
    _numTails = 0;
    for (auto &task : _tasks)
    {
        if(task->nexts().empty())
        {
            _numTails++;
        }
    }
    LOG_DBG("_numTails : "+_numTails)
    // DXRT_ASSERT(_numTails==1, "Invalid Graph : check the number of tail task 
    //                            (The final inference output aggregation for multi-tail tasks is not yet supported.)");

    InferenceJob::InitInferenceJob(); // static
    _occupiedInferenceJobs = std::vector<bool>(INFERENCE_ID_MAX_VALUE);
#ifdef PRINT_ALL_INFERENCE_ENGINE
    for (auto &task : _tasks)
    {
        cout << "[ ";
        for (const auto& prev : task->prevs())
        {
            cout << prev->name() << ", ";
        }
        cout << " ] -> " << task->name() << " -> [";
        for (const auto& next : task->nexts())
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
    for (size_t i = 0; i < _occupiedInferenceJobs.size(); ++i)
    {
        // wait for the job to finish
        if ( _occupiedInferenceJobs[i] ) {
            Wait(static_cast<int>(i));
        }
    }
    for (size_t i = 0; i < _occupiedInferenceJobs.size(); ++i)
    {
        if ( !_occupiedInferenceJobs[i] ) {
            InferenceJob::GetById(i)->Clear();
        }
    }
    for (auto &task : _tasks)
    {
        // cout << *task << endl;
        // LOG_VALUE(task.use_count());
        task->prevs().clear();
        task->nexts().clear();
        task->getData()->ClearOutputBuffer();
    }
    _tasks.clear();
    _taskMap.clear();
    _head.reset();
    _tail.reset();
    _userCallback = nullptr;
    Request::Clear();

    // usleep(500);
}

TensorPtrs InferenceEngine::Run(void *inputPtr, void *userArg, void *outputPtr)
{
    int jobId = RunAsync(inputPtr, userArg, outputPtr);
    return Wait(jobId);
}

int InferenceEngine::RunAsync(void *inputPtr, void *userArg, void *outputPtr)
{
    // return InferenceJob instance from InferenceJob pool (reused)
    std::shared_ptr<InferenceJob> &infJob = InferenceJob::Pick(); 

    infJob->SetInferenceJob(_tasks, _head);
    infJob->setInferenceEngineInterface(&_inferenceTimer);
    infJob->setCallBack([this](TensorPtrs &outputs, void *userArg, int jobId)->int{
        int retval = 0;
        if (_userCallback !=nullptr)
        {
            retval = _userCallback(outputs, userArg);
        }
        
        // unoccupired inference job id
        this->_occupiedInferenceJobs[jobId] = false;

        return retval;
    }); // inference engine callback 

    int jobId = infJob->startJob(inputPtr, userArg, outputPtr);

    // occupired inference job id
    _occupiedInferenceJobs[jobId] = true;

    return jobId;
}
void InferenceEngine::RegisterCallBack(function<int(TensorPtrs &outputs, void *userArg)> f)
{
    LOG_DXRT_DBG << endl;
    _userCallback = f;
}
float InferenceEngine::RunBenchMark(int num, void *inputPtr)
{
    float sum = 0.;
    auto& profiler = dxrt::Profiler::GetInstance();
    vector<float> fps;
    int todo = max(30, num);


    std::atomic<int> done_count;
    auto callBack = [&done_count](TensorPtrs &outputs, void *userArg) -> int{
        std::ignore = outputs;
        std::ignore = userArg;
        done_count++;
        return 0;
    }; //callback used to count inference
    RegisterCallBack(callBack);
    while (todo > 0)
    {
        uint64_t infTime = 0;
        int infCnt = min(todo, REQUEST_ID_MAX_VALUE);
        done_count = 0;
        profiler.Start("benchmark");
        // profiler.Start("req");
        for (int i=0 ; i < infCnt ; i++)
        {
            RunAsync(inputPtr);
        }
        // profiler.End("req");
        while (done_count < infCnt)continue;
        profiler.End("benchmark");
        infTime = profiler.Get("benchmark");
        todo -= infCnt;
        //LOG_VALUE(infTime);
        //LOG_VALUE(infCnt);
        fps.emplace_back(1000000.0 * infCnt/infTime);
    }
    profiler.Erase("benchmark");
    for (auto &val : fps)
    {
        sum += val;
        //cout << "fps: " << val << endl;
    }
    return sum / fps.size();
}
TensorPtrs InferenceEngine::ValidateDevice(void *inputPtr, int deviceId)
{
    Device::_sNpuValidateOpt = true;
    DXRT_ASSERT(_head->processor()==Processor::NPU, "invalid model for device validation");
    DXRT_ASSERT(_tasks.size()==1, "invalid model for device validation");
    auto devices = dxrt::CheckDevices();
    DXRT_ASSERT(static_cast<size_t>(deviceId) < devices.size(), "invalid device id");
    auto req = Request::Create(_head.get(), inputPtr, nullptr, nullptr);
    req->setCallback([](RequestPtr){});
    req->SetStatus(Request::Status::REQ_BUSY);
    devices[deviceId]->InferenceRequest(req->getData());
    req->Wait();
    Device::_sNpuValidateOpt = false;
    return devices[deviceId]->Validate(req, true);
}
TensorPtrs InferenceEngine::Wait(int jobId)
{
    LOG_DXRT_DBG << jobId << endl;
    
    // if unoccupied inference job id, return empty value
    if ( _occupiedInferenceJobs[jobId] == false ) {
        return {};
    }

    std::shared_ptr<InferenceJob>& infJob = InferenceJob::GetById(jobId);
    while (infJob->getStatus() == Request::Status::REQ_BUSY)
    {
        usleep(1);
        continue;
    }

    LOG_DXRT_DBG << jobId << " done" << endl;
    return infJob->getOutput();
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
    return _inferenceTimer.latency();
}
uint32_t InferenceEngine::inference_time()
{
    LOG_DXRT_DBG << endl;
    return _inferenceTimer.inference_time();
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
vector<uint8_t> InferenceEngine::bitmatch_mask(int index) {
    const vector<char>& maskBuffer = _modelData.deepx_binary.bitmatch_mask(index).buffer();
    vector<uint8_t> data(maskBuffer.begin(), maskBuffer.end());
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
