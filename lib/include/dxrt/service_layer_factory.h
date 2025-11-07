#pragma once

#include <memory>
#include <string>
#include "dxrt/service_abstract_layer.h"
#include "dxrt/multiprocess_memory.h"

namespace dxrt {

// Factory functions:
// 1) CreateServiceLayer(useService, mem)
//    - If useService == true and mem == nullptr: creates and owns MultiprocessMemory internally
//    - If useService == true and mem != nullptr: uses external mem (no ownership)
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
    CreateServiceLayer(bool useService, std::shared_ptr<MultiprocessMemory> mem);

    static std::shared_ptr<ServiceLayerInterface>
    CreateServiceLayerFromEnv();

    static std::shared_ptr<ServiceLayerInterface>
    CreateDefaultServiceLayer();
};

} // namespace dxrt
