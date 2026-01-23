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
#include "../include/logger.h"

#include <string>
#include <iostream>
#include <condition_variable>


int main(int argc, char* argv[])
{
    std::string model_path;
    int loop_count;
    bool verbose;
    
    auto& log = dxrt::Logger::GetInstance();

    cxxopts::Options options("run_async_model", "Run asynchronous model inference");
    options.add_options()
        ("m,model", "Path to model file (.dxnn)", cxxopts::value<std::string>(model_path))
        ("l,loops", "Number of inference loops", cxxopts::value<int>(loop_count)->default_value("1"))
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

    log.Info("Start async inference test for model: " + model_path);

    int callback_count_1 = 0;
    int callback_count_2 = 0;

    try 
    {

        std::mutex cv_mutex_1, cv_mutex_2;
        std::condition_variable cv_1, cv_2;
    
        // create inference engine instance with model
        dxrt::InferenceOption option_1;
        option_1.bufferCount = 6; // set buffer count to 6
        log.Info("Creating InferenceEngine_1 with buffer count: " + std::to_string(option_1.bufferCount));
        dxrt::InferenceEngine ie_1(model_path, option_1);

        dxrt::InferenceOption option_2;
        option_2.bufferCount = 3; // set buffer count to 3
        log.Info("Creating InferenceEngine_2 with buffer count: " + std::to_string(option_2.bufferCount));
        dxrt::InferenceEngine ie_2(model_path, option_2);

        // register call back function
        ie_1.RegisterCallback([&callback_count_1, &loop_count, &cv_mutex_1, &cv_1] 
            (dxrt::TensorPtrs &outputs, void *userArg) {

            std::ignore = outputs;
            std::ignore = userArg;

            std::unique_lock<std::mutex> lock(cv_mutex_1);
            callback_count_1++;
            if ( callback_count_1 == loop_count ) cv_1.notify_one();

            return 0;
        });

        ie_2.RegisterCallback([&callback_count_2, &loop_count, &cv_mutex_2, &cv_2] 
            (dxrt::TensorPtrs &outputs, void *userArg) {

            std::ignore = outputs;
            std::ignore = userArg;

            std::unique_lock<std::mutex> lock(cv_mutex_2);
            callback_count_2++;
            if ( callback_count_2 == loop_count ) cv_2.notify_one();

            return 0;
        });

        // create temporary input buffer for example
        std::vector<uint8_t> inputPtr_1(ie_1.GetInputSize(), 0);
        std::vector<uint8_t> inputPtr_2(ie_2.GetInputSize(), 0);

        auto start = std::chrono::high_resolution_clock::now();

        // inference loop
        for(int i = 0; i < loop_count; ++i)
        {

            // inference asynchronously, use all npu cores
            ie_1.RunAsync(inputPtr_1.data(), nullptr);
            ie_2.RunAsync(inputPtr_2.data(), nullptr);

            log.Debug("Inference request submitted with user_arg(" + std::to_string(i) + ")");
        }

        // wait until all callbacks have been processed
        std::unique_lock<std::mutex> lock_1(cv_mutex_1);
        cv_1.wait(lock_1, [&callback_count_1, &loop_count] { 
            return callback_count_1 == loop_count;
        });

        std::unique_lock<std::mutex> lock_2(cv_mutex_2);
        cv_2.wait(lock_2, [&callback_count_2, &loop_count] { 
            return callback_count_2 == loop_count;
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
            std::to_string(callback_count_1 + callback_count_2) + " / " + std::to_string(loop_count*2) + 
            ((callback_count_1 + callback_count_2 == loop_count*2) ? " (Success)" : " (Failure)"));
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
    
    return (callback_count_1 == loop_count && callback_count_2 == loop_count ? 0 : -1);
}