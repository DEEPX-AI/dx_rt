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

#include "input_provider.h"
#include <ncurses.h>

namespace dxrt {

    class NcursesInputProvider: public InputProvider
    {
    public:
        InputEvent PollInput() override
        {
            int ch = getch();
            switch (ch)
            {
                case ERR:
                    return InputEvent::NONE;
                case 'q':
                case 'Q':
                    return InputEvent::QUIT;
                case 'h':
                case 'H':
                    return InputEvent::HELP;
                case 'n':
                case KEY_RIGHT:
                case KEY_PPAGE:
                    return InputEvent::NEXT_PAGE;
                case 'p':
                case KEY_LEFT:
                case KEY_NPAGE:
                    return InputEvent::PREV_PAGE;
                default:
                    return InputEvent::NONE;
            }
        }
    };
}