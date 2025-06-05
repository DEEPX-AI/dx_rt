#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <memory>
#include <mutex>

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
    // static
public:
        static constexpr int INFERENCE_JOB_MAX_COUNT = 1024; // max job count

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
    
    /**
     * @brief Runs the inference engine using a specific input pointer vector.
     *
     * This function executes inference based on the provided input data pointer vector and returns a vector of output tensors.
     * Users can optionally provide additional user-defined arguments and output pointer vectors.
     * If userArgs is used, the number of elements in inputPtrs must be the same as the number of elements in userArgs.
     * An InvalidArgumentException is thrown if the size of inputPtrs and userArgs are different.
     * An InvalidArgumentException is thrown if the size of inputPtrs and OutputPtrs are different.
     * An InvalidArgumentException is thrown if the size of inputPtrs is 0.
     * 
     * @param[in] inputBuffers Vector of input data pointers used for inference.
     * @param[out] outputBuffers Vector of output data pointers.
     * @param[in] userArgs Vector of user-defined arguments (e.g., original frame data, input metadata, etc.). (Optional)
     *
     * @return Vector of output tensors as smart pointer instances.
     */
    std::vector<TensorPtrs> Run(
        const std::vector<void*>& inputBuffers,
        const std::vector<void*>& outputBuffers,
        const std::vector<void*>& userArgs = {}
    );
    

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
    
    /** 
     * @deprecated Use RunBenchmark() instead. 
     * @brief run benchmark with loop n times (Legacy API)
     * @param[in] num number of inferences
     * @param[in] inputPtr input data pointer to run inference
     * @return average fps
     */
    [[deprecated("Use RunBenchmark() instead")]]
    float RunBenchMark(int num, void* inputPtr=nullptr) { return RunBenchmark(num, inputPtr); }

    /** @brief run benchmark with loop n times
     * @param[in] num number of inferences
     * @param[in] inputPtr input data pointer to run inference
     * @return average fps
     */
    float RunBenchmark(int num, void* inputPtr=nullptr);

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

    /** 
     * @deprecated Use RegisterCallback() instead. 
     * @brief Register user callback function to be called by inference completion. (Legacy API)
     * @param[in] callbackFunc Function which is called when inference is complete, it gets outputs and user_arg ptr
     * @param outputs output tensors data
     * @param userArg userArg given by Run();
     */
    [[deprecated("Use RegisterCallback() instead")]]
    void RegisterCallBack(std::function<int(TensorPtrs& outputs, void* userArg)> callbackFunc) { return RegisterCallback(callbackFunc); }

    /** @brief Register user callback function to be called by inference completion.
     * @param[in] callbackFunc Function which is called when inference is complete, it gets outputs and user_arg ptr
     * @param outputs output tensors data
     * @param userArg userArg given by Run();
     */
    void RegisterCallback(std::function<int(TensorPtrs& outputs, void* userArg)> callbackFunc);

    /** @brief Wait until an request is complete and returns output
     * @param[in] jobId job Id returned by RunAsync()
     * @return output tensors as vector of smart pointer instances 
     */
    TensorPtrs Wait(int jobId);

    /**
     *  @deprecated Use GetInputs() instead. 
     *  @brief Get input tensor (Legacy API)
     *  @param[in] ptr pointer to virtual address
     *  @param[in] phyAddr pointer to physical address
     *  @return if ptr is null, input memory area in engine is returned
     *  @return if ptr and phyAddr is given, inputs tensors that contains output addresses
     */
    [[deprecated("Use GetInputs() instead")]]
    Tensors inputs(void *ptr=nullptr, uint64_t phyAddr=0) { return GetInputs(ptr, phyAddr); }
    
    /** @brief Get input tensor
     *  @param[in] ptr pointer to virtual address
     *  @param[in] phyAddr pointer to physical address
     *  @return if ptr is null, input memory area in engine is returned
     *  @return if ptr and phyAddr is given, inputs tensors that contains output addresses
     */
    Tensors GetInputs(void *ptr=nullptr, uint64_t phyAddr=0);
    
    /** 
     *  @deprecated Use GetInputs() instead.
     *  @brief Get input tensor (Legacy API)
     *  @param[in] devId device id
     *  @return vector of input tensors
     */
    [[deprecated("Use GetInputs() instead")]]
    std::vector<Tensors> inputs(int devId) { return GetInputs(devId); }
    
    /** @brief Get input tensor
     *  @param[in] devId device id
     *  @return vector of input tensors
     */
    std::vector<Tensors> GetInputs(int devId);
    
    /** 
     *  @deprecated Use GetOutputs() instead.
     *  @brief Get output tensor (Legacy API)
     *  @param[in] ptr pointer to virtual address
     *  @param[in] phyAddr pointer to physical address
     *  @return if ptr is null, output memory area in engine is returned
     *  @return if ptr and phyAddr is given, outputs tensors that contains output addresses
     */
    [[deprecated("Use GetOutputs() instead")]]
    Tensors outputs(void *ptr=nullptr, uint64_t phyAddr=0) { return GetOutputs(ptr, phyAddr); }

    /** @brief Get output tensor
     *  @param[in] ptr pointer to virtual address
     *  @param[in] phyAddr pointer to physical address
     *  @return if ptr is null, output memory area in engine is returned
     *  @return if ptr and phyAddr is given, outputs tensors that contains output addresses
     */
    Tensors GetOutputs(void *ptr=nullptr, uint64_t phyAddr=0);
    
    /** 
     * @deprecated Use GetInputSize() instead.
     * @brief Get total size of input tensors (Legacy API)
     * @return Input size of one inference in bytes
     */
    [[deprecated("Use GetInputSize() instead")]]
    uint64_t input_size() { return GetInputSize(); }

    /** 
     * @brief Get total size of input tensors
     * @return Input size of one inference in bytes
     */
    uint64_t GetInputSize();
    
    /** 
     * @deprecated Use GetOutputSize() instead.
     * @brief Get total size of output tensors (Legacy API)
     * @return Output size of one inference in bytes
     */
    [[deprecated("Use GetOutputSize() instead")]]
    uint64_t output_size() { return GetOutputSize(); }
    
    /** 
     * @brief Get total size of output tensors
     * @return Output size of one inference in bytes
     */
    uint64_t GetOutputSize();
    
     /** 
     * @deprecated Use GetModelName() instead.
     * @brief Get model name (Legacy API)
     * @return model name
     */
    [[deprecated("Use GetModelName() instead")]]
    std::string name() { return GetModelName(); }

    /** 
     * @brief Get model name
     * @return model name
     */
    std::string GetModelName();

    /** 
     * @deprecated Use GetTaskOrder() instead.
     * @brief Get model task order (Legacy API)
     * @return task order
     */
    [[deprecated("Use GetTaskOrder() instead")]]
    std::vector<string> task_order() { return GetTaskOrder(); }
    
    /** 
     * @brief Get model task order
     * @return task order
     */
    std::vector<string> GetTaskOrder();
    
    /**
     * @deprecated Use GetLatency() instead. 
     * @brief Get latest latency (Legacy API)
     * @return latency (microseconds)
     */
    [[deprecated("Use GetLatency() instead")]]
    int latency() { return GetLatency(); }

    /** @brief Get latest latency
     * @return latency (microseconds)
     */
    int GetLatency();    

    /**
     * @deprecated Use GetNpuInferenceTime() instead.  
     * @brief Get latest inference time (Legacy API)
     * @return inference time (microseconds)
     */
    [[deprecated("Use GetNpuInferenceTime() instead")]]
    uint32_t inference_time() { return GetNpuInferenceTime(); }
    
    /** @brief Get latest inference time
     * @return inference time (microseconds)
     */
    uint32_t GetNpuInferenceTime();
    
    /** @brief Get recent Latency
     * @return latency (microseconds)
     */
    std::vector<int> GetLatencyVector();
    
    /** @brief Get recent inference time
     * @return inference time (microseconds)
     */
    std::vector<uint32_t> GetNpuInferenceTimeVector();
    
    /** @brief Get latency Mean
     * @return latency Mean (microseconds)
     */
    double GetLatencyMean();
    
    /** @brief Get inference time Mean
     * @return inference time Mean (microseconds)
     */
    double GetNpuInferenceTimeMean();
    
    /** @brief Get latency Standard Deviation
     * @return latency Standard Deviation 
     */
    double GetLatencyStdDev();
    
    /** @brief Get inference time Standard Deviation
     * @return inference time Standard Deviation 
     */
    double GetNpuInferenceTimeStdDev();
    
    /** @brief Get latency Count
     * @return latency Count 
     */
    int GetLatencyCnt();
    
    /** @brief Get inference time Count
     * @return inference time Count
     */
    int GetNpuInferenceTimeCnt();

    /** 
     *  @deprecated Use GetAllTaskOutputs() instead.
     *  @brief Get output tensors of all tasks (Legacy API)
     *  @return the output of all tasks as a vector of smart pointer instance vectors.  
     */
    [[deprecated("Use GetAllTaskOutputs() instead")]]
    vector<TensorPtrs> get_outputs() { return GetAllTaskOutputs(); }

    /** 
     *  @brief Get output tensors of all tasks
     *  @return the output of all Tasks as a vector of smart pointer instance vectors.  
     */
    vector<TensorPtrs> GetAllTaskOutputs();

    /** 
     *  @deprecated Use GetBitmatchMask() instead.
     *  @internal
     *  @brief Get bitmatch mask (Legacy API)
     *  @param[in] index index to npu task (rmap)
     *  @return bitmatch mask for a given index
     */
    [[deprecated("Use GetBitmatchMask() instead")]]
    vector<uint8_t> bitmatch_mask(int index) { return GetBitmatchMask(index); }
    
    /** 
     *  @internal
     *  @brief Get bitmatch mask
     *  @param[in] index index to npu task (rmap)
     *  @return bitmatch mask for a given index
     */
    vector<uint8_t> GetBitmatchMask(int index);

    /** 
     * @deprecated Use GetNumTailTasks() instead.
     * @brief Returns the number of tail tasks in the model. (Legacy API)
     * @return The number of tasks that have no subsequent tasks.
     * 
     * Tail tasks are those which do not have any tasks following them in the model's task chain.
     * This function provides the count of such tail tasks.
     */
    [[deprecated("Use GetNumTailTasks() instead")]]
    int get_num_tails() { return GetNumTailTasks(); }

    /** 
     * @brief Returns the number of tail tasks in the model.
     * @return The number of tasks that have no subsequent tasks.
     * 
     * Tail tasks are those which do not have any tasks following them in the model's task chain.
     * This function provides the count of such tail tasks.
     */
    int GetNumTailTasks();

    /** 
     * @deprecated Use GetCompileType() instead.
     * @brief Returns the compile type of the model. (Legacy API)
     * @return The compile type of the model.
     */
    [[deprecated("Use GetCompileType() instead")]]
    string get_compile_type() { return GetCompileType(); }
    
    /** 
     * @brief Returns the compile type of the model.
     * @return The compile type of the model.
     */
    string GetCompileType(); 

    /** 
     * @deprecated Use IsPPU() instead.
     * @brief Returns whether the model is using PPU. (Legacy API)
     * @return whether the model is using PPU.
     */
    [[deprecated("Use IsPPU() instead")]]
    bool is_PPU() { return IsPPU(); }

    /** 
     * @brief Returns whether the model is using PPU.
     * @return whether the model is using PPU.
     */
    bool IsPPU();

    /** 
     * @brief Resource deallocation and cleanup
     */
    void Dispose();

#ifdef _WIN32
    float RunBenchMarkWindows(int num, void* inputPtr = nullptr);
#endif // _WIN32
    friend DXRT_API std::ostream& operator<<(std::ostream&, const InferenceEngine&);
    InferenceTimer* getTimer(){return &_inferenceTimer;}

private: // private functions
    int runAsync(void *inputPtr, void *userArg, void *outputPtr,
        std::function<void(TensorPtrs &outputs, void *userArg, int jobId)> batchCallback); 

    void runSubBatch(std::vector<TensorPtrs>& result, int batchCount, int startIndex, void* batchArgs,
            const std::vector<void*>& inputPtrs,
            const std::vector<void*>& outputPtrs,
            const std::vector<void*>& userArgs);

private:
    std::string _modelFile;
    std::string _modelDir;
    std::string _name;
    std::string _modelCompileType;
    bool _isPPU = false;
    
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
    
    void disposeOnce();
    std::once_flag _disposeOnceFlag;
    bool _isDisposed = false;

    // inference job for IE
    std::shared_ptr<CircularDataPool<InferenceJob>> _inferenceJobPool;

private:
    static std::mutex _sInferenceEngineMutex;
};

} /* namespace dxrt */
