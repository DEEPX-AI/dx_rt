# Multi-Input Model Inference Examples

This directory contains examples of inference using multi-input models in DXRT.

## File Structure

```
examples/
├── cpp/
│   └── multi_input_model_inference
│       ├── multi_input_model_inference.cpp  # C++ example code
│       └── CMakeLists.txt                   # Build configuration
├── python/
│   └── multi_input_model_inference.py       # Python example code
└── README_multi_input_model_inference.md    # This file
```

## Example Content

### C++ Example (`multi_input_model_inference.cpp`)

This file includes the following 9 examples:

#### Single Inference - No Output Buffer (Auto-allocated)

1.  **Dictionary Format (No Buffer)**: Maps input tensors by name; output is auto-allocated.
2.  **Vector Format (No Buffer)**: Provides input tensors as a vector in order; output is auto-allocated.
3.  **Auto-Split (No Buffer)**: Automatically splits a single concatenated buffer; output is auto-allocated.

#### Single Inference - With Output Buffer (User-provided)

4.  **Dictionary Format (With Buffer)**: Maps input tensors by name; uses a user-provided output buffer.
5.  **Vector Format (With Buffer)**: Provides input tensors as a vector in order; uses a user-provided output buffer.
6.  **Auto-Split (With Buffer)**: Automatically splits a single concatenated buffer; uses a user-provided output buffer.

#### Batch Inference

7.  **Batch Explicit**: Processes batch samples with an explicit structure.

#### Asynchronous Inference

8.  **Async Callback**: Asynchronous inference based on a callback function.
9.  **Simple Async**: Simple asynchronous inference.

### Python Example (`multi_input_model_inference.py`)

This file includes the following 10 examples:

#### Single Inference - No Output Buffer (Auto-allocated)

1.  **Dictionary Format (No Buffer)**: Provides input as a dictionary; output is auto-allocated.
2.  **Vector Format (No Buffer)**: Provides input as a list; output is auto-allocated.
3.  **Auto-Split (No Buffer)**: Automatically splits a single concatenated buffer; output is auto-allocated.

#### Single Inference - With Output Buffer (User-provided)

4.  **Dictionary Format (With Buffer)**: Provides input as a dictionary; uses a user-provided output buffer.
5.  **Vector Format (With Buffer)**: Provides input as a list; uses a user-provided output buffer.
6.  **Auto-Split (With Buffer)**: Automatically splits a single concatenated buffer; uses a user-provided output buffer.

#### Batch Inference

7.  **Explicit Batch Inference**: Inference with an explicit batch format (`List[List[np.ndarray]]`).
8.  **Flattened Batch Inference**: Inference with a flattened batch format (`List[np.ndarray]`).

#### Asynchronous Inference

9.  **Asynchronous Inference (Callback)**: Asynchronous inference using a callback.
10. **Simple Asynchronous Inference**: Asynchronous inference using the `run_async` method.

### Enhanced Validation Features

All examples perform the following enhanced output validation:

#### Validation Checks

  - **Output Existence**: Checks for `None` or empty lists.
  - **Data Type**: Verifies `numpy.ndarray`/`TensorPtr` types.
  - **Tensor Size**: Checks for empty tensors (`size=0`).
  - **Shape Validity**: Checks for invalid shapes.
  - **Numerical Validity**: Checks for `NaN` and `Inf` values (Python).
  - **Pointer Validity**: Checks for `null` pointers (C++).
  - **Batch Structure**: Verifies the correct nested structure of batch outputs.

#### Output Validation Logic

```cpp
// C++ Validation Example
bool validateOutputs(const dxrt::TensorPtrs& outputs, size_t expectedCount, const std::string& testName) {
    // 1. Check for existence
    if (outputs.empty()) return false;
    
    // 2. Verify count
    if (outputs.size() != expectedCount) return false;
    
    // 3. Validate each tensor
    for (const auto& output : outputs) {
        if (!output || output->size_in_bytes() == 0 || 
            output->shape().empty() || !output->data()) {
            return false;
        }
    }
    return true;
}
```

```python
# Python Validation Example
def validate_outputs(outputs, expected_count, test_name):
    # 1. Basic checks
    if outputs is None or not isinstance(outputs, list):
        return False
    
    # 2. Differentiate batch/single
    is_batch = isinstance(outputs[0], list) if outputs else False
    
    # 3. Validate each tensor (including NaN/Inf)
    for output in outputs:
        if not isinstance(output, np.ndarray) or output.size == 0:
            return False
        if np.any(np.isnan(output)) or np.any(np.isinf(output)):
            return False
    
    return True
```

### Running the C++ Example

```bash
./multi_input_model_inference <model_path>
```

Example:

```bash
./multi_input_model_inference /path/to/your/multi_input_model.dxnn
```

### Running the Python Example

#### Prerequisites

  - Python 3.8+
  - numpy
  - dx\_engine (DXRT Python package)

<!-- end list -->

```bash
cd examples/python
python multi_input_model_inference.py <model_path>
```

Example:

```bash
python multi_input_model_inference.py /path/to/your/multi_input_model.dxnn
```

## Model Requirements

To run these examples, you need a model that meets the following conditions:

1.  **Multi-Input Model**: A model with multiple input tensors.
2.  **Compiled Model**: A `.dxnn` file compiled with DX-COM.

## Example Output

### Model Information Output Example

```
============================================================
                      MODEL INFORMATION
============================================================
Multi-input model: Yes
Input tensor count: 2
Total input size: 614400 bytes
Total output size: 4000 bytes

Input tensor details:
  input1: 150528 bytes -> Task: npu_task_0
  input2: 463872 bytes -> Task: npu_task_0

Output tensor details:
  output1: 4000 bytes
============================================================
```

### Test Execution Output Example

```
============================================================
                       RUNNING TESTS
============================================================

1. Dictionary Format Single Inference (No Output Buffer)
   - Input: Dictionary mapping tensor names to data
   - API: ie.RunMultiInput(input_dict) - auto-allocated output

[RESULT] Dictionary Format (No Buffer): All outputs valid (1 tensors)
         Inference time: 15.34 ms

------------------------------------------------------------

...

============================================================
                       TEST SUMMARY
============================================================
* PASS | Dictionary Format (No Buffer)
* PASS | Vector Format (No Buffer)
* PASS | Auto-Split (No Buffer)
* PASS | Dictionary Format (With Buffer)
* PASS | Vector Format (With Buffer)
* PASS | Auto-Split (With Buffer)
* PASS | Batch Explicit
* PASS | Async Callback
* PASS | Simple Async
------------------------------------------------------------
Total: 9 | Passed: 9 | Failed: 0
 *** All tests passed successfully! ***
```

## Performance Tips

1.  **Pre-allocate Output Buffers**: Pre-allocate output buffers, as shown in the "With Buffer" tests, to improve performance.
2.  **Batch Processing**: Process multiple samples in a batch to increase throughput.
3.  **Asynchronous Processing**: Perform CPU and NPU tasks in parallel to improve overall performance.
4.  **Memory Reuse**: Reuse input/output buffers to reduce memory allocation overhead.
5.  **Optimize Validation**: Simplify validation logic in a production environment for better performance.

## Troubleshooting

### Common Errors

1.  **"This model is not a multi-input model"**: Occurs when using a multi-input API with a single-input model.
2.  **"Input tensor names mismatch"**: Occurs when the provided input tensor names do not match the model's requirements.
3.  **"Buffer size mismatch"**: Occurs when the input/output buffer sizes do not match the model's requirements.
4.  **"[ERROR] Output validation failed"**: Occurs when an issue is found during output validation (e.g., NaN, empty tensor).

### Debugging Tips

1.  **Check Model Information**: Use `ie.IsMultiInputModel()`, `ie.GetInputTensorNames()` to verify model information.
2.  **Check Buffer Sizes**: Use `ie.GetInputTensorSizes()`, `ie.GetOutputTensorSizes()` to check required buffer sizes.
3.  **Check Validation Logs**: Identify issues by reviewing the validation result messages for each test.
4.  **Understand Output Buffer Types**: Understand the differences between user-provided and auto-allocated output buffers.

## Characteristics of Test Scenarios

### No Output Buffer vs. With Output Buffer

  - **No Buffer**: Simple memory management; the inference engine handles allocation automatically.
  - **With Buffer**: Allows for performance optimization and memory reuse.

### Synchronous vs. Asynchronous

  - **Synchronous**: Simple implementation, sequential processing.
  - **Asynchronous**: Higher throughput, requires more complex callback management.

### Single vs. Batch

  - **Single**: Optimized for latency.
  - **Batch**: Optimized for throughput, uses more memory.

## Additional References

  - DXRT API Documentation
  - Multi-Input Model Inference Guide (`multi_input_model_inference_guide.md`)