/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 *
 * This file uses cxxopts (MIT License) - Copyright (c) 2014 Jarryd Beck.
 */

#pragma once
#include "dxrt/common.h"
#include <chrono>
#include <ostream>
#include <string>
#include "dxrt/device_core.h"
#include "dxrt/fw.h"
#include <vector>
#include <memory>




namespace dxrt {

    /*
     * After UPDATE_FIRMWARE the FW acks once the flash is written, then resets 0.5 s
     * later; the new image answers IDENTIFY again once it is up (the driver refuses
     * mailbox commands while the device reboots). WaitFwUpdateReboot polls until
     * every target answers, instead of sleeping a fixed time.
     */
    enum class FwRebootStatus { Ready, WrongVersion, Timeout };
    struct FwRebootTarget {
        std::shared_ptr<DeviceCore> device;
        std::string fwVer;                                  // image version "A.B.C[-suffix]"
        std::chrono::steady_clock::time_point ackTime;      // when UPDATE_FIRMWARE was acked
        FwRebootStatus status = FwRebootStatus::Timeout;
        uint32_t elapsedMs = 0;                             // ack to the answer (or last poll)
        dxrt_device_info_t info{};                          // the answering FW
    };
    struct FwRebootWaitParam {
        uint32_t timeoutMs = 60000;     // boot2nd may retrain LPDDR after an update
        uint32_t pollMs = 200;
        uint32_t minMs = 1500;          // before this, an answer may be the old image
    };
    DXRT_API bool FwVersionMatches(const std::string& fwVer, const dxrt_device_info_t& info);
    // false when a target's RT driver (< 2.5.1) or the platform (Windows) does not refuse
    // mailbox commands while the FW reboots; the caller then waits a fixed time instead
    DXRT_API bool FwRebootWaitSupported(const std::vector<FwRebootTarget>& targets);
    DXRT_API void WaitFwUpdateReboot(std::vector<FwRebootTarget>& targets,
                                     const FwRebootWaitParam& param = FwRebootWaitParam());
    // one line per target; returns false if any target is not Ready
    DXRT_API bool ReportFwUpdateReboot(const std::vector<FwRebootTarget>& targets, std::ostream& os);

    int UpdateFw(std::shared_ptr<DeviceCore> devicePtr, const std::string& fwFile, int subCmd);
    int UploadFw(std::shared_ptr<DeviceCore> devicePtr, const std::string& fwFile, int subCmd);
    int UpdateFwConfig(std::shared_ptr<DeviceCore> devicePtr, const std::string& jsonFile);
    std::vector<uint32_t> Dump(std::shared_ptr<DeviceCore> devicePtr);
    void UpdateFwConfig(std::shared_ptr<DeviceCore> devicePtr, std::vector<uint32_t> cfg);
    std::shared_ptr<FwLog> GetFwLog(std::shared_ptr<DeviceCore> devicePtr);
}  // namespace dxrt
