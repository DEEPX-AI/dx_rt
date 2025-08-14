// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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


/** @brief This class abstracts runtime inference executor for user's compiled model.
 * @details After user loads compiled model to InferenceEngine, real-time device tasks will be scheduled by internal runtime libraries.
 *          It supports both inference mode (synchronous/asynchronous) according to user's request.
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
    /** @brief Perform the task of loading the model and configuring the NPU to run.
     * @param[in] modelPath model path
     * @param[in] option device and npu core options
     * @code
     * auto modelPath = "model.dxnn"; // assume compiled model path name is "model.dxnn"
     * dxrt::InferenceEngine(modelPath) ie(modelPath);
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

    /** @brief Run inference engine using specific input pointer Synchronously
     * @param[in] inputPtr input data pointer to run inference
     * @param[in] userArg user-defined arguments as a void pointer(e.g. original frame data, metadata about input, ... )
     * @param[out] outputPtr pointer to output data, if it is nullptr, output data is stored in buffer inside DXRT.
     * @code
     * auto modelPath = "model.dxnn"; // assume compiled model path name is "model.dxnn"
     * dxrt::InferenceEngine ie(modelPath);
     * auto outputs = ie.Run();
     * @endcode
     * @return output tensors as vector of smart pointer instances 
     */
    TensorPtrs Run(void *inputPtr, void *userArg = nullptr, void *outputPtr = nullptr);

    /**
     * @brief Runs the inference engine using a specific input pointer vector.
     *
     * This function executes inference based on the provided input data pointer vector and returns a vector of output tensors.
     * Users can optionally provide additional user-defined arguments and output pointer vectors.
     * If userArgs is used, the number of elements in inputPtrs must be the same as the number of elements in userArgs.
     * An InvalidArgumentException is thrown if the size of inputPtrs and userArgs are different.
     * An InvalidArgumentException is thrown if the size of inputPtrs and OutputPtrs are different.
     * An InvalidArgumentException is thrown if the size of inputPtrs is 0.
     * 
     * @param[in] inputBuffers Vector of input data pointers used for inference.
     * @param[out] outputBuffers Vector of output data pointers.
     * @param[in] userArgs Vector of user-defined arguments (e.g., original frame data, input metadata, etc.). (Optional)
     *
     * @return Vector of output tensors as smart pointer instances.
     */
    std::vector<TensorPtrs> Run(
        const std::vector<void*>& inputBuffers,
        const std::vector<void*>& outputBuffers,
        const std::vector<void*>& userArgs = {}
    );


    /** @brief Non-blocking call to request asynchronous inference by input pointer, and get job ID from inference engine.
     * @param[in] inputPtr input data pointer to run inference
     * @param[in] userArg user-defined arguments as a void pointer(e.g. original frame data, metadata about input, ... )
     * @param[out] outputPtr pointer to output data, if it is nullptr, output data area is allocated by DXRT.
     * @code
     * auto modelPath = "model"; // assume compiled model path name is "model"
     * dxrt::InferenceOption option;
     * option.devices = {0,1,3};  //use only 0,1,3 device
     * dxrt::InferenceEngine ie(modelPath, option);
     * auto outputs = ie.Run();
     * @endcode
     * @return job id that can be used to wait() function 
     */
    int RunAsync(void *inputPtr, void *userArg=nullptr, void *outputPtr = nullptr);

    /** @brief Non-blocking call to request asynchronous inference with automatic multi-input detection.
     * This function automatically detects whether the input should be interpreted as multi-input single inference
     * or batch inference based on the model requirements and input count.
     * @param[in] inputPtrs Vector of input data pointers
     * @param[in] userArg user-defined arguments as a void pointer
     * @param[out] outputPtr pointer to output data, if it is nullptr, output data area is allocated by DXRT.
     * @return job id that can be used to wait() function 
     */
    int RunAsync(const std::vector<void*>& inputPtrs, void *userArg=nullptr, void *outputPtr = nullptr);

    /** @brief Run async inference with multiple input tensors for multi-input models
     * @param[in] inputTensors Map of tensor name to input data pointer
     * @param[in] userArg user-defined arguments as a void pointer
     * @param[out] outputPtr pointer to output data, if it is nullptr, output data is stored in buffer inside DXRT.
     * @return job id that can be used to wait() function 
     */
    int RunAsyncMultiInput(const std::map<std::string, void*>& inputTensors, void *userArg=nullptr, void *outputPtr = nullptr);

    /** @brief Run async inference with multiple input tensors (vector format) for multi-input models
     * @param[in] inputPtrs Vector of input data pointers in the order specified by GetInputTensorNames()
     * @param[in] userArg user-defined arguments as a void pointer
     * @param[out] outputPtr pointer to output data, if it is nullptr, output data is stored in buffer inside DXRT.
     * @return job id that can be used to wait() function 
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

    /** @brief run benchmark with loop n times
     * @param[in] num number of inferences
     * @param[in] inputPtr input data pointer to run inference
     * @return average fps
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
     * 
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
     * 
     */
    TensorPtrs ValidateDevice(const std::vector<void*>& inputPtrs, int deviceId = 0);

    /** @brief Validate NPU device with multiple input tensors for multi-input models
     * @param[in] inputTensors Map of tensor name to input data pointer
     * @param[in] deviceId ID of the NPU device to validate. Default is 0 (first device).
     * @return Output tensors as a vector of smart pointer instances, representing the validation results.
     */
    TensorPtrs ValidateDeviceMultiInput(const std::map<std::string, void*>& inputTensors, int deviceId = 0);

    /** @brief Validate NPU device with multiple input tensors (vector format) for multi-input models
     * @param[in] inputPtrs Vector of input data pointers in the order specified by GetInputTensorNames()
     * @param[in] deviceId ID of the NPU device to validate. Default is 0 (first device).
     * @return Output tensors as a vector of smart pointer instances, representing the validation results.
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

    /** @brief Register user callback function to be called by inference completion.
     * @param[in] callbackFunc Function which is called when inference is complete, it gets outputs and user_arg ptr
     * @param outputs output tensors data
     * @param userArg userArg given by Run();
     */
    void RegisterCallback(std::function<int(TensorPtrs& outputs, void* userArg)> callbackFunc);

    /** @brief Wait until an request is complete and returns output
     * @param[in] jobId job Id returned by RunAsync()
     * @return output tensors as vector of smart pointer instances 
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

    /** @brief Get input tensor
     *  @param[in] ptr pointer to virtual address
     *  @param[in] phyAddr pointer to physical address
     *  @return if ptr is null, input memory area in engine is returned
     *  @return if ptr and phyAddr is given, inputs tensors that contains output addresses
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

    /** @brief Get input tensor
     *  @param[in] devId device id
     *  @return vector of input tensors
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

    /** @brief Get output tensor
     *  @param[in] ptr pointer to virtual address
     *  @param[in] phyAddr pointer to physical address
     *  @return if ptr is null, output memory area in engine is returned
     *  @return if ptr and phyAddr is given, outputs tensors that contains output addresses
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
     * @brief Get total size of input tensors
     * @return Input size of one inference in bytes
     */
    uint64_t GetInputSize();

    /** 
     * @brief Get individual input tensor sizes for multi-input models
     * @return Vector of input tensor sizes in bytes, in the order specified by GetInputTensorNames()
     */
    std::vector<uint64_t> GetInputTensorSizes();

    /** 
     * @brief Get individual output tensor sizes for multi-output models
     * @return Vector of output tensor sizes in bytes, in the order specified by GetOutputTensorNames()
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
     * @brief Get total size of output tensors
     * @return Output size of one inference in bytes
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
     * @brief Get model name
     * @return model name
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
     * @brief Get model task order
     * @return task order
     */
    std::vector<std::string> GetTaskOrder();

    /**
     * @deprecated Use GetLatency() instead. 
     * @brief Get latest latency (Legacy API)
     * @return latency (microseconds)
     */
    [[deprecated("Use GetLatency() instead")]]
    int latency() { return GetLatency(); }

    /** @brief Get latest latency
     * @return latency (microseconds)
     */
    int GetLatency();

    /**
     * @deprecated Use GetNpuInferenceTime() instead.  
     * @brief Get latest inference time (Legacy API)
     * @return inference time (microseconds)
     */
    [[deprecated("Use GetNpuInferenceTime() instead")]]
    uint32_t inference_time() { return GetNpuInferenceTime(); }

    /** @brief Get latest inference time
     * @return inference time (microseconds)
     */
    uint32_t GetNpuInferenceTime();

    /** @brief Get recent Latency
     * @return latency (microseconds)
     */
    std::vector<int> GetLatencyVector();

    /** @brief Get recent inference time
     * @return inference time (microseconds)
     */
    std::vector<uint32_t> GetNpuInferenceTimeVector();

    /** @brief Get latency Mean
     * @return latency Mean (microseconds)
     */
    double GetLatencyMean();

    /** @brief Get inference time Mean
     * @return inference time Mean (microseconds)
     */
    double GetNpuInferenceTimeMean();

    /** @brief Get latency Standard Deviation
     * @return latency Standard Deviation 
     */
    double GetLatencyStdDev();

    /** @brief Get inference time Standard Deviation
     * @return inference time Standard Deviation 
     */
    double GetNpuInferenceTimeStdDev();

    /** @brief Get latency Count
     * @return latency Count 
     */
    int GetLatencyCnt();

    /** @brief Get inference time Count
     * @return inference time Count
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
     *  @brief Get output tensors of all tasks
     *  @return the output of all Tasks as a vector of smart pointer instance vectors.  
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
     *  @brief Get bitmatch mask
     *  @param[in] index index to npu task (rmap)
     *  @return bitmatch mask for a given index
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
     * @brief Returns the number of tail tasks in the model.
     * @return The number of tasks that have no subsequent tasks.
     * 
     * Tail tasks are those which do not have any tasks following them in the model's task chain.
     * This function provides the count of such tail tasks.
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
     * @brief Returns the compile type of the model.
     * @return The compile type of the model.
     */
    std::string GetCompileType();

    /** 
     * @brief Returns the DXNN file format version of the model.
     * @return The DXNN file format version as a string.
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
     * @brief Returns whether the model is using PPU.
     * @return whether the model is using PPU.
     */
    bool IsPPU();

    /**
     * @brief Checks whether ORT (ONNX Runtime) is configured.
     * @return true if ORT is configured, false otherwise.
     * @throw InvalidArgumentException If USE_ORT is OFF but the useORT option is set to true.
     */
    bool IsOrtConfigured();

    /** 
     * @brief Returns whether the model has multiple input tensors.
     * @return true if the model has multiple input tensors, false otherwise.
     */
    bool IsMultiInputModel() const;

    /** 
     * @brief Returns the number of input tensors required by the model.
     * @return The number of input tensors.
     */
    int GetInputTensorCount() const;

    /** 
     * @brief Returns the names of all input tensors in the model.
     * @return Vector of input tensor names in the order they should be provided.
     */
    std::vector<std::string> GetInputTensorNames() const;

    /** 
     * @brief Returns the names of all output tensors in the model.
     * @return Vector of output tensor names in the order they are produced.
     */
    std::vector<std::string> GetOutputTensorNames() const;

    /** 
     * @brief Returns the mapping from input tensor names to their target tasks.
     * @return Map where key is tensor name and value is task name.
     */
    std::map<std::string, std::string> GetInputTensorToTaskMapping() const;

    /** 
     * @brief Resource deallocation and cleanup
     */
    void Dispose();

#ifdef _WIN32
    float RunBenchMarkWindows(int num, void* inputPtr = nullptr);
#endif  // _WIN32
    friend DXRT_API std::ostream& operator<<(std::ostream&, const InferenceEngine&);
    InferenceTimer* getTimer(){return &_inferenceTimer;}

    /** @brief Run inference engine with multiple input tensors for multi-input models
     * @param[in] inputTensors Map of tensor name to input data pointer
     * @param[in] userArg user-defined arguments as a void pointer
     * @param[out] outputPtr pointer to output data, if it is nullptr, output data is stored in buffer inside DXRT.
     * @return output tensors as vector of smart pointer instances 
     */
    TensorPtrs RunMultiInput(const std::map<std::string, void*>& inputTensors, void *userArg=nullptr, void *outputPtr=nullptr);

    /** @brief Run inference engine with multiple input tensors (vector format) for multi-input models
     * @param[in] inputPtrs Vector of input data pointers in the order specified by GetInputTensorNames()
     * @param[in] userArg user-defined arguments as a void pointer
     * @param[out] outputPtr pointer to output data, if it is nullptr, output data is stored in buffer inside DXRT.
     * @return output tensors as vector of smart pointer instances 
     */
    TensorPtrs RunMultiInput(const std::vector<void*>& inputPtrs, void *userArg=nullptr, void *outputPtr=nullptr);

    /** @brief Check if tensor-centric offset calculation is supported
     * @return true if supported, false otherwise
     */
    bool supportsTensorCentricOffsets() const;

    /**
     * @brief Gets the offset for a specific output tensor in the final output buffer
     * @param tensorName Name of the output tensor
     * @return Offset in bytes for the tensor in the output buffer
     */
    size_t GetOutputTensorOffset(const std::string& tensorName) const;

    // DSP Code
    int DSP_GetDeviceBufferPtr(uint64_t *inputPtr, uint64_t *outputPtr);
    void *DSP_Run(void *inputPtr, void *outputPtr = nullptr, void *userArg = nullptr);
    void *DSP_Wait(int jobId);

 private:  // private functions
    int runAsync(void *inputPtr, void *userArg, void *outputPtr,
        std::function<void(TensorPtrs &outputs, void *userArg, int jobId)> batchCallback);

    void runSubBatch(std::vector<TensorPtrs>& result, int batchCount, int startIndex, void* batchArgs,
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
    std::vector<std::string> _lastOutputOrder;  // Keep for backward compatibility
    std::vector<std::string> _finalOutputOrder;  // New tensor-centric order

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
};

} /* namespace dxrt */
