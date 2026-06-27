/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * DXRT Wrapper — DeviceStatus (prebuilt delivery).
 */

#pragma once
#include "dxrt/dxrt_c_api.h"
#include "dxrt/common.h"
#include <memory>

namespace dxrt {

class DeviceStatus
{
public:
    static int GetDeviceCount()
    {
        int count = 0;
        dxrt_device_get_count(&count);
        return count;
    }

    static DeviceStatus GetCurrentStatus(int device_id)
    {
        DeviceStatus ds;
        ds.device_id_ = device_id;
        return ds;
    }

    // Accept any type with id() method (Device, DeviceCore, shared_ptr<Device>, etc.)
    template<typename T>
    static DeviceStatus GetCurrentStatus(const std::shared_ptr<T>& device)
    {
        return GetCurrentStatus(device->id());
    }

    std::string GetInfoString() const
    {
        char buf[4096];
        if (dxrt_device_get_status(device_id_, buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::string GetStatusString() const
    {
        char buf[4096];
        if (dxrt_device_get_status(device_id_, buf, sizeof(buf)) == DXRT_OK) {
            return std::string(buf);
        }
        return "";
    }

    std::string DeviceTypeWord() const
    {
        char buf[128];
        if (dxrt_device_get_type_word(device_id_, buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::string DeviceVariantStr() const
    {
        char buf[128];
        if (dxrt_device_get_variant(device_id_, buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::string BoardTypeStr() const
    {
        char buf[128];
        if (dxrt_device_get_board_type(device_id_, buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::string MemoryTypeStr() const
    {
        char buf[128];
        if (dxrt_device_get_memory_type(device_id_, buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::string MemorySizeStrBinaryPrefix() const
    {
        char buf[128];
        if (dxrt_device_get_memory_size(device_id_, buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::string AllMemoryInfoStr() const
    {
        char buf[512];
        if (dxrt_device_get_all_memory_info(device_id_, buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    int Temperature(int channel) const
    {
        int temp = 0;
        dxrt_device_get_temperature(device_id_, channel, &temp);
        return temp;
    }

    uint32_t NpuClock(int channel) const
    {
        uint32_t clk = 0;
        dxrt_device_get_npu_clock(device_id_, channel, &clk);
        return clk;
    }

    uint32_t Voltage(int channel) const
    {
        uint32_t v = 0;
        dxrt_device_get_voltage(device_id_, channel, &v);
        return v;
    }

    std::string NpuStatusStr(int channel) const
    {
        char buf[128];
        if (dxrt_device_get_npu_status(device_id_, channel, buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::string DdrStatusStr(int channel) const
    {
        char buf[128];
        if (dxrt_device_get_ddr_status(device_id_, channel, buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::string DdrBitErrStr() const
    {
        char buf[256];
        if (dxrt_device_get_ddr_bit_err(device_id_, buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    int GetId() const { return device_id_; }

    int64_t MemorySize() const
    {
        int64_t bytes = 0;
        dxrt_device_get_memory_size_bytes(device_id_, &bytes);
        return bytes;
    }

    uint64_t MemoryClock() const
    {
        uint64_t mhz = 0;
        dxrt_device_get_memory_clock(device_id_, &mhz);
        return mhz;
    }

    int MemoryFrequency() const { return static_cast<int>(MemoryClock()); }

    uint64_t DmaChannel() const
    {
        uint64_t count = 0;
        dxrt_device_get_dma_channel_count(device_id_, &count);
        return count;
    }

    DeviceType GetDeviceType() const
    {
        uint32_t t = 0;
        dxrt_device_get_type_enum(device_id_, &t);
        return static_cast<DeviceType>(t);
    }

    std::string DeviceTypeStr() const
    {
        char buf[16];
        if (dxrt_device_get_type_str(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string PcieInfoStr() const
    {
        char buf[256];
        if (dxrt_device_get_pcie_info(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string PcieInfoStr(int spd, int wd, int bus, int dev, int func) const
    {
        (void)spd; (void)wd; (void)bus; (void)dev; (void)func;
        return PcieInfoStr();
    }

    std::string MemorySizeStrWithComma() const
    {
        char buf[128];
        if (dxrt_device_get_memory_size_comma(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string FirmwareVersionStr() const
    {
        char buf[128];
        if (dxrt_device_get_firmware_version(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::ostream& StatusToStream(std::ostream& os) const
    {
        // Print NPU status for all channels
        for (int i = 0; i < 4; i++) {
            std::string s = NpuStatusStr(i);
            if (!s.empty()) os << s << "\n";
        }
        os << "=======================================================" << "\n";
        return os;
    }

    std::ostream& DebugStatusToStream(std::ostream& os) const
    {
        for (int i = 0; i < 4; i++) {
            std::string s = DdrStatusStr(i);
            if (!s.empty()) os << s << "\n";
        }
        os << DdrBitErrStr() << "\n";
        os << "=======================================================" << "\n";
        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const DeviceStatus& d)
    {
        os << d.GetInfoString() << "\n";
        d.StatusToStream(os);
        return os;
    }

private:
    int device_id_ = 0;
};

} // namespace dxrt
