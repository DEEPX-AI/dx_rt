// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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