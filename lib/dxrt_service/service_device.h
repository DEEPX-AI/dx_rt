/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#ifdef __linux__
    #include <unistd.h>
#endif
#include <errno.h>
#include <string.h>
#ifdef __linux__
    #include <sys/mman.h>
    #include <sys/ioctl.h>
#endif
#include <sys/types.h>
#include <limits>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <atomic>
#include <thread>
#include <vector>
#include <condition_variable>

#include "dxrt/request.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/worker.h"
#include "dxrt/driver_adapter/driver_adapter.h"
#include "dxrt/util.h"
#include "dxrt/device.h"
#include "dxrt/memory.h"
#include "dxrt/task.h"
#include "dxrt/buffer.h"
#include "dxrt/profiler.h"
#include "dxrt/filesys_support.h"
#include "dxrt/device_version.h"
#include "service_error.h"
#include "dxrt/fw.h"
#include "dxrt/multiprocess_memory.h"
#include "dxrt/driver_adapter/linux_driver_adapter.h"
#include "usage_timer.h"

#ifdef __linux__
    #include <poll.h>
#elif _WIN32
    #include <windows.h>

#endif

namespace dxrt {


class Worker;
class Memory;
class InferenceOption;
class TaskData;
class Profiler;
class Buffer;
class FwLog;
class DXRT_API ServiceDevice
{
 public:
    explicit ServiceDevice(const std::string &);
    virtual ~ServiceDevice(void);
    std::string name() const { return _name; }
    int id() const { return _id; }

    dxrt_device_info_t info() const{ return _info;}
    dxrt_device_status_t status();
    int Process(dxrt_cmd_t, void*, uint32_t size = 0, uint32_t sub_cmd = 0);


    virtual int InferenceRequest(dxrt_request_acc_t* req);

    void Identify(int id_, uint32_t subCmd = 0);
    void SetSubMode(uint32_t cmd) { _subCmd = cmd; }
    void Terminate();

    int AddBound(npu_bound_op boundOp);
    int DeleteBound(npu_bound_op boundOp);
    int GetBoundCount(npu_bound_op boundOp);
    int GetBoundTypeCount();
    bool CanAcceptBound(npu_bound_op boundOp);



    void CallBack();


    //void Identify(int id_, dxrt::SkipMode skip);
    void SetCallback(std::function<void(const dxrt_response_t&)> f);
    void SetErrorCallback(std::function<void(dxrt::dxrt_server_err_t, uint32_t, int)> f);
    static std::vector<shared_ptr<ServiceDevice>> CheckServiceDevices(uint32_t subCmd = 0);
    bool isBlocked(){return _isBlocked;}

    double getUsage(int core_id);

    void usageTimerTick();
    void DoCustomCommand(void *data, uint32_t subCmd, uint32_t size);
    void LoadPPCPUFirmware(uint64_t offset);

 protected:
    int _id = 0;
    DeviceType _type = DeviceType::ACC_TYPE; /* 0: ACC type, 1: STD type */

    int _bound_count[static_cast<int>(npu_bound_op::N_BOUND_INF_MAX)];

    uint32_t _variant;
    int _devFd = -1;
#ifdef __linux__
    struct pollfd _devPollFd;
#elif _WIN32
    HANDLE _devHandle = INVALID_HANDLE_VALUE;
#endif
    std::string _file;
    std::string _name;
    dxrt_device_info_t _info;
    dxrt_device_status_t _status;
    uint32_t _subCmd;
    int _load = 0;
    int _inferenceCnt = 0;
    bool _hasWorkers = false;
    Profiler &_profiler;

    std::thread _thread[3];
    std::thread _eventThread;

    std::mutex _lock;

    SharedMutex _boundLock;
    std::atomic<bool> _stop {false};


    std::shared_ptr<DriverAdapter> _driverAdapter;

    std::function<void(const dxrt_response_t&)> _callBack;

    std::function<void(dxrt::dxrt_server_err_t, uint32_t, int)> _errCallBack;
    bool _isBlocked = false;
    UsageTimer _timer[3];

    int BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp);
    int GetBoundTypeCountInternal();

    int WaitThread(int ids);
    int EventThread();

};

}  // namespace dxrt

