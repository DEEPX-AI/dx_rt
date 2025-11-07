#pragma once

// project common
#include "dxrt/common.h"

// C headers
#include <cstdint>

// C++ headers (none)

// project headers
#include "dxrt/driver.h"
#include "dxrt/memory.h"
#include "dxrt/memory_interface.h"

namespace dxrt {

class NoMultiprocessMemory : public MemoryInterface {
 public:
  NoMultiprocessMemory();
  uint64_t Allocate(int deviceId, uint64_t required) override;
  uint64_t BackwardAllocate(int deviceId, uint64_t required) override;
  void Deallocate(int deviceId, uint64_t addr) override;
  void DeallocateAll(int deviceId) override;
  uint64_t start() override;
  uint64_t end() override;
  uint64_t size() override;
  uint64_t AllocateForTask(int deviceId, int taskId, uint64_t required) override;
  uint64_t BackwardAllocateForTask(int deviceId, int taskId, uint64_t required) override;
  void SignalScheduller(int deviceId, const dxrt_request_acc_t &req) override { std::ignore = deviceId; std::ignore = req; }
  void SignalEndJobs(int deviceId) override { std::ignore = deviceId; }
  void SignalDeviceReset(int deviceId) override;
  void SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize) override;
  void SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound) override;
  void DeallocateTaskMemory(int deviceId, int taskId) override;
 private:
  Memory _memory;
};

} // namespace dxrt
