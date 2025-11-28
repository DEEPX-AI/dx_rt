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
#elif defined(_WIN32)
    #include <curses.h>
#endif
