/* Automatically generated from DX_M1.xlsx
 * by Deepx Neural Network Generator v1.0
 */
#pragma once

#include <stdint.h>
#include "dxrt/npuif/npuif.h"

namespace dxrt{
extern bool logNpuRegAccess;

/* Address */
#define REG_DX_M1_DMA_OFFSET 0x10000
#define REG_DX_M1_DMA_AXI_BASE_ADDR_LOW    0x00000000 /* 0 */
#define REG_DX_M1_DMA_AXI_BASE_ADDR_HIGH    0x00000004 /* 4 */
#define REG_DX_M1_DMA_AXI4_RADDR    0x00000008 /* 8 */
#define REG_DX_M1_DMA_AXI4_WADDR    0x0000000c /* 12 */
#define REG_DX_M1_DMA_RSVD0    0x00000010 /* 16 */
#define REG_DX_M1_DMA_RSVD1    0x00000014 /* 20 */
#define REG_DX_M1_DMA_SRAM_DST_ADDR0    0x00000018 /* 24 */
#define REG_DX_M1_DMA_SRAM_DST_ADDR1    0x0000001c /* 28 */
#define REG_DX_M1_DMA_SRAM_SRC_ADDR    0x00000020 /* 32 */
#define REG_DX_M1_DMA_READ_SIZE    0x00000024 /* 36 */
#define REG_DX_M1_DMA_WRITE_SIZE    0x00000028 /* 40 */
#define REG_DX_M1_DMA_RSVD2    0x0000002c /* 44 */
#define REG_DX_M1_DMA_RSVD3    0x00000030 /* 48 */
#define REG_DX_M1_DMA_RSVD4    0x00000034 /* 52 */
#define REG_DX_M1_DMA_RSVD5    0x00000038 /* 56 */
#define REG_DX_M1_DMA_IRQ    0x0000003c /* 60 */
#define REG_DX_M1_DMA_CMD    0x00000040 /* 64 */
#define REG_DX_M1_DMA_STATUS    0x00000044 /* 68 */
#define REG_DX_M1_DMA_SFR_BASE_ADDR_LOW    0x00000048 /* 72 */
#define REG_DX_M1_DMA_SFR_BASE_ADDR_HIGH    0x0000004c /* 76 */
#define REG_DX_M1_DMA_SFR_ADDR_OFFS    0x00000050 /* 80 */
#define REG_DX_M1_DMA_SFR_START_ADDR    0x00000054 /* 84 */
#define REG_DX_M1_DMA_SFR_MOVE_CNT    0x00000058 /* 88 */
#define REG_DX_M1_DMA_SFR_ID    0x0000005c /* 92 */
#define REG_DX_M1_DMA_SFR_DMA_STATUS    0x00000060 /* 96 */
#define REG_DX_M1_DMA_DMA_CTRL    0x00000064 /* 100 */
#define REG_DX_M1_DMA_FLUSH_STATUS    0x00000068 /* 104 */
#define REG_DX_M1_DMA_AXI_CFG0    0x0000006c /* 108 */
#define REG_DX_M1_DMA_AXI_CFG1    0x00000070 /* 112 */
#define REG_DX_M1_DMA_AXI_CFG2    0x00000074 /* 116 */
#define REG_DX_M1_DMA_AXI_CFG3    0x00000078 /* 120 */
#define REG_DX_M1_DMA_AXI_CFG4    0x0000007c /* 124 */

/* Register Definition */
typedef volatile union _DX_M1_DMA_AXI_BASE_ADDR_LOW_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.AXI_BASE_ADDR_LOW*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t AXI4_BASE_ADDR_LOW:32;  /* DX_M1_DMA.AXI_BASE_ADDR_LOW.AXI4_BASE_ADDR_LOW [31:0] (RW, 0x0) AXI4 BASE ADDRESS LOW 32BIT */
    };
} DX_M1_DMA_AXI_BASE_ADDR_LOW_t;
typedef volatile union _DX_M1_DMA_AXI_BASE_ADDR_HIGH_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.AXI_BASE_ADDR_HIGH*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t AXI4_BASE_ADDR_HIGH:32;  /* DX_M1_DMA.AXI_BASE_ADDR_HIGH.AXI4_BASE_ADDR_HIGH [31:0] (RW, 0x0) AXI4 BASE ADDRESS HIGH 32BIT */
    };
} DX_M1_DMA_AXI_BASE_ADDR_HIGH_t;
typedef volatile union _DX_M1_DMA_AXI4_RADDR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.AXI4_RADDR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA0_AXI4_RADDR:32;  /* DX_M1_DMA.AXI4_RADDR.DMA0_AXI4_RADDR [31:0] (RW, 0x0) AXI4 ADDRESS 32BIT */
    };
} DX_M1_DMA_AXI4_RADDR_t;
typedef volatile union _DX_M1_DMA_AXI4_WADDR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.AXI4_WADDR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA0_AXI4_WADDR:32;  /* DX_M1_DMA.AXI4_WADDR.DMA0_AXI4_WADDR [31:0] (RW, 0x0)  */
    };
} DX_M1_DMA_AXI4_WADDR_t;
typedef volatile union _DX_M1_DMA_RSVD0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.RSVD0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_M1_DMA.RSVD0.RESERVED0 [31:0] (RW, 0x0)  */
    };
} DX_M1_DMA_RSVD0_t;
typedef volatile union _DX_M1_DMA_RSVD1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.RSVD1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_M1_DMA.RSVD1.RESERVED0 [31:0] (RW, 0x0)  */
    };
} DX_M1_DMA_RSVD1_t;
typedef volatile union _DX_M1_DMA_SRAM_DST_ADDR0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.SRAM_DST_ADDR0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA0_SRAM_DST_ADDR0:32;  /* DX_M1_DMA.SRAM_DST_ADDR0.DMA0_SRAM_DST_ADDR0 [31:0] (RW, 0x0)  */
    };
} DX_M1_DMA_SRAM_DST_ADDR0_t;
typedef volatile union _DX_M1_DMA_SRAM_DST_ADDR1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.SRAM_DST_ADDR1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA0_SRAM_DST_ADDR1:32;  /* DX_M1_DMA.SRAM_DST_ADDR1.DMA0_SRAM_DST_ADDR1 [31:0] (RW, 0x0)  */
    };
} DX_M1_DMA_SRAM_DST_ADDR1_t;
typedef volatile union _DX_M1_DMA_SRAM_SRC_ADDR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.SRAM_SRC_ADDR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA0_SRAM_SRC_ADDR:32;  /* DX_M1_DMA.SRAM_SRC_ADDR.DMA0_SRAM_SRC_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1_DMA_SRAM_SRC_ADDR_t;
typedef volatile union _DX_M1_DMA_READ_SIZE_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.READ_SIZE*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED1:6;  /* DX_M1_DMA.READ_SIZE.RESERVED1 [5:0] (R, 0x0)  */
        volatile uint32_t DMA0_READ_SIZE:16;  /* DX_M1_DMA.READ_SIZE.DMA0_READ_SIZE [21:6] (RW, 0x0) DATA MOVE BYTE SIZE. MULTIPLE OF 16BYTE(128BIT) */
        volatile uint32_t RESERVED0:10;  /* DX_M1_DMA.READ_SIZE.RESERVED0 [31:22] (RW, 0x0)  */
    };
} DX_M1_DMA_READ_SIZE_t;
typedef volatile union _DX_M1_DMA_WRITE_SIZE_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.WRITE_SIZE*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED1:6;  /* DX_M1_DMA.WRITE_SIZE.RESERVED1 [5:0] (R, 0x0)  */
        volatile uint32_t DMA0_WRITE_SIZE:16;  /* DX_M1_DMA.WRITE_SIZE.DMA0_WRITE_SIZE [21:6] (RW, 0x0) DATA MOVE BYTE SIZE. MULTIPLE OF 16BYTE(128BIT) */
        volatile uint32_t RESERVED0:10;  /* DX_M1_DMA.WRITE_SIZE.RESERVED0 [31:22] (RW, 0x0)  */
    };
} DX_M1_DMA_WRITE_SIZE_t;
typedef volatile union _DX_M1_DMA_RSVD2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.RSVD2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_M1_DMA.RSVD2.RESERVED0 [31:0] (RW, 0x0)  */
    };
} DX_M1_DMA_RSVD2_t;
typedef volatile union _DX_M1_DMA_RSVD3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.RSVD3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_M1_DMA.RSVD3.RESERVED0 [31:0] (RW, 0x0)  */
    };
} DX_M1_DMA_RSVD3_t;
typedef volatile union _DX_M1_DMA_RSVD4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.RSVD4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_M1_DMA.RSVD4.RESERVED0 [31:0] (RW, 0x0)  */
    };
} DX_M1_DMA_RSVD4_t;
typedef volatile union _DX_M1_DMA_RSVD5_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.RSVD5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_M1_DMA.RSVD5.RESERVED0 [31:0] (RW, 0x0)  */
    };
} DX_M1_DMA_RSVD5_t;
typedef volatile union _DX_M1_DMA_IRQ_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.IRQ*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_IRQ_EN:5;  /* DX_M1_DMA.IRQ.DMA_IRQ_EN [4:0] (RW, 0x18) DMA IRQ EN [0] : READ DMA IRQ EN [1] : WRITE DMA IRQ EN [2] : MEM COPY IRQ EN [3] : RDMA ERR EN [4] : WDMA ERR EN */
        volatile uint32_t RESERVED0:27;  /* DX_M1_DMA.IRQ.RESERVED0 [31:5] (RW, 0x0)  */
    };
} DX_M1_DMA_IRQ_t;
typedef volatile union _DX_M1_DMA_CMD_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.CMD*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_START_CMD:4;  /* DX_M1_DMA.CMD.DMA_START_CMD [3:0] (W, 0x0) DMA START COMMAND [0] : READ DMA START [1] : WRITE DMA START [2] : MEM COPY START [3] : READ DMA WITH 2-SRAM WRITE */
        volatile uint32_t RESERVED0:28;  /* DX_M1_DMA.CMD.RESERVED0 [31:4] (W, 0x0)  */
    };
} DX_M1_DMA_CMD_t;
typedef volatile union _DX_M1_DMA_STATUS_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.STATUS*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_DONE_STATUS:9;  /* DX_M1_DMA.STATUS.DMA_DONE_STATUS [8:0] (R, 0x0) DMA DONE STATUS [0] : READ DMA DONE [1] : WRITE DMA DONE [2] : MEM COPY DONE [3] : READ DMA ERROR [4] : WRITE DMA ERROR [6:5] : READ DMA ERROR CODE [8:7] : WRITE DMA ERROR CODE */
        volatile uint32_t RESERVED0:23;  /* DX_M1_DMA.STATUS.RESERVED0 [31:9] (R, 0x0)  */
    };
} DX_M1_DMA_STATUS_t;
typedef volatile union _DX_M1_DMA_SFR_BASE_ADDR_LOW_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.SFR_BASE_ADDR_LOW*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SFR_BASE_ADDR_LOW:32;  /* DX_M1_DMA.SFR_BASE_ADDR_LOW.SFR_BASE_ADDR_LOW [31:0] (RW, 0x0) SFR BASE AXI4 ADDRESS LOW 32BIT */
    };
} DX_M1_DMA_SFR_BASE_ADDR_LOW_t;
typedef volatile union _DX_M1_DMA_SFR_BASE_ADDR_HIGH_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.SFR_BASE_ADDR_HIGH*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SFR_BASE_ADDR_HIGH:32;  /* DX_M1_DMA.SFR_BASE_ADDR_HIGH.SFR_BASE_ADDR_HIGH [31:0] (RW, 0x0) SFR BASE AXI4 ADDRESS HIGH 32BIT */
    };
} DX_M1_DMA_SFR_BASE_ADDR_HIGH_t;
typedef volatile union _DX_M1_DMA_SFR_ADDR_OFFS_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.SFR_ADDR_OFFS*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:4;  /* DX_M1_DMA.SFR_ADDR_OFFS.RESERVED0 [3:0] (R, 0x0) SFR_ADDR_OFFS[3:0] */
        volatile uint32_t SFR_ADDR_OFFS:28;  /* DX_M1_DMA.SFR_ADDR_OFFS.SFR_ADDR_OFFS [31:4] (RW, 0x0) SFR ADDRESS OFFSET. AXI ADDRESS = SFR BASE ADDRESS + (LAYER ID X SFR ADDRESS OFFSET) */
    };
} DX_M1_DMA_SFR_ADDR_OFFS_t;
typedef volatile union _DX_M1_DMA_SFR_START_ADDR_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.SFR_START_ADDR*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SFR_START_ADDR:16;  /* DX_M1_DMA.SFR_START_ADDR.SFR_START_ADDR [15:0] (RW, 0x0) SFR WRITE START ADDRESS */
        volatile uint32_t RESERVED0:16;  /* DX_M1_DMA.SFR_START_ADDR.RESERVED0 [31:16] (RW, 0x0)  */
    };
} DX_M1_DMA_SFR_START_ADDR_t;
typedef volatile union _DX_M1_DMA_SFR_MOVE_CNT_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.SFR_MOVE_CNT*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED1:2;  /* DX_M1_DMA.SFR_MOVE_CNT.RESERVED1 [1:0] (R, 0x0) DATA MOVE BYTE COUNT */
        volatile uint32_t SFR_MOVE_CNT:12;  /* DX_M1_DMA.SFR_MOVE_CNT.SFR_MOVE_CNT [13:2] (RW, 0x0)  */
        volatile uint32_t RESERVED0:18;  /* DX_M1_DMA.SFR_MOVE_CNT.RESERVED0 [31:14] (RW, 0x0)  */
    };
} DX_M1_DMA_SFR_MOVE_CNT_t;
typedef volatile union _DX_M1_DMA_SFR_ID_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.SFR_ID*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SFR_ID:16;  /* DX_M1_DMA.SFR_ID.SFR_ID [15:0] (W, 0x0)  */
        volatile uint32_t RESERVED0:16;  /* DX_M1_DMA.SFR_ID.RESERVED0 [31:16] (W, 0x0)  */
    };
} DX_M1_DMA_SFR_ID_t;
typedef volatile union _DX_M1_DMA_SFR_DMA_STATUS_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.SFR_DMA_STATUS*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SFR_DMA_DONE_STATUS:1;  /* DX_M1_DMA.SFR_DMA_STATUS.SFR_DMA_DONE_STATUS [0:0] (R, 0x0) SFR DMA DONE IQR REGISTER. CLEAR THIS BIT AFTER CHECK IT DONE. */
        volatile uint32_t RESERVED0:31;  /* DX_M1_DMA.SFR_DMA_STATUS.RESERVED0 [31:1] (R, 0x0)  */
    };
} DX_M1_DMA_SFR_DMA_STATUS_t;
typedef volatile union _DX_M1_DMA_DMA_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.DMA_CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t DMA_CTRL_ENABLE:3;  /* DX_M1_DMA.DMA_CTRL.DMA_CTRL_ENABLE [2:0] (RW, 0x4) [0] : DMA FLUSH    0: NORMAL OPERATION,  1: FLUSH CURRENT DMA & DON'T ACCEPT DMA REQUESTS. [1] : USER DMA EN    0: DISABLE,   1: ENABLE [2] : NPU DMA EN    0: DISABLE,   1: ENABLE */
        volatile uint32_t RESERVED0:29;  /* DX_M1_DMA.DMA_CTRL.RESERVED0 [31:3] (RW, 0x0)  */
    };
} DX_M1_DMA_DMA_CTRL_t;
typedef volatile union _DX_M1_DMA_FLUSH_STATUS_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.FLUSH_STATUS*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FLUSH_STATUS:1;  /* DX_M1_DMA.FLUSH_STATUS.FLUSH_STATUS [0:0] (R, 0x0) NPU DMA FLUSH DONE.   0: FLUSH IS NOT DONE,  1: FLUSH IS DONE. */
        volatile uint32_t RESERVED0:31;  /* DX_M1_DMA.FLUSH_STATUS.RESERVED0 [31:1] (R, 0x0)  */
    };
} DX_M1_DMA_FLUSH_STATUS_t;
typedef volatile union _DX_M1_DMA_AXI_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.AXI_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t AXI_RDMA_BURST_LENGTH:9;  /* DX_M1_DMA.AXI_CFG0.AXI_RDMA_BURST_LENGTH [8:0] (RW, 0x10) AXI READ BURST LENGTH */
        volatile uint32_t RESERVED0:23;  /* DX_M1_DMA.AXI_CFG0.RESERVED0 [31:9] (RW, 0x0)  */
    };
} DX_M1_DMA_AXI_CFG0_t;
typedef volatile union _DX_M1_DMA_AXI_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.AXI_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t AXI_WDMA_BURST_LENGTH:9;  /* DX_M1_DMA.AXI_CFG1.AXI_WDMA_BURST_LENGTH [8:0] (RW, 0x10) AXI WRITE BURST LENGTH */
        volatile uint32_t RESERVED0:23;  /* DX_M1_DMA.AXI_CFG1.RESERVED0 [31:9] (RW, 0x0)  */
    };
} DX_M1_DMA_AXI_CFG1_t;
typedef volatile union _DX_M1_DMA_AXI_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.AXI_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t AXI_RDMA_MO:4;  /* DX_M1_DMA.AXI_CFG2.AXI_RDMA_MO [3:0] (RW, 0xf) AXI READ MUTI-OUTSTANDING NUMBER */
        volatile uint32_t RESERVED0:28;  /* DX_M1_DMA.AXI_CFG2.RESERVED0 [31:4] (RW, 0x0)  */
    };
} DX_M1_DMA_AXI_CFG2_t;
typedef volatile union _DX_M1_DMA_AXI_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.AXI_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t AXI_WDMA_MO:4;  /* DX_M1_DMA.AXI_CFG3.AXI_WDMA_MO [3:0] (RW, 0xf) AXI WRITE MULTI-OUTSTANDING NUMBER */
        volatile uint32_t RESERVED0:28;  /* DX_M1_DMA.AXI_CFG3.RESERVED0 [31:4] (RW, 0x0)  */
    };
} DX_M1_DMA_AXI_CFG3_t;
typedef volatile union _DX_M1_DMA_AXI_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1_DMA.AXI_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t AXI_4KB_BOUNDARY:1;  /* DX_M1_DMA.AXI_CFG4.AXI_4KB_BOUNDARY [0:0] (RW, 0x1) 4KB BOUNDARY PROCESS. ‘0’ : OFF , ‘1’ : ON */
        volatile uint32_t RESERVED0:31;  /* DX_M1_DMA.AXI_CFG4.RESERVED0 [31:1] (RW, 0x0)  */
    };
} DX_M1_DMA_AXI_CFG4_t;

/* DX_M1_DMA Module Definition */
typedef volatile struct _dxDX_M1_DMA_t
{
    DX_M1_DMA_AXI_BASE_ADDR_LOW_t AXI_BASE_ADDR_LOW;  /* DMA 0x0 */
    DX_M1_DMA_AXI_BASE_ADDR_HIGH_t AXI_BASE_ADDR_HIGH;  /* DMA 0x4 */
    DX_M1_DMA_AXI4_RADDR_t AXI4_RADDR;  /* DMA 0x8 */
    DX_M1_DMA_AXI4_WADDR_t AXI4_WADDR;  /* DMA 0xc */
    DX_M1_DMA_RSVD0_t RSVD0;  /* DMA 0x10 */
    DX_M1_DMA_RSVD1_t RSVD1;  /* DMA 0x14 */
    DX_M1_DMA_SRAM_DST_ADDR0_t SRAM_DST_ADDR0;  /* DMA 0x18 */
    DX_M1_DMA_SRAM_DST_ADDR1_t SRAM_DST_ADDR1;  /* DMA 0x1c */
    DX_M1_DMA_SRAM_SRC_ADDR_t SRAM_SRC_ADDR;  /* DMA 0x20 */
    DX_M1_DMA_READ_SIZE_t READ_SIZE;  /* DMA 0x24 */
    DX_M1_DMA_WRITE_SIZE_t WRITE_SIZE;  /* DMA 0x28 */
    DX_M1_DMA_RSVD2_t RSVD2;  /* DMA 0x2c */
    DX_M1_DMA_RSVD3_t RSVD3;  /* DMA 0x30 */
    DX_M1_DMA_RSVD4_t RSVD4;  /* DMA 0x34 */
    DX_M1_DMA_RSVD5_t RSVD5;  /* DMA 0x38 */
    DX_M1_DMA_IRQ_t IRQ;  /* DMA 0x3c */
    DX_M1_DMA_CMD_t CMD;  /* DMA 0x40 */
    DX_M1_DMA_STATUS_t STATUS;  /* DMA 0x44 */
    DX_M1_DMA_SFR_BASE_ADDR_LOW_t SFR_BASE_ADDR_LOW;  /* DMA 0x48 */
    DX_M1_DMA_SFR_BASE_ADDR_HIGH_t SFR_BASE_ADDR_HIGH;  /* DMA 0x4c */
    DX_M1_DMA_SFR_ADDR_OFFS_t SFR_ADDR_OFFS;  /* DMA 0x50 */
    DX_M1_DMA_SFR_START_ADDR_t SFR_START_ADDR;  /* DMA 0x54 */
    DX_M1_DMA_SFR_MOVE_CNT_t SFR_MOVE_CNT;  /* DMA 0x58 */
    DX_M1_DMA_SFR_ID_t SFR_ID;  /* DMA 0x5c */
    DX_M1_DMA_SFR_DMA_STATUS_t SFR_DMA_STATUS;  /* DMA 0x60 */
    DX_M1_DMA_DMA_CTRL_t DMA_CTRL;  /* DMA 0x64 */
    DX_M1_DMA_FLUSH_STATUS_t FLUSH_STATUS;  /* DMA 0x68 */
    DX_M1_DMA_AXI_CFG0_t AXI_CFG0;  /* DMA 0x6c */
    DX_M1_DMA_AXI_CFG1_t AXI_CFG1;  /* DMA 0x70 */
    DX_M1_DMA_AXI_CFG2_t AXI_CFG2;  /* DMA 0x74 */
    DX_M1_DMA_AXI_CFG3_t AXI_CFG3;  /* DMA 0x78 */
    DX_M1_DMA_AXI_CFG4_t AXI_CFG4;  /* DMA 0x7c */
} dxDX_M1_DMA_t;

/* Register access API */
/* DX_M1_DMA.AXI_BASE_ADDR_LOW (Category: DMA) */
#define READ_DX_M1_DMA_AXI_BASE_ADDR_LOW(base) dxrt::readl((void*)base, REG_DX_M1_DMA_AXI_BASE_ADDR_LOW)
#define WRITE_DX_M1_DMA_AXI_BASE_ADDR_LOW(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_AXI_BASE_ADDR_LOW, val)
/* DX_M1_DMA.AXI_BASE_ADDR_LOW.AXI4_BASE_ADDR_LOW (Category: DMA) */
#define DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW_LEN        32
#define DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW_OFFSET     0
#define DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_BASE_ADDR_LOW, DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW_MASK)
#define READ_DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_BASE_ADDR_LOW, DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW_MASK, DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW_OFFSET)
#define WRITE_DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_BASE_ADDR_LOW, bits, DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW_MASK, DX_M1_DMA_AXI_BASE_ADDR_LOW_AXI4_BASE_ADDR_LOW_OFFSET)

/* DX_M1_DMA.AXI_BASE_ADDR_HIGH (Category: DMA) */
#define READ_DX_M1_DMA_AXI_BASE_ADDR_HIGH(base) dxrt::readl((void*)base, REG_DX_M1_DMA_AXI_BASE_ADDR_HIGH)
#define WRITE_DX_M1_DMA_AXI_BASE_ADDR_HIGH(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_AXI_BASE_ADDR_HIGH, val)
/* DX_M1_DMA.AXI_BASE_ADDR_HIGH.AXI4_BASE_ADDR_HIGH (Category: DMA) */
#define DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH_LEN        32
#define DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH_OFFSET     0
#define DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_BASE_ADDR_HIGH, DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH_MASK)
#define READ_DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_BASE_ADDR_HIGH, DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH_MASK, DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH_OFFSET)
#define WRITE_DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_BASE_ADDR_HIGH, bits, DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH_MASK, DX_M1_DMA_AXI_BASE_ADDR_HIGH_AXI4_BASE_ADDR_HIGH_OFFSET)

/* DX_M1_DMA.AXI4_RADDR (Category: DMA) */
#define READ_DX_M1_DMA_AXI4_RADDR(base) dxrt::readl((void*)base, REG_DX_M1_DMA_AXI4_RADDR)
#define WRITE_DX_M1_DMA_AXI4_RADDR(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_AXI4_RADDR, val)
/* DX_M1_DMA.AXI4_RADDR.DMA0_AXI4_RADDR (Category: DMA) */
#define DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR_LEN        32
#define DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR_OFFSET     0
#define DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI4_RADDR, DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR_MASK)
#define READ_DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI4_RADDR, DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR_MASK, DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR_OFFSET)
#define WRITE_DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI4_RADDR, bits, DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR_MASK, DX_M1_DMA_AXI4_RADDR_DMA0_AXI4_RADDR_OFFSET)

/* DX_M1_DMA.AXI4_WADDR (Category: DMA) */
#define READ_DX_M1_DMA_AXI4_WADDR(base) dxrt::readl((void*)base, REG_DX_M1_DMA_AXI4_WADDR)
#define WRITE_DX_M1_DMA_AXI4_WADDR(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_AXI4_WADDR, val)
/* DX_M1_DMA.AXI4_WADDR.DMA0_AXI4_WADDR (Category: DMA) */
#define DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR_LEN        32
#define DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR_OFFSET     0
#define DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI4_WADDR, DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR_MASK)
#define READ_DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI4_WADDR, DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR_MASK, DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR_OFFSET)
#define WRITE_DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI4_WADDR, bits, DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR_MASK, DX_M1_DMA_AXI4_WADDR_DMA0_AXI4_WADDR_OFFSET)

/* DX_M1_DMA.RSVD0 (Category: DMA) */
#define READ_DX_M1_DMA_RSVD0(base) dxrt::readl((void*)base, REG_DX_M1_DMA_RSVD0)
#define WRITE_DX_M1_DMA_RSVD0(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_RSVD0, val)
/* DX_M1_DMA.RSVD0.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_RSVD0_RESERVED0_LEN        32
#define DX_M1_DMA_RSVD0_RESERVED0_OFFSET     0
#define DX_M1_DMA_RSVD0_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_RSVD0_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_RSVD0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_RSVD0, DX_M1_DMA_RSVD0_RESERVED0_MASK)
#define READ_DX_M1_DMA_RSVD0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_RSVD0, DX_M1_DMA_RSVD0_RESERVED0_MASK, DX_M1_DMA_RSVD0_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_RSVD0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_RSVD0, bits, DX_M1_DMA_RSVD0_RESERVED0_MASK, DX_M1_DMA_RSVD0_RESERVED0_OFFSET)

/* DX_M1_DMA.RSVD1 (Category: DMA) */
#define READ_DX_M1_DMA_RSVD1(base) dxrt::readl((void*)base, REG_DX_M1_DMA_RSVD1)
#define WRITE_DX_M1_DMA_RSVD1(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_RSVD1, val)
/* DX_M1_DMA.RSVD1.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_RSVD1_RESERVED0_LEN        32
#define DX_M1_DMA_RSVD1_RESERVED0_OFFSET     0
#define DX_M1_DMA_RSVD1_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_RSVD1_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_RSVD1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_RSVD1, DX_M1_DMA_RSVD1_RESERVED0_MASK)
#define READ_DX_M1_DMA_RSVD1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_RSVD1, DX_M1_DMA_RSVD1_RESERVED0_MASK, DX_M1_DMA_RSVD1_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_RSVD1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_RSVD1, bits, DX_M1_DMA_RSVD1_RESERVED0_MASK, DX_M1_DMA_RSVD1_RESERVED0_OFFSET)

/* DX_M1_DMA.SRAM_DST_ADDR0 (Category: DMA) */
#define READ_DX_M1_DMA_SRAM_DST_ADDR0(base) dxrt::readl((void*)base, REG_DX_M1_DMA_SRAM_DST_ADDR0)
#define WRITE_DX_M1_DMA_SRAM_DST_ADDR0(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_SRAM_DST_ADDR0, val)
/* DX_M1_DMA.SRAM_DST_ADDR0.DMA0_SRAM_DST_ADDR0 (Category: DMA) */
#define DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0_LEN        32
#define DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0_OFFSET     0
#define DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SRAM_DST_ADDR0, DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0_MASK)
#define READ_DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SRAM_DST_ADDR0, DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0_MASK, DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0_OFFSET)
#define WRITE_DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SRAM_DST_ADDR0, bits, DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0_MASK, DX_M1_DMA_SRAM_DST_ADDR0_DMA0_SRAM_DST_ADDR0_OFFSET)

/* DX_M1_DMA.SRAM_DST_ADDR1 (Category: DMA) */
#define READ_DX_M1_DMA_SRAM_DST_ADDR1(base) dxrt::readl((void*)base, REG_DX_M1_DMA_SRAM_DST_ADDR1)
#define WRITE_DX_M1_DMA_SRAM_DST_ADDR1(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_SRAM_DST_ADDR1, val)
/* DX_M1_DMA.SRAM_DST_ADDR1.DMA0_SRAM_DST_ADDR1 (Category: DMA) */
#define DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1_LEN        32
#define DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1_OFFSET     0
#define DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SRAM_DST_ADDR1, DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1_MASK)
#define READ_DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SRAM_DST_ADDR1, DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1_MASK, DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1_OFFSET)
#define WRITE_DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SRAM_DST_ADDR1, bits, DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1_MASK, DX_M1_DMA_SRAM_DST_ADDR1_DMA0_SRAM_DST_ADDR1_OFFSET)

/* DX_M1_DMA.SRAM_SRC_ADDR (Category: DMA) */
#define READ_DX_M1_DMA_SRAM_SRC_ADDR(base) dxrt::readl((void*)base, REG_DX_M1_DMA_SRAM_SRC_ADDR)
#define WRITE_DX_M1_DMA_SRAM_SRC_ADDR(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_SRAM_SRC_ADDR, val)
/* DX_M1_DMA.SRAM_SRC_ADDR.DMA0_SRAM_SRC_ADDR (Category: DMA) */
#define DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR_LEN        32
#define DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR_OFFSET     0
#define DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SRAM_SRC_ADDR, DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR_MASK)
#define READ_DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SRAM_SRC_ADDR, DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR_MASK, DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR_OFFSET)
#define WRITE_DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SRAM_SRC_ADDR, bits, DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR_MASK, DX_M1_DMA_SRAM_SRC_ADDR_DMA0_SRAM_SRC_ADDR_OFFSET)

/* DX_M1_DMA.READ_SIZE (Category: DMA) */
#define READ_DX_M1_DMA_READ_SIZE(base) dxrt::readl((void*)base, REG_DX_M1_DMA_READ_SIZE)
#define WRITE_DX_M1_DMA_READ_SIZE(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_READ_SIZE, val)
/* DX_M1_DMA.READ_SIZE.RESERVED1 (Category: DMA) */
#define DX_M1_DMA_READ_SIZE_RESERVED1_LEN        6
#define DX_M1_DMA_READ_SIZE_RESERVED1_OFFSET     0
#define DX_M1_DMA_READ_SIZE_RESERVED1_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1_DMA_READ_SIZE_RESERVED1_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_M1_DMA_READ_SIZE_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_READ_SIZE, DX_M1_DMA_READ_SIZE_RESERVED1_MASK)
#define READ_DX_M1_DMA_READ_SIZE_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_READ_SIZE, DX_M1_DMA_READ_SIZE_RESERVED1_MASK, DX_M1_DMA_READ_SIZE_RESERVED1_OFFSET)
#define WRITE_DX_M1_DMA_READ_SIZE_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_READ_SIZE, bits, DX_M1_DMA_READ_SIZE_RESERVED1_MASK, DX_M1_DMA_READ_SIZE_RESERVED1_OFFSET)

/* DX_M1_DMA.READ_SIZE.DMA0_READ_SIZE (Category: DMA) */
#define DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE_LEN        16
#define DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE_OFFSET     6
#define DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE_MASK       0x003fffc0 /* [21:6] 0b00000000001111111111111111000000 */
#define READ_MASK_DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_READ_SIZE, DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE_MASK)
#define READ_DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_READ_SIZE, DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE_MASK, DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE_OFFSET)
#define WRITE_DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_READ_SIZE, bits, DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE_MASK, DX_M1_DMA_READ_SIZE_DMA0_READ_SIZE_OFFSET)

/* DX_M1_DMA.READ_SIZE.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_READ_SIZE_RESERVED0_LEN        10
#define DX_M1_DMA_READ_SIZE_RESERVED0_OFFSET     22
#define DX_M1_DMA_READ_SIZE_RESERVED0_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_M1_DMA_READ_SIZE_RESERVED0_MASK       0xffc00000 /* [31:22] 0b11111111110000000000000000000000 */
#define READ_MASK_DX_M1_DMA_READ_SIZE_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_READ_SIZE, DX_M1_DMA_READ_SIZE_RESERVED0_MASK)
#define READ_DX_M1_DMA_READ_SIZE_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_READ_SIZE, DX_M1_DMA_READ_SIZE_RESERVED0_MASK, DX_M1_DMA_READ_SIZE_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_READ_SIZE_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_READ_SIZE, bits, DX_M1_DMA_READ_SIZE_RESERVED0_MASK, DX_M1_DMA_READ_SIZE_RESERVED0_OFFSET)

/* DX_M1_DMA.WRITE_SIZE (Category: DMA) */
#define READ_DX_M1_DMA_WRITE_SIZE(base) dxrt::readl((void*)base, REG_DX_M1_DMA_WRITE_SIZE)
#define WRITE_DX_M1_DMA_WRITE_SIZE(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_WRITE_SIZE, val)
/* DX_M1_DMA.WRITE_SIZE.RESERVED1 (Category: DMA) */
#define DX_M1_DMA_WRITE_SIZE_RESERVED1_LEN        6
#define DX_M1_DMA_WRITE_SIZE_RESERVED1_OFFSET     0
#define DX_M1_DMA_WRITE_SIZE_RESERVED1_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1_DMA_WRITE_SIZE_RESERVED1_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_M1_DMA_WRITE_SIZE_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_WRITE_SIZE, DX_M1_DMA_WRITE_SIZE_RESERVED1_MASK)
#define READ_DX_M1_DMA_WRITE_SIZE_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_WRITE_SIZE, DX_M1_DMA_WRITE_SIZE_RESERVED1_MASK, DX_M1_DMA_WRITE_SIZE_RESERVED1_OFFSET)
#define WRITE_DX_M1_DMA_WRITE_SIZE_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_WRITE_SIZE, bits, DX_M1_DMA_WRITE_SIZE_RESERVED1_MASK, DX_M1_DMA_WRITE_SIZE_RESERVED1_OFFSET)

/* DX_M1_DMA.WRITE_SIZE.DMA0_WRITE_SIZE (Category: DMA) */
#define DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE_LEN        16
#define DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE_OFFSET     6
#define DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE_MASK       0x003fffc0 /* [21:6] 0b00000000001111111111111111000000 */
#define READ_MASK_DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_WRITE_SIZE, DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE_MASK)
#define READ_DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_WRITE_SIZE, DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE_MASK, DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE_OFFSET)
#define WRITE_DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_WRITE_SIZE, bits, DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE_MASK, DX_M1_DMA_WRITE_SIZE_DMA0_WRITE_SIZE_OFFSET)

/* DX_M1_DMA.WRITE_SIZE.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_WRITE_SIZE_RESERVED0_LEN        10
#define DX_M1_DMA_WRITE_SIZE_RESERVED0_OFFSET     22
#define DX_M1_DMA_WRITE_SIZE_RESERVED0_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_M1_DMA_WRITE_SIZE_RESERVED0_MASK       0xffc00000 /* [31:22] 0b11111111110000000000000000000000 */
#define READ_MASK_DX_M1_DMA_WRITE_SIZE_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_WRITE_SIZE, DX_M1_DMA_WRITE_SIZE_RESERVED0_MASK)
#define READ_DX_M1_DMA_WRITE_SIZE_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_WRITE_SIZE, DX_M1_DMA_WRITE_SIZE_RESERVED0_MASK, DX_M1_DMA_WRITE_SIZE_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_WRITE_SIZE_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_WRITE_SIZE, bits, DX_M1_DMA_WRITE_SIZE_RESERVED0_MASK, DX_M1_DMA_WRITE_SIZE_RESERVED0_OFFSET)

/* DX_M1_DMA.RSVD2 (Category: DMA) */
#define READ_DX_M1_DMA_RSVD2(base) dxrt::readl((void*)base, REG_DX_M1_DMA_RSVD2)
#define WRITE_DX_M1_DMA_RSVD2(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_RSVD2, val)
/* DX_M1_DMA.RSVD2.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_RSVD2_RESERVED0_LEN        32
#define DX_M1_DMA_RSVD2_RESERVED0_OFFSET     0
#define DX_M1_DMA_RSVD2_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_RSVD2_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_RSVD2_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_RSVD2, DX_M1_DMA_RSVD2_RESERVED0_MASK)
#define READ_DX_M1_DMA_RSVD2_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_RSVD2, DX_M1_DMA_RSVD2_RESERVED0_MASK, DX_M1_DMA_RSVD2_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_RSVD2_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_RSVD2, bits, DX_M1_DMA_RSVD2_RESERVED0_MASK, DX_M1_DMA_RSVD2_RESERVED0_OFFSET)

/* DX_M1_DMA.RSVD3 (Category: DMA) */
#define READ_DX_M1_DMA_RSVD3(base) dxrt::readl((void*)base, REG_DX_M1_DMA_RSVD3)
#define WRITE_DX_M1_DMA_RSVD3(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_RSVD3, val)
/* DX_M1_DMA.RSVD3.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_RSVD3_RESERVED0_LEN        32
#define DX_M1_DMA_RSVD3_RESERVED0_OFFSET     0
#define DX_M1_DMA_RSVD3_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_RSVD3_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_RSVD3_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_RSVD3, DX_M1_DMA_RSVD3_RESERVED0_MASK)
#define READ_DX_M1_DMA_RSVD3_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_RSVD3, DX_M1_DMA_RSVD3_RESERVED0_MASK, DX_M1_DMA_RSVD3_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_RSVD3_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_RSVD3, bits, DX_M1_DMA_RSVD3_RESERVED0_MASK, DX_M1_DMA_RSVD3_RESERVED0_OFFSET)

/* DX_M1_DMA.RSVD4 (Category: DMA) */
#define READ_DX_M1_DMA_RSVD4(base) dxrt::readl((void*)base, REG_DX_M1_DMA_RSVD4)
#define WRITE_DX_M1_DMA_RSVD4(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_RSVD4, val)
/* DX_M1_DMA.RSVD4.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_RSVD4_RESERVED0_LEN        32
#define DX_M1_DMA_RSVD4_RESERVED0_OFFSET     0
#define DX_M1_DMA_RSVD4_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_RSVD4_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_RSVD4_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_RSVD4, DX_M1_DMA_RSVD4_RESERVED0_MASK)
#define READ_DX_M1_DMA_RSVD4_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_RSVD4, DX_M1_DMA_RSVD4_RESERVED0_MASK, DX_M1_DMA_RSVD4_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_RSVD4_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_RSVD4, bits, DX_M1_DMA_RSVD4_RESERVED0_MASK, DX_M1_DMA_RSVD4_RESERVED0_OFFSET)

/* DX_M1_DMA.RSVD5 (Category: DMA) */
#define READ_DX_M1_DMA_RSVD5(base) dxrt::readl((void*)base, REG_DX_M1_DMA_RSVD5)
#define WRITE_DX_M1_DMA_RSVD5(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_RSVD5, val)
/* DX_M1_DMA.RSVD5.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_RSVD5_RESERVED0_LEN        32
#define DX_M1_DMA_RSVD5_RESERVED0_OFFSET     0
#define DX_M1_DMA_RSVD5_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_RSVD5_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_RSVD5_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_RSVD5, DX_M1_DMA_RSVD5_RESERVED0_MASK)
#define READ_DX_M1_DMA_RSVD5_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_RSVD5, DX_M1_DMA_RSVD5_RESERVED0_MASK, DX_M1_DMA_RSVD5_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_RSVD5_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_RSVD5, bits, DX_M1_DMA_RSVD5_RESERVED0_MASK, DX_M1_DMA_RSVD5_RESERVED0_OFFSET)

/* DX_M1_DMA.IRQ (Category: DMA) */
#define READ_DX_M1_DMA_IRQ(base) dxrt::readl((void*)base, REG_DX_M1_DMA_IRQ)
#define WRITE_DX_M1_DMA_IRQ(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_IRQ, val)
/* DX_M1_DMA.IRQ.DMA_IRQ_EN (Category: DMA) */
#define DX_M1_DMA_IRQ_DMA_IRQ_EN_LEN        5
#define DX_M1_DMA_IRQ_DMA_IRQ_EN_OFFSET     0
#define DX_M1_DMA_IRQ_DMA_IRQ_EN_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1_DMA_IRQ_DMA_IRQ_EN_MASK       0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define READ_MASK_DX_M1_DMA_IRQ_DMA_IRQ_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_IRQ, DX_M1_DMA_IRQ_DMA_IRQ_EN_MASK)
#define READ_DX_M1_DMA_IRQ_DMA_IRQ_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_IRQ, DX_M1_DMA_IRQ_DMA_IRQ_EN_MASK, DX_M1_DMA_IRQ_DMA_IRQ_EN_OFFSET)
#define WRITE_DX_M1_DMA_IRQ_DMA_IRQ_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_IRQ, bits, DX_M1_DMA_IRQ_DMA_IRQ_EN_MASK, DX_M1_DMA_IRQ_DMA_IRQ_EN_OFFSET)

/* DX_M1_DMA.IRQ.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_IRQ_RESERVED0_LEN        27
#define DX_M1_DMA_IRQ_RESERVED0_OFFSET     5
#define DX_M1_DMA_IRQ_RESERVED0_MASK_BITS  0x07ffffff /* [26:0] 0b00000111111111111111111111111111 */
#define DX_M1_DMA_IRQ_RESERVED0_MASK       0xffffffe0 /* [31:5] 0b11111111111111111111111111100000 */
#define READ_MASK_DX_M1_DMA_IRQ_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_IRQ, DX_M1_DMA_IRQ_RESERVED0_MASK)
#define READ_DX_M1_DMA_IRQ_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_IRQ, DX_M1_DMA_IRQ_RESERVED0_MASK, DX_M1_DMA_IRQ_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_IRQ_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_IRQ, bits, DX_M1_DMA_IRQ_RESERVED0_MASK, DX_M1_DMA_IRQ_RESERVED0_OFFSET)

/* DX_M1_DMA.CMD (Category: DMA) */
#define READ_DX_M1_DMA_CMD(base) dxrt::readl((void*)base, REG_DX_M1_DMA_CMD)
#define WRITE_DX_M1_DMA_CMD(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_CMD, val)
/* DX_M1_DMA.CMD.DMA_START_CMD (Category: DMA) */
#define DX_M1_DMA_CMD_DMA_START_CMD_LEN        4
#define DX_M1_DMA_CMD_DMA_START_CMD_OFFSET     0
#define DX_M1_DMA_CMD_DMA_START_CMD_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1_DMA_CMD_DMA_START_CMD_MASK       0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define READ_MASK_DX_M1_DMA_CMD_DMA_START_CMD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_CMD, DX_M1_DMA_CMD_DMA_START_CMD_MASK)
#define READ_DX_M1_DMA_CMD_DMA_START_CMD(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_CMD, DX_M1_DMA_CMD_DMA_START_CMD_MASK, DX_M1_DMA_CMD_DMA_START_CMD_OFFSET)
#define WRITE_DX_M1_DMA_CMD_DMA_START_CMD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_CMD, bits, DX_M1_DMA_CMD_DMA_START_CMD_MASK, DX_M1_DMA_CMD_DMA_START_CMD_OFFSET)

/* DX_M1_DMA.CMD.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_CMD_RESERVED0_LEN        28
#define DX_M1_DMA_CMD_RESERVED0_OFFSET     4
#define DX_M1_DMA_CMD_RESERVED0_MASK_BITS  0x0fffffff /* [27:0] 0b00001111111111111111111111111111 */
#define DX_M1_DMA_CMD_RESERVED0_MASK       0xfffffff0 /* [31:4] 0b11111111111111111111111111110000 */
#define READ_MASK_DX_M1_DMA_CMD_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_CMD, DX_M1_DMA_CMD_RESERVED0_MASK)
#define READ_DX_M1_DMA_CMD_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_CMD, DX_M1_DMA_CMD_RESERVED0_MASK, DX_M1_DMA_CMD_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_CMD_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_CMD, bits, DX_M1_DMA_CMD_RESERVED0_MASK, DX_M1_DMA_CMD_RESERVED0_OFFSET)

/* DX_M1_DMA.STATUS (Category: DMA) */
#define READ_DX_M1_DMA_STATUS(base) dxrt::readl((void*)base, REG_DX_M1_DMA_STATUS)
#define WRITE_DX_M1_DMA_STATUS(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_STATUS, val)
/* DX_M1_DMA.STATUS.DMA_DONE_STATUS (Category: DMA) */
#define DX_M1_DMA_STATUS_DMA_DONE_STATUS_LEN        9
#define DX_M1_DMA_STATUS_DMA_DONE_STATUS_OFFSET     0
#define DX_M1_DMA_STATUS_DMA_DONE_STATUS_MASK_BITS  0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define DX_M1_DMA_STATUS_DMA_DONE_STATUS_MASK       0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define READ_MASK_DX_M1_DMA_STATUS_DMA_DONE_STATUS(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_STATUS, DX_M1_DMA_STATUS_DMA_DONE_STATUS_MASK)
#define READ_DX_M1_DMA_STATUS_DMA_DONE_STATUS(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_STATUS, DX_M1_DMA_STATUS_DMA_DONE_STATUS_MASK, DX_M1_DMA_STATUS_DMA_DONE_STATUS_OFFSET)
#define WRITE_DX_M1_DMA_STATUS_DMA_DONE_STATUS(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_STATUS, bits, DX_M1_DMA_STATUS_DMA_DONE_STATUS_MASK, DX_M1_DMA_STATUS_DMA_DONE_STATUS_OFFSET)

/* DX_M1_DMA.STATUS.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_STATUS_RESERVED0_LEN        23
#define DX_M1_DMA_STATUS_RESERVED0_OFFSET     9
#define DX_M1_DMA_STATUS_RESERVED0_MASK_BITS  0x007fffff /* [22:0] 0b00000000011111111111111111111111 */
#define DX_M1_DMA_STATUS_RESERVED0_MASK       0xfffffe00 /* [31:9] 0b11111111111111111111111000000000 */
#define READ_MASK_DX_M1_DMA_STATUS_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_STATUS, DX_M1_DMA_STATUS_RESERVED0_MASK)
#define READ_DX_M1_DMA_STATUS_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_STATUS, DX_M1_DMA_STATUS_RESERVED0_MASK, DX_M1_DMA_STATUS_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_STATUS_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_STATUS, bits, DX_M1_DMA_STATUS_RESERVED0_MASK, DX_M1_DMA_STATUS_RESERVED0_OFFSET)

/* DX_M1_DMA.SFR_BASE_ADDR_LOW (Category: DMA) */
#define READ_DX_M1_DMA_SFR_BASE_ADDR_LOW(base) dxrt::readl((void*)base, REG_DX_M1_DMA_SFR_BASE_ADDR_LOW)
#define WRITE_DX_M1_DMA_SFR_BASE_ADDR_LOW(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_SFR_BASE_ADDR_LOW, val)
/* DX_M1_DMA.SFR_BASE_ADDR_LOW.SFR_BASE_ADDR_LOW (Category: DMA) */
#define DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW_LEN        32
#define DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW_OFFSET     0
#define DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_BASE_ADDR_LOW, DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW_MASK)
#define READ_DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_BASE_ADDR_LOW, DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW_MASK, DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW_OFFSET)
#define WRITE_DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_BASE_ADDR_LOW, bits, DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW_MASK, DX_M1_DMA_SFR_BASE_ADDR_LOW_SFR_BASE_ADDR_LOW_OFFSET)

/* DX_M1_DMA.SFR_BASE_ADDR_HIGH (Category: DMA) */
#define READ_DX_M1_DMA_SFR_BASE_ADDR_HIGH(base) dxrt::readl((void*)base, REG_DX_M1_DMA_SFR_BASE_ADDR_HIGH)
#define WRITE_DX_M1_DMA_SFR_BASE_ADDR_HIGH(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_SFR_BASE_ADDR_HIGH, val)
/* DX_M1_DMA.SFR_BASE_ADDR_HIGH.SFR_BASE_ADDR_HIGH (Category: DMA) */
#define DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH_LEN        32
#define DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH_OFFSET     0
#define DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_BASE_ADDR_HIGH, DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH_MASK)
#define READ_DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_BASE_ADDR_HIGH, DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH_MASK, DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH_OFFSET)
#define WRITE_DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_BASE_ADDR_HIGH, bits, DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH_MASK, DX_M1_DMA_SFR_BASE_ADDR_HIGH_SFR_BASE_ADDR_HIGH_OFFSET)

/* DX_M1_DMA.SFR_ADDR_OFFS (Category: DMA) */
#define READ_DX_M1_DMA_SFR_ADDR_OFFS(base) dxrt::readl((void*)base, REG_DX_M1_DMA_SFR_ADDR_OFFS)
#define WRITE_DX_M1_DMA_SFR_ADDR_OFFS(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_SFR_ADDR_OFFS, val)
/* DX_M1_DMA.SFR_ADDR_OFFS.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0_LEN        4
#define DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0_OFFSET     0
#define DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0_MASK       0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define READ_MASK_DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_ADDR_OFFS, DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0_MASK)
#define READ_DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_ADDR_OFFS, DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0_MASK, DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_ADDR_OFFS, bits, DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0_MASK, DX_M1_DMA_SFR_ADDR_OFFS_RESERVED0_OFFSET)

/* DX_M1_DMA.SFR_ADDR_OFFS.SFR_ADDR_OFFS (Category: DMA) */
#define DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS_LEN        28
#define DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS_OFFSET     4
#define DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS_MASK_BITS  0x0fffffff /* [27:0] 0b00001111111111111111111111111111 */
#define DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS_MASK       0xfffffff0 /* [31:4] 0b11111111111111111111111111110000 */
#define READ_MASK_DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_ADDR_OFFS, DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS_MASK)
#define READ_DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_ADDR_OFFS, DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS_MASK, DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS_OFFSET)
#define WRITE_DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_ADDR_OFFS, bits, DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS_MASK, DX_M1_DMA_SFR_ADDR_OFFS_SFR_ADDR_OFFS_OFFSET)

/* DX_M1_DMA.SFR_START_ADDR (Category: DMA) */
#define READ_DX_M1_DMA_SFR_START_ADDR(base) dxrt::readl((void*)base, REG_DX_M1_DMA_SFR_START_ADDR)
#define WRITE_DX_M1_DMA_SFR_START_ADDR(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_SFR_START_ADDR, val)
/* DX_M1_DMA.SFR_START_ADDR.SFR_START_ADDR (Category: DMA) */
#define DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR_LEN        16
#define DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR_OFFSET     0
#define DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_START_ADDR, DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR_MASK)
#define READ_DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_START_ADDR, DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR_MASK, DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR_OFFSET)
#define WRITE_DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_START_ADDR, bits, DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR_MASK, DX_M1_DMA_SFR_START_ADDR_SFR_START_ADDR_OFFSET)

/* DX_M1_DMA.SFR_START_ADDR.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_SFR_START_ADDR_RESERVED0_LEN        16
#define DX_M1_DMA_SFR_START_ADDR_RESERVED0_OFFSET     16
#define DX_M1_DMA_SFR_START_ADDR_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1_DMA_SFR_START_ADDR_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1_DMA_SFR_START_ADDR_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_START_ADDR, DX_M1_DMA_SFR_START_ADDR_RESERVED0_MASK)
#define READ_DX_M1_DMA_SFR_START_ADDR_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_START_ADDR, DX_M1_DMA_SFR_START_ADDR_RESERVED0_MASK, DX_M1_DMA_SFR_START_ADDR_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_SFR_START_ADDR_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_START_ADDR, bits, DX_M1_DMA_SFR_START_ADDR_RESERVED0_MASK, DX_M1_DMA_SFR_START_ADDR_RESERVED0_OFFSET)

/* DX_M1_DMA.SFR_MOVE_CNT (Category: DMA) */
#define READ_DX_M1_DMA_SFR_MOVE_CNT(base) dxrt::readl((void*)base, REG_DX_M1_DMA_SFR_MOVE_CNT)
#define WRITE_DX_M1_DMA_SFR_MOVE_CNT(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_SFR_MOVE_CNT, val)
/* DX_M1_DMA.SFR_MOVE_CNT.RESERVED1 (Category: DMA) */
#define DX_M1_DMA_SFR_MOVE_CNT_RESERVED1_LEN        2
#define DX_M1_DMA_SFR_MOVE_CNT_RESERVED1_OFFSET     0
#define DX_M1_DMA_SFR_MOVE_CNT_RESERVED1_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1_DMA_SFR_MOVE_CNT_RESERVED1_MASK       0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define READ_MASK_DX_M1_DMA_SFR_MOVE_CNT_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_MOVE_CNT, DX_M1_DMA_SFR_MOVE_CNT_RESERVED1_MASK)
#define READ_DX_M1_DMA_SFR_MOVE_CNT_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_MOVE_CNT, DX_M1_DMA_SFR_MOVE_CNT_RESERVED1_MASK, DX_M1_DMA_SFR_MOVE_CNT_RESERVED1_OFFSET)
#define WRITE_DX_M1_DMA_SFR_MOVE_CNT_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_MOVE_CNT, bits, DX_M1_DMA_SFR_MOVE_CNT_RESERVED1_MASK, DX_M1_DMA_SFR_MOVE_CNT_RESERVED1_OFFSET)

/* DX_M1_DMA.SFR_MOVE_CNT.SFR_MOVE_CNT (Category: DMA) */
#define DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT_LEN        12
#define DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT_OFFSET     2
#define DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT_MASK       0x00003ffc /* [13:2] 0b00000000000000000011111111111100 */
#define READ_MASK_DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_MOVE_CNT, DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT_MASK)
#define READ_DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_MOVE_CNT, DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT_MASK, DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT_OFFSET)
#define WRITE_DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_MOVE_CNT, bits, DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT_MASK, DX_M1_DMA_SFR_MOVE_CNT_SFR_MOVE_CNT_OFFSET)

/* DX_M1_DMA.SFR_MOVE_CNT.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_SFR_MOVE_CNT_RESERVED0_LEN        18
#define DX_M1_DMA_SFR_MOVE_CNT_RESERVED0_OFFSET     14
#define DX_M1_DMA_SFR_MOVE_CNT_RESERVED0_MASK_BITS  0x0003ffff /* [17:0] 0b00000000000000111111111111111111 */
#define DX_M1_DMA_SFR_MOVE_CNT_RESERVED0_MASK       0xffffc000 /* [31:14] 0b11111111111111111100000000000000 */
#define READ_MASK_DX_M1_DMA_SFR_MOVE_CNT_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_MOVE_CNT, DX_M1_DMA_SFR_MOVE_CNT_RESERVED0_MASK)
#define READ_DX_M1_DMA_SFR_MOVE_CNT_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_MOVE_CNT, DX_M1_DMA_SFR_MOVE_CNT_RESERVED0_MASK, DX_M1_DMA_SFR_MOVE_CNT_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_SFR_MOVE_CNT_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_MOVE_CNT, bits, DX_M1_DMA_SFR_MOVE_CNT_RESERVED0_MASK, DX_M1_DMA_SFR_MOVE_CNT_RESERVED0_OFFSET)

/* DX_M1_DMA.SFR_ID (Category: DMA) */
#define READ_DX_M1_DMA_SFR_ID(base) dxrt::readl((void*)base, REG_DX_M1_DMA_SFR_ID)
#define WRITE_DX_M1_DMA_SFR_ID(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_SFR_ID, val)
/* DX_M1_DMA.SFR_ID.SFR_ID (Category: DMA) */
#define DX_M1_DMA_SFR_ID_SFR_ID_LEN        16
#define DX_M1_DMA_SFR_ID_SFR_ID_OFFSET     0
#define DX_M1_DMA_SFR_ID_SFR_ID_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1_DMA_SFR_ID_SFR_ID_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1_DMA_SFR_ID_SFR_ID(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_ID, DX_M1_DMA_SFR_ID_SFR_ID_MASK)
#define READ_DX_M1_DMA_SFR_ID_SFR_ID(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_ID, DX_M1_DMA_SFR_ID_SFR_ID_MASK, DX_M1_DMA_SFR_ID_SFR_ID_OFFSET)
#define WRITE_DX_M1_DMA_SFR_ID_SFR_ID(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_ID, bits, DX_M1_DMA_SFR_ID_SFR_ID_MASK, DX_M1_DMA_SFR_ID_SFR_ID_OFFSET)

/* DX_M1_DMA.SFR_ID.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_SFR_ID_RESERVED0_LEN        16
#define DX_M1_DMA_SFR_ID_RESERVED0_OFFSET     16
#define DX_M1_DMA_SFR_ID_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1_DMA_SFR_ID_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1_DMA_SFR_ID_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_ID, DX_M1_DMA_SFR_ID_RESERVED0_MASK)
#define READ_DX_M1_DMA_SFR_ID_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_ID, DX_M1_DMA_SFR_ID_RESERVED0_MASK, DX_M1_DMA_SFR_ID_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_SFR_ID_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_ID, bits, DX_M1_DMA_SFR_ID_RESERVED0_MASK, DX_M1_DMA_SFR_ID_RESERVED0_OFFSET)

/* DX_M1_DMA.SFR_DMA_STATUS (Category: DMA) */
#define READ_DX_M1_DMA_SFR_DMA_STATUS(base) dxrt::readl((void*)base, REG_DX_M1_DMA_SFR_DMA_STATUS)
#define WRITE_DX_M1_DMA_SFR_DMA_STATUS(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_SFR_DMA_STATUS, val)
/* DX_M1_DMA.SFR_DMA_STATUS.SFR_DMA_DONE_STATUS (Category: DMA) */
#define DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS_LEN        1
#define DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS_OFFSET     0
#define DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_DMA_STATUS, DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS_MASK)
#define READ_DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_DMA_STATUS, DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS_MASK, DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS_OFFSET)
#define WRITE_DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_DMA_STATUS, bits, DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS_MASK, DX_M1_DMA_SFR_DMA_STATUS_SFR_DMA_DONE_STATUS_OFFSET)

/* DX_M1_DMA.SFR_DMA_STATUS.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_SFR_DMA_STATUS_RESERVED0_LEN        31
#define DX_M1_DMA_SFR_DMA_STATUS_RESERVED0_OFFSET     1
#define DX_M1_DMA_SFR_DMA_STATUS_RESERVED0_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1_DMA_SFR_DMA_STATUS_RESERVED0_MASK       0xfffffffe /* [31:1] 0b11111111111111111111111111111110 */
#define READ_MASK_DX_M1_DMA_SFR_DMA_STATUS_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_SFR_DMA_STATUS, DX_M1_DMA_SFR_DMA_STATUS_RESERVED0_MASK)
#define READ_DX_M1_DMA_SFR_DMA_STATUS_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_SFR_DMA_STATUS, DX_M1_DMA_SFR_DMA_STATUS_RESERVED0_MASK, DX_M1_DMA_SFR_DMA_STATUS_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_SFR_DMA_STATUS_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_SFR_DMA_STATUS, bits, DX_M1_DMA_SFR_DMA_STATUS_RESERVED0_MASK, DX_M1_DMA_SFR_DMA_STATUS_RESERVED0_OFFSET)

/* DX_M1_DMA.DMA_CTRL (Category: DMA) */
#define READ_DX_M1_DMA_DMA_CTRL(base) dxrt::readl((void*)base, REG_DX_M1_DMA_DMA_CTRL)
#define WRITE_DX_M1_DMA_DMA_CTRL(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_DMA_CTRL, val)
/* DX_M1_DMA.DMA_CTRL.DMA_CTRL_ENABLE (Category: DMA) */
#define DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE_LEN        3
#define DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE_OFFSET     0
#define DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE_MASK       0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define READ_MASK_DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_DMA_CTRL, DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE_MASK)
#define READ_DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_DMA_CTRL, DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE_MASK, DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE_OFFSET)
#define WRITE_DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_DMA_CTRL, bits, DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE_MASK, DX_M1_DMA_DMA_CTRL_DMA_CTRL_ENABLE_OFFSET)

/* DX_M1_DMA.DMA_CTRL.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_DMA_CTRL_RESERVED0_LEN        29
#define DX_M1_DMA_DMA_CTRL_RESERVED0_OFFSET     3
#define DX_M1_DMA_DMA_CTRL_RESERVED0_MASK_BITS  0x1fffffff /* [28:0] 0b00011111111111111111111111111111 */
#define DX_M1_DMA_DMA_CTRL_RESERVED0_MASK       0xfffffff8 /* [31:3] 0b11111111111111111111111111111000 */
#define READ_MASK_DX_M1_DMA_DMA_CTRL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_DMA_CTRL, DX_M1_DMA_DMA_CTRL_RESERVED0_MASK)
#define READ_DX_M1_DMA_DMA_CTRL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_DMA_CTRL, DX_M1_DMA_DMA_CTRL_RESERVED0_MASK, DX_M1_DMA_DMA_CTRL_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_DMA_CTRL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_DMA_CTRL, bits, DX_M1_DMA_DMA_CTRL_RESERVED0_MASK, DX_M1_DMA_DMA_CTRL_RESERVED0_OFFSET)

/* DX_M1_DMA.FLUSH_STATUS (Category: DMA) */
#define READ_DX_M1_DMA_FLUSH_STATUS(base) dxrt::readl((void*)base, REG_DX_M1_DMA_FLUSH_STATUS)
#define WRITE_DX_M1_DMA_FLUSH_STATUS(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_FLUSH_STATUS, val)
/* DX_M1_DMA.FLUSH_STATUS.FLUSH_STATUS (Category: DMA) */
#define DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS_LEN        1
#define DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS_OFFSET     0
#define DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_FLUSH_STATUS, DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS_MASK)
#define READ_DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_FLUSH_STATUS, DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS_MASK, DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS_OFFSET)
#define WRITE_DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_FLUSH_STATUS, bits, DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS_MASK, DX_M1_DMA_FLUSH_STATUS_FLUSH_STATUS_OFFSET)

/* DX_M1_DMA.FLUSH_STATUS.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_FLUSH_STATUS_RESERVED0_LEN        31
#define DX_M1_DMA_FLUSH_STATUS_RESERVED0_OFFSET     1
#define DX_M1_DMA_FLUSH_STATUS_RESERVED0_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1_DMA_FLUSH_STATUS_RESERVED0_MASK       0xfffffffe /* [31:1] 0b11111111111111111111111111111110 */
#define READ_MASK_DX_M1_DMA_FLUSH_STATUS_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_FLUSH_STATUS, DX_M1_DMA_FLUSH_STATUS_RESERVED0_MASK)
#define READ_DX_M1_DMA_FLUSH_STATUS_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_FLUSH_STATUS, DX_M1_DMA_FLUSH_STATUS_RESERVED0_MASK, DX_M1_DMA_FLUSH_STATUS_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_FLUSH_STATUS_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_FLUSH_STATUS, bits, DX_M1_DMA_FLUSH_STATUS_RESERVED0_MASK, DX_M1_DMA_FLUSH_STATUS_RESERVED0_OFFSET)

/* DX_M1_DMA.AXI_CFG0 (Category: DMA) */
#define READ_DX_M1_DMA_AXI_CFG0(base) dxrt::readl((void*)base, REG_DX_M1_DMA_AXI_CFG0)
#define WRITE_DX_M1_DMA_AXI_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_AXI_CFG0, val)
/* DX_M1_DMA.AXI_CFG0.AXI_RDMA_BURST_LENGTH (Category: DMA) */
#define DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH_LEN        9
#define DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH_OFFSET     0
#define DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH_MASK_BITS  0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH_MASK       0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define READ_MASK_DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_CFG0, DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH_MASK)
#define READ_DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_CFG0, DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH_MASK, DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH_OFFSET)
#define WRITE_DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_CFG0, bits, DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH_MASK, DX_M1_DMA_AXI_CFG0_AXI_RDMA_BURST_LENGTH_OFFSET)

/* DX_M1_DMA.AXI_CFG0.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_AXI_CFG0_RESERVED0_LEN        23
#define DX_M1_DMA_AXI_CFG0_RESERVED0_OFFSET     9
#define DX_M1_DMA_AXI_CFG0_RESERVED0_MASK_BITS  0x007fffff /* [22:0] 0b00000000011111111111111111111111 */
#define DX_M1_DMA_AXI_CFG0_RESERVED0_MASK       0xfffffe00 /* [31:9] 0b11111111111111111111111000000000 */
#define READ_MASK_DX_M1_DMA_AXI_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_CFG0, DX_M1_DMA_AXI_CFG0_RESERVED0_MASK)
#define READ_DX_M1_DMA_AXI_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_CFG0, DX_M1_DMA_AXI_CFG0_RESERVED0_MASK, DX_M1_DMA_AXI_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_AXI_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_CFG0, bits, DX_M1_DMA_AXI_CFG0_RESERVED0_MASK, DX_M1_DMA_AXI_CFG0_RESERVED0_OFFSET)

/* DX_M1_DMA.AXI_CFG1 (Category: DMA) */
#define READ_DX_M1_DMA_AXI_CFG1(base) dxrt::readl((void*)base, REG_DX_M1_DMA_AXI_CFG1)
#define WRITE_DX_M1_DMA_AXI_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_AXI_CFG1, val)
/* DX_M1_DMA.AXI_CFG1.AXI_WDMA_BURST_LENGTH (Category: DMA) */
#define DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH_LEN        9
#define DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH_OFFSET     0
#define DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH_MASK_BITS  0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH_MASK       0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define READ_MASK_DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_CFG1, DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH_MASK)
#define READ_DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_CFG1, DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH_MASK, DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH_OFFSET)
#define WRITE_DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_CFG1, bits, DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH_MASK, DX_M1_DMA_AXI_CFG1_AXI_WDMA_BURST_LENGTH_OFFSET)

/* DX_M1_DMA.AXI_CFG1.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_AXI_CFG1_RESERVED0_LEN        23
#define DX_M1_DMA_AXI_CFG1_RESERVED0_OFFSET     9
#define DX_M1_DMA_AXI_CFG1_RESERVED0_MASK_BITS  0x007fffff /* [22:0] 0b00000000011111111111111111111111 */
#define DX_M1_DMA_AXI_CFG1_RESERVED0_MASK       0xfffffe00 /* [31:9] 0b11111111111111111111111000000000 */
#define READ_MASK_DX_M1_DMA_AXI_CFG1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_CFG1, DX_M1_DMA_AXI_CFG1_RESERVED0_MASK)
#define READ_DX_M1_DMA_AXI_CFG1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_CFG1, DX_M1_DMA_AXI_CFG1_RESERVED0_MASK, DX_M1_DMA_AXI_CFG1_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_AXI_CFG1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_CFG1, bits, DX_M1_DMA_AXI_CFG1_RESERVED0_MASK, DX_M1_DMA_AXI_CFG1_RESERVED0_OFFSET)

/* DX_M1_DMA.AXI_CFG2 (Category: DMA) */
#define READ_DX_M1_DMA_AXI_CFG2(base) dxrt::readl((void*)base, REG_DX_M1_DMA_AXI_CFG2)
#define WRITE_DX_M1_DMA_AXI_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_AXI_CFG2, val)
/* DX_M1_DMA.AXI_CFG2.AXI_RDMA_MO (Category: DMA) */
#define DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO_LEN        4
#define DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO_OFFSET     0
#define DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO_MASK       0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define READ_MASK_DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_CFG2, DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO_MASK)
#define READ_DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_CFG2, DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO_MASK, DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO_OFFSET)
#define WRITE_DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_CFG2, bits, DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO_MASK, DX_M1_DMA_AXI_CFG2_AXI_RDMA_MO_OFFSET)

/* DX_M1_DMA.AXI_CFG2.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_AXI_CFG2_RESERVED0_LEN        28
#define DX_M1_DMA_AXI_CFG2_RESERVED0_OFFSET     4
#define DX_M1_DMA_AXI_CFG2_RESERVED0_MASK_BITS  0x0fffffff /* [27:0] 0b00001111111111111111111111111111 */
#define DX_M1_DMA_AXI_CFG2_RESERVED0_MASK       0xfffffff0 /* [31:4] 0b11111111111111111111111111110000 */
#define READ_MASK_DX_M1_DMA_AXI_CFG2_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_CFG2, DX_M1_DMA_AXI_CFG2_RESERVED0_MASK)
#define READ_DX_M1_DMA_AXI_CFG2_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_CFG2, DX_M1_DMA_AXI_CFG2_RESERVED0_MASK, DX_M1_DMA_AXI_CFG2_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_AXI_CFG2_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_CFG2, bits, DX_M1_DMA_AXI_CFG2_RESERVED0_MASK, DX_M1_DMA_AXI_CFG2_RESERVED0_OFFSET)

/* DX_M1_DMA.AXI_CFG3 (Category: DMA) */
#define READ_DX_M1_DMA_AXI_CFG3(base) dxrt::readl((void*)base, REG_DX_M1_DMA_AXI_CFG3)
#define WRITE_DX_M1_DMA_AXI_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_AXI_CFG3, val)
/* DX_M1_DMA.AXI_CFG3.AXI_WDMA_MO (Category: DMA) */
#define DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO_LEN        4
#define DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO_OFFSET     0
#define DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO_MASK       0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define READ_MASK_DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_CFG3, DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO_MASK)
#define READ_DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_CFG3, DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO_MASK, DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO_OFFSET)
#define WRITE_DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_CFG3, bits, DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO_MASK, DX_M1_DMA_AXI_CFG3_AXI_WDMA_MO_OFFSET)

/* DX_M1_DMA.AXI_CFG3.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_AXI_CFG3_RESERVED0_LEN        28
#define DX_M1_DMA_AXI_CFG3_RESERVED0_OFFSET     4
#define DX_M1_DMA_AXI_CFG3_RESERVED0_MASK_BITS  0x0fffffff /* [27:0] 0b00001111111111111111111111111111 */
#define DX_M1_DMA_AXI_CFG3_RESERVED0_MASK       0xfffffff0 /* [31:4] 0b11111111111111111111111111110000 */
#define READ_MASK_DX_M1_DMA_AXI_CFG3_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_CFG3, DX_M1_DMA_AXI_CFG3_RESERVED0_MASK)
#define READ_DX_M1_DMA_AXI_CFG3_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_CFG3, DX_M1_DMA_AXI_CFG3_RESERVED0_MASK, DX_M1_DMA_AXI_CFG3_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_AXI_CFG3_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_CFG3, bits, DX_M1_DMA_AXI_CFG3_RESERVED0_MASK, DX_M1_DMA_AXI_CFG3_RESERVED0_OFFSET)

/* DX_M1_DMA.AXI_CFG4 (Category: DMA) */
#define READ_DX_M1_DMA_AXI_CFG4(base) dxrt::readl((void*)base, REG_DX_M1_DMA_AXI_CFG4)
#define WRITE_DX_M1_DMA_AXI_CFG4(base, val) dxrt::writel((void*)base, REG_DX_M1_DMA_AXI_CFG4, val)
/* DX_M1_DMA.AXI_CFG4.AXI_4KB_BOUNDARY (Category: DMA) */
#define DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY_LEN        1
#define DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY_OFFSET     0
#define DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_CFG4, DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY_MASK)
#define READ_DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_CFG4, DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY_MASK, DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY_OFFSET)
#define WRITE_DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_CFG4, bits, DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY_MASK, DX_M1_DMA_AXI_CFG4_AXI_4KB_BOUNDARY_OFFSET)

/* DX_M1_DMA.AXI_CFG4.RESERVED0 (Category: DMA) */
#define DX_M1_DMA_AXI_CFG4_RESERVED0_LEN        31
#define DX_M1_DMA_AXI_CFG4_RESERVED0_OFFSET     1
#define DX_M1_DMA_AXI_CFG4_RESERVED0_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1_DMA_AXI_CFG4_RESERVED0_MASK       0xfffffffe /* [31:1] 0b11111111111111111111111111111110 */
#define READ_MASK_DX_M1_DMA_AXI_CFG4_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1_DMA_AXI_CFG4, DX_M1_DMA_AXI_CFG4_RESERVED0_MASK)
#define READ_DX_M1_DMA_AXI_CFG4_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1_DMA_AXI_CFG4, DX_M1_DMA_AXI_CFG4_RESERVED0_MASK, DX_M1_DMA_AXI_CFG4_RESERVED0_OFFSET)
#define WRITE_DX_M1_DMA_AXI_CFG4_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1_DMA_AXI_CFG4, bits, DX_M1_DMA_AXI_CFG4_RESERVED0_MASK, DX_M1_DMA_AXI_CFG4_RESERVED0_OFFSET)


int Verify_Dx_m1_dma_Registers(dxDX_M1_DMA_t* base);

} /* namespace dxrt */
