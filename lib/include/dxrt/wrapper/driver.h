/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * SDK wrapper for driver-level types (OTP, LED, custom commands).
 * This header replaces the internal driver.h for SDK consumers.
 * Types and constants are binary-compatible with the original definitions.
 */
#pragma once

#include <cstdint>
#ifdef __linux__
#include <sys/ioctl.h>
#endif

namespace dxrt {

#pragma pack(push, 1)
typedef struct otp_info {
    uint8_t     JEP_ID;
    uint8_t     CONTINUATION_CODE;
    char        CHIP_NAME[2];
    char        DEVICE_REV[2];
    uint16_t    RESERVED0;
    uint32_t    ECID;
    char        FOUNDRY_FAB[4];
    char        PROCESS[4];
    char        LOT_ID[12];
    char        WAFER_ID[4];
    char        X_AXIS[4];
    char        Y_AXIS[4];
    char        TEST_PGM[4];
    char        BARCODE[16];
    uint32_t    BARCODE_IDX;
} otp_info_t;
#pragma pack(pop)
static_assert(sizeof(otp_info_t) == 68, "otp_info_t binary layout mismatch");

typedef enum {
    DX_SET_DDR_FREQ         = 1,
    DX_GET_OTP              = 2,
    DX_SET_OTP              = 3,
    DX_SET_LED              = 4,
    DX_ADD_WEIGHT_INFO      = 5,
    DX_DEL_WEIGHT_INFO      = 6,
} dxrt_custom_sub_cmt_t;

typedef struct _dxrt_message
{
    int32_t cmd = 0;
    int32_t sub_cmd = 0;
    void* data = nullptr;
    uint32_t size = 0;
} dxrt_message_t;

static constexpr int32_t DXRT_CMD_GET_STATUS = 1;

#define DXRT_IOCTL_MAGIC 'D'

#ifdef __linux__
static constexpr unsigned long DXRT_IOCTL_MESSAGE = _IOW(DXRT_IOCTL_MAGIC, 0, dxrt_message_t);
#else
static constexpr unsigned long DXRT_IOCTL_MESSAGE = 0;
#endif

} // namespace dxrt
