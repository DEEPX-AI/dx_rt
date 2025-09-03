/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/dxrt_api.h"

#include <string>
#include <iostream>


int main()
{
    try
    {

        // print version
        std::cout << "Runtime Framework Version: " << dxrt::Configuration::GetInstance().GetVersion() << std::endl; 

        // print device driver version
        std::string device_driver = dxrt::Configuration::GetInstance().GetDriverVersion();
        std::cout << "Device Driver Version: v" << device_driver << std::endl; 

        // print pcie driver version
        std::cout << "PCIe Driver Version: v" << dxrt::Configuration::GetInstance().GetPCIeDriverVersion() << std::endl; 

        // print firmware versions
        std::vector<std::pair<int, std::string>> fws = dxrt::Configuration::GetInstance().GetFirmwareVersions();
        for(auto& v : fws)
        {
            std::cout << "Firmware Version: device-id=" 
                        << v.first << " v" << v.second << std::endl;
        }

        // print ONNX version
        std::string onnx_version = dxrt::Configuration::GetInstance().GetONNXRuntimeVersion();
        std::cout << "ONNX Runtime Version: v" << onnx_version << std::endl; 
    }
    catch (const dxrt::Exception &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}