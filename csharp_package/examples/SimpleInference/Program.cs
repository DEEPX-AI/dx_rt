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
using System.Diagnostics;
using System.IO;
using DxEngine;

namespace DxEngine.Examples
{
    /// <summary>
    /// Simple inference example without OpenCV dependency.
    /// Demonstrates pure DxEngine usage for NPU inference.
    /// </summary>
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("=== Simple NPU Inference Example (No OpenCV) ===");
            Console.WriteLine();

            // Parse arguments
            string? modelPath = null;
            string? inputPath = null;
            string? outputPath = null;
            int loops = 30;
            bool benchmark = false;
            bool verbose = false;

            for (int i = 0; i < args.Length; i++)
            {
                switch (args[i].ToLower())
                {
                    case "--model":
                    case "-m":
                        if (i + 1 < args.Length) modelPath = args[++i];
                        break;
                    case "--input":
                    case "-i":
                        if (i + 1 < args.Length) inputPath = args[++i];
                        break;
                    case "--output":
                    case "-o":
                        if (i + 1 < args.Length) outputPath = args[++i];
                        break;
                    case "--loops":
                    case "-l":
                        if (i + 1 < args.Length && int.TryParse(args[++i], out int l)) loops = l;
                        break;
                    case "--benchmark":
                    case "-b":
                        benchmark = true;
                        break;
                    case "--verbose":
                    case "-v":
                        verbose = true;
                        break;
                    case "--help":
                    case "-h":
                        PrintHelp();
                        return;
                }
            }

            // Check native library
            if (!NativeBridge.IsAvailable)
            {
                Console.WriteLine($"[ERROR] Native library not available: {NativeBridge.LoadError}");
                return;
            }

            Console.WriteLine($"[INFO] DX-RT Version: {NativeBridge.GetVersion()}");
            Console.WriteLine($"[INFO] NPU Device Count: {NativeBridge.GetDeviceCount()}");
            Console.WriteLine();

            if (string.IsNullOrEmpty(modelPath))
            {
                PrintHelp();
                return;
            }

            if (!File.Exists(modelPath))
            {
                Console.WriteLine($"[ERROR] Model file not found: {modelPath}");
                return;
            }

            try
            {
                RunInference(modelPath, inputPath, outputPath, loops, benchmark, verbose);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[ERROR] {ex.Message}");
                if (ex.InnerException != null)
                    Console.WriteLine($"  Inner: {ex.InnerException.Message}");
            }
        }

        static void RunInference(string modelPath, string? inputPath, string? outputPath, 
                                  int loops, bool benchmark, bool verbose)
        {
            Console.WriteLine($"[INFO] Loading model: {modelPath}");

            // Create inference option
            var option = new InferenceOption
            {
                UseOrt = false,
                BoundOption = BoundOption.NpuAll,
                BufferCount = 6
            };

            // Create inference engine
            using var engine = new InferenceEngineNative(modelPath, option);

            // Print model info
            Console.WriteLine();
            Console.WriteLine("=== Model Information ===");
            Console.WriteLine($"  Model Name  : {engine.GetModelName()}");
            Console.WriteLine($"  Input Count : {engine.InputCount}");
            Console.WriteLine($"  Output Count: {engine.OutputCount}");
            Console.WriteLine($"  Input Size  : {engine.GetInputSize():N0} bytes");
            Console.WriteLine($"  Output Size : {engine.GetOutputSize():N0} bytes");

            Console.WriteLine();
            Console.WriteLine("  Input Tensors:");
            foreach (var info in engine.InputTensorInfo)
            {
                Console.WriteLine($"    - {info.Name} [{string.Join(", ", info.Shape)}]");
            }

            Console.WriteLine("  Output Tensors:");
            foreach (var info in engine.OutputTensorInfo)
            {
                Console.WriteLine($"    - {info.Name} [{string.Join(", ", info.Shape)}]");
            }
            Console.WriteLine();

            // Prepare input data
            byte[] inputData;
            if (!string.IsNullOrEmpty(inputPath) && File.Exists(inputPath))
            {
                inputData = File.ReadAllBytes(inputPath);
                Console.WriteLine($"[INFO] Loaded input from file: {inputPath} ({inputData.Length:N0} bytes)");
                
                if ((ulong)inputData.Length != engine.GetInputSize())
                {
                    Console.WriteLine($"[WARN] Input size mismatch. Expected {engine.GetInputSize()}, got {inputData.Length}");
                }
            }
            else
            {
                // Generate random input data for testing
                inputData = new byte[engine.GetInputSize()];
                new Random(42).NextBytes(inputData);
                Console.WriteLine($"[INFO] Using random input data ({inputData.Length:N0} bytes)");
            }

            if (benchmark)
            {
                // Benchmark mode
                RunBenchmark(engine, inputData, loops, verbose);
            }
            else
            {
                // Single inference mode
                RunSingleInference(engine, inputData, outputPath, loops, verbose);
            }
        }

        static void RunSingleInference(InferenceEngineNative engine, byte[] inputData, 
                                        string? outputPath, int loops, bool verbose)
        {
            Console.WriteLine($"\n=== Running Single Inference ({loops} loop(s)) ===");

            var latencies = new List<double>();
            var npuTimes = new List<double>();
            byte[]? lastOutput = null;

            for (int i = 0; i < loops; i++)
            {
                var sw = Stopwatch.StartNew();
                var outputs = engine.Run(inputData);
                sw.Stop();

                double latencyMs = engine.GetLatency() / 1000.0;
                double npuTimeMs = engine.GetNpuInferenceTime() / 1000.0;
                double wallTimeMs = sw.Elapsed.TotalMilliseconds;

                latencies.Add(latencyMs);
                npuTimes.Add(npuTimeMs);

                if (outputs.Count > 0)
                    lastOutput = outputs[0];

                if (verbose)
                {
                    Console.WriteLine($"  Loop {i + 1,3}: Latency={latencyMs,7:F3}ms, NPU={npuTimeMs,6:F3}ms, Wall={wallTimeMs,7:F3}ms");
                }
            }

            // Calculate statistics
            double avgLatency = Average(latencies);
            double avgNpuTime = Average(npuTimes);
            double minLatency = Min(latencies);
            double maxLatency = Max(latencies);

            Console.WriteLine();
            Console.WriteLine("=== Inference Results ===");
            Console.WriteLine($"  Loops          : {loops}");
            Console.WriteLine($"  Avg Latency    : {avgLatency:F3} ms");
            Console.WriteLine($"  Avg NPU Time   : {avgNpuTime:F3} ms");
            Console.WriteLine($"  Min Latency    : {minLatency:F3} ms");
            Console.WriteLine($"  Max Latency    : {maxLatency:F3} ms");
            Console.WriteLine($"  Throughput     : {1000.0 / avgLatency:F2} inferences/sec");

            // Save output if requested
            if (!string.IsNullOrEmpty(outputPath) && lastOutput != null)
            {
                File.WriteAllBytes(outputPath, lastOutput);
                Console.WriteLine($"\n[INFO] Output saved to: {outputPath} ({lastOutput.Length:N0} bytes)");
            }

            // Print first few output values (as float32)
            if (lastOutput != null && lastOutput.Length >= 4)
            {
                Console.WriteLine("\n  Output Preview (first 10 float32 values):");
                int numValues = Math.Min(10, lastOutput.Length / 4);
                Console.Write("    ");
                for (int i = 0; i < numValues; i++)
                {
                    float value = BitConverter.ToSingle(lastOutput, i * 4);
                    Console.Write($"{value,10:F4} ");
                }
                Console.WriteLine();
            }
        }

        static void RunBenchmark(InferenceEngineNative engine, byte[] inputData, int loops, bool verbose)
        {
            Console.WriteLine($"\n=== Running Benchmark ({loops} iterations) ===");

            // Warmup
            Console.WriteLine("[INFO] Warmup (3 iterations)...");
            for (int i = 0; i < 3; i++)
            {
                engine.Run(inputData);
            }

            // Benchmark
            Console.WriteLine("[INFO] Benchmarking...");
            var sw = Stopwatch.StartNew();
            float fps = engine.RunBenchmark(loops, inputData);
            sw.Stop();

            double latencyMean = engine.GetLatencyMean() / 1000.0;
            double npuTimeMean = engine.GetNpuInferenceTimeMean() / 1000.0;

            Console.WriteLine();
            Console.WriteLine("=== Benchmark Results ===");
            Console.WriteLine($"  Iterations     : {loops}");
            Console.WriteLine($"  Total Time     : {sw.Elapsed.TotalSeconds:F2} sec");
            Console.WriteLine($"  FPS            : {fps:F2}");
            Console.WriteLine($"  Avg Latency    : {latencyMean:F3} ms");
            Console.WriteLine($"  Avg NPU Time   : {npuTimeMean:F3} ms");
            Console.WriteLine($"  Throughput     : {fps:F2} inferences/sec");
        }

        static double Average(List<double> values)
        {
            if (values.Count == 0) return 0;
            double sum = 0;
            foreach (var v in values) sum += v;
            return sum / values.Count;
        }

        static double Min(List<double> values)
        {
            if (values.Count == 0) return 0;
            double min = double.MaxValue;
            foreach (var v in values) if (v < min) min = v;
            return min;
        }

        static double Max(List<double> values)
        {
            if (values.Count == 0) return 0;
            double max = double.MinValue;
            foreach (var v in values) if (v > max) max = v;
            return max;
        }

        static void PrintHelp()
        {
            Console.WriteLine("Simple NPU Inference Example");
            Console.WriteLine();
            Console.WriteLine("Usage:");
            Console.WriteLine("  SimpleInference --model <model.dxnn> [options]");
            Console.WriteLine();
            Console.WriteLine("Options:");
            Console.WriteLine("  --model, -m <path>   Path to DXNN model file (required)");
            Console.WriteLine("  --input, -i <path>   Path to raw input binary file (optional)");
            Console.WriteLine("  --output, -o <path>  Path to save output binary file (optional)");
            Console.WriteLine("  --loops, -l <n>      Number of inference loops (default: 30)");
            Console.WriteLine("  --benchmark, -b      Run benchmark mode");
            Console.WriteLine("  --verbose, -v        Show detailed output");
            Console.WriteLine("  --help, -h           Show this help");
            Console.WriteLine();
            Console.WriteLine("Examples:");
            Console.WriteLine("  SimpleInference -m model.dxnn -l 100 -v");
            Console.WriteLine("  SimpleInference -m model.dxnn -b -l 1000");
            Console.WriteLine("  SimpleInference -m model.dxnn -i input.bin -o output.bin");
        }
    }
}
