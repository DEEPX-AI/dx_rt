#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>   
#include <pybind11/stl.h>      
#include <pybind11/functional.h> 
#include <pybind11/chrono.h>     

#include <optional> 
#include <vector>
#include <string>
#include <iostream> 
#include <numeric>  
#include <stdexcept>

#include "dxrt/dxrt_api.h" 

#if defined(_MSC_VER) && !defined(SSIZE_T_DEFINED)
#include <BaseTsd.h> 
typedef SSIZE_T ssize_t;
#define SSIZE_T_DEFINED
#endif

namespace dxrt
{
namespace py = pybind11;

// Wrapper for Python objects passed as user arguments.
struct UserArgWrapper {
    py::object user_pyObj;       
    py::object output_arg_pyObj;

    explicit UserArgWrapper(py::object user_obj, py::object output_obj = py::none())
        : user_pyObj(std::move(user_obj)), output_arg_pyObj(std::move(output_obj)) {}

    ~UserArgWrapper() = default;

    UserArgWrapper(const UserArgWrapper&) = delete;
    UserArgWrapper& operator=(const UserArgWrapper&) = delete;
    UserArgWrapper(UserArgWrapper&&) = delete;
    UserArgWrapper& operator=(UserArgWrapper&&) = delete;
};

std::string pyFormatDescriptorTable[DataType::MAX_TYPE]; 

void initializePyFormatDescriptorTable() {
    pyFormatDescriptorTable[DataType::NONE_TYPE] = py::format_descriptor<uint8_t>::format(); // Placeholder, should ideally not be used.
    pyFormatDescriptorTable[DataType::FLOAT] = py::format_descriptor<float>::format();     // Python float32
    pyFormatDescriptorTable[DataType::UINT8] = py::format_descriptor<uint8_t>::format();     // Python uint8
    pyFormatDescriptorTable[DataType::INT8] = py::format_descriptor<int8_t>::format();       // Python int8
    pyFormatDescriptorTable[DataType::UINT16] = py::format_descriptor<uint16_t>::format();    // Python uint16
    pyFormatDescriptorTable[DataType::INT16] = py::format_descriptor<int16_t>::format();     // Python int16
    pyFormatDescriptorTable[DataType::INT32] = py::format_descriptor<int32_t>::format();     // Python int32
    pyFormatDescriptorTable[DataType::INT64] = py::format_descriptor<int64_t>::format();     // Python int64
    pyFormatDescriptorTable[DataType::UINT32] = py::format_descriptor<uint32_t>::format();    // Python uint32
    pyFormatDescriptorTable[DataType::UINT64] = py::format_descriptor<uint64_t>::format();    // Python uint64
    // Structured types are viewed as byte arrays for flexibility in Python.
    pyFormatDescriptorTable[DataType::BBOX] = py::format_descriptor<uint8_t>::format();
    pyFormatDescriptorTable[DataType::FACE] = py::format_descriptor<uint8_t>::format();
    pyFormatDescriptorTable[DataType::POSE] = py::format_descriptor<uint8_t>::format();
}

static struct InitializePyFormatDescriptorTable {
    InitializePyFormatDescriptorTable() {
        initializePyFormatDescriptorTable();
    }
} initPyFormatDescriptorTable_instance; 

// Retrieves the NumPy format descriptor string for a given DXRT DataType.
std::string pyGetFormatDescriptor(DataType dtype) {
    if (dtype >= DataType::NONE_TYPE && dtype < DataType::MAX_TYPE) {
        const auto& desc = pyFormatDescriptorTable[static_cast<int>(dtype)]; // Use static_cast<int> if DataType is enum class
        if (desc.empty()) {
            throw std::runtime_error("Format descriptor for DataType ID " + std::to_string(static_cast<int>(dtype)) + " is not initialized or invalid.");
        }
        return desc;
    }
    throw std::runtime_error("Unknown DataType ID in pyGetFormatDescriptor: " + std::to_string(static_cast<int>(dtype)));
}

// Converts a C++ TensorPtr to a Python NumPy array.
void convertToPyArray(const TensorPtr& cpp_tensor,
                      py::handle base_python_array_for_view, 
                      std::vector<py::array>& py_array_list) {
    if (!cpp_tensor) {
        py_array_list.emplace_back(py::none());
        return;
    }

    DataType dtype = cpp_tensor->type();
    void* data_ptr = cpp_tensor->data(); 
    const auto& shape_cpp = cpp_tensor->shape(); // Expected to be actual (dynamic) shape
    size_t original_elem_size_bytes = cpp_tensor->elem_size(); // Expected to be C++ element/struct size

    if (!data_ptr && cpp_tensor->size_in_bytes() > 0) {
        std::cerr << "Warning (convertToPyArray): Tensor '" << cpp_tensor->name() 
                  << "' has size > 0 but null data pointer. Creating empty NumPy array." << std::endl;
        py_array_list.emplace_back(py::array(py::dtype(pyGetFormatDescriptor(dtype)), std::vector<ssize_t>{0}));
        return;
    }
    
    std::vector<ssize_t> py_final_shape; // Shape for the Python NumPy array
    for(long long s : shape_cpp) py_final_shape.push_back(static_cast<ssize_t>(s));

    if (cpp_tensor->size_in_bytes() == 0) {
        if (py_final_shape.empty() && shape_cpp.empty()) { // Scalar with zero size / uninitialized
            py_final_shape.push_back(0); 
        }
        py_array_list.emplace_back(py::array(py::dtype(pyGetFormatDescriptor(dtype)), py_final_shape));
        return;
    }

    std::vector<ssize_t> py_final_strides;
    size_t py_final_itemsize_bytes;
    std::string py_final_format_descriptor;

    if (dtype == DataType::BBOX || dtype == DataType::FACE || dtype == DataType::POSE) {
        py_final_itemsize_bytes = 1; // Python views these as byte arrays
        py_final_format_descriptor = py::format_descriptor<uint8_t>::format();

        if (shape_cpp.size() < 2) {
            // [batch_for_tensor, actual_item_count]. This could be an error or an unexpected format.
            throw std::runtime_error("Tensor for BBOX/FACE/POSE type has < 2 dimensions in its shape: " + cpp_tensor->name());
        }

        ssize_t batch_size_for_tensor = static_cast<ssize_t>(shape_cpp[0]); // Usually 1 for a single tensor from a batch item
        ssize_t actual_item_count = static_cast<ssize_t>(shape_cpp[1]);   // Dynamically determined count
        
        size_t cpp_struct_size_bytes = original_elem_size_bytes;

        size_t expected_sizeof_struct = 0;
        if (dtype == DataType::BBOX) expected_sizeof_struct = sizeof(DeviceBoundingBox_t);
        else if (dtype == DataType::FACE) expected_sizeof_struct = sizeof(DeviceFace_t);
        else if (dtype == DataType::POSE) expected_sizeof_struct = sizeof(DevicePose_t);

        if (cpp_struct_size_bytes == 0 && expected_sizeof_struct > 0) {
            cpp_struct_size_bytes = expected_sizeof_struct; 
        }
        if (cpp_struct_size_bytes == 0) {
            throw std::runtime_error("Struct size for BBOX/FACE/POSE for tensor '" + cpp_tensor->name() + "' is zero.");
        }
        if (expected_sizeof_struct > 0 && cpp_struct_size_bytes != expected_sizeof_struct) {
             std::cerr << "Warning (convertToPyArray): Mismatch between C++ Tensor's elem_size ("
                       << cpp_struct_size_bytes << ") and expected sizeof(DeviceStruct) ("
                       << expected_sizeof_struct << ") for tensor " << cpp_tensor->name()
                       << ". Using C++ Tensor's elem_size." << std::endl;
        }

        // Python shape becomes: [batch_for_this_tensor, actual_item_count, bytes_per_item_struct]
        if (actual_item_count == 0) {
            py_final_shape = {batch_size_for_tensor, 0, static_cast<ssize_t>(cpp_struct_size_bytes)};
        } else {
            py_final_shape = {batch_size_for_tensor, actual_item_count, static_cast<ssize_t>(cpp_struct_size_bytes)};
        }
        py_final_strides = py::detail::c_strides(py_final_shape, py_final_itemsize_bytes);
        
    } else { 
        if (py_final_shape.empty() && cpp_tensor->size_in_bytes() > 0) { 
            py_final_shape.push_back(1); 
        }
        
        py_final_itemsize_bytes = original_elem_size_bytes;
        py_final_format_descriptor = pyGetFormatDescriptor(dtype);
        py_final_strides = py::detail::c_strides(py_final_shape, py_final_itemsize_bytes);
    }
    
    bool readonly_flag = true; 

    py::buffer_info info(
        data_ptr,
        py_final_itemsize_bytes,
        py_final_format_descriptor,
        py_final_shape.size(),
        py_final_shape,
        py_final_strides,
        readonly_flag 
    );

    py::array arr(info, base_python_array_for_view);

    /*
    // Debug logging (as per user's last provided code)
    void* py_arr_data_ptr = readonly_flag ? const_cast<void*>(arr.data()) : arr.mutable_data();
    std::cout << "  convertToPyArray (base passed: " << (!base_python_array_for_view.is_none())
              << ", readonly: " << readonly_flag
              << "): C++ Tensor data_ptr = " << data_ptr
              << ", py::array data_ptr = " << py_arr_data_ptr
              << ", Match = " << (data_ptr == py_arr_data_ptr) << std::endl;
    */
    py_array_list.push_back(std::move(arr));
}


// Synchronous inference for single or batch.
py::object pyRun(InferenceEngine &ie,
                 const py::object &py_inputs,
                 const py::object &py_output_buffers, 
                 const py::object &py_user_args) {   

    py::gil_scoped_acquire gil; 

    bool is_batch = false;
    if (!py_inputs.is_none() && py::isinstance<py::list>(py_inputs)) {
        py::list inputs_top_list = py_inputs.cast<py::list>();
        if (!inputs_top_list.empty()) {
            if (py::isinstance<py::list>(inputs_top_list[0])) { 
                 py::list first_inner_list = inputs_top_list[0].cast<py::list>();
                if (!first_inner_list.empty() && py::isinstance<py::array>(first_inner_list[0])) { is_batch = true; }
                else if (first_inner_list.empty()){ is_batch = true; } 
                else { throw py::type_error("Batch input items must be lists of numpy arrays."); }
            } else if (py::isinstance<py::array>(inputs_top_list[0])) { is_batch = false;  }
            else { throw py::type_error("Input data elements must be np.ndarray or List[np.ndarray]."); }
        } else { throw py::value_error("Input data list cannot be empty."); }
    } else { throw py::type_error("Input data must be a list."); }

    std::vector<void*> cpp_batch_input_ptrs;
    void* cpp_single_input_ptr = nullptr;
    std::vector<void*> cpp_batch_output_ptrs; 
    void* cpp_single_output_ptr = nullptr;   
    std::vector<UserArgWrapper*> batch_user_arg_wrappers; 
    UserArgWrapper* single_user_arg_wrapper = nullptr;   
    std::vector<void*> user_args_raw_ptrs_for_batch_call; 

    if (is_batch) {
        py::list batch_inputs_py = py_inputs.cast<py::list>();
        for (const auto& batch_item_py_handle : batch_inputs_py) { 
            py::list item_tensors_py = batch_item_py_handle.cast<py::list>();
            if (item_tensors_py.empty()) { cpp_batch_input_ptrs.push_back(nullptr); } 
            else { cpp_batch_input_ptrs.push_back(item_tensors_py[0].cast<py::array>().request().ptr); }
        }
        if (!py_output_buffers.is_none() && py::isinstance<py::list>(py_output_buffers)) { 
            py::list batch_outputs_py = py_output_buffers.cast<py::list>();
            if (py::len(batch_outputs_py) != cpp_batch_input_ptrs.size()) { throw py::value_error("Output buffer sets mismatch input batches."); }
            for (const auto& batch_item_outputs_py_handle : batch_outputs_py) {
                py::list item_output_tensors_py = batch_item_outputs_py_handle.cast<py::list>();
                if (item_output_tensors_py.empty()) { cpp_batch_output_ptrs.push_back(nullptr); } 
                else { cpp_batch_output_ptrs.push_back(item_output_tensors_py[0].cast<py::array>().request().ptr); }
            }
        } else { throw py::value_error("output_buffers must be provided for batch sync inference."); }

        if (!py_user_args.is_none()) { 
            py::list user_args_list_py = py_user_args.cast<py::list>();
            if (py::len(user_args_list_py) != cpp_batch_input_ptrs.size()) { throw py::value_error("User_args mismatch input batches."); }
            for (const auto& arg_py_handle : user_args_list_py) {
                UserArgWrapper* wrapper = new UserArgWrapper(arg_py_handle.cast<py::object>());
                batch_user_arg_wrappers.push_back(wrapper); 
                user_args_raw_ptrs_for_batch_call.push_back(reinterpret_cast<void*>(wrapper));
            }
        } else { for (size_t i = 0; i < cpp_batch_input_ptrs.size(); ++i) { user_args_raw_ptrs_for_batch_call.push_back(nullptr); } }
    } else { 
        py::list inputs_list_py = py_inputs.cast<py::list>();
        if (inputs_list_py.empty()) throw py::value_error("Input tensor list cannot be empty for single inference.");
        cpp_single_input_ptr = inputs_list_py[0].cast<py::array>().request().ptr;
        if (!py_output_buffers.is_none() && py::isinstance<py::list>(py_output_buffers)) { 
            py::list output_list_py = py_output_buffers.cast<py::list>();
            if (!output_list_py.empty()) { cpp_single_output_ptr = output_list_py[0].cast<py::array>().request().ptr;}
        }
        if (!py_user_args.is_none()) { single_user_arg_wrapper = new UserArgWrapper(py_user_args.cast<py::object>());}
    }

    py::object original_py_output_buffers_obj = py_output_buffers;

    py::gil_scoped_release release_gil; 
    std::vector<TensorPtrs> cpp_batch_results; 
    TensorPtrs cpp_single_results;      

    try {
        if (is_batch) {
            cpp_batch_results = ie.Run(cpp_batch_input_ptrs, cpp_batch_output_ptrs, user_args_raw_ptrs_for_batch_call);
        } else {
            cpp_single_results = ie.Run(cpp_single_input_ptr, reinterpret_cast<void*>(single_user_arg_wrapper), cpp_single_output_ptr);
        }
    } catch (const std::exception& e) {
        py::gil_scoped_acquire acquire_gil_for_cleanup; 
        if (single_user_arg_wrapper) { delete single_user_arg_wrapper; single_user_arg_wrapper = nullptr; }
        for (UserArgWrapper* wrapper : batch_user_arg_wrappers) { if (wrapper) delete wrapper; }
        batch_user_arg_wrappers.clear();
        throw; 
    }
    
    { // Scope for GIL for cleanup
        py::gil_scoped_acquire acquire_gil_for_cleanup;
        if (single_user_arg_wrapper) { delete single_user_arg_wrapper; }
        for (UserArgWrapper* wrapper : batch_user_arg_wrappers) { if (wrapper) delete wrapper; }
        batch_user_arg_wrappers.clear(); 
    }

    py::gil_scoped_acquire acquire_gil_for_results;

    if (is_batch) {
        py::list py_final_batch_results_list; 
        py::list py_orig_output_buffers_top_list; 
        if (!original_py_output_buffers_obj.is_none() && py::isinstance<py::list>(original_py_output_buffers_obj)) {
            py_orig_output_buffers_top_list = original_py_output_buffers_obj.cast<py::list>();
        }

        for (size_t i = 0; i < cpp_batch_results.size(); ++i) { 
            const TensorPtrs& item_cpp_tensor_ptrs = cpp_batch_results[i];
            std::vector<py::array> item_py_arrays; 

            py::list original_py_item_output_arrays; 
            if (py_orig_output_buffers_top_list && i < py::len(py_orig_output_buffers_top_list)) {
                if (py::isinstance<py::list>(py_orig_output_buffers_top_list[i])) {
                    original_py_item_output_arrays = py_orig_output_buffers_top_list[i].cast<py::list>();
                }
            }

            for (size_t j = 0; j < item_cpp_tensor_ptrs.size(); ++j) {
                const TensorPtr& tensor_cpp_ptr = item_cpp_tensor_ptrs[j];
                py::handle base_for_this_tensor_view = py::none(); 
                if (original_py_item_output_arrays && j < py::len(original_py_item_output_arrays)) {
                    if (py::isinstance<py::array>(original_py_item_output_arrays[j])) {
                        base_for_this_tensor_view = original_py_item_output_arrays[j];
                    }
                }
                convertToPyArray(tensor_cpp_ptr, base_for_this_tensor_view, item_py_arrays);
            }
            py_final_batch_results_list.append(py::cast(item_py_arrays));
        }
        return py_final_batch_results_list;
    } else { 
        std::vector<py::array> py_final_single_results_list;
        py::list py_orig_single_output_list; 
        if (!original_py_output_buffers_obj.is_none() && py::isinstance<py::list>(original_py_output_buffers_obj)) {
            py_orig_single_output_list = original_py_output_buffers_obj.cast<py::list>();
        }

        for (size_t j = 0; j < cpp_single_results.size(); ++j) { 
            const TensorPtr& tensor_cpp_ptr = cpp_single_results[j];
            py::handle base_for_this_tensor_view = py::none();
            if (py_orig_single_output_list && j < py::len(py_orig_single_output_list)) {
                 if (py::isinstance<py::array>(py_orig_single_output_list[j])) {
                    base_for_this_tensor_view = py_orig_single_output_list[j];
                }
            }
            convertToPyArray(tensor_cpp_ptr, base_for_this_tensor_view, py_final_single_results_list);
        }
        return py::cast(py_final_single_results_list);
    }
}

// Asynchronous inference call.
int pyRunAsync(InferenceEngine &ie,
               const std::vector<py::array> &inputs_py, 
               const py::object &userArg_py,            
               const py::object &outputArg_py) { 

    py::gil_scoped_acquire gil_for_args; 

    if (inputs_py.empty()) {
        throw py::value_error("Input array list for async run cannot be empty.");
    }
    void* first_input_c_ptr = inputs_py[0].request().ptr;

    void* output_c_ptr_for_engine = nullptr;
    py::object output_base_obj_for_callback = py::none();

    if (!outputArg_py.is_none()) {
        if (py::isinstance<py::array>(outputArg_py)) {
            output_c_ptr_for_engine = outputArg_py.cast<py::array>().request().ptr;
            output_base_obj_for_callback = outputArg_py; 
        } else if (py::isinstance<py::list>(outputArg_py)) {
            py::list output_list = outputArg_py.cast<py::list>();
            if (!output_list.empty()) {
                if(py::isinstance<py::array>(output_list[0])) {
                    output_c_ptr_for_engine = output_list[0].cast<py::array>().request().ptr;
                    output_base_obj_for_callback = output_list; // Store the whole list for callback
                } else {
                    throw py::type_error("Async output_arg list must contain numpy arrays.");
                }
            } 
        } else {
            throw py::type_error("Async output_arg must be np.ndarray, List[np.ndarray], or None.");
        }
    }
    
    UserArgWrapper* wrapper = new UserArgWrapper(userArg_py, output_base_obj_for_callback);
    
    gil_for_args.disarm(); 
    py::gil_scoped_release release_gil_for_c_call; 

    return ie.RunAsync(first_input_c_ptr, reinterpret_cast<void*>(wrapper), output_c_ptr_for_engine);
}

// Registers a Python callback for asynchronous completions.
void pyRegisterCallback(InferenceEngine &ie, const py::object &pyCallback_obj) {
    py::gil_scoped_acquire gil; 

    if (pyCallback_obj.is_none()) {
        ie.RegisterCallback(nullptr); 
        return;
    }
    if (!py::isinstance<py::function>(pyCallback_obj)){
        throw py::type_error("Callback must be a Python function or None.");
    }

    py::object captured_py_callback = pyCallback_obj;

    ie.RegisterCallback(
        [captured_py_callback] 
        (TensorPtrs &outputs_cpp, void *userArg_ptr_raw) -> int {
        
        py::gil_scoped_acquire gil_in_callback; 

        UserArgWrapper* wrapper = reinterpret_cast<UserArgWrapper*>(userArg_ptr_raw);
        py::object user_data_to_py_callback = py::none();
        py::object base_outputs_obj_from_async = py::none();

        if (wrapper) {
            user_data_to_py_callback = wrapper->user_pyObj;
            base_outputs_obj_from_async = wrapper->output_arg_pyObj; 
        }

        std::vector<py::array> py_outputs_for_callback;
        try {
            py::list base_list_for_tensors; 
            bool base_is_list = false;
            if (!base_outputs_obj_from_async.is_none() && py::isinstance<py::list>(base_outputs_obj_from_async)) {
                base_list_for_tensors = base_outputs_obj_from_async.cast<py::list>();
                base_is_list = true;
            }

            for(size_t j = 0; j < outputs_cpp.size(); ++j) { 
                const auto &output_tensor_cpp_ptr = outputs_cpp[j];
                py::handle base_for_this_tensor = py::none();
                
                if (!base_outputs_obj_from_async.is_none()) {
                    if (base_is_list) { 
                        if (j < py::len(base_list_for_tensors) && py::isinstance<py::array>(base_list_for_tensors[j])) {
                            base_for_this_tensor = base_list_for_tensors[j];
                        }
                    } else if (py::isinstance<py::array>(base_outputs_obj_from_async) && j == 0) {
                        base_for_this_tensor = base_outputs_obj_from_async;
                    }
                }
                convertToPyArray(output_tensor_cpp_ptr, base_for_this_tensor, py_outputs_for_callback);
            }
        } catch (const std::exception& e) {
             std::cerr << "C++ exception during tensor conversion in callback: " << e.what() << std::endl;
            if (wrapper) { delete wrapper; } 
            return -1; 
        }

        try {
            if (!captured_py_callback.is_none()) { 
                 captured_py_callback(py_outputs_for_callback, user_data_to_py_callback);
            }
        } catch (py::error_already_set &e) {
            std::cerr << "Python exception occurred in callback: ";
            e.restore(); PyErr_Print(); std::cerr << std::endl; 
            if (wrapper) { delete wrapper; }
            return -1; 
        } catch (const std::exception &e) {
            std::cerr << "C++ exception occurred during Python callback execution: " << e.what() << std::endl;
            if (wrapper) { delete wrapper; }
            return -1; 
        }
        
        if (wrapper) { 
            delete wrapper;
        }
        return 0; 
    });
}

// Waits for an asynchronous job and retrieves its output.
std::vector<py::array> pyWait(InferenceEngine &ie, int jobId) {
    TensorPtrs outputs_cpp;
    {
        py::gil_scoped_release release_gil;
        outputs_cpp = ie.Wait(jobId); 
    }
    py::gil_scoped_acquire acquire_gil;
    std::vector<py::array> result_py_list;
    for(const auto &output_tensor_cpp_ptr : outputs_cpp) {
         convertToPyArray(output_tensor_cpp_ptr, py::none(), result_py_list);
    }
    return result_py_list;
}

// Runs benchmark.
float pyRunBenchmark(InferenceEngine &ie, int num_loops, const py::object &inputs_py_obj) {
    void* input_c_ptr = nullptr;
    py::gil_scoped_acquire gil_for_args; 
    if (!inputs_py_obj.is_none()) {
        if (py::isinstance<py::list>(inputs_py_obj)) {
            py::list input_list_py = inputs_py_obj.cast<py::list>();
            if (!input_list_py.empty() && py::isinstance<py::array>(input_list_py[0])) {
                input_c_ptr = input_list_py[0].cast<py::array>().request().ptr;
            } else if (!input_list_py.empty()) {
                throw py::type_error("Benchmark input_data list must contain numpy arrays.");
            }
        } else {
            throw py::type_error("Benchmark input_data must be a List[np.ndarray] or None.");
        }
    } 
    gil_for_args.disarm();
    py::gil_scoped_release release_gil_for_c_call;
    return ie.RunBenchmark(num_loops, input_c_ptr);
}

// Validates an NPU device.
std::vector<py::array> pyValidateDevice(InferenceEngine &ie, const std::vector<py::array> &inputs_py, int deviceId) {
    if (inputs_py.empty()) {
        throw py::value_error("Input array list for ValidateDevice cannot be empty.");
    }
    void* input_c_ptr = inputs_py[0].request().ptr; 
    TensorPtrs outputs_cpp;
    {
        py::gil_scoped_release release_gil;
        outputs_cpp = ie.ValidateDevice(input_c_ptr, deviceId);
    }
    py::gil_scoped_acquire acquire_gil;
    std::vector<py::array> result_py_list;
    for(const auto &output_tensor_cpp_ptr : outputs_cpp) {
        convertToPyArray(output_tensor_cpp_ptr, py::none(), result_py_list);
    }
    return result_py_list;
}

// Retrieves output tensors of all tasks (for debugging/advanced use).
std::vector<std::vector<py::array>> pyGetAllTaskOutputs(InferenceEngine &ie) {
    std::vector<TensorPtrs> cpp_task_outputs_vector;
    {
        py::gil_scoped_release release_gil;
        cpp_task_outputs_vector = ie.GetAllTaskOutputs();
    }
    py::gil_scoped_acquire acquire_gil;
    std::vector<std::vector<py::array>> py_results_vector;
    for (const auto& task_outputs_cpp : cpp_task_outputs_vector) {
        std::vector<py::array> py_single_task_outputs_list;
        for (const auto& tensor_cpp_ptr : task_outputs_cpp) { 
            convertToPyArray(tensor_cpp_ptr, py::none(), py_single_task_outputs_list); 
        }
        py_results_vector.push_back(py_single_task_outputs_list);
    }
    return py_results_vector;
}

// Helper to convert C++ Tensors (metadata list) to Python list of dicts.
std::vector<py::dict> tensorsToPyDicts(Tensors& tensors_cpp) { 
    std::vector<py::dict> py_dicts_list;
    for (auto& tensor_info_cpp : tensors_cpp) { 
        py::dict d;
        d["name"] = tensor_info_cpp.name();          
        d["shape"] = tensor_info_cpp.shape(); 
        d["dtype"] = DataTypeToString(tensor_info_cpp.type()); 
        d["elem_size"] = tensor_info_cpp.elem_size();
        py_dicts_list.push_back(d);
    }
    return py_dicts_list;
}

// Get input tensor(s) information.
std::vector<py::dict> pyGetInputsInfo(InferenceEngine &ie, py::object ptr_py_obj, uint64_t phyAddr) {
    void* ptr_c = nullptr;
    if (!ptr_py_obj.is_none()) {
        if(py::isinstance<py::array>(ptr_py_obj)) { 
            ptr_c = ptr_py_obj.cast<py::array>().request().ptr;
        } else {
            throw py::type_error("Argument 'ptr' for get_inputs_info must be a NumPy array or None.");
        }
    }
    Tensors tensors_cpp = ie.GetInputs(ptr_c, phyAddr); 
    return tensorsToPyDicts(tensors_cpp);
}

// Get input tensor(s) information for a specific device.
std::vector<std::vector<py::dict>> pyGetInputsInfoDev(InferenceEngine &ie, int devId) {
    std::vector<Tensors> list_tensors_cpp = ie.GetInputs(devId); 
    std::vector<std::vector<py::dict>> result_py_list_of_lists;
    for(auto& tensors_item_cpp : list_tensors_cpp) { 
        result_py_list_of_lists.push_back(tensorsToPyDicts(tensors_item_cpp));
    }
    return result_py_list_of_lists;
}

// Get output tensor(s) information.
std::vector<py::dict> pyGetOutputsInfo(InferenceEngine &ie, py::object ptr_py_obj, uint64_t phyAddr) {
    void* ptr_c = nullptr;
    if (!ptr_py_obj.is_none()) {
         if(py::isinstance<py::array>(ptr_py_obj)) {
            ptr_c = ptr_py_obj.cast<py::array>().request().ptr;
        } else {
            throw py::type_error("Argument 'ptr' for get_outputs_info must be a NumPy array or None.");
        }
    }
    Tensors tensors_cpp = ie.GetOutputs(ptr_c, phyAddr); 
    return tensorsToPyDicts(tensors_cpp);               
}

// Get input data types as strings.
std::vector<std::string> pyGetInputDataType(InferenceEngine& ie) {
    Tensors input_tensors = ie.GetInputs(); 
    std::vector<std::string> ret;
    for (auto& tensor : input_tensors) { 
        ret.emplace_back(DataTypeToString(tensor.type()));
    }
    return ret;
}

// Get output data types as strings.
std::vector<std::string> pyGetOutputDataType(InferenceEngine& ie) {
    Tensors output_tensors = ie.GetOutputs(); 
    std::vector<std::string> ret;
    for (auto& tensor : output_tensors) { 
        ret.emplace_back(DataTypeToString(tensor.type()));
    }
    return ret;
}

// Get model's task execution order.
std::vector<std::string> pyGetTaskOrder(InferenceEngine &ie) {
    return ie.GetTaskOrder(); 
}

// For Python 'with' statement: __enter__
void pyInferenceEngine_Enter(InferenceEngine &ie) {
    std::ignore = ie; 
}

// For Python 'with' statement: __exit__
void pyInferenceEngine_Exit(InferenceEngine &ie) {
    ie.Dispose(); 
}

// Module definition
PYBIND11_MODULE(_pydxrt, m) {
    m.doc() = "Python bindings for DXRT Inference Engine";

    // InferenceOption class binding
    py::class_<InferenceOption>(m, "InferenceOption")
        .def(py::init<>())
        .def_readwrite("useORT", &InferenceOption::useORT) 
        .def_readwrite("boundOption", &InferenceOption::boundOption) 
        .def_property("devices",
            [](const InferenceOption &opt) { return py::cast(opt.devices); }, // Getter
            [](InferenceOption &opt, const std::vector<int> &new_devices) { opt.devices = new_devices; } // Setter
        );

    // InferenceEngine class binding (member functions are bound directly)
    py::class_<InferenceEngine>(m, "InferenceEngine")
        .def(py::init<const std::string&, InferenceOption&>(),
             py::arg("model_path"), 
             py::arg("inference_option")) // Python __init__ in InferenceEngine.py handles providing default
        .def("get_input_size", &InferenceEngine::GetInputSize)
        .def("get_output_size", &InferenceEngine::GetOutputSize)
        .def("get_latency", &InferenceEngine::GetLatency)
        .def("get_npu_inference_time", &InferenceEngine::GetNpuInferenceTime)
        .def("get_latency_list", &InferenceEngine::GetLatencyVector)
        .def("get_npu_inference_time_list", &InferenceEngine::GetNpuInferenceTimeVector)
        .def("get_latency_mean", &InferenceEngine::GetLatencyMean)
        .def("get_npu_inference_time_mean", &InferenceEngine::GetNpuInferenceTimeMean)
        .def("get_latency_std", &InferenceEngine::GetLatencyStdDev)
        .def("get_npu_inference_time_std", &InferenceEngine::GetNpuInferenceTimeStdDev)
        .def("get_latency_count", &InferenceEngine::GetLatencyCnt)
        .def("get_npu_inference_time_count", &InferenceEngine::GetNpuInferenceTimeCnt)
        .def("get_bitmatch_mask", &InferenceEngine::GetBitmatchMask, py::arg("index") = 0)
        .def("get_num_tail_tasks", &InferenceEngine::GetNumTailTasks)
        .def("get_compile_type", &InferenceEngine::GetCompileType)
        .def("is_ppu", &InferenceEngine::IsPPU)
        .def("dispose", &InferenceEngine::Dispose);
        // Note: Python class __enter__ and __exit__ will call the module-level C functions below.

    // Module-level functions that operate on InferenceEngine instances
    m.def("run", &pyRun, py::arg("engine"), py::arg("inputs"),
          py::arg("output_buffers") = py::none(), py::arg("user_args") = py::none(),
          "Runs inference synchronously. Handles single or batch. GIL released during C++ call.");

    m.def("run_async", &pyRunAsync, py::arg("engine"), py::arg("inputs"),
          py::arg("user_arg") = py::none(), py::arg("output_arg") = py::none(),
          "Runs inference asynchronously. GIL released during C++ call.");

    m.def("register_callback", &pyRegisterCallback, py::arg("engine"), py::arg("callback"),
          "Registers a Python callback for asynchronous inference completion.");

    m.def("wait", &pyWait, py::arg("engine"), py::arg("job_id"), 
          "Waits for an asynchronous job to complete and returns its output. GIL released during C++ call.");

    m.def("run_benchmark", &pyRunBenchmark, py::arg("engine"), py::arg("num_loops"), 
          py::arg("inputs") = py::none(), 
          "Runs a benchmark and returns average FPS. GIL released during C++ call.");

    m.def("validate_device", &pyValidateDevice, py::arg("engine"), py::arg("inputs"), 
          py::arg("device_id") = 0, 
          "Validates an NPU device with given inputs. GIL released during C++ call.");
    
    m.def("get_inputs_info", &pyGetInputsInfo, py::arg("engine"), 
          py::arg("ptr") = py::none(), py::arg("phy_addr") = 0,
          "Get input tensor(s) information (name, shape, dtype).");

    m.def("get_inputs_info_dev", &pyGetInputsInfoDev, py::arg("engine"), py::arg("dev_id"),
           "Get input tensor(s) information for a specific NPU device.");

    m.def("get_outputs_info", &pyGetOutputsInfo, py::arg("engine"), 
          py::arg("ptr") = py::none(), py::arg("phy_addr") = 0,
           "Get output tensor(s) information (name, shape, dtype).");
    
    m.def("get_input_dtype", &pyGetInputDataType, py::arg("engine"),
          "Get input data types as a list of strings.");
    m.def("get_output_dtype", &pyGetOutputDataType, py::arg("engine"),
          "Get output data types as a list of strings.");
    m.def("get_task_order", &pyGetTaskOrder, py::arg("engine"),
          "Get the execution order of tasks in the model.");

    m.def("get_all_task_outputs", &pyGetAllTaskOutputs, py::arg("engine"),
          "Gets all task outputs (for debugging). GIL released during C++ call.");
    
    // Assuming ParseModel is a free function: `std::string dxrt::ParseModel(const std::string&)`
    m.def("parse_model", &ParseModel, py::arg("model_path"), 
          "Parses a model file and returns its string representation or info.");

    // For Python's `with InferenceEngine(...) as ie:` context manager
    m.def("inference_engine_enter", &pyInferenceEngine_Enter, py::arg("engine"));
    m.def("inference_engine_exit", &pyInferenceEngine_Exit, py::arg("engine"));

}

} // namespace dxrt