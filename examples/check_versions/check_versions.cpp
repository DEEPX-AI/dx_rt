// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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