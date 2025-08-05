// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "text_formatter.h"

namespace dxrt {

std::string TextFormatter::Format(const Field& field)
{
    std::string s = field.value;
    if (s.size() >= field.width)
        return s;

    size_t pad = field.width - s.size();
    size_t left = 0, right = 0;

    switch(field.align)
    {
        case Field::Align::LEFT:
            left = 0, right = pad; break;

        case Field::Align::CENTER:
            left = pad/2; right = pad -left; break;

        case Field::Align::RIGHT:
            left = pad; right = 0; break;
    }

    return std::string(left, ' ') + s + std::string(right, ' ');
}

}