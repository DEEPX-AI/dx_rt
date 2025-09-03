/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <iostream>
#include <mutex>
#include <atomic>

namespace dxrt {

class Logger
{
    public:
        enum class Level {
            NONE = 0,
            ERROR,
            INFO,
            DEBUG
        };

    private:
        std::atomic<Level> _level;

    public:
        static Logger& GetInstance()
        {
            static Logger logger;
            return logger;
        }

        void SetLevel(Level level)
        {
            _level.store(level);
        }

        Level GetLevel() const
        {
            return _level.load();
        }

        //member functions
        void Error(const std::string& msg) const
        {
            if(_level.load() >= Level::ERROR)
            {
                std::cerr << "[ERROR] " << msg << "\n";
            }
        }
    
        void Info(const std::string& msg) const
        {
            if(_level.load() >= Level::INFO)
            {
                std::cout << "[INFO] " << msg << "\n";
            }
        }

        void Debug(const std::string& msg) const
        {
            if(_level.load() >= Level::DEBUG)
            {
                std::cout << "[DEBUG] " << msg << "\n";
            }
        }

        

        

    private:
        Logger() : _level(Level::INFO) {}
        ~Logger() = default;

        //forbidden copy
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
};


}