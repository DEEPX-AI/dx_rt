/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#pragma once

#pragma once

#include "dxrt/common.h"

// C system headers

// C++ standard headers
#include <atomic>
#include <memory>
#include <string>

// Project headers
#include "dxrt/driver_adapter/driver_adapter.h"
#include "dxrt/fw.h"

namespace dxrt {

class DXRT_API DeviceCore {
 public:
    explicit DeviceCore(int id, std::unique_ptr<DriverAdapter> adapter);

    dxrt_device_info_t info() const { return _info;}
    dxrt_device_status_t Status();
    dxrt_dev_info_t devInfo() const { return _devInfo; }
    int id() const { return _id; }

    int Process(dxrt_cmd_t cmd, void *data, uint32_t size = 0, uint32_t sub_cmd = 0, uint64_t address = 0);
    int Poll();
    int Write(dxrt_meminfo_t &, int ch);
    int Write(dxrt_meminfo_t &);
    int WriteData(void *data, size_t len) { return _adapter->Write(data, len); }
    int Read(dxrt_meminfo_t &);
    int Read(dxrt_meminfo_t &, int ch, bool ctrlCmd = true);
    int ReadDriverData(void *ptr, uint32_t size);
    int Wait();
    void Identify(int id_, uint32_t subCmd = 0);

    void Reset(int opt);

    /* this must be moved to CLI itself
    int UpdateFw(std::string fwFile, int subCmd = 0);
    int UploadFw(std::string fwFile, int subCmd = 0);

    uint32_t UploadModel(std::string filePath, uint64_t base_addr);
    */

    int UpdateFwConfig(std::string jsonFile);
    void DoCustomCommand(void *data, uint32_t subCmd, uint32_t size = 0);

    void StartDev(uint32_t option);
    DeviceType GetDeviceType() const;

    void DoPcieCommand(void *data, uint32_t subCmd, uint32_t size);
    void ShowPCIEDetails(std::ostream& os);
    void ShowPCIEDetails();
    std::string name() const { return _name;  }

    bool isBlocked() const { return _isBlocked;  }
    void block() {
      _isBlocked = true;
   }
    void unblock() { _isBlocked = false; }

    void BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp);
    void* CreateMemoryMap();
    void CheckVersion();

    int GetReadChannel();
    int GetWriteChannel();

 private:
   int _id;
   std::unique_ptr<DriverAdapter> _adapter;
   std::string _name;
   dxrt_device_info_t _info;
   dxrt_device_status_t _status;
   dxrt_dev_info_t _devInfo;
   std::atomic<int> _readChannel{0};
   std::atomic<int> _writeChannel{0};
   std::atomic<bool> _isBlocked{false};
};

} // namespace dxrt
