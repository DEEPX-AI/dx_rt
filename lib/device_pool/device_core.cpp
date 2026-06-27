/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/device_core.h"

#include <cstring>
#include <iostream>
#include <vector>

#include "dxrt/device_struct.h"
#include "dxrt/device_version.h"
#include "dxrt/driver.h"
#include "dxrt/filesys_support.h"
#include "dxrt/fw.h"
#include "dxrt/safe_cast.h"
#include "dxrt/util.h"
#include "dxrt/exception/exception.h"
#include "../resource/log_messages.h"
#include "../data/ppcpu.h"
#include "dxrt/fixed_size_buffer.h"

using std::endl;

#ifdef USE_VNPU
namespace {

struct FirmwareDmaInfo {
    dxrt::dxrt_meminfo_t meminfo;
    std::unique_ptr<dxrt::FixedSizeBuffer> dmaBuffer;
};

static FirmwareDmaInfo makeFirmwareDmaInfo(
    uint64_t baseAddr, uint64_t memOffset, size_t fwSize, int devId)
{
    auto buf = std::make_unique<dxrt::FixedSizeBuffer>(fwSize, 1, dxrt::BufferAllocType::CMA_DMA);
    void* vaddr = buf->getBuffer();
    uint64_t paddr = buf->getPhysicalAddress(vaddr);
    DXRT_ASSERT(vaddr != nullptr && paddr != 0,
        "Failed to allocate CMA buffer for PPCPU firmware");
    memcpy(vaddr, dxrt::PPCPUDataLoader::GetData(), fwSize);
    buf->flushCache(vaddr, fwSize, false);
    dxrt::dxrt_meminfo_t fw{};
    fw.base   = baseAddr;
    fw.offset = memOffset;
    fw.size   = static_cast<uint32_t>(fwSize);
    fw.data   = paddr;
    LOG_DXRT << "Device " << devId << " Writing PPCPU firmware: vaddr=0x" << std::hex << vaddr
             << ", paddr=0x" << paddr
             << ", base=0x" << fw.base << ", offset=0x" << fw.offset
             << ", size=" << std::dec << fw.size << std::endl;
    return {fw, std::move(buf)};
}

}  // namespace
#endif  // USE_VNPU

namespace dxrt {

DeviceCore::DeviceCore(int id, std::unique_ptr<DriverAdapter> adapter)
: _id(id), _adapter(std::move(adapter)), _name(_adapter->GetName())
{
}

int DeviceCore::Process(dxrt_cmd_t cmd, void *data, uint32_t size, uint32_t sub_cmd, uint64_t address)
{
#ifdef __linux__
  #if DXRT_USB_NETWORK_DRIVER
    return _adapter->NetControl(cmd, data, size, sub_cmd, address);
  #else
    std::ignore = address;
    int ret = _adapter->IOControl(cmd, data, size, sub_cmd);
    if (ret < 0) ret = errno*(-1);
    return ret;
  #endif
#else
    return _adapter->IOControl(cmd, data, size, sub_cmd);
#endif
}

dxrt_device_status_t DeviceCore::Status()
{
    _status = dxrt_device_status_t{};
    Process(dxrt::dxrt_cmd_t::DXRT_CMD_GET_STATUS, &_status);
    return _status;
}

int DeviceCore::Write(const dxrt_meminfo_t &meminfo)
{
#if DXRT_USB_NETWORK_DRIVER == 0
    int ch = _writeChannel.load();
    constexpr int max_ch = 2;
    _writeChannel.store((ch + 1) % max_ch);
    return Write(meminfo, ch);
#else
    {
        net_control_info info;
        info.address = meminfo.base + meminfo.offset;
        info.size = meminfo.size;
        info.type = 2;
        _driverAdapter->Write(&info, sizeof(info));
        _driverAdapter->Write(SafeCast::IntegerToPointer<void*>(meminfo.data), meminfo.size);
    }
    return 0;
#endif
}
int DeviceCore::Write(const dxrt_meminfo_t &meminfo, int ch)
{
    LOG_DXRT_DBG << "Device " << _id << " Write : " << meminfo << endl;
    int ret = 0;
    DXRT_ASSERT(meminfo.base + meminfo.offset != 0, "DeviceCore Write ZERO NPU MEMORY ADDRESS");
    DXRT_ASSERT(meminfo.data != 0, "DeviceCore Write ZERO CPU MEMORY ADDRESS");
    DXRT_ASSERT(ch < 4, "DeviceCore Write CHANNEL INVALID");
    DXRT_ASSERT(meminfo.offset != (std::numeric_limits<uint32_t>::max)(), "DeviceCore Write to ERROR offset");
    // Profiler::GetInstance().Start("Write");
#if DXRT_USB_NETWORK_DRIVER == 0
    BeginDmaIoctl();
    dxrt_req_meminfo_t mem_info_req;
    mem_info_req.data = meminfo.data;
    mem_info_req.base = meminfo.base;
    mem_info_req.offset = meminfo.offset;
    mem_info_req.size = meminfo.size;
    mem_info_req.ch = ch;

    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_MEM, static_cast<void*>(&mem_info_req));

    EndDmaIoctl();
#else
    ignore = ch;
    ret = _driverAdapter->NetControl(
        DXRT_CMD_WRITE_MEM,
        SafeCast::IntegerToPointer<void*>(meminfo.data),
        meminfo.size,
        0,
        meminfo.base + meminfo.offset);
#endif
    if (ret < 0)
    {
        return ret;
    }
    return 0;
}

int DeviceCore::Read(const dxrt_meminfo_t &meminfo)
{
    int ch = _readChannel.load();
    constexpr int max_ch = 3;
    _readChannel.store((ch + 1) % max_ch);
    return Read(meminfo, ch);
}

int DeviceCore::Read(const dxrt_meminfo_t &meminfo, int ch, bool ctrlCmd)
{
    LOG_DXRT_DBG << "Device " << _id << " Read : " << meminfo << endl;
    int ret = 0;
    DXRT_ASSERT(meminfo.base + meminfo.offset != 0, "DeviceCore Read ZERO NPU MEMORY ADDRESS");
    DXRT_ASSERT(meminfo.data != 0, "DeviceCore Read ZERO CPU MEMORY ADDRESS");
    DXRT_ASSERT(ch < 4, "DeviceCore Read CHANNEL INVALID");
    DXRT_ASSERT(meminfo.offset != (std::numeric_limits<uint32_t>::max)(), "DeviceCore Read to ERROR offset");
#if DXRT_USB_NETWORK_DRIVER == 0
    BeginDmaIoctl();
    dxrt_req_meminfo_t mem_info_req;
    mem_info_req.data = meminfo.data;
    mem_info_req.base = meminfo.base;
    mem_info_req.offset = meminfo.offset;
    mem_info_req.size = meminfo.size;
    mem_info_req.ch = ch;


    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_READ_MEM, static_cast<void*>(&mem_info_req));

    EndDmaIoctl();
    std::ignore = ctrlCmd;
#else
    std::ignore = ch;
    ret = _driverAdapter->NetControl(
        DXRT_CMD_READ_MEM,
        SafeCast::IntegerToPointer<void*>(meminfo.data),
        meminfo.size,
        0,
        meminfo.base + meminfo.offset,
        ctrlCmd);
#endif

    if (ret < 0)
    {
        return ret;
    }
    return 0;
}

int DeviceCore::Wait(void)
{
    LOG_DXRT_DBG << "Device " << _id << " Wait" << endl;
    int ret = 0;
#ifdef __linux__
    ret = _adapter->Poll();
    LOG_DXRT_DBG << "Device " << _id << " Wakeup" << endl;
    if (ret < 0)
    {
        LOG_DXRT << "Error: Device " << _id << "poll fail." << endl;
        return -1;
    }
#elif _WIN32
    ret = _adapter->Poll();  // unused in windows
#endif
    std::ignore = ret;
    return 0;
}

void DeviceCore::PauseDMA()
{
    std::unique_lock<std::mutex> lock(_dmaPauseMutex);
    _dmaPaused = true;
    _dmaPauseCv.wait(lock, [this]() { return _inflightDmaIoctl == 0; });
}

void DeviceCore::ResumeDMA()
{
    {
        std::lock_guard<std::mutex> lock(_dmaPauseMutex);
        _dmaPaused = false;
    }
    _dmaPauseCv.notify_all();
}

void DeviceCore::BeginDmaIoctl()
{
    std::unique_lock<std::mutex> lock(_dmaPauseMutex);
    _dmaPauseCv.wait(lock, [this]() { return !_dmaPaused; });
    ++_inflightDmaIoctl;
}

void DeviceCore::EndDmaIoctl()
{
    std::lock_guard<std::mutex> lock(_dmaPauseMutex);
    DXRT_ASSERT(_inflightDmaIoctl > 0, "DeviceCore DMA inflight counter underflow");
    --_inflightDmaIoctl;
    if (_inflightDmaIoctl == 0)
    {
        _dmaPauseCv.notify_all();
    }
}


int DeviceCore::Poll()
{
    return _adapter->Poll();
}


void DeviceCore::Identify(int id_, uint32_t subCmd)
{
    LOG_DXRT_DBG << "Device " << _id << " Identify" << endl;
    int ret;

#if DXRT_USB_NETWORK_DRIVER == 0
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, static_cast<void*>(&_info), 0, subCmd);
#else
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, static_cast<void*>(&_info), sizeof(_info), subCmd, true);
#endif
    if (ret != 0)
    {
        LOG_DXRT_DBG << "failed to identify device " << id_ <<", ret=" << ret << endl;
        //_isBlocked = true;
        throw DeviceIOException(EXCEPTION_MESSAGE(LogMessages::Device_FailToInitialize(id_)));
    }

    // Version Check

    #ifdef __linux__
            DxDeviceVersion dxVer(this, _info.fw_ver, _info.type, _info.interface, _info.variant);
    #elif _WIN32
            DxDeviceVersion dxVer(this, _info.fw_ver, _info.type, _info.interface_value, _info.variant);
    #endif
            _devInfo = dxVer.GetVersion();




    LOG_DXRT_DBG << _name << ": device info : type " << _info.type
        << std::hex << ", variant " << _info.variant
        << ", mem_addr " << _info.mem_addr
        << ", mem_size " << _info.mem_size
        << std::dec << ", num_dma_ch " << _info.num_dma_ch << endl;
    DXRT_ASSERT(_info.mem_size > 0, "invalid device memory size");



    LOG_DXRT_DBG << "    Device " << _id << ": " << _info << endl;
}

void DeviceCore::Reset(int opt)
{
    Process(dxrt::dxrt_cmd_t::DXRT_CMD_RESET, &opt, sizeof(int));
}



void DeviceCore::DoPcieCommand(void *data, uint32_t subCmd, uint32_t size)
{
    auto sCmd = static_cast<dxrt_pcie_sub_cmd_t>(subCmd);
    if (data == nullptr)
    {
        LOG_DXRT_ERR("Null data pointer received");
        return;
    }
    switch (sCmd) {
        case DX_GET_PCIE_INFO:
        {
            auto *info = static_cast<dxrt_pcie_info_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_PCIE,
                    info,
                    sizeof(dxrt_pcie_info_t),
                    sCmd);
            break;
        }
        case DX_CLEAR_ERR_STAT:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_PCIE,
                    data,
                    size,
                    sCmd);
            break;
        }
        default:
        {
            LOG_DXRT_ERR("Unknown Command");
            break;
        }
    }
}


void DeviceCore::DoCustomCommand(void *data, uint32_t subCmd, uint32_t size)
{
    auto sCmd = static_cast<dxrt_custom_sub_cmt_t>(subCmd);
    if (data == nullptr)
    {
        LOG_DXRT_ERR("Null data pointer received");
        return;
    }

    switch (sCmd)
    {
        case DX_SET_DDR_FREQ:
        {
            uint32_t freq = *static_cast<uint32_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    &freq,
                    sizeof(uint32_t),
                    sCmd);
            break;
        }
        case DX_GET_OTP:
        {
            auto *info = static_cast<otp_info_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    info,
                    sizeof(otp_info_t),
                    sCmd);
            break;
        }
        case DX_SET_OTP:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    size,
                    sCmd);
            break;
        }
        case DX_SET_LED:
        {
            uint32_t ledVal = *static_cast<uint32_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    &ledVal,
                    sizeof(uint32_t),
                    sCmd);
            break;
        }
        case DX_UPLOAD_MODEL:
        {
            auto *model_info = static_cast<uint32_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    model_info,
                    sizeof(uint32_t)*3,
                    sCmd);
            break;
        }
        case DX_INTERNAL_TESTCASE:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    size,
                    sCmd);
            break;
        }
        case DX_GET_FCT_TESTCASE_RESULT:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    size,
                    sCmd);
            break;
        }
        case DX_RUN_FCT_TESTCASE:
        {
            uint32_t type = *static_cast<uint32_t *>(data);
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    &type,
                    sizeof(uint32_t),
                    sCmd);
            break;
        }
        case DX_INIT_PPCPU:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    size,
                    sCmd);
            break;
        }
        case DX_ADD_WEIGHT_INFO:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    sizeof(dxrt_custom_weight_info_t),
                    sCmd);
            break;
        }
        case DX_DEL_WEIGHT_INFO:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    sizeof(dxrt_custom_weight_info_t),
                    sCmd);
            break;
        }
        default:
            LOG_DXRT_ERR("Unknown sub command: " << sCmd);
            break;
    }
}

void DeviceCore::InitPPCPU(uint64_t mem_offset)
{
    size_t fw_size = PPCPUDataLoader::GetDataSize();

#ifndef USE_VNPU
    void* fw_raw = PPCPUDataLoader::GetData();
    dxrt_meminfo_t fw_meminfo;
    fw_meminfo.base   = _info.mem_addr;
    fw_meminfo.offset = static_cast<uint32_t>(mem_offset);
    fw_meminfo.size   = static_cast<uint32_t>(fw_size);
    fw_meminfo.data   = SafeCast::PointerToInteger<void*>(fw_raw);
#else
    auto firmwareDma = makeFirmwareDmaInfo(_info.mem_addr, mem_offset, fw_size, _id);
    const dxrt_meminfo_t& fw_meminfo = firmwareDma.meminfo;
#endif  // USE_VNPU

    // DeviceCore is a low-level driver-facing layer and does not depend on ServiceLayer abstractions.
    // Use Write() directly instead of DMAWrite() here.
    int ret = Write(fw_meminfo);
    DXRT_ASSERT(ret == 0, "Failed to load PPCPU firmware to device: ret=" + std::to_string(ret));

    dxrt_req_meminfo_t meminfo_req;
    meminfo_req.base   = fw_meminfo.base;
    meminfo_req.offset = fw_meminfo.offset;
    meminfo_req.size   = fw_meminfo.size;
    meminfo_req.data   = fw_meminfo.data;
    meminfo_req.ch     = 0;

    DoCustomCommand(&meminfo_req, dxrt_custom_sub_cmt_t::DX_INIT_PPCPU, sizeof(dxrt_req_meminfo_t));

#ifndef USE_VNPU
    // Integrity check: read back firmware and verify it matches
    std::vector<uint8_t> readBuf(fw_size, 0);
    dxrt_meminfo_t check_meminfo = fw_meminfo;
    check_meminfo.data = SafeCast::PointerToInteger<void*>(readBuf.data());
    int retCheck = Read(check_meminfo);  // directly call Read instead of DMARead because this is low layer class
    DXRT_ASSERT(retCheck == 0,
        "Failed to read back PPCPU firmware from device " + std::to_string(_id) + ": ret=" + std::to_string(retCheck));
    DXRT_ASSERT(memcmp(fw_raw, readBuf.data(), fw_size) == 0,
        "PPCPU firmware data mismatch on device " + std::to_string(_id));
#endif  // USE_VNPU

    LOG_DXRT_S << "PPCPU firmware loaded to device " << _id << " successfully." << std::endl;
    // CMA buffer automatically released via RAII when unique_ptr goes out of scope
}

void DeviceCore::ShowPCIEDetails(std::ostream& os)
{

#ifndef USE_VNPU
    // check fw version
    static constexpr int MIN_PCIE_VERSION = 1700;
    static constexpr int MIN_FW_VERSION = 211;
    bool unsupportedVersion = false;
    if (_devInfo.rt_drv_ver.driver_version < MIN_PCIE_VERSION)
    {
        os << "Device " << id() << ":PCIE status is not supported due to low RT driver version "<< endl
         << LogMessages::NotSupported_DeviceDriverVersion(_devInfo.rt_drv_ver.driver_version, MIN_PCIE_VERSION) << endl;
        unsupportedVersion = true;
    }
    if (_info.fw_ver < MIN_FW_VERSION)
    {
        os << "Device " << id() << ":PCIE status is not supported due to low fw version" << endl
         << LogMessages::NotSupported_FirmwareVersion(_info.fw_ver, MIN_FW_VERSION) << endl;
        unsupportedVersion = true;
    }
    if (unsupportedVersion == true)
    {
        return;
    }
#endif

    dxrt_pcie_info_t pcieInfo;
    memset(&pcieInfo, 0, sizeof(dxrt_pcie_info_t));
    DoPcieCommand(&pcieInfo, DX_GET_PCIE_INFO, sizeof(dxrt_pcie_info_t));
    os << "Device "<< id() << " pcie status:" << endl;

    dxrt_device_status_t status_data = Status();

    os << "DDR Memory Error information";
    for (int i = 0; i < 4; i++)
    {
        os << " ch" << i << ":";
        uint32_t sbe = status_data.ddr_sbe_cnt[i];
        uint32_t dbe = status_data.ddr_dbe_cnt[i];
#if 1
        if ((sbe == 0) && (dbe == 0))
        {
            os << "None";
        }
        else
#endif
        {
            os << "SBE " << sbe
              << ",DBE " << dbe;
        }
    }
    os << endl;
}
void DeviceCore::ShowPCIEDetails()
{
    ShowPCIEDetails(std::cout);
}
DeviceType DeviceCore::GetDeviceType() const
{
    return static_cast<DeviceType>(_info.type);
}

void DeviceCore::StartDev(uint32_t option)
{
    std::ignore = option;

    uint32_t start = 1;
    Process(dxrt::dxrt_cmd_t::DXRT_CMD_START, &start, sizeof(start));
    unblock();
}


void DeviceCore::BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp)
{


    int ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_SCHEDULE, static_cast<void*>(&boundOp), sizeof(dxrt_sche_sub_cmd_t), subCmd);
    DXRT_ASSERT(ret == 0, "failed to apply bound option to device");

}

int DeviceCore::UpdateFwConfig(const std::string& jsonFile)
{
    DXRT_ASSERT(fileExists(jsonFile), jsonFile + " doesn't exist.");
    std::vector<uint8_t> buf(dxrt::getFileSize(jsonFile));
    DataFromFile(jsonFile, buf.data());
    Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_CONFIG_JSON, buf.data(), static_cast<uint32_t>(buf.size()));
    return buf[0];
}

int DeviceCore::ReadDriverData(void *ptr, uint32_t size)
{
    int ret = _adapter->Read(ptr, size);

    return ret;
}

void* DeviceCore::CreateMemoryMap()
{
    void* mem_ptr = _adapter->MemoryMap(nullptr, _info.mem_size, 0);
    int64_t mem_ptr_int;
    memcpy(&mem_ptr_int, &mem_ptr, sizeof(void*));
    if (mem_ptr_int == -1)
    {
        mem_ptr = nullptr;
    }
    return mem_ptr;
}
void DeviceCore::CheckVersion()
{

#ifndef USE_VNPU
    #ifdef __linux__
        DxDeviceVersion dxVer(this, _info.fw_ver, _info.type, _info.interface, _info.variant);
    #elif _WIN32
        DxDeviceVersion dxVer(this, _info.fw_ver, _info.type, _info.interface_value, _info.variant);
    #endif
    dxVer.CheckVersion();
#else
    std::ignore = _info;
#endif // USE_VNPU
}

int DeviceCore::GetReadChannel() const
{
    if (static_cast<DeviceType>(_info.type) == DeviceType::ACC_TYPE)
    {
        return 4;
    }
    else if (static_cast<DeviceType>(_info.type) == DeviceType::STD_TYPE)
    {
        return 1;
    }
    else
    {
        DXRT_ASSERT(false, "UNKNOWN device type");
        return 0;
    }
}

int DeviceCore::GetWriteChannel() const
{
    if (static_cast<DeviceType>(_info.type) == DeviceType::ACC_TYPE)
    {
        //check for version
        auto driver_version = _devInfo.rt_drv_ver.driver_version;
        auto pcie_version = _devInfo.pcie.driver_version;

        if ((driver_version >= RT_DRIVER_WRITE_CHANNEL_CHANGE_VERSION) &&
            (pcie_version >= PCIE_DRIVER_WRITE_CHANNEL_CHANGE_VERSION))
        {
            return 2;
        }
        else
        {
            return 3;
        }
    }
    else if (static_cast<DeviceType>(_info.type) == DeviceType::STD_TYPE)
    {
        return 1;
    }
    else
    {
        DXRT_ASSERT(false, "UNKNOWN device type");
        return 0;
    }
}
void DeviceCore::Close()
{
    _adapter->Close();
}

} // namespace dxrt
