/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/fw.h"
#include "dxrt/util.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
using std::cout;
using std::endl;

namespace dxrt
{

std::string ParseFwLog(dxrt_device_log_t &log)
{
    std::string ret;
    std::ostringstream oss;
    oss << std::dec;
    if (log.cmd >= dxrt::dxrt_fwlog_cmd_t::FW_LOG_MAX)
    {
        return "";
    }
    if (log.cmd >= dxrt::dxrt_cmd_t::DXRT_CMD_MAX && log.cmd<dxrt::dxrt_fwlog_cmd_t::FW_LOG_TEMP)
    {
        return "";
    }
    oss << "[" << log.timestamp << "] ";
    switch (log.cmd)
    {
        case dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE:
            oss << "identify: variant " << log.args[0] << ", "
                << "mem addr [" << std::hex << log.args[1] << ", " << log.args[2] << "], "
                << "mem size " << log.args[3] << ", num_dma_ch " << log.args[4] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_INFERENCE_REQUEST:
            oss << "req " << log.args[0] << " -> npu" << log.args[1]
                << ", type " << log.args[2] << ", input offset " << std::hex << log.args[3]
                << ", output offset " << log.args[4] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_INFERENCE_RESPONSE:
            oss << "response " << log.args[0] << " <- npu" << log.args[1]
                << ", inf_time " << log.args[2] << ", status " << log.args[3] << ", argmax " << log.args[4] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_NPU_HANG:
            oss << "npu hang detected: " << log.args[0] << ", " << log.args[1]
                << ", " << log.args[2] << ", " << log.args[3] << ", " << log.args[4] << endl;
            break;
        case dxrt::dxrt_cmd_t::DXRT_CMD_RESET:
            oss << "reset: opt" << log.args[0] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_DXRT_DEQUEUE_IRQ:
            oss << "deque(irq)  id:" << log.args[0] << ", "
                << "front:" << log.args[1] << ", rear:" << log.args[2] << ", "
                << "locked: " << log.args[3] << ", count: " << log.args[4] << ", "
                << "access_count: " << log.args[5] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_DXRT_DEQUEUE_POLLING:
            oss << "deque(poll)  id:" << log.args[0] << ", "
                << "front:" << log.args[1] << ", rear:" << log.args[2] << ", "
                << "locked: " << log.args[3] << ", count: " << log.args[4] << ", "
                << "access_count: " << log.args[5] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_DXRT_DEQUEUE_POPED:
            oss << " > poped id:" << log.args[0] << ", "
                << "front:" << log.args[1] << ", rear:" << log.args[2] << ", "
                << "locked: " << log.args[3] << ", count: " << log.args[4] << ", "
                << "access_count: " << log.args[5] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_NORMAL_LOCK_IRQ:
            oss << " > irq_lock flag:" << log.args[0] << ", "
                << "locked:" << log.args[1] << ", count:" << log.args[2] << ", "
                << "front: " << log.args[3] << ", rear: " << log.args[4] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_NORMAL_UNLOCK_IRQ:
            oss << " > irq_unlock flag:" << log.args[0] << ", "
                << "locked:" << log.args[1] << ", count:" << log.args[2] << ", "
                << "front: " << log.args[3] << ", rear: " << log.args[4] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_HIGH_LOCK_IRQ:
            oss << " > irq_lock(high) flag:" << log.args[0] << ", "
                << "locked:" << log.args[1] << ", count:" << log.args[2] << ", "
                << "front: " << log.args[3] << ", rear: " << log.args[4] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_HIGH_UNLOCK_IRQ:
            oss << " > irq_unlock(high) flag:" << log.args[0] << ", "
                << "locked:" << log.args[1] << ", count:" << log.args[2] << ", "
                << "front: " << log.args[3] << ", rear: " << log.args[4] << endl;
            break;

            case dxrt::dxrt_fwlog_cmd_t::FW_LOG_TASK_LOCK:
            oss << " > task flag:" << log.args[0] << ", "
                << "locked:" << log.args[1] << ", count:" << log.args[2] << ", "
                << "front: " << log.args[3] << ", rear: " << log.args[4] << ", "
                << "timeout: " << log.args[5] << endl;
            break;
            case dxrt::dxrt_fwlog_cmd_t::FW_LOG_VOLT_UNDER_IRQ:
            oss << " > voltage drop detected::NPU@" << log.args[0] << ", "
                << "detected Voltage: " << log.args[1] << endl;
            break;
        case dxrt::dxrt_cmd_t::DXRT_CMD_GET_STATUS:
        case dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_CONFIG:
        case dxrt::dxrt_cmd_t::DXRT_CMD_GET_LOG:
        case dxrt::dxrt_cmd_t::DXRT_CMD_DUMP:
        case dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_MEM:
        case dxrt::dxrt_cmd_t::DXRT_CMD_READ_MEM:
        case dxrt::dxrt_cmd_t::DXRT_CMD_CPU_CACHE_FLUSH:
        case dxrt::dxrt_cmd_t::DXRT_CMD_SOC_CUSTOM:
        case dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_INPUT_DMA_CH0:
        case dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_INPUT_DMA_CH1:
        case dxrt::dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH0:
        case dxrt::dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH1:
        case dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE:
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_TEMP:
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_GENERATE_MSI:
            oss << "[" << log.timestamp << "] "
                << log.args[0] << ", "
                << log.args[1] << ", "
                << log.args[2] << ", "
                << log.args[3] << ", "
                << log.args[4] << ", "
                << log.args[5] << ", "
                //<< log.args[6] << ", "
                << endl;
            break;
        default:
            break;
    }
    return oss.str();
}

FwLog::FwLog(std::vector<dxrt_device_log_t> logs_)
:_logs(logs_), _str("")
{
    for (auto &log : _logs)
    {
        _str.append(ParseFwLog(log));
    }
}

FwLog::~FwLog()
{

}

std::string FwLog::str()
{
    return _str;
}
void FwLog::ToFileAppend(std::string file)
{
    std::ofstream outputFile(file, std::ios::app);
    if (outputFile.is_open())
    {
        outputFile << _deviceInfoString << std::endl;
        outputFile << _str;
        outputFile.close();
    }
    else
    {
        cout << "Error: Can't open file " << file << endl;
    }
}

Fw::Fw(std::string file)
{
    std::vector<char> data(sizeof(dx_fw_header_t));
    DataFromFile(file, static_cast<void*>(data.data()), sizeof(dx_fw_header_t));
    memcpy(&fwHeader, data.data(), sizeof(dx_fw_header_t));
    //Show();
}

Fw::~Fw()
{
}

uint32_t Fw::GetBoardType() const
{
    return fwHeader.board_type;
}

// constexpr std::array<pair_type, 3> board_types = {{{1, "SOM"}, {2, "M.2"}, {3, "H1"}}};
std::string Fw::GetBoardTypeString() const
{
    switch (fwHeader.board_type) {
        case 1:
            return "SOM";
        case 2:
            return "M.2";
        case 3:
            return "H1";
        default:
            return std::to_string(fwHeader.board_type);
    }
}

uint32_t Fw::GetDdrType() const
{
    return fwHeader.ddr_type;
}

std::string Fw::GetDdrTypeString() const
{
    switch (fwHeader.ddr_type) {
        case 1:
            return "LPDDR4";
        case 2:
            return "LPDDR5";
        default:
            return std::to_string(fwHeader.ddr_type);
    }
}

void Fw::Show(void)
{
    cout << "============ FW Binary Information ============" << endl;
    cout << "Signature   : " << fwHeader.signature << endl;
    //cout << "Total Image : " << fwHeader.length << endl;
    cout << "Board Type  : " << GetBoardTypeString() << endl;
    cout << "DDR Type    : " << GetDdrTypeString() << endl;
    cout << "Firmware Ver: " << fwHeader.fw_ver << endl;
}

std::string Fw::GetFwBinVersion()
{
    return std::string(fwHeader.fw_ver);
}

bool Fw::IsMatchSignature()
{
    std::string dxSign = "DEEPX GENESIS-M";
    return (dxSign.compare(std::string(fwHeader.signature)) == 0) ? true : false;
}

std::string Fw::GetFwUpdateResult(uint32_t errCode)
{
    std::string errMsg = "";
    for (uint32_t i = 0; i < sizeof(fw_update_err_code_t) * 8; ++i)
    {
        uint32_t mask = 1 << i;
        if (errCode & mask) {
            switch (mask) {
                case static_cast<uint32_t>(fw_update_err_code_t::ERR_HEADER_MISMATCH):
                    errMsg += "Header mismatch error detected\n";
                    break;
                case static_cast<uint32_t>(fw_update_err_code_t::ERR_BOARD_TYPE):
                    errMsg += "Board type error detected\n";
                    break;
                case static_cast<uint32_t>(fw_update_err_code_t::ERR_DDR_TYPE):
                    errMsg += "DDR type error detected\n";
                    break;
                case static_cast<uint32_t>(fw_update_err_code_t::ERR_CRC_MISMATCH):
                    errMsg += "CRC mismatch error detected\n";
                    break;
                case static_cast<uint32_t>(fw_update_err_code_t::ERR_SF_ERASE):
                    errMsg += "SF erase error detected\n";
                    break;
                case static_cast<uint32_t>(fw_update_err_code_t::ERR_SF_FLASH):
                    errMsg += "SF flash error detected\n";
                    break;
                case static_cast<uint32_t>(fw_update_err_code_t::ERR_LOW_FW_VER):
                    errMsg += "Low firmware version error detected\n";
                    break;
                case static_cast<uint32_t>(fw_update_err_code_t::ERR_NOT_SUPPORT):
                    errMsg += "Firmware version 2.x.x and above cannot be downgraded to version 1.x.x." +
                            std::string("\nPlease upgrade to version 2.x.x or later\n");
                    break;
                default:
                    errMsg += ("Unknown error detected("+ std::to_string(mask) +")");
                    break;
            }
        }
    }
    return errMsg;
}

}  // namespace dxrt
