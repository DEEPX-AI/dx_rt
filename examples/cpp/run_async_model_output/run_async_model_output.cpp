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
#include "simple_circular_buffer_pool.h"

#include <string>
#include <iostream>


// concurrent queue is a thread-safe queue data structure 
// designed to be used in a multi-threaded environment
static ConcurrentQueue<std::pair<int, uint8_t*>> gJobIdQueue(200);

static const int BUFFER_POOL_SIZE = 200;
static std::shared_ptr<SimpleCircularBufferPool<uint8_t>> gOutputBufferPool;
static std::atomic<int> gOutputSuccessCount = {0};

// user thread to wait for the completion of inference 
static int inferenceThreadFunc(dxrt::InferenceEngine& ie, int loopCount)
{   
    static auto& log = dxrt::Logger::GetInstance();
    int count = 0;

    while(true)
    {
        // pop item from queue 
        //int jobId = gJobIdQueue.pop();
        auto jobInfo = gJobIdQueue.pop();

        try
        {
            // waiting for the inference to complete by jobId
            // ownership of the outputs is transferred to the user 
            auto outputs = ie.Wait(jobInfo.first);

            // post processing
            // postProcessing(outputs);
            (void)outputs;

            // check user buffer pointer
            bool check_user_buffer = false;
            uint8_t* user_buffer_start = reinterpret_cast<uint8_t*>(jobInfo.second);
            uint8_t* user_buffer_end = user_buffer_start + ie.GetOutputSize();
            
            for(const auto& output : outputs)
            {
                uint8_t* tensor_ptr = reinterpret_cast<uint8_t*>(output->data());
                
                // Check if the tensor pointer is within the user buffer range
                if (tensor_ptr >= user_buffer_start && tensor_ptr < user_buffer_end)
                {
                    check_user_buffer = true;
                    break;
                }
            }

            if ( !check_user_buffer ) 
            {
                log.Error("The output buffer pointer and the user-provided output pointer do not match");
                log.Error("User buffer range: " + std::to_string(reinterpret_cast<uintptr_t>(user_buffer_start)) +
                          " - " + std::to_string(reinterpret_cast<uintptr_t>(user_buffer_end)));
                for(size_t i = 0; i < outputs.size(); ++i)
                {
                    log.Error("Output[" + std::to_string(i) + "] pointer: " +
                              std::to_string(reinterpret_cast<uintptr_t>(outputs[i]->data())));
                }
            }
            else 
            {
                gOutputSuccessCount++;
            }

            // something to do
        }
        catch (const dxrt::Exception& e)
        {
            log.Error(std::string("DXRT Exception: ") + e.what() + " error-code=" + std::to_string(e.code()));
            return -1;
        }

        log.Debug("Inference outputs corresponding to jobId(" + std::to_string(jobInfo.first) + ")");

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
        log.Info("[Usage] run_async_model_output [dxnn-file-path] [loop-count] [--verbose|-v]");
        return -1;
    }

    if (logging) {
        log.SetLevel(dxrt::Logger::Level::DEBUG);
    }

    log.Info("Start async_model_output test for model: " + model_path);

    try
    {
        // create inference engine instance with model
        dxrt::InferenceEngine ie(model_path);

        // create output buffer pool
        gOutputBufferPool = std::make_shared<SimpleCircularBufferPool<uint8_t>>(BUFFER_POOL_SIZE, ie.GetOutputSize());

        // create thread
        auto t1 = std::thread(inferenceThreadFunc, std::ref(ie), loop_count);

        // create temporary input buffer for example
        std::vector<uint8_t> inputPtr(ie.GetInputSize(), 0);

        auto start = std::chrono::high_resolution_clock::now();

        // inference loop
        gOutputSuccessCount.store(0);
        for(int i = 0; i < loop_count; ++i)
        {
            // no need user argument
            // UserData *userData = getUserDataInstanceFromDataPool();

            // inference asynchronously, use all npu cores
            // if device-load >= max-load-value, this function will block
            // provide the output buffer pointer so the user can manage the output directly
            auto pointer = gOutputBufferPool->acquire_buffer();
            auto jobId = ie.RunAsync(inputPtr.data(), nullptr, pointer);

            // push jobId in global queue variable
            gJobIdQueue.push(std::pair<int, uint8_t*>(jobId, pointer));

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
        log.Info("FPS: " + std::to_string(fps) + " frames/sec");
        log.Info("loop-count=" + std::to_string(loop_count) +
                 " output-success-count=" + std::to_string(gOutputSuccessCount.load()));
                 
        if ( gOutputSuccessCount.load() == loop_count ) log.Info("Success");
        else log.Info("Failure");
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
    
    return (gOutputSuccessCount == loop_count ? 0 : -1);
}

