/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <string>
#include <cstdint>
#include "../view/view_model.h"

namespace dxrt {

    class TextFormatter
    {
    public:

        static std::string Format(const Field& field);
        
    private:
        //no constructor
        TextFormatter() = delete; 
    };

}