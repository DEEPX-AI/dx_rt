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
#include "concurrent_queue.h"

#include <string>
#include <iostream>


// concurrent queue is a thread-safe queue data structure 
// designed to be used in a multi-threaded environment
static ConcurrentQueue<int> gJobIdQueue(32);

// user thread to wait for the completion of inference 
static int inferenceThreadFunc(dxrt::InferenceEngine& ie, int loopCount)
{   
    static auto& log = dxrt::Logger::GetInstance();
    int count = 0;

    while(true)
    {
        // pop item from queue 
        int jobId = gJobIdQueue.pop();

        try
        {
            // waiting for the inference to complete by jobId
            // ownership of the outputs is transferred to the user 
            auto outputs = ie.Wait(jobId);

            // post processing
            // postProcessing(outputs);
            (void)outputs;

            // something to do
        }
        catch (const dxrt::Exception& e)
        {
            log.Error(std::string(e.what()) + " error-code=" + std::to_string(e.code()));
            return -1;
        }

        log.Debug("Inference outputs corresponding to jobId(" + std::to_string(jobId) + ")");

        count++;
        if ( count >= loopCount ) break;

    } // while

    return 0;
}

int main(int argc, char* argv[])
{
    const int DEFAULT_LOOP_COUNT = 1;

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
        log.Info("[Usage] run_async_model_wait [dxnn-file-path] [loop-count] [--verbose|-v]");
        return -1;
    }

    if (logging) {
        log.SetLevel(dxrt::Logger::Level::DEBUG);
    }

    log.Info("Start async_model_wait test for model: " + model_path);

    try
    {
        // create inference engine instance with model
        dxrt::InferenceEngine ie(model_path);

        // do not register call back function
        // inferenceEngine.RegisterCallback(onInferenceCallbackFunc);

        // create thread
        auto t1 = std::thread(inferenceThreadFunc, std::ref(ie), loop_count);

        // create temporary input buffer for example
        std::vector<uint8_t> inputPtr(ie.GetInputSize(), 0);

        auto start = std::chrono::high_resolution_clock::now();

        // inference loop
        for(int i = 0; i < loop_count; ++i)
        {
            // no need user argument
            // UserData *userData = getUserDataInstanceFromDataPool();

            // inference asynchronously, use all npu cores
            // if device-load >= max-load-value, this function will block
            auto jobId = ie.RunAsync(inputPtr.data());

            // push jobId in global queue variable
            gJobIdQueue.push(jobId);
            
            log.Debug("Inference request submitted with jobId(" + std::to_string(jobId) + ")");
        } // for i

        t1.join();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        log.Info("-----------------------------------");
        log.Info("Total Time: " + std::to_string(total_time) + " ms");
        log.Info("Average Latency: " + std::to_string(avg_latency) + " ms");
        log.Info("FPS: " + std::to_string(fps) + " frame/sec");
        log.Info("Success");
        log.Info("-----------------------------------");
    }
    catch (const dxrt::Exception& e)
    {
        log.Error(std::string("dxrt::Exception: ") + e.what());
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

