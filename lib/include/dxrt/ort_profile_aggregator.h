/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * Aggregates per-node timing information from ONNX Runtime's built-in
 * profiler (SessionOptions::EnableProfiling). Each CpuHandle session
 * registers its profile JSON path on destruction; statistics across all
 * sessions are computed and reported when this singleton is destroyed
 * (i.e. at program exit).
 */

#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "dxrt/common.h"

namespace dxrt
{

class DXRT_API OrtProfileAggregator
{
public:
    static OrtProfileAggregator& GetInstance();

    // Whether ORT node-level profiling is enabled for this process.
    // Controlled by env var DXRT_ORT_PROFILING=ON (default OFF).
    bool IsEnabled() const { return _enabled; }

    // Returns a unique file-prefix (without extension) to be passed to
    // SessionOptions::EnableProfiling. ORT will append a timestamp and
    // ".json" to produce the final file name.
    std::string MakeProfilePrefix(const std::string& taskName);

    // Register a profile JSON path emitted by Session::EndProfiling.
    void RegisterProfileFile(const std::string& path);

    OrtProfileAggregator(const OrtProfileAggregator&) = delete;
    OrtProfileAggregator& operator=(const OrtProfileAggregator&) = delete;

private:
    OrtProfileAggregator();
    ~OrtProfileAggregator();

    void aggregateAndReport();

    bool _enabled = false;
    bool _keepRaw = false;
    std::string _outputDir;
    std::mutex _mutex;
    std::vector<std::string> _profileFiles;
    int _prefixCounter = 0;
};

} /* namespace dxrt */
