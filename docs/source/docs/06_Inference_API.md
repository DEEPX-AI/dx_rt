This chapter introduces the inference APIs provided by **DX-RT**, including both C++ and Python interfaces. It covers synchronous and asynchronous execution, support for single and multi-input models, and guidance on input/output formatting. Key topics include model execution, input parsing, special handling cases, and performance tuning to help developers integrate inference efficiently.  

---

## C++ Inference API

This section describes the C++ interface for executing inference using the **DX-RT** SDK. It covers both synchronous and asynchronous execution modes, and provides guidance on initializing the engine, managing input/output buffers, and utilizing multiple NPU cores. These APIs are optimized for performance-critical environments and offer granular control over execution flow.  

### Running Synchronous Inference

This section covers the blocking inference methods in the **DX-RT** C++ API. It includes single-input, batch, and multi-input execution using dictionary or vector formats. These APIs return results after completion and are suited for real-time or latency-sensitive applications.  

***Run (Single Input/Output)***  

The `Run()` function provides a synchronous inference interface for single-frame execution. It accepts a raw input pointer and returns a vector of output tensors. The behavior of this API varies depending on the model type and how the input buffer is formatted.  

```cpp
TensorPtrs Run(void *inputPtr, void *userArg = nullptr, void *outputPtr = nullptr)
```

| Input Format | Description | Model Type | Output Format | Notes |
|---|---|---|---|---|
| `void* inputPtr` | Single input pointer | Single-Input | `TensorPtrs` (Vector) | Traditional method |
| `void* inputPtr` | Concatenated buffer pointer | Multi-Input | `TensorPtrs` (Vector) | Auto-split applied |

Example
```cpp
// Single input model
auto outputs = ie.Run(inputData);

// Multi-input model (auto-split)
auto outputs = ie.Run(concatenatedInput);
```

In both cases, the output is returned as a vector of TensorPtr, representing each output tensor.  


***Run (Batch)***  

The `Run()` batch variant enables synchronous batched inference for both single-input and multi-input models. It accepts a flat list of input pointers, optional output buffers, and optional user arguments. The input vector is interpreted based on model type and size.

```cpp
std::vector<TensorPtrs> Run(
    const std::vector<void*>& inputBuffers,
    const std::vector<void*>& outputBuffers,
    const std::vector<void*>& userArgs
)
```

| Input Format | Condition | Interpretation | Output Format | Notes |
|---|---|---|---|---|
| `vector<void*>` (size=1) | Single-Input | Single Inference | `vector<TensorPtrs>` (size=1) | Special case |
| `vector<void*>` (size=N) | Single-Input | Batch Inference | `vector<TensorPtrs>` (size=N) | N samples |
| `vector<void*>` (size=M) | Multi-Input, M==input\_count | Single Inference | `vector<TensorPtrs>` (size=1) | Multi-input single |
| `vector<void*>` (size=N*M) | Multi-Input, N*M==multiple | Batch Inference | `vector<TensorPtrs>` (size=N) | N samples, M inputs |

Example
```cpp
// Single input batch
std::vector<void*> batchInputs = {sample1, sample2, sample3};
auto batchOutputs = ie.Run(batchInputs, outputBuffers, userArgs);

// Multi-input single
std::vector<void*> multiInputs = {input1, input2}; // M=2
auto singleOutput = ie.Run(multiInputs, {outputBuffer}, {userArg});

// Multi-input batch
std::vector<void*> multiBatch = {s1_i1, s1_i2, s2_i1, s2_i2}; // N=2, M=2
auto batchOutputs = ie.Run(multiBatch, outputBuffers, userArgs);
```

***RunMultiInput (Dictionary)***

The `RunMultiInput()` function provides a synchronous interface for multi-input models using a dictionary-style input. Each input tensor is mapped by name, allowing for flexible and explicit data assignment.

```cpp
TensorPtrs RunMultiInput(
    const std::map<std::string, void*>& inputTensors,
    void *userArg = nullptr,
    void *outputPtr = nullptr
)
```

| Input Format | Constraints | Output Format | Notes |
|---|---|---|---|
| `map<string, void*>` | Must include all input tensor names | `TensorPtrs` | For multi-input models only |

Example
```cpp
std::map<std::string, void*> inputs = {
    {"input1", data1},
    {"input2", data2}
};
auto outputs = ie.RunMultiInput(inputs);
```

***RunMultiInput (Vector)***

```cpp
TensorPtrs RunMultiInput(
    const std::vector<void*>& inputPtrs,
    void *userArg = nullptr,
    void *outputPtr = nullptr
)
```

| Input Format | Constraints | Output Format | Notes |
|---|---|---|---|
| `vector<void*>` | size == input\_tensor\_count | `TensorPtrs` | Order matches GetInputTensorNames() |

---

### Running Asynchronous Inference  

This section describes the non-blocking inference functions in the C++ API. Asynchronous methods support parallel execution across NPU cores and allow synchronization via callbacks or `Wait()`. They are optimized for high-throughput and multi-threaded workloads.

***RunAsync (Single)***

The `RunAsync()` function initiates a non-blocking inference job using a single input pointer. It supports both single-input and multi-input models. Upon submission, the function returns a job ID, which is later used with the `Wait()` function to retrieve the result.

```cpp
int RunAsync(void *inputPtr, void *userArg = nullptr, void *outputPtr = nullptr)
```

| Input Format | Model Type | Output Format | Notes |
|---|---|---|---|
| `void* inputPtr` | Single-Input | `int` (jobId) | Result received via Wait(jobId) |
| `void* inputPtr` | Multi-Input | `int` (jobId) | Auto-split applied |

***RunAsync (Vector)***

The `RunAsync()` vector-based variant provides non-blocking inference using an explicit list of input pointers. It is especially suitable for multi-input models, where each input tensor is provided as a separate pointer in the vector.

```cpp
int RunAsync(const std::vector<void*>& inputPtrs, void *userArg = nullptr, void *outputPtr = nullptr)
```

| Input Format | Condition | Interpretation | Output Format | Notes |
|---|---|---|---|---|
| `vector<void*>` (size==input\_count) | Multi-Input | Multi-input single | `int` (jobId) | Recommended method |
| `vector<void*>` (size\!=input\_count) | Any | Uses only the first element | `int` (jobId) | Fallback |

***RunAsyncMultiInput (Dictionary)***

The `RunAsyncMultiInput()` function performs non-blocking asynchronous inference for multi-input models, where each input tensor is provided in a named dictionary. This is the most explicit and type-safe method for specifying inputs, ensuring each tensor is matched to the correct model input.

```cpp
int RunAsyncMultiInput(
    const std::map<std::string, void*>& inputTensors,
    void *userArg = nullptr,
    void *outputPtr = nullptr
)
```

| Input Format | Constraints | Output Format | Notes |
|---|---|---|---|
| `map<string, void*>` | For multi-input models only | `int` (jobId) | Most explicit method |

This API is intended only for multi-input models. If used with single-input models, behavior is undefined.  

***RunAsyncMultiInput (Vector)***

The `RunAsyncMultiInput()` function performs asynchronous inference for multi-input models using a vector of input pointers. Each element in the vector corresponds to one input tensor, and the function returns a job ID to retrieve results later via `Wait()`.

```cpp
int RunAsyncMultiInput(
    const std::vector<void*>& inputPtrs,
    void *userArg = nullptr,
    void *outputPtr = nullptr
)
```

| Input Format | Constraints | Output Format | Notes |
|---|---|---|---|
| `vector<void*>` | size == input\_tensor\_count | `int` (jobId) | Converted to a dictionary internally |

This method requires the input vector to be in the same order as the model's input tensor definitions. You can retrieve the correct order using `GetInputTensorNames()`.

***Wait***

The `Wait()` function blocks the current thread until the specified asynchronous inference job, identified by jobId, is completed. Once the job finishes, the function returns the corresponding inference output.

```cpp
TensorPtrs Wait(
    int jobId
)
```

---

## Python Inference API

This section outlines the Python API for running inference with the **DX-RT** SDK. Designed for ease of use and rapid development, the Python interface supports both synchronous and asynchronous execution, and offers flexible input handling for single, batch, and multi-input models. It is ideal for prototyping and integration into Python-based AI workflows.

### Running Synchronous Inference

This section introduces the blocking inference methods in the Python API. These functions support single and multi-input models using a unified or dictionary-based interface. Results are returned after execution, making them suitable for sequential or real-time use.

***run (Unified API)***

The run() function is the primary Python API for synchronous inference. It supports both single-input and multi-input models, with automatic detection of batch size and input format. This unified interface simplifies inference for a variety of model types and use cases.

```python
def run(
    input_data: Union[np.ndarray, List[np.ndarray], List[List[np.ndarray]]],
    output_buffers: Optional[Union[List[np.ndarray], List[List[np.ndarray]]]] = None,
    user_args: Optional[Union[Any, List[Any]]] = None
) -> Union[List[np.ndarray], List[List[np.ndarray]]]
```

Detailed Input/Output Matrix  

*Input: `np.ndarray`*  

- **Multi-Input Model** (`size == total_input_size`)  
  : Interpretation: Auto-split single  
  : Output: `List[np.ndarray]` (Single sample output)  
- **Single-Input Model** (`size != total_input_size`):  
  : Interpretation: Single Inference  
  : Output: `List[np.ndarray]` (Single sample output)  

*Input: `List[np.ndarray]`*  

- **Single-Input Model** (`len == 1`)  
  : Interpretation: Single Inference  
  : Output: `List[np.ndarray]` (Single sample output)  
- **Multi-Input Model** (`len == input_count`)  
  : Interpretation: Single Inference  
  : Output: `List[np.ndarray]` (Single sample output)  
- **Multi-Input Model** (`len == N * input_count`)  
  : Interpretation: Batch Inference (N samples)  
  : Output: `List[List[np.ndarray]]` (N sample outputs)  
- **Single-Input Model** (`len > 1`)  
  : Interpretation: Batch Inference  
  : Output: `List[List[np.ndarray]]` (`len` sample outputs)  

*Input: `List[List[np.ndarray]]`*  

- **Any Model** (Explicit batch)  
  : Interpretation: Batch Inference  
  : Output: `List[List[np.ndarray]]` (Matches outer list size)  


Auto-split Special Cases  

- **Multi-input + first element is total_size** (e.g., `[concatenated_array]`)  
  : Interpretation: Auto-split single  
  : Output: `List[np.ndarray]`  
- **Multi-input + all elements are total_size** (e.g., `[concat1, concat2, concat3]`)  
  : Interpretation: Auto-split batch  
  :  Output: `List[List[np.ndarray]]`  

Example  
```python
# 1. Single array auto-split (multi-input)
concatenated = np.zeros(ie.get_input_size(), dtype=np.uint8)
outputs = ie.run(concatenated)  # List[np.ndarray]

# 2. Multi-input single
input_list = [input1_array, input2_array]  # len == 2
outputs = ie.run(input_list)  # List[np.ndarray]

# 3. Multi-input batch (flattened)
flattened = [s1_i1, s1_i2, s2_i1, s2_i2]  # 2 samples, 2 inputs each
outputs = ie.run(flattened)  # List[List[np.ndarray]], len=2

# 4. Multi-input batch (explicit)
explicit_batch = [[s1_i1, s1_i2], [s2_i1, s2_i2]]
outputs = ie.run(explicit_batch)  # List[List[np.ndarray]], len=2

# 5. Single-input batch
single_batch = [sample1, sample2, sample3]
outputs = ie.run(single_batch)  # List[List[np.ndarray]], len=3
```

***run_multi_input (Dictionary)***

The `run_multi_input()` function performs synchronous inference using a dictionary format for input, where each key-value pair maps an input tensor name to a corresponding NumPy array. This method is ideal for multi-input models, offering clarity and input safety through name matching.

```python
def run_multi_input(
    input_tensors: Dict[str, np.ndarray],
    output_buffers: Optional[List[np.ndarray]] = None,
    user_arg: Any = None
) -> List[np.ndarray]
```

| Input Type | Constraints | Output Type | Notes |
|---|---|---|---|
| `Dict[str, np.ndarray]` | Must include all input tensors | `List[np.ndarray]` | For multi-input models only |

---

### Running Asynchronous Inference  

This section covers the non-blocking inference methods in the Python API. These functions support parallel execution and multi-input handling. Use `wait()` to retrieve results when ready, ideal for high-throughput or multi-threaded workflows.

***run_async***  

The `run_async()` function submits a non-blocking inference request using either a single input tensor or a list of input tensors. It is suitable for single-sample asynchronous execution and returns a job ID used to retrieve the result via wait(`job_id`).

```python
def run_async(
    input_data: Union[np.ndarray, List[np.ndarray]],
    user_arg: Any = None,
    output_buffer: Optional[Union[np.ndarray, List[np.ndarray]]] = None
) -> int
```

| Input Type | Condition | Interpretation | Output Type | Constraints |
|---|---|---|---|---|
| `np.ndarray` | Any | Single Inference | `int` (jobId) | Batch not supported |
| `List[np.ndarray]` | len == input\_count | Multi-input single | `int` (jobId) | Batch not supported |
| `List[np.ndarray]` | len == 1 | Single-input single | `int` (jobId) | Batch not supported |

***run_async_multi_input***  

The `run_async_multi_input()` function performs asynchronous single-sample inference using a dictionary to map input tensor names to their corresponding NumPy arrays. This method is intended for multi-input models and returns a job ID to be used with wait(`job_id`).  

```python
def run_async_multi_input(
    input_tensors: Dict[str, np.ndarray],
    user_arg: Any = None,
    output_buffer: Optional[List[np.ndarray]] = None
) -> int
```

| Input Type | Constraints | Output Type | Notes |
|---|---|---|---|
| `Dict[str, np.ndarray]` | For multi-input models only | `int` (jobId) | Single inference only |

***wait***  

The `wait()` function blocks the execution until the specified asynchronous inference job is complete and returns the corresponding inference results.  

```python
def wait(job_id: int) -> List[np.ndarray]
```

---

## Input Format Parsing

This section explains how the SDK determines the appropriate inference mode based on the input data format. The engine automatically analyzes invputs such as np.ndarray, `List[np.ndarray]`, and nested lists to decide between single, batch, or auto-split inference

### Python Input Parsing Flow

```python
def _analyze_input_format(input_data):
    # 1. Check for np.ndarray
    if isinstance(input_data, np.ndarray):
        if should_auto_split_input(input_data):
            return auto_split_single_inference()
        else:
            return single_inference()

    # 2. Check for List
    if isinstance(input_data, list):
        if isinstance(input_data[0], list):
            # List[List[np.ndarray]] - Explicit batch
            return explicit_batch_inference()
        else:
            # List[np.ndarray] - Requires complex analysis
            return analyze_list_ndarray(input_data)
```

---

### `List[np.ndarray]` Parsing Rules

```python
def analyze_list_ndarray(input_data):
    input_count = len(input_data)

    if is_multi_input_model():
        expected_count = get_input_tensor_count()

        if input_count == expected_count:
            return single_inference()
        elif input_count % expected_count == 0:
            batch_size = input_count // expected_count
            return batch_inference(batch_size)
        elif all(should_auto_split_input(arr) for arr in input_data):
            return auto_split_batch_inference()
        else:
            raise ValueError("Invalid input count")
    else:  # Single-input model
        if input_count == 1:
            return single_inference()
        else:
            return batch_inference(input_count)
```

---

## Output Format Specification

This section describes the structure of inference outputs returned by the SDK, depending on the API (C++ or Python) and the inference mode (single, batch, or asynchronous). The output format varies to match the input pattern and execution method.  

### Single Inference Output

| API | Output Format | Structure |
|---|---|---|
| C++ Run | `TensorPtrs` | `vector<shared_ptr<Tensor>>` |
| Python run | `List[np.ndarray]`| `[output1, output2, ...]` |

### Batch Inference Output

| API | Output Format | Structure |
|---|---|---|
| C++ Run (batch) | `vector<TensorPtrs>` | `[sample1_outputs, sample2_outputs, ...]` |
| Python run (batch)| `List[List[np.ndarray]]` | `[[s1_o1, s1_o2], [s2_o1, s2_o2], ...]` |

### Asynchronous Inference Output

| API | Immediate Return | After `wait` |
|---|---|---|
| C++ RunAsync | `int` (jobId) | `TensorPtrs` |
| Python run\_async| `int` (jobId) | `List[np.ndarray]` |

---

## Edge Cases and Special Handling

This section describes how the SDK handles non-standard input scenarios, such as auto-splitting, batch size calculation, invalid input conditions, and custom output buffers. These rules ensure robustness and flexibility across a wide range of inference use cases.  

### Auto-Split Detection Logic

**C++**
```cpp
bool shouldAutoSplitInput() const {
    return _isMultiInput && _inputTasks.size() == 1;
}
```

**Python**
```python
def _should_auto_split_input(input_data: np.ndarray) -> bool:
    if not self.is_multi_input_model():
        return False

    expected_total_size = self.get_input_size()
    actual_size = input_data.nbytes

    return actual_size == expected_total_size
```

### Batch Size Determination

| Condition | Batch Size Calculation |
|---|---|
| Single-input + List[np.ndarray] | `len(input_data)` |
| Multi-input + List[np.ndarray] | `len(input_data) // input_tensor_count` |
| List[List[np.ndarray]] | `len(input_data)` |

### Common Error Conditions

| Condition | Error Type | Message |
|---|---|---|
| Multi-input + invalid size | `ValueError` | "Invalid input count for multi-input model" |
| Async + batch | `ValueError` | "Batch inference not supported in async" |
| Empty input | `ValueError` | "Input data cannot be empty" |
| Type mismatch | `TypeError` | "Expected np.ndarray or List[np.ndarray]" |

### Custom Output Buffer Handling (Python)

| Input Format | Output Buffer Format | Handling |
|---|---|---|
| Single Inference | `None` | Auto-allocated |
| Single Inference | `List[np.ndarray]` | User-provided |
| Single Inference | `np.ndarray` (total\_size) | Used after auto-split |
| Batch Inference | `List[List[np.ndarray]]` | Explicit batch buffer |
| Batch Inference | `List[np.ndarray]` | Flattened batch buffer |

---

## Performance Optimization Guidelines

This section describes key performance-related trade-offs when using the inference API, including memory allocation strategies and the impact of different inference methods on latency and throughput.  

### Memory Allocation Strategy

| Method | Pros | Cons |
|---|---|---|
| Auto-allocation (No Buffer) | Ease of use | Memory allocated on every call |
| User-provided (With Buffer) | Performance optimization | Complex memory management |

### Choosing Inference Methods (Sync vs Async vs Batch)

| Method | Use Case | Characteristics |
|---|---|---|
| Synchronous | Simple processing | Sequential execution |
| Asynchronous | High throughput | Requires callback management |
| Batch | Bulk processing | Increased memory usage |

---
