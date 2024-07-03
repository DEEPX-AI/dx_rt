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


/** @brief This class abstracts runtime inference executor for user's compiled model.
 * @details After user loads compiled model to InferenceEngine, real-time device tasks will be scheduled by internal runtime libraries.
 *          It supports both inference mode (synchronous/asynchronous) according to user's request.
 * @code
 * // Use default inference option
 * auto modelPath = "model.dxnn"; // assume compiled model path name is "model.dxnn"
 * auto ie = dxrt::InferenceEngine(modelPath, nullptr);
 * @endcode
 * @code
 * // Use a new inference option
 * auto modelPath = "model.dxnn"; // assume compiled model path name is "model.dxnn"
 * dxrt::InferenceOption option(1, 1, 2, dxrt::InferenceMode::MODE_ASYNC);
 * auto ie = dxrt::InferenceEngine(modelPath, option);
 * @endcode
 * @headerfile "dxrt/dxrt_api.h"
*/
class InferenceEngine
{
public:
    InferenceEngine(const std::string &, InferenceOption &option=DefaultInferenceOption);
    ~InferenceEngine(void);
    /** @brief Run inference engine using specific input pointer Synchronously
     * @param[in] inputPtr input data pointer to run inference
     * @param[in] userArg user-defined arguments as a void pointer(e.g. original frame data, metadata about input, ... )
     * @param[out] outputPtr pointer to output data, if it is nullptr, output data is stored in buffer inside DXRT.
     * @code
     * auto modelPath = "model"; // assume compiled model path name is "model"
     * dxrt::InferenceOption option(1, 1, 2, dxrt::InferenceMode::MODE_ASYNC);
     * auto ie = dxrt::InferenceEngine(modelPath, option);
     * auto outputs = ie.Run();
     * @endcode
     * @return output tensors as vector of smart pointer instances 
     */
    TensorPtrs Run(void *inputPtr, void *userArg=nullptr, void *outputPtr=nullptr);
    // void Run(Tensors &inputs, Tensors &outputs); /* TODO */
    /** @brief Non-blocking call to request asynchronous inference by input pointer, and get request ID from inference engine.
     * @param[in] inputPtr input data pointer to run inference
     * @param[in] userArg user-defined arguments as a void pointer(e.g. original frame data, metadata about input, ... )
     * @param[out] outputPtr pointer to output data, if it is nullptr, output data area is allocated by DXRT.
     * @code
     * auto modelPath = "model"; // assume compiled model path name is "model"
     * dxrt::InferenceOption option(1, 1, 2, dxrt::InferenceMode::MODE_ASYNC);
     * auto ie = dxrt::InferenceEngine(modelPath, option);
     * auto outputs = ie.Run();
     * @endcode
     * @return request id that can be used to wait() function 
     */
    int RunAsync(void *inputPtr, void *userArg=nullptr, void *outputPtr=nullptr);
    /** @brief run benchmark with loop n times
     * @param[in] num number of inferences
     * @return average fps
     */
    float RunBenchMark(int num);
    TensorPtrs ValidateDevice(void *inputPtr, int deviceId=0);
    /** @brief Register user callback function to be called by inference completion.
     * @param[in] callbackFunc Function which is called when inference is complete, it gets outputs and user_arg ptr
     * @param outputs output tensors data
     * @param userArg userArg given by Run();
     */
    void RegisterCallBack(std::function<int(TensorPtrs& outputs, void* userArg)> callbackFunc);

    /** @brief Wait until an request is complete and returns output
     * @param[in] reqId request Id returned by RunAsync()
     * @return output tensors as vector of smart pointer instances 
     */
    TensorPtrs Wait(int reqId);
    void PushLatency(int latency);
    void PushInferenceTime(uint32_t infTime);

    /** @brief Get input tensor
     *  @param[in] ptr pointer to virtual address
     *  @param[in] phyAddr pointer to physical address
     *  @return if ptr is null, input memory area in engine is returned
     *  @return if ptr and phyAddr is given, inputs tensors that contains output addresses
     */
    Tensors inputs(void *ptr=nullptr, uint64_t phyAddr=0);
    std::vector<Tensors> inputs(int devId);
    /** @brief Get output tensor
     *  @param[in] ptr pointer to virtual address
     *  @param[in] phyAddr pointer to physical address
     *  @return if ptr is null, output memory area in engine is returned
     *  @return if ptr and phyAddr is given, outputs tensors that contains output addresses
     */
    Tensors outputs(void *ptr=nullptr, uint64_t phyAddr=0);
    Tensors outputs(int devId);
    /** @brief Get total size of input tensors 
     *  @return input size of one inference in bytes
     */
    uint64_t input_size();
    /** @brief Get total size of output tensors 
     *  @return output size of one inference in bytes
     */
    uint64_t output_size();
    /** @brief Get model name
     * @return model name
     */
    std::string name();
    /** @brief Get recent latency
     * @return latency (microseconds)
     */
    int latency();
    /** @brief Get recent inference time
     * @return inference time (microseconds)
     */
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
