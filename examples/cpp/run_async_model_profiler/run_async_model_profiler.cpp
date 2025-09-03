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
#include <condition_variable>


int main(int argc, char* argv[])
{
    const int DEFAULT_LOOP_COUNT = 1;
    
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
        log.Info("[Usage] run_async_model_profiler [dxnn-file-path] [loop-count] [--verbose|-v]");
        return -1;
    }

    if (logging) {
        log.SetLevel(dxrt::Logger::Level::DEBUG);
    }

    log.Info("Start async_model_profiler test for model: " + model_path);

    int callback_count = 0;

    try 
    {

        // enable profiler
        dxrt::Configuration::GetInstance().SetEnable(dxrt::Configuration::ITEM::PROFILER, true);

        // print profiling infomation
        dxrt::Configuration::GetInstance().SetAttribute(dxrt::Configuration::ITEM::PROFILER, 
                                            dxrt::Configuration::ATTRIBUTE::PROFILER_SHOW_DATA, "ON");

        // save profiling infomation to file
        dxrt::Configuration::GetInstance().SetAttribute(dxrt::Configuration::ITEM::PROFILER, 
                                            dxrt::Configuration::ATTRIBUTE::PROFILER_SAVE_DATA, "ON");
        

        std::mutex cv_mutex;
        std::condition_variable cv;
    
        // create inference engine instance with model
        dxrt::InferenceEngine ie(model_path);

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

            log.Debug("Inference request submitted with user_arg(" + std::to_string(i) + ")");
            
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

        log.Info("-----------------------------------");
        log.Info("Total Time: " + std::to_string(total_time) + " ms");
        log.Info("Average Latency: " + std::to_string(avg_latency) + " ms");
        log.Info("FPS: " + std::to_string(fps) + " frame/sec");
        log.Info("Total callback-count / loop-count: " + 
            std::to_string(callback_count) + " / " + std::to_string(loop_count) + 
            (callback_count == loop_count ? " (Success)" : " (Failure)"));
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

    return (callback_count == loop_count ? 0 : -1);
}