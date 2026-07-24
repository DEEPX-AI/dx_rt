/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "service_layer_factory.hpp"

#include <cstdlib>
#include <cstring>

#include "dxrt/device_pool.h"

namespace dxrt {

std::shared_ptr<ServiceLayerInterface>
ServiceLayerFactory::CreateServiceLayer(bool useService)
{
    if (useService) {
        return std::make_shared<ServiceLayer>();
    }
    auto layer = std::shared_ptr<ServiceLayerInterface>(std::make_shared<NoServiceLayer>());
    auto device_len = static_cast<int>(DevicePool::GetInstance().GetDeviceCount());
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
    return CreateServiceLayer(useService);
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
    return CreateServiceLayer(useService);
#else
    return CreateServiceLayer(false);
#endif
}

}  // namespace dxrt
