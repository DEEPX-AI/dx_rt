This section offers Python tutorials demonstrating inference, configuration, and multi-input handling using the DXRT SDK. These examples are ideal for quick prototyping and integration into Python-based AI workflows.

---

### run (Synchronous)

The synchronous Run method uses a single NPU core to perform inference in a blocking manner. It can be configured to utilize multiple NPU cores simultaneously by employing threads to run each core independently. (Refer to **Figure** in **Section. Inference Workflow**)  

**Inference Engine Run (Python)**  

***`run_sync_model.py`***

```
# DX-RT importes
from dx_engine import InferenceEngine
...


if __name__ == "__main__":
    ...    

    # create inference engine instance with model
    ie = InferenceEngine(modelPath)

    input = [np.zeros(ie.GetInputSize(), dtype=np.uint8)]

    # inference loop
    for i in range(loop_count):

        # inference synchronously 
        # use only one npu core 
        outputs = ie.run(input)

        # post processing 
        postProcessing(outputs)

    exit(0)
```

-----

### run_async (Asynchronous)

The asynchronous Run mode is a method that performs inference asynchronously while utilizing multiple NPU cores simultaneously. It can be implemented to maximize NPU resources through a callback function or a thread wait mechanism. 

Inference Engine RunAsync, Callback, User Argument  

- the outputs are guaranteed to be valid **only** within this callback function  
- processing this callback functions as quickly as possible is beneficial for improving inference performance  
- inference asynchronously, use all npu cores  
- if `device-load >= max-load-value`, this function will block  

The following is an example of asynchronous inference using a callback function. A user argument can be used to synchronize the input with the output of the callback.  

**Inference Engine RunAsync, Callback, User Argument (Python)**

***`run_async_model.py`***

```
from dx_engine import InferenceEngine
...

q = queue.Queue()
gLoopCount = 0

lock = threading.Lock()

def onInferenceCallbackFunc(outputs, user_arg):

    # the outputs are guaranteed to be valid only within this callback function
    # processing this callback functions as quickly as possible is beneficial 
    # for improving inference performance

    global gLoopCount

    # Mutex locks should be properly adjusted 
    # to ensure that callback functions are thread-safe.
    with lock:
        # user data type casting
        index, loop_count = user_arg
    

        # post processing
        #postProcessing(outputs);

        # something to do

        print("Inference output (callback) index=", index)

        gLoopCount += 1
        if ( gLoopCount == loop_count ) :
            print("Complete Callback")
            q.put(0)

    return 0


if __name__ == "__main__":

    ...    

    # create inference engine instance with model
    ie = InferenceEngine(modelPath)

    # register call back function
    ie.register_callback(onInferenceCallbackFunc)


    input = [np.zeros(ie.GetInputSize(), dtype=np.uint8)]

    # inference loop
    for i in range(loop_count):

        # inference asynchronously, use all npu cores
        # if device-load >= max-load-value, this function will block  
        ie.run_async(input, user_arg=[i, loop_count])

        print("Inference start (async)", i)

    exit(q.get())
```

The following is an example where multiple threads start input and inference, and a single callback processes the output.

Inference Engine RunAsync, Callback, User Argument, Thread  

- the outputs are guaranteed to be valid **only** within this callback function  
- processing this callback functions as quickly as possible is beneficial for improving inference performance  
- inference asynchronously, use all npu cores  
- if `device-load >= max-load-value`, this function will block  

**Inference Engine run_async, Callback, User Argument, Thread (Python)**

***`run_async_model_thread.py`***

```
from dx_engine import InferenceEngine
...

THRAD_COUNT = 3
total_count = 0
q = queue.Queue()

lock = threading.Lock()


def inferenceThreadFunc(ie, threadIndex, loopCount):

    # input
    input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]
    
    # inference loop
    for i in range(loopCount):

        # inference asynchronously, use all npu cores
        # if device-load >= max-load-value, this function will block  
        ie.run_async(input,user_arg = [i, loopCount, threadIndex])
    
    return 0

def onInferenceCallbackFunc(outputs, user_arg):
    # the outputs are guaranteed to be valid only within this callback function
    # processing this callback functions as quickly as possible is beneficial 
    # for improving inference performance

    global total_count

    # Mutex locks should be properly adjusted 
    # to ensure that callback functions are thread-safe.
    with lock:
        # user data type casting
        index = user_arg[0]
        loop_count = user_arg[1]
        thread_index = user_arg[2]

        # post processing
        #postProcessing(outputs);

        # something to do

        total_count += 1

        if ( total_count ==  loop_count * THRAD_COUNT) :
            q.put(0)

    return 0


if __name__ == "__main__":
    ...    

    # create inference engine instance with model
    ie = InferenceEngine(modelPath)

    # register call back function
    ie.register_callback(onInferenceCallbackFunc)

   
    t1 = threading.Thread(target=inferenceThreadFunc, args=(ie, 0, loop_count))
    t2 = threading.Thread(target=inferenceThreadFunc, args=(ie, 1, loop_count))
    t3 = threading.Thread(target=inferenceThreadFunc, args=(ie, 2, loop_count))

    # Start and join
    t1.start()
    t2.start()
    t3.start()


    # join
    t1.join()
    t2.join()
    t3.join()
        

    exit(q.get())
```

The following is an example of performing asynchronous inference by creating an inference wait thread. The main thread starts input and inference, and the inference wait thread retrieves the output data corresponding to the input.

Inference Engine run_async, wait  

- inference asynchronously, use all npu cores  
- if `device-load >= max-load-value`, this function will block  

**Inference Engine run_async, wait (Python)**

***`run_async_model_wait.py`***

```
# DX-RT imports
from dx_engine import InferenceEngine
...

q = queue.Queue()


def inferenceThreadFunc(ie, loopCount):

    count = 0

    while(True):
    
        # pop item from queue 
        jobId = q.get()

        # waiting for the inference to complete by jobId
        # ownership of the outputs is transferred to the user 
        outputs = ie.wait(jobId)

        # post processing
        # postProcessing(outputs);

        # something to do

        count += 1
        if ( count >= loopCount ):
            break
   
    return 0


if __name__ == "__main__":
    ...

    # create inference engine instance with model
    with InferenceEngine(modelPath) as ie:

        # do not register call back function
        # ie.register_callback(onInferenceCallbackFunc)

        t1 = threading.Thread(target=inferenceThreadFunc, args=(ie, loop_count))

        t1.start()

        input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]

        # inference loop
        for i in range(loop_count):

            # inference asynchronously, use all npu cores
            # if device-load >= max-load-value, this function will block  
            jobId = ie.run_async(input, user_arg=0)

            q.put(jobId)

        t1.join()

    exit(0)
    
```

-----

### run (Batch)
The following is an example of batch inference with multiple inputs and multiple outputs.

***`run_batch_model.py`***

```
   
import numpy as np
import sys
from dx_engine import InferenceEngine
from dx_engine import InferenceOption


if __name__ == "__main__":
    ...
   
    # create inference engine instance with model
    with InferenceEngine(modelPath) as ie:

        input_buffers = []
        output_buffers = []
        index = 0
        for b in range(batch_count):
            input_buffers.append([np.array([np.random.randint(0, 255)],  dtype=np.uint8)])
            output_buffers.append([np.zeros(ie.get_output_size(), dtype=np.uint8)])
            index = index + 1

        # inference loop
        for i in range(loop_count):

            # batch inference
            # It operates asynchronously internally 
            # for the specified number of batches and returns the results
            results = ie.run(input_buffers, output_buffers)

            # post processing 

    exit(0)

```

-----

### Inference Option

The following inference options allow you to specify an NPU core for performing inference.

Inference Engine Run, Inference Option  

- select devices  
  : default device is `[]`  
  : Choose the device to utilize  (ex. `[0, 2]`)  
- select bound option per device  
  : `InferenceOption.BOUND_OPTION.NPU_ALL`  
  : `InferenceOption.BOUND_OPTION.NPU_0`  
  : `InferenceOption.BOUND_OPTION.NPU_1`  
  : `InferenceOption.BOUND_OPTION.NPU_2` 
  : `InferenceOption.BOUND_OPTION.NPU_01`  
  : `InferenceOption.BOUND_OPTION.NPU_12`  
  : `InferenceOption.BOUND_OPTION.NPU_02`    
- use onnx runtime library (`ORT`)  
  : `set_use_ort / get_use_ort`  

NPU_ALL / NPU_0 / NPU_1 / NPU_2
```
# DX-RT imports
from dx_engine import InferenceEngine, InferenceOption
...

if __name__ == "__main__":
    ...
    
    # inference option
    option = InferenceOption()

    print("Inference Options:")

    # select devices
    option.devices = [0]

    # NPU bound opion (NPU_ALL or NPU_0 or NPU_1 or NPU_2)
    option.bound_option = InferenceOption.BOUND_OPTION.NPU_ALL

    # use ONNX Runtime (True or False)
    option.use_ort = False
   
    # create inference engine instance with model
    with InferenceEngine(modelPath, option) as ie:

        input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]

        # inference loop
        for i in range(loop_count):

            # inference synchronously 
            # use only one npu core 
            # ownership of the outputs is transferred to the user 
            outputs = ie.run(input)

            # post processing 
            #postProcessing(outputs)
            print("Inference outputs ", i)

    exit(0)
```

---

### Configuration and DeviceStatus

This guide explains how to use the `Configuration` class to set up the inference engine and the `DeviceStatus` class to monitor hardware status.

#### Engine Configuration

The `Configuration` class allows you to set engine parameters and retrieve version information before running inference.

```python
# Create a configuration object
config = Configuration()

# Enable options like showing model details or profiling information
config.set_enable(Configuration.ITEM.SHOW_MODEL_INFO, True)
config.set_enable(Configuration.ITEM.SHOW_PROFILE, True)

# Retrieve version information
logger.info('Runtime framework version: ' + config.get_version())
logger.info('Device driver version: ' + config.get_driver_version())
```

  - **`Configuration()`**: Creates an object to manage engine settings.
  - **`config.set_enable(...)`**: Turns specific engine features on or off. In this case, it enables printing model information and performance profiles upon loading.
  - **`config.get_version()`**: Fetches read-only information, such as software and driver versions.

#### Querying Device Status

The `DeviceStatus` class is used to get the real-time operational status of the NPU hardware, such as temperature and clock speed. This is typically done after inference is complete to check the hardware's state.

```python
# Get the number of available devices
device_count = DeviceStatus.get_device_count()

# Loop through each device
for i in range(device_count):
    # Get a status snapshot for the current device
    device_status = DeviceStatus.get_current_status(i)
    logger.info(f'Device {device_status.get_id()}')

    # Loop through each NPU core to get its metrics
    for c in range(3): # Assuming 3 cores for this example
        logger.info(
            f'   NPU Core {c} '
            f'Temperature: {device_status.get_temperature(c)} '
            f'Voltage: {device_status.get_npu_voltage(c)} '
            f'Clock: {device_status.get_npu_clock(c)}'
        )
```

  - **`DeviceStatus.get_device_count()`**: A static method that returns the number of connected DEEPX devices.
  - **`DeviceStatus.get_current_status(i)`**: Returns a status object containing a **snapshot** of the hardware metrics for device `i` at that moment.
  - **`device_status.get_temperature(c)`**: An instance method that returns the temperature (in Celsius) for a specific NPU core `c`. The `get_npu_voltage` and `get_npu_clock` methods work similarly.

---

### Profiler Configuration

This guide provides a simple, code-focused manual on how to configure the profiler using the DXRT Python wrapper. The profiler is a powerful tool for analyzing the performance of each layer within your model.  

Configuration is managed through an instance of the `Configuration` class.  

#### Enabling the Profiler

Before you can use any profiler features, you **must** first create a `Configuration` object and enable the profiler. This is the essential first step.

```python
# Create a Configuration instance
config = Configuration()

# Enable the profiler feature
config.set_enable(Configuration.ITEM.PROFILER, True)
```

  - **`config = Configuration()`**: Creates the object that controls system-wide settings for the runtime.
  - **`set_enable()`**: This method activates or deactivates a specific DXRT feature.
  - **`Configuration.ITEM.PROFILER`**: Specifies that the target feature is the profiler.
  - **`True`**: Enables the profiler. Set to `False` to disable it.

#### Configuration Options

Once enabled, you can set specific attributes for the profiler's behavior using the same `config` object.

***Displaying Profiler Data in the Console***

To see the profiling results printed directly to your console after the inference runs, use the `PROFILER_SHOW_DATA` attribute.

```python
# Configure the profiler to print its report to the console
config.set_attribute(Configuration.ITEM.PROFILER,
                     Configuration.ATTRIBUTE.PROFILER_SHOW_DATA, "ON")
```

  - **`set_attribute()`**: Sets a specific property for a DXRT feature.
  - **`PROFILER_SHOW_DATA`**: The attribute to control console output.
  - **`"ON"`**: A string value to enable this attribute. Use `"OFF"` to disable it.

***Saving Profiler Data to a File***

To save the profiling report to a file for later analysis, use the `PROFILER_SAVE_DATA` attribute. The resulting report is generated in the same folder with the name **`profiler.json`**. 📄

```python
# Configure the profiler to save its report to a file
config.set_attribute(Configuration.ITEM.PROFILER,
                     Configuration.ATTRIBUTE.PROFILER_SAVE_DATA, "ON")
```

  - **`PROFILER_SAVE_DATA`**: The attribute to control file output.
  - **`"ON"`**: A string value to enable file saving. Use `"OFF"` to disable it.

#### Complete Code Example

Here is a complete example showing how to apply all the configurations at the start of your script. These settings are applied globally, and any `InferenceEngine` instance created afterward will automatically use them.

```python
if __name__ == "__main__":

    # Step 1: Create a Configuration instance and enable the profiler
    config = Configuration()
    config.set_enable(Configuration.ITEM.PROFILER, True)

    # Step 2: Set attributes to show data in console and save to a file
    config.set_attribute(Configuration.ITEM.PROFILER,
                         Configuration.ATTRIBUTE.PROFILER_SHOW_DATA, "ON")
    config.set_attribute(Configuration.ITEM.PROFILER,
                         Configuration.ATTRIBUTE.PROFILER_SAVE_DATA, "ON")

    # The configuration is now active.
    # ...
    
    # Create an inference engine instance that will now be profiled
    with InferenceEngine(modelPath) as ie:
        # ... register callback and run inference loop ...
```

---

### Multi-input Inference

This guide explains various methods for performing inference on multi-input models using the `InferenceEngine`. The examples cover different input formats, synchronous and asynchronous execution, and batch processing.

#### Model Information

Before running inference, it's useful to inspect the model's properties. The `print_model_info` function in the example script shows how to query the inference engine for details about the model's input and output tensors.

  - **`ie.is_multi_input_model()`**: Checks if the loaded model has multiple inputs.
  - **`ie.get_input_tensor_count()`**: Gets the number of input tensors.
  - **`ie.get_input_tensor_names()`**: Retrieves the names of all input tensors.
  - **`ie.get_input_tensor_sizes()`**: Gets the size (in bytes) of each input tensor.
  - **`ie.get_output_tensor_names()` / `ie.get_output_tensor_sizes()`**: Provide similar information for output tensors.

<!-- end list -->

```python
def print_model_info(ie: InferenceEngine) -> None:
    if ie.is_multi_input_model():
        print(f"Input tensor count: {ie.get_input_tensor_count()}")
        input_names = ie.get_input_tensor_names()
        input_sizes = ie.get_input_tensor_sizes()
        for i, name in enumerate(input_names):
            print(f"  {name}: {input_sizes[i]} bytes")
```

#### Synchronous Single Inference

These examples demonstrate different ways to run a single inference request synchronously.

***Input Formats***

**A.** Dictionary Format (`Dict[str, np.ndarray]`)

This is the most robust method. You provide a dictionary where keys are the tensor names and values are the `numpy` arrays. This format is **not** sensitive to the order of tensors.

  - **API**: `ie.run_multi_input(input_tensors)`
  - **Use Case**: Recommended for clarity and to avoid errors from tensor reordering.

<!-- end list -->

```python
# Create input data
input_names = ie.get_input_tensor_names()
input_sizes = ie.get_input_tensor_sizes()
input_tensors = {name: create_dummy_input(size) for name, size in zip(input_names, input_sizes)}

# Run inference
outputs = ie.run_multi_input(input_tensors)
```

**B.** List Format (`List[np.ndarray]`)

You provide a list of `numpy` arrays. The order of arrays in the list **must** match the order returned by `ie.get_input_tensor_names()`.

  - **API**: `ie.run(input_list)`
  - **Use Case**: When tensor order is known and fixed. Can be slightly more performant than the dictionary-based approach due to less overhead.

<!-- end list -->

```python
# Create input data in the correct order
input_sizes = ie.get_input_tensor_sizes()
input_list = [create_dummy_input(size) for size in input_sizes]

# Run inference
outputs = ie.run(input_list)
```

**C.** Auto-Split Concatenated Buffer

You provide a single, contiguous `numpy` array containing all input data concatenated together. The engine automatically splits this buffer into the correct tensor inputs based on their sizes. The concatenation order **must** match the order from `ie.get_input_tensor_names()`.

  - **API**: `ie.run(concatenated_input)`
  - **Use Case**: Efficient when input data is already in a single block or when interfacing with systems that provide data this way.

<!-- end list -->

```python
# Create a single buffer with all input data concatenated
total_input_size = ie.get_input_size()
concatenated_input = create_dummy_input(total_input_size)

# Run inference
outputs = ie.run(concatenated_input)
```

***Output Buffer Management***

For each synchronous method, you can either let the engine allocate output memory automatically or provide pre-allocated buffers for performance gains.

  - **Auto-Allocated Output (No Buffer Provided)**: Simpler to use. The engine returns a new list of `numpy` arrays.

    ```python
    # Engine allocates and manages output memory
    outputs = ie.run_multi_input(input_tensors)
    ```

  - **User-Provided Output Buffers**: More performant as it avoids repeated memory allocations. The user is responsible for creating a list of `numpy` arrays with the correct sizes.

    ```python
    # User creates the output buffers
    output_sizes = ie.get_output_tensor_sizes()
    output_buffers = [np.zeros(size, dtype=np.uint8) for size in output_sizes]

    # Run inference, placing results in the provided buffers
    outputs = ie.run_multi_input(input_tensors, output_buffers=output_buffers)
    ```

#### Synchronous Batch Inference

For processing multiple inputs at once to maximize throughput, you can use the batch inference capabilities of the `run` method. This is more efficient than running single inferences in a loop.  

**A.** Explicit Batch Format (`List[List[np.ndarray]]`)  

This is the clearest way to represent a batch. The input is a list of lists, where the outer list represents the batch and each inner list contains all input tensors for a single sample.

  - **API**: `ie.run(batch_inputs, output_buffers=...)`
  - **Input**: A `List[List[np.ndarray]]`.
  - **Output**: A `List[List[np.ndarray]]`.

<!-- end list -->

```python
batch_size = 3
input_sizes = ie.get_input_tensor_sizes()
batch_inputs = []
for i in range(batch_size):
    sample_inputs = [create_dummy_input(size) for size in input_sizes]
    batch_inputs.append(sample_inputs)

# Output buffers must also match the batch structure
# ... create batch_outputs ...

# Run batch inference
results = ie.run(batch_inputs, output_buffers=batch_outputs)
```

**B.** Flattened Batch Format (`List[np.ndarray]`)    

As a convenience, the API can also accept a single "flattened" list of `numpy` arrays. The total number of arrays **must** be a multiple of the model's input tensor count. The engine will automatically group them into batches.

  - **API**: `ie.run(flattened_inputs, output_buffers=...)`
  - **Input**: A `List[np.ndarray]` containing `batch_size * num_input_tensors` arrays.
  - **Output**: The result is still returned in the explicit batch format (`List[List[np.ndarray]]`).

<!-- end list -->

```python
batch_size = 3
input_sizes = ie.get_input_tensor_sizes()
flattened_inputs = []
for i in range(batch_size):
    for size in input_sizes:
        flattened_inputs.append(create_dummy_input(size))

# ... create flattened_output_buffers ...

# Run batch inference
results = ie.run(flattened_inputs, output_buffers=flattened_output_buffers)
```

#### Asynchronous Inference

Asynchronous APIs allow you to submit inference requests without blocking the calling thread. The results are returned later via a callback function. This is ideal for applications that need to remain responsive.  

  - **APIs**:
      - `ie.run_async_multi_input(input_tensors, user_arg=...)`
      - `ie.run_async(input_data, user_arg=...)`
  - **Callback Registration**: `ie.register_callback(callback_function)`

The `AsyncInferenceHandler` class in the example demonstrates how to manage state across multiple asynchronous calls.  

  - **Register a Callback**: Provide a function that the engine will call upon completion of each async request. The callback receives the output arrays and a `user_arg` for context.
  - **Submit Requests**: Call an `run_async` variant. This call returns immediately with a job ID.
  - **Process in Callback**: The callback function is executed in a separate worker thread. Here, you can process the results. It's crucial to ensure thread safety (e.g., using a `threading.Lock`) if you modify shared data.

<!-- end list -->

```python
# 1. Create a handler and register its callback method
handler = AsyncInferenceHandler(async_count)
ie.register_callback(handler.callback)

# 2. Submit multiple async requests in a loop
for i in range(async_count):
    user_arg = f"async_sample_{i}"
    # Each call is non-blocking
    job_id = ie.run_async_multi_input(input_tensors, user_arg=user_arg)

# 3. Wait for all callbacks to complete
handler.wait_for_completion()

# 4. Clear the callback when done
ie.register_callback(None)
```

---

### Examples

The DX-RT SDK provides comprehensive Python examples in the `examples/python/` directory. These examples demonstrate various inference patterns, configuration options, and performance optimization techniques using the Python API. All examples are ready to run.

**Location:** `examples/python/`

#### Basic Inference Examples

- **`run_sync_model.py`**  
  Demonstrates synchronous inference using a single thread. This is the simplest way to perform inference in Python, suitable for sequential processing and prototyping.

- **`run_async_model.py`**  
  Performance-optimized asynchronous inference using callback functions. Shows how to maximize NPU utilization with non-blocking execution and thread-safe callback handling.

- **`run_async_model_callback.py`**  
  Additional asynchronous inference example focusing on callback pattern variations and user argument passing for input-output synchronization.

- **`run_async_model_wait.py`**  
  Asynchronous inference using the `wait()` method instead of callbacks, providing more control over output retrieval timing and job management.

- **`run_async_model_thread.py`**  
  Single inference engine with multiple threads using callbacks. Demonstrates how to handle multiple concurrent inputs to a single model efficiently.

- **`run_batch_model.py`**  
  Shows how to perform batch inference to process multiple inputs simultaneously, improving throughput for batch-oriented workloads.

#### Configuration Examples

- **`run_sync_model_bound.py`**  
  Demonstrates how to bind inference to specific NPU cores using `InferenceOption.bound_option` for fine-grained resource control.

- **`run_sync_model_bufcount.py`**  
  Illustrates buffer count configuration with `InferenceOption.buffer_count` to optimize memory usage and inference pipeline depth.

- **`run_async_model_bufcount.py`**  
  Shows buffer count tuning for asynchronous inference to balance throughput and memory consumption in high-performance scenarios.

- **`run_async_model_conf.py`**  
  Comprehensive example of using `dx_engine.Configuration` to set runtime parameters, monitor device status, and control logging behavior.

#### Memory Management Examples

- **`run_sync_model_memory.py`**  
  Demonstrates loading models from memory buffers (bytes) instead of files, useful for embedded systems or when models are loaded from custom sources.

#### Profiling Examples

- **`run_async_model_profiler.py`**  
  Demonstrates how to enable and use the DX-RT profiler to collect detailed timing information for performance analysis and bottleneck identification.

#### Multi-Input Model Examples

- **`multi_input_model_inference.py`**  
  Shows how to handle models with multiple input tensors, including proper buffer preparation, numpy array handling, and correct data layout.

#### Running Examples

All examples support command-line arguments for flexibility:

```bash
# Basic synchronous inference
python examples/python/run_sync_model.py -m /path/to/model.dxnn -l 100

# Asynchronous inference with verbose output
python examples/python/run_async_model.py -m /path/to/model.dxnn -l 1000 -v

# Batch inference
python examples/python/run_batch_model.py -m /path/to/model.dxnn -b 4 -l 100

# Bound to specific NPU core
python examples/python/run_sync_model_bound.py -m /path/to/model.dxnn

# With profiling enabled
python examples/python/run_async_model_profiler.py -m /path/to/model.dxnn -l 500
```

**Common Command-Line Options:**
- `-m, --model`: Path to the model file (.dxnn) - **required**
- `-l, --loops`: Number of inference iterations (default: 1)
- `-b, --batch`: Batch count for batch inference (default: 1)
- `-v, --verbose`: Enable verbose/debug logging
- `-h, --help`: Display usage information

**Prerequisites:**

Ensure the DX-RT Python package is installed:
```bash
pip install dx_engine
```

---
