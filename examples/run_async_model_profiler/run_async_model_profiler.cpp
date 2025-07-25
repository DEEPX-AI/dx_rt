// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"

#include <string>
#include <iostream>
#include <condition_variable>


int main(int argc, char* argv[])
{
    const int DEFAULT_LOOP_COUNT = 1;
    
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
        std::cout << "[Usage] run_async_model_profiler [dxnn-file-path] [loop-count]" << std::endl;
        return -1;
    }

    int callback_count = 0;

    try 
    {

        // enable profiler
        dxrt::Configuration::GetInstance().SetEnable(dxrt::Configuration::ITEM::PROFILER, true);
        dxrt::Configuration::GetInstance().SetAttribute(dxrt::Configuration::ITEM::PROFILER, 
                                            dxrt::Configuration::ATTRIBUTE::PROFILER_SHOW_DATA, "ON");
        dxrt::Configuration::GetInstance().SetAttribute(dxrt::Configuration::ITEM::PROFILER, 
                                            dxrt::Configuration::ATTRIBUTE::PROFILER_SAVE_DATA, "ON");
        

        std::mutex cv_mutex;
        std::condition_variable cv;
    
        // create inference engine instance with model
        dxrt::InferenceEngine ie(modelPath);

        // register call back function
        ie.RegisterCallback([&callback_count, &loop_count, &cv_mutex, &cv] 
            (dxrt::TensorPtrs &outputs, void *userArg) {

            std::ignore = outputs;
            std::ignore = userArg;

            std::unique_lock<std::mutex> lock(cv_mutex);
            callback_count++;
            if ( callback_count == loop_count ) cv.notify_one();

            return 0;
        });

        // create temporary input buffer for example
        std::vector<uint8_t> inputPtr(ie.GetInputSize(), 0);

        auto start = std::chrono::high_resolution_clock::now();

        // inference loop
        for(int i = 0; i < loop_count; ++i)
        {
            // user argument
            std::pair<int, int> *userData = new std::pair<int, int>(i, loop_count);

            // inference asynchronously, use all npu cores
            ie.RunAsync(inputPtr.data(), userData);

            std::cout << "Inference request submitted with user_arg(" << i << ")" << std::endl;
            
        }

        // wait until all callbacks have been processed
        std::unique_lock<std::mutex> lock(cv_mutex);
        cv.wait(lock, [&callback_count, &loop_count] { 
            return callback_count == loop_count;
        });

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Total Time: " << total_time << " ms" << std::endl;
        std::cout << "Average Latency: " << avg_latency << " ms" << std::endl;
        std::cout << "FPS: " << fps << " frames/sec" << std::endl;
        std::cout << "Total callback-count / loop-count: " << callback_count << " / " << loop_count 
            << (callback_count == loop_count ? " (Success)" : " (Failure)") << std::endl;
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

    return (callback_count == loop_count ? 0 : -1);
}