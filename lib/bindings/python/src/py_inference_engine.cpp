#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>
// #include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>

#include "dxrt/dxrt_api.h"

using namespace std;

namespace dxrt
{
namespace py = pybind11;

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
vector<py::array> pyRunEngine(InferenceEngine &ie, const vector<py::array> &inputs)
{
    vector<py::array> result;
    auto outputs = ie.Run(inputs.front().request().ptr);
    // DataDumpBin("input.bin.rt", inputs.front().request().ptr, ie.input_size());
    // DataDumpBin("output.bin.rt", outputs.front()->GetData(), ie.output_size());
    for(auto &output : outputs)
    {
        auto dtype = output->type();
        if (dtype == DataType::BBOX || dtype == DataType::FACE || dtype == DataType::POSE) {
            std::vector<int64_t> shape;
            std::vector<ssize_t> strides;
            int outputCnt = outputs.front()->shape()[0];
            if (outputCnt == 0) {
                shape = {0};
                strides = {1}; 
            } else if (dtype == DataType::BBOX) {
                shape = {outputCnt, static_cast<int64_t>(sizeof(DeviceBoundingBox_t))};
                strides = {shape[1], 1};
            } else if (dtype == DataType::FACE) {
                shape = {outputCnt, static_cast<int64_t>(sizeof(DeviceFace_t))};
                strides = {shape[1], 1};
            } else if (dtype == DataType::POSE) {
                shape = {outputCnt, static_cast<int64_t>(sizeof(DevicePose_t))};
                strides = {shape[1], 1};
            }

            result.emplace_back(
                py::array(
                    py::buffer_info(
                        output->data(),
                        1,
                        pyGetFormatDescriptor(dtype),
                        shape.size(),
                        shape,
                        strides
                    )
                )
            );
        }
        else{
            auto shape = output->shape();
            result.emplace_back(
                py::array(
                    py::buffer_info(
                        output->data(),
                        output->elem_size(),
                        pyGetFormatDescriptor(dtype),
                        shape.size(),
                        shape,
                        py::detail::c_strides(shape, output->elem_size()) // TODO : Prebuild this vector in inference engine
                    )
                )
            );
        }
        // for(int i=0;i<10;i++) cout << *((float*)output->data() + i) << endl;
    }
    return result;
}

std::vector<std::vector<py::array>> pyBatchRunAsync(InferenceEngine &ie, const std::vector<py::array> &inputs, int repeat)
{
    std::vector<std::vector<py::array>> results(inputs.size() * repeat); // Preallocate result vector
    std::atomic<int> callBackCnt(0);
    std::mutex mtx;
    std::condition_variable cv;

    ie.RegisterCallBack([&](dxrt::TensorPtrs &outputs, void *arg) -> int {
        int index = *reinterpret_cast<int*>(arg);
        std::cout << "Callback for index: " << index << std::endl;

        std::vector<py::array> result; 
        for(auto &output : outputs)
        {
            auto dtype = output->type();
            if (dtype == DataType::BBOX || dtype == DataType::FACE || dtype == DataType::POSE) {
                std::vector<int64_t> shape;
                std::vector<ssize_t> strides;
                int outputCnt = outputs.front()->shape()[0];
                //cout << "outputCnt : "<< outputCnt<<endl;
                if (outputCnt == 0) {
                    shape = {0};
                    strides = {1}; 
                } else if (dtype == DataType::BBOX) {
                    shape = {outputCnt, static_cast<int64_t>(sizeof(DeviceBoundingBox_t))};
                    strides = {shape[1], 1};
                } else if (dtype == DataType::FACE) {
                    shape = {outputCnt, static_cast<int64_t>(sizeof(DeviceFace_t))};
                    strides = {shape[1], 1};
                } else if (dtype == DataType::POSE) {
                    shape = {outputCnt, static_cast<int64_t>(sizeof(DevicePose_t))};
                    strides = {shape[1], 1};
                }

                result.emplace_back(
                    py::array(
                        py::buffer_info(
                            output->data(),
                            1,
                            pyGetFormatDescriptor(dtype),
                            shape.size(),
                            shape,
                            strides
                        )
                    )
                );
            }
            else{
                auto shape = output->shape();
                result.emplace_back(
                    py::array(
                        py::buffer_info(
                            output->data(),
                            output->elem_size(),
                            pyGetFormatDescriptor(dtype),
                            shape.size(),
                            shape,
                            py::detail::c_strides(shape, output->elem_size()) // TODO : Prebuild this vector in inference engine
                        )
                    )
                );
            }
            // for(int i=0;i<10;i++) cout << *((float*)output->data() + i) << endl;
        }
        {
            std::lock_guard<std::mutex> lock(mtx);
            results[index] = std::move(result);
        }
        callBackCnt.fetch_add(1);
        cv.notify_one();
        delete reinterpret_cast<int*>(arg);
        return 0;
    });

    std::vector<int> requests;
    for (int i = 0; i < repeat; ++i)
    {
        for (size_t j = 0; j < inputs.size(); ++j)
        {
            int* index = new int(i * inputs.size() + j);
            requests.emplace_back(ie.RunAsync(inputs[j].request().ptr, index));
            std::cout << "Started request for index: " << *index << " j : "<< j << std::endl;
        }
    }

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return static_cast<uint32_t>(callBackCnt.load()) >= inputs.size() * repeat; });
    }

    std::cout << "All callbacks completed." << std::endl;

    return results;
}

vector<py::array> pyValidateDevice(InferenceEngine &ie, const vector<py::array> &inputs, int deviceId)
{
    vector<py::array> result;
    auto outputs = ie.ValidateDevice(inputs.front().request().ptr, deviceId);
    // DataDumpBin("input.bin.rt", inputs.front().request().ptr, ie.input_size());
    // DataDumpBin("output.bin.rt", outputs.front()->GetData(), ie.output_size());
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
    auto outputsVector = ie.GetOutputs();
    for(auto &outputs:outputsVector){
        vector<py::array> result;
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
        results.emplace_back(result);
    }
    return results;
}
PYBIND11_MODULE(_pydxrt, m) {
    py::class_<InferenceEngine>(m, "InferenceEngine")
        .def(py::init<string>())
        .def("benchmark", &InferenceEngine::RunBenchMark)
        .def("input_size", &InferenceEngine::input_size)
        .def("output_size", &InferenceEngine::output_size)
        .def("task_order", &InferenceEngine::task_order)
        .def("latency", &InferenceEngine::latency)
        .def("inf_time", &InferenceEngine::inference_time)
        .def("bitmatch_mask", &InferenceEngine::bitmatch_mask)
        ;
    m.def("run_engine", &pyRunEngine);
    m.def("batch_run_async", &pyBatchRunAsync);
    m.def("validate_device", &pyValidateDevice);
    m.def("parse_model", &ParseModel);
    m.def("input_dtype", &pyGetInputDataType);
    m.def("output_dtype", &pyGetOutputDataType);
    m.def("task_order", &pyGetTaskOrder);
    m.def("get_outputs", &pyGetOutputs);
}

}/* namespace dxrt */