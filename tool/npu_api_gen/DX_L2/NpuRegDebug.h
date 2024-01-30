/* Automatically generated from DX_L2.xlsx
 * by Deepx Neural Network Generator v1.0
 */
#pragma once

#include <stdint.h>
#include "dxrt/npuif/npuif.h"

namespace dxrt{
extern bool logNpuRegAccess;

/* Address */
#define REG_DX_L2_DEBUG_OFFSET 0x100
#define REG_DX_L2_DEBUG_STAMP0    0x00000000 /* 0 */
#define REG_DX_L2_DEBUG_STAMP1    0x00000004 /* 4 */
#define REG_DX_L2_DEBUG_STAMP2    0x00000008 /* 8 */
#define REG_DX_L2_DEBUG_STAMP3    0x0000000c /* 12 */
#define REG_DX_L2_DEBUG_STAMP4    0x00000010 /* 16 */
#define REG_DX_L2_DEBUG_STAMP5    0x00000014 /* 20 */
#define REG_DX_L2_DEBUG_STAMP6    0x00000018 /* 24 */
#define REG_DX_L2_DEBUG_STAMP7    0x0000001c /* 28 */
#define REG_DX_L2_DEBUG_STAMP8    0x00000020 /* 32 */
#define REG_DX_L2_DEBUG_STAMP9    0x00000024 /* 36 */
#define REG_DX_L2_DEBUG_STAMP10    0x00000028 /* 40 */
#define REG_DX_L2_DEBUG_STAMP11    0x0000002c /* 44 */
#define REG_DX_L2_DEBUG_STAMP12    0x00000030 /* 48 */
#define REG_DX_L2_DEBUG_STAMP13    0x00000034 /* 52 */
#define REG_DX_L2_DEBUG_STAMP14    0x00000038 /* 56 */
#define REG_DX_L2_DEBUG_STAMP15    0x0000003c /* 60 */
#define REG_DX_L2_DEBUG_STAMP16    0x00000040 /* 64 */
#define REG_DX_L2_DEBUG_STAMP17    0x00000044 /* 68 */
#define REG_DX_L2_DEBUG_STAMP18    0x00000048 /* 72 */
#define REG_DX_L2_DEBUG_STAMP19    0x0000004c /* 76 */
#define REG_DX_L2_DEBUG_STAMP20    0x00000050 /* 80 */
#define REG_DX_L2_DEBUG_STAMP21    0x00000054 /* 84 */
#define REG_DX_L2_DEBUG_STAMP22    0x00000058 /* 88 */
#define REG_DX_L2_DEBUG_STAMP23    0x0000005c /* 92 */
#define REG_DX_L2_DEBUG_STAMP24    0x00000060 /* 96 */
#define REG_DX_L2_DEBUG_STAMP25    0x00000064 /* 100 */
#define REG_DX_L2_DEBUG_STAMP26    0x00000068 /* 104 */
#define REG_DX_L2_DEBUG_STAMP27    0x0000006c /* 108 */
#define REG_DX_L2_DEBUG_STAMP28    0x00000070 /* 112 */
#define REG_DX_L2_DEBUG_CKSUM0    0x00000074 /* 116 */
#define REG_DX_L2_DEBUG_CKSUM1    0x00000078 /* 120 */
#define REG_DX_L2_DEBUG_CKSUM2    0x0000007c /* 124 */
#define REG_DX_L2_DEBUG_CKSUM3    0x00000080 /* 128 */
#define REG_DX_L2_DEBUG_CKSUM4    0x00000084 /* 132 */
#define REG_DX_L2_DEBUG_CKSUM5    0x00000088 /* 136 */
#define REG_DX_L2_DEBUG_CKSUM6    0x0000008c /* 140 */

/* Register Definition */
typedef volatile union _DX_L2_DEBUG_STAMP0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t LAYER_IDX:16;  /* DX_L2_DEBUG.STAMP0.LAYER_IDX [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:15;  /* DX_L2_DEBUG.STAMP0.RESERVED0 [30:16] (RW, 0x0)  */
        volatile uint32_t STAMP_EN:1;  /* DX_L2_DEBUG.STAMP0.STAMP_EN [31:31] (RW, 0x0)  */
    };
} DX_L2_DEBUG_STAMP0_t;
typedef volatile union _DX_L2_DEBUG_STAMP1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CLK_CNT:32;  /* DX_L2_DEBUG.STAMP1.CLK_CNT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP1_t;
typedef volatile union _DX_L2_DEBUG_STAMP2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DDR_RD_CNT_IN:32;  /* DX_L2_DEBUG.STAMP2.DDR_RD_CNT_IN [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP2_t;
typedef volatile union _DX_L2_DEBUG_STAMP3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DDR_RD_CNT_W:32;  /* DX_L2_DEBUG.STAMP3.DDR_RD_CNT_W [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP3_t;
typedef volatile union _DX_L2_DEBUG_STAMP4_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DDR_WR_CNT_OUT:32;  /* DX_L2_DEBUG.STAMP4.DDR_WR_CNT_OUT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP4_t;
typedef volatile union _DX_L2_DEBUG_STAMP5_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CONV_CNT:32;  /* DX_L2_DEBUG.STAMP5.CONV_CNT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP5_t;
typedef volatile union _DX_L2_DEBUG_STAMP6_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IDLE_CNT:32;  /* DX_L2_DEBUG.STAMP6.IDLE_CNT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP6_t;
typedef volatile union _DX_L2_DEBUG_STAMP7_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t WAIT_CNT:32;  /* DX_L2_DEBUG.STAMP7.WAIT_CNT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP7_t;
typedef volatile union _DX_L2_DEBUG_STAMP8_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t BN_CNT:32;  /* DX_L2_DEBUG.STAMP8.BN_CNT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP8_t;
typedef volatile union _DX_L2_DEBUG_STAMP9_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t BIAS_CNT:32;  /* DX_L2_DEBUG.STAMP9.BIAS_CNT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP9_t;
typedef volatile union _DX_L2_DEBUG_STAMP10_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP10*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t MUL_CNT:32;  /* DX_L2_DEBUG.STAMP10.MUL_CNT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP10_t;
typedef volatile union _DX_L2_DEBUG_STAMP11_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP11*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t ELEMENT_ADD_CNT:32;  /* DX_L2_DEBUG.STAMP11.ELEMENT_ADD_CNT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP11_t;
typedef volatile union _DX_L2_DEBUG_STAMP12_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP12*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CLK_CNT_SEL:24;  /* DX_L2_DEBUG.STAMP12.CLK_CNT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP12.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP12_t;
typedef volatile union _DX_L2_DEBUG_STAMP13_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP13*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DDR_RD_CNT_IN_SEL:24;  /* DX_L2_DEBUG.STAMP13.DDR_RD_CNT_IN_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP13.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP13_t;
typedef volatile union _DX_L2_DEBUG_STAMP14_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP14*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DDR_RD_CNT_W_SEL:24;  /* DX_L2_DEBUG.STAMP14.DDR_RD_CNT_W_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP14.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP14_t;
typedef volatile union _DX_L2_DEBUG_STAMP15_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP15*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DDR_WR_CNT_OUT_SEL:24;  /* DX_L2_DEBUG.STAMP15.DDR_WR_CNT_OUT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP15.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP15_t;
typedef volatile union _DX_L2_DEBUG_STAMP16_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP16*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CONV_CNT_SEL:24;  /* DX_L2_DEBUG.STAMP16.CONV_CNT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP16.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP16_t;
typedef volatile union _DX_L2_DEBUG_STAMP17_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP17*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t IDLE_CNT_SEL:24;  /* DX_L2_DEBUG.STAMP17.IDLE_CNT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP17.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP17_t;
typedef volatile union _DX_L2_DEBUG_STAMP18_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP18*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t WAIT_CNT_SEL:24;  /* DX_L2_DEBUG.STAMP18.WAIT_CNT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP18.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP18_t;
typedef volatile union _DX_L2_DEBUG_STAMP19_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP19*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t BN_CNT_SEL:24;  /* DX_L2_DEBUG.STAMP19.BN_CNT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP19.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP19_t;
typedef volatile union _DX_L2_DEBUG_STAMP20_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP20*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t BIAS_CNT_SEL:24;  /* DX_L2_DEBUG.STAMP20.BIAS_CNT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP20.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP20_t;
typedef volatile union _DX_L2_DEBUG_STAMP21_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP21*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t MUL_CNT_SEL:24;  /* DX_L2_DEBUG.STAMP21.MUL_CNT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP21.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP21_t;
typedef volatile union _DX_L2_DEBUG_STAMP22_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP22*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t ELEMENT_ADD_CNT_SEL:24;  /* DX_L2_DEBUG.STAMP22.ELEMENT_ADD_CNT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP22.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP22_t;
typedef volatile union _DX_L2_DEBUG_STAMP23_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP23*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_RD_WR_CNT:32;  /* DX_L2_DEBUG.STAMP23.DMA_RD_WR_CNT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP23_t;
typedef volatile union _DX_L2_DEBUG_STAMP24_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP24*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_RD_ONLY_CNT:32;  /* DX_L2_DEBUG.STAMP24.DMA_RD_ONLY_CNT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP24_t;
typedef volatile union _DX_L2_DEBUG_STAMP25_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP25*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_WR_ONLY_CNT:32;  /* DX_L2_DEBUG.STAMP25.DMA_WR_ONLY_CNT [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP25_t;
typedef volatile union _DX_L2_DEBUG_STAMP26_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP26*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_RD_WR_CNT_SEL:24;  /* DX_L2_DEBUG.STAMP26.DMA_RD_WR_CNT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP26.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP26_t;
typedef volatile union _DX_L2_DEBUG_STAMP27_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP27*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_RD_ONLY_CNT_SEL:24;  /* DX_L2_DEBUG.STAMP27.DMA_RD_ONLY_CNT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP27.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP27_t;
typedef volatile union _DX_L2_DEBUG_STAMP28_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.STAMP28*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_WR_ONLY_CNT_SEL:24;  /* DX_L2_DEBUG.STAMP28.DMA_WR_ONLY_CNT_SEL [23:0] (R, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_DEBUG.STAMP28.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_L2_DEBUG_STAMP28_t;
typedef volatile union _DX_L2_DEBUG_CKSUM0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.CKSUM0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CKSUM_PE0_SFU_IN:32;  /* DX_L2_DEBUG.CKSUM0.CKSUM_PE0_SFU_IN [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_CKSUM0_t;
typedef volatile union _DX_L2_DEBUG_CKSUM1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.CKSUM1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CKSUM_PE1_SFU_IN:32;  /* DX_L2_DEBUG.CKSUM1.CKSUM_PE1_SFU_IN [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_CKSUM1_t;
typedef volatile union _DX_L2_DEBUG_CKSUM2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.CKSUM2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CKSUM_PE1_SFU_OUT:8;  /* DX_L2_DEBUG.CKSUM2.CKSUM_PE1_SFU_OUT [7:0] (R, 0x0)  */
        volatile uint32_t CKSUM_PE0_SFU_OUT:8;  /* DX_L2_DEBUG.CKSUM2.CKSUM_PE0_SFU_OUT [15:8] (R, 0x0)  */
        volatile uint32_t RESERVED0:16;  /* DX_L2_DEBUG.CKSUM2.RESERVED0 [31:16] (R, 0x0)  */
    };
} DX_L2_DEBUG_CKSUM2_t;
typedef volatile union _DX_L2_DEBUG_CKSUM3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.CKSUM3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:30;  /* DX_L2_DEBUG.CKSUM3.RESERVED0 [29:0] (R, 0x0)  */
        volatile uint32_t CKSUM_ACC_RST:1;  /* DX_L2_DEBUG.CKSUM3.CKSUM_ACC_RST [30:30] (RW, 0x0)  */
        volatile uint32_t CKSUM_EN:1;  /* DX_L2_DEBUG.CKSUM3.CKSUM_EN [31:31] (RW, 0x0)  */
    };
} DX_L2_DEBUG_CKSUM3_t;
typedef volatile union _DX_L2_DEBUG_CKSUM4_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.CKSUM4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CKSUM_PE0_SFU_IN_ACC:32;  /* DX_L2_DEBUG.CKSUM4.CKSUM_PE0_SFU_IN_ACC [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_CKSUM4_t;
typedef volatile union _DX_L2_DEBUG_CKSUM5_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.CKSUM5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CKSUM_PE1_SFU_IN_ACC:32;  /* DX_L2_DEBUG.CKSUM5.CKSUM_PE1_SFU_IN_ACC [31:0] (R, 0x0)  */
    };
} DX_L2_DEBUG_CKSUM5_t;
typedef volatile union _DX_L2_DEBUG_CKSUM6_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_DEBUG.CKSUM6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CKSUM_PE1_SFU_OUT_ACC:8;  /* DX_L2_DEBUG.CKSUM6.CKSUM_PE1_SFU_OUT_ACC [7:0] (R, 0x0)  */
        volatile uint32_t CKSUM_PE0_SFU_OUT_ACC:8;  /* DX_L2_DEBUG.CKSUM6.CKSUM_PE0_SFU_OUT_ACC [15:8] (R, 0x0)  */
        volatile uint32_t RESERVED0:16;  /* DX_L2_DEBUG.CKSUM6.RESERVED0 [31:16] (R, 0x0)  */
    };
} DX_L2_DEBUG_CKSUM6_t;

/* DX_L2_DEBUG Module Definition */
typedef volatile struct _dxDX_L2_DEBUG_t
{
    DX_L2_DEBUG_STAMP0_t STAMP0;  /* DEBUG 0x0 */
    DX_L2_DEBUG_STAMP1_t STAMP1;  /* DEBUG 0x4 */
    DX_L2_DEBUG_STAMP2_t STAMP2;  /* DEBUG 0x8 */
    DX_L2_DEBUG_STAMP3_t STAMP3;  /* DEBUG 0xc */
    DX_L2_DEBUG_STAMP4_t STAMP4;  /* DEBUG 0x10 */
    DX_L2_DEBUG_STAMP5_t STAMP5;  /* DEBUG 0x14 */
    DX_L2_DEBUG_STAMP6_t STAMP6;  /* DEBUG 0x18 */
    DX_L2_DEBUG_STAMP7_t STAMP7;  /* DEBUG 0x1c */
    DX_L2_DEBUG_STAMP8_t STAMP8;  /* DEBUG 0x20 */
    DX_L2_DEBUG_STAMP9_t STAMP9;  /* DEBUG 0x24 */
    DX_L2_DEBUG_STAMP10_t STAMP10;  /* DEBUG 0x28 */
    DX_L2_DEBUG_STAMP11_t STAMP11;  /* DEBUG 0x2c */
    DX_L2_DEBUG_STAMP12_t STAMP12;  /* DEBUG 0x30 */
    DX_L2_DEBUG_STAMP13_t STAMP13;  /* DEBUG 0x34 */
    DX_L2_DEBUG_STAMP14_t STAMP14;  /* DEBUG 0x38 */
    DX_L2_DEBUG_STAMP15_t STAMP15;  /* DEBUG 0x3c */
    DX_L2_DEBUG_STAMP16_t STAMP16;  /* DEBUG 0x40 */
    DX_L2_DEBUG_STAMP17_t STAMP17;  /* DEBUG 0x44 */
    DX_L2_DEBUG_STAMP18_t STAMP18;  /* DEBUG 0x48 */
    DX_L2_DEBUG_STAMP19_t STAMP19;  /* DEBUG 0x4c */
    DX_L2_DEBUG_STAMP20_t STAMP20;  /* DEBUG 0x50 */
    DX_L2_DEBUG_STAMP21_t STAMP21;  /* DEBUG 0x54 */
    DX_L2_DEBUG_STAMP22_t STAMP22;  /* DEBUG 0x58 */
    DX_L2_DEBUG_STAMP23_t STAMP23;  /* DEBUG 0x5c */
    DX_L2_DEBUG_STAMP24_t STAMP24;  /* DEBUG 0x60 */
    DX_L2_DEBUG_STAMP25_t STAMP25;  /* DEBUG 0x64 */
    DX_L2_DEBUG_STAMP26_t STAMP26;  /* DEBUG 0x68 */
    DX_L2_DEBUG_STAMP27_t STAMP27;  /* DEBUG 0x6c */
    DX_L2_DEBUG_STAMP28_t STAMP28;  /* DEBUG 0x70 */
    DX_L2_DEBUG_CKSUM0_t CKSUM0;  /* DEBUG 0x74 */
    DX_L2_DEBUG_CKSUM1_t CKSUM1;  /* DEBUG 0x78 */
    DX_L2_DEBUG_CKSUM2_t CKSUM2;  /* DEBUG 0x7c */
    DX_L2_DEBUG_CKSUM3_t CKSUM3;  /* DEBUG 0x80 */
    DX_L2_DEBUG_CKSUM4_t CKSUM4;  /* DEBUG 0x84 */
    DX_L2_DEBUG_CKSUM5_t CKSUM5;  /* DEBUG 0x88 */
    DX_L2_DEBUG_CKSUM6_t CKSUM6;  /* DEBUG 0x8c */
} dxDX_L2_DEBUG_t;

/* Register access API */
/* DX_L2_DEBUG.STAMP0 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP0(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP0)
#define WRITE_DX_L2_DEBUG_STAMP0(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP0, val)
/* DX_L2_DEBUG.STAMP0.LAYER_IDX (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP0_LAYER_IDX_LEN        16
#define DX_L2_DEBUG_STAMP0_LAYER_IDX_OFFSET     0
#define DX_L2_DEBUG_STAMP0_LAYER_IDX_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_DEBUG_STAMP0_LAYER_IDX_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP0_LAYER_IDX(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP0, DX_L2_DEBUG_STAMP0_LAYER_IDX_MASK)
#define READ_DX_L2_DEBUG_STAMP0_LAYER_IDX(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP0, DX_L2_DEBUG_STAMP0_LAYER_IDX_MASK, DX_L2_DEBUG_STAMP0_LAYER_IDX_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP0_LAYER_IDX(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP0, bits, DX_L2_DEBUG_STAMP0_LAYER_IDX_MASK, DX_L2_DEBUG_STAMP0_LAYER_IDX_OFFSET)

/* DX_L2_DEBUG.STAMP0.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP0_RESERVED0_LEN        15
#define DX_L2_DEBUG_STAMP0_RESERVED0_OFFSET     16
#define DX_L2_DEBUG_STAMP0_RESERVED0_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_L2_DEBUG_STAMP0_RESERVED0_MASK       0x7fff0000 /* [30:16] 0b01111111111111110000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP0, DX_L2_DEBUG_STAMP0_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP0, DX_L2_DEBUG_STAMP0_RESERVED0_MASK, DX_L2_DEBUG_STAMP0_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP0, bits, DX_L2_DEBUG_STAMP0_RESERVED0_MASK, DX_L2_DEBUG_STAMP0_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP0.STAMP_EN (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP0_STAMP_EN_LEN        1
#define DX_L2_DEBUG_STAMP0_STAMP_EN_OFFSET     31
#define DX_L2_DEBUG_STAMP0_STAMP_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_DEBUG_STAMP0_STAMP_EN_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP0_STAMP_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP0, DX_L2_DEBUG_STAMP0_STAMP_EN_MASK)
#define READ_DX_L2_DEBUG_STAMP0_STAMP_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP0, DX_L2_DEBUG_STAMP0_STAMP_EN_MASK, DX_L2_DEBUG_STAMP0_STAMP_EN_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP0_STAMP_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP0, bits, DX_L2_DEBUG_STAMP0_STAMP_EN_MASK, DX_L2_DEBUG_STAMP0_STAMP_EN_OFFSET)

/* DX_L2_DEBUG.STAMP1 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP1(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP1)
#define WRITE_DX_L2_DEBUG_STAMP1(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP1, val)
/* DX_L2_DEBUG.STAMP1.CLK_CNT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP1_CLK_CNT_LEN        32
#define DX_L2_DEBUG_STAMP1_CLK_CNT_OFFSET     0
#define DX_L2_DEBUG_STAMP1_CLK_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP1_CLK_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP1_CLK_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP1, DX_L2_DEBUG_STAMP1_CLK_CNT_MASK)
#define READ_DX_L2_DEBUG_STAMP1_CLK_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP1, DX_L2_DEBUG_STAMP1_CLK_CNT_MASK, DX_L2_DEBUG_STAMP1_CLK_CNT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP1_CLK_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP1, bits, DX_L2_DEBUG_STAMP1_CLK_CNT_MASK, DX_L2_DEBUG_STAMP1_CLK_CNT_OFFSET)

/* DX_L2_DEBUG.STAMP2 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP2(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP2)
#define WRITE_DX_L2_DEBUG_STAMP2(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP2, val)
/* DX_L2_DEBUG.STAMP2.DDR_RD_CNT_IN (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN_LEN        32
#define DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN_OFFSET     0
#define DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP2, DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN_MASK)
#define READ_DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP2, DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN_MASK, DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP2, bits, DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN_MASK, DX_L2_DEBUG_STAMP2_DDR_RD_CNT_IN_OFFSET)

/* DX_L2_DEBUG.STAMP3 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP3(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP3)
#define WRITE_DX_L2_DEBUG_STAMP3(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP3, val)
/* DX_L2_DEBUG.STAMP3.DDR_RD_CNT_W (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W_LEN        32
#define DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W_OFFSET     0
#define DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP3, DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W_MASK)
#define READ_DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP3, DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W_MASK, DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP3, bits, DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W_MASK, DX_L2_DEBUG_STAMP3_DDR_RD_CNT_W_OFFSET)

/* DX_L2_DEBUG.STAMP4 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP4(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP4)
#define WRITE_DX_L2_DEBUG_STAMP4(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP4, val)
/* DX_L2_DEBUG.STAMP4.DDR_WR_CNT_OUT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT_LEN        32
#define DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT_OFFSET     0
#define DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP4, DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT_MASK)
#define READ_DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP4, DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT_MASK, DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP4, bits, DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT_MASK, DX_L2_DEBUG_STAMP4_DDR_WR_CNT_OUT_OFFSET)

/* DX_L2_DEBUG.STAMP5 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP5(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP5)
#define WRITE_DX_L2_DEBUG_STAMP5(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP5, val)
/* DX_L2_DEBUG.STAMP5.CONV_CNT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP5_CONV_CNT_LEN        32
#define DX_L2_DEBUG_STAMP5_CONV_CNT_OFFSET     0
#define DX_L2_DEBUG_STAMP5_CONV_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP5_CONV_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP5_CONV_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP5, DX_L2_DEBUG_STAMP5_CONV_CNT_MASK)
#define READ_DX_L2_DEBUG_STAMP5_CONV_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP5, DX_L2_DEBUG_STAMP5_CONV_CNT_MASK, DX_L2_DEBUG_STAMP5_CONV_CNT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP5_CONV_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP5, bits, DX_L2_DEBUG_STAMP5_CONV_CNT_MASK, DX_L2_DEBUG_STAMP5_CONV_CNT_OFFSET)

/* DX_L2_DEBUG.STAMP6 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP6(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP6)
#define WRITE_DX_L2_DEBUG_STAMP6(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP6, val)
/* DX_L2_DEBUG.STAMP6.IDLE_CNT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP6_IDLE_CNT_LEN        32
#define DX_L2_DEBUG_STAMP6_IDLE_CNT_OFFSET     0
#define DX_L2_DEBUG_STAMP6_IDLE_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP6_IDLE_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP6_IDLE_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP6, DX_L2_DEBUG_STAMP6_IDLE_CNT_MASK)
#define READ_DX_L2_DEBUG_STAMP6_IDLE_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP6, DX_L2_DEBUG_STAMP6_IDLE_CNT_MASK, DX_L2_DEBUG_STAMP6_IDLE_CNT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP6_IDLE_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP6, bits, DX_L2_DEBUG_STAMP6_IDLE_CNT_MASK, DX_L2_DEBUG_STAMP6_IDLE_CNT_OFFSET)

/* DX_L2_DEBUG.STAMP7 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP7(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP7)
#define WRITE_DX_L2_DEBUG_STAMP7(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP7, val)
/* DX_L2_DEBUG.STAMP7.WAIT_CNT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP7_WAIT_CNT_LEN        32
#define DX_L2_DEBUG_STAMP7_WAIT_CNT_OFFSET     0
#define DX_L2_DEBUG_STAMP7_WAIT_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP7_WAIT_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP7_WAIT_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP7, DX_L2_DEBUG_STAMP7_WAIT_CNT_MASK)
#define READ_DX_L2_DEBUG_STAMP7_WAIT_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP7, DX_L2_DEBUG_STAMP7_WAIT_CNT_MASK, DX_L2_DEBUG_STAMP7_WAIT_CNT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP7_WAIT_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP7, bits, DX_L2_DEBUG_STAMP7_WAIT_CNT_MASK, DX_L2_DEBUG_STAMP7_WAIT_CNT_OFFSET)

/* DX_L2_DEBUG.STAMP8 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP8(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP8)
#define WRITE_DX_L2_DEBUG_STAMP8(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP8, val)
/* DX_L2_DEBUG.STAMP8.BN_CNT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP8_BN_CNT_LEN        32
#define DX_L2_DEBUG_STAMP8_BN_CNT_OFFSET     0
#define DX_L2_DEBUG_STAMP8_BN_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP8_BN_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP8_BN_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP8, DX_L2_DEBUG_STAMP8_BN_CNT_MASK)
#define READ_DX_L2_DEBUG_STAMP8_BN_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP8, DX_L2_DEBUG_STAMP8_BN_CNT_MASK, DX_L2_DEBUG_STAMP8_BN_CNT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP8_BN_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP8, bits, DX_L2_DEBUG_STAMP8_BN_CNT_MASK, DX_L2_DEBUG_STAMP8_BN_CNT_OFFSET)

/* DX_L2_DEBUG.STAMP9 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP9(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP9)
#define WRITE_DX_L2_DEBUG_STAMP9(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP9, val)
/* DX_L2_DEBUG.STAMP9.BIAS_CNT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP9_BIAS_CNT_LEN        32
#define DX_L2_DEBUG_STAMP9_BIAS_CNT_OFFSET     0
#define DX_L2_DEBUG_STAMP9_BIAS_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP9_BIAS_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP9_BIAS_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP9, DX_L2_DEBUG_STAMP9_BIAS_CNT_MASK)
#define READ_DX_L2_DEBUG_STAMP9_BIAS_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP9, DX_L2_DEBUG_STAMP9_BIAS_CNT_MASK, DX_L2_DEBUG_STAMP9_BIAS_CNT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP9_BIAS_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP9, bits, DX_L2_DEBUG_STAMP9_BIAS_CNT_MASK, DX_L2_DEBUG_STAMP9_BIAS_CNT_OFFSET)

/* DX_L2_DEBUG.STAMP10 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP10(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP10)
#define WRITE_DX_L2_DEBUG_STAMP10(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP10, val)
/* DX_L2_DEBUG.STAMP10.MUL_CNT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP10_MUL_CNT_LEN        32
#define DX_L2_DEBUG_STAMP10_MUL_CNT_OFFSET     0
#define DX_L2_DEBUG_STAMP10_MUL_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP10_MUL_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP10_MUL_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP10, DX_L2_DEBUG_STAMP10_MUL_CNT_MASK)
#define READ_DX_L2_DEBUG_STAMP10_MUL_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP10, DX_L2_DEBUG_STAMP10_MUL_CNT_MASK, DX_L2_DEBUG_STAMP10_MUL_CNT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP10_MUL_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP10, bits, DX_L2_DEBUG_STAMP10_MUL_CNT_MASK, DX_L2_DEBUG_STAMP10_MUL_CNT_OFFSET)

/* DX_L2_DEBUG.STAMP11 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP11(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP11)
#define WRITE_DX_L2_DEBUG_STAMP11(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP11, val)
/* DX_L2_DEBUG.STAMP11.ELEMENT_ADD_CNT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT_LEN        32
#define DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT_OFFSET     0
#define DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP11, DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT_MASK)
#define READ_DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP11, DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT_MASK, DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP11, bits, DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT_MASK, DX_L2_DEBUG_STAMP11_ELEMENT_ADD_CNT_OFFSET)

/* DX_L2_DEBUG.STAMP12 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP12(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP12)
#define WRITE_DX_L2_DEBUG_STAMP12(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP12, val)
/* DX_L2_DEBUG.STAMP12.CLK_CNT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP12_CLK_CNT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP12_CLK_CNT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP12_CLK_CNT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP12_CLK_CNT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP12_CLK_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP12, DX_L2_DEBUG_STAMP12_CLK_CNT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP12_CLK_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP12, DX_L2_DEBUG_STAMP12_CLK_CNT_SEL_MASK, DX_L2_DEBUG_STAMP12_CLK_CNT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP12_CLK_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP12, bits, DX_L2_DEBUG_STAMP12_CLK_CNT_SEL_MASK, DX_L2_DEBUG_STAMP12_CLK_CNT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP12.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP12_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP12_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP12_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP12_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP12_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP12, DX_L2_DEBUG_STAMP12_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP12_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP12, DX_L2_DEBUG_STAMP12_RESERVED0_MASK, DX_L2_DEBUG_STAMP12_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP12_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP12, bits, DX_L2_DEBUG_STAMP12_RESERVED0_MASK, DX_L2_DEBUG_STAMP12_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP13 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP13(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP13)
#define WRITE_DX_L2_DEBUG_STAMP13(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP13, val)
/* DX_L2_DEBUG.STAMP13.DDR_RD_CNT_IN_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL_LEN        24
#define DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP13, DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP13, DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL_MASK, DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP13, bits, DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL_MASK, DX_L2_DEBUG_STAMP13_DDR_RD_CNT_IN_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP13.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP13_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP13_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP13_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP13_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP13_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP13, DX_L2_DEBUG_STAMP13_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP13_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP13, DX_L2_DEBUG_STAMP13_RESERVED0_MASK, DX_L2_DEBUG_STAMP13_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP13_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP13, bits, DX_L2_DEBUG_STAMP13_RESERVED0_MASK, DX_L2_DEBUG_STAMP13_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP14 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP14(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP14)
#define WRITE_DX_L2_DEBUG_STAMP14(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP14, val)
/* DX_L2_DEBUG.STAMP14.DDR_RD_CNT_W_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL_LEN        24
#define DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP14, DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP14, DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL_MASK, DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP14, bits, DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL_MASK, DX_L2_DEBUG_STAMP14_DDR_RD_CNT_W_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP14.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP14_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP14_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP14_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP14_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP14_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP14, DX_L2_DEBUG_STAMP14_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP14_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP14, DX_L2_DEBUG_STAMP14_RESERVED0_MASK, DX_L2_DEBUG_STAMP14_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP14_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP14, bits, DX_L2_DEBUG_STAMP14_RESERVED0_MASK, DX_L2_DEBUG_STAMP14_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP15 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP15(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP15)
#define WRITE_DX_L2_DEBUG_STAMP15(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP15, val)
/* DX_L2_DEBUG.STAMP15.DDR_WR_CNT_OUT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP15, DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP15, DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL_MASK, DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP15, bits, DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL_MASK, DX_L2_DEBUG_STAMP15_DDR_WR_CNT_OUT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP15.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP15_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP15_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP15_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP15_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP15_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP15, DX_L2_DEBUG_STAMP15_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP15_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP15, DX_L2_DEBUG_STAMP15_RESERVED0_MASK, DX_L2_DEBUG_STAMP15_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP15_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP15, bits, DX_L2_DEBUG_STAMP15_RESERVED0_MASK, DX_L2_DEBUG_STAMP15_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP16 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP16(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP16)
#define WRITE_DX_L2_DEBUG_STAMP16(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP16, val)
/* DX_L2_DEBUG.STAMP16.CONV_CNT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP16_CONV_CNT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP16_CONV_CNT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP16_CONV_CNT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP16_CONV_CNT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP16_CONV_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP16, DX_L2_DEBUG_STAMP16_CONV_CNT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP16_CONV_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP16, DX_L2_DEBUG_STAMP16_CONV_CNT_SEL_MASK, DX_L2_DEBUG_STAMP16_CONV_CNT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP16_CONV_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP16, bits, DX_L2_DEBUG_STAMP16_CONV_CNT_SEL_MASK, DX_L2_DEBUG_STAMP16_CONV_CNT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP16.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP16_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP16_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP16_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP16_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP16_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP16, DX_L2_DEBUG_STAMP16_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP16_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP16, DX_L2_DEBUG_STAMP16_RESERVED0_MASK, DX_L2_DEBUG_STAMP16_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP16_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP16, bits, DX_L2_DEBUG_STAMP16_RESERVED0_MASK, DX_L2_DEBUG_STAMP16_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP17 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP17(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP17)
#define WRITE_DX_L2_DEBUG_STAMP17(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP17, val)
/* DX_L2_DEBUG.STAMP17.IDLE_CNT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP17, DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP17, DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL_MASK, DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP17, bits, DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL_MASK, DX_L2_DEBUG_STAMP17_IDLE_CNT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP17.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP17_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP17_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP17_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP17_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP17_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP17, DX_L2_DEBUG_STAMP17_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP17_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP17, DX_L2_DEBUG_STAMP17_RESERVED0_MASK, DX_L2_DEBUG_STAMP17_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP17_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP17, bits, DX_L2_DEBUG_STAMP17_RESERVED0_MASK, DX_L2_DEBUG_STAMP17_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP18 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP18(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP18)
#define WRITE_DX_L2_DEBUG_STAMP18(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP18, val)
/* DX_L2_DEBUG.STAMP18.WAIT_CNT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP18, DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP18, DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL_MASK, DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP18, bits, DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL_MASK, DX_L2_DEBUG_STAMP18_WAIT_CNT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP18.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP18_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP18_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP18_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP18_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP18_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP18, DX_L2_DEBUG_STAMP18_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP18_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP18, DX_L2_DEBUG_STAMP18_RESERVED0_MASK, DX_L2_DEBUG_STAMP18_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP18_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP18, bits, DX_L2_DEBUG_STAMP18_RESERVED0_MASK, DX_L2_DEBUG_STAMP18_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP19 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP19(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP19)
#define WRITE_DX_L2_DEBUG_STAMP19(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP19, val)
/* DX_L2_DEBUG.STAMP19.BN_CNT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP19_BN_CNT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP19_BN_CNT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP19_BN_CNT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP19_BN_CNT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP19_BN_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP19, DX_L2_DEBUG_STAMP19_BN_CNT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP19_BN_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP19, DX_L2_DEBUG_STAMP19_BN_CNT_SEL_MASK, DX_L2_DEBUG_STAMP19_BN_CNT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP19_BN_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP19, bits, DX_L2_DEBUG_STAMP19_BN_CNT_SEL_MASK, DX_L2_DEBUG_STAMP19_BN_CNT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP19.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP19_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP19_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP19_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP19_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP19_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP19, DX_L2_DEBUG_STAMP19_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP19_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP19, DX_L2_DEBUG_STAMP19_RESERVED0_MASK, DX_L2_DEBUG_STAMP19_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP19_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP19, bits, DX_L2_DEBUG_STAMP19_RESERVED0_MASK, DX_L2_DEBUG_STAMP19_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP20 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP20(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP20)
#define WRITE_DX_L2_DEBUG_STAMP20(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP20, val)
/* DX_L2_DEBUG.STAMP20.BIAS_CNT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP20, DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP20, DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL_MASK, DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP20, bits, DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL_MASK, DX_L2_DEBUG_STAMP20_BIAS_CNT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP20.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP20_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP20_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP20_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP20_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP20_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP20, DX_L2_DEBUG_STAMP20_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP20_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP20, DX_L2_DEBUG_STAMP20_RESERVED0_MASK, DX_L2_DEBUG_STAMP20_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP20_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP20, bits, DX_L2_DEBUG_STAMP20_RESERVED0_MASK, DX_L2_DEBUG_STAMP20_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP21 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP21(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP21)
#define WRITE_DX_L2_DEBUG_STAMP21(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP21, val)
/* DX_L2_DEBUG.STAMP21.MUL_CNT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP21_MUL_CNT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP21_MUL_CNT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP21_MUL_CNT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP21_MUL_CNT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP21_MUL_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP21, DX_L2_DEBUG_STAMP21_MUL_CNT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP21_MUL_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP21, DX_L2_DEBUG_STAMP21_MUL_CNT_SEL_MASK, DX_L2_DEBUG_STAMP21_MUL_CNT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP21_MUL_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP21, bits, DX_L2_DEBUG_STAMP21_MUL_CNT_SEL_MASK, DX_L2_DEBUG_STAMP21_MUL_CNT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP21.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP21_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP21_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP21_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP21_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP21_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP21, DX_L2_DEBUG_STAMP21_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP21_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP21, DX_L2_DEBUG_STAMP21_RESERVED0_MASK, DX_L2_DEBUG_STAMP21_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP21_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP21, bits, DX_L2_DEBUG_STAMP21_RESERVED0_MASK, DX_L2_DEBUG_STAMP21_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP22 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP22(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP22)
#define WRITE_DX_L2_DEBUG_STAMP22(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP22, val)
/* DX_L2_DEBUG.STAMP22.ELEMENT_ADD_CNT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP22, DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP22, DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL_MASK, DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP22, bits, DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL_MASK, DX_L2_DEBUG_STAMP22_ELEMENT_ADD_CNT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP22.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP22_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP22_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP22_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP22_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP22_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP22, DX_L2_DEBUG_STAMP22_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP22_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP22, DX_L2_DEBUG_STAMP22_RESERVED0_MASK, DX_L2_DEBUG_STAMP22_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP22_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP22, bits, DX_L2_DEBUG_STAMP22_RESERVED0_MASK, DX_L2_DEBUG_STAMP22_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP23 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP23(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP23)
#define WRITE_DX_L2_DEBUG_STAMP23(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP23, val)
/* DX_L2_DEBUG.STAMP23.DMA_RD_WR_CNT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT_LEN        32
#define DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT_OFFSET     0
#define DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP23, DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT_MASK)
#define READ_DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP23, DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT_MASK, DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP23, bits, DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT_MASK, DX_L2_DEBUG_STAMP23_DMA_RD_WR_CNT_OFFSET)

/* DX_L2_DEBUG.STAMP24 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP24(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP24)
#define WRITE_DX_L2_DEBUG_STAMP24(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP24, val)
/* DX_L2_DEBUG.STAMP24.DMA_RD_ONLY_CNT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT_LEN        32
#define DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT_OFFSET     0
#define DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP24, DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT_MASK)
#define READ_DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP24, DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT_MASK, DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP24, bits, DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT_MASK, DX_L2_DEBUG_STAMP24_DMA_RD_ONLY_CNT_OFFSET)

/* DX_L2_DEBUG.STAMP25 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP25(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP25)
#define WRITE_DX_L2_DEBUG_STAMP25(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP25, val)
/* DX_L2_DEBUG.STAMP25.DMA_WR_ONLY_CNT (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT_LEN        32
#define DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT_OFFSET     0
#define DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP25, DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT_MASK)
#define READ_DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP25, DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT_MASK, DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP25, bits, DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT_MASK, DX_L2_DEBUG_STAMP25_DMA_WR_ONLY_CNT_OFFSET)

/* DX_L2_DEBUG.STAMP26 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP26(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP26)
#define WRITE_DX_L2_DEBUG_STAMP26(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP26, val)
/* DX_L2_DEBUG.STAMP26.DMA_RD_WR_CNT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP26, DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP26, DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL_MASK, DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP26, bits, DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL_MASK, DX_L2_DEBUG_STAMP26_DMA_RD_WR_CNT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP26.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP26_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP26_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP26_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP26_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP26_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP26, DX_L2_DEBUG_STAMP26_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP26_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP26, DX_L2_DEBUG_STAMP26_RESERVED0_MASK, DX_L2_DEBUG_STAMP26_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP26_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP26, bits, DX_L2_DEBUG_STAMP26_RESERVED0_MASK, DX_L2_DEBUG_STAMP26_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP27 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP27(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP27)
#define WRITE_DX_L2_DEBUG_STAMP27(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP27, val)
/* DX_L2_DEBUG.STAMP27.DMA_RD_ONLY_CNT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP27, DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP27, DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL_MASK, DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP27, bits, DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL_MASK, DX_L2_DEBUG_STAMP27_DMA_RD_ONLY_CNT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP27.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP27_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP27_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP27_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP27_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP27_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP27, DX_L2_DEBUG_STAMP27_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP27_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP27, DX_L2_DEBUG_STAMP27_RESERVED0_MASK, DX_L2_DEBUG_STAMP27_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP27_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP27, bits, DX_L2_DEBUG_STAMP27_RESERVED0_MASK, DX_L2_DEBUG_STAMP27_RESERVED0_OFFSET)

/* DX_L2_DEBUG.STAMP28 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_STAMP28(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_STAMP28)
#define WRITE_DX_L2_DEBUG_STAMP28(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_STAMP28, val)
/* DX_L2_DEBUG.STAMP28.DMA_WR_ONLY_CNT_SEL (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL_LEN        24
#define DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL_OFFSET     0
#define DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP28, DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL_MASK)
#define READ_DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP28, DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL_MASK, DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP28, bits, DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL_MASK, DX_L2_DEBUG_STAMP28_DMA_WR_ONLY_CNT_SEL_OFFSET)

/* DX_L2_DEBUG.STAMP28.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_STAMP28_RESERVED0_LEN        8
#define DX_L2_DEBUG_STAMP28_RESERVED0_OFFSET     24
#define DX_L2_DEBUG_STAMP28_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_STAMP28_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_STAMP28_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_STAMP28, DX_L2_DEBUG_STAMP28_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_STAMP28_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_STAMP28, DX_L2_DEBUG_STAMP28_RESERVED0_MASK, DX_L2_DEBUG_STAMP28_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_STAMP28_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_STAMP28, bits, DX_L2_DEBUG_STAMP28_RESERVED0_MASK, DX_L2_DEBUG_STAMP28_RESERVED0_OFFSET)

/* DX_L2_DEBUG.CKSUM0 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_CKSUM0(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_CKSUM0)
#define WRITE_DX_L2_DEBUG_CKSUM0(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_CKSUM0, val)
/* DX_L2_DEBUG.CKSUM0.CKSUM_PE0_SFU_IN (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN_LEN        32
#define DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN_OFFSET     0
#define DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM0, DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN_MASK)
#define READ_DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM0, DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN_MASK, DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM0, bits, DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN_MASK, DX_L2_DEBUG_CKSUM0_CKSUM_PE0_SFU_IN_OFFSET)

/* DX_L2_DEBUG.CKSUM1 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_CKSUM1(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_CKSUM1)
#define WRITE_DX_L2_DEBUG_CKSUM1(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_CKSUM1, val)
/* DX_L2_DEBUG.CKSUM1.CKSUM_PE1_SFU_IN (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN_LEN        32
#define DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN_OFFSET     0
#define DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM1, DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN_MASK)
#define READ_DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM1, DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN_MASK, DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM1, bits, DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN_MASK, DX_L2_DEBUG_CKSUM1_CKSUM_PE1_SFU_IN_OFFSET)

/* DX_L2_DEBUG.CKSUM2 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_CKSUM2(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_CKSUM2)
#define WRITE_DX_L2_DEBUG_CKSUM2(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_CKSUM2, val)
/* DX_L2_DEBUG.CKSUM2.CKSUM_PE1_SFU_OUT (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT_LEN        8
#define DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT_OFFSET     0
#define DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM2, DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT_MASK)
#define READ_DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM2, DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT_MASK, DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM2, bits, DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT_MASK, DX_L2_DEBUG_CKSUM2_CKSUM_PE1_SFU_OUT_OFFSET)

/* DX_L2_DEBUG.CKSUM2.CKSUM_PE0_SFU_OUT (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT_LEN        8
#define DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT_OFFSET     8
#define DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM2, DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT_MASK)
#define READ_DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM2, DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT_MASK, DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM2, bits, DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT_MASK, DX_L2_DEBUG_CKSUM2_CKSUM_PE0_SFU_OUT_OFFSET)

/* DX_L2_DEBUG.CKSUM2.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM2_RESERVED0_LEN        16
#define DX_L2_DEBUG_CKSUM2_RESERVED0_OFFSET     16
#define DX_L2_DEBUG_CKSUM2_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_DEBUG_CKSUM2_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_DEBUG_CKSUM2_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM2, DX_L2_DEBUG_CKSUM2_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_CKSUM2_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM2, DX_L2_DEBUG_CKSUM2_RESERVED0_MASK, DX_L2_DEBUG_CKSUM2_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM2_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM2, bits, DX_L2_DEBUG_CKSUM2_RESERVED0_MASK, DX_L2_DEBUG_CKSUM2_RESERVED0_OFFSET)

/* DX_L2_DEBUG.CKSUM3 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_CKSUM3(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_CKSUM3)
#define WRITE_DX_L2_DEBUG_CKSUM3(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_CKSUM3, val)
/* DX_L2_DEBUG.CKSUM3.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM3_RESERVED0_LEN        30
#define DX_L2_DEBUG_CKSUM3_RESERVED0_OFFSET     0
#define DX_L2_DEBUG_CKSUM3_RESERVED0_MASK_BITS  0x3fffffff /* [29:0] 0b00111111111111111111111111111111 */
#define DX_L2_DEBUG_CKSUM3_RESERVED0_MASK       0x3fffffff /* [29:0] 0b00111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_CKSUM3_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM3, DX_L2_DEBUG_CKSUM3_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_CKSUM3_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM3, DX_L2_DEBUG_CKSUM3_RESERVED0_MASK, DX_L2_DEBUG_CKSUM3_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM3_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM3, bits, DX_L2_DEBUG_CKSUM3_RESERVED0_MASK, DX_L2_DEBUG_CKSUM3_RESERVED0_OFFSET)

/* DX_L2_DEBUG.CKSUM3.CKSUM_ACC_RST (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST_LEN        1
#define DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST_OFFSET     30
#define DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM3, DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST_MASK)
#define READ_DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM3, DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST_MASK, DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM3, bits, DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST_MASK, DX_L2_DEBUG_CKSUM3_CKSUM_ACC_RST_OFFSET)

/* DX_L2_DEBUG.CKSUM3.CKSUM_EN (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM3_CKSUM_EN_LEN        1
#define DX_L2_DEBUG_CKSUM3_CKSUM_EN_OFFSET     31
#define DX_L2_DEBUG_CKSUM3_CKSUM_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_DEBUG_CKSUM3_CKSUM_EN_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_DEBUG_CKSUM3_CKSUM_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM3, DX_L2_DEBUG_CKSUM3_CKSUM_EN_MASK)
#define READ_DX_L2_DEBUG_CKSUM3_CKSUM_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM3, DX_L2_DEBUG_CKSUM3_CKSUM_EN_MASK, DX_L2_DEBUG_CKSUM3_CKSUM_EN_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM3_CKSUM_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM3, bits, DX_L2_DEBUG_CKSUM3_CKSUM_EN_MASK, DX_L2_DEBUG_CKSUM3_CKSUM_EN_OFFSET)

/* DX_L2_DEBUG.CKSUM4 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_CKSUM4(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_CKSUM4)
#define WRITE_DX_L2_DEBUG_CKSUM4(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_CKSUM4, val)
/* DX_L2_DEBUG.CKSUM4.CKSUM_PE0_SFU_IN_ACC (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC_LEN        32
#define DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC_OFFSET     0
#define DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM4, DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC_MASK)
#define READ_DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM4, DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC_MASK, DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM4, bits, DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC_MASK, DX_L2_DEBUG_CKSUM4_CKSUM_PE0_SFU_IN_ACC_OFFSET)

/* DX_L2_DEBUG.CKSUM5 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_CKSUM5(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_CKSUM5)
#define WRITE_DX_L2_DEBUG_CKSUM5(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_CKSUM5, val)
/* DX_L2_DEBUG.CKSUM5.CKSUM_PE1_SFU_IN_ACC (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC_LEN        32
#define DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC_OFFSET     0
#define DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM5, DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC_MASK)
#define READ_DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM5, DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC_MASK, DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM5, bits, DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC_MASK, DX_L2_DEBUG_CKSUM5_CKSUM_PE1_SFU_IN_ACC_OFFSET)

/* DX_L2_DEBUG.CKSUM6 (Category: DEBUG) */
#define READ_DX_L2_DEBUG_CKSUM6(base) dxrt::readl((void*)base, REG_DX_L2_DEBUG_CKSUM6)
#define WRITE_DX_L2_DEBUG_CKSUM6(base, val) dxrt::writel((void*)base, REG_DX_L2_DEBUG_CKSUM6, val)
/* DX_L2_DEBUG.CKSUM6.CKSUM_PE1_SFU_OUT_ACC (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC_LEN        8
#define DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC_OFFSET     0
#define DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM6, DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC_MASK)
#define READ_DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM6, DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC_MASK, DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM6, bits, DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC_MASK, DX_L2_DEBUG_CKSUM6_CKSUM_PE1_SFU_OUT_ACC_OFFSET)

/* DX_L2_DEBUG.CKSUM6.CKSUM_PE0_SFU_OUT_ACC (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC_LEN        8
#define DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC_OFFSET     8
#define DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM6, DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC_MASK)
#define READ_DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM6, DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC_MASK, DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM6, bits, DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC_MASK, DX_L2_DEBUG_CKSUM6_CKSUM_PE0_SFU_OUT_ACC_OFFSET)

/* DX_L2_DEBUG.CKSUM6.RESERVED0 (Category: DEBUG) */
#define DX_L2_DEBUG_CKSUM6_RESERVED0_LEN        16
#define DX_L2_DEBUG_CKSUM6_RESERVED0_OFFSET     16
#define DX_L2_DEBUG_CKSUM6_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_DEBUG_CKSUM6_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_DEBUG_CKSUM6_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_DEBUG_CKSUM6, DX_L2_DEBUG_CKSUM6_RESERVED0_MASK)
#define READ_DX_L2_DEBUG_CKSUM6_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_DEBUG_CKSUM6, DX_L2_DEBUG_CKSUM6_RESERVED0_MASK, DX_L2_DEBUG_CKSUM6_RESERVED0_OFFSET)
#define WRITE_DX_L2_DEBUG_CKSUM6_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_DEBUG_CKSUM6, bits, DX_L2_DEBUG_CKSUM6_RESERVED0_MASK, DX_L2_DEBUG_CKSUM6_RESERVED0_OFFSET)


int Verify_Dx_l2_debug_Registers(dxDX_L2_DEBUG_t* base);

} /* namespace dxrt */
