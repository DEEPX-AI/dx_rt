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


int main(int argc, char* argv[])
{
    std::string model_path;
    int loop_count;
    int batch_count;
    bool verbose;
    bool logging = false;

    auto &log = dxrt::Logger::GetInstance();

    cxxopts::Options options("run_batch_model", "Run batch model inference");
    options.add_options()
        ("m,model", "Path to model file (.dxnn)", cxxopts::value<std::string>(model_path))
        ("l,loops", "Number of inference loops", cxxopts::value<int>(loop_count)->default_value("1"))
        ("b,batch", "Batch count", cxxopts::value<int>(batch_count)->default_value("1"))
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
            logging = true;
        }
    }
    catch (const std::exception& e)
    {
        log.Error(std::string("Error parsing arguments: ") + e.what());
        std::cout << options.help() << std::endl;
        return -1;
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
        uint64_t outputSize = ie.GetOutputSize();
        bool isDynamic = ie.HasDynamicOutput();  // Explicit check for dynamic output
        
        if (isDynamic) {
            log.Info("Dynamic shape model detected - using engine-managed output buffers");
            log.Info("Model has dynamic output shapes that vary based on input");
            log.Info("Static output size calculation: " + std::to_string(outputSize) + " bytes (may be 0 for dynamic tensors)");
            
            // Show individual tensor size estimates
            auto tensorSizes = ie.GetOutputTensorSizes();
            log.Info("Output tensor count: " + std::to_string(tensorSizes.size()));
            for (size_t i = 0; i < tensorSizes.size(); ++i) {
                log.Info("Tensor " + std::to_string(i) + " estimated size: " + std::to_string(tensorSizes[i]) + " bytes");
            }
            
            // For dynamic models, pass nullptr to use engine-managed buffers
            // The engine will allocate appropriate buffers based on actual output shapes
        } else {
            // Static shape model - allocate user buffers
            if (outputSize == 0) {
                log.Info("Static model with zero output size - no buffer allocation needed");
            } else {
                for(auto& ptr : output_buffers)
                {
                    ptr = new uint8_t[outputSize];
                } // for i
                log.Info("Allocated " + std::to_string(outputSize) + " bytes per output buffer");
            }
        }

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
            
            // For dynamic shape models, show actual output shapes
            if (isDynamic && logging && i == 0) {  // Use explicit dynamic flag
                log.Debug("[Dynamic] Actual output shapes for first batch:");
                for (size_t batch_idx = 0; batch_idx < outputPtrs.size(); ++batch_idx) {
                    for (size_t tensor_idx = 0; tensor_idx < outputPtrs[batch_idx].size(); ++tensor_idx) {
                        const auto& tensor = outputPtrs[batch_idx][tensor_idx];
                        std::string shape_str = "[";
                        for (size_t dim_idx = 0; dim_idx < tensor->shape().size(); ++dim_idx) {
                            if (dim_idx > 0) shape_str += ", ";
                            shape_str += std::to_string(tensor->shape()[dim_idx]);
                        }
                        shape_str += "]";
                        log.Debug("[Dynamic] Batch " + std::to_string(batch_idx) + 
                                 ", Tensor " + std::to_string(tensor_idx) + 
                                 " (" + tensor->name() + "): " + shape_str + 
                                 " = " + std::to_string(tensor->size_in_bytes()) + " bytes");
                    }
                }
            }

            // postProcessing(outputs);
            (void)outputPtrs;
            log.Debug("[output-user] Reuse the user's output buffers");
        }

        auto end = std::chrono::high_resolution_clock::now();

        // Deallocated the user's output buffers (only for static shape models)
        if (!isDynamic && outputSize > 0) {  // Static model with actual buffers allocated
            for(auto& ptr : output_buffers)
            {
                if (ptr != nullptr) {
                    delete[] static_cast<uint8_t*>(ptr);
                }
            } // for i
            log.Debug("[output-user] Deallocated the user's output buffers");
        } else {
            log.Debug("[output-user] Dynamic shape model - buffers managed by engine");
        }

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