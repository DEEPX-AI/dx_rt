/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"
#include <string>

namespace dxrt {
    class DXRT_API LogMessages
    {
    public:
        static std::string ConvertIntToVersion(int version);

        static std::string NotSupported_ModelCompilerVersion(const std::string& currentCompilerVersion,
                                                            const std::string& requiredCompilerVersion);

        static std::string NotSupported_ModelFileFormatVersion(int currentFileFormatVersion,
                                                            int requiredFileFormatMinVersion,
                                                            int requiredFileFormatMaxVersion);

        static std::string NotSupported_DeviceDriverVersion(int currentDriverVersion, int requiredDriverVersion);
        static std::string NotSupported_PCIEDriverVersion(int currentDriverVersion, int requiredDriverVersion);
        static std::string NotSupported_FirmwareVersion(int currentVersion, int requiredVersion);

        static std::string DeviceNotFound();
        static std::string AllDeviceBlocked();

        static std::string InvalidDXNNFileFormat();
        static std::string InvalidDXNNModelHeader(int errorCode);

        static std::string NotSupported_ONNXRuntimeVersion(const std::string& currentVersion, const std::string& requiredVersion);


        static std::string CPUHandle_NoInputTensorsAvailable(const std::string& taskName, int currentInputCount, int requiredInputCount);
        static std::string CPUHandle_NoOutputTensorsAvailable(const std::string& taskName, int currentInputCount, int requiredInputCount);
        static std::string CPUHandle_NotFoundInONNXOutputs(const std::string& tensorName, const std::string& taskName);

        static std::string CPUHandle_InputTensorCountMismatch(int currentCount, int expectedCount);
        static std::string CPUHandle_OutputTensorCountMismatch(int currentCount, int expectedCount);

        static std::string ModelParser_OutputOffsetIsNotZero();

        static std::string InferenceEngine_InvaildModel();
        static std::string InferenceEngine_BatchArgumentIsNull();
        static std::string InferenceEngine_BatchFailToAllocateOutputBuffer();
        static std::string InferenceEngine_TimeoutRunBenchmark();
        static std::string InferenceEngine_InvalidJobId(int jobId);

        static std::string CLI_UpdatingFirmware(const std::string& boardType, const std::string& version);
        static std::string CLI_DonotTurnOffDuringUpdateFirmware();
        static std::string CLI_InvalidFirmwareFile(const std::string& filename);
        static std::string CLI_NoUpdateDeviceFound();
        static std::string CLI_UpdateFirmwareSkip();
        static std::string CLI_UpdateCondition(const std::string& version);

        static std::string Profiler_MemoryUsage(uint64_t current_memory);

        static std::string Device_FailToInitialize(int id);
        static std::string Device_DeviceErrorEvent(int errorCode);

        // Runtime error messages for RuntimeErrorDispatcher
        static std::string RuntimeDispatch_FailToReadOutput(int errorCode, int requestId, int channelId);
        static std::string RuntimeDispatch_FailToWriteInput(int errorCode, int requestId, int channelId);
        static std::string RuntimeDispatch_RanOutOfNPUMemory();
        static std::string RuntimeDispatch_RanOutOfNPUMemoryForTask(int taskId);
        static std::string RuntimeDispatch_DeviceRecovery(int deviceId, const std::string& type);
        static std::string RuntimeDispatch_DeviceEventError(int deviceId, const std::string& errCodeStr);
        static std::string RuntimeDispatch_ThrottlingNotice(int deviceId, int npuId, const std::string& mesg, int temperature);
        static std::string RuntimeDispatch_ThrottlingEmergency(int deviceId, int npuId, const std::string& emergencyCodeStr);
        
    };

}  // namespace dxrt
