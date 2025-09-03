/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/model.h"
#include "dxrt/inference_engine.h"
#include "dxrt/exception/exception.h"
#include "dxrt/util.h"
#include "dxrt/model_parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string>
#include <memory>
#include <vector>

#include "dxrt/datatype.h"
#include "dxrt/inference_engine.h"
#include "dxrt/exception/exception.h"
#include "dxrt/util.h"
#include "dxrt/filesys_support.h"

#include "dxrt/extern/rapidjson/document.h"
#include "dxrt/extern/rapidjson/writer.h"
#include "dxrt/extern/rapidjson/prettywriter.h"
#include "dxrt/extern/rapidjson/stringbuffer.h"
#include "dxrt/extern/rapidjson/pointer.h"
#include "dxrt/extern/rapidjson/rapidjson.h"

#ifdef __linux__
    #include <cxxabi.h>
#endif

#include "resource/log_messages.h"
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>

using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::to_string;
using std::stoi;
using std::unique_ptr;
using std::vector;
using rapidjson::Document;
using rapidjson::SizeType;
using rapidjson::Value;
using deepx_rmapinfo::GetDataTypeNum;
using deepx_rmapinfo::GetMemoryTypeNum;

namespace dxrt
{

static string dataFormatTable[] =
{
    "NONE",
    "NCHW",
    "NHWC",
    "NHW"
};

ModelDataBase LoadModelParam(string file)
{
    ModelDataBase param;
    LoadModelParam(param, file);

    return param;
}

string LoadModelParam(ModelDataBase& param, string file)
{
    // Use new parser system for version-specific parsing
    try
    {
        auto parser = ModelParserFactory::CreateParser(file);
        LOG_DXRT_DBG << "Using " << parser->GetParserName() << " for file: " << file << std::endl;
        return parser->ParseModel(file, param);
    }
    catch (const std::exception &e)
    {
        // If any error occurs during parsing, propagate to caller
        throw;
    }

    // The actual parsing logic has been moved to version-specific parsers
    // This should not be reached if the factory call above succeeds
    throw InvalidOperationException(EXCEPTION_MESSAGE("LoadModelParam: Parser factory call failed"));
}

ostream& operator<<(ostream& os, const ModelDataBase& m)
{
    auto graphsDb = m.deepx_graph;
    auto binDb = m.deepx_binary;
    auto graphs = graphsDb.subgraphs();
    auto toposortOrder = graphsDb.topoSort_order();
    for (const auto &name : toposortOrder)
    {
        for (auto &graph : graphs)
        {
            if (graph.name() == name)
            {
                // if(binDb.npu_models())
                os << "-- " << graph.name() << endl;
                break;
            }
            else continue;
        }
    }
    return os;
}

std::tuple<int, int, int> convertVersion(const string& vers)
{
    char delimiter = '.';
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(vers);

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return std::make_tuple(std::stoi(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2]));
}

bool isSupporterModelVersion(const string& vers)
{
    auto min_version = convertVersion(std::string(MIN_COMPILER_VERSION));
    auto this_version = convertVersion(vers);
    return this_version >= min_version;
}

} /* namespace dxrt */
