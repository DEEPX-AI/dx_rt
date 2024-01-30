// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include <map>
#include <linux/ioctl.h>

using namespace std;

namespace dxrt {

vector<pair<int,string>> ioctlTable = {
    { DXRT_IOCTL_MESSAGE, "IOCTL_MESSAGE" },
    { DXRT_IOCTL_DUMMY, "IOCTL_DUMMY" },
};
map<int,string> errTable = {
    {ERR_NPU0_HANG, "NPU0 Hang"},
    {ERR_NPU1_HANG, "NPU1 Hang"},
    {ERR_PCIE_DMA_CH0_FAIL, "PCIe-DMA Fail in ch0"},
    {ERR_PCIE_DMA_CH1_FAIL, "PCIe-DMA Fail in ch1"},
};

std::ostream& operator<<(std::ostream& os, const dxrt_error_t& error)
{
    os << errTable[error] ;
    return os;
}
std::string ErrTable(dxrt_error_t error)
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
        << "interface " << info.interface << ", "
        << string(info.fw_info)
        << dec;
    return os;
}

} // namespace dxrt
