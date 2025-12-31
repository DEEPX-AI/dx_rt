/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 *
 * This file uses cxxopts (MIT License) - Copyright (c) 2014 Jarryd Beck.
 * This file uses pybind11 (BSD 3-Clause License) - Copyright (c) 2016 Wenzel Jakob.
 */

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>

#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <map>

#include "dxrt/dxrt_api.h"
#include "dxrt/exception/exception.h"
#include "dxrt/device_info_status.h"
#include "dxrt/extern/cxxopts.hpp"

#if defined(_MSC_VER) && !defined(SSIZE_T_DEFINED)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#define SSIZE_T_DEFINED
#endif

namespace dxrt
{
namespace py = pybind11;

// Exception translation function to convert C++ exceptions to Python exceptions
void translateException(const std::exception_ptr& p) {
    try {
        if (p) std::rethrow_exception(p);
    } catch (const dxrt::FileNotFoundException& e) {
        PyErr_SetString(PyExc_FileNotFoundError, e.what());
    } catch (const dxrt::NullPointerException& e) {
        PyErr_SetString(PyExc_ValueError, e.what());
    } catch (const dxrt::FileIOException& e) {
        PyErr_SetString(PyExc_IOError, e.what());
    } catch (const dxrt::InvalidArgumentException& e) {
        PyErr_SetString(PyExc_ValueError, e.what());
    } catch (const dxrt::InvalidOperationException& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
    } catch (const dxrt::InvalidModelException& e) {
        PyErr_SetString(PyExc_ValueError, e.what());
    } catch (const dxrt::ModelParsingException& e) {
        PyErr_SetString(PyExc_ValueError, e.what());
    } catch (const dxrt::ServiceIOException& e) {
        PyErr_SetString(PyExc_IOError, e.what());
    } catch (const dxrt::DeviceIOException& e) {
        PyErr_SetString(PyExc_IOError, e.what());
    } catch (const dxrt::Exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
    } catch (const std::runtime_error& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
    } catch (const std::invalid_argument& e) {
        PyErr_SetString(PyExc_ValueError, e.what());
    } catch (const std::exception& e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
    } catch (...) {
        PyErr_SetString(PyExc_RuntimeError, "Unknown C++ exception occurred");
    }
}

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

// Converts a C++ TensorPtr to a Python NumPy array and appends to output vector.
void convertToPyArray(const TensorPtr& cpp_tensor,
                      py::handle base_python_array_for_view,
                      std::vector<py::array>& py_array_list) {
    // Ensure positional consistency even if tensor pointer is null.
    if (!cpp_tensor) {
        py_array_list.emplace_back(py::none());
        return;
    }

    DataType dtype = cpp_tensor->type();
    void* data_ptr = cpp_tensor->data();
    const auto& shape_cpp = cpp_tensor->shape();
    size_t elem_size_bytes = cpp_tensor->elem_size();

    // Will fill these for buffer_info
    std::vector<ssize_t> py_final_shape;
    std::vector<ssize_t> py_final_strides;
    size_t py_itemsize = 0;
    std::string py_format;

    // Null data with non-zero size_in_bytes indicates backend anomaly; create empty array.
    if (!data_ptr && cpp_tensor->size_in_bytes() > 0) {
        std::cerr << "Warning(convertToPyArray): tensor '" << cpp_tensor->name()
                  << "' reports size>0 but data() is null. Returning empty array." << std::endl;
        py_array_list.emplace_back(py::array(py::dtype(pyGetFormatDescriptor(dtype)), std::vector<ssize_t>{0}));
        return;
    }

    // Base shape copy (dynamic runtime shape) - copy as-is from C++ tensor
    for (auto d : shape_cpp) py_final_shape.push_back(static_cast<ssize_t>(d));

    // Handle empty tensors (no data)
    if (cpp_tensor->size_in_bytes() == 0) {
        if (py_final_shape.empty()) {
            // Scalar tensor with no data → shape=(0,)
            py_final_shape.push_back(0);
        }
        // Regular tensors keep original shape (e.g., [0, 256])
        py_array_list.emplace_back(py::array(py::dtype(pyGetFormatDescriptor(dtype)), py_final_shape));
        return;
    }

    // Handle scalar tensors with data
    if (py_final_shape.empty()) {
        // Scalar + data exists → shape=(1,) for NumPy compatibility
        py_final_shape.push_back(1);
    }

    // Structured type specialisation: reinterpret as bytes [B, N, struct_bytes]
    if (dtype == DataType::BBOX || dtype == DataType::FACE || dtype == DataType::POSE) {
        if (shape_cpp.size() < 2) {
            throw std::runtime_error("Structured tensor has <2 dims (expected [batch, count]) : " + cpp_tensor->name());
        }
        ssize_t batch_sz = static_cast<ssize_t>(shape_cpp[0]);
        ssize_t item_cnt = static_cast<ssize_t>(shape_cpp[1]);
        size_t reported_struct_size = elem_size_bytes;

        size_t expected_struct_size = 0;
        if (dtype == DataType::BBOX) expected_struct_size = sizeof(DeviceBoundingBox_t);
        else if (dtype == DataType::FACE) expected_struct_size = sizeof(DeviceFace_t);
        else if (dtype == DataType::POSE) expected_struct_size = sizeof(DevicePose_t);

        if (reported_struct_size == 0 && expected_struct_size > 0) {
            reported_struct_size = expected_struct_size; // fallback
        }
        if (expected_struct_size && reported_struct_size != expected_struct_size) {
            std::cerr << "Warning(convertToPyArray): elem_size(" << reported_struct_size
                      << ") differs from expected struct sizeof(" << expected_struct_size
                      << ") for tensor '" << cpp_tensor->name() << "'. Using reported size." << std::endl;
        }

        py_itemsize = 1; // Viewing as raw bytes
        py_format = py::format_descriptor<uint8_t>::format();
        py_final_shape = { batch_sz, item_cnt, static_cast<ssize_t>(reported_struct_size) };
        py_final_strides = py::detail::c_strides(py_final_shape, py_itemsize);
    } else {
        py_itemsize = elem_size_bytes;
        py_format = pyGetFormatDescriptor(dtype);
        py_final_strides = py::detail::c_strides(py_final_shape, py_itemsize);
    }

    bool readonly_flag = true; // Expose as read-only view
    py::buffer_info info(
        data_ptr,
        py_itemsize,
        py_format,
        py_final_shape.size(),
        py_final_shape,
        py_final_strides
    );
    info.readonly = readonly_flag;

    py::array arr_to_add;
    if (!base_python_array_for_view.is_none()) {
        arr_to_add = py::array(info, base_python_array_for_view);
    } else {
        auto* owned_shared_ptr_for_capsule = new TensorPtr(cpp_tensor);
        py::capsule lifetime_keeper_capsule(owned_shared_ptr_for_capsule, [](void* p){
            delete static_cast<TensorPtr*>(p);
        });
        arr_to_add = py::array(info, lifetime_keeper_capsule);
    }
    py_array_list.push_back(std::move(arr_to_add));
}


// Synchronous inference for single or batch - simplified version.
// Python side has already analyzed input format and standardized the data.
py::object pyRun(InferenceEngine &ie,
                 const py::object &py_inputs,
                 const py::object &py_output_buffers,
                 const py::object &py_user_args) {

    py::gil_scoped_acquire gil;

    // Python side has already standardized the input format
    // py_inputs is either:
    // - List[np.ndarray] for single inference
    // - List[List[np.ndarray]] for batch inference

    // Declare variables at function scope
    bool is_batch = false;
    std::vector<void*> cpp_batch_input_ptrs;
    void* cpp_single_input_ptr = nullptr;
    std::vector<void*> cpp_batch_output_ptrs;
    void* cpp_single_output_ptr = nullptr;
    std::vector<UserArgWrapper*> batch_user_arg_wrappers;
    UserArgWrapper* single_user_arg_wrapper = nullptr;
    std::vector<void*> user_args_raw_ptrs_for_batch_call;

    if (!py_inputs.is_none() && py::isinstance<py::list>(py_inputs)) {
        py::list inputs_list = py_inputs.cast<py::list>();
        if (inputs_list.empty()) {
            throw py::value_error("Input data list cannot be empty.");
        }

        // Determine if batch by checking first element type
        is_batch = py::isinstance<py::list>(inputs_list[0]);

        if (is_batch) {
            // Batch processing: List[List[np.ndarray]]
            py::list batch_inputs_py = inputs_list.cast<py::list>();
            for (const auto& batch_item_py_handle : batch_inputs_py) {
                py::list item_tensors_py = batch_item_py_handle.cast<py::list>();
                if (item_tensors_py.empty()) {
                    cpp_batch_input_ptrs.push_back(nullptr);
                } else {
                    cpp_batch_input_ptrs.push_back(item_tensors_py[0].cast<py::array>().request().ptr);
                }
            }

            // Process output buffers for batch
            if (!py_output_buffers.is_none() && py::isinstance<py::list>(py_output_buffers)) {
                py::list batch_outputs_py = py_output_buffers.cast<py::list>();
                if (py::len(batch_outputs_py) != cpp_batch_input_ptrs.size()) {
                    throw py::value_error("Output buffer batch size mismatch input batch size.");
                }
                for (const auto& batch_item_outputs_py_handle : batch_outputs_py) {
                    py::list item_output_tensors_py = batch_item_outputs_py_handle.cast<py::list>();
                    if (item_output_tensors_py.empty()) {
                        cpp_batch_output_ptrs.push_back(nullptr);
                    } else {
                        cpp_batch_output_ptrs.push_back(item_output_tensors_py[0].cast<py::array>().request().ptr);
                    }
                }
            } else {
                throw py::value_error("output_buffers must be provided for batch sync inference.");
            }

            // Process user args for batch
            if (!py_user_args.is_none()) {
                py::list user_args_list_py = py_user_args.cast<py::list>();
                if (py::len(user_args_list_py) != cpp_batch_input_ptrs.size()) {
                    throw py::value_error("User_args batch size mismatch input batch size.");
                }
                for (const auto& arg_py_handle : user_args_list_py) {
                    UserArgWrapper* wrapper = new UserArgWrapper(arg_py_handle.cast<py::object>());
                    batch_user_arg_wrappers.push_back(wrapper);
                    user_args_raw_ptrs_for_batch_call.push_back(reinterpret_cast<void*>(wrapper));
                }
            } else {
                for (size_t i = 0; i < cpp_batch_input_ptrs.size(); ++i) {
                    user_args_raw_ptrs_for_batch_call.push_back(nullptr);
                }
            }
        } else {
            // Single processing: List[np.ndarray]
            py::list inputs_list_py = inputs_list.cast<py::list>();
            if (inputs_list_py.empty()) {
                throw py::value_error("Input tensor list cannot be empty for single inference.");
            }
            cpp_single_input_ptr = inputs_list_py[0].cast<py::array>().request().ptr;

            // Process output buffer for single
            if (!py_output_buffers.is_none() && py::isinstance<py::list>(py_output_buffers)) {
                py::list output_list_py = py_output_buffers.cast<py::list>();
                if (!output_list_py.empty()) {
                    cpp_single_output_ptr = output_list_py[0].cast<py::array>().request().ptr;
                }
            }

            // Process user arg for single
            if (!py_user_args.is_none()) {
                single_user_arg_wrapper = new UserArgWrapper(py_user_args.cast<py::object>());
            }
        }
    } else {
        throw py::type_error("Input data must be a list.");
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
        // Re-throw the exception to be handled by the translator
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

    // Determine if we have multiple input tensors (multi-input case)
    bool multi_input = inputs_py.size() > 1;

    if (multi_input) {
        // Build C++ vector<void*> for all input tensors
        std::vector<void*> input_ptr_vec;
        input_ptr_vec.reserve(inputs_py.size());
        for (const auto& arr_py : inputs_py) {
            input_ptr_vec.push_back(arr_py.request().ptr);
        }

        // For multi-input we still only pass one output pointer (first element / scalar) if provided.
        return ie.RunAsync(input_ptr_vec, reinterpret_cast<void*>(wrapper), output_c_ptr_for_engine);
    }

    // ----- single-input path remains unchanged -----
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

// Runs multi-input inference synchronously with dictionary format
std::vector<py::array> pyRunMultiInputDict(InferenceEngine &ie, const py::dict &input_tensors_dict,
                                           const py::object &userArg_py = py::none(),
                                           const py::object &outputArg_py = py::none()) {
    py::gil_scoped_acquire gil_for_args;

    // Convert Python dict to C++ map
    std::map<std::string, void*> inputTensors;
    for (auto item : input_tensors_dict) {
        std::string key = item.first.cast<std::string>();
        py::array value = item.second.cast<py::array>();
        inputTensors[key] = value.request().ptr;
    }

    void* output_c_ptr_for_engine = nullptr;
    if (!outputArg_py.is_none()) {
        if (py::isinstance<py::array>(outputArg_py)) {
            output_c_ptr_for_engine = outputArg_py.cast<py::array>().request().ptr;
        } else if (py::isinstance<py::list>(outputArg_py)) {
            py::list output_list = outputArg_py.cast<py::list>();
            if (!output_list.empty() && py::isinstance<py::array>(output_list[0])) {
                output_c_ptr_for_engine = output_list[0].cast<py::array>().request().ptr;
            }
        }
    }

    UserArgWrapper* wrapper = nullptr;
    if (!userArg_py.is_none()) {
        wrapper = new UserArgWrapper(userArg_py);
    }

    gil_for_args.disarm();
    py::gil_scoped_release release_gil_for_c_call;

    TensorPtrs outputs_cpp;
    try {
        outputs_cpp = ie.RunMultiInput(inputTensors, reinterpret_cast<void*>(wrapper), output_c_ptr_for_engine);
    } catch (const std::exception& e) {
        py::gil_scoped_acquire acquire_gil_for_cleanup;
        if (wrapper) delete wrapper;
        // Re-throw the exception to be handled by the translator
        throw;
    }

    py::gil_scoped_acquire acquire_gil_for_cleanup;
    if (wrapper) delete wrapper;

    py::gil_scoped_acquire acquire_gil_for_results;
    std::vector<py::array> result_py_list;
    for(const auto &output_tensor_cpp_ptr : outputs_cpp) {
         convertToPyArray(output_tensor_cpp_ptr, py::none(), result_py_list);
    }
    return result_py_list;
}

// Runs multi-input inference asynchronously with dictionary format
int pyRunAsyncMultiInputDict(InferenceEngine &ie, const py::dict &input_tensors_dict,
                            const py::object &userArg_py = py::none(),
                            const py::object &outputArg_py = py::none()) {
    py::gil_scoped_acquire gil_for_args;

    // Convert Python dict to C++ map
    std::map<std::string, void*> inputTensors;
    for (auto item : input_tensors_dict) {
        std::string key = item.first.cast<std::string>();
        py::array value = item.second.cast<py::array>();
        inputTensors[key] = value.request().ptr;
    }

    void* output_c_ptr_for_engine = nullptr;
    py::object output_base_obj_for_callback = py::none();

    if (!outputArg_py.is_none()) {
        if (py::isinstance<py::array>(outputArg_py)) {
            output_c_ptr_for_engine = outputArg_py.cast<py::array>().request().ptr;
            output_base_obj_for_callback = outputArg_py;
        } else if (py::isinstance<py::list>(outputArg_py)) {
            py::list output_list = outputArg_py.cast<py::list>();
            if (!output_list.empty() && py::isinstance<py::array>(output_list[0])) {
                output_c_ptr_for_engine = output_list[0].cast<py::array>().request().ptr;
                output_base_obj_for_callback = output_list;
            }
        }
    }

    UserArgWrapper* wrapper = new UserArgWrapper(userArg_py, output_base_obj_for_callback);

    gil_for_args.disarm();
    py::gil_scoped_release release_gil_for_c_call;

    return ie.RunAsyncMultiInput(inputTensors, reinterpret_cast<void*>(wrapper), output_c_ptr_for_engine);
}

// Validates NPU device with multi-input dictionary format
std::vector<py::array> pyValidateDeviceMultiInputDict(InferenceEngine &ie, const py::dict &input_tensors_dict, int deviceId) {
    py::gil_scoped_acquire gil_for_args;

    // Convert Python dict to C++ map
    std::map<std::string, void*> inputTensors;
    for (auto item : input_tensors_dict) {
        std::string key = item.first.cast<std::string>();
        py::array value = item.second.cast<py::array>();
        inputTensors[key] = value.request().ptr;
    }

    gil_for_args.disarm();
    py::gil_scoped_release release_gil_for_c_call;

    TensorPtrs outputs_cpp = ie.ValidateDeviceMultiInput(inputTensors, deviceId);

    py::gil_scoped_acquire acquire_gil_for_results;
    std::vector<py::array> result_py_list;
    for(const auto &output_tensor_cpp_ptr : outputs_cpp) {
        convertToPyArray(output_tensor_cpp_ptr, py::none(), result_py_list);
    }
    return result_py_list;
}


// Configuration
void pyConfiguration_SetEnable(dxrt::Configuration &conf, int item, bool enabled);
int pyConfiguration_GetEnable(dxrt::Configuration &conf, int item);
void pyConfiguration_SetAttribute(dxrt::Configuration &conf, int item, int attrib, std::string value);
std::string pyConfiguration_GetAttribute(dxrt::Configuration &conf, int item, int attrib);
std::string pyConfiguration_GetVersion(dxrt::Configuration &conf);
std::string pyConfiguration_GetDriverVersion(dxrt::Configuration &conf);
std::string pyConfiguration_GetPCIeDriverVersion(dxrt::Configuration &conf);
void pyConfiguration_LoadConfigFile(dxrt::Configuration &conf, const std::string &fileName);
void pyConfiguration_SetFWConfigWithJson(dxrt::Configuration &conf, std::string json_file);

// DeviceStatus
int pyDeviceStatus_GetTemperature(DeviceStatus &deviceStatus, int ch);
int pyDeviceStatus_GetId(DeviceStatus &deviceStatus);
int pyDeviceStatus_GetNpuVoltage(DeviceStatus &deviceStatus, int ch);
int pyDeviceStatus_GetNpuClock(DeviceStatus &deviceStatus, int ch);

// RuntimeEventDispatcher
void pyRuntimeEventDispatcher_DispatchEvent(dxrt::RuntimeEventDispatcher &dispatcher, int level, int type, int code, const std::string &eventMessage);
void pyRuntimeEventDispatcher_SetCurrentLevel(dxrt::RuntimeEventDispatcher &dispatcher, int level);
int pyRuntimeEventDispatcher_GetCurrentLevel(dxrt::RuntimeEventDispatcher &dispatcher);


// Module definition
PYBIND11_MODULE(_pydxrt, m) {
    m.doc() = "Python bindings for DXRT Inference Engine";

    // Register exception translators
    py::register_exception_translator([](std::exception_ptr p) {
        translateException(p);
    });

    // RuntimeEventDispatcher class binding
    
    py::class_<dxrt::RuntimeEventDispatcher>(m, "RuntimeEventDispatcher")
        // Binds the static GetInstance() method to a Python static method `get_instance()`.
        .def_static("get_instance", &dxrt::RuntimeEventDispatcher::GetInstance, py::return_value_policy::reference)
        ; // End of class binding

        
    m.def("runtime_event_dispatcher_dispatch_event", &pyRuntimeEventDispatcher_DispatchEvent,
        py::arg("runtime_event_dispatcher"), py::arg("level"), py::arg("type"), py::arg("code"), py::arg("event_message"),
        "Dispatches a runtime event with specified parameters.");
    
    m.def("runtime_event_dispatcher_register_event_handler", 
        [](dxrt::RuntimeEventDispatcher &dispatcher, py::function eventHandler) {
            if (eventHandler.is_none()) {
                dispatcher.RegisterEventHandler(nullptr);
                return;
            }
            
            py::object captured_handler = eventHandler;
            dispatcher.RegisterEventHandler(
                [captured_handler](dxrt::RuntimeEventDispatcher::LEVEL level,
                                 dxrt::RuntimeEventDispatcher::TYPE type,
                                 dxrt::RuntimeEventDispatcher::CODE code,
                                 const std::string& message,
                                 const std::string& timestamp) {
                    py::gil_scoped_acquire gil;
                    try {
                        captured_handler(static_cast<int>(level),
                                       static_cast<int>(type),
                                       static_cast<int>(code),
                                       message,
                                       timestamp);
                    } catch (const py::error_already_set &e) {
                        std::cerr << "Python error in event handler: " << e.what() << std::endl;
                    }
                });
        },
        py::arg("runtime_event_dispatcher"), py::arg("event_handler"),
        "Registers a Python event handler callback for runtime events.");

    // .def("register_callback", [](InferenceEngine &ie, const py::object &pyCallback_obj) {
    //         pyRegisterCallback(ie, pyCallback_obj);
    //     })

    m.def("runtime_event_dispatcher_set_current_level", &pyRuntimeEventDispatcher_SetCurrentLevel,
        py::arg("runtime_event_dispatcher"), py::arg("level"),
        "Sets the current event level for the dispatcher.");

    m.def("runtime_event_dispatcher_get_current_level", &pyRuntimeEventDispatcher_GetCurrentLevel,
        py::arg("runtime_event_dispatcher"),
        "Gets the current event level of the dispatcher.");


    // Configuration class binding
    py::class_<dxrt::Configuration>(m, "Configuration")
        // Binds the static GetInstance() method to a Python static method `get_instance()`.
        .def_static("get_instance", &dxrt::Configuration::GetInstance, py::return_value_policy::reference)
        ; // End of class binding

    m.def("configuration_set_enable", &pyConfiguration_SetEnable,
        py::arg("configuration"), py::arg("item"), py::arg("enabled"),
        "Sets the enabled status for a specific configuration item.");

    m.def("configuration_get_enable", &pyConfiguration_GetEnable,
        py::arg("configuration"), py::arg("item"),
        "Retrieves the enabled status of a specific configuration item.");

    m.def("configuration_set_attribute", &pyConfiguration_SetAttribute,
        py::arg("configuration"), py::arg("item"), py::arg("attrib"), py::arg("value"),
        "Sets a specific attribute value for a given configuration item.");

    m.def("configuration_get_attribute", &pyConfiguration_GetAttribute,
        py::arg("configuration"), py::arg("item"), py::arg("attrib"),
        "Retrieves the value of a specific attribute for a given configuration item.");

    m.def("configuration_get_version", &pyConfiguration_GetVersion,
        py::arg("configuration"),
        "Retrieves the framework version.");

    m.def("configuration_get_driver_version", &pyConfiguration_GetDriverVersion,
        py::arg("configuration"),
        "Retrieves the device driver version.");

    m.def("configuration_get_pcie_driver_version", &pyConfiguration_GetPCIeDriverVersion,
        py::arg("configuration"),
        "Retrieves the PCIe driver version.");

    m.def("configuration_load_config_file", &pyConfiguration_LoadConfigFile,
        py::arg("configuration"), py::arg("file_name"),
        "Loads configuration settings from a file.");

    m.def("configuration_set_fw_config_with_json", &pyConfiguration_SetFWConfigWithJson,
        py::arg("configuration"), py::arg("json_file"),
        "Sets the firmware configuration using a JSON file.");

    // DeviceStatus class binding
    py::class_<DeviceStatus>(m, "DeviceStatus")
        // Binds the static GetCurrentStatus() method to a Python static method `get_current_status()`.
        .def_static("get_current_status", py::overload_cast<int>(&DeviceStatus::GetCurrentStatus))
        .def_static("get_device_count", &DeviceStatus::GetDeviceCount)
        ; // End of class binding

    m.def("device_status_get_temperature", &pyDeviceStatus_GetTemperature,
        py::arg("device_status"), py::arg("ch"),
        "Retrieves the temperature of the specified NPU channel.");

    m.def("device_status_get_id", &pyDeviceStatus_GetId,
        py::arg("device_status"),
        "Retrieves the unique identifier of the device.");

    m.def("device_status_get_npu_voltage", &pyDeviceStatus_GetNpuVoltage,
        py::arg("device_status"), py::arg("ch"),
        "Retrieves the voltage level of the specified NPU channel.");

    m.def("device_status_get_npu_clock", &pyDeviceStatus_GetNpuClock,
        py::arg("device_status"), py::arg("ch"),
        "Retrieves the clock frequency of the specified NPU channel.");


    // InferenceOption class binding
    py::class_<InferenceOption>(m, "InferenceOption")
        .def(py::init<>())
        .def_readwrite("useORT", &InferenceOption::useORT)
        .def_readwrite("bufferCount", &InferenceOption::bufferCount)
        .def_readwrite("boundOption", &InferenceOption::boundOption)
        .def_property("devices",
            [](const InferenceOption &opt) { return py::cast(opt.devices); }, // Getter
            [](InferenceOption &opt, const std::vector<int> &new_devices) { opt.devices = new_devices; } // Setter
        );

    // Runtime support query for ORT (reflects compile-time flag USE_ORT)
    m.def("is_ort_supported", []() -> bool {
#ifdef USE_ORT
        return true;
#else
        return false;
#endif
    }, "Returns True if this build supports ONNX Runtime (USE_ORT), otherwise False.");

    // InferenceEngine class binding (member functions are bound directly)
    py::class_<InferenceEngine>(m, "InferenceEngine")
        .def(py::init<const std::string&, InferenceOption&>(),
             py::arg("model_path"),
             py::arg("inference_option")) // Python __init__ in InferenceEngine.py handles providing default
        .def(py::init([](const py::array& model_array, InferenceOption& opt) {
            // Get buffer info from numpy array
            py::buffer_info buf = model_array.request();
            return new InferenceEngine(
                reinterpret_cast<const std::uint8_t*>(buf.ptr),
                static_cast<size_t>(buf.size * buf.itemsize),
                opt
            );
        }),
             py::arg("model_array"),
             py::arg("inference_option"),
             "Creates InferenceEngine from model data as numpy array")
        .def("get_input_size", &InferenceEngine::GetInputSize)
        .def("get_input_tensor_sizes", &InferenceEngine::GetInputTensorSizes)
        .def("get_output_size", &InferenceEngine::GetOutputSize)
        .def("get_output_tensor_sizes", &InferenceEngine::GetOutputTensorSizes)
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
        .def("get_model_version", &InferenceEngine::GetModelVersion)
        .def("is_ppu", &InferenceEngine::IsPPU)
        .def("has_dynamic_output", &InferenceEngine::HasDynamicOutput)

        .def("is_multi_input_model", &InferenceEngine::IsMultiInputModel)
        .def("get_input_tensor_count", &InferenceEngine::GetInputTensorCount)
        .def("get_input_tensor_names", &InferenceEngine::GetInputTensorNames)
        .def("get_output_tensor_names", &InferenceEngine::GetOutputTensorNames)
        .def("get_input_tensor_to_task_mapping", &InferenceEngine::GetInputTensorToTaskMapping)
        .def("get_task_order", &InferenceEngine::GetTaskOrder)
        .def("run_async", [](InferenceEngine &ie, const std::vector<py::array> &inputs_py, const py::object &userArg_py, const py::object &outputArg_py) {
            return pyRunAsync(ie, inputs_py, userArg_py, outputArg_py);
        }, py::arg("inputs"), py::arg("user_arg") = py::none(), py::arg("output_arg") = py::none())
        .def("register_callback", [](InferenceEngine &ie, const py::object &pyCallback_obj) {
            pyRegisterCallback(ie, pyCallback_obj);
        })
        .def("run_benchmark", [](InferenceEngine &ie, int num_loops, const py::object &inputs_py_obj) {
            return pyRunBenchmark(ie, num_loops, inputs_py_obj);
        }, py::arg("num_loops"), py::arg("inputs") = py::none())
        .def("validate_device", [](InferenceEngine &ie, const std::vector<py::array> &inputs_py, int deviceId) {
            return pyValidateDevice(ie, inputs_py, deviceId);
        }, py::arg("inputs"), py::arg("device_id") = 0)
        .def("wait", [](InferenceEngine &ie, int jobId) {
            return pyWait(ie, jobId);
        })
        .def("get_inputs_info", [](InferenceEngine &ie, py::object ptr_py_obj, uint64_t phyAddr) {
            return pyGetInputsInfo(ie, ptr_py_obj, phyAddr);
        }, py::arg("ptr") = py::none(), py::arg("phy_addr") = 0)
        .def("get_outputs_info", [](InferenceEngine &ie, py::object ptr_py_obj, uint64_t phyAddr) {
            return pyGetOutputsInfo(ie, ptr_py_obj, phyAddr);
        }, py::arg("ptr") = py::none(), py::arg("phy_addr") = 0)
        .def("get_input_dtype", [](InferenceEngine &ie) {
            return pyGetInputDataType(ie);
        })
        .def("get_output_dtype", [](InferenceEngine &ie) {
            return pyGetOutputDataType(ie);
        })
        .def("get_all_task_outputs", [](InferenceEngine &ie) {
            return pyGetAllTaskOutputs(ie);
        })
        .def("dispose", &InferenceEngine::Dispose);

    // Module-level functions that operate on InferenceEngine instances
    m.def("run", &pyRun, py::arg("engine"), py::arg("inputs"),
          py::arg("output_buffers") = py::none(), py::arg("user_args") = py::none(),
          "Runs inference synchronously. Handles single or batch. GIL released during C++ call.");

    m.def("get_inputs_info_dev", &pyGetInputsInfoDev, py::arg("engine"), py::arg("dev_id"),
           "Get input tensor(s) information for a specific NPU device.");

    // Assuming ParseModel is a free function: `std::string dxrt::ParseModel(const std::string&)`
    m.def("parse_model", static_cast<int(*)(std::string)>(&ParseModel), py::arg("model_path"),
          "Parses a model file and returns its string representation or info.");

    // Parse model with options
    m.def("parse_model", [](const std::string& model_path, py::dict options) {
        ParseOptions parse_opts;

        // Extract options from Python dict
        if (options.contains("verbose"))
            parse_opts.verbose = options["verbose"].cast<bool>();
        if (options.contains("json_extract"))
            parse_opts.json_extract = options["json_extract"].cast<bool>();
        if (options.contains("no_color"))
            parse_opts.no_color = options["no_color"].cast<bool>();
        if (options.contains("output_file"))
            parse_opts.output_file = options["output_file"].cast<std::string>();

        int result = ParseModel(model_path, parse_opts);
        return result;
    }, py::arg("model_path"), py::arg("options"),
    "Parses a model file with options (verbose, json_extract, no_color, output_file).");    // Multi-input specific functions for dictionary input format
    m.def("run_multi_input_dict", &pyRunMultiInputDict, py::arg("engine"), py::arg("input_tensors_dict"),
          py::arg("user_arg") = py::none(), py::arg("output_arg") = py::none(),
          "Runs multi-input inference synchronously with dictionary format. GIL released during C++ call.");

    m.def("run_async_multi_input_dict", &pyRunAsyncMultiInputDict, py::arg("engine"), py::arg("input_tensors_dict"),
          py::arg("user_arg") = py::none(), py::arg("output_arg") = py::none(),
          "Runs multi-input inference asynchronously with dictionary format. GIL released during C++ call.");

    m.def("validate_device_multi_input_dict", &pyValidateDeviceMultiInputDict, py::arg("engine"), py::arg("input_tensors_dict"),
          py::arg("device_id") = 0,
          "Validates NPU device with multi-input dictionary format. GIL released during C++ call.");

}

} // namespace dxrt
