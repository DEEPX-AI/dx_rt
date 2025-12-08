

#include "dxrt/runtime_event_dispatcher.h"
#include <future>
#include <iomanip>
#include <sstream>
#include <chrono>


namespace dxrt {
    RuntimeEventDispatcher* RuntimeEventDispatcher::_staticInstance = nullptr;

    RuntimeEventDispatcher& RuntimeEventDispatcher::GetInstance()
    {
        static RuntimeEventDispatcher instance;
        return instance;
    }

    void RuntimeEventDispatcher::DispatchEvent(LEVEL level, TYPE type, CODE code, const std::string& eventMessage)
    {
        if ( level >= _currentLevel.load() )
        {
            auto now = std::chrono::system_clock::now();
            auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
            auto ms_count = std::chrono::duration_cast<std::chrono::milliseconds>(
                now_ms.time_since_epoch());
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            std::stringstream ss;
            std::tm tm_buf;
#ifdef _WIN32
            localtime_s(&tm_buf, &in_time_t);
#else
            localtime_r(&in_time_t, &tm_buf);
#endif
            ss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
            ss << '.' << std::setfill('0') << std::setw(3) << (ms_count.count() % 1000);
            std::string timestamp = ss.str();

            handleEventLogging(level, type, code, eventMessage, timestamp);
            invokeEventHandler(level, type, code, eventMessage, timestamp);
        }
    }

    void RuntimeEventDispatcher::RegisterEventHandler(std::function<void(LEVEL, TYPE, CODE, const std::string&, const std::string&)> handler)
    {
        std::lock_guard<std::mutex> lock(_handlerMutex);

        _eventHandler = handler;
    }

    bool RuntimeEventDispatcher::invokeEventHandler(LEVEL level, TYPE type, CODE code, const std::string& eventMessage, const std::string& timestamp)
    {
        std::function<void(LEVEL, TYPE, CODE, const std::string&, const std::string&)> handlerCopy;

        {
            // lock range minimized
            std::lock_guard<std::mutex> lock(_handlerMutex);
            handlerCopy = _eventHandler;
        }

        if (handlerCopy)
        {
            // Invoke the handler in a separate thread to avoid blocking
            handlerCopy(level, type, code, eventMessage, timestamp);
            return true;
        }
        return false;
    }

    void RuntimeEventDispatcher::handleEventLogging(LEVEL level, TYPE type, CODE code, const std::string& eventMessage, const std::string& timestamp)
    {
        // Convert enum values to readable strings
        std::string levelStr;
        switch (level) {
            case LEVEL::INFO:     levelStr = "INFO"; break;
            case LEVEL::WARNING:  levelStr = "WARNING"; break;
            case LEVEL::ERROR:    levelStr = "ERROR"; break;
            case LEVEL::CRITICAL: levelStr = "CRITICAL"; break;
            default:                    levelStr = "UNKNOWN"; break;
        }

        std::string typeStr;
        switch (type) {
            case TYPE::DEVICE_CORE:       typeStr = "DEVICE_CORE"; break;
            case TYPE::DEVICE_STATUS:     typeStr = "DEVICE_STATUS"; break;
            case TYPE::DEVICE_IO:         typeStr = "DEVICE_IO"; break;
            case TYPE::DEVICE_MEMORY:     typeStr = "DEVICE_MEMORY"; break;
            default:                            typeStr = "UNKNOWN"; break;
        }

        std::string codeStr;
        switch (code) {
            case CODE::WRITE_INPUT:         codeStr = "WRITE_INPUT"; break;
            case CODE::READ_OUTPUT:         codeStr = "READ_OUTPUT"; break;
            case CODE::MEMORY_OVERFLOW:     codeStr = "MEMORY_OVERFLOW"; break;
            case CODE::MEMORY_ALLOCATION:   codeStr = "MEMORY_ALLOCATION"; break;
            case CODE::DEVICE_EVENT:        codeStr = "DEVICE_EVENT"; break;
            case CODE::RECOVERY_OCCURRED:   codeStr = "RECOVERY_OCCURRED"; break;
            case CODE::TIMEOUT_OCCURRED:    codeStr = "TIMEOUT_OCCURRED"; break;
            case CODE::THROTTLING_NOTICE:   codeStr = "THROTTLING_NOTICE"; break;
            case CODE::THROTTLING_EMERGENCY:    codeStr = "THROTTLING_EMERGENCY"; break;
            default:                        codeStr = "UNKNOWN"; break;
        }

        static std::mutex logging_mutex;
        std::lock_guard<std::mutex> lock(logging_mutex);

        std::cout << "[RuntimeEventDispatcher] level=" << levelStr
                    << " type=" << typeStr
                    << " code=" << codeStr
                    << " message=\"" << eventMessage << "\""
                    << " timestamp=\"" << timestamp << "\"" << std::endl;
    }


}  // namespace dxrt
