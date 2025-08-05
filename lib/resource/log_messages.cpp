

#include "log_messages.h"
#include "dxrt/device_version.h"
#include <iostream>

namespace dxrt {

    std::string LogMessages::NotSupported_ModelCompilerVersion(
        const std::string& currentCompilerVersion, 
        const std::string& requiredCompilerVersion)
    {
        return "The model's compiler version(" 
            + currentCompilerVersion
            + ") is not compatible in this RT library.\nPlease use a model file generated with a compiler version " 
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
                + " is not supported.\nPlease use model file version "
                + std::to_string(requiredFileFormatVersion)
                + " or higher.";
    }

    std::string LogMessages::NotSupported_ModelFileFormatMaxVersion(
        int currentFileFormatVersion, 
        int requiredFileFormatMaxVersion)
    {
        return 
                "Model file format version "
                + std::to_string(currentFileFormatVersion)
                + " is not supported.\nPlease use model file version "
                + std::to_string(requiredFileFormatMaxVersion)
                + " or lower.";
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
        
        return 
            "The current device driver version is " 
            + ConvertIntToVersion(currentDriverVersion) 
            + ".\nPlease update your device driver to version " 
            + ConvertIntToVersion(requiredDriverVersion) 
            + " or higher.";
            
    }

    std::string LogMessages::NotSupported_PCIEDriverVersion(int currentDriverVersion, int requiredDriverVersion)
    {
        
        return 
            "The current pcie driver version is " 
            + ConvertIntToVersion(currentDriverVersion) 
            + ".\nPlease update your pcie driver to version " 
            + ConvertIntToVersion(requiredDriverVersion) 
            + " or higher.";
    }

    std::string LogMessages::NotSupported_FirmwareVersion(int currentVersion, int requiredVersion)
    {
        return 
            "The current firmware version is " 
            + ConvertIntToVersion(currentVersion) 
            + ".\nPlease update your firmware to version " 
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

    std::string LogMessages::InvalidDXNNFileFormat()
    {
        return "Invalid .dxnn file format";
    }

    std::string LogMessages::InvalidDXNNModelHeader(int errorCode)
    {
        return "Model Header info parsing failed. error-code=" + std::to_string(errorCode);
    }

    std::string LogMessages::NotSupported_ONNXRuntimeVersion(const std::string& currentVersion, const std::string& requiredVersion)
    {

        return 
            "The current ONNX Runtime version is " 
            + currentVersion
            + ".\nPlease update your ONNX Runtime to version " 
            + requiredVersion 
            + " or higher.";
    }


    std::string LogMessages::CPUHandle_NoInputTensorsAvailable(const std::string& taskName, int currentInputCount, int requiredInputCount)
    {
        return "No input tensors available for CPU task: " + taskName
                    + ", required-input-count: " + std::to_string(requiredInputCount)
                    + ", current-input-count: " + std::to_string(currentInputCount);
    }

    std::string LogMessages::CPUHandle_NotFoundInONNXOutputs(const std::string& tensorName, const std::string& taskName)
    {
        return "Tensor '" + tensorName + "' not found in ONNX outputs for task: " + taskName;
    }

    std::string LogMessages::CPUHandle_NoOutputTensorsAvailable(const std::string& taskName, int currentInputCount, int requiredInputCount)
    {
        return "No output tensors available for CPU task: " + taskName
                    + ", required-input-count: " + std::to_string(requiredInputCount)
                    + ", current-input-count: " + std::to_string(currentInputCount);
    }

    std::string LogMessages::CPUHandle_InputTensorCountMismatch(int currentCount, int expectedCount)
    {
    
        return "Input tensor count mismatch: expected " + std::to_string(expectedCount)
                    + ", got " + std::to_string(currentCount);
    }

    std::string LogMessages::CPUHandle_OutputTensorCountMismatch(int currentCount, int expectedCount)
    {
    
        return "Output tensor count mismatch: expected " + std::to_string(expectedCount)
                    + ", got " + std::to_string(currentCount);
    }


    std::string LogMessages::ModelParser_OutputOffsetIsNotZero()
    {

        return "The output offset is not zero, please check the DX-COM.";
    }

    std::string LogMessages::InferenceEngine_InvaildModel()
    {
        return "Invalid Model";
    }

    std::string LogMessages::InferenceEngine_BatchArgumentIsNull()
    {
        return "Internal argument for run-batch operation is nullptr.";
    }

    std::string LogMessages::InferenceEngine_BatchFailToAllocateOutputBuffer()
    {
        return "Fail to allocate output buffer for run-batch exception=";
    }

    std::string LogMessages::InferenceEngine_TimeoutRunBenchmark()
    {
        return "RunBenchmark timeout - possible deadlock detected";
    }
    std::string LogMessages::InferenceEngine_InvalidJobId(int jobId)
    {
        return "InferenceEngine::Wait job id "+ std::to_string(jobId) + " is out of range";
    }

    std::string LogMessages::CLI_DonotTurnOffDuringUpdateFirmware()
    {
        return "Don't turn off the power until the firmware download is done.";
    }

    std::string LogMessages::CLI_InvalidFirmwareFile(const std::string& filename)
    {
        return "Invalid firmware file format: " + filename + ". \nPlease check the file.";
    }

    std::string LogMessages::CLI_UpdatingFirmware(const std::string& boardType, const std::string& version)
    {
        return "Updating the " + boardType + " device to firmware version " + version + ".";
    }

    std::string LogMessages::CLI_NoUpdateDeviceFound()
    {
        return "No device found for the firmware.";
    }

    std::string LogMessages::CLI_UpdateFirmwareSkip()
    {
        return "A higher or equal version of the firmware has been downloaded.";
    }

} // namespace dxrt