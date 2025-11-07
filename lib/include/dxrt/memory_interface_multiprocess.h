#pragma once

// project common
#include "dxrt/common.h"

// C headers
#include <cstdint>

// C++ headers
#include <mutex>

// project headers
#include "dxrt/driver.h"
#include "dxrt/ipc_wrapper/ipc_client_wrapper.h"
#include "dxrt/memory_interface.h"

namespace dxrt {

class MultiprocessMemoryInterface : public MemoryInterface {
 public:
  MultiprocessMemoryInterface();
  uint64_t Allocate(int deviceId, uint64_t required) override;
  uint64_t BackwardAllocate(int deviceId, uint64_t required) override;
  void Deallocate(int deviceId, uint64_t addr) override;
  void DeallocateAll(int deviceId) override;
  uint64_t start() override;
  uint64_t end() override;
  uint64_t size() override;
  uint64_t AllocateForTask(int deviceId, int taskId, uint64_t required) override;
  uint64_t BackwardAllocateForTask(int deviceId, int taskId, uint64_t required) override;
  void SignalScheduller(int deviceId, const dxrt_request_acc_t& req) override;
  void SignalEndJobs(int deviceId) override;
  void SignalDeviceReset(int deviceId) override;
  void SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize) override;
  void SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound) override;
  void DeallocateTaskMemory(int deviceId, int taskId) override;
  static bool other_running(bool release);
 private:
  void mpConnect();
  void mpConnect_once_wrapper();
  std::once_flag _connectFlag;
  IPCClientWrapper ipcClientWrapper;
  IPCClientWrapper ipcClientWrapperSync;
};

} // namespace dxrt
