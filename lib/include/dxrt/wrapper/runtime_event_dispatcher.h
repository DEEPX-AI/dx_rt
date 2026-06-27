/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * DXRT Wrapper — RuntimeEventDispatcher (prebuilt delivery).
 */

#pragma once
#include "dxrt/dxrt_c_api.h"
#include <functional>
#include <string>

namespace dxrt {

class RuntimeEventDispatcher
{
public:
    enum class LEVEL
    {
        INFO     = 1,
        WARNING  = 2,
        ERROR    = 3,
        CRITICAL = 4
    };

    enum class TYPE
    {
        DEVICE_CORE   = 1000,
        DEVICE_STATUS = 1001,
        DEVICE_IO     = 1002,
        DEVICE_MEMORY = 1003,
        UNKNOWN       = 1004
    };

    enum class CODE
    {
        WRITE_INPUT          = 2000,
        READ_OUTPUT          = 2001,
        MEMORY_OVERFLOW      = 2002,
        MEMORY_ALLOCATION    = 2003,
        DEVICE_EVENT         = 2004,
        RECOVERY_OCCURRED    = 2005,
        TIMEOUT_OCCURRED     = 2006,
        THROTTLING_NOTICE    = 2007,
        THROTTLING_EMERGENCY = 2008,
        UNKNOWN              = 2009
    };

    static RuntimeEventDispatcher& GetInstance()
    {
        static RuntimeEventDispatcher instance;
        return instance;
    }

    void DispatchEvent(LEVEL level, TYPE type, CODE code, const std::string& message)
    {
        dxrt_event_dispatch(
            static_cast<dxrt_event_level_t>(level),
            static_cast<dxrt_event_type_t>(type),
            static_cast<dxrt_event_code_t>(code),
            message.c_str());
    }

    void RegisterEventHandler(
        const std::function<void(LEVEL, TYPE, CODE,
                                 const std::string&,
                                 const std::string&)>& handler)
    {
        handler_ = handler;
        if (handler_)
        {
            dxrt_event_register_handler(&callback_trampoline, this);
        }
    }

    void SetCurrentLevel(LEVEL level)
    {
        dxrt_event_set_level(static_cast<dxrt_event_level_t>(level));
    }

    LEVEL GetCurrentLevel() const
    {
        dxrt_event_level_t level;
        dxrt_event_get_level(&level);
        return static_cast<LEVEL>(level);
    }

private:
    RuntimeEventDispatcher() = default;
    RuntimeEventDispatcher(const RuntimeEventDispatcher&) = delete;
    RuntimeEventDispatcher& operator=(const RuntimeEventDispatcher&) = delete;

    std::function<void(LEVEL, TYPE, CODE,
                       const std::string&,
                       const std::string&)> handler_;

    static void callback_trampoline(dxrt_event_level_t level,
                                    dxrt_event_type_t type,
                                    dxrt_event_code_t code,
                                    const char* message,
                                    const char* timestamp,
                                    void* user_data)
    {
        auto* self = static_cast<RuntimeEventDispatcher*>(user_data);
        if (self->handler_)
        {
            self->handler_(
                static_cast<LEVEL>(level),
                static_cast<TYPE>(type),
                static_cast<CODE>(code),
                message ? message : "",
                timestamp ? timestamp : "");
        }
    }
};

} // namespace dxrt
