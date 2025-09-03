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

#include "renderer.h"
#include "linux_renderer.h"

namespace dxrt{

void NcursesRenderer::Initialize()
{  
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    //Set ncurses input to non-blocking mode
    nodelay(stdscr, TRUE);
    curs_set(0);

    if (has_colors())
    {
        start_color();

        // foreground, background 색쌍 초기화
        init_pair(1, COLOR_WHITE,   COLOR_BLACK); 
        init_pair(2, COLOR_GREEN,   COLOR_BLACK); 
        init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
        //Orange
        // init_pair(4, 214, COLOR_BLACK);
        init_pair(5, COLOR_RED,     COLOR_BLACK); 
        init_pair(6, COLOR_CYAN,    COLOR_BLACK);
        init_pair(7, COLOR_MAGENTA, COLOR_BLACK); 
        init_pair(8, COLOR_BLUE, COLOR_BLACK);
        
    }
    else
    {
        mvprintw(0, 0, "Warning: Terminal does not support colors.");
    }
}

void NcursesRenderer::Refresh()
{
    ::refresh();
}

void NcursesRenderer::Stop()
{
    if(!isendwin())
    {
        // Restore terminal to normal mode (clean up ncurses)
        endwin();
    }    
}

void NcursesRenderer::RenderMain(const MonitorViewModel& viewModel)
{
    clear();

    int max_row = 0;
    int max_col = 0;
    getmaxyx(stdscr, max_row, max_col);

    //Header
    int row = 0;
    for (const auto& line : viewModel.headerLines)
    {
        mvprintw(row, 0, "%s", line.c_str());
        row++;
    }

    //Header seperator
    mvhline(row, 0, '-', max_col);
    row++;

    //Devices
    for (const auto& device : viewModel.devices)
    {
        //Device fields in a row
        renderDeviceRow(row, 2, device.fields);
        row++;

        const Field& npu_usage = device.fields[4];
        renderDeviceDramUsage(row, 2, npu_usage);
        row++;

        //Core under the device
        for (const auto& core: device.cores)
        {
            renderCoreRow(row, 4, core.fields);
            row++;
        }

        mvhline(row, 0, '-', max_col);
        row++;
    }
    //Footer seperator
    // mvhline(max_row-2, 0, '-', max_col);

    //Footer Left
    mvprintw(max_row -1, 0, "%s", viewModel.footerLeft.c_str());

    //Footer Right
    mvprintw(max_row -1, max_col - static_cast<int>(viewModel.footerRight.size()), "%s", viewModel.footerRight.c_str());

    refresh();
}

void NcursesRenderer::RenderHelp()
{
    clear();

    std::vector<std::string> help_text = {
    "Help for DX-TOP - Interactive Commands",
    "",
    "  q                      Quit",
    "  h                      Toggle Help View",
    "  n / PageDown / -->     Next page",
    "  p / PageUp   / <--     Previous page",
    "",
    "  (Press 'q' again to return to the main view)",
    };


    for (size_t i = 0; i < help_text.size(); ++i)
    {
        mvprintw(static_cast<int>(i), 0, "%s", help_text[i].c_str());
    }

    refresh();
}

void NcursesRenderer::renderDeviceRow(int row, int col, const std::vector<Field>& fields)
{
    int x = col;
    for (const auto& field : fields)
    {
        if(!field.makeGraph)
        {
        //1) Lable Bold
            attron(A_BOLD | COLOR_PAIR(6));
            mvprintw(row, x, "%s:", field.label.c_str());
            attroff(A_BOLD | COLOR_PAIR(6));

            //2) Value
            attr_t attr = A_NORMAL;
            if(field.bold)
            {
                attr |= A_BOLD;
            }

            x += static_cast<int>(field.label.size()) + 1;

            attron(COLOR_PAIR(field.colorPair) | attr);
            mvprintw(row, x, "%s", TextFormatter::Format(field).c_str());
            attroff(COLOR_PAIR(field.colorPair) | attr);

            x += static_cast<int>(field.width) + 2;
        }
    }
}

void NcursesRenderer::renderDeviceDramUsage(int row, int col, const Field& dramField)
{
    int current_x = col;

    // 1. Print the label
    attron(A_BOLD | COLOR_PAIR(6));
    mvprintw(row, current_x, "%s:", dramField.label.c_str());
    attroff(A_BOLD | COLOR_PAIR(6));

    // Update current_x to be after the label
    current_x += static_cast<int>(dramField.label.size()) + 3; // +x for the colon and space after label

    // 2. Render the Graph
    constexpr int graphWidth = 20; // Let's use 20 for a more compact view as discussed
    int barLength = static_cast<int>((dramField.numericValue / 100.0) * graphWidth);
    if (barLength < 0) barLength = 0; // Ensure non-negative barLength
    if (barLength > graphWidth) barLength = graphWidth;

    attron(COLOR_PAIR(dramField.colorPair)); // Apply color to the graph

    // Print the opening bracket at the calculated position
    mvprintw(row, current_x, "[");
    current_x++; // Move cursor past the '['

    // Print the bar
    for (int i = 0; i < graphWidth; ++i)
    {
        if (i < barLength)
            mvaddch(row, current_x + i, '|'); // Use mvaddch to explicitly place characters
        else
            mvaddch(row, current_x + i, ' ');
    }

    // Print the closing bracket
    mvprintw(row, current_x + graphWidth, "]"); // Place ']' after the graph characters
    attroff(COLOR_PAIR(dramField.colorPair));

    //Update current_x to be after the graph
    current_x += graphWidth + 2;

    // 3. Print the formatted value (e.g., "0 B / 3.92 GiB (0.00%)")
    std::string formattedValue = TextFormatter::Format(dramField);

    attr_t attr = A_NORMAL;
    if (dramField.bold)
    {
        attr |= A_BOLD;
    }

    attron(COLOR_PAIR(dramField.colorPair) | attr);
    mvprintw(row, current_x, "%s", formattedValue.c_str());
    attroff(COLOR_PAIR(dramField.colorPair) | attr);
}

void NcursesRenderer::renderCoreRow(int row, int col, const std::vector<Field>& fields)
{
    int x = col;
    for (const auto& field : fields)
    {
        //1) Lable Bold
        attron(A_BOLD | COLOR_PAIR(7));
        mvprintw(row, x, "%s:", field.label.c_str());
        attroff(A_BOLD | COLOR_PAIR(7));

        //2) Value
        attr_t attr = A_NORMAL;
        if(field.bold)
        {
            attr |= A_BOLD;
        }

        std::string formattedValue = TextFormatter::Format(field);

        x += static_cast<int>(field.label.size()) + 1;

        attron(COLOR_PAIR(field.colorPair) | attr);
        // attron(COLOR_PAIR(1) | attr);
        mvprintw(row, x, "%s", TextFormatter::Format(field).c_str());
        // attroff(COLOR_PAIR(1) | attr);
        attroff(COLOR_PAIR(field.colorPair) | attr);
        

        x += static_cast<int>(field.width) + 2;
    }
}

void NcursesRenderer::renderSeperator(int row)
{
    (void)row;
}

}