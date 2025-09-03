/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/dxrt_api.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <memory>
#include <atomic>
#include <thread>
#include <iomanip>
#include <sstream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <cmath>

// Helper function to create dummy input data
std::vector<uint8_t> createDummyInput(size_t size, uint8_t offset = 0) {
    std::vector<uint8_t> data(size);
    for (size_t i = 0; i < size; ++i) {
        data[i] = static_cast<uint8_t>((i + offset) % 256);
    }
    return data;
}

// Print model information
void printModelInfo(dxrt::InferenceEngine& ie) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "                    MODEL INFORMATION" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << "Multi-input model: " << (ie.IsMultiInputModel() ? "Yes" : "No") << std::endl;
    
    if (ie.IsMultiInputModel()) {
        std::cout << "Input tensor count: " << ie.GetInputTensorCount() << std::endl;
        std::cout << "Total input size: " << ie.GetInputSize() << " bytes" << std::endl;
        std::cout << "Total output size: " << ie.GetOutputSize() << " bytes" << std::endl;
        
        // Input tensor information
        auto inputNames = ie.GetInputTensorNames();
        auto inputSizes = ie.GetInputTensorSizes();
        auto mapping = ie.GetInputTensorToTaskMapping();
        
        std::cout << "\nInput tensor details:" << std::endl;
        for (size_t i = 0; i < inputNames.size(); ++i) {
            auto it = mapping.find(inputNames[i]);
            std::string taskName = (it != mapping.end()) ? it->second : "Unknown";
            std::cout << "  " << inputNames[i] << ": " << inputSizes[i] 
                      << " bytes -> Task: " << taskName << std::endl;
        }

        // Output tensor information
        auto outputNames = ie.GetOutputTensorNames();
        auto outputSizes = ie.GetOutputTensorSizes();
        
        std::cout << "\nOutput tensor details:" << std::endl;
        for (size_t i = 0; i < outputNames.size(); ++i) {
            std::cout << "  " << outputNames[i] << ": " << outputSizes[i] << " bytes" << std::endl;
        }
    }
    
    std::cout << std::string(60, '=') << std::endl;
}

// Validate inference outputs
bool validateOutputs(const dxrt::TensorPtrs& outputs, size_t expectedCount, const std::string& testName) {
    std::cout << std::endl;
    
    // Handle empty outputs case
    if (outputs.empty()) {
        if (expectedCount == 0) {
            std::cout << "[RESULT] " << testName << ": Empty outputs (expected)" << std::endl;
            return true;
        } else {
            std::cout << "[ERROR] " << testName << ": Empty outputs but expected " << expectedCount << std::endl;
            return false;
        }
    }
    
    // Check output count
    if (outputs.size() != expectedCount) {
        std::cout << "[ERROR] " << testName << ": Expected " << expectedCount 
                  << " outputs, got " << outputs.size() << std::endl;
        return false;
    }
    
    // Validate each output tensor
    for (size_t i = 0; i < outputs.size(); ++i) {
        if (!outputs[i]) {
            std::cout << "[ERROR] " << testName << ": Output " << i << " is null" << std::endl;
            return false;
        }
        
        // Check tensor size
        if (outputs[i]->size_in_bytes() == 0) {
            std::cout << "[ERROR] " << testName << ": Output " << i << " is empty (size=0)" << std::endl;
            return false;
        }
        
        // Check tensor shape
        if (outputs[i]->shape().empty()) {
            std::cout << "[ERROR] " << testName << ": Output " << i << " has invalid shape" << std::endl;
            return false;
        }
        
        // Check data pointer
        if (!outputs[i]->data()) {
            std::cout << "[ERROR] " << testName << ": Output " << i << " has null data pointer" << std::endl;
            return false;
        }
    }
    
    std::cout << "[RESULT] " << testName << ": All outputs valid (" << outputs.size() << " tensors)" << std::endl;
    return true;
}

// Example 1: Multi-Input Single Inference (Dictionary Format) - No Output Buffer
bool example1_singleInferenceDictionaryNoBuffer(dxrt::InferenceEngine& ie) {
    std::cout << "\n1. Dictionary Format Single Inference (No Output Buffer)" << std::endl;
    std::cout << "   - Input: Dictionary mapping tensor names to data" << std::endl;
    std::cout << "   - API: ie.RunMultiInput(input_dict) - auto-allocated output" << std::endl;
    
    if (!ie.IsMultiInputModel()) {
        std::cout << "   [WARNING]  Skipped: Not a multi-input model" << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return true;
    }
    
    try {
        // Get input tensor information
        auto inputNames = ie.GetInputTensorNames();
        auto inputSizes = ie.GetInputTensorSizes();
        
        // Create input data for each tensor
        std::vector<std::vector<uint8_t>> inputDataList;
        std::map<std::string, void*> inputTensors;
        
        for (size_t i = 0; i < inputNames.size(); ++i) {
            inputDataList.emplace_back(createDummyInput(inputSizes[i]));
            inputTensors[inputNames[i]] = inputDataList[i].data();
        }
        
        // Run inference without output buffers (auto-allocated)
        auto start = std::chrono::high_resolution_clock::now();
        auto outputs = ie.RunMultiInput(inputTensors);
        auto end = std::chrono::high_resolution_clock::now();
        
        // Validate and report
        auto outputSizes = ie.GetOutputTensorSizes();
        size_t expectedOutputCount = outputSizes.size();
        bool success = validateOutputs(outputs, expectedOutputCount, "Dictionary Format (No Buffer)");
        if (success) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "         Inference time: " << std::fixed << std::setprecision(2) 
                      << (duration.count() / 1000.0) << " ms" << std::endl;
        }
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return success;
        
    } catch (const std::exception& e) {
        std::cout << "   [ERROR] Error: " << e.what() << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return false;
    }
}

// Example 2: Multi-Input Single Inference (Vector Format) - No Output Buffer
bool example2_singleInferenceVectorNoBuffer(dxrt::InferenceEngine& ie) {
    std::cout << "\n2. Vector Format Single Inference (No Output Buffer)" << std::endl;
    std::cout << "   - Input: List of arrays in tensor name order" << std::endl;
    std::cout << "   - API: ie.RunMultiInput(input_list) - auto-allocated output" << std::endl;
    
    if (!ie.IsMultiInputModel()) {
        std::cout << "   [WARNING]  Skipped: Not a multi-input model" << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return true;
    }
    
    try {
        // Get input tensor information
        auto inputSizes = ie.GetInputTensorSizes();
        
        // Create input data in the order of GetInputTensorNames()
        std::vector<std::vector<uint8_t>> inputDataList;
        std::vector<void*> inputPtrs;
        
        for (size_t i = 0; i < inputSizes.size(); ++i) {
            inputDataList.emplace_back(createDummyInput(inputSizes[i]));
            inputPtrs.push_back(inputDataList[i].data());
        }
        
        // Run inference without output buffers (auto-allocated)
        auto start = std::chrono::high_resolution_clock::now();
        auto outputs = ie.RunMultiInput(inputPtrs);
        auto end = std::chrono::high_resolution_clock::now();
        
        // Validate and report
        auto outputSizes = ie.GetOutputTensorSizes();
        size_t expectedOutputCount = outputSizes.size();
        bool success = validateOutputs(outputs, expectedOutputCount, "Vector Format (No Buffer)");
        if (success) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "         Inference time: " << std::fixed << std::setprecision(2) 
                      << (duration.count() / 1000.0) << " ms" << std::endl;
        }
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return success;
        
    } catch (const std::exception& e) {
        std::cout << "   [ERROR] Error: " << e.what() << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return false;
    }
}

// Example 3: Auto-Split Single Buffer Inference - No Output Buffer
bool example3_autoSplitInferenceNoBuffer(dxrt::InferenceEngine& ie) {
    std::cout << "\n3. Auto-Split Single Buffer Inference (No Output Buffer)" << std::endl;
    std::cout << "   - Input: Single concatenated buffer (auto-split)" << std::endl;
    std::cout << "   - API: ie.Run(concatenated_buffer) - auto-allocated output" << std::endl;
    
    if (!ie.IsMultiInputModel()) {
        std::cout << "   [WARNING]  Skipped: Not a multi-input model" << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return true;
    }
    
    try {
        // Create concatenated input buffer
        size_t totalInputSize = ie.GetInputSize();
        auto concatenatedInput = createDummyInput(totalInputSize);
        
        // Run inference without output buffers (auto-allocated)
        auto start = std::chrono::high_resolution_clock::now();
        auto outputs = ie.Run(concatenatedInput.data());
        auto end = std::chrono::high_resolution_clock::now();
        
        // Validate and report
        auto outputSizes = ie.GetOutputTensorSizes();
        size_t expectedOutputCount = outputSizes.size();
        bool success = validateOutputs(outputs, expectedOutputCount, "Auto-Split (No Buffer)");
        if (success) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "         Inference time: " << std::fixed << std::setprecision(2) 
                      << (duration.count() / 1000.0) << " ms" << std::endl;
        }
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return success;
        
    } catch (const std::exception& e) {
        std::cout << "   [ERROR] Error: " << e.what() << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return false;
    }
}

// Example 4: Multi-Input Single Inference (Dictionary Format) - With Output Buffer
bool example4_singleInferenceDictionary(dxrt::InferenceEngine& ie) {
    std::cout << "\n4. Dictionary Format Single Inference (With Output Buffer)" << std::endl;
    std::cout << "   - Input: Dictionary mapping tensor names to data" << std::endl;
    std::cout << "   - API: ie.RunMultiInput(input_dict, output_buffer)" << std::endl;
    
    if (!ie.IsMultiInputModel()) {
        std::cout << "   [WARNING]  Skipped: Not a multi-input model" << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return true;
    }
    
    try {
        // Get input tensor information
        auto inputNames = ie.GetInputTensorNames();
        auto inputSizes = ie.GetInputTensorSizes();
        
        // Create input data for each tensor
        std::vector<std::vector<uint8_t>> inputDataList;
        std::map<std::string, void*> inputTensors;
        
        for (size_t i = 0; i < inputNames.size(); ++i) {
            inputDataList.emplace_back(createDummyInput(inputSizes[i]));
            inputTensors[inputNames[i]] = inputDataList[i].data();
        }
        
        // Create output buffer
        std::vector<uint8_t> outputBuffer(ie.GetOutputSize());
        
        // Run inference
        auto start = std::chrono::high_resolution_clock::now();
        auto outputs = ie.RunMultiInput(inputTensors, nullptr, outputBuffer.data());
        auto end = std::chrono::high_resolution_clock::now();
        
        // Validate and report
        auto outputSizes = ie.GetOutputTensorSizes();
        size_t expectedOutputCount = outputSizes.size();
        bool success = validateOutputs(outputs, expectedOutputCount, "Dictionary Format");
        if (success) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "         Inference time: " << std::fixed << std::setprecision(2) 
                      << (duration.count() / 1000.0) << " ms" << std::endl;
        }
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return success;
        
    } catch (const std::exception& e) {
        std::cout << "   [ERROR] Error: " << e.what() << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return false;
    }
}

// Example 5: Multi-Input Single Inference (Vector Format) - With Output Buffer
bool example5_singleInferenceVector(dxrt::InferenceEngine& ie) {
    std::cout << "\n5. Vector Format Single Inference (With Output Buffer)" << std::endl;
    std::cout << "   - Input: List of arrays in tensor name order" << std::endl;
    std::cout << "   - API: ie.RunMultiInput(input_list, output_buffer)" << std::endl;
    
    if (!ie.IsMultiInputModel()) {
        std::cout << "   [WARNING]  Skipped: Not a multi-input model" << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return true;
    }
    
    try {
        // Get input tensor information
        auto inputSizes = ie.GetInputTensorSizes();
        
        // Create input data in the order of GetInputTensorNames()
        std::vector<std::vector<uint8_t>> inputDataList;
        std::vector<void*> inputPtrs;
        
        for (size_t i = 0; i < inputSizes.size(); ++i) {
            inputDataList.emplace_back(createDummyInput(inputSizes[i]));
            inputPtrs.push_back(inputDataList[i].data());
        }
        
        // Create output buffer
        std::vector<uint8_t> outputBuffer(ie.GetOutputSize());
        
        // Run inference
        auto start = std::chrono::high_resolution_clock::now();
        auto outputs = ie.RunMultiInput(inputPtrs, nullptr, outputBuffer.data());
        auto end = std::chrono::high_resolution_clock::now();
        
        // Validate and report
        auto outputSizes = ie.GetOutputTensorSizes();
        size_t expectedOutputCount = outputSizes.size();
        bool success = validateOutputs(outputs, expectedOutputCount, "Vector Format");
        if (success) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "         Inference time: " << std::fixed << std::setprecision(2) 
                      << (duration.count() / 1000.0) << " ms" << std::endl;
        }
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return success;
        
    } catch (const std::exception& e) {
        std::cout << "   [ERROR] Error: " << e.what() << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return false;
    }
}

// Example 6: Auto-Split Single Buffer Inference - With Output Buffer
bool example6_autoSplitInference(dxrt::InferenceEngine& ie) {
    std::cout << "\n6. Auto-Split Single Buffer Inference (With Output Buffer)" << std::endl;
    std::cout << "   - Input: Single concatenated buffer (auto-split)" << std::endl;
    std::cout << "   - API: ie.Run(concatenated_buffer, output_buffer)" << std::endl;
    
    if (!ie.IsMultiInputModel()) {
        std::cout << "   [WARNING]  Skipped: Not a multi-input model" << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return true;
    }
    
    try {
        // Create concatenated input buffer
        size_t totalInputSize = ie.GetInputSize();
        auto concatenatedInput = createDummyInput(totalInputSize);
        
        // Create output buffer
        std::vector<uint8_t> outputBuffer(ie.GetOutputSize());
        
        // Run inference
        auto start = std::chrono::high_resolution_clock::now();
        auto outputs = ie.Run(concatenatedInput.data(), nullptr, outputBuffer.data());
        auto end = std::chrono::high_resolution_clock::now();
        
        // Validate and report
        auto outputSizes = ie.GetOutputTensorSizes();
        size_t expectedOutputCount = outputSizes.size();
        bool success = validateOutputs(outputs, expectedOutputCount, "Auto-Split");
        if (success) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "         Inference time: " << std::fixed << std::setprecision(2) 
                      << (duration.count() / 1000.0) << " ms" << std::endl;
        }
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return success;
        
    } catch (const std::exception& e) {
        std::cout << "   [ERROR] Error: " << e.what() << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return false;
    }
}

// Example 7: Multi-Input Batch Inference (Explicit Batch Format)
bool example7_batchInferenceExplicit(dxrt::InferenceEngine& ie, int batchSize = 3) {
    std::cout << "\n7. Batch Inference - Explicit Format (batch_size=" << batchSize << ")" << std::endl;
    std::cout << "   - Input: Vector of concatenated buffers (batch structure)" << std::endl;
    std::cout << "   - API: ie.Run(batch_inputs, batch_outputs)" << std::endl;
    
    if (!ie.IsMultiInputModel()) {
        std::cout << "   [WARNING]  Skipped: Not a multi-input model" << std::endl;
        return true;
    }
    
    try {
        // Create batch input data
        std::vector<std::vector<uint8_t>> batchInputData(batchSize);
        std::vector<void*> batchInputPtrs;
        std::vector<std::vector<uint8_t>> batchOutputData(batchSize);
        std::vector<void*> batchOutputPtrs;
        std::vector<void*> userArgs;
        
        size_t totalInputSize = ie.GetInputSize();
        size_t totalOutputSize = ie.GetOutputSize();
        
        for (int i = 0; i < batchSize; ++i) {
            // Create concatenated input buffer for this sample
            batchInputData[i] = createDummyInput(totalInputSize, i * 10);
            batchInputPtrs.push_back(batchInputData[i].data());
            
            // Create output buffer for this sample
            batchOutputData[i].resize(totalOutputSize);
            batchOutputPtrs.push_back(batchOutputData[i].data());
            
            // User argument for this sample
            userArgs.push_back(reinterpret_cast<void*>(static_cast<uintptr_t>(i)));
        }
        
        // Run batch inference
        auto start = std::chrono::high_resolution_clock::now();
        auto batchOutputs = ie.Run(batchInputPtrs, batchOutputPtrs, userArgs);
        auto end = std::chrono::high_resolution_clock::now();
        
        // Validate batch outputs
        auto outputSizes = ie.GetOutputTensorSizes();
        size_t expectedOutputCount = outputSizes.size();
        
        std::cout << std::endl;
        bool success = true;
        if (batchOutputs.size() != static_cast<size_t>(batchSize)) {
            std::cout << "[ERROR] Batch Explicit: Expected " << batchSize 
                      << " batch outputs, got " << batchOutputs.size() << std::endl;
            success = false;
        } else {
            for (size_t i = 0; i < batchOutputs.size(); ++i) {
                std::stringstream ss;
                ss << "Batch Explicit Sample " << i;
                if (!validateOutputs(batchOutputs[i], expectedOutputCount, ss.str())) {
                    success = false;
                    break;
                }
            }
            if (success) {
                std::cout << "[RESULT] Batch Explicit: All batch outputs valid (" 
                          << batchOutputs.size() << " samples)" << std::endl;
            }
        }
        
        if (success) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            double totalTimeMs = duration.count() / 1000.0;
            double avgTimeMs = totalTimeMs / batchSize;
            std::cout << "     Total time: " << std::fixed << std::setprecision(2) 
                      << totalTimeMs << " ms" << std::endl;
            std::cout << "     Average per sample: " << std::fixed << std::setprecision(2) 
                      << avgTimeMs << " ms" << std::endl;
        }
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return success;
        
    } catch (const std::exception& e) {
        std::cout << "   [ERROR] Error: " << e.what() << std::endl;
        return false;
    }
}

// Async inference handler
class AsyncInferenceHandler {
private:
    std::atomic<int> completedCount{0};
    int totalCount;
    std::map<int, dxrt::TensorPtrs> results;
    std::mutex lock;
    std::queue<std::string> completionQueue;
    std::vector<std::string> validationErrors;
    
public:
    explicit AsyncInferenceHandler(int total) : totalCount(total) {}
    
    int callback(dxrt::TensorPtrs& outputs, void* userArg) {
        std::unique_lock<std::mutex> lockGuard(lock);
        int sampleId = static_cast<int>(reinterpret_cast<uintptr_t>(userArg));
        results[sampleId] = outputs;
        int completed = ++completedCount;
        
        // Validate outputs in callback
        try {
            if (outputs.empty()) {
                validationErrors.push_back("Sample " + std::to_string(sampleId) + ": empty outputs");
            } else {
                // Validate each output tensor
                for (size_t i = 0; i < outputs.size(); ++i) {
                    if (!outputs[i]) {
                        validationErrors.push_back("Sample " + std::to_string(sampleId) + 
                                                 ": Output " + std::to_string(i) + " is null");
                    } else if (outputs[i]->size_in_bytes() == 0) {
                        validationErrors.push_back("Sample " + std::to_string(sampleId) + 
                                                 ": Output " + std::to_string(i) + " is empty (size=0)");
                    } else if (outputs[i]->shape().empty()) {
                        validationErrors.push_back("Sample " + std::to_string(sampleId) + 
                                                 ": Output " + std::to_string(i) + " has invalid shape");
                    } else if (!outputs[i]->data()) {
                        validationErrors.push_back("Sample " + std::to_string(sampleId) + 
                                                 ": Output " + std::to_string(i) + " has null data pointer");
                    }
                }
            }
        } catch (const std::exception& e) {
            validationErrors.push_back("Sample " + std::to_string(sampleId) + 
                                     ": Validation error - " + std::string(e.what()));
        }
        
        std::cout << "   Async callback: sample_" << sampleId << " completed (" 
                  << completed << "/" << totalCount << ")" << std::endl;
        
        if (completed >= totalCount) {
            completionQueue.push("done");
        }
        
        return 0;
    }
    
    bool waitForCompletion(int timeoutSeconds = 30) {
        auto startTime = std::chrono::steady_clock::now();
        while (completionQueue.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
            if (elapsed.count() >= timeoutSeconds) {
                return false;
            }
        }
        return true;
    }
    
    const std::vector<std::string>& getValidationErrors() const {
        return validationErrors;
    }
    
    size_t getResultCount() const {
        return results.size();
    }
};

// Example 8: Multi-Input Async Inference with Callback
bool example8_asyncInferenceCallback(dxrt::InferenceEngine& ie, int asyncCount = 3) {
    std::cout << "\n8. Async Inference with Callback (async_count=" << asyncCount << ")" << std::endl;
    std::cout << "   - Input: Dictionary format with callback" << std::endl;
    std::cout << "   - API: ie.RunAsyncMultiInput(input_dict, callback)" << std::endl;
    
    if (!ie.IsMultiInputModel()) {
        std::cout << "   [WARNING]  Skipped: Not a multi-input model" << std::endl;
        return true;
    }
    
    try {
        // Create async handler
        AsyncInferenceHandler handler(asyncCount);
        
        // Register callback
        ie.RegisterCallback([&handler](dxrt::TensorPtrs& outputs, void* userArg) -> int {
            return handler.callback(outputs, userArg);
        });
        
        // Get input tensor information
        auto inputNames = ie.GetInputTensorNames();
        auto inputSizes = ie.GetInputTensorSizes();
        
        // Submit async requests
        std::vector<std::vector<std::vector<uint8_t>>> asyncInputData(asyncCount);
        std::vector<std::map<std::string, void*>> asyncInputTensors(asyncCount);
        std::vector<int> jobIds;
        
        for (int i = 0; i < asyncCount; ++i) {
            // Create input data for each tensor
            asyncInputData[i].resize(inputNames.size());
            for (size_t j = 0; j < inputNames.size(); ++j) {
                asyncInputData[i][j] = createDummyInput(inputSizes[j], i * 15);
                asyncInputTensors[i][inputNames[j]] = asyncInputData[i][j].data();
            }
            
            // Submit async inference
            void* userArg = reinterpret_cast<void*>(static_cast<uintptr_t>(i));
            int jobId = ie.RunAsyncMultiInput(asyncInputTensors[i], userArg);
            jobIds.push_back(jobId);
        }
        
        std::cout << "   Submitted " << asyncCount << " async requests (job IDs: ";
        for (size_t i = 0; i < jobIds.size(); ++i) {
            std::cout << jobIds[i];
            if (i < jobIds.size() - 1) std::cout << ", ";
        }
        std::cout << ")" << std::endl;
        
        // Wait for completion
        bool success = handler.waitForCompletion(30);
        
        // Clear callback
        ie.RegisterCallback(nullptr);
        
        if (success && handler.getResultCount() == static_cast<size_t>(asyncCount)) {
            // Check for validation errors
            const auto& validationErrors = handler.getValidationErrors();
            if (!validationErrors.empty()) {
                std::cout << "   [ERROR] Async inference validation errors:" << std::endl;
                for (const auto& error : validationErrors) {
                    std::cout << "     " << error << std::endl;
                }
                std::cout << "\n" << std::string(60, '-') << std::endl;
                return false;
            } else {
                std::cout << "   [RESULT] All async inferences completed successfully" << std::endl;
                std::cout << "\n" << std::string(60, '-') << std::endl;
                return true;
            }
        } else {
            std::cout << "   [ERROR] Async inference failed or timed out" << std::endl;
            std::cout << "\n" << std::string(60, '-') << std::endl;
            return false;
        }
        
    } catch (const std::exception& e) {
        std::cout << "   [ERROR] Error: " << e.what() << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return false;
    }
}

// Example 9: Simple Async Inference
bool example9_simpleAsyncInference(dxrt::InferenceEngine& ie, int loopCount = 3) {
    std::cout << "\n9. Simple Async Inference (loop_count=" << loopCount << ")" << std::endl;
    std::cout << "   - Input: Single buffer with simple async" << std::endl;
    std::cout << "   - API: ie.RunAsync(input_buffer)" << std::endl;
    
    try {
        // Global variables for callback
        std::atomic<int> globalLoopCount{0};
        std::queue<int> completionQueue;
        std::mutex callbackLock;
        
        auto simpleCallback = [&globalLoopCount, &completionQueue, &callbackLock, loopCount]
                             (dxrt::TensorPtrs& outputs, void* userArg) -> int {
            std::unique_lock<std::mutex> lockGuard(callbackLock);
            int index = static_cast<int>(reinterpret_cast<uintptr_t>(userArg));
            int completed = ++globalLoopCount;
            
            // Validate outputs in callback
            try {
                if (outputs.empty()) {
                    std::cout << "   [ERROR] Simple async callback " << index << ": empty outputs" << std::endl;
                } else {
                    // Validate each output tensor
                    bool allValid = true;
                    for (size_t i = 0; i < outputs.size(); ++i) {
                        if (!outputs[i]) {
                            std::cout << "   [ERROR] Simple async callback " << index 
                                      << ": Output " << i << " is null" << std::endl;
                            allValid = false;
                        } else if (outputs[i]->size_in_bytes() == 0) {
                            std::cout << "   [ERROR] Simple async callback " << index 
                                      << ": Output " << i << " is empty (size=0)" << std::endl;
                            allValid = false;
                        } else if (outputs[i]->shape().empty()) {
                            std::cout << "   [ERROR] Simple async callback " << index 
                                      << ": Output " << i << " has invalid shape" << std::endl;
                            allValid = false;
                        } else if (!outputs[i]->data()) {
                            std::cout << "   [ERROR] Simple async callback " << index 
                                      << ": Output " << i << " has null data pointer" << std::endl;
                            allValid = false;
                        }
                    }
                    if (allValid) {
                        std::cout << "   Simple async callback: index=" << index 
                                  << " (" << completed << "/" << loopCount << ")" << std::endl;
                    }
                }
            } catch (const std::exception& e) {
                std::cout << "   [ERROR] Simple async callback " << index 
                          << ": validation error - " << e.what() << std::endl;
            }
            
            if (completed == loopCount) {
                completionQueue.push(0);
            }
            return 0;
        };
        
        // Register callback
        ie.RegisterCallback(simpleCallback);
        
        // Create input buffer
        size_t inputSize = ie.GetInputSize();
        auto inputBuffer = createDummyInput(inputSize);
        
        // Submit async inferences
        for (int i = 0; i < loopCount; ++i) {
            void* userArg = reinterpret_cast<void*>(static_cast<uintptr_t>(i));
            ie.RunAsync(inputBuffer.data(), userArg);
        }
        
        std::cout << "   Submitted " << loopCount << " simple async requests" << std::endl;
        
        // Wait for completion
        auto startTime = std::chrono::steady_clock::now();
        bool completed = false;
        while (!completed) {
            {
                std::unique_lock<std::mutex> lockGuard(callbackLock);
                if (!completionQueue.empty()) {
                    completionQueue.pop();
                    completed = true;
                }
            }
            if (!completed) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
                if (elapsed.count() >= 30) {
                    break;
                }
            }
        }
        
        ie.RegisterCallback(nullptr);  // Clear callback
        
        if (completed) {
            std::cout << "   [RESULT] All simple async inferences completed" << std::endl;
            std::cout << "\n" << std::string(60, '-') << std::endl;
            return true;
        } else {
            std::cout << "   [ERROR] Simple async inference timed out" << std::endl;
            std::cout << "\n" << std::string(60, '-') << std::endl;
            return false;
        }
        
    } catch (const std::exception& e) {
        std::cout << "   [ERROR] Error: " << e.what() << std::endl;
        std::cout << "\n" << std::string(60, '-') << std::endl;
        return false;
    }
}

int main(int argc, char* argv[]) {
    std::string modelPath;
    
    if (argc > 1) {
        modelPath = argv[1];
    } else {
        std::cout << "Usage: " << argv[0] << " <model_path>" << std::endl;
        std::cout << "Example: " << argv[0] << " model.dxnn" << std::endl;
        return -1;
    }
    
    std::cout << "Multi-Input Model Inference Examples" << std::endl;
    std::cout << "Model: " << modelPath << std::endl;
    
    // Track test results
    std::vector<std::pair<std::string, bool>> testResults;
    
    try {
        // Create inference engine
        dxrt::InferenceEngine ie(modelPath);
        
        // Print model information once
        printModelInfo(ie);
        
        // Run all examples and collect results
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "                    RUNNING TESTS" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        // Single inference tests without output buffers (auto-allocated)
        testResults.emplace_back("Dictionary Format (No Buffer)", example1_singleInferenceDictionaryNoBuffer(ie));
        testResults.emplace_back("Vector Format (No Buffer)", example2_singleInferenceVectorNoBuffer(ie));
        testResults.emplace_back("Auto-Split (No Buffer)", example3_autoSplitInferenceNoBuffer(ie));
        
        // Single inference tests with output buffers
        testResults.emplace_back("Dictionary Format (With Buffer)", example4_singleInferenceDictionary(ie));
        testResults.emplace_back("Vector Format (With Buffer)", example5_singleInferenceVector(ie));
        testResults.emplace_back("Auto-Split (With Buffer)", example6_autoSplitInference(ie));
        
        // Batch inference tests (output buffers required)
        testResults.emplace_back("Batch Explicit", example7_batchInferenceExplicit(ie, 3));
        
        // Async inference tests
        testResults.emplace_back("Async Callback", example8_asyncInferenceCallback(ie, 3));
        testResults.emplace_back("Simple Async", example9_simpleAsyncInference(ie, 3));
        
    } catch (const dxrt::Exception& e) {
        std::cerr << "[ERROR] Critical Error: " << e.what() << " (Code: " << e.code() << ")" << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Critical Error: " << e.what() << std::endl;
        return -1;
    }
    
    // Print test summary
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "                    TEST SUMMARY" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    int passed = 0;
    int failed = 0;
    
    for (const auto& result : testResults) {
        std::string status = result.second ? "* PASS" : "* FAIL";
        std::cout << status << " | " << result.first << std::endl;
        if (result.second) {
            passed++;
        } else {
            failed++;
        }
    }
    
    std::cout << std::string(60, '-') << std::endl;
    std::cout << "Total: " << testResults.size() << " | Passed: " << passed << " | Failed: " << failed << std::endl;
    
    if (failed == 0) {
        std::cout << " *** All tests passed successfully! ***" << std::endl;
        return 0;
    } else {
        std::cout << "[WARNING]  Some tests failed!" << std::endl;
        return 1;
    }
} 