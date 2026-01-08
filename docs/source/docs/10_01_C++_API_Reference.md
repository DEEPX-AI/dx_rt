This section documents the core C++ classes provided by the DX-RT SDK. It includes detailed descriptions of the inference engine, configuration options, device status monitoring, and tensor structures. These APIs are designed for high-performance, real-time applications and offer fine-grained control over NPU execution.  

---

### class dxrt::InferenceEngine  
  
This class abstracts the runtime inference executor for a user's compiled model. After a model is loaded, real-time device tasks are scheduled by internal runtime libraries. It supports both synchronous and asynchronous inference modes.  
  
#### Constructor  
  
***`explicit InferenceEngine(const std::string &modelPath, InferenceOption &option = DefaultInferenceOption)`***  
-   **Description**: Loads a model from the specified file path and configures the NPU to run it.  
-   **Parameters**:  
    -   `modelPath`: The file path to the compiled model (e.g., `model.dxnn`).  
    -   `option`: A reference to an `InferenceOption` object to configure devices and NPU cores. If not provided, uses `DefaultInferenceOption`.  
-   **Example**:
    ```cpp
    // Load model from file path
    dxrt::InferenceEngine ie("model.dxnn");
    
    // Load model with custom options
    dxrt::InferenceOption option;
    option.devices = {0, 1};
    option.boundOption = dxrt::InferenceOption::NPU_ALL;
    dxrt::InferenceEngine ie2("model.dxnn", option);
    ```

***`explicit InferenceEngine(const uint8_t* modelBuffer, size_t modelSize, InferenceOption &option = DefaultInferenceOption)`***  
-   **Description**: Loads a model from a memory buffer and configures the NPU to run it. This constructor is useful for embedded systems or when models are loaded from custom sources (e.g., encrypted storage, network).  
-   **Parameters**:  
    -   `modelBuffer`: A pointer to the compiled model data in memory.  
    -   `modelSize`: The size of the model data in bytes.  
    -   `option`: A reference to an `InferenceOption` object to configure devices and NPU cores. If not provided, uses `DefaultInferenceOption`.  
-   **Example**:
    ```cpp
    // Load model file into memory buffer
    std::ifstream file("model.dxnn", std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        // Create inference engine from memory buffer
        dxrt::InferenceEngine ie(buffer.data(), buffer.size());
        
        // With custom options
        dxrt::InferenceOption option;
        option.useORT = true;
        dxrt::InferenceEngine ie2(buffer.data(), buffer.size(), option);
    }
    ```
-   **Use Cases**:
    -   Loading models from encrypted or compressed storage
    -   Network-based model distribution
    -   Embedded systems with models stored in ROM
    -   Dynamic model loading without filesystem access


  
#### Member Functions  
  
***`Dispose()`***  
-   **Signature**: `void Dispose()`  
-   **Description**: Deallocates resources and performs cleanup. This should be called to release memory and handles held by the engine.  
  
***`GetAllTaskOutputs()`***  
-   **Signature**: `std::vector<TensorPtrs> GetAllTaskOutputs()`  
-   **Description**: Retrieves the output tensors of all internal tasks in the model.  
-   **Returns**: A vector of `TensorPtrs`, where each element represents the outputs of a single task.  
-   **Note**: The legacy function `get_outputs()` is deprecated.  
  
***`GetBitmatchMask(int index)`***  
-   **Signature**: `std::vector<uint8_t> GetBitmatchMask(int index)`  
-   **Description**: An internal function to get the bitmatch mask for a given NPU task index.  
-   **Parameters**:  
    -   `index`: The index of the NPU task.  
-   **Returns**: A vector of `uint8_t` representing the mask.  
-   **Note**: The legacy function `bitmatch_mask(int index)` is deprecated.  
  
***`GetCompileType()`***  
-   **Signature**: `std::string GetCompileType()`  
-   **Description**: Returns the compile type of the loaded model.  
-   **Returns**: The compile type as a `std::string`.  
-   **Note**: The legacy function `get_compile_type()` is deprecated.  
  
***`GetInputSize()`***  
-   **Signature**: `uint64_t GetInputSize()`  
-   **Description**: Gets the total size of all input tensors combined in bytes.  
-   **Returns**: The total input size as a `uint64_t`.  
-   **Note**: The legacy function `input_size()` is deprecated.  
  
***`GetInputs(void *ptr = nullptr, uint64_t phyAddr = 0)`***  
-   **Signature**: `Tensors GetInputs(void *ptr = nullptr, uint64_t phyAddr = 0)`  
-   **Description**: Retrieves the input tensors for the model. If `ptr` is null, it returns information about the input memory area within the engine. If `ptr` and `phyAddr` are provided, it returns tensor objects pointing to those addresses.  
-   **Parameters**:  
    -   `ptr`: An optional pointer to a virtual address for the input data.  
    -   `phyAddr`: An optional pointer to a physical address for the input data.  
-   **Returns**: A `Tensors` (vector of `Tensor`) object.  
-   **Note**: The legacy function `inputs(...)` is deprecated.  
  
***`GetInputs(int devId)`***  
-   **Signature**: `std::vector<Tensors> GetInputs(int devId)`  
-   **Description**: Retrieves the input tensors for a specific device ID.  
-   **Parameters**:  
    -   `devId`: The ID of the device.  
-   **Returns**: A vector of `Tensors` objects.  
-   **Note**: The legacy function `inputs(int devId)` is deprecated.  
  
***`GetInputTensorCount()`***  
-   **Signature**: `int GetInputTensorCount() const`  
-   **Description**: Returns the number of input tensors required by the model.  
-   **Returns**: The count of input tensors.  
  
***`GetInputTensorNames()`***  
-   **Signature**: `std::vector<std::string> GetInputTensorNames() const`  
-   **Description**: Returns the names of all input tensors in the order they should be provided.  
-   **Returns**: A vector of input tensor names.  
  
***`GetInputTensorSizes()`***  
-   **Signature**: `std::vector<uint64_t> GetInputTensorSizes()`  
-   **Description**: Gets the individual sizes (in bytes) of each input tensor for multi-input models.  
-   **Returns**: A vector of input tensor sizes, in the order specified by `GetInputTensorNames()`.  
  
***`GetInputTensorToTaskMapping()`***  
-   **Signature**: `std::map<std::string, std::string> GetInputTensorToTaskMapping() const`  
-   **Description**: Returns the mapping from input tensor names to their target tasks within the model graph.  
-   **Returns**: A map where the key is the tensor name and the value is the task name.  
  
***`GetLatency()`***  
-   **Signature**: `int GetLatency()`  
-   **Description**: Gets the latency of the most recent inference in microseconds.  
-   **Returns**: The latency value.  
-   **Note**: The legacy function `latency()` is deprecated.  
  
***`GetLatencyCnt()`***  
-   **Signature**: `int GetLatencyCnt()`  
-   **Description**: Gets the total count of latency measurements recorded.  
-   **Returns**: The number of latency measurements.  
  
***`GetLatencyMean()`***  
-   **Signature**: `double GetLatencyMean()`  
-   **Description**: Gets the mean (average) of all collected latency values.  
-   **Returns**: The mean latency in microseconds.  
  
***`GetLatencyStdDev()`***  
-   **Signature**: `double GetLatencyStdDev()`  
-   **Description**: Gets the standard deviation of all collected latency values.  
-   **Returns**: The standard deviation of latency.  
  
***`GetLatencyVector()`***  
-   **Signature**: `std::vector<int> GetLatencyVector()`  
-   **Description**: Gets a vector of recent latency measurements.  
-   **Returns**: A vector of latencies in microseconds.  
  
***`GetModelName()`***  
-   **Signature**: `std::string GetModelName()`  
-   **Description**: Gets the name of the model.  
-   **Returns**: The model name as a `std::string`.  
-   **Note**: The legacy function `name()` is deprecated.  
  
***`GetModelVersion()`***  
-   **Signature**: `std::string GetModelVersion()`  
-   **Description**: Returns the DXNN file format version of the loaded model.  
-   **Returns**: The model version string.  
  
***`GetNpuInferenceTime()`***  
-   **Signature**: `uint32_t GetNpuInferenceTime()`  
-   **Description**: Gets the pure NPU processing time for the most recent inference in microseconds.  
-   **Returns**: The NPU inference time.  
-   **Note**: The legacy function `inference_time()` is deprecated.  
  
***`GetNpuInferenceTimeCnt()`***  
-   **Signature**: `int GetNpuInferenceTimeCnt()`  
-   **Description**: Gets the total count of NPU inference time measurements recorded.  
-   **Returns**: The number of measurements.  
  
***`GetNpuInferenceTimeMean()`***  
-   **Signature**: `double GetNpuInferenceTimeMean()`  
-   **Description**: Gets the mean (average) of all collected NPU inference times.  
-   **Returns**: The mean NPU inference time in microseconds.  
  
***`GetNpuInferenceTimeStdDev()`***  
-   **Signature**: `double GetNpuInferenceTimeStdDev()`  
-   **Description**: Gets the standard deviation of all collected NPU inference times.  
-   **Returns**: The standard deviation of NPU inference time.  
  
***`GetNpuInferenceTimeVector()`***  
-   **Signature**: `std::vector<uint32_t> GetNpuInferenceTimeVector()`  
-   **Description**: Gets a vector of recent NPU inference time measurements.  
-   **Returns**: A vector of NPU inference times in microseconds.  
  
***`GetNumTailTasks()`***  
-   **Signature**: `int GetNumTailTasks()`  
-   **Description**: Returns the number of "tail" tasks in the model, which are tasks that have no subsequent tasks.  
-   **Returns**: The number of tail tasks.  
-   **Note**: The legacy function `get_num_tails()` is deprecated.  
  
***`GetOutputs(void *ptr = nullptr, uint64_t phyAddr = 0)`***  
-   **Signature**: `Tensors GetOutputs(void *ptr = nullptr, uint64_t phyAddr = 0)`  
-   **Description**: Retrieves the output tensors. If `ptr` is null, it returns information about the output memory area within the engine. If `ptr` and `phyAddr` are provided, it returns tensor objects pointing to those addresses.  
-   **Parameters**:  
    -   `ptr`: An optional pointer to a virtual address for the output data.  
    -   `phyAddr`: An optional pointer to a physical address for the output data.  
-   **Returns**: A `Tensors` (vector of `Tensor`) object.  
-   **Note**: The legacy function `outputs(...)` is deprecated.  
  
***`GetOutputSize()`***  
-   **Signature**: `uint64_t GetOutputSize()`  
-   **Description**: Gets the total size of all output tensors combined in bytes.  
-   **Returns**: The total output size as a `uint64_t`.  
-   **Note**: The legacy function `output_size()` is deprecated.  
  
***`GetOutputTensorNames()`***  
-   **Signature**: `std::vector<std::string> GetOutputTensorNames() const`  
-   **Description**: Returns the names of all output tensors in the order they are produced.  
-   **Returns**: A vector of output tensor names.  
  
***`GetOutputTensorOffset(const std::string& tensorName) const`***  
-   **Signature**: `size_t GetOutputTensorOffset(const std::string& tensorName) const`  
-   **Description**: Gets the byte offset for a specific output tensor within the final concatenated output buffer.  
-   **Parameters**:  
    -   `tensorName`: The name of the output tensor.  
-   **Returns**: The offset in bytes.  
  
***`GetOutputTensorSizes()`***  
-   **Signature**: `std::vector<uint64_t> GetOutputTensorSizes()`  
-   **Description**: Gets the individual sizes (in bytes) of each output tensor.  
-   **Returns**: A vector of output tensor sizes, in the order specified by `GetOutputTensorNames()`.  
  
***`GetTaskOrder()`***  
-   **Signature**: `std::vector<std::string> GetTaskOrder()`  
-   **Description**: Gets the model's task execution order.  
-   **Returns**: A vector of strings representing the task order.  
-   **Note**: The legacy function `task_order()` is deprecated.  
  
***`IsMultiInputModel()`***  
-   **Signature**: `bool IsMultiInputModel() const`  
-   **Description**: Checks if the loaded model requires multiple input tensors.  
-   **Returns**: `true` if the model has multiple inputs, `false` otherwise.  
  
***`IsOrtConfigured()`***  
-   **Signature**: `bool IsOrtConfigured()`  
-   **Description**: Checks whether ONNX Runtime (ORT) is configured and available for use.  
-   **Returns**: `true` if ORT is configured, `false` otherwise.  
  
***`IsPPU()`***  
-   **Signature**: `bool IsPPU()`  
-   **Description**: Checks if the loaded model utilizes a Post-Processing Unit (PPU).  
-   **Returns**: `true` if the model uses a PPU, `false` otherwise.  
-   **Note**: The legacy function `is_PPU()` is deprecated.  
  
***`RegisterCallback(std::function<int(TensorPtrs& outputs, void* userArg)> callbackFunc)`***  
-   **Signature**: `void RegisterCallback(std::function<int(TensorPtrs& outputs, void* userArg)> callbackFunc)`  
-   **Description**: Registers a user-defined callback function that will be executed upon completion of an asynchronous inference request.  
-   **Parameters**:  
    -   `callbackFunc`: The function to be called. It receives the output tensors and the user-provided argument.  
-   **Note**: The legacy function `RegisterCallBack(...)` is deprecated.  
  
***`Run(void *inputPtr, void *userArg = nullptr, void *outputPtr = nullptr)`***  
-   **Signature**: `TensorPtrs Run(void *inputPtr, void *userArg = nullptr, void *outputPtr = nullptr)`  
-   **Description**: Performs a synchronous inference for a single input, blocking until the operation is complete.  
-   **Parameters**:  
    -   `inputPtr`: A pointer to the input data.  
    -   `userArg`: An optional user-defined argument.  
    -   `outputPtr`: An optional pointer to a pre-allocated output buffer.  
-   **Returns**: A `TensorPtrs` object containing the output data.  
  
***`Run(const std::vector<void*>& inputBuffers, const std::vector<void*>& outputBuffers, const std::vector<void*>& userArgs = {})`***  
-   **Signature**: `std::vector<TensorPtrs> Run(const std::vector<void*>& inputBuffers, const std::vector<void*>& outputBuffers, const std::vector<void*>& userArgs = {})`  
-   **Description**: Performs a synchronous batch inference.  
-   **Parameters**:  
    -   `inputBuffers`: A vector of pointers to input data for each sample in the batch.  
    -   `outputBuffers`: A vector of pointers to pre-allocated output buffers.  
    -   `userArgs`: An optional vector of user-defined arguments.  
-   **Returns**: A vector of `TensorPtrs`, where each element corresponds to the output of one sample.  
  
***`RunAsync(void *inputPtr, void *userArg=nullptr, void *outputPtr = nullptr)`***  
-   **Signature**: `int RunAsync(void *inputPtr, void *userArg=nullptr, void *outputPtr = nullptr)`  
-   **Description**: Submits a non-blocking, asynchronous inference request for a single input.  
-   **Parameters**:  
    -   `inputPtr`: A pointer to the input data.  
    -   `userArg`: An optional user-defined argument to be passed to the callback.  
    -   `outputPtr`: An optional pointer to a pre-allocated output buffer.  
-   **Returns**: An integer `jobId` for this asynchronous operation.  
  
***`RunAsync(const std::vector<void*>& inputPtrs, void *userArg=nullptr, void *outputPtr = nullptr)`***  
-   **Signature**: `int RunAsync(const std::vector<void*>& inputPtrs, void *userArg=nullptr, void *outputPtr = nullptr)`  
-   **Description**: Submits an asynchronous inference request, automatically detecting if the input is for a multi-input model.  
-   **Parameters**:  
    -   `inputPtrs`: A vector of pointers to input data.  
    -   `userArg`: An optional user-defined argument.  
    -   `outputPtr`: An optional pointer to a pre-allocated output buffer.  
-   **Returns**: An integer `jobId`.  
  
***`RunAsyncMultiInput(const std::map<std::string, void*>& inputTensors, void *userArg=nullptr, void *outputPtr = nullptr)`***  
-   **Signature**: `int RunAsyncMultiInput(const std::map<std::string, void*>& inputTensors, void *userArg=nullptr, void *outputPtr = nullptr)`  
-   **Description**: Submits an asynchronous inference request for a multi-input model using a map of named tensors.  
-   **Parameters**:  
    -   `inputTensors`: A map of tensor names to input data pointers.  
    -   `userArg`: An optional user-defined argument.  
    -   `outputPtr`: An optional pointer to a pre-allocated output buffer.  
-   **Returns**: An integer `jobId`.  
  
***`RunAsyncMultiInput(const std::vector<void*>& inputPtrs, void *userArg=nullptr, void *outputPtr = nullptr)`***  
-   **Signature**: `int RunAsyncMultiInput(const std::vector<void*>& inputPtrs, void *userArg=nullptr, void *outputPtr = nullptr)`  
-   **Description**: Submits an asynchronous inference request for a multi-input model using a vector of input pointers.  
-   **Parameters**:  
    -   `inputPtrs`: A vector of input pointers in the order specified by `GetInputTensorNames()`.  
    -   `userArg`: An optional user-defined argument.  
    -   `outputPtr`: An optional pointer to a pre-allocated output buffer.  
-   **Returns**: An integer `jobId`.  
  
***`RunBenchmark(int num, void* inputPtr = nullptr)`***  
-   **Signature**: `float RunBenchmark(int num, void* inputPtr = nullptr)`  
-   **Description**: Runs a performance benchmark for a specified number of loops.  
-   **Parameters**:  
    -   `num`: The number of inference iterations to run.  
    -   `inputPtr`: An optional pointer to the input data to use for the benchmark.  
-   **Returns**: The average frames per second (FPS) as a float.  
-   **Note**: The legacy function `RunBenchMark(...)` is deprecated.  
  
***`RunMultiInput(const std::map<std::string, void*>& inputTensors, void *userArg=nullptr, void *outputPtr=nullptr)`***  
-   **Signature**: `TensorPtrs RunMultiInput(const std::map<std::string, void*>& inputTensors, void *userArg=nullptr, void *outputPtr=nullptr)`  
-   **Description**: Runs synchronous inference for a multi-input model using a map of named tensors.  
-   **Parameters**:  
    -   `inputTensors`: A map of tensor names to input data pointers.  
    -   `userArg`: An optional user-defined argument.  
    -   `outputPtr`: An optional pointer to a pre-allocated output buffer.  
-   **Returns**: A `TensorPtrs` object containing the output.  
  
***`RunMultiInput(const std::vector<void*>& inputPtrs, void *userArg=nullptr, void *outputPtr=nullptr)`***  
-   **Signature**: `TensorPtrs RunMultiInput(const std::vector<void*>& inputPtrs, void *userArg=nullptr, void *outputPtr=nullptr)`  
-   **Description**: Runs synchronous inference for a multi-input model using a vector of input pointers.  
-   **Parameters**:  
    -   `inputPtrs`: A vector of input pointers in the order specified by `GetInputTensorNames()`.  
    -   `userArg`: An optional user-defined argument.  
    -   `outputPtr`: An optional pointer to a pre-allocated output buffer.  
-   **Returns**: A `TensorPtrs` object containing the output.  
  
***`Wait(int jobId)`***  
-   **Signature**: `TensorPtrs Wait(int jobId)`  
-   **Description**: Blocks execution and waits until the asynchronous request identified by `jobId` is complete.  
-   **Parameters**:  
    -   `jobId`: The job ID returned from a `RunAsync` call.  
-   **Returns**: A `TensorPtrs` object containing the output from the completed job.  
  
---  
  
### class dxrt::InferenceOption  
  
This class specifies inference options applied to an `InferenceEngine`, allowing users to configure which devices and NPU cores are used.  
  
#### Nested Enums  
  
***`enum BOUND_OPTION`***  
-   **Description**: Defines how NPU cores are bound or utilized for inference.  
-   **Members**: `NPU_ALL`, `NPU_0`, `NPU_1`, `NPU_2`, `NPU_01`, `NPU_12`, `NPU_02`.  
  
#### Public Members  
  
-   **`boundOption`**: `uint32_t`. Selects the NPU core(s) to use within a device, using a value from the `BOUND_OPTION` enum. Default is `NPU_ALL`.  
-   **`devices`**: `std::vector<int>`. A list of device IDs to be used for inference. If the list is empty (default), all available devices are used.  
-   **`useORT`**: `bool`. If `true`, both NPU and CPU (via ONNX Runtime) tasks will be executed. If `false`, only NPU tasks will run.  
-   **`bufferCount`**: `int`. Specifies the number of internal buffers allocated for inference. Higher values can improve throughput in pipelined inference scenarios by allowing more concurrent operations, but consume more memory. Default is `DXRT_TASK_MAX_LOAD_VALUE` (typically 6). Valid range is 1-100.
    ```cpp
    dxrt::InferenceOption option;
    option.bufferCount = 8;  // Allocate 8 buffers for higher throughput
    dxrt::InferenceEngine ie("model.dxnn", option);
    ```
  
  

---  
  
### class dxrt::Configuration  
  
A singleton class for managing global application configurations. Access is thread-safe and should be done via the `GetInstance()` method.  
  
#### Nested Enums  
  
***`enum class ITEM`***  
-   **Description**: Defines configuration categories.  
-   **Members**: `DEBUG`, `PROFILER`, `SERVICE`, `DYNAMIC_CPU_THREAD`, `TASK_FLOW`, `SHOW_THROTTLING`, `SHOW_PROFILE`, `SHOW_MODEL_INFO`, `CUSTOM_INTRA_OP_THREADS`, `CUSTOM_INTER_OP_THREADS`.  
  
***`enum class ATTRIBUTE`***  
-   **Description**: Defines attributes for configuration items.  
-   **Members**: `PROFILER_SHOW_DATA`, `PROFILER_SAVE_DATA`, `CUSTOM_INTRA_OP_THREADS_NUM`, `CUSTOM_INTER_OP_THREADS_NUM`.  
  
#### Static Member Functions  
  
***`GetInstance()`***  
-   **Signature**: `static Configuration& GetInstance()`  
-   **Description**: Returns the unique static instance of the `Configuration` class. This is the only way to access the configuration object.  
-   **Returns**: A reference to the `Configuration` instance.  
  
#### Member Functions  
  
***`GetAttribute(const ITEM item, const ATTRIBUTE attrib)`***  
-   **Signature**: `std::string GetAttribute(const ITEM item, const ATTRIBUTE attrib)`  
-   **Description**: Retrieves the value of a specific attribute for a given configuration item.  
-   **Parameters**:  
    -   `item`: The configuration item.  
    -   `attrib`: The attribute to retrieve.  
-   **Returns**: The attribute value as a `std::string`.  
  
***`GetIntAttribute(const ITEM item, const ATTRIBUTE attrib)`***  
-   **Signature**: `int GetIntAttribute(const ITEM item, const ATTRIBUTE attrib)`  
-   **Description**: Retrieves the value of a specific attribute as an integer for a given configuration item. Automatically converts string attributes to integers.  
-   **Parameters**:  
    -   `item`: The configuration item.  
    -   `attrib`: The attribute to retrieve.  
-   **Returns**: The attribute value as an `int`. Returns 0 if the attribute is not found or cannot be parsed as an integer.  
  
***`GetDriverVersion()`***  
-   **Signature**: `std::string GetDriverVersion() const`  
-   **Description**: Retrieves the version of the associated device driver.  
-   **Returns**: The driver version string.  
  
***`GetEnable(const ITEM item)`***  
-   **Signature**: `bool GetEnable(const ITEM item)`  
-   **Description**: Retrieves the enabled status of a specific configuration item.  
-   **Parameters**:  
    -   `item`: The configuration item to check.  
-   **Returns**: `true` if the item is enabled, `false` otherwise.  
  
***`GetFirmwareVersions()`***  
-   **Signature**: `std::vector<std::pair<int, std::string>> GetFirmwareVersions() const`  
-   **Description**: Retrieves the firmware versions of all detected devices.  
-   **Returns**: A vector of pairs, where each pair contains a device ID and its firmware version string.  
  
***`GetONNXRuntimeVersion()`***  
-   **Signature**: `std::string GetONNXRuntimeVersion() const`  
-   **Description**: Retrieves the version of the ONNX Runtime library being used.  
-   **Returns**: The ONNX Runtime version string.  
  
***`GetPCIeDriverVersion()`***  
-   **Signature**: `std::string GetPCIeDriverVersion() const`  
-   **Description**: Retrieves the version of the PCIe driver.  
-   **Returns**: The PCIe driver version string.  
  
***`GetVersion()`***  
-   **Signature**: `std::string GetVersion() const`  
-   **Description**: Retrieves the version of the DXRT library.  
-   **Returns**: The library version string.  
  
***`LoadConfigFile(const std::string& fileName)`***  
-   **Signature**: `void LoadConfigFile(const std::string& fileName)`  
-   **Description**: Loads configuration settings from the specified file.  
-   **Parameters**:  
    -   `fileName`: The path and name of the configuration file.  
  
***`LockEnable(const ITEM item)`***  
-   **Signature**: `void LockEnable(const ITEM item)`  
-   **Description**: Locks a specific configuration item, making it read-only.  
-   **Parameters**:  
    -   `item`: The configuration item to lock.  
  
***`SetAttribute(const ITEM item, const ATTRIBUTE attrib, const std::string& value)`***  
-   **Signature**: `void SetAttribute(const ITEM item, const ATTRIBUTE attrib, const std::string& value)`  
-   **Description**: Sets a specific attribute value for a given configuration item (e.g., setting `PROFILER_SAVE_DATA` to `"ON"`).  
-   **Parameters**:  
    -   `item`: The configuration item.  
    -   `attrib`: The attribute to set.  
    -   `value`: The attribute value as a string.  
  
***`SetEnable(const ITEM item, bool enabled)`***  
-   **Signature**: `void SetEnable(const ITEM item, bool enabled)`  
-   **Description**: Sets the enabled status for a specific configuration item (e.g., enables the profiler).  
-   **Parameters**:  
    -   `item`: The configuration item.  
    -   `enabled`: `true` to enable, `false` to disable.  
  
---  
  
### class dxrt::DeviceStatus  
  
Provides an abstraction for retrieving device information and real-time status.  
  
#### Static Member Functions  
  
***`GetCurrentStatus(int id)`***  
-   **Signature**: `static DeviceStatus GetCurrentStatus(int id)`  
-   **Description**: Retrieves the real-time status for the device with the specified ID.  
-   **Parameters**:  
    -   `id`: The unique identifier of the device.  
-   **Returns**: A `DeviceStatus` object containing the device's current status.  
  
***`GetDeviceCount()`***  
-   **Signature**: `static int GetDeviceCount()`  
-   **Description**: Retrieves the total number of hardware devices currently recognized by the system.  
-   **Returns**: The total number of available devices.  
  
#### Member Functions  
  
***`AllMemoryInfoStr()`***  
-   **Signature**: `std::string AllMemoryInfoStr() const`  
-   **Description**: Retrieves a summary of the device's memory specifications (type, frequency, size) in a single line.  
-   **Returns**: A formatted string.  
  
***`BoardTypeStr()`***  
-   **Signature**: `std::string BoardTypeStr() const`  
-   **Description**: Returns the device board type.  
-   **Returns**: A string such as "SOM" or "M.2".  
  
***`DdrBitErrStr()`***  
-   **Signature**: `std::string DdrBitErrStr() const`  
-   **Description**: Retrieves the count of LPDDR Double-bit & Single-bit Errors.  
-   **Returns**: A formatted string.  
  
***`DdrStatusStr(int ch)`***  
-   **Signature**: `std::string DdrStatusStr(int ch) const`  
-   **Description**: Retrieves the status of a specified LPDDR memory channel.  
-   **Parameters**:  
    -   `ch`: The LPDDR memory channel index (0 to 3).  
-   **Returns**: A formatted string containing the channel status.  
  
***`DeviceTypeStr()`***  
-   **Signature**: `std::string DeviceTypeStr() const`  
-   **Description**: Retrieves the device type as a three-letter abbreviation.  
-   **Returns**: A string ("ACC" for Accelerator or "STD" for Standalone).  
  
***`DeviceTypeWord()`***  
-   **Signature**: `std::string DeviceTypeWord() const`  
-   **Description**: Retrieves the full name of the device type.  
-   **Returns**: A string ("Accelerator" or "Standalone").  
  
***`DeviceVariantStr()`***  
-   **Signature**: `std::string DeviceVariantStr() const`  
-   **Description**: Returns the device chip variant type.  
-   **Returns**: A string such as "L1" or "M1".  
  
***`DmaChannel()`***  
-   **Signature**: `uint64_t DmaChannel() const`  
-   **Description**: Retrieves the number of DMA (Direct Memory Access) channels available for the NPU.  
-   **Returns**: The number of DMA channels.  
  
***`DvfsStateInfoStr()`***  
-   **Signature**: `std::string DvfsStateInfoStr() const`  
-   **Description**: Retrieves the current Dynamic Voltage and Frequency Scaling (DVFS) state of the device.  
-   **Returns**: A formatted string indicating the DVFS state.  
  
***`FirmwareVersionStr()`***  
-   **Signature**: `std::string FirmwareVersionStr() const`  
-   -**Description**: Retrieves the firmware version of the NPU.  
-   **Returns**: The version string (e.g., "1.2.3").  
  
***`GetDeviceType()`***  
-   **Signature**: `DeviceType GetDeviceType() const`  
-   **Description**: Retrieves the device type as a `DeviceType` enum.  
-   **Returns**: A `DeviceType` enum value.  
  
***`GetId()`***  
-   **Signature**: `int GetId() const`  
-   **Description**: Retrieves the unique identifier of the device.  
-   **Returns**: The device ID as an integer.  
  
***`GetInfoString()`***  
-   **Signature**: `std::string GetInfoString() const`  
-   **Description**: Retrieves detailed static information about the device, equivalent to `dxrt-cli -i`.  
-   **Returns**: A formatted string with device specifications.  
  
***`GetNpuClock(int ch)`***  
-   **Signature**: `uint32_t GetNpuClock(int ch) const`  
-   **Description**: Retrieves the current clock frequency of the specified NPU channel.  
-   **Parameters**:  
    -   `ch`: The NPU channel index.  
-   **Returns**: The clock frequency in megahertz (MHz).  
  
***`GetNpuVoltage(int ch)`***  
-   **Signature**: `uint32_t GetNpuVoltage(int ch) const`  
-   **Description**: Retrieves the voltage level of the specified NPU channel.  
-   **Parameters**:  
    -   `ch`: The NPU channel index.  
-   **Returns**: The voltage level in millivolts (mV).  
  
***`GetStatusString()`***  
-   **Signature**: `std::string GetStatusString() const`  
-   **Description**: Retrieves the real-time status of the device, equivalent to `dxrt-cli -s`.  
-   **Returns**: A formatted string with real-time status.  
  
***`GetTemperature(int ch)`***  
-   **Signature**: `int GetTemperature(int ch) const`  
-   **Description**: Retrieves the temperature of the specified NPU channel.  
-   **Parameters**:  
    -   `ch`: The NPU channel index.  
-   **Returns**: The temperature in degrees Celsius.  
  
***`MemoryClock()`***  
-   **Signature**: `uint64_t MemoryClock() const`  
-   **Description**: Retrieves the memory clock frequency of the NPU.  
-   **Returns**: The frequency in megahertz (MHz).  
  
***`MemoryFrequency()`***  
-   **Signature**: `int MemoryFrequency() const`  
-   **Description**: Retrieves the memory operating frequency of the device.  
-   **Returns**: The frequency in megahertz (MHz).  
  
***`MemorySize()`***  
-   **Signature**: `int64_t MemorySize() const`  
-   **Description**: Retrieves the total memory size available for the NPU.  
-   **Returns**: The total memory size in bytes.  
  
***`MemorySizeStrBinaryPrefix()`***  
-   **Signature**: `std::string MemorySizeStrBinaryPrefix() const`  
-   **Description**: Retrieves the total memory size as a string using binary units (e.g., "1.98 GiB").  
-   **Returns**: A formatted string.  
  
***`MemorySizeStrWithComma()`***  
-   **Signature**: `std::string MemorySizeStrWithComma() const`  
-   **Description**: Retrieves the total memory size as a string in bytes, formatted with commas.  
-   **Returns**: A formatted string.  
  
***`MemoryTypeStr()`***  
-   **Signature**: `std::string MemoryTypeStr() const`  
-   **Description**: Retrieves the type of memory used in the device.  
-   **Returns**: A string (e.g., "LPDDR4").  
  
***`NpuStatusStr(int ch)`***  
-   **Signature**: `std::string NpuStatusStr(int ch) const`  
-   **Description**: Retrieves the status of a specific NPU as a formatted string (voltage, clock, temperature).  
-   **Parameters**:  
    -   `ch`: The NPU index.  
-   **Returns**: A formatted string.  
  
***`PcieInfoStr(int spd, int wd, int bus, int dev, int func)`***  
-   **Signature**: `std::string PcieInfoStr(int spd, int wd, int bus, int dev, int func) const`  
-   **Description**: Returns PCIe information (speed, generation, etc.) as a string.  
-   **Parameters**:  
    -   `spd`, `wd`, `bus`, `dev`, `func`: PCIe configuration parameters.  
-   **Returns**: A formatted string with PCIe information.  
  
---  
  
### class dxrt::Tensor  
  
This class abstracts a DXRT tensor object, which defines a data array composed of uniform elements.  
  
#### Constructor  
  
***`Tensor(std::string name_, std::vector<int64_t> shape_, DataType type_, void *data_=nullptr)`***  
-   **Description**: Constructs a Tensor object.  
-   **Parameters**:  
    -   `name_`: The name of the tensor.  
    -   `shape_`: A vector defining the tensor's shape (dimensions).  
    -   `type_`: The tensor's data type.  
    -   `data_`: An optional pointer to the tensor's data.  
  
#### Member Functions  
  
***`data()`***  
-   **Signature**: `void* &data()`  
-   **Description**: Accessor for the tensor's data pointer.  
-   **Returns**: A reference to the void pointer holding the tensor's data.  
  
***`data(int height, int width, int channel)`***  
-   **Signature**: `void* data(int height, int width, int channel)`  
-   **Description**: Gets a pointer to a specific element by its index, assuming NHWC data layout.  
-   **Parameters**:  
    -   `height`: The height index.  
    -   `width`: The width index.  
    -   `channel`: The channel index.  
-   **Returns**: A void pointer to the specified element.  
  
***`elem_size()`***  
-   **Signature**: `uint32_t &elem_size()`  
-   **Description**: Accessor for the size of a single element in the tensor.  
-   **Returns**: A reference to the element size in bytes.  
  
***`name()`***  
-   **Signature**: `const std::string &name() const`  
-   **Description**: Accessor for the tensor's name.  
-   **Returns**: A constant reference to the tensor's name string.  
  
***`phy_addr()`***  
-   **Signature**: `uint64_t &phy_addr()`  
-   **Description**: Accessor for the physical address of the tensor's data.  
-   **Returns**: A reference to the physical address.  
  
***`shape()`***  
-   **Signature**: `std::vector<int64_t> &shape()`  
-   **Description**: Accessor for the tensor's shape.  
-   **Returns**: A reference to the vector defining the tensor's dimensions.  
  
***`size_in_bytes()`***  
-   **Signature**: `uint64_t size_in_bytes() const`  
-   **Description**: Calculates and returns the total size of the tensor's data in bytes based on its shape and element size.  
-   **Returns**: The total size in bytes.  
  
***`type()`***  
-   **Signature**: `DataType &type()`  
-   **Description**: Accessor for the tensor's data type.  
-   **Returns**: A reference to the `DataType` enum.  
  
---  
