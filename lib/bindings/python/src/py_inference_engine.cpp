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

string pyFormatDescriptorTable[] = {
    [DataType::NONE_TYPE] = py::format_descriptor<uint8_t>::format(),
    [DataType::UINT8] = py::format_descriptor<uint8_t>::format(),
    [DataType::UINT16] = py::format_descriptor<uint16_t>::format(),
    [DataType::UINT32] = py::format_descriptor<uint32_t>::format(),
    [DataType::INT8] = py::format_descriptor<int8_t>::format(),
    [DataType::INT16] = py::format_descriptor<int16_t>::format(),
    [DataType::INT32] = py::format_descriptor<int32_t>::format(),
    [DataType::FLOAT] = py::format_descriptor<float>::format(),
    [DataType::BBOX] = py::format_descriptor<float>::format(),
    [DataType::FACE] = py::format_descriptor<float>::format(),
    [DataType::POSE] = py::format_descriptor<float>::format(),
};
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

PYBIND11_MODULE(_pydxrt, m) {
    py::class_<InferenceEngine>(m, "InferenceEngine")
        .def(py::init<string>())
        .def("benchmark", &InferenceEngine::RunBenchMark)
        .def("input_size", &InferenceEngine::input_size)
        .def("output_size", &InferenceEngine::output_size)
        ;
    m.def("run_engine", &pyRunEngine);
    m.def("parse_model", &ParseModel);
    m.def("input_dtype", &pyGetInputDataType);
    m.def("output_dtype", &pyGetOutputDataType);
}

}/* namespace dxrt */