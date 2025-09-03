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
#include <vector>

namespace dxrt {

struct Field
{
    std::string label;
    std::string value;
    size_t width = 0;
    enum class Align { LEFT, CENTER, RIGHT } align = Align::LEFT;
    int colorPair = 0;
    bool bold = false;

    //for Graph Rendering
    bool makeGraph = false;
    double numericValue = 0.0;

    Field(
        const std::string& label,
        const std::string& value,
        size_t width,
        Align align = Align::LEFT,
        int colorPair = 1,
        bool bold = false)
        : label(label), value(value), width(width), align(align), colorPair(colorPair), bold(bold)
    {}
};

struct CoreViewModel
{
    std::vector<Field> fields;
};

struct DeviceViewModel
{
    std::vector<Field> fields;
    std::vector<CoreViewModel> cores;
};

struct MonitorViewModel
{
    std::vector<std::string> headerLines;
    std::vector<DeviceViewModel> devices;
    std::string footerLeft;
    std::string footerRight;
};

}