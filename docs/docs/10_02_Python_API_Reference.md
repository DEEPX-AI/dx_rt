This section describes the Python bindings of the DX-RT SDK. It provides a streamlined interface to the same core functionalities as the C++ SDK, making it ideal for rapid development, prototyping, and integration with Python-based AI workflows.

### `class dx_engine.InferenceEngine`

This class is the main Python wrapper for the DXRT Inference Engine. It provides an interface to load a compiled model and perform inference tasks, either synchronously or asynchronously, supporting both single and batch inference.

#### Constructor

***`__init__(self, model_path: str, inference_option: Optional[InferenceOption] = None)`***
-   **Description**: Initializes the InferenceEngine by loading a compiled model from the specified path.
-   **Parameters**:
    -   `model_path`: `str`. Path to the compiled model file (e.g., `*.dxnn`).
    -   `inference_option`: `Optional[InferenceOption]`. An `InferenceOption` object for configuration. If `None`, default options are used.
-   **Raises**: `RuntimeError` if the underlying C++ engine fails to initialize.

#### Member Functions

***`dispose(self)`***
-   **Signature**: `def dispose(self) -> None`
-   **Description**: Explicitly releases the underlying C++ resources held by the inference engine. This is automatically called when using a `with` statement, so manual invocation is typically **not** required.

***`get_all_task_outputs(self)`***
-   **Signature**: `def get_all_task_outputs(self) -> List[List[np.ndarray]]`
-   **Description**: Retrieves the outputs of all internal tasks in their execution order. This is useful for debugging the intermediate steps of a model.
-   **Returns**: A list of lists, where each inner list contains the output `np.ndarray` objects for a single task.

***`get_bitmatch_mask(self, index: int = 0)`***
-   **Signature**: `def get_bitmatch_mask(self, index: int = 0) -> np.ndarray`
-   **Description**: Retrieves a bitmatch mask for a specific NPU task, which can be used for validation and debugging purposes.
-   **Parameters**:
    -   `index`: `int`. The index of the NPU task.
-   **Returns**: A boolean `np.ndarray` representing the bitmatch mask.

***`get_compile_type(self)`***
-   **Signature**: `def get_compile_type(self) -> str`
-   **Description**: Returns the compilation type or strategy of the loaded model (e.g., "debug", "release").
-   **Returns**: The compilation type as a string.

***`get_input_size(self)`***
-   **Signature**: `def get_input_size(self) -> int`
-   **Description**: Gets the total expected size of all input tensors combined in bytes.
-   **Returns**: The total input size as an integer.

***`get_input_tensor_count(self)`***
-   **Signature**: `def get_input_tensor_count(self) -> int`
-   **Description**: Returns the number of input tensors required by the model.
-   **Returns**: The number of input tensors.

***`get_input_tensor_names(self)`***
-   **Signature**: `def get_input_tensor_names(self) -> List[str]`
-   **Description**: Returns the names of all input tensors in the order they should be provided.
-   **Returns**: A list of input tensor names.

***`get_input_tensor_sizes(self)`***
-   **Signature**: `def get_input_tensor_sizes(self) -> List[int]`
-   **Description**: Gets the individual sizes of each input tensor in bytes, in their correct order.
-   **Returns**: A list of integer sizes.

***`get_input_tensor_to_task_mapping(self)`***
-   **Signature**: `def get_input_tensor_to_task_mapping(self) -> Dict[str, str]`
-   **Description**: Returns the mapping from input tensor names to their target tasks within the model graph.
-   **Returns**: A dictionary mapping tensor names to task names.

***`get_input_tensors_info(self)`***
-   **Signature**: `def get_input_tensors_info(self) -> List[Dict[str, Any]]`
-   **Description**: Returns detailed information for each input tensor.
-   **Returns**: A list of dictionaries, where each dictionary contains keys: `'name'` (str), `'shape'` (List[int]), `'dtype'` (np.dtype), and `'elem_size'` (int).

***`get_latency(self)`***
-   **Signature**: `def get_latency(self) -> int`
-   **Description**: Returns the latency of the most recent inference in microseconds.
-   **Returns**: The latency value as an integer.

***`get_latency_count(self)`***
-   **Signature**: `def get_latency_count(self) -> int`
-   **Description**: Returns the total count of latency values collected since initialization.
-   **Returns**: The number of measurements.

***`get_latency_list(self)`***
-   **Signature**: `def get_latency_list(self) -> List[int]`
-   **Description**: Returns a list of recent latency measurements in microseconds.
-   **Returns**: A list of integers.

***`get_latency_mean(self)`***
-   **Signature**: `def get_latency_mean(self) -> float`
-   **Description**: Returns the mean (average) of all collected latency values.
-   **Returns**: The mean latency as a float.

***`get_latency_std(self)`***
-   **Signature**: `def get_latency_std(self) -> float`
-   **Description**: Returns the standard deviation of all collected latency values.
-   **Returns**: The standard deviation as a float.

***`get_model_version(self)`***
-   **Signature**: `def get_model_version(self) -> str`
-   **Description**: Returns the DXNN file format version of the loaded model.
-   **Returns**: The model version string.

***`get_npu_inference_time(self)`***
-   **Signature**: `def get_npu_inference_time(self) -> int`
-   **Description**: Returns the pure NPU processing time for the most recent inference in microseconds.
-   **Returns**: The NPU inference time as an integer.

***`get_npu_inference_time_count(self)`***
-   **Signature**: `def get_npu_inference_time_count(self) -> int`
-   **Description**: Returns the total count of NPU inference time values collected.
-   **Returns**: The number of measurements.

***`get_npu_inference_time_list(self)`***
-   **Signature**: `def get_npu_inference_time_list(self) -> List[int]`
-   **Description**: Returns a list of recent NPU inference time measurements in microseconds.
-   **Returns**: A list of integers.

***`get_npu_inference_time_mean(self)`***
-   **Signature**: `def get_npu_inference_time_mean(self) -> float`
-   **Description**: Returns the mean (average) of all collected NPU inference times.
-   **Returns**: The mean time as a float.

***`get_npu_inference_time_std(self)`***
-   **Signature**: `def get_npu_inference_time_std(self) -> float`
-   **Description**: Returns the standard deviation of all collected NPU inference times.
-   **Returns**: The standard deviation as a float.

***`get_num_tail_tasks(self)`***
-   **Signature**: `def get_num_tail_tasks(self) -> int`
-   **Description**: Returns the number of 'tail' tasks (tasks with no successors) in the model graph.
-   **Returns**: The number of tail tasks.

***`get_output_size(self)`***
-   **Signature**: `def get_output_size(self) -> int`
-   **Description**: Gets the total size of all output tensors combined in bytes.
-   **Returns**: The total output size as an integer.

***`get_output_tensor_count(self)`***
-   **Signature**: `def get_output_tensor_count(self) -> int`
-   **Description**: Returns the number of output tensors produced by the model.
-   **Returns**: The number of output tensors.

***`get_output_tensor_names(self)`***
-   **Signature**: `def get_output_tensor_names(self) -> List[str]`
-   **Description**: Returns the names of all output tensors in the order they are produced.
-   **Returns**: A list of output tensor names.

***`get_output_tensor_sizes(self)`***
-   **Signature**: `def get_output_tensor_sizes(self) -> List[int]`
-   **Description**: Gets the individual sizes of each output tensor in bytes, in their correct order.
-   **Returns**: A list of integer sizes.

***`get_output_tensors_info(self)`***
-   **Signature**: `def get_output_tensors_info(self) -> List[Dict[str, Any]]`
-   **Description**: Returns detailed information for each output tensor.
-   **Returns**: A list of dictionaries with keys: `'name'` (str), `'shape'` (List[int]), `'dtype'` (np.dtype), and `'elem_size'` (int).

***`get_task_order(self)`***
-   **Signature**: `def get_task_order(self) -> np.ndarray`
-   **Description**: Returns the execution order of tasks/subgraphs within the model.
-   **Returns**: A numpy array of strings representing the task order.

***`is_multi_input_model(self)`***
-   **Signature**: `def is_multi_input_model(self) -> bool`
-   **Description**: Checks if the loaded model requires multiple input tensors.
-   **Returns**: `True` if the model has multiple inputs, `False` otherwise.

***`is_ppu(self)`***
-   **Signature**: `def is_ppu(self) -> bool`
-   **Description**: Checks if the loaded model utilizes a Post-Processing Unit (PPU).
-   **Returns**: `True` if the model uses a PPU, `False` otherwise.

***`register_callback(self, callback: Optional[Callable[[List[np.ndarray], Any], int]])`***
-   **Signature**: `def register_callback(self, callback: Optional[Callable[[List[np.ndarray], Any], int]]) -> None`
-   **Description**: Registers a user-defined callback function to be executed upon completion of an asynchronous inference.
-   **Parameters**:
    -   `callback`: A callable function or `None` to unregister. The callback receives the list of output arrays and the user argument.

***`run(self, input_data: Union[np.ndarray, List[np.ndarray], List[List[np.ndarray]]], output_buffers: Optional[Union[List[np.ndarray], List[List[np.ndarray]]]] = None, user_args: Optional[Union[Any, List[Any]]] = None)`***
-   **Signature**: `def run(self, input_data, output_buffers=None, user_args=None) -> Union[List[np.ndarray], List[List[np.ndarray]]]`
-   **Description**: Runs inference synchronously. This versatile method handles single-item, multi-input, and batch inference based on the format of `input_data`.
-   **Parameters**:
    -   `input_data`: Input data in various formats (`np.ndarray`, `List[np.ndarray]`, `List[List[np.ndarray]]`).
    -   `output_buffers`: Optional pre-allocated buffers for the output.
    -   `user_args`: Optional user-defined argument or list of arguments for batch mode.
-   **Returns**: The inference result(s). A `List[np.ndarray]` for single inference or a `List[List[np.ndarray]]` for batch inference.

***`run_async(self, input_data: Union[np.ndarray, List[np.ndarray]], user_arg: Any = None, output_buffer: Optional[Union[np.ndarray, List[np.ndarray]]] = None)`***
-   **Signature**: `def run_async(self, input_data, user_arg=None, output_buffer=None) -> int`
-   **Description**: Runs inference asynchronously for a single item. Batch processing is **not** supported with this method.
-   **Parameters**:
    -   `input_data`: A single `np.ndarray` or a `List[np.ndarray]` for multi-input models.
    -   `user_arg`: An optional user-defined argument to be passed to the callback.
    -   `output_buffer`: An optional pre-allocated buffer for the output.
-   **Returns**: An integer `job_id` for this asynchronous operation.

***`run_async_multi_input(self, input_tensors: Dict[str, np.ndarray], user_arg: Any = None, output_buffer: Optional[List[np.ndarray]] = None)`***
-   **Signature**: `def run_async_multi_input(self, input_tensors, user_arg=None, output_buffer=None) -> int`
-   **Description**: A convenience method to run asynchronous inference on a multi-input model using a dictionary of named tensors.
-   **Parameters**:
    -   `input_tensors`: A dictionary mapping input tensor names to `np.ndarray` data.
    -   `user_arg`: An optional user-defined argument.
    -   `output_buffer`: An optional list of pre-allocated output arrays.
-   **Returns**: An integer `job_id`.

***`run_benchmark(self, num_loops: int, input_data: Optional[List[np.ndarray]] = None)`***
-   **Signature**: `def run_benchmark(self, num_loops: int, input_data: Optional[List[np.ndarray]] = None) -> float`
-   **Description**: Runs a performance benchmark for a specified number of loops.
-   **Parameters**:
    -   `num_loops`: The number of inference iterations to run.
    -   `input_data`: An optional list of `np.ndarray` to use as input for the benchmark.
-   **Returns**: The average frames per second (FPS) as a float.

***`run_multi_input(self, input_tensors: Dict[str, np.ndarray], output_buffers: Optional[List[np.ndarray]] = None, user_arg: Any = None)`***
-   **Signature**: `def run_multi_input(self, input_tensors, output_buffers=None, user_arg=None) -> List[np.ndarray]`
-   **Description**: A convenience method to run synchronous inference on a multi-input model using a dictionary of named tensors.
-   **Parameters**:
    -   `input_tensors`: A dictionary mapping input tensor names to `np.ndarray` data.
    -   `output_buffers`: An optional list of pre-allocated output arrays.
    -   `user_arg`: An optional user-defined argument.
-   **Returns**: A list of `np.ndarray` objects containing the output.

***`wait(self, job_id: int)`***
-   **Signature**: `def wait(self, job_id: int) -> List[np.ndarray]`
-   **Description**: Waits for an asynchronous job (identified by `job_id`) to complete and retrieves its output.
-   **Parameters**:
    -   `job_id`: The integer job ID returned from a `run_async` call.
-   **Returns**: A list of `np.ndarray` objects containing the output from the completed job.

---

### `class dx_engine.InferenceOption`

This class provides a Pythonic interface to configure inference options such as device selection and core binding. It wraps the C++ `InferenceOption` struct.

#### Constructor

***`__init__(self)`***
-   **Signature**: `def __init__(self) -> None`
-   **Description**: Initializes a new `InferenceOption` object with default values from the C++ backend.

#### Properties

***`bound_option`***
-   **Description**: Gets or sets the NPU core binding strategy.
-   **Type**: `InferenceOption.BOUND_OPTION` (Enum).

***`devices`***
-   **Description**: Gets or sets the list of device IDs to be used for inference. An empty list means all available devices will be used.
-   **Type**: `List[int]`.

***`use_ort`***
-   **Description**: Gets or sets whether to use the ONNX Runtime for executing CPU-based tasks in the model graph.
-   **Type**: `bool`.

#### Member Functions

***`get_bound_option(self)`***
-   **Signature**: `def get_bound_option(self) -> BOUND_OPTION`
-   **Description**: Returns the current NPU core binding option.
-   **Returns**: An `InferenceOption.BOUND_OPTION` enum member.

***`get_devices(self)`***
-   **Signature**: `def get_devices(self) -> List[int]`
-   **Description**: Returns the list of device IDs targeted for inference.
-   **Returns**: A list of integers.

***`get_use_ort(self)`***
-   **Signature**: `def get_use_ort(self) -> bool`
-   **Description**: Returns whether ONNX Runtime usage is enabled.
-   **Returns**: A boolean value.

***`set_bound_option(self, boundOption: BOUND_OPTION)`***
-   **Signature**: `def set_bound_option(self, boundOption: BOUND_OPTION)`
-   **Description**: Sets the NPU core binding option.
-   **Parameters**:
    -   `boundOption`: An `InferenceOption.BOUND_OPTION` enum member.

***`set_devices(self, devices: List[int])`***
-   **Signature**: `def set_devices(self, devices: List[int])`
-   **Description**: Sets the list of device IDs to be used for inference.
-   **Parameters**:
    -   `devices`: A list of integers representing device IDs.

***`set_use_ort(self, use_ort: bool)`***
-   **Signature**: `def set_use_ort(self, use_ort: bool)`
-   **Description**: Enables or disables the use of ONNX Runtime for CPU tasks.
-   **Parameters**:
    -   `use_ort`: A boolean value.

#### Nested Classes

***`class BOUND_OPTION(Enum)`***
-   **Description**: An enumeration defining how NPU cores are utilized.
-   **Members**: `NPU_ALL`, `NPU_0`, `NPU_1`, `NPU_2`, `NPU_01`, `NPU_12`, `NPU_02`.

---

### `class dx_engine.Configuration`

Provides access to the global DXRT configuration singleton, allowing for system-wide settings like enabling the profiler.

#### Constructor

***`__init__(self)`***
-   **Signature**: `def __init__(self)`
-   **Description**: Initializes the Configuration object by getting a reference to the underlying C++ singleton instance.

#### Member Functions

***`get_attribute(self, item: ITEM, attrib: ATTRIBUTE)`***
-   **Signature**: `def get_attribute(self, item: ITEM, attrib: ATTRIBUTE) -> str`
-   **Description**: Retrieves the value of a specific attribute for a configuration item.
-   **Parameters**:
    -   `item`: The configuration category (e.g., `Configuration.ITEM.PROFILER`).
    -   `attrib`: The attribute to retrieve (e.g., `Configuration.ATTRIBUTE.PROFILER_SHOW_DATA`).
-   **Returns**: The attribute value as a string.

***`get_driver_version(self)`***
-   **Signature**: `def get_driver_version(self) -> str`
-   **Description**: Returns the version of the installed device driver.
-   **Returns**: The driver version string.

***`get_enable(self, item: ITEM)`***
-   **Signature**: `def get_enable(self, item: ITEM) -> bool`
-   **Description**: Checks if a specific configuration item is enabled.
-   **Parameters**:
    -   `item`: The configuration category to check.
-   **Returns**: `True` if enabled, `False` otherwise.

***`get_pcie_driver_version(self)`***
-   **Signature**: `def get_pcie_driver_version(self) -> str`
-   **Description**: Returns the version of the installed PCIe driver.
-   **Returns**: The PCIe driver version string.

***`get_version(self)`***
-   **Signature**: `def get_version(self) -> str`
-   **Description**: Returns the version of the DXRT library.
-   **Returns**: The library version string.

***`load_config_file(self, file_name: str)`***
-   **Signature**: `def load_config_file(self, file_name: str)`
-   **Description**: Loads configuration settings from a specified file.
-   **Parameters**:
    -   `file_name`: The path to the configuration file.

***`set_attribute(self, item: ITEM, attrib: ATTRIBUTE, value: str)`***
-   **Signature**: `def set_attribute(self, item: ITEM, attrib: ATTRIBUTE, value: str)`
-   **Description**: Sets a string value for a specific attribute of a configuration item (e.g., setting `PROFILER_SAVE_DATA` to `"ON"`).
-   **Parameters**:
    -   `item`: The configuration category.
    -   `attrib`: The attribute to set.
    -   `value`: The string value to assign.

***`set_enable(self, item: ITEM, enabled: bool)`***
-   **Signature**: `def set_enable(self, item: ITEM, enabled: bool)`
-   **Description**: Enables or disables a global configuration item, such as `PROFILER`.
-   **Parameters**:
    -   `item`: The configuration category.
    -   `enabled`: A boolean value to enable (`True`) or disable (`False`) the item.

#### Nested Classes

***`class ITEM`***
-   **Description**: An enumeration-like class defining configuration categories.
-   **Members**: `DEBUG`, `PROFILER`, `SERVICE`, `DYNAMIC_CPU_THREAD`, `TASK_FLOW`, `SHOW_THROTTLING`, `SHOW_PROFILE`, `SHOW_MODEL_INFO`, `CUSTOM_INTRA_OP_THREADS`, `CUSTOM_INTER_OP_THREADS`.

***`class ATTRIBUTE`***
-   **Description**: An enumeration-like class defining attributes for configuration items.
-   **Members**: `PROFILER_SHOW_DATA`, `PROFILER_SAVE_DATA`, `CUSTOM_INTRA_OP_THREADS_NUM`, `CUSTOM_INTER_OP_THREADS_NUM`.

---

### `class dx_engine.DeviceStatus`

Provides an interface to query real-time status and static information about hardware devices.

#### Class Methods

***`get_current_status(cls, deviceId: int)`***
-   **Signature**: `def get_current_status(cls, deviceId: int) -> object`
-   **Description**: Creates and returns a `DeviceStatus` object populated with the current status of the specified device.
-   **Parameters**:
    -   `deviceId`: The integer ID of the device to query.
-   **Returns**: An instance of `DeviceStatus`.

***`get_device_count(cls)`***
-   **Signature**: `def get_device_count(cls) -> int`
-   **Description**: Returns the total number of hardware devices detected by the system.
-   **Returns**: The number of devices as an integer.

#### Instance Methods

***`get_id(self)`***
-   **Signature**: `def get_id(self) -> int`
-   **Description**: Returns the unique ID of the device associated with this `DeviceStatus` instance.
-   **Returns**: The device ID as an integer.

***`get_npu_clock(self, ch: int)`***
-   **Signature**: `def get_npu_clock(self, ch: int) -> int`
-   **Description**: Returns the current clock frequency of a specific NPU core.
-   **Parameters**:
    -   `ch`: The integer index of the NPU core.
-   **Returns**: The clock speed in MHz.

***`get_npu_voltage(self, ch: int)`***
-   **Signature**: `def get_npu_voltage(self, ch: int) -> int`
-   **Description**: Returns the current voltage of a specific NPU core.
-   **Parameters**:
    -   `ch`: The integer index of the NPU core.
-   **Returns**: The voltage in millivolts (mV).

***`get_temperature(self, ch: int)`***
-   **Signature**: `def get_temperature(self, ch: int) -> int`
-   **Description**: Returns the current temperature of a specific NPU core.
-   **Parameters**:
    -   `ch`: The integer index of the NPU core.
-   **Returns**: The temperature in degrees Celsius.

---

### Standalone Functions

***`dx_engine.parse_model(model_path: str)`***
-   **Signature**: `def parse_model(model_path: str) -> str`
-   **Description**: Parses a model file using the C++ backend and returns a string containing information about the model's structure and properties.
-   **Parameters**:
    -   `model_path`: The path to the compiled model file.
-   **Returns**: A string with model information.

---
