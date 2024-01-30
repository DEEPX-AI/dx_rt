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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <filesystem>
#include <cxxabi.h>
#include <regex>

using namespace std;

namespace dxrt
{
InferenceEngine::InferenceEngine(const string &path_, InferenceOption &option_)
:_modelFile(path_), _option(option_)
{
    _modelDir = string(filesystem::absolute(_modelFile).parent_path());
    /* check files */
    if(filesystem::exists(_modelFile))
    {
        _modelFile = string(filesystem::absolute(_modelFile));
        _name = _modelFile;
        deepx_modelinfo::ModelInfoDatabase modelData;
        modelData = LoadModelParam(_modelFile);
        vector<string> topoSort_order; 
        topoSort_order = modelData.deepx_graph.topoSort_order();
        if(topoSort_order.empty())
        {
            topoSort_order.push_back(
                modelData.deepx_binary.rmap_info(0).name()
            );
            // cout << modelData.deepx_binary.rmap_info(0).name() << endl;
        }
        for(auto &order : topoSort_order )
        {
            vector<dxrt::rmapinfo> rmapInfos;
            vector<vector<uint8_t>> data;
            bool found = false;
            // cout << "check " << order << endl;
            for(int j=0; j<modelData.deepx_binary.rmap_info().size(); j++)
            {
                if(order == modelData.deepx_binary.rmap_info(j).name())
                {
                    // cout << "found " << order << endl;
                    auto rmapInfo = modelData.deepx_rmap.m_rmap(j);
                    rmapInfos.emplace_back(rmapInfo);
                    data.emplace_back(move(vector<uint8_t>(rmapInfo.memorys().memory(0).size(), 0)) );
                    memcpy(data.back().data(), modelData.deepx_binary.rmap(j).buffer(), data.back().size());
                    DXRT_ASSERT(0 < data.back().size(), "invalid model");
                    data.emplace_back(move(vector<uint8_t>(rmapInfo.memorys().memory(1).size(), 0)) );
                    memcpy(data.back().data(), modelData.deepx_binary.weight(j).buffer(), data.back().size());
                    DXRT_ASSERT(0 < data.back().size(), "invalid model");
                    found = true;
                }
            }
            if(!found)
            {
                for(int j=0; j<modelData.deepx_binary.cpu_models().size(); j++)
                {
                    if(order == modelData.deepx_binary.cpu_models(j).name())
                    {
#ifndef USE_ORT
                        DXRT_ASSERT(false, "Multi-task model is not supported if USE_ORT is disabled in dx-rt.");
#endif
                        // cout << "found " << order << endl;
                        data.emplace_back(move(vector<uint8_t>(modelData.deepx_binary.cpu_models(j).size(), 0)) );
                        memcpy(data.back().data(), (void*)modelData.deepx_binary.cpu_models(j).buffer(), data.back().size());
                        found = true;
                        break;
                    }
                }
            }
            DXRT_ASSERT(found==true, "invalid graph info in model");
            _tasks.emplace_back( make_shared<Task>(order, rmapInfos, move(data)) );
            // break; // force single task for test
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
    auto task = _head;
    while(1)
    {
        cout << task->name();
        task = task->next();
        if(task==nullptr)
            break;
        else
            cout << " -> ";
    }
    cout << endl;
    cout << *this << endl;
    LOG_DBG("InferenceEngine created.");
}
InferenceEngine::~InferenceEngine(void)
{
    // cout << "--------Model::Release--------" << endl;
    LOG_DBG("InferenceEngine released.");
    // usleep(500);
    // cout << *this << endl;
}

TensorPtrs InferenceEngine::Run(void *inputPtr, void *userArg, void *outputPtr)
{
    auto req = Request::Create(_head.get(), inputPtr, outputPtr, userArg, outputPtr);
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
static int DataIntegrityCheckCallBack(vector<shared_ptr<Tensor>> outputs, void *arg,
    atomic<int> &passCnt, atomic<int> &failCnt, atomic<int> &callBackCnt, InferenceEngine *ie, vector<TestData> &dataPool, bool print)
{
    auto& profiler = dxrt::Profiler::GetInstance();
    profiler.Start("callback");
    int cmp = -1;
    uint64_t idx = (uint64_t) arg;
    if(print)
    {
        cout << "  [" << callBackCnt << "] " << endl;
    }
    if(dataPool.empty())
    {
        cout << "dataPool empty" << endl;
        callBackCnt++;
        return 0;
    }
    auto &data = dataPool[idx];
    if(data.size<=0)
    {
        cout << "data empty" << endl;
        callBackCnt++;
        return 0;
    }
    for(auto &refOutput:data.refOutput)
    {        
        if(!refOutput.empty() && data.size>0)
        {
            auto dataType = ie->outputs().front().type();
            if(dataType>=DataType::BBOX && dataType<=DataType::POSE)
            {
                if(data.type==0)
                    cmp = 0;
                else
                    cmp = DataCompare(refOutput.data(), outputs.front()->data(), data.size);
            }
            else
            {
                cmp = DataCompare(refOutput.data(), outputs.front()->data(), ie->output_size());
                // cmp = DataCompare(refOutput.data(), outputs.front()->GetData(), data.size);
            }
        }
        if(cmp==0) break;
    }
    /* 0 : data same, -1 : different at offset zero, n(>0) : different at offset n */
    if(cmp==0)
    {
        passCnt++;
    }
    else
    {
        failCnt++;
        if(cmp==-1)
        {
            cmp = 0; // offset zero
        }
        // if(data.refOutput.size()==1)
        // {
        //     cout << "[" << callBackCnt << "] diff. at " << hex << cmp << ": " 
        //         << *(((uint64_t*)data.refOutput.front().data())+cmp/sizeof(uint64_t)) << " vs "
        //         << *(((uint64_t*)outputs.front()->data())+cmp/sizeof(uint64_t)) << dec << endl;
        // }
        // else
        // {
        //     cout << "[" << callBackCnt << "] diff. at " << hex << cmp << endl;
        // }
        cout << "[" << callBackCnt << "] diff. at " << hex << cmp << endl;
        // ie->ReadOutput(data.outputFile + ".fail", nullptr, data.type);
        DataDumpBin(data.outputFile + ".fail", outputs.front()->data(), ie->output_size());
    }
    callBackCnt++;
    profiler.End("callback");
    return 0;
}
pair<int, int> InferenceEngine::RunDataIntegrityCheck(int loops, vector<TestData> &dataPool)
{
    auto& profiler = dxrt::Profiler::GetInstance();
    atomic<int> pass = 0, fail = 0, callBackCnt = 0;

    /* If data pool is empty, get test data from model path */
    string inputPattern = "npu_input_(\\d+)\\.bin";
    vector<string> outputPatterns{"npu_output_$1.bin", "npu_output_$1.argmax.bin", "npu_output_$1.ppu.bin"};
    auto dataType = _tail->outputs().front().type();
    auto elemSize = _tail->outputs().front().elem_size();
    auto isArgmax = _tail->is_argmax();
    regex inputRegex(inputPattern);
    int id = 0;
    // if(loops<30) loops = 30;
    if(dataPool.empty())
    {
        for(const auto& entry : filesystem::directory_iterator(_modelDir))
        {
            if(filesystem::is_regular_file(entry.path()))
            {
                string fileName = entry.path().filename().string();                    
                if(regex_match(fileName, inputRegex))
                {
                    for(auto &outputPattern:outputPatterns)
                    {
                        string refOutputFile = entry.path().parent_path().string() + "/" + regex_replace(fileName, inputRegex, outputPattern);
                        if(filesystem::exists(refOutputFile))
                        {
                            TestData data(
                                id,
                                entry.path().string(),
                                {refOutputFile},
                                entry.path().parent_path().string() + "/" + regex_replace(fileName, inputRegex, outputPattern) + ".rt",
                                _modelDir,
                                input_size(),
                                output_size()
                            );
                            dataPool.emplace_back(move(data));
                            id++;
                        }
                    }
                }
            }
        }
    }
    for(auto &data:dataPool)
    {                
        if(dataType>=DataType::BBOX && dataType<=DataType::POSE)
        {
            int bufSize = 16*1024;
            if(data.size == _tasks.back()->npu_model().front().output_all_size - bufSize)
            {
                data.type = 0;
            }
            else
            {
                data.type = 1;
                data.refOutput.front() = move(vector<uint8_t>(bufSize, 0));
                if(data.size <= bufSize)
                {
                    DataFromFile(data.refOutputFile.front(), data.refOutput.front().data());
                }
                else
                {
                        int totalElems = data.size / elemSize;
                        int elemsPerHw = bufSize / elemSize;
                        data.size = bufSize;
                        LOG_VALUE(totalElems);
                        LOG_VALUE(elemsPerHw);
                        cout << "Overflow data case: " << totalElems << " elements." << endl;
                        ifstream in(data.refOutputFile.front(), ifstream::binary);
                        if(in)
                        {
                            while(true)
                            {
                                in.read((char*)data.refOutput.front().data(), bufSize);
                                int bytesRead = in.gcount();
                                cout << bytesRead << endl;
                                if(in.eof())
                                {
                                    break;
                                }
                            }
                            in.close();
                        }
                        // DataDumpBin("debug.bin", (void*)data.refOutput.data(), bufSize);
                }
            }
        }
    }
    // for(auto &data:dataPool)
    // {
    //     data.Show();
    // }
    cout << "Data test: Sequential" << endl;
    bool stop = false;
    for(auto &data:dataPool)
    {
        if(stop) break;
        data.Show();
        // if(!data.refOutput.empty() && data.refOutput.front().empty())
        // {
        //     cout << "    empty data" << endl;
        //     continue;
        // }
        for(int loop=0; loop<loops; loop++)
        {
            int ret = -1;
            string FailLog;
            auto outputs = Run(data.input.data());
            auto task = _tasks.back();            
            std::vector<uint8_t> *refOutputPtr = nullptr;
            for(auto &refOutput:data.refOutput)
            {
                if(data.size>0)
                {
                    if(task->processor()==Processor::NPU)
                    {
                        auto npuParam = task->npu_param().front();
                        if(isArgmax)
                        {
                            ret = DataCompare(refOutput.data(), outputs.front()->data(), output_size());
                            // LOG_VALUE_HEX(*(uint16_t*)refOutput.data()); LOG_VALUE_HEX(*(uint16_t*)outputs.front()->data());
                        }
                        else if(dataType>=DataType::BBOX && dataType<=DataType::POSE)
                        {
                            // ret = DataCompare(dataType, data.refOutput.data(), outputs.front()->GetData(), data.refOutput.size());
                            ret = DataCompare(dataType, refOutput.data(), outputs.front()->data(), data.size);
                        }
                        else
                        {
                            ret = DataCompare((char*)refOutput.data(), (char*)outputs.front()->data(), npuParam, &FailLog, 0, data.type);
                        }
                    }
                    else
                    {
                        ret = memcmp(refOutput.data(), outputs.front()->data(), output_size());
                    }
                }
                else
                {
                    ret = 0;
                }
                if(ret==0)
                {
                    refOutputPtr = &refOutput;
                    break;
                }
            }
            // LOG_VALUE(ret);
            if(data.refOutput.empty())
            {
                ret = 0;
            }
            else if(ret==0)
            {
                pass++;
            }
            else
            {
                int cmp = ret==-1?0:ret;
                fail++;
                // if(data.refOutput.size()==1)
                // {
                //     cout << " diff. at " << hex << cmp << ": " 
                //         << *(((uint64_t*)data.refOutput.front().data())+cmp/sizeof(uint64_t)) << " vs "
                //         << *(((uint64_t*)outputs.front()->data())+cmp/sizeof(uint64_t)) << dec << endl;
                // }
                // else
                // {
                //     cout << " diff. at " << hex << cmp << endl;
                // }
                cout << " diff. at " << hex << cmp << endl;
#if 0
                ReadOutput(data.outputFile, nullptr, data.type);
#else
                DataDumpBin(data.outputFile, outputs.front()->data(), output_size());
#endif
                // DataDumpBin(data.refOutputFile+".debug", data.refOutput.data(), data.refOutput.size());
            }
            /* last loop */
            if(loop==(loops-1))
            {
                if(0==ret)
                {
                    // overwrite ref. output buf. for parallel test
                    // memcpy(data.refOutput.data(), outputs.front()->GetData(), data.refOutput.size()<GetOutputSize()?data.refOutput.size():GetOutputSize());
                    // cout << data.refOutputFile << endl;
                    if(refOutputPtr!=nullptr)
                    {
                        // memcpy(refOutputPtr->data(), outputs.front()->data(), data.refOutput.size()<output_size()?data.refOutput.size():output_size());
                        memcpy(refOutputPtr->data(), outputs.front()->data(), output_size());
                    }
                }
                else
                {
                    cout << "error: " << data.inputFile << endl;
                }
                // dump to file
#if 0
                ReadOutput(data.outputFile, nullptr, data.type);
#else
                // for(auto &output:outputs)
                // {
                //     for(int i=0;i<10;i++) cout << *((float*)output->data() + i) << endl;
                // }
                DataDumpBin(data.outputFile, outputs.front()->data(), output_size());
                // cout << hex << *(uint16_t*)(outputs.front()->data()) << endl;
#endif
            }
            if(fail>0)
            {
                stop = true;
                break;
            }
        }
    }
    auto devices = dxrt::CheckDevices();
    if(fail==0 && pass>0 && devices.front()->info().type==0)
    {
        cout << "Data test: Parallel" << endl;        
        RegisterCallBack(
            [&](vector<shared_ptr<Tensor>> outputs, void* arg)->int
            {
                return DataIntegrityCheckCallBack(outputs, arg,
                    pass, fail, callBackCnt, this, dataPool, false);
            }
        );
        for(auto &data:dataPool)
        {
            data.Show();
            int units = REQUEST_ID_MAX_VALUE;
            callBackCnt = 0;
            while(callBackCnt<loops)
            {
                int check = callBackCnt + units;
                int todo;
                if(check>loops)
                {
                    todo = loops - callBackCnt;
                }
                else
                {
                    todo = units;
                }
                // LOG_VALUE(callBackCnt);
                // LOG_VALUE(todo);
                thread waitThread( [&](void) {
                    // Wait(todo);
                    while( (callBackCnt%units) != todo);
                });
                thread requestThread( [&](void) {
                    for(int i=0; i<todo; i++)
                    {
                        RunAsync( data.input.data(), (void*)data.id );
                    }
                });
                requestThread.join();
                waitThread.join();
                usleep(50*1000);
            }
        }
    }
    return make_pair((int)pass, (int)fail);
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
int InferenceEngine::latency()
{
    int sum = 0;
    for(auto &task:_tasks)
    {
        sum += task->latency();
    }
    return sum;
}
uint32_t InferenceEngine::inference_time()
{
    uint32_t sum = 0;
    for(auto &task:_tasks)
    {
        sum += task->inference_time();
    }
    return sum;
}

ostream& operator<<(ostream& os, const InferenceEngine& ie)
{
    os << " InferenceEngine " << ie._name << endl;
    for(auto &task:ie._tasks)
    {
        os << *task << endl;
    }
    return os;
}

} // namespace dxrt
