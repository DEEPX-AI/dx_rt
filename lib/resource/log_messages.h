// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include <string>

namespace dxrt {
    class LogMessages
    {
    public:
        static std::string ConvertIntToVersion(int version);

        static std::string NotSupported_ModelCompilerVersion(const std::string& currentCompilerVersion,
                                                            const std::string& requiredCompilerVersion);

        static std::string NotSupported_ModelFileFormatVersion(int currentFileFormatVersion,
                                                            int requiredFileFormatVersion);
        static std::string NotSupported_ModelFileFormatMaxVersion(int currentFileFormatVersion,
                                                            int requiredMaxFileFormatVersion);

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
    };

}  // namespace dxrt
