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

namespace dxrt {

std::ostream& operator<<(std::ostream& os, const p_corr_err_t& o);
std::ostream& operator<<(std::ostream& os, const p_fatal_err_t& o);
std::ostream& operator<<(std::ostream& os, const p_nonfatal_err_t& o);
std::ostream& operator<<(std::ostream& os, const dxrt_pcie_err_stat_t& o);
std::ostream& operator<<(std::ostream& os, const p_evt_by_lane& e);
std::ostream& operator<<(std::ostream& os, const p_evt_common& e);
std::ostream& operator<<(std::ostream& os, const dxrt_pcie_evt_stat_t& e);
std::ostream& operator<<(std::ostream& os, const dxrt_pcie_power_stat_t& e);
std::ostream& operator<<(std::ostream& os, const dma_ch& c);
std::ostream& operator<<(std::ostream& os, const dxrt_pcie_info_t& e);


}  // namespace dxrt


