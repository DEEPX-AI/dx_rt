/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 * 
 * This file uses ncurses (MIT License) - Copyright (c) 1998-2018,2019 Free Software Foundation, Inc.
 */

#pragma once

#ifdef __linux__
    #include <ncurses.h>
#endif

#include <vector>

#include "../util/text_formatter.h"
#include "renderer.h"
#include "view_model.h"

namespace dxrt {
    class NcursesRenderer : public Renderer
    {
    public:
        void Initialize() override;
        void RenderMain(const MonitorViewModel& viewModel) override;
        void RenderHelp() override;
        void Refresh() override;
        void Stop() override;

    private:
        void renderDeviceRow(int row, int col, const std::vector<Field>& fields) override;
        void renderDeviceDramUsage(int row, int col, const Field& field);
        void renderCoreRow(int row, int col, const std::vector<Field>& fields) override;
        void renderSeperator(int row) override;
    };
}