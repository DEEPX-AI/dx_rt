/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"

#include <string>
#include <iostream>
// #include <fstream>
#include <cassert>
#include <map>
#include <memory>
#include <mutex>
#include <functional>
#include <vector>


#include "dxrt/model.h"
// #include "dxrt/inference_option.h"
#include "dxrt/tensor.h"
#include "dxrt/inference_option.h"
// #include "dxrt/testdata.h"
#include "dxrt/inference_job.h"
#include "dxrt/inference_timer.h"



#define NPU_PARAM_FILE "rmap.info"


/**
 * @mainpage DX Runtime API
 *
 * @subsection intro_sec Introduction
 * This is the main page of the documentation.
 * The DX Runtime API is an API for developing applications using the DX NPU Core.
 *
 */

namespace dxrt {
using rmap_info = deepx_rmapinfo::RegisterInfoDatabase;
class Task;
struct TimePoint;


/** @brief This class abstracts the runtime inference executor for a user's compiled model.
 * @details After a model is loaded, real-time device tasks are scheduled by internal runtime libraries.
 *          It supports both synchronous and asynchronous inference modes.
 * @code
 * // Use default inference option
 * auto modelPath = "model.dxnn"; // assume compiled model path name is "model.dxnn"
 * auto ie = dxrt::InferenceEngine(modelPath, nullptr);
 * @endcode
 * @code
 * // Use a new inference option
 * auto modelPath = "model.dxnn"; // assume compiled model path name is "model.dxnn"
 * dxrt::InferenceOption option;
 * option.devices = {0,1,3};  //use only 0,1,3 device
 * auto ie = dxrt::InferenceEngine(modelPath, option);
 * @endcode
 * @headerfile "dxrt/dxrt_api.h"
*/
class DXRT_API InferenceEngine
{
    // static
 public:
        static constexpr int INFERENCE_JOB_MAX_COUNT = 1024;  // max job count

 public:
    /** @brief Loads a model from the specified path and configures the NPU to run it.
     * @param[in] modelPath The file path to the compiled model (e.g., model.dxnn).
     * @param[in] option A reference to an InferenceOption object to configure devices and NPU cores.
     * @code
     * auto modelPath = "model.dxnn"; // assume compiled model path name is "model.dxnn"
     * dxrt::InferenceEngine ie(modelPath);
     * auto outputs = ie.Run();
     *
     * dxrt::InferenceOption op;
     * op.devices.push_back(0);
     * op.boundOption = dxrt::InferenceOption::BOUND_OPTION::NPU_0; // NPU_0 only
     * dxrt::InferenceEngine ie(modelPath, op);
     * auto outputs = ie.Run();
     * @endcode
     */
    explicit InferenceEngine(const std::string &modelPath, InferenceOption &option = DefaultInferenceOption);
    ~InferenceEngine(void);

    /** @brief Performs a synchronous inference for a single input, blocking until the operation is complete.
     * @param[in] inputPtr A pointer to the input data.
     * @param[in] userArg An optional user-defined argument.
     * @param[out] outputPtr An optional pointer to a pre-allocated output buffer.
     * @code
     * auto modelPath = "model.dxnn"; // assume compiled model path name is "model.dxnn"
     * dxrt::InferenceEngine ie(modelPath);
     * auto outputs = ie.Run();
     * @endcode
     * @return A TensorPtrs object containing the output data.
     */
    TensorPtrs Run(void *inputPtr, void *userArg = nullptr, void *outputPtr = nullptr);

    /**
     * @brief Performs a synchronous batch inference.
     * @param[in] inputBuffers A vector of pointers to input data for each sample in the batch.
     * @param[out] outputBuffers A vector of pointers to pre-allocated output buffers.
     * @param[in] userArgs An optional vector of user-defined arguments.
     * @return A vector of TensorPtrs, where each element corresponds to the output of one sample.
     */
    std::vector<TensorPtrs> Run(
        const std::vector<void*>& inputBuffers,
        const std::vector<void*>& outputBuffers,
        const std::vector<void*>& userArgs = {}
    );


    /** @brief Submits a non-blocking, asynchronous inference request for a single input.
     * @param[in] inputPtr A pointer to the input data.
     * @param[in] userArg An optional user-defined argument to be passed to the callback.
     * @param[out] outputPtr An optional pointer to a pre-allocated output buffer.
     * @code
     * auto modelPath = "model"; // assume compiled model path name is "model"
     * dxrt::InferenceOption option;
     * option.devices = {0,1,3};  //use only 0,1,3 device
     * dxrt::InferenceEngine ie(modelPath, option);
     * auto outputs = ie.Run();
     * @endcode
     * @return An integer jobId for this asynchronous operation.
     */
    int RunAsync(void *inputPtr, void *userArg=nullptr, void *outputPtr = nullptr);

    /** @brief Submits an asynchronous inference request, automatically detecting if the input is for a multi-input model.
     * @param[in] inputPtrs A vector of pointers to input data.
     * @param[in] userArg An optional user-defined argument.
     * @param[out] outputPtr An optional pointer to a pre-allocated output buffer.
     * @return An integer jobId.
     */
    int RunAsync(const std::vector<void*>& inputPtrs, void *userArg=nullptr, void *outputPtr = nullptr);

    /** @brief Submits an asynchronous inference request for a multi-input model using a map of named tensors.
     * @param[in] inputTensors A map of tensor names to input data pointers.
     * @param[in] userArg An optional user-defined argument.
     * @param[out] outputPtr An optional pointer to a pre-allocated output buffer.
     * @return An integer jobId.
     */
    int RunAsyncMultiInput(const std::map<std::string, void*>& inputTensors, void *userArg=nullptr, void *outputPtr = nullptr);

    /** @brief Submits an asynchronous inference request for a multi-input model using a vector of input pointers.
     * @param[in] inputPtrs A vector of input pointers in the order specified by GetInputTensorNames().
     * @param[in] userArg An optional user-defined argument.
     * @param[out] outputPtr An optional pointer to a pre-allocated output buffer.
     * @return An integer jobId.
     */
    int RunAsyncMultiInput(const std::vector<void*>& inputPtrs, void *userArg=nullptr, void *outputPtr = nullptr);

    /**
     * @deprecated Use RunBenchmark() instead.
     * @brief run benchmark with loop n times (Legacy API)
     * @param[in] num number of inferences
     * @param[in] inputPtr input data pointer to run inference
     * @return average fps
     */
    [[deprecated("Use RunBenchmark() instead")]]
    float RunBenchMark(int num, void* inputPtr = nullptr) { return RunBenchmark(num, inputPtr); }

    /** @brief Runs a performance benchmark for a specified number of loops.
     * @param[in] num The number of inference iterations to run.
     * @param[in] inputPtr An optional pointer to the input data to use for the benchmark.
     * @return The average frames per second (FPS) as a float.
     */
    float RunBenchmark(int num, void* inputPtr = nullptr);

    /**
     * @brief Validate inference of a specific NPU device connected to the host.
     * This function runs a validation process using the provided input data on the specified NPU device.
     * It can be used to ensure that the NPU device is operational and can process inference tasks correctly.
     *
     * @param[in] inputPtr Pointer to the input data used for validation.
     * @param[in] deviceId ID of the NPU device to validate. Default is 0 (first device).
     * @return Output tensors as a vector of smart pointer instances, representing the validation results.
     * @warning This function is intended for validation purposes only and may not be optimized for performance.
     */
    TensorPtrs ValidateDevice(void *inputPtr, int deviceId = 0);

    /**
     * @brief Validate inference of a specific NPU device with automatic multi-input detection.
     * This function automatically detects whether the input should be interpreted as multi-input
     * based on the model requirements and input count.
     *
     * @param[in] inputPtrs Vector of input data pointers for validation.
     * @param[in] deviceId ID of the NPU device to validate. Default is 0 (first device).
     * @return Output tensors as a vector of smart pointer instances, representing the validation results.
     * @warning This function is intended for validation purposes only and may not be optimized for performance.
     */
    TensorPtrs ValidateDevice(const std::vector<void*>& inputPtrs, int deviceId = 0);

    /** @brief Validate NPU device with multiple input tensors for multi-input models
     * @param[in] inputTensors Map of tensor name to input data pointer
     * @param[in] deviceId ID of the NPU device to validate. Default is 0 (first device).
     * @return Output tensors as a vector of smart pointer instances, representing the validation results.
     * @warning This function is intended for validation purposes only and may not be optimized for performance.
     */
    TensorPtrs ValidateDeviceMultiInput(const std::map<std::string, void*>& inputTensors, int deviceId = 0);

    /** @brief Validate NPU device with multiple input tensors (vector format) for multi-input models
     * @param[in] inputPtrs Vector of input data pointers in the order specified by GetInputTensorNames()
     * @param[in] deviceId ID of the NPU device to validate. Default is 0 (first device).
     * @return Output tensors as a vector of smart pointer instances, representing the validation results.
     * @warning This function is intended for validation purposes only and may not be optimized for performance.
     */
    TensorPtrs ValidateDeviceMultiInput(const std::vector<void*>& inputPtrs, int deviceId = 0);

    /**
     * @deprecated Use RegisterCallback() instead.
     * @brief Register user callback function to be called by inference completion. (Legacy API)
     * @param[in] callbackFunc Function which is called when inference is complete, it gets outputs and user_arg ptr
     * @param outputs output tensors data
     * @param userArg userArg given by Run();
     */
    [[deprecated("Use RegisterCallback() instead")]]
    void RegisterCallBack(std::function<int(TensorPtrs& outputs, void* userArg)> callbackFunc) { return RegisterCallback(callbackFunc); }

    /** @brief Registers a user-defined callback function that will be executed upon completion of an asynchronous inference request.
     * @param[in] callbackFunc The function to be called. It receives the output tensors and the user-provided argument.
     */
    void RegisterCallback(std::function<int(TensorPtrs& outputs, void* userArg)> callbackFunc);

    /** @brief Blocks execution and waits until the asynchronous request identified by jobId is complete.
     * @param[in] jobId The job ID returned from a RunAsync call.
     * @return A TensorPtrs object containing the output from the completed job.
     */
    TensorPtrs Wait(int jobId);

    /**
     *  @deprecated Use GetInputs() instead.
     *  @brief Get input tensor (Legacy API)
     *  @param[in] ptr pointer to virtual address
     *  @param[in] phyAddr pointer to physical address
     *  @return if ptr is null, input memory area in engine is returned
     *  @return if ptr and phyAddr is given, inputs tensors that contains output addresses
     */
    [[deprecated("Use GetInputs() instead")]]
    Tensors inputs(void *ptr = nullptr, uint64_t phyAddr = 0) { return GetInputs(ptr, phyAddr); }

    /** @brief Retrieves the input tensors for the model. If ptr is null, it returns information about the input memory area within the engine. If ptr and phyAddr are provided, it returns tensor objects pointing to those addresses.
     *  @param[in] ptr An optional pointer to a virtual address for the input data.
     *  @param[in] phyAddr An optional pointer to a physical address for the input data.
     *  @return A Tensors (vector of Tensor) object.
     */
    Tensors GetInputs(void *ptr = nullptr, uint64_t phyAddr = 0);

    /**
     *  @deprecated Use GetInputs() instead.
     *  @brief Get input tensor (Legacy API)
     *  @param[in] devId device id
     *  @return vector of input tensors
     */
    [[deprecated("Use GetInputs() instead")]]
    std::vector<Tensors> inputs(int devId) { return GetInputs(devId); }

    /** @brief Retrieves the input tensors for a specific device ID.
     *  @param[in] devId The ID of the device.
     *  @return A vector of Tensors objects.
     */
    std::vector<Tensors> GetInputs(int devId);

    /**
     *  @deprecated Use GetOutputs() instead.
     *  @brief Get output tensor (Legacy API)
     *  @param[in] ptr pointer to virtual address
     *  @param[in] phyAddr pointer to physical address
     *  @return if ptr is null, output memory area in engine is returned
     *  @return if ptr and phyAddr is given, outputs tensors that contains output addresses
     */
    [[deprecated("Use GetOutputs() instead")]]
    Tensors outputs(void *ptr = nullptr, uint64_t phyAddr = 0) { return GetOutputs(ptr, phyAddr); }

    /** @brief Retrieves the output tensors. If ptr is null, it returns information about the output memory area within the engine. If ptr and phyAddr are provided, it returns tensor objects pointing to those addresses.
     *  @param[in] ptr An optional pointer to a virtual address for the output data.
     *  @param[in] phyAddr An optional pointer to a physical address for the output data.
     *  @return A Tensors (vector of Tensor) object.
     */
    Tensors GetOutputs(void *ptr = nullptr, uint64_t phyAddr = 0);

    /**
     * @deprecated Use GetInputSize() instead.
     * @brief Get total size of input tensors (Legacy API)
     * @return Input size of one inference in bytes
     */
    [[deprecated("Use GetInputSize() instead")]]
    uint64_t input_size() { return GetInputSize(); }

    /**
     * @brief Gets the total size of all input tensors combined in bytes.
     * @return The total input size as a uint64_t.
     */
    uint64_t GetInputSize();

    /**
     * @brief Gets the individual sizes (in bytes) of each input tensor for multi-input models.
     * @return A vector of input tensor sizes, in the order specified by GetInputTensorNames().
     */
    std::vector<uint64_t> GetInputTensorSizes();

    /**
     * @brief Gets the individual sizes (in bytes) of each output tensor.
     * @return A vector of output tensor sizes, in the order specified by GetOutputTensorNames().
     *         For dynamic shape tensors, returns 0 as the size cannot be determined at compile time.
     *         Actual sizes for dynamic tensors are available after inference execution.
     * @warning Dynamic shape tensors will return 0 and log a warning message.
     */
    std::vector<uint64_t> GetOutputTensorSizes();

    /**
     * @deprecated Use GetOutputSize() instead.
     * @brief Get total size of output tensors (Legacy API)
     * @return Output size of one inference in bytes
     */
    [[deprecated("Use GetOutputSize() instead")]]
    uint64_t output_size() { return GetOutputSize(); }

    /**
     * @brief Gets the total size of all output tensors combined in bytes.
     * @return The total output size as a uint64_t. Returns 0 for models with dynamic shape outputs,
     *         as the size cannot be determined statically. For dynamic models, use GetOutputTensorSizes()
     *         for individual tensor sizes or allocate buffers after inference when actual shapes are known.
     * @warning For dynamic shape models, this method returns 0 and logs a warning.
     *          Do not use the return value for memory allocation in such cases.
     */
    uint64_t GetOutputSize();

     /**
     * @deprecated Use GetModelName() instead.
     * @brief Get model name (Legacy API)
     * @return model name
     */
    [[deprecated("Use GetModelName() instead")]]
    std::string name() { return GetModelName(); }

    /**
     * @brief Gets the name of the model.
     * @return The model name as a std::string.
     */
    std::string GetModelName();

    /**
     * @deprecated Use GetTaskOrder() instead.
     * @brief Get model task order (Legacy API)
     * @return task order
     */
    [[deprecated("Use GetTaskOrder() instead")]]
    std::vector<std::string> task_order() { return GetTaskOrder(); }

    /**
     * @brief Gets the model's task execution order.
     * @return A vector of strings representing the task order.
     */
    std::vector<std::string> GetTaskOrder();

    /**
     * @deprecated Use GetLatency() instead.
     * @brief Get latest latency (Legacy API)
     * @return latency (microseconds)
     */
    [[deprecated("Use GetLatency() instead")]]
    int latency() { return GetLatency(); }

    /** @brief Gets the latency of the most recent inference in microseconds.
     * @return The latency value.
     */
    int GetLatency();

    /**
     * @deprecated Use GetNpuInferenceTime() instead.
     * @brief Get latest inference time (Legacy API)
     * @return inference time (microseconds)
     */
    [[deprecated("Use GetNpuInferenceTime() instead")]]
    uint32_t inference_time() { return GetNpuInferenceTime(); }

    /** @brief Gets the pure NPU processing time for the most recent inference in microseconds.
     * @return The NPU inference time.
     */
    uint32_t GetNpuInferenceTime();

    /** @brief Gets a vector of recent latency measurements.
     * @return A vector of latencies in microseconds.
     */
    std::vector<int> GetLatencyVector();

    /** @brief Gets a vector of recent NPU inference time measurements.
     * @return A vector of NPU inference times in microseconds.
     */
    std::vector<uint32_t> GetNpuInferenceTimeVector();

    /** @brief Gets the mean (average) of all collected latency values.
     * @return The mean latency in microseconds.
     */
    double GetLatencyMean();

    /** @brief Gets the mean (average) of all collected NPU inference times.
     * @return The mean NPU inference time in microseconds.
     */
    double GetNpuInferenceTimeMean();

    /** @brief Gets the standard deviation of all collected latency values.
     * @return The standard deviation of latency.
     */
    double GetLatencyStdDev();

    /** @brief Gets the standard deviation of all collected NPU inference times.
     * @return The standard deviation of NPU inference time.
     */
    double GetNpuInferenceTimeStdDev();

    /** @brief Gets the total count of latency measurements recorded.
     * @return The number of latency measurements.
     */
    int GetLatencyCnt();

    /** @brief Gets the total count of NPU inference time measurements recorded.
     * @return The number of measurements.
     */
    int GetNpuInferenceTimeCnt();

    /**
     *  @deprecated Use GetAllTaskOutputs() instead.
     *  @brief Get output tensors of all tasks (Legacy API)
     *  @return the output of all tasks as a vector of smart pointer instance vectors.
     */
    [[deprecated("Use GetAllTaskOutputs() instead")]]
    std::vector<TensorPtrs> get_outputs() { return GetAllTaskOutputs(); }

    /**
     *  @brief Retrieves the output tensors of all internal tasks in the model.
     *  @return A vector of TensorPtrs, where each element represents the outputs of a single task.
     */
    std::vector<TensorPtrs> GetAllTaskOutputs();

    /**
     *  @deprecated Use GetBitmatchMask() instead.
     *  @internal
     *  @brief Get bitmatch mask (Legacy API)
     *  @param[in] index index to npu task (rmap)
     *  @return bitmatch mask for a given index
     */
    [[deprecated("Use GetBitmatchMask() instead")]]
    std::vector<uint8_t> bitmatch_mask(int index) { return GetBitmatchMask(index); }

    /**
     *  @internal
     *  @brief An internal function to get the bitmatch mask for a given NPU task index.
     *  @param[in] index The index of the NPU task.
     *  @return A vector of uint8_t representing the mask.
     */
    std::vector<uint8_t> GetBitmatchMask(int index);

    /**
     * @deprecated Use GetNumTailTasks() instead.
     * @brief Returns the number of tail tasks in the model. (Legacy API)
     * @return The number of tasks that have no subsequent tasks.
     *
     * Tail tasks are those which do not have any tasks following them in the model's task chain.
     * This function provides the count of such tail tasks.
     */
    [[deprecated("Use GetNumTailTasks() instead")]]
    int get_num_tails() { return GetNumTailTasks(); }

    /**
     * @brief Returns the number of "tail" tasks in the model, which are tasks that have no subsequent tasks.
     * @return The number of tail tasks.
     */
    int GetNumTailTasks();

    /**
     * @deprecated Use GetCompileType() instead.
     * @brief Returns the compile type of the model. (Legacy API)
     * @return The compile type of the model.
     */
    [[deprecated("Use GetCompileType() instead")]]
    std::string get_compile_type() { return GetCompileType(); }

    /**
     * @brief Returns the compile type of the loaded model.
     * @return The compile type as a std::string.
     */
    std::string GetCompileType();

    /**
     * @brief Returns the DXNN file format version of the loaded model.
     * @return The model version string.
     */
    std::string GetModelVersion();

    /**
     * @deprecated Use IsPPU() instead.
     * @brief Returns whether the model is using PPU. (Legacy API)
     * @return whether the model is using PPU.
     */
    [[deprecated("Use IsPPU() instead")]]
    bool is_PPU() { return IsPPU(); }

    /**
     * @brief Checks if the loaded model utilizes a Post-Processing Unit (PPU).
     * @return true if the model uses a PPU, false otherwise.
     */
    bool IsPPU();

    /**
     * @brief Checks whether any output tensor has dynamic shape.
     * @return true if at least one output tensor has dynamic shape, false otherwise.
     */
    bool HasDynamicOutput();

    /**
     * @brief Checks whether ONNX Runtime (ORT) is configured and available for use.
     * @return true if ORT is configured, false otherwise.
     */
    bool IsOrtConfigured();

    /**
     * @brief Checks if the loaded model requires multiple input tensors.
     * @return true if the model has multiple inputs, false otherwise.
     */
    bool IsMultiInputModel() const;

    /**
     * @brief Returns the number of input tensors required by the model.
     * @return The count of input tensors.
     */
    int GetInputTensorCount() const;

    /**
     * @brief Returns the names of all input tensors in the order they should be provided.
     * @return A vector of input tensor names.
     */
    std::vector<std::string> GetInputTensorNames() const;

    /**
     * @brief Returns the names of all output tensors in the order they are produced.
     * @return A vector of output tensor names.
     */
    std::vector<std::string> GetOutputTensorNames() const;



    /**
     * @brief Returns the mapping from input tensor names to their target tasks within the model graph.
     * @return A map where the key is the tensor name and the value is the task name.
     */
    std::map<std::string, std::string> GetInputTensorToTaskMapping() const;

    /**
     * @brief Deallocates resources and performs cleanup. This should be called to release memory and handles held by the engine.
     */
    void Dispose();

#ifdef _WIN32
    float RunBenchMarkWindows(int num, void* inputPtr = nullptr);
#endif  // _WIN32
    friend DXRT_API std::ostream& operator<<(std::ostream&, const InferenceEngine&);
    InferenceTimer* getTimer(){return &_inferenceTimer;}

    /** @brief Runs synchronous inference for a multi-input model using a map of named tensors.
     * @param[in] inputTensors A map of tensor names to input data pointers.
     * @param[in] userArg An optional user-defined argument.
     * @param[out] outputPtr An optional pointer to a pre-allocated output buffer.
     * @return A TensorPtrs object containing the output.
     */
    TensorPtrs RunMultiInput(const std::map<std::string, void*>& inputTensors, void *userArg=nullptr, void *outputPtr=nullptr);

    /** @brief Runs synchronous inference for a multi-input model using a vector of input pointers.
     * @param[in] inputPtrs A vector of input pointers in the order specified by GetInputTensorNames().
     * @param[in] userArg An optional user-defined argument.
     * @param[out] outputPtr An optional pointer to a pre-allocated output buffer.
     * @return A TensorPtrs object containing the output.
     */
    TensorPtrs RunMultiInput(const std::vector<void*>& inputPtrs, void *userArg=nullptr, void *outputPtr=nullptr);

    /** @brief Check if tensor-centric offset calculation is supported
     * @return true if supported, false otherwise
     */
    bool supportsTensorCentricOffsets() const;

    /**
     * @brief Gets the byte offset for a specific output tensor within the final concatenated output buffer.
     * @param tensorName The name of the output tensor.
     * @return The offset in bytes.
     */
    size_t GetOutputTensorOffset(const std::string& tensorName) const;

 private:  // private functions
    int runAsync(void *inputPtr, void *userArg, void *outputPtr, int batchIndex,
        std::function<void(TensorPtrs &outputs, void *userArg, int jobId)> batchCallback);

    void runSubBatch(std::vector<TensorPtrs>& result, int batchCount, int startIndex,
            const std::vector<void*>& inputPtrs,
            const std::vector<void*>& outputPtrs,
            const std::vector<void*>& userArgs);

    // Helper method to check if single input buffer should be auto-split for multi-input models
    bool shouldAutoSplitInput() const;

    // Helper method to check if user output buffer should be used for final output allocation
    bool shouldUseUserOutputBuffer() const;

 private:
    std::string _modelFile;
    std::string _modelDir;
    std::string _name;
    std::string _modelCompileType;
    bool _isOffloadingModel = false;
    bool _isPPU = false;

    ModelDataBase _modelData;
    std::vector<uint8_t> _maskBuf;
    std::map<std::string, deepx_graphinfo::SubGraph> _subGraphMap;
    InferenceOption _option;
    std::vector<std::shared_ptr<Task>> _tasks;  // to be changed to complex graph
    std::shared_ptr<Task> _head;  // Primary head task (for backward compatibility), actual multi-head processing uses _inputTasks
    std::vector<std::shared_ptr<Task>> _tails;
    int _numTails;
    std::map<std::string, std::shared_ptr<Task>> _taskMap;
    InferenceTimer _inferenceTimer;
    std::vector<std::string> _taskOrder;
    std::vector<std::string> _lastOutputOrder;

    // Multi-input support
    bool _isMultiInput = false;
    std::vector<std::shared_ptr<Task>> _inputTasks;
    std::vector<std::string> _modelInputOrder;
    std::map<std::string, std::string> _inputTensorToTaskMap;

    // Multi-output buffer management
    bool _hasUserOutputBuffer = false;
    void* _userOutputPtr = nullptr;

    // Tensor-centric management for better extensibility
    struct TensorDescriptor {
        std::string name;
        std::string producerTask;
        std::vector<std::string> consumerTasks;
        bool isModelInput = false;
        bool isModelOutput = false;
        uint64_t sizeInBytes = 0;
        uint64_t outputBufferOffset = 0;  // Offset in final output buffer

        TensorDescriptor() = default;
        TensorDescriptor(const std::string& tensorName, const std::string& producer)
            : name(tensorName), producerTask(producer) {}
    };

    // Tensor registry for comprehensive management
    std::map<std::string, TensorDescriptor> _tensorRegistry;

    // Helper methods for tensor-centric management
    void initializeEnvironmentVariables();
    void initializeModel();
    void buildTasksAndSubgraphMap();
    void buildInputTensorMapping();
    void buildTaskGraph();
    void buildTensorRegistry();
    void calculateTensorOffsets();
    bool isTensorModelOutput(const std::string& tensorName) const;
    bool isTensorModelInput(const std::string& tensorName) const;

    // Debug logging for model data comparison
    void LogModelDataDetails();

    // Callback and disposal management
    std::function<int(TensorPtrs &outputs, void *userArg)> _userCallback;

    void disposeOnce();
    std::once_flag _disposeOnceFlag;
    bool _isDisposed = false;

    // inference job pool for IE
    std::shared_ptr<CircularDataPool<InferenceJob>> _inferenceJobPool;

    // Thread-safe output buffer management for multi-CPU task scenarios
    mutable std::mutex _outputBufferMutex;
    std::map<std::string, uint64_t> _cachedOutputOffsets;
    std::atomic<bool> _outputOffsetsCalculated{false};

 private:
    static std::mutex _sInferenceEngineMutex;

    std::vector<uint8_t> _validationOutputBuffer;
};

} /* namespace dxrt */
