#pragma once
#include <vector>
#include <string>

#include "runner.h"
#include "utils.h"

using std::vector;

const string PREFIX = "DXBENCHMARK_";

class Reporter
{
    public:
        Reporter(const HostInform& hostInfo, const vector<Result>& res, const std::map<string, vector<std::map<string, vector<int64_t>>>>& ts, const string& resultPath);

        void makeReport();
        void makeData(const string& rtVersion, const string& fwVersion, const string& ddVersion, const string& pdVersion);

    private:
        HostInform _inform;
        vector<Result> _results;
        std::map<string, vector<std::map<string, vector<int64_t>>>> _timeSeries;
        string _resultPath;
        string _currentTime;

};