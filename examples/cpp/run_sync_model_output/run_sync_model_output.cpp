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
#include "simple_circular_buffer_pool.h"

#include <string>
#include <iostream>


// Size of the output buffer pool
static const int BUFFER_POOL_SIZE = 200;

// Global output buffer pool and output counter
static std::shared_ptr<SimpleCircularBufferPool<uint8_t>> gOutputBufferPool;
static std::atomic<int> gOutputSuccessCount = {0};


int main(int argc, char* argv[])
{
    std::string model_path;
    int loop_count;
    bool verbose;

    auto &log = dxrt::Logger::GetInstance();

    cxxopts::Options options("run_sync_model_output", "Run synchronous model inference with output buffer management");
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

    log.Info("Start run_sync_model_output test for model: " + model_path);

    try
    {
        // create inference engine instance with model
        dxrt::InferenceEngine ie(model_path);

        // create output buffer pool
        gOutputBufferPool = std::make_shared<SimpleCircularBufferPool<uint8_t>>(BUFFER_POOL_SIZE, ie.GetOutputSize());

        // create temporary input buffer for example
        std::vector<uint8_t> inputPtr(ie.GetInputSize(), 0);

        auto start = std::chrono::high_resolution_clock::now();

        gOutputSuccessCount.store(0);

        // Run inference loop
        for(int i = 0; i < loop_count; ++i)
        {
            // Get user-provided output buffer from pool
            auto outputPtr = gOutputBufferPool->acquire_buffer();
            if (!outputPtr) {
                log.Error("Failed to retrieve output buffer from the pool.");
                continue;
            }

            // no need user argument
            // UserData *userData = getUserDataInstanceFromDataPool();

            // Run synchronous inference using all NPU cores
            // If the device is fully loaded, this call will block until resources are available
            // provide the output buffer pointer so the user can manage the output directly
            auto outputs = ie.Run(inputPtr.data(), nullptr, outputPtr);
            
            log.Debug("Inference outputs (" + std::to_string(i) + ")");

            //post processing
            //postProcessing(outputs)
            (void)outputs;

            // check user buffer pointer
            bool check_user_buffer = false;
            uint8_t* user_buffer_start = reinterpret_cast<uint8_t*>(outputPtr);
            uint8_t* user_buffer_end = user_buffer_start + ie.GetOutputSize();
            
            for(const auto& output : outputs)
            {
                uint8_t* tensor_ptr = reinterpret_cast<uint8_t*>(output->data());
                
                // Check if the tensor pointer is within the user buffer range
                // This is especially important for multi-tail models where different output tensors
                // may be located at different offsets within the user-provided buffer
                if (tensor_ptr >= user_buffer_start && tensor_ptr < user_buffer_end)
                {
                    check_user_buffer = true;
                    break;
                }
            }

            if ( !check_user_buffer ) 
            {
                std::cerr << "The output buffer pointer and the user-provided output pointer do not match" << std::endl;
                std::cerr << "User buffer range: " << static_cast<void*>(user_buffer_start) 
                         << " - " << static_cast<void*>(user_buffer_end) << std::endl;
                for (size_t i = 0; i < outputs.size(); ++i)
                {
                    std::cerr << "Output[" << i << "] pointer: " << outputs[i]->data() << std::endl;
                }
            }
            else 
            {
                gOutputSuccessCount++;
            }

        } // for i
        
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
                " output-count=" + std::to_string(gOutputSuccessCount.load()));
        if ( gOutputSuccessCount.load() == loop_count )
            log.Info("Success");
        else
            log.Info("Failure");
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
    
    return (gOutputSuccessCount == loop_count ? 0 : -1);
}

