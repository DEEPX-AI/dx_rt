/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/driver_adapter/driver_adapter_factory.h"

#if defined(__linux__)
  #include "dxrt/driver_adapter/linux_driver_adapter.h"
  #if DXRT_USB_NETWORK_DRIVER
    #include "dxrt/driver_adapter/network_driver_adapter.h"
  #endif
#elif defined(_WIN32)
  #include "dxrt/driver_adapter/windows_driver_adapter.h"
#endif

namespace dxrt {

std::unique_ptr<DriverAdapter> DriverAdapterFactory::CreateForDeviceFile(const std::string& devicePath) {
#if defined(__linux__)
    return std::make_unique<LinuxDriverAdapter>(devicePath.c_str());
#elif defined(_WIN32)
    return std::make_unique<WindowsDriverAdapter>(devicePath.c_str());
#else
    (void)devicePath; return nullptr;
#endif
}

std::unique_ptr<DriverAdapter> DriverAdapterFactory::CreateForNetwork() {
#if defined(__linux__) && DXRT_USB_NETWORK_DRIVER
    return std::make_unique<NetworkDriverAdapter>();
#else
    return nullptr;
#endif
}

}  // namespace dxrt
