// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once



namespace dxrt {
    
    enum class InputEvent
    {
        NONE,
        QUIT,
        HELP,
        NEXT_PAGE,
        PREV_PAGE
    };
    
    class InputProvider
    {
    public:
        virtual ~InputProvider() = default;
        virtual InputEvent PollInput() = 0;
    };
}