/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * Wrapper for model parsing functions via C ABI.
 */
#pragma once
#include "dxrt/dxrt_c_api.h"
#include "dxrt/exception/exception.h"
#include <string>

namespace dxrt {

struct ParseOptions {
    bool verbose = false;
    bool json_extract = false;
    bool no_color = false;       // not yet supported via C ABI
    std::string output_file;     // not yet supported via C ABI
};

inline int ParseModel(const std::string& file)
{
    auto st = dxrt_parse_model(file.c_str());
    if (st != DXRT_OK)
    {
        throw Exception(dxrt_last_error_message());
    }
    return 0;
}

inline int ParseModel(const std::string& file, const ParseOptions& options)
{
    auto st = dxrt_parse_model_with_options(
        file.c_str(), options.verbose ? 1 : 0, options.json_extract ? 1 : 0);
    if (st != DXRT_OK)
    {
        throw Exception(dxrt_last_error_message());
    }
    return 0;
}

}  // namespace dxrt
