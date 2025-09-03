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
        log.Info("[Usage] run_sync_model [dxnn-file-path] [loop-count] [--verbose|-v]");
        return -1;
    }

    if (logging) {
        log.SetLevel(dxrt::Logger::Level::DEBUG);
    }

    log.Info("Start run_sync_model test for model: " + model_path);

    try
    {

        // create inference engine instance with model
        dxrt::InferenceEngine ie(model_path);

        // create temporary input buffer for example
        std::vector<uint8_t> inputPtr(ie.GetInputSize(), 0);

        auto start = std::chrono::high_resolution_clock::now();

        // inference loop
        for(int i = 0; i < loop_count; ++i)
        {
            
            // inference synchronously, use one npu core
            auto outputs = ie.Run(inputPtr.data());

            log.Debug("Inference outputs (" + std::to_string(i) + ")");
            // poset processing
            // postProcessing(outputs);
            (void)outputs;

        }

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