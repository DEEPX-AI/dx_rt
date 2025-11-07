/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/device_info_status.h"
#include "dxrt/device.h"
#include "dxrt/device_util.h"
#include "dxrt/exception/exception.h"
#include "dxrt/map_lookup_template.h"
#include "dxrt/device_pool.h"
#include "dxrt/device_core.h"
#include "dxrt/device_task_layer.h"
#include "dxrt/common.h"
#include<map>
#include<iostream>
#include<iomanip>
#include<sstream>
#include<array>

using std::string;
using std::endl;
using std::map;

using pair_type = std::pair<int, const char*>;

constexpr std::array<pair_type, 2> device_types = {{{0, "ACC"}, {1, "STD"}}};

constexpr std::array<pair_type, 2> device_type_words = {{{0, "Accelerator"}, {1, "Standalone"}}};
constexpr std::array<pair_type, 7> device_variants = {{{100, "L1"}, {101, "L2"}, {102, "L3"}, {103, "L4"}, {104, "V3"},
    {200, "M1"}, {202, "M1"}}};
constexpr std::array<pair_type, 3> memory_types{{{1, "LPDDR4"}, {2, "LPDDR5"}, {3, "LPDDR5x"}}};
constexpr std::array<pair_type, 5> memory_vendors{{{0x0, "NOT SUPPORTED"}, {0x4, "SS"}, {6, "HY"}, {0x08, "WB"}, {0xFF, "MI"}}};
constexpr std::array<pair_type, 3> board_types = {{{1, "SOM"}, {2, "M.2"}, {3, "H1"}}};


string convert_capacity(uint64_t n)
{
    constexpr uint64_t killo = 1024;
    constexpr uint64_t mega = killo *killo;
    constexpr uint64_t giga = mega*killo;
    constexpr uint64_t tera = giga*killo;
    double value = n;
    string postfix = "B";
    if (n >= tera)
    {
        value = static_cast<double>(n)/static_cast<double>(tera);
        postfix = "TiB";
    }
    else if (n >= giga)
    {
        value = static_cast<double>(n)/static_cast<double>(giga);
        postfix = "GiB";
    }
    else if (n >= mega)
    {
        value = static_cast<double>(n)/static_cast<double>(mega);
        postfix = "MiB";
    }
    else if (n >= killo)
    {
        value = static_cast<double>(n)/static_cast<double>(killo);
        postfix = "KiB";
    }
    char buffer[dxrt::CHARBUFFER_SIZE];
    snprintf(buffer, dxrt::CHARBUFFER_SIZE, "%.3g", value);
    return string(buffer)+postfix;
}
static string insert_comma(const string& str)
{
    int str_len = str.length();
    string ret = "";
    ret.reserve(str_len * 1.5);
    for (int i = 0; i < str_len; i++)
    {
        ret.push_back(str[i]);
        int divisor = str_len - i;
        if ((divisor > 1) && (divisor % 3 == 1))
            ret.push_back(',');
    }
    return ret;
}

namespace dxrt {

DeviceStatus::DeviceStatus(int id, dxrt_device_info_t info, dxrt_device_status_t status, dxrt_dev_info_t devInfo)
:_id(id), _info(info), _status(status), _devInfo(devInfo)
{
}
DeviceStatus DeviceStatus::GetCurrentStatus(std::shared_ptr<DeviceTaskLayer> device)
{
    return GetCurrentStatus(device->core());
}
DeviceStatus DeviceStatus::GetCurrentStatus(std::shared_ptr<DeviceCore> device)
{
    int deviceId = device->id();
    auto info = device->info();
    auto status = device->Status();
    auto devInfo = device->devInfo();
    return DeviceStatus(deviceId, info, status, devInfo);
}
DeviceStatus DeviceStatus::GetCurrentStatus(std::shared_ptr<Device> device)
{
    return device->GetCurrentStatus();
}
DeviceStatus DeviceStatus::GetCurrentStatus(int id)
{

    if (GetDeviceCount() <= id)
    {
        throw dxrt::InvalidArgumentException("Not exist device id:"+ std::to_string(id));
    }
    return GetCurrentStatus(DevicePool::GetInstance().GetDeviceCores(id));
}

int DeviceStatus::GetDeviceCount()
{
    return DevicePool::GetInstance().GetDeviceCount();
}

string DeviceStatus::DdrStatusStr(int ch) const
{
    char buf[CHARBUFFER_SIZE];
    uint32_t rm_1, rm_0 = 0, derate = 0;

    switch(_status.ddr_status[ch]) {
		case 0x01 : rm_1 = 8;             break;
		case 0x02 : rm_1 = 6;             break;
		case 0x03 : rm_1 = 4;             break;
		case 0x04 : rm_1 = 3; rm_0 = 3;   break;
		case 0x05 : rm_1 = 2; rm_0 = 5;   break;
		case 0x06 : rm_1 = 2;             break;
		case 0x07 : rm_1 = 1; rm_0 = 7;   break;
		case 0x08 : rm_1 = 1; rm_0 = 3;   break;
		case 0x09 : rm_1 = 1; rm_0 = 0;   break;
		case 0x0A : rm_1 = 0; rm_0 = 7;   break;
		case 0x0B : rm_1 = 0; rm_0 = 5;   break;
		case 0x0C : rm_1 = 0; rm_0 = 25;  break;
		case 0x0D : rm_1 = 0; rm_0 = 25;  derate = 1; break;
		case 0x0E : rm_1 = 0; rm_0 = 125; break;
		case 0x0F : rm_1 = 0; rm_0 = 125; derate = 1; break;
		default   : rm_1 = 0xF; break;
    }
    snprintf(buf, CHARBUFFER_SIZE, "LPDDR CH[%d]: RM: 0x%x(%u.%ux)%s",
            ch, _status.ddr_status[ch], rm_1, rm_0, (derate ? " with de-rating" : ""));
    return string(buf);
}

string DeviceStatus::DdrBitErrStr(void) const
{
    char buf[CHARBUFFER_SIZE];
    snprintf(buf, CHARBUFFER_SIZE, "SBE[%u, %u, %u, %u] DBE[%u, %u, %u, %u]",
        _status.ddr_sbe_cnt[0], _status.ddr_sbe_cnt[1], _status.ddr_sbe_cnt[2], _status.ddr_sbe_cnt[3],
        _status.ddr_dbe_cnt[0], _status.ddr_dbe_cnt[1], _status.ddr_dbe_cnt[2], _status.ddr_dbe_cnt[3]);
    return string(buf);
}

string DeviceStatus::NpuStatusStr(int no) const
{
    char buf[CHARBUFFER_SIZE];
    snprintf(buf, CHARBUFFER_SIZE, "NPU %d: voltage %u mV, clock %u MHz, temperature %d'C",
        no, _status.voltage[no], _status.clock[no], static_cast<int32_t>(_status.temperature[no]));
    return string(buf);
}

string DeviceStatus::DeviceTypeStr() const
{
    return map_lookup(device_types, _info.type);
}
string DeviceStatus::DeviceTypeWord() const
{
    return map_lookup(device_type_words, _info.type);
}
string DeviceStatus::DeviceVariantStr() const
{
    return map_lookup(device_variants, _info.variant);
}
string DeviceStatus::BoardTypeStr() const
{
    return map_lookup(board_types, _info.bd_type);
}
string DeviceStatus::MemoryTypeStr() const
{
    return map_lookup(memory_types, _info.ddr_type);
}
string DeviceStatus::MemorySizeStrBinaryPrefix() const
{
    return convert_capacity(_info.mem_size);
}
string DeviceStatus::MemorySizeStrWithComma() const
{
    return insert_comma(std::to_string(_info.mem_size))+"Byte";
}

string DeviceStatus::AllMemoryInfoStr() const
{
     char buffer[CHARBUFFER_SIZE];
     snprintf(buffer, CHARBUFFER_SIZE, "Type:%s, Addr:%p, size: %s(%s), clock: %udMHz",
      MemoryTypeStr().c_str(), reinterpret_cast<void*>(_info.mem_addr),
      MemorySizeStrBinaryPrefix().c_str(), MemorySizeStrWithComma().c_str(), _info.ddr_freq);
     return string(buffer);
}

string DeviceStatus::PcieInfoStr(int spd, int wd, int bus, int dev, int func) const
{
    char buf[64];
    snprintf(buf, sizeof(buf), "Gen%d X%d [%02d:%02d:%02d]", spd, wd, bus, dev, func);
    return string(buf);
}

#define FW_VERSION_SUPPORT_SUFFIX 230

std::ostream& DeviceStatus::InfoToStream(std::ostream& os) const
{
    os << "=======================================================" << endl;
    os << std::showbase << std::dec << " * Device " << GetId()
      << ": " << DeviceVariantStr()<< ", "<< DeviceTypeWord() <<" type" << endl;
    os << "---------------------   Version   ---------------------" << endl;
    os << " * RT Driver version   : v" << GetDrvVersionFromRT(_devInfo.rt_drv_ver) << endl;
    if (_info.type == static_cast<uint32_t>(DeviceType::ACC_TYPE))
    {
        os << " * PCIe Driver version : v" << GetDrvVersionWithDot(_devInfo.pcie.driver_version) << endl;
    }
    os << "-------------------------------------------------------" << endl;
    if (_info.fw_ver >= FW_VERSION_SUPPORT_SUFFIX || _info.fw_ver == 216) {
        os << " * FW version          : v"<< GetFWVersionFromDeviceInfo(_info.fw_ver, _info.fw_ver_suffix) << endl;
    } else {
        os << " * FW version          : v"<< GetFwVersionWithDot(_info.fw_ver) << endl;
    }
    os << "--------------------- Device Info ---------------------" << endl;
    os << " * Memory : " << MemoryTypeStr() << " " << _info.ddr_freq <<" Mbps, "
      << MemorySizeStrBinaryPrefix() << endl;
    os << " * Board  : "<< BoardTypeStr();
    os << std::fixed << std::setprecision(1) << ", Rev " << static_cast<double>(Info().bd_rev)/10.0 << endl;
    os << " * Chip Offset : " << _info.chip_offset << endl;
    if (_info.type == static_cast<uint32_t>(DeviceType::ACC_TYPE))
    {
        os << " * PCIe   : "<<
            PcieInfoStr(_devInfo.pcie.speed,
                _devInfo.pcie.width,
                _devInfo.pcie.bus,
                _devInfo.pcie.dev,
                _devInfo.pcie.func) << endl;
    }
    return os;
}

string DeviceStatus::GetInfoString() const
{
    std::ostringstream os;
    InfoToStream(os);
    return os.str();
}

std::ostream&DeviceStatus::StatusToStream(std::ostream& os) const
{
    os << std::showbase << std::dec;
    for (int i = 0; i < static_cast<int>(Info().num_dma_ch); i++)
    {
        os << NpuStatusStr(i)<< endl;
    }
    os << "=======================================================" << endl;
    return os;
}


std::ostream&DeviceStatus::DebugStatusToStream(std::ostream& os) const
{
    for (int i = 0; i < 4; i++)
    {
        os << DdrStatusStr(i) << endl;
    }
    os << DdrBitErrStr() << endl;
    os << "=======================================================" << endl;
    return os;
}

string DeviceStatus::GetStatusString() const
{
    std::ostringstream os;
    StatusToStream(os);
    return os.str();
}

std::ostream& operator<<(std::ostream& os, const DeviceStatus& d)
{
    d.InfoToStream(os);
    os << endl;
    d.StatusToStream(os);
    return os;
}


uint32_t DeviceStatus::Voltage(int ch) const
{
    if ((ch < 0) || (ch >= static_cast<int>(_info.num_dma_ch)))
    {
        return 0;
    }
    return _status.voltage[ch];
}
uint32_t DeviceStatus::NpuClock(int ch) const
{
    if ((ch < 0) || (ch >= static_cast<int>(_info.num_dma_ch)))
    {
        return 0;
    }
    return _status.clock[ch];
}

int DeviceStatus::Temperature(int ch) const
{
    if ((ch < 0) || (ch >= static_cast<int>(_info.num_dma_ch)))
    {
        return 0;
    }
    return _status.temperature[ch];
}



}  // namespace dxrt
