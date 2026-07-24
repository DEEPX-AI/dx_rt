#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <cstring>

#include "../include/render.h"
#include "../include/utils.h"

#ifdef _WIN32
#include <direct.h>

#define MKDIR_FUNC(path) _mkdir(path)
#define IS_DIR(st) ((st).st_mode & _S_IFDIR)
#define PATH_SEPARATOR '\\'
#else
#define MKDIR_FUNC(path) mkdir(path, 0755)
#define IS_DIR(st) S_ISDIR((st).st_mode)
#define PATH_SEPARATOR '/'
#endif

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;

bool ensureDirectoryExists(const string& path)
{
    string platformPath = path;
    if (PATH_SEPARATOR != '/') {
        std::replace(platformPath.begin(), platformPath.end(), '/', PATH_SEPARATOR);
    }

    // Check if directory already exists
    struct stat st;
    if (stat(platformPath.c_str(), &st) == 0)
    {
        if (IS_DIR(st))
        {
            return true;  // Directory already exists
        }
        else
        {
            std::cerr << "Path exists but is not a directory: " << platformPath << std::endl;
            return false;
        }
    }

    // Directory doesn't exist, need to create it recursively
    string currentPath;
    size_t pos = 0;
    char separator = PATH_SEPARATOR;

#ifdef _WIN32
    // Handle drive letter on Windows
    if (platformPath.length() >= 2 && platformPath[1] == ':' && (platformPath.length() == 2 || platformPath[2] == separator)) {
        currentPath = platformPath.substr(0, 3);
        pos = 3;
    }
    else
#endif
    // Skip leading slash for absolute paths
    if (!platformPath.empty() && platformPath[0] == '/')
    {
        currentPath = "/";
        pos = 1;
    }

    while (pos < platformPath.length())
    {
        size_t nextSeparator = platformPath.find(separator, pos);
        if (nextSeparator == string::npos)
        {
            nextSeparator = platformPath.length();
        }

        currentPath += platformPath.substr(pos, nextSeparator - pos);

        // Try to create this level of directory
        if (stat(currentPath.c_str(), &st) != 0)
        {
            // Directory doesn't exist, create it
            if (MKDIR_FUNC(currentPath.c_str()) != 0)
            {
                if (errno != EEXIST)
                {
                    std::cerr << "Error creating directory " << currentPath
                        << ": " << std::strerror(errno) << std::endl;
                    return false;
                }
            }
            else
            {
                std::cout << "Created directory: " << currentPath << std::endl;
            }
        }

        if (nextSeparator < platformPath.length())
        {
            currentPath += separator;
            pos = nextSeparator + 1;
        }
        else
        {
            break;
        }
    }

    return true;
}

string sanitizeForJs(string s)
{
    std::replace(s.begin(), s.end(), ' ', '_');
    std::replace(s.begin(), s.end(), '-', '_');
    std::replace(s.begin(), s.end(), '/', '_');
    std::replace(s.begin(), s.end(), ':', '_');
    std::replace(s.begin(), s.end(), '[', '_');
    std::replace(s.begin(), s.end(), ']', '_');
    return s;
}

string escapeJsString(const string& s)
{
    string result;
    for (char c : s)
    {
        if (c == '\'' || c == '\\' || c == '\n' || c == '\r' || c == '\t')
        {
            result += '\\';
        }
        result += c;
    }
    return result;
}

struct PerfStats
{
    double _min = 0;
    double _max = 0;
    double _avg = 0;
    double _p50 = 0;
    double _p99 = 0;
    double _cv = 0;
};

PerfStats computeStats(const vector<double>& values)
{
    PerfStats s;
    if (values.empty())
    {
        return s;
    }

    vector<double> sorted = values;
    std::sort(sorted.begin(), sorted.end());
    size_t n = sorted.size();

    s._min = sorted.front();
    s._max = sorted.back();
    s._avg = std::accumulate(sorted.begin(), sorted.end(), 0.0) / n;

    // p50 (median)
    if (n % 2 == 0)
    {
        s._p50 = (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
    }
    else
    {
        s._p50 = sorted[n / 2];
    }

    // p99
    size_t p99_idx = static_cast<size_t>(std::ceil(n * 0.99)) - 1;
    s._p99 = sorted[(std::min)(p99_idx, n - 1)];

    // CV (Coefficient of Variation) = stddev / mean
    if (s._avg > 0.0 && n > 1)
    {
        double sum_sq_diff = 0.0;
        for (const auto& v : values)
        {
            double diff = v - s._avg;
            sum_sq_diff += diff * diff;
        }
        double stddev = std::sqrt(sum_sq_diff / (n - 1));
        s._cv = stddev / s._avg;
    }

    return s;
}

string getDisplayName(const string& fullPath)
{
    // Extract filename from path
    size_t lastSlash = fullPath.find_last_of("/\\");
    if (lastSlash != string::npos)
    {
        return fullPath.substr(lastSlash + 1);
    }
    return fullPath;
}

string getShortPath(const string& fullPath, size_t maxDepth = 2)
{
    // Get last 'maxDepth' directory components
    vector<string> parts;
    size_t found;
    string temp = fullPath;

    while ((found = temp.find_last_of("/\\")) != string::npos)
    {
        if (found == 0)
        {
            break;
        }
        string part = temp.substr(found + 1);
        if (!part.empty())
        {
            parts.push_back(part);
        }
        temp = temp.substr(0, found);
        if (parts.size() >= maxDepth) break;
    }

    if (parts.empty())
    {
        return fullPath;
    }

    string result;
    for (auto it = parts.rbegin(); it != parts.rend(); ++it)
    {
        if (!result.empty()) result += "/";
        result += *it;
    }
    return result;
}

Reporter::Reporter(const HostInform& inform, const vector<Result>& results,
                   const map<string, map<string, vector<double>>>& timeSeries,
                   const string& resultPath)
    : _inform(inform), _results(results), _timeSeries(timeSeries), _resultPath(resultPath)
{
    _currentTime = getCurrentTime();
}

void Reporter::makeReport()
{
    // Ensure directory exists
    if (!ensureDirectoryExists(_resultPath))
    {
        std::cerr << "Failed to create result directory: " << _resultPath << std::endl;
        return;
    }

    std::string outputName = _resultPath + "/" + PREFIX + _currentTime + ".html";
    std::ofstream report_file(outputName);

    report_file << R"(
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>Benchmark Report</title>
    <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; background-color: #f8f9fa; color: #212529; line-height: 1.6; margin: 0; padding: 20px; }
        .container { max-width: 1200px; margin: 20px auto; background-color: #ffffff; border-radius: 8px; box-shadow: 0 4px 15px rgba(0,0,0,0.05); padding: 30px 40px; }
        h1 { text-align: center; color: #0165B3; font-weight: 600; margin-bottom: 15px; }
        h2 { color: #0165B3; border-bottom: 2px solid #e9ecef; padding-bottom: 10px; margin-top: 40px; }
        h3 { color: #343a40; margin-top: 30px; border-left: 4px solid #0165B3; padding-left: 10px;}
        .host-info { background-color: #f8f9fa; border: 1px solid #e9ecef; border-radius: 6px; padding: 20px; margin: 30px 0; display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 15px; }
        .info-item { display: flex; flex-direction: column; }
        .info-item .label { font-size: 0.9em; color: #6c757d; font-weight: 500; }
        .info-item .value { font-size: 1.1em; color: #343a40; font-weight: 600; }
        table { width: 100%; border-collapse: collapse; margin-top: 20px; }
        th, td { border: 1px solid #dee2e6; padding: 12px 15px; text-align: center; vertical-align: middle; }
        th { background-color: #0165B3; color: white; font-weight: 500; }
        .plot-container { width: 100%; margin: 20px auto; }
    </style>
</head>
<body>
    <div class="container">
        <h1>DXBenchmark Report</h1>
    )";

    report_file << R"(
        <div class="host-info">
            <div class="info-item"><span class="label">Architecture</span><span class="value">)" << _inform.arch << R"(</span></div>
            <div class="info-item"><span class="label">CPU Model</span><span class="value">)" << _inform.coreModel << R"(</span></div>
            <div class="info-item"><span class="label">CPU Cores</span><span class="value">)" << _inform.numCore << R"(</span></div>
            <div class="info-item"><span class="label">Memory Size</span><span class="value">)" << _inform.memSize << R"(</span></div>
            <div class="info-item"><span class="label">Operating System</span><span class="value">)" << _inform.os << R"(</span></div>
        </div>
    )";

    const size_t chunkSize = 30;
    size_t numChunks = (_results.size() + chunkSize - 1) / chunkSize;

    report_file << "<h2>Performance Summary by Model (Total: " << _results.size() << " Models)</h2>";

    report_file << "<h3>FPS Comparision</h3>";
    for (size_t i = 0; i < numChunks; ++i) {
        report_file << R"(<div id="fps-plot-)" << i << R"(" class="plot-container"></div>)";
    }

    report_file << "<h3>NPU Inference Time Comparison</h3>";
    for (size_t i = 0; i < numChunks; ++i) {
        report_file << R"(<div id="npu-plot-)" << i << R"(" class="plot-container"></div>)";
    }

    report_file << "<h3>E2E Latency Comparison</h3>";
    for (size_t i = 0; i < numChunks; ++i) {
        report_file << R"(<div id="latency-plot-)" << i << R"(" class="plot-container"></div>)";
    }

    map<string, map<string, const vector<double>*>> restructuredData;
    size_t maxLoopCount = 0;
    for (const auto& modelEntry : _timeSeries) {
        const string& modelName = modelEntry.first;
        const auto& perfMap = modelEntry.second;
        for (const auto& taskEntry : perfMap) {
            const string& taskName = taskEntry.first;
            const vector<double>& values = taskEntry.second;
            restructuredData[taskName][modelName] = &values;
            if (values.size() > maxLoopCount) {
                maxLoopCount = values.size();
            }
        }
    }

    if (!restructuredData.empty()) {
        report_file << R"(<h2>Performance Metrics Over Loops</h2>)";
        for (const auto& plotEntry : restructuredData) {
            const string& taskName = plotEntry.first;
            string plotId = "timeseries_" + sanitizeForJs(taskName) + "_plot";
            report_file << "<h3>" << taskName << "</h3>\n";
            report_file << R"(<div id=")" << plotId << R"(" class="plot-container"></div>)" << "\n";
        }
    }

    report_file << R"(
        <h2>Benchmark Results</h2>
        <table>
            <thead>
                <tr>
                    <th rowspan="2">Model Name</th>
                    <th rowspan="2">FPS</th>
                    <th colspan="6">NPU Inference Time (ms)</th>
                    <th colspan="6">E2E Latency (ms)</th>
                </tr>
                <tr>
                    <th>Min</th><th>Max</th><th>Avg</th><th>P50</th><th>P99</th><th>CV</th>
                    <th>Min</th><th>Max</th><th>Avg</th><th>P50</th><th>P99</th><th>CV</th>
                </tr>
            </thead>
            <tbody>
    )";
    for (const auto& result : _results) {
        string displayName = getShortPath(result.modelName.first, 2);
        const string& modelKey = result.modelName.first;

        PerfStats npu_stats;
        PerfStats e2e_stats;
        auto tsIt = _timeSeries.find(modelKey);
        if (tsIt != _timeSeries.end())
        {
            const auto& perfMap = tsIt->second;
            auto npuIt = perfMap.find("NPU Inference Time");
            if (npuIt != perfMap.end())
            {
                npu_stats = computeStats(npuIt->second);
            }
            auto e2eIt = perfMap.find("E2E Latency");
            if (e2eIt != perfMap.end())
            {
                e2e_stats = computeStats(e2eIt->second);
            }
        }

        report_file << std::fixed << std::setprecision(2);
        report_file << "<tr>"
                    << "<td title='" << escapeJsString(result.modelName.first) << "'>" << displayName << "</td>"
                    << "<td>" << result.fps << "</td>"
                    << "<td>" << npu_stats._min << "</td>"
                    << "<td>" << npu_stats._max << "</td>"
                    << "<td>" << npu_stats._avg << "</td>"
                    << "<td>" << npu_stats._p50 << "</td>"
                    << "<td>" << npu_stats._p99 << "</td>"
                    << "<td>" << npu_stats._cv << "</td>"
                    << "<td>" << e2e_stats._min << "</td>"
                    << "<td>" << e2e_stats._max << "</td>"
                    << "<td>" << e2e_stats._avg << "</td>"
                    << "<td>" << e2e_stats._p50 << "</td>"
                    << "<td>" << e2e_stats._p99 << "</td>"
                    << "<td>" << e2e_stats._cv << "</td>"
                    << "</tr>";
    }
    report_file << "</tbody></table>";

    report_file << "<script>";

    float maxFps = 0.0f;
    float maxNpuTime = 0.0f;
    float maxLatency = 0.0f;
    for (const auto& result : _results) {
        if (result.fps > maxFps) maxFps = result.fps;
        if ((result.infTime.mean + result.infTime.sd) > maxNpuTime) maxNpuTime = result.infTime.mean + result.infTime.sd;
        if ((result.latency.mean + result.latency.sd) > maxLatency) maxLatency = result.latency.mean + result.latency.sd;
    }
    maxFps *= 1.1f;
    maxNpuTime *= 1.1f;
    maxLatency *= 1.1f;

    size_t modelsPerChunk = (std::min)(chunkSize, _results.size());
    int barPlotHeight = (std::min)(250 + (int)modelsPerChunk * 30, 1200);
    report_file << "const barPlotHeight = " << barPlotHeight << ";\n";
    report_file << "const commonConfig = { responsive: true, displayModeBar: false };\n";

    report_file << "const maxFpsValue = " << maxFps << ";\n";
    report_file << "const maxNpuTimeValue = " << maxNpuTime << ";\n";
    report_file << "const maxLatencyValue = " << maxLatency << ";\n";

    for (size_t i = 0; i < numChunks; ++i) {
        size_t start = i * chunkSize;
        size_t end = (std::min)(start + chunkSize, _results.size());
        std::string chartTitle = "Models " + std::to_string(start + 1) + " - " + std::to_string(end);

        report_file << "const models_fps_" << i << " = [";
        for (size_t j = start; j < end; ++j) {
            string shortName = getShortPath(_results[j].modelName.first, 2);
            report_file << "'" << escapeJsString(shortName) << "'" << (j == end - 1 ? "" : ",");
        }
        report_file << "];\n";
        report_file << "const fpsData_" << i << " = [";
        for (size_t j = start; j < end; ++j) { report_file << _results[j].fps << (j == end - 1 ? "" : ","); }
        report_file << "];\n";
        report_file << "document.getElementById('fps-plot-" << i << "').style.height = barPlotHeight + 'px';";
        report_file << "const fpsLayout_" << i << " = { "
                    << "margin: { l: 250, r: 40, t: 40, b: 50 }, font: { size: 12, color: '#333' }, bargap: 0.15, "
                    << "xaxis: { title: 'FPS (Higher is Better)', gridcolor: '#e9ecef', range: [0, maxFpsValue] }, "
                    << "yaxis: { automargin: true, tickfont: {size: 10}, autorange: 'reversed'}, "
                    << "title: { text: '" << chartTitle << "' }, "
                    << "annotations: [] };";
        for (size_t j = 0; j < (end - start); ++j) { report_file << "fpsLayout_" << i << ".annotations.push({ y: models_fps_" << i << "[" << j << "], x: fpsData_" << i << "[" << j << "], text: '<b>' + fpsData_" << i << "[" << j << "].toFixed(2) + '</b>', xanchor: 'left', showarrow: false, font: { color: '#2ca02c', size: 12 } });\n"; }
        report_file << "Plotly.newPlot('fps-plot-" << i << "', [{ y: models_fps_" << i << ", x: fpsData_" << i << ", type: 'bar', orientation: 'h', marker: { color: '#2ca02c' } }], fpsLayout_" << i << ", commonConfig);\n";
    }

    for (size_t i = 0; i < numChunks; ++i) {
        size_t start = i * chunkSize;
        size_t end = (std::min)(start + chunkSize, _results.size());
        std::string chartTitle = "Models " + std::to_string(start + 1) + " - " + std::to_string(end);

        report_file << "const models_npu_" << i << " = [";
        for (size_t j = start; j < end; ++j) {
            string shortName = getShortPath(_results[j].modelName.first, 2);
            report_file << "'" << escapeJsString(shortName) << "'" << (j == end - 1 ? "" : ",");
        }
        report_file << "];\n";
        report_file << "const npuMeans_" << i << " = [";
        for (size_t j = start; j < end; ++j) { report_file << _results[j].infTime.mean << (j == end - 1 ? "" : ","); }
        report_file << "];\n";
        report_file << "const npuStdDevs_" << i << " = [";
        for (size_t j = start; j < end; ++j) { report_file << _results[j].infTime.sd << (j == end - 1 ? "" : ","); }
        report_file << "];\n";
        report_file << "document.getElementById('npu-plot-" << i << "').style.height = barPlotHeight + 'px';";
        report_file << "const npuLayout_" << i << " = { "
                    << "margin: { l: 250, r: 40, t: 40, b: 50 }, font: { size: 12, color: '#333' }, bargap: 0.15, "
                    << "xaxis: { title: 'Average Inference Time (ms, Lower is Better)', gridcolor: '#e9ecef', range: [0, maxNpuTimeValue] }, " // range 속성 추가
                    << "yaxis: { automargin: true, tickfont: {size: 10}, autorange: 'reversed'}, "
                    << "title: { text: '" << chartTitle << "' }, "
                    << "annotations: [] };";
        for (size_t j = 0; j < (end - start); ++j) { report_file << "npuLayout_" << i << ".annotations.push({ y: models_npu_" << i << "[" << j << "], x: npuMeans_" << i << "[" << j << "] + npuStdDevs_" << i << "[" << j << "], text: '<b>' + npuMeans_" << i << "[" << j << "].toFixed(2) + '</b>', xanchor: 'left', showarrow: false, font: { color: '#0165B3', size: 12 } });\n"; }
        report_file << "Plotly.newPlot('npu-plot-" << i << "', [{ y: models_npu_" << i << ", x: npuMeans_" << i << ", error_x: { array: npuStdDevs_" << i << ", visible: true }, type: 'bar', orientation: 'h', marker: { color: '#0165B3' } }], npuLayout_" << i << ", commonConfig);\n";
    }

    for (size_t i = 0; i < numChunks; ++i) {
        size_t start = i * chunkSize;
        size_t end = (std::min)(start + chunkSize, _results.size());
        std::string chartTitle = "Models " + std::to_string(start + 1) + " - " + std::to_string(end);

        report_file << "const models_latency_" << i << " = [";
        for (size_t j = start; j < end; ++j) {
            string shortName = getShortPath(_results[j].modelName.first, 2);
            report_file << "'" << escapeJsString(shortName) << "'" << (j == end - 1 ? "" : ",");
        }
        report_file << "];\n";
        report_file << "const latencyMeans_" << i << " = [";
        for (size_t j = start; j < end; ++j) { report_file << _results[j].latency.mean << (j == end - 1 ? "" : ","); }
        report_file << "];\n";
        report_file << "const latencyStdDevs_" << i << " = [";
        for (size_t j = start; j < end; ++j) { report_file << _results[j].latency.sd << (j == end - 1 ? "" : ","); }
        report_file << "];\n";
        report_file << "document.getElementById('latency-plot-" << i << "').style.height = barPlotHeight + 'px';";
        report_file << "const latencyLayout_" << i << " = { "
                    << "margin: { l: 250, r: 40, t: 40, b: 50 }, font: { size: 12, color: '#333' }, bargap: 0.15, "
                    << "xaxis: { title: 'Average E2E Latency (ms, Lower is Better)', gridcolor: '#e9ecef', range: [0, maxLatencyValue] }, "
                    << "yaxis: { automargin: true, tickfont: {size: 10}, autorange: 'reversed'}, "
                    << "title: { text: '" << chartTitle << "' }, "
                    << "annotations: [] };";
        for (size_t j = 0; j < (end - start); ++j) { report_file << "latencyLayout_" << i << ".annotations.push({ y: models_latency_" << i << "[" << j << "], x: latencyMeans_" << i << "[" << j << "] + latencyStdDevs_" << i << "[" << j << "], text: '<b>' + latencyMeans_" << i << "[" << j << "].toFixed(2) + '</b>', xanchor: 'left', showarrow: false, font: { color: '#0165B3', size: 12 } });\n"; }
        report_file << "Plotly.newPlot('latency-plot-" << i << "', [{ y: models_latency_" << i << ", x: latencyMeans_" << i << ", error_x: { array: latencyStdDevs_" << i << ", visible: true }, type: 'bar', orientation: 'h', marker: { color: '#0165B3' } }], latencyLayout_" << i << ", commonConfig);\n";
    }
    if (!restructuredData.empty()) {
        report_file << R"(
        const lineCommonLayout = {
            margin: { l: 50, r: 40, t: 50, b: 50 }, font: { size: 12, color: '#333' },
            xaxis: { gridcolor: '#e9ecef', title: 'Loop Count' }, yaxis: { automargin: true, title: 'Time (ms)' },
            legend: { x: 1.02, xanchor: 'left', y: 1, bgcolor: 'rgba(255, 255, 255, 0.7)', bordercolor: '#e9ecef', borderwidth: 1 }
        };
        )";
        report_file << "const loopIndices = Array.from({length: " << maxLoopCount << "}, (_, i) => i + 1);\n";
        for (const auto& plotEntry : restructuredData) {
            const string& taskName = plotEntry.first;
            const string sanitizedTaskName = sanitizeForJs(taskName);
            const string plotId = "timeseries_" + sanitizedTaskName + "_plot";
            report_file << "const " << sanitizedTaskName << "_traces = [];\n";
            for (const auto& modelData : plotEntry.second) {
                const string& modelName = modelData.first;
                string shortName = getShortPath(modelName, 2);
                const vector<double>* values = modelData.second;
                report_file << "{\n    let trace = {};\n";
                report_file << "    trace.x = loopIndices.slice(0, " << values->size() << ");\n";
                report_file << "    trace.y = [";
                for (size_t i = 0; i < values->size(); ++i) {
                    report_file << (*values)[i] << (i == values->size() - 1 ? "" : ",");
                }
                report_file << "];\n";
                report_file << "    trace.name = '" << escapeJsString(shortName) << "';\n";
                report_file << "    trace.type = 'scatter';\n";
                report_file << "    trace.mode = 'lines+markers';\n";
                report_file << "    " << sanitizedTaskName << "_traces.push(trace);\n}\n";
            }
            report_file << "const " << sanitizedTaskName << "_layout = { ...lineCommonLayout };\n";
            report_file << "Plotly.newPlot('" << plotId << "', " << sanitizedTaskName << "_traces, " << sanitizedTaskName << "_layout, commonConfig);\n";
        }
    }

    report_file << R"(
    </script>

    <h2>Metrics Glossary</h2>
    <div style="background-color: #f8f9fa; border-radius: 6px; padding: 25px; margin-top: 30px;">
        <div style="display: grid; gap: 20px;">
            <div style="border-left: 4px solid #0165B3; padding-left: 15px;">
                <h4 style="margin: 0 0 8px 0; color: #0165B3; font-size: 1.1em;">FPS (Frames Per Second)</h4>
                <p style="margin: 0; color: #495057; line-height: 1.6;">The number of frames the model processes per second. Higher values indicate better throughput performance.</p>
            </div>

            <div style="border-left: 4px solid #0165B3; padding-left: 15px;">
                <h4 style="margin: 0 0 8px 0; color: #0165B3; font-size: 1.1em;">NPU Inference Time</h4>
                <p style="margin: 0; color: #495057; line-height: 1.6;">The time taken for the NPU Core to complete an inference operation. Lower values indicate faster processing on the NPU hardware.</p>
            </div>

            <div style="border-left: 4px solid #0165B3; padding-left: 15px;">
                <h4 style="margin: 0 0 8px 0; color: #0165B3; font-size: 1.1em;">E2E Latency (End-to-End Latency)</h4>
                <p style="margin: 0; color: #495057; line-height: 1.6;">The total time for a single inference, including NPU task (data transfer + inference) and CPU task processing. Lower values indicate better end-to-end performance.</p>
            </div>

            <div style="border-left: 4px solid #6c757d; padding-left: 15px;">
                <h4 style="margin: 0 0 8px 0; color: #6c757d; font-size: 1.1em;">P50 / P99 (Percentiles)</h4>
                <p style="margin: 0; color: #495057; line-height: 1.6;">P50 is the median value (50th percentile), representing typical performance. P99 is the 99th percentile, representing worst-case performance excluding outliers.</p>
            </div>

            <div style="border-left: 4px solid #6c757d; padding-left: 15px;">
                <h4 style="margin: 0 0 8px 0; color: #6c757d; font-size: 1.1em;">CV (Coefficient of Variation)</h4>
                <p style="margin: 0; color: #495057; line-height: 1.6;">The ratio of the standard deviation to the mean (CV = &sigma; / &mu;). A lower CV indicates more stable and consistent performance. Values below 0.1 generally suggest low jitter.</p>
            </div>
        </div>
    </div>

    </div>
</body>
</html>
    )";
    report_file.close();
}

void Reporter::makeData(const string& rtVersion, const string& fwVersion, const string& ddVersion, const string& pdVersion)
{

    if (_results.empty()) {
        std::cout << "No Available Data" << std::endl;
        return;
    }

    // Ensure directory exists
    if (!ensureDirectoryExists(_resultPath)) {
        std::cerr << "Failed to create result directory: " << _resultPath << std::endl;
        return;
    }

    std::string csvFilename = _resultPath + "/" + PREFIX + _currentTime + ".csv";
    std::ofstream csvFile(csvFilename, std::ios_base::app);

    if (!csvFile.is_open()) {
        std::cerr << "Cannot Open File: " << csvFilename << std::endl;
    } else {
        csvFile.seekp(0, std::ios::end);
        if (csvFile.tellp() == 0) {
            csvFile << "Runtime Version, Firmware Version, Device Driver Version,PCIe Driver Version,Model Name,FPS,NPU Inference Time Mean,NPU Inference Time SD,NPU Inference Time CV,Latency Mean,Latency SD,Latency CV\n";
        }

        for (const auto& result : _results) {
            csvFile << rtVersion << ","
                    << fwVersion << ","
                    << ddVersion << ","
                    << pdVersion << ","
                    << result.modelName.first << ","
                    << std::fixed << std::setprecision(2) << result.fps << ","
                    << result.infTime.mean << ","
                    << result.infTime.sd << ","
                    << result.infTime.cv << ","
                    << result.latency.mean << ","
                    << result.latency.sd << ","
                    << result.latency.cv << "\n";
        }
        csvFile.close();
    }

    std::string jsonFilename = _resultPath + "/" + PREFIX + _currentTime + ".json";
    std::ofstream jsonFile(jsonFilename);
    if (!jsonFile.is_open()) {
        std::cerr << "[Error]: " <<  " Cannot open file:" << jsonFilename << std::endl;
    } else {
        jsonFile << "{\n";

        jsonFile << "  \"Runtime Version\": \"" << rtVersion << "\",\n";
        jsonFile << "  \"Firmware Version\": \"" << fwVersion << "\",\n";
        jsonFile << "  \"Device Driver Version\": \"" << ddVersion << "\",\n";
        jsonFile << "  \"PCIe Driver Version\": \"" << pdVersion << "\",\n";

        jsonFile << "  \"results\": [\n";

        for (size_t i = 0; i < _results.size(); ++i) {
            const auto& result = _results[i];

            jsonFile << "    {\n";

            jsonFile << "      \"Model Name\": \"" << result.modelName.first << "\",\n";
            jsonFile << "      \"FPS\": " << std::fixed << std::setprecision(2) << result.fps << ",\n";
            jsonFile << "      \"NPU Inference Time\": {\n";
            jsonFile << "        \"mean\": " << result.infTime.mean << ",\n";
            jsonFile << "        \"sd\": " << result.infTime.sd << ",\n";
            jsonFile << "        \"cv\": " << result.infTime.cv << "\n";
            jsonFile << "      },\n";
            jsonFile << "      \"Latency\": {\n";
            jsonFile << "        \"mean\": " << result.latency.mean << ",\n";
            jsonFile << "        \"sd\": " << result.latency.sd << ",\n";
            jsonFile << "        \"cv\": " << result.latency.cv << "\n";
            jsonFile << "      }\n";

            jsonFile << "    }";

            if (i < _results.size() - 1) {
                jsonFile << ",\n";
            } else {
                jsonFile << "\n";
            }
        }

        jsonFile << "  ]\n";
        jsonFile << "}\n";

        jsonFile.close();
    }
}
