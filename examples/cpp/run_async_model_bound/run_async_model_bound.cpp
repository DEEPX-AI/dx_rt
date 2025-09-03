/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/dxrt_api.h"
#include "../include/logger.h"

#include <string>
#include <iostream>

void RunInferenceThread(std::string modelPath, int loopCount, int threadIndex, dxrt::InferenceOption::BOUND_OPTION boundOption)
{
    auto &log = dxrt::Logger::GetInstance();

    // create inference engine instance with model
    dxrt::InferenceOption io;
    io.boundOption = boundOption;

    dxrt::InferenceEngine ie(modelPath, io);

    std::mutex cv_mutex;
    std::condition_variable cv;
    int cb_index = 0;


    // register call back function
    ie.RegisterCallback([loopCount, &cb_index, &cv_mutex, &cv, threadIndex] (dxrt::TensorPtrs &outputs, void *userArg) {
        std::ignore = outputs;
        std::ignore = userArg;
        
        static auto& log = dxrt::Logger::GetInstance();
        std::unique_lock<std::mutex> lock(cv_mutex);
        cb_index ++;

        log.Debug("[Thread " + std::to_string(threadIndex) + "] callback triggered for inference with callbackIndex(" + std::to_string(cb_index) + ")");
        
        if ( loopCount == cb_index )
        { 
            cv.notify_one();
        }

        return 0;
    });

    // create input buffer 
    std::vector<uint8_t> input_buffer(ie.GetInputSize(), 0);


    // Submit 'loopCount' number of asynchronous inference requests
    for(int i = 0; i < loopCount; ++i)
    {
        // inference asynchronously, use all npu cores
        ie.RunAsync(input_buffer.data());

        log.Debug("[Thread " + std::to_string(threadIndex) + "] request submitted with loopcount(" + std::to_string(i + 1) + ")");
    }

    std::unique_lock<std::mutex> lock(cv_mutex);
    cv.wait(lock, [&cb_index, loopCount]{ return cb_index == loopCount; });
    log.Debug("[Thread " + std::to_string(threadIndex) + "] finished");
}

int main(int argc, char* argv[])
{
    const int DEFAULT_LOOP_COUNT = 1;
    const int THREAD_COUNT = 2;  
    
    std::string model_path;
    int loop_count = DEFAULT_LOOP_COUNT;
    bool logging = false;

    auto& log = dxrt::Logger::GetInstance();

    if ( argc > 1 )
    {
        model_path = argv[1];

        if ( argc > 2 ) 
        {
            loop_count = std::stoi(argv[2]);

            if (argc > 3 )
            {
                std::string last_arg = argv[3];
                if (last_arg == "--verbose" || last_arg == "-v")
                {
                    logging = true;
                }
            }
        }
    }
    else
    {
        log.Info("[Usage] run_async_model_bound [dxnn-file-path] [loop-count] [--verbose|-v]");
        return -1;
    }

    if (logging) {
        log.SetLevel(dxrt::Logger::Level::DEBUG);
    }

    log.Info("Start async_model_bound test for model: " + model_path);

    try 
    {
        
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        
        // Create two threads with different bound options
        threads.emplace_back(RunInferenceThread, model_path, loop_count, 0, dxrt::InferenceOption::BOUND_OPTION::NPU_0);
        threads.emplace_back(RunInferenceThread, model_path, loop_count, 1, dxrt::InferenceOption::BOUND_OPTION::NPU_12);

        log.Info("Created " + std::to_string(THREAD_COUNT) + " threads with bound options:");
        log.Info("  Thread 0: NPU_0 (Bound " + std::to_string(dxrt::InferenceOption::BOUND_OPTION::NPU_0) + ")");
        log.Info("  Thread 1: NPU_12 (Bound " + std::to_string(dxrt::InferenceOption::BOUND_OPTION::NPU_12) + ")");
        log.Info("  Total different bound types: 2 (within 3-type limit)");

        // wait for all threads to complete
        for (auto& th: threads) {
            th.join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        log.Info("-----------------------------------");
        log.Info("Total Time: " + std::to_string(total_time) + " ms");
        log.Info("Average Latency: " + std::to_string(avg_latency) + " ms");
        log.Info("FPS: " + std::to_string(fps) + " frames/sec");
        log.Info("-----------------------------------");
    }
    catch (const dxrt::Exception& e)
    {
        log.Error(std::string(e.what()) + " error-code=" + std::to_string(e.code()));
        return -1;
    }
    catch (const std::exception& e)
    {
        log.Error(e.what());
        return -1;
    }
    catch(...)
    {
        log.Error("Exception");
        return -1;
    }
    
    return 0;
}