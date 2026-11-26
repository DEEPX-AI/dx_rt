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

#include "dxrt/common.h"
#include "dxrt/cli.h"

#include <cstring>
#include <ostream>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "dxrt/device.h"
#include "dxrt/fw.h"
#include "dxrt/util.h"
#include "dxrt/device_info_status.h"
#include "dxrt/filesys_support.h"
#include "dxrt/driver.h"
#include "dxrt/extern/rapidjson/document.h"
#include "dxrt/extern/rapidjson/istreamwrapper.h"
#include "dxrt/objects_pool.h"
#include "../lib/resource/log_messages.h"
#include "dxrt/device_version.h"
#include "dxrt/device_struct.h"
#include "dxrt/device_struct_operators.h"
#include "dxrt/device_pool.h"
#include "dxrt/device_util.h"
#include "dxrt/cli_support.h"


namespace dxrt {

namespace {

// FW reports fw_ver_suffix from this version on (see device_info_status.cpp)
constexpr uint16_t kFwVersionSupportSuffix = 230;

// RT driver from which mailbox commands are refused while the FW reboots (DLMSG
// MAILBOX_READY gate). Older drivers would write the mailbox in SYSRAM while boot2nd
// runs from it, so they keep the fixed wait instead of polling.
constexpr uint32_t kRtDrvVersionFwRebootGate = 2501;  // 2.5.1

// Same rules as the FW: dotnum_to_integer() / version_to_suffix() in plat commons/dx_utils.c
uint16_t FwVersionNumber(const std::string& ver)
{
    uint32_t number = 0;
    uint32_t segment = 0;
    for (char c : ver)
    {
        if (c == '.')
        {
            number = number * 10 + segment;
            segment = 0;
        }
        else if (c == '-')
        {
            break;
        }
        else if (c >= '0' && c <= '9')
        {
            segment = segment * 10 + static_cast<uint32_t>(c - '0');
        }
    }
    return static_cast<uint16_t>(number * 10 + segment);
}

std::string FwVersionSuffix(const std::string& ver)
{
    const auto dash = ver.find('-');
    if (dash == std::string::npos)
        return "";
    std::string suffix = ver.substr(dash + 1);
    // the FW drops a suffix that does not fit fw_ver_suffix[16]
    return (suffix.size() < sizeof(dxrt_device_info_t::fw_ver_suffix)) ? suffix : "";
}

uint32_t MsSince(std::chrono::steady_clock::time_point t)
{
    return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - t).count());
}

}  // namespace

bool FwVersionMatches(const std::string& fwVer, const dxrt_device_info_t& info)
{
    if (info.fw_ver != FwVersionNumber(fwVer))
        return false;
    const std::string want = FwVersionSuffix(fwVer);
    if (want.empty() || info.fw_ver < kFwVersionSupportSuffix)
        return true;
    const size_t len = strnlen(info.fw_ver_suffix, sizeof(info.fw_ver_suffix));
    return want == std::string(info.fw_ver_suffix, len);
}

bool FwRebootWaitSupported(const std::vector<FwRebootTarget>& targets)
{
#ifdef _WIN32
    std::ignore = targets;
    return false;       // the Windows driver has no reboot gate
#else
    for (const auto& t : targets)
    {
        if (t.device->devInfo().rt_drv_ver.driver_version < kRtDrvVersionFwRebootGate)
            return false;
    }
    return true;
#endif
}

bool ReportFwUpdateReboot(const std::vector<FwRebootTarget>& targets, std::ostream& os)
{
    bool allReady = true;
    for (const auto& t : targets)
    {
        os << "    Device " << t.device->id() << ": ";
        if (t.status == FwRebootStatus::Ready)
        {
            os << "firmware " << t.fwVer << " is up (" << t.elapsedMs
               << " ms after the update ack)" << std::endl;
            continue;
        }
        allReady = false;
        if (t.status == FwRebootStatus::WrongVersion)
        {
            const std::string suffix(t.info.fw_ver_suffix,
                                     strnlen(t.info.fw_ver_suffix, sizeof(t.info.fw_ver_suffix)));
            os << "came back with firmware " << GetFWVersionFromDeviceInfo(t.info.fw_ver, suffix.c_str())
               << ", not " << t.fwVer << std::endl;
        }
        else
        {
            os << "no answer " << t.elapsedMs << " ms after the update ack" << std::endl;
        }
    }
    return allReady;
}

void WaitFwUpdateReboot(std::vector<FwRebootTarget>& targets, const FwRebootWaitParam& param)
{
    std::vector<bool> sawDown(targets.size(), false);
    std::vector<bool> done(targets.size(), false);
    size_t left = targets.size();
    const auto start = std::chrono::steady_clock::now();

    while (left > 0)
    {
        for (size_t i = 0; i < targets.size(); i++)
        {
            if (done[i])
                continue;
            auto& t = targets[i];
            dxrt_device_info_t info{};
#if DXRT_USB_NETWORK_DRIVER == 0
            const int ret = t.device->Process(dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, &info, 0, 0);
#else
            const int ret = t.device->Process(dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, &info,
                                              static_cast<uint32_t>(sizeof(info)), 0, true);
#endif
            t.elapsedMs = MsSince(t.ackTime);
            if (ret != 0)
            {
                sawDown[i] = true;      // rebooting: the driver refuses or the FW does not ack
                continue;
            }
            if (!sawDown[i] && t.elapsedMs < param.minMs)
                continue;               // may still be the old image before its reset
            t.info = info;
            t.status = FwVersionMatches(t.fwVer, info) ? FwRebootStatus::Ready
                                                       : FwRebootStatus::WrongVersion;
            done[i] = true;
            left--;
        }
        if (left == 0 || MsSince(start) >= param.timeoutMs)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(param.pollMs));
    }
}

int UpdateFw(std::shared_ptr<DeviceCore> devicePtr, const std::string& fwFile, int subCmd)
{
    DXRT_ASSERT(fileExists(fwFile), fwFile + " doesn't exist.");
    std::vector<uint8_t> buf(getFileSize(fwFile));
    DataFromFile(fwFile, buf.data());
    return devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_FIRMWARE,
        buf.data(), static_cast<uint32_t>(buf.size()), subCmd);
}

int UploadFw(std::shared_ptr<DeviceCore> devicePtr, const std::string& fwFile, int subCmd)
{
    DXRT_ASSERT(fileExists(fwFile), fwFile + " doesn't exist.");
    std::vector<uint8_t> buf(getFileSize(fwFile));
    DataFromFile(fwFile, buf.data());
    return devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPLOAD_FIRMWARE,
        buf.data(), static_cast<uint32_t>(buf.size()), subCmd);
}

int UpdateFwConfig(std::shared_ptr<DeviceCore> devicePtr, const std::string& jsonFile)
{
    DXRT_ASSERT(fileExists(jsonFile), jsonFile + " doesn't exist.");
    std::vector<uint8_t> buf(getFileSize(jsonFile));
    DataFromFile(jsonFile, buf.data());
    devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_CONFIG_JSON,
        buf.data(), static_cast<uint32_t>(buf.size()));
    return buf[0];
}

std::vector<uint32_t> Dump(std::shared_ptr<DeviceCore> devicePtr)
{
    std::vector<uint32_t> dump(1000, 0);
    devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DUMP, dump.data());
    return dump;
}
void UpdateFwConfig(std::shared_ptr<DeviceCore> devicePtr, std::vector<uint32_t> cfg)
{
    auto size = sizeof(uint32_t) * cfg.size();
    devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_CONFIG,
        cfg.data(), static_cast<uint32_t>(size));
}
std::shared_ptr<FwLog> GetFwLog(std::shared_ptr<DeviceCore> devicePtr)
{
    std::vector<dxrt_device_log_t> logBuf(static_cast<int>(16 * 1024 / sizeof(dxrt_device_log_t)), {0, 0, {0, }});
    devicePtr ->Process(dxrt::dxrt_cmd_t::DXRT_CMD_GET_LOG, logBuf.data());
    auto fwlog = std::make_shared<FwLog>(logBuf);
    return fwlog;
}


}  // namespace dxrt
