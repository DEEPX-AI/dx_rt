/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "npu_monitor.h"

#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include "dxrt/device_pool.h"
#include "dxrt/device_core.h"
#include "util/unicode_literal_support.h"

namespace dxrt {

NpuMonitor* NpuMonitor::s_instance = nullptr;

NpuMonitor::NpuMonitor()
: _running(false), _currentPage(0), _currentView(ViewState::MAIN)
{
    DevicePool::GetInstance().InitCores();
    int deviceCount = DevicePool::GetInstance().GetDeviceCount();

    //std::vector<std::shared_ptr<dxrt::Device>> rawDevices = CheckDevices(SkipMode::COMMON_SKIP, dxrt::dxrt_ident_sub_cmd_t::DX_IDENTIFY_NONE);

    _devices.clear();
    _devices.reserve(deviceCount);

    uint8_t index = 0;

    for (int i = 0; i < deviceCount; i++)
    {
        auto deviceCore = DevicePool::GetInstance().GetDeviceCores(i);
        _devices.emplace_back(std::make_shared<dxrt::NpuDevice>(index++, deviceCore, _ipcClient));
    }

    _totalDeviceCount = _devices.size();

    if (_totalDeviceCount == 0)
    {
        _dev.rt_drv_ver.driver_version = 0;
        _dev.rt_drv_ver.driver_version_suffix[0] = 0;
        _dev.pcie.driver_version = 0;
    }

    else
    {
        _dev = _devices[0]->GetDevInfo();
    }
}

void NpuMonitor::Initialize(Renderer& renderer)
{
    std::srand((unsigned)std::time(nullptr));

    // Store current instance in static pointer
    s_instance = this;
    // Instance pointer for Signal handler
    signal(SIGINT, NpuMonitor::SignalHandler);
    signal(SIGTERM, NpuMonitor::SignalHandler);

    renderer.Initialize();
}

void NpuMonitor::Run(InputProvider& inputProvider, Renderer& renderer)
{
    _running = true;

    this->updateDevices(_monitorViewModel);
    renderer.RenderMain(_monitorViewModel);

    // Update per 2 seconds
    const std::chrono::milliseconds update_interval(2000);
    auto last_update_time = std::chrono::steady_clock::now();

    while (_running)
    {
        auto event = inputProvider.PollInput();
        handleInput(event, renderer);

        if (_currentView == ViewState::MAIN)
        {
            auto current_time = std::chrono::steady_clock::now();
            if (current_time - last_update_time >= update_interval)
            {
                updateDevices(_monitorViewModel);
                renderer.RenderMain(_monitorViewModel);
                last_update_time = current_time;
            }
        }

        // Provent CPU busy-waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }

    this->stop(renderer);
}

void NpuMonitor::SignalHandler(const int signum)
{
     if ((signum == SIGINT || signum == SIGTERM) && s_instance != nullptr)
    {
        s_instance->requestStop();
    }
}

void NpuMonitor::updateDevices(MonitorViewModel& monitorViewModel)
{
    for (const auto& device : _devices)
    {
        // IPC Call
        device->UpdateDramUsageByIPC(_ipcClient);
        // cout<<"Device Variant = "<<device->GetDeviceVariant()<< endl;

        device->UpdateCoreData(_ipcClient);
    }
    monitorViewModel = createMonitorViewModel();
}

void NpuMonitor::requestStop()
{
    _running = false;
}

void NpuMonitor::stop(Renderer& renderer)
{
    _running = false;

    renderer.Stop();

    if (s_instance == this)
    {
        s_instance = nullptr;
    }
}

void NpuMonitor::handleInput(InputEvent event, Renderer& renderer)
{
    switch (_currentView)
    {
    case ViewState::MAIN:
        handleMainInput(event, renderer);
        break;

    case ViewState::HELP:
        handleHelpInput(event, renderer);
        break;

    default:
        break;
    }
}

void NpuMonitor::handleMainInput(InputEvent event, Renderer& renderer)
{
    uint8_t totalPages = (_totalDeviceCount + DEVICES_PER_PAGE - 1) / DEVICES_PER_PAGE;

    switch (event)
    {
        case InputEvent::QUIT:
            this->requestStop();
            break;

        case InputEvent::HELP:
            _currentView = ViewState::HELP;
            renderer.RenderHelp();
            break;

        case InputEvent::NEXT_PAGE:
            {
                if (_currentPage < totalPages - 1)
                {
                    _currentPage++;
                }
                // for immediate view rendering
                renderer.RenderMain(_monitorViewModel);
                break;
            }

        case InputEvent::PREV_PAGE:
            {
                if (_currentPage > 0)
                {
                    _currentPage--;
                }
                // for immediate view rendering
                renderer.RenderMain(_monitorViewModel);
                break;
            }

        case InputEvent::NONE:
            break;
    }
}

void NpuMonitor::handleHelpInput(InputEvent event, Renderer& renderer)
{

    switch (event)
    {
        case InputEvent::QUIT:
        {
            _currentView = ViewState::MAIN;
            renderer.RenderMain(_monitorViewModel);
            break;
        }

        default:
            renderer.RenderHelp();
            break;
    }
}

MonitorViewModel NpuMonitor::createMonitorViewModel()
{
    MonitorViewModel view_model;

    // Header
    view_model.headerLines.push_back("[DX-TOP]  (q) Quit   (n) Next Page   (p) Prev Page   (h) Help");

    auto _now = std::chrono::system_clock::now();
    time_t now = std::chrono::system_clock::to_time_t(_now);
    view_model.headerLines.push_back(std::string(std::ctime(&now)));

    std::string version_string;
    version_string = "DX-RT: " + std::string(DXRT_VERSION) + \
            "\t  NPU Device driver: v" + NpuDeviceFormatter::FormatRTDriverVersion(_dev.rt_drv_ver.driver_version) + \
            "\tDX-TOP: v" + std::string(DX_TOP_VERSION);

    view_model.headerLines.push_back(version_string);

    uint8_t start_device_idx = _currentPage * DEVICES_PER_PAGE;
    uint8_t end_device_idx = start_device_idx + DEVICES_PER_PAGE;
    if (end_device_idx > _totalDeviceCount)
    {
        end_device_idx = _totalDeviceCount;
    }

    // Footer
    view_model.footerLeft = "Total Devices: " + std::to_string(_totalDeviceCount);
    uint8_t totalPages = (_totalDeviceCount + DEVICES_PER_PAGE - 1) / DEVICES_PER_PAGE;

    // Prepare page information string (e.g., "Page: 1 / 5")
    // Display page information at the bottom-right corner of the terminal
    view_model.footerRight = "Page: " + std::to_string(_currentPage + 1) + " / " + std::to_string(totalPages);


    // Devices
    for (size_t i = start_device_idx; i < end_device_idx; ++i)
    {
        const NpuDevice& device = *(_devices[i]);
        view_model.devices.push_back(createDeviceViewModel(device));
    }

    return view_model;
}

DeviceViewModel NpuMonitor::createDeviceViewModel(const NpuDevice& device)
{
    DeviceViewModel view_models;

    // Device fields

    // Device Number
    view_models.fields.emplace_back(Field{
        "Device ",
        std::to_string(device.GetDeviceNumber()),
        5,
        Field::Align::LEFT,
        1,
        false
    });

    // Device Variant
    view_models.fields.emplace_back(Field{
        "Variant",
        NpuDeviceFormatter::FormatDeviceVariant(device.GetDeviceVariant()),
        10,
        Field::Align::CENTER,
        1,
        false
    });

    // PCIE Bus Number
    view_models.fields.emplace_back(Field{
        "PCIe Bus Number",
        device.GetPcieBusNumber(),
        12,
        Field::Align::CENTER,
        1,
        false
    });
    // Device Type
    // view_models.fields.emplace_back(Field{
    //     "Type",
    //     NpuDeviceFormatter::FormatDeviceType(device.GetDeviceType()),
    //     10,
    //     Field::Align::CENTER,
    //     1,
    //     false
    // });

    // Device Firmware
    view_models.fields.emplace_back(Field{
        "Firmware",
        NpuDeviceFormatter::FormatFirmwareVersion(device.GetFirmwareVersion()),
        10,
        Field::Align::CENTER,
        1,
        false
    });

    // NPU DRAM USAGE
    uint64_t dram_usage_bytes = device.GetDramUsage();
    uint64_t total_usable_bytes = device.GetTotalUsableMemory();
    double usage_percent = (total_usable_bytes > 0)
                          ? (static_cast<double>(dram_usage_bytes) / static_cast<double>(total_usable_bytes) * 100.0)
                          : 0.0;

    std::ostringstream dram_stream;
    dram_stream << NpuDeviceFormatter::FormatCapacity(dram_usage_bytes)
                << " / "
                << NpuDeviceFormatter::FormatCapacity(total_usable_bytes)
                << " (" << std::fixed << std::setprecision(2) << usage_percent << "%)";

    Field dram_field{
        "NPU Memory",
        dram_stream.str(),
        30,
        Field::Align::LEFT,
        2,
        false
    };

    dram_field.makeGraph = true;
    dram_field.numericValue = usage_percent;

    if (usage_percent < 20)
    {
        dram_field.colorPair = 8;//Blue
    }
    else if (usage_percent < 40)
    {
        dram_field.colorPair = 2;//Green
    }
    else if (usage_percent < 80)
    {
        dram_field.colorPair = 3;//Yellow
    }
    else
    {
        dram_field.colorPair = 5;//Red
    }

    view_models.fields.push_back(std::move(dram_field));

    //Cores
    for (const auto& corePtr : device.GetCores())
    {
        view_models.cores.push_back(createCoreViewModel(*corePtr));
    }

    return view_models;
}

CoreViewModel NpuMonitor::createCoreViewModel(const NpuCore& core)
{
    CoreViewModel view_models;

    // Core fields

    // Core #
    view_models.fields.push_back(Field{
        "Core ",
        std::to_string(core.GetCoreNumber()),
        2,
        Field::Align::LEFT,
        1,
        false
    });

    // Utilization
    const double utilization = std::min(100.0, core.GetUtilization() / 10.0);
    std::ostringstream utilization_stream;
    utilization_stream <<std::fixed << std::setprecision(1) << std::setw(5) << utilization << "%";

    Field utilization_field{
        "Util",
        utilization_stream.str(),
        8,
        Field::Align::CENTER,
        1,
        false
    };


    //Utilization color logic here

    view_models.fields.push_back(std::move(utilization_field));


    //Temperature
    int32_t temperature = core.GetTemperature();
    std::ostringstream temperature_stream;
    temperature_stream << std::setw(3) << core.GetTemperature() << convertLiteralUTF8(u8" \u00B0C");

    Field temperature_field{
        "Temp",
        temperature_stream.str(),
        8,
        Field::Align::CENTER,
        1,
        false
    };

    if (temperature < 50) {
        temperature_field.colorPair = 2;//Green
    }
    else if (temperature < 90) {
        temperature_field.colorPair = 3;//Yellow
    }
    else {
        temperature_field.colorPair = 5;//Red
    }

    view_models.fields.push_back(std::move(temperature_field));

    //Voltage
    std::ostringstream voltage_stream;
    voltage_stream << std::setw(4) << core.GetVoltage() << " mV";
    view_models.fields.emplace_back(Field{
        "Voltage",
        voltage_stream.str(),
        8,
        Field::Align::CENTER,
        1,
        false
    });

    //Clock
    std::ostringstream clock_stream;
    clock_stream << std::setw(4) << core.GetClock() << " MHz";
    view_models.fields.emplace_back(Field{
        "Clock",
        clock_stream.str(),
        10,
        Field::Align::CENTER,
        1,
        false
    });

    return view_models;
}


}

extern "C" void globalSignalHandler(int signum)
{
    if (dxrt::NpuMonitor::s_instance != nullptr)
    {
        dxrt::NpuMonitor::SignalHandler(signum);
    }
    else
    {
        std::cerr<< "Error: NpuMonitor instance not available for signal handling." << std::endl;
    }
}
