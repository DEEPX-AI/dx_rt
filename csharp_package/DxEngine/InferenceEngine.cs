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
using System.Runtime.InteropServices;

namespace DxEngine
{
    /// <summary>
    /// DXRT Inference Engine C# wrapper.
    /// This class provides an interface to load a compiled model and perform
    /// inference tasks, either synchronously or asynchronously.
    /// It supports both single and batch inference.
    /// </summary>
    public class InferenceEngine : IDisposable
    {
        private InferenceEngineNative? _nativeEngine;
        private bool _disposed = false;

        /// <summary>
        /// Initializes the InferenceEngine.
        /// </summary>
        /// <param name="modelPath">Path to the compiled model file (e.g., *.dxnn).</param>
        /// <param name="inferenceOption">Configuration options for inference. If null, default options are used.</param>
        /// <exception cref="ArgumentNullException">Thrown when modelPath is null or empty.</exception>
        /// <exception cref="DxEngineException">Thrown when engine creation fails.</exception>
        public InferenceEngine(string modelPath, InferenceOption? inferenceOption = null)
        {
            if (string.IsNullOrEmpty(modelPath))
                throw new ArgumentNullException(nameof(modelPath), "Model path must be a non-empty string.");

            try
            {
                _nativeEngine = new InferenceEngineNative(modelPath, inferenceOption);
            }
            catch (Exception ex) when (ex is not DxEngineException)
            {
                throw new DxEngineException($"Failed to create InferenceEngine for model '{modelPath}': {ex.Message}", ex);
            }
        }

        /// <summary>
        /// Performs synchronous inference with the given input data.
        /// </summary>
        /// <param name="inputData">Input tensor data as a list of arrays.</param>
        /// <returns>Output tensor data as a list of arrays.</returns>
        public List<float[]> Run(List<float[]> inputData)
        {
            ThrowIfDisposed();

            if (inputData == null || inputData.Count == 0)
                throw new ArgumentNullException(nameof(inputData), "Input data cannot be null or empty.");

            // Convert float[] to byte[]
            var byteInputs = new List<byte>();
            foreach (var input in inputData)
            {
                var bytes = new byte[input.Length * sizeof(float)];
                Buffer.BlockCopy(input, 0, bytes, 0, bytes.Length);
                byteInputs.AddRange(bytes);
            }

            var byteOutputs = _nativeEngine!.Run(byteInputs.ToArray());
            
            var result = new List<float[]>();
            foreach (var byteOutput in byteOutputs)
            {
                var floatOutput = new float[byteOutput.Length / sizeof(float)];
                Buffer.BlockCopy(byteOutput, 0, floatOutput, 0, byteOutput.Length);
                result.Add(floatOutput);
            }

            return result;
        }

        /// <summary>
        /// Performs synchronous inference with a single input tensor.
        /// </summary>
        /// <param name="inputData">Input tensor data as a single array.</param>
        /// <returns>Output tensor data as a list of arrays.</returns>
        public List<float[]> Run(float[] inputData)
        {
            return Run(new List<float[]> { inputData });
        }

        /// <summary>
        /// Performs asynchronous inference with the given input data.
        /// </summary>
        /// <param name="inputData">Input tensor data as a list of arrays.</param>
        /// <returns>A task representing the asynchronous operation with output tensor data.</returns>
        public async Task<List<float[]>> RunAsync(List<float[]> inputData)
        {
            return await Task.Run(() => Run(inputData));
        }

        /// <summary>
        /// Performs batch inference with multiple input sets.
        /// </summary>
        /// <param name="batchInputData">List of input tensor sets for batch processing.</param>
        /// <returns>List of output tensor sets corresponding to each input.</returns>
        public List<List<float[]>> RunBatch(List<List<float[]>> batchInputData)
        {
            ThrowIfDisposed();

            if (batchInputData == null || batchInputData.Count == 0)
                throw new ArgumentNullException(nameof(batchInputData), "Batch input data cannot be null or empty.");

            var results = new List<List<float[]>>();
            foreach (var inputData in batchInputData)
            {
                results.Add(Run(inputData));
            }
            return results;
        }

        /// <summary>
        /// Gets information about input tensors.
        /// </summary>
        public List<TensorInfo> InputTensorInfo
        {
            get
            {
                ThrowIfDisposed();
                return _nativeEngine!.InputTensorInfo;
            }
        }

        /// <summary>
        /// Gets information about output tensors.
        /// </summary>
        public List<TensorInfo> OutputTensorInfo
        {
            get
            {
                ThrowIfDisposed();
                return _nativeEngine!.OutputTensorInfo;
            }
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
        /// Gets the total size of all input tensors in bytes.
        /// </summary>
        /// <returns>Total input size in bytes.</returns>
        public ulong GetInputSize()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetInputSize();
        }

        /// <summary>
        /// Gets the total size of all output tensors in bytes.
        /// </summary>
        /// <returns>Total output size in bytes.</returns>
        public ulong GetOutputSize()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetOutputSize();
        }

        /// <summary>
        /// Gets the latency of the last inference in microseconds.
        /// </summary>
        /// <returns>Latency in microseconds.</returns>
        public int GetLatency()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetLatency();
        }

        /// <summary>
        /// Gets the NPU inference time of the last inference in microseconds.
        /// </summary>
        /// <returns>NPU inference time in microseconds.</returns>
        public uint GetNpuInferenceTime()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetNpuInferenceTime();
        }

        /// <summary>
        /// Gets the mean latency across multiple inferences in microseconds.
        /// </summary>
        /// <returns>Mean latency in microseconds.</returns>
        public double GetLatencyMean()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetLatencyMean();
        }

        /// <summary>
        /// Gets the mean NPU inference time across multiple inferences in microseconds.
        /// </summary>
        /// <returns>Mean NPU inference time in microseconds.</returns>
        public double GetNpuInferenceTimeMean()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetNpuInferenceTimeMean();
        }

        /// <summary>
        /// Gets the model name.
        /// </summary>
        /// <returns>Model name string.</returns>
        public string GetModelName()
        {
            ThrowIfDisposed();
            return _nativeEngine!.GetModelName();
        }

        /// <summary>
        /// Runs a benchmark test with the specified number of loops.
        /// </summary>
        /// <param name="loops">Number of inference loops to perform.</param>
        /// <param name="inputData">Input tensor data (optional, if null random data will be used).</param>
        /// <returns>Measured FPS (frames per second).</returns>
        public double RunBenchmark(int loops, List<float[]>? inputData = null)
        {
            ThrowIfDisposed();

            byte[]? byteInput = null;
            if (inputData != null && inputData.Count > 0)
            {
                var byteInputs = new List<byte>();
                foreach (var input in inputData)
                {
                    var bytes = new byte[input.Length * sizeof(float)];
                    Buffer.BlockCopy(input, 0, bytes, 0, bytes.Length);
                    byteInputs.AddRange(bytes);
                }
                byteInput = byteInputs.ToArray();
            }

            return _nativeEngine!.RunBenchmark(loops, byteInput);
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(InferenceEngine));
        }

        /// <summary>
        /// Releases all resources used by the InferenceEngine.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Releases the unmanaged resources used by the InferenceEngine.
        /// </summary>
        /// <param name="disposing">True if called from Dispose(), false if called from finalizer.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    _nativeEngine?.Dispose();
                    _nativeEngine = null;
                }
                _disposed = true;
            }
        }

        /// <summary>
        /// Finalizer for InferenceEngine.
        /// </summary>
        ~InferenceEngine()
        {
            Dispose(false);
        }
    }
}
