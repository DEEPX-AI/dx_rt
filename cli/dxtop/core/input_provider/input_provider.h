/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

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