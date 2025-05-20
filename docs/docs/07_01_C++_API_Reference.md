## C++ API Reference

### Inference Engine

```

class InferenceEngine
This class provides an abstraction for the runtime inference executor of the user's compiled model. Once the user loads a compiled model into the InferenceEngine, real-time device tasks are managed and scheduled by internal runtime libraries. It supports both synchronous and asynchronous inference modes, depending on the user's request.


InferenceEngine(const std::string& modelPath, InferenceOption& option = DefaultInferenceOption)
   Constructs an InferenceEngine instance.
   parameters:
      [in] modelPath	: Path to the model file.
      [in] option	: Inference options, including device and NPU core settings. 
                        (default: DefaultInferenceOption).


std::vector<TensorPtrs> GetAllTaskOutputs()
   Retrieves the output tensors for all tasks.
   returns:
      A vector containing the output tensors of all tasks.


std::vector<TensorPtrs> get_outputs()
   Retrieves the output tensors for all tasks.
   returns:
      A vector containing the output tensors of all tasks.


std::string GetCompileType()
   Retrieves the compile type of the model.
   returns:
      The compile type of the model as a std::string.


std::string get_compile_type()
   [[deprecated("Use GetCompileType() instead")]]
   Retrieves the compile type of the model.
   returns:
      The compile type of the model as a std::string.


Tensors GetInputs(void* ptr = nullptr, uint64_t phyAddr = 0)
   Retrieves the input tensor.
   This function provides access to the input tensor based on the provided 
   virtual and physical address pointers.
   parameters:
      [in] ptr		: pointer to virtual the address (optional)
      [in] phyAddr	: pointer to physical the address (optional)
   returns:
     If `ptr` is `nullptr`, returns the input memory area in the engine.
     If both `ptr` and `phyAddr` are provided, returns input tensors containing 
     output addresses.


Tensors inputs(void* ptr = nullptr, uint64_t phyAddr = 0)
   [[deprecated("Use GetInputs() instead")]]
   Retrieves the input tensor.
   This function provides access to the input tensor based on the provided 
   virtual and physical address pointers.
   parameters:
      [in] ptr		: pointer to virtual the address (optional)
      [in] phyAddr	: pointer to physical the address (optional)
   returns:
     If `ptr` is `nullptr`, returns the input memory area in the engine.
     If both `ptr` and `phyAddr` are provided, returns input tensors containing 
     output addresses.


std::vector<Tensors> GetInputs(int devId)
   Retrieves the input tensors.
   parameters:
      [in] devId		: Device Id.
   returns:
      A vector of the input tensors.


std::vector<Tensors> inputs(int devId)
   [[deprecated("Use GetInputs() instead")]]
   Retrieves the input tensors.
   parameters:
      [in] devId		: Device Id.
   returns:
      A vector of the input tensors.


uint64_t GetInputSize()
   Retrieves the total size of input tensors.
   This function returns the total memory size required for the input tensors 
   of a single inference operation.
   returns:
      Input size in bytes for one inference.


uint64_t input_size()
   [[deprecated("Use GetInputSize() instead")]]
   Retrieves the total size of input tensors.
   This function returns the total memory size required for the input tensors 
   of a single inference operation.
   returns:
      Input size in bytes for one inference.


int GetLatency()
   Retrieves the most recent inference latency.
   This function returns the latency time taken for the most recent inference operation.
   returns:
      Latency in microseconds


int latency()
   [deprecated("Use GetLatency() instead")]]
   Retrieves the most recent inference latency.
   This function returns the latency time taken for the most recent inference operation.
   returns:
      Latency in microseconds


int GetLatencyCnt()
   Retrieves the number of latency measurements.  
   This function returns the total count of latency measurements. 
   returns:
      The count of latency measurements.


std::vector<int> GetLatencyVector()
   Retrieves a vector of inference latency values.
   returns:
      A vector of inference latency values in microseconds.


double GetLatencyMean()
   Retrieves the average inference latency.  
   This function returns the mean latency of inference execution.
   returns:
      The average inference latency in microseconds.


double GetLatencyStdDev()
   Retrieves the standard deviation of inference latency.  
   This function calculates the variation in inference latency time.
   returns:
      The standard deviation of inference latency in microseconds.


std::string GetModelName()
   Retrieves the name of the model.
   This function returns the model's name.
   returns:
      The name of the model as a string.


std::string name()
   [[deprecated("Use GetModelName() instead")]]
   Retrieves the name of the model.
   This function returns the model's name.
   returns:
      The name of the model as a string.


uint32_t GetNpuInferenceTime()
   Retrieves the most recent inference time.
   This function returns the duration of the most recent inference operation.
   returns:
      Inference time in microseconds.


uint32_t inference_time()
   [[deprecated("Use GetNpuInferenceTime() instead")]]
   Retrieves the most recent inference time.
   This function returns the duration of the most recent inference operation.
   returns:
      Inference time in microseconds.


int GetNpuInferenceTimeCnt()
   Retrieves the number of NPU inference time measurements.  
   This function returns the total count of NPU inference time measurements.
   returns:
      The count of NPU inference time measurements.


std::vector<uint32_t> GetNpuInferenceTimeVector()
   Retrieves a vector of NPU inference time values.
   returns:
      A vector of inference time values in microseconds.


double GetNpuInferenceTimeMean()
   Retrieves the average NPU inference time.  
   This function returns the mean execution time for NPU inference. 
   returns:
      The average NPU inference time in microseconds.


double GetNpuInferenceTimeStdDev()
   Retrieves the standard deviation of NPU inference time.  
   This function returns the standard deviation of the inference time.
   returns:
      The standard deviation of inference time in microseconds.


int GetNumTailTasks()
   Retrieves the number of tail tasks in the model.
   Tail tasks are tasks that do not have any subsequent tasks in the model's task chain.
   This function returns the count of such tail tasks. 
   returns:
      The number of tail tasks.


int get_num_tails()
   [[deprecated("Use GetNumTailTasks() instead")]]
   Retrieves the number of tail tasks in the model.
   Tail tasks are tasks that do not have any subsequent tasks in the model's task chain.
   This function returns the count of such tail tasks. 
   returns:
      The number of tail tasks.


Tensors GetOutputs(void *ptr=nullptr, uint64_t phyAddr=0)
   Retrieves the output tensor.
   parameters:
      [in] ptr		: Pointer to the virtual address.
      [in] phyAddr	: Pointer to the physical address.
   returns:
      If `ptr` is `nullptr`, the output memory area in the engine is returned
      If both `ptr` and `phyAddr` are provided, returns output tensors containing 
      the output addresses.


Tensors outputs(void *ptr=nullptr, uint64_t phyAddr=0)
   [[deprecated("Use GetOutputs() instead")]]
   Retrieves the output tensor.
   parameters:
      [in] ptr		: Pointer to the virtual address.
      [in] phyAddr	: Pointer to the physical address.
   returns:
      If ptr is null, the output memory area in the engine is returned
      If both ptr and phyAddr are provided, returns output tensors containing 
      the output addresses.


uint64_t GetOutputSize()
   Retrieves the total size of output tensors.
   This function returns the total memory size required for the output tensors 
   of a single inference operation.
   returns:
      Output size in bytes for one inference.


uint64_t output_size()
   [[deprecated("Use GetOutputSize() instead")]]
   Retrieves the total size of output tensors.
   This function returns the total memory size required for the output tensors 
   of a single inference operation.
   returns:
      Output size in bytes for one inference.


std::vector<string> GetTaskOrder()
   Retrieves the execution order of tasks in the model.
   This function returns the sequence of tasks as they are executed within the model.
   returns:
      A vector of strings representing the task execution order.


std::vector<string> task_order()
   [[deprecated("Use GetTaskOrder() instead")]]
   Retrieves the execution order of tasks in the model.
   This function returns the sequence of tasks as they are executed within the model.
   returns:
      A vector of strings representing the task execution order.


bool IsPPU()
   Checks whether PPU is utilized.
   returns:
     true if PPU is being utilized, otherwise false.


bool is_PPU()
   [[deprecated("Use IsPPU() instead")]]
   Checks whether PPU is utilized.
   returns:
     true if PPU is being utilized, otherwise false.


void RegisterCallback(std::function<int(TensorPtrs &outputs, void *userArg)> 	callbackFunc)	
   Register user callback function to be called by inference completion.
   parameters:
      [in] callbackFunc : A function that is called when inference is complete.  
                          It receives two arguments: outputs and userArg.
                          outputs : output tensors data
                          userArg : userArg given by RunAsync();



void RegisterCallBack(std::function<int(TensorPtrs &outputs, void *userArg)> 	callbackFunc)	
   [[deprecated("Use RegisterCallback() instead")]]
   Register user callback function to be called by inference completion.
   parameters:
      [in] callbackFunc : A function that is called when inference is complete.  
                          It receives two arguments: outputs and userArg.
                          outputs : output tensors data
                          userArg : userArg given by RunAsync();


TensorPtrs Run(void* inputPtr, void * userArg = nullptr, void* outputPtr = nullptr)
   Runs the inference engine synchronously using the specified input pointer.
   This function executes inference with the given input data and returns the output tensors.
   parameters:
      [in] inputPtr	: Pointer to the input data for inference.
      [in] userArg	: User-defined arguments as a void pointer (optional).
                        (e.g. original frame data, metadata about input, etc.)
      [out] outputPtr	: Pointer to store the output data. If `nullptr`, the output data is 
                         stored in an internal buffer.
   returns:
      Output tensors as a vector of smart pointer instances.


int RunAsync(void* inputPtr, void * userArg = nullptr, void* outputPtr = nullptr)
   Initiates an asynchronous inference request and returns a job ID.
   This function performs a non-blocking inference operation using the specified input
   pointer. It returns a job ID that can be used with the `wait()` function to retrieve 
   the results.
   parameters:
      [in] inputPtr	: Pointer to the input data for inference.
      [in] userArg	: User-defined arguments as a void pointer (optional).
                        (e.g. original frame data, metadata about input, etc.)
      [out] outputPtr	: Pointer to store the output data. If `nullptr`, the output data is 
                         stored in an internal buffer.
   returns:
      Job ID that can be used with the `wait()` function to retrieve the inference result.


float RunBenchmark(int num, void* inputPtr = nullptr)
   Runs a benchmark test by executing inference multiple times.
   This function performs inference in a loop for the specified number of times 
   and calculates the average frames per second (FPS).
   parameters:
      [in] num		: Number of inference iterations.
      [in] inputPtr	: Pointer to the input data for inference (optional). 
                         If `nullptr`, default input data is used.
   returns:
      Average FPS (frames per second) over the benchmark runs.


float RunBenchMark(int num, void* inputPtr = nullptr)
   [[deprecated("Use RunBenchmark() instead")]]
   Runs a benchmark test by executing inference multiple times.
   This function performs inference in a loop for the specified number of times 
   and calculates the average frames per second (FPS).
   parameters:
      [in] num		: Number of inference iterations.
      [in] inputPtr	: Pointer to the input data for inference (optional). 
                         If `nullptr`, default input data is used.
   returns:
      Average FPS (frames per second) over the benchmark runs.


TensorPtrs Wait(int jobId)
   Waits for the completion of an asynchronous inference request.
   This function blocks execution until the specified inference job, identified by `jobId`, 
   is complete and then returns the output tensors.   
   parameters:
      [in] jobId	: Job ID returned by `RunAsync()`, used to track the inference request.
   returns:
      Output tensors as a vector of smart pointer instances.
```

### Inference Option

```

class InferenceOption
This struct specifies inference options applied to dxrt::InferenceEngine.
User can configure which npu device is used to inference.

enum BOUND_OPTION {
        NPU_ALL = 0,
        NPU_0,
        NPU_1, 
        NPU_2
    };

uint32_t boundOption = BOUND_OPTION::NPU_ALL
   variables:
   Select the NPU core inside the device.
   NPU_ALL is an option that uses all NPU cores simultaneously. NPU_0, NPU_1, and NPU_2 are   
   options that allow using only a single NPU core.


std::vector< int> devices = {}
   variables:
   device ID list to use
   make a list which contains a list of device ID to use. if it is empty
   (or use default value), 
   then all devices are used. list of device ID to use (it is empty by default, then all 
   devices are used.)
```

### Profiler

```

class Profiler
Provides a time measurement API based on timestamps.
The `Profiler` class is used to measure execution time using timestamps,
enabling performance analysis of various operations.

void Add(const std::string& event)
   Registers an event in the profiler.
   This function records an event with the specified name. If the profiler is used 
   in a multi-threaded environment, this function should be called first to ensure 
   proper event tracking.
   parameters:
      [in] event	: Name of the event to be registered.


void AddTimePoint(const std::string& event, TimePointPtr tp)
   Adds a timing data point for the specified event.
   This function records a time point associated with a given event name, 
   allowing precise measurement of execution timing.
   parameters:
      [in] event	: Name of the event to associate with the time point.
      [in] tp		: Pointer to the timing data.


void End(const std::string& event)
   Records the end point of a specified event.
   This function marks the completion of an event, allowing for 
   measurement of the event's duration when used with corresponding start points.
   parameters:
      [in] event	: Name of the event to mark as completed.


void Erase(const std::string& event)
   Clears the timing data of a specified event.
   parameters:
      [in] event	: Name of the event whose timing data should be cleared.


uint64_t Get(const std::string& event)
   Retrieves the most recent elapsed time of a specified event.
   This function returns the elapsed time (in microseconds) for the given event, 
   based on the most recent recorded start and end points.
   parameters:
      [in] event	: Name of the event for which the elapsed time is requested.
   returns:
      Elapsed time in microseconds.


double GetAverage(const std::string& event)
   Retrieves the average elapsed time of a specified event.
   This function returns the average elapsed time (in microseconds) for the given event, 
   calculated from all recorded start and end points.
   parameters:
      [in] event	: Name of the event for which the average elapsed time is requested.
   returns:
      Average elapsed time in microseconds.


static Profiler& GetInstance()
   Retrieves the singleton instance of the Profiler.
   This function returns a reference to the pre-created singleton instance of 
   the `Profiler`. Users should not create their own instance.
   returns:
      A reference to the singleton instance of `dxrt::Profiler`.


void Save(const std::string& file)
   Saves the timing data of all events to a specified file.
   This function exports all recorded timing data for each event and saves it to the 
   given file. The data can be used for further analysis or reporting.
   parameters:
      [in] file		: Name of the file where the timing data will be saved.


void Show(bool 	showDurations = false)
   Displays the elapsed times for all events.
   This function prints the elapsed times for each event. If `showDurations` is set to true, 
   the durations of each event will also be shown.
   parameters:
      [in] showDurations : If true, displays the durations of each event.
                           If false, only the elapsed times are shown.


void Start(const std::string& event)
   Records the start point of a specified event.
   This function marks the beginning of an event, allowing for the calculation of 
   its duration when paired with an end point.
   parameters:
      [in] event	: Name of the event to mark as started.
```

### Tensor

```
class Tensor
Represents a DX-RT tensor object, which defines a data array composed of uniform elements.
The `Tensor` class abstracts a tensor object that holds a multi-dimensional array of data 
elements, typically used in machine learning models. This tensor is generally connected to 
inference engine objects for computations.

void* data(int 	height, int width, int channel)
   Retrieves a pointer to a specific element in the tensor by its indices.
   This function returns the address of the element at the specified indices 
   (height, width, channel) for a tensor in NHWC (height, width, channel) data format.
   parameters:
      [in] height	: The height index of the desired element.
      [in] width	: The width index of the desired element.
      [in] channel	: The channel index of the desired element.
   returns:
      A pointer to the specified tensor element at the given indices (height, width, channel).

```

---
