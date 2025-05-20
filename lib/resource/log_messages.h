
#include <string>

namespace dxrt {


    class LogMessages
    {
    public:
        static std::string ConvertIntToVersion(int version);

        static std::string NotSupported_ModelCompilerVersion(const std::string& currentCompilerVersion, 
                                                            const std::string& requiredCompilerVersion,
                                                            const std::string& supportedRTVersion);

        static std::string NotSupported_ModelFileFormatVersion(int currentFileFormatVersion, 
                                                            int requiredFileFormatVersion);

        static std::string NotSupported_DeviceDriverVersion(int currentDriverVersion, int requiredDriverVersion);
        static std::string NotSupported_PCIEDriverVersion(int currentDriverVersion, int requiredDriverVersion);
        static std::string NotSupported_FirmwareVersion(int currentVersion, int requiredVersion);

        static std::string DeviceNotFound();
    };

}