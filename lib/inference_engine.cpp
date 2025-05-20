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
#include "dxrt/exception/exception.h"
#include "dxrt/device_info_status.h"
#include "resource/log_messages.h"
#include "dxrt/objects_pool.h"

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

struct BatchArgument
{
    void* userArg;
    int resultIndex;
};

static const int SUB_BATCH_MAX_COUNT = 128;

InferenceEngine::InferenceEngine(const std::string &path_, InferenceOption &option_)
:_modelFile(path_), _option(option_)
{
    _modelDir = getParentPath(getAbsolutePath(_modelFile));

    LOG_DXRT_DBG <<_modelFile << endl;
    LOG_DXRT_DBG << getAbsolutePath(_modelFile) << endl;
    LOG_DXRT_DBG << _modelDir << endl;
    const char* env = getenv("DXRT_DEBUG_DATA");

    if (env != nullptr) {
        try {
            DEBUG_DATA = std::stoi(env);
        } catch (const std::invalid_argument&) {
            cerr << "Environment variable DXRT_DEBUG_DATA is not a valid integer.\n";
        } catch (const std::out_of_range&) {
            cerr << "Environment variable DXRT_DEBUG_DATA is out of range.\n";
        }
    }
    if (DEBUG_DATA == 1 )
    {
        Device::_sNpuValidateOpt.store(true);
    }
#ifdef USE_ORT
    if (_option.useORT == true)
    {
        CpuHandle::SetDynamicCpuThread();
    }
#else
    if (_option.useORT == true)
    {
        throw dxrt::InvalidArgumentException("USE_ORT NOT SUPPORTED");
    }
#endif
    /* check files */
    if (dxrt::fileExists(_modelFile))
    {
        _modelFile = std::string(getAbsolutePath(_modelFile));
        _name = _modelFile;
        //_modelData = LoadModelParam(_modelFile);
        _modelCompileType = LoadModelParam(_modelData, _modelFile);
        std::vector<std::string> orgTaskOrder;
        orgTaskOrder = _modelData.deepx_graph.topoSort_order();

        // topoSort_order = {"npu_1"};// for debug

        if (orgTaskOrder.empty())
        {
            orgTaskOrder.push_back(
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
        for (auto &order : orgTaskOrder )
        {
            std::vector<dxrt::rmapinfo> rmapInfos;
            std::vector<std::vector<uint8_t>> data;
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

                    // version check
                    std::string version_str = _modelData.deepx_binary._compilerVersion;
                    if ( !isSupporterModelVersion(version_str) ) 
                        throw InvalidModelException(EXCEPTION_MESSAGE(LogMessages::NotSupported_ModelCompilerVersion(version_str, MIN_COMPILER_VERSION, "2.7.0")));


                    if(_graphMap.find(order)!=_graphMap.end())
                        rmapInfo.input().memory().name() = _graphMap[order].inputs().front().key(); // Temp. workaround : rmapinfo doesn't have input tensor name
                    rmapInfos.emplace_back(rmapInfo);

                   for (size_t i = 0; i < 2; ++i) {
                        const auto& bufferSource = (i == 0) ? _modelData.deepx_binary.rmap(j).buffer() : _modelData.deepx_binary.weight(j).buffer();
                        
                        data.emplace_back(bufferSource.begin(), bufferSource.end());
                        //DXRT_ASSERT(!data.back().empty(), "invalid model");
                        if ( data.back().empty() ) throw InvalidModelException(EXCEPTION_MESSAGE("invalid model"));

                    }
                    found = true;
                }
            }
#ifdef USE_ORT
            if ((found == false) && (_option.useORT == true))
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
                std::vector<dxrt::DevicePtr>& devices = CheckDevices();
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
                auto task = make_shared<Task>(order, rmapInfos, std::move(data), static_cast<npu_bound_op>(_option.boundOption), selected_devices);
                _tasks.emplace_back(task);

#ifdef USE_ORT
                if (_option.useORT == true)
                {
                    auto &graph = _graphMap[order];

                    for(auto src : graph.inputs())
                    {
                        if(src.val().empty())
                        {
                            if(_head==nullptr)
                            {
                                _head = task;
                                task->set_head();
                            }
                            else
                            {
                                if((_head == task) && (_head->processor()==Processor::NPU))
                                {
                                    DXRT_ASSERT(false, "NPU Multi-input is not supported.");
                                }
                                else
                                {
                                    DXRT_ASSERT(false, "Multi-head model(Start with multi task on the dxnn graph) is not supported.");
                                }
                            }
                        }
                    }
                    for(auto dst : graph.outputs())
                    {
                        if(dst.val().empty())
                        {
                            task->set_tail();
                            _tails.push_back(task);
                        }
                    }
                }
                else
#endif
                {
                    _head = task;
                    task->set_head();
                    _tails.push_back(task);
                    task->set_tail();
                }
                _taskMap[task->name()] = task;
                _taskOrder.push_back(task->name());
#ifdef USE_ORT
                if (_option.useORT == false)
#endif
                    break; // force single task
            }
        }
        // exit(1);
    }
    else
    {
        //DXRT_ASSERT(false, "Can't find " + _modelFile);
        throw dxrt::FileNotFoundException(_modelFile);
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
        //cout << graph.name() << endl;
        std::ignore = inputs;  // for(auto &v:inputs) cout << v.key() << ", " << v.val() << endl;
        std::ignore = outputs;  // for(auto &v:outputs) cout << v.key() << ", " << v.val() << endl;
        if (!(task->is_tail()))
        {
            auto &nexts = task->nexts();

            for (auto &node : graph.outputs()) {
                std::string tensorName = node.key();
                std::string nextTaskName = node.val();
                auto target = _taskMap.find(nextTaskName);

                if (target != _taskMap.end())
                {
                    auto it = std::find(nexts.begin(), nexts.end(), target->second);

                    if (it == nexts.end()) {
                        nexts.emplace_back(target->second);
                    }
                    LOG_DBG("[OUTPUT][" + task->name() + "] Tensorname : " + tensorName + " / nextTaskName : " + nextTaskName);
                }
            }
        }

        if (!(task->is_head())) 
        {
            auto &prevs = task->prevs();
            //for (size_t i = 0; i < task->inputs().size(); ++i) {
            //    cout <<task->name() << "->inputs (" <<to_std::string(i) <<") " <<task->inputs()[i].name() << endl;
            //}
            for (auto &node : inputs) {

                std::string tensorName = node.key();
                std::string prevTaskName = node.val();

                auto target = _taskMap[prevTaskName];
                auto it = std::find(prevs.begin(), prevs.end(), target);

                if (it == prevs.end()) {
                    prevs.emplace_back(target);
                }
                LOG_DBG("[INPUT][" + task->name() + "] Tensorname : " + tensorName + " / prevTaskName : " + prevTaskName);
            }
        }
        task->SetInferenceEngineTimer(&_inferenceTimer);
        if(task->is_PPU()){
            _isPPU = true;
        }
    }

#ifdef USE_ORT
    if (_option.useORT == true)
    {
        _lastOutputOrder = _modelData.deepx_graph.origin_output();
    }
    else
#endif
        _lastOutputOrder.clear();

    _numTails = 0;
    int64_t _tailOffset = 0;
    for (auto &task : _tasks)
    {
        if(task->nexts().empty())
        {
#ifdef USE_ORT
            if (_option.useORT == false)
#endif
            for (auto& output : task->outputs())
            {
                _lastOutputOrder.push_back(output.name());
            }
            task->setTailOffset(_tailOffset);
            _tailOffset += task->output_size();
            _numTails++;
        }
    }

    /*
    LOG << "Last Output Tensors: ";
    for (size_t i = 0; i < _lastOutputOrder.size(); ++i) {
        cout << _lastOutputOrder[i];
        if (i < _lastOutputOrder.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
    */
    LOG_DBG("_numTails : "+std::to_string(_numTails));
    //DXRT_ASSERT(_numTails==1, "Invalid Graph : check the number of tail task (The final inference output aggregation for multi-tail tasks is not yet supported.)");
    
    InferenceJob::InitInferenceJob(); // static
    _occupiedInferenceJobs = std::vector<bool>(ObjectsPool::INFERENCE_JOB_MAX_COUNT);
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
    std::call_once(_disposeOnceFlag, [this]() { this->disposeOnce(); });
    LOG_DXRT_DBG <<" DONE"<< endl;
}

TensorPtrs InferenceEngine::Run(void *inputPtr, void *userArg, void *outputPtr)
{
    if (_isDisposed)
    {
        throw InvalidOperationException("InferenceEngine already Disposed");
    }
    std::shared_ptr<InferenceJob> infJob = InferenceJob::Pick(); 

    infJob->SetInferenceJob(_tasks, _head, _lastOutputOrder);
    infJob->setInferenceEngineInterface(this);
    infJob->SetStoreResult(true);
    infJob->setCallBack([this](TensorPtrs &outputs, void *userArg, int jobId)->int{
        int retval = 0;
        if (_userCallback !=nullptr)
        {
            retval = _userCallback(outputs, userArg);
        }
        {
            std::lock_guard<std::mutex> lock(_occupiedInferenceJobsLock);
            // unoccupired inference job id
            this->_occupiedInferenceJobs[jobId] = false;
        }

        return retval;
    }); // inference engine callback 

    int jobId = infJob->startJob(inputPtr, userArg, outputPtr);
    {
        std::lock_guard<std::mutex> lock(_occupiedInferenceJobsLock);
        // occupired inference job id
        _occupiedInferenceJobs[jobId] = true;
    }
    return Wait(jobId);
}

int InferenceEngine::RunAsync(void *inputPtr, void *userArg, void *outputPtr)
{
    return runAsync(inputPtr, userArg, outputPtr, nullptr);
}

std::vector<TensorPtrs> InferenceEngine::Run(
    const std::vector<void*>& inputBuffers,
    const std::vector<void*>& outputBuffers,
    const std::vector<void*>& userArgs
)
{
    int batch_count = static_cast<int>(inputBuffers.size());

    if ( batch_count == 0 )
    {
        throw dxrt::InvalidArgumentException(EXCEPTION_MESSAGE("The number of elements in inputPtrs must be greater than 0."));
    }

    // check arguments size
    if ( userArgs.size() > 0 )
    {
        if ( batch_count != static_cast<int>(userArgs.size()) )
        {
            throw dxrt::InvalidArgumentException(EXCEPTION_MESSAGE("The number of elements in inputPtrs does not match the number of elements in userArgs."));
        }
    } 

    // check outputPtrs size
    // it must be same size as inputBuffers
    if ( batch_count != static_cast<int>(outputBuffers.size()) )
    {
        throw dxrt::InvalidArgumentException("The number of elements in inputPtrs does not match the number of elements in outputPtrs.");
    }
    
    
    // create outputs data
    std::vector<TensorPtrs> result(batch_count);

    try
    {
        

        // argruments
        std::vector<BatchArgument> batch_args(SUB_BATCH_MAX_COUNT);

        int start_index = 0;
        int sub_batch_loop = static_cast<int>(batch_count / SUB_BATCH_MAX_COUNT);
        int sub_batch_remain = static_cast<int>(batch_count % SUB_BATCH_MAX_COUNT);

        //std::cout << "sub-batch-loop=" << sub_batch_loop << " sub-batch-count=" << SUB_BATCH_MAX_COUNT 
        //        << " total=" << sub_batch_loop * SUB_BATCH_MAX_COUNT << std::endl;
        //std::cout << "sub-batch-remain=" << sub_batch_remain << std::endl;


        if ( sub_batch_loop > 0 )
        {
            for(int i = 0; i < sub_batch_loop; ++i)
            {
                runSubBatch(result, SUB_BATCH_MAX_COUNT, start_index, 
                    batch_args.data(), inputBuffers, outputBuffers, userArgs);

                start_index += SUB_BATCH_MAX_COUNT;
            } // for i
        }

        if ( sub_batch_remain > 0 )
        {
            runSubBatch(result, sub_batch_remain, start_index, 
                batch_args.data(), inputBuffers, outputBuffers, userArgs);
        }

        batch_args.clear();
        
    }
    catch (const dxrt::Exception& e)
    {
        LOG_DXRT_ERR(e.what());
    }
    catch(const std::exception& e)
    {
        LOG_DXRT_ERR(e.what());
    }

    return result; 
}

void InferenceEngine::runSubBatch(std::vector<TensorPtrs>& result, int batchCount, int startIndex, 
        void* batchArgs,
        const std::vector<void*>& inputBuffers,
        const std::vector<void*>& outputBuffers,
        const std::vector<void*>& userArgs
    )
{
    
    BatchArgument* batch_args_array = reinterpret_cast<BatchArgument*>(batchArgs);
    
    std::atomic<int> complete_count{0};
    std::mutex mtx_cv;  // mutex lock
    std::condition_variable cv_complete;  // complete condition variable

    auto batch_callback = [&complete_count, &cv_complete, &mtx_cv, &result, batchCount](TensorPtrs &outputs, void *userArg, int jobId) {

            //std::ignore = userArg; // reserved
            BatchArgument* batch_arg = reinterpret_cast<BatchArgument*>(userArg);
            if ( batch_arg == nullptr )
            {
                throw dxrt::InvalidOperationException(EXCEPTION_MESSAGE("Batch argument is nullptr"));
            }

            int batch_index = -1;

            try {
            
                // find batch_index by jobId
                batch_index = batch_arg->resultIndex;
                //std::cout << "callback batch-index=" << batch_index << std::endl;

                if ( batch_index >= 0 ) 
                {
                    result.at(batch_index) = outputs;
                }
                else 
                {
                    LOG_DXRT << "ERROR jobId=" << jobId << ", batch_index=" << batch_index << std::endl;
                }
            }
            catch(std::exception &e)
            {
                LOG_DXRT_ERR("[exception] allocated_output_buffer what=" << e.what());
            }
            
            complete_count++;
            LOG_DXRT_DBG << "runAsync complete-count=" << complete_count.load() << std::endl;
            if ( complete_count.load() == batchCount ) 
            {
                std::unique_lock<std::mutex> lock(mtx_cv);
                cv_complete.notify_one();
                LOG_DXRT_DBG << "runAsync completed" << std::endl;
            }
        };

    try 
    {
        // Run asynchronous operations for each batch
        for(int i = 0; i < batchCount; ++i)
        {
            BatchArgument* batchArg = reinterpret_cast<BatchArgument*>(&batch_args_array[i]);
            void* userArg = userArgs.size() > 0 ? userArgs.at(i) : nullptr;
            int current_index = startIndex + i;

            batchArg->userArg = userArg;
            batchArg->resultIndex = current_index;
            int job_id = runAsync(inputBuffers.at(current_index), batchArg, outputBuffers.at(current_index), batch_callback);

            //std::cout << "runAsync index=" << current_index << std::endl;
            //std::cout << "inputPtrs size=" << inputPtrs.size() << " OutputPtrs size=" << pOutputPtrs->size() << std::endl;

            //map_insert(job_id, i);
            LOG_DXRT_DBG << "Insert jobId=" << job_id << ", batch_index=" << i << std::endl;

        } // for i

        // wait for inference done
        std::unique_lock<std::mutex> lock(mtx_cv);
        cv_complete.wait(lock, [&complete_count, batchCount]() { return complete_count.load() == batchCount; });
        LOG_DXRT_DBG << "runAsync return" << std::endl;
    }
    catch(const dxrt::Exception& e)
    {
        LOG_DXRT_ERR(e.what());
    }
    catch (const std::exception& e)
    {
        LOG_DXRT_ERR(e.what());
    }
}


// private
int InferenceEngine::runAsync(void *inputPtr, void *userArg, void *outputPtr, 
    std::function<void(TensorPtrs &outputs, void *userArg, int jobId)> batchCallback)
{
    if (_isDisposed)
    {
        throw InvalidOperationException("InferenceEngine already Disposed");
    }
    // return InferenceJob instance from InferenceJob pool (reused)
    std::shared_ptr<InferenceJob> infJob = InferenceJob::Pick(); 

    infJob->SetInferenceJob(_tasks, _head, _lastOutputOrder);
    infJob->setInferenceEngineInterface(this);
    infJob->setCallBack([this, batchCallback](TensorPtrs &outputs, void *userArg, int jobId)->int{

            int retval = 0;
            if (this->_userCallback != nullptr)
            {
                if ( batchCallback != nullptr && userArg != nullptr )
                {
                    retval = this->_userCallback(outputs, 
                        (reinterpret_cast<BatchArgument*>(userArg))->userArg);
                }
                else 
                {
                    retval = this->_userCallback(outputs, userArg);
                }
            }

            //LOG_DXRT << "InferenceJob Callback-1 jobId=" << jobId << std::endl;
            if ( batchCallback != nullptr )
            {
                batchCallback(outputs, userArg, jobId);
            }
            {
                std::lock_guard<std::mutex> lock(_occupiedInferenceJobsLock);
                // unoccupired inference job id
                this->_occupiedInferenceJobs[jobId] = false;
            }
            
            return retval;
        }); // inference engine callback 
    
    if(_userCallback == nullptr)
    {
        infJob->SetStoreResult(true);
    }
    if(infJob->getId()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || infJob->getId()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
        cout<<"[PROC         ][Job_"<<infJob->getId()<<"] Start"<<endl;

    int jobId = infJob->startJob(inputPtr, userArg, outputPtr);

    // occupired inference job id
    {
        std::lock_guard<std::mutex> lock(_occupiedInferenceJobsLock);
        _occupiedInferenceJobs[jobId] = true;
    }

    return jobId;
}

void InferenceEngine::RegisterCallback(function<int(TensorPtrs &outputs, void *userArg)> f)
{
    LOG_DXRT_DBG << endl;
    _userCallback = f;
}

float InferenceEngine::RunBenchmark(int num, void *inputPtr)
{
#ifdef _WIN32
    return RunBenchMarkWindows(num, inputPtr);
#endif
    float sum = 0.;
    auto& profiler = dxrt::Profiler::GetInstance();
    std::vector<float> fps;
    int todo = max(30, num);

    std::atomic<int> done_count;
    auto callBack = [&done_count](TensorPtrs &outputs, void *userArg) -> int{
        std::ignore = outputs;
        std::ignore = userArg;
        done_count++;
        return 0;
    }; //callback used to count inference
    RegisterCallback(callBack);
    bool isStandalone = (dxrt::DeviceStatus::GetCurrentStatus(0).GetDeviceType() == DeviceType::STD_TYPE);

    while (todo > 0)
    {
        uint64_t infTime = 0;
        int infCnt = min(todo, ObjectsPool::REQUEST_MAX_COUNT);
        done_count = 0;
        profiler.Start("benchmark");
        auto start_clock = std::chrono::steady_clock::now();
        for (int i=0 ; i < infCnt ; i++)
        {
            if (isStandalone)
            {
                while ((i-done_count) >= DEVICE_NUM_BUF) continue;
                //usleep(20000);
            }
            RunAsync(inputPtr);
        }
        // profiler.End("req");
        while (done_count < infCnt)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
        auto end_clock = std::chrono::steady_clock::now();
        infTime = std::chrono::duration_cast<chrono::microseconds>(end_clock - start_clock).count();
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
    RegisterCallback(nullptr);
    return sum / fps.size();
}

#ifdef _WIN32
// in windows, verbose mode
float InferenceEngine::RunBenchMarkWindows(int num, void* inputPtr)
{
    float sum = 0.;
    auto& profiler = dxrt::Profiler::GetInstance();
    std::vector<float> fps;
    int todo = max(10, num);

    std::atomic<int> done_count, i_last, done_todo;
    auto callBack = [&done_count, &i_last,&done_todo](const TensorPtrs& outputs, void* userArg) -> int {
        std::ignore = outputs;
        std::ignore = userArg;
        // cout << "BenchMark(" << ((int)userArg) << ")" << endl;
        int userArgInt = reinterpret_cast<uint64_t>(userArg);

        done_count++;
        i_last = userArgInt;
        return 0;
        };  // callback used to count inference
    RegisterCallback(callBack);
    done_todo = 0;
    while (todo > 0)
    {
        uint64_t infTime = 0;
        int infCnt = min(todo, REQUEST_ID_MAX_VALUE);
        done_count = 0; i_last = 0;
        profiler.Start("benchmark");
        // profiler.Start("req");
        for (int i = 0; i < infCnt; i++)
        {
            RunAsync(inputPtr, reinterpret_cast<void*>(i));
        }
        // profiler.End("req");
        while (done_count < infCnt)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
        // while (done_count < infCnt || (i_last!=(infCnt-1)) )continue;
        profiler.End("benchmark");
        infTime = profiler.Get("benchmark");
        todo -= infCnt;
        done_todo += infCnt;
        fps.emplace_back(1000000.0 * infCnt / infTime);
    }
    profiler.Erase("benchmark");
    for (const auto& val : fps)
    {
        sum += val;
        // cout << "fps: " << val << endl;
    }
    RegisterCallback(nullptr);
    return sum / fps.size();
}
#endif  // _WIN32

TensorPtrs InferenceEngine::ValidateDevice(void *inputPtr, int deviceId)
{
    Device::_sNpuValidateOpt.store(true);

    auto npuTaskIter = std::find_if(_tasks.begin(), _tasks.end(), [](const std::shared_ptr<dxrt::Task>& task) {
        return task->processor() == Processor::NPU;
    });

    if (npuTaskIter == _tasks.end()) {
        throw InvalidModelException(EXCEPTION_MESSAGE("No NPU task found for device validation"));
    }

    auto npuTask = *npuTaskIter;

    std::vector<dxrt::DevicePtr>& devices = dxrt::CheckDevices();
    if (static_cast<size_t>(deviceId) >= devices.size()) {
        throw DeviceIOException(EXCEPTION_MESSAGE("invalid device id"));
    }
    auto req = Request::Create(npuTask.get(), inputPtr, nullptr, nullptr);
    req->setInferenceJob(nullptr);
    req->SetStatus(Request::Status::REQ_BUSY);
    req->model_type() = req->taskData()->_npuModel.front().type;
    //devices[deviceId]->InferenceRequest(req->getData());
    //req->Wait();
    //TensorPtrs ret = devices[deviceId]->Validate(req, true);
    TensorPtrs ret = devices[deviceId]->Validate(req, false);
    Device::_sNpuValidateOpt.store(false);
    return ret;
}

TensorPtrs InferenceEngine::Wait(int jobId)
{
    LOG_DXRT_DBG << jobId << endl;
    
    // if unoccupied inference job id, return empty value
    //if ( _occupiedInferenceJobs[jobId] == false ) {
    //    return {};
    //}

    std::shared_ptr<InferenceJob> infJob = InferenceJob::GetById(jobId);
    while (infJob->getStatus() == Request::Status::REQ_BUSY)
    {
        this_thread::sleep_for(chrono::microseconds(1));
//#ifdef __linux__
//        usleep(1);
//#elif _WIN32
//        this_thread::sleep_for(chrono::microseconds(1));
//#endif
        continue;
    }

    LOG_DXRT_DBG << jobId << " done" << endl;
    return infJob->getOutput();
}

Tensors InferenceEngine::GetInputs(void *ptr, uint64_t phyAddr)
{
    return _head->inputs(ptr, phyAddr);
}

std::vector<Tensors> InferenceEngine::GetInputs(int devId)
{
    std::vector<dxrt::DevicePtr>& devices = dxrt::CheckDevices();
    if(devices.empty()) return {};
    auto device = devices[devId];
    return device->inputs(_head->id());
}

Tensors InferenceEngine::GetOutputs(void *ptr, uint64_t phyAddr)
{
    Tensors filteredTensors(_lastOutputOrder.size(), Tensor("",{},DataType::FLOAT));

    for (auto &task : _tasks)
    {
        TaskData* tempTaskDataPtr = task->getData();
        Tensors tempTensors = tempTaskDataPtr->output_tensors();

        if (ptr == nullptr) {
            for (size_t i = 0; i < _lastOutputOrder.size(); i++) 
            {
                for (Tensor &tensor : tempTensors)
                {
                    if (tensor.name() == _lastOutputOrder[i]) {
                        filteredTensors[i] = tensor;
                    }
                }
            }
        } 
        else {
            int i = 0;
            for (auto &t : tempTensors)
            {
                t.data() = static_cast<void*>(static_cast<uint8_t*>(ptr) + tempTaskDataPtr->_outputOffsets[i] + task->getTailOffset());
                t.phy_addr() = phyAddr + tempTaskDataPtr->_outputOffsets[i];
                for (size_t j = 0; j < _lastOutputOrder.size(); j++) 
                {
                    if (t.name() == _lastOutputOrder[j])
                    {
                        filteredTensors[j] = t;
                    }
                }
                i++;
            }
        }
    }

    return filteredTensors;
}


uint64_t InferenceEngine::GetInputSize()
{
    return _head->input_size();
}

uint64_t InferenceEngine::GetOutputSize()
{
    uint64_t outputSize = 0;
    for (auto &name : _lastOutputOrder)
    {
        for (auto &task : _tasks)
        {
            if(task->is_PPU())
            {
                return task->output_size();
            }
            for(Tensor tensor: task->outputs())
            {
                if(tensor.name() == name)
                {
                    outputSize += tensor.size_in_bytes();
                }
            }
        }
    }
    return outputSize;
}

std::string InferenceEngine::GetModelName()
{
    return _name;
}

std::vector<std::string> InferenceEngine::GetTaskOrder()
{
    return _taskOrder;
}

int InferenceEngine::GetLatency()
{
    LOG_DXRT_DBG << endl;
    return _inferenceTimer.latency();
}

std::vector<int> InferenceEngine::GetLatencyVector()
{
    LOG_DXRT_DBG << endl;
    return _inferenceTimer.GetLatencyVector();
}

uint32_t InferenceEngine::GetNpuInferenceTime()
{
    LOG_DXRT_DBG << endl;
    return _inferenceTimer.inference_time();
}

std::vector<uint32_t> InferenceEngine::GetNpuInferenceTimeVector()
{
    LOG_DXRT_DBG << endl;
    return _inferenceTimer.GetNpuInferenceTimeVector();
}

double InferenceEngine::GetLatencyMean()
{
    return _inferenceTimer.GetLatencyMean();
}

double InferenceEngine::GetNpuInferenceTimeMean()
{
    return _inferenceTimer.GetNpuInferenceTimeMean();
}

double InferenceEngine::GetLatencyStdDev()
{
    return _inferenceTimer.GetLatencyStdDev();
}

double InferenceEngine::GetNpuInferenceTimeStdDev()
{
    return _inferenceTimer.GetNpuInferenceTimeStdDev();
}

int InferenceEngine::GetLatencyCnt()
{
    return _inferenceTimer.GetLatencyCnt();
}

int InferenceEngine::GetNpuInferenceTimeCnt()
{
    return _inferenceTimer.GetNpuInferenceTimeCnt();
}

std::vector<TensorPtrs> InferenceEngine::GetAllTaskOutputs()
{
    LOG_DXRT_DBG << "Collecting outputs from all tasks in order." << endl;
    std::vector<TensorPtrs> all_outputs;

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
            for (auto& tensor : task->getLastOutput())
            {
                task_outputs.emplace_back(make_shared<Tensor>(tensor));
            }
            // Add the outputs to the list
            all_outputs.push_back(task_outputs);
        }
        #ifdef USE_ORT
        else
        {
            LOG_DXRT << "Task " << task_name << " not found in task map." << endl;
        }
        #endif
    }
    return all_outputs;
}

int InferenceEngine::GetNumTailTasks()
{ 
    return _numTails; 
}

std::string InferenceEngine::GetCompileType()
{ 
    return _modelCompileType; 
}

bool InferenceEngine::IsPPU()
{ 
    return _isPPU; 
}

void InferenceEngine::disposeOnce()
{
    _isDisposed = true;
    LOG_DXRT_DBG << endl;
    std::unique_lock<std::mutex> lock(_occupiedInferenceJobsLock);

    for (size_t i = 0; i < _occupiedInferenceJobs.size(); ++i)
    {
        // wait for the job to finish
        if ( _occupiedInferenceJobs[i] ) {
            lock.unlock();
            Wait(static_cast<int>(i));
            lock.lock();
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
        task->ClearOutputBuffer();
    }
    _tasks.clear();
    _taskMap.clear();
    _head.reset();
    _tails.clear();
    _userCallback = nullptr;
    Request::Clear();
    LOG_DXRT_DBG <<" Done"<< endl;

}
void InferenceEngine::Dispose()
{
    std::call_once(_disposeOnceFlag, [this]() { this->disposeOnce(); });
}

std::vector<uint8_t> InferenceEngine::GetBitmatchMask(int index) {
    const std::vector<char>& maskBuffer = _modelData.deepx_binary.bitmatch_mask(index).buffer();
    std::vector<uint8_t> data(maskBuffer.begin(), maskBuffer.end());
    return data;
}

ostream& operator<<(ostream& os, const InferenceEngine& ie)
{
    os << " InferenceEngine " << ie._name << endl;
    for (const auto& task_name : ie._taskOrder)
    {
        auto it = ie._taskMap.find(task_name);
        if (it != ie._taskMap.end())
        {
            os << *(it->second) << endl;
        }

    }
    /*
    for(auto &pair:ie._taskMap)
    {
        os << *pair.second << endl;
    }
    */
    return os;

}

} // namespace dxrt
