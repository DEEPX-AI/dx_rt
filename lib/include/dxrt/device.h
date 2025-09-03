/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <signal.h>
#include <mutex>
#include <atomic>
#include <thread>
#include <string>
#include <condition_variable>
#include <ostream>
#include "dxrt/common.h"
#include "dxrt/request_data.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/worker.h"
#include "dxrt/driver_adapter/driver_adapter.h"
#include "dxrt/task_data.h"
#include "dxrt/npu_memory_cache.h"
#include "dxrt/exception/server_err.h"

#define DEVICE_NUM_BUF 2
#define DEVICE_OUTPUT_WORKER_NUM 4
// DSP //////////////////////////////////////////////////////
#define DSP_INPUT_SIZE    (4096*2048*3)      //4KRGBx1 = 24MB
#define DSP_OUTPUT_SIZE   (4096*2048*3)      //4KRGBx1 = 24MB
#define DSP_INFO_MEM_SIZE (0x100000)         //1MB
#define DSP_IN_MEM_SIZE   (DSP_INPUT_SIZE *2)//4KRGBx2 = 48MB
#define DSP_OUT_MEM_SIZE  (DSP_OUTPUT_SIZE*2)//4KRGBx2 = 48MB
#define DSP_SRAM_SIZE     (256*1024)         //256KB

#define MESSAGE_OFFSET_IN_SRAM 0x0003F000
#define MESSAGE_MAX_SIZE 256
#define MESSAGE_HEAD_SIZE 8

#define FUNC_ID_YUV420_TO_RGB_LETTER_PAD 0x0031
#define FUNC_ID_BOUNDING_BOX             0x2010
#define FUNC_ID_DRAW_BOX_TEXT            0x4010

#define DSPCV_8UC1 1     // (1: uint8 single channel)
#define DSPCV_8UC3 3     // (3: uint8 single channel)
//~DSP //////////////////////////////////////////////////////


#define RMAP_RECOVERY_DONE      (1)
#define WEIGHT_RECOVERY_DONE    (2)

#ifdef __linux__
    #include <poll.h>
#elif _WIN32
    #include <windows.h>

#endif

namespace dxrt {
using DevicePtr = std::shared_ptr<Device>;

enum SkipMode
{
    NONE            = 0,
    VERSION_CHECK,
    COMMON_SKIP,
    STATIC_SAVE_SKIP,
    IDENTIFY_SKIP
};



class DeviceInputWorker;
class DeviceOutputWorker;
class DeviceEventWorker;
class Memory;
class InferenceOption;
class Profiler;
class Buffer;
class FwLog;
class MultiprocessMemory;
class Request;

class DXRT_API Device
{
public:
    const static std::string DeviceStatusTable[];
    static std::atomic<bool> _sNpuValidateOpt; 
    
public:
    Device(const std::string &);
    ~Device(void);
    std::string name() { return _name; }
    int id() { return _id; }    
    //void *input_buf(int taskId, int bufId);
    int load();
    void pick();
    int infCnt();
#ifdef __linux__
    int fd();
#elif _WIN32
    HANDLE fd();
#endif

    dxrt_device_info_t info() { return _info;}
    dxrt_device_status_t status();
    dxrt_dev_info_t devInfo() { return _devInfo; }
    int Process(dxrt_cmd_t, void*, uint32_t size = 0, uint32_t sub_cmd = 0, uint64_t address = 0);


    int InferenceRequest(RequestData* req, npu_bound_op boundOp = N_BOUND_NORMAL);
    TensorPtrs Validate(std::shared_ptr<Request> req, bool skipInference = false);
    int Release(TaskData *task);
    int Response(dxrt_response_t &response);
    int Write(dxrt_meminfo_t &, int ch);
    int Write(dxrt_meminfo_t &);
    int Read(dxrt_meminfo_t &);
    int Read(dxrt_meminfo_t &, int ch, bool ctrlCmd = true);
    int Wait();
    void Identify(int id_, SkipMode skip = NONE, uint32_t subCmd = 0);
    void SetSubMode(uint32_t cmd) { _subCmd = cmd; }
    void BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp);
    void Terminate();
    void Reset(int opt);
    void ResetBuffer(int opt);
    int UpdateFw(std::string fwFile, int subCmd = 0);
    int UploadFw(std::string fwFile, int subCmd = 0);
    int UpdateFwConfig(std::string jsonFile);
    uint32_t UploadModel(std::string filePath, uint64_t base_addr);
    void DoCustomCommand(void *data, uint32_t subCmd, uint32_t size = 0);
    std::shared_ptr<FwLog> GetFwLog();
    int64_t Allocate(uint64_t size);
    int64_t Allocate(dxrt_request_t &inference);
    void Deallocate(uint64_t addr);
    void Deallocate(dxrt_request_t &inference);
    void ThreadImpl(void);
    int RegisterTask(TaskData *task);
    void CallBack();
    dxrt_model_t npu_model(int taskId);
    std::vector<Tensors> inputs(int taskId);
    Tensors outputs(int taskId);
    friend DXRT_API std::ostream& operator<<(std::ostream &, const Device&);

    dxrt_request_acc_t peekInferenceAcc(uint32_t requestId);
    dxrt_request_t* peekInferenceStd(uint32_t requestId);
    void popInferenceStruct(uint32_t requestId);
    void signalToWorker(int channel);
    void signalToDevice(npu_bound_op boundOp, int taskId, uint32_t checksum);
    void signalToDeviceDeInit(npu_bound_op boundOp, int taskId,  uint32_t checksum);
    void Deallocate_npuBuf(int64_t addr, int taskId);
    void StartDev(uint32_t option);
    bool isBlocked(){return _isBlocked.load();}
    void block(){_isBlocked.store(true); std::cout << "BLOCKED\n";}
    void unblock(){_isBlocked.store(false);}
    DeviceType getDeviceType(){return _type;}

    void DSP_ThreadImpl(void);
	void DSP_Identify(int id_, SkipMode skip = NONE, uint32_t subCmd = 0);
	void DSP_SetDspEnable(int enable) { _isDsp.store(enable); }
    int DSP_GetDspEnable() { return _isDsp.load(); }	
    int DSP_FlushCache(uint64_t targetAddr, uint32_t sizeInByte);    
    int DSP_SetCommand(dxrt_dspcvmat_t *dspCvMatInPtr, dxrt_dspcvmat_t *dspCvMatOutPtr, dxrt_dsp_request_t *dsp_req_packet);
    int DSP_ProcessRequest(RequestData* req, dxrt_dspcvmat_t *dspCvMatInPtr, dxrt_dspcvmat_t *dspCvMatOutPtr);   
    int DSP_GetBufferPtrFromMem(uint64_t *inputPtr, uint64_t *outputPtr); 
	
    void DoPcieCommand(void *data, uint32_t subCmd, uint32_t size);
    void ShowPCIEDetails(std::ostream& os);
    void ShowPCIEDetails();

#ifdef USE_SERVICE
    void SignalToService(dxrt_request_acc_t* npu_inference_acc);
    void ProcessResponseFromService(const dxrt_response_t& resp);
    void ProcessErrorFromService(dxrt_server_err_t err, int value);
    #endif
    std::unordered_map<int, dxrt_model_t> _npuModel;
protected:
    int _id = 0;
    DeviceType _type = DeviceType::ACC_TYPE; /* 0: ACC type, 1: STD type */
    SkipMode _skip;
    npu_bound_op _boundOp;
    uint32_t _variant;
    std::atomic<int> _isDsp{0};
    
#ifdef __linux__
    int _devFd = -1;
    struct pollfd _devPollFd;
    void SelectDriver();
#elif _WIN32
    HANDLE _devHandle = INVALID_HANDLE_VALUE;
#endif
    std::string _file;
    std::string _name;
    dxrt_device_info_t _info;
    dxrt_device_status_t _status;
    dxrt_dev_info_t _devInfo;
    uint32_t _subCmd;
    std::atomic<int> _load{0};
    int _inferenceCnt = 0;
    bool _hasWorkers = false;
    Profiler &_profiler;
    std::shared_ptr<Memory> _memory;
    std::shared_ptr<Memory> _modelMem;
    std::shared_ptr<Memory> _featureMem;
    std::thread _thread;
    std::mutex _lock;
    std::atomic<bool> _stop {false};
    std::unordered_map<int, std::vector< dxrt_request_t >> _npuInference;
    std::unordered_map<int, std::vector< dxrt_request_acc_t >> _npuInferenceAcc;
    std::unordered_map<int, std::vector<Tensors>> _inputTensors;
    std::unordered_map<int, std::vector<Tensors>> _outputTensors;
    //std::unordered_map<int, std::vector<std::vector<uint8_t>>> _inputTensorBuffers;
    //std::unordered_map<int, std::vector<std::vector<uint8_t>>> _outputTensorBuffers;
    std::unordered_map<int, std::vector<uint8_t>> _outputValidateBuffers;
    std::unordered_map<int, int> _bufIdx;
    std::unordered_map<int, dxrt_request_t> _ongoingRequestsStd;
    std::unordered_map<int, dxrt_request_acc_t> _ongoingRequestsAcc;
    std::shared_ptr<DeviceInputWorker> _inputWorker = nullptr;
    std::shared_ptr<DeviceOutputWorker> _outputWorker = nullptr;
    std::shared_ptr<DeviceEventWorker> _eventWorker = nullptr;
    //std::shared_ptr<Buffer> _buffer = nullptr;
    std::shared_ptr<DriverAdapter> _driverAdapter;
    std::atomic<int> _readChannel{0};
    std::atomic<int> _writeChannel{0};
    int RegisterTask_STD(TaskData *task);
    int RegisterTask_ACC(TaskData *task);
    int InferenceRequest_STD(RequestData* req, npu_bound_op boundOp);
    int InferenceRequest_ACC(RequestData* req, npu_bound_op boundOp);


    dxrt_meminfo_t _dspInData;
    dxrt_meminfo_t _dspOutData;
    std::unordered_map<int, std::vector< dxrt_request_t >> _dspProcRequests;
    int DSP_RegisterTask_STD(TaskData *task);
    bool _isBoundOptionSet = false;
    npu_bound_op _setBoundOption;

    NpuMemoryCacheManager _npuMemoryCacheManager;
    std::mutex _npuInferenceLock;

    std::atomic<bool> _isBlocked = {false};
    SharedMutex _registerTaskMutex;

};

extern DXRT_API std::shared_ptr<Device> PickOneDevice(std::vector<std::shared_ptr<Device>> &devices_, int isDspReq);
extern DXRT_API std::vector<std::shared_ptr<Device>>& CheckDevices(SkipMode skip = NONE, uint32_t subCmd = 0);
extern DXRT_API void WaitDeviceResponses(std::vector<std::shared_ptr<Device>> &devices_); // temp.
extern DXRT_API int DSP_GetBufferPtrFromObjPools(uint64_t *inputPtr, uint64_t *outputPtr);

DXRT_API std::ostream& operator<<(std::ostream&, const dxrt_device_status_t&);
DXRT_API std::ostream& operator<<(std::ostream& os, const dxrt_device_info_t& info);
DXRT_API std::ostream& operator<<(std::ostream& os, const dx_pcie_dev_ntfy_throt_t& notify);
} // namespace dxrt
