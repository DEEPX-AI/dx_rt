/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_util.h"
#include "dxrt/device_struct_operators.h"
#include "dxrt/map_lookup_template.h"

#include <map>
#ifdef __linux__
    #include <linux/ioctl.h>
#elif _WIN32
    #include <windows.h>
#endif
#include<sstream>
#include <iomanip>

using std::string;
using std::vector;
using std::setfill;
using std::setw;
using std::hex;
using std::dec;
using std::showbase;
using std::endl;
using std::pair;


namespace dxrt {

DXRT_API vector<pair<int,string>> ioctlTable = {
    { dxrt::dxrt_ioctl_t::DXRT_IOCTL_MESSAGE, "IOCTL_MESSAGE" },
    { dxrt::dxrt_ioctl_t::DXRT_IOCTL_DUMMY, "IOCTL_DUMMY" },
};
static constexpr std::array<pair_type, 9> errTable = {{
    {dxrt::dxrt_error_t::ERR_NPU0_HANG, "NPU0 Hang"},
    {dxrt::dxrt_error_t::ERR_NPU1_HANG, "NPU1 Hang"},
    {dxrt::dxrt_error_t::ERR_NPU2_HANG, "NPU2 Hang"},
    {dxrt::dxrt_error_t::ERR_NPU_BUS, "NPU BUS Error"},
    {dxrt::dxrt_error_t::ERR_PCIE_DMA_CH0_FAIL, "PCIe-DMA Fail in ch0"},
    {dxrt::dxrt_error_t::ERR_PCIE_DMA_CH1_FAIL, "PCIe-DMA Fail in ch1"},
    {dxrt::dxrt_error_t::ERR_PCIE_DMA_CH2_FAIL, "PCIe-DMA Fail in ch2"},
    {dxrt::dxrt_error_t::ERR_LPDDR_DED_WR, "LPDDR Link-ECC Write Error"},
    {dxrt::dxrt_error_t::ERR_LPDDR_DED_RD, "LPDDR Link-ECC Read Error"},
}};

std::ostream& operator<<(std::ostream& os, const dx_pcie_dev_err_t& error) {
    auto formatPcieBDF = [](int bus, int dev, int func) {
        std::ostringstream ss;
        ss << setfill('0') << setw(2) << hex << bus << ":"
           << setfill('0') << setw(2) << hex << dev << ":"
           << setfill('0') << setw(2) << hex << func;
        return ss.str();
    };
    string pcieBDF = formatPcieBDF(error.bus, error.dev, error.func);

    os << "\n==========================================================================================" << endl;
    os << "* Error Code       : " << map_lookup(errTable, error.err_code) << endl;
    os << "* NPU ID           : " << error.npu_id << endl;
    os << "* Rt drv version   : v" << GetDrvVersionWithDot(error.rt_driver_version) << endl;
    os << "* Pcie drv version : v" << GetDrvVersionWithDot(error.pcie_driver_version) << endl;
    os << "* Firmware version : v" << GetFWVersionFromDeviceInfo(error.fw_ver, error.fw_version_suffix) << endl;
    os << "------------------------------------------------------------------------------------------" << endl;

    // Print base addresses
    os << hex;
    os << "* Base Addresses" << endl;
    os << "  - AXI            : 0x" << error.base_axi << endl;
    os << "  - RMAP           : 0x" << error.base_rmap << endl;
    os << "  - WEIGHT         : 0x" << error.base_weight << endl;
    os << "  - IN             : 0x" << error.base_in << endl;
    os << "  - OUT            : 0x" << error.base_out << endl;
    os << dec;

    os << "------------------------------------------------------------------------------------------" << endl;

    // Print NPU debug information
    os << "* NPU Debug Information" << endl;
    os << "  - Cmd Num        : " << error.cmd_num << endl;
    os << "  - Last Cmd Num   : " << error.last_cmd << endl;
    os << "  - Abnormal Cnt   : " << error.abnormal_cnt << endl;
    os << "  - IRQ Status     : " << error.irq_status << endl;
    os << "  - Busy           : " << (error.busy ? "Yes" : "No") << endl;

    os << "------------------------------------------------------------------------------------------" << endl;

    // Print device information
    os << "* Device Information" << endl;
    os << "  - NPU DMA Status : " << error.dma_err << endl;
    os << "  - Temperature    : ";

    for (const auto& temp : error.temperature) {
        if (temp > 10000) break;
        //@no_else: guard_clause
        os << temp << " ";
    }
    os << endl;
    os << "  - Voltage(mV)    : ["
       << error.npu_voltage[0] << ", "
       << error.npu_voltage[1] << ", "
       << error.npu_voltage[2] << "]" << endl;

    os << "  - Frequency(MHz) : ["
       << error.npu_freq[0] << ", "
       << error.npu_freq[1] << ", "
       << error.npu_freq[2] << "]" << endl;

    os << "------------------------------------------------------------------------------------------" << endl;

    // Print PCIe information
    os << "* PCIe Information (" << "Gen" << error.speed << " X" << error.width << ", " << pcieBDF << ")" << endl;
    os << "  - LTSSM State    : " << error.ltssm << endl;
    os << "==========================================================================================" << endl;

    // Print LPDDR information
    os << "* LPDDR Information (" << "LPDDR" << error.ddr_type << " , Frequency: " << error.ddr_freq << "MHz)" << endl;
    os << "  - LPDDR MR Register Info ch[0, 1, 2, 3] : [";
    for (int ddr_ch = 0; ddr_ch < 4; ddr_ch++) {
        os << error.ddr_mr_reg[ddr_ch] << ", ";
    }
    os << "]" << endl;

    if (error.dbe_cnt[0] || error.dbe_cnt[1] || error.dbe_cnt[2] || error.dbe_cnt[3]) {
        os << "  - LPDDR double bit error count ch[0, 1, 2, 3] : [";
        for (int ddr_ch = 0; ddr_ch < 4; ddr_ch++) {
            os << error.dbe_cnt[ddr_ch] << ", ";
        }
        os << "]" << endl;
    }
    //@no_else: error_handling
    os << "==========================================================================================" << endl;

    os << "************************************************************************" << endl;
    os << " * Error occurred! Please follow the steps below to recover the device." << endl;
    os << " * Refer to the user guide if additional help is needed." << endl;
    os << endl;
    os << " Step 1: Reset the device using dxrt-cli" << endl;
    os << "         > dxrt-cli -r 0" << endl;
    os << " Step 2: Retry the inference using run_model" << endl;
    os << "         > run_model -m [model.dxnn]" << endl;
    os << " ** If the error persists, please contact DeepX support for assistance." << endl;
    os << "************************************************************************" << endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const dxrt_error_t& error)
{
    os << ErrTable(error);
    return os;
}
DXRT_API std::string ErrTable(dxrt_error_t error)
{
    return map_lookup(errTable, error);
}
std::ostream& operator<<(std::ostream& os, const dxrt_meminfo_t& meminfo)
{
    os << showbase << hex << meminfo.data << " [" << meminfo.base << " + "
        << meminfo.offset << ", "
        << meminfo.base + meminfo.offset << " ~ "
        << meminfo.base + meminfo.offset + meminfo.size << ", "
        << meminfo.size << "]" << dec;
    return os;
}
std::ostream& operator<<(std::ostream& os, const dxrt_request_t& inf)
{
    os << dec << "[" << inf.req_id << "] "
        << inf.input << " -> "
        << inf.output << ", ["
        << inf.model_type << ", "
        << inf.model_cmds << "] @ ["
        << showbase << hex << inf.cmd_offset << ", "
        << inf.weight_offset << "]" << dec;
    return os;
}
std::ostream& operator<<(std::ostream& os, const dxrt_request_acc_t& inf)
{
    os << dec << "[" << inf.req_id << " -> " << inf.task_id << "] "
        << inf.input << " -> "
        << inf.output << ", ["
        << inf.model_type << "], ["
        << inf.model_cmds << "] @ ["
        << showbase << hex << inf.cmd_offset << ", "
        << inf.weight_offset << "], "
        << dec;
    return os;
}
std::ostream& operator<<(std::ostream& os, const dxrt_response_t& res)
{
    os << dec << "[" << res.req_id << "] "
        << res.inf_time << ", "
        << res.argmax << ", "
        << res.ppu_filter_num << ", "
        << res.status;
    return os;
}
std::ostream& operator<<(std::ostream& os, const dxrt_model_t& model)
{
    os << dec << model.npu_id << ", " << model.type << ", "
        << model.rmap << ", "
        << model.weight << ", "
        << hex << model.input_all_offset << ", "
        << model.input_all_size << ", "
        << hex << model.output_all_offset << ", "
        << model.output_all_size << ", "
        << model.last_output_offset << ", "
        << model.last_output_size << dec;
    return os;
}
std::ostream& operator<<(std::ostream& os, const dxrt_device_info_t& info)
{
    os << showbase << dec
        << "type " << info.type << ", "
        << "var " << info.variant << ", "
        << hex << "addr " << info.mem_addr << ", "
        << "size " << info.mem_size << ", " << dec
        << "dma_ch " << info.num_dma_ch << ", "
        << "fw_ver " << info.fw_ver << ", "
        << "board rev " << info.bd_rev << ", "
        << "board type " << info.bd_type << ", "
        << "ddr freq " << info.ddr_freq << ", "
        << "ddr type " << info.ddr_type << ", "
#ifdef __linux__
        << "interface " << info.interface << ", "
#elif _WIN32
       << "interface " << info.interface_value << ", "
#endif
       << GetFWVersionFromDeviceInfo(info.fw_ver, info.fw_ver_suffix)
       << dec;
    return os;
}
std::ostream& operator<<(std::ostream& os, const dx_pcie_dev_ntfy_throt_t& notify)
{
    if (notify.ntfy_code == NTFY_EMERGENCY_BLOCK || notify.ntfy_code == NTFY_EMERGENCY_RELEASE) {
        os  << "[Emergency] NPU@" << notify.npu_id
            << ":: " << (notify.ntfy_code == NTFY_EMERGENCY_BLOCK ? "BLOCKED" : "RELEASED")
            << " temperature:: " << notify.throt_temper << "\'C";
    }
    else if (notify.ntfy_code == NTFY_EMERGENCY_WARN)
    {
        os  << "[Emergency] NPU@" << notify.npu_id
            << ":: " << "Warning - Temperature has reached the Emergency Point "
            << "(" << notify.throt_temper << ")\'C";
    }
    else
    {
        os << "[Throttling] NPU@" << notify.npu_id
           << " voltage:: " << notify.throt_voltage[0] / 1000 << "mV -> " << notify.throt_voltage[1] / 1000 << "mV"
           << " frequency:: " << notify.throt_freq[0] << "mhz -> " << notify.throt_freq[1] << "mhz"
           << " temperature:: " << notify.throt_temper << "\'C";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const otp_info_t& info)
{
    using std::uppercase;
    using std::left;

    os << "=====================================\n"
        << "              OTP Info              \n"
        << "=====================================\n"
        << uppercase
        << left << setw(20) << "JEP ID:"            << "0x" << hex <<static_cast<int>(info.JEP_ID) << dec << "\n"
        << left << setw(20) << "Continuation Code:" << "0x" << hex << static_cast<int>(info.CONTINUATION_CODE) << dec << "\n"
        << left << setw(20) << "Chip Name:"         << string(info.CHIP_NAME, sizeof(info.CHIP_NAME)) << "\n"
        << left << setw(20) << "Device Revision:"   << string(info.DEVICE_REV, sizeof(info.DEVICE_REV)) << "\n"
        << left << setw(20) << "ECID:"              << "0x" << hex << static_cast<int>(info.ECID) << dec << "\n"
        << left << setw(20) << "Foundry Fab:"       << string(info.FOUNDRY_FAB, sizeof(info.FOUNDRY_FAB)) << "\n"
        << left << setw(20) << "Process:"           << string(info.PROCESS, sizeof(info.PROCESS)) << "\n"
        << left << setw(20) << "Lot ID:"            << string(info.LOT_ID, sizeof(info.LOT_ID)) << "\n"
        << left << setw(20) << "Wafer ID:"          << string(info.WAFER_ID, sizeof(info.WAFER_ID)) << "\n"
        << left << setw(20) << "X Axis:"            << string(info.X_AXIS, sizeof(info.X_AXIS)) << "\n"
        << left << setw(20) << "Y Axis:"            << string(info.Y_AXIS, sizeof(info.Y_AXIS)) << "\n"
        << left << setw(20) << "Test Program:"      << string(info.TEST_PGM, sizeof(info.TEST_PGM)) << "\n"
        << left << setw(20) << "Barcode:"           << string(info.BARCODE, sizeof(info.BARCODE)) << "\n"
        << left << setw(20) << "Barcode index:"     << "0x" << hex << static_cast<int>(info.BARCODE_IDX) << dec << "\n"
        << "=====================================\n";

    return os;
}

std::ostream& operator<<(std::ostream& os, const dxrt_fct_result_t& info)
{
    using std::uppercase;
    using std::left;
    using std::setw;

    os << "=====================================\n"
       << "           FCT Result Info           \n"
       << "=====================================\n";

    for (int i = 0; i < 4; ++i) {
        os << left << setw(20) << "WR Margin[" + std::to_string(i) + "]:" << info.wr_margin[i] << "%\n";
    }

    for (int i = 0; i < 4; ++i) {
        os << left << setw(20) << "RD Margin[" + std::to_string(i) + "]:" << info.rd_margin[i] << "%\n";
    }

    // ddr_margin, ddr_mf, i2c_fail
    os  << left << setw(20) << "DDR Manufacturer:" << static_cast<int>(info.ddr_mf) << "\n"
        << left << setw(20) << "DDR Margin:" << (info.ddr_margin == 1 ? "PASS" : "FAIL") << "\n"
        << left << setw(20) << "I2C Fail:" << (info.i2c_fail == 1 ? "FAIL" : "PASS") << "\n"
        << left << setw(20) << "Test Done:" << static_cast<int>(info.test_done) << "\n";
        // << left << setw(20) << "Test Done:" << (info.test_done == 1 ? "PASS" : "FAIL") << "\n";
    os << "=====================================\n";

    return os;
}

static constexpr std::array<pair_type, 20> dxrt_cmd_map = {{
    {dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, "DXRT_CMD_IDENTIFY_DEVICE"},
    {dxrt_cmd_t::DXRT_CMD_GET_STATUS, "DXRT_CMD_GET_STATUS"},
    {dxrt_cmd_t::DXRT_CMD_RESET, "DXRT_CMD_RESET"},
    {dxrt_cmd_t::DXRT_CMD_UPDATE_CONFIG, "DXRT_CMD_UPDATE_CONFIG"},
    {dxrt_cmd_t::DXRT_CMD_UPDATE_FIRMWARE, "DXRT_CMD_UPDATE_FIRMWARE"},
    {dxrt_cmd_t::DXRT_CMD_GET_LOG, "DXRT_CMD_GET_LOG"},
    {dxrt_cmd_t::DXRT_CMD_DUMP, "DXRT_CMD_DUMP"},
    {dxrt_cmd_t::DXRT_CMD_WRITE_MEM, "DXRT_CMD_WRITE_MEM"},
    {dxrt_cmd_t::DXRT_CMD_READ_MEM, "DXRT_CMD_READ_MEM"},
    {dxrt_cmd_t::DXRT_CMD_CPU_CACHE_FLUSH, "DXRT_CMD_CPU_CACHE_FLUSH"},
    {dxrt_cmd_t::DXRT_CMD_SOC_CUSTOM, "DXRT_CMD_SOC_CUSTOM"},
    {dxrt_cmd_t::DXRT_CMD_WRITE_INPUT_DMA_CH0, "DXRT_CMD_WRITE_INPUT_DMA_CH0"},
    {dxrt_cmd_t::DXRT_CMD_WRITE_INPUT_DMA_CH1, "DXRT_CMD_WRITE_INPUT_DMA_CH1"},
    {dxrt_cmd_t::DXRT_CMD_WRITE_INPUT_DMA_CH2, "DXRT_CMD_WRITE_INPUT_DMA_CH2"},
    {dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH0, "DXRT_CMD_READ_OUTPUT_DMA_CH0"},
    {dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH1, "DXRT_CMD_READ_OUTPUT_DMA_CH1"},
    {dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH2, "DXRT_CMD_READ_OUTPUT_DMA_CH2"},
    {dxrt_cmd_t::DXRT_CMD_TERMINATE, "DXRT_CMD_TERMINATE"},
    {dxrt_cmd_t::DXRT_CMD_EVENT, "DXRT_CMD_EVENT"},
    {dxrt_cmd_t::DXRT_CMD_DRV_INFO, "DXRT_CMD_DRV_INFO"},
}};

DXRT_API std::string dxrt_cmd_t_str(dxrt::dxrt_cmd_t c)
{
    return map_lookup(dxrt_cmd_map, c, "UNKNOWN_DXRT_CMD");
}

} // namespace dxrt
