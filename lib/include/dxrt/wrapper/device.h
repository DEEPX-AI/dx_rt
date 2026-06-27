/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * SDK wrapper for device management (Device, DevicePool, CheckDevices).
 * Header-only — all methods call C ABI functions (dxrt_*).
 * Replaces the internal device.h / device_pool.h for SDK consumers.
 */
#pragma once

#include "dxrt/dxrt_c_api.h"
#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_info_status.h"

#include <memory>
#include <mutex>
#include <vector>

namespace dxrt {

class DeviceCore {
 public:
    explicit DeviceCore(int id) : id_(id) {}

    void DoCustomCommand(void* data, uint32_t subCmd, uint32_t size = 0) {
        dxrt_status_t st = dxrt_device_custom_command(id_, data, subCmd, size);
        if (st != DXRT_OK) {
            throw Exception(std::string("DoCustomCommand failed: ") +
                            dxrt_last_error_message());
        }
    }

    int id() const { return id_; }

 private:
    int id_;
};

class Device {
 public:
    explicit Device(int id) : id_(id) {}

    int id() const { return id_; }

    void DoCustomCommand(void* data, uint32_t subCmd, uint32_t size = 0) const {
        dxrt_status_t st = dxrt_device_custom_command(id_, data, subCmd, size);
        if (st != DXRT_OK) {
            throw Exception(std::string("DoCustomCommand failed: ") +
                            dxrt_last_error_message());
        }
    }

    DeviceStatus GetCurrentStatus() const {
        return DeviceStatus::GetCurrentStatus(id_);
    }

 private:
    int id_;
};

class DevicePool {
 public:
    static DevicePool& GetInstance() {
        static DevicePool instance;
        return instance;
    }

    void InitCores() {
        std::call_once(initFlag_, [this]() {
            dxrt_status_t st = dxrt_device_pool_init();
            if (st != DXRT_OK) {
                throw Exception(std::string("DevicePool init failed: ") +
                                dxrt_last_error_message());
            }
            int count = 0;
            dxrt_device_get_count(&count);
            cores_.clear();
            cores_.reserve(count);
            for (int i = 0; i < count; ++i) {
                cores_.push_back(std::make_shared<DeviceCore>(i));
            }
        });
    }

    size_t GetDeviceCount() {
        InitCores();
        return cores_.size();
    }

    std::shared_ptr<DeviceCore> GetDeviceCores(int deviceId) {
        InitCores();
        return cores_.at(deviceId);
    }

 private:
    DevicePool() = default;
    ~DevicePool() = default;
    DevicePool(const DevicePool&) = delete;
    DevicePool& operator=(const DevicePool&) = delete;

    std::vector<std::shared_ptr<DeviceCore>> cores_;
    std::once_flag initFlag_;
};

[[deprecated("Use DevicePool instead")]]
inline std::vector<std::shared_ptr<Device>>& CheckDevices() {
    static std::vector<std::shared_ptr<Device>> devices;
    static std::once_flag flag;
    std::call_once(flag, []() {
        dxrt_status_t st = dxrt_device_pool_init();
        if (st != DXRT_OK) {
            throw Exception(std::string("CheckDevices init failed: ") +
                            dxrt_last_error_message());
        }
        int count = 0;
        dxrt_device_get_count(&count);
        devices.reserve(count);
        for (int i = 0; i < count; ++i) {
            devices.push_back(std::make_shared<Device>(i));
        }
    });
    return devices;
}

} // namespace dxrt
