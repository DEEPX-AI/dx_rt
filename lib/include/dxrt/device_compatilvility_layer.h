/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


//#include "dxrt/device.h"
#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/device_info_status.h"


namespace dxrt {

class FwLog;
class DeviceCore;
class DeviceTaskLayer;

class Device
{
public:
    explicit Device(int id);

    int id() const { return _id; }
    dxrt_device_info_t info();
    dxrt_device_status_t status();
    dxrt_dev_info_t devInfo();
    int Process(dxrt_cmd_t, void *, uint32_t size = 0, uint32_t sub_cmd = 0, uint64_t address = 0);

    //int InferenceRequest(RequestData* req, npu_bound_op boundOp = N_BOUND_NORMAL);
    //int Release(TaskData *task);
    //int RegisterTask(TaskData *task);

    void Terminate();
    void Reset(int opt);

    int UpdateFw(std::string fwFile, int subCmd = 0);
    int UploadFw(std::string fwFile, int subCmd = 0);
    int UpdateFwConfig(std::string jsonFile);
    uint32_t UploadModel(std::string filePath, uint64_t base_addr);
    void DoCustomCommand(void *data, uint32_t subCmd, uint32_t size = 0);
    std::shared_ptr<FwLog> GetFwLog();


    dxrt_model_t npu_model(int taskId);
    friend DXRT_API std::ostream &operator<<(std::ostream &, const Device &);

    DeviceType getDeviceType();

    void DoPcieCommand(void *data, uint32_t subCmd, uint32_t size);
    void ShowPCIEDetails(std::ostream& os);
    void ShowPCIEDetails();
    DeviceStatus GetCurrentStatus();

private:
    int _id;
    std::shared_ptr<DeviceCore> GetCore();
    std::shared_ptr<DeviceTaskLayer> GetTaskLayer();
};

std::vector<std::shared_ptr<Device> >& CheckDevices();


}  // namespace dxrt
