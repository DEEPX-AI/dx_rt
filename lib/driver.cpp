// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_util.h"

#include <map>
#ifdef __linux__
    #include <linux/ioctl.h>
#elif _WIN32
    #include <windows.h>
#endif
#include<sstream>
#include <iomanip>

using namespace std;

namespace dxrt {

DXRT_API vector<pair<int,string>> ioctlTable = {
    { dxrt::dxrt_ioctl_t::DXRT_IOCTL_MESSAGE, "IOCTL_MESSAGE" },
    { dxrt::dxrt_ioctl_t::DXRT_IOCTL_DUMMY, "IOCTL_DUMMY" },
};
map<int,string> errTable = {
    {dxrt::dxrt_error_t::ERR_NPU0_HANG, "NPU0 Hang"},
    {dxrt::dxrt_error_t::ERR_NPU1_HANG, "NPU1 Hang"},
    {dxrt::dxrt_error_t::ERR_NPU2_HANG, "NPU2 Hang"},
    {dxrt::dxrt_error_t::ERR_NPU_BUS, "NPU BUS Error"},
    {dxrt::dxrt_error_t::ERR_PCIE_DMA_CH0_FAIL, "PCIe-DMA Fail in ch0"},
    {dxrt::dxrt_error_t::ERR_PCIE_DMA_CH1_FAIL, "PCIe-DMA Fail in ch1"},
    {dxrt::dxrt_error_t::ERR_PCIE_DMA_CH2_FAIL, "PCIe-DMA Fail in ch2"},
    {dxrt::dxrt_error_t::ERR_LPDDR_DED_WR, "LPDDR Link-ECC Write Error"},
    {dxrt::dxrt_error_t::ERR_LPDDR_DED_RD, "LPDDR Link-ECC Read Error"},
};

std::ostream& operator<<(std::ostream& os, const dx_pcie_dev_err_t& error) {
    auto formatPcieBDF = [](int bus, int dev, int func) {
        ostringstream ss;
        ss << setfill('0') << setw(2) << hex << bus << ":"
           << setfill('0') << setw(2) << hex << dev << ":"
           << setfill('0') << setw(2) << hex << func;
        return ss.str();
    };
    string pcieBDF = formatPcieBDF(error.bus, error.dev, error.func);

    os << "\n==========================================================================================" << endl;
    os << "* Error Code       : " << errTable[error.err_code] << endl;
    os << "* NPU ID           : " << error.npu_id << endl;
    os << "* Rt drv version   : v" << GetDrvVersionWithDot(error.rt_driver_version) << endl;
    os << "* Pcie drv version : v" << GetDrvVersionWithDot(error.pcie_driver_version) << endl;
    os << "* Firmware version : v" << GetFwVersionWithDot(error.fw_ver) << endl;
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
    os << "  - LPDDR double bit error occured ch : ";
    for (int ddr_ch = 0; ddr_ch < 4; ddr_ch++) {
        if (error.ddr_dbe_ch & (1 << ddr_ch))
            os << "CH" << ddr_ch << ", ";
    }
    os << endl;

    os << "  - LPDDR MR Register Info ch[0, 1, 2, 3] : [";
    for (int ddr_ch = 0; ddr_ch < 4; ddr_ch++) {
        os << error.ddr_mr_reg[ddr_ch] << ", ";
    }
    os << "]" << endl;
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
    os << errTable[error] ;
    return os;
}
DXRT_API std::string ErrTable(dxrt_error_t error)
{
    return errTable[error];
}
std::ostream& operator<<(std::ostream& os, const dxrt_meminfo_t& meminfo)
{
    os << showbase << hex << meminfo.data << " [" << meminfo.base << " + "
        << meminfo.offset << ", "
        << meminfo.base + meminfo.offset << " ~ "
        << meminfo.base + meminfo.offset + meminfo.size << ", "
        << meminfo.size << "]" << dec ;
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
        << inf.weight_offset << "], [" << dec
        << inf.model_cmds2 << "] @ ["
        << showbase << hex << inf.cmd_offset2 << ", "
        << inf.weight_offset2 << "], "
        << inf.arg0 << dec;
    return os;
}
std::ostream& operator<<(std::ostream& os, const dxrt_response_t& res)
{
    os << dec << "[" << res.req_id << "] "        
        << res.inf_time << ", "
        << res.argmax << ", "
        << res.ppu_filter_num << ", "
        << res.status ;        
    return os;
}
std::ostream& operator<<(std::ostream& os, const dxrt_model_t& model)
{
    os << dec << model.npu_id << ", " << model.type << ", "
        << model.cmd << ", "
        << model.weight << ", "
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
        << string(info.fw_info)
        << dec;
    return os;
}
std::ostream& operator<<(std::ostream& os, const dx_pcie_dev_ntfy_throt_t& notify) {
    if (notify.ntfy_code == NTFY_EMERGENCY_BLOCK || notify.ntfy_code == NTFY_EMERGENCY_RELEASE) {
        os  << "[Emergency] NPU@" << notify.npu_id
            << ":: " << (notify.ntfy_code == NTFY_EMERGENCY_BLOCK ? "BLOCKED" : "RELEASED")
            << " temperature:: " << notify.throt_temper << "\'C";
    }
    else if (notify.ntfy_code == NTFY_EMERGENCY_WARN) {
        os  << "[Emergency] NPU@" << notify.npu_id
            << ":: " << "Warning - Temperature has reached the Emergency Point "
            << "(" << notify.throt_temper << ")\'C";
    }
    else {
        os << "[Throttling] NPU@" << notify.npu_id
           << " voltage:: " << notify.throt_voltage[0] / 1000 << "mV -> " << notify.throt_voltage[1] / 1000 << "mV"
           << " frequency:: " << notify.throt_freq[0] << "mhz -> " << notify.throt_freq[1] << "mhz"
           << " temperature:: " << notify.throt_temper << "\'C";
    }
    return os;
}


DXRT_API std::string dxrt_cmd_t_str(dxrt::dxrt_cmd_t c)
{
    static std::map<dxrt::dxrt_cmd_t, std::string> m;
    if (m.size() == 0) {
        m[dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE] = "DXRT_CMD_IDENTIFY_DEVICE";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_GET_STATUS] = "DXRT_CMD_GET_STATUS";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_RESET] = "DXRT_CMD_RESET";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_CONFIG] = "DXRT_CMD_UPDATE_CONFIG";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_FIRMWARE] = "DXRT_CMD_UPDATE_FIRMWARE";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_GET_LOG] = "DXRT_CMD_GET_LOG";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_DUMP] = "DXRT_CMD_DUMP";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_MEM] = "DXRT_CMD_WRITE_MEM";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_READ_MEM] = "DXRT_CMD_READ_MEM";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_CPU_CACHE_FLUSH] = "DXRT_CMD_CPU_CACHE_FLUSH";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_SOC_CUSTOM] = "DXRT_CMD_SOC_CUSTOM";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_INPUT_DMA_CH0] = "DXRT_CMD_WRITE_INPUT_DMA_CH0";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_INPUT_DMA_CH1] = "DXRT_CMD_WRITE_INPUT_DMA_CH1";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_INPUT_DMA_CH2] = "DXRT_CMD_WRITE_INPUT_DMA_CH2";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH0] = "DXRT_CMD_READ_OUTPUT_DMA_CH0";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH1] = "DXRT_CMD_READ_OUTPUT_DMA_CH1";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH2] = "DXRT_CMD_READ_OUTPUT_DMA_CH2";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE] = "DXRT_CMD_TERMINATE";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_EVENT] = "DXRT_CMD_EVENT";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_DRV_INFO] = "DXRT_CMD_DRV_INFO";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_SCHEDULE] = "DXRT_CMD_SCHEDULE";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_UPLOAD_FIRMWARE] = "DXRT_CMD_UPLOAD_FIRMWARE";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_REQ] = "DXRT_CMD_NPU_RUN_REQ";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP] = "DXRT_CMD_NPU_RUN_RESP";
        m[dxrt::dxrt_cmd_t::DXRT_CMD_MAX] = "DXRT_CMD_MAX";
    }
    return m.find(c) == m.end() ? "dxrt_cmd_t_Unknown" : m[c];
}

} // namespace dxrt
