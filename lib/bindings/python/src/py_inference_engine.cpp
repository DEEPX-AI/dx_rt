#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>
// #include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>

#include "dxrt/dxrt_api.h"

#ifdef __linux__
#else
typedef SSIZE_T ssize_t;
#endif


using namespace std;

namespace dxrt
{
namespace py = pybind11;

struct UserArgWrapper {
    py::object pyObj;
    explicit UserArgWrapper(py::object obj) : pyObj(std::move(obj)) {}
};

#ifdef __linux__
string pyFormatDescriptorTable[] = {
    [DataType::NONE_TYPE] = py::format_descriptor<uint8_t>::format(),
    [DataType::UINT8] = py::format_descriptor<uint8_t>::format(),
    [DataType::UINT16] = py::format_descriptor<uint16_t>::format(),
    [DataType::UINT32] = py::format_descriptor<uint32_t>::format(),
    [DataType::UINT64] = py::format_descriptor<uint64_t>::format(),
    [DataType::INT8] = py::format_descriptor<int8_t>::format(),
    [DataType::INT16] = py::format_descriptor<int16_t>::format(),
    [DataType::INT32] = py::format_descriptor<int32_t>::format(),
    [DataType::INT64] = py::format_descriptor<int64_t>::format(),
    [DataType::FLOAT] = py::format_descriptor<float>::format(),
    [DataType::BBOX] = py::format_descriptor<int8_t>::format(),
    [DataType::FACE] = py::format_descriptor<int8_t>::format(),
    [DataType::POSE] = py::format_descriptor<int8_t>::format(),
};
#elif _WIN32
string pyFormatDescriptorTable[DataType::MAX_TYPE];

void initializePyFormatDescriptorTable() {
    pyFormatDescriptorTable[DataType::NONE_TYPE] = py::format_descriptor<uint8_t>::format();
    pyFormatDescriptorTable[DataType::UINT8] = py::format_descriptor<uint8_t>::format();
    pyFormatDescriptorTable[DataType::UINT16] = py::format_descriptor<uint16_t>::format();
    pyFormatDescriptorTable[DataType::UINT32] = py::format_descriptor<uint32_t>::format();
    pyFormatDescriptorTable[DataType::UINT64] = py::format_descriptor<uint64_t>::format();
    pyFormatDescriptorTable[DataType::INT8] = py::format_descriptor<int8_t>::format();
    pyFormatDescriptorTable[DataType::INT16] = py::format_descriptor<int16_t>::format();
    pyFormatDescriptorTable[DataType::INT32] = py::format_descriptor<int32_t>::format();
    pyFormatDescriptorTable[DataType::INT64] = py::format_descriptor<int64_t>::format();
    pyFormatDescriptorTable[DataType::FLOAT] = py::format_descriptor<float>::format();
    pyFormatDescriptorTable[DataType::BBOX] = py::format_descriptor<int8_t>::format();
    pyFormatDescriptorTable[DataType::FACE] = py::format_descriptor<int8_t>::format();
    pyFormatDescriptorTable[DataType::POSE] = py::format_descriptor<int8_t>::format();
}

static struct InitializePyFormatDescriptorTable {
    InitializePyFormatDescriptorTable() {
        initializePyFormatDescriptorTable();
    }
} initPyFormatDescriptorTable;

#endif 
string pyGetFormatDescriptor(DataType dtype)
{
    return pyFormatDescriptorTable[dtype];
}

void convertToPyArray(const TensorPtr& output, DataType dtype, std::vector<py::array>& result) {
    if (dtype == DataType::BBOX || dtype == DataType::FACE || dtype == DataType::POSE) {
        std::vector<int64_t> shape;
        std::vector<ssize_t> strides;
        int outputCnt = output->shape()[0];
        
        if (outputCnt == 0) {
            shape = {0};
            strides = {1};
        } else {
            switch(dtype) {
                case DataType::BBOX:
                    shape = {outputCnt, static_cast<int64_t>(sizeof(DeviceBoundingBox_t))};
                    break;
                case DataType::FACE:
                    shape = {outputCnt, static_cast<int64_t>(sizeof(DeviceFace_t))};
                    break;
                case DataType::POSE:
                    shape = {outputCnt, static_cast<int64_t>(sizeof(DevicePose_t))};
                    break;
                default:
                    throw std::runtime_error("Unexpected dtype in device output conversion");
            }
            strides = {shape[1], 1};
        }

        result.emplace_back(
            py::array(
                py::buffer_info(
                    output->data(),
                    1,  // itemsize
                    pyGetFormatDescriptor(dtype),
                    shape.size(),
                    shape,
                    strides
                )
            )
        );
    }
    else {
        auto shape = output->shape();
        auto elemSize = output->elem_size();
        
        auto strides = py::detail::c_strides(shape, elemSize);
        
        result.emplace_back(
            py::array(
                py::buffer_info(
                    output->data(),
                    elemSize,
                    pyGetFormatDescriptor(dtype),
                    shape.size(),
                    shape,
                    strides
                )
            )
        );
    }
}

void convertToPyArrayWithCopy(const TensorPtr& output, DataType dtype, std::vector<py::array>& result) {
    //py::gil_scoped_acquire acquire_gil;
    if (dtype == DataType::BBOX || dtype == DataType::FACE || dtype == DataType::POSE) {
        std::vector<int64_t> shape;
        std::vector<ssize_t> strides;
        int outputCnt = output->shape()[0];
        size_t buffer_size = 0;
        
        if (outputCnt == 0) {
            shape = {0};
            strides = {1};
            buffer_size = 1;
        } else {
            switch(dtype) {
                case DataType::BBOX:
                    shape = {outputCnt, static_cast<int64_t>(sizeof(DeviceBoundingBox_t))};
                    buffer_size = outputCnt * sizeof(DeviceBoundingBox_t);
                    break;
                case DataType::FACE:
                    shape = {outputCnt, static_cast<int64_t>(sizeof(DeviceFace_t))};
                    buffer_size = outputCnt * sizeof(DeviceFace_t);
                    break;
                case DataType::POSE:
                    shape = {outputCnt, static_cast<int64_t>(sizeof(DevicePose_t))};
                    buffer_size = outputCnt * sizeof(DevicePose_t);
                    break;
                default:
                    throw std::runtime_error("Unexpected dtype in device output conversion");
            }
            strides = {shape[1], 1};
        }
        
        void* buffer_data = malloc(buffer_size);
        if (!buffer_data) {
            throw std::runtime_error("Failed to allocate memory for output conversion");
        }
        
        try {
            memcpy(buffer_data, output->data(), buffer_size);
            
            result.emplace_back(
                py::array(
                    py::buffer_info(
                        buffer_data,
                        1,
                        pyGetFormatDescriptor(dtype),
                        shape.size(),
                        shape,
                        strides
                    )
                )
            );
        }
        catch (...) {
            free(buffer_data);
            throw;
        }
    }
    else {
        auto shape = output->shape();
        auto elemSize = output->elem_size();
        
        size_t buffer_size = elemSize;
        for (const auto& dim : shape) {
            buffer_size *= dim;
        }
        
        void* buffer_data = malloc(buffer_size);
        if (!buffer_data) {
            throw std::runtime_error("Failed to allocate memory for output conversion");
        }
        
        try {
            memcpy(buffer_data, output->data(), buffer_size);
            
            // Create Python array using buffer protocol
            result.emplace_back(
                py::array(
                    py::buffer_info(
                        buffer_data,
                        elemSize,
                        pyGetFormatDescriptor(dtype),
                        shape.size(),
                        shape,
                        py::detail::c_strides(shape, elemSize)
                    )
                )
            );
        }
        catch (...) {
            free(buffer_data);
            throw;
        }
    }
}

vector<py::array> pyRunEngine(InferenceEngine &ie, const vector<py::array> &inputs)
{
    py::gil_scoped_acquire gil;
    vector<py::array> result;
    auto outputs = ie.Run(inputs.front().request().ptr);
    for(auto &output : outputs)
    {
        auto dtype = output->type();
        convertToPyArray(output, dtype, result);  
    }
    return result;
}


int pyRunAsyncEngine(InferenceEngine &ie, const vector<py::array> &inputs, py::object userArg) 
{
    py::gil_scoped_release release;

    if (inputs.empty()) {
        throw std::runtime_error("Input array is empty");
    }
    
    py::gil_scoped_acquire gil;
    auto wrapper = new UserArgWrapper(userArg);
    void* inputPtr = const_cast<void*>(inputs.front().request().ptr);
    
    {
        py::gil_scoped_release release;
        return ie.RunAsync(inputPtr, static_cast<void*>(wrapper), nullptr);
    }
    
}

void pyRegisterCallBack(InferenceEngine &ie, const py::function &pyCallback) {
    ie.RegisterCallBack([pyCallback](TensorPtrs &outputs, void *userArg) -> int {
        py::gil_scoped_acquire gil;
        try {
            std::vector<py::array> result;
            
            {
                for(auto &output : outputs) {
                    
                    auto dtype = output->type();
                    convertToPyArray(output, dtype, result);  
                }
            }
            UserArgWrapper* wrapper = static_cast<UserArgWrapper*>(userArg);
            pyCallback(result, wrapper->pyObj);
            
            return 0;
        }
        catch (const std::exception &e) {
            std::cerr << "Exception in callback: " << e.what() << std::endl;
            return -1;
        }
        catch (...) {
            std::cerr << "Unknown exception in callback" << std::endl;
            return -1;
        }
    });
}

vector<py::array> pyWait(InferenceEngine &ie, int reqId) {
    py::gil_scoped_release release;

    auto outputs = ie.Wait(reqId);
    try{
        py::gil_scoped_acquire gil;
        std::vector<py::array> result;
        for(auto &output : outputs)
        {
            auto dtype = output->type();
            convertToPyArray(output, dtype, result);  
        }
        return result;      
    }
    catch (const std::exception &e) {
        std::cerr << "Exception in callback: " << e.what() << std::endl;
        exit(1);
    }
    catch (...) {
        std::cerr << "Unknown exception in callback" << std::endl;
        exit(1);
    }

}

vector<py::array> pyValidateDevice(InferenceEngine &ie, const vector<py::array> &inputs, int deviceId)
{
    vector<py::array> result;
    auto outputs = ie.ValidateDevice(inputs.front().request().ptr, deviceId);
    for(auto &output:outputs)
    {
        auto shape = output->shape();
        result.emplace_back(
            py::array(
                py::buffer_info(
                    output->data(),
                    output->elem_size(),
                    pyGetFormatDescriptor(output->type()),
                    shape.size(),
                    shape,
                    py::detail::c_strides(shape, output->elem_size()) // TODO : Prebuild this vector in inference engine
                )
            )
        );
        // for(int i=0;i<10;i++) cout << *((float*)output->data() + i) << endl;
    }
    return result;
}
vector<string> pyGetInputDataType(InferenceEngine &ie)
{
    vector<string> ret;
    for(auto &tensor:ie.inputs())
    {
        ret.emplace_back( DataTypeToString( tensor.type() ) );
    }
    return ret;
}
vector<string> pyGetOutputDataType(InferenceEngine &ie)
{
    vector<string> ret;
    for(auto &tensor:ie.outputs())
    {
        ret.emplace_back( DataTypeToString( tensor.type() ) );
    }
    return ret;
}
vector<string> pyGetTaskOrder(InferenceEngine &ie)
{
    vector<string> ret;
    ret = ie.task_order();
    return ret;
}
vector<vector<py::array>> pyGetOutputs(InferenceEngine &ie)
{
    vector<vector<py::array>> results;
    auto outputsVector = ie.get_outputs();
    for(auto &outputs:outputsVector){
        vector<py::array> result;
        for(auto &output : outputs)
        {
            auto dtype = output->type();
            convertToPyArray(output, dtype, result);  
        }
        results.emplace_back(result);
    }
    return results;
}

PYBIND11_MODULE(_pydxrt, m) {
    py::class_<InferenceEngine>(m, "InferenceEngine")
        .def(py::init<string>())
        .def("benchmark", &InferenceEngine::RunBenchMark, py::arg("num"), py::arg("inputPtr") = nullptr)
        .def("input_size", &InferenceEngine::input_size)
        .def("output_size", &InferenceEngine::output_size)
        .def("latency", &InferenceEngine::latency)
        .def("inf_time", &InferenceEngine::inference_time)
        .def("bitmatch_mask", &InferenceEngine::bitmatch_mask)
        .def("get_num_tails", &InferenceEngine::get_num_tails)
        .def("get_compile_type", &InferenceEngine::get_compile_type)
        .def("register_callback", &pyRegisterCallBack)
        ;
    m.def("run_engine", &pyRunEngine);
    m.def("run_async_engine", &pyRunAsyncEngine);
    m.def("wait", &pyWait);
    m.def("validate_device", &pyValidateDevice);
    m.def("parse_model", &ParseModel);
    m.def("input_dtype", &pyGetInputDataType);
    m.def("output_dtype", &pyGetOutputDataType);
    m.def("task_order", &pyGetTaskOrder);
    m.def("get_outputs", &pyGetOutputs);
}

}/* namespace dxrt */