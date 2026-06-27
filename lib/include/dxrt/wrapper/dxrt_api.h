/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * DXRT Wrapper — Umbrella header for prebuilt delivery.
 *
 * @deprecated This header is in MAINTENANCE MODE — no new features will be
 *             added here. New code should use <dxrt/dxrt_cxx_api.h> instead,
 *             which provides ABI-stable C++ wrappers over the C API.
 *
 * To suppress the compile-time notice below, define DXRT_LEGACY_HEADER_OK
 * (e.g., -DDXRT_LEGACY_HEADER_OK in your build flags).
 *
 * User code includes "dxrt/dxrt_api.h" — when installed as a prebuilt
 * package, this header resolves to the wrapper classes that call C ABI
 * functions. No user source code changes needed; just recompile.
 */

#pragma once

#if !defined(DXRT_LEGACY_HEADER_OK)
#pragma message("note: <dxrt/dxrt_api.h> is in maintenance mode. " \
               "New features require <dxrt/dxrt_cxx_api.h>. " \
               "Define DXRT_LEGACY_HEADER_OK to suppress this message.")
#endif

#include "dxrt/dxrt_c_api.h"
#include "dxrt/common.h"
#include "dxrt/datatype.h"
#include "dxrt/model.h"
#include "dxrt/inference_engine.h"
#include "dxrt/configuration.h"
#include "dxrt/device_info_status.h"
#include "dxrt/device.h"
#include "dxrt/profiler.h"
#include "dxrt/runtime_event_dispatcher.h"
