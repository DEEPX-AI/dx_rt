/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/dxrt_api.h"
#include "dxrt/extern/cxxopts.hpp"
#include "concurrent_queue.h"
#include "../include/logger.h"
#include <string>
#include <iostream>

// 1 InferenceEngine and 500 threads (asyncronous)

static std::atomic<int> gResultCount = {0};
static std::atomic<int> gTotalCount = {0};
static ConcurrentQueue<int> gResultQueue(1);
static std::mutex gCBMutex;

class UserData
{
    int _threadIndex = -1;
    int _loopIndex = -1;
    int _loopCount = -1;
public:
    void setThreadIndex(int index) {
        _threadIndex = index;
    }

    int getThreadIndex() {
        return _threadIndex;
    }

    void setLoopIndex(int index) {
        _loopIndex = index;
    }

    int getLoopIndex() {
        return _loopIndex;
    }

    void setLoopCount(int count) {
        _loopCount = count;
    }

    int getLoopCount() {
        return _loopCount;
    }
};

// submits inference jobs using shared InferenceEngine
static int inferenceThreadFunc(dxrt::InferenceEngine& ie, std::vector<uint8_t>& inputPtr, int threadIndex, int loopCount)
{
    static auto& log = dxrt::Logger::GetInstance();
    //inference Loop
    for(int i = 0; i < loopCount; ++i )
    {
        //user argument
        UserData *userData = new UserData();

        //thread index
        userData->setThreadIndex(threadIndex);

        //total loop count
        userData->setLoopCount(loopCount);

        //total index
        userData->setLoopIndex(i);

        try
        {
            // inference asynchronously, use all npu cores
            // if device-load >= max-load-value, this function will block  
            ie.RunAsync(inputPtr.data(), userData);
        }
        catch(const dxrt::Exception& e)
        {
            log.Error(std::string(e.what()) + " error-code=" + std::to_string(e.code()));
            std::exit(-1);
        }
        catch(const std::exception& e)
        {
            log.Error(std::string("std::exception: ") + e.what());
            std::exit(-1);
        }
        log.Debug("inferenceThreadFunc thread-index=" + std::to_string(threadIndex) + " loop-index=" + std::to_string(i));
    }// for i
    
    return 0;
}

// invoke this function asynchronously after the inference is completed
static int onInferenceCallbackFunc(dxrt::TensorPtrs &outputs, void *userArg)
{
    static auto& log = dxrt::Logger::GetInstance();
    // the outputs are guaranteed to be valid only within this callback function
    // processing this callback functions as quickly as possible is beneficial 
    // for improving inference performance

    // user data type casting
    UserData *user_data = reinterpret_cast<UserData*>(userArg);

    // thread index
    int thread_index = user_data->getThreadIndex();

    // loop index
    int loop_index = user_data->getLoopIndex();

    // post processing
    // transfer outputs to the target thread by thread_index
    // postProcessing(outputs, thread_index);
    (void)outputs;

    log.Debug("onInferenceCallbackFunc thread-index=" + std::to_string(thread_index) + " loop-index=" + std::to_string(loop_index));

    // result count
    {
        // Mutex locks should be properly adjusted 
        // to ensure that callback functions are thread-safe.
        std::lock_guard<std::mutex> lock(gCBMutex);

        gResultCount++;
        if ( gResultCount.load() == gTotalCount.load() ) gResultQueue.push(0);
    }

    // delete argument object
    delete user_data;

    return 0;
}

int main(int argc, char* argv[])
{
    const bool ENABLE_ORT = false;
    
    std::string model_path;
    int loop_count;
    int thread_count;
    bool verbose;

    auto &log = dxrt::Logger::GetInstance();

    cxxopts::Options options("display_async_thread", "Display async inference with multiple threads");
    options.add_options()
        ("m,model", "Path to model file (.dxnn)", cxxopts::value<std::string>(model_path))
        ("l,loops", "Number of inference loops", cxxopts::value<int>(loop_count)->default_value("1"))
        ("t,threads", "Number of threads", cxxopts::value<int>(thread_count)->default_value("3"))
        ("v,verbose", "Enable verbose/debug logging", cxxopts::value<bool>(verbose)->default_value("false"))
        ("h,help", "Print usage");

    try
    {
        auto result = options.parse(argc, argv);

        if (result.count("help") || !result.count("model"))
        {
            std::cout << options.help() << std::endl;
            return result.count("help") ? 0 : -1;
        }

        if (verbose) {
            log.SetLevel(dxrt::Logger::Level::LOGLEVEL_DEBUG);
        }
    }
    catch (const std::exception& e)
    {
        log.Error(std::string("Error parsing arguments: ") + e.what());
        std::cout << options.help() << std::endl;
        return -1;
    }

    log.Info("Start display_async_thread test for model: " + model_path);

    bool result = false;
    
    try     
    {
        
        // create inference engine instance with model
        dxrt::InferenceOption io;
        io.useORT = ENABLE_ORT;

        dxrt::InferenceEngine ie(model_path, io);

        // register call back function
        ie.RegisterCallback(onInferenceCallbackFunc);

        auto start = std::chrono::high_resolution_clock::now();

        // create input buffers for each thread (same size as model input)
        std::vector<uint8_t> input_buffer(ie.GetInputSize(), 0); 

        gTotalCount.store(loop_count * thread_count);

        // launch THREAD_COUNT threads to submit async inference jobs
        std::vector<std::thread> threads;
        threads.reserve(thread_count);
        
        for(int t = 0; t < thread_count; ++t) {
            threads.emplace_back(inferenceThreadFunc, std::ref(ie), std::ref(input_buffer), t, loop_count);
        }

        // wait for all threads to complete
        for (auto& th: threads) {
            th.join();
        }

        // wait until all callbacks have been processed
        gResultQueue.pop();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        
        result = (gResultCount.load() == (loop_count * thread_count));
        
        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        log.Info("-----------------------------------");
        log.Info("Total Time: " + std::to_string(total_time) + " ms");
        log.Info("Average Latency: " + std::to_string(avg_latency) + " ms");
        log.Info("FPS: " + std::to_string(fps) + " frames/sec");
        log.Info("Total count=(" + std::to_string(gResultCount.load()) + "/" + std::to_string(loop_count * thread_count) + ") " + 
                    (result ? "Success" : "Failure"));
        log.Info("-----------------------------------");

    }
    catch (const dxrt::Exception& e)
    {
        log.Error(std::string(e.what()) + " error-code=" + std::to_string(e.code()));
        return -1;
    }
    catch (const std::exception& e)
    {
        log.Error(std::string("std::exception: ") + e.what());
        return -1;
    }
    catch(...)
    {
        log.Error("Exception");
        return -1;
    }
    
    return result ? 0 : -1;
}