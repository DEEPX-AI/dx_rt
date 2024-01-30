/* Automatically generated from DX_M1A.xlsx
 * by Deepx Neural Network Generator v1.0
 */
#pragma once

#include <stdint.h>
#include "dxrt/npuif/npuif.h"

namespace dxrt{
extern bool logNpuRegAccess;

/* Address */
#define REG_DX_M1A_DEBUG_OFFSET 0x100
#define REG_DX_M1A_DEBUG_STAMP0    0x00000000 /* 0 */
#define REG_DX_M1A_DEBUG_STAMP1    0x00000004 /* 4 */
#define REG_DX_M1A_DEBUG_STAMP2    0x00000008 /* 8 */
#define REG_DX_M1A_DEBUG_STAMP3    0x0000000c /* 12 */
#define REG_DX_M1A_DEBUG_STAMP4    0x00000010 /* 16 */
#define REG_DX_M1A_DEBUG_STAMP5    0x00000014 /* 20 */
#define REG_DX_M1A_DEBUG_STAMP6    0x00000018 /* 24 */
#define REG_DX_M1A_DEBUG_STAMP7    0x0000001c /* 28 */
#define REG_DX_M1A_DEBUG_STAMP8    0x00000020 /* 32 */
#define REG_DX_M1A_DEBUG_STAMP9    0x00000024 /* 36 */
#define REG_DX_M1A_DEBUG_STAMP10    0x00000028 /* 40 */
#define REG_DX_M1A_DEBUG_STAMP11    0x0000002c /* 44 */
#define REG_DX_M1A_DEBUG_STAMP12    0x00000030 /* 48 */
#define REG_DX_M1A_DEBUG_STAMP13    0x00000034 /* 52 */
#define REG_DX_M1A_DEBUG_STAMP14    0x00000038 /* 56 */
#define REG_DX_M1A_DEBUG_STAMP15    0x0000003c /* 60 */
#define REG_DX_M1A_DEBUG_STAMP16    0x00000040 /* 64 */
#define REG_DX_M1A_DEBUG_STAMP17    0x00000044 /* 68 */
#define REG_DX_M1A_DEBUG_FSM_PE0_RD    0x00000048 /* 72 */
#define REG_DX_M1A_DEBUG_FSM_PE0_WR    0x0000004c /* 76 */
#define REG_DX_M1A_DEBUG_FSM_PE1_RD    0x00000050 /* 80 */
#define REG_DX_M1A_DEBUG_FSM_PE1_WR    0x00000054 /* 84 */
#define REG_DX_M1A_DEBUG_FSM_PE2_RD    0x00000058 /* 88 */
#define REG_DX_M1A_DEBUG_FSM_PE2_WR    0x0000005c /* 92 */
#define REG_DX_M1A_DEBUG_FSM_PE3_RD    0x00000060 /* 96 */
#define REG_DX_M1A_DEBUG_FSM_PE3_WR    0x00000064 /* 100 */
#define REG_DX_M1A_DEBUG_DMA_CMD_ARB    0x00000068 /* 104 */
#define REG_DX_M1A_DEBUG_DMA_AXI_RD    0x0000006c /* 108 */
#define REG_DX_M1A_DEBUG_DMA_AXI_WR    0x00000070 /* 112 */
#define REG_DX_M1A_DEBUG_DMA_MEM_WR    0x00000074 /* 116 */
#define REG_DX_M1A_DEBUG_DMA_MEM_RD    0x00000078 /* 120 */
#define REG_DX_M1A_DEBUG_DMA_MEM_CP    0x0000007c /* 124 */
#define REG_DX_M1A_DEBUG_DMA_SFR_WR    0x00000080 /* 128 */
#define REG_DX_M1A_DEBUG_DMA_CMEM_RD    0x00000084 /* 132 */
#define REG_DX_M1A_DEBUG_DMA_CMEM_WR    0x00000088 /* 136 */
#define REG_DX_M1A_DEBUG_SFU_FIFO    0x0000008c /* 140 */
#define REG_DX_M1A_DEBUG_NPU_DBG0    0x00000090 /* 144 */
#define REG_DX_M1A_DEBUG_NPU_DBG1    0x00000094 /* 148 */
#define REG_DX_M1A_DEBUG_NPU_DBG2    0x00000098 /* 152 */
#define REG_DX_M1A_DEBUG_NPU_DBG3    0x0000009c /* 156 */
#define REG_DX_M1A_DEBUG_NPU_DBG4    0x000000a0 /* 160 */
#define REG_DX_M1A_DEBUG_NPU_DBG5    0x000000a4 /* 164 */
#define REG_DX_M1A_DEBUG_NPU_DBG6    0x000000a8 /* 168 */
#define REG_DX_M1A_DEBUG_NPU_DBG7    0x000000ac /* 172 */
#define REG_DX_M1A_DEBUG_NPU_DBG8    0x000000b0 /* 176 */
#define REG_DX_M1A_DEBUG_NPU_DBG9    0x000000b4 /* 180 */
#define REG_DX_M1A_DEBUG_NPU_DBG10    0x000000b8 /* 184 */
#define REG_DX_M1A_DEBUG_NPU_DBG11    0x000000bc /* 188 */
#define REG_DX_M1A_DEBUG_FSM_RD    0x000000c0 /* 192 */

/* Register Definition */
typedef volatile union _DX_M1A_DEBUG_STAMP0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:30;  /* DX_M1A_DEBUG.STAMP0.RESERVED0 [29:0] (R, 0x0)  */
        volatile uint32_t STAMP_OPT:1;  /* DX_M1A_DEBUG.STAMP0.STAMP_OPT [30:30] (RW, 0x0) 0: EACH LAYER, 1: EACH TILE */
        volatile uint32_t STAMP_EN:1;  /* DX_M1A_DEBUG.STAMP0.STAMP_EN [31:31] (RW, 0x0)  */
    };
} DX_M1A_DEBUG_STAMP0_t;
typedef volatile union _DX_M1A_DEBUG_STAMP1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t TILE_IDX:16;  /* DX_M1A_DEBUG.STAMP1.TILE_IDX [15:0] (RW, 0x0) 측정할 TILE NO. */
        volatile uint32_t LAYER_IDX:16;  /* DX_M1A_DEBUG.STAMP1.LAYER_IDX [31:16] (RW, 0x0) 측정할 LAYER NO. */
    };
} DX_M1A_DEBUG_STAMP1_t;
typedef volatile union _DX_M1A_DEBUG_STAMP2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CLK_CNT:32;  /* DX_M1A_DEBUG.STAMP2.CLK_CNT [31:0] (R, 0x0) INFERENCE CLOCK COUNT */
    };
} DX_M1A_DEBUG_STAMP2_t;
typedef volatile union _DX_M1A_DEBUG_STAMP3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t OP_CNT:32;  /* DX_M1A_DEBUG.STAMP3.OP_CNT [31:0] (R, 0x0) OPERATION CLOCK COUNT */
    };
} DX_M1A_DEBUG_STAMP3_t;
typedef volatile union _DX_M1A_DEBUG_STAMP4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_RD_WR_CNT:32;  /* DX_M1A_DEBUG.STAMP4.DMA_RD_WR_CNT [31:0] (R, 0x0) DMA READ/WRITE CLOCK COUNT */
    };
} DX_M1A_DEBUG_STAMP4_t;
typedef volatile union _DX_M1A_DEBUG_STAMP5_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_RD_CNT:32;  /* DX_M1A_DEBUG.STAMP5.DMA_RD_CNT [31:0] (R, 0x0) DMA READ COUNT */
    };
} DX_M1A_DEBUG_STAMP5_t;
typedef volatile union _DX_M1A_DEBUG_STAMP6_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_WR_CNT:32;  /* DX_M1A_DEBUG.STAMP6.DMA_WR_CNT [31:0] (R, 0x0) DMA WRITE COUNT */
    };
} DX_M1A_DEBUG_STAMP6_t;
typedef volatile union _DX_M1A_DEBUG_STAMP7_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t HIDE_CNT:32;  /* DX_M1A_DEBUG.STAMP7.HIDE_CNT [31:0] (R, 0x0) DMA와 OPERATION이 동시 수행되는 CLOCK COUNT */
    };
} DX_M1A_DEBUG_STAMP7_t;
typedef volatile union _DX_M1A_DEBUG_STAMP8_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t WAIT_CNT:32;  /* DX_M1A_DEBUG.STAMP8.WAIT_CNT [31:0] (R, 0x0) DMA/OPERATION이 하나만 수행되는 CLOCK COUNT */
    };
} DX_M1A_DEBUG_STAMP8_t;
typedef volatile union _DX_M1A_DEBUG_STAMP9_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RMAP_RD_ONLY_CNT:32;  /* DX_M1A_DEBUG.STAMP9.RMAP_RD_ONLY_CNT [31:0] (R, 0x0) RMAP READ하는 CLOCK COUNT */
    };
} DX_M1A_DEBUG_STAMP9_t;
typedef volatile union _DX_M1A_DEBUG_STAMP10_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP10*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CLK_CNT_SEL:32;  /* DX_M1A_DEBUG.STAMP10.CLK_CNT_SEL [31:0] (R, 0x0) INFERENCE CLOCK COUNT @ SELECT REGION */
    };
} DX_M1A_DEBUG_STAMP10_t;
typedef volatile union _DX_M1A_DEBUG_STAMP11_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP11*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t OP_CNT_SEL:32;  /* DX_M1A_DEBUG.STAMP11.OP_CNT_SEL [31:0] (R, 0x0) OPERATION CLOCK COUNT @ SELECT REGION */
    };
} DX_M1A_DEBUG_STAMP11_t;
typedef volatile union _DX_M1A_DEBUG_STAMP12_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP12*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_RD_WR_CNT_SEL:32;  /* DX_M1A_DEBUG.STAMP12.DMA_RD_WR_CNT_SEL [31:0] (R, 0x0) DMA READ/WRITE CLOCK COUNT @ SELECT REGION */
    };
} DX_M1A_DEBUG_STAMP12_t;
typedef volatile union _DX_M1A_DEBUG_STAMP13_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP13*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_RD_CNT_SEL:32;  /* DX_M1A_DEBUG.STAMP13.DMA_RD_CNT_SEL [31:0] (R, 0x0) DMA READ COUNT @ SELECT REGION */
    };
} DX_M1A_DEBUG_STAMP13_t;
typedef volatile union _DX_M1A_DEBUG_STAMP14_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP14*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_WR_CNT_SEL:32;  /* DX_M1A_DEBUG.STAMP14.DMA_WR_CNT_SEL [31:0] (R, 0x0) DMA WRITE COUNT @ SELECT REGION */
    };
} DX_M1A_DEBUG_STAMP14_t;
typedef volatile union _DX_M1A_DEBUG_STAMP15_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP15*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t HIDE_CNT_SEL:32;  /* DX_M1A_DEBUG.STAMP15.HIDE_CNT_SEL [31:0] (R, 0x0) DMA와 OPERATION이 동시 수행되는 CLOCK COUNT @ SELECT REGION */
    };
} DX_M1A_DEBUG_STAMP15_t;
typedef volatile union _DX_M1A_DEBUG_STAMP16_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP16*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t WAIT_CNT_SEL:32;  /* DX_M1A_DEBUG.STAMP16.WAIT_CNT_SEL [31:0] (R, 0x0) DMA/OPERATION이 하나만 수행되는 CLOCK COUNT @ SELECT REGION */
    };
} DX_M1A_DEBUG_STAMP16_t;
typedef volatile union _DX_M1A_DEBUG_STAMP17_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.STAMP17*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RMAP_RD_ONLY_CNT_SEL:32;  /* DX_M1A_DEBUG.STAMP17.RMAP_RD_ONLY_CNT_SEL [31:0] (R, 0x0) RMAP READ하는 CLOCK COUNT @ SELECT REGION */
    };
} DX_M1A_DEBUG_STAMP17_t;
typedef volatile union _DX_M1A_DEBUG_FSM_PE0_RD_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.FSM_PE0_RD*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FSM_PE0_RD:15;  /* DX_M1A_DEBUG.FSM_PE0_RD.FSM_PE0_RD [14:0] (R, 0x1)  */
        volatile uint32_t RESERVED0:17;  /* DX_M1A_DEBUG.FSM_PE0_RD.RESERVED0 [31:15] (R, 0x0)  */
    };
} DX_M1A_DEBUG_FSM_PE0_RD_t;
typedef volatile union _DX_M1A_DEBUG_FSM_PE0_WR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.FSM_PE0_WR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FSM_PE0_WR:15;  /* DX_M1A_DEBUG.FSM_PE0_WR.FSM_PE0_WR [14:0] (R, 0x1)  */
        volatile uint32_t RESERVED0:17;  /* DX_M1A_DEBUG.FSM_PE0_WR.RESERVED0 [31:15] (R, 0x0)  */
    };
} DX_M1A_DEBUG_FSM_PE0_WR_t;
typedef volatile union _DX_M1A_DEBUG_FSM_PE1_RD_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.FSM_PE1_RD*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FSM_PE1_RD:15;  /* DX_M1A_DEBUG.FSM_PE1_RD.FSM_PE1_RD [14:0] (R, 0x1)  */
        volatile uint32_t RESERVED0:17;  /* DX_M1A_DEBUG.FSM_PE1_RD.RESERVED0 [31:15] (R, 0x0)  */
    };
} DX_M1A_DEBUG_FSM_PE1_RD_t;
typedef volatile union _DX_M1A_DEBUG_FSM_PE1_WR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.FSM_PE1_WR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FSM_PE1_WR:15;  /* DX_M1A_DEBUG.FSM_PE1_WR.FSM_PE1_WR [14:0] (R, 0x1)  */
        volatile uint32_t RESERVED0:17;  /* DX_M1A_DEBUG.FSM_PE1_WR.RESERVED0 [31:15] (R, 0x0)  */
    };
} DX_M1A_DEBUG_FSM_PE1_WR_t;
typedef volatile union _DX_M1A_DEBUG_FSM_PE2_RD_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.FSM_PE2_RD*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FSM_PE2_RD:15;  /* DX_M1A_DEBUG.FSM_PE2_RD.FSM_PE2_RD [14:0] (R, 0x1)  */
        volatile uint32_t RESERVED0:17;  /* DX_M1A_DEBUG.FSM_PE2_RD.RESERVED0 [31:15] (R, 0x0)  */
    };
} DX_M1A_DEBUG_FSM_PE2_RD_t;
typedef volatile union _DX_M1A_DEBUG_FSM_PE2_WR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.FSM_PE2_WR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FSM_PE2_WR:15;  /* DX_M1A_DEBUG.FSM_PE2_WR.FSM_PE2_WR [14:0] (R, 0x1)  */
        volatile uint32_t RESERVED0:17;  /* DX_M1A_DEBUG.FSM_PE2_WR.RESERVED0 [31:15] (R, 0x0)  */
    };
} DX_M1A_DEBUG_FSM_PE2_WR_t;
typedef volatile union _DX_M1A_DEBUG_FSM_PE3_RD_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.FSM_PE3_RD*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FSM_PE3_RD:15;  /* DX_M1A_DEBUG.FSM_PE3_RD.FSM_PE3_RD [14:0] (R, 0x1)  */
        volatile uint32_t RESERVED0:17;  /* DX_M1A_DEBUG.FSM_PE3_RD.RESERVED0 [31:15] (R, 0x0)  */
    };
} DX_M1A_DEBUG_FSM_PE3_RD_t;
typedef volatile union _DX_M1A_DEBUG_FSM_PE3_WR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.FSM_PE3_WR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FSM_PE3_WR:15;  /* DX_M1A_DEBUG.FSM_PE3_WR.FSM_PE3_WR [14:0] (R, 0x1)  */
        volatile uint32_t RESERVED0:17;  /* DX_M1A_DEBUG.FSM_PE3_WR.RESERVED0 [31:15] (R, 0x0)  */
    };
} DX_M1A_DEBUG_FSM_PE3_WR_t;
typedef volatile union _DX_M1A_DEBUG_DMA_CMD_ARB_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.DMA_CMD_ARB*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CMD_ARB_RIDFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RIDFF_EMPTY_RD [0:0] (R, 0x0) NCLK, FIFO_WRAP, U_RIDFIFO */
        volatile uint32_t CMD_ARB_RIDFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RIDFF_FULL_WR [1:1] (R, 0x0) NCLK, FIFO_WRAP, U_RIDFIFO */
        volatile uint32_t CMD_ARB_WIDFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WIDFF_EMPTY_RD [2:2] (R, 0x0) NCLK, FIFO_WRAP, U_WIDFIFO */
        volatile uint32_t CMD_ARB_WIDFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WIDFF_FULL_WR [3:3] (R, 0x0) NCLK, FIFO_WRAP, U_WIDFIFO */
        volatile uint32_t CMD_ARB_WDMAFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WDMAFF_EMPTY_RD [4:4] (R, 0x0) NCLK, FIFOA, U_WDMA_DONE */
        volatile uint32_t CMD_ARB_WDMAFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WDMAFF_FULL_WR [5:5] (R, 0x0) ACLK, FIFOA, U_WDMA_DONE */
        volatile uint32_t CMD_ARB_WMFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WMFF_EMPTY_RD [6:6] (R, 0x0) NCLK, FIFO_WRAP, U_WMFIFO */
        volatile uint32_t CMD_ARB_WMFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WMFF_FULL_WR [7:7] (R, 0x0) NCLK, FIFO_WRAP, U_WMFIFO */
        volatile uint32_t CMD_ARB_RAFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RAFF_EMPTY_RD [8:8] (R, 0x0) ACLK, FIFOA_WRAP, U_RAFIFO */
        volatile uint32_t CMD_ARB_RAFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RAFF_FULL_WR [9:9] (R, 0x0) NCLK, FIFOA_WRAP, U_RAFIFO */
        volatile uint32_t CMD_ARB_RMFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RMFF_EMPTY_RD [10:10] (R, 0x0) NCLK, FIFO_WRAP, U_RMFIFO */
        volatile uint32_t CMD_ARB_RMFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RMFF_FULL_WR [11:11] (R, 0x0) NCLK, FIFO_WRAP, U_RMFIFO */
        volatile uint32_t CMD_ARB_WAFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WAFF_EMPTY_RD [12:12] (R, 0x0) ACLK, FIFOA_WRAP, U_WAFIFO */
        volatile uint32_t CMD_ARB_WAFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WAFF_FULL_WR [13:13] (R, 0x0) NCLK, FIFOA_WRAP, U_WAFIFO */
        volatile uint32_t CMD_ARB_WCMEN_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WCMEN_EMPTY_RD [14:14] (R, 0x0) NCLK, FIFOA, U_WCMEN_DONE */
        volatile uint32_t CMD_ARB_WCMEN_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WCMEN_FULL_WR [15:15] (R, 0x0) CCLK, FIFOA, U_WCMEN_DONE */
        volatile uint32_t CMD_ARB_WCMFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WCMFF_EMPTY_RD [16:16] (R, 0x0) CCLK, FIFOA_WRAP, U_WCMFIFO */
        volatile uint32_t CMD_ARB_WCMFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WCMFF_FULL_WR [17:17] (R, 0x0) NCLK, FIFOA_WRAP, U_WCMFIFO */
        volatile uint32_t CMD_ARB_RCMFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RCMFF_EMPTY_RD [18:18] (R, 0x0) CCLK, FIFOA_WRAP, U_RCMFIFO */
        volatile uint32_t CMD_ARB_RCMFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RCMFF_FULL_WR [19:19] (R, 0x0) NCLK, FIFOA_WRAP, U_RCMFIFO */
        volatile uint32_t RESERVED0:12;  /* DX_M1A_DEBUG.DMA_CMD_ARB.RESERVED0 [31:20] (R, 0x0)  */
    };
} DX_M1A_DEBUG_DMA_CMD_ARB_t;
typedef volatile union _DX_M1A_DEBUG_DMA_AXI_RD_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.DMA_AXI_RD*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t AXI_RD_ACSM:3;  /* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_ACSM [2:0] (R, 0x1) ACLK */
        volatile uint32_t RESERVED2:5;  /* DX_M1A_DEBUG.DMA_AXI_RD.RESERVED2 [7:3] (R, 0x0)  */
        volatile uint32_t AXI_RD_DCSM:2;  /* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_DCSM [9:8] (R, 0x1) ACLK */
        volatile uint32_t RESERVED1:6;  /* DX_M1A_DEBUG.DMA_AXI_RD.RESERVED1 [15:10] (R, 0x0)  */
        volatile uint32_t AXI_RD_DMAR_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_DMAR_EMPTY_RD [16:16] (R, 0x0) NCLK, FIFOA_WRAP, U_DMAR_FIFO */
        volatile uint32_t AXI_RD_DMAR_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_DMAR_FULL_WR [17:17] (R, 0x0) ACLK, FIFOA_WRAP, U_DMAR_FIFO */
        volatile uint32_t AXI_RD_AFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_AFF_EMPTY_RD [18:18] (R, 0x0) ACLK, FIFO, U_AFIFO */
        volatile uint32_t AXI_RD_AFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_AFF_FULL_WR [19:19] (R, 0x0) ACLK, FIFO, U_AFIFO */
        volatile uint32_t RESERVED0:12;  /* DX_M1A_DEBUG.DMA_AXI_RD.RESERVED0 [31:20] (R, 0x0)  */
    };
} DX_M1A_DEBUG_DMA_AXI_RD_t;
typedef volatile union _DX_M1A_DEBUG_DMA_AXI_WR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.DMA_AXI_WR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t AXI_WR_ACSM:3;  /* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_ACSM [2:0] (R, 0x1) ACLK */
        volatile uint32_t RESERVED2:5;  /* DX_M1A_DEBUG.DMA_AXI_WR.RESERVED2 [7:3] (R, 0x0)  */
        volatile uint32_t AXI_WR_DCSM:2;  /* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_DCSM [9:8] (R, 0x1) ACLK */
        volatile uint32_t RESERVED1:6;  /* DX_M1A_DEBUG.DMA_AXI_WR.RESERVED1 [15:10] (R, 0x0)  */
        volatile uint32_t AXI_WR_DMAW_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_DMAW_EMPTY_RD [16:16] (R, 0x0) ACLK, FIFOA_WRAP, U_DMAW_FIFO */
        volatile uint32_t AXI_WR_DMAW_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_DMAW_FULL_WR [17:17] (R, 0x0) NCLK, FIFOA_WRAP, U_DMAW_FIFO */
        volatile uint32_t AXI_WR_BFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_BFF_EMPTY_RD [18:18] (R, 0x0) ACLK, FIFO_WRAP, U_BFIFO */
        volatile uint32_t AXI_WR_BFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_BFF_FULL_WR [19:19] (R, 0x0) ACLK, FIFO_WRAP, U_BFIFO */
        volatile uint32_t AXI_WR_AFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_AFF_EMPTY_RD [20:20] (R, 0x0) ACLK, FIFO, U_AFIFO */
        volatile uint32_t AXI_WR_AFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_AFF_FULL_WR [21:21] (R, 0x0) ACLK, FIFO, U_AFIFO */
        volatile uint32_t RESERVED0:10;  /* DX_M1A_DEBUG.DMA_AXI_WR.RESERVED0 [31:22] (R, 0x0)  */
    };
} DX_M1A_DEBUG_DMA_AXI_WR_t;
typedef volatile union _DX_M1A_DEBUG_DMA_MEM_WR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.DMA_MEM_WR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t MEM_WR_CSM:8;  /* DX_M1A_DEBUG.DMA_MEM_WR.MEM_WR_CSM [7:0] (R, 0x1) NCLK */
        volatile uint32_t RESERVED0:24;  /* DX_M1A_DEBUG.DMA_MEM_WR.RESERVED0 [31:8] (R, 0x0)  */
    };
} DX_M1A_DEBUG_DMA_MEM_WR_t;
typedef volatile union _DX_M1A_DEBUG_DMA_MEM_RD_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.DMA_MEM_RD*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t MEM_RD_CSM:4;  /* DX_M1A_DEBUG.DMA_MEM_RD.MEM_RD_CSM [3:0] (R, 0x1) NCLK */
        volatile uint32_t RESERVED0:28;  /* DX_M1A_DEBUG.DMA_MEM_RD.RESERVED0 [31:4] (R, 0x0)  */
    };
} DX_M1A_DEBUG_DMA_MEM_RD_t;
typedef volatile union _DX_M1A_DEBUG_DMA_MEM_CP_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.DMA_MEM_CP*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t MEM_CP_CSM:3;  /* DX_M1A_DEBUG.DMA_MEM_CP.MEM_CP_CSM [2:0] (R, 0x1) NCLK */
        volatile uint32_t RESERVED0:29;  /* DX_M1A_DEBUG.DMA_MEM_CP.RESERVED0 [31:3] (R, 0x0)  */
    };
} DX_M1A_DEBUG_DMA_MEM_CP_t;
typedef volatile union _DX_M1A_DEBUG_DMA_SFR_WR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.DMA_SFR_WR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SFR_WR_CSM:3;  /* DX_M1A_DEBUG.DMA_SFR_WR.SFR_WR_CSM [2:0] (R, 0x1) NCLK */
        volatile uint32_t RESERVED0:29;  /* DX_M1A_DEBUG.DMA_SFR_WR.RESERVED0 [31:3] (R, 0x0)  */
    };
} DX_M1A_DEBUG_DMA_SFR_WR_t;
typedef volatile union _DX_M1A_DEBUG_DMA_CMEM_RD_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.DMA_CMEM_RD*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CMEM_RD_CSM:2;  /* DX_M1A_DEBUG.DMA_CMEM_RD.CMEM_RD_CSM [1:0] (R, 0x1) CCLK */
        volatile uint32_t RESERVED1:6;  /* DX_M1A_DEBUG.DMA_CMEM_RD.RESERVED1 [7:2] (R, 0x0)  */
        volatile uint32_t CMEM_RD_DMARFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMEM_RD.CMEM_RD_DMARFF_EMPTY_RD [8:8] (R, 0x0) NCLK, FIFOA_WRAP, U_DMAR_FIFO */
        volatile uint32_t CMEM_RD_DMARFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMEM_RD.CMEM_RD_DMARFF_FULL_WR [9:9] (R, 0x0) CCLK, FIFOA_WRAP, U_DMAR_FIFO */
        volatile uint32_t RESERVED0:22;  /* DX_M1A_DEBUG.DMA_CMEM_RD.RESERVED0 [31:10] (R, 0x0)  */
    };
} DX_M1A_DEBUG_DMA_CMEM_RD_t;
typedef volatile union _DX_M1A_DEBUG_DMA_CMEM_WR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.DMA_CMEM_WR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CMEM_WR_CSM:2;  /* DX_M1A_DEBUG.DMA_CMEM_WR.CMEM_WR_CSM [1:0] (R, 0x1) CCLK */
        volatile uint32_t RESERVED1:6;  /* DX_M1A_DEBUG.DMA_CMEM_WR.RESERVED1 [7:2] (R, 0x0)  */
        volatile uint32_t CMEM_WR_DMARFF_EMPTY_RD:1;  /* DX_M1A_DEBUG.DMA_CMEM_WR.CMEM_WR_DMARFF_EMPTY_RD [8:8] (R, 0x0) CCLK, FIFOA_WRAP, U_DMAR_FIFO */
        volatile uint32_t CMEM_WR_DMARFF_FULL_WR:1;  /* DX_M1A_DEBUG.DMA_CMEM_WR.CMEM_WR_DMARFF_FULL_WR [9:9] (R, 0x0) NCLK, FIFOA_WRAP, U_DMAR_FIFO */
        volatile uint32_t RESERVED0:22;  /* DX_M1A_DEBUG.DMA_CMEM_WR.RESERVED0 [31:10] (R, 0x0)  */
    };
} DX_M1A_DEBUG_DMA_CMEM_WR_t;
typedef volatile union _DX_M1A_DEBUG_SFU_FIFO_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.SFU_FIFO*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SFU_FF_EMPTY_RD_LSB_PE1:1;  /* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_EMPTY_RD_LSB_PE1 [0:0] (R, 0x0) NCLK, SYNC_FIFO, GEN_PE_DATA_FIFO_[1]_PE_FIFO_0 */
        volatile uint32_t SFU_FF_EMPTY_RD_MSB_PE1:1;  /* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_EMPTY_RD_MSB_PE1 [1:1] (R, 0x0) NCLK, SYNC_FIFO, GEN_PE_DATA_FIFO_[1]_PE_FIFO_1 */
        volatile uint32_t SFU_FF_EMPTY_RD_LSB_PE0:1;  /* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_EMPTY_RD_LSB_PE0 [2:2] (R, 0x0) NCLK, SYNC_FIFO, GEN_PE_DATA_FIFO_[0]_PE_FIFO_0 */
        volatile uint32_t SFU_FF_EMPTY_RD_MSB_PE0:1;  /* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_EMPTY_RD_MSB_PE0 [3:3] (R, 0x0) NCLK, SYNC_FIFO, GEN_PE_DATA_FIFO_[0]_PE_FIFO_1 */
        volatile uint32_t SFU_FF_FULL_WR_LSB_PE1:1;  /* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_FULL_WR_LSB_PE1 [4:4] (R, 0x0) NCLK, SYNC_FIFO, GEN_PE_DATA_FIFO_[1]_PE_FIFO_0 */
        volatile uint32_t SFU_FF_FULL_WR_MSB_PE1:1;  /* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_FULL_WR_MSB_PE1 [5:5] (R, 0x0) NCLK, SYNC_FIFO, GEN_PE_DATA_FIFO_[1]_PE_FIFO_1 */
        volatile uint32_t SFU_FF_FULL_WR_LSB_PE0:1;  /* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_FULL_WR_LSB_PE0 [6:6] (R, 0x0) NCLK, SYNC_FIFO, GEN_PE_DATA_FIFO_[0]_PE_FIFO_0 */
        volatile uint32_t SFU_FF_FULL_WR_MSB_PE0:1;  /* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_FULL_WR_MSB_PE0 [7:7] (R, 0x0) NCLK, SYNC_FIFO, GEN_PE_DATA_FIFO_[0]_PE_FIFO_1 */
        volatile uint32_t RESERVED0:24;  /* DX_M1A_DEBUG.SFU_FIFO.RESERVED0 [31:8] (R, 0x0)  */
    };
} DX_M1A_DEBUG_SFU_FIFO_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FINISH_CNT:16;  /* DX_M1A_DEBUG.NPU_DBG0.FINISH_CNT [15:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:16;  /* DX_M1A_DEBUG.NPU_DBG0.RESERVED0 [31:16] (R, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG0_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CAPTURE_START_TILE_NUM:16;  /* DX_M1A_DEBUG.NPU_DBG1.CAPTURE_START_TILE_NUM [15:0] (RW, 0xffff) TILE 번호가 유효할 경우 ENABLE */
        volatile uint32_t RESERVED0:16;  /* DX_M1A_DEBUG.NPU_DBG1.RESERVED0 [31:16] (R, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG1_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CAPTURE_PE_VALID_NUM:32;  /* DX_M1A_DEBUG.NPU_DBG2.CAPTURE_PE_VALID_NUM [31:0] (RW, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG2_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CAPTURE_PE_CHANNEL_NUM:6;  /* DX_M1A_DEBUG.NPU_DBG3.CAPTURE_PE_CHANNEL_NUM [5:0] (RW, 0x0) 0~63 */
        volatile uint32_t RESERVED0:26;  /* DX_M1A_DEBUG.NPU_DBG3.RESERVED0 [31:6] (R, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG3_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CAPTURE_PE0_OUT:26;  /* DX_M1A_DEBUG.NPU_DBG4.CAPTURE_PE0_OUT [25:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:6;  /* DX_M1A_DEBUG.NPU_DBG4.RESERVED0 [31:26] (R, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG4_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG5_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CAPTURE_PE1_OUT:26;  /* DX_M1A_DEBUG.NPU_DBG5.CAPTURE_PE1_OUT [25:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:6;  /* DX_M1A_DEBUG.NPU_DBG5.RESERVED0 [31:26] (R, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG5_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG6_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CAPTURE_SFU_VALID_NUM:32;  /* DX_M1A_DEBUG.NPU_DBG6.CAPTURE_SFU_VALID_NUM [31:0] (RW, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG6_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG7_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CAPTURE_SFU_CHANNEL_NUM:5;  /* DX_M1A_DEBUG.NPU_DBG7.CAPTURE_SFU_CHANNEL_NUM [4:0] (RW, 0x0) 0~31 */
        volatile uint32_t RESERVED0:27;  /* DX_M1A_DEBUG.NPU_DBG7.RESERVED0 [31:5] (R, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG7_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG8_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CAPTURE_SFU_DEQ_DATA:24;  /* DX_M1A_DEBUG.NPU_DBG8.CAPTURE_SFU_DEQ_DATA [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_M1A_DEBUG.NPU_DBG8.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG8_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG9_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CAPTURE_SFU_PAF_DATA:24;  /* DX_M1A_DEBUG.NPU_DBG9.CAPTURE_SFU_PAF_DATA [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_M1A_DEBUG.NPU_DBG9.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG9_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG10_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG10*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CAPTURE_SFU_POSTA_DATA:24;  /* DX_M1A_DEBUG.NPU_DBG10.CAPTURE_SFU_POSTA_DATA [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_M1A_DEBUG.NPU_DBG10.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG10_t;
typedef volatile union _DX_M1A_DEBUG_NPU_DBG11_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.NPU_DBG11*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CAPTURE_SFU_POSTB_DATA:24;  /* DX_M1A_DEBUG.NPU_DBG11.CAPTURE_SFU_POSTB_DATA [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_M1A_DEBUG.NPU_DBG11.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_M1A_DEBUG_NPU_DBG11_t;
typedef volatile union _DX_M1A_DEBUG_FSM_RD_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_DEBUG.FSM_RD*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FSM_RD_PULSE:1;  /* DX_M1A_DEBUG.FSM_RD.FSM_RD_PULSE [0:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:31;  /* DX_M1A_DEBUG.FSM_RD.RESERVED0 [31:1] (R, 0x0)  */
    };
} DX_M1A_DEBUG_FSM_RD_t;

/* DX_M1A_DEBUG Module Definition */
typedef volatile struct _dxDX_M1A_DEBUG_t
{
    DX_M1A_DEBUG_STAMP0_t STAMP0;  /* DEBUG 0x0 */
    DX_M1A_DEBUG_STAMP1_t STAMP1;  /* DEBUG 0x4 */
    DX_M1A_DEBUG_STAMP2_t STAMP2;  /* DEBUG 0x8 */
    DX_M1A_DEBUG_STAMP3_t STAMP3;  /* DEBUG 0xc */
    DX_M1A_DEBUG_STAMP4_t STAMP4;  /* DEBUG 0x10 */
    DX_M1A_DEBUG_STAMP5_t STAMP5;  /* DEBUG 0x14 */
    DX_M1A_DEBUG_STAMP6_t STAMP6;  /* DEBUG 0x18 */
    DX_M1A_DEBUG_STAMP7_t STAMP7;  /* DEBUG 0x1c */
    DX_M1A_DEBUG_STAMP8_t STAMP8;  /* DEBUG 0x20 */
    DX_M1A_DEBUG_STAMP9_t STAMP9;  /* DEBUG 0x24 */
    DX_M1A_DEBUG_STAMP10_t STAMP10;  /* DEBUG 0x28 */
    DX_M1A_DEBUG_STAMP11_t STAMP11;  /* DEBUG 0x2c */
    DX_M1A_DEBUG_STAMP12_t STAMP12;  /* DEBUG 0x30 */
    DX_M1A_DEBUG_STAMP13_t STAMP13;  /* DEBUG 0x34 */
    DX_M1A_DEBUG_STAMP14_t STAMP14;  /* DEBUG 0x38 */
    DX_M1A_DEBUG_STAMP15_t STAMP15;  /* DEBUG 0x3c */
    DX_M1A_DEBUG_STAMP16_t STAMP16;  /* DEBUG 0x40 */
    DX_M1A_DEBUG_STAMP17_t STAMP17;  /* DEBUG 0x44 */
    DX_M1A_DEBUG_FSM_PE0_RD_t FSM_PE0_RD;  /* DEBUG 0x48 */
    DX_M1A_DEBUG_FSM_PE0_WR_t FSM_PE0_WR;  /* DEBUG 0x4c */
    DX_M1A_DEBUG_FSM_PE1_RD_t FSM_PE1_RD;  /* DEBUG 0x50 */
    DX_M1A_DEBUG_FSM_PE1_WR_t FSM_PE1_WR;  /* DEBUG 0x54 */
    DX_M1A_DEBUG_FSM_PE2_RD_t FSM_PE2_RD;  /* DEBUG 0x58 */
    DX_M1A_DEBUG_FSM_PE2_WR_t FSM_PE2_WR;  /* DEBUG 0x5c */
    DX_M1A_DEBUG_FSM_PE3_RD_t FSM_PE3_RD;  /* DEBUG 0x60 */
    DX_M1A_DEBUG_FSM_PE3_WR_t FSM_PE3_WR;  /* DEBUG 0x64 */
    DX_M1A_DEBUG_DMA_CMD_ARB_t DMA_CMD_ARB;  /* DEBUG 0x68 */
    DX_M1A_DEBUG_DMA_AXI_RD_t DMA_AXI_RD;  /* DEBUG 0x6c */
    DX_M1A_DEBUG_DMA_AXI_WR_t DMA_AXI_WR;  /* DEBUG 0x70 */
    DX_M1A_DEBUG_DMA_MEM_WR_t DMA_MEM_WR;  /* DEBUG 0x74 */
    DX_M1A_DEBUG_DMA_MEM_RD_t DMA_MEM_RD;  /* DEBUG 0x78 */
    DX_M1A_DEBUG_DMA_MEM_CP_t DMA_MEM_CP;  /* DEBUG 0x7c */
    DX_M1A_DEBUG_DMA_SFR_WR_t DMA_SFR_WR;  /* DEBUG 0x80 */
    DX_M1A_DEBUG_DMA_CMEM_RD_t DMA_CMEM_RD;  /* DEBUG 0x84 */
    DX_M1A_DEBUG_DMA_CMEM_WR_t DMA_CMEM_WR;  /* DEBUG 0x88 */
    DX_M1A_DEBUG_SFU_FIFO_t SFU_FIFO;  /* DEBUG 0x8c */
    DX_M1A_DEBUG_NPU_DBG0_t NPU_DBG0;  /* DEBUG 0x90 */
    DX_M1A_DEBUG_NPU_DBG1_t NPU_DBG1;  /* DEBUG 0x94 */
    DX_M1A_DEBUG_NPU_DBG2_t NPU_DBG2;  /* DEBUG 0x98 */
    DX_M1A_DEBUG_NPU_DBG3_t NPU_DBG3;  /* DEBUG 0x9c */
    DX_M1A_DEBUG_NPU_DBG4_t NPU_DBG4;  /* DEBUG 0xa0 */
    DX_M1A_DEBUG_NPU_DBG5_t NPU_DBG5;  /* DEBUG 0xa4 */
    DX_M1A_DEBUG_NPU_DBG6_t NPU_DBG6;  /* DEBUG 0xa8 */
    DX_M1A_DEBUG_NPU_DBG7_t NPU_DBG7;  /* DEBUG 0xac */
    DX_M1A_DEBUG_NPU_DBG8_t NPU_DBG8;  /* DEBUG 0xb0 */
    DX_M1A_DEBUG_NPU_DBG9_t NPU_DBG9;  /* DEBUG 0xb4 */
    DX_M1A_DEBUG_NPU_DBG10_t NPU_DBG10;  /* DEBUG 0xb8 */
    DX_M1A_DEBUG_NPU_DBG11_t NPU_DBG11;  /* DEBUG 0xbc */
    DX_M1A_DEBUG_FSM_RD_t FSM_RD;  /* DEBUG 0xc0 */
} dxDX_M1A_DEBUG_t;

/* Register access API */
/* DX_M1A_DEBUG.STAMP0 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP0(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP0)
#define WRITE_DX_M1A_DEBUG_STAMP0(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP0, val)
/* DX_M1A_DEBUG.STAMP0.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP0_RESERVED0_LEN        30
#define DX_M1A_DEBUG_STAMP0_RESERVED0_OFFSET     0
#define DX_M1A_DEBUG_STAMP0_RESERVED0_MASK_BITS  0x3fffffff /* [29:0] 0b00111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP0_RESERVED0_MASK       0x3fffffff /* [29:0] 0b00111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP0, DX_M1A_DEBUG_STAMP0_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_STAMP0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP0, DX_M1A_DEBUG_STAMP0_RESERVED0_MASK, DX_M1A_DEBUG_STAMP0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP0, bits, DX_M1A_DEBUG_STAMP0_RESERVED0_MASK, DX_M1A_DEBUG_STAMP0_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.STAMP0.STAMP_OPT (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP0_STAMP_OPT_LEN        1
#define DX_M1A_DEBUG_STAMP0_STAMP_OPT_OFFSET     30
#define DX_M1A_DEBUG_STAMP0_STAMP_OPT_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_STAMP0_STAMP_OPT_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_STAMP0_STAMP_OPT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP0, DX_M1A_DEBUG_STAMP0_STAMP_OPT_MASK)
#define READ_DX_M1A_DEBUG_STAMP0_STAMP_OPT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP0, DX_M1A_DEBUG_STAMP0_STAMP_OPT_MASK, DX_M1A_DEBUG_STAMP0_STAMP_OPT_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP0_STAMP_OPT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP0, bits, DX_M1A_DEBUG_STAMP0_STAMP_OPT_MASK, DX_M1A_DEBUG_STAMP0_STAMP_OPT_OFFSET)

/* DX_M1A_DEBUG.STAMP0.STAMP_EN (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP0_STAMP_EN_LEN        1
#define DX_M1A_DEBUG_STAMP0_STAMP_EN_OFFSET     31
#define DX_M1A_DEBUG_STAMP0_STAMP_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_STAMP0_STAMP_EN_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_STAMP0_STAMP_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP0, DX_M1A_DEBUG_STAMP0_STAMP_EN_MASK)
#define READ_DX_M1A_DEBUG_STAMP0_STAMP_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP0, DX_M1A_DEBUG_STAMP0_STAMP_EN_MASK, DX_M1A_DEBUG_STAMP0_STAMP_EN_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP0_STAMP_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP0, bits, DX_M1A_DEBUG_STAMP0_STAMP_EN_MASK, DX_M1A_DEBUG_STAMP0_STAMP_EN_OFFSET)

/* DX_M1A_DEBUG.STAMP1 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP1(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP1)
#define WRITE_DX_M1A_DEBUG_STAMP1(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP1, val)
/* DX_M1A_DEBUG.STAMP1.TILE_IDX (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP1_TILE_IDX_LEN        16
#define DX_M1A_DEBUG_STAMP1_TILE_IDX_OFFSET     0
#define DX_M1A_DEBUG_STAMP1_TILE_IDX_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_DEBUG_STAMP1_TILE_IDX_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP1_TILE_IDX(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP1, DX_M1A_DEBUG_STAMP1_TILE_IDX_MASK)
#define READ_DX_M1A_DEBUG_STAMP1_TILE_IDX(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP1, DX_M1A_DEBUG_STAMP1_TILE_IDX_MASK, DX_M1A_DEBUG_STAMP1_TILE_IDX_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP1_TILE_IDX(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP1, bits, DX_M1A_DEBUG_STAMP1_TILE_IDX_MASK, DX_M1A_DEBUG_STAMP1_TILE_IDX_OFFSET)

/* DX_M1A_DEBUG.STAMP1.LAYER_IDX (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP1_LAYER_IDX_LEN        16
#define DX_M1A_DEBUG_STAMP1_LAYER_IDX_OFFSET     16
#define DX_M1A_DEBUG_STAMP1_LAYER_IDX_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_DEBUG_STAMP1_LAYER_IDX_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_STAMP1_LAYER_IDX(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP1, DX_M1A_DEBUG_STAMP1_LAYER_IDX_MASK)
#define READ_DX_M1A_DEBUG_STAMP1_LAYER_IDX(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP1, DX_M1A_DEBUG_STAMP1_LAYER_IDX_MASK, DX_M1A_DEBUG_STAMP1_LAYER_IDX_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP1_LAYER_IDX(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP1, bits, DX_M1A_DEBUG_STAMP1_LAYER_IDX_MASK, DX_M1A_DEBUG_STAMP1_LAYER_IDX_OFFSET)

/* DX_M1A_DEBUG.STAMP2 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP2(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP2)
#define WRITE_DX_M1A_DEBUG_STAMP2(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP2, val)
/* DX_M1A_DEBUG.STAMP2.CLK_CNT (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP2_CLK_CNT_LEN        32
#define DX_M1A_DEBUG_STAMP2_CLK_CNT_OFFSET     0
#define DX_M1A_DEBUG_STAMP2_CLK_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP2_CLK_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP2_CLK_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP2, DX_M1A_DEBUG_STAMP2_CLK_CNT_MASK)
#define READ_DX_M1A_DEBUG_STAMP2_CLK_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP2, DX_M1A_DEBUG_STAMP2_CLK_CNT_MASK, DX_M1A_DEBUG_STAMP2_CLK_CNT_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP2_CLK_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP2, bits, DX_M1A_DEBUG_STAMP2_CLK_CNT_MASK, DX_M1A_DEBUG_STAMP2_CLK_CNT_OFFSET)

/* DX_M1A_DEBUG.STAMP3 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP3(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP3)
#define WRITE_DX_M1A_DEBUG_STAMP3(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP3, val)
/* DX_M1A_DEBUG.STAMP3.OP_CNT (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP3_OP_CNT_LEN        32
#define DX_M1A_DEBUG_STAMP3_OP_CNT_OFFSET     0
#define DX_M1A_DEBUG_STAMP3_OP_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP3_OP_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP3_OP_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP3, DX_M1A_DEBUG_STAMP3_OP_CNT_MASK)
#define READ_DX_M1A_DEBUG_STAMP3_OP_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP3, DX_M1A_DEBUG_STAMP3_OP_CNT_MASK, DX_M1A_DEBUG_STAMP3_OP_CNT_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP3_OP_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP3, bits, DX_M1A_DEBUG_STAMP3_OP_CNT_MASK, DX_M1A_DEBUG_STAMP3_OP_CNT_OFFSET)

/* DX_M1A_DEBUG.STAMP4 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP4(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP4)
#define WRITE_DX_M1A_DEBUG_STAMP4(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP4, val)
/* DX_M1A_DEBUG.STAMP4.DMA_RD_WR_CNT (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT_LEN        32
#define DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT_OFFSET     0
#define DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP4, DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT_MASK)
#define READ_DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP4, DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT_MASK, DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP4, bits, DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT_MASK, DX_M1A_DEBUG_STAMP4_DMA_RD_WR_CNT_OFFSET)

/* DX_M1A_DEBUG.STAMP5 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP5(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP5)
#define WRITE_DX_M1A_DEBUG_STAMP5(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP5, val)
/* DX_M1A_DEBUG.STAMP5.DMA_RD_CNT (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP5_DMA_RD_CNT_LEN        32
#define DX_M1A_DEBUG_STAMP5_DMA_RD_CNT_OFFSET     0
#define DX_M1A_DEBUG_STAMP5_DMA_RD_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP5_DMA_RD_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP5_DMA_RD_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP5, DX_M1A_DEBUG_STAMP5_DMA_RD_CNT_MASK)
#define READ_DX_M1A_DEBUG_STAMP5_DMA_RD_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP5, DX_M1A_DEBUG_STAMP5_DMA_RD_CNT_MASK, DX_M1A_DEBUG_STAMP5_DMA_RD_CNT_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP5_DMA_RD_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP5, bits, DX_M1A_DEBUG_STAMP5_DMA_RD_CNT_MASK, DX_M1A_DEBUG_STAMP5_DMA_RD_CNT_OFFSET)

/* DX_M1A_DEBUG.STAMP6 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP6(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP6)
#define WRITE_DX_M1A_DEBUG_STAMP6(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP6, val)
/* DX_M1A_DEBUG.STAMP6.DMA_WR_CNT (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP6_DMA_WR_CNT_LEN        32
#define DX_M1A_DEBUG_STAMP6_DMA_WR_CNT_OFFSET     0
#define DX_M1A_DEBUG_STAMP6_DMA_WR_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP6_DMA_WR_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP6_DMA_WR_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP6, DX_M1A_DEBUG_STAMP6_DMA_WR_CNT_MASK)
#define READ_DX_M1A_DEBUG_STAMP6_DMA_WR_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP6, DX_M1A_DEBUG_STAMP6_DMA_WR_CNT_MASK, DX_M1A_DEBUG_STAMP6_DMA_WR_CNT_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP6_DMA_WR_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP6, bits, DX_M1A_DEBUG_STAMP6_DMA_WR_CNT_MASK, DX_M1A_DEBUG_STAMP6_DMA_WR_CNT_OFFSET)

/* DX_M1A_DEBUG.STAMP7 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP7(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP7)
#define WRITE_DX_M1A_DEBUG_STAMP7(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP7, val)
/* DX_M1A_DEBUG.STAMP7.HIDE_CNT (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP7_HIDE_CNT_LEN        32
#define DX_M1A_DEBUG_STAMP7_HIDE_CNT_OFFSET     0
#define DX_M1A_DEBUG_STAMP7_HIDE_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP7_HIDE_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP7_HIDE_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP7, DX_M1A_DEBUG_STAMP7_HIDE_CNT_MASK)
#define READ_DX_M1A_DEBUG_STAMP7_HIDE_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP7, DX_M1A_DEBUG_STAMP7_HIDE_CNT_MASK, DX_M1A_DEBUG_STAMP7_HIDE_CNT_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP7_HIDE_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP7, bits, DX_M1A_DEBUG_STAMP7_HIDE_CNT_MASK, DX_M1A_DEBUG_STAMP7_HIDE_CNT_OFFSET)

/* DX_M1A_DEBUG.STAMP8 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP8(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP8)
#define WRITE_DX_M1A_DEBUG_STAMP8(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP8, val)
/* DX_M1A_DEBUG.STAMP8.WAIT_CNT (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP8_WAIT_CNT_LEN        32
#define DX_M1A_DEBUG_STAMP8_WAIT_CNT_OFFSET     0
#define DX_M1A_DEBUG_STAMP8_WAIT_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP8_WAIT_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP8_WAIT_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP8, DX_M1A_DEBUG_STAMP8_WAIT_CNT_MASK)
#define READ_DX_M1A_DEBUG_STAMP8_WAIT_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP8, DX_M1A_DEBUG_STAMP8_WAIT_CNT_MASK, DX_M1A_DEBUG_STAMP8_WAIT_CNT_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP8_WAIT_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP8, bits, DX_M1A_DEBUG_STAMP8_WAIT_CNT_MASK, DX_M1A_DEBUG_STAMP8_WAIT_CNT_OFFSET)

/* DX_M1A_DEBUG.STAMP9 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP9(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP9)
#define WRITE_DX_M1A_DEBUG_STAMP9(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP9, val)
/* DX_M1A_DEBUG.STAMP9.RMAP_RD_ONLY_CNT (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT_LEN        32
#define DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT_OFFSET     0
#define DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP9, DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT_MASK)
#define READ_DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP9, DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT_MASK, DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP9, bits, DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT_MASK, DX_M1A_DEBUG_STAMP9_RMAP_RD_ONLY_CNT_OFFSET)

/* DX_M1A_DEBUG.STAMP10 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP10(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP10)
#define WRITE_DX_M1A_DEBUG_STAMP10(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP10, val)
/* DX_M1A_DEBUG.STAMP10.CLK_CNT_SEL (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL_LEN        32
#define DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL_OFFSET     0
#define DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP10, DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL_MASK)
#define READ_DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP10, DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP10, bits, DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP10_CLK_CNT_SEL_OFFSET)

/* DX_M1A_DEBUG.STAMP11 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP11(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP11)
#define WRITE_DX_M1A_DEBUG_STAMP11(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP11, val)
/* DX_M1A_DEBUG.STAMP11.OP_CNT_SEL (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP11_OP_CNT_SEL_LEN        32
#define DX_M1A_DEBUG_STAMP11_OP_CNT_SEL_OFFSET     0
#define DX_M1A_DEBUG_STAMP11_OP_CNT_SEL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP11_OP_CNT_SEL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP11_OP_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP11, DX_M1A_DEBUG_STAMP11_OP_CNT_SEL_MASK)
#define READ_DX_M1A_DEBUG_STAMP11_OP_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP11, DX_M1A_DEBUG_STAMP11_OP_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP11_OP_CNT_SEL_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP11_OP_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP11, bits, DX_M1A_DEBUG_STAMP11_OP_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP11_OP_CNT_SEL_OFFSET)

/* DX_M1A_DEBUG.STAMP12 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP12(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP12)
#define WRITE_DX_M1A_DEBUG_STAMP12(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP12, val)
/* DX_M1A_DEBUG.STAMP12.DMA_RD_WR_CNT_SEL (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL_LEN        32
#define DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL_OFFSET     0
#define DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP12, DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL_MASK)
#define READ_DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP12, DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP12, bits, DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP12_DMA_RD_WR_CNT_SEL_OFFSET)

/* DX_M1A_DEBUG.STAMP13 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP13(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP13)
#define WRITE_DX_M1A_DEBUG_STAMP13(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP13, val)
/* DX_M1A_DEBUG.STAMP13.DMA_RD_CNT_SEL (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL_LEN        32
#define DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL_OFFSET     0
#define DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP13, DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL_MASK)
#define READ_DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP13, DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP13, bits, DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP13_DMA_RD_CNT_SEL_OFFSET)

/* DX_M1A_DEBUG.STAMP14 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP14(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP14)
#define WRITE_DX_M1A_DEBUG_STAMP14(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP14, val)
/* DX_M1A_DEBUG.STAMP14.DMA_WR_CNT_SEL (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL_LEN        32
#define DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL_OFFSET     0
#define DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP14, DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL_MASK)
#define READ_DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP14, DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP14, bits, DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP14_DMA_WR_CNT_SEL_OFFSET)

/* DX_M1A_DEBUG.STAMP15 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP15(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP15)
#define WRITE_DX_M1A_DEBUG_STAMP15(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP15, val)
/* DX_M1A_DEBUG.STAMP15.HIDE_CNT_SEL (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL_LEN        32
#define DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL_OFFSET     0
#define DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP15, DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL_MASK)
#define READ_DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP15, DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP15, bits, DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP15_HIDE_CNT_SEL_OFFSET)

/* DX_M1A_DEBUG.STAMP16 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP16(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP16)
#define WRITE_DX_M1A_DEBUG_STAMP16(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP16, val)
/* DX_M1A_DEBUG.STAMP16.WAIT_CNT_SEL (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL_LEN        32
#define DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL_OFFSET     0
#define DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP16, DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL_MASK)
#define READ_DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP16, DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP16, bits, DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP16_WAIT_CNT_SEL_OFFSET)

/* DX_M1A_DEBUG.STAMP17 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_STAMP17(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_STAMP17)
#define WRITE_DX_M1A_DEBUG_STAMP17(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_STAMP17, val)
/* DX_M1A_DEBUG.STAMP17.RMAP_RD_ONLY_CNT_SEL (Category: DEBUG) */
#define DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL_LEN        32
#define DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL_OFFSET     0
#define DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_STAMP17, DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL_MASK)
#define READ_DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_STAMP17, DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL_OFFSET)
#define WRITE_DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_STAMP17, bits, DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL_MASK, DX_M1A_DEBUG_STAMP17_RMAP_RD_ONLY_CNT_SEL_OFFSET)

/* DX_M1A_DEBUG.FSM_PE0_RD (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_FSM_PE0_RD(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_RD)
#define WRITE_DX_M1A_DEBUG_FSM_PE0_RD(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_RD, val)
/* DX_M1A_DEBUG.FSM_PE0_RD.FSM_PE0_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD_LEN        15
#define DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD_OFFSET     0
#define DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD_MASK       0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_RD, DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_RD, DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD_MASK, DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_RD, bits, DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD_MASK, DX_M1A_DEBUG_FSM_PE0_RD_FSM_PE0_RD_OFFSET)

/* DX_M1A_DEBUG.FSM_PE0_RD.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0_LEN        17
#define DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0_OFFSET     15
#define DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_RD, DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_RD, DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_RD, bits, DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE0_RD_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.FSM_PE0_WR (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_FSM_PE0_WR(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_WR)
#define WRITE_DX_M1A_DEBUG_FSM_PE0_WR(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_WR, val)
/* DX_M1A_DEBUG.FSM_PE0_WR.FSM_PE0_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR_LEN        15
#define DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR_OFFSET     0
#define DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR_MASK       0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_WR, DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_WR, DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR_MASK, DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_WR, bits, DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR_MASK, DX_M1A_DEBUG_FSM_PE0_WR_FSM_PE0_WR_OFFSET)

/* DX_M1A_DEBUG.FSM_PE0_WR.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0_LEN        17
#define DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0_OFFSET     15
#define DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_WR, DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_WR, DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE0_WR, bits, DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE0_WR_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.FSM_PE1_RD (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_FSM_PE1_RD(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_RD)
#define WRITE_DX_M1A_DEBUG_FSM_PE1_RD(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_RD, val)
/* DX_M1A_DEBUG.FSM_PE1_RD.FSM_PE1_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD_LEN        15
#define DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD_OFFSET     0
#define DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD_MASK       0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_RD, DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_RD, DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD_MASK, DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_RD, bits, DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD_MASK, DX_M1A_DEBUG_FSM_PE1_RD_FSM_PE1_RD_OFFSET)

/* DX_M1A_DEBUG.FSM_PE1_RD.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0_LEN        17
#define DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0_OFFSET     15
#define DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_RD, DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_RD, DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_RD, bits, DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE1_RD_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.FSM_PE1_WR (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_FSM_PE1_WR(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_WR)
#define WRITE_DX_M1A_DEBUG_FSM_PE1_WR(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_WR, val)
/* DX_M1A_DEBUG.FSM_PE1_WR.FSM_PE1_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR_LEN        15
#define DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR_OFFSET     0
#define DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR_MASK       0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_WR, DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_WR, DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR_MASK, DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_WR, bits, DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR_MASK, DX_M1A_DEBUG_FSM_PE1_WR_FSM_PE1_WR_OFFSET)

/* DX_M1A_DEBUG.FSM_PE1_WR.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0_LEN        17
#define DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0_OFFSET     15
#define DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_WR, DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_WR, DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE1_WR, bits, DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE1_WR_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.FSM_PE2_RD (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_FSM_PE2_RD(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_RD)
#define WRITE_DX_M1A_DEBUG_FSM_PE2_RD(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_RD, val)
/* DX_M1A_DEBUG.FSM_PE2_RD.FSM_PE2_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD_LEN        15
#define DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD_OFFSET     0
#define DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD_MASK       0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_RD, DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_RD, DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD_MASK, DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_RD, bits, DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD_MASK, DX_M1A_DEBUG_FSM_PE2_RD_FSM_PE2_RD_OFFSET)

/* DX_M1A_DEBUG.FSM_PE2_RD.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0_LEN        17
#define DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0_OFFSET     15
#define DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_RD, DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_RD, DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_RD, bits, DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE2_RD_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.FSM_PE2_WR (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_FSM_PE2_WR(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_WR)
#define WRITE_DX_M1A_DEBUG_FSM_PE2_WR(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_WR, val)
/* DX_M1A_DEBUG.FSM_PE2_WR.FSM_PE2_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR_LEN        15
#define DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR_OFFSET     0
#define DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR_MASK       0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_WR, DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_WR, DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR_MASK, DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_WR, bits, DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR_MASK, DX_M1A_DEBUG_FSM_PE2_WR_FSM_PE2_WR_OFFSET)

/* DX_M1A_DEBUG.FSM_PE2_WR.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0_LEN        17
#define DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0_OFFSET     15
#define DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_WR, DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_WR, DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE2_WR, bits, DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE2_WR_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.FSM_PE3_RD (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_FSM_PE3_RD(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_RD)
#define WRITE_DX_M1A_DEBUG_FSM_PE3_RD(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_RD, val)
/* DX_M1A_DEBUG.FSM_PE3_RD.FSM_PE3_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD_LEN        15
#define DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD_OFFSET     0
#define DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD_MASK       0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_RD, DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_RD, DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD_MASK, DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_RD, bits, DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD_MASK, DX_M1A_DEBUG_FSM_PE3_RD_FSM_PE3_RD_OFFSET)

/* DX_M1A_DEBUG.FSM_PE3_RD.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0_LEN        17
#define DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0_OFFSET     15
#define DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_RD, DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_RD, DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_RD, bits, DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE3_RD_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.FSM_PE3_WR (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_FSM_PE3_WR(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_WR)
#define WRITE_DX_M1A_DEBUG_FSM_PE3_WR(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_WR, val)
/* DX_M1A_DEBUG.FSM_PE3_WR.FSM_PE3_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR_LEN        15
#define DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR_OFFSET     0
#define DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR_MASK       0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_WR, DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_WR, DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR_MASK, DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_WR, bits, DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR_MASK, DX_M1A_DEBUG_FSM_PE3_WR_FSM_PE3_WR_OFFSET)

/* DX_M1A_DEBUG.FSM_PE3_WR.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0_LEN        17
#define DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0_OFFSET     15
#define DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_WR, DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_WR, DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_PE3_WR, bits, DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0_MASK, DX_M1A_DEBUG_FSM_PE3_WR_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, val)
/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RIDFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD_OFFSET     0
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RIDFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR_OFFSET     1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR_MASK       0x00000002 /* [1:1] 0b00000000000000000000000000000010 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RIDFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WIDFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD_OFFSET     2
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD_MASK       0x00000004 /* [2:2] 0b00000000000000000000000000000100 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WIDFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR_OFFSET     3
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR_MASK       0x00000008 /* [3:3] 0b00000000000000000000000000001000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WIDFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WDMAFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD_OFFSET     4
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD_MASK       0x00000010 /* [4:4] 0b00000000000000000000000000010000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WDMAFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR_OFFSET     5
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR_MASK       0x00000020 /* [5:5] 0b00000000000000000000000000100000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WDMAFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WMFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD_OFFSET     6
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD_MASK       0x00000040 /* [6:6] 0b00000000000000000000000001000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WMFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR_OFFSET     7
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR_MASK       0x00000080 /* [7:7] 0b00000000000000000000000010000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WMFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RAFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD_OFFSET     8
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RAFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR_OFFSET     9
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR_MASK       0x00000200 /* [9:9] 0b00000000000000000000001000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RAFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RMFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD_OFFSET     10
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD_MASK       0x00000400 /* [10:10] 0b00000000000000000000010000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RMFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR_OFFSET     11
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR_MASK       0x00000800 /* [11:11] 0b00000000000000000000100000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RMFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WAFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD_OFFSET     12
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD_MASK       0x00001000 /* [12:12] 0b00000000000000000001000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WAFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR_OFFSET     13
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR_MASK       0x00002000 /* [13:13] 0b00000000000000000010000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WAFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WCMEN_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD_OFFSET     14
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD_MASK       0x00004000 /* [14:14] 0b00000000000000000100000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WCMEN_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR_OFFSET     15
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR_MASK       0x00008000 /* [15:15] 0b00000000000000001000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMEN_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WCMFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD_OFFSET     16
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD_MASK       0x00010000 /* [16:16] 0b00000000000000010000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_WCMFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR_OFFSET     17
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR_MASK       0x00020000 /* [17:17] 0b00000000000000100000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_WCMFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RCMFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD_OFFSET     18
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD_MASK       0x00040000 /* [18:18] 0b00000000000001000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.CMD_ARB_RCMFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR_OFFSET     19
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR_MASK       0x00080000 /* [19:19] 0b00000000000010000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_CMD_ARB_RCMFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMD_ARB.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0_LEN        12
#define DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0_OFFSET     20
#define DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0_MASK       0xfff00000 /* [31:20] 0b11111111111100000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMD_ARB, bits, DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0_MASK, DX_M1A_DEBUG_DMA_CMD_ARB_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_RD (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_DMA_AXI_RD(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_RD(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, val)
/* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_ACSM (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM_LEN        3
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM_OFFSET     0
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM_MASK       0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, bits, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_ACSM_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_RD.RESERVED2 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2_LEN        5
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2_OFFSET     3
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2_MASK       0x000000f8 /* [7:3] 0b00000000000000000000000011111000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2_MASK, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, bits, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2_MASK, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED2_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_DCSM (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM_LEN        2
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM_OFFSET     8
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM_MASK       0x00000300 /* [9:8] 0b00000000000000000000001100000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, bits, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DCSM_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_RD.RESERVED1 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1_LEN        6
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1_OFFSET     10
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1_MASK       0x0000fc00 /* [15:10] 0b00000000000000001111110000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1_MASK, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, bits, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1_MASK, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED1_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_DMAR_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD_OFFSET     16
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD_MASK       0x00010000 /* [16:16] 0b00000000000000010000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, bits, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_DMAR_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR_OFFSET     17
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR_MASK       0x00020000 /* [17:17] 0b00000000000000100000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, bits, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_DMAR_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_AFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD_OFFSET     18
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD_MASK       0x00040000 /* [18:18] 0b00000000000001000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, bits, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_RD.AXI_RD_AFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR_OFFSET     19
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR_MASK       0x00080000 /* [19:19] 0b00000000000010000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, bits, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_AXI_RD_AXI_RD_AFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_RD.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0_LEN        12
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0_OFFSET     20
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0_MASK       0xfff00000 /* [31:20] 0b11111111111100000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0_MASK, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_RD, bits, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0_MASK, DX_M1A_DEBUG_DMA_AXI_RD_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_WR (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_DMA_AXI_WR(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, val)
/* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_ACSM (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM_LEN        3
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM_OFFSET     0
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM_MASK       0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, bits, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_ACSM_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_WR.RESERVED2 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2_LEN        5
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2_OFFSET     3
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2_MASK       0x000000f8 /* [7:3] 0b00000000000000000000000011111000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2_MASK, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, bits, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2_MASK, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED2_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_DCSM (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM_LEN        2
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM_OFFSET     8
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM_MASK       0x00000300 /* [9:8] 0b00000000000000000000001100000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, bits, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DCSM_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_WR.RESERVED1 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1_LEN        6
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1_OFFSET     10
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1_MASK       0x0000fc00 /* [15:10] 0b00000000000000001111110000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1_MASK, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, bits, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1_MASK, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED1_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_DMAW_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD_OFFSET     16
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD_MASK       0x00010000 /* [16:16] 0b00000000000000010000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, bits, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_DMAW_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR_OFFSET     17
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR_MASK       0x00020000 /* [17:17] 0b00000000000000100000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, bits, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_DMAW_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_BFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD_OFFSET     18
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD_MASK       0x00040000 /* [18:18] 0b00000000000001000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, bits, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_BFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR_OFFSET     19
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR_MASK       0x00080000 /* [19:19] 0b00000000000010000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, bits, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_BFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_AFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD_OFFSET     20
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD_MASK       0x00100000 /* [20:20] 0b00000000000100000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, bits, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_WR.AXI_WR_AFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR_OFFSET     21
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR_MASK       0x00200000 /* [21:21] 0b00000000001000000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, bits, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_AXI_WR_AXI_WR_AFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_AXI_WR.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0_LEN        10
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0_OFFSET     22
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0_MASK       0xffc00000 /* [31:22] 0b11111111110000000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0_MASK, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_AXI_WR, bits, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0_MASK, DX_M1A_DEBUG_DMA_AXI_WR_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.DMA_MEM_WR (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_DMA_MEM_WR(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_WR)
#define WRITE_DX_M1A_DEBUG_DMA_MEM_WR(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_WR, val)
/* DX_M1A_DEBUG.DMA_MEM_WR.MEM_WR_CSM (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM_LEN        8
#define DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM_OFFSET     0
#define DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_WR, DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM_MASK)
#define READ_DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_WR, DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM_MASK, DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_WR, bits, DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM_MASK, DX_M1A_DEBUG_DMA_MEM_WR_MEM_WR_CSM_OFFSET)

/* DX_M1A_DEBUG.DMA_MEM_WR.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0_LEN        24
#define DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0_OFFSET     8
#define DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0_MASK       0xffffff00 /* [31:8] 0b11111111111111111111111100000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_WR, DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_WR, DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0_MASK, DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_WR, bits, DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0_MASK, DX_M1A_DEBUG_DMA_MEM_WR_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.DMA_MEM_RD (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_DMA_MEM_RD(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_RD)
#define WRITE_DX_M1A_DEBUG_DMA_MEM_RD(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_RD, val)
/* DX_M1A_DEBUG.DMA_MEM_RD.MEM_RD_CSM (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM_LEN        4
#define DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM_OFFSET     0
#define DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM_MASK       0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define READ_MASK_DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_RD, DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM_MASK)
#define READ_DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_RD, DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM_MASK, DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_RD, bits, DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM_MASK, DX_M1A_DEBUG_DMA_MEM_RD_MEM_RD_CSM_OFFSET)

/* DX_M1A_DEBUG.DMA_MEM_RD.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0_LEN        28
#define DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0_OFFSET     4
#define DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0_MASK_BITS  0x0fffffff /* [27:0] 0b00001111111111111111111111111111 */
#define DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0_MASK       0xfffffff0 /* [31:4] 0b11111111111111111111111111110000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_RD, DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_RD, DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0_MASK, DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_RD, bits, DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0_MASK, DX_M1A_DEBUG_DMA_MEM_RD_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.DMA_MEM_CP (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_DMA_MEM_CP(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_CP)
#define WRITE_DX_M1A_DEBUG_DMA_MEM_CP(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_CP, val)
/* DX_M1A_DEBUG.DMA_MEM_CP.MEM_CP_CSM (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM_LEN        3
#define DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM_OFFSET     0
#define DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM_MASK       0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define READ_MASK_DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_CP, DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM_MASK)
#define READ_DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_CP, DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM_MASK, DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_CP, bits, DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM_MASK, DX_M1A_DEBUG_DMA_MEM_CP_MEM_CP_CSM_OFFSET)

/* DX_M1A_DEBUG.DMA_MEM_CP.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0_LEN        29
#define DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0_OFFSET     3
#define DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0_MASK_BITS  0x1fffffff /* [28:0] 0b00011111111111111111111111111111 */
#define DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0_MASK       0xfffffff8 /* [31:3] 0b11111111111111111111111111111000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_CP, DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_CP, DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0_MASK, DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_MEM_CP, bits, DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0_MASK, DX_M1A_DEBUG_DMA_MEM_CP_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.DMA_SFR_WR (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_DMA_SFR_WR(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_DMA_SFR_WR)
#define WRITE_DX_M1A_DEBUG_DMA_SFR_WR(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_DMA_SFR_WR, val)
/* DX_M1A_DEBUG.DMA_SFR_WR.SFR_WR_CSM (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM_LEN        3
#define DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM_OFFSET     0
#define DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM_MASK       0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define READ_MASK_DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_SFR_WR, DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM_MASK)
#define READ_DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_SFR_WR, DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM_MASK, DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_SFR_WR, bits, DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM_MASK, DX_M1A_DEBUG_DMA_SFR_WR_SFR_WR_CSM_OFFSET)

/* DX_M1A_DEBUG.DMA_SFR_WR.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0_LEN        29
#define DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0_OFFSET     3
#define DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0_MASK_BITS  0x1fffffff /* [28:0] 0b00011111111111111111111111111111 */
#define DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0_MASK       0xfffffff8 /* [31:3] 0b11111111111111111111111111111000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_SFR_WR, DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_SFR_WR, DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0_MASK, DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_SFR_WR, bits, DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0_MASK, DX_M1A_DEBUG_DMA_SFR_WR_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.DMA_CMEM_RD (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_DMA_CMEM_RD(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_RD(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, val)
/* DX_M1A_DEBUG.DMA_CMEM_RD.CMEM_RD_CSM (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM_LEN        2
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM_OFFSET     0
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM_MASK       0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM_MASK, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, bits, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM_MASK, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_CSM_OFFSET)

/* DX_M1A_DEBUG.DMA_CMEM_RD.RESERVED1 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1_LEN        6
#define DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1_OFFSET     2
#define DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1_MASK       0x000000fc /* [7:2] 0b00000000000000000000000011111100 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1_MASK, DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, bits, DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1_MASK, DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED1_OFFSET)

/* DX_M1A_DEBUG.DMA_CMEM_RD.CMEM_RD_DMARFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD_OFFSET     8
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, bits, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMEM_RD.CMEM_RD_DMARFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR_OFFSET     9
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR_MASK       0x00000200 /* [9:9] 0b00000000000000000000001000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, bits, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMEM_RD_CMEM_RD_DMARFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMEM_RD.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0_LEN        22
#define DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0_OFFSET     10
#define DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0_MASK_BITS  0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0_MASK       0xfffffc00 /* [31:10] 0b11111111111111111111110000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0_MASK, DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_RD, bits, DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0_MASK, DX_M1A_DEBUG_DMA_CMEM_RD_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.DMA_CMEM_WR (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_DMA_CMEM_WR(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_WR(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, val)
/* DX_M1A_DEBUG.DMA_CMEM_WR.CMEM_WR_CSM (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM_LEN        2
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM_OFFSET     0
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM_MASK       0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM_MASK, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, bits, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM_MASK, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_CSM_OFFSET)

/* DX_M1A_DEBUG.DMA_CMEM_WR.RESERVED1 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1_LEN        6
#define DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1_OFFSET     2
#define DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1_MASK       0x000000fc /* [7:2] 0b00000000000000000000000011111100 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1_MASK, DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, bits, DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1_MASK, DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED1_OFFSET)

/* DX_M1A_DEBUG.DMA_CMEM_WR.CMEM_WR_DMARFF_EMPTY_RD (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD_LEN        1
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD_OFFSET     8
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, bits, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD_MASK, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_EMPTY_RD_OFFSET)

/* DX_M1A_DEBUG.DMA_CMEM_WR.CMEM_WR_DMARFF_FULL_WR (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR_LEN        1
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR_OFFSET     9
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR_MASK       0x00000200 /* [9:9] 0b00000000000000000000001000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, bits, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR_MASK, DX_M1A_DEBUG_DMA_CMEM_WR_CMEM_WR_DMARFF_FULL_WR_OFFSET)

/* DX_M1A_DEBUG.DMA_CMEM_WR.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0_LEN        22
#define DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0_OFFSET     10
#define DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0_MASK_BITS  0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0_MASK       0xfffffc00 /* [31:10] 0b11111111111111111111110000000000 */
#define READ_MASK_DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0_MASK, DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_DMA_CMEM_WR, bits, DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0_MASK, DX_M1A_DEBUG_DMA_CMEM_WR_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.SFU_FIFO (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_SFU_FIFO(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO)
#define WRITE_DX_M1A_DEBUG_SFU_FIFO(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, val)
/* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_EMPTY_RD_LSB_PE1 (Category: DEBUG) */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1_LEN        1
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1_OFFSET     0
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1_MASK)
#define READ_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1_OFFSET)
#define WRITE_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, bits, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE1_OFFSET)

/* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_EMPTY_RD_MSB_PE1 (Category: DEBUG) */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1_LEN        1
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1_OFFSET     1
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1_MASK       0x00000002 /* [1:1] 0b00000000000000000000000000000010 */
#define READ_MASK_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1_MASK)
#define READ_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1_OFFSET)
#define WRITE_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, bits, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE1_OFFSET)

/* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_EMPTY_RD_LSB_PE0 (Category: DEBUG) */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0_LEN        1
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0_OFFSET     2
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0_MASK       0x00000004 /* [2:2] 0b00000000000000000000000000000100 */
#define READ_MASK_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0_MASK)
#define READ_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0_OFFSET)
#define WRITE_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, bits, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_LSB_PE0_OFFSET)

/* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_EMPTY_RD_MSB_PE0 (Category: DEBUG) */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0_LEN        1
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0_OFFSET     3
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0_MASK       0x00000008 /* [3:3] 0b00000000000000000000000000001000 */
#define READ_MASK_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0_MASK)
#define READ_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0_OFFSET)
#define WRITE_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, bits, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_EMPTY_RD_MSB_PE0_OFFSET)

/* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_FULL_WR_LSB_PE1 (Category: DEBUG) */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1_LEN        1
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1_OFFSET     4
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1_MASK       0x00000010 /* [4:4] 0b00000000000000000000000000010000 */
#define READ_MASK_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1_MASK)
#define READ_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1_OFFSET)
#define WRITE_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, bits, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE1_OFFSET)

/* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_FULL_WR_MSB_PE1 (Category: DEBUG) */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1_LEN        1
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1_OFFSET     5
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1_MASK       0x00000020 /* [5:5] 0b00000000000000000000000000100000 */
#define READ_MASK_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1_MASK)
#define READ_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1_OFFSET)
#define WRITE_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, bits, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE1_OFFSET)

/* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_FULL_WR_LSB_PE0 (Category: DEBUG) */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0_LEN        1
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0_OFFSET     6
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0_MASK       0x00000040 /* [6:6] 0b00000000000000000000000001000000 */
#define READ_MASK_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0_MASK)
#define READ_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0_OFFSET)
#define WRITE_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, bits, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_LSB_PE0_OFFSET)

/* DX_M1A_DEBUG.SFU_FIFO.SFU_FF_FULL_WR_MSB_PE0 (Category: DEBUG) */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0_LEN        1
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0_OFFSET     7
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0_MASK       0x00000080 /* [7:7] 0b00000000000000000000000010000000 */
#define READ_MASK_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0_MASK)
#define READ_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0_OFFSET)
#define WRITE_DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, bits, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0_MASK, DX_M1A_DEBUG_SFU_FIFO_SFU_FF_FULL_WR_MSB_PE0_OFFSET)

/* DX_M1A_DEBUG.SFU_FIFO.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_SFU_FIFO_RESERVED0_LEN        24
#define DX_M1A_DEBUG_SFU_FIFO_RESERVED0_OFFSET     8
#define DX_M1A_DEBUG_SFU_FIFO_RESERVED0_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_M1A_DEBUG_SFU_FIFO_RESERVED0_MASK       0xffffff00 /* [31:8] 0b11111111111111111111111100000000 */
#define READ_MASK_DX_M1A_DEBUG_SFU_FIFO_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_SFU_FIFO_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, DX_M1A_DEBUG_SFU_FIFO_RESERVED0_MASK, DX_M1A_DEBUG_SFU_FIFO_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_SFU_FIFO_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_SFU_FIFO, bits, DX_M1A_DEBUG_SFU_FIFO_RESERVED0_MASK, DX_M1A_DEBUG_SFU_FIFO_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG0 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG0(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG0)
#define WRITE_DX_M1A_DEBUG_NPU_DBG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG0, val)
/* DX_M1A_DEBUG.NPU_DBG0.FINISH_CNT (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT_LEN        16
#define DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG0, DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG0, DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT_MASK, DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG0, bits, DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT_MASK, DX_M1A_DEBUG_NPU_DBG0_FINISH_CNT_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG0.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG0_RESERVED0_LEN        16
#define DX_M1A_DEBUG_NPU_DBG0_RESERVED0_OFFSET     16
#define DX_M1A_DEBUG_NPU_DBG0_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG0_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG0, DX_M1A_DEBUG_NPU_DBG0_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG0, DX_M1A_DEBUG_NPU_DBG0_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG0, bits, DX_M1A_DEBUG_NPU_DBG0_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG0_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG1 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG1(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG1)
#define WRITE_DX_M1A_DEBUG_NPU_DBG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG1, val)
/* DX_M1A_DEBUG.NPU_DBG1.CAPTURE_START_TILE_NUM (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM_LEN        16
#define DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG1, DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG1, DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM_MASK, DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG1, bits, DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM_MASK, DX_M1A_DEBUG_NPU_DBG1_CAPTURE_START_TILE_NUM_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG1.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG1_RESERVED0_LEN        16
#define DX_M1A_DEBUG_NPU_DBG1_RESERVED0_OFFSET     16
#define DX_M1A_DEBUG_NPU_DBG1_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG1_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG1, DX_M1A_DEBUG_NPU_DBG1_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG1, DX_M1A_DEBUG_NPU_DBG1_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG1_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG1, bits, DX_M1A_DEBUG_NPU_DBG1_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG1_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG2 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG2(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG2)
#define WRITE_DX_M1A_DEBUG_NPU_DBG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG2, val)
/* DX_M1A_DEBUG.NPU_DBG2.CAPTURE_PE_VALID_NUM (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM_LEN        32
#define DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG2, DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG2, DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM_MASK, DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG2, bits, DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM_MASK, DX_M1A_DEBUG_NPU_DBG2_CAPTURE_PE_VALID_NUM_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG3 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG3(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG3)
#define WRITE_DX_M1A_DEBUG_NPU_DBG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG3, val)
/* DX_M1A_DEBUG.NPU_DBG3.CAPTURE_PE_CHANNEL_NUM (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM_LEN        6
#define DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG3, DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG3, DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM_MASK, DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG3, bits, DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM_MASK, DX_M1A_DEBUG_NPU_DBG3_CAPTURE_PE_CHANNEL_NUM_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG3.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG3_RESERVED0_LEN        26
#define DX_M1A_DEBUG_NPU_DBG3_RESERVED0_OFFSET     6
#define DX_M1A_DEBUG_NPU_DBG3_RESERVED0_MASK_BITS  0x03ffffff /* [25:0] 0b00000011111111111111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG3_RESERVED0_MASK       0xffffffc0 /* [31:6] 0b11111111111111111111111111000000 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG3_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG3, DX_M1A_DEBUG_NPU_DBG3_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG3_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG3, DX_M1A_DEBUG_NPU_DBG3_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG3_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG3_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG3, bits, DX_M1A_DEBUG_NPU_DBG3_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG3_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG4 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG4(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG4)
#define WRITE_DX_M1A_DEBUG_NPU_DBG4(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG4, val)
/* DX_M1A_DEBUG.NPU_DBG4.CAPTURE_PE0_OUT (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT_LEN        26
#define DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT_MASK_BITS  0x03ffffff /* [25:0] 0b00000011111111111111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT_MASK       0x03ffffff /* [25:0] 0b00000011111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG4, DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG4, DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT_MASK, DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG4, bits, DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT_MASK, DX_M1A_DEBUG_NPU_DBG4_CAPTURE_PE0_OUT_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG4.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG4_RESERVED0_LEN        6
#define DX_M1A_DEBUG_NPU_DBG4_RESERVED0_OFFSET     26
#define DX_M1A_DEBUG_NPU_DBG4_RESERVED0_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_DEBUG_NPU_DBG4_RESERVED0_MASK       0xfc000000 /* [31:26] 0b11111100000000000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG4_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG4, DX_M1A_DEBUG_NPU_DBG4_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG4_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG4, DX_M1A_DEBUG_NPU_DBG4_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG4_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG4_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG4, bits, DX_M1A_DEBUG_NPU_DBG4_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG4_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG5 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG5(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG5)
#define WRITE_DX_M1A_DEBUG_NPU_DBG5(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG5, val)
/* DX_M1A_DEBUG.NPU_DBG5.CAPTURE_PE1_OUT (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT_LEN        26
#define DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT_MASK_BITS  0x03ffffff /* [25:0] 0b00000011111111111111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT_MASK       0x03ffffff /* [25:0] 0b00000011111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG5, DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG5, DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT_MASK, DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG5, bits, DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT_MASK, DX_M1A_DEBUG_NPU_DBG5_CAPTURE_PE1_OUT_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG5.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG5_RESERVED0_LEN        6
#define DX_M1A_DEBUG_NPU_DBG5_RESERVED0_OFFSET     26
#define DX_M1A_DEBUG_NPU_DBG5_RESERVED0_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_DEBUG_NPU_DBG5_RESERVED0_MASK       0xfc000000 /* [31:26] 0b11111100000000000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG5_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG5, DX_M1A_DEBUG_NPU_DBG5_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG5_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG5, DX_M1A_DEBUG_NPU_DBG5_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG5_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG5_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG5, bits, DX_M1A_DEBUG_NPU_DBG5_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG5_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG6 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG6(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG6)
#define WRITE_DX_M1A_DEBUG_NPU_DBG6(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG6, val)
/* DX_M1A_DEBUG.NPU_DBG6.CAPTURE_SFU_VALID_NUM (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM_LEN        32
#define DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG6, DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG6, DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM_MASK, DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG6, bits, DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM_MASK, DX_M1A_DEBUG_NPU_DBG6_CAPTURE_SFU_VALID_NUM_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG7 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG7(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG7)
#define WRITE_DX_M1A_DEBUG_NPU_DBG7(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG7, val)
/* DX_M1A_DEBUG.NPU_DBG7.CAPTURE_SFU_CHANNEL_NUM (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM_LEN        5
#define DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM_MASK       0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG7, DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG7, DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM_MASK, DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG7, bits, DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM_MASK, DX_M1A_DEBUG_NPU_DBG7_CAPTURE_SFU_CHANNEL_NUM_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG7.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG7_RESERVED0_LEN        27
#define DX_M1A_DEBUG_NPU_DBG7_RESERVED0_OFFSET     5
#define DX_M1A_DEBUG_NPU_DBG7_RESERVED0_MASK_BITS  0x07ffffff /* [26:0] 0b00000111111111111111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG7_RESERVED0_MASK       0xffffffe0 /* [31:5] 0b11111111111111111111111111100000 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG7_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG7, DX_M1A_DEBUG_NPU_DBG7_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG7_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG7, DX_M1A_DEBUG_NPU_DBG7_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG7_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG7_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG7, bits, DX_M1A_DEBUG_NPU_DBG7_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG7_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG8 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG8(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG8)
#define WRITE_DX_M1A_DEBUG_NPU_DBG8(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG8, val)
/* DX_M1A_DEBUG.NPU_DBG8.CAPTURE_SFU_DEQ_DATA (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA_LEN        24
#define DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG8, DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG8, DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA_MASK, DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG8, bits, DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA_MASK, DX_M1A_DEBUG_NPU_DBG8_CAPTURE_SFU_DEQ_DATA_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG8.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG8_RESERVED0_LEN        8
#define DX_M1A_DEBUG_NPU_DBG8_RESERVED0_OFFSET     24
#define DX_M1A_DEBUG_NPU_DBG8_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_DEBUG_NPU_DBG8_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG8_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG8, DX_M1A_DEBUG_NPU_DBG8_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG8_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG8, DX_M1A_DEBUG_NPU_DBG8_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG8_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG8_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG8, bits, DX_M1A_DEBUG_NPU_DBG8_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG8_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG9 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG9(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG9)
#define WRITE_DX_M1A_DEBUG_NPU_DBG9(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG9, val)
/* DX_M1A_DEBUG.NPU_DBG9.CAPTURE_SFU_PAF_DATA (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA_LEN        24
#define DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG9, DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG9, DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA_MASK, DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG9, bits, DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA_MASK, DX_M1A_DEBUG_NPU_DBG9_CAPTURE_SFU_PAF_DATA_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG9.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG9_RESERVED0_LEN        8
#define DX_M1A_DEBUG_NPU_DBG9_RESERVED0_OFFSET     24
#define DX_M1A_DEBUG_NPU_DBG9_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_DEBUG_NPU_DBG9_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG9_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG9, DX_M1A_DEBUG_NPU_DBG9_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG9_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG9, DX_M1A_DEBUG_NPU_DBG9_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG9_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG9_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG9, bits, DX_M1A_DEBUG_NPU_DBG9_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG9_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG10 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG10(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG10)
#define WRITE_DX_M1A_DEBUG_NPU_DBG10(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG10, val)
/* DX_M1A_DEBUG.NPU_DBG10.CAPTURE_SFU_POSTA_DATA (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA_LEN        24
#define DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG10, DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG10, DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA_MASK, DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG10, bits, DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA_MASK, DX_M1A_DEBUG_NPU_DBG10_CAPTURE_SFU_POSTA_DATA_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG10.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG10_RESERVED0_LEN        8
#define DX_M1A_DEBUG_NPU_DBG10_RESERVED0_OFFSET     24
#define DX_M1A_DEBUG_NPU_DBG10_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_DEBUG_NPU_DBG10_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG10_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG10, DX_M1A_DEBUG_NPU_DBG10_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG10_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG10, DX_M1A_DEBUG_NPU_DBG10_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG10_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG10_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG10, bits, DX_M1A_DEBUG_NPU_DBG10_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG10_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG11 (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_NPU_DBG11(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_NPU_DBG11)
#define WRITE_DX_M1A_DEBUG_NPU_DBG11(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_NPU_DBG11, val)
/* DX_M1A_DEBUG.NPU_DBG11.CAPTURE_SFU_POSTB_DATA (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA_LEN        24
#define DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA_OFFSET     0
#define DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG11, DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG11, DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA_MASK, DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG11, bits, DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA_MASK, DX_M1A_DEBUG_NPU_DBG11_CAPTURE_SFU_POSTB_DATA_OFFSET)

/* DX_M1A_DEBUG.NPU_DBG11.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_NPU_DBG11_RESERVED0_LEN        8
#define DX_M1A_DEBUG_NPU_DBG11_RESERVED0_OFFSET     24
#define DX_M1A_DEBUG_NPU_DBG11_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_DEBUG_NPU_DBG11_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_M1A_DEBUG_NPU_DBG11_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_NPU_DBG11, DX_M1A_DEBUG_NPU_DBG11_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_NPU_DBG11_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG11, DX_M1A_DEBUG_NPU_DBG11_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG11_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_NPU_DBG11_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_NPU_DBG11, bits, DX_M1A_DEBUG_NPU_DBG11_RESERVED0_MASK, DX_M1A_DEBUG_NPU_DBG11_RESERVED0_OFFSET)

/* DX_M1A_DEBUG.FSM_RD (Category: DEBUG) */
#define READ_DX_M1A_DEBUG_FSM_RD(base) dxrt::readl((void*)base, REG_DX_M1A_DEBUG_FSM_RD)
#define WRITE_DX_M1A_DEBUG_FSM_RD(base, val) dxrt::writel((void*)base, REG_DX_M1A_DEBUG_FSM_RD, val)
/* DX_M1A_DEBUG.FSM_RD.FSM_RD_PULSE (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE_LEN        1
#define DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE_OFFSET     0
#define DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_RD, DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE_MASK)
#define READ_DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_RD, DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE_MASK, DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_RD, bits, DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE_MASK, DX_M1A_DEBUG_FSM_RD_FSM_RD_PULSE_OFFSET)

/* DX_M1A_DEBUG.FSM_RD.RESERVED0 (Category: DEBUG) */
#define DX_M1A_DEBUG_FSM_RD_RESERVED0_LEN        31
#define DX_M1A_DEBUG_FSM_RD_RESERVED0_OFFSET     1
#define DX_M1A_DEBUG_FSM_RD_RESERVED0_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1A_DEBUG_FSM_RD_RESERVED0_MASK       0xfffffffe /* [31:1] 0b11111111111111111111111111111110 */
#define READ_MASK_DX_M1A_DEBUG_FSM_RD_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_DEBUG_FSM_RD, DX_M1A_DEBUG_FSM_RD_RESERVED0_MASK)
#define READ_DX_M1A_DEBUG_FSM_RD_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_DEBUG_FSM_RD, DX_M1A_DEBUG_FSM_RD_RESERVED0_MASK, DX_M1A_DEBUG_FSM_RD_RESERVED0_OFFSET)
#define WRITE_DX_M1A_DEBUG_FSM_RD_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_DEBUG_FSM_RD, bits, DX_M1A_DEBUG_FSM_RD_RESERVED0_MASK, DX_M1A_DEBUG_FSM_RD_RESERVED0_OFFSET)


int Verify_Dx_m1a_debug_Registers(dxDX_M1A_DEBUG_t* base);

} /* namespace dxrt */
