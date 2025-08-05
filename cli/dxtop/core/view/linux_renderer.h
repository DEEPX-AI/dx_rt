// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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