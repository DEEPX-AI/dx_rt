// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"

#include <string>
#include <iostream>
                                      

void RunInferenceThread(dxrt::InferenceEngine *ie, int loopCount, int threadIndex)
{

    std::mutex cv_mutex;
    std::condition_variable cv;
    int cb_index = 0;


    // register call back function
    ie->RegisterCallback([loopCount, &cb_index, &cv_mutex, &cv, threadIndex] (dxrt::TensorPtrs &outputs, void *userArg) {
        std::ignore = outputs;
        std::ignore = userArg;
       
        std::unique_lock<std::mutex> lock(cv_mutex);
        cb_index ++;

        std::cout << "[Thread " << threadIndex << "] callback triggered for inference with callbackIndex(" 
            << cb_index << ")" << std::endl;
        
        if ( loopCount == cb_index )
        { 
            cv.notify_one();
        }

        return 0;
    });

    // create input buffer 
    std::vector<uint8_t> inputBuffer(ie->GetInputSize(), 0);


    // Submit 'loopCount' number of asynchronous inference requests
    for(int i = 0; i < loopCount; ++i)
    {
        // inference asynchronously, use all npu cores
        ie->RunAsync(inputBuffer.data());

        std::cout << "[Thread " << threadIndex << "] request submitted with loopcount(" << i + 1 << ")" << std::endl;
    }

    std::unique_lock<std::mutex> lock(cv_mutex);
    cv.wait(lock, [&cb_index, loopCount]{ return cb_index == loopCount; });
    std::cout << "[Thread " << threadIndex << "] finished" << std::endl;
}

int main(int argc, char* argv[])
{
    const int DEFAULT_LOOP_COUNT = 1;
    const int THREAD_COUNT = 2;
    const bool ENABLE_ORT = false;
    
    std::string modelPath;
    int loop_count = DEFAULT_LOOP_COUNT;
    if ( argc > 1 )
    {
        modelPath = argv[1];

        if ( argc > 2 ) 
        {
            loop_count = std::stoi(argv[2]);
        }

    }
    else
    {
        std::cout << "[Usage] display_async_models_2 [dxnn-file-path] [loop-count]" << std::endl;
        return -1;
    }


    try 
    {   
        // create inference engine instance with model
        dxrt::InferenceOption op;
        op.useORT = ENABLE_ORT;

        std::vector<std::shared_ptr<dxrt::InferenceEngine>> inference_engines;
        for(int i = 0; i < THREAD_COUNT; ++i )
        {
            dxrt::InferenceOption op;
            op.useORT = ENABLE_ORT;
            inference_engines.emplace_back(std::make_shared<dxrt::InferenceEngine>(modelPath, op));
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

        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Total Time: " << total_time << " ms" << std::endl;
        std::cout << "Average Latency: " << avg_latency << " ms" << std::endl;
        std::cout << "FPS: " << fps << " frames/sec" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
    }
    catch (const dxrt::Exception& e)
    {
        std::cerr << e.what() << " error-code=" << e.code() << std::endl;
        return -1;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    catch(...)
    {
        std::cerr << "Exception" << std::endl;
        return -1;
    }
    
    return 0;
}