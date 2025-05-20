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


string pyFormatDescriptorTable[DataType::MAX_TYPE];

void initializePyFormatDescriptorTable() {
    pyFormatDescriptorTable[DataType::NONE_TYPE] = py::format_descriptor<uint8_t>::format();
    pyFormatDescriptorTable[DataType::FLOAT] = py::format_descriptor<float>::format();
    pyFormatDescriptorTable[DataType::UINT8] = py::format_descriptor<uint8_t>::format();
    pyFormatDescriptorTable[DataType::INT8] = py::format_descriptor<int8_t>::format();
    pyFormatDescriptorTable[DataType::UINT16] = py::format_descriptor<uint16_t>::format();
    pyFormatDescriptorTable[DataType::INT16] = py::format_descriptor<int16_t>::format();
    pyFormatDescriptorTable[DataType::INT32] = py::format_descriptor<int32_t>::format();
    pyFormatDescriptorTable[DataType::INT64] = py::format_descriptor<int64_t>::format();
    pyFormatDescriptorTable[DataType::UINT32] = py::format_descriptor<uint32_t>::format();
    pyFormatDescriptorTable[DataType::UINT64] = py::format_descriptor<uint64_t>::format();
    pyFormatDescriptorTable[DataType::BBOX] = py::format_descriptor<int8_t>::format();
    pyFormatDescriptorTable[DataType::FACE] = py::format_descriptor<int8_t>::format();
    pyFormatDescriptorTable[DataType::POSE] = py::format_descriptor<int8_t>::format();
}

static struct InitializePyFormatDescriptorTable {
    InitializePyFormatDescriptorTable() {
        initializePyFormatDescriptorTable();
    }
} initPyFormatDescriptorTable;


string pyGetFormatDescriptor(DataType dtype)
{
    return pyFormatDescriptorTable[dtype];
}

void convertToPyArray(const TensorPtr& output, DataType dtype, std::vector<py::array>& result) {
    if (dtype == DataType::BBOX || dtype == DataType::FACE || dtype == DataType::POSE) {
        std::vector<int64_t> shape;
        std::vector<ssize_t> strides;
        int batch_size = output->shape()[0];
        int output_cnt = output->shape()[1];

        if (output_cnt == 0) {
            shape = {0};
            strides = {1};
        } else {
            switch(dtype) {
                case DataType::BBOX:
                    shape = {batch_size, output_cnt, static_cast<int64_t>(sizeof(DeviceBoundingBox_t))};
                    break;
                case DataType::FACE:
                    shape = {batch_size, output_cnt, static_cast<int64_t>(sizeof(DeviceFace_t))};
                    break;
                case DataType::POSE:
                    shape = {batch_size, output_cnt, static_cast<int64_t>(sizeof(DevicePose_t))};
                    break;
                default:
                    throw std::runtime_error("Unexpected dtype in device output conversion");
            }
            strides = {shape[1]*shape[2], shape[2], 1};
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
        int batch_size = output->shape()[0];
        int output_cnt = output->shape()[1];
        size_t buffer_size = 0;
        
        if (output_cnt == 0) {
            shape = {0};
            strides = {1};
            buffer_size = 1;
        } else {
            switch(dtype) {
                case DataType::BBOX:
                    shape = {batch_size, output_cnt, static_cast<int64_t>(sizeof(DeviceBoundingBox_t))};
                    buffer_size = batch_size * output_cnt * sizeof(DeviceBoundingBox_t);
                    break;
                case DataType::FACE:
                    shape = {batch_size, output_cnt, static_cast<int64_t>(sizeof(DeviceFace_t))};
                    buffer_size = batch_size * output_cnt * sizeof(DeviceFace_t);
                    break;
                case DataType::POSE:
                    shape = {batch_size, output_cnt, static_cast<int64_t>(sizeof(DevicePose_t))};
                    buffer_size = batch_size * output_cnt * sizeof(DevicePose_t);
                    break;
                default:
                    throw std::runtime_error("Unexpected dtype in device output conversion");
            }
            strides = {shape[1]*shape[2], shape[2], 1};
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

vector<py::array> pyRun(InferenceEngine &ie, const vector<py::array> &inputs)
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

int pyRunAsync(InferenceEngine &ie, const vector<py::array> &inputs, py::object userArg) 
{
    py::gil_scoped_release release;

    if (inputs.empty()) {
        throw std::runtime_error("Input array is empty");
    }
    
    py::gil_scoped_acquire gil;
    auto wrapper = new UserArgWrapper(userArg);
    
    /*void* userArgPointer = nullptr;
    if ( !userArg.is_none() ) 
    {
        userArgPointer = reinterpret_cast<void*>(&userArg);
        userArg.inc_ref();
    }*/

    void* inputPtr = const_cast<void*>(inputs.front().request().ptr);
    
    {
        py::gil_scoped_release release;
        return ie.RunAsync(inputPtr, reinterpret_cast<void*>(wrapper), nullptr);
        //return ie.RunAsync(inputPtr, userArgPointer, nullptr);
    }
    
}

float pyRunBenchmark(InferenceEngine &ie, int num, const vector<py::array> &inputs)
{
    py::gil_scoped_acquire gil;
    float fps;
    if (inputs.empty()) 
    {
        throw std::runtime_error("Input array is empty");
    }
    else
    {
        fps = ie.RunBenchmark(num, inputs.front().request().ptr);
    }
    
    return fps;
}

void pyRegisterCallback(InferenceEngine &ie, const py::function &pyCallback) {

    ie.RegisterCallback([pyCallback](TensorPtrs &outputs, void *userArg) -> int {

        py::gil_scoped_acquire gil;

        try {

            std::vector<py::array> result;
            
            {
                for(auto &output : outputs) {
                    
                    auto dtype = output->type();
                    convertToPyArray(output, dtype, result);
                }
            }

            UserArgWrapper* wrapper = reinterpret_cast<UserArgWrapper*>(userArg);
            pyCallback(result, wrapper->pyObj);
            if ( wrapper != nullptr ) delete wrapper;
            
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

vector<vector<py::array>> pyRunBatch(InferenceEngine &ie, const vector<vector<py::array>> &inputBuffers, 
    vector<vector<py::array>> &outputBuffers, vector<py::object>& userArgs)
{

    std::ignore = ie;
    std::ignore = inputBuffers;
    std::ignore = outputBuffers;
    std::ignore = userArgs;

    std::cout << "pyRunBatch input_buffers size=" << inputBuffers.size() << std::endl;
    std::cout << "pyRunBatch output_buffers size=" << outputBuffers.size() << std::endl;
    std::cout << "pyRunBatch user_args size=" << userArgs.size() << std::endl;

    // input buffer: convert python list to vector
    std::vector<void*> vectorInputBuffers;
    for(auto& input : inputBuffers)
    {
        vectorInputBuffers.emplace_back(const_cast<void*>(input.front().request().ptr));
    }

    // output buffer: convert python list to vector
    std::vector<void*> vectorOutputBuffers;
    for(auto& output : outputBuffers)
    {
        vectorOutputBuffers.emplace_back(const_cast<void*>(output.front().request().ptr));
    }

    
    
    try {
        // create user arguments wrapper
        std::vector<void*> vectorUserArgs;
        // for(auto& arg : userArgs)
        // {
        //     std::cout << "argument=" << arg << std::endl;
        //     vectorUserArgs.emplace_back(new UserArgWrapper(arg));
        // }
        
        vector<dxrt::TensorPtrs> vectorOutputTensorPtrs = ie.Run(vectorInputBuffers, vectorOutputBuffers);

        // delete user arguments wrapper
        // for(auto& argPointer : vectorUserArgs)
        // {
        //     delete reinterpret_cast<UserArgWrapper*>(argPointer);
        //     argPointer = nullptr;
        // }

        vector<vector<py::array>> vectorResult(vectorOutputTensorPtrs.size());

        //py::gil_scoped_acquire gil;
        int index = 0;
        for(auto &outputs : vectorOutputTensorPtrs)
        {
            for(auto &output : outputs) {
                        
                auto dtype = output->type();
                convertToPyArray(output, dtype, vectorResult[index]);
            }
            index++;
        } // data conversion

        return vectorResult;

    }
    catch(const dxrt::Exception &e)
    {
        std::cerr << "DXRT Exception in run-batch: " << e.what() << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << "STD Exception in run-batch: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Exception in run-batch: " << std::endl;
    }

    return {};
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
    for(auto &tensor:ie.GetInputs())
    {
        ret.emplace_back( DataTypeToString( tensor.type() ) );
    }
    return ret;
}

vector<string> pyGetOutputDataType(InferenceEngine &ie)
{
    vector<string> ret;
    for(auto &tensor:ie.GetOutputs())
    {
        ret.emplace_back( DataTypeToString( tensor.type() ) );
    }
    return ret;
}

vector<string> pyGetTaskOrder(InferenceEngine &ie)
{
    vector<string> ret;
    ret = ie.GetTaskOrder();
    return ret;
}

vector<vector<py::array>> pyGetAllTaskOutputs(InferenceEngine &ie)
{
    vector<vector<py::array>> results;
    auto outputsVector = ie.GetAllTaskOutputs();
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

// InferenceEngine::Enter
void pyInferenceEngine_Enter(InferenceEngine &ie)
{
    std::ignore = ie;
}

// InferenceEngine::Exit
void pyInferenceEngine_Exit(InferenceEngine &ie)
{
    ie.Dispose();
}

// InferencOptoin::SetUseORT
void pyInferenceOption_SetUseORT(InferenceOption &option, bool useORT)
{
    option.useORT = useORT;
}

// InferencOptoin::GetUseORT
bool pyInferenceOption_GetUseORT(InferenceOption &option)
{
    return option.useORT;
}

// InferencOptoin::SetBoundOption
void pyInferenceOption_SetBoundOption(InferenceOption &option, int boundOption)
{
    option.boundOption = boundOption;
}

// InferencOptoin::GetBoundOption
int pyInferenceOption_GetBoundOption(InferenceOption &option)
{
    return option.boundOption;
}

// InferencOptoin::SetDevices
void pyInferenceOption_SetDevices(InferenceOption &option, py::array_t<int>& devices)
{
    py::buffer_info buf = devices.request();

    int* ptr = static_cast<int*>(buf.ptr);
    option.devices = std::vector<int>(ptr, ptr + buf.size);
}

// InferencOptoin::GetDevices
py::array_t<int> pyInferenceOption_GetDevices(InferenceOption &option)
{
    return py::array_t<int>(option.devices.size(), option.devices.data());
}

PYBIND11_MODULE(_pydxrt, m) {

    // InferenceOption
    py::class_<InferenceOption>(m, "InferenceOption")
        .def(py::init<>());

    m.def("inference_option_set_use_ort", &pyInferenceOption_SetUseORT);
    m.def("inference_option_get_use_ort", &pyInferenceOption_GetUseORT);
    m.def("inference_option_set_bound_option", &pyInferenceOption_SetBoundOption);
    m.def("inference_option_get_bound_option", &pyInferenceOption_GetBoundOption);
    m.def("inference_option_set_devices", &pyInferenceOption_SetDevices);
    m.def("inference_option_get_devices", &pyInferenceOption_GetDevices);


    py::class_<InferenceEngine>(m, "InferenceEngine")
        .def(py::init<string, InferenceOption&>())
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

        .def("get_bitmatch_mask", &InferenceEngine::GetBitmatchMask)
        .def("get_num_tail_tasks", &InferenceEngine::GetNumTailTasks)
        .def("get_compile_type", &InferenceEngine::GetCompileType)
        .def("is_ppu", &InferenceEngine::IsPPU)
        ;
    m.def("register_callback", &pyRegisterCallback);
    m.def("run", &pyRun);
    m.def("run_async", &pyRunAsync);
    m.def("run_batch", &pyRunBatch);
    m.def("run_benchmark", &pyRunBenchmark);
    m.def("wait", &pyWait);
    m.def("validate_device", &pyValidateDevice);
    m.def("get_input_dtype", &pyGetInputDataType);
    m.def("get_output_dtype", &pyGetOutputDataType);
    m.def("get_task_order", &pyGetTaskOrder);
    m.def("get_all_task_outputs", &pyGetAllTaskOutputs);
    m.def("parse_model", &ParseModel);
    m.def("inference_engine_enter", &pyInferenceEngine_Enter);
    m.def("inference_engine_exit", &pyInferenceEngine_Exit);
}

}/* namespace dxrt */