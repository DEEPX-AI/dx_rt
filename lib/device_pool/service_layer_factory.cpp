/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/common.h"
#include "dxrt/service_layer_factory.h"
#include "dxrt/objects_pool.h"
#include "dxrt/device_pool.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace dxrt {

std::shared_ptr<ServiceLayerInterface>
ServiceLayerFactory::CreateServiceLayer(bool useService, std::shared_ptr<MultiprocessMemory> mem)
{
    if (useService) {
        if (mem == nullptr) {
            mem = ObjectsPool::GetInstance().GetMultiProcessMemory();
        }

        return std::make_shared<ServiceLayer>(mem);

    }
    auto layer = std::shared_ptr<ServiceLayerInterface>(std::make_shared<NoServiceLayer>());
    int device_len = DevicePool::GetInstance().GetDeviceCount();
    for (int device_id = 0; device_id < device_len; device_id++)
    {
        layer->RegisterDeviceCore(DevicePool::GetInstance().GetDeviceCores(device_id).get());
    }

    return layer;
}

std::shared_ptr<ServiceLayerInterface>
ServiceLayerFactory::CreateServiceLayerFromEnv()
{
    const char* env = std::getenv("DXRT_USE_SERVICE");
    bool useService = (env && (std::strcmp(env, "1") == 0 || std::strcmp(env, "true") == 0));
    return CreateServiceLayer(useService, nullptr);
}

std::shared_ptr<ServiceLayerInterface>
ServiceLayerFactory::CreateDefaultServiceLayer()
{
#ifdef USE_SERVICE
    const char* env = std::getenv("DXRT_USE_SERVICE");
    bool useService = true;
    if (env) {
        useService = (std::strcmp(env, "0") != 0 && std::strcmp(env, "false") != 0);
    }
    return CreateServiceLayer(useService, nullptr);
#else
    return CreateServiceLayer(false, nullptr);
#endif
}

} // namespace dxrt
