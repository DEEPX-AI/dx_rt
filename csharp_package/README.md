# DxEngine - C# Wrapper for DX-RT

C# wrapper class for dx_rt - DEEPX NPU Inference Engine

## Installation

### NuGet Package

```bash
dotnet add package DxEngine
```

### Building from Source

```bash
cd csharp_package/DxEngine
dotnet build
```

## Quick Start

```csharp
using DxEngine;

// Create inference engine with model
using var engine = new InferenceEngine("model.dxnn");

// Prepare input data
var inputData = new List<float[]> { new float[] { 1.0f, 2.0f, 3.0f } };

// Run inference
var outputs = engine.Run(inputData);

// Process outputs
foreach (var output in outputs)
{
    Console.WriteLine($"Output: {string.Join(", ", output)}");
}
```

## Classes

### InferenceEngine

Main class for running inference on DEEPX NPU.

```csharp
// Create with model path
using var engine = new InferenceEngine("model.dxnn");

// Create with options
var option = new InferenceOption { UseOrt = false };
using var engine = new InferenceEngine("model.dxnn", option);

// Create from memory buffer
byte[] modelData = File.ReadAllBytes("model.dxnn");
using var engine = InferenceEngine.FromBuffer(modelData);

// Run inference
var outputs = engine.Run(inputData);

// Async inference
var outputs = await engine.RunAsync(inputData);

// Batch inference
var batchOutputs = engine.RunBatch(batchInputData);
```

### InferenceOption

Configuration options for the inference engine.

```csharp
var option = new InferenceOption
{
    UseOrt = false,
    BoundOption = BoundOption.NpuAll,
    Devices = new List<int> { 0, 1 },
    BufferCount = 2
};
```

### DeviceStatus

Access device information and status.

```csharp
int deviceCount = DeviceStatus.GetDeviceCount();

using var status = DeviceStatus.GetCurrentStatus(0);
int temperature = status.GetTemperature(0);
int voltage = status.GetNpuVoltage(0);
int clock = status.GetNpuClock(0);
```

### Configuration

System-wide configuration management (singleton).

```csharp
var config = Configuration.Instance;

// Load configuration file
config.LoadConfigFile("config.json");

// Enable/disable features
config.SetEnable(ConfigurationItem.Profiler, true);

// Get version information
string version = config.GetVersion();
string driverVersion = config.GetDriverVersion();
```

### RuntimeEventDispatcher

Event handling and dispatching (singleton).

```csharp
var dispatcher = RuntimeEventDispatcher.Instance;

// Subscribe to events
dispatcher.EventReceived += (sender, e) =>
{
    Console.WriteLine($"[{e.Level}] {e.Message}");
};

// Register the handler
dispatcher.RegisterEventHandler();

// Set event filtering level
dispatcher.SetCurrentLevel(EventLevel.Warning);
```

## CLI Tools

The package includes command-line tools for model operations.

### Building CLI

```bash
cd csharp_package/cli
dotnet build
```

### parse_model

Parse and display model information.

```bash
# Basic usage
dxcli parse -m model.dxnn

# With verbose output
dxcli parse -m model.dxnn -v

# Save output to file
dxcli parse -m model.dxnn -o output.txt

# Extract JSON data
dxcli parse -m model.dxnn -j
```

### run_model

Run model inference with benchmarking capabilities.

```bash
# Basic benchmark (default mode)
dxcli run -m model.dxnn

# Single inference mode
dxcli run -m model.dxnn -s

# Benchmark with specific loops
dxcli run -m model.dxnn -l 100 -v

# Target FPS mode
dxcli run -m model.dxnn -f 30 -l 100

# With NPU binding option
dxcli run -m model.dxnn -n 1

# With input file
dxcli run -m model.dxnn -i input.bin -o output.bin
```

#### Options

| Option | Description |
|--------|-------------|
| `-m, --model` | Model file path (.dxnn) [required] |
| `-i, --input` | Input data file |
| `-o, --output` | Output data file (default: output.bin) |
| `-b, --benchmark` | Perform benchmark test (default mode) |
| `-s, --single` | Perform single run test |
| `-v, --verbose` | Show NPU Processing Time and Latency |
| `-n, --npu` | NPU bounding option (0=ALL, 1-6=specific) |
| `-l, --loops` | Number of inference loops (default: 30) |
| `-w, --warmup-runs` | Number of warmup runs (default: 0) |
| `-d, --devices` | Target NPU devices ('all', '0,1,2', 'count:N') |
| `-f, --fps` | Target FPS for async mode |
| `--skip-io` | Skip inference I/O (benchmark only) |
| `--use-ort` | Enable ONNX Runtime for CPU tasks |
| `--profiler` | Enable profiler |
| `--buffer-count` | Number of I/O buffers (default: 6) |

## Requirements

- .NET 8.0 or later
- DEEPX NPU hardware and drivers
- Native dxrt library

## License

Copyright (C) 2018- DEEPX Ltd. All rights reserved.

This software is the property of DEEPX and is provided exclusively to customers
who are supplied with DEEPX NPU (Neural Processing Unit).
Unauthorized sharing or usage is strictly prohibited by law.
