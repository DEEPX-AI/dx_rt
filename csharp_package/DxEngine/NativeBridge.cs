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
using System.Reflection;
using System.Runtime.InteropServices;

namespace DxEngine
{
    /// <summary>
    /// Native bridge that connects to the C++/CLI wrapper (DxEngineNative.dll).
    /// This class provides the actual implementation using native dxrt library.
    /// </summary>
    public static class NativeBridge
    {
        private static bool _isAvailable;
        private static string? _loadError;
        private static Assembly? _nativeAssembly;

        static NativeBridge()
        {
            try
            {
                // For C++/CLI mixed mode assembly, we need to load it from the file path
                string? assemblyLocation = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                if (assemblyLocation != null)
                {
                    string nativeAssemblyPath = Path.Combine(assemblyLocation, "DxEngineNative.dll");
                    if (File.Exists(nativeAssemblyPath))
                    {
                        // Load the C++/CLI assembly
                        _nativeAssembly = Assembly.LoadFrom(nativeAssemblyPath);
                        _isAvailable = _nativeAssembly != null;
                    }
                    else
                    {
                        _loadError = $"DxEngineNative.dll not found at {nativeAssemblyPath}";
                        _isAvailable = false;
                    }
                }
                else
                {
                    _loadError = "Could not determine assembly location";
                    _isAvailable = false;
                }
            }
            catch (Exception ex)
            {
                _isAvailable = false;
                _loadError = ex.Message;
            }
        }

        /// <summary>
        /// Gets whether the native bridge is available.
        /// </summary>
        public static bool IsAvailable => _isAvailable;

        /// <summary>
        /// Gets the error message if the native bridge is not available.
        /// </summary>
        public static string? LoadError => _loadError;

        /// <summary>
        /// Gets the loaded native assembly.
        /// </summary>
        internal static Assembly? NativeAssembly => _nativeAssembly;

        /// <summary>
        /// Throws an exception if the native bridge is not available.
        /// </summary>
        public static void ThrowIfNotAvailable()
        {
            if (!_isAvailable)
            {
                throw new DxEngineException(
                    $"Native DxEngine library is not available. " +
                    $"Ensure DxEngineNative.dll and dxrt.dll are in the application directory. " +
                    $"Error: {_loadError ?? "Unknown"}");
            }
        }

        /// <summary>
        /// Parse a model file and display information.
        /// </summary>
        /// <param name="modelPath">Path to the model file</param>
        /// <param name="verbose">Show verbose output</param>
        /// <param name="jsonExtract">Extract JSON data to files</param>
        /// <param name="outputFile">Output file path (optional)</param>
        /// <returns>0 on success, -1 on failure</returns>
        public static int ParseModel(string modelPath, bool verbose = false, bool jsonExtract = false, string? outputFile = null)
        {
            ThrowIfNotAvailable();

            try
            {
                var utilityType = NativeAssembly?.GetType("DxEngineNative.DxUtility");
                if (utilityType == null)
                    throw new InvalidOperationException("DxUtility type not found");

                var parseMethod = utilityType.GetMethod("ParseModel");
                if (parseMethod == null)
                    throw new InvalidOperationException("ParseModel method not found");

                return (int)parseMethod.Invoke(null, new object?[] { modelPath, verbose, jsonExtract, outputFile ?? "" })!;
            }
            catch (Exception ex)
            {
                var innerMsg = ex.InnerException?.Message ?? ex.Message;
                throw new DxEngineException($"Failed to parse model: {innerMsg}", ex);
            }
        }

        /// <summary>
        /// Get the number of available NPU devices.
        /// </summary>
        public static int GetDeviceCount()
        {
            ThrowIfNotAvailable();

            try
            {
                var utilityType = NativeAssembly?.GetType("DxEngineNative.DxUtility");
                if (utilityType == null)
                    throw new InvalidOperationException("DxUtility type not found");

                var method = utilityType.GetMethod("GetDeviceCount");
                if (method == null)
                    throw new InvalidOperationException("GetDeviceCount method not found");

                return (int)method.Invoke(null, null)!;
            }
            catch (Exception ex)
            {
                var innerMsg = ex.InnerException?.Message ?? ex.Message;
                throw new DxEngineException($"Failed to get device count: {innerMsg}", ex);
            }
        }

        /// <summary>
        /// Get the DX-RT version string.
        /// </summary>
        public static string GetVersion()
        {
            ThrowIfNotAvailable();

            try
            {
                var utilityType = NativeAssembly?.GetType("DxEngineNative.DxUtility");
                if (utilityType == null)
                    throw new InvalidOperationException("DxUtility type not found");

                var method = utilityType.GetMethod("GetVersion");
                if (method == null)
                    throw new InvalidOperationException("GetVersion method not found");

                return (string)method.Invoke(null, null)!;
            }
            catch (Exception ex)
            {
                var innerMsg = ex.InnerException?.Message ?? ex.Message;
                throw new DxEngineException($"Failed to get version: {innerMsg}", ex);
            }
        }
    }

    /// <summary>
    /// Inference engine implementation using C++/CLI native wrapper.
    /// This class wraps DxEngineNative.InferenceEngineWrapper.
    /// </summary>
    public class InferenceEngineNative : IDisposable
    {
        private dynamic? _nativeEngine;
        private dynamic? _nativeOption;
        private bool _disposed = false;
        private List<TensorInfo>? _inputTensorInfoCache;
        private List<TensorInfo>? _outputTensorInfoCache;

        /// <summary>
        /// Creates an InferenceEngine from model path using native implementation.
        /// </summary>
        public InferenceEngineNative(string modelPath, InferenceOption? option = null)
        {
            NativeBridge.ThrowIfNotAvailable();

            try
            {
                // Load native wrapper types dynamically from the pre-loaded assembly
                var nativeAssembly = NativeBridge.NativeAssembly;
                if (nativeAssembly == null)
                    throw new InvalidOperationException("Native assembly not loaded");

                var optionWrapperType = nativeAssembly.GetType("DxEngineNative.InferenceOptionWrapper");
                var engineWrapperType = nativeAssembly.GetType("DxEngineNative.InferenceEngineWrapper");

                if (option != null && optionWrapperType != null)
                {
                    _nativeOption = Activator.CreateInstance(optionWrapperType);
                    
                    // Set properties using reflection to avoid dynamic type issues
                    var useOrtProp = optionWrapperType.GetProperty("UseOrt");
                    var boundOptProp = optionWrapperType.GetProperty("BoundOpt");
                    var bufferCountProp = optionWrapperType.GetProperty("BufferCount");
                    var devicesProp = optionWrapperType.GetProperty("Devices");

                    useOrtProp?.SetValue(_nativeOption, option.UseOrt);
                    boundOptProp?.SetValue(_nativeOption, (int)option.BoundOption);
                    bufferCountProp?.SetValue(_nativeOption, option.BufferCount);
                    
                    if (option.Devices.Count > 0)
                    {
                        var devicesList = new List<int>(option.Devices);
                        devicesProp?.SetValue(_nativeOption, devicesList);
                    }

                    _nativeEngine = Activator.CreateInstance(engineWrapperType, modelPath, _nativeOption);
                }
                else
                {
                    _nativeEngine = Activator.CreateInstance(engineWrapperType, modelPath);
                }
            }
            catch (Exception ex)
            {
                var innerMsg = ex.InnerException?.Message ?? ex.Message;
                var innerInnerMsg = ex.InnerException?.InnerException?.Message ?? "";
                throw new DxEngineException($"Failed to create native InferenceEngine: {innerMsg} {innerInnerMsg}", ex);
            }
        }

        /// <summary>
        /// Run synchronous inference.
        /// </summary>
        public List<byte[]> Run(byte[] inputData)
        {
            ThrowIfDisposed();
            
            var outputs = _nativeEngine!.Run(inputData);
            var result = new List<byte[]>();
            
            foreach (var output in outputs)
            {
                result.Add((byte[])output);
            }
            
            return result;
        }

        /// <summary>
        /// Run benchmark.
        /// </summary>
        public float RunBenchmark(int loops, byte[]? inputData = null)
        {
            ThrowIfDisposed();
            return _nativeEngine!.RunBenchmark(loops, inputData);
        }

        /// <summary>
        /// Gets the number of input tensors.
        /// </summary>
        public int InputCount
        {
            get
            {
                ThrowIfDisposed();
                return _nativeEngine!.InputCount;
            }
        }

        /// <summary>
        /// Gets the number of output tensors.
        /// </summary>
        public int OutputCount
        {
            get
            {
                ThrowIfDisposed();
                return _nativeEngine!.OutputCount;
            }
        }

        /// <summary>
        /// Gets the total input size in bytes.
        /// </summary>
        public ulong GetInputSize()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetInputSize();
        }

        /// <summary>
        /// Gets the total output size in bytes.
        /// </summary>
        public ulong GetOutputSize()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetOutputSize();
        }

        /// <summary>
        /// Gets the model name.
        /// </summary>
        public string GetModelName()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetModelName();
        }

        /// <summary>
        /// Gets the latest latency in microseconds.
        /// </summary>
        public int GetLatency()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetLatency();
        }

        /// <summary>
        /// Gets the NPU inference time in microseconds.
        /// </summary>
        public uint GetNpuInferenceTime()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetNpuInferenceTime();
        }

        /// <summary>
        /// Gets the mean latency.
        /// </summary>
        public double GetLatencyMean()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetLatencyMean();
        }

        /// <summary>
        /// Gets the mean NPU inference time.
        /// </summary>
        public double GetNpuInferenceTimeMean()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetNpuInferenceTimeMean();
        }

        /// <summary>
        /// Gets input tensor information.
        /// </summary>
        public List<TensorInfo> InputTensorInfo
        {
            get
            {
                ThrowIfDisposed();
                if (_inputTensorInfoCache == null)
                {
                    _inputTensorInfoCache = ConvertTensorInfo(_nativeEngine!.GetInputTensorInfo());
                }
                return _inputTensorInfoCache;
            }
        }

        /// <summary>
        /// Gets output tensor information.
        /// </summary>
        public List<TensorInfo> OutputTensorInfo
        {
            get
            {
                ThrowIfDisposed();
                if (_outputTensorInfoCache == null)
                {
                    _outputTensorInfoCache = ConvertTensorInfo(_nativeEngine!.GetOutputTensorInfo());
                }
                return _outputTensorInfoCache;
            }
        }

        private List<TensorInfo> ConvertTensorInfo(dynamic nativeInfoList)
        {
            var result = new List<TensorInfo>();
            foreach (var nativeInfo in nativeInfoList)
            {
                var info = new TensorInfo
                {
                    Index = nativeInfo.Index,
                    Name = nativeInfo.Name,
                    Shape = (int[])nativeInfo.Shape,
                    DataType = DataType.Float32 // Default, could be parsed from nativeInfo.DataType
                };
                result.Add(info);
            }
            return result;
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(InferenceEngineNative));
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    if (_nativeEngine != null)
                    {
                        (_nativeEngine as IDisposable)?.Dispose();
                        _nativeEngine = null;
                    }
                    if (_nativeOption != null)
                    {
                        (_nativeOption as IDisposable)?.Dispose();
                        _nativeOption = null;
                    }
                }
                _disposed = true;
            }
        }

        ~InferenceEngineNative()
        {
            Dispose(false);
        }
    }
}
