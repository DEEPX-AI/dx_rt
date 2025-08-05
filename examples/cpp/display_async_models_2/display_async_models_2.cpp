// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"
#include "../include/logger.h"
#include <string>
#include <iostream>
                                      
// InferenceEngine instances are created in main and passed to each thread.
// Reduces setup overhead and allows controlled resource management.
static int RunInferenceThread(dxrt::InferenceEngine *ie, int loopCount, int threadIndex)
{
    static auto& log = dxrt::Logger::GetInstance();
    std::mutex cv_mutex;
    std::condition_variable cv;
    int cb_index = 0;

    // register call back function
    ie->RegisterCallback([loopCount, &cb_index, &cv_mutex, &cv, threadIndex] (dxrt::TensorPtrs &outputs, void *userArg) {
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
    std::vector<uint8_t> input_buffer(ie->GetInputSize(), 0);


    // Submit 'loopCount' number of asynchronous inference requests
    for(int i = 0; i < loopCount; ++i)
    {
        // inference asynchronously, use all npu cores
        ie->RunAsync(input_buffer.data());

        log.Debug("[Thread " + std::to_string(threadIndex) + "] request submitted with loopcount(" + std::to_string(i + 1) + ")");

    }

    std::unique_lock<std::mutex> lock(cv_mutex);
    cv.wait(lock, [&cb_index, loopCount]{ return cb_index == loopCount; });
    log.Debug("[Thread " + std::to_string(threadIndex) + "] finished");

    return 0;
}

int main(int argc, char* argv[])
{
    const int DEFAULT_LOOP_COUNT = 1;
    const int THREAD_COUNT = 2;
    const bool ENABLE_ORT = false;
    
    std::string model_path;
    int loop_count = DEFAULT_LOOP_COUNT;
    bool logging = false;

    auto &log = dxrt::Logger::GetInstance();

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
        log.Info("[Usage] display_async_models_2 [dxnn-file-path] [loop-count] [--verbose|-v]");
        return -1;
    }

    if (logging) {
        log.SetLevel(dxrt::Logger::Level::DEBUG);
    }

    log.Info("Start run_display_async_models_2 for model: " + model_path);

    try 
    {   
        // create inference engine instance with model
        std::vector<std::shared_ptr<dxrt::InferenceEngine>> inference_engines;

        dxrt::InferenceOption io;
        io.useORT = ENABLE_ORT;

        for(int i = 0; i < THREAD_COUNT; ++i )
        {
            inference_engines.emplace_back(std::make_shared<dxrt::InferenceEngine>(model_path, io));
        }

   
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;

        for(int t = 0; t < THREAD_COUNT; ++t) { 
            threads.emplace_back(RunInferenceThread, inference_engines[t].get(), loop_count, t);
        }

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
        log.Error(std::string("std::exception: ") + e.what());
        return -1;
    }
    catch(...)
    {
        log.Error("Exception");
        return -1;
    }
    
    return 0;
}