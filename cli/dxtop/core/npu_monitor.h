/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip> 
#include <csignal>

#include "npu_device.h"
#include "version.h"
#include "util/npu_device_formatter.h"
#include "ipc/dxtop_ipc_client.h"
#include "dxrt/device_info_status.h"
#include "input_provider/input_provider.h"
#include "view/view_model.h"
#include "view/renderer.h"

extern "C" void globalSignalHandler(int signum);  

namespace dxrt {
    
    class NpuMonitor
    {
    public:
        enum class ViewState
        {
            MAIN,
            HELP
        };

        const static uint8_t DEVICES_PER_PAGE = 4;

    public:
        explicit NpuMonitor();
        virtual ~NpuMonitor() = default;

        void Initialize(Renderer& renderer);
        void Run(InputProvider& inputProvider, Renderer& renderer);

        //Pointer to the NpuMonitor instance for the signal handler
        static NpuMonitor* s_instance;  
        static void SignalHandler(const int signum);

    private:
        void updateDevices(MonitorViewModel& monitorViewModel);
        void requestStop();
        void stop(Renderer& renderer);
        void handleInput(InputEvent event, Renderer& renderer);
        void handleMainInput(InputEvent event, Renderer& renderer);
        void handleHelpInput(InputEvent event, Renderer& renderer);

        MonitorViewModel createMonitorViewModel();
        DeviceViewModel createDeviceViewModel(const NpuDevice& device);
        CoreViewModel createCoreViewModel(const NpuCore& core);

    private:
        bool _running;
        MonitorViewModel _monitorViewModel;
        uint8_t _currentPage;
        ViewState _currentView = ViewState::MAIN;
        
        std::vector<std::shared_ptr<dxrt::NpuDevice>> _devices;
        uint8_t _totalDeviceCount;
        dxrt_dev_info_t _dev;

        DXTopIPCClient _ipcClient;
    };
}

