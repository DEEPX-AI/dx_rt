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

#include "unicode_literal_support.h"

#include<string>
#include<vector>

#if _WIN32
#include <tchar.h>
#include <windows.h>
#endif



#ifdef __linux__

std::string dxrt::convertLiteralUTF8(const char* ch)
{
	return std::string(ch);
}

#elif _WIN32


std::string dxrt::convertLiteralUTF8(const char* ch)
{

	int char_size1 = MultiByteToWideChar(CP_UTF8, NULL, ch, -1, nullptr, 0);

	std::vector<wchar_t> buffer(char_size1);

	MultiByteToWideChar(CP_UTF8, NULL, ch, -1, buffer.data(), char_size1);

	int char_size2 = WideCharToMultiByte(CP_ACP, NULL, buffer.data(), -1, nullptr, 0, NULL, NULL);

	std::vector<char> buffer2(char_size2);

	WideCharToMultiByte(CP_ACP, NULL, buffer.data(), -1, buffer2.data(), char_size2, NULL, NULL);

	return std::string(buffer2.data());
}

#endif
