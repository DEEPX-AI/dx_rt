# DxEngine NuGet Package Guide

## Overview

DxEngine is a C# wrapper library for the DEEPX NPU Inference Engine (DX-RT).
It allows you to easily perform NPU inference in .NET 8.0 projects via NuGet package.

## System Requirements

- Windows x64
- .NET 8.0 SDK or later
- DEEPX NPU device and driver

## Installation

### Method 1: Using a Local .nupkg File

1. **Register a local NuGet source**

   ```bash
   dotnet nuget add source C:\path\to\nupkg\folder --name DxEngine-Local
   ```

2. **Add the package to your project**

   ```bash
   dotnet add package DxEngine --version 3.3.0 --source DxEngine-Local
   ```

### Method 2: Register a Local Source in nuget.config

Create or edit a `nuget.config` file at the project root:

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <packageSources>
    <add key="nuget.org" value="https://api.nuget.org/v3/index.json" />
    <add key="DxEngine-Local" value="C:\path\to\nupkg\folder" />
  </packageSources>
</configuration>
```

Then add the package:

```bash
dotnet add package DxEngine
```

### Method 3: Add Directly to .csproj

```xml
<ItemGroup>
  <PackageReference Include="DxEngine" Version="3.3.0" />
</ItemGroup>
```

## Quick Start

```csharp
using DxEngine;

// Load model and create engine
using var engine = new InferenceEngine("model.dxnn");

// Check input tensor info
var inputInfo = engine.InputTensorInfo;
Console.WriteLine($"Input: {inputInfo.Name}, Shape: [{string.Join(", ", inputInfo.Shape)}]");

// Prepare input data (e.g., image byte array)
byte[] inputData = File.ReadAllBytes("input.bin");

// Run inference
byte[][] outputs = engine.Run(inputData);

// Use output
Console.WriteLine($"Output size: {outputs[0].Length} bytes");
```

## Key APIs

### InferenceEngine

```csharp
// Basic creation
var engine = new InferenceEngine("model.dxnn");

// Creation with options
var option = new InferenceOption
{
    UseOrt = false,          // Whether to use ONNX Runtime
    BoundOption = BoundOption.NpuAll,  // NPU core binding
    BufferCount = 4          // Number of inference buffers
};
var engine = new InferenceEngine("model.dxnn", option);

// Synchronous inference
byte[][] outputs = engine.Run(inputData);

// Asynchronous inference
byte[][] outputs = await engine.RunAsync(inputData);

// Benchmark (returns FPS)
float fps = engine.RunBenchmark(iterations: 100);

// Check latency (microseconds)
int latency = engine.GetLatency();
double avgLatency = engine.GetLatencyMean();
```

### DeviceStatus

```csharp
// Get device count
int count = DeviceStatus.GetDeviceCount();

// Query device status
var status = DeviceStatus.GetCurrentStatus(deviceId: 0);
Console.WriteLine($"Temperature: {status.GetTemperature()}°C");
```

### Configuration

```csharp
// Load configuration file
Configuration.Instance.LoadConfigFile("dxrt.cfg");

// Check version info
string version = Configuration.GetVersion();
string driverVersion = Configuration.GetDriverVersion();
```

## Package Structure

The NuGet package includes the following native DLLs, which are automatically copied to the output directory at build time:

| File | Description |
|------|-------------|
| `DxEngineNative.dll` | C++/CLI bridge |
| `Ijwhost.dll` | .NET C++/CLI host |
| `dxrt.dll` | DX-RT native runtime |
| `onnxruntime.dll` | ONNX Runtime |
| `onnxruntime_providers_shared.dll` | ONNX Runtime provider |

## Troubleshooting

### DLL Not Found

Verify that the native DLLs have been copied to the output directory:

```bash
ls bin\Release\net8.0\dxrt.dll
```

If not copied, perform a clean build:

```bash
dotnet clean
dotnet build
```

### NPU Device Not Found

1. Verify the DEEPX NPU driver is installed
2. Check Device Manager for NPU device recognition
3. Confirm `DeviceStatus.GetDeviceCount()` returns greater than 0

## License

Copyright (C) 2018- DEEPX Ltd. All rights reserved.
