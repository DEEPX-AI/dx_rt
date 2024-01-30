#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>

#include "dxrt/common.h"
#include "dxrt/model.h"
// #include "dxrt/inference_option.h"
#include "dxrt/tensor.h"
#include "dxrt/inference_option.h"
#include "dxrt/testdata.h"

#define NPU_PARAM_FILE "rmap.info"

namespace dxrt {
using rmapinfo = deepx_rmapinfo::RegisterInfoDatabase;
struct InferenceOption;
class Task;
class InferenceEngine
{
public:
    InferenceEngine(const std::string &, InferenceOption &option=DefaultInferenceOption);
    ~InferenceEngine(void);
    TensorPtrs Run(void *inputPtr, void *userArg=nullptr, void *outputPtr=nullptr);
    // void Run(Tensors &inputs, Tensors &outputs); /* TODO */
    int RunAsync(void *inputPtr, void *userArg=nullptr, void *outputPtr=nullptr);
    float RunBenchMark(int num);
    /** \brief Find test data (inputs and reference outputs), and perform data integrity test
     * \param loops number of inferences to check data integrity
     * \param dataPool test data pool
     * \return return data integrity test results as pair of pass & fail numbers
    */
    std::pair<int, int> RunDataIntegrityCheck(int loops, std::vector<TestData> &dataPool);
    void RegisterCallBack(std::function<int(TensorPtrs&, void*)>);
    TensorPtrs Wait(int reqId);
    Tensors inputs(void *ptr=nullptr, uint64_t phyAddr=0);
    std::vector<Tensors> inputs(int devId);
    Tensors outputs(void *ptr=nullptr, uint64_t phyAddr=0);
    Tensors outputs(int devId);
    uint64_t input_size();
    uint64_t output_size();
    std::string name();
    int latency();
    uint32_t inference_time();
    friend std::ostream& operator<<(std::ostream&, const InferenceEngine&);
private:
    std::string _modelFile;
    std::string _modelDir;
    std::string _name;
    InferenceOption _option;
    std::vector<std::shared_ptr<Task>> _tasks; // to be changed to complex graph
    std::shared_ptr<Task> _head;
    std::shared_ptr<Task> _tail;
};

} /* namespace dxrt */
