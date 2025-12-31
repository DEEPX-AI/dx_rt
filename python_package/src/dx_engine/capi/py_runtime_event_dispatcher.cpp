#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <stdexcept>

#include "dxrt/dxrt_api.h"

namespace dxrt
{
    void pyRuntimeEventDispatcher_DispatchEvent(RuntimeEventDispatcher &dispatcher, int level, int type, int code, const std::string &message)
    {
        dispatcher.DispatchEvent(static_cast<RuntimeEventDispatcher::LEVEL>(level),
                                 static_cast<RuntimeEventDispatcher::TYPE>(type),
                                 static_cast<RuntimeEventDispatcher::CODE>(code),
                                 message);
    }

    void pyRuntimeEventDispatcher_SetCurrentLevel(RuntimeEventDispatcher &dispatcher, int level)
    {
        dispatcher.SetCurrentLevel(static_cast<RuntimeEventDispatcher::LEVEL>(level));
    }

    int pyRuntimeEventDispatcher_GetCurrentLevel(RuntimeEventDispatcher &dispatcher)
    {
        return static_cast<int>(dispatcher.GetCurrentLevel());
    }

    void RuntimeEventDispatcher_RegisterEventHandler(RuntimeEventDispatcher &dispatcher,
        std::function<void(int, int, int, const std::string&, const std::string&)> handler)
    {
        dispatcher.RegisterEventHandler(
            [handler](RuntimeEventDispatcher::LEVEL level,
                      RuntimeEventDispatcher::TYPE type,
                      RuntimeEventDispatcher::CODE code,
                      const std::string& message,
                      const std::string& timestamp)
            {
                handler(static_cast<int>(level),
                        static_cast<int>(type),
                        static_cast<int>(code),
                        message,
                        timestamp);
            });
    }

    

    

} // namespace dxrt
