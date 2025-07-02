

#include "log_messages.h"
#include <iostream>

namespace dxrt {

    std::string LogMessages::NotSupported_ModelCompilerVersion(
        const std::string& currentCompilerVersion, 
        const std::string& requiredCompilerVersion)
    {
        return "The model's compiler version(" 
            + currentCompilerVersion
            + ") is not compatible in this RT library. Please use a model file generated with a compiler version " 
            + requiredCompilerVersion
            + " or higher.";
    }

    std::string LogMessages::NotSupported_ModelFileFormatVersion(
        int currentFileFormatVersion, 
        int requiredFileFormatVersion)
    {
        return 
                "Model file format version "
                + std::to_string(currentFileFormatVersion)
                + " is not supported. Please use model file version "
                + std::to_string(requiredFileFormatVersion)
                + " or higher.";
    }

    std::string LogMessages::ConvertIntToVersion(int version)
    {

        std::string strValue = std::to_string(version);
        //std::cout << strValue << std::endl;

        if ( strValue.length() >= 3 ) 
        {
            std::string strVersion = "";
            strVersion += strValue[0];
            strVersion += ".";
            strVersion += strValue[1];
            strVersion += ".";

            std::string patch = "";
            if ( strValue.length() == 4 )
            {
                patch += strValue[2];
                patch += strValue[3];
            }
            else if ( strValue.length() == 3 )
                patch = strValue[2];
            else patch = "0";

            strVersion += std::to_string(std::stoi(patch));

            return strVersion;
        }

        return strValue;
    }

    std::string LogMessages::NotSupported_DeviceDriverVersion(int currentDriverVersion, int requiredDriverVersion)
    {
        //#define RT_DRV_VERSION_CHECK (1300)
        //#define FW_VERSION_CHECK     (163)
        //#define PCIE_VERSION_CHECK   (1201)
        
        return 
            "The current device driver version is " 
            + ConvertIntToVersion(currentDriverVersion) 
            + ". Please update your device driver to version " 
            + ConvertIntToVersion(requiredDriverVersion) 
            + " or higher.";
            
    }

    std::string LogMessages::NotSupported_PCIEDriverVersion(int currentDriverVersion, int requiredDriverVersion)
    {
        //#define RT_DRV_VERSION_CHECK (1300)
        //#define FW_VERSION_CHECK     (163)
        //#define PCIE_VERSION_CHECK   (1201)
        
        return 
            "The current pcie driver version is " 
            + ConvertIntToVersion(currentDriverVersion) 
            + ". Please update your pcie driver to version " 
            + ConvertIntToVersion(requiredDriverVersion) 
            + " or higher.";
    }

    std::string LogMessages::NotSupported_FirmwareVersion(int currentVersion, int requiredVersion)
    {
        return 
            "The current firmware version is " 
            + ConvertIntToVersion(currentVersion) 
            + ". Please update your firmware to version " 
            + ConvertIntToVersion(requiredVersion) 
            + " or higher.";
    }

    std::string LogMessages::DeviceNotFound()
    {
        return "Device not found";
    }
    std::string LogMessages::AllDeviceBlocked()
    {
        return "All Devices are blocked";
    }



} // namespace dxrt