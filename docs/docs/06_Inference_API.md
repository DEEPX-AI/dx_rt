# Inference API

## C++ Inference Engine API

### Synchronous Inference API

#### Run (Single Input/Output)

```cpp
TensorPtrs Run(void *inputPtr, void *userArg = nullptr, void *outputPtr = nullptr)
```

| Input Format | Description | Model Type | Output Format | Notes |
|---|---|---|---|---|
| `void* inputPtr` | Single input pointer | Single-Input | `TensorPtrs` (Vector) | Traditional method |
| `void* inputPtr` | Concatenated buffer pointer | Multi-Input | `TensorPtrs` (Vector) | Auto-split applied |

**Example:**

```cpp
// Single input model
auto outputs = ie.Run(inputData);

// Multi-input model (auto-split)
auto outputs = ie.Run(concatenatedInput);
```

#### Run (Batch)

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

**Example:**

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

#### RunMultiInput (Dictionary)

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

**Example:**

```cpp
std::map<std::string, void*> inputs = {
    {"input1", data1},
    {"input2", data2}
};
auto outputs = ie.RunMultiInput(inputs);
```

#### RunMultiInput (Vector)

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

### Asynchronous Inference API

#### RunAsync (Single)

```cpp
int RunAsync(void *inputPtr, void *userArg = nullptr, void *outputPtr = nullptr)
```

| Input Format | Model Type | Output Format | Notes |
|---|---|---|---|
| `void* inputPtr` | Single-Input | `int` (jobId) | Result received via Wait(jobId) |
| `void* inputPtr` | Multi-Input | `int` (jobId) | Auto-split applied |

#### RunAsync (Vector)

```cpp
int RunAsync(const std::vector<void*>& inputPtrs, void *userArg = nullptr, void *outputPtr = nullptr)
```

| Input Format | Condition | Interpretation | Output Format | Notes |
|---|---|---|---|---|
| `vector<void*>` (size==input\_count) | Multi-Input | Multi-input single | `int` (jobId) | Recommended method |
| `vector<void*>` (size\!=input\_count) | Any | Uses only the first element | `int` (jobId) | Fallback |

#### RunAsyncMultiInput (Dictionary)

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

#### RunAsyncMultiInput (Vector)

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

-----

#### Wait

```cpp
TensorPtrs Wait(
    int jobId
)
```

## Python Inference Engine API

### Synchronous Inference API

#### run (Unified API)

```python
def run(
    input_data: Union[np.ndarray, List[np.ndarray], List[List[np.ndarray]]],
    output_buffers: Optional[Union[List[np.ndarray], List[List[np.ndarray]]]] = None,
    user_args: Optional[Union[Any, List[Any]]] = None
) -> Union[List[np.ndarray], List[List[np.ndarray]]]
```

**Detailed Input/Output Matrix:**

| Input Type | Input Condition | Model Type | Interpretation | Output Type | Output Structure |
|---|---|---|---|---|---|
| `np.ndarray` | size == total\_input\_size | Multi-Input | Auto-split single | `List[np.ndarray]` | Single sample output |
| `np.ndarray` | size \!= total\_input\_size | Single-Input | Single Inference | `List[np.ndarray]` | Single sample output |
| `List[np.ndarray]` | len == 1 | Single-Input | Single Inference | `List[np.ndarray]` | Single sample output |
| `List[np.ndarray]` | len == input\_count | Multi-Input | Single Inference | `List[np.ndarray]` | Single sample output |
| `List[np.ndarray]` | len == N\*input\_count | Multi-Input | Batch Inference (N samples) | `List[List[np.ndarray]]` | N sample outputs |
| `List[np.ndarray]` | len \> 1 | Single-Input | Batch Inference | `List[List[np.ndarray]]` | `len` sample outputs |
| `List[List[np.ndarray]]`| Explicit batch | Any | Batch Inference | `List[List[np.ndarray]]` | Matches outer list size |

**Auto-split Special Cases:**

| Condition | Example Input | Interpretation | Output |
|---|---|---|---|
| Multi-input + first element is total\_size | `[concatenated_array]` | Auto-split single | `List[np.ndarray]` |
| Multi-input + all elements are total\_size| `[concat1, concat2, concat3]`| Auto-split batch | `List[List[np.ndarray]]`|

**Example:**

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

#### run\_multi\_input (Dictionary)

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

### Asynchronous Inference API

#### run\_async

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

#### run\_async\_multi\_input

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

---

#### wait

```python
def wait(job_id: int) -> List[np.ndarray]
```

-----

## Input Format Analysis Logic

### Python Input Analysis Flow

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

### `List[np.ndarray]` Analysis Details

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

## Output Format Rules

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

### Asynchronous Output

| API | Immediate Return | After `wait` |
|---|---|---|
| C++ RunAsync | `int` (jobId) | `TensorPtrs` |
| Python run\_async| `int` (jobId) | `List[np.ndarray]` |

## Special Cases

### Auto-Split Condition

**C++:**

```cpp
bool shouldAutoSplitInput() const {
    return _isMultiInput && _inputTasks.size() == 1;
}
```

**Python:**

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

### Error Conditions

| Condition | Error Type | Message |
|---|---|---|
| Multi-input + invalid size | `ValueError` | "Invalid input count for multi-input model" |
| Async + batch | `ValueError` | "Batch inference not supported in async" |
| Empty input | `ValueError` | "Input data cannot be empty" |
| Type mismatch | `TypeError` | "Expected np.ndarray or List[np.ndarray]" |

### Output Buffer Handling

#### Python Output Buffer Matrix

| Input Format | Output Buffer Format | Handling |
|---|---|---|
| Single Inference | `None` | Auto-allocated |
| Single Inference | `List[np.ndarray]` | User-provided |
| Single Inference | `np.ndarray` (total\_size) | Used after auto-split |
| Batch Inference | `List[List[np.ndarray]]` | Explicit batch buffer |
| Batch Inference | `List[np.ndarray]` | Flattened batch buffer |

## Performance Considerations

### Memory Allocation

| Method | Pros | Cons |
|---|---|---|
| Auto-allocation (No Buffer) | Ease of use | Memory allocated on every call |
| User-provided (With Buffer) | Performance optimization | Complex memory management |

### Inference Method

| Method | Use Case | Characteristics |
|---|---|---|
| Synchronous | Simple processing | Sequential execution |
| Asynchronous | High throughput | Requires callback management |
| Batch | Bulk processing | Increased memory usage |

-----
