#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <map>

#include "dxrt/common.h"
#include "dxrt/model.h"
// #include "dxrt/inference_option.h"
#include "dxrt/tensor.h"
#include "dxrt/inference_option.h"
#include "dxrt/testdata.h"
#include "dxrt/circular_buffer.h"

#define NPU_PARAM_FILE "rmap.info"

namespace dxrt {
using rmapinfo = deepx_rmapinfo::RegisterInfoDatabase;
struct InferenceOption;
class Task;
struct TimePoint;
class InferenceEngine
{
public:
    InferenceEngine(const std::string &, InferenceOption &option=DefaultInferenceOption);
    ~InferenceEngine(void);
    TensorPtrs Run(void *inputPtr, void *userArg=nullptr, void *outputPtr=nullptr);
    // void Run(Tensors &inputs, Tensors &outputs); /* TODO */
    int RunAsync(void *inputPtr, void *userArg=nullptr, void *outputPtr=nullptr);
    float RunBenchMark(int num);
    TensorPtrs ValidateDevice(void *inputPtr, int deviceId=0);
    void RegisterCallBack(std::function<int(TensorPtrs&, void*)>);
    TensorPtrs Wait(int reqId);
    void PushLatency(int latency);
    void PushInferenceTime(uint32_t infTime);
    Tensors inputs(void *ptr=nullptr, uint64_t phyAddr=0);
    std::vector<Tensors> inputs(int devId);
    Tensors outputs(void *ptr=nullptr, uint64_t phyAddr=0);
    Tensors outputs(int devId);
    uint64_t input_size();
    uint64_t output_size();
    std::string name();
    int latency();
    uint32_t inference_time();
    vector<uint8_t> bitmatch_mask(int index);
    friend std::ostream& operator<<(std::ostream&, const InferenceEngine&);
private:
    std::string _modelFile;
    std::string _modelDir;
    std::string _name;
    ModelDataBase _modelData;
    std::vector<uint8_t> _maskBuf;
    std::map<std::string, deepx_graphinfo::GraphBindingDatabase> _graphMap;
    InferenceOption _option;
    std::vector<std::shared_ptr<Task>> _tasks; // to be changed to complex graph
    std::shared_ptr<Task> _head;
    std::shared_ptr<Task> _tail;
    int _numTails;
    std::map<std::string, std::shared_ptr<Task>> _taskMap;
    CircularBuffer<int> _latency;
    CircularBuffer<uint32_t> _infTime;
};

} /* namespace dxrt */
