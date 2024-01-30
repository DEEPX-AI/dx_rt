/* Automatically generated from DX_M1A.xlsx
 * by Deepx Neural Network Generator v1.0
 */
#pragma once

#include <stdint.h>
#include "dxrt/npuif/npuif.h"

namespace dxrt{
extern bool logNpuRegAccess;

/* Address */
#define REG_DX_M1A_SYSTEM_OFFSET 0x0
#define REG_DX_M1A_SYSTEM_ID    0x00000000 /* 0 */
#define REG_DX_M1A_SYSTEM_RUN_OPT    0x00000004 /* 4 */
#define REG_DX_M1A_SYSTEM_STATUS1    0x00000008 /* 8 */
#define REG_DX_M1A_SYSTEM_STATUS2    0x0000000c /* 12 */
#define REG_DX_M1A_SYSTEM_NET_INFO    0x00000010 /* 16 */
#define REG_DX_M1A_SYSTEM_IRQ_STATUS    0x00000014 /* 20 */
#define REG_DX_M1A_SYSTEM_IRQ_SET    0x00000018 /* 24 */
#define REG_DX_M1A_SYSTEM_IRQ_CNT0    0x0000001c /* 28 */
#define REG_DX_M1A_SYSTEM_IRQ_CNT1    0x00000020 /* 32 */
#define REG_DX_M1A_SYSTEM_IRQ_CNT2    0x00000024 /* 36 */
#define REG_DX_M1A_SYSTEM_IRQ_CNT3    0x00000028 /* 40 */
#define REG_DX_M1A_SYSTEM_IRQ_CNT4    0x0000002c /* 44 */
#define REG_DX_M1A_SYSTEM_IN_FEATURE_BASE    0x00000030 /* 48 */
#define REG_DX_M1A_SYSTEM_OUT_FEATURE_BASE    0x00000034 /* 52 */
#define REG_DX_M1A_SYSTEM_WEIGHT_BASE    0x00000038 /* 56 */
#define REG_DX_M1A_SYSTEM_IRQ_TIMER_DMA    0x0000003c /* 60 */
#define REG_DX_M1A_SYSTEM_IRQ_TIMER_OP    0x00000040 /* 64 */
#define REG_DX_M1A_SYSTEM_CMD    0x00000044 /* 68 */
#define REG_DX_M1A_SYSTEM_SWREG0    0x00000048 /* 72 */
#define REG_DX_M1A_SYSTEM_SWREG1    0x0000004c /* 76 */
#define REG_DX_M1A_SYSTEM_SWREG2    0x00000050 /* 80 */
#define REG_DX_M1A_SYSTEM_SWREG3    0x00000054 /* 84 */
#define REG_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT    0x00000058 /* 88 */
#define REG_DX_M1A_SYSTEM_RSV0    0x0000005c /* 92 */
#define REG_DX_M1A_SYSTEM_RSV1    0x00000060 /* 96 */
#define REG_DX_M1A_SYSTEM_RSV2    0x00000064 /* 100 */
#define REG_DX_M1A_SYSTEM_CPU_STATUS    0x00000068 /* 104 */
#define REG_DX_M1A_SYSTEM_CPU_IRQ_SET    0x0000006c /* 108 */
#define REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS    0x00000070 /* 112 */
#define REG_DX_M1A_SYSTEM_CPU_START    0x00000074 /* 116 */
#define REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO    0x00000078 /* 120 */
#define REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO    0x0000007c /* 124 */
#define REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE    0x00000080 /* 128 */
#define REG_DX_M1A_SYSTEM_SWREG4    0x00000084 /* 132 */
#define REG_DX_M1A_SYSTEM_SWREG5    0x00000088 /* 136 */
#define REG_DX_M1A_SYSTEM_SWREG6    0x0000008c /* 140 */
#define REG_DX_M1A_SYSTEM_SWREG7    0x00000090 /* 144 */
#define REG_DX_M1A_SYSTEM_SWREG8    0x00000094 /* 148 */
#define REG_DX_M1A_SYSTEM_SWREG9    0x00000098 /* 152 */
#define REG_DX_M1A_SYSTEM_SWREG10    0x0000009c /* 156 */
#define REG_DX_M1A_SYSTEM_SWREG11    0x000000a0 /* 160 */
#define REG_DX_M1A_SYSTEM_SWREG12    0x000000a4 /* 164 */
#define REG_DX_M1A_SYSTEM_SWREG13    0x000000a8 /* 168 */
#define REG_DX_M1A_SYSTEM_SWREG14    0x000000ac /* 172 */
#define REG_DX_M1A_SYSTEM_SWREG15    0x000000b0 /* 176 */

/* Register Definition */
typedef volatile union _DX_M1A_SYSTEM_ID_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.ID*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RMAP_ID:16;  /* DX_M1A_SYSTEM.ID.RMAP_ID [15:0] (R, 0xa6) RMAP VERSION */
        volatile uint32_t REVISION_ID:6;  /* DX_M1A_SYSTEM.ID.REVISION_ID [21:16] (R, 0x0) RTL REVISION # */
        volatile uint32_t NPU_ID:2;  /* DX_M1A_SYSTEM.ID.NPU_ID [23:22] (R, 0x0)  */
        volatile uint32_t DEVICE_ID:8;  /* DX_M1A_SYSTEM.ID.DEVICE_ID [31:24] (R, 0xb2) L1:A1, L2:A2, M1:B1, M1A:B2 */
    };
} DX_M1A_SYSTEM_ID_t;
typedef volatile union _DX_M1A_SYSTEM_RUN_OPT_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.RUN_OPT*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:31;  /* DX_M1A_SYSTEM.RUN_OPT.RESERVED0 [30:0] (R, 0x0)  */
        volatile uint32_t AUTORUN_EN:1;  /* DX_M1A_SYSTEM.RUN_OPT.AUTORUN_EN [31:31] (RW, 0x0) INFERENCE 종료 後, NEXT INFERENCE 자동 START */
    };
} DX_M1A_SYSTEM_RUN_OPT_t;
typedef volatile union _DX_M1A_SYSTEM_STATUS1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.STATUS1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CMD_NUM:16;  /* DX_M1A_SYSTEM.STATUS1.CMD_NUM [15:0] (R, 0x0) 현재 수행중인 TILE NO.(0~) */
        volatile uint32_t ARGMAX_ID:16;  /* DX_M1A_SYSTEM.STATUS1.ARGMAX_ID [31:16] (R, 0x0) 최종 검출된 CLASS INDEX NO.(IC MODEL) */
    };
} DX_M1A_SYSTEM_STATUS1_t;
typedef volatile union _DX_M1A_SYSTEM_STATUS2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.STATUS2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t BUSY:1;  /* DX_M1A_SYSTEM.STATUS2.BUSY [0:0] (R, 0x0) NPU 동작상태 */
        volatile uint32_t RESERVED0:31;  /* DX_M1A_SYSTEM.STATUS2.RESERVED0 [31:1] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_STATUS2_t;
typedef volatile union _DX_M1A_SYSTEM_NET_INFO_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.NET_INFO*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t LAST_CMD_NUM:16;  /* DX_M1A_SYSTEM.NET_INFO.LAST_CMD_NUM [15:0] (RW, 0x0) NETWORK 구동에 사용되는 RMAP TILE 개수 */
        volatile uint32_t RESERVED0:16;  /* DX_M1A_SYSTEM.NET_INFO.RESERVED0 [31:16] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_NET_INFO_t;
typedef volatile union _DX_M1A_SYSTEM_IRQ_STATUS_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.IRQ_STATUS*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t STATUS_TILE:16;  /* DX_M1A_SYSTEM.IRQ_STATUS.STATUS_TILE [15:0] (R, 0x0) IRQ 발생 TILE NO.(ONLY ABNORMAL IRQ 만, FRAME/TILE/INPUT_DONE 제외) */
        volatile uint32_t STATUS_OP:4;  /* DX_M1A_SYSTEM.IRQ_STATUS.STATUS_OP [19:16] (R, 0x0) IRQ 발생 TILE OPERATION(ONLY ABNORMAL IRQ 만, FRAME/TILE/INPUT_DONE 제외) */
        volatile uint32_t IRQ_MEM_COLLISION:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_MEM_COLLISION [20:20] (R, 0x0) SRAM 같은 BANK 접근 */
        volatile uint32_t IRQ_CMD_REACHED:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_CMD_REACHED [21:21] (R, 0x0) RMAP LAST ADDRESS NOT REACHED */
        volatile uint32_t IRQ_CMD_CKSUM:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_CMD_CKSUM [22:22] (R, 0x0) RMAP DATA 무결성 CHECK */
        volatile uint32_t IRQ_NPU_FIFO:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_NPU_FIFO [23:23] (R, 0x0) SFU 내부 FIFO 오동작 */
        volatile uint32_t IRQ_DMA_FIFO_CCLK:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_DMA_FIFO_CCLK [24:24] (R, 0x0) DMA 내부 FIFO 오동작(CPU CLOCK 사용) */
        volatile uint32_t IRQ_DMA_FIFO_NCLK:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_DMA_FIFO_NCLK [25:25] (R, 0x0) DMA 내부 FIFO 오동작(NPU CLOCK 사용) */
        volatile uint32_t IRQ_DMA_FIFO_ACLK:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_DMA_FIFO_ACLK [26:26] (R, 0x0) DMA 내부 FIFO 오동작(AXI CLOCK 사용) */
        volatile uint32_t IRQ_TIMEOUT_OP:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_TIMEOUT_OP [27:27] (R, 0x0) NPU 오동작으로 인한 OPERATION TIME 잘못 계산 */
        volatile uint32_t IRQ_TIMEOUT_DMA:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_TIMEOUT_DMA [28:28] (R, 0x0) AXI ↔ DMA 오동작으로 인한 DRAM WRITE/READ TIME OUT */
        volatile uint32_t IRQ_INPUT_DONE:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_INPUT_DONE [29:29] (R, 0x0) IMG 사용 종료 */
        volatile uint32_t IRQ_TILE:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_TILE [30:30] (R, 0x0) TILE OPERATION 종료 */
        volatile uint32_t IRQ_FRAME:1;  /* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_FRAME [31:31] (R, 0x0) INFERENCE 종료 */
    };
} DX_M1A_SYSTEM_IRQ_STATUS_t;
typedef volatile union _DX_M1A_SYSTEM_IRQ_SET_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.IRQ_SET*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:20;  /* DX_M1A_SYSTEM.IRQ_SET.RESERVED0 [19:0] (R, 0x0) IRQ_FRAME COUNT */
        volatile uint32_t IRQ_MEM_COLLISION_SET:1;  /* DX_M1A_SYSTEM.IRQ_SET.IRQ_MEM_COLLISION_SET [20:20] (RW, 0x0) 1: NPU OUTPUT 반영, 0: 내부 MONITORING만 사용 */
        volatile uint32_t IRQ_CMD_REACHED_SET:1;  /* DX_M1A_SYSTEM.IRQ_SET.IRQ_CMD_REACHED_SET [21:21] (RW, 0x0) 1: NPU OUTPUT 반영, 0: 내부 MONITORING만 사용 */
        volatile uint32_t IRQ_CMD_CKSUM_SET:1;  /* DX_M1A_SYSTEM.IRQ_SET.IRQ_CMD_CKSUM_SET [22:22] (RW, 0x0) 1: NPU OUTPUT 반영, 0: 내부 MONITORING만 사용 */
        volatile uint32_t IRQ_NPU_FIFO_SET:1;  /* DX_M1A_SYSTEM.IRQ_SET.IRQ_NPU_FIFO_SET [23:23] (RW, 0x0) 1: NPU OUTPUT 반영, 0: 내부 MONITORING만 사용 */
        volatile uint32_t IRQ_DMA_FIFO_SET:3;  /* DX_M1A_SYSTEM.IRQ_SET.IRQ_DMA_FIFO_SET [26:24] (RW, 0x0) 2:ACLK, 1:NCLK, 0 :CCLK */
        volatile uint32_t IRQ_TIMEOUT_OP_SET:1;  /* DX_M1A_SYSTEM.IRQ_SET.IRQ_TIMEOUT_OP_SET [27:27] (RW, 0x0) 1: NPU OUTPUT 반영, 0: 내부 MONITORING만 사용 */
        volatile uint32_t IRQ_TIMEOUT_DMA_SET:1;  /* DX_M1A_SYSTEM.IRQ_SET.IRQ_TIMEOUT_DMA_SET [28:28] (RW, 0x0) 1: NPU OUTPUT 반영, 0: 내부 MONITORING만 사용 */
        volatile uint32_t IRQ_INPUT_DONE_SET:1;  /* DX_M1A_SYSTEM.IRQ_SET.IRQ_INPUT_DONE_SET [29:29] (RW, 0x0) 1: NPU OUTPUT 반영, 0: 내부 MONITORING만 사용 */
        volatile uint32_t IRQ_TILE_SET:1;  /* DX_M1A_SYSTEM.IRQ_SET.IRQ_TILE_SET [30:30] (RW, 0x0) 1: NPU OUTPUT 반영, 0: 내부 MONITORING만 사용 */
        volatile uint32_t IRQ_FRAME_SET:1;  /* DX_M1A_SYSTEM.IRQ_SET.IRQ_FRAME_SET [31:31] (RW, 0x0) 1: NPU OUTPUT 반영, 0: 내부 MONITORING만 사용 */
    };
} DX_M1A_SYSTEM_IRQ_SET_t;
typedef volatile union _DX_M1A_SYSTEM_IRQ_CNT0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.IRQ_CNT0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IRQ_CNT_NPU_NORMAL:32;  /* DX_M1A_SYSTEM.IRQ_CNT0.IRQ_CNT_NPU_NORMAL [31:0] (R, 0x0) IRQ_INPUT_DONE COUNT */
    };
} DX_M1A_SYSTEM_IRQ_CNT0_t;
typedef volatile union _DX_M1A_SYSTEM_IRQ_CNT1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.IRQ_CNT1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IRQ_CNT_NPU_INPUT_DONE:32;  /* DX_M1A_SYSTEM.IRQ_CNT1.IRQ_CNT_NPU_INPUT_DONE [31:0] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_IRQ_CNT1_t;
typedef volatile union _DX_M1A_SYSTEM_IRQ_CNT2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.IRQ_CNT2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IRQ_CNT_NPU_ABNORMAL:32;  /* DX_M1A_SYSTEM.IRQ_CNT2.IRQ_CNT_NPU_ABNORMAL [31:0] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_IRQ_CNT2_t;
typedef volatile union _DX_M1A_SYSTEM_IRQ_CNT3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.IRQ_CNT3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IRQ_CNT_CPU_FINISH:32;  /* DX_M1A_SYSTEM.IRQ_CNT3.IRQ_CNT_CPU_FINISH [31:0] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_IRQ_CNT3_t;
typedef volatile union _DX_M1A_SYSTEM_IRQ_CNT4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.IRQ_CNT4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IRQ_CNT_CPU_INIT:32;  /* DX_M1A_SYSTEM.IRQ_CNT4.IRQ_CNT_CPU_INIT [31:0] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_IRQ_CNT4_t;
typedef volatile union _DX_M1A_SYSTEM_IN_FEATURE_BASE_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.IN_FEATURE_BASE*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IN_FEATURE_OFFSET:32;  /* DX_M1A_SYSTEM.IN_FEATURE_BASE.IN_FEATURE_OFFSET [31:0] (RW, 0x0) IMG DDR BASE ADDRESS */
    };
} DX_M1A_SYSTEM_IN_FEATURE_BASE_t;
typedef volatile union _DX_M1A_SYSTEM_OUT_FEATURE_BASE_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.OUT_FEATURE_BASE*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t OUT_FEATURE_OFFSET:32;  /* DX_M1A_SYSTEM.OUT_FEATURE_BASE.OUT_FEATURE_OFFSET [31:0] (RW, 0x0) TENSOR(NPU에서 발생하는 모든 DATA) DDR BASE ADDRESS */
    };
} DX_M1A_SYSTEM_OUT_FEATURE_BASE_t;
typedef volatile union _DX_M1A_SYSTEM_WEIGHT_BASE_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.WEIGHT_BASE*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t WEIGHT_OFFSET:32;  /* DX_M1A_SYSTEM.WEIGHT_BASE.WEIGHT_OFFSET [31:0] (RW, 0x0) WEIGHT DDR BASE ADDRESS */
    };
} DX_M1A_SYSTEM_WEIGHT_BASE_t;
typedef volatile union _DX_M1A_SYSTEM_IRQ_TIMER_DMA_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.IRQ_TIMER_DMA*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IRQ_TIMER_DMA:32;  /* DX_M1A_SYSTEM.IRQ_TIMER_DMA.IRQ_TIMER_DMA [31:0] (RW, 0xffffffff) IRQ_TIMEOUT_DMA의 TIMER(1.25GHZ, 1SEC:4A817C80) */
    };
} DX_M1A_SYSTEM_IRQ_TIMER_DMA_t;
typedef volatile union _DX_M1A_SYSTEM_IRQ_TIMER_OP_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.IRQ_TIMER_OP*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IRQ_TIMER_OP:32;  /* DX_M1A_SYSTEM.IRQ_TIMER_OP.IRQ_TIMER_OP [31:0] (RW, 0xffffffff) IRQ_TIMEOUT_OP의 TIMER(1.25GHZ, 1SEC:4A817C80) */
    };
} DX_M1A_SYSTEM_IRQ_TIMER_OP_t;
typedef volatile union _DX_M1A_SYSTEM_CMD_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.CMD*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t START:1;  /* DX_M1A_SYSTEM.CMD.START [0:0] (RW, 0x0) NPU INFERENCE START */
        volatile uint32_t RESERVED0:31;  /* DX_M1A_SYSTEM.CMD.RESERVED0 [31:1] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_CMD_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG0:32;  /* DX_M1A_SYSTEM.SWREG0.SWREG0 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG0_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG1:32;  /* DX_M1A_SYSTEM.SWREG1.SWREG1 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG1_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG2:32;  /* DX_M1A_SYSTEM.SWREG2.SWREG2 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG2_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG3:32;  /* DX_M1A_SYSTEM.SWREG3.SWREG3 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG3_t;
typedef volatile union _DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.IRQ_EXT_CLR_OPT*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IRQ_EXT_CLR_OPT:1;  /* DX_M1A_SYSTEM.IRQ_EXT_CLR_OPT.IRQ_EXT_CLR_OPT [0:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:31;  /* DX_M1A_SYSTEM.IRQ_EXT_CLR_OPT.RESERVED0 [31:1] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_t;
typedef volatile union _DX_M1A_SYSTEM_RSV0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.RSV0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_M1A_SYSTEM.RSV0.RESERVED0 [31:0] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_RSV0_t;
typedef volatile union _DX_M1A_SYSTEM_RSV1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.RSV1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_M1A_SYSTEM.RSV1.RESERVED0 [31:0] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_RSV1_t;
typedef volatile union _DX_M1A_SYSTEM_RSV2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.RSV2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_M1A_SYSTEM.RSV2.RESERVED0 [31:0] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_RSV2_t;
typedef volatile union _DX_M1A_SYSTEM_CPU_STATUS_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.CPU_STATUS*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CPU_LOCKUP:1;  /* DX_M1A_SYSTEM.CPU_STATUS.CPU_LOCKUP [0:0] (R, 0x0) INDICATES THAT PROCESSOR IS IN DEEP SLEEP MODE. */
        volatile uint32_t CPU_HALTED:1;  /* DX_M1A_SYSTEM.CPU_STATUS.CPU_HALTED [1:1] (R, 0x0) INDICATES THAT PROCESSOR IS IN SLEEP MODE. */
        volatile uint32_t CPU_SLEEPDEEP:1;  /* DX_M1A_SYSTEM.CPU_STATUS.CPU_SLEEPDEEP [2:2] (R, 0x0) INDICATES THAT THE CORE IS LOCKED UP. */
        volatile uint32_t CPU_SLEEPING:1;  /* DX_M1A_SYSTEM.CPU_STATUS.CPU_SLEEPING [3:3] (R, 0x0) IN HALTING MODE DEBUG. HALTED REMAINS ASSERTED WHILE THE CORE IS IN DEBUG. */
        volatile uint32_t RESERVED0:28;  /* DX_M1A_SYSTEM.CPU_STATUS.RESERVED0 [31:4] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_CPU_STATUS_t;
typedef volatile union _DX_M1A_SYSTEM_CPU_IRQ_SET_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.CPU_IRQ_SET*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IRQ_CPU_INIT_SET:1;  /* DX_M1A_SYSTEM.CPU_IRQ_SET.IRQ_CPU_INIT_SET [0:0] (RW, 0x0) ENABLE IRQ_CPU ABOUT  LOADING COMPLETE CODE DATA */
        volatile uint32_t IRQ_CPU_FINISH_SET:1;  /* DX_M1A_SYSTEM.CPU_IRQ_SET.IRQ_CPU_FINISH_SET [1:1] (RW, 0x0) ENABLE IRQ_CPU ABOUT ALL OPERATION COMPLETED */
        volatile uint32_t RESERVED0:30;  /* DX_M1A_SYSTEM.CPU_IRQ_SET.RESERVED0 [31:2] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_CPU_IRQ_SET_t;
typedef volatile union _DX_M1A_SYSTEM_CPU_IRQ_STATUS_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.CPU_IRQ_STATUS*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IRQ_CPU_INIT:1;  /* DX_M1A_SYSTEM.CPU_IRQ_STATUS.IRQ_CPU_INIT [0:0] (R, 0x0) NOTIFICATION OF CPU CODE DATA LOAD COMPLETION. */
        volatile uint32_t IRQ_CPU_FINISH:1;  /* DX_M1A_SYSTEM.CPU_IRQ_STATUS.IRQ_CPU_FINISH [1:1] (R, 0x0) NOTIFICATION THAT THE CPU HAS COMPLETED ALL OPERATIONS */
        volatile uint32_t CPU_BUSY:1;  /* DX_M1A_SYSTEM.CPU_IRQ_STATUS.CPU_BUSY [2:2] (R, 0x0) NOTIFICATION THAT THE CPU IS ONGOING */
        volatile uint32_t RESERVED0:29;  /* DX_M1A_SYSTEM.CPU_IRQ_STATUS.RESERVED0 [31:3] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_CPU_IRQ_STATUS_t;
typedef volatile union _DX_M1A_SYSTEM_CPU_START_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.CPU_START*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CPU_START:1;  /* DX_M1A_SYSTEM.CPU_START.CPU_START [0:0] (RW, 0x0) ENABLE CPU START */
        volatile uint32_t RESERVED0:31;  /* DX_M1A_SYSTEM.CPU_START.RESERVED0 [31:1] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_CPU_START_t;
typedef volatile union _DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.PPU_OUT_FILTER_INFO*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PPU_FILTERING_NUM:10;  /* DX_M1A_SYSTEM.PPU_OUT_FILTER_INFO.PPU_FILTERING_NUM [9:0] (R, 0x0) TOTAL NUMBER OF PP_FILTERING RESULT COUNTS(WRITE BY M4 CPU) */
        volatile uint32_t PPU_FILTERING_WNUM:16;  /* DX_M1A_SYSTEM.PPU_OUT_FILTER_INFO.PPU_FILTERING_WNUM [25:10] (R, 0x0)  */
        volatile uint32_t PPU_FILTERING_BUFSEL:1;  /* DX_M1A_SYSTEM.PPU_OUT_FILTER_INFO.PPU_FILTERING_BUFSEL [26:26] (R, 0x0)  */
        volatile uint32_t RESERVED0:5;  /* DX_M1A_SYSTEM.PPU_OUT_FILTER_INFO.RESERVED0 [31:27] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_t;
typedef volatile union _DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.PPU_OUT_FEATURE_INFO*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PPU_OUT_FEATURE_NUM:10;  /* DX_M1A_SYSTEM.PPU_OUT_FEATURE_INFO.PPU_OUT_FEATURE_NUM [9:0] (RW, 0x0) POST-PROCESS 최종 NMS OUTPUT(BOX INFO) 개수 */
        volatile uint32_t RESERVED0:22;  /* DX_M1A_SYSTEM.PPU_OUT_FEATURE_INFO.RESERVED0 [31:10] (R, 0x0)  */
    };
} DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_t;
typedef volatile union _DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.PPU_OUT_FEATURE_BASE*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PPU_OUT_FEATURE_OFFSET:32;  /* DX_M1A_SYSTEM.PPU_OUT_FEATURE_BASE.PPU_OUT_FEATURE_OFFSET [31:0] (RW, 0x0) POST-PROCESS 최종 OUTPUT(BOX INFO) DDR BASE ADDRESS */
    };
} DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG4:32;  /* DX_M1A_SYSTEM.SWREG4.SWREG4 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG4_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG5_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG5:32;  /* DX_M1A_SYSTEM.SWREG5.SWREG5 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG5_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG6_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG6:32;  /* DX_M1A_SYSTEM.SWREG6.SWREG6 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG6_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG7_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG7:32;  /* DX_M1A_SYSTEM.SWREG7.SWREG7 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG7_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG8_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG8:32;  /* DX_M1A_SYSTEM.SWREG8.SWREG8 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG8_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG9_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG9:32;  /* DX_M1A_SYSTEM.SWREG9.SWREG9 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG9_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG10_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG10*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG10:32;  /* DX_M1A_SYSTEM.SWREG10.SWREG10 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG10_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG11_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG11*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG11:32;  /* DX_M1A_SYSTEM.SWREG11.SWREG11 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG11_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG12_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG12*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG12:32;  /* DX_M1A_SYSTEM.SWREG12.SWREG12 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG12_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG13_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG13*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG13:32;  /* DX_M1A_SYSTEM.SWREG13.SWREG13 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG13_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG14_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG14*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG14:32;  /* DX_M1A_SYSTEM.SWREG14.SWREG14 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG14_t;
typedef volatile union _DX_M1A_SYSTEM_SWREG15_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_SYSTEM.SWREG15*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SWREG15:32;  /* DX_M1A_SYSTEM.SWREG15.SWREG15 [31:0] (RW, 0x0) RT 사용 REGISTER */
    };
} DX_M1A_SYSTEM_SWREG15_t;

/* DX_M1A_SYSTEM Module Definition */
typedef volatile struct _dxDX_M1A_SYSTEM_t
{
    DX_M1A_SYSTEM_ID_t ID;  /* SYSTEM 0x0 */
    DX_M1A_SYSTEM_RUN_OPT_t RUN_OPT;  /* SYSTEM 0x4 */
    DX_M1A_SYSTEM_STATUS1_t STATUS1;  /* SYSTEM 0x8 */
    DX_M1A_SYSTEM_STATUS2_t STATUS2;  /* SYSTEM 0xc */
    DX_M1A_SYSTEM_NET_INFO_t NET_INFO;  /* SYSTEM 0x10 */
    DX_M1A_SYSTEM_IRQ_STATUS_t IRQ_STATUS;  /* SYSTEM 0x14 */
    DX_M1A_SYSTEM_IRQ_SET_t IRQ_SET;  /* SYSTEM 0x18 */
    DX_M1A_SYSTEM_IRQ_CNT0_t IRQ_CNT0;  /* SYSTEM 0x1c */
    DX_M1A_SYSTEM_IRQ_CNT1_t IRQ_CNT1;  /* SYSTEM 0x20 */
    DX_M1A_SYSTEM_IRQ_CNT2_t IRQ_CNT2;  /* SYSTEM 0x24 */
    DX_M1A_SYSTEM_IRQ_CNT3_t IRQ_CNT3;  /* SYSTEM 0x28 */
    DX_M1A_SYSTEM_IRQ_CNT4_t IRQ_CNT4;  /* SYSTEM 0x2c */
    DX_M1A_SYSTEM_IN_FEATURE_BASE_t IN_FEATURE_BASE;  /* SYSTEM 0x30 */
    DX_M1A_SYSTEM_OUT_FEATURE_BASE_t OUT_FEATURE_BASE;  /* SYSTEM 0x34 */
    DX_M1A_SYSTEM_WEIGHT_BASE_t WEIGHT_BASE;  /* SYSTEM 0x38 */
    DX_M1A_SYSTEM_IRQ_TIMER_DMA_t IRQ_TIMER_DMA;  /* SYSTEM 0x3c */
    DX_M1A_SYSTEM_IRQ_TIMER_OP_t IRQ_TIMER_OP;  /* SYSTEM 0x40 */
    DX_M1A_SYSTEM_CMD_t CMD;  /* SYSTEM 0x44 */
    DX_M1A_SYSTEM_SWREG0_t SWREG0;  /* SYSTEM 0x48 */
    DX_M1A_SYSTEM_SWREG1_t SWREG1;  /* SYSTEM 0x4c */
    DX_M1A_SYSTEM_SWREG2_t SWREG2;  /* SYSTEM 0x50 */
    DX_M1A_SYSTEM_SWREG3_t SWREG3;  /* SYSTEM 0x54 */
    DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_t IRQ_EXT_CLR_OPT;  /* SYSTEM 0x58 */
    DX_M1A_SYSTEM_RSV0_t RSV0;  /* SYSTEM 0x5c */
    DX_M1A_SYSTEM_RSV1_t RSV1;  /* SYSTEM 0x60 */
    DX_M1A_SYSTEM_RSV2_t RSV2;  /* SYSTEM 0x64 */
    DX_M1A_SYSTEM_CPU_STATUS_t CPU_STATUS;  /* SYSTEM 0x68 */
    DX_M1A_SYSTEM_CPU_IRQ_SET_t CPU_IRQ_SET;  /* SYSTEM 0x6c */
    DX_M1A_SYSTEM_CPU_IRQ_STATUS_t CPU_IRQ_STATUS;  /* SYSTEM 0x70 */
    DX_M1A_SYSTEM_CPU_START_t CPU_START;  /* SYSTEM 0x74 */
    DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_t PPU_OUT_FILTER_INFO;  /* SYSTEM 0x78 */
    DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_t PPU_OUT_FEATURE_INFO;  /* SYSTEM 0x7c */
    DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_t PPU_OUT_FEATURE_BASE;  /* SYSTEM 0x80 */
    DX_M1A_SYSTEM_SWREG4_t SWREG4;  /* SYSTEM 0x84 */
    DX_M1A_SYSTEM_SWREG5_t SWREG5;  /* SYSTEM 0x88 */
    DX_M1A_SYSTEM_SWREG6_t SWREG6;  /* SYSTEM 0x8c */
    DX_M1A_SYSTEM_SWREG7_t SWREG7;  /* SYSTEM 0x90 */
    DX_M1A_SYSTEM_SWREG8_t SWREG8;  /* SYSTEM 0x94 */
    DX_M1A_SYSTEM_SWREG9_t SWREG9;  /* SYSTEM 0x98 */
    DX_M1A_SYSTEM_SWREG10_t SWREG10;  /* SYSTEM 0x9c */
    DX_M1A_SYSTEM_SWREG11_t SWREG11;  /* SYSTEM 0xa0 */
    DX_M1A_SYSTEM_SWREG12_t SWREG12;  /* SYSTEM 0xa4 */
    DX_M1A_SYSTEM_SWREG13_t SWREG13;  /* SYSTEM 0xa8 */
    DX_M1A_SYSTEM_SWREG14_t SWREG14;  /* SYSTEM 0xac */
    DX_M1A_SYSTEM_SWREG15_t SWREG15;  /* SYSTEM 0xb0 */
} dxDX_M1A_SYSTEM_t;

/* Register access API */
/* DX_M1A_SYSTEM.ID (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_ID(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_ID)
#define WRITE_DX_M1A_SYSTEM_ID(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_ID, val)
/* DX_M1A_SYSTEM.ID.RMAP_ID (Category: SYSTEM) */
#define DX_M1A_SYSTEM_ID_RMAP_ID_LEN        16
#define DX_M1A_SYSTEM_ID_RMAP_ID_OFFSET     0
#define DX_M1A_SYSTEM_ID_RMAP_ID_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_SYSTEM_ID_RMAP_ID_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_ID_RMAP_ID(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_ID, DX_M1A_SYSTEM_ID_RMAP_ID_MASK)
#define READ_DX_M1A_SYSTEM_ID_RMAP_ID(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_ID, DX_M1A_SYSTEM_ID_RMAP_ID_MASK, DX_M1A_SYSTEM_ID_RMAP_ID_OFFSET)
#define WRITE_DX_M1A_SYSTEM_ID_RMAP_ID(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_ID, bits, DX_M1A_SYSTEM_ID_RMAP_ID_MASK, DX_M1A_SYSTEM_ID_RMAP_ID_OFFSET)

/* DX_M1A_SYSTEM.ID.REVISION_ID (Category: SYSTEM) */
#define DX_M1A_SYSTEM_ID_REVISION_ID_LEN        6
#define DX_M1A_SYSTEM_ID_REVISION_ID_OFFSET     16
#define DX_M1A_SYSTEM_ID_REVISION_ID_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_SYSTEM_ID_REVISION_ID_MASK       0x003f0000 /* [21:16] 0b00000000001111110000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_ID_REVISION_ID(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_ID, DX_M1A_SYSTEM_ID_REVISION_ID_MASK)
#define READ_DX_M1A_SYSTEM_ID_REVISION_ID(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_ID, DX_M1A_SYSTEM_ID_REVISION_ID_MASK, DX_M1A_SYSTEM_ID_REVISION_ID_OFFSET)
#define WRITE_DX_M1A_SYSTEM_ID_REVISION_ID(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_ID, bits, DX_M1A_SYSTEM_ID_REVISION_ID_MASK, DX_M1A_SYSTEM_ID_REVISION_ID_OFFSET)

/* DX_M1A_SYSTEM.ID.NPU_ID (Category: SYSTEM) */
#define DX_M1A_SYSTEM_ID_NPU_ID_LEN        2
#define DX_M1A_SYSTEM_ID_NPU_ID_OFFSET     22
#define DX_M1A_SYSTEM_ID_NPU_ID_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_SYSTEM_ID_NPU_ID_MASK       0x00c00000 /* [23:22] 0b00000000110000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_ID_NPU_ID(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_ID, DX_M1A_SYSTEM_ID_NPU_ID_MASK)
#define READ_DX_M1A_SYSTEM_ID_NPU_ID(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_ID, DX_M1A_SYSTEM_ID_NPU_ID_MASK, DX_M1A_SYSTEM_ID_NPU_ID_OFFSET)
#define WRITE_DX_M1A_SYSTEM_ID_NPU_ID(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_ID, bits, DX_M1A_SYSTEM_ID_NPU_ID_MASK, DX_M1A_SYSTEM_ID_NPU_ID_OFFSET)

/* DX_M1A_SYSTEM.ID.DEVICE_ID (Category: SYSTEM) */
#define DX_M1A_SYSTEM_ID_DEVICE_ID_LEN        8
#define DX_M1A_SYSTEM_ID_DEVICE_ID_OFFSET     24
#define DX_M1A_SYSTEM_ID_DEVICE_ID_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_SYSTEM_ID_DEVICE_ID_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_ID_DEVICE_ID(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_ID, DX_M1A_SYSTEM_ID_DEVICE_ID_MASK)
#define READ_DX_M1A_SYSTEM_ID_DEVICE_ID(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_ID, DX_M1A_SYSTEM_ID_DEVICE_ID_MASK, DX_M1A_SYSTEM_ID_DEVICE_ID_OFFSET)
#define WRITE_DX_M1A_SYSTEM_ID_DEVICE_ID(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_ID, bits, DX_M1A_SYSTEM_ID_DEVICE_ID_MASK, DX_M1A_SYSTEM_ID_DEVICE_ID_OFFSET)

/* DX_M1A_SYSTEM.RUN_OPT (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_RUN_OPT(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_RUN_OPT)
#define WRITE_DX_M1A_SYSTEM_RUN_OPT(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_RUN_OPT, val)
/* DX_M1A_SYSTEM.RUN_OPT.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_RUN_OPT_RESERVED0_LEN        31
#define DX_M1A_SYSTEM_RUN_OPT_RESERVED0_OFFSET     0
#define DX_M1A_SYSTEM_RUN_OPT_RESERVED0_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1A_SYSTEM_RUN_OPT_RESERVED0_MASK       0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_RUN_OPT_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_RUN_OPT, DX_M1A_SYSTEM_RUN_OPT_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_RUN_OPT_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_RUN_OPT, DX_M1A_SYSTEM_RUN_OPT_RESERVED0_MASK, DX_M1A_SYSTEM_RUN_OPT_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_RUN_OPT_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_RUN_OPT, bits, DX_M1A_SYSTEM_RUN_OPT_RESERVED0_MASK, DX_M1A_SYSTEM_RUN_OPT_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.RUN_OPT.AUTORUN_EN (Category: SYSTEM) */
#define DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN_LEN        1
#define DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN_OFFSET     31
#define DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_RUN_OPT, DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN_MASK)
#define READ_DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_RUN_OPT, DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN_MASK, DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN_OFFSET)
#define WRITE_DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_RUN_OPT, bits, DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN_MASK, DX_M1A_SYSTEM_RUN_OPT_AUTORUN_EN_OFFSET)

/* DX_M1A_SYSTEM.STATUS1 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_STATUS1(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_STATUS1)
#define WRITE_DX_M1A_SYSTEM_STATUS1(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_STATUS1, val)
/* DX_M1A_SYSTEM.STATUS1.CMD_NUM (Category: SYSTEM) */
#define DX_M1A_SYSTEM_STATUS1_CMD_NUM_LEN        16
#define DX_M1A_SYSTEM_STATUS1_CMD_NUM_OFFSET     0
#define DX_M1A_SYSTEM_STATUS1_CMD_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_SYSTEM_STATUS1_CMD_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_STATUS1_CMD_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_STATUS1, DX_M1A_SYSTEM_STATUS1_CMD_NUM_MASK)
#define READ_DX_M1A_SYSTEM_STATUS1_CMD_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_STATUS1, DX_M1A_SYSTEM_STATUS1_CMD_NUM_MASK, DX_M1A_SYSTEM_STATUS1_CMD_NUM_OFFSET)
#define WRITE_DX_M1A_SYSTEM_STATUS1_CMD_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_STATUS1, bits, DX_M1A_SYSTEM_STATUS1_CMD_NUM_MASK, DX_M1A_SYSTEM_STATUS1_CMD_NUM_OFFSET)

/* DX_M1A_SYSTEM.STATUS1.ARGMAX_ID (Category: SYSTEM) */
#define DX_M1A_SYSTEM_STATUS1_ARGMAX_ID_LEN        16
#define DX_M1A_SYSTEM_STATUS1_ARGMAX_ID_OFFSET     16
#define DX_M1A_SYSTEM_STATUS1_ARGMAX_ID_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_SYSTEM_STATUS1_ARGMAX_ID_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_STATUS1_ARGMAX_ID(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_STATUS1, DX_M1A_SYSTEM_STATUS1_ARGMAX_ID_MASK)
#define READ_DX_M1A_SYSTEM_STATUS1_ARGMAX_ID(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_STATUS1, DX_M1A_SYSTEM_STATUS1_ARGMAX_ID_MASK, DX_M1A_SYSTEM_STATUS1_ARGMAX_ID_OFFSET)
#define WRITE_DX_M1A_SYSTEM_STATUS1_ARGMAX_ID(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_STATUS1, bits, DX_M1A_SYSTEM_STATUS1_ARGMAX_ID_MASK, DX_M1A_SYSTEM_STATUS1_ARGMAX_ID_OFFSET)

/* DX_M1A_SYSTEM.STATUS2 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_STATUS2(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_STATUS2)
#define WRITE_DX_M1A_SYSTEM_STATUS2(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_STATUS2, val)
/* DX_M1A_SYSTEM.STATUS2.BUSY (Category: SYSTEM) */
#define DX_M1A_SYSTEM_STATUS2_BUSY_LEN        1
#define DX_M1A_SYSTEM_STATUS2_BUSY_OFFSET     0
#define DX_M1A_SYSTEM_STATUS2_BUSY_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_STATUS2_BUSY_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_SYSTEM_STATUS2_BUSY(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_STATUS2, DX_M1A_SYSTEM_STATUS2_BUSY_MASK)
#define READ_DX_M1A_SYSTEM_STATUS2_BUSY(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_STATUS2, DX_M1A_SYSTEM_STATUS2_BUSY_MASK, DX_M1A_SYSTEM_STATUS2_BUSY_OFFSET)
#define WRITE_DX_M1A_SYSTEM_STATUS2_BUSY(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_STATUS2, bits, DX_M1A_SYSTEM_STATUS2_BUSY_MASK, DX_M1A_SYSTEM_STATUS2_BUSY_OFFSET)

/* DX_M1A_SYSTEM.STATUS2.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_STATUS2_RESERVED0_LEN        31
#define DX_M1A_SYSTEM_STATUS2_RESERVED0_OFFSET     1
#define DX_M1A_SYSTEM_STATUS2_RESERVED0_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1A_SYSTEM_STATUS2_RESERVED0_MASK       0xfffffffe /* [31:1] 0b11111111111111111111111111111110 */
#define READ_MASK_DX_M1A_SYSTEM_STATUS2_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_STATUS2, DX_M1A_SYSTEM_STATUS2_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_STATUS2_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_STATUS2, DX_M1A_SYSTEM_STATUS2_RESERVED0_MASK, DX_M1A_SYSTEM_STATUS2_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_STATUS2_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_STATUS2, bits, DX_M1A_SYSTEM_STATUS2_RESERVED0_MASK, DX_M1A_SYSTEM_STATUS2_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.NET_INFO (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_NET_INFO(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_NET_INFO)
#define WRITE_DX_M1A_SYSTEM_NET_INFO(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_NET_INFO, val)
/* DX_M1A_SYSTEM.NET_INFO.LAST_CMD_NUM (Category: SYSTEM) */
#define DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM_LEN        16
#define DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM_OFFSET     0
#define DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_NET_INFO, DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM_MASK)
#define READ_DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_NET_INFO, DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM_MASK, DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM_OFFSET)
#define WRITE_DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_NET_INFO, bits, DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM_MASK, DX_M1A_SYSTEM_NET_INFO_LAST_CMD_NUM_OFFSET)

/* DX_M1A_SYSTEM.NET_INFO.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_NET_INFO_RESERVED0_LEN        16
#define DX_M1A_SYSTEM_NET_INFO_RESERVED0_OFFSET     16
#define DX_M1A_SYSTEM_NET_INFO_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_SYSTEM_NET_INFO_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_NET_INFO_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_NET_INFO, DX_M1A_SYSTEM_NET_INFO_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_NET_INFO_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_NET_INFO, DX_M1A_SYSTEM_NET_INFO_RESERVED0_MASK, DX_M1A_SYSTEM_NET_INFO_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_NET_INFO_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_NET_INFO, bits, DX_M1A_SYSTEM_NET_INFO_RESERVED0_MASK, DX_M1A_SYSTEM_NET_INFO_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_IRQ_STATUS(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, val)
/* DX_M1A_SYSTEM.IRQ_STATUS.STATUS_TILE (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE_LEN        16
#define DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE_OFFSET     0
#define DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE_MASK, DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE_MASK, DX_M1A_SYSTEM_IRQ_STATUS_STATUS_TILE_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.STATUS_OP (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP_LEN        4
#define DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP_OFFSET     16
#define DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP_MASK       0x000f0000 /* [19:16] 0b00000000000011110000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP_MASK, DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP_MASK, DX_M1A_SYSTEM_IRQ_STATUS_STATUS_OP_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_MEM_COLLISION (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION_OFFSET     20
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION_MASK       0x00100000 /* [20:20] 0b00000000000100000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_MEM_COLLISION_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_CMD_REACHED (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED_OFFSET     21
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED_MASK       0x00200000 /* [21:21] 0b00000000001000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_REACHED_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_CMD_CKSUM (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM_OFFSET     22
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM_MASK       0x00400000 /* [22:22] 0b00000000010000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_CMD_CKSUM_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_NPU_FIFO (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO_OFFSET     23
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO_MASK       0x00800000 /* [23:23] 0b00000000100000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_NPU_FIFO_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_DMA_FIFO_CCLK (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK_OFFSET     24
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_CCLK_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_DMA_FIFO_NCLK (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK_OFFSET     25
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK_MASK       0x02000000 /* [25:25] 0b00000010000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_NCLK_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_DMA_FIFO_ACLK (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK_OFFSET     26
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK_MASK       0x04000000 /* [26:26] 0b00000100000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_DMA_FIFO_ACLK_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_TIMEOUT_OP (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP_OFFSET     27
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP_MASK       0x08000000 /* [27:27] 0b00001000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_OP_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_TIMEOUT_DMA (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA_OFFSET     28
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA_MASK       0x10000000 /* [28:28] 0b00010000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TIMEOUT_DMA_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_INPUT_DONE (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE_OFFSET     29
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE_MASK       0x20000000 /* [29:29] 0b00100000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_INPUT_DONE_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_TILE (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE_OFFSET     30
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_TILE_OFFSET)

/* DX_M1A_SYSTEM.IRQ_STATUS.IRQ_FRAME (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME_LEN        1
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME_OFFSET     31
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_STATUS, bits, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME_MASK, DX_M1A_SYSTEM_IRQ_STATUS_IRQ_FRAME_OFFSET)

/* DX_M1A_SYSTEM.IRQ_SET (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_IRQ_SET(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, val)
/* DX_M1A_SYSTEM.IRQ_SET.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_SET_RESERVED0_LEN        20
#define DX_M1A_SYSTEM_IRQ_SET_RESERVED0_OFFSET     0
#define DX_M1A_SYSTEM_IRQ_SET_RESERVED0_MASK_BITS  0x000fffff /* [19:0] 0b00000000000011111111111111111111 */
#define DX_M1A_SYSTEM_IRQ_SET_RESERVED0_MASK       0x000fffff /* [19:0] 0b00000000000011111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_SET_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_SET_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_RESERVED0_MASK, DX_M1A_SYSTEM_IRQ_SET_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, bits, DX_M1A_SYSTEM_IRQ_SET_RESERVED0_MASK, DX_M1A_SYSTEM_IRQ_SET_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.IRQ_SET.IRQ_MEM_COLLISION_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET_LEN        1
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET_OFFSET     20
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET_MASK       0x00100000 /* [20:20] 0b00000000000100000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, bits, DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_MEM_COLLISION_SET_OFFSET)

/* DX_M1A_SYSTEM.IRQ_SET.IRQ_CMD_REACHED_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET_LEN        1
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET_OFFSET     21
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET_MASK       0x00200000 /* [21:21] 0b00000000001000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, bits, DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_REACHED_SET_OFFSET)

/* DX_M1A_SYSTEM.IRQ_SET.IRQ_CMD_CKSUM_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET_LEN        1
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET_OFFSET     22
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET_MASK       0x00400000 /* [22:22] 0b00000000010000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, bits, DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_CMD_CKSUM_SET_OFFSET)

/* DX_M1A_SYSTEM.IRQ_SET.IRQ_NPU_FIFO_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET_LEN        1
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET_OFFSET     23
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET_MASK       0x00800000 /* [23:23] 0b00000000100000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, bits, DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_NPU_FIFO_SET_OFFSET)

/* DX_M1A_SYSTEM.IRQ_SET.IRQ_DMA_FIFO_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET_LEN        3
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET_OFFSET     24
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET_MASK       0x07000000 /* [26:24] 0b00000111000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, bits, DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_DMA_FIFO_SET_OFFSET)

/* DX_M1A_SYSTEM.IRQ_SET.IRQ_TIMEOUT_OP_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET_LEN        1
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET_OFFSET     27
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET_MASK       0x08000000 /* [27:27] 0b00001000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, bits, DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_OP_SET_OFFSET)

/* DX_M1A_SYSTEM.IRQ_SET.IRQ_TIMEOUT_DMA_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET_LEN        1
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET_OFFSET     28
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET_MASK       0x10000000 /* [28:28] 0b00010000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, bits, DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_TIMEOUT_DMA_SET_OFFSET)

/* DX_M1A_SYSTEM.IRQ_SET.IRQ_INPUT_DONE_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET_LEN        1
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET_OFFSET     29
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET_MASK       0x20000000 /* [29:29] 0b00100000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, bits, DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_INPUT_DONE_SET_OFFSET)

/* DX_M1A_SYSTEM.IRQ_SET.IRQ_TILE_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET_LEN        1
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET_OFFSET     30
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, bits, DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_TILE_SET_OFFSET)

/* DX_M1A_SYSTEM.IRQ_SET.IRQ_FRAME_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET_LEN        1
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET_OFFSET     31
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_SET, bits, DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET_MASK, DX_M1A_SYSTEM_IRQ_SET_IRQ_FRAME_SET_OFFSET)

/* DX_M1A_SYSTEM.IRQ_CNT0 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_IRQ_CNT0(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT0)
#define WRITE_DX_M1A_SYSTEM_IRQ_CNT0(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT0, val)
/* DX_M1A_SYSTEM.IRQ_CNT0.IRQ_CNT_NPU_NORMAL (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL_LEN        32
#define DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL_OFFSET     0
#define DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT0, DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT0, DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL_MASK, DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT0, bits, DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL_MASK, DX_M1A_SYSTEM_IRQ_CNT0_IRQ_CNT_NPU_NORMAL_OFFSET)

/* DX_M1A_SYSTEM.IRQ_CNT1 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_IRQ_CNT1(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT1)
#define WRITE_DX_M1A_SYSTEM_IRQ_CNT1(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT1, val)
/* DX_M1A_SYSTEM.IRQ_CNT1.IRQ_CNT_NPU_INPUT_DONE (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE_LEN        32
#define DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE_OFFSET     0
#define DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT1, DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT1, DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE_MASK, DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT1, bits, DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE_MASK, DX_M1A_SYSTEM_IRQ_CNT1_IRQ_CNT_NPU_INPUT_DONE_OFFSET)

/* DX_M1A_SYSTEM.IRQ_CNT2 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_IRQ_CNT2(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT2)
#define WRITE_DX_M1A_SYSTEM_IRQ_CNT2(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT2, val)
/* DX_M1A_SYSTEM.IRQ_CNT2.IRQ_CNT_NPU_ABNORMAL (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL_LEN        32
#define DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL_OFFSET     0
#define DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT2, DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT2, DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL_MASK, DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT2, bits, DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL_MASK, DX_M1A_SYSTEM_IRQ_CNT2_IRQ_CNT_NPU_ABNORMAL_OFFSET)

/* DX_M1A_SYSTEM.IRQ_CNT3 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_IRQ_CNT3(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT3)
#define WRITE_DX_M1A_SYSTEM_IRQ_CNT3(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT3, val)
/* DX_M1A_SYSTEM.IRQ_CNT3.IRQ_CNT_CPU_FINISH (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH_LEN        32
#define DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH_OFFSET     0
#define DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT3, DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT3, DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH_MASK, DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT3, bits, DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH_MASK, DX_M1A_SYSTEM_IRQ_CNT3_IRQ_CNT_CPU_FINISH_OFFSET)

/* DX_M1A_SYSTEM.IRQ_CNT4 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_IRQ_CNT4(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT4)
#define WRITE_DX_M1A_SYSTEM_IRQ_CNT4(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT4, val)
/* DX_M1A_SYSTEM.IRQ_CNT4.IRQ_CNT_CPU_INIT (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT_LEN        32
#define DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT_OFFSET     0
#define DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT4, DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT4, DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT_MASK, DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_CNT4, bits, DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT_MASK, DX_M1A_SYSTEM_IRQ_CNT4_IRQ_CNT_CPU_INIT_OFFSET)

/* DX_M1A_SYSTEM.IN_FEATURE_BASE (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_IN_FEATURE_BASE(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_IN_FEATURE_BASE)
#define WRITE_DX_M1A_SYSTEM_IN_FEATURE_BASE(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_IN_FEATURE_BASE, val)
/* DX_M1A_SYSTEM.IN_FEATURE_BASE.IN_FEATURE_OFFSET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET_LEN        32
#define DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET_OFFSET     0
#define DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IN_FEATURE_BASE, DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET_MASK)
#define READ_DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IN_FEATURE_BASE, DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET_MASK, DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IN_FEATURE_BASE, bits, DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET_MASK, DX_M1A_SYSTEM_IN_FEATURE_BASE_IN_FEATURE_OFFSET_OFFSET)

/* DX_M1A_SYSTEM.OUT_FEATURE_BASE (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_OUT_FEATURE_BASE(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_OUT_FEATURE_BASE)
#define WRITE_DX_M1A_SYSTEM_OUT_FEATURE_BASE(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_OUT_FEATURE_BASE, val)
/* DX_M1A_SYSTEM.OUT_FEATURE_BASE.OUT_FEATURE_OFFSET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET_LEN        32
#define DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET_OFFSET     0
#define DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_OUT_FEATURE_BASE, DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET_MASK)
#define READ_DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_OUT_FEATURE_BASE, DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET_MASK, DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_OUT_FEATURE_BASE, bits, DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET_MASK, DX_M1A_SYSTEM_OUT_FEATURE_BASE_OUT_FEATURE_OFFSET_OFFSET)

/* DX_M1A_SYSTEM.WEIGHT_BASE (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_WEIGHT_BASE(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_WEIGHT_BASE)
#define WRITE_DX_M1A_SYSTEM_WEIGHT_BASE(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_WEIGHT_BASE, val)
/* DX_M1A_SYSTEM.WEIGHT_BASE.WEIGHT_OFFSET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET_LEN        32
#define DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET_OFFSET     0
#define DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_WEIGHT_BASE, DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET_MASK)
#define READ_DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_WEIGHT_BASE, DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET_MASK, DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_WEIGHT_BASE, bits, DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET_MASK, DX_M1A_SYSTEM_WEIGHT_BASE_WEIGHT_OFFSET_OFFSET)

/* DX_M1A_SYSTEM.IRQ_TIMER_DMA (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_IRQ_TIMER_DMA(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_IRQ_TIMER_DMA)
#define WRITE_DX_M1A_SYSTEM_IRQ_TIMER_DMA(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_IRQ_TIMER_DMA, val)
/* DX_M1A_SYSTEM.IRQ_TIMER_DMA.IRQ_TIMER_DMA (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA_LEN        32
#define DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA_OFFSET     0
#define DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_TIMER_DMA, DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_TIMER_DMA, DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA_MASK, DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_TIMER_DMA, bits, DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA_MASK, DX_M1A_SYSTEM_IRQ_TIMER_DMA_IRQ_TIMER_DMA_OFFSET)

/* DX_M1A_SYSTEM.IRQ_TIMER_OP (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_IRQ_TIMER_OP(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_IRQ_TIMER_OP)
#define WRITE_DX_M1A_SYSTEM_IRQ_TIMER_OP(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_IRQ_TIMER_OP, val)
/* DX_M1A_SYSTEM.IRQ_TIMER_OP.IRQ_TIMER_OP (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP_LEN        32
#define DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP_OFFSET     0
#define DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_TIMER_OP, DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_TIMER_OP, DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP_MASK, DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_TIMER_OP, bits, DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP_MASK, DX_M1A_SYSTEM_IRQ_TIMER_OP_IRQ_TIMER_OP_OFFSET)

/* DX_M1A_SYSTEM.CMD (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_CMD(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_CMD)
#define WRITE_DX_M1A_SYSTEM_CMD(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_CMD, val)
/* DX_M1A_SYSTEM.CMD.START (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CMD_START_LEN        1
#define DX_M1A_SYSTEM_CMD_START_OFFSET     0
#define DX_M1A_SYSTEM_CMD_START_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_CMD_START_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_SYSTEM_CMD_START(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CMD, DX_M1A_SYSTEM_CMD_START_MASK)
#define READ_DX_M1A_SYSTEM_CMD_START(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CMD, DX_M1A_SYSTEM_CMD_START_MASK, DX_M1A_SYSTEM_CMD_START_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CMD_START(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CMD, bits, DX_M1A_SYSTEM_CMD_START_MASK, DX_M1A_SYSTEM_CMD_START_OFFSET)

/* DX_M1A_SYSTEM.CMD.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CMD_RESERVED0_LEN        31
#define DX_M1A_SYSTEM_CMD_RESERVED0_OFFSET     1
#define DX_M1A_SYSTEM_CMD_RESERVED0_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1A_SYSTEM_CMD_RESERVED0_MASK       0xfffffffe /* [31:1] 0b11111111111111111111111111111110 */
#define READ_MASK_DX_M1A_SYSTEM_CMD_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CMD, DX_M1A_SYSTEM_CMD_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_CMD_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CMD, DX_M1A_SYSTEM_CMD_RESERVED0_MASK, DX_M1A_SYSTEM_CMD_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CMD_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CMD, bits, DX_M1A_SYSTEM_CMD_RESERVED0_MASK, DX_M1A_SYSTEM_CMD_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.SWREG0 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG0(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG0)
#define WRITE_DX_M1A_SYSTEM_SWREG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG0, val)
/* DX_M1A_SYSTEM.SWREG0.SWREG0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG0_SWREG0_LEN        32
#define DX_M1A_SYSTEM_SWREG0_SWREG0_OFFSET     0
#define DX_M1A_SYSTEM_SWREG0_SWREG0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG0_SWREG0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG0_SWREG0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG0, DX_M1A_SYSTEM_SWREG0_SWREG0_MASK)
#define READ_DX_M1A_SYSTEM_SWREG0_SWREG0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG0, DX_M1A_SYSTEM_SWREG0_SWREG0_MASK, DX_M1A_SYSTEM_SWREG0_SWREG0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG0_SWREG0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG0, bits, DX_M1A_SYSTEM_SWREG0_SWREG0_MASK, DX_M1A_SYSTEM_SWREG0_SWREG0_OFFSET)

/* DX_M1A_SYSTEM.SWREG1 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG1(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG1)
#define WRITE_DX_M1A_SYSTEM_SWREG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG1, val)
/* DX_M1A_SYSTEM.SWREG1.SWREG1 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG1_SWREG1_LEN        32
#define DX_M1A_SYSTEM_SWREG1_SWREG1_OFFSET     0
#define DX_M1A_SYSTEM_SWREG1_SWREG1_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG1_SWREG1_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG1_SWREG1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG1, DX_M1A_SYSTEM_SWREG1_SWREG1_MASK)
#define READ_DX_M1A_SYSTEM_SWREG1_SWREG1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG1, DX_M1A_SYSTEM_SWREG1_SWREG1_MASK, DX_M1A_SYSTEM_SWREG1_SWREG1_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG1_SWREG1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG1, bits, DX_M1A_SYSTEM_SWREG1_SWREG1_MASK, DX_M1A_SYSTEM_SWREG1_SWREG1_OFFSET)

/* DX_M1A_SYSTEM.SWREG2 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG2(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG2)
#define WRITE_DX_M1A_SYSTEM_SWREG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG2, val)
/* DX_M1A_SYSTEM.SWREG2.SWREG2 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG2_SWREG2_LEN        32
#define DX_M1A_SYSTEM_SWREG2_SWREG2_OFFSET     0
#define DX_M1A_SYSTEM_SWREG2_SWREG2_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG2_SWREG2_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG2_SWREG2(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG2, DX_M1A_SYSTEM_SWREG2_SWREG2_MASK)
#define READ_DX_M1A_SYSTEM_SWREG2_SWREG2(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG2, DX_M1A_SYSTEM_SWREG2_SWREG2_MASK, DX_M1A_SYSTEM_SWREG2_SWREG2_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG2_SWREG2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG2, bits, DX_M1A_SYSTEM_SWREG2_SWREG2_MASK, DX_M1A_SYSTEM_SWREG2_SWREG2_OFFSET)

/* DX_M1A_SYSTEM.SWREG3 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG3(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG3)
#define WRITE_DX_M1A_SYSTEM_SWREG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG3, val)
/* DX_M1A_SYSTEM.SWREG3.SWREG3 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG3_SWREG3_LEN        32
#define DX_M1A_SYSTEM_SWREG3_SWREG3_OFFSET     0
#define DX_M1A_SYSTEM_SWREG3_SWREG3_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG3_SWREG3_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG3_SWREG3(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG3, DX_M1A_SYSTEM_SWREG3_SWREG3_MASK)
#define READ_DX_M1A_SYSTEM_SWREG3_SWREG3(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG3, DX_M1A_SYSTEM_SWREG3_SWREG3_MASK, DX_M1A_SYSTEM_SWREG3_SWREG3_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG3_SWREG3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG3, bits, DX_M1A_SYSTEM_SWREG3_SWREG3_MASK, DX_M1A_SYSTEM_SWREG3_SWREG3_OFFSET)

/* DX_M1A_SYSTEM.IRQ_EXT_CLR_OPT (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT)
#define WRITE_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT, val)
/* DX_M1A_SYSTEM.IRQ_EXT_CLR_OPT.IRQ_EXT_CLR_OPT (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT_LEN        1
#define DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT_OFFSET     0
#define DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT, DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT, DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT_MASK, DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT, bits, DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT_MASK, DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_IRQ_EXT_CLR_OPT_OFFSET)

/* DX_M1A_SYSTEM.IRQ_EXT_CLR_OPT.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0_LEN        31
#define DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0_OFFSET     1
#define DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0_MASK       0xfffffffe /* [31:1] 0b11111111111111111111111111111110 */
#define READ_MASK_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT, DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT, DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0_MASK, DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT, bits, DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0_MASK, DX_M1A_SYSTEM_IRQ_EXT_CLR_OPT_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.RSV0 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_RSV0(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_RSV0)
#define WRITE_DX_M1A_SYSTEM_RSV0(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_RSV0, val)
/* DX_M1A_SYSTEM.RSV0.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_RSV0_RESERVED0_LEN        32
#define DX_M1A_SYSTEM_RSV0_RESERVED0_OFFSET     0
#define DX_M1A_SYSTEM_RSV0_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_RSV0_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_RSV0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_RSV0, DX_M1A_SYSTEM_RSV0_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_RSV0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_RSV0, DX_M1A_SYSTEM_RSV0_RESERVED0_MASK, DX_M1A_SYSTEM_RSV0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_RSV0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_RSV0, bits, DX_M1A_SYSTEM_RSV0_RESERVED0_MASK, DX_M1A_SYSTEM_RSV0_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.RSV1 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_RSV1(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_RSV1)
#define WRITE_DX_M1A_SYSTEM_RSV1(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_RSV1, val)
/* DX_M1A_SYSTEM.RSV1.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_RSV1_RESERVED0_LEN        32
#define DX_M1A_SYSTEM_RSV1_RESERVED0_OFFSET     0
#define DX_M1A_SYSTEM_RSV1_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_RSV1_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_RSV1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_RSV1, DX_M1A_SYSTEM_RSV1_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_RSV1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_RSV1, DX_M1A_SYSTEM_RSV1_RESERVED0_MASK, DX_M1A_SYSTEM_RSV1_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_RSV1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_RSV1, bits, DX_M1A_SYSTEM_RSV1_RESERVED0_MASK, DX_M1A_SYSTEM_RSV1_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.RSV2 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_RSV2(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_RSV2)
#define WRITE_DX_M1A_SYSTEM_RSV2(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_RSV2, val)
/* DX_M1A_SYSTEM.RSV2.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_RSV2_RESERVED0_LEN        32
#define DX_M1A_SYSTEM_RSV2_RESERVED0_OFFSET     0
#define DX_M1A_SYSTEM_RSV2_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_RSV2_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_RSV2_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_RSV2, DX_M1A_SYSTEM_RSV2_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_RSV2_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_RSV2, DX_M1A_SYSTEM_RSV2_RESERVED0_MASK, DX_M1A_SYSTEM_RSV2_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_RSV2_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_RSV2, bits, DX_M1A_SYSTEM_RSV2_RESERVED0_MASK, DX_M1A_SYSTEM_RSV2_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.CPU_STATUS (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_CPU_STATUS(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS)
#define WRITE_DX_M1A_SYSTEM_CPU_STATUS(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, val)
/* DX_M1A_SYSTEM.CPU_STATUS.CPU_LOCKUP (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP_LEN        1
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP_OFFSET     0
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP_MASK)
#define READ_DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP_MASK, DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, bits, DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP_MASK, DX_M1A_SYSTEM_CPU_STATUS_CPU_LOCKUP_OFFSET)

/* DX_M1A_SYSTEM.CPU_STATUS.CPU_HALTED (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED_LEN        1
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED_OFFSET     1
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED_MASK       0x00000002 /* [1:1] 0b00000000000000000000000000000010 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED_MASK)
#define READ_DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED_MASK, DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, bits, DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED_MASK, DX_M1A_SYSTEM_CPU_STATUS_CPU_HALTED_OFFSET)

/* DX_M1A_SYSTEM.CPU_STATUS.CPU_SLEEPDEEP (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP_LEN        1
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP_OFFSET     2
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP_MASK       0x00000004 /* [2:2] 0b00000000000000000000000000000100 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP_MASK)
#define READ_DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP_MASK, DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, bits, DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP_MASK, DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPDEEP_OFFSET)

/* DX_M1A_SYSTEM.CPU_STATUS.CPU_SLEEPING (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING_LEN        1
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING_OFFSET     3
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING_MASK       0x00000008 /* [3:3] 0b00000000000000000000000000001000 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING_MASK)
#define READ_DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING_MASK, DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, bits, DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING_MASK, DX_M1A_SYSTEM_CPU_STATUS_CPU_SLEEPING_OFFSET)

/* DX_M1A_SYSTEM.CPU_STATUS.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_STATUS_RESERVED0_LEN        28
#define DX_M1A_SYSTEM_CPU_STATUS_RESERVED0_OFFSET     4
#define DX_M1A_SYSTEM_CPU_STATUS_RESERVED0_MASK_BITS  0x0fffffff /* [27:0] 0b00001111111111111111111111111111 */
#define DX_M1A_SYSTEM_CPU_STATUS_RESERVED0_MASK       0xfffffff0 /* [31:4] 0b11111111111111111111111111110000 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_STATUS_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, DX_M1A_SYSTEM_CPU_STATUS_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_CPU_STATUS_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, DX_M1A_SYSTEM_CPU_STATUS_RESERVED0_MASK, DX_M1A_SYSTEM_CPU_STATUS_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_STATUS_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_STATUS, bits, DX_M1A_SYSTEM_CPU_STATUS_RESERVED0_MASK, DX_M1A_SYSTEM_CPU_STATUS_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.CPU_IRQ_SET (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_CPU_IRQ_SET(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_SET)
#define WRITE_DX_M1A_SYSTEM_CPU_IRQ_SET(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_SET, val)
/* DX_M1A_SYSTEM.CPU_IRQ_SET.IRQ_CPU_INIT_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET_LEN        1
#define DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET_OFFSET     0
#define DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_SET, DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET_MASK)
#define READ_DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_SET, DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET_MASK, DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_SET, bits, DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET_MASK, DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_INIT_SET_OFFSET)

/* DX_M1A_SYSTEM.CPU_IRQ_SET.IRQ_CPU_FINISH_SET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET_LEN        1
#define DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET_OFFSET     1
#define DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET_MASK       0x00000002 /* [1:1] 0b00000000000000000000000000000010 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_SET, DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET_MASK)
#define READ_DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_SET, DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET_MASK, DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_SET, bits, DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET_MASK, DX_M1A_SYSTEM_CPU_IRQ_SET_IRQ_CPU_FINISH_SET_OFFSET)

/* DX_M1A_SYSTEM.CPU_IRQ_SET.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0_LEN        30
#define DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0_OFFSET     2
#define DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0_MASK_BITS  0x3fffffff /* [29:0] 0b00111111111111111111111111111111 */
#define DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0_MASK       0xfffffffc /* [31:2] 0b11111111111111111111111111111100 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_SET, DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_SET, DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0_MASK, DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_SET, bits, DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0_MASK, DX_M1A_SYSTEM_CPU_IRQ_SET_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.CPU_IRQ_STATUS (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_CPU_IRQ_STATUS(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS)
#define WRITE_DX_M1A_SYSTEM_CPU_IRQ_STATUS(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, val)
/* DX_M1A_SYSTEM.CPU_IRQ_STATUS.IRQ_CPU_INIT (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT_LEN        1
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT_OFFSET     0
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT_MASK)
#define READ_DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT_MASK, DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, bits, DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT_MASK, DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_INIT_OFFSET)

/* DX_M1A_SYSTEM.CPU_IRQ_STATUS.IRQ_CPU_FINISH (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH_LEN        1
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH_OFFSET     1
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH_MASK       0x00000002 /* [1:1] 0b00000000000000000000000000000010 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH_MASK)
#define READ_DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH_MASK, DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, bits, DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH_MASK, DX_M1A_SYSTEM_CPU_IRQ_STATUS_IRQ_CPU_FINISH_OFFSET)

/* DX_M1A_SYSTEM.CPU_IRQ_STATUS.CPU_BUSY (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY_LEN        1
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY_OFFSET     2
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY_MASK       0x00000004 /* [2:2] 0b00000000000000000000000000000100 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY_MASK)
#define READ_DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY_MASK, DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, bits, DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY_MASK, DX_M1A_SYSTEM_CPU_IRQ_STATUS_CPU_BUSY_OFFSET)

/* DX_M1A_SYSTEM.CPU_IRQ_STATUS.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0_LEN        29
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0_OFFSET     3
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0_MASK_BITS  0x1fffffff /* [28:0] 0b00011111111111111111111111111111 */
#define DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0_MASK       0xfffffff8 /* [31:3] 0b11111111111111111111111111111000 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0_MASK, DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_IRQ_STATUS, bits, DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0_MASK, DX_M1A_SYSTEM_CPU_IRQ_STATUS_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.CPU_START (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_CPU_START(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_CPU_START)
#define WRITE_DX_M1A_SYSTEM_CPU_START(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_CPU_START, val)
/* DX_M1A_SYSTEM.CPU_START.CPU_START (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_START_CPU_START_LEN        1
#define DX_M1A_SYSTEM_CPU_START_CPU_START_OFFSET     0
#define DX_M1A_SYSTEM_CPU_START_CPU_START_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_CPU_START_CPU_START_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_START_CPU_START(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_START, DX_M1A_SYSTEM_CPU_START_CPU_START_MASK)
#define READ_DX_M1A_SYSTEM_CPU_START_CPU_START(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_START, DX_M1A_SYSTEM_CPU_START_CPU_START_MASK, DX_M1A_SYSTEM_CPU_START_CPU_START_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_START_CPU_START(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_START, bits, DX_M1A_SYSTEM_CPU_START_CPU_START_MASK, DX_M1A_SYSTEM_CPU_START_CPU_START_OFFSET)

/* DX_M1A_SYSTEM.CPU_START.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_CPU_START_RESERVED0_LEN        31
#define DX_M1A_SYSTEM_CPU_START_RESERVED0_OFFSET     1
#define DX_M1A_SYSTEM_CPU_START_RESERVED0_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1A_SYSTEM_CPU_START_RESERVED0_MASK       0xfffffffe /* [31:1] 0b11111111111111111111111111111110 */
#define READ_MASK_DX_M1A_SYSTEM_CPU_START_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_CPU_START, DX_M1A_SYSTEM_CPU_START_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_CPU_START_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_CPU_START, DX_M1A_SYSTEM_CPU_START_RESERVED0_MASK, DX_M1A_SYSTEM_CPU_START_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_CPU_START_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_CPU_START, bits, DX_M1A_SYSTEM_CPU_START_RESERVED0_MASK, DX_M1A_SYSTEM_CPU_START_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.PPU_OUT_FILTER_INFO (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO)
#define WRITE_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, val)
/* DX_M1A_SYSTEM.PPU_OUT_FILTER_INFO.PPU_FILTERING_NUM (Category: SYSTEM) */
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM_LEN        10
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM_OFFSET     0
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM_MASK       0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define READ_MASK_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM_MASK)
#define READ_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM_MASK, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM_OFFSET)
#define WRITE_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, bits, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM_MASK, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_NUM_OFFSET)

/* DX_M1A_SYSTEM.PPU_OUT_FILTER_INFO.PPU_FILTERING_WNUM (Category: SYSTEM) */
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM_LEN        16
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM_OFFSET     10
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM_MASK       0x03fffc00 /* [25:10] 0b00000011111111111111110000000000 */
#define READ_MASK_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM_MASK)
#define READ_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM_MASK, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM_OFFSET)
#define WRITE_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, bits, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM_MASK, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_WNUM_OFFSET)

/* DX_M1A_SYSTEM.PPU_OUT_FILTER_INFO.PPU_FILTERING_BUFSEL (Category: SYSTEM) */
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL_LEN        1
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL_OFFSET     26
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL_MASK       0x04000000 /* [26:26] 0b00000100000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL_MASK)
#define READ_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL_MASK, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL_OFFSET)
#define WRITE_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, bits, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL_MASK, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_PPU_FILTERING_BUFSEL_OFFSET)

/* DX_M1A_SYSTEM.PPU_OUT_FILTER_INFO.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0_LEN        5
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0_OFFSET     27
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0_MASK       0xf8000000 /* [31:27] 0b11111000000000000000000000000000 */
#define READ_MASK_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0_MASK, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO, bits, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0_MASK, DX_M1A_SYSTEM_PPU_OUT_FILTER_INFO_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.PPU_OUT_FEATURE_INFO (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO)
#define WRITE_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO, val)
/* DX_M1A_SYSTEM.PPU_OUT_FEATURE_INFO.PPU_OUT_FEATURE_NUM (Category: SYSTEM) */
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM_LEN        10
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM_OFFSET     0
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM_MASK       0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define READ_MASK_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO, DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM_MASK)
#define READ_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO, DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM_MASK, DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM_OFFSET)
#define WRITE_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO, bits, DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM_MASK, DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_PPU_OUT_FEATURE_NUM_OFFSET)

/* DX_M1A_SYSTEM.PPU_OUT_FEATURE_INFO.RESERVED0 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0_LEN        22
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0_OFFSET     10
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0_MASK_BITS  0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0_MASK       0xfffffc00 /* [31:10] 0b11111111111111111111110000000000 */
#define READ_MASK_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO, DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0_MASK)
#define READ_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO, DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0_MASK, DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0_OFFSET)
#define WRITE_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO, bits, DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0_MASK, DX_M1A_SYSTEM_PPU_OUT_FEATURE_INFO_RESERVED0_OFFSET)

/* DX_M1A_SYSTEM.PPU_OUT_FEATURE_BASE (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE)
#define WRITE_DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE, val)
/* DX_M1A_SYSTEM.PPU_OUT_FEATURE_BASE.PPU_OUT_FEATURE_OFFSET (Category: SYSTEM) */
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET_LEN        32
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET_OFFSET     0
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE, DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET_MASK)
#define READ_DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE, DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET_MASK, DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET_OFFSET)
#define WRITE_DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE, bits, DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET_MASK, DX_M1A_SYSTEM_PPU_OUT_FEATURE_BASE_PPU_OUT_FEATURE_OFFSET_OFFSET)

/* DX_M1A_SYSTEM.SWREG4 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG4(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG4)
#define WRITE_DX_M1A_SYSTEM_SWREG4(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG4, val)
/* DX_M1A_SYSTEM.SWREG4.SWREG4 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG4_SWREG4_LEN        32
#define DX_M1A_SYSTEM_SWREG4_SWREG4_OFFSET     0
#define DX_M1A_SYSTEM_SWREG4_SWREG4_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG4_SWREG4_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG4_SWREG4(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG4, DX_M1A_SYSTEM_SWREG4_SWREG4_MASK)
#define READ_DX_M1A_SYSTEM_SWREG4_SWREG4(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG4, DX_M1A_SYSTEM_SWREG4_SWREG4_MASK, DX_M1A_SYSTEM_SWREG4_SWREG4_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG4_SWREG4(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG4, bits, DX_M1A_SYSTEM_SWREG4_SWREG4_MASK, DX_M1A_SYSTEM_SWREG4_SWREG4_OFFSET)

/* DX_M1A_SYSTEM.SWREG5 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG5(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG5)
#define WRITE_DX_M1A_SYSTEM_SWREG5(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG5, val)
/* DX_M1A_SYSTEM.SWREG5.SWREG5 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG5_SWREG5_LEN        32
#define DX_M1A_SYSTEM_SWREG5_SWREG5_OFFSET     0
#define DX_M1A_SYSTEM_SWREG5_SWREG5_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG5_SWREG5_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG5_SWREG5(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG5, DX_M1A_SYSTEM_SWREG5_SWREG5_MASK)
#define READ_DX_M1A_SYSTEM_SWREG5_SWREG5(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG5, DX_M1A_SYSTEM_SWREG5_SWREG5_MASK, DX_M1A_SYSTEM_SWREG5_SWREG5_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG5_SWREG5(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG5, bits, DX_M1A_SYSTEM_SWREG5_SWREG5_MASK, DX_M1A_SYSTEM_SWREG5_SWREG5_OFFSET)

/* DX_M1A_SYSTEM.SWREG6 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG6(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG6)
#define WRITE_DX_M1A_SYSTEM_SWREG6(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG6, val)
/* DX_M1A_SYSTEM.SWREG6.SWREG6 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG6_SWREG6_LEN        32
#define DX_M1A_SYSTEM_SWREG6_SWREG6_OFFSET     0
#define DX_M1A_SYSTEM_SWREG6_SWREG6_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG6_SWREG6_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG6_SWREG6(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG6, DX_M1A_SYSTEM_SWREG6_SWREG6_MASK)
#define READ_DX_M1A_SYSTEM_SWREG6_SWREG6(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG6, DX_M1A_SYSTEM_SWREG6_SWREG6_MASK, DX_M1A_SYSTEM_SWREG6_SWREG6_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG6_SWREG6(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG6, bits, DX_M1A_SYSTEM_SWREG6_SWREG6_MASK, DX_M1A_SYSTEM_SWREG6_SWREG6_OFFSET)

/* DX_M1A_SYSTEM.SWREG7 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG7(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG7)
#define WRITE_DX_M1A_SYSTEM_SWREG7(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG7, val)
/* DX_M1A_SYSTEM.SWREG7.SWREG7 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG7_SWREG7_LEN        32
#define DX_M1A_SYSTEM_SWREG7_SWREG7_OFFSET     0
#define DX_M1A_SYSTEM_SWREG7_SWREG7_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG7_SWREG7_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG7_SWREG7(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG7, DX_M1A_SYSTEM_SWREG7_SWREG7_MASK)
#define READ_DX_M1A_SYSTEM_SWREG7_SWREG7(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG7, DX_M1A_SYSTEM_SWREG7_SWREG7_MASK, DX_M1A_SYSTEM_SWREG7_SWREG7_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG7_SWREG7(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG7, bits, DX_M1A_SYSTEM_SWREG7_SWREG7_MASK, DX_M1A_SYSTEM_SWREG7_SWREG7_OFFSET)

/* DX_M1A_SYSTEM.SWREG8 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG8(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG8)
#define WRITE_DX_M1A_SYSTEM_SWREG8(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG8, val)
/* DX_M1A_SYSTEM.SWREG8.SWREG8 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG8_SWREG8_LEN        32
#define DX_M1A_SYSTEM_SWREG8_SWREG8_OFFSET     0
#define DX_M1A_SYSTEM_SWREG8_SWREG8_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG8_SWREG8_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG8_SWREG8(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG8, DX_M1A_SYSTEM_SWREG8_SWREG8_MASK)
#define READ_DX_M1A_SYSTEM_SWREG8_SWREG8(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG8, DX_M1A_SYSTEM_SWREG8_SWREG8_MASK, DX_M1A_SYSTEM_SWREG8_SWREG8_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG8_SWREG8(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG8, bits, DX_M1A_SYSTEM_SWREG8_SWREG8_MASK, DX_M1A_SYSTEM_SWREG8_SWREG8_OFFSET)

/* DX_M1A_SYSTEM.SWREG9 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG9(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG9)
#define WRITE_DX_M1A_SYSTEM_SWREG9(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG9, val)
/* DX_M1A_SYSTEM.SWREG9.SWREG9 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG9_SWREG9_LEN        32
#define DX_M1A_SYSTEM_SWREG9_SWREG9_OFFSET     0
#define DX_M1A_SYSTEM_SWREG9_SWREG9_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG9_SWREG9_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG9_SWREG9(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG9, DX_M1A_SYSTEM_SWREG9_SWREG9_MASK)
#define READ_DX_M1A_SYSTEM_SWREG9_SWREG9(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG9, DX_M1A_SYSTEM_SWREG9_SWREG9_MASK, DX_M1A_SYSTEM_SWREG9_SWREG9_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG9_SWREG9(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG9, bits, DX_M1A_SYSTEM_SWREG9_SWREG9_MASK, DX_M1A_SYSTEM_SWREG9_SWREG9_OFFSET)

/* DX_M1A_SYSTEM.SWREG10 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG10(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG10)
#define WRITE_DX_M1A_SYSTEM_SWREG10(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG10, val)
/* DX_M1A_SYSTEM.SWREG10.SWREG10 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG10_SWREG10_LEN        32
#define DX_M1A_SYSTEM_SWREG10_SWREG10_OFFSET     0
#define DX_M1A_SYSTEM_SWREG10_SWREG10_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG10_SWREG10_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG10_SWREG10(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG10, DX_M1A_SYSTEM_SWREG10_SWREG10_MASK)
#define READ_DX_M1A_SYSTEM_SWREG10_SWREG10(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG10, DX_M1A_SYSTEM_SWREG10_SWREG10_MASK, DX_M1A_SYSTEM_SWREG10_SWREG10_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG10_SWREG10(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG10, bits, DX_M1A_SYSTEM_SWREG10_SWREG10_MASK, DX_M1A_SYSTEM_SWREG10_SWREG10_OFFSET)

/* DX_M1A_SYSTEM.SWREG11 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG11(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG11)
#define WRITE_DX_M1A_SYSTEM_SWREG11(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG11, val)
/* DX_M1A_SYSTEM.SWREG11.SWREG11 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG11_SWREG11_LEN        32
#define DX_M1A_SYSTEM_SWREG11_SWREG11_OFFSET     0
#define DX_M1A_SYSTEM_SWREG11_SWREG11_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG11_SWREG11_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG11_SWREG11(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG11, DX_M1A_SYSTEM_SWREG11_SWREG11_MASK)
#define READ_DX_M1A_SYSTEM_SWREG11_SWREG11(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG11, DX_M1A_SYSTEM_SWREG11_SWREG11_MASK, DX_M1A_SYSTEM_SWREG11_SWREG11_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG11_SWREG11(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG11, bits, DX_M1A_SYSTEM_SWREG11_SWREG11_MASK, DX_M1A_SYSTEM_SWREG11_SWREG11_OFFSET)

/* DX_M1A_SYSTEM.SWREG12 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG12(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG12)
#define WRITE_DX_M1A_SYSTEM_SWREG12(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG12, val)
/* DX_M1A_SYSTEM.SWREG12.SWREG12 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG12_SWREG12_LEN        32
#define DX_M1A_SYSTEM_SWREG12_SWREG12_OFFSET     0
#define DX_M1A_SYSTEM_SWREG12_SWREG12_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG12_SWREG12_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG12_SWREG12(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG12, DX_M1A_SYSTEM_SWREG12_SWREG12_MASK)
#define READ_DX_M1A_SYSTEM_SWREG12_SWREG12(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG12, DX_M1A_SYSTEM_SWREG12_SWREG12_MASK, DX_M1A_SYSTEM_SWREG12_SWREG12_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG12_SWREG12(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG12, bits, DX_M1A_SYSTEM_SWREG12_SWREG12_MASK, DX_M1A_SYSTEM_SWREG12_SWREG12_OFFSET)

/* DX_M1A_SYSTEM.SWREG13 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG13(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG13)
#define WRITE_DX_M1A_SYSTEM_SWREG13(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG13, val)
/* DX_M1A_SYSTEM.SWREG13.SWREG13 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG13_SWREG13_LEN        32
#define DX_M1A_SYSTEM_SWREG13_SWREG13_OFFSET     0
#define DX_M1A_SYSTEM_SWREG13_SWREG13_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG13_SWREG13_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG13_SWREG13(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG13, DX_M1A_SYSTEM_SWREG13_SWREG13_MASK)
#define READ_DX_M1A_SYSTEM_SWREG13_SWREG13(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG13, DX_M1A_SYSTEM_SWREG13_SWREG13_MASK, DX_M1A_SYSTEM_SWREG13_SWREG13_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG13_SWREG13(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG13, bits, DX_M1A_SYSTEM_SWREG13_SWREG13_MASK, DX_M1A_SYSTEM_SWREG13_SWREG13_OFFSET)

/* DX_M1A_SYSTEM.SWREG14 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG14(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG14)
#define WRITE_DX_M1A_SYSTEM_SWREG14(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG14, val)
/* DX_M1A_SYSTEM.SWREG14.SWREG14 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG14_SWREG14_LEN        32
#define DX_M1A_SYSTEM_SWREG14_SWREG14_OFFSET     0
#define DX_M1A_SYSTEM_SWREG14_SWREG14_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG14_SWREG14_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG14_SWREG14(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG14, DX_M1A_SYSTEM_SWREG14_SWREG14_MASK)
#define READ_DX_M1A_SYSTEM_SWREG14_SWREG14(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG14, DX_M1A_SYSTEM_SWREG14_SWREG14_MASK, DX_M1A_SYSTEM_SWREG14_SWREG14_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG14_SWREG14(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG14, bits, DX_M1A_SYSTEM_SWREG14_SWREG14_MASK, DX_M1A_SYSTEM_SWREG14_SWREG14_OFFSET)

/* DX_M1A_SYSTEM.SWREG15 (Category: SYSTEM) */
#define READ_DX_M1A_SYSTEM_SWREG15(base) dxrt::readl((void*)base, REG_DX_M1A_SYSTEM_SWREG15)
#define WRITE_DX_M1A_SYSTEM_SWREG15(base, val) dxrt::writel((void*)base, REG_DX_M1A_SYSTEM_SWREG15, val)
/* DX_M1A_SYSTEM.SWREG15.SWREG15 (Category: SYSTEM) */
#define DX_M1A_SYSTEM_SWREG15_SWREG15_LEN        32
#define DX_M1A_SYSTEM_SWREG15_SWREG15_OFFSET     0
#define DX_M1A_SYSTEM_SWREG15_SWREG15_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_SYSTEM_SWREG15_SWREG15_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_SYSTEM_SWREG15_SWREG15(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_SYSTEM_SWREG15, DX_M1A_SYSTEM_SWREG15_SWREG15_MASK)
#define READ_DX_M1A_SYSTEM_SWREG15_SWREG15(base) \
    dxrt::readb((void*)base, REG_DX_M1A_SYSTEM_SWREG15, DX_M1A_SYSTEM_SWREG15_SWREG15_MASK, DX_M1A_SYSTEM_SWREG15_SWREG15_OFFSET)
#define WRITE_DX_M1A_SYSTEM_SWREG15_SWREG15(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_SYSTEM_SWREG15, bits, DX_M1A_SYSTEM_SWREG15_SWREG15_MASK, DX_M1A_SYSTEM_SWREG15_SWREG15_OFFSET)


int Verify_Dx_m1a_system_Registers(dxDX_M1A_SYSTEM_t* base);

} /* namespace dxrt */
