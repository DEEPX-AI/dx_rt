/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <msclr/marshal_cppstd.h>
#include <vcclr.h>
#include <string>
#include <vector>

// Include native dxrt headers
#include "dxrt/dxrt_api.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

namespace DxEngineNative {

    /// <summary>
    /// Managed wrapper for dxrt::InferenceOption::BOUND_OPTION enum
    /// </summary>
    public enum class BoundOption {
        NpuAll = 0,
        Npu0 = 1,
        Npu1 = 2,
        Npu2 = 3,
        Npu01 = 4,
        Npu12 = 5,
        Npu02 = 6
    };

    /// <summary>
    /// Managed wrapper for dxrt::InferenceOption
    /// </summary>
    public ref class InferenceOptionWrapper {
    private:
        dxrt::InferenceOption* _native;
        bool _disposed;

    public:
        InferenceOptionWrapper();
        ~InferenceOptionWrapper();
        !InferenceOptionWrapper();

        property bool UseOrt {
            bool get();
            void set(bool value);
        }

        property int BoundOpt {
            int get();
            void set(int value);
        }

        property int BufferCount {
            int get();
            void set(int value);
        }

        property List<int>^ Devices {
            List<int>^ get();
            void set(List<int>^ value);
        }

        dxrt::InferenceOption* GetNative() { return _native; }
    };

    /// <summary>
    /// Tensor information for managed code
    /// </summary>
    public ref class TensorInfoWrapper {
    public:
        property int Index;
        property String^ Name;
        property array<int>^ Shape;
        property String^ DataType;
        property Int64 ElementCount;
        property Int64 SizeInBytes;
    };

    /// <summary>
    /// Managed wrapper for dxrt::InferenceEngine
    /// </summary>
    public ref class InferenceEngineWrapper {
    private:
        dxrt::InferenceEngine* _native;
        bool _disposed;

    public:
        /// <summary>
        /// Creates an InferenceEngine from model path
        /// </summary>
        InferenceEngineWrapper(String^ modelPath);
        
        /// <summary>
        /// Creates an InferenceEngine with options
        /// </summary>
        InferenceEngineWrapper(String^ modelPath, InferenceOptionWrapper^ option);

        ~InferenceEngineWrapper();
        !InferenceEngineWrapper();

        /// <summary>
        /// Run synchronous inference
        /// </summary>
        array<array<Byte>^>^ Run(array<Byte>^ inputData);

        /// <summary>
        /// Run benchmark
        /// </summary>
        float RunBenchmark(int loops, array<Byte>^ inputData);

        /// <summary>
        /// Get input tensor count
        /// </summary>
        property int InputCount {
            int get();
        }

        /// <summary>
        /// Get output tensor count
        /// </summary>
        property int OutputCount {
            int get();
        }

        /// <summary>
        /// Get total input size in bytes
        /// </summary>
        UInt64 GetInputSize();

        /// <summary>
        /// Get total output size in bytes
        /// </summary>
        UInt64 GetOutputSize();

        /// <summary>
        /// Get model name
        /// </summary>
        String^ GetModelName();

        /// <summary>
        /// Get latest latency in microseconds
        /// </summary>
        int GetLatency();

        /// <summary>
        /// Get NPU inference time in microseconds
        /// </summary>
        UInt32 GetNpuInferenceTime();

        /// <summary>
        /// Get mean latency
        /// </summary>
        double GetLatencyMean();

        /// <summary>
        /// Get mean NPU inference time
        /// </summary>
        double GetNpuInferenceTimeMean();

        /// <summary>
        /// Get input tensor info list
        /// </summary>
        List<TensorInfoWrapper^>^ GetInputTensorInfo();

        /// <summary>
        /// Get output tensor info list
        /// </summary>
        List<TensorInfoWrapper^>^ GetOutputTensorInfo();
    };

    /// <summary>
    /// Static utility functions
    /// </summary>
    public ref class DxUtility {
    public:
        /// <summary>
        /// Get the number of available NPU devices
        /// </summary>
        static int GetDeviceCount();

        /// <summary>
        /// Get DX-RT version string
        /// </summary>
        static String^ GetVersion();

        /// <summary>
        /// Parse a model file and display information
        /// </summary>
        static int ParseModel(String^ modelPath, bool verbose, bool jsonExtract, String^ outputFile);
    };
}
