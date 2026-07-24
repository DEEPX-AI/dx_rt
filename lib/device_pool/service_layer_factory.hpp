#pragma once

#include <memory>
#include <string>
#include "dxrt/service_abstract_layer.h"

namespace dxrt {

class MultiprocessMemory;

// Factory functions:
// 1) CreateServiceLayer(useService, mem)
//    - If useService == true: uses ServiceLayer Dynamic IPC
//    - If useService == false: creates NoServiceLayer
//
// 2) CreateServiceLayerFromEnv()
//    - If environment variable DXRT_USE_SERVICE=1: service mode
//    - Otherwise: NoServiceLayer
//
// 3) CreateDefaultServiceLayer()
//    - If compile-time macro USE_SERVICE is enabled, attempts service mode (falls back to NoServiceLayer on failure)
//    - Otherwise: NoServiceLayer
//
// Returns: std::shared_ptr<ServiceLayerInterface>
class ServiceLayerFactory {
public:
    static std::shared_ptr<ServiceLayerInterface>
    CreateServiceLayer(bool useService);

    static std::shared_ptr<ServiceLayerInterface>
    CreateServiceLayerFromEnv();

    static std::shared_ptr<ServiceLayerInterface>
    CreateDefaultServiceLayer();
};

} // namespace dxrt
