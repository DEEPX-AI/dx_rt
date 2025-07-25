// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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