// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/fwlog.h"
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
    if(log.cmd>=FW_LOG_MAX)
    {
        return "";
    }
    if(log.cmd>=DXRT_CMD_MAX && log.cmd<FW_LOG_TEMP)
    {
        return "";
    }
    oss << "[" << log.timestamp << "] ";
    switch(log.cmd)
    {
        case DXRT_CMD_IDENTIFY_DEVICE:
            oss << "identify: variant " << log.args[0] << ", "
                << "mem addr [" << hex << log.args[1] << ", " << log.args[2] << "], "
                << "mem size " << log.args[3] << ", num_dma_ch " << log.args[4] << endl;
            break;
        case FW_LOG_INFERENCE_REQUEST:
            oss << "req " << log.args[0] << " -> npu" << log.args[1]
                << ", type " << log.args[2] << ", input offset " << hex << log.args[3] << ", output offset " << log.args[4] << endl;
            break;
        case FW_LOG_INFERENCE_RESPONSE:
            oss << "response " << log.args[0] << " <- npu" << log.args[1]
                << ", inf_time " << log.args[2] << ", status " << log.args[3] << ", argmax " << log.args[4] << endl;
            break;
        case FW_LOG_NPU_HANG:
            oss << "npu hang detected: " << log.args[0] << ", " << log.args[1]
                << ", " << log.args[2] << ", " << log.args[3] << ", " << log.args[4] << endl;
            break;
        case DXRT_CMD_RESET:
            oss << "reset: opt" << log.args[0] << endl;
            break;
        case DXRT_CMD_GET_STATUS:
        case DXRT_CMD_UPDATE_CONFIG:
        case DXRT_CMD_GET_LOG:
        case DXRT_CMD_DUMP:
        case DXRT_CMD_WRITE_MEM:
        case DXRT_CMD_READ_MEM:
        case DXRT_CMD_CPU_CACHE_FLUSH:
        case DXRT_CMD_SOC_CUSTOM:
        case DXRT_CMD_WRITE_INPUT_DMA_CH0:
        case DXRT_CMD_WRITE_INPUT_DMA_CH1:
        case DXRT_CMD_READ_OUTPUT_DMA_CH0:
        case DXRT_CMD_READ_OUTPUT_DMA_CH1:
        case DXRT_CMD_TERMINATE:
        case FW_LOG_TEMP:
        case FW_LOG_DXRT_DEQUEUE_REQUEST:
        case FW_LOG_GENERATE_MSI:
            oss << "[" << log.timestamp << "] " 
                << log.args[0] << ", "
                << log.args[1] << ", "
                << log.args[2] << ", "
                << log.args[3] << ", "
                << log.args[4] << ", "
                << log.args[5] << ", "
                << log.args[6] << ", "
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

} // namespace dxrt