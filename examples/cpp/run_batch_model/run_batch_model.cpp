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
    const int DEFAULT_BATCH_COUNT = 1;
    
    std::string model_path;
    int loop_count = DEFAULT_LOOP_COUNT;
    int batch_count = DEFAULT_BATCH_COUNT;
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
                batch_count = std::stoi(argv[3]);

                if (argc > 4)
                {
                    std::string last_arg = argv[4];
                    if (last_arg == "--verbose" || last_arg == "-v")
                    {
                        logging = true;
                    }
                }
            }
        }
    }
    else
    {
        log.Info("[Usage] run_batch_model [dxnn-file-path] [loop-count] [batch-count] [--verbose|-v]");
        return -1;
    }

    if (logging) {
        log.SetLevel(dxrt::Logger::Level::DEBUG);
    }

    log.Info("Start run_batch_model test for model: " + model_path);

    try
    {

        // create inference engine instance with model
        dxrt::InferenceEngine ie(model_path);

        // create temporary input buffer for example
        std::vector<uint8_t> inputBuffer(ie.GetInputSize(), 0);

        // input buffer vector
        std::vector<void*> inputBuffers;
        for(int i = 0; i < batch_count; ++i)
        {
            // assigns the same buffer pointer in this example
            inputBuffers.emplace_back(inputBuffer.data());
        }
        
        log.Debug("[output-internal] Use user's output buffers");
        

        // output buffer vector
        std::vector<void*> output_buffers(batch_count, 0);

        // create user output buffers
        for(auto& ptr : output_buffers)
        {
            ptr = new uint8_t[ie.GetOutputSize()];
        } // for i

        log.Debug("[output-user] Create output buffers by user");
        log.Debug("[output-user] These buffers should be deallocated by user");

        auto start = std::chrono::high_resolution_clock::now();

        // batch inference loop
        for(int i = 0; i < loop_count; ++i)
        {
            // inference asynchronously, use all npu core
            auto outputPtrs = ie.Run(inputBuffers, output_buffers);

            log.Debug("[output-user] Inference outputs (" + std::to_string(i) + ")");
            log.Debug("[output-user] Inference outputs size=" + std::to_string(outputPtrs.size()));
            log.Debug("[output-user] Inference outputs first-tensor-name=" + outputPtrs.front().front()->name());

            // postProcessing(outputs);
            (void)outputPtrs;
            log.Debug("[output-user] Reuse the user's output buffers");
        }

        auto end = std::chrono::high_resolution_clock::now();

        // Deallocated the user's output buffers
        for(auto& ptr : output_buffers)
        {
            delete[] static_cast<uint8_t*>(ptr);
        } // for i
        log.Debug("[output-user] Deallocated the user's output buffers");

        std::chrono::duration<double, std::milli> duration = end - start;

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count*batch_count);
        double fps = 1000.0 / avg_latency;

        log.Info("---------------------------------------------");
        log.Info("Use user's output buffers");
        log.Info("Total Count: loop=" + std::to_string(loop_count) +
                    ", batch=" + std::to_string(batch_count) +
                    ", total=" + std::to_string(loop_count * batch_count));
        log.Info("Total Time: " + std::to_string(total_time) + " ms");
        log.Info("Average Latency: " + std::to_string(avg_latency) + " ms");
        log.Info("FPS: " + std::to_string(fps) + " frames/sec");
        log.Info("Success");
        log.Info("---------------------------------------------");

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