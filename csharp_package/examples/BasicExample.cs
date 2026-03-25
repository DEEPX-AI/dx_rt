//
// Copyright (C) 2018- DEEPX Ltd.
// All rights reserved.
//
// This software is the property of DEEPX and is provided exclusively to customers
// who are supplied with DEEPX NPU (Neural Processing Unit).
// Unauthorized sharing or usage is strictly prohibited by law.
//

using System;
using System.Collections.Generic;
using System.IO;
using DxEngine;

namespace DxEngine.Examples
{
    /// <summary>
    /// Example demonstrating basic usage of the DxEngine library with C++/CLI wrapper.
    /// </summary>
    class Program
    {
        // Default model path - change this to your model file
        static string ModelPath = @"C:\20260129\assets\models\models-2_2_0\MobileNetV2_2.dxnn";

        static void Main(string[] args)
        {
            // Allow model path override from command line
            if (args.Length > 0 && File.Exists(args[0]))
            {
                ModelPath = args[0];
            }

            Console.WriteLine("=== DxEngine C# Examples ===");
            Console.WriteLine();

            // Check if native library is available
            if (!NativeBridge.IsAvailable)
            {
                Console.WriteLine($"Error: Native library not available. {NativeBridge.LoadError}");
                return;
            }

            Console.WriteLine($"Native library loaded successfully!");
            Console.WriteLine($"DX-RT Version: {NativeBridge.GetVersion()}");
            Console.WriteLine();

            // Example 1: Device Status
            DeviceStatusExample();

            // Example 2: Basic Inference
            BasicInferenceExample();

            // Example 3: Benchmark
            BenchmarkExample();

            Console.WriteLine("=== All examples completed ===");
        }

        static void DeviceStatusExample()
        {
            Console.WriteLine("=== Device Status Example ===");
            
            try
            {
                int deviceCount = NativeBridge.GetDeviceCount();
                Console.WriteLine($"Found {deviceCount} NPU device(s)");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
            
            Console.WriteLine();
        }

        static void BasicInferenceExample()
        {
            Console.WriteLine("=== Basic Inference Example ===");
            
            if (!File.Exists(ModelPath))
            {
                Console.WriteLine($"Model file not found: {ModelPath}");
                Console.WriteLine("Skipping inference example.");
                Console.WriteLine();
                return;
            }

            try
            {
                Console.WriteLine($"Loading model: {ModelPath}");

                // Create inference option
                var option = new InferenceOption
                {
                    UseOrt = false,
                    BoundOption = BoundOption.NpuAll,
                    BufferCount = 6
                };

                // Create inference engine using native wrapper
                using var engine = new InferenceEngineNative(ModelPath, option);

                // Get model information
                Console.WriteLine($"Model name: {engine.GetModelName()}");
                Console.WriteLine($"Input count: {engine.InputCount}");
                Console.WriteLine($"Output count: {engine.OutputCount}");
                Console.WriteLine($"Input size: {engine.GetInputSize()} bytes");
                Console.WriteLine($"Output size: {engine.GetOutputSize()} bytes");

                // Print tensor information
                Console.WriteLine("\nInput Tensors:");
                foreach (var info in engine.InputTensorInfo)
                {
                    Console.WriteLine($"  - {info.Name} (shape: [{string.Join(", ", info.Shape)}])");
                }
                
                Console.WriteLine("Output Tensors:");
                foreach (var info in engine.OutputTensorInfo)
                {
                    Console.WriteLine($"  - {info.Name} (shape: [{string.Join(", ", info.Shape)}])");
                }

                // Prepare input data (zero-filled for demo)
                var inputData = new byte[engine.GetInputSize()];

                // Run inference
                Console.WriteLine("\nRunning inference...");
                var outputs = engine.Run(inputData);

                // Process outputs
                Console.WriteLine($"Got {outputs.Count} output(s)");
                for (int i = 0; i < outputs.Count; i++)
                {
                    Console.WriteLine($"  Output[{i}] size: {outputs[i].Length} bytes");
                }

                // Get timing info
                Console.WriteLine($"\nLatency: {engine.GetLatency() / 1000.0:F3} ms");
                Console.WriteLine($"NPU Inference Time: {engine.GetNpuInferenceTime() / 1000.0:F3} ms");

                Console.WriteLine("\nInference completed successfully!");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
                if (ex.InnerException != null)
                {
                    Console.WriteLine($"  Inner: {ex.InnerException.Message}");
                }
            }
            
            Console.WriteLine();
        }

        static void BenchmarkExample()
        {
            Console.WriteLine("=== Benchmark Example ===");
            
            if (!File.Exists(ModelPath))
            {
                Console.WriteLine($"Model file not found: {ModelPath}");
                Console.WriteLine("Skipping benchmark example.");
                Console.WriteLine();
                return;
            }

            try
            {
                Console.WriteLine($"Loading model: {ModelPath}");

                var option = new InferenceOption
                {
                    BufferCount = 6
                };

                using var engine = new InferenceEngineNative(ModelPath, option);

                // Prepare input data
                var inputData = new byte[engine.GetInputSize()];

                // Run benchmark
                int loops = 30;
                Console.WriteLine($"\nRunning benchmark ({loops} iterations)...");
                
                float fps = engine.RunBenchmark(loops, inputData);

                // Get timing statistics
                double latencyMean = engine.GetLatencyMean() / 1000.0;
                double npuTimeMean = engine.GetNpuInferenceTimeMean() / 1000.0;

                Console.WriteLine($"\nBenchmark Results:");
                Console.WriteLine($"  - FPS: {fps:F2}");
                Console.WriteLine($"  - Avg Latency: {latencyMean:F3} ms");
                Console.WriteLine($"  - Avg NPU Time: {npuTimeMean:F3} ms");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
                if (ex.InnerException != null)
                {
                    Console.WriteLine($"  Inner: {ex.InnerException.Message}");
                }
            }
            
            Console.WriteLine();
        }
    }
}
