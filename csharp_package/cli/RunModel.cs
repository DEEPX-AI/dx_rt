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
using System.Threading;
using DxEngine;

namespace DxEngine.Cli
{
    /// <summary>
    /// Run mode for model execution.
    /// </summary>
    public enum RunModelMode
    {
        /// <summary>Maximum throughput benchmark mode (default).</summary>
        BenchmarkMode = 0,
        /// <summary>Sequential single-input inference on single-core.</summary>
        SingleMode = 1,
        /// <summary>Target FPS mode with async execution.</summary>
        TargetFpsMode = 2
    }

    /// <summary>
    /// CLI tool for running model inference with benchmarking capabilities.
    /// Directly uses dxrt.dll through C++/CLI wrapper, similar to Python's run_model.py.
    /// </summary>
    public class RunModel
    {
        private const string AppName = "DXRT C# run_model";
        private readonly RunOptions _options;
        private RunModelMode _currentMode = RunModelMode.BenchmarkMode;

        /// <summary>
        /// Options for run model command.
        /// </summary>
        public class RunOptions
        {
            /// <summary>Model file path (required).</summary>
            public string ModelPath { get; set; } = string.Empty;

            /// <summary>Input data file (optional).</summary>
            public string InputFile { get; set; } = string.Empty;

            /// <summary>Output data file (default: output.bin).</summary>
            public string OutputFile { get; set; } = "output.bin";

            /// <summary>Perform benchmark test (default mode).</summary>
            public bool Benchmark { get; set; } = false;

            /// <summary>Perform single run test.</summary>
            public bool Single { get; set; } = false;

            /// <summary>Show NPU Processing Time and Latency.</summary>
            public bool Verbose { get; set; } = false;

            /// <summary>NPU bounding option (default: 0 for NPU_ALL).</summary>
            public int NpuOption { get; set; } = 0;

            /// <summary>Number of inference loops (default: 30).</summary>
            public int Loops { get; set; } = 30;

            /// <summary>Number of warmup runs (default: 0).</summary>
            public int WarmupRuns { get; set; } = 0;

            /// <summary>Target NPU devices specification.</summary>
            public string Devices { get; set; } = "all";

            /// <summary>Target FPS (enables target FPS mode if > 0).</summary>
            public int TargetFps { get; set; } = 0;

            /// <summary>Skip inference I/O (benchmark mode only).</summary>
            public bool SkipIo { get; set; } = false;

            /// <summary>Enable ONNX Runtime for CPU tasks.</summary>
            public bool UseOrt { get; set; } = false;

            /// <summary>Enable profiler.</summary>
            public bool Profiler { get; set; } = false;

            /// <summary>Number of input/output buffers.</summary>
            public int BufferCount { get; set; } = 6;
        }

        public RunModel(RunOptions options)
        {
            _options = options ?? throw new ArgumentNullException(nameof(options));
        }

        /// <summary>
        /// Execute the run model command using native InferenceEngine via C++/CLI wrapper.
        /// </summary>
        /// <returns>Exit code (0 for success).</returns>
        public int Execute()
        {
            try
            {
                // Validate options
                if (string.IsNullOrEmpty(_options.ModelPath))
                {
                    Console.Error.WriteLine("Error: Model path is required.");
                    return 1;
                }

                if (!File.Exists(_options.ModelPath))
                {
                    Console.Error.WriteLine($"Error: Model path '{_options.ModelPath}' does not exist.");
                    return 1;
                }

                // Determine run mode
                SetRunModelMode();

                if (_options.SkipIo && _currentMode != RunModelMode.BenchmarkMode)
                {
                    Console.Error.WriteLine("[ERR] --skip-io option is only available in BENCHMARK_MODE.");
                    return 1;
                }

                // Print configuration
                PrintConfiguration();

                // Create InferenceOption
                var option = CreateInferenceOption();

                // Create InferenceEngine using native wrapper
                Console.WriteLine($"\n=== Model File: {_options.ModelPath} ===\n");

                using var engine = new InferenceEngineNative(_options.ModelPath, option);

                // Print model information
                PrintModelInfo(engine);

                // Prepare input data
                var inputData = PrepareInputData(engine);

                // Perform warmup if specified
                if (_options.WarmupRuns > 0)
                {
                    Console.WriteLine($"Performing {_options.WarmupRuns} warmup run(s)...");
                    for (int i = 0; i < _options.WarmupRuns; i++)
                    {
                        engine.Run(inputData);
                    }
                    Console.WriteLine("Warmup completed.\n");
                }

                // Execute based on mode
                switch (_currentMode)
                {
                    case RunModelMode.SingleMode:
                        return ExecuteSingleMode(engine, inputData);

                    case RunModelMode.TargetFpsMode:
                        return ExecuteTargetFpsMode(engine, inputData);

                    case RunModelMode.BenchmarkMode:
                    default:
                        return ExecuteBenchmarkMode(engine, inputData);
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"[ERR] An unexpected error occurred: {ex.Message}");
                if (ex.InnerException != null)
                {
                    Console.Error.WriteLine($"  Inner: {ex.InnerException.Message}");
                }
                return 1;
            }
        }

        private void SetRunModelMode()
        {
            if (_options.Single)
            {
                _currentMode = RunModelMode.SingleMode;
            }
            else if (_options.TargetFps > 0)
            {
                _currentMode = RunModelMode.TargetFpsMode;
            }
            else
            {
                _currentMode = RunModelMode.BenchmarkMode;
            }
        }

        private void PrintConfiguration()
        {
            string modeStr = _currentMode switch
            {
                RunModelMode.SingleMode => "Single Mode",
                RunModelMode.TargetFpsMode => "Target FPS Mode",
                _ => "Benchmark Mode"
            };

            Console.WriteLine($"Run model target mode : {modeStr}");
            Console.WriteLine($"Model file: {_options.ModelPath}");
            
            if (!string.IsNullOrEmpty(_options.InputFile))
            {
                Console.WriteLine($"Input data file: {_options.InputFile}");
                Console.WriteLine($"Output data file: {_options.OutputFile}");
            }
            
            Console.WriteLine($"Loops: {_options.Loops}");
            Console.WriteLine($"Input/Output buffer count: {_options.BufferCount}");

            // Print device specification
            var devicesSpec = _options.Devices.Trim().ToLower();
            if (devicesSpec == "all" || string.IsNullOrEmpty(devicesSpec))
            {
                Console.WriteLine("Device specification: 'all' (engine default)");
            }
            else
            {
                Console.WriteLine($"Device specification: {_options.Devices}");
            }
        }

        private InferenceOption CreateInferenceOption()
        {
            var option = new InferenceOption
            {
                UseOrt = _options.UseOrt,
                BoundOption = (BoundOption)_options.NpuOption,
                BufferCount = _options.BufferCount
            };

            // Parse devices
            var devicesSpec = _options.Devices.Trim().ToLower();
            if (devicesSpec != "all" && !string.IsNullOrEmpty(devicesSpec))
            {
                if (devicesSpec.StartsWith("count:"))
                {
                    if (int.TryParse(devicesSpec.Substring(6), out int count) && count > 0)
                    {
                        for (int i = 0; i < count; i++)
                        {
                            option.Devices.Add(i);
                        }
                    }
                }
                else
                {
                    foreach (var part in devicesSpec.Split(','))
                    {
                        if (int.TryParse(part.Trim(), out int deviceId))
                        {
                            option.Devices.Add(deviceId);
                        }
                    }
                }
            }

            return option;
        }

        private void PrintModelInfo(InferenceEngineNative engine)
        {
            Console.WriteLine("Model Input Tensors:");
            foreach (var tensor in engine.InputTensorInfo)
            {
                Console.WriteLine($"  - {tensor.Name}");
            }

            Console.WriteLine("Model Output Tensors:");
            foreach (var tensor in engine.OutputTensorInfo)
            {
                Console.WriteLine($"  - {tensor.Name}");
            }

            Console.WriteLine($"\nInput size: {engine.GetInputSize()} bytes");
            Console.WriteLine($"Output size: {engine.GetOutputSize()} bytes");
        }

        private byte[] PrepareInputData(InferenceEngineNative engine)
        {
            int inputSize = (int)engine.GetInputSize();

            if (!string.IsNullOrEmpty(_options.InputFile) && File.Exists(_options.InputFile))
            {
                var fileData = File.ReadAllBytes(_options.InputFile);
                if (fileData.Length != inputSize)
                {
                    Console.Error.WriteLine($"[ERR] Input file size mismatch. Expected {inputSize}, got {fileData.Length}.");
                    throw new InvalidOperationException("Input file size mismatch.");
                }
                Console.WriteLine($"Loaded input data from: {_options.InputFile}");
                return fileData;
            }

            // Create zero-filled input buffer
            Console.WriteLine("Using zero-filled input buffer.");
            return new byte[inputSize];
        }

        private int ExecuteBenchmarkMode(InferenceEngineNative engine, byte[] inputData)
        {
            Console.WriteLine($"\nInference by loops: count={_options.Loops}");

            // Run benchmark
            float measuredFps = engine.RunBenchmark(_options.Loops, inputData);

            // Get timing statistics
            double latencyMeanMs = engine.GetLatencyMean() / 1000.0;
            double npuTimeMeanMs = engine.GetNpuInferenceTimeMean() / 1000.0;

            // Save output if input file was provided
            if (!string.IsNullOrEmpty(_options.InputFile) && !_options.SkipIo)
            {
                var outputs = engine.Run(inputData);
                if (outputs.Count > 0)
                {
                    File.WriteAllBytes(_options.OutputFile, outputs[0]);
                    Console.WriteLine($"Output saved to: {_options.OutputFile}");
                }
            }

            // Print result
            PrintBenchmarkResult(_options.Loops, latencyMeanMs, npuTimeMeanMs, measuredFps);

            return 0;
        }

        private int ExecuteSingleMode(InferenceEngineNative engine, byte[] inputData)
        {
            Console.WriteLine($"\nRunning single mode: {_options.Loops} iterations");

            double totalLatencyUs = 0;
            double totalNpuTimeUs = 0;
            double totalWallTimeUs = 0;

            for (int i = 0; i < _options.Loops; i++)
            {
                var sw = Stopwatch.StartNew();
                var outputs = engine.Run(inputData);
                sw.Stop();

                double loopWallTimeUs = sw.Elapsed.TotalMicroseconds;
                totalWallTimeUs += loopWallTimeUs;

                double currentLatencyUs = engine.GetLatency();
                double currentNpuTimeUs = engine.GetNpuInferenceTime();

                totalLatencyUs += currentLatencyUs;
                totalNpuTimeUs += currentNpuTimeUs;

                double loopFps = loopWallTimeUs > 0 ? 1_000_000.0 / loopWallTimeUs : 0;

                // Save output if input file was provided
                if (!string.IsNullOrEmpty(_options.InputFile) && !_options.SkipIo && outputs.Count > 0)
                {
                    File.WriteAllBytes(_options.OutputFile, outputs[0]);
                }

                // Print result for each iteration
                PrintSingleResult(i + 1, currentLatencyUs / 1000.0, currentNpuTimeUs / 1000.0, loopFps);
            }

            // Print average if multiple loops
            if (_options.Loops > 1)
            {
                double avgLatencyMs = (totalLatencyUs / _options.Loops) / 1000.0;
                double avgNpuMs = (totalNpuTimeUs / _options.Loops) / 1000.0;
                double avgFps = totalWallTimeUs > 0 ? (_options.Loops * 1_000_000.0) / totalWallTimeUs : 0;

                Console.WriteLine();
                Console.WriteLine(new string('=', 80));
                Console.WriteLine($"* Average over {_options.Loops} loops (Single Mode)");
                Console.WriteLine($"  - Avg NPU Processing Time: {avgNpuMs:F3} ms");
                Console.WriteLine($"  - Avg Latency            : {avgLatencyMs:F3} ms");
                Console.WriteLine($"  - Avg FPS                : {avgFps:F2}");
                Console.WriteLine(new string('=', 80));
            }

            return 0;
        }

        private int ExecuteTargetFpsMode(InferenceEngineNative engine, byte[] inputData)
        {
            Console.WriteLine($"\nTarget FPS: {_options.TargetFps}");
            Console.WriteLine($"Running {_options.Loops} inference loops...");

            var sw = Stopwatch.StartNew();

            for (int i = 0; i < _options.Loops; i++)
            {
                engine.Run(inputData);

                if (_options.TargetFps > 0)
                {
                    double targetTimePerFrameMs = 1000.0 / _options.TargetFps;
                    double expectedElapsedMs = (i + 1) * targetTimePerFrameMs;
                    double actualElapsedMs = sw.Elapsed.TotalMilliseconds;

                    if (actualElapsedMs < expectedElapsedMs)
                    {
                        int sleepMs = (int)(expectedElapsedMs - actualElapsedMs);
                        if (sleepMs > 0)
                        {
                            Thread.Sleep(sleepMs);
                        }
                    }
                }
            }

            sw.Stop();
            double totalWallTimeUs = sw.Elapsed.TotalMicroseconds;
            double actualFps = totalWallTimeUs > 0 ? (_options.Loops * 1_000_000.0) / totalWallTimeUs : 0;

            double latencyMeanMs = engine.GetLatencyMean() / 1000.0;
            double npuTimeMeanMs = engine.GetNpuInferenceTimeMean() / 1000.0;

            PrintBenchmarkResult(_options.Loops, latencyMeanMs, npuTimeMeanMs, actualFps);

            return 0;
        }

        private void PrintSingleResult(int iteration, double latencyMs, double npuTimeMs, double fps)
        {
            const int descCol = 50;
            string separator = new string('-', 80);

            Console.WriteLine(separator);
            Console.WriteLine($"* Iteration {iteration}");

            if (_options.Verbose)
            {
                string npuLine = $"  - NPU Processing Time  : {npuTimeMs:F3} ms";
                string latLine = $"  - Latency              : {latencyMs:F3} ms";
                string fpsLine = $"  - FPS                  : {fps:F2}";

                Console.WriteLine(npuLine.PadRight(descCol) + "(NPU core computation time)");
                Console.WriteLine(latLine.PadRight(descCol) + "(End-to-end time per request)");
                Console.WriteLine(fpsLine.PadRight(descCol) + "(Throughput)");
            }
            else
            {
                Console.WriteLine($"  - FPS : {fps:F2}");
            }
        }

        private void PrintBenchmarkResult(int loops, double latencyMeanMs, double npuTimeMeanMs, double fps)
        {
            const int descCol = 50;
            string separator = new string('=', 80);

            Console.WriteLine();
            Console.WriteLine(separator);
            Console.WriteLine($"* Benchmark Result ({loops} inputs)");

            if (_options.Verbose)
            {
                string npuLine = $"  - NPU Processing Time Average : {npuTimeMeanMs:F3} ms";
                string latLine = $"  - Latency Average             : {latencyMeanMs:F3} ms";
                string fpsLine = $"  - FPS                         : {fps:F2}";

                Console.WriteLine(npuLine.PadRight(descCol) + "(NPU core computation time)");
                Console.WriteLine(latLine.PadRight(descCol) + "(End-to-end time per request)");
                Console.WriteLine(fpsLine.PadRight(descCol) + "(Throughput)");
            }
            else
            {
                Console.WriteLine($"  - FPS : {fps:F2}");
            }

            Console.WriteLine(separator);
        }

        /// <summary>
        /// Parse command line arguments and create RunOptions.
        /// </summary>
        public static RunOptions ParseArguments(string[] args)
        {
            var options = new RunOptions();

            for (int i = 0; i < args.Length; i++)
            {
                switch (args[i].ToLower())
                {
                    case "-m":
                    case "--model":
                        if (i + 1 < args.Length)
                            options.ModelPath = args[++i];
                        break;
                    case "-i":
                    case "--input":
                        if (i + 1 < args.Length)
                            options.InputFile = args[++i];
                        break;
                    case "-o":
                    case "--output":
                        if (i + 1 < args.Length)
                            options.OutputFile = args[++i];
                        break;
                    case "-b":
                    case "--benchmark":
                        options.Benchmark = true;
                        break;
                    case "-s":
                    case "--single":
                        options.Single = true;
                        break;
                    case "-v":
                    case "--verbose":
                        options.Verbose = true;
                        break;
                    case "-n":
                    case "--npu":
                        if (i + 1 < args.Length && int.TryParse(args[++i], out int npu))
                            options.NpuOption = npu;
                        break;
                    case "-l":
                    case "--loops":
                        if (i + 1 < args.Length && int.TryParse(args[++i], out int loops))
                            options.Loops = loops;
                        break;
                    case "-w":
                    case "--warmup-runs":
                        if (i + 1 < args.Length && int.TryParse(args[++i], out int warmup))
                            options.WarmupRuns = warmup;
                        break;
                    case "-d":
                    case "--devices":
                        if (i + 1 < args.Length)
                            options.Devices = args[++i];
                        break;
                    case "-f":
                    case "--fps":
                        if (i + 1 < args.Length && int.TryParse(args[++i], out int fps))
                            options.TargetFps = fps;
                        break;
                    case "--skip-io":
                        options.SkipIo = true;
                        break;
                    case "--use-ort":
                        options.UseOrt = true;
                        break;
                    case "--profiler":
                        options.Profiler = true;
                        break;
                    case "--buffer-count":
                        if (i + 1 < args.Length && int.TryParse(args[++i], out int bufferCount))
                            options.BufferCount = bufferCount;
                        break;
                    case "-h":
                    case "--help":
                        PrintHelp();
                        Environment.Exit(0);
                        break;
                }
            }

            return options;
        }

        /// <summary>
        /// Print help message.
        /// </summary>
        public static void PrintHelp()
        {
            Console.WriteLine($"{AppName}");
            Console.WriteLine();
            Console.WriteLine("Usage: run_model -m <model_path> [options]");
            Console.WriteLine();
            Console.WriteLine("Options:");
            Console.WriteLine("  -m, --model <path>       Model file (.dxnn) (required)");
            Console.WriteLine("  -i, --input <file>       Input data file (optional)");
            Console.WriteLine("  -o, --output <file>      Output data file (default: output.bin)");
            Console.WriteLine("  -b, --benchmark          Perform benchmark test (default mode)");
            Console.WriteLine("  -s, --single             Perform single run test");
            Console.WriteLine("  -v, --verbose            Show NPU Processing Time and Latency");
            Console.WriteLine("  -n, --npu <option>       NPU bounding option (default: 0 for NPU_ALL)");
            Console.WriteLine("                             0: NPU_ALL, 1: NPU_0, 2: NPU_1, 3: NPU_2");
            Console.WriteLine("                             4: NPU_0/1, 5: NPU_1/2, 6: NPU_0/2");
            Console.WriteLine("  -l, --loops <count>      Number of inference loops (default: 30)");
            Console.WriteLine("  -w, --warmup-runs <n>    Number of warmup runs (default: 0)");
            Console.WriteLine("  -d, --devices <spec>     Target NPU devices (default: 'all')");
            Console.WriteLine("                             'all', '0', '0,1,2', 'count:N'");
            Console.WriteLine("  -f, --fps <target>       Target FPS for async mode");
            Console.WriteLine("  --skip-io                Skip inference I/O (benchmark only)");
            Console.WriteLine("  --use-ort                Enable ONNX Runtime for CPU tasks");
            Console.WriteLine("  --profiler               Enable profiler");
            Console.WriteLine("  --buffer-count <n>       Number of I/O buffers (default: 6)");
            Console.WriteLine("  -h, --help               Show this help message");
            Console.WriteLine();
            Console.WriteLine("Examples:");
            Console.WriteLine("  run_model -m model.dxnn");
            Console.WriteLine("  run_model -m model.dxnn -s -l 10");
            Console.WriteLine("  run_model -m model.dxnn -b -l 100 -v");
            Console.WriteLine("  run_model -m model.dxnn -f 30 -l 100");
        }

        /// <summary>
        /// Main entry point for standalone execution.
        /// </summary>
        public static int Main(string[] args)
        {
            if (args.Length == 0)
            {
                PrintHelp();
                return 1;
            }

            var options = ParseArguments(args);
            var runner = new RunModel(options);
            return runner.Execute();
        }
    }
}
