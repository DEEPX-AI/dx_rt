// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <vector>

#include "view_model.h"

namespace dxrt {

    class Renderer
    {
    public:
        virtual void Initialize() = 0;
        virtual ~Renderer() = default;
        virtual void RenderMain(const MonitorViewModel& viewModel) = 0;
        virtual void RenderHelp() = 0;
        virtual void Refresh() = 0;
        virtual void Stop() = 0;
    
    private:
        virtual void renderDeviceRow(int row, int col, const std::vector<Field>& fields) = 0;
        virtual void renderCoreRow(int row, int cool, const std::vector<Field>& fields) = 0;
        virtual void renderSeperator(int row) = 0;
    };
}