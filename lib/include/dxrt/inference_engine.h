#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <memory>

#include "dxrt/common.h"
#include "dxrt/model.h"
// #include "dxrt/inference_option.h"
#include "dxrt/tensor.h"
#include "dxrt/inference_option.h"
#include "dxrt/testdata.h"
#include "dxrt/inference_job.h"
#include "dxrt/inference_timer.h"

#ifdef _WIN32
#include <chrono>
#include <thread>
#endif

#define NPU_PARAM_FILE "rmap.info"


/** 
 * @mainpage DX Runtime API
 * 
 * @subsection intro_sec Introduction
 * This is the main page of the documentation.  
 * The DX Runtime API is an API for developing applications using the DX NPU Core.
 * 
 */

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
 * dxrt::InferenceOption option;
 * option.devices = {0,1,3};  //use only 0,1,3 device
 * auto ie = dxrt::InferenceEngine(modelPath, option);
 * @endcode
 * @headerfile "dxrt/dxrt_api.h"
*/
class DXRT_API InferenceEngine
{
public:
    /** @brief Perform the task of loading the model and configuring the NPU to run.
     * @param[in] modelPath model path
     * @param[in] option device and npu core options
     * @code
     * auto modelPath = "model.dxnn"; // assume compiled model path name is "model.dxnn"
     * dxrt::InferenceEngine(modelPath) ie(modelPath);
     * auto outputs = ie.Run();
     * 
     * dxrt::InferenceOption op;
     * op.devices.push_back(0); 
     * op.boundOption = dxrt::InferenceOption::BOUND_OPTION::NPU_0; // NPU_0 only
     * dxrt::InferenceEngine ie(modelPath, op);
     * auto outputs = ie.Run();
     * @endcode
     */
    InferenceEngine(const std::string &modelPath, InferenceOption &option=DefaultInferenceOption);
    ~InferenceEngine(void);

    /** @brief Run inference engine using specific input pointer Synchronously
     * @param[in] inputPtr input data pointer to run inference
     * @param[in] userArg user-defined arguments as a void pointer(e.g. original frame data, metadata about input, ... )
     * @param[out] outputPtr pointer to output data, if it is nullptr, output data is stored in buffer inside DXRT.
     * @code
     * auto modelPath = "model.dxnn"; // assume compiled model path name is "model.dxnn"
     * dxrt::InferenceEngine ie(modelPath);
     * auto outputs = ie.Run();
     * @endcode
     * @return output tensors as vector of smart pointer instances 
     */
    TensorPtrs Run(void *inputPtr, void *userArg=nullptr, void *outputPtr=nullptr);
    // void Run(Tensors &inputs, Tensors &outputs); /* TODO */
    /** @brief Non-blocking call to request asynchronous inference by input pointer, and get job ID from inference engine.
     * @param[in] inputPtr input data pointer to run inference
     * @param[in] userArg user-defined arguments as a void pointer(e.g. original frame data, metadata about input, ... )
     * @param[out] outputPtr pointer to output data, if it is nullptr, output data area is allocated by DXRT.
     * @code
     * auto modelPath = "model"; // assume compiled model path name is "model"
     * dxrt::InferenceOption option;
     * option.devices = {0,1,3};  //use only 0,1,3 device
     * dxrt::InferenceEngine ie(modelPath, option);
     * auto outputs = ie.Run();
     * @endcode
     * @return job id that can be used to wait() function 
     */
    int RunAsync(void *inputPtr, void *userArg=nullptr, void *outputPtr=nullptr);
    /** @brief run benchmark with loop n times
     * @param[in] num number of inferences
     * @param[in] inputPtr input data pointer to run inference
     * @return average fps
     */
    float RunBenchMark(int num, void* inputPtr=nullptr);
    /** 
     * @brief Validate inference of a specific NPU device connected to the host.
     * This function runs a validation process using the provided input data on the specified NPU device.
     * It can be used to ensure that the NPU device is operational and can process inference tasks correctly.
     * 
     * @param[in] inputPtr Pointer to the input data used for validation.
     * @param[in] deviceId ID of the NPU device to validate. Default is 0 (first device).
     * @return Output tensors as a vector of smart pointer instances, representing the validation results.
     * 
     */
    TensorPtrs ValidateDevice(void *inputPtr, int deviceId=0);
    /** @brief Register user callback function to be called by inference completion.
     * @param[in] callbackFunc Function which is called when inference is complete, it gets outputs and user_arg ptr
     * @param outputs output tensors data
     * @param userArg userArg given by Run();
     */
    void RegisterCallBack(std::function<int(TensorPtrs& outputs, void* userArg)> callbackFunc);

    /** @brief Wait until an request is complete and returns output
     * @param[in] jobId job Id returned by RunAsync()
     * @return output tensors as vector of smart pointer instances 
     */
    TensorPtrs Wait(int jobId);

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
    /** @brief Get model task order
     * @return task order
     */
    std::vector<string> task_order();
    /** @brief Get recent latency
     * @return latency (microseconds)
     */
    int latency();
    /** @brief Get recent inference time
     * @return inference time (microseconds)
     */
    uint32_t inference_time();
    vector<TensorPtrs> get_outputs();
    vector<uint8_t> bitmatch_mask(int index);
    /** 
     * @brief Returns the number of tail tasks in the model.
     * @return The number of tasks that have no subsequent tasks.
     * 
     * Tail tasks are those which do not have any tasks following them in the model's task chain.
     * This function provides the count of such tail tasks.
     */
    int get_num_tails();
    /** 
     * @brief Returns the compile type of the model.
     * @return The compile type of the model.
     */
    string get_compile_type();    
    friend DXRT_API std::ostream& operator<<(std::ostream&, const InferenceEngine&);
private:
    std::string _modelFile;
    std::string _modelDir;
    std::string _name;
    std::string _modelCompileType;
    ModelDataBase _modelData;
    std::vector<uint8_t> _maskBuf;
    std::map<std::string, deepx_graphinfo::GraphBindingDatabase> _graphMap;
    InferenceOption _option;
    std::vector<std::shared_ptr<Task>> _tasks; // to be changed to complex graph
    std::shared_ptr<Task> _head;
    std::vector<std::shared_ptr<Task>> _tails;
    int _numTails;
    std::map<std::string, std::shared_ptr<Task>> _taskMap;
    InferenceTimer _inferenceTimer;
    std::vector<string> _taskOrder;
    std::vector<string> _lastOutputOrder; 

    std::function<int(TensorPtrs &outputs, void *userArg)> _userCallback;
    std::vector<bool> _occupiedInferenceJobs;
};

} /* namespace dxrt */
