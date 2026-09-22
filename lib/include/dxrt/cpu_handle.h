/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 *
 * This file uses ONNX Runtime (MIT License) - Copyright (c) Microsoft Corporation.
 */

#pragma once

#include <vector>
#include <atomic>
#include <unordered_map>
#include <mutex>
#include <memory>

#include "dxrt/common.h"
#include "dxrt/datatype.h"

#ifdef USE_ORT
#include <onnxruntime_cxx_api.h>
#endif

namespace dxrt {
class Buffer;
class CpuHandleWorker;
class Request;
using RequestPtr = std::shared_ptr<Request>;


// TODO: Refactor CpuHandle to reduce field count (currently 31). Consider extracting
//       ORT session config, I/O metadata, and dynamic shape info into separate classes.
class DXRT_API CpuHandle // NOSONAR: Too many fields - stable as-is, refactoring deferred
{

 public:

    CpuHandle(const void* data_, int64_t size_, const std::string& name_, size_t device_num_, int buffer_count_); // NOSONAR
    ~CpuHandle();

    CpuHandle(const CpuHandle&) = delete;
    CpuHandle& operator=(const CpuHandle&) = delete;
    CpuHandle(CpuHandle&&) = delete;
    CpuHandle& operator=(CpuHandle&&) = delete;

#ifdef USE_ORT
    Ort::Env _env;
    Ort::SessionOptions _sessionOptions;
    std::shared_ptr<Ort::Session> _session;

    // For DYNAMIC THREAD: store model data for worker session creation
    std::vector<uint8_t> _modelData;
    int64_t _modelSize;

    // ORT built-in profiling: file prefix passed to EnableProfiling. Non-empty
    // when DXRT_ORT_PROFILING=ON. Used at destruction to flush via
    // Session::EndProfiling.
    std::string _ortProfilePrefix;

    // Create individual session for worker (DYNAMIC THREAD mode)
    std::shared_ptr<Ort::Session> CreateWorkerSession();
    void RunWithSession(RequestPtr req, std::shared_ptr<Ort::Session> session);

    // One-time warm-up run of `session` with zero-filled dummy inputs. Intended
    // to be called a few times (e.g. 3x) by a CpuHandleWorker thread right
    // after it starts, before it begins servicing real requests from the
    // queue. This pre-pays ONNX Runtime's CPU memory arena growth (which, per
    // measurement, ramps up over the first ~2 real Run() calls before
    // stabilizing) and intra/inter-op thread pool spin-up cost during worker
    // start-up instead of on real jobs. Safe to call multiple times and safe
    // to call with a session shared across multiple worker threads. Skips
    // itself (no-op) if any input has a dynamic (non-fixed) shape. Never
    // throws: any failure is logged and swallowed so it can never prevent a
    // worker thread from servicing real requests.
    void WarmupSession(std::shared_ptr<Ort::Session> session);

#endif


    static std::atomic<int> _totalNumThreads;
    static bool _dynamicCpuThread;
    static void SetDynamicCpuThread();


    uint32_t _inputSize = 0;
    uint32_t _outputSize = 0;
    uint32_t _outputMemSize = 0;
    std::string _name;
    size_t _device_num = 1;
    std::vector<DataType> _inputDataTypes;
    std::vector<DataType> _outputDataTypes;
    int _numInputs = 1;
    int _numOutputs;
    int _numThreads = 1;
    int _initDynamicThreads = 0;
    std::vector<std::string> _inputNames;
    std::vector<const char*> _inputNamesChar;
    std::vector<std::string> _outputNames;
    std::vector<const char*> _outputNamesChar;
    std::vector<std::vector<int64_t>> _inputShapes;
    std::vector<std::vector<int64_t>> _outputShapes;
    std::vector<uint64_t> _inputOffsets = {0};
    std::vector<uint64_t> _outputOffsets;
    std::vector<uint64_t> _inputSizes;
    std::vector<uint64_t> _outputSizes;

    // Dynamic shape output support
    std::vector<bool> _outputIsDynamic;  // Track which outputs have dynamic shapes
    bool _hasDynamicOutput = false;      // Flag if any output is dynamic

    void* _cpuTaskOutputBufferPtr;
    std::shared_ptr<CpuHandleWorker> _worker=nullptr;

    int InferenceRequest(RequestPtr req) const;
    void Start();
    void Run(RequestPtr req);
    void Terminate(void) const;

    // Dynamic output management functions
    bool DetectDynamicShape(const std::vector<int64_t>& shape) const;
#ifdef USE_ORT
    void SetupOutputsWithBinding(RequestPtr req, Ort::IoBinding& binding);
    void UpdateRequestOutputsFromBinding(RequestPtr req, std::vector<Ort::Value> ortOutputs);
#endif

    // Getter for dynamic output status
    bool HasDynamicOutput() const { return _hasDynamicOutput; }

    friend DXRT_API std::ostream& operator<<(std::ostream&, const CpuHandle&);

private:
    int _bufferCount = DXRT_TASK_MAX_LOAD_VALUE;

};
} /* namespace dxrt */
