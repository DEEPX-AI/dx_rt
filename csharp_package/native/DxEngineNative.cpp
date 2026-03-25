/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "DxEngineNative.h"
#include <msclr/marshal_cppstd.h>

using namespace msclr::interop;

namespace DxEngineNative {

    // ============================================================
    // InferenceOptionWrapper Implementation
    // ============================================================

    InferenceOptionWrapper::InferenceOptionWrapper() {
        _native = new dxrt::InferenceOption();
        _disposed = false;
    }

    InferenceOptionWrapper::~InferenceOptionWrapper() {
        this->!InferenceOptionWrapper();
    }

    InferenceOptionWrapper::!InferenceOptionWrapper() {
        if (!_disposed && _native != nullptr) {
            delete _native;
            _native = nullptr;
            _disposed = true;
        }
    }

    bool InferenceOptionWrapper::UseOrt::get() {
        return _native->useORT;
    }

    void InferenceOptionWrapper::UseOrt::set(bool value) {
        _native->useORT = value;
    }

    int InferenceOptionWrapper::BoundOpt::get() {
        return static_cast<int>(_native->boundOption);
    }

    void InferenceOptionWrapper::BoundOpt::set(int value) {
        _native->boundOption = static_cast<uint32_t>(value);
    }

    int InferenceOptionWrapper::BufferCount::get() {
        return _native->bufferCount;
    }

    void InferenceOptionWrapper::BufferCount::set(int value) {
        _native->bufferCount = value;
    }

    List<int>^ InferenceOptionWrapper::Devices::get() {
        auto list = gcnew List<int>();
        for (int dev : _native->devices) {
            list->Add(dev);
        }
        return list;
    }

    void InferenceOptionWrapper::Devices::set(List<int>^ value) {
        _native->devices.clear();
        if (value != nullptr) {
            for each (int dev in value) {
                _native->devices.push_back(dev);
            }
        }
    }

    // ============================================================
    // InferenceEngineWrapper Implementation
    // ============================================================

    InferenceEngineWrapper::InferenceEngineWrapper(String^ modelPath) {
        try {
            std::string nativeModelPath = marshal_as<std::string>(modelPath);
            _native = new dxrt::InferenceEngine(nativeModelPath);
            _disposed = false;
        }
        catch (const std::exception& ex) {
            throw gcnew System::Exception(gcnew String(ex.what()));
        }
        catch (...) {
            throw gcnew System::Exception("Unknown native exception occurred");
        }
    }

    InferenceEngineWrapper::InferenceEngineWrapper(String^ modelPath, InferenceOptionWrapper^ option) {
        try {
            std::string nativeModelPath = marshal_as<std::string>(modelPath);
            if (option != nullptr) {
                _native = new dxrt::InferenceEngine(nativeModelPath, *option->GetNative());
            } else {
                _native = new dxrt::InferenceEngine(nativeModelPath);
            }
            _disposed = false;
        }
        catch (const std::exception& ex) {
            throw gcnew System::Exception(gcnew String(ex.what()));
        }
        catch (...) {
            throw gcnew System::Exception("Unknown native exception occurred");
        }
    }

    InferenceEngineWrapper::~InferenceEngineWrapper() {
        this->!InferenceEngineWrapper();
    }

    InferenceEngineWrapper::!InferenceEngineWrapper() {
        if (!_disposed && _native != nullptr) {
            delete _native;
            _native = nullptr;
            _disposed = true;
        }
    }

    array<array<Byte>^>^ InferenceEngineWrapper::Run(array<Byte>^ inputData) {
        // Pin the managed array and get native pointer
        pin_ptr<Byte> pinnedInput = &inputData[0];
        void* inputPtr = pinnedInput;

        // Run inference
        auto outputs = _native->Run(inputPtr);

        // Convert outputs to managed arrays
        auto result = gcnew array<array<Byte>^>(static_cast<int>(outputs.size()));
        
        for (size_t i = 0; i < outputs.size(); i++) {
            auto& tensor = outputs[i];
            size_t size = tensor->size_in_bytes();
            auto outputArray = gcnew array<Byte>(static_cast<int>(size));
            
            // Copy data
            pin_ptr<Byte> pinnedOutput = &outputArray[0];
            memcpy(pinnedOutput, tensor->data(), size);
            
            result[static_cast<int>(i)] = outputArray;
        }

        return result;
    }

    float InferenceEngineWrapper::RunBenchmark(int loops, array<Byte>^ inputData) {
        void* inputPtr = nullptr;
        pin_ptr<Byte> pinnedInput = nullptr;
        
        if (inputData != nullptr && inputData->Length > 0) {
            pinnedInput = &inputData[0];
            inputPtr = pinnedInput;
        }

        return _native->RunBenchmark(loops, inputPtr);
    }

    int InferenceEngineWrapper::InputCount::get() {
        auto inputs = _native->GetInputs();
        return static_cast<int>(inputs.size());
    }

    int InferenceEngineWrapper::OutputCount::get() {
        auto outputs = _native->GetOutputs();
        return static_cast<int>(outputs.size());
    }

    UInt64 InferenceEngineWrapper::GetInputSize() {
        return _native->GetInputSize();
    }

    UInt64 InferenceEngineWrapper::GetOutputSize() {
        return _native->GetOutputSize();
    }

    String^ InferenceEngineWrapper::GetModelName() {
        return gcnew String(_native->GetModelName().c_str());
    }

    int InferenceEngineWrapper::GetLatency() {
        return _native->GetLatency();
    }

    UInt32 InferenceEngineWrapper::GetNpuInferenceTime() {
        return _native->GetNpuInferenceTime();
    }

    double InferenceEngineWrapper::GetLatencyMean() {
        return _native->GetLatencyMean();
    }

    double InferenceEngineWrapper::GetNpuInferenceTimeMean() {
        return _native->GetNpuInferenceTimeMean();
    }

    List<TensorInfoWrapper^>^ InferenceEngineWrapper::GetInputTensorInfo() {
        auto list = gcnew List<TensorInfoWrapper^>();
        auto inputs = _native->GetInputs();
        
        int index = 0;
        for (auto& tensor : inputs) {
            auto info = gcnew TensorInfoWrapper();
            info->Index = index++;
            info->Name = gcnew String(tensor.name().c_str());
            
            // Convert shape
            auto& shapeVec = tensor.shape();
            auto shape = gcnew array<int>(static_cast<int>(shapeVec.size()));
            for (size_t i = 0; i < shapeVec.size(); i++) {
                shape[static_cast<int>(i)] = static_cast<int>(shapeVec[i]);
            }
            info->Shape = shape;
            
            // Calculate element count
            Int64 elements = 1;
            for (auto dim : shapeVec) {
                elements *= dim;
            }
            info->ElementCount = elements;
            info->SizeInBytes = static_cast<Int64>(tensor.size_in_bytes());
            
            list->Add(info);
        }
        
        return list;
    }

    List<TensorInfoWrapper^>^ InferenceEngineWrapper::GetOutputTensorInfo() {
        auto list = gcnew List<TensorInfoWrapper^>();
        auto outputs = _native->GetOutputs();
        
        int index = 0;
        for (auto& tensor : outputs) {
            auto info = gcnew TensorInfoWrapper();
            info->Index = index++;
            info->Name = gcnew String(tensor.name().c_str());
            
            // Convert shape
            auto& shapeVec = tensor.shape();
            auto shape = gcnew array<int>(static_cast<int>(shapeVec.size()));
            for (size_t i = 0; i < shapeVec.size(); i++) {
                shape[static_cast<int>(i)] = static_cast<int>(shapeVec[i]);
            }
            info->Shape = shape;
            
            // Calculate element count
            Int64 elements = 1;
            for (auto dim : shapeVec) {
                elements *= dim;
            }
            info->ElementCount = elements;
            info->SizeInBytes = static_cast<Int64>(tensor.size_in_bytes());
            
            list->Add(info);
        }
        
        return list;
    }

    // ============================================================
    // DxUtility Implementation
    // ============================================================

    int DxUtility::GetDeviceCount() {
        return dxrt::DeviceStatus::GetDeviceCount();
    }

    String^ DxUtility::GetVersion() {
        return gcnew String(dxrt::Configuration::GetInstance().GetVersion().c_str());
    }

    int DxUtility::ParseModel(String^ modelPath, bool verbose, bool jsonExtract, String^ outputFile) {
        try {
            std::string nativeModelPath = marshal_as<std::string>(modelPath);
            
            dxrt::ParseOptions options;
            options.verbose = verbose;
            options.json_extract = jsonExtract;
            options.no_color = (outputFile != nullptr && outputFile->Length > 0);
            
            if (outputFile != nullptr && outputFile->Length > 0) {
                options.output_file = marshal_as<std::string>(outputFile);
            }
            
            return dxrt::ParseModel(nativeModelPath, options);
        }
        catch (const std::exception& ex) {
            throw gcnew System::Exception(gcnew String(ex.what()));
        }
        catch (...) {
            throw gcnew System::Exception("Unknown native exception occurred");
        }
    }
}
