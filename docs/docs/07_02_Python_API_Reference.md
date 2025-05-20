## Python API Reference

### Inference Engine

```

class InferenceEngine
This class provides an abstraction for the runtime inference executor of the user's compiled model. Once the user loads a compiled model into the InferenceEngine, real-time device tasks are managed and scheduled by internal runtime libraries. It supports both synchronous and asynchronous inference modes, depending on the user's request.

InferenceEngine(model_path: str, inference_option=None)
   Constructs an InferenceEngine instance.
   parameters:
      [in] model_path		: Path to the model file.
      [in] inference_option 	: Inference options, including device and NPU core settings. 
                                (default: DefaultInferenceOption).


get_all_task_outputs()
   Retrieves the outputs from all tasks in sequence.
   returns:
      The outputs from all tasks in order.


get_outputs()
   @deprecated: Use `get_all_task_outputs()` instead.
   Retrieves the outputs from all tasks in sequence.
   returns:
       The outputs from all tasks in order.


get_compile_type()
   Retrieves the compile type of the model.
   returns:
      The compile type of the model.


get_input_data_type()
   Retrieves the required output data types as a list of strings.
   returns:
      A list of strings representing the input data types.


input_dtype()
   @deprecated: Use `get_input_data_type()` instead.
   Retrieves the required output data types as a list of strings.
   returns:
      A list of strings representing the input data types.


get_input_size()
   Retrieves the total size of input tensors.
   This function returns the total memory size required for the input tensors 
   of a single inference operation.
   returns:
      Input size in bytes for one inference.


input_size()
   @deprecated: Use `get_input_size()` instead.
   Retrieves the total size of input tensors.
   This function returns the total memory size required for the input tensors 
   of a single inference operation.
   returns:
      Input size in bytes for one inference.


get_latency()
   Retrieves the most recent inference latency.
   This function returns the latency taken for the most recent inference operation.
   returns:
      Latency in microseconds.


latency()
   @deprecated: Use `get_latency()` instead.
   Retrieves the most recent inference latency.
   This function returns the latency taken for the most recent inference operation.
   returns:
      Latency in microseconds.


get_latency_count()
   Retrieves the number of latency measurements.  
   This function returns the total count of latency measurements. 
   returns:
      The count of latency measurements.


get_latency_list()
   Retrieves a list of inference latency values.
   returns:
      A list of inference latency values in microseconds.


get_latency_mean()
   Retrieves the average inference latency.  
   This function returns the mean latency of inference execution.
   returns:
      The average inference latency in microseconds.


get_latency_std()
   Retrieves the standard deviation of inference latency.  
   This function calculates the variation in inference latency time.
   returns:
      The standard deviation of inference latency in microseconds.


get_npu_inference_time()
   Retrieves the most recent inference time.
   This function returns the duration of the most recent inference execution.
   returns:
      Inference time in microseconds.


inference_time()
   @deprecated: Use `get_npu_inference_time()` instead.
   Retrieves the most recent inference time.
   This function returns the duration of the most recent inference execution.
   returns:
      Inference time in microseconds.


get_npu_inference_time_count()
   Retrieves the number of NPU inference time measurements.  
   This function returns the total count of NPU inference time measurements.
   returns:
      The count of NPU inference time measurements.


get_npu_inference_time_list()
   Retrieves a list of NPU inference time values.
   returns:
      A list of inference time values in microseconds.


get_npu_inference_time_mean()
   Retrieves the average NPU inference time.  
   This function returns the mean execution time for NPU inference. 
   returns:
      The average NPU inference time in microseconds.


get_npu_inference_time_std()
   Retrieves the standard deviation of NPU inference time.  
   This function returns the standard deviation of the inference time.
   returns:
      The standard deviation of inference time in microseconds.


get_num_tail_tasks()
   Retrieves the number of tail tasks in the model.
   Tail tasks are tasks that do not have any subsequent tasks in the model's task chain.
   This function returns the count of such tail tasks. 
   returns:
      The number of tail tasks.


get_num_tails()
   @deprecated: Use `get_num_tail_tasks()` instead.
   Retrieves the number of tail tasks in the model.
   Tail tasks are tasks that do not have any subsequent tasks in the model's task chain.
   This function returns the count of such tail tasks. 
   returns:
      The number of tail tasks.


get_output_data_type()
   @deprecated: Use `get_output_data_type()` instead.
   Retrieves the required output data types as a list of strings.
   returns:
      A list of strings representing the output data types.


output_dtype()
   Retrieves the required output data types as a list of strings.
   returns:
      A list of strings representing the output data types.


get_output_size()
   Retrieves the total size of output tensors.
   This function returns the total memory size required for the output tensors 
   of a single inference operation.
   returns:
     Output size in bytes for one inference.


output_size()
   @deprecated: Use `get_output_size()` instead.
   Retrieves the total size of output tensors.
   This function returns the total memory size required for the output tensors 
   of a single inference operation.
   returns:
     Output size in bytes for one inference.


get_task_order()
   Retrieves the execution order of tasks in the model.
   This function returns the sequence of tasks as they are executed within the model.
   returns:
      A vector of strings representing the task execution order.


task_order()
   @deprecated: Use `get_task_order()` instead.
   Retrieves the execution order of tasks in the model.
   This function returns the sequence of tasks as they are executed within the model.
   returns:
      A vector of strings representing the task execution order.


is_ppu()
   Checks whether PPU is utilized.
   returns:
     True if PPU is being utilized, otherwise False.


is_PPU()
   @deprecated: Use `is_ppu()` instead.
   Checks whether PPU is utilized.
   returns:
     True if PPU is being utilized, otherwise False.


register_callBack(callback)
   Register user callback function to be called by inference completion.
   parameters:
      [in] callback : A function that is called when inference is complete.  
                      It receives two arguments: outputs and user_arg.
                      - outputs: The data from the output tensors.  
                      - user_arg: A user-defined argument passed via RunAsync().


RegisterCallBack(callback)
   @deprecated: Use `register_callback()` instead.
   Register user callback function to be called by inference completion.
   parameters:
      [in] callback : A function that is called when inference is complete.  
                      It receives two arguments: outputs and user_arg.
                      - outputs: The data from the output tensors.  
                      - user_arg: A user-defined argument passed via RunAsync().


run(input_feed_list: List[np.ndarray])
   Runs the inference engine synchronously using the specified input data.  
   This function executes inference with the provided input data and returns
   the output tensors.
   parameters:
      [in] input_feed_list	: A list of Numpy arrays representing the input data.
   returns:
      A list of Numpy arrays representing the output tensors.


Run(input_feed_list: List[np.ndarray])
   @deprecated: Use run() instead.
   Runs the inference engine synchronously using the specified input data.  
   This function executes inference with the provided input data and returns
   the output tensors.
   parameters:
      [in] input_feed_list	: A list of Numpy arrays representing the input data.
   returns:
      A list of Numpy arrays representing the output tensors.


run_async(input_feed_list: List[np.ndarray], user_arg)
   Initiates an asynchronous inference request and returns a job ID.
   This function performs a non-blocking inference operation using the specified input
   data. It returns a job ID that can be used with the `wait()` function to retrieve 
   the results.
   parameters:
      [in] input_feed_list	: input data for inference.
      [in] user_arg		: user-defined arguments.
                        	(e.g. original frame data, metadata about input, ... )
   returns:
      Job ID that can be used with the `wait()` function to retrieve the inference result.


RunAsync(input_feed_list: List[np.ndarray], user_arg)
   @deprecated: Use `run_async()` instead.
   Initiates an asynchronous inference request and returns a job ID.
   This function performs a non-blocking inference operation using the specified input
   data. It returns a job ID that can be used with the `wait()` function to retrieve 
   the results.
   parameters:
      [in] input_feed_list	: input data for inference.
      [in] user_arg		: user-defined arguments.
                        	(e.g. original frame data, metadata about input, ... )
   returns:
      Job ID that can be used with the `wait()` function to retrieve the inference result.


run_benchmark(loop_cnt,input_feed_list)
   Runs a benchmark test by executing inference multiple times.
   This function performs inference in a loop for the specified number of times 
   and calculates the average frames per second (FPS).
   parameters:
      [in] loop_cnt		: Number of inference iterations.
      [in] input_feed_list	: the input data for inference
   returns:
      Average FPS (frames per second) over the benchmark runs.


RunBenchMark(loop_cnt,input_feed_list)
   @deprecated: Use `run_benchmark()` instead.
   Runs a benchmark test by executing inference multiple times.
   This function performs inference in a loop for the specified number of times 
   and calculates the average frames per second (FPS).
   parameters:
      [in] loop_cnt		: Number of inference iterations.
      [in] input_feed_list	: the input data for inference
   returns:
      Average FPS (frames per second) over the benchmark runs.


wait(int jobId)
   Wait until a request is complete and returns output.
   parameters:
      [in] jobId	: job Id returned by run_async()
   returns:
      output tensors as vector


wait(int jobId)
   @deprecated: Use `wait()` instead.
   Wait until a request is complete and returns output.
   parameters:
      [in] jobId	: job Id returned by RunAsync()
   returns:
      output tensors as vector

```

---
