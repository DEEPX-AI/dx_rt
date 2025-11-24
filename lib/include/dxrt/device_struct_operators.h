/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <ostream>
#include "dxrt/device_struct.h"
#include "dxrt/driver.h"

namespace dxrt {

DXRT_API std::ostream& operator<<(std::ostream& os, const p_corr_err_t& o);
DXRT_API std::ostream& operator<<(std::ostream& os, const p_fatal_err_t& o);
DXRT_API std::ostream& operator<<(std::ostream& os, const p_nonfatal_err_t& o);
DXRT_API std::ostream& operator<<(std::ostream& os, const dxrt_pcie_err_stat_t& o);
DXRT_API std::ostream& operator<<(std::ostream& os, const p_evt_by_lane& e);
DXRT_API std::ostream& operator<<(std::ostream& os, const p_evt_common& e);
DXRT_API std::ostream& operator<<(std::ostream& os, const dxrt_pcie_evt_stat_t& e);
DXRT_API std::ostream& operator<<(std::ostream& os, const dxrt_pcie_power_stat_t& e);
DXRT_API std::ostream& operator<<(std::ostream& os, const dma_ch& c);
DXRT_API std::ostream& operator<<(std::ostream& os, const dxrt_pcie_info_t& e);
DXRT_API std::ostream& operator<<(std::ostream&, const dxrt_device_status_t&);
DXRT_API std::ostream& operator<<(std::ostream& os, const dxrt_device_info_t& info);
DXRT_API std::ostream& operator<<(std::ostream& os, const dx_pcie_dev_ntfy_throt_t& notify);


}  // namespace dxrt


