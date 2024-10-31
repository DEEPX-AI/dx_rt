// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/fw.h"
#include "dxrt/util.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

namespace dxrt
{

string ParseFwLog(dxrt_device_log_t &log)
{
    string ret;
    ostringstream oss;
    oss << dec ;
    if(log.cmd>=dxrt::dxrt_fwlog_cmd_t::FW_LOG_MAX)
    {
        return "";
    }
    if(log.cmd>=dxrt::dxrt_cmd_t::DXRT_CMD_MAX && log.cmd<dxrt::dxrt_fwlog_cmd_t::FW_LOG_TEMP)
    {
        return "";
    }
    oss << "[" << log.timestamp << "] ";
    switch(log.cmd)
    {
        case dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE:
            oss << "identify: variant " << log.args[0] << ", "
                << "mem addr [" << hex << log.args[1] << ", " << log.args[2] << "], "
                << "mem size " << log.args[3] << ", num_dma_ch " << log.args[4] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_INFERENCE_REQUEST:
            oss << "req " << log.args[0] << " -> npu" << log.args[1]
                << ", type " << log.args[2] << ", input offset " << hex << log.args[3] << ", output offset " << log.args[4] << endl;
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
                << "poped: " << log.args[3] << ", count: " << log.args[4] << ", "
                << "access_count: " << log.args[5] << endl;
            break;
        case dxrt::dxrt_fwlog_cmd_t::FW_LOG_DXRT_DEQUEUE_POLLING:
            oss << "deque(poll)  id:" << log.args[0] << ", "
                << "front:" << log.args[1] << ", rear:" << log.args[2] << ", "
                << "poped: " << log.args[3] << ", count: " << log.args[4] << ", "
                << "access_count: " << log.args[5] << endl;
            break;
        case FW_LOG_DXRT_DEQUEUE_POPED:
            oss << " > poped id:" << log.args[0] << ", "
                << "front:" << log.args[1] << ", rear:" << log.args[2] << ", "
                << "poped: " << log.args[3] << ", count: " << log.args[4] << ", "
                << "access_count: " << log.args[5] << endl;
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

FwLog::FwLog(vector<dxrt_device_log_t> logs_)
:_logs(logs_), _str("")
{
    for(auto &log:_logs)
    {
        _str.append(ParseFwLog(log));
    }
}

FwLog::~FwLog()
{

}

string FwLog::str()
{
    return _str;
}
void FwLog::toFile(string file)
{
    ofstream outputFile(file);
    if(outputFile.is_open())
    {
        outputFile << _str;
        outputFile.close();
    }
    else
    {
        cout << "Error: Can't open file " << file << endl;
    }
}

Fw::Fw(string file)
{
    vector<char> data(sizeof(dx_fw_header_t));
    DataFromFile(file, static_cast<void*>(data.data()), sizeof(dx_fw_header_t));
    memcpy(&fwHeader, data.data(), sizeof(dx_fw_header_t));
    Show();
}

Fw::~Fw()
{

}

void Fw::Show(void)
{
    cout << "============ FW Binary Information ============" << endl;
    cout << "Signature   : " << fwHeader.signature << endl;
    cout << "Total Image : " << fwHeader.length << endl;
    cout << "Board Type  : " << fwHeader.board_type << endl;
    cout << "DDR Type    : " << fwHeader.ddr_type << endl;
    cout << "Firmware Ver: " << fwHeader.fw_ver << endl;
}

string Fw::GetFwBinVersion()
{
    return string(fwHeader.fw_ver);
}

bool Fw::IsMatchSignature()
{
    string dxSign = "DEEPX GENESIS-M";
    return (dxSign.compare(string(fwHeader.signature)) == 0) ? true : false;
}

string Fw::GetFwUpdateResult(uint32_t errCode)
{
    string errMsg = "";
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
                default:
                    errMsg += ("Unknown error detected("+ to_string(mask) +")");
                    break;
            }
        }
    }
    return errMsg;
}

} // namespace dxrt