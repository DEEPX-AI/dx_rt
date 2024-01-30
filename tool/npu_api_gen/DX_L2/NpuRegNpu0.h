/* Automatically generated from DX_L2.xlsx
 * by Deepx Neural Network Generator v1.0
 */
#pragma once

#include <stdint.h>
#include "dxrt/npuif/npuif.h"

namespace dxrt{
extern bool logNpuRegAccess;

/* Address */
#define REG_DX_L2_NPU0_OFFSET 0x200
#define REG_DX_L2_NPU0_DATA_CFG0    0x00000000 /* 0 */
#define REG_DX_L2_NPU0_DATA_CFG1    0x00000004 /* 4 */
#define REG_DX_L2_NPU0_IM2COL    0x00000008 /* 8 */
#define REG_DX_L2_NPU0_CTRL    0x0000000c /* 12 */
#define REG_DX_L2_NPU0_MISC    0x00000010 /* 16 */
#define REG_DX_L2_NPU0_ACTIVE    0x00000014 /* 20 */
#define REG_DX_L2_NPU0_RSVD0    0x00000018 /* 24 */
#define REG_DX_L2_NPU0_SFU_CTRL    0x0000001c /* 28 */
#define REG_DX_L2_NPU0_SFU_CLIP_MAX    0x00000020 /* 32 */
#define REG_DX_L2_NPU0_SFU_CLIP_MIN    0x00000024 /* 36 */
#define REG_DX_L2_NPU0_SFU_COEFF0    0x00000028 /* 40 */
#define REG_DX_L2_NPU0_SFU_COEFF1    0x0000002c /* 44 */
#define REG_DX_L2_NPU0_SFU_COEFF2    0x00000030 /* 48 */
#define REG_DX_L2_NPU0_SFU_COEFF3    0x00000034 /* 52 */
#define REG_DX_L2_NPU0_SFU_COEFF4    0x00000038 /* 56 */
#define REG_DX_L2_NPU0_SFU_COEFF5    0x0000003c /* 60 */
#define REG_DX_L2_NPU0_SFU_COEFF6    0x00000040 /* 64 */
#define REG_DX_L2_NPU0_SFU_COEFF7    0x00000044 /* 68 */
#define REG_DX_L2_NPU0_SFU_COEFF8    0x00000048 /* 72 */
#define REG_DX_L2_NPU0_SFU_COEFF9    0x0000004c /* 76 */
#define REG_DX_L2_NPU0_SFU_COEFF10    0x00000050 /* 80 */
#define REG_DX_L2_NPU0_SFU_COEFF11    0x00000054 /* 84 */
#define REG_DX_L2_NPU0_SFU_COEFF12    0x00000058 /* 88 */
#define REG_DX_L2_NPU0_SFU_COEFF13    0x0000005c /* 92 */
#define REG_DX_L2_NPU0_SFU_COEFF14    0x00000060 /* 96 */
#define REG_DX_L2_NPU0_SFU_COEFF15    0x00000064 /* 100 */
#define REG_DX_L2_NPU0_SFU_COEFF16    0x00000068 /* 104 */
#define REG_DX_L2_NPU0_SFU_COEFF17    0x0000006c /* 108 */
#define REG_DX_L2_NPU0_SFU_COEFF18    0x00000070 /* 112 */
#define REG_DX_L2_NPU0_SFU_COEFF19    0x00000074 /* 116 */
#define REG_DX_L2_NPU0_SFU_COEFF20    0x00000078 /* 120 */
#define REG_DX_L2_NPU0_SFU_COEFF21    0x0000007c /* 124 */
#define REG_DX_L2_NPU0_SFU_COEFF22    0x00000080 /* 128 */
#define REG_DX_L2_NPU0_SFU_COEFF23    0x00000084 /* 132 */
#define REG_DX_L2_NPU0_SFU_COEFF24    0x00000088 /* 136 */
#define REG_DX_L2_NPU0_SFU_COEFF25    0x0000008c /* 140 */
#define REG_DX_L2_NPU0_SFU_COEFF26    0x00000090 /* 144 */
#define REG_DX_L2_NPU0_SFU_COEFF27    0x00000094 /* 148 */
#define REG_DX_L2_NPU0_SFU_COEFF28    0x00000098 /* 152 */
#define REG_DX_L2_NPU0_SFU_COEFF29    0x0000009c /* 156 */
#define REG_DX_L2_NPU0_SFU_COEFF30    0x000000a0 /* 160 */
#define REG_DX_L2_NPU0_SFU_COEFF31    0x000000a4 /* 164 */
#define REG_DX_L2_NPU0_SFU_COEFF32    0x000000a8 /* 168 */
#define REG_DX_L2_NPU0_SFU_COEFF33    0x000000ac /* 172 */
#define REG_DX_L2_NPU0_SFU_COEFF34    0x000000b0 /* 176 */
#define REG_DX_L2_NPU0_SFU_COEFF35    0x000000b4 /* 180 */
#define REG_DX_L2_NPU0_SFU_COEFF36    0x000000b8 /* 184 */
#define REG_DX_L2_NPU0_SFU_COEFF37    0x000000bc /* 188 */
#define REG_DX_L2_NPU0_SFU_COEFF38    0x000000c0 /* 192 */
#define REG_DX_L2_NPU0_SFU_COEFF39    0x000000c4 /* 196 */
#define REG_DX_L2_NPU0_SFU_COEFF40    0x000000c8 /* 200 */
#define REG_DX_L2_NPU0_SFU_COEFF41    0x000000cc /* 204 */
#define REG_DX_L2_NPU0_SFU_COEFF42    0x000000d0 /* 208 */
#define REG_DX_L2_NPU0_SFU_COEFF43    0x000000d4 /* 212 */
#define REG_DX_L2_NPU0_SFU_COEFF44    0x000000d8 /* 216 */
#define REG_DX_L2_NPU0_SFU_COEFF45    0x000000dc /* 220 */
#define REG_DX_L2_NPU0_SFU_COEFF46    0x000000e0 /* 224 */
#define REG_DX_L2_NPU0_SFU_COEFF47    0x000000e4 /* 228 */
#define REG_DX_L2_NPU0_SFU_COEFF48    0x000000e8 /* 232 */
#define REG_DX_L2_NPU0_SFU_COEFF49    0x000000ec /* 236 */
#define REG_DX_L2_NPU0_SFU_COEFF50    0x000000f0 /* 240 */
#define REG_DX_L2_NPU0_SFU_COEFF51    0x000000f4 /* 244 */
#define REG_DX_L2_NPU0_SFU_COEFF52    0x000000f8 /* 248 */
#define REG_DX_L2_NPU0_SFU_COEFF53    0x000000fc /* 252 */
#define REG_DX_L2_NPU0_SFU_COEFF54    0x00000100 /* 256 */
#define REG_DX_L2_NPU0_RSVD1    0x00000104 /* 260 */
#define REG_DX_L2_NPU0_RSVD2    0x00000108 /* 264 */
#define REG_DX_L2_NPU0_PE0_IMG2COL0    0x0000010c /* 268 */
#define REG_DX_L2_NPU0_PE0_IMG2COL1    0x00000110 /* 272 */
#define REG_DX_L2_NPU0_PE0_FMT    0x00000114 /* 276 */
#define REG_DX_L2_NPU0_PE0_FMT_CFG0    0x00000118 /* 280 */
#define REG_DX_L2_NPU0_PE0_ADDR_GEN0    0x0000011c /* 284 */
#define REG_DX_L2_NPU0_PE0_ADDR_GEN1    0x00000120 /* 288 */
#define REG_DX_L2_NPU0_PE0_ADDR_GEN2    0x00000124 /* 292 */
#define REG_DX_L2_NPU0_PE0_ADDR_GEN3    0x00000128 /* 296 */
#define REG_DX_L2_NPU0_PE0_CTRL    0x0000012c /* 300 */
#define REG_DX_L2_NPU0_PE0_CFG0    0x00000130 /* 304 */
#define REG_DX_L2_NPU0_PE0_CFG1    0x00000134 /* 308 */
#define REG_DX_L2_NPU0_PE0_CFG2    0x00000138 /* 312 */
#define REG_DX_L2_NPU0_PE0_CFG3    0x0000013c /* 316 */
#define REG_DX_L2_NPU0_PE0_CFG4    0x00000140 /* 320 */
#define REG_DX_L2_NPU0_PE0_CFG5    0x00000144 /* 324 */
#define REG_DX_L2_NPU0_PE0_CFG6    0x00000148 /* 328 */
#define REG_DX_L2_NPU0_PE0_CFG7    0x0000014c /* 332 */
#define REG_DX_L2_NPU0_PE0_CFG8    0x00000150 /* 336 */
#define REG_DX_L2_NPU0_PE0_CFG9    0x00000154 /* 340 */
#define REG_DX_L2_NPU0_PE0_CFG10    0x00000158 /* 344 */
#define REG_DX_L2_NPU0_PE0_CFG11    0x0000015c /* 348 */
#define REG_DX_L2_NPU0_PE0_CFG12    0x00000160 /* 352 */
#define REG_DX_L2_NPU0_PE0_CFG13    0x00000164 /* 356 */
#define REG_DX_L2_NPU0_PE0_CFG14    0x00000168 /* 360 */
#define REG_DX_L2_NPU0_PE0_CFG15    0x0000016c /* 364 */
#define REG_DX_L2_NPU0_PE0_CFG16    0x00000170 /* 368 */
#define REG_DX_L2_NPU0_PE0_CFG17    0x00000174 /* 372 */
#define REG_DX_L2_NPU0_PE0_CFG18    0x00000178 /* 376 */
#define REG_DX_L2_NPU0_PE0_CFG19    0x0000017c /* 380 */
#define REG_DX_L2_NPU0_PE0_CFG20    0x00000180 /* 384 */
#define REG_DX_L2_NPU0_PE0_CFG21    0x00000184 /* 388 */
#define REG_DX_L2_NPU0_PE0_CFG22    0x00000188 /* 392 */
#define REG_DX_L2_NPU0_PE0_CFG23    0x0000018c /* 396 */
#define REG_DX_L2_NPU0_PE0_CFG24    0x00000190 /* 400 */
#define REG_DX_L2_NPU0_PE0_CFG25    0x00000194 /* 404 */
#define REG_DX_L2_NPU0_PE0_VMEM_CFG0    0x00000198 /* 408 */
#define REG_DX_L2_NPU0_PE0_SMEM_CFG0    0x0000019c /* 412 */
#define REG_DX_L2_NPU0_PE1_IMG2COL0    0x000001a0 /* 416 */
#define REG_DX_L2_NPU0_PE1_IMG2COL1    0x000001a4 /* 420 */
#define REG_DX_L2_NPU0_PE1_FMT    0x000001a8 /* 424 */
#define REG_DX_L2_NPU0_PE1_FMT_CFG0    0x000001ac /* 428 */
#define REG_DX_L2_NPU0_PE1_ADDR_GEN0    0x000001b0 /* 432 */
#define REG_DX_L2_NPU0_PE1_ADDR_GEN1    0x000001b4 /* 436 */
#define REG_DX_L2_NPU0_PE1_ADDR_GEN2    0x000001b8 /* 440 */
#define REG_DX_L2_NPU0_PE1_ADDR_GEN3    0x000001bc /* 444 */
#define REG_DX_L2_NPU0_PE1_CTRL    0x000001c0 /* 448 */
#define REG_DX_L2_NPU0_PE1_CFG0    0x000001c4 /* 452 */
#define REG_DX_L2_NPU0_PE1_CFG1    0x000001c8 /* 456 */
#define REG_DX_L2_NPU0_PE1_CFG2    0x000001cc /* 460 */
#define REG_DX_L2_NPU0_PE1_CFG3    0x000001d0 /* 464 */
#define REG_DX_L2_NPU0_PE1_CFG4    0x000001d4 /* 468 */
#define REG_DX_L2_NPU0_PE1_CFG5    0x000001d8 /* 472 */
#define REG_DX_L2_NPU0_PE1_CFG6    0x000001dc /* 476 */
#define REG_DX_L2_NPU0_PE1_CFG7    0x000001e0 /* 480 */
#define REG_DX_L2_NPU0_PE1_CFG8    0x000001e4 /* 484 */
#define REG_DX_L2_NPU0_PE1_CFG9    0x000001e8 /* 488 */
#define REG_DX_L2_NPU0_PE1_CFG10    0x000001ec /* 492 */
#define REG_DX_L2_NPU0_PE1_CFG11    0x000001f0 /* 496 */
#define REG_DX_L2_NPU0_PE1_CFG12    0x000001f4 /* 500 */
#define REG_DX_L2_NPU0_PE1_CFG13    0x000001f8 /* 504 */
#define REG_DX_L2_NPU0_PE1_CFG14    0x000001fc /* 508 */
#define REG_DX_L2_NPU0_PE1_CFG15    0x00000200 /* 512 */
#define REG_DX_L2_NPU0_PE1_CFG16    0x00000204 /* 516 */
#define REG_DX_L2_NPU0_PE1_CFG17    0x00000208 /* 520 */
#define REG_DX_L2_NPU0_PE1_CFG18    0x0000020c /* 524 */
#define REG_DX_L2_NPU0_PE1_CFG19    0x00000210 /* 528 */
#define REG_DX_L2_NPU0_PE1_CFG20    0x00000214 /* 532 */
#define REG_DX_L2_NPU0_PE1_CFG21    0x00000218 /* 536 */
#define REG_DX_L2_NPU0_PE1_CFG22    0x0000021c /* 540 */
#define REG_DX_L2_NPU0_PE1_CFG23    0x00000220 /* 544 */
#define REG_DX_L2_NPU0_PE1_CFG24    0x00000224 /* 548 */
#define REG_DX_L2_NPU0_PE1_CFG25    0x00000228 /* 552 */
#define REG_DX_L2_NPU0_PE1_VMEM_CFG0    0x0000022c /* 556 */
#define REG_DX_L2_NPU0_PE1_SMEM_CFG0    0x00000230 /* 560 */
#define REG_DX_L2_NPU0_PE0_DMA_CTRL    0x00000234 /* 564 */
#define REG_DX_L2_NPU0_PE0_DMA_BN_CFG0    0x00000238 /* 568 */
#define REG_DX_L2_NPU0_PE0_DMA_BN_CFG1    0x0000023c /* 572 */
#define REG_DX_L2_NPU0_PE0_DMA_BN_CFG2    0x00000240 /* 576 */
#define REG_DX_L2_NPU0_PE0_DMA_BN_CFG3    0x00000244 /* 580 */
#define REG_DX_L2_NPU0_PE0_DMA_B_CFG0    0x00000248 /* 584 */
#define REG_DX_L2_NPU0_PE0_DMA_B_CFG1    0x0000024c /* 588 */
#define REG_DX_L2_NPU0_PE0_DMA_B_CFG2    0x00000250 /* 592 */
#define REG_DX_L2_NPU0_PE0_DMA_B_CFG3    0x00000254 /* 596 */
#define REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0    0x00000258 /* 600 */
#define REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1    0x0000025c /* 604 */
#define REG_DX_L2_NPU0_PE0_DMA_MUL_CFG2    0x00000260 /* 608 */
#define REG_DX_L2_NPU0_PE0_DMA_MUL_CFG3    0x00000264 /* 612 */
#define REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0    0x00000268 /* 616 */
#define REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG1    0x0000026c /* 620 */
#define REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG2    0x00000270 /* 624 */
#define REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG3    0x00000274 /* 628 */
#define REG_DX_L2_NPU0_PE0_DMA_W_CFG0    0x00000278 /* 632 */
#define REG_DX_L2_NPU0_PE0_DMA_W_CFG1    0x0000027c /* 636 */
#define REG_DX_L2_NPU0_PE0_DMA_W_CFG2    0x00000280 /* 640 */
#define REG_DX_L2_NPU0_PE0_DMA_W_CFG3    0x00000284 /* 644 */
#define REG_DX_L2_NPU0_PE0_DMA_INF_CFG0    0x00000288 /* 648 */
#define REG_DX_L2_NPU0_PE0_DMA_INF_CFG1    0x0000028c /* 652 */
#define REG_DX_L2_NPU0_PE0_DMA_INF_CFG2    0x00000290 /* 656 */
#define REG_DX_L2_NPU0_PE0_DMA_INF_CFG3    0x00000294 /* 660 */
#define REG_DX_L2_NPU0_PE0_DMA_INF_CFG4    0x00000298 /* 664 */
#define REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0    0x0000029c /* 668 */
#define REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG1    0x000002a0 /* 672 */
#define REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG2    0x000002a4 /* 676 */
#define REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG3    0x000002a8 /* 680 */
#define REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG4    0x000002ac /* 684 */
#define REG_DX_L2_NPU0_RSVD5    0x000002b0 /* 688 */
#define REG_DX_L2_NPU0_RSVD6    0x000002b4 /* 692 */
#define REG_DX_L2_NPU0_PE1_DMA_CTRL    0x000002b8 /* 696 */
#define REG_DX_L2_NPU0_PE1_DMA_BN_CFG0    0x000002bc /* 700 */
#define REG_DX_L2_NPU0_PE1_DMA_BN_CFG1    0x000002c0 /* 704 */
#define REG_DX_L2_NPU0_PE1_DMA_BN_CFG2    0x000002c4 /* 708 */
#define REG_DX_L2_NPU0_PE1_DMA_BN_CFG3    0x000002c8 /* 712 */
#define REG_DX_L2_NPU0_PE1_DMA_B_CFG0    0x000002cc /* 716 */
#define REG_DX_L2_NPU0_PE1_DMA_B_CFG1    0x000002d0 /* 720 */
#define REG_DX_L2_NPU0_PE1_DMA_B_CFG2    0x000002d4 /* 724 */
#define REG_DX_L2_NPU0_PE1_DMA_B_CFG3    0x000002d8 /* 728 */
#define REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0    0x000002dc /* 732 */
#define REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1    0x000002e0 /* 736 */
#define REG_DX_L2_NPU0_PE1_DMA_MUL_CFG2    0x000002e4 /* 740 */
#define REG_DX_L2_NPU0_PE1_DMA_MUL_CFG3    0x000002e8 /* 744 */
#define REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0    0x000002ec /* 748 */
#define REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG1    0x000002f0 /* 752 */
#define REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG2    0x000002f4 /* 756 */
#define REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG3    0x000002f8 /* 760 */
#define REG_DX_L2_NPU0_PE1_DMA_W_CFG0    0x000002fc /* 764 */
#define REG_DX_L2_NPU0_PE1_DMA_W_CFG1    0x00000300 /* 768 */
#define REG_DX_L2_NPU0_PE1_DMA_W_CFG2    0x00000304 /* 772 */
#define REG_DX_L2_NPU0_PE1_DMA_W_CFG3    0x00000308 /* 776 */
#define REG_DX_L2_NPU0_PE1_DMA_INF_CFG0    0x0000030c /* 780 */
#define REG_DX_L2_NPU0_PE1_DMA_INF_CFG1    0x00000310 /* 784 */
#define REG_DX_L2_NPU0_PE1_DMA_INF_CFG2    0x00000314 /* 788 */
#define REG_DX_L2_NPU0_PE1_DMA_INF_CFG3    0x00000318 /* 792 */
#define REG_DX_L2_NPU0_PE1_DMA_INF_CFG4    0x0000031c /* 796 */
#define REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0    0x00000320 /* 800 */
#define REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG1    0x00000324 /* 804 */
#define REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG2    0x00000328 /* 808 */
#define REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG3    0x0000032c /* 812 */
#define REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG4    0x00000330 /* 816 */
#define REG_DX_L2_NPU0_RSVD7    0x00000334 /* 820 */
#define REG_DX_L2_NPU0_RSVD8    0x00000338 /* 824 */

/* Register Definition */
typedef volatile union _DX_L2_NPU0_DATA_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.DATA_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FEATURE_BIT_WIDTH:8;  /* DX_L2_NPU0.DATA_CFG0.FEATURE_BIT_WIDTH [7:0] (RW, 0x0) INPUT FEATURE DATA BIT WIDTH */
        volatile uint32_t RESERVED0:23;  /* DX_L2_NPU0.DATA_CFG0.RESERVED0 [30:8] (RW, 0x0) - */
        volatile uint32_t FEATURE_DATA_TYPE:1;  /* DX_L2_NPU0.DATA_CFG0.FEATURE_DATA_TYPE [31:31] (RW, 0x0) 0: UNSIGNED, 1 : SIGNED */
    };
} DX_L2_NPU0_DATA_CFG0_t;
typedef volatile union _DX_L2_NPU0_DATA_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.DATA_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t WEIGHT_BIT_WIDTH:8;  /* DX_L2_NPU0.DATA_CFG1.WEIGHT_BIT_WIDTH [7:0] (RW, 0x0) IN WEIGHT DATA BIT WIDTH */
        volatile uint32_t BIAS_BIT_WIDTH:8;  /* DX_L2_NPU0.DATA_CFG1.BIAS_BIT_WIDTH [15:8] (RW, 0x0) IN BIAS DATA BIT WIDTH */
        volatile uint32_t BN_COEFF_BIT_WIDTH:8;  /* DX_L2_NPU0.DATA_CFG1.BN_COEFF_BIT_WIDTH [23:16] (RW, 0x0) BN COEFF DATA BIT WIDTH */
        volatile uint32_t W_FEATURE_BIT_WIDTH:8;  /* DX_L2_NPU0.DATA_CFG1.W_FEATURE_BIT_WIDTH [31:24] (RW, 0x0) OUTPUT FEATURE DATA BIT WIDTH */
    };
} DX_L2_NPU0_DATA_CFG1_t;
typedef volatile union _DX_L2_NPU0_IM2COL_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.IM2COL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t GLB2RF_RF_WSIZE:16;  /* DX_L2_NPU0.IM2COL.GLB2RF_RF_WSIZE [15:0] (RW, 0x0) NOT USE */
        volatile uint32_t RESERVED0:13;  /* DX_L2_NPU0.IM2COL.RESERVED0 [28:16] (RW, 0x0) - */
        volatile uint32_t GLB2RF_WRITE:1;  /* DX_L2_NPU0.IM2COL.GLB2RF_WRITE [29:29] (RW, 0x0) NOT USE */
        volatile uint32_t GLB2RF_START:1;  /* DX_L2_NPU0.IM2COL.GLB2RF_START [30:30] (RW, 0x0) NOT USE */
        volatile uint32_t IMG2COL_EN:1;  /* DX_L2_NPU0.IM2COL.IMG2COL_EN [31:31] (RW, 0x0) INPUT FEATURE MAP CONVERT SHAPE FOR POINTWISE CONV */
    };
} DX_L2_NPU0_IM2COL_t;
typedef volatile union _DX_L2_NPU0_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t WINOGRAD_EN:1;  /* DX_L2_NPU0.CTRL.WINOGRAD_EN [0:0] (RW, 0x0) TBD, NOT USE */
        volatile uint32_t FEATURE_COMPRESS_EN:1;  /* DX_L2_NPU0.CTRL.FEATURE_COMPRESS_EN [1:1] (RW, 0x0) TBD, NOT USE */
        volatile uint32_t WEIGHT_COMPRESS_EN:1;  /* DX_L2_NPU0.CTRL.WEIGHT_COMPRESS_EN [2:2] (RW, 0x0) TBD, NOT USE */
        volatile uint32_t SKIP_CONNECTION_EN:1;  /* DX_L2_NPU0.CTRL.SKIP_CONNECTION_EN [3:3] (RW, 0x0) TBD, NOT USE */
        volatile uint32_t SKIP_CONNECTION_TYPE:2;  /* DX_L2_NPU0.CTRL.SKIP_CONNECTION_TYPE [5:4] (RW, 0x0) TBD, NOT USE */
        volatile uint32_t RESIZE:2;  /* DX_L2_NPU0.CTRL.RESIZE [7:6] (RW, 0x0) TBD */
        volatile uint32_t RESERVED0:24;  /* DX_L2_NPU0.CTRL.RESERVED0 [31:8] (RW, 0x0) - */
    };
} DX_L2_NPU0_CTRL_t;
typedef volatile union _DX_L2_NPU0_MISC_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.MISC*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t TILE_NUM:16;  /* DX_L2_NPU0.MISC.TILE_NUM [15:0] (RW, 0x0) NOT USE, BUT FOR DEBUG */
        volatile uint32_t LAYER_NUM:16;  /* DX_L2_NPU0.MISC.LAYER_NUM [31:16] (RW, 0x0) NOT USE, BUT FOR DEBUG */
    };
} DX_L2_NPU0_MISC_t;
typedef volatile union _DX_L2_NPU0_ACTIVE_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.ACTIVE*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t ACTIVE_CNT:8;  /* DX_L2_NPU0.ACTIVE.ACTIVE_CNT [7:0] (RW, 0x0) - */
        volatile uint32_t ACTIVE_PERIOD:8;  /* DX_L2_NPU0.ACTIVE.ACTIVE_PERIOD [15:8] (RW, 0x0) - */
        volatile uint32_t RESERVED0:16;  /* DX_L2_NPU0.ACTIVE.RESERVED0 [31:16] (RW, 0x0) NOT USE, BUT FOR DEBUG */
    };
} DX_L2_NPU0_ACTIVE_t;
typedef volatile union _DX_L2_NPU0_RSVD0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.RSVD0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_L2_NPU0.RSVD0.RESERVED0 [31:0] (RW, 0x0) NOT USE, BUT FOR DEBUG */
    };
} DX_L2_NPU0_RSVD0_t;
typedef volatile union _DX_L2_NPU0_SFU_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PRE_ACT_MODE:3;  /* DX_L2_NPU0.SFU_CTRL.PRE_ACT_MODE [2:0] (RW, 0x0) 000: BYPASS, 001: ADD INT BIAS, 010: ADD FLOAT BIAS, 011: ADD INT ONLY, 100: ADD FLOAT ONLY, 101:MULT FLOAT ONLY */
        volatile uint32_t PRE_MULT_COEFF_SEL:1;  /* DX_L2_NPU0.SFU_CTRL.PRE_MULT_COEFF_SEL [3:3] (RW, 0x0) 0: MULTIPLIER COEFFICIENT FROM REG, 1: MULTIPLIER COEFFICIENT FROM REG, */
        volatile uint32_t ACT_MODE:3;  /* DX_L2_NPU0.SFU_CTRL.ACT_MODE [6:4] (RW, 0x0) 000: BYPASS , 001: RELU, 010: LEAKY-RELU, 011: RELU6, 100: PAF */
        volatile uint32_t ACT_INOUT_CTRL:1;  /* DX_L2_NPU0.SFU_CTRL.ACT_INOUT_CTRL [7:7] (RW, 0x0) 0: ACTIVATION INPUT FROM PRE_ACT OUTPUT, 1: ACTIVATION INPUT FROM POST_ACT OUTPUT */
        volatile uint32_t POST_ACT_MODE:1;  /* DX_L2_NPU0.SFU_CTRL.POST_ACT_MODE [8:8] (RW, 0x0) 0: BYPASS , 1: ADD SKIP */
        volatile uint32_t SKIP_IN_TYPE:1;  /* DX_L2_NPU0.SFU_CTRL.SKIP_IN_TYPE [9:9] (RW, 0x0) 0: SKIP INPUT IS SIGNED 8BIT INTEGER NUMBER, 1: SKIP INPUT IS UNSIGNED 8BIT INTEGER NUMBER */
        volatile uint32_t FLOAT2INT_MODE:1;  /* DX_L2_NPU0.SFU_CTRL.FLOAT2INT_MODE [10:10] (RW, 0x0) 0: BYPASS, 1: ENABLE */
        volatile uint32_t POOL_MODE:2;  /* DX_L2_NPU0.SFU_CTRL.POOL_MODE [12:11] (RW, 0x0) 00: BYPASS , 01: AVG POOL, 10: MAX POOL */
        volatile uint32_t POOL_WINSIZE:8;  /* DX_L2_NPU0.SFU_CTRL.POOL_WINSIZE [20:13] (RW, 0x0) SIZE(NUMBER OF ELEMENT) OF KERNEL WHEN MAX_POOLING */
        volatile uint32_t PRE_ADD_COEFF_SEL:1;  /* DX_L2_NPU0.SFU_CTRL.PRE_ADD_COEFF_SEL [21:21] (RW, 0x0)  */
        volatile uint32_t SE_EN:1;  /* DX_L2_NPU0.SFU_CTRL.SE_EN [22:22] (RW, 0x0) TBD, NOT USE */
        volatile uint32_t SE_WRITE:1;  /* DX_L2_NPU0.SFU_CTRL.SE_WRITE [23:23] (RW, 0x0) TBD, NOT USE */
        volatile uint32_t ARG_MAX_EN:1;  /* DX_L2_NPU0.SFU_CTRL.ARG_MAX_EN [24:24] (RW, 0x0)  */
        volatile uint32_t ARG_CLASS_NUM:7;  /* DX_L2_NPU0.SFU_CTRL.ARG_CLASS_NUM [31:25] (RW, 0x0) - */
    };
} DX_L2_NPU0_SFU_CTRL_t;
typedef volatile union _DX_L2_NPU0_SFU_CLIP_MAX_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_CLIP_MAX*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CLIP_MAX:32;  /* DX_L2_NPU0.SFU_CLIP_MAX.CLIP_MAX [31:0] (RW, 0x0) CLIPPING UPPER LIMIT */
    };
} DX_L2_NPU0_SFU_CLIP_MAX_t;
typedef volatile union _DX_L2_NPU0_SFU_CLIP_MIN_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_CLIP_MIN*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CLIP_MIN:32;  /* DX_L2_NPU0.SFU_CLIP_MIN.CLIP_MIN [31:0] (RW, 0x0) CLIPPING LOWER LIMIT */
    };
} DX_L2_NPU0_SFU_CLIP_MIN_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_L2_NPU0.SFU_COEFF0.RESERVED0 [31:0] (RW, 0x0) TBD, NOT USE */
    };
} DX_L2_NPU0_SFU_COEFF0_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PRE_ACT_MULT_COEFF:32;  /* DX_L2_NPU0.SFU_COEFF1.PRE_ACT_MULT_COEFF [31:0] (RW, 0x0) VALUE WHEN PRE_MULT_COEFF_SEL = 0 (FLAOT32) */
    };
} DX_L2_NPU0_SFU_COEFF1_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PRE_ACT_ADD_COEFF:32;  /* DX_L2_NPU0.SFU_COEFF2.PRE_ACT_ADD_COEFF [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_SFU_COEFF2_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t AVG_POOL_MULT_COEFF:32;  /* DX_L2_NPU0.SFU_COEFF3.AVG_POOL_MULT_COEFF [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_SFU_COEFF3_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF4_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t ACT_OFFSET_VAL:32;  /* DX_L2_NPU0.SFU_COEFF4.ACT_OFFSET_VAL [31:0] (RW, 0x0) VALUE WHEN ACT_OFFSET_SEL = 0 (FLAOT32) */
    };
} DX_L2_NPU0_SFU_COEFF4_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF5_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SKIP_SCALE:32;  /* DX_L2_NPU0.SFU_COEFF5.SKIP_SCALE [31:0] (RW, 0x0) SCALE VALUE FOR SKIP INPUT(FLAOT32) */
    };
} DX_L2_NPU0_SFU_COEFF5_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF6_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t LEAKY_RELU_SLOPE:32;  /* DX_L2_NPU0.SFU_COEFF6.LEAKY_RELU_SLOPE [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_SFU_COEFF6_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF7_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RELU6_CLIP_MAX:32;  /* DX_L2_NPU0.SFU_COEFF7.RELU6_CLIP_MAX [31:0] (RW, 0x0) RELU6 CLIPPING VALUE */
    };
} DX_L2_NPU0_SFU_COEFF7_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF8_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY0:32;  /* DX_L2_NPU0.SFU_COEFF8.PAF_BOUNDARY0 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION0 AND REGION1 */
    };
} DX_L2_NPU0_SFU_COEFF8_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF9_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY1:32;  /* DX_L2_NPU0.SFU_COEFF9.PAF_BOUNDARY1 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION1 AND REGION2 */
    };
} DX_L2_NPU0_SFU_COEFF9_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF10_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF10*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY2:32;  /* DX_L2_NPU0.SFU_COEFF10.PAF_BOUNDARY2 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION2 AND REGION3 */
    };
} DX_L2_NPU0_SFU_COEFF10_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF11_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF11*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY3:32;  /* DX_L2_NPU0.SFU_COEFF11.PAF_BOUNDARY3 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION3 AND REGION4 */
    };
} DX_L2_NPU0_SFU_COEFF11_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF12_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF12*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY4:32;  /* DX_L2_NPU0.SFU_COEFF12.PAF_BOUNDARY4 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION4 AND REGION5 */
    };
} DX_L2_NPU0_SFU_COEFF12_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF13_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF13*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY5:32;  /* DX_L2_NPU0.SFU_COEFF13.PAF_BOUNDARY5 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION5 AND REGION6 */
    };
} DX_L2_NPU0_SFU_COEFF13_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF14_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF14*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY6:32;  /* DX_L2_NPU0.SFU_COEFF14.PAF_BOUNDARY6 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION6 AND REGION7 */
    };
} DX_L2_NPU0_SFU_COEFF14_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF15_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF15*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY7:32;  /* DX_L2_NPU0.SFU_COEFF15.PAF_BOUNDARY7 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION7 AND REGION8 */
    };
} DX_L2_NPU0_SFU_COEFF15_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF16_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF16*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY8:32;  /* DX_L2_NPU0.SFU_COEFF16.PAF_BOUNDARY8 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION8 AND REGION9 */
    };
} DX_L2_NPU0_SFU_COEFF16_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF17_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF17*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY9:32;  /* DX_L2_NPU0.SFU_COEFF17.PAF_BOUNDARY9 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION9 AND REGION10 */
    };
} DX_L2_NPU0_SFU_COEFF17_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF18_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF18*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY10:32;  /* DX_L2_NPU0.SFU_COEFF18.PAF_BOUNDARY10 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION10 AND REGION11 */
    };
} DX_L2_NPU0_SFU_COEFF18_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF19_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF19*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY11:32;  /* DX_L2_NPU0.SFU_COEFF19.PAF_BOUNDARY11 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION11 AND REGION12 */
    };
} DX_L2_NPU0_SFU_COEFF19_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF20_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF20*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY12:32;  /* DX_L2_NPU0.SFU_COEFF20.PAF_BOUNDARY12 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION12 AND REGION13 */
    };
} DX_L2_NPU0_SFU_COEFF20_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF21_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF21*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY13:32;  /* DX_L2_NPU0.SFU_COEFF21.PAF_BOUNDARY13 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION13 AND REGION14 */
    };
} DX_L2_NPU0_SFU_COEFF21_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF22_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF22*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY14:32;  /* DX_L2_NPU0.SFU_COEFF22.PAF_BOUNDARY14 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION14 AND REGION15 */
    };
} DX_L2_NPU0_SFU_COEFF22_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF23_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF23*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF0:32;  /* DX_L2_NPU0.SFU_COEFF23.PAF_MULT_COEFF0 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION0 */
    };
} DX_L2_NPU0_SFU_COEFF23_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF24_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF24*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF1:32;  /* DX_L2_NPU0.SFU_COEFF24.PAF_MULT_COEFF1 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION1 */
    };
} DX_L2_NPU0_SFU_COEFF24_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF25_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF25*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF2:32;  /* DX_L2_NPU0.SFU_COEFF25.PAF_MULT_COEFF2 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION2 */
    };
} DX_L2_NPU0_SFU_COEFF25_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF26_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF26*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF3:32;  /* DX_L2_NPU0.SFU_COEFF26.PAF_MULT_COEFF3 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION3 */
    };
} DX_L2_NPU0_SFU_COEFF26_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF27_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF27*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF4:32;  /* DX_L2_NPU0.SFU_COEFF27.PAF_MULT_COEFF4 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION4 */
    };
} DX_L2_NPU0_SFU_COEFF27_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF28_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF28*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF5:32;  /* DX_L2_NPU0.SFU_COEFF28.PAF_MULT_COEFF5 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION5 */
    };
} DX_L2_NPU0_SFU_COEFF28_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF29_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF29*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF6:32;  /* DX_L2_NPU0.SFU_COEFF29.PAF_MULT_COEFF6 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION6 */
    };
} DX_L2_NPU0_SFU_COEFF29_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF30_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF30*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF7:32;  /* DX_L2_NPU0.SFU_COEFF30.PAF_MULT_COEFF7 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION7 */
    };
} DX_L2_NPU0_SFU_COEFF30_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF31_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF31*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF8:32;  /* DX_L2_NPU0.SFU_COEFF31.PAF_MULT_COEFF8 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION8 */
    };
} DX_L2_NPU0_SFU_COEFF31_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF32_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF32*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF9:32;  /* DX_L2_NPU0.SFU_COEFF32.PAF_MULT_COEFF9 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION9 */
    };
} DX_L2_NPU0_SFU_COEFF32_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF33_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF33*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF10:32;  /* DX_L2_NPU0.SFU_COEFF33.PAF_MULT_COEFF10 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION10 */
    };
} DX_L2_NPU0_SFU_COEFF33_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF34_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF34*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF11:32;  /* DX_L2_NPU0.SFU_COEFF34.PAF_MULT_COEFF11 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION11 */
    };
} DX_L2_NPU0_SFU_COEFF34_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF35_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF35*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF12:32;  /* DX_L2_NPU0.SFU_COEFF35.PAF_MULT_COEFF12 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION12 */
    };
} DX_L2_NPU0_SFU_COEFF35_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF36_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF36*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF13:32;  /* DX_L2_NPU0.SFU_COEFF36.PAF_MULT_COEFF13 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION13 */
    };
} DX_L2_NPU0_SFU_COEFF36_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF37_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF37*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF14:32;  /* DX_L2_NPU0.SFU_COEFF37.PAF_MULT_COEFF14 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION14 */
    };
} DX_L2_NPU0_SFU_COEFF37_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF38_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF38*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF15:32;  /* DX_L2_NPU0.SFU_COEFF38.PAF_MULT_COEFF15 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION15 */
    };
} DX_L2_NPU0_SFU_COEFF38_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF39_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF39*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF0:32;  /* DX_L2_NPU0.SFU_COEFF39.PAF_ADD_COEFF0 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION0 */
    };
} DX_L2_NPU0_SFU_COEFF39_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF40_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF40*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF1:32;  /* DX_L2_NPU0.SFU_COEFF40.PAF_ADD_COEFF1 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION1 */
    };
} DX_L2_NPU0_SFU_COEFF40_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF41_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF41*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF2:32;  /* DX_L2_NPU0.SFU_COEFF41.PAF_ADD_COEFF2 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION2 */
    };
} DX_L2_NPU0_SFU_COEFF41_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF42_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF42*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF3:32;  /* DX_L2_NPU0.SFU_COEFF42.PAF_ADD_COEFF3 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION3 */
    };
} DX_L2_NPU0_SFU_COEFF42_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF43_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF43*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF4:32;  /* DX_L2_NPU0.SFU_COEFF43.PAF_ADD_COEFF4 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION4 */
    };
} DX_L2_NPU0_SFU_COEFF43_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF44_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF44*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF5:32;  /* DX_L2_NPU0.SFU_COEFF44.PAF_ADD_COEFF5 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION5 */
    };
} DX_L2_NPU0_SFU_COEFF44_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF45_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF45*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF6:32;  /* DX_L2_NPU0.SFU_COEFF45.PAF_ADD_COEFF6 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION6 */
    };
} DX_L2_NPU0_SFU_COEFF45_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF46_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF46*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF7:32;  /* DX_L2_NPU0.SFU_COEFF46.PAF_ADD_COEFF7 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION7 */
    };
} DX_L2_NPU0_SFU_COEFF46_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF47_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF47*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF8:32;  /* DX_L2_NPU0.SFU_COEFF47.PAF_ADD_COEFF8 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION8 */
    };
} DX_L2_NPU0_SFU_COEFF47_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF48_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF48*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF9:32;  /* DX_L2_NPU0.SFU_COEFF48.PAF_ADD_COEFF9 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION9 */
    };
} DX_L2_NPU0_SFU_COEFF48_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF49_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF49*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF10:32;  /* DX_L2_NPU0.SFU_COEFF49.PAF_ADD_COEFF10 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION10 */
    };
} DX_L2_NPU0_SFU_COEFF49_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF50_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF50*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF11:32;  /* DX_L2_NPU0.SFU_COEFF50.PAF_ADD_COEFF11 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION11 */
    };
} DX_L2_NPU0_SFU_COEFF50_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF51_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF51*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF12:32;  /* DX_L2_NPU0.SFU_COEFF51.PAF_ADD_COEFF12 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION12 */
    };
} DX_L2_NPU0_SFU_COEFF51_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF52_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF52*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF13:32;  /* DX_L2_NPU0.SFU_COEFF52.PAF_ADD_COEFF13 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION13 */
    };
} DX_L2_NPU0_SFU_COEFF52_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF53_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF53*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF14:32;  /* DX_L2_NPU0.SFU_COEFF53.PAF_ADD_COEFF14 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION14 */
    };
} DX_L2_NPU0_SFU_COEFF53_t;
typedef volatile union _DX_L2_NPU0_SFU_COEFF54_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.SFU_COEFF54*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF15:32;  /* DX_L2_NPU0.SFU_COEFF54.PAF_ADD_COEFF15 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION15 */
    };
} DX_L2_NPU0_SFU_COEFF54_t;
typedef volatile union _DX_L2_NPU0_RSVD1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.RSVD1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED3:8;  /* DX_L2_NPU0.RSVD1.RESERVED3 [7:0] (RW, 0x0) - */
        volatile uint32_t RESERVED2:8;  /* DX_L2_NPU0.RSVD1.RESERVED2 [15:8] (RW, 0x0) - */
        volatile uint32_t RESERVED1:8;  /* DX_L2_NPU0.RSVD1.RESERVED1 [23:16] (RW, 0x0) - */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.RSVD1.RESERVED0 [31:24] (RW, 0x0) - */
    };
} DX_L2_NPU0_RSVD1_t;
typedef volatile union _DX_L2_NPU0_RSVD2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.RSVD2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED3:8;  /* DX_L2_NPU0.RSVD2.RESERVED3 [7:0] (RW, 0x0) - */
        volatile uint32_t RESERVED2:8;  /* DX_L2_NPU0.RSVD2.RESERVED2 [15:8] (RW, 0x0) - */
        volatile uint32_t RESERVED1:8;  /* DX_L2_NPU0.RSVD2.RESERVED1 [23:16] (RW, 0x0) - */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.RSVD2.RESERVED0 [31:24] (RW, 0x0) - */
    };
} DX_L2_NPU0_RSVD2_t;
typedef volatile union _DX_L2_NPU0_PE0_IMG2COL0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_IMG2COL0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_IMG2COL_CH_OFFSET:16;  /* DX_L2_NPU0.PE0_IMG2COL0.PE0_IMG2COL_CH_OFFSET [15:0] (RW, 0x0)  */
        volatile uint32_t PE0_IMG2COL_LINE_OFFSET:8;  /* DX_L2_NPU0.PE0_IMG2COL0.PE0_IMG2COL_LINE_OFFSET [23:16] (RW, 0x0)  */
        volatile uint32_t RESERVED0:7;  /* DX_L2_NPU0.PE0_IMG2COL0.RESERVED0 [30:24] (RW, 0x0)  */
        volatile uint32_t PE0_IMG2COL_EN:1;  /* DX_L2_NPU0.PE0_IMG2COL0.PE0_IMG2COL_EN [31:31] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_IMG2COL0_t;
typedef volatile union _DX_L2_NPU0_PE0_IMG2COL1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_IMG2COL1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_IMG2COL_OUT_FEATURE_WIDTH:16;  /* DX_L2_NPU0.PE0_IMG2COL1.PE0_IMG2COL_OUT_FEATURE_WIDTH [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.PE0_IMG2COL1.RESERVED0 [23:16] (RW, 0x0)  */
        volatile uint32_t PE0_IMG2COL_OHEIGHT_OFFSET:8;  /* DX_L2_NPU0.PE0_IMG2COL1.PE0_IMG2COL_OHEIGHT_OFFSET [31:24] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_IMG2COL1_t;
typedef volatile union _DX_L2_NPU0_PE0_FMT_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_FMT*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_FMT_FIRST_LINE_OPT:2;  /* DX_L2_NPU0.PE0_FMT.PE0_FMT_FIRST_LINE_OPT [1:0] (RW, 0x0)  */
        volatile uint32_t PE0_FMT_FIRST_VALID_NUM:6;  /* DX_L2_NPU0.PE0_FMT.PE0_FMT_FIRST_VALID_NUM [7:2] (RW, 0x0)  */
        volatile uint32_t RESERVED0:23;  /* DX_L2_NPU0.PE0_FMT.RESERVED0 [30:8] (RW, 0x0)  */
        volatile uint32_t PE0_FMT_ENABLE:1;  /* DX_L2_NPU0.PE0_FMT.PE0_FMT_ENABLE [31:31] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_FMT_t;
typedef volatile union _DX_L2_NPU0_PE0_FMT_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_FMT_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_FMT_READ_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_FMT_CFG0.PE0_FMT_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_FMT_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE0_ADDR_GEN0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_ADDR_GEN0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM:16;  /* DX_L2_NPU0.PE0_ADDR_GEN0.PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:16;  /* DX_L2_NPU0.PE0_ADDR_GEN0.RESERVED0 [31:16] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_ADDR_GEN0_t;
typedef volatile union _DX_L2_NPU0_PE0_ADDR_GEN1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_ADDR_GEN1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_ADDR_GEN1.PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_ADDR_GEN1_t;
typedef volatile union _DX_L2_NPU0_PE0_ADDR_GEN2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_ADDR_GEN2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_BN_ADDR_GEN_MODE:2;  /* DX_L2_NPU0.PE0_ADDR_GEN2.PE0_BN_ADDR_GEN_MODE [1:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:30;  /* DX_L2_NPU0.PE0_ADDR_GEN2.RESERVED0 [31:2] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_ADDR_GEN2_t;
typedef volatile union _DX_L2_NPU0_PE0_ADDR_GEN3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_ADDR_GEN3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_BN_ADDR_GEN_MODE1_OFFSET:16;  /* DX_L2_NPU0.PE0_ADDR_GEN3.PE0_BN_ADDR_GEN_MODE1_OFFSET [15:0] (RW, 0x0)  */
        volatile uint32_t PE0_BN_ADDR_GEN_MODE1_MAX_NUM:16;  /* DX_L2_NPU0.PE0_ADDR_GEN3.PE0_BN_ADDR_GEN_MODE1_MAX_NUM [31:16] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_ADDR_GEN3_t;
typedef volatile union _DX_L2_NPU0_PE0_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_CONV_TYPE:8;  /* DX_L2_NPU0.PE0_CTRL.PE0_CONV_TYPE [7:0] (RW, 0x0) 0: STANDARD, 1: DEPTHWISE, 2: POINTWISE */
        volatile uint32_t PE0_CONV_EN:1;  /* DX_L2_NPU0.PE0_CTRL.PE0_CONV_EN [8:8] (RW, 0x0) 0: BYPASS, 1: OPERATION ENABLE */
        volatile uint32_t RESERVED0:7;  /* DX_L2_NPU0.PE0_CTRL.RESERVED0 [15:9] (RW, 0x0) - */
        volatile uint32_t PE0_SYSTOLIC_DEPTH:8;  /* DX_L2_NPU0.PE0_CTRL.PE0_SYSTOLIC_DEPTH [23:16] (RW, 0x0) SYSTOLIC ARRAY ENABLE DEPTH */
        volatile uint32_t PE0_SYSTOLIC_EN:1;  /* DX_L2_NPU0.PE0_CTRL.PE0_SYSTOLIC_EN [24:24] (RW, 0x0) 0: BYPASS, 1: OPERATION ENABLE (SYSTOLIC FUNCTION FOR FPGA) */
        volatile uint32_t PE0_SFUIN_VALID_NUM:7;  /* DX_L2_NPU0.PE0_CTRL.PE0_SFUIN_VALID_NUM [31:25] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_CTRL_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_LAST_CONV_NUM:16;  /* DX_L2_NPU0.PE0_CFG0.PE0_LAST_CONV_NUM [15:0] (RW, 0x0)  */
        volatile uint32_t PE0_LAST_CONV_MOD:8;  /* DX_L2_NPU0.PE0_CFG0.PE0_LAST_CONV_MOD [23:16] (RW, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.PE0_CFG0.RESERVED0 [31:24] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_OFEATURE_CHANNEL:16;  /* DX_L2_NPU0.PE0_CFG1.PE0_OFEATURE_CHANNEL [15:0] (RW, 0x0)  */
        volatile uint32_t PE0_OFEATURE_NUMBER:15;  /* DX_L2_NPU0.PE0_CFG1.PE0_OFEATURE_NUMBER [30:16] (RW, 0x0)  */
        volatile uint32_t PE0_OFEATURE_DISABLE:1;  /* DX_L2_NPU0.PE0_CFG1.PE0_OFEATURE_DISABLE [31:31] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_OFEATURE_WIDTH:10;  /* DX_L2_NPU0.PE0_CFG2.PE0_OFEATURE_WIDTH [9:0] (RW, 0x0)  */
        volatile uint32_t PE0_OFEATURE_HEIGHT:10;  /* DX_L2_NPU0.PE0_CFG2.PE0_OFEATURE_HEIGHT [19:10] (RW, 0x0)  */
        volatile uint32_t RESERVED0:12;  /* DX_L2_NPU0.PE0_CFG2.RESERVED0 [31:20] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_FEATURE_CHANNEL:16;  /* DX_L2_NPU0.PE0_CFG3.PE0_FEATURE_CHANNEL [15:0] (RW, 0x0) INPUT FEATURE MAP CHANNEL, CONVOLUTION DIMENSIONS */
        volatile uint32_t RESERVED0:16;  /* DX_L2_NPU0.PE0_CFG3.RESERVED0 [31:16] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_FEATURE_WIDTH:10;  /* DX_L2_NPU0.PE0_CFG4.PE0_FEATURE_WIDTH [9:0] (RW, 0x0) INPUT FEATURE MAP WIDTH, CONVOLUTION DIMENSIONS */
        volatile uint32_t PE0_FEATURE_HEIGHT:10;  /* DX_L2_NPU0.PE0_CFG4.PE0_FEATURE_HEIGHT [19:10] (RW, 0x0) INPUT FEATURE MAP HEIGHT, CONVOLUTION DIMENSIONS */
        volatile uint32_t RESERVED0:12;  /* DX_L2_NPU0.PE0_CFG4.RESERVED0 [31:20] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG4_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG5_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_FILTER_CHANNEL:16;  /* DX_L2_NPU0.PE0_CFG5.PE0_FILTER_CHANNEL [15:0] (RW, 0x0) FILTER WINDOW DEPTH, CONVOLUTION DIMENSIONS */
        volatile uint32_t PE0_FILTER_NUMBER:16;  /* DX_L2_NPU0.PE0_CFG5.PE0_FILTER_NUMBER [31:16] (RW, 0x0) FILTER NUMBER, CONVOLUTION DIMENSIONS */
    };
} DX_L2_NPU0_PE0_CFG5_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG6_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_FILTER_WIDTH:4;  /* DX_L2_NPU0.PE0_CFG6.PE0_FILTER_WIDTH [3:0] (RW, 0x0) FILTER WINDOW WIDTH, CONVOLUTION DIMENSIONS */
        volatile uint32_t PE0_FILTER_HEIGHT:4;  /* DX_L2_NPU0.PE0_CFG6.PE0_FILTER_HEIGHT [7:4] (RW, 0x0) FILTER WINDOW HEIGHT, CONVOLUTION DIMENSIONS */
        volatile uint32_t RESERVED0:24;  /* DX_L2_NPU0.PE0_CFG6.RESERVED0 [31:8] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG6_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG7_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_PAD_TOP_SIZE:4;  /* DX_L2_NPU0.PE0_CFG7.PE0_PAD_TOP_SIZE [3:0] (RW, 0x0) TOP PADDING SIZE */
        volatile uint32_t PE0_PAD_BOTTOM_SIZE:4;  /* DX_L2_NPU0.PE0_CFG7.PE0_PAD_BOTTOM_SIZE [7:4] (RW, 0x0) BOTTOM PADDING SIZE */
        volatile uint32_t PE0_PAD_LEFT_SIZE:4;  /* DX_L2_NPU0.PE0_CFG7.PE0_PAD_LEFT_SIZE [11:8] (RW, 0x0) LEFT PADDING SIZE */
        volatile uint32_t PE0_PAD_RIGHT_SIZE:4;  /* DX_L2_NPU0.PE0_CFG7.PE0_PAD_RIGHT_SIZE [15:12] (RW, 0x0) RIGHT PADDING SIZE */
        volatile uint32_t PE0_PAD_VALUE:8;  /* DX_L2_NPU0.PE0_CFG7.PE0_PAD_VALUE [23:16] (RW, 0x0) TBD, PADDING VALUE */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.PE0_CFG7.RESERVED0 [31:24] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_CFG7_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG8_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_STRIDE_X:4;  /* DX_L2_NPU0.PE0_CFG8.PE0_STRIDE_X [3:0] (RW, 0x0) HORIZONTAL STRIDE NUMBER OF THE SLIDING WINDOW */
        volatile uint32_t PE0_STRIDE_Y:4;  /* DX_L2_NPU0.PE0_CFG8.PE0_STRIDE_Y [7:4] (RW, 0x0) VERTICAL STRIDE NUMBER OF THE SLIDING WINDOW */
        volatile uint32_t PE0_DILATION_X:4;  /* DX_L2_NPU0.PE0_CFG8.PE0_DILATION_X [11:8] (RW, 0x0) TBD, HORIZONTAL DILATION NUMBER OF THE SLIDING WINDOW */
        volatile uint32_t PE0_DILATION_Y:4;  /* DX_L2_NPU0.PE0_CFG8.PE0_DILATION_Y [15:12] (RW, 0x0) TBD, VERTICAL DILATION NUMBER OF THE SLIDING WINDOW */
        volatile uint32_t PE0_CHANNEL_OFFSET:16;  /* DX_L2_NPU0.PE0_CFG8.PE0_CHANNEL_OFFSET [31:16] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_CFG8_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG9_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_READ_FEATURE_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_CFG9.PE0_READ_FEATURE_BASE_ADDR [31:0] (RW, 0x0) READ FEATURE DATA SRAM BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_CFG9_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG10_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG10*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WRITE_FEATURE_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_CFG10.PE0_WRITE_FEATURE_BASE_ADDR [31:0] (RW, 0x0) WRITE FEATURE DATA SRAM BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_CFG10_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG11_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG11*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_READ_WEIGHT_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_CFG11.PE0_READ_WEIGHT_BASE_ADDR [31:0] (RW, 0x0) READ WEIGHT DATA SRAM BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_CFG11_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG12_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG12*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_BN_COEFF_READ_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_CFG12.PE0_BN_COEFF_READ_BASE_ADDR [31:0] (RW, 0x0) READ BN DATA SRAM BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_CFG12_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG13_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG13*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_BIAS_READ_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_CFG13.PE0_BIAS_READ_BASE_ADDR [31:0] (RW, 0x0) READ BIAS DATA SRAM BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_CFG13_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG14_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG14*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_MUL_READ_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_CFG14.PE0_MUL_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG14_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG15_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG15*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_ELEM_READ_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_CFG15.PE0_ELEM_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG15_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG16_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG16*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_ELEM_CHANNEL_OFFSET:16;  /* DX_L2_NPU0.PE0_CFG16.PE0_ELEM_CHANNEL_OFFSET [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:16;  /* DX_L2_NPU0.PE0_CFG16.RESERVED0 [31:16] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG16_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG17_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG17*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_ADDR_MODE1_LINE_SIZE:16;  /* DX_L2_NPU0.PE0_CFG17.PE0_WF_ADDR_MODE1_LINE_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:14;  /* DX_L2_NPU0.PE0_CFG17.RESERVED0 [29:16] (RW, 0x0)  */
        volatile uint32_t PE0_WF_ADDR_GEN_MODE:2;  /* DX_L2_NPU0.PE0_CFG17.PE0_WF_ADDR_GEN_MODE [31:30] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG17_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG18_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG18*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_ADDR_MODE1_LINE_STRIDE:32;  /* DX_L2_NPU0.PE0_CFG18.PE0_WF_ADDR_MODE1_LINE_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG18_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG19_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG19*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_ADDR_LINE_STRIDE:16;  /* DX_L2_NPU0.PE0_CFG19.PE0_WF_ADDR_LINE_STRIDE [15:0] (RW, 0x0) LINE STRIDE OF OUTPUT FEATURE ADDRESS */
        volatile uint32_t PE0_WF_ADDR_SURFACE_STRIDE:16;  /* DX_L2_NPU0.PE0_CFG19.PE0_WF_ADDR_SURFACE_STRIDE [31:16] (RW, 0x0) SURFACE STRIDE OF OUTPUT FEATURE ADDRESS */
    };
} DX_L2_NPU0_PE0_CFG19_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG20_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG20*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_WRITE_CNT:16;  /* DX_L2_NPU0.PE0_CFG20.PE0_WF_WRITE_CNT [15:0] (RW, 0xffff) COUNT BY UNIT WORD(512BIT), LAYER OUTPUT SIZE */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.PE0_CFG20.RESERVED0 [23:16] (RW, 0x0) - */
        volatile uint32_t PE0_WF_RF2RF_TIME:8;  /* DX_L2_NPU0.PE0_CFG20.PE0_WF_RF2RF_TIME [31:24] (RW, 0x0) TBD, WRITE TIME */
    };
} DX_L2_NPU0_PE0_CFG20_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG21_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG21*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_RF2RF_SIZE:16;  /* DX_L2_NPU0.PE0_CFG21.PE0_WF_RF2RF_SIZE [15:0] (RW, 0x0) TBD, WRITE DATA SIZE */
        volatile uint32_t PE0_WF_RF2RF_ADDR_STRIDE:16;  /* DX_L2_NPU0.PE0_CFG21.PE0_WF_RF2RF_ADDR_STRIDE [31:16] (RW, 0x0) TBD, STRIDE VALUE OF SOURCE, DESTINATION ADDRESS */
    };
} DX_L2_NPU0_PE0_CFG21_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG22_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG22*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_RF2RF_SRC_ADDR:32;  /* DX_L2_NPU0.PE0_CFG22.PE0_WF_RF2RF_SRC_ADDR [31:0] (RW, 0x0) TBD */
    };
} DX_L2_NPU0_PE0_CFG22_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG23_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG23*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_RF2RF_DST_ADDR:32;  /* DX_L2_NPU0.PE0_CFG23.PE0_WF_RF2RF_DST_ADDR [31:0] (RW, 0x0) TBD */
    };
} DX_L2_NPU0_PE0_CFG23_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG24_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG24*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_SFU_OUT_DATA_VALID_NUM:3;  /* DX_L2_NPU0.PE0_CFG24.PE0_SFU_OUT_DATA_VALID_NUM [2:0] (RW, 0x0) SFU OUTPUT ENABLE LENGTH( 0 : OUT FEATURE BIT 8, 4: OUT FEATURE BIT 32) */
        volatile uint32_t RESERVED1:5;  /* DX_L2_NPU0.PE0_CFG24.RESERVED1 [7:3] (RW, 0x0) - */
        volatile uint32_t PE0_ST_ADDR_GEN_SHIFT_LEN:5;  /* DX_L2_NPU0.PE0_CFG24.PE0_ST_ADDR_GEN_SHIFT_LEN [12:8] (RW, 0x0) ADDRESS GEN DIVIDE PARAMETER  USING ONE FORMULA */
        volatile uint32_t RESERVED0:19;  /* DX_L2_NPU0.PE0_CFG24.RESERVED0 [31:13] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_CFG24_t;
typedef volatile union _DX_L2_NPU0_PE0_CFG25_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_CFG25*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_ST_ADDR_GEN_MUL:31;  /* DX_L2_NPU0.PE0_CFG25.PE0_ST_ADDR_GEN_MUL [30:0] (RW, 0x0) ADDRESS GEN MULTIPLIER PARAMETER USING ONE FORMULA */
        volatile uint32_t RESERVED0:1;  /* DX_L2_NPU0.PE0_CFG25.RESERVED0 [31:31] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_CFG25_t;
typedef volatile union _DX_L2_NPU0_PE0_VMEM_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_VMEM_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_START_NO_IN_BUFF:6;  /* DX_L2_NPU0.PE0_VMEM_CFG0.PE0_START_NO_IN_BUFF [5:0] (RW, 0x0) SRAM(WRITE INPUT FEATURE) START INDEX, USING DOUBLE BUFFER, IF NOT USE THEN NONE */
        volatile uint32_t RESERVED3:2;  /* DX_L2_NPU0.PE0_VMEM_CFG0.RESERVED3 [7:6] (RW, 0x0) - */
        volatile uint32_t PE0_START_NO_WEIGHT_BUFF:6;  /* DX_L2_NPU0.PE0_VMEM_CFG0.PE0_START_NO_WEIGHT_BUFF [13:8] (RW, 0x0) SRAM(WRITE WEIGHT) START INDEX, USING DOUBLE BUFFER, IF NOT USE THEN NONE */
        volatile uint32_t RESERVED2:2;  /* DX_L2_NPU0.PE0_VMEM_CFG0.RESERVED2 [15:14] (RW, 0x0) - */
        volatile uint32_t PE0_START_NO_OUT_BUFF:6;  /* DX_L2_NPU0.PE0_VMEM_CFG0.PE0_START_NO_OUT_BUFF [21:16] (RW, 0x0) SRAM(WRITE OUTPUT FEATURE) START INDEX, USING DOUBLE BUFFER, IF NOT USE THEN NONE */
        volatile uint32_t RESERVED1:2;  /* DX_L2_NPU0.PE0_VMEM_CFG0.RESERVED1 [23:22] (RW, 0x0) - */
        volatile uint32_t PE0_START_NO_ELEM_BUFF:6;  /* DX_L2_NPU0.PE0_VMEM_CFG0.PE0_START_NO_ELEM_BUFF [29:24] (RW, 0x0) SRAM(ELEMENTWISE ADD) START INDEX, USING DOUBLE BUFFER, IF NOT USE THEN NONE */
        volatile uint32_t RESERVED0:2;  /* DX_L2_NPU0.PE0_VMEM_CFG0.RESERVED0 [31:30] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_VMEM_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE0_SMEM_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_SMEM_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_BN_BUFF_SEL:1;  /* DX_L2_NPU0.PE0_SMEM_CFG0.PE0_BN_BUFF_SEL [0:0] (RW, 0x0) BN SRAM ADDRESS USED BY NPU */
        volatile uint32_t PE0_B_BUFF_SEL:1;  /* DX_L2_NPU0.PE0_SMEM_CFG0.PE0_B_BUFF_SEL [1:1] (RW, 0x0) BIAS SRAM ADDRESS USED BY NPU, CHOICE 0 OR 1 */
        volatile uint32_t PE0_MUL_BUFF_SEL:1;  /* DX_L2_NPU0.PE0_SMEM_CFG0.PE0_MUL_BUFF_SEL [2:2] (RW, 0x0) FLOATING POINT BN SRAM ADDRESS USED BY NPU, CHOICE 0 OR 1 */
        volatile uint32_t RESERVED0:29;  /* DX_L2_NPU0.PE0_SMEM_CFG0.RESERVED0 [31:3] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_SMEM_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE1_IMG2COL0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_IMG2COL0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_IMG2COL_CH_OFFSET:16;  /* DX_L2_NPU0.PE1_IMG2COL0.PE1_IMG2COL_CH_OFFSET [15:0] (RW, 0x0)  */
        volatile uint32_t PE1_IMG2COL_LINE_OFFSET:8;  /* DX_L2_NPU0.PE1_IMG2COL0.PE1_IMG2COL_LINE_OFFSET [23:16] (RW, 0x0)  */
        volatile uint32_t RESERVED0:7;  /* DX_L2_NPU0.PE1_IMG2COL0.RESERVED0 [30:24] (RW, 0x0)  */
        volatile uint32_t PE1_IMG2COL_EN:1;  /* DX_L2_NPU0.PE1_IMG2COL0.PE1_IMG2COL_EN [31:31] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_IMG2COL0_t;
typedef volatile union _DX_L2_NPU0_PE1_IMG2COL1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_IMG2COL1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_IMG2COL_OUT_FEATURE_WIDTH:16;  /* DX_L2_NPU0.PE1_IMG2COL1.PE1_IMG2COL_OUT_FEATURE_WIDTH [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.PE1_IMG2COL1.RESERVED0 [23:16] (RW, 0x0)  */
        volatile uint32_t PE1_IMG2COL_OHEIGHT_OFFSET:8;  /* DX_L2_NPU0.PE1_IMG2COL1.PE1_IMG2COL_OHEIGHT_OFFSET [31:24] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_IMG2COL1_t;
typedef volatile union _DX_L2_NPU0_PE1_FMT_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_FMT*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_FMT_FIRST_LINE_OPT:2;  /* DX_L2_NPU0.PE1_FMT.PE1_FMT_FIRST_LINE_OPT [1:0] (RW, 0x0)  */
        volatile uint32_t PE1_FMT_FIRST_VALID_NUM:6;  /* DX_L2_NPU0.PE1_FMT.PE1_FMT_FIRST_VALID_NUM [7:2] (RW, 0x0)  */
        volatile uint32_t RESERVED0:23;  /* DX_L2_NPU0.PE1_FMT.RESERVED0 [30:8] (RW, 0x0)  */
        volatile uint32_t PE1_FMT_ENABLE:1;  /* DX_L2_NPU0.PE1_FMT.PE1_FMT_ENABLE [31:31] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_FMT_t;
typedef volatile union _DX_L2_NPU0_PE1_FMT_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_FMT_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_FMT_READ_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_FMT_CFG0.PE1_FMT_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_FMT_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE1_ADDR_GEN0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_ADDR_GEN0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM:16;  /* DX_L2_NPU0.PE1_ADDR_GEN0.PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:16;  /* DX_L2_NPU0.PE1_ADDR_GEN0.RESERVED0 [31:16] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_ADDR_GEN0_t;
typedef volatile union _DX_L2_NPU0_PE1_ADDR_GEN1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_ADDR_GEN1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_ADDR_GEN1.PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_ADDR_GEN1_t;
typedef volatile union _DX_L2_NPU0_PE1_ADDR_GEN2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_ADDR_GEN2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_BN_ADDR_GEN_MODE:2;  /* DX_L2_NPU0.PE1_ADDR_GEN2.PE1_BN_ADDR_GEN_MODE [1:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:30;  /* DX_L2_NPU0.PE1_ADDR_GEN2.RESERVED0 [31:2] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_ADDR_GEN2_t;
typedef volatile union _DX_L2_NPU0_PE1_ADDR_GEN3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_ADDR_GEN3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_BN_ADDR_GEN_MODE1_OFFSET:16;  /* DX_L2_NPU0.PE1_ADDR_GEN3.PE1_BN_ADDR_GEN_MODE1_OFFSET [15:0] (RW, 0x0)  */
        volatile uint32_t PE1_BN_ADDR_GEN_MODE1_MAX_NUM:16;  /* DX_L2_NPU0.PE1_ADDR_GEN3.PE1_BN_ADDR_GEN_MODE1_MAX_NUM [31:16] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_ADDR_GEN3_t;
typedef volatile union _DX_L2_NPU0_PE1_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_CONV_TYPE:8;  /* DX_L2_NPU0.PE1_CTRL.PE1_CONV_TYPE [7:0] (RW, 0x0) 0: STANDARD, 1: DEPTHWISE, 2: POINTWISE */
        volatile uint32_t PE1_CONV_EN:1;  /* DX_L2_NPU0.PE1_CTRL.PE1_CONV_EN [8:8] (RW, 0x0) 0: BYPASS, 1: OPERATION ENABLE */
        volatile uint32_t RESERVED0:7;  /* DX_L2_NPU0.PE1_CTRL.RESERVED0 [15:9] (RW, 0x0) - */
        volatile uint32_t PE1_SYSTOLIC_DEPTH:8;  /* DX_L2_NPU0.PE1_CTRL.PE1_SYSTOLIC_DEPTH [23:16] (RW, 0x0) SYSTOLIC ARRAY ENABLE DEPTH */
        volatile uint32_t PE1_SYSTOLIC_EN:1;  /* DX_L2_NPU0.PE1_CTRL.PE1_SYSTOLIC_EN [24:24] (RW, 0x0) 0: BYPASS, 1: OPERATION ENABLE (SYSTOLIC FUNCTION FOR FPGA) */
        volatile uint32_t PE1_SFUIN_VALID_NUM:7;  /* DX_L2_NPU0.PE1_CTRL.PE1_SFUIN_VALID_NUM [31:25] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_CTRL_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_LAST_CONV_NUM:16;  /* DX_L2_NPU0.PE1_CFG0.PE1_LAST_CONV_NUM [15:0] (RW, 0x0)  */
        volatile uint32_t PE1_LAST_CONV_MOD:8;  /* DX_L2_NPU0.PE1_CFG0.PE1_LAST_CONV_MOD [23:16] (RW, 0x0)  */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.PE1_CFG0.RESERVED0 [31:24] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_OFEATURE_CHANNEL:16;  /* DX_L2_NPU0.PE1_CFG1.PE1_OFEATURE_CHANNEL [15:0] (RW, 0x0)  */
        volatile uint32_t PE1_OFEATURE_NUMBER:15;  /* DX_L2_NPU0.PE1_CFG1.PE1_OFEATURE_NUMBER [30:16] (RW, 0x0)  */
        volatile uint32_t PE1_OFEATURE_DISABLE:1;  /* DX_L2_NPU0.PE1_CFG1.PE1_OFEATURE_DISABLE [31:31] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_OFEATURE_WIDTH:10;  /* DX_L2_NPU0.PE1_CFG2.PE1_OFEATURE_WIDTH [9:0] (RW, 0x0)  */
        volatile uint32_t PE1_OFEATURE_HEIGHT:10;  /* DX_L2_NPU0.PE1_CFG2.PE1_OFEATURE_HEIGHT [19:10] (RW, 0x0)  */
        volatile uint32_t RESERVED0:12;  /* DX_L2_NPU0.PE1_CFG2.RESERVED0 [31:20] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_FEATURE_CHANNEL:16;  /* DX_L2_NPU0.PE1_CFG3.PE1_FEATURE_CHANNEL [15:0] (RW, 0x0) INPUT FEATURE MAP CHANNEL, CONVOLUTION DIMENSIONS */
        volatile uint32_t RESERVED0:16;  /* DX_L2_NPU0.PE1_CFG3.RESERVED0 [31:16] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_FEATURE_WIDTH:10;  /* DX_L2_NPU0.PE1_CFG4.PE1_FEATURE_WIDTH [9:0] (RW, 0x0) INPUT FEATURE MAP WIDTH, CONVOLUTION DIMENSIONS */
        volatile uint32_t PE1_FEATURE_HEIGHT:10;  /* DX_L2_NPU0.PE1_CFG4.PE1_FEATURE_HEIGHT [19:10] (RW, 0x0) INPUT FEATURE MAP HEIGHT, CONVOLUTION DIMENSIONS */
        volatile uint32_t RESERVED0:12;  /* DX_L2_NPU0.PE1_CFG4.RESERVED0 [31:20] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG4_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG5_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_FILTER_CHANNEL:16;  /* DX_L2_NPU0.PE1_CFG5.PE1_FILTER_CHANNEL [15:0] (RW, 0x0) FILTER WINDOW DEPTH, CONVOLUTION DIMENSIONS */
        volatile uint32_t PE1_FILTER_NUMBER:16;  /* DX_L2_NPU0.PE1_CFG5.PE1_FILTER_NUMBER [31:16] (RW, 0x0) FILTER NUMBER, CONVOLUTION DIMENSIONS */
    };
} DX_L2_NPU0_PE1_CFG5_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG6_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_FILTER_WIDTH:4;  /* DX_L2_NPU0.PE1_CFG6.PE1_FILTER_WIDTH [3:0] (RW, 0x0) FILTER WINDOW WIDTH, CONVOLUTION DIMENSIONS */
        volatile uint32_t PE1_FILTER_HEIGHT:4;  /* DX_L2_NPU0.PE1_CFG6.PE1_FILTER_HEIGHT [7:4] (RW, 0x0) FILTER WINDOW HEIGHT, CONVOLUTION DIMENSIONS */
        volatile uint32_t RESERVED0:24;  /* DX_L2_NPU0.PE1_CFG6.RESERVED0 [31:8] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG6_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG7_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_PAD_TOP_SIZE:4;  /* DX_L2_NPU0.PE1_CFG7.PE1_PAD_TOP_SIZE [3:0] (RW, 0x0) TOP PADDING SIZE */
        volatile uint32_t PE1_PAD_BOTTOM_SIZE:4;  /* DX_L2_NPU0.PE1_CFG7.PE1_PAD_BOTTOM_SIZE [7:4] (RW, 0x0) BOTTOM PADDING SIZE */
        volatile uint32_t PE1_PAD_LEFT_SIZE:4;  /* DX_L2_NPU0.PE1_CFG7.PE1_PAD_LEFT_SIZE [11:8] (RW, 0x0) LEFT PADDING SIZE */
        volatile uint32_t PE1_PAD_RIGHT_SIZE:4;  /* DX_L2_NPU0.PE1_CFG7.PE1_PAD_RIGHT_SIZE [15:12] (RW, 0x0) RIGHT PADDING SIZE */
        volatile uint32_t PE1_PAD_VALUE:8;  /* DX_L2_NPU0.PE1_CFG7.PE1_PAD_VALUE [23:16] (RW, 0x0) TBD, PADDING VALUE */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.PE1_CFG7.RESERVED0 [31:24] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_CFG7_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG8_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_STRIDE_X:4;  /* DX_L2_NPU0.PE1_CFG8.PE1_STRIDE_X [3:0] (RW, 0x0) HORIZONTAL STRIDE NUMBER OF THE SLIDING WINDOW */
        volatile uint32_t PE1_STRIDE_Y:4;  /* DX_L2_NPU0.PE1_CFG8.PE1_STRIDE_Y [7:4] (RW, 0x0) VERTICAL STRIDE NUMBER OF THE SLIDING WINDOW */
        volatile uint32_t PE1_DILATION_X:4;  /* DX_L2_NPU0.PE1_CFG8.PE1_DILATION_X [11:8] (RW, 0x0) TBD, HORIZONTAL DILATION NUMBER OF THE SLIDING WINDOW */
        volatile uint32_t PE1_DILATION_Y:4;  /* DX_L2_NPU0.PE1_CFG8.PE1_DILATION_Y [15:12] (RW, 0x0) TBD, VERTICAL DILATION NUMBER OF THE SLIDING WINDOW */
        volatile uint32_t PE1_CHANNEL_OFFSET:16;  /* DX_L2_NPU0.PE1_CFG8.PE1_CHANNEL_OFFSET [31:16] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_CFG8_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG9_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_READ_FEATURE_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_CFG9.PE1_READ_FEATURE_BASE_ADDR [31:0] (RW, 0x0) READ FEATURE DATA SRAM BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_CFG9_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG10_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG10*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WRITE_FEATURE_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_CFG10.PE1_WRITE_FEATURE_BASE_ADDR [31:0] (RW, 0x0) WRITE FEATURE DATA SRAM BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_CFG10_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG11_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG11*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_READ_WEIGHT_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_CFG11.PE1_READ_WEIGHT_BASE_ADDR [31:0] (RW, 0x0) READ WEIGHT DATA SRAM BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_CFG11_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG12_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG12*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_BN_COEFF_READ_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_CFG12.PE1_BN_COEFF_READ_BASE_ADDR [31:0] (RW, 0x0) READ BN DATA SRAM BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_CFG12_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG13_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG13*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_BIAS_READ_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_CFG13.PE1_BIAS_READ_BASE_ADDR [31:0] (RW, 0x0) READ BIAS DATA SRAM BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_CFG13_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG14_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG14*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_MUL_READ_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_CFG14.PE1_MUL_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG14_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG15_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG15*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_ELEM_READ_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_CFG15.PE1_ELEM_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG15_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG16_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG16*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_ELEM_CHANNEL_OFFSET:16;  /* DX_L2_NPU0.PE1_CFG16.PE1_ELEM_CHANNEL_OFFSET [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:16;  /* DX_L2_NPU0.PE1_CFG16.RESERVED0 [31:16] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG16_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG17_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG17*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_ADDR_MODE1_LINE_SIZE:16;  /* DX_L2_NPU0.PE1_CFG17.PE1_WF_ADDR_MODE1_LINE_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:14;  /* DX_L2_NPU0.PE1_CFG17.RESERVED0 [29:16] (RW, 0x0)  */
        volatile uint32_t PE1_WF_ADDR_GEN_MODE:2;  /* DX_L2_NPU0.PE1_CFG17.PE1_WF_ADDR_GEN_MODE [31:30] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG17_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG18_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG18*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_ADDR_MODE1_LINE_STRIDE:32;  /* DX_L2_NPU0.PE1_CFG18.PE1_WF_ADDR_MODE1_LINE_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG18_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG19_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG19*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_ADDR_LINE_STRIDE:16;  /* DX_L2_NPU0.PE1_CFG19.PE1_WF_ADDR_LINE_STRIDE [15:0] (RW, 0x0) LINE STRIDE OF OUTPUT FEATURE ADDRESS */
        volatile uint32_t PE1_WF_ADDR_SURFACE_STRIDE:16;  /* DX_L2_NPU0.PE1_CFG19.PE1_WF_ADDR_SURFACE_STRIDE [31:16] (RW, 0x0) SURFACE STRIDE OF OUTPUT FEATURE ADDRESS */
    };
} DX_L2_NPU0_PE1_CFG19_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG20_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG20*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_WRITE_CNT:16;  /* DX_L2_NPU0.PE1_CFG20.PE1_WF_WRITE_CNT [15:0] (RW, 0xffff) COUNT BY UNIT WORD, LAYER OUTPUT SIZE */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.PE1_CFG20.RESERVED0 [23:16] (RW, 0x0) - */
        volatile uint32_t PE1_WF_RF2RF_TIME:8;  /* DX_L2_NPU0.PE1_CFG20.PE1_WF_RF2RF_TIME [31:24] (RW, 0x0) TBD, WRITE TIME */
    };
} DX_L2_NPU0_PE1_CFG20_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG21_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG21*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_RF2RF_SIZE:16;  /* DX_L2_NPU0.PE1_CFG21.PE1_WF_RF2RF_SIZE [15:0] (RW, 0x0) TBD, WRITE DATA SIZE */
        volatile uint32_t PE1_WF_RF2RF_ADDR_STRIDE:16;  /* DX_L2_NPU0.PE1_CFG21.PE1_WF_RF2RF_ADDR_STRIDE [31:16] (RW, 0x0) TBD, STRIDE VALUE OF SOURCE, DESTINATION ADDRESS */
    };
} DX_L2_NPU0_PE1_CFG21_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG22_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG22*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_RF2RF_SRC_ADDR:32;  /* DX_L2_NPU0.PE1_CFG22.PE1_WF_RF2RF_SRC_ADDR [31:0] (RW, 0x0) TBD */
    };
} DX_L2_NPU0_PE1_CFG22_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG23_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG23*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_RF2RF_DST_ADDR:32;  /* DX_L2_NPU0.PE1_CFG23.PE1_WF_RF2RF_DST_ADDR [31:0] (RW, 0x0) TBD */
    };
} DX_L2_NPU0_PE1_CFG23_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG24_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG24*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_SFU_OUT_DATA_VALID_NUM:3;  /* DX_L2_NPU0.PE1_CFG24.PE1_SFU_OUT_DATA_VALID_NUM [2:0] (RW, 0x0) SFU OUTPUT ENABLE LENGTH( 0 : OUT FEATURE BIT 8, 4: OUT FEATURE BIT 32) */
        volatile uint32_t RESERVED1:5;  /* DX_L2_NPU0.PE1_CFG24.RESERVED1 [7:3] (RW, 0x0) - */
        volatile uint32_t PE1_ST_ADDR_GEN_SHIFT_LEN:5;  /* DX_L2_NPU0.PE1_CFG24.PE1_ST_ADDR_GEN_SHIFT_LEN [12:8] (RW, 0x0) ADDRESS GEN DIVIDE PARAMETER  USING ONE FORMULA */
        volatile uint32_t RESERVED0:19;  /* DX_L2_NPU0.PE1_CFG24.RESERVED0 [31:13] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_CFG24_t;
typedef volatile union _DX_L2_NPU0_PE1_CFG25_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_CFG25*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_ST_ADDR_GEN_MUL:31;  /* DX_L2_NPU0.PE1_CFG25.PE1_ST_ADDR_GEN_MUL [30:0] (RW, 0x0) ADDRESS GEN MULTIPLIER PARAMETER USING ONE FORMULA */
        volatile uint32_t RESERVED0:1;  /* DX_L2_NPU0.PE1_CFG25.RESERVED0 [31:31] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_CFG25_t;
typedef volatile union _DX_L2_NPU0_PE1_VMEM_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_VMEM_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_START_NO_IN_BUFF:6;  /* DX_L2_NPU0.PE1_VMEM_CFG0.PE1_START_NO_IN_BUFF [5:0] (RW, 0x0) SRAM(WRITE INPUT FEATURE) START INDEX, USING DOUBLE BUFFER, IF NOT USE THEN NONE */
        volatile uint32_t RESERVED3:2;  /* DX_L2_NPU0.PE1_VMEM_CFG0.RESERVED3 [7:6] (RW, 0x0) - */
        volatile uint32_t PE1_START_NO_WEIGHT_BUFF:6;  /* DX_L2_NPU0.PE1_VMEM_CFG0.PE1_START_NO_WEIGHT_BUFF [13:8] (RW, 0x0) SRAM(WRITE WEIGHT) START INDEX, USING DOUBLE BUFFER, IF NOT USE THEN NONE */
        volatile uint32_t RESERVED2:2;  /* DX_L2_NPU0.PE1_VMEM_CFG0.RESERVED2 [15:14] (RW, 0x0) - */
        volatile uint32_t PE1_START_NO_OUT_BUFF:6;  /* DX_L2_NPU0.PE1_VMEM_CFG0.PE1_START_NO_OUT_BUFF [21:16] (RW, 0x0) SRAM(WRITE OUTPUT FEATURE) START INDEX, USING DOUBLE BUFFER, IF NOT USE THEN NONE */
        volatile uint32_t RESERVED1:2;  /* DX_L2_NPU0.PE1_VMEM_CFG0.RESERVED1 [23:22] (RW, 0x0) - */
        volatile uint32_t PE1_START_NO_ELEM_BUFF:6;  /* DX_L2_NPU0.PE1_VMEM_CFG0.PE1_START_NO_ELEM_BUFF [29:24] (RW, 0x0) SRAM(ELEMENTWISE ADD) START INDEX, USING DOUBLE BUFFER, IF NOT USE THEN NONE */
        volatile uint32_t RESERVED0:2;  /* DX_L2_NPU0.PE1_VMEM_CFG0.RESERVED0 [31:30] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_VMEM_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE1_SMEM_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_SMEM_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_BN_BUFF_SEL:1;  /* DX_L2_NPU0.PE1_SMEM_CFG0.PE1_BN_BUFF_SEL [0:0] (RW, 0x0) BN SRAM ADDRESS USED BY NPU */
        volatile uint32_t PE1_B_BUFF_SEL:1;  /* DX_L2_NPU0.PE1_SMEM_CFG0.PE1_B_BUFF_SEL [1:1] (RW, 0x0) BIAS SRAM ADDRESS USED BY NPU, CHOICE 0 OR 1 */
        volatile uint32_t PE1_MUL_BUFF_SEL:1;  /* DX_L2_NPU0.PE1_SMEM_CFG0.PE1_MUL_BUFF_SEL [2:2] (RW, 0x0) FLOATING POINT BN SRAM ADDRESS USED BY NPU, CHOICE 0 OR 1 */
        volatile uint32_t RESERVED0:29;  /* DX_L2_NPU0.PE1_SMEM_CFG0.RESERVED0 [31:3] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_SMEM_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_START_OP_EN:1;  /* DX_L2_NPU0.PE0_DMA_CTRL.PE0_START_OP_EN [0:0] (RW, 0x0) OPERATION ENABLE( 0 : BYPASS, 1 : CONVOLUTION OR POOLING) */
        volatile uint32_t PE0_DMA_CTRL_MODE:2;  /* DX_L2_NPU0.PE0_DMA_CTRL.PE0_DMA_CTRL_MODE [2:1] (RW, 0x0)  */
        volatile uint32_t RESERVED0:29;  /* DX_L2_NPU0.PE0_DMA_CTRL.RESERVED0 [31:3] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_DMA_CTRL_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_BN_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_BN_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_BN_TIME:6;  /* DX_L2_NPU0.PE0_DMA_BN_CFG0.PE0_RD_BN_TIME [5:0] (RW, 0x0) COUNT BY UNIT WORD(512 BIT) */
        volatile uint32_t PE0_RD_BN_MODE:2;  /* DX_L2_NPU0.PE0_DMA_BN_CFG0.PE0_RD_BN_MODE [7:6] (RW, 0x0) - */
        volatile uint32_t PE0_RD_BN_SEL:1;  /* DX_L2_NPU0.PE0_DMA_BN_CFG0.PE0_RD_BN_SEL [8:8] (RW, 0x0) BN DMA DESTINATION ADDRESS */
        volatile uint32_t PE0_RD_BN_EN:1;  /* DX_L2_NPU0.PE0_DMA_BN_CFG0.PE0_RD_BN_EN [9:9] (RW, 0x0) BN ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:22;  /* DX_L2_NPU0.PE0_DMA_BN_CFG0.RESERVED0 [31:10] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_DMA_BN_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_BN_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_BN_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_BN_STRIDE:16;  /* DX_L2_NPU0.PE0_DMA_BN_CFG1.PE0_RD_BN_STRIDE [15:0] (RW, 0x0) STRIDE OF BN ADDRESS */
        volatile uint32_t PE0_RD_BN_SIZE:12;  /* DX_L2_NPU0.PE0_DMA_BN_CFG1.PE0_RD_BN_SIZE [27:16] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t RESERVED0:4;  /* DX_L2_NPU0.PE0_DMA_BN_CFG1.RESERVED0 [31:28] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_DMA_BN_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_BN_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_BN_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_BN_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_BN_CFG2.PE0_RD_BN_BASE_ADDR [31:0] (RW, 0x0) BN DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_BN_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_BN_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_BN_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WT_BN_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_BN_CFG3.PE0_WT_BN_BASE_ADDR [31:0] (RW, 0x0) NOT USE */
    };
} DX_L2_NPU0_PE0_DMA_BN_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_B_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_B_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_B_TIME:6;  /* DX_L2_NPU0.PE0_DMA_B_CFG0.PE0_RD_B_TIME [5:0] (RW, 0x0) COUNT BY UNIT WORD(512 BIT) */
        volatile uint32_t PE0_RD_B_MODE:2;  /* DX_L2_NPU0.PE0_DMA_B_CFG0.PE0_RD_B_MODE [7:6] (RW, 0x0) - */
        volatile uint32_t PE0_RD_B_SEL:1;  /* DX_L2_NPU0.PE0_DMA_B_CFG0.PE0_RD_B_SEL [8:8] (RW, 0x0) BIAS DMA DESTINATION ADDRESS, CHOICE 0 OR 1 */
        volatile uint32_t PE0_RD_B_EN:1;  /* DX_L2_NPU0.PE0_DMA_B_CFG0.PE0_RD_B_EN [9:9] (RW, 0x0) BIAS ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:22;  /* DX_L2_NPU0.PE0_DMA_B_CFG0.RESERVED0 [31:10] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_DMA_B_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_B_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_B_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_B_STRIDE:16;  /* DX_L2_NPU0.PE0_DMA_B_CFG1.PE0_RD_B_STRIDE [15:0] (RW, 0x0) STRIDE OF BIAS ADDRESS */
        volatile uint32_t PE0_RD_B_SIZE:12;  /* DX_L2_NPU0.PE0_DMA_B_CFG1.PE0_RD_B_SIZE [27:16] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t RESERVED0:4;  /* DX_L2_NPU0.PE0_DMA_B_CFG1.RESERVED0 [31:28] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_DMA_B_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_B_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_B_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_B_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_B_CFG2.PE0_RD_B_BASE_ADDR [31:0] (RW, 0x0) BIAS DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_B_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_B_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_B_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WT_B_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_B_CFG3.PE0_WT_B_BASE_ADDR [31:0] (RW, 0x0) NOT USE */
    };
} DX_L2_NPU0_PE0_DMA_B_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_MUL_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_MUL_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_MUL_TIME:6;  /* DX_L2_NPU0.PE0_DMA_MUL_CFG0.PE0_RD_MUL_TIME [5:0] (RW, 0x0) COUNT BY UNIT WORD(512 BIT) */
        volatile uint32_t PE0_RD_MUL_MODE:2;  /* DX_L2_NPU0.PE0_DMA_MUL_CFG0.PE0_RD_MUL_MODE [7:6] (RW, 0x0) - */
        volatile uint32_t PE0_RD_MUL_SEL:1;  /* DX_L2_NPU0.PE0_DMA_MUL_CFG0.PE0_RD_MUL_SEL [8:8] (RW, 0x0) FLOATING POINT BN DMA DESTINATION ADDRESS, CHOICE 0 OR 1 */
        volatile uint32_t PE0_RD_MUL_EN:1;  /* DX_L2_NPU0.PE0_DMA_MUL_CFG0.PE0_RD_MUL_EN [9:9] (RW, 0x0) FLOATING POINT BN ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:22;  /* DX_L2_NPU0.PE0_DMA_MUL_CFG0.RESERVED0 [31:10] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_DMA_MUL_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_MUL_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_MUL_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_MUL_STRIDE:16;  /* DX_L2_NPU0.PE0_DMA_MUL_CFG1.PE0_RD_MUL_STRIDE [15:0] (RW, 0x0) STRIDE OF FLOATING POINT BN ADDRESS */
        volatile uint32_t PE0_RD_MUL_SIZE:12;  /* DX_L2_NPU0.PE0_DMA_MUL_CFG1.PE0_RD_MUL_SIZE [27:16] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t RESERVED0:4;  /* DX_L2_NPU0.PE0_DMA_MUL_CFG1.RESERVED0 [31:28] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_DMA_MUL_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_MUL_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_MUL_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_MUL_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_MUL_CFG2.PE0_RD_MUL_BASE_ADDR [31:0] (RW, 0x0) FLOATING POINT BN DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_MUL_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_MUL_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_MUL_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WT_MUL_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_MUL_CFG3.PE0_WT_MUL_BASE_ADDR [31:0] (RW, 0x0) NOT USE */
    };
} DX_L2_NPU0_PE0_DMA_MUL_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_ELEM_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_ELEM_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_ELEM_SIZE:16;  /* DX_L2_NPU0.PE0_DMA_ELEM_CFG0.PE0_RD_ELEM_SIZE [15:0] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t PE0_RD_ELEM_TIME:6;  /* DX_L2_NPU0.PE0_DMA_ELEM_CFG0.PE0_RD_ELEM_TIME [21:16] (RW, 0x0) COUNT BY UNIT WORD(64 CHANNEL) */
        volatile uint32_t PE0_RD_ELEM_MODE:2;  /* DX_L2_NPU0.PE0_DMA_ELEM_CFG0.PE0_RD_ELEM_MODE [23:22] (RW, 0x0) - */
        volatile uint32_t PE0_RD_ELEM_EN:1;  /* DX_L2_NPU0.PE0_DMA_ELEM_CFG0.PE0_RD_ELEM_EN [24:24] (RW, 0x0) ELEMENTWISE ADD ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:7;  /* DX_L2_NPU0.PE0_DMA_ELEM_CFG0.RESERVED0 [31:25] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_DMA_ELEM_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_ELEM_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_ELEM_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_ELEM_STRIDE:32;  /* DX_L2_NPU0.PE0_DMA_ELEM_CFG1.PE0_RD_ELEM_STRIDE [31:0] (RW, 0x0) STRIDE OF ELEMENTWISE ADD ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_ELEM_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_ELEM_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_ELEM_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_ELEM_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_ELEM_CFG2.PE0_RD_ELEM_BASE_ADDR [31:0] (RW, 0x0) ELEMENTWISE ADD DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_ELEM_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_ELEM_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_ELEM_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WT_ELEM_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_ELEM_CFG3.PE0_WT_ELEM_BASE_ADDR [31:0] (RW, 0x0) ELEMENTWISE ADD NPU BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_ELEM_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_W_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_W_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_W_TIME:12;  /* DX_L2_NPU0.PE0_DMA_W_CFG0.PE0_RD_W_TIME [11:0] (RW, 0x0) COUNT BY UNIT WORD(64 NUMBER) */
        volatile uint32_t PE0_RD_W_MODE:2;  /* DX_L2_NPU0.PE0_DMA_W_CFG0.PE0_RD_W_MODE [13:12] (RW, 0x0) - */
        volatile uint32_t PE0_RD_W_EN:1;  /* DX_L2_NPU0.PE0_DMA_W_CFG0.PE0_RD_W_EN [14:14] (RW, 0x0) WEIGHT ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:17;  /* DX_L2_NPU0.PE0_DMA_W_CFG0.RESERVED0 [31:15] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_DMA_W_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_W_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_W_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_W_STRIDE:16;  /* DX_L2_NPU0.PE0_DMA_W_CFG1.PE0_RD_W_STRIDE [15:0] (RW, 0x0) STRIDE OF WEIGHT ADDRESS */
        volatile uint32_t PE0_RD_W_SIZE:16;  /* DX_L2_NPU0.PE0_DMA_W_CFG1.PE0_RD_W_SIZE [31:16] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
    };
} DX_L2_NPU0_PE0_DMA_W_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_W_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_W_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_W_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_W_CFG2.PE0_RD_W_BASE_ADDR [31:0] (RW, 0x0) WEIGHT DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_W_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_W_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_W_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WT_W_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_W_CFG3.PE0_WT_W_BASE_ADDR [31:0] (RW, 0x0) WEIGHT NPU BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_W_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_INF_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_INF_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_INF_SIZE:16;  /* DX_L2_NPU0.PE0_DMA_INF_CFG0.PE0_RD_INF_SIZE [15:0] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t PE0_RD_INF_TIME:12;  /* DX_L2_NPU0.PE0_DMA_INF_CFG0.PE0_RD_INF_TIME [27:16] (RW, 0x0) COUNT BY UNIT WORD(64 CHANNEL) */
        volatile uint32_t PE0_RD_INF_MODE:2;  /* DX_L2_NPU0.PE0_DMA_INF_CFG0.PE0_RD_INF_MODE [29:28] (RW, 0x0) - */
        volatile uint32_t PE0_RD_INF_EN:1;  /* DX_L2_NPU0.PE0_DMA_INF_CFG0.PE0_RD_INF_EN [30:30] (RW, 0x0) INPUT FEATURE ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:1;  /* DX_L2_NPU0.PE0_DMA_INF_CFG0.RESERVED0 [31:31] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_DMA_INF_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_INF_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_INF_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_INF_STRIDE:32;  /* DX_L2_NPU0.PE0_DMA_INF_CFG1.PE0_RD_INF_STRIDE [31:0] (RW, 0x0) STRIDE OF INPUT FEATURE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_INF_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_INF_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_INF_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_INF_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_INF_CFG2.PE0_RD_INF_BASE_ADDR [31:0] (RW, 0x0) INPUT FEATURE DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_INF_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_INF_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_INF_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WT_INF_STRIDE:32;  /* DX_L2_NPU0.PE0_DMA_INF_CFG3.PE0_WT_INF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_DMA_INF_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_INF_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_INF_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WT_INF_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_INF_CFG4.PE0_WT_INF_BASE_ADDR [31:0] (RW, 0x0) INPUT FEATURE NPU BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_INF_CFG4_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_OUTF_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_OUTF_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WT_OUTF_SIZE:16;  /* DX_L2_NPU0.PE0_DMA_OUTF_CFG0.PE0_WT_OUTF_SIZE [15:0] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t PE0_WT_OUTF_TIME:12;  /* DX_L2_NPU0.PE0_DMA_OUTF_CFG0.PE0_WT_OUTF_TIME [27:16] (RW, 0x0) COUNT BY UNIT WORD(64 CHANNEL) */
        volatile uint32_t RESERVED1:2;  /* DX_L2_NPU0.PE0_DMA_OUTF_CFG0.RESERVED1 [29:28] (RW, 0x0) - */
        volatile uint32_t PE0_WT_OUTF_EN:1;  /* DX_L2_NPU0.PE0_DMA_OUTF_CFG0.PE0_WT_OUTF_EN [30:30] (RW, 0x0) OUTPUT FEATURE ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:1;  /* DX_L2_NPU0.PE0_DMA_OUTF_CFG0.RESERVED0 [31:31] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE0_DMA_OUTF_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_OUTF_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_OUTF_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WT_OUTF_STRIDE:32;  /* DX_L2_NPU0.PE0_DMA_OUTF_CFG1.PE0_WT_OUTF_STRIDE [31:0] (RW, 0x0) STRIDE OF OUTPUT FEATURE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_OUTF_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_OUTF_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_OUTF_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WT_OUTF_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_OUTF_CFG2.PE0_WT_OUTF_BASE_ADDR [31:0] (RW, 0x0) OUTPUT FEATURE DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_OUTF_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_OUTF_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_OUTF_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_OUTF_STRIDE:32;  /* DX_L2_NPU0.PE0_DMA_OUTF_CFG3.PE0_RD_OUTF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE0_DMA_OUTF_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE0_DMA_OUTF_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE0_DMA_OUTF_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_RD_OUTF_BASE_ADDR:32;  /* DX_L2_NPU0.PE0_DMA_OUTF_CFG4.PE0_RD_OUTF_BASE_ADDR [31:0] (RW, 0x0) OUTPUT FEATURE NPU BASE ADDRESS */
    };
} DX_L2_NPU0_PE0_DMA_OUTF_CFG4_t;
typedef volatile union _DX_L2_NPU0_RSVD5_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.RSVD5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED3:8;  /* DX_L2_NPU0.RSVD5.RESERVED3 [7:0] (RW, 0x0) - */
        volatile uint32_t RESERVED2:8;  /* DX_L2_NPU0.RSVD5.RESERVED2 [15:8] (RW, 0x0) - */
        volatile uint32_t RESERVED1:8;  /* DX_L2_NPU0.RSVD5.RESERVED1 [23:16] (RW, 0x0) - */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.RSVD5.RESERVED0 [31:24] (RW, 0x0) - */
    };
} DX_L2_NPU0_RSVD5_t;
typedef volatile union _DX_L2_NPU0_RSVD6_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.RSVD6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED3:8;  /* DX_L2_NPU0.RSVD6.RESERVED3 [7:0] (RW, 0x0) - */
        volatile uint32_t RESERVED2:8;  /* DX_L2_NPU0.RSVD6.RESERVED2 [15:8] (RW, 0x0) - */
        volatile uint32_t RESERVED1:8;  /* DX_L2_NPU0.RSVD6.RESERVED1 [23:16] (RW, 0x0) - */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.RSVD6.RESERVED0 [31:24] (RW, 0x0) - */
    };
} DX_L2_NPU0_RSVD6_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_START_OP_EN:1;  /* DX_L2_NPU0.PE1_DMA_CTRL.PE1_START_OP_EN [0:0] (RW, 0x0) OPERATION ENABLE( 0 : BYPASS, 1 : CONVOLUTION OR POOLING) */
        volatile uint32_t PE1_DMA_CTRL_MODE:2;  /* DX_L2_NPU0.PE1_DMA_CTRL.PE1_DMA_CTRL_MODE [2:1] (RW, 0x0)  */
        volatile uint32_t RESERVED0:29;  /* DX_L2_NPU0.PE1_DMA_CTRL.RESERVED0 [31:3] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_DMA_CTRL_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_BN_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_BN_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_BN_TIME:6;  /* DX_L2_NPU0.PE1_DMA_BN_CFG0.PE1_RD_BN_TIME [5:0] (RW, 0x0) COUNT BY UNIT WORD(512 BIT) */
        volatile uint32_t PE1_RD_BN_MODE:2;  /* DX_L2_NPU0.PE1_DMA_BN_CFG0.PE1_RD_BN_MODE [7:6] (RW, 0x0) - */
        volatile uint32_t PE1_RD_BN_SEL:1;  /* DX_L2_NPU0.PE1_DMA_BN_CFG0.PE1_RD_BN_SEL [8:8] (RW, 0x0) BN DMA DESTINATION ADDRESS, CHOICE 0 OR 1 */
        volatile uint32_t PE1_RD_BN_EN:1;  /* DX_L2_NPU0.PE1_DMA_BN_CFG0.PE1_RD_BN_EN [9:9] (RW, 0x0) BN ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:22;  /* DX_L2_NPU0.PE1_DMA_BN_CFG0.RESERVED0 [31:10] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_DMA_BN_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_BN_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_BN_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_BN_STRIDE:16;  /* DX_L2_NPU0.PE1_DMA_BN_CFG1.PE1_RD_BN_STRIDE [15:0] (RW, 0x0) STRIDE OF BN ADDRESS */
        volatile uint32_t PE1_RD_BN_SIZE:12;  /* DX_L2_NPU0.PE1_DMA_BN_CFG1.PE1_RD_BN_SIZE [27:16] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t RESERVED0:4;  /* DX_L2_NPU0.PE1_DMA_BN_CFG1.RESERVED0 [31:28] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_DMA_BN_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_BN_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_BN_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_BN_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_BN_CFG2.PE1_RD_BN_BASE_ADDR [31:0] (RW, 0x0) BN DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_BN_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_BN_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_BN_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WT_BN_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_BN_CFG3.PE1_WT_BN_BASE_ADDR [31:0] (RW, 0x0) NOT USE */
    };
} DX_L2_NPU0_PE1_DMA_BN_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_B_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_B_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_B_TIME:6;  /* DX_L2_NPU0.PE1_DMA_B_CFG0.PE1_RD_B_TIME [5:0] (RW, 0x0) COUNT BY UNIT WORD(512 BIT) */
        volatile uint32_t PE1_RD_B_MODE:2;  /* DX_L2_NPU0.PE1_DMA_B_CFG0.PE1_RD_B_MODE [7:6] (RW, 0x0) - */
        volatile uint32_t PE1_RD_B_SEL:1;  /* DX_L2_NPU0.PE1_DMA_B_CFG0.PE1_RD_B_SEL [8:8] (RW, 0x0) BIAS DMA DESTINATION ADDRESS, CHOICE 0 OR 1 */
        volatile uint32_t PE1_RD_B_EN:1;  /* DX_L2_NPU0.PE1_DMA_B_CFG0.PE1_RD_B_EN [9:9] (RW, 0x0) BIAS ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:22;  /* DX_L2_NPU0.PE1_DMA_B_CFG0.RESERVED0 [31:10] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_DMA_B_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_B_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_B_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_B_STRIDE:16;  /* DX_L2_NPU0.PE1_DMA_B_CFG1.PE1_RD_B_STRIDE [15:0] (RW, 0x0) STRIDE OF BIAS ADDRESS */
        volatile uint32_t PE1_RD_B_SIZE:12;  /* DX_L2_NPU0.PE1_DMA_B_CFG1.PE1_RD_B_SIZE [27:16] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t RESERVED0:4;  /* DX_L2_NPU0.PE1_DMA_B_CFG1.RESERVED0 [31:28] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_DMA_B_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_B_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_B_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_B_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_B_CFG2.PE1_RD_B_BASE_ADDR [31:0] (RW, 0x0) BIAS DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_B_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_B_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_B_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WT_B_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_B_CFG3.PE1_WT_B_BASE_ADDR [31:0] (RW, 0x0) NOT USE */
    };
} DX_L2_NPU0_PE1_DMA_B_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_MUL_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_MUL_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_MUL_TIME:6;  /* DX_L2_NPU0.PE1_DMA_MUL_CFG0.PE1_RD_MUL_TIME [5:0] (RW, 0x0) COUNT BY UNIT WORD(512 BIT) */
        volatile uint32_t PE1_RD_MUL_MODE:2;  /* DX_L2_NPU0.PE1_DMA_MUL_CFG0.PE1_RD_MUL_MODE [7:6] (RW, 0x0) - */
        volatile uint32_t PE1_RD_MUL_SEL:1;  /* DX_L2_NPU0.PE1_DMA_MUL_CFG0.PE1_RD_MUL_SEL [8:8] (RW, 0x0) FLOATING POINT BN DMA DESTINATION ADDRESS, CHOICE 0 OR 1 */
        volatile uint32_t PE1_RD_MUL_EN:1;  /* DX_L2_NPU0.PE1_DMA_MUL_CFG0.PE1_RD_MUL_EN [9:9] (RW, 0x0) FLOATING POINT BN ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:22;  /* DX_L2_NPU0.PE1_DMA_MUL_CFG0.RESERVED0 [31:10] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_DMA_MUL_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_MUL_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_MUL_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_MUL_STRIDE:16;  /* DX_L2_NPU0.PE1_DMA_MUL_CFG1.PE1_RD_MUL_STRIDE [15:0] (RW, 0x0) STRIDE OF FLOATING POINT BN ADDRESS */
        volatile uint32_t PE1_RD_MUL_SIZE:12;  /* DX_L2_NPU0.PE1_DMA_MUL_CFG1.PE1_RD_MUL_SIZE [27:16] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t RESERVED0:4;  /* DX_L2_NPU0.PE1_DMA_MUL_CFG1.RESERVED0 [31:28] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_DMA_MUL_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_MUL_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_MUL_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_MUL_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_MUL_CFG2.PE1_RD_MUL_BASE_ADDR [31:0] (RW, 0x0) FLOATING POINT BN DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_MUL_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_MUL_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_MUL_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WT_MUL_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_MUL_CFG3.PE1_WT_MUL_BASE_ADDR [31:0] (RW, 0x0) NOT USE */
    };
} DX_L2_NPU0_PE1_DMA_MUL_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_ELEM_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_ELEM_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_ELEM_SIZE:16;  /* DX_L2_NPU0.PE1_DMA_ELEM_CFG0.PE1_RD_ELEM_SIZE [15:0] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t PE1_RD_ELEM_TIME:6;  /* DX_L2_NPU0.PE1_DMA_ELEM_CFG0.PE1_RD_ELEM_TIME [21:16] (RW, 0x0) COUNT BY UNIT WORD(64 CHANNEL) */
        volatile uint32_t PE1_RD_ELEM_MODE:2;  /* DX_L2_NPU0.PE1_DMA_ELEM_CFG0.PE1_RD_ELEM_MODE [23:22] (RW, 0x0) - */
        volatile uint32_t PE1_RD_ELEM_EN:1;  /* DX_L2_NPU0.PE1_DMA_ELEM_CFG0.PE1_RD_ELEM_EN [24:24] (RW, 0x0) ELEMENTWISE ADD ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:7;  /* DX_L2_NPU0.PE1_DMA_ELEM_CFG0.RESERVED0 [31:25] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_DMA_ELEM_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_ELEM_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_ELEM_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_ELEM_STRIDE:32;  /* DX_L2_NPU0.PE1_DMA_ELEM_CFG1.PE1_RD_ELEM_STRIDE [31:0] (RW, 0x0) STRIDE OF ELEMENTWISE ADD ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_ELEM_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_ELEM_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_ELEM_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_ELEM_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_ELEM_CFG2.PE1_RD_ELEM_BASE_ADDR [31:0] (RW, 0x0) ELEMENTWISE ADD DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_ELEM_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_ELEM_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_ELEM_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WT_ELEM_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_ELEM_CFG3.PE1_WT_ELEM_BASE_ADDR [31:0] (RW, 0x0) ELEMENTWISE ADD NPU BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_ELEM_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_W_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_W_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_W_TIME:12;  /* DX_L2_NPU0.PE1_DMA_W_CFG0.PE1_RD_W_TIME [11:0] (RW, 0x0) COUNT BY UNIT WORD(64 NUMBER) */
        volatile uint32_t PE1_RD_W_MODE:2;  /* DX_L2_NPU0.PE1_DMA_W_CFG0.PE1_RD_W_MODE [13:12] (RW, 0x0) - */
        volatile uint32_t PE1_RD_W_EN:1;  /* DX_L2_NPU0.PE1_DMA_W_CFG0.PE1_RD_W_EN [14:14] (RW, 0x0) WEIGHT ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:17;  /* DX_L2_NPU0.PE1_DMA_W_CFG0.RESERVED0 [31:15] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_DMA_W_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_W_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_W_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_W_STRIDE:16;  /* DX_L2_NPU0.PE1_DMA_W_CFG1.PE1_RD_W_STRIDE [15:0] (RW, 0x0) STRIDE OF WEIGHT ADDRESS */
        volatile uint32_t PE1_RD_W_SIZE:16;  /* DX_L2_NPU0.PE1_DMA_W_CFG1.PE1_RD_W_SIZE [31:16] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
    };
} DX_L2_NPU0_PE1_DMA_W_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_W_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_W_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_W_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_W_CFG2.PE1_RD_W_BASE_ADDR [31:0] (RW, 0x0) WEIGHT DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_W_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_W_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_W_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WT_W_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_W_CFG3.PE1_WT_W_BASE_ADDR [31:0] (RW, 0x0) WEIGHT NPU BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_W_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_INF_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_INF_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_INF_SIZE:16;  /* DX_L2_NPU0.PE1_DMA_INF_CFG0.PE1_RD_INF_SIZE [15:0] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t PE1_RD_INF_TIME:12;  /* DX_L2_NPU0.PE1_DMA_INF_CFG0.PE1_RD_INF_TIME [27:16] (RW, 0x0) COUNT BY UNIT WORD(64 CHANNEL) */
        volatile uint32_t PE1_RD_INF_MODE:2;  /* DX_L2_NPU0.PE1_DMA_INF_CFG0.PE1_RD_INF_MODE [29:28] (RW, 0x0) - */
        volatile uint32_t PE1_RD_INF_EN:1;  /* DX_L2_NPU0.PE1_DMA_INF_CFG0.PE1_RD_INF_EN [30:30] (RW, 0x0) INPUT FEATURE ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:1;  /* DX_L2_NPU0.PE1_DMA_INF_CFG0.RESERVED0 [31:31] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_DMA_INF_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_INF_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_INF_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_INF_STRIDE:32;  /* DX_L2_NPU0.PE1_DMA_INF_CFG1.PE1_RD_INF_STRIDE [31:0] (RW, 0x0) STRIDE OF INPUT FEATURE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_INF_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_INF_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_INF_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_INF_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_INF_CFG2.PE1_RD_INF_BASE_ADDR [31:0] (RW, 0x0) INPUT FEATURE DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_INF_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_INF_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_INF_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WT_INF_STRIDE:32;  /* DX_L2_NPU0.PE1_DMA_INF_CFG3.PE1_WT_INF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_DMA_INF_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_INF_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_INF_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WT_INF_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_INF_CFG4.PE1_WT_INF_BASE_ADDR [31:0] (RW, 0x0) INPUT FEATURE NPU BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_INF_CFG4_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_OUTF_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_OUTF_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WT_OUTF_SIZE:16;  /* DX_L2_NPU0.PE1_DMA_OUTF_CFG0.PE1_WT_OUTF_SIZE [15:0] (RW, 0x0) SIZE BY UNIT SEGMENT(512 BIT) */
        volatile uint32_t PE1_WT_OUTF_TIME:12;  /* DX_L2_NPU0.PE1_DMA_OUTF_CFG0.PE1_WT_OUTF_TIME [27:16] (RW, 0x0) COUNT BY UNIT WORD(64 CHANNEL) */
        volatile uint32_t RESERVED1:2;  /* DX_L2_NPU0.PE1_DMA_OUTF_CFG0.RESERVED1 [29:28] (RW, 0x0) - */
        volatile uint32_t PE1_WT_OUTF_EN:1;  /* DX_L2_NPU0.PE1_DMA_OUTF_CFG0.PE1_WT_OUTF_EN [30:30] (RW, 0x0) OUTPUT FEATURE ENABLE( 0 : BYPASS, 1 : ENABLE) */
        volatile uint32_t RESERVED0:1;  /* DX_L2_NPU0.PE1_DMA_OUTF_CFG0.RESERVED0 [31:31] (RW, 0x0) - */
    };
} DX_L2_NPU0_PE1_DMA_OUTF_CFG0_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_OUTF_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_OUTF_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WT_OUTF_STRIDE:32;  /* DX_L2_NPU0.PE1_DMA_OUTF_CFG1.PE1_WT_OUTF_STRIDE [31:0] (RW, 0x0) STRIDE OF OUTPUT FEATURE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_OUTF_CFG1_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_OUTF_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_OUTF_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WT_OUTF_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_OUTF_CFG2.PE1_WT_OUTF_BASE_ADDR [31:0] (RW, 0x0) OUTPUT FEATURE DDR BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_OUTF_CFG2_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_OUTF_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_OUTF_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_OUTF_STRIDE:32;  /* DX_L2_NPU0.PE1_DMA_OUTF_CFG3.PE1_RD_OUTF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_L2_NPU0_PE1_DMA_OUTF_CFG3_t;
typedef volatile union _DX_L2_NPU0_PE1_DMA_OUTF_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.PE1_DMA_OUTF_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_RD_OUTF_BASE_ADDR:32;  /* DX_L2_NPU0.PE1_DMA_OUTF_CFG4.PE1_RD_OUTF_BASE_ADDR [31:0] (RW, 0x0) OUTPUT FEATURE NPU BASE ADDRESS */
    };
} DX_L2_NPU0_PE1_DMA_OUTF_CFG4_t;
typedef volatile union _DX_L2_NPU0_RSVD7_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.RSVD7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED3:8;  /* DX_L2_NPU0.RSVD7.RESERVED3 [7:0] (RW, 0x0) - */
        volatile uint32_t RESERVED2:8;  /* DX_L2_NPU0.RSVD7.RESERVED2 [15:8] (RW, 0x0) - */
        volatile uint32_t RESERVED1:8;  /* DX_L2_NPU0.RSVD7.RESERVED1 [23:16] (RW, 0x0) - */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.RSVD7.RESERVED0 [31:24] (RW, 0x0) - */
    };
} DX_L2_NPU0_RSVD7_t;
typedef volatile union _DX_L2_NPU0_RSVD8_t
{
    volatile uint32_t U;  /* Register-Access : DX_L2_NPU0.RSVD8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED3:8;  /* DX_L2_NPU0.RSVD8.RESERVED3 [7:0] (RW, 0x0) - */
        volatile uint32_t RESERVED2:8;  /* DX_L2_NPU0.RSVD8.RESERVED2 [15:8] (RW, 0x0) - */
        volatile uint32_t RESERVED1:8;  /* DX_L2_NPU0.RSVD8.RESERVED1 [23:16] (RW, 0x0) - */
        volatile uint32_t RESERVED0:8;  /* DX_L2_NPU0.RSVD8.RESERVED0 [31:24] (RW, 0x0) - */
    };
} DX_L2_NPU0_RSVD8_t;

/* DX_L2_NPU0 Module Definition */
typedef volatile struct _dxDX_L2_NPU0_t
{
    DX_L2_NPU0_DATA_CFG0_t DATA_CFG0;  /* NPU0 0x0 */
    DX_L2_NPU0_DATA_CFG1_t DATA_CFG1;  /* NPU0 0x4 */
    DX_L2_NPU0_IM2COL_t IM2COL;  /* NPU0 0x8 */
    DX_L2_NPU0_CTRL_t CTRL;  /* NPU0 0xc */
    DX_L2_NPU0_MISC_t MISC;  /* NPU0 0x10 */
    DX_L2_NPU0_ACTIVE_t ACTIVE;  /* NPU0 0x14 */
    DX_L2_NPU0_RSVD0_t RSVD0;  /* NPU0 0x18 */
    DX_L2_NPU0_SFU_CTRL_t SFU_CTRL;  /* NPU0 0x1c */
    DX_L2_NPU0_SFU_CLIP_MAX_t SFU_CLIP_MAX;  /* NPU0 0x20 */
    DX_L2_NPU0_SFU_CLIP_MIN_t SFU_CLIP_MIN;  /* NPU0 0x24 */
    DX_L2_NPU0_SFU_COEFF0_t SFU_COEFF0;  /* NPU0 0x28 */
    DX_L2_NPU0_SFU_COEFF1_t SFU_COEFF1;  /* NPU0 0x2c */
    DX_L2_NPU0_SFU_COEFF2_t SFU_COEFF2;  /* NPU0 0x30 */
    DX_L2_NPU0_SFU_COEFF3_t SFU_COEFF3;  /* NPU0 0x34 */
    DX_L2_NPU0_SFU_COEFF4_t SFU_COEFF4;  /* NPU0 0x38 */
    DX_L2_NPU0_SFU_COEFF5_t SFU_COEFF5;  /* NPU0 0x3c */
    DX_L2_NPU0_SFU_COEFF6_t SFU_COEFF6;  /* NPU0 0x40 */
    DX_L2_NPU0_SFU_COEFF7_t SFU_COEFF7;  /* NPU0 0x44 */
    DX_L2_NPU0_SFU_COEFF8_t SFU_COEFF8;  /* NPU0 0x48 */
    DX_L2_NPU0_SFU_COEFF9_t SFU_COEFF9;  /* NPU0 0x4c */
    DX_L2_NPU0_SFU_COEFF10_t SFU_COEFF10;  /* NPU0 0x50 */
    DX_L2_NPU0_SFU_COEFF11_t SFU_COEFF11;  /* NPU0 0x54 */
    DX_L2_NPU0_SFU_COEFF12_t SFU_COEFF12;  /* NPU0 0x58 */
    DX_L2_NPU0_SFU_COEFF13_t SFU_COEFF13;  /* NPU0 0x5c */
    DX_L2_NPU0_SFU_COEFF14_t SFU_COEFF14;  /* NPU0 0x60 */
    DX_L2_NPU0_SFU_COEFF15_t SFU_COEFF15;  /* NPU0 0x64 */
    DX_L2_NPU0_SFU_COEFF16_t SFU_COEFF16;  /* NPU0 0x68 */
    DX_L2_NPU0_SFU_COEFF17_t SFU_COEFF17;  /* NPU0 0x6c */
    DX_L2_NPU0_SFU_COEFF18_t SFU_COEFF18;  /* NPU0 0x70 */
    DX_L2_NPU0_SFU_COEFF19_t SFU_COEFF19;  /* NPU0 0x74 */
    DX_L2_NPU0_SFU_COEFF20_t SFU_COEFF20;  /* NPU0 0x78 */
    DX_L2_NPU0_SFU_COEFF21_t SFU_COEFF21;  /* NPU0 0x7c */
    DX_L2_NPU0_SFU_COEFF22_t SFU_COEFF22;  /* NPU0 0x80 */
    DX_L2_NPU0_SFU_COEFF23_t SFU_COEFF23;  /* NPU0 0x84 */
    DX_L2_NPU0_SFU_COEFF24_t SFU_COEFF24;  /* NPU0 0x88 */
    DX_L2_NPU0_SFU_COEFF25_t SFU_COEFF25;  /* NPU0 0x8c */
    DX_L2_NPU0_SFU_COEFF26_t SFU_COEFF26;  /* NPU0 0x90 */
    DX_L2_NPU0_SFU_COEFF27_t SFU_COEFF27;  /* NPU0 0x94 */
    DX_L2_NPU0_SFU_COEFF28_t SFU_COEFF28;  /* NPU0 0x98 */
    DX_L2_NPU0_SFU_COEFF29_t SFU_COEFF29;  /* NPU0 0x9c */
    DX_L2_NPU0_SFU_COEFF30_t SFU_COEFF30;  /* NPU0 0xa0 */
    DX_L2_NPU0_SFU_COEFF31_t SFU_COEFF31;  /* NPU0 0xa4 */
    DX_L2_NPU0_SFU_COEFF32_t SFU_COEFF32;  /* NPU0 0xa8 */
    DX_L2_NPU0_SFU_COEFF33_t SFU_COEFF33;  /* NPU0 0xac */
    DX_L2_NPU0_SFU_COEFF34_t SFU_COEFF34;  /* NPU0 0xb0 */
    DX_L2_NPU0_SFU_COEFF35_t SFU_COEFF35;  /* NPU0 0xb4 */
    DX_L2_NPU0_SFU_COEFF36_t SFU_COEFF36;  /* NPU0 0xb8 */
    DX_L2_NPU0_SFU_COEFF37_t SFU_COEFF37;  /* NPU0 0xbc */
    DX_L2_NPU0_SFU_COEFF38_t SFU_COEFF38;  /* NPU0 0xc0 */
    DX_L2_NPU0_SFU_COEFF39_t SFU_COEFF39;  /* NPU0 0xc4 */
    DX_L2_NPU0_SFU_COEFF40_t SFU_COEFF40;  /* NPU0 0xc8 */
    DX_L2_NPU0_SFU_COEFF41_t SFU_COEFF41;  /* NPU0 0xcc */
    DX_L2_NPU0_SFU_COEFF42_t SFU_COEFF42;  /* NPU0 0xd0 */
    DX_L2_NPU0_SFU_COEFF43_t SFU_COEFF43;  /* NPU0 0xd4 */
    DX_L2_NPU0_SFU_COEFF44_t SFU_COEFF44;  /* NPU0 0xd8 */
    DX_L2_NPU0_SFU_COEFF45_t SFU_COEFF45;  /* NPU0 0xdc */
    DX_L2_NPU0_SFU_COEFF46_t SFU_COEFF46;  /* NPU0 0xe0 */
    DX_L2_NPU0_SFU_COEFF47_t SFU_COEFF47;  /* NPU0 0xe4 */
    DX_L2_NPU0_SFU_COEFF48_t SFU_COEFF48;  /* NPU0 0xe8 */
    DX_L2_NPU0_SFU_COEFF49_t SFU_COEFF49;  /* NPU0 0xec */
    DX_L2_NPU0_SFU_COEFF50_t SFU_COEFF50;  /* NPU0 0xf0 */
    DX_L2_NPU0_SFU_COEFF51_t SFU_COEFF51;  /* NPU0 0xf4 */
    DX_L2_NPU0_SFU_COEFF52_t SFU_COEFF52;  /* NPU0 0xf8 */
    DX_L2_NPU0_SFU_COEFF53_t SFU_COEFF53;  /* NPU0 0xfc */
    DX_L2_NPU0_SFU_COEFF54_t SFU_COEFF54;  /* NPU0 0x100 */
    DX_L2_NPU0_RSVD1_t RSVD1;  /* NPU0 0x104 */
    DX_L2_NPU0_RSVD2_t RSVD2;  /* NPU0 0x108 */
    DX_L2_NPU0_PE0_IMG2COL0_t PE0_IMG2COL0;  /* NPU0 0x10c */
    DX_L2_NPU0_PE0_IMG2COL1_t PE0_IMG2COL1;  /* NPU0 0x110 */
    DX_L2_NPU0_PE0_FMT_t PE0_FMT;  /* NPU0 0x114 */
    DX_L2_NPU0_PE0_FMT_CFG0_t PE0_FMT_CFG0;  /* NPU0 0x118 */
    DX_L2_NPU0_PE0_ADDR_GEN0_t PE0_ADDR_GEN0;  /* NPU0 0x11c */
    DX_L2_NPU0_PE0_ADDR_GEN1_t PE0_ADDR_GEN1;  /* NPU0 0x120 */
    DX_L2_NPU0_PE0_ADDR_GEN2_t PE0_ADDR_GEN2;  /* NPU0 0x124 */
    DX_L2_NPU0_PE0_ADDR_GEN3_t PE0_ADDR_GEN3;  /* NPU0 0x128 */
    DX_L2_NPU0_PE0_CTRL_t PE0_CTRL;  /* NPU0 0x12c */
    DX_L2_NPU0_PE0_CFG0_t PE0_CFG0;  /* NPU0 0x130 */
    DX_L2_NPU0_PE0_CFG1_t PE0_CFG1;  /* NPU0 0x134 */
    DX_L2_NPU0_PE0_CFG2_t PE0_CFG2;  /* NPU0 0x138 */
    DX_L2_NPU0_PE0_CFG3_t PE0_CFG3;  /* NPU0 0x13c */
    DX_L2_NPU0_PE0_CFG4_t PE0_CFG4;  /* NPU0 0x140 */
    DX_L2_NPU0_PE0_CFG5_t PE0_CFG5;  /* NPU0 0x144 */
    DX_L2_NPU0_PE0_CFG6_t PE0_CFG6;  /* NPU0 0x148 */
    DX_L2_NPU0_PE0_CFG7_t PE0_CFG7;  /* NPU0 0x14c */
    DX_L2_NPU0_PE0_CFG8_t PE0_CFG8;  /* NPU0 0x150 */
    DX_L2_NPU0_PE0_CFG9_t PE0_CFG9;  /* NPU0 0x154 */
    DX_L2_NPU0_PE0_CFG10_t PE0_CFG10;  /* NPU0 0x158 */
    DX_L2_NPU0_PE0_CFG11_t PE0_CFG11;  /* NPU0 0x15c */
    DX_L2_NPU0_PE0_CFG12_t PE0_CFG12;  /* NPU0 0x160 */
    DX_L2_NPU0_PE0_CFG13_t PE0_CFG13;  /* NPU0 0x164 */
    DX_L2_NPU0_PE0_CFG14_t PE0_CFG14;  /* NPU0 0x168 */
    DX_L2_NPU0_PE0_CFG15_t PE0_CFG15;  /* NPU0 0x16c */
    DX_L2_NPU0_PE0_CFG16_t PE0_CFG16;  /* NPU0 0x170 */
    DX_L2_NPU0_PE0_CFG17_t PE0_CFG17;  /* NPU0 0x174 */
    DX_L2_NPU0_PE0_CFG18_t PE0_CFG18;  /* NPU0 0x178 */
    DX_L2_NPU0_PE0_CFG19_t PE0_CFG19;  /* NPU0 0x17c */
    DX_L2_NPU0_PE0_CFG20_t PE0_CFG20;  /* NPU0 0x180 */
    DX_L2_NPU0_PE0_CFG21_t PE0_CFG21;  /* NPU0 0x184 */
    DX_L2_NPU0_PE0_CFG22_t PE0_CFG22;  /* NPU0 0x188 */
    DX_L2_NPU0_PE0_CFG23_t PE0_CFG23;  /* NPU0 0x18c */
    DX_L2_NPU0_PE0_CFG24_t PE0_CFG24;  /* NPU0 0x190 */
    DX_L2_NPU0_PE0_CFG25_t PE0_CFG25;  /* NPU0 0x194 */
    DX_L2_NPU0_PE0_VMEM_CFG0_t PE0_VMEM_CFG0;  /* NPU0 0x198 */
    DX_L2_NPU0_PE0_SMEM_CFG0_t PE0_SMEM_CFG0;  /* NPU0 0x19c */
    DX_L2_NPU0_PE1_IMG2COL0_t PE1_IMG2COL0;  /* NPU0 0x1a0 */
    DX_L2_NPU0_PE1_IMG2COL1_t PE1_IMG2COL1;  /* NPU0 0x1a4 */
    DX_L2_NPU0_PE1_FMT_t PE1_FMT;  /* NPU0 0x1a8 */
    DX_L2_NPU0_PE1_FMT_CFG0_t PE1_FMT_CFG0;  /* NPU0 0x1ac */
    DX_L2_NPU0_PE1_ADDR_GEN0_t PE1_ADDR_GEN0;  /* NPU0 0x1b0 */
    DX_L2_NPU0_PE1_ADDR_GEN1_t PE1_ADDR_GEN1;  /* NPU0 0x1b4 */
    DX_L2_NPU0_PE1_ADDR_GEN2_t PE1_ADDR_GEN2;  /* NPU0 0x1b8 */
    DX_L2_NPU0_PE1_ADDR_GEN3_t PE1_ADDR_GEN3;  /* NPU0 0x1bc */
    DX_L2_NPU0_PE1_CTRL_t PE1_CTRL;  /* NPU0 0x1c0 */
    DX_L2_NPU0_PE1_CFG0_t PE1_CFG0;  /* NPU0 0x1c4 */
    DX_L2_NPU0_PE1_CFG1_t PE1_CFG1;  /* NPU0 0x1c8 */
    DX_L2_NPU0_PE1_CFG2_t PE1_CFG2;  /* NPU0 0x1cc */
    DX_L2_NPU0_PE1_CFG3_t PE1_CFG3;  /* NPU0 0x1d0 */
    DX_L2_NPU0_PE1_CFG4_t PE1_CFG4;  /* NPU0 0x1d4 */
    DX_L2_NPU0_PE1_CFG5_t PE1_CFG5;  /* NPU0 0x1d8 */
    DX_L2_NPU0_PE1_CFG6_t PE1_CFG6;  /* NPU0 0x1dc */
    DX_L2_NPU0_PE1_CFG7_t PE1_CFG7;  /* NPU0 0x1e0 */
    DX_L2_NPU0_PE1_CFG8_t PE1_CFG8;  /* NPU0 0x1e4 */
    DX_L2_NPU0_PE1_CFG9_t PE1_CFG9;  /* NPU0 0x1e8 */
    DX_L2_NPU0_PE1_CFG10_t PE1_CFG10;  /* NPU0 0x1ec */
    DX_L2_NPU0_PE1_CFG11_t PE1_CFG11;  /* NPU0 0x1f0 */
    DX_L2_NPU0_PE1_CFG12_t PE1_CFG12;  /* NPU0 0x1f4 */
    DX_L2_NPU0_PE1_CFG13_t PE1_CFG13;  /* NPU0 0x1f8 */
    DX_L2_NPU0_PE1_CFG14_t PE1_CFG14;  /* NPU0 0x1fc */
    DX_L2_NPU0_PE1_CFG15_t PE1_CFG15;  /* NPU0 0x200 */
    DX_L2_NPU0_PE1_CFG16_t PE1_CFG16;  /* NPU0 0x204 */
    DX_L2_NPU0_PE1_CFG17_t PE1_CFG17;  /* NPU0 0x208 */
    DX_L2_NPU0_PE1_CFG18_t PE1_CFG18;  /* NPU0 0x20c */
    DX_L2_NPU0_PE1_CFG19_t PE1_CFG19;  /* NPU0 0x210 */
    DX_L2_NPU0_PE1_CFG20_t PE1_CFG20;  /* NPU0 0x214 */
    DX_L2_NPU0_PE1_CFG21_t PE1_CFG21;  /* NPU0 0x218 */
    DX_L2_NPU0_PE1_CFG22_t PE1_CFG22;  /* NPU0 0x21c */
    DX_L2_NPU0_PE1_CFG23_t PE1_CFG23;  /* NPU0 0x220 */
    DX_L2_NPU0_PE1_CFG24_t PE1_CFG24;  /* NPU0 0x224 */
    DX_L2_NPU0_PE1_CFG25_t PE1_CFG25;  /* NPU0 0x228 */
    DX_L2_NPU0_PE1_VMEM_CFG0_t PE1_VMEM_CFG0;  /* NPU0 0x22c */
    DX_L2_NPU0_PE1_SMEM_CFG0_t PE1_SMEM_CFG0;  /* NPU0 0x230 */
    DX_L2_NPU0_PE0_DMA_CTRL_t PE0_DMA_CTRL;  /* NPU0 0x234 */
    DX_L2_NPU0_PE0_DMA_BN_CFG0_t PE0_DMA_BN_CFG0;  /* NPU0 0x238 */
    DX_L2_NPU0_PE0_DMA_BN_CFG1_t PE0_DMA_BN_CFG1;  /* NPU0 0x23c */
    DX_L2_NPU0_PE0_DMA_BN_CFG2_t PE0_DMA_BN_CFG2;  /* NPU0 0x240 */
    DX_L2_NPU0_PE0_DMA_BN_CFG3_t PE0_DMA_BN_CFG3;  /* NPU0 0x244 */
    DX_L2_NPU0_PE0_DMA_B_CFG0_t PE0_DMA_B_CFG0;  /* NPU0 0x248 */
    DX_L2_NPU0_PE0_DMA_B_CFG1_t PE0_DMA_B_CFG1;  /* NPU0 0x24c */
    DX_L2_NPU0_PE0_DMA_B_CFG2_t PE0_DMA_B_CFG2;  /* NPU0 0x250 */
    DX_L2_NPU0_PE0_DMA_B_CFG3_t PE0_DMA_B_CFG3;  /* NPU0 0x254 */
    DX_L2_NPU0_PE0_DMA_MUL_CFG0_t PE0_DMA_MUL_CFG0;  /* NPU0 0x258 */
    DX_L2_NPU0_PE0_DMA_MUL_CFG1_t PE0_DMA_MUL_CFG1;  /* NPU0 0x25c */
    DX_L2_NPU0_PE0_DMA_MUL_CFG2_t PE0_DMA_MUL_CFG2;  /* NPU0 0x260 */
    DX_L2_NPU0_PE0_DMA_MUL_CFG3_t PE0_DMA_MUL_CFG3;  /* NPU0 0x264 */
    DX_L2_NPU0_PE0_DMA_ELEM_CFG0_t PE0_DMA_ELEM_CFG0;  /* NPU0 0x268 */
    DX_L2_NPU0_PE0_DMA_ELEM_CFG1_t PE0_DMA_ELEM_CFG1;  /* NPU0 0x26c */
    DX_L2_NPU0_PE0_DMA_ELEM_CFG2_t PE0_DMA_ELEM_CFG2;  /* NPU0 0x270 */
    DX_L2_NPU0_PE0_DMA_ELEM_CFG3_t PE0_DMA_ELEM_CFG3;  /* NPU0 0x274 */
    DX_L2_NPU0_PE0_DMA_W_CFG0_t PE0_DMA_W_CFG0;  /* NPU0 0x278 */
    DX_L2_NPU0_PE0_DMA_W_CFG1_t PE0_DMA_W_CFG1;  /* NPU0 0x27c */
    DX_L2_NPU0_PE0_DMA_W_CFG2_t PE0_DMA_W_CFG2;  /* NPU0 0x280 */
    DX_L2_NPU0_PE0_DMA_W_CFG3_t PE0_DMA_W_CFG3;  /* NPU0 0x284 */
    DX_L2_NPU0_PE0_DMA_INF_CFG0_t PE0_DMA_INF_CFG0;  /* NPU0 0x288 */
    DX_L2_NPU0_PE0_DMA_INF_CFG1_t PE0_DMA_INF_CFG1;  /* NPU0 0x28c */
    DX_L2_NPU0_PE0_DMA_INF_CFG2_t PE0_DMA_INF_CFG2;  /* NPU0 0x290 */
    DX_L2_NPU0_PE0_DMA_INF_CFG3_t PE0_DMA_INF_CFG3;  /* NPU0 0x294 */
    DX_L2_NPU0_PE0_DMA_INF_CFG4_t PE0_DMA_INF_CFG4;  /* NPU0 0x298 */
    DX_L2_NPU0_PE0_DMA_OUTF_CFG0_t PE0_DMA_OUTF_CFG0;  /* NPU0 0x29c */
    DX_L2_NPU0_PE0_DMA_OUTF_CFG1_t PE0_DMA_OUTF_CFG1;  /* NPU0 0x2a0 */
    DX_L2_NPU0_PE0_DMA_OUTF_CFG2_t PE0_DMA_OUTF_CFG2;  /* NPU0 0x2a4 */
    DX_L2_NPU0_PE0_DMA_OUTF_CFG3_t PE0_DMA_OUTF_CFG3;  /* NPU0 0x2a8 */
    DX_L2_NPU0_PE0_DMA_OUTF_CFG4_t PE0_DMA_OUTF_CFG4;  /* NPU0 0x2ac */
    DX_L2_NPU0_RSVD5_t RSVD5;  /* NPU0 0x2b0 */
    DX_L2_NPU0_RSVD6_t RSVD6;  /* NPU0 0x2b4 */
    DX_L2_NPU0_PE1_DMA_CTRL_t PE1_DMA_CTRL;  /* NPU0 0x2b8 */
    DX_L2_NPU0_PE1_DMA_BN_CFG0_t PE1_DMA_BN_CFG0;  /* NPU0 0x2bc */
    DX_L2_NPU0_PE1_DMA_BN_CFG1_t PE1_DMA_BN_CFG1;  /* NPU0 0x2c0 */
    DX_L2_NPU0_PE1_DMA_BN_CFG2_t PE1_DMA_BN_CFG2;  /* NPU0 0x2c4 */
    DX_L2_NPU0_PE1_DMA_BN_CFG3_t PE1_DMA_BN_CFG3;  /* NPU0 0x2c8 */
    DX_L2_NPU0_PE1_DMA_B_CFG0_t PE1_DMA_B_CFG0;  /* NPU0 0x2cc */
    DX_L2_NPU0_PE1_DMA_B_CFG1_t PE1_DMA_B_CFG1;  /* NPU0 0x2d0 */
    DX_L2_NPU0_PE1_DMA_B_CFG2_t PE1_DMA_B_CFG2;  /* NPU0 0x2d4 */
    DX_L2_NPU0_PE1_DMA_B_CFG3_t PE1_DMA_B_CFG3;  /* NPU0 0x2d8 */
    DX_L2_NPU0_PE1_DMA_MUL_CFG0_t PE1_DMA_MUL_CFG0;  /* NPU0 0x2dc */
    DX_L2_NPU0_PE1_DMA_MUL_CFG1_t PE1_DMA_MUL_CFG1;  /* NPU0 0x2e0 */
    DX_L2_NPU0_PE1_DMA_MUL_CFG2_t PE1_DMA_MUL_CFG2;  /* NPU0 0x2e4 */
    DX_L2_NPU0_PE1_DMA_MUL_CFG3_t PE1_DMA_MUL_CFG3;  /* NPU0 0x2e8 */
    DX_L2_NPU0_PE1_DMA_ELEM_CFG0_t PE1_DMA_ELEM_CFG0;  /* NPU0 0x2ec */
    DX_L2_NPU0_PE1_DMA_ELEM_CFG1_t PE1_DMA_ELEM_CFG1;  /* NPU0 0x2f0 */
    DX_L2_NPU0_PE1_DMA_ELEM_CFG2_t PE1_DMA_ELEM_CFG2;  /* NPU0 0x2f4 */
    DX_L2_NPU0_PE1_DMA_ELEM_CFG3_t PE1_DMA_ELEM_CFG3;  /* NPU0 0x2f8 */
    DX_L2_NPU0_PE1_DMA_W_CFG0_t PE1_DMA_W_CFG0;  /* NPU0 0x2fc */
    DX_L2_NPU0_PE1_DMA_W_CFG1_t PE1_DMA_W_CFG1;  /* NPU0 0x300 */
    DX_L2_NPU0_PE1_DMA_W_CFG2_t PE1_DMA_W_CFG2;  /* NPU0 0x304 */
    DX_L2_NPU0_PE1_DMA_W_CFG3_t PE1_DMA_W_CFG3;  /* NPU0 0x308 */
    DX_L2_NPU0_PE1_DMA_INF_CFG0_t PE1_DMA_INF_CFG0;  /* NPU0 0x30c */
    DX_L2_NPU0_PE1_DMA_INF_CFG1_t PE1_DMA_INF_CFG1;  /* NPU0 0x310 */
    DX_L2_NPU0_PE1_DMA_INF_CFG2_t PE1_DMA_INF_CFG2;  /* NPU0 0x314 */
    DX_L2_NPU0_PE1_DMA_INF_CFG3_t PE1_DMA_INF_CFG3;  /* NPU0 0x318 */
    DX_L2_NPU0_PE1_DMA_INF_CFG4_t PE1_DMA_INF_CFG4;  /* NPU0 0x31c */
    DX_L2_NPU0_PE1_DMA_OUTF_CFG0_t PE1_DMA_OUTF_CFG0;  /* NPU0 0x320 */
    DX_L2_NPU0_PE1_DMA_OUTF_CFG1_t PE1_DMA_OUTF_CFG1;  /* NPU0 0x324 */
    DX_L2_NPU0_PE1_DMA_OUTF_CFG2_t PE1_DMA_OUTF_CFG2;  /* NPU0 0x328 */
    DX_L2_NPU0_PE1_DMA_OUTF_CFG3_t PE1_DMA_OUTF_CFG3;  /* NPU0 0x32c */
    DX_L2_NPU0_PE1_DMA_OUTF_CFG4_t PE1_DMA_OUTF_CFG4;  /* NPU0 0x330 */
    DX_L2_NPU0_RSVD7_t RSVD7;  /* NPU0 0x334 */
    DX_L2_NPU0_RSVD8_t RSVD8;  /* NPU0 0x338 */
} dxDX_L2_NPU0_t;

/* Register access API */
/* DX_L2_NPU0.DATA_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_DATA_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_DATA_CFG0)
#define WRITE_DX_L2_NPU0_DATA_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_DATA_CFG0, val)
/* DX_L2_NPU0.DATA_CFG0.FEATURE_BIT_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_LEN        8
#define DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_OFFSET     0
#define DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_DATA_CFG0, DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_MASK)
#define READ_DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_DATA_CFG0, DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_MASK, DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_DATA_CFG0, bits, DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_MASK, DX_L2_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_OFFSET)

/* DX_L2_NPU0.DATA_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_DATA_CFG0_RESERVED0_LEN        23
#define DX_L2_NPU0_DATA_CFG0_RESERVED0_OFFSET     8
#define DX_L2_NPU0_DATA_CFG0_RESERVED0_MASK_BITS  0x007fffff /* [22:0] 0b00000000011111111111111111111111 */
#define DX_L2_NPU0_DATA_CFG0_RESERVED0_MASK       0x7fffff00 /* [30:8] 0b01111111111111111111111100000000 */
#define READ_MASK_DX_L2_NPU0_DATA_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_DATA_CFG0, DX_L2_NPU0_DATA_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_DATA_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_DATA_CFG0, DX_L2_NPU0_DATA_CFG0_RESERVED0_MASK, DX_L2_NPU0_DATA_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_DATA_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_DATA_CFG0, bits, DX_L2_NPU0_DATA_CFG0_RESERVED0_MASK, DX_L2_NPU0_DATA_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.DATA_CFG0.FEATURE_DATA_TYPE (Category: NPU0) */
#define DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE_LEN        1
#define DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE_OFFSET     31
#define DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_DATA_CFG0, DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE_MASK)
#define READ_DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_DATA_CFG0, DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE_MASK, DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE_OFFSET)
#define WRITE_DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_DATA_CFG0, bits, DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE_MASK, DX_L2_NPU0_DATA_CFG0_FEATURE_DATA_TYPE_OFFSET)

/* DX_L2_NPU0.DATA_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_DATA_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_DATA_CFG1)
#define WRITE_DX_L2_NPU0_DATA_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_DATA_CFG1, val)
/* DX_L2_NPU0.DATA_CFG1.WEIGHT_BIT_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH_LEN        8
#define DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH_OFFSET     0
#define DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_DATA_CFG1, DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH_MASK)
#define READ_DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_DATA_CFG1, DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH_MASK, DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_DATA_CFG1, bits, DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH_MASK, DX_L2_NPU0_DATA_CFG1_WEIGHT_BIT_WIDTH_OFFSET)

/* DX_L2_NPU0.DATA_CFG1.BIAS_BIT_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH_LEN        8
#define DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH_OFFSET     8
#define DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_DATA_CFG1, DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH_MASK)
#define READ_DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_DATA_CFG1, DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH_MASK, DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_DATA_CFG1, bits, DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH_MASK, DX_L2_NPU0_DATA_CFG1_BIAS_BIT_WIDTH_OFFSET)

/* DX_L2_NPU0.DATA_CFG1.BN_COEFF_BIT_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH_LEN        8
#define DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH_OFFSET     16
#define DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_DATA_CFG1, DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH_MASK)
#define READ_DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_DATA_CFG1, DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH_MASK, DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_DATA_CFG1, bits, DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH_MASK, DX_L2_NPU0_DATA_CFG1_BN_COEFF_BIT_WIDTH_OFFSET)

/* DX_L2_NPU0.DATA_CFG1.W_FEATURE_BIT_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH_LEN        8
#define DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH_OFFSET     24
#define DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_DATA_CFG1, DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH_MASK)
#define READ_DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_DATA_CFG1, DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH_MASK, DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_DATA_CFG1, bits, DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH_MASK, DX_L2_NPU0_DATA_CFG1_W_FEATURE_BIT_WIDTH_OFFSET)

/* DX_L2_NPU0.IM2COL (Category: NPU0) */
#define READ_DX_L2_NPU0_IM2COL(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_IM2COL)
#define WRITE_DX_L2_NPU0_IM2COL(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_IM2COL, val)
/* DX_L2_NPU0.IM2COL.GLB2RF_RF_WSIZE (Category: NPU0) */
#define DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE_LEN        16
#define DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE_OFFSET     0
#define DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_IM2COL, DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE_MASK)
#define READ_DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_IM2COL, DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE_MASK, DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE_OFFSET)
#define WRITE_DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_IM2COL, bits, DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE_MASK, DX_L2_NPU0_IM2COL_GLB2RF_RF_WSIZE_OFFSET)

/* DX_L2_NPU0.IM2COL.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_IM2COL_RESERVED0_LEN        13
#define DX_L2_NPU0_IM2COL_RESERVED0_OFFSET     16
#define DX_L2_NPU0_IM2COL_RESERVED0_MASK_BITS  0x00001fff /* [12:0] 0b00000000000000000001111111111111 */
#define DX_L2_NPU0_IM2COL_RESERVED0_MASK       0x1fff0000 /* [28:16] 0b00011111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_IM2COL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_IM2COL, DX_L2_NPU0_IM2COL_RESERVED0_MASK)
#define READ_DX_L2_NPU0_IM2COL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_IM2COL, DX_L2_NPU0_IM2COL_RESERVED0_MASK, DX_L2_NPU0_IM2COL_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_IM2COL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_IM2COL, bits, DX_L2_NPU0_IM2COL_RESERVED0_MASK, DX_L2_NPU0_IM2COL_RESERVED0_OFFSET)

/* DX_L2_NPU0.IM2COL.GLB2RF_WRITE (Category: NPU0) */
#define DX_L2_NPU0_IM2COL_GLB2RF_WRITE_LEN        1
#define DX_L2_NPU0_IM2COL_GLB2RF_WRITE_OFFSET     29
#define DX_L2_NPU0_IM2COL_GLB2RF_WRITE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_IM2COL_GLB2RF_WRITE_MASK       0x20000000 /* [29:29] 0b00100000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_IM2COL_GLB2RF_WRITE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_IM2COL, DX_L2_NPU0_IM2COL_GLB2RF_WRITE_MASK)
#define READ_DX_L2_NPU0_IM2COL_GLB2RF_WRITE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_IM2COL, DX_L2_NPU0_IM2COL_GLB2RF_WRITE_MASK, DX_L2_NPU0_IM2COL_GLB2RF_WRITE_OFFSET)
#define WRITE_DX_L2_NPU0_IM2COL_GLB2RF_WRITE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_IM2COL, bits, DX_L2_NPU0_IM2COL_GLB2RF_WRITE_MASK, DX_L2_NPU0_IM2COL_GLB2RF_WRITE_OFFSET)

/* DX_L2_NPU0.IM2COL.GLB2RF_START (Category: NPU0) */
#define DX_L2_NPU0_IM2COL_GLB2RF_START_LEN        1
#define DX_L2_NPU0_IM2COL_GLB2RF_START_OFFSET     30
#define DX_L2_NPU0_IM2COL_GLB2RF_START_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_IM2COL_GLB2RF_START_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_IM2COL_GLB2RF_START(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_IM2COL, DX_L2_NPU0_IM2COL_GLB2RF_START_MASK)
#define READ_DX_L2_NPU0_IM2COL_GLB2RF_START(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_IM2COL, DX_L2_NPU0_IM2COL_GLB2RF_START_MASK, DX_L2_NPU0_IM2COL_GLB2RF_START_OFFSET)
#define WRITE_DX_L2_NPU0_IM2COL_GLB2RF_START(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_IM2COL, bits, DX_L2_NPU0_IM2COL_GLB2RF_START_MASK, DX_L2_NPU0_IM2COL_GLB2RF_START_OFFSET)

/* DX_L2_NPU0.IM2COL.IMG2COL_EN (Category: NPU0) */
#define DX_L2_NPU0_IM2COL_IMG2COL_EN_LEN        1
#define DX_L2_NPU0_IM2COL_IMG2COL_EN_OFFSET     31
#define DX_L2_NPU0_IM2COL_IMG2COL_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_IM2COL_IMG2COL_EN_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_IM2COL_IMG2COL_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_IM2COL, DX_L2_NPU0_IM2COL_IMG2COL_EN_MASK)
#define READ_DX_L2_NPU0_IM2COL_IMG2COL_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_IM2COL, DX_L2_NPU0_IM2COL_IMG2COL_EN_MASK, DX_L2_NPU0_IM2COL_IMG2COL_EN_OFFSET)
#define WRITE_DX_L2_NPU0_IM2COL_IMG2COL_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_IM2COL, bits, DX_L2_NPU0_IM2COL_IMG2COL_EN_MASK, DX_L2_NPU0_IM2COL_IMG2COL_EN_OFFSET)

/* DX_L2_NPU0.CTRL (Category: NPU0) */
#define READ_DX_L2_NPU0_CTRL(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_CTRL)
#define WRITE_DX_L2_NPU0_CTRL(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_CTRL, val)
/* DX_L2_NPU0.CTRL.WINOGRAD_EN (Category: NPU0) */
#define DX_L2_NPU0_CTRL_WINOGRAD_EN_LEN        1
#define DX_L2_NPU0_CTRL_WINOGRAD_EN_OFFSET     0
#define DX_L2_NPU0_CTRL_WINOGRAD_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_CTRL_WINOGRAD_EN_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_L2_NPU0_CTRL_WINOGRAD_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_WINOGRAD_EN_MASK)
#define READ_DX_L2_NPU0_CTRL_WINOGRAD_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_WINOGRAD_EN_MASK, DX_L2_NPU0_CTRL_WINOGRAD_EN_OFFSET)
#define WRITE_DX_L2_NPU0_CTRL_WINOGRAD_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_CTRL, bits, DX_L2_NPU0_CTRL_WINOGRAD_EN_MASK, DX_L2_NPU0_CTRL_WINOGRAD_EN_OFFSET)

/* DX_L2_NPU0.CTRL.FEATURE_COMPRESS_EN (Category: NPU0) */
#define DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN_LEN        1
#define DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN_OFFSET     1
#define DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN_MASK       0x00000002 /* [1:1] 0b00000000000000000000000000000010 */
#define READ_MASK_DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN_MASK)
#define READ_DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN_MASK, DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN_OFFSET)
#define WRITE_DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_CTRL, bits, DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN_MASK, DX_L2_NPU0_CTRL_FEATURE_COMPRESS_EN_OFFSET)

/* DX_L2_NPU0.CTRL.WEIGHT_COMPRESS_EN (Category: NPU0) */
#define DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN_LEN        1
#define DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN_OFFSET     2
#define DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN_MASK       0x00000004 /* [2:2] 0b00000000000000000000000000000100 */
#define READ_MASK_DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN_MASK)
#define READ_DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN_MASK, DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN_OFFSET)
#define WRITE_DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_CTRL, bits, DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN_MASK, DX_L2_NPU0_CTRL_WEIGHT_COMPRESS_EN_OFFSET)

/* DX_L2_NPU0.CTRL.SKIP_CONNECTION_EN (Category: NPU0) */
#define DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN_LEN        1
#define DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN_OFFSET     3
#define DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN_MASK       0x00000008 /* [3:3] 0b00000000000000000000000000001000 */
#define READ_MASK_DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN_MASK)
#define READ_DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN_MASK, DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN_OFFSET)
#define WRITE_DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_CTRL, bits, DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN_MASK, DX_L2_NPU0_CTRL_SKIP_CONNECTION_EN_OFFSET)

/* DX_L2_NPU0.CTRL.SKIP_CONNECTION_TYPE (Category: NPU0) */
#define DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE_LEN        2
#define DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE_OFFSET     4
#define DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE_MASK       0x00000030 /* [5:4] 0b00000000000000000000000000110000 */
#define READ_MASK_DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE_MASK)
#define READ_DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE_MASK, DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE_OFFSET)
#define WRITE_DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_CTRL, bits, DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE_MASK, DX_L2_NPU0_CTRL_SKIP_CONNECTION_TYPE_OFFSET)

/* DX_L2_NPU0.CTRL.RESIZE (Category: NPU0) */
#define DX_L2_NPU0_CTRL_RESIZE_LEN        2
#define DX_L2_NPU0_CTRL_RESIZE_OFFSET     6
#define DX_L2_NPU0_CTRL_RESIZE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_CTRL_RESIZE_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_L2_NPU0_CTRL_RESIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_RESIZE_MASK)
#define READ_DX_L2_NPU0_CTRL_RESIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_RESIZE_MASK, DX_L2_NPU0_CTRL_RESIZE_OFFSET)
#define WRITE_DX_L2_NPU0_CTRL_RESIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_CTRL, bits, DX_L2_NPU0_CTRL_RESIZE_MASK, DX_L2_NPU0_CTRL_RESIZE_OFFSET)

/* DX_L2_NPU0.CTRL.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_CTRL_RESERVED0_LEN        24
#define DX_L2_NPU0_CTRL_RESERVED0_OFFSET     8
#define DX_L2_NPU0_CTRL_RESERVED0_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_NPU0_CTRL_RESERVED0_MASK       0xffffff00 /* [31:8] 0b11111111111111111111111100000000 */
#define READ_MASK_DX_L2_NPU0_CTRL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_RESERVED0_MASK)
#define READ_DX_L2_NPU0_CTRL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_CTRL, DX_L2_NPU0_CTRL_RESERVED0_MASK, DX_L2_NPU0_CTRL_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_CTRL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_CTRL, bits, DX_L2_NPU0_CTRL_RESERVED0_MASK, DX_L2_NPU0_CTRL_RESERVED0_OFFSET)

/* DX_L2_NPU0.MISC (Category: NPU0) */
#define READ_DX_L2_NPU0_MISC(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_MISC)
#define WRITE_DX_L2_NPU0_MISC(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_MISC, val)
/* DX_L2_NPU0.MISC.TILE_NUM (Category: NPU0) */
#define DX_L2_NPU0_MISC_TILE_NUM_LEN        16
#define DX_L2_NPU0_MISC_TILE_NUM_OFFSET     0
#define DX_L2_NPU0_MISC_TILE_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_MISC_TILE_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_MISC_TILE_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_MISC, DX_L2_NPU0_MISC_TILE_NUM_MASK)
#define READ_DX_L2_NPU0_MISC_TILE_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_MISC, DX_L2_NPU0_MISC_TILE_NUM_MASK, DX_L2_NPU0_MISC_TILE_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_MISC_TILE_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_MISC, bits, DX_L2_NPU0_MISC_TILE_NUM_MASK, DX_L2_NPU0_MISC_TILE_NUM_OFFSET)

/* DX_L2_NPU0.MISC.LAYER_NUM (Category: NPU0) */
#define DX_L2_NPU0_MISC_LAYER_NUM_LEN        16
#define DX_L2_NPU0_MISC_LAYER_NUM_OFFSET     16
#define DX_L2_NPU0_MISC_LAYER_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_MISC_LAYER_NUM_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_MISC_LAYER_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_MISC, DX_L2_NPU0_MISC_LAYER_NUM_MASK)
#define READ_DX_L2_NPU0_MISC_LAYER_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_MISC, DX_L2_NPU0_MISC_LAYER_NUM_MASK, DX_L2_NPU0_MISC_LAYER_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_MISC_LAYER_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_MISC, bits, DX_L2_NPU0_MISC_LAYER_NUM_MASK, DX_L2_NPU0_MISC_LAYER_NUM_OFFSET)

/* DX_L2_NPU0.ACTIVE (Category: NPU0) */
#define READ_DX_L2_NPU0_ACTIVE(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_ACTIVE)
#define WRITE_DX_L2_NPU0_ACTIVE(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_ACTIVE, val)
/* DX_L2_NPU0.ACTIVE.ACTIVE_CNT (Category: NPU0) */
#define DX_L2_NPU0_ACTIVE_ACTIVE_CNT_LEN        8
#define DX_L2_NPU0_ACTIVE_ACTIVE_CNT_OFFSET     0
#define DX_L2_NPU0_ACTIVE_ACTIVE_CNT_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_ACTIVE_ACTIVE_CNT_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_NPU0_ACTIVE_ACTIVE_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_ACTIVE, DX_L2_NPU0_ACTIVE_ACTIVE_CNT_MASK)
#define READ_DX_L2_NPU0_ACTIVE_ACTIVE_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_ACTIVE, DX_L2_NPU0_ACTIVE_ACTIVE_CNT_MASK, DX_L2_NPU0_ACTIVE_ACTIVE_CNT_OFFSET)
#define WRITE_DX_L2_NPU0_ACTIVE_ACTIVE_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_ACTIVE, bits, DX_L2_NPU0_ACTIVE_ACTIVE_CNT_MASK, DX_L2_NPU0_ACTIVE_ACTIVE_CNT_OFFSET)

/* DX_L2_NPU0.ACTIVE.ACTIVE_PERIOD (Category: NPU0) */
#define DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD_LEN        8
#define DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD_OFFSET     8
#define DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_ACTIVE, DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD_MASK)
#define READ_DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_ACTIVE, DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD_MASK, DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD_OFFSET)
#define WRITE_DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_ACTIVE, bits, DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD_MASK, DX_L2_NPU0_ACTIVE_ACTIVE_PERIOD_OFFSET)

/* DX_L2_NPU0.ACTIVE.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_ACTIVE_RESERVED0_LEN        16
#define DX_L2_NPU0_ACTIVE_RESERVED0_OFFSET     16
#define DX_L2_NPU0_ACTIVE_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_ACTIVE_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_ACTIVE_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_ACTIVE, DX_L2_NPU0_ACTIVE_RESERVED0_MASK)
#define READ_DX_L2_NPU0_ACTIVE_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_ACTIVE, DX_L2_NPU0_ACTIVE_RESERVED0_MASK, DX_L2_NPU0_ACTIVE_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_ACTIVE_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_ACTIVE, bits, DX_L2_NPU0_ACTIVE_RESERVED0_MASK, DX_L2_NPU0_ACTIVE_RESERVED0_OFFSET)

/* DX_L2_NPU0.RSVD0 (Category: NPU0) */
#define READ_DX_L2_NPU0_RSVD0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_RSVD0)
#define WRITE_DX_L2_NPU0_RSVD0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_RSVD0, val)
/* DX_L2_NPU0.RSVD0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_RSVD0_RESERVED0_LEN        32
#define DX_L2_NPU0_RSVD0_RESERVED0_OFFSET     0
#define DX_L2_NPU0_RSVD0_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_RSVD0_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_RSVD0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD0, DX_L2_NPU0_RSVD0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_RSVD0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD0, DX_L2_NPU0_RSVD0_RESERVED0_MASK, DX_L2_NPU0_RSVD0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD0, bits, DX_L2_NPU0_RSVD0_RESERVED0_MASK, DX_L2_NPU0_RSVD0_RESERVED0_OFFSET)

/* DX_L2_NPU0.SFU_CTRL (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_CTRL(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_CTRL)
#define WRITE_DX_L2_NPU0_SFU_CTRL(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_CTRL, val)
/* DX_L2_NPU0.SFU_CTRL.PRE_ACT_MODE (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE_LEN        3
#define DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE_OFFSET     0
#define DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE_MASK       0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE_MASK, DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE_MASK, DX_L2_NPU0_SFU_CTRL_PRE_ACT_MODE_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.PRE_MULT_COEFF_SEL (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL_LEN        1
#define DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL_OFFSET     3
#define DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL_MASK       0x00000008 /* [3:3] 0b00000000000000000000000000001000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL_MASK, DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL_MASK, DX_L2_NPU0_SFU_CTRL_PRE_MULT_COEFF_SEL_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.ACT_MODE (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_ACT_MODE_LEN        3
#define DX_L2_NPU0_SFU_CTRL_ACT_MODE_OFFSET     4
#define DX_L2_NPU0_SFU_CTRL_ACT_MODE_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_L2_NPU0_SFU_CTRL_ACT_MODE_MASK       0x00000070 /* [6:4] 0b00000000000000000000000001110000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_ACT_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_ACT_MODE_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_ACT_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_ACT_MODE_MASK, DX_L2_NPU0_SFU_CTRL_ACT_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_ACT_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_ACT_MODE_MASK, DX_L2_NPU0_SFU_CTRL_ACT_MODE_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.ACT_INOUT_CTRL (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL_LEN        1
#define DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL_OFFSET     7
#define DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL_MASK       0x00000080 /* [7:7] 0b00000000000000000000000010000000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL_MASK, DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL_MASK, DX_L2_NPU0_SFU_CTRL_ACT_INOUT_CTRL_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.POST_ACT_MODE (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE_LEN        1
#define DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE_OFFSET     8
#define DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE_MASK, DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE_MASK, DX_L2_NPU0_SFU_CTRL_POST_ACT_MODE_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.SKIP_IN_TYPE (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE_LEN        1
#define DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE_OFFSET     9
#define DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE_MASK       0x00000200 /* [9:9] 0b00000000000000000000001000000000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE_MASK, DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE_MASK, DX_L2_NPU0_SFU_CTRL_SKIP_IN_TYPE_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.FLOAT2INT_MODE (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE_LEN        1
#define DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE_OFFSET     10
#define DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE_MASK       0x00000400 /* [10:10] 0b00000000000000000000010000000000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE_MASK, DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE_MASK, DX_L2_NPU0_SFU_CTRL_FLOAT2INT_MODE_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.POOL_MODE (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_POOL_MODE_LEN        2
#define DX_L2_NPU0_SFU_CTRL_POOL_MODE_OFFSET     11
#define DX_L2_NPU0_SFU_CTRL_POOL_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_SFU_CTRL_POOL_MODE_MASK       0x00001800 /* [12:11] 0b00000000000000000001100000000000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_POOL_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_POOL_MODE_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_POOL_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_POOL_MODE_MASK, DX_L2_NPU0_SFU_CTRL_POOL_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_POOL_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_POOL_MODE_MASK, DX_L2_NPU0_SFU_CTRL_POOL_MODE_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.POOL_WINSIZE (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE_LEN        8
#define DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE_OFFSET     13
#define DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE_MASK       0x001fe000 /* [20:13] 0b00000000000111111110000000000000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE_MASK, DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE_MASK, DX_L2_NPU0_SFU_CTRL_POOL_WINSIZE_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.PRE_ADD_COEFF_SEL (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL_LEN        1
#define DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL_OFFSET     21
#define DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL_MASK       0x00200000 /* [21:21] 0b00000000001000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL_MASK, DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL_MASK, DX_L2_NPU0_SFU_CTRL_PRE_ADD_COEFF_SEL_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.SE_EN (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_SE_EN_LEN        1
#define DX_L2_NPU0_SFU_CTRL_SE_EN_OFFSET     22
#define DX_L2_NPU0_SFU_CTRL_SE_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_SFU_CTRL_SE_EN_MASK       0x00400000 /* [22:22] 0b00000000010000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_SE_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_SE_EN_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_SE_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_SE_EN_MASK, DX_L2_NPU0_SFU_CTRL_SE_EN_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_SE_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_SE_EN_MASK, DX_L2_NPU0_SFU_CTRL_SE_EN_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.SE_WRITE (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_SE_WRITE_LEN        1
#define DX_L2_NPU0_SFU_CTRL_SE_WRITE_OFFSET     23
#define DX_L2_NPU0_SFU_CTRL_SE_WRITE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_SFU_CTRL_SE_WRITE_MASK       0x00800000 /* [23:23] 0b00000000100000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_SE_WRITE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_SE_WRITE_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_SE_WRITE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_SE_WRITE_MASK, DX_L2_NPU0_SFU_CTRL_SE_WRITE_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_SE_WRITE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_SE_WRITE_MASK, DX_L2_NPU0_SFU_CTRL_SE_WRITE_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.ARG_MAX_EN (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN_LEN        1
#define DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN_OFFSET     24
#define DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN_MASK, DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN_MASK, DX_L2_NPU0_SFU_CTRL_ARG_MAX_EN_OFFSET)

/* DX_L2_NPU0.SFU_CTRL.ARG_CLASS_NUM (Category: NPU0) */
#define DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM_LEN        7
#define DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM_OFFSET     25
#define DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM_MASK       0xfe000000 /* [31:25] 0b11111110000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM_MASK)
#define READ_DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM_MASK, DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CTRL, bits, DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM_MASK, DX_L2_NPU0_SFU_CTRL_ARG_CLASS_NUM_OFFSET)

/* DX_L2_NPU0.SFU_CLIP_MAX (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_CLIP_MAX(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_CLIP_MAX)
#define WRITE_DX_L2_NPU0_SFU_CLIP_MAX(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_CLIP_MAX, val)
/* DX_L2_NPU0.SFU_CLIP_MAX.CLIP_MAX (Category: NPU0) */
#define DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX_LEN        32
#define DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX_OFFSET     0
#define DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CLIP_MAX, DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX_MASK)
#define READ_DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CLIP_MAX, DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX_MASK, DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CLIP_MAX, bits, DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX_MASK, DX_L2_NPU0_SFU_CLIP_MAX_CLIP_MAX_OFFSET)

/* DX_L2_NPU0.SFU_CLIP_MIN (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_CLIP_MIN(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_CLIP_MIN)
#define WRITE_DX_L2_NPU0_SFU_CLIP_MIN(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_CLIP_MIN, val)
/* DX_L2_NPU0.SFU_CLIP_MIN.CLIP_MIN (Category: NPU0) */
#define DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN_LEN        32
#define DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN_OFFSET     0
#define DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_CLIP_MIN, DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN_MASK)
#define READ_DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_CLIP_MIN, DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN_MASK, DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_CLIP_MIN, bits, DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN_MASK, DX_L2_NPU0_SFU_CLIP_MIN_CLIP_MIN_OFFSET)

/* DX_L2_NPU0.SFU_COEFF0 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF0)
#define WRITE_DX_L2_NPU0_SFU_COEFF0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF0, val)
/* DX_L2_NPU0.SFU_COEFF0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF0_RESERVED0_LEN        32
#define DX_L2_NPU0_SFU_COEFF0_RESERVED0_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF0_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF0_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF0, DX_L2_NPU0_SFU_COEFF0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF0, DX_L2_NPU0_SFU_COEFF0_RESERVED0_MASK, DX_L2_NPU0_SFU_COEFF0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF0, bits, DX_L2_NPU0_SFU_COEFF0_RESERVED0_MASK, DX_L2_NPU0_SFU_COEFF0_RESERVED0_OFFSET)

/* DX_L2_NPU0.SFU_COEFF1 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF1)
#define WRITE_DX_L2_NPU0_SFU_COEFF1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF1, val)
/* DX_L2_NPU0.SFU_COEFF1.PRE_ACT_MULT_COEFF (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF_LEN        32
#define DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF1, DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF1, DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF_MASK, DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF1, bits, DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF_MASK, DX_L2_NPU0_SFU_COEFF1_PRE_ACT_MULT_COEFF_OFFSET)

/* DX_L2_NPU0.SFU_COEFF2 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF2)
#define WRITE_DX_L2_NPU0_SFU_COEFF2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF2, val)
/* DX_L2_NPU0.SFU_COEFF2.PRE_ACT_ADD_COEFF (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF_LEN        32
#define DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF2, DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF2, DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF_MASK, DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF2, bits, DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF_MASK, DX_L2_NPU0_SFU_COEFF2_PRE_ACT_ADD_COEFF_OFFSET)

/* DX_L2_NPU0.SFU_COEFF3 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF3)
#define WRITE_DX_L2_NPU0_SFU_COEFF3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF3, val)
/* DX_L2_NPU0.SFU_COEFF3.AVG_POOL_MULT_COEFF (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF_LEN        32
#define DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF3, DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF3, DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF_MASK, DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF3, bits, DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF_MASK, DX_L2_NPU0_SFU_COEFF3_AVG_POOL_MULT_COEFF_OFFSET)

/* DX_L2_NPU0.SFU_COEFF4 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF4(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF4)
#define WRITE_DX_L2_NPU0_SFU_COEFF4(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF4, val)
/* DX_L2_NPU0.SFU_COEFF4.ACT_OFFSET_VAL (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL_LEN        32
#define DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF4, DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF4, DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL_MASK, DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF4, bits, DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL_MASK, DX_L2_NPU0_SFU_COEFF4_ACT_OFFSET_VAL_OFFSET)

/* DX_L2_NPU0.SFU_COEFF5 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF5(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF5)
#define WRITE_DX_L2_NPU0_SFU_COEFF5(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF5, val)
/* DX_L2_NPU0.SFU_COEFF5.SKIP_SCALE (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE_LEN        32
#define DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF5, DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF5, DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE_MASK, DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF5, bits, DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE_MASK, DX_L2_NPU0_SFU_COEFF5_SKIP_SCALE_OFFSET)

/* DX_L2_NPU0.SFU_COEFF6 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF6(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF6)
#define WRITE_DX_L2_NPU0_SFU_COEFF6(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF6, val)
/* DX_L2_NPU0.SFU_COEFF6.LEAKY_RELU_SLOPE (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE_LEN        32
#define DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF6, DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF6, DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE_MASK, DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF6, bits, DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE_MASK, DX_L2_NPU0_SFU_COEFF6_LEAKY_RELU_SLOPE_OFFSET)

/* DX_L2_NPU0.SFU_COEFF7 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF7(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF7)
#define WRITE_DX_L2_NPU0_SFU_COEFF7(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF7, val)
/* DX_L2_NPU0.SFU_COEFF7.RELU6_CLIP_MAX (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX_LEN        32
#define DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF7, DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF7, DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX_MASK, DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF7, bits, DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX_MASK, DX_L2_NPU0_SFU_COEFF7_RELU6_CLIP_MAX_OFFSET)

/* DX_L2_NPU0.SFU_COEFF8 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF8(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF8)
#define WRITE_DX_L2_NPU0_SFU_COEFF8(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF8, val)
/* DX_L2_NPU0.SFU_COEFF8.PAF_BOUNDARY0 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0_LEN        32
#define DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF8, DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF8, DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0_MASK, DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF8, bits, DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0_MASK, DX_L2_NPU0_SFU_COEFF8_PAF_BOUNDARY0_OFFSET)

/* DX_L2_NPU0.SFU_COEFF9 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF9(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF9)
#define WRITE_DX_L2_NPU0_SFU_COEFF9(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF9, val)
/* DX_L2_NPU0.SFU_COEFF9.PAF_BOUNDARY1 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1_LEN        32
#define DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF9, DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF9, DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1_MASK, DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF9, bits, DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1_MASK, DX_L2_NPU0_SFU_COEFF9_PAF_BOUNDARY1_OFFSET)

/* DX_L2_NPU0.SFU_COEFF10 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF10(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF10)
#define WRITE_DX_L2_NPU0_SFU_COEFF10(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF10, val)
/* DX_L2_NPU0.SFU_COEFF10.PAF_BOUNDARY2 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2_LEN        32
#define DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF10, DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF10, DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2_MASK, DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF10, bits, DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2_MASK, DX_L2_NPU0_SFU_COEFF10_PAF_BOUNDARY2_OFFSET)

/* DX_L2_NPU0.SFU_COEFF11 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF11(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF11)
#define WRITE_DX_L2_NPU0_SFU_COEFF11(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF11, val)
/* DX_L2_NPU0.SFU_COEFF11.PAF_BOUNDARY3 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3_LEN        32
#define DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF11, DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF11, DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3_MASK, DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF11, bits, DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3_MASK, DX_L2_NPU0_SFU_COEFF11_PAF_BOUNDARY3_OFFSET)

/* DX_L2_NPU0.SFU_COEFF12 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF12(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF12)
#define WRITE_DX_L2_NPU0_SFU_COEFF12(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF12, val)
/* DX_L2_NPU0.SFU_COEFF12.PAF_BOUNDARY4 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4_LEN        32
#define DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF12, DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF12, DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4_MASK, DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF12, bits, DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4_MASK, DX_L2_NPU0_SFU_COEFF12_PAF_BOUNDARY4_OFFSET)

/* DX_L2_NPU0.SFU_COEFF13 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF13(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF13)
#define WRITE_DX_L2_NPU0_SFU_COEFF13(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF13, val)
/* DX_L2_NPU0.SFU_COEFF13.PAF_BOUNDARY5 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5_LEN        32
#define DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF13, DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF13, DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5_MASK, DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF13, bits, DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5_MASK, DX_L2_NPU0_SFU_COEFF13_PAF_BOUNDARY5_OFFSET)

/* DX_L2_NPU0.SFU_COEFF14 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF14(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF14)
#define WRITE_DX_L2_NPU0_SFU_COEFF14(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF14, val)
/* DX_L2_NPU0.SFU_COEFF14.PAF_BOUNDARY6 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6_LEN        32
#define DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF14, DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF14, DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6_MASK, DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF14, bits, DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6_MASK, DX_L2_NPU0_SFU_COEFF14_PAF_BOUNDARY6_OFFSET)

/* DX_L2_NPU0.SFU_COEFF15 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF15(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF15)
#define WRITE_DX_L2_NPU0_SFU_COEFF15(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF15, val)
/* DX_L2_NPU0.SFU_COEFF15.PAF_BOUNDARY7 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7_LEN        32
#define DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF15, DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF15, DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7_MASK, DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF15, bits, DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7_MASK, DX_L2_NPU0_SFU_COEFF15_PAF_BOUNDARY7_OFFSET)

/* DX_L2_NPU0.SFU_COEFF16 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF16(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF16)
#define WRITE_DX_L2_NPU0_SFU_COEFF16(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF16, val)
/* DX_L2_NPU0.SFU_COEFF16.PAF_BOUNDARY8 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8_LEN        32
#define DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF16, DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF16, DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8_MASK, DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF16, bits, DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8_MASK, DX_L2_NPU0_SFU_COEFF16_PAF_BOUNDARY8_OFFSET)

/* DX_L2_NPU0.SFU_COEFF17 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF17(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF17)
#define WRITE_DX_L2_NPU0_SFU_COEFF17(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF17, val)
/* DX_L2_NPU0.SFU_COEFF17.PAF_BOUNDARY9 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9_LEN        32
#define DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF17, DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF17, DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9_MASK, DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF17, bits, DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9_MASK, DX_L2_NPU0_SFU_COEFF17_PAF_BOUNDARY9_OFFSET)

/* DX_L2_NPU0.SFU_COEFF18 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF18(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF18)
#define WRITE_DX_L2_NPU0_SFU_COEFF18(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF18, val)
/* DX_L2_NPU0.SFU_COEFF18.PAF_BOUNDARY10 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10_LEN        32
#define DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF18, DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF18, DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10_MASK, DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF18, bits, DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10_MASK, DX_L2_NPU0_SFU_COEFF18_PAF_BOUNDARY10_OFFSET)

/* DX_L2_NPU0.SFU_COEFF19 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF19(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF19)
#define WRITE_DX_L2_NPU0_SFU_COEFF19(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF19, val)
/* DX_L2_NPU0.SFU_COEFF19.PAF_BOUNDARY11 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11_LEN        32
#define DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF19, DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF19, DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11_MASK, DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF19, bits, DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11_MASK, DX_L2_NPU0_SFU_COEFF19_PAF_BOUNDARY11_OFFSET)

/* DX_L2_NPU0.SFU_COEFF20 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF20(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF20)
#define WRITE_DX_L2_NPU0_SFU_COEFF20(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF20, val)
/* DX_L2_NPU0.SFU_COEFF20.PAF_BOUNDARY12 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12_LEN        32
#define DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF20, DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF20, DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12_MASK, DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF20, bits, DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12_MASK, DX_L2_NPU0_SFU_COEFF20_PAF_BOUNDARY12_OFFSET)

/* DX_L2_NPU0.SFU_COEFF21 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF21(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF21)
#define WRITE_DX_L2_NPU0_SFU_COEFF21(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF21, val)
/* DX_L2_NPU0.SFU_COEFF21.PAF_BOUNDARY13 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13_LEN        32
#define DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF21, DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF21, DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13_MASK, DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF21, bits, DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13_MASK, DX_L2_NPU0_SFU_COEFF21_PAF_BOUNDARY13_OFFSET)

/* DX_L2_NPU0.SFU_COEFF22 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF22(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF22)
#define WRITE_DX_L2_NPU0_SFU_COEFF22(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF22, val)
/* DX_L2_NPU0.SFU_COEFF22.PAF_BOUNDARY14 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14_LEN        32
#define DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF22, DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF22, DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14_MASK, DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF22, bits, DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14_MASK, DX_L2_NPU0_SFU_COEFF22_PAF_BOUNDARY14_OFFSET)

/* DX_L2_NPU0.SFU_COEFF23 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF23(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF23)
#define WRITE_DX_L2_NPU0_SFU_COEFF23(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF23, val)
/* DX_L2_NPU0.SFU_COEFF23.PAF_MULT_COEFF0 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0_LEN        32
#define DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF23, DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF23, DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0_MASK, DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF23, bits, DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0_MASK, DX_L2_NPU0_SFU_COEFF23_PAF_MULT_COEFF0_OFFSET)

/* DX_L2_NPU0.SFU_COEFF24 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF24(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF24)
#define WRITE_DX_L2_NPU0_SFU_COEFF24(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF24, val)
/* DX_L2_NPU0.SFU_COEFF24.PAF_MULT_COEFF1 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1_LEN        32
#define DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF24, DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF24, DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1_MASK, DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF24, bits, DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1_MASK, DX_L2_NPU0_SFU_COEFF24_PAF_MULT_COEFF1_OFFSET)

/* DX_L2_NPU0.SFU_COEFF25 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF25(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF25)
#define WRITE_DX_L2_NPU0_SFU_COEFF25(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF25, val)
/* DX_L2_NPU0.SFU_COEFF25.PAF_MULT_COEFF2 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2_LEN        32
#define DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF25, DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF25, DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2_MASK, DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF25, bits, DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2_MASK, DX_L2_NPU0_SFU_COEFF25_PAF_MULT_COEFF2_OFFSET)

/* DX_L2_NPU0.SFU_COEFF26 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF26(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF26)
#define WRITE_DX_L2_NPU0_SFU_COEFF26(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF26, val)
/* DX_L2_NPU0.SFU_COEFF26.PAF_MULT_COEFF3 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3_LEN        32
#define DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF26, DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF26, DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3_MASK, DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF26, bits, DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3_MASK, DX_L2_NPU0_SFU_COEFF26_PAF_MULT_COEFF3_OFFSET)

/* DX_L2_NPU0.SFU_COEFF27 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF27(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF27)
#define WRITE_DX_L2_NPU0_SFU_COEFF27(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF27, val)
/* DX_L2_NPU0.SFU_COEFF27.PAF_MULT_COEFF4 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4_LEN        32
#define DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF27, DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF27, DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4_MASK, DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF27, bits, DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4_MASK, DX_L2_NPU0_SFU_COEFF27_PAF_MULT_COEFF4_OFFSET)

/* DX_L2_NPU0.SFU_COEFF28 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF28(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF28)
#define WRITE_DX_L2_NPU0_SFU_COEFF28(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF28, val)
/* DX_L2_NPU0.SFU_COEFF28.PAF_MULT_COEFF5 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5_LEN        32
#define DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF28, DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF28, DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5_MASK, DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF28, bits, DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5_MASK, DX_L2_NPU0_SFU_COEFF28_PAF_MULT_COEFF5_OFFSET)

/* DX_L2_NPU0.SFU_COEFF29 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF29(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF29)
#define WRITE_DX_L2_NPU0_SFU_COEFF29(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF29, val)
/* DX_L2_NPU0.SFU_COEFF29.PAF_MULT_COEFF6 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6_LEN        32
#define DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF29, DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF29, DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6_MASK, DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF29, bits, DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6_MASK, DX_L2_NPU0_SFU_COEFF29_PAF_MULT_COEFF6_OFFSET)

/* DX_L2_NPU0.SFU_COEFF30 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF30(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF30)
#define WRITE_DX_L2_NPU0_SFU_COEFF30(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF30, val)
/* DX_L2_NPU0.SFU_COEFF30.PAF_MULT_COEFF7 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7_LEN        32
#define DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF30, DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF30, DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7_MASK, DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF30, bits, DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7_MASK, DX_L2_NPU0_SFU_COEFF30_PAF_MULT_COEFF7_OFFSET)

/* DX_L2_NPU0.SFU_COEFF31 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF31(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF31)
#define WRITE_DX_L2_NPU0_SFU_COEFF31(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF31, val)
/* DX_L2_NPU0.SFU_COEFF31.PAF_MULT_COEFF8 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8_LEN        32
#define DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF31, DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF31, DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8_MASK, DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF31, bits, DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8_MASK, DX_L2_NPU0_SFU_COEFF31_PAF_MULT_COEFF8_OFFSET)

/* DX_L2_NPU0.SFU_COEFF32 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF32(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF32)
#define WRITE_DX_L2_NPU0_SFU_COEFF32(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF32, val)
/* DX_L2_NPU0.SFU_COEFF32.PAF_MULT_COEFF9 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9_LEN        32
#define DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF32, DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF32, DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9_MASK, DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF32, bits, DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9_MASK, DX_L2_NPU0_SFU_COEFF32_PAF_MULT_COEFF9_OFFSET)

/* DX_L2_NPU0.SFU_COEFF33 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF33(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF33)
#define WRITE_DX_L2_NPU0_SFU_COEFF33(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF33, val)
/* DX_L2_NPU0.SFU_COEFF33.PAF_MULT_COEFF10 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10_LEN        32
#define DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF33, DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF33, DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10_MASK, DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF33, bits, DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10_MASK, DX_L2_NPU0_SFU_COEFF33_PAF_MULT_COEFF10_OFFSET)

/* DX_L2_NPU0.SFU_COEFF34 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF34(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF34)
#define WRITE_DX_L2_NPU0_SFU_COEFF34(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF34, val)
/* DX_L2_NPU0.SFU_COEFF34.PAF_MULT_COEFF11 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11_LEN        32
#define DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF34, DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF34, DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11_MASK, DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF34, bits, DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11_MASK, DX_L2_NPU0_SFU_COEFF34_PAF_MULT_COEFF11_OFFSET)

/* DX_L2_NPU0.SFU_COEFF35 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF35(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF35)
#define WRITE_DX_L2_NPU0_SFU_COEFF35(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF35, val)
/* DX_L2_NPU0.SFU_COEFF35.PAF_MULT_COEFF12 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12_LEN        32
#define DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF35, DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF35, DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12_MASK, DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF35, bits, DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12_MASK, DX_L2_NPU0_SFU_COEFF35_PAF_MULT_COEFF12_OFFSET)

/* DX_L2_NPU0.SFU_COEFF36 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF36(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF36)
#define WRITE_DX_L2_NPU0_SFU_COEFF36(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF36, val)
/* DX_L2_NPU0.SFU_COEFF36.PAF_MULT_COEFF13 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13_LEN        32
#define DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF36, DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF36, DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13_MASK, DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF36, bits, DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13_MASK, DX_L2_NPU0_SFU_COEFF36_PAF_MULT_COEFF13_OFFSET)

/* DX_L2_NPU0.SFU_COEFF37 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF37(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF37)
#define WRITE_DX_L2_NPU0_SFU_COEFF37(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF37, val)
/* DX_L2_NPU0.SFU_COEFF37.PAF_MULT_COEFF14 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14_LEN        32
#define DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF37, DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF37, DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14_MASK, DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF37, bits, DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14_MASK, DX_L2_NPU0_SFU_COEFF37_PAF_MULT_COEFF14_OFFSET)

/* DX_L2_NPU0.SFU_COEFF38 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF38(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF38)
#define WRITE_DX_L2_NPU0_SFU_COEFF38(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF38, val)
/* DX_L2_NPU0.SFU_COEFF38.PAF_MULT_COEFF15 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15_LEN        32
#define DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF38, DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF38, DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15_MASK, DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF38, bits, DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15_MASK, DX_L2_NPU0_SFU_COEFF38_PAF_MULT_COEFF15_OFFSET)

/* DX_L2_NPU0.SFU_COEFF39 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF39(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF39)
#define WRITE_DX_L2_NPU0_SFU_COEFF39(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF39, val)
/* DX_L2_NPU0.SFU_COEFF39.PAF_ADD_COEFF0 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0_LEN        32
#define DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF39, DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF39, DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0_MASK, DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF39, bits, DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0_MASK, DX_L2_NPU0_SFU_COEFF39_PAF_ADD_COEFF0_OFFSET)

/* DX_L2_NPU0.SFU_COEFF40 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF40(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF40)
#define WRITE_DX_L2_NPU0_SFU_COEFF40(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF40, val)
/* DX_L2_NPU0.SFU_COEFF40.PAF_ADD_COEFF1 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1_LEN        32
#define DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF40, DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF40, DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1_MASK, DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF40, bits, DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1_MASK, DX_L2_NPU0_SFU_COEFF40_PAF_ADD_COEFF1_OFFSET)

/* DX_L2_NPU0.SFU_COEFF41 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF41(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF41)
#define WRITE_DX_L2_NPU0_SFU_COEFF41(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF41, val)
/* DX_L2_NPU0.SFU_COEFF41.PAF_ADD_COEFF2 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2_LEN        32
#define DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF41, DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF41, DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2_MASK, DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF41, bits, DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2_MASK, DX_L2_NPU0_SFU_COEFF41_PAF_ADD_COEFF2_OFFSET)

/* DX_L2_NPU0.SFU_COEFF42 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF42(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF42)
#define WRITE_DX_L2_NPU0_SFU_COEFF42(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF42, val)
/* DX_L2_NPU0.SFU_COEFF42.PAF_ADD_COEFF3 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3_LEN        32
#define DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF42, DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF42, DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3_MASK, DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF42, bits, DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3_MASK, DX_L2_NPU0_SFU_COEFF42_PAF_ADD_COEFF3_OFFSET)

/* DX_L2_NPU0.SFU_COEFF43 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF43(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF43)
#define WRITE_DX_L2_NPU0_SFU_COEFF43(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF43, val)
/* DX_L2_NPU0.SFU_COEFF43.PAF_ADD_COEFF4 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4_LEN        32
#define DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF43, DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF43, DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4_MASK, DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF43, bits, DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4_MASK, DX_L2_NPU0_SFU_COEFF43_PAF_ADD_COEFF4_OFFSET)

/* DX_L2_NPU0.SFU_COEFF44 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF44(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF44)
#define WRITE_DX_L2_NPU0_SFU_COEFF44(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF44, val)
/* DX_L2_NPU0.SFU_COEFF44.PAF_ADD_COEFF5 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5_LEN        32
#define DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF44, DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF44, DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5_MASK, DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF44, bits, DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5_MASK, DX_L2_NPU0_SFU_COEFF44_PAF_ADD_COEFF5_OFFSET)

/* DX_L2_NPU0.SFU_COEFF45 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF45(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF45)
#define WRITE_DX_L2_NPU0_SFU_COEFF45(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF45, val)
/* DX_L2_NPU0.SFU_COEFF45.PAF_ADD_COEFF6 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6_LEN        32
#define DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF45, DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF45, DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6_MASK, DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF45, bits, DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6_MASK, DX_L2_NPU0_SFU_COEFF45_PAF_ADD_COEFF6_OFFSET)

/* DX_L2_NPU0.SFU_COEFF46 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF46(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF46)
#define WRITE_DX_L2_NPU0_SFU_COEFF46(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF46, val)
/* DX_L2_NPU0.SFU_COEFF46.PAF_ADD_COEFF7 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7_LEN        32
#define DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF46, DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF46, DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7_MASK, DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF46, bits, DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7_MASK, DX_L2_NPU0_SFU_COEFF46_PAF_ADD_COEFF7_OFFSET)

/* DX_L2_NPU0.SFU_COEFF47 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF47(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF47)
#define WRITE_DX_L2_NPU0_SFU_COEFF47(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF47, val)
/* DX_L2_NPU0.SFU_COEFF47.PAF_ADD_COEFF8 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8_LEN        32
#define DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF47, DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF47, DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8_MASK, DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF47, bits, DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8_MASK, DX_L2_NPU0_SFU_COEFF47_PAF_ADD_COEFF8_OFFSET)

/* DX_L2_NPU0.SFU_COEFF48 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF48(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF48)
#define WRITE_DX_L2_NPU0_SFU_COEFF48(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF48, val)
/* DX_L2_NPU0.SFU_COEFF48.PAF_ADD_COEFF9 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9_LEN        32
#define DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF48, DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF48, DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9_MASK, DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF48, bits, DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9_MASK, DX_L2_NPU0_SFU_COEFF48_PAF_ADD_COEFF9_OFFSET)

/* DX_L2_NPU0.SFU_COEFF49 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF49(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF49)
#define WRITE_DX_L2_NPU0_SFU_COEFF49(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF49, val)
/* DX_L2_NPU0.SFU_COEFF49.PAF_ADD_COEFF10 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10_LEN        32
#define DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF49, DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF49, DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10_MASK, DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF49, bits, DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10_MASK, DX_L2_NPU0_SFU_COEFF49_PAF_ADD_COEFF10_OFFSET)

/* DX_L2_NPU0.SFU_COEFF50 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF50(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF50)
#define WRITE_DX_L2_NPU0_SFU_COEFF50(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF50, val)
/* DX_L2_NPU0.SFU_COEFF50.PAF_ADD_COEFF11 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11_LEN        32
#define DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF50, DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF50, DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11_MASK, DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF50, bits, DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11_MASK, DX_L2_NPU0_SFU_COEFF50_PAF_ADD_COEFF11_OFFSET)

/* DX_L2_NPU0.SFU_COEFF51 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF51(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF51)
#define WRITE_DX_L2_NPU0_SFU_COEFF51(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF51, val)
/* DX_L2_NPU0.SFU_COEFF51.PAF_ADD_COEFF12 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12_LEN        32
#define DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF51, DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF51, DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12_MASK, DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF51, bits, DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12_MASK, DX_L2_NPU0_SFU_COEFF51_PAF_ADD_COEFF12_OFFSET)

/* DX_L2_NPU0.SFU_COEFF52 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF52(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF52)
#define WRITE_DX_L2_NPU0_SFU_COEFF52(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF52, val)
/* DX_L2_NPU0.SFU_COEFF52.PAF_ADD_COEFF13 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13_LEN        32
#define DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF52, DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF52, DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13_MASK, DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF52, bits, DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13_MASK, DX_L2_NPU0_SFU_COEFF52_PAF_ADD_COEFF13_OFFSET)

/* DX_L2_NPU0.SFU_COEFF53 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF53(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF53)
#define WRITE_DX_L2_NPU0_SFU_COEFF53(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF53, val)
/* DX_L2_NPU0.SFU_COEFF53.PAF_ADD_COEFF14 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14_LEN        32
#define DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF53, DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF53, DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14_MASK, DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF53, bits, DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14_MASK, DX_L2_NPU0_SFU_COEFF53_PAF_ADD_COEFF14_OFFSET)

/* DX_L2_NPU0.SFU_COEFF54 (Category: NPU0) */
#define READ_DX_L2_NPU0_SFU_COEFF54(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_SFU_COEFF54)
#define WRITE_DX_L2_NPU0_SFU_COEFF54(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_SFU_COEFF54, val)
/* DX_L2_NPU0.SFU_COEFF54.PAF_ADD_COEFF15 (Category: NPU0) */
#define DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15_LEN        32
#define DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15_OFFSET     0
#define DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_SFU_COEFF54, DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15_MASK)
#define READ_DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_SFU_COEFF54, DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15_MASK, DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15_OFFSET)
#define WRITE_DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_SFU_COEFF54, bits, DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15_MASK, DX_L2_NPU0_SFU_COEFF54_PAF_ADD_COEFF15_OFFSET)

/* DX_L2_NPU0.RSVD1 (Category: NPU0) */
#define READ_DX_L2_NPU0_RSVD1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_RSVD1)
#define WRITE_DX_L2_NPU0_RSVD1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_RSVD1, val)
/* DX_L2_NPU0.RSVD1.RESERVED3 (Category: NPU0) */
#define DX_L2_NPU0_RSVD1_RESERVED3_LEN        8
#define DX_L2_NPU0_RSVD1_RESERVED3_OFFSET     0
#define DX_L2_NPU0_RSVD1_RESERVED3_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD1_RESERVED3_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_NPU0_RSVD1_RESERVED3(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD1, DX_L2_NPU0_RSVD1_RESERVED3_MASK)
#define READ_DX_L2_NPU0_RSVD1_RESERVED3(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD1, DX_L2_NPU0_RSVD1_RESERVED3_MASK, DX_L2_NPU0_RSVD1_RESERVED3_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD1_RESERVED3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD1, bits, DX_L2_NPU0_RSVD1_RESERVED3_MASK, DX_L2_NPU0_RSVD1_RESERVED3_OFFSET)

/* DX_L2_NPU0.RSVD1.RESERVED2 (Category: NPU0) */
#define DX_L2_NPU0_RSVD1_RESERVED2_LEN        8
#define DX_L2_NPU0_RSVD1_RESERVED2_OFFSET     8
#define DX_L2_NPU0_RSVD1_RESERVED2_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD1_RESERVED2_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_L2_NPU0_RSVD1_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD1, DX_L2_NPU0_RSVD1_RESERVED2_MASK)
#define READ_DX_L2_NPU0_RSVD1_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD1, DX_L2_NPU0_RSVD1_RESERVED2_MASK, DX_L2_NPU0_RSVD1_RESERVED2_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD1_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD1, bits, DX_L2_NPU0_RSVD1_RESERVED2_MASK, DX_L2_NPU0_RSVD1_RESERVED2_OFFSET)

/* DX_L2_NPU0.RSVD1.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_RSVD1_RESERVED1_LEN        8
#define DX_L2_NPU0_RSVD1_RESERVED1_OFFSET     16
#define DX_L2_NPU0_RSVD1_RESERVED1_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD1_RESERVED1_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD1_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD1, DX_L2_NPU0_RSVD1_RESERVED1_MASK)
#define READ_DX_L2_NPU0_RSVD1_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD1, DX_L2_NPU0_RSVD1_RESERVED1_MASK, DX_L2_NPU0_RSVD1_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD1_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD1, bits, DX_L2_NPU0_RSVD1_RESERVED1_MASK, DX_L2_NPU0_RSVD1_RESERVED1_OFFSET)

/* DX_L2_NPU0.RSVD1.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_RSVD1_RESERVED0_LEN        8
#define DX_L2_NPU0_RSVD1_RESERVED0_OFFSET     24
#define DX_L2_NPU0_RSVD1_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD1_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD1, DX_L2_NPU0_RSVD1_RESERVED0_MASK)
#define READ_DX_L2_NPU0_RSVD1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD1, DX_L2_NPU0_RSVD1_RESERVED0_MASK, DX_L2_NPU0_RSVD1_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD1, bits, DX_L2_NPU0_RSVD1_RESERVED0_MASK, DX_L2_NPU0_RSVD1_RESERVED0_OFFSET)

/* DX_L2_NPU0.RSVD2 (Category: NPU0) */
#define READ_DX_L2_NPU0_RSVD2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_RSVD2)
#define WRITE_DX_L2_NPU0_RSVD2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_RSVD2, val)
/* DX_L2_NPU0.RSVD2.RESERVED3 (Category: NPU0) */
#define DX_L2_NPU0_RSVD2_RESERVED3_LEN        8
#define DX_L2_NPU0_RSVD2_RESERVED3_OFFSET     0
#define DX_L2_NPU0_RSVD2_RESERVED3_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD2_RESERVED3_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_NPU0_RSVD2_RESERVED3(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD2, DX_L2_NPU0_RSVD2_RESERVED3_MASK)
#define READ_DX_L2_NPU0_RSVD2_RESERVED3(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD2, DX_L2_NPU0_RSVD2_RESERVED3_MASK, DX_L2_NPU0_RSVD2_RESERVED3_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD2_RESERVED3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD2, bits, DX_L2_NPU0_RSVD2_RESERVED3_MASK, DX_L2_NPU0_RSVD2_RESERVED3_OFFSET)

/* DX_L2_NPU0.RSVD2.RESERVED2 (Category: NPU0) */
#define DX_L2_NPU0_RSVD2_RESERVED2_LEN        8
#define DX_L2_NPU0_RSVD2_RESERVED2_OFFSET     8
#define DX_L2_NPU0_RSVD2_RESERVED2_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD2_RESERVED2_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_L2_NPU0_RSVD2_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD2, DX_L2_NPU0_RSVD2_RESERVED2_MASK)
#define READ_DX_L2_NPU0_RSVD2_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD2, DX_L2_NPU0_RSVD2_RESERVED2_MASK, DX_L2_NPU0_RSVD2_RESERVED2_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD2_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD2, bits, DX_L2_NPU0_RSVD2_RESERVED2_MASK, DX_L2_NPU0_RSVD2_RESERVED2_OFFSET)

/* DX_L2_NPU0.RSVD2.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_RSVD2_RESERVED1_LEN        8
#define DX_L2_NPU0_RSVD2_RESERVED1_OFFSET     16
#define DX_L2_NPU0_RSVD2_RESERVED1_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD2_RESERVED1_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD2_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD2, DX_L2_NPU0_RSVD2_RESERVED1_MASK)
#define READ_DX_L2_NPU0_RSVD2_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD2, DX_L2_NPU0_RSVD2_RESERVED1_MASK, DX_L2_NPU0_RSVD2_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD2_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD2, bits, DX_L2_NPU0_RSVD2_RESERVED1_MASK, DX_L2_NPU0_RSVD2_RESERVED1_OFFSET)

/* DX_L2_NPU0.RSVD2.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_RSVD2_RESERVED0_LEN        8
#define DX_L2_NPU0_RSVD2_RESERVED0_OFFSET     24
#define DX_L2_NPU0_RSVD2_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD2_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD2_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD2, DX_L2_NPU0_RSVD2_RESERVED0_MASK)
#define READ_DX_L2_NPU0_RSVD2_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD2, DX_L2_NPU0_RSVD2_RESERVED0_MASK, DX_L2_NPU0_RSVD2_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD2_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD2, bits, DX_L2_NPU0_RSVD2_RESERVED0_MASK, DX_L2_NPU0_RSVD2_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_IMG2COL0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_IMG2COL0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0)
#define WRITE_DX_L2_NPU0_PE0_IMG2COL0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, val)
/* DX_L2_NPU0.PE0_IMG2COL0.PE0_IMG2COL_CH_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET_LEN        16
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET_OFFSET     0
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET_MASK, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, bits, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET_MASK, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_CH_OFFSET_OFFSET)

/* DX_L2_NPU0.PE0_IMG2COL0.PE0_IMG2COL_LINE_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET_LEN        8
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET_OFFSET     16
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET_MASK, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, bits, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET_MASK, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_LINE_OFFSET_OFFSET)

/* DX_L2_NPU0.PE0_IMG2COL0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_IMG2COL0_RESERVED0_LEN        7
#define DX_L2_NPU0_PE0_IMG2COL0_RESERVED0_OFFSET     24
#define DX_L2_NPU0_PE0_IMG2COL0_RESERVED0_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_L2_NPU0_PE0_IMG2COL0_RESERVED0_MASK       0x7f000000 /* [30:24] 0b01111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_IMG2COL0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, DX_L2_NPU0_PE0_IMG2COL0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_IMG2COL0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, DX_L2_NPU0_PE0_IMG2COL0_RESERVED0_MASK, DX_L2_NPU0_PE0_IMG2COL0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_IMG2COL0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, bits, DX_L2_NPU0_PE0_IMG2COL0_RESERVED0_MASK, DX_L2_NPU0_PE0_IMG2COL0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_IMG2COL0.PE0_IMG2COL_EN (Category: NPU0) */
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN_LEN        1
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN_OFFSET     31
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN_MASK)
#define READ_DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN_MASK, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL0, bits, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN_MASK, DX_L2_NPU0_PE0_IMG2COL0_PE0_IMG2COL_EN_OFFSET)

/* DX_L2_NPU0.PE0_IMG2COL1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_IMG2COL1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL1)
#define WRITE_DX_L2_NPU0_PE0_IMG2COL1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL1, val)
/* DX_L2_NPU0.PE0_IMG2COL1.PE0_IMG2COL_OUT_FEATURE_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH_LEN        16
#define DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH_OFFSET     0
#define DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL1, DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH_MASK)
#define READ_DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL1, DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH_MASK, DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL1, bits, DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH_MASK, DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OUT_FEATURE_WIDTH_OFFSET)

/* DX_L2_NPU0.PE0_IMG2COL1.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_IMG2COL1_RESERVED0_LEN        8
#define DX_L2_NPU0_PE0_IMG2COL1_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE0_IMG2COL1_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE0_IMG2COL1_RESERVED0_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_IMG2COL1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL1, DX_L2_NPU0_PE0_IMG2COL1_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_IMG2COL1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL1, DX_L2_NPU0_PE0_IMG2COL1_RESERVED0_MASK, DX_L2_NPU0_PE0_IMG2COL1_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_IMG2COL1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL1, bits, DX_L2_NPU0_PE0_IMG2COL1_RESERVED0_MASK, DX_L2_NPU0_PE0_IMG2COL1_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_IMG2COL1.PE0_IMG2COL_OHEIGHT_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET_LEN        8
#define DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET_OFFSET     24
#define DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL1, DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL1, DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET_MASK, DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_IMG2COL1, bits, DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET_MASK, DX_L2_NPU0_PE0_IMG2COL1_PE0_IMG2COL_OHEIGHT_OFFSET_OFFSET)

/* DX_L2_NPU0.PE0_FMT (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_FMT(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_FMT)
#define WRITE_DX_L2_NPU0_PE0_FMT(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_FMT, val)
/* DX_L2_NPU0.PE0_FMT.PE0_FMT_FIRST_LINE_OPT (Category: NPU0) */
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT_LEN        2
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT_OFFSET     0
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT_MASK       0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define READ_MASK_DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_FMT, DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT_MASK)
#define READ_DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_FMT, DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT_MASK, DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_FMT, bits, DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT_MASK, DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_LINE_OPT_OFFSET)

/* DX_L2_NPU0.PE0_FMT.PE0_FMT_FIRST_VALID_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM_LEN        6
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM_OFFSET     2
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM_MASK       0x000000fc /* [7:2] 0b00000000000000000000000011111100 */
#define READ_MASK_DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_FMT, DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM_MASK)
#define READ_DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_FMT, DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM_MASK, DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_FMT, bits, DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM_MASK, DX_L2_NPU0_PE0_FMT_PE0_FMT_FIRST_VALID_NUM_OFFSET)

/* DX_L2_NPU0.PE0_FMT.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_FMT_RESERVED0_LEN        23
#define DX_L2_NPU0_PE0_FMT_RESERVED0_OFFSET     8
#define DX_L2_NPU0_PE0_FMT_RESERVED0_MASK_BITS  0x007fffff /* [22:0] 0b00000000011111111111111111111111 */
#define DX_L2_NPU0_PE0_FMT_RESERVED0_MASK       0x7fffff00 /* [30:8] 0b01111111111111111111111100000000 */
#define READ_MASK_DX_L2_NPU0_PE0_FMT_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_FMT, DX_L2_NPU0_PE0_FMT_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_FMT_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_FMT, DX_L2_NPU0_PE0_FMT_RESERVED0_MASK, DX_L2_NPU0_PE0_FMT_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_FMT_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_FMT, bits, DX_L2_NPU0_PE0_FMT_RESERVED0_MASK, DX_L2_NPU0_PE0_FMT_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_FMT.PE0_FMT_ENABLE (Category: NPU0) */
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE_LEN        1
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE_OFFSET     31
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_FMT, DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE_MASK)
#define READ_DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_FMT, DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE_MASK, DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_FMT, bits, DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE_MASK, DX_L2_NPU0_PE0_FMT_PE0_FMT_ENABLE_OFFSET)

/* DX_L2_NPU0.PE0_FMT_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_FMT_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_FMT_CFG0)
#define WRITE_DX_L2_NPU0_PE0_FMT_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_FMT_CFG0, val)
/* DX_L2_NPU0.PE0_FMT_CFG0.PE0_FMT_READ_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_FMT_CFG0, DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_FMT_CFG0, DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_FMT_CFG0, bits, DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE0_FMT_CFG0_PE0_FMT_READ_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_ADDR_GEN0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_ADDR_GEN0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN0)
#define WRITE_DX_L2_NPU0_PE0_ADDR_GEN0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN0, val)
/* DX_L2_NPU0.PE0_ADDR_GEN0.PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_LEN        16
#define DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET     0
#define DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN0, DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK)
#define READ_DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN0, DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK, DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN0, bits, DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK, DX_L2_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET)

/* DX_L2_NPU0.PE0_ADDR_GEN0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0_LEN        16
#define DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN0, DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN0, DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0_MASK, DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN0, bits, DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0_MASK, DX_L2_NPU0_PE0_ADDR_GEN0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_ADDR_GEN1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_ADDR_GEN1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN1)
#define WRITE_DX_L2_NPU0_PE0_ADDR_GEN1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN1, val)
/* DX_L2_NPU0.PE0_ADDR_GEN1.PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN1, DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN1, DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK, DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN1, bits, DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK, DX_L2_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_ADDR_GEN2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_ADDR_GEN2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN2)
#define WRITE_DX_L2_NPU0_PE0_ADDR_GEN2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN2, val)
/* DX_L2_NPU0.PE0_ADDR_GEN2.PE0_BN_ADDR_GEN_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE_LEN        2
#define DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE_OFFSET     0
#define DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE_MASK       0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define READ_MASK_DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN2, DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE_MASK)
#define READ_DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN2, DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE_MASK, DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN2, bits, DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE_MASK, DX_L2_NPU0_PE0_ADDR_GEN2_PE0_BN_ADDR_GEN_MODE_OFFSET)

/* DX_L2_NPU0.PE0_ADDR_GEN2.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0_LEN        30
#define DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0_OFFSET     2
#define DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0_MASK_BITS  0x3fffffff /* [29:0] 0b00111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0_MASK       0xfffffffc /* [31:2] 0b11111111111111111111111111111100 */
#define READ_MASK_DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN2, DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN2, DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0_MASK, DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN2, bits, DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0_MASK, DX_L2_NPU0_PE0_ADDR_GEN2_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_ADDR_GEN3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_ADDR_GEN3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN3)
#define WRITE_DX_L2_NPU0_PE0_ADDR_GEN3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN3, val)
/* DX_L2_NPU0.PE0_ADDR_GEN3.PE0_BN_ADDR_GEN_MODE1_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET_LEN        16
#define DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET_OFFSET     0
#define DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN3, DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN3, DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET_MASK, DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN3, bits, DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET_MASK, DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_OFFSET_OFFSET)

/* DX_L2_NPU0.PE0_ADDR_GEN3.PE0_BN_ADDR_GEN_MODE1_MAX_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM_LEN        16
#define DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM_OFFSET     16
#define DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN3, DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM_MASK)
#define READ_DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN3, DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM_MASK, DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_ADDR_GEN3, bits, DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM_MASK, DX_L2_NPU0_PE0_ADDR_GEN3_PE0_BN_ADDR_GEN_MODE1_MAX_NUM_OFFSET)

/* DX_L2_NPU0.PE0_CTRL (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CTRL(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CTRL)
#define WRITE_DX_L2_NPU0_PE0_CTRL(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CTRL, val)
/* DX_L2_NPU0.PE0_CTRL.PE0_CONV_TYPE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE_LEN        8
#define DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE_OFFSET     0
#define DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE_MASK)
#define READ_DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE_MASK, DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, bits, DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE_MASK, DX_L2_NPU0_PE0_CTRL_PE0_CONV_TYPE_OFFSET)

/* DX_L2_NPU0.PE0_CTRL.PE0_CONV_EN (Category: NPU0) */
#define DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN_LEN        1
#define DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN_OFFSET     8
#define DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN_MASK)
#define READ_DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN_MASK, DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, bits, DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN_MASK, DX_L2_NPU0_PE0_CTRL_PE0_CONV_EN_OFFSET)

/* DX_L2_NPU0.PE0_CTRL.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CTRL_RESERVED0_LEN        7
#define DX_L2_NPU0_PE0_CTRL_RESERVED0_OFFSET     9
#define DX_L2_NPU0_PE0_CTRL_RESERVED0_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_L2_NPU0_PE0_CTRL_RESERVED0_MASK       0x0000fe00 /* [15:9] 0b00000000000000001111111000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CTRL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CTRL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_RESERVED0_MASK, DX_L2_NPU0_PE0_CTRL_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CTRL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, bits, DX_L2_NPU0_PE0_CTRL_RESERVED0_MASK, DX_L2_NPU0_PE0_CTRL_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_CTRL.PE0_SYSTOLIC_DEPTH (Category: NPU0) */
#define DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_LEN        8
#define DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_OFFSET     16
#define DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_MASK)
#define READ_DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_MASK, DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, bits, DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_MASK, DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_OFFSET)

/* DX_L2_NPU0.PE0_CTRL.PE0_SYSTOLIC_EN (Category: NPU0) */
#define DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_LEN        1
#define DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_OFFSET     24
#define DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_MASK)
#define READ_DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_MASK, DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, bits, DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_MASK, DX_L2_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_OFFSET)

/* DX_L2_NPU0.PE0_CTRL.PE0_SFUIN_VALID_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_LEN        7
#define DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_OFFSET     25
#define DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_MASK       0xfe000000 /* [31:25] 0b11111110000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_MASK)
#define READ_DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_MASK, DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CTRL, bits, DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_MASK, DX_L2_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_OFFSET)

/* DX_L2_NPU0.PE0_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG0)
#define WRITE_DX_L2_NPU0_PE0_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG0, val)
/* DX_L2_NPU0.PE0_CFG0.PE0_LAST_CONV_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_LEN        16
#define DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_OFFSET     0
#define DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG0, DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_MASK)
#define READ_DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG0, DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_MASK, DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG0, bits, DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_MASK, DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_OFFSET)

/* DX_L2_NPU0.PE0_CFG0.PE0_LAST_CONV_MOD (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_LEN        8
#define DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_OFFSET     16
#define DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG0, DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_MASK)
#define READ_DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG0, DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_MASK, DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG0, bits, DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_MASK, DX_L2_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_OFFSET)

/* DX_L2_NPU0.PE0_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG0_RESERVED0_LEN        8
#define DX_L2_NPU0_PE0_CFG0_RESERVED0_OFFSET     24
#define DX_L2_NPU0_PE0_CFG0_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE0_CFG0_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG0, DX_L2_NPU0_PE0_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG0, DX_L2_NPU0_PE0_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG0, bits, DX_L2_NPU0_PE0_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG1)
#define WRITE_DX_L2_NPU0_PE0_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG1, val)
/* DX_L2_NPU0.PE0_CFG1.PE0_OFEATURE_CHANNEL (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_LEN        16
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_OFFSET     0
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG1, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_MASK)
#define READ_DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG1, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_MASK, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG1, bits, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_MASK, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_OFFSET)

/* DX_L2_NPU0.PE0_CFG1.PE0_OFEATURE_NUMBER (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_LEN        15
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_OFFSET     16
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_MASK       0x7fff0000 /* [30:16] 0b01111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG1, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_MASK)
#define READ_DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG1, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_MASK, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG1, bits, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_MASK, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_OFFSET)

/* DX_L2_NPU0.PE0_CFG1.PE0_OFEATURE_DISABLE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE_LEN        1
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE_OFFSET     31
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG1, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG1, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE_MASK, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG1, bits, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE_MASK, DX_L2_NPU0_PE0_CFG1_PE0_OFEATURE_DISABLE_OFFSET)

/* DX_L2_NPU0.PE0_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG2)
#define WRITE_DX_L2_NPU0_PE0_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG2, val)
/* DX_L2_NPU0.PE0_CFG2.PE0_OFEATURE_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_LEN        10
#define DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_OFFSET     0
#define DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_MASK       0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG2, DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_MASK)
#define READ_DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG2, DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_MASK, DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG2, bits, DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_MASK, DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_OFFSET)

/* DX_L2_NPU0.PE0_CFG2.PE0_OFEATURE_HEIGHT (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_LEN        10
#define DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_OFFSET     10
#define DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_MASK       0x000ffc00 /* [19:10] 0b00000000000011111111110000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG2, DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_MASK)
#define READ_DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG2, DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_MASK, DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG2, bits, DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_MASK, DX_L2_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_OFFSET)

/* DX_L2_NPU0.PE0_CFG2.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG2_RESERVED0_LEN        12
#define DX_L2_NPU0_PE0_CFG2_RESERVED0_OFFSET     20
#define DX_L2_NPU0_PE0_CFG2_RESERVED0_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE0_CFG2_RESERVED0_MASK       0xfff00000 /* [31:20] 0b11111111111100000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG2_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG2, DX_L2_NPU0_PE0_CFG2_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CFG2_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG2, DX_L2_NPU0_PE0_CFG2_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG2_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG2_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG2, bits, DX_L2_NPU0_PE0_CFG2_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG2_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG3)
#define WRITE_DX_L2_NPU0_PE0_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG3, val)
/* DX_L2_NPU0.PE0_CFG3.PE0_FEATURE_CHANNEL (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL_LEN        16
#define DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL_OFFSET     0
#define DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG3, DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL_MASK)
#define READ_DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG3, DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL_MASK, DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG3, bits, DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL_MASK, DX_L2_NPU0_PE0_CFG3_PE0_FEATURE_CHANNEL_OFFSET)

/* DX_L2_NPU0.PE0_CFG3.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG3_RESERVED0_LEN        16
#define DX_L2_NPU0_PE0_CFG3_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE0_CFG3_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG3_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG3_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG3, DX_L2_NPU0_PE0_CFG3_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CFG3_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG3, DX_L2_NPU0_PE0_CFG3_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG3_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG3_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG3, bits, DX_L2_NPU0_PE0_CFG3_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG3_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_CFG4 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG4(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG4)
#define WRITE_DX_L2_NPU0_PE0_CFG4(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG4, val)
/* DX_L2_NPU0.PE0_CFG4.PE0_FEATURE_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH_LEN        10
#define DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH_OFFSET     0
#define DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH_MASK       0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG4, DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH_MASK)
#define READ_DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG4, DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH_MASK, DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG4, bits, DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH_MASK, DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_WIDTH_OFFSET)

/* DX_L2_NPU0.PE0_CFG4.PE0_FEATURE_HEIGHT (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_LEN        10
#define DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_OFFSET     10
#define DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_MASK       0x000ffc00 /* [19:10] 0b00000000000011111111110000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG4, DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_MASK)
#define READ_DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG4, DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_MASK, DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG4, bits, DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_MASK, DX_L2_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_OFFSET)

/* DX_L2_NPU0.PE0_CFG4.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG4_RESERVED0_LEN        12
#define DX_L2_NPU0_PE0_CFG4_RESERVED0_OFFSET     20
#define DX_L2_NPU0_PE0_CFG4_RESERVED0_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE0_CFG4_RESERVED0_MASK       0xfff00000 /* [31:20] 0b11111111111100000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG4_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG4, DX_L2_NPU0_PE0_CFG4_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CFG4_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG4, DX_L2_NPU0_PE0_CFG4_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG4_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG4_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG4, bits, DX_L2_NPU0_PE0_CFG4_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG4_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_CFG5 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG5(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG5)
#define WRITE_DX_L2_NPU0_PE0_CFG5(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG5, val)
/* DX_L2_NPU0.PE0_CFG5.PE0_FILTER_CHANNEL (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL_LEN        16
#define DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL_OFFSET     0
#define DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG5, DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL_MASK)
#define READ_DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG5, DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL_MASK, DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG5, bits, DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL_MASK, DX_L2_NPU0_PE0_CFG5_PE0_FILTER_CHANNEL_OFFSET)

/* DX_L2_NPU0.PE0_CFG5.PE0_FILTER_NUMBER (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_LEN        16
#define DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_OFFSET     16
#define DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG5, DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_MASK)
#define READ_DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG5, DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_MASK, DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG5, bits, DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_MASK, DX_L2_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_OFFSET)

/* DX_L2_NPU0.PE0_CFG6 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG6(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG6)
#define WRITE_DX_L2_NPU0_PE0_CFG6(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG6, val)
/* DX_L2_NPU0.PE0_CFG6.PE0_FILTER_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_LEN        4
#define DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_OFFSET     0
#define DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_MASK       0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG6, DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_MASK)
#define READ_DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG6, DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_MASK, DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG6, bits, DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_MASK, DX_L2_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_OFFSET)

/* DX_L2_NPU0.PE0_CFG6.PE0_FILTER_HEIGHT (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_LEN        4
#define DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_OFFSET     4
#define DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_MASK       0x000000f0 /* [7:4] 0b00000000000000000000000011110000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG6, DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_MASK)
#define READ_DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG6, DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_MASK, DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG6, bits, DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_MASK, DX_L2_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_OFFSET)

/* DX_L2_NPU0.PE0_CFG6.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG6_RESERVED0_LEN        24
#define DX_L2_NPU0_PE0_CFG6_RESERVED0_OFFSET     8
#define DX_L2_NPU0_PE0_CFG6_RESERVED0_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG6_RESERVED0_MASK       0xffffff00 /* [31:8] 0b11111111111111111111111100000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG6_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG6, DX_L2_NPU0_PE0_CFG6_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CFG6_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG6, DX_L2_NPU0_PE0_CFG6_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG6_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG6_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG6, bits, DX_L2_NPU0_PE0_CFG6_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG6_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_CFG7 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG7(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG7)
#define WRITE_DX_L2_NPU0_PE0_CFG7(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG7, val)
/* DX_L2_NPU0.PE0_CFG7.PE0_PAD_TOP_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_LEN        4
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_OFFSET     0
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_MASK       0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_MASK, DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, bits, DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_MASK, DX_L2_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_CFG7.PE0_PAD_BOTTOM_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_LEN        4
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_OFFSET     4
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_MASK       0x000000f0 /* [7:4] 0b00000000000000000000000011110000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_MASK, DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, bits, DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_MASK, DX_L2_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_CFG7.PE0_PAD_LEFT_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_LEN        4
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_OFFSET     8
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_MASK       0x00000f00 /* [11:8] 0b00000000000000000000111100000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_MASK, DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, bits, DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_MASK, DX_L2_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_CFG7.PE0_PAD_RIGHT_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_LEN        4
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_OFFSET     12
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_MASK       0x0000f000 /* [15:12] 0b00000000000000001111000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_MASK, DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, bits, DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_MASK, DX_L2_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_CFG7.PE0_PAD_VALUE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE_LEN        8
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE_OFFSET     16
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE_MASK, DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, bits, DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE_MASK, DX_L2_NPU0_PE0_CFG7_PE0_PAD_VALUE_OFFSET)

/* DX_L2_NPU0.PE0_CFG7.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG7_RESERVED0_LEN        8
#define DX_L2_NPU0_PE0_CFG7_RESERVED0_OFFSET     24
#define DX_L2_NPU0_PE0_CFG7_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE0_CFG7_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG7_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CFG7_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, DX_L2_NPU0_PE0_CFG7_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG7_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG7_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG7, bits, DX_L2_NPU0_PE0_CFG7_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG7_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_CFG8 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG8(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG8)
#define WRITE_DX_L2_NPU0_PE0_CFG8(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG8, val)
/* DX_L2_NPU0.PE0_CFG8.PE0_STRIDE_X (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X_LEN        4
#define DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X_OFFSET     0
#define DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X_MASK       0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG8, DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X_MASK)
#define READ_DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG8, DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X_MASK, DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG8, bits, DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X_MASK, DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_X_OFFSET)

/* DX_L2_NPU0.PE0_CFG8.PE0_STRIDE_Y (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y_LEN        4
#define DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y_OFFSET     4
#define DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y_MASK       0x000000f0 /* [7:4] 0b00000000000000000000000011110000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG8, DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y_MASK)
#define READ_DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG8, DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y_MASK, DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG8, bits, DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y_MASK, DX_L2_NPU0_PE0_CFG8_PE0_STRIDE_Y_OFFSET)

/* DX_L2_NPU0.PE0_CFG8.PE0_DILATION_X (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X_LEN        4
#define DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X_OFFSET     8
#define DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X_MASK       0x00000f00 /* [11:8] 0b00000000000000000000111100000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG8, DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X_MASK)
#define READ_DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG8, DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X_MASK, DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG8, bits, DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X_MASK, DX_L2_NPU0_PE0_CFG8_PE0_DILATION_X_OFFSET)

/* DX_L2_NPU0.PE0_CFG8.PE0_DILATION_Y (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y_LEN        4
#define DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y_OFFSET     12
#define DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y_MASK       0x0000f000 /* [15:12] 0b00000000000000001111000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG8, DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y_MASK)
#define READ_DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG8, DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y_MASK, DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG8, bits, DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y_MASK, DX_L2_NPU0_PE0_CFG8_PE0_DILATION_Y_OFFSET)

/* DX_L2_NPU0.PE0_CFG8.PE0_CHANNEL_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_LEN        16
#define DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_OFFSET     16
#define DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG8, DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG8, DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_MASK, DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG8, bits, DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_MASK, DX_L2_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_OFFSET)

/* DX_L2_NPU0.PE0_CFG9 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG9(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG9)
#define WRITE_DX_L2_NPU0_PE0_CFG9(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG9, val)
/* DX_L2_NPU0.PE0_CFG9.PE0_READ_FEATURE_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG9, DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG9, DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG9, bits, DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG9_PE0_READ_FEATURE_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_CFG10 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG10(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG10)
#define WRITE_DX_L2_NPU0_PE0_CFG10(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG10, val)
/* DX_L2_NPU0.PE0_CFG10.PE0_WRITE_FEATURE_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG10, DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG10, DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG10, bits, DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG10_PE0_WRITE_FEATURE_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_CFG11 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG11(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG11)
#define WRITE_DX_L2_NPU0_PE0_CFG11(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG11, val)
/* DX_L2_NPU0.PE0_CFG11.PE0_READ_WEIGHT_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG11, DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG11, DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG11, bits, DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG11_PE0_READ_WEIGHT_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_CFG12 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG12(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG12)
#define WRITE_DX_L2_NPU0_PE0_CFG12(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG12, val)
/* DX_L2_NPU0.PE0_CFG12.PE0_BN_COEFF_READ_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG12, DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG12, DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG12, bits, DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG12_PE0_BN_COEFF_READ_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_CFG13 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG13(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG13)
#define WRITE_DX_L2_NPU0_PE0_CFG13(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG13, val)
/* DX_L2_NPU0.PE0_CFG13.PE0_BIAS_READ_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG13, DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG13, DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG13, bits, DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG13_PE0_BIAS_READ_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_CFG14 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG14(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG14)
#define WRITE_DX_L2_NPU0_PE0_CFG14(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG14, val)
/* DX_L2_NPU0.PE0_CFG14.PE0_MUL_READ_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG14, DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG14, DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG14, bits, DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG14_PE0_MUL_READ_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_CFG15 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG15(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG15)
#define WRITE_DX_L2_NPU0_PE0_CFG15(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG15, val)
/* DX_L2_NPU0.PE0_CFG15.PE0_ELEM_READ_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG15, DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG15, DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG15, bits, DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE0_CFG15_PE0_ELEM_READ_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_CFG16 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG16(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG16)
#define WRITE_DX_L2_NPU0_PE0_CFG16(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG16, val)
/* DX_L2_NPU0.PE0_CFG16.PE0_ELEM_CHANNEL_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET_LEN        16
#define DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET_OFFSET     0
#define DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG16, DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG16, DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET_MASK, DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG16, bits, DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET_MASK, DX_L2_NPU0_PE0_CFG16_PE0_ELEM_CHANNEL_OFFSET_OFFSET)

/* DX_L2_NPU0.PE0_CFG16.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG16_RESERVED0_LEN        16
#define DX_L2_NPU0_PE0_CFG16_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE0_CFG16_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG16_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG16_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG16, DX_L2_NPU0_PE0_CFG16_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CFG16_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG16, DX_L2_NPU0_PE0_CFG16_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG16_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG16_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG16, bits, DX_L2_NPU0_PE0_CFG16_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG16_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_CFG17 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG17(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG17)
#define WRITE_DX_L2_NPU0_PE0_CFG17(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG17, val)
/* DX_L2_NPU0.PE0_CFG17.PE0_WF_ADDR_MODE1_LINE_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE_LEN        16
#define DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE_OFFSET     0
#define DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG17, DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG17, DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE_MASK, DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG17, bits, DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE_MASK, DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_MODE1_LINE_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_CFG17.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG17_RESERVED0_LEN        14
#define DX_L2_NPU0_PE0_CFG17_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE0_CFG17_RESERVED0_MASK_BITS  0x00003fff /* [13:0] 0b00000000000000000011111111111111 */
#define DX_L2_NPU0_PE0_CFG17_RESERVED0_MASK       0x3fff0000 /* [29:16] 0b00111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG17_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG17, DX_L2_NPU0_PE0_CFG17_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CFG17_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG17, DX_L2_NPU0_PE0_CFG17_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG17_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG17_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG17, bits, DX_L2_NPU0_PE0_CFG17_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG17_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_CFG17.PE0_WF_ADDR_GEN_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE_LEN        2
#define DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE_OFFSET     30
#define DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE_MASK       0xc0000000 /* [31:30] 0b11000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG17, DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG17, DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE_MASK, DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG17, bits, DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE_MASK, DX_L2_NPU0_PE0_CFG17_PE0_WF_ADDR_GEN_MODE_OFFSET)

/* DX_L2_NPU0.PE0_CFG18 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG18(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG18)
#define WRITE_DX_L2_NPU0_PE0_CFG18(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG18, val)
/* DX_L2_NPU0.PE0_CFG18.PE0_WF_ADDR_MODE1_LINE_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE_LEN        32
#define DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG18, DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG18, DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE_MASK, DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG18, bits, DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE_MASK, DX_L2_NPU0_PE0_CFG18_PE0_WF_ADDR_MODE1_LINE_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_CFG19 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG19(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG19)
#define WRITE_DX_L2_NPU0_PE0_CFG19(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG19, val)
/* DX_L2_NPU0.PE0_CFG19.PE0_WF_ADDR_LINE_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE_LEN        16
#define DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG19, DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG19, DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE_MASK, DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG19, bits, DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE_MASK, DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_LINE_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_CFG19.PE0_WF_ADDR_SURFACE_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE_LEN        16
#define DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE_OFFSET     16
#define DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG19, DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG19, DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE_MASK, DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG19, bits, DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE_MASK, DX_L2_NPU0_PE0_CFG19_PE0_WF_ADDR_SURFACE_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_CFG20 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG20(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG20)
#define WRITE_DX_L2_NPU0_PE0_CFG20(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG20, val)
/* DX_L2_NPU0.PE0_CFG20.PE0_WF_WRITE_CNT (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT_LEN        16
#define DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT_OFFSET     0
#define DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG20, DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT_MASK)
#define READ_DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG20, DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT_MASK, DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG20, bits, DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT_MASK, DX_L2_NPU0_PE0_CFG20_PE0_WF_WRITE_CNT_OFFSET)

/* DX_L2_NPU0.PE0_CFG20.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG20_RESERVED0_LEN        8
#define DX_L2_NPU0_PE0_CFG20_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE0_CFG20_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE0_CFG20_RESERVED0_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG20_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG20, DX_L2_NPU0_PE0_CFG20_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CFG20_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG20, DX_L2_NPU0_PE0_CFG20_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG20_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG20_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG20, bits, DX_L2_NPU0_PE0_CFG20_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG20_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_CFG20.PE0_WF_RF2RF_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME_LEN        8
#define DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME_OFFSET     24
#define DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG20, DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME_MASK)
#define READ_DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG20, DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME_MASK, DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG20, bits, DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME_MASK, DX_L2_NPU0_PE0_CFG20_PE0_WF_RF2RF_TIME_OFFSET)

/* DX_L2_NPU0.PE0_CFG21 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG21(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG21)
#define WRITE_DX_L2_NPU0_PE0_CFG21(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG21, val)
/* DX_L2_NPU0.PE0_CFG21.PE0_WF_RF2RF_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE_LEN        16
#define DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE_OFFSET     0
#define DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG21, DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG21, DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE_MASK, DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG21, bits, DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE_MASK, DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_CFG21.PE0_WF_RF2RF_ADDR_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE_LEN        16
#define DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE_OFFSET     16
#define DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG21, DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG21, DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE_MASK, DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG21, bits, DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE_MASK, DX_L2_NPU0_PE0_CFG21_PE0_WF_RF2RF_ADDR_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_CFG22 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG22(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG22)
#define WRITE_DX_L2_NPU0_PE0_CFG22(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG22, val)
/* DX_L2_NPU0.PE0_CFG22.PE0_WF_RF2RF_SRC_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR_LEN        32
#define DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG22, DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG22, DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR_MASK, DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG22, bits, DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR_MASK, DX_L2_NPU0_PE0_CFG22_PE0_WF_RF2RF_SRC_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_CFG23 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG23(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG23)
#define WRITE_DX_L2_NPU0_PE0_CFG23(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG23, val)
/* DX_L2_NPU0.PE0_CFG23.PE0_WF_RF2RF_DST_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR_LEN        32
#define DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG23, DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG23, DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR_MASK, DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG23, bits, DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR_MASK, DX_L2_NPU0_PE0_CFG23_PE0_WF_RF2RF_DST_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_CFG24 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG24(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG24)
#define WRITE_DX_L2_NPU0_PE0_CFG24(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG24, val)
/* DX_L2_NPU0.PE0_CFG24.PE0_SFU_OUT_DATA_VALID_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM_LEN        3
#define DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM_OFFSET     0
#define DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM_MASK       0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG24, DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM_MASK)
#define READ_DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG24, DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM_MASK, DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG24, bits, DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM_MASK, DX_L2_NPU0_PE0_CFG24_PE0_SFU_OUT_DATA_VALID_NUM_OFFSET)

/* DX_L2_NPU0.PE0_CFG24.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG24_RESERVED1_LEN        5
#define DX_L2_NPU0_PE0_CFG24_RESERVED1_OFFSET     3
#define DX_L2_NPU0_PE0_CFG24_RESERVED1_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_L2_NPU0_PE0_CFG24_RESERVED1_MASK       0x000000f8 /* [7:3] 0b00000000000000000000000011111000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG24_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG24, DX_L2_NPU0_PE0_CFG24_RESERVED1_MASK)
#define READ_DX_L2_NPU0_PE0_CFG24_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG24, DX_L2_NPU0_PE0_CFG24_RESERVED1_MASK, DX_L2_NPU0_PE0_CFG24_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG24_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG24, bits, DX_L2_NPU0_PE0_CFG24_RESERVED1_MASK, DX_L2_NPU0_PE0_CFG24_RESERVED1_OFFSET)

/* DX_L2_NPU0.PE0_CFG24.PE0_ST_ADDR_GEN_SHIFT_LEN (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN_LEN        5
#define DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN_OFFSET     8
#define DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN_MASK       0x00001f00 /* [12:8] 0b00000000000000000001111100000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG24, DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN_MASK)
#define READ_DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG24, DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN_MASK, DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG24, bits, DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN_MASK, DX_L2_NPU0_PE0_CFG24_PE0_ST_ADDR_GEN_SHIFT_LEN_OFFSET)

/* DX_L2_NPU0.PE0_CFG24.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG24_RESERVED0_LEN        19
#define DX_L2_NPU0_PE0_CFG24_RESERVED0_OFFSET     13
#define DX_L2_NPU0_PE0_CFG24_RESERVED0_MASK_BITS  0x0007ffff /* [18:0] 0b00000000000001111111111111111111 */
#define DX_L2_NPU0_PE0_CFG24_RESERVED0_MASK       0xffffe000 /* [31:13] 0b11111111111111111110000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG24_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG24, DX_L2_NPU0_PE0_CFG24_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CFG24_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG24, DX_L2_NPU0_PE0_CFG24_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG24_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG24_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG24, bits, DX_L2_NPU0_PE0_CFG24_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG24_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_CFG25 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_CFG25(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_CFG25)
#define WRITE_DX_L2_NPU0_PE0_CFG25(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_CFG25, val)
/* DX_L2_NPU0.PE0_CFG25.PE0_ST_ADDR_GEN_MUL (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL_LEN        31
#define DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL_OFFSET     0
#define DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL_MASK       0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG25, DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL_MASK)
#define READ_DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG25, DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL_MASK, DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG25, bits, DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL_MASK, DX_L2_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_MUL_OFFSET)

/* DX_L2_NPU0.PE0_CFG25.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_CFG25_RESERVED0_LEN        1
#define DX_L2_NPU0_PE0_CFG25_RESERVED0_OFFSET     31
#define DX_L2_NPU0_PE0_CFG25_RESERVED0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_CFG25_RESERVED0_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_CFG25_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_CFG25, DX_L2_NPU0_PE0_CFG25_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_CFG25_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_CFG25, DX_L2_NPU0_PE0_CFG25_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG25_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_CFG25_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_CFG25, bits, DX_L2_NPU0_PE0_CFG25_RESERVED0_MASK, DX_L2_NPU0_PE0_CFG25_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_VMEM_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_VMEM_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0)
#define WRITE_DX_L2_NPU0_PE0_VMEM_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, val)
/* DX_L2_NPU0.PE0_VMEM_CFG0.PE0_START_NO_IN_BUFF (Category: NPU0) */
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_LEN        6
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_OFFSET     0
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_MASK)
#define READ_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, bits, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_OFFSET)

/* DX_L2_NPU0.PE0_VMEM_CFG0.RESERVED3 (Category: NPU0) */
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3_LEN        2
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3_OFFSET     6
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3_MASK)
#define READ_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, bits, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED3_OFFSET)

/* DX_L2_NPU0.PE0_VMEM_CFG0.PE0_START_NO_WEIGHT_BUFF (Category: NPU0) */
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_LEN        6
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_OFFSET     8
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_MASK       0x00003f00 /* [13:8] 0b00000000000000000011111100000000 */
#define READ_MASK_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_MASK)
#define READ_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, bits, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_OFFSET)

/* DX_L2_NPU0.PE0_VMEM_CFG0.RESERVED2 (Category: NPU0) */
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2_LEN        2
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2_OFFSET     14
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2_MASK       0x0000c000 /* [15:14] 0b00000000000000001100000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2_MASK)
#define READ_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, bits, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED2_OFFSET)

/* DX_L2_NPU0.PE0_VMEM_CFG0.PE0_START_NO_OUT_BUFF (Category: NPU0) */
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_LEN        6
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_OFFSET     16
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_MASK       0x003f0000 /* [21:16] 0b00000000001111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_MASK)
#define READ_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, bits, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_OFFSET)

/* DX_L2_NPU0.PE0_VMEM_CFG0.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1_LEN        2
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1_OFFSET     22
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1_MASK       0x00c00000 /* [23:22] 0b00000000110000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1_MASK)
#define READ_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, bits, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED1_OFFSET)

/* DX_L2_NPU0.PE0_VMEM_CFG0.PE0_START_NO_ELEM_BUFF (Category: NPU0) */
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_LEN        6
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_OFFSET     24
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_MASK       0x3f000000 /* [29:24] 0b00111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_MASK)
#define READ_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, bits, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_OFFSET)

/* DX_L2_NPU0.PE0_VMEM_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0_LEN        2
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0_OFFSET     30
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0_MASK       0xc0000000 /* [31:30] 0b11000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_VMEM_CFG0, bits, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_VMEM_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_SMEM_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_SMEM_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0)
#define WRITE_DX_L2_NPU0_PE0_SMEM_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, val)
/* DX_L2_NPU0.PE0_SMEM_CFG0.PE0_BN_BUFF_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL_LEN        1
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL_OFFSET     0
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL_MASK)
#define READ_DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL_MASK, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, bits, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL_MASK, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_BN_BUFF_SEL_OFFSET)

/* DX_L2_NPU0.PE0_SMEM_CFG0.PE0_B_BUFF_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL_LEN        1
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL_OFFSET     1
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL_MASK       0x00000002 /* [1:1] 0b00000000000000000000000000000010 */
#define READ_MASK_DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL_MASK)
#define READ_DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL_MASK, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, bits, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL_MASK, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_B_BUFF_SEL_OFFSET)

/* DX_L2_NPU0.PE0_SMEM_CFG0.PE0_MUL_BUFF_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL_LEN        1
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL_OFFSET     2
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL_MASK       0x00000004 /* [2:2] 0b00000000000000000000000000000100 */
#define READ_MASK_DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL_MASK)
#define READ_DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL_MASK, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, bits, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL_MASK, DX_L2_NPU0_PE0_SMEM_CFG0_PE0_MUL_BUFF_SEL_OFFSET)

/* DX_L2_NPU0.PE0_SMEM_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0_LEN        29
#define DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0_OFFSET     3
#define DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0_MASK_BITS  0x1fffffff /* [28:0] 0b00011111111111111111111111111111 */
#define DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0_MASK       0xfffffff8 /* [31:3] 0b11111111111111111111111111111000 */
#define READ_MASK_DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_SMEM_CFG0, bits, DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_SMEM_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_IMG2COL0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_IMG2COL0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0)
#define WRITE_DX_L2_NPU0_PE1_IMG2COL0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, val)
/* DX_L2_NPU0.PE1_IMG2COL0.PE1_IMG2COL_CH_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET_LEN        16
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET_OFFSET     0
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET_MASK, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, bits, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET_MASK, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_CH_OFFSET_OFFSET)

/* DX_L2_NPU0.PE1_IMG2COL0.PE1_IMG2COL_LINE_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET_LEN        8
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET_OFFSET     16
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET_MASK, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, bits, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET_MASK, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_LINE_OFFSET_OFFSET)

/* DX_L2_NPU0.PE1_IMG2COL0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_IMG2COL0_RESERVED0_LEN        7
#define DX_L2_NPU0_PE1_IMG2COL0_RESERVED0_OFFSET     24
#define DX_L2_NPU0_PE1_IMG2COL0_RESERVED0_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_L2_NPU0_PE1_IMG2COL0_RESERVED0_MASK       0x7f000000 /* [30:24] 0b01111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_IMG2COL0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, DX_L2_NPU0_PE1_IMG2COL0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_IMG2COL0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, DX_L2_NPU0_PE1_IMG2COL0_RESERVED0_MASK, DX_L2_NPU0_PE1_IMG2COL0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_IMG2COL0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, bits, DX_L2_NPU0_PE1_IMG2COL0_RESERVED0_MASK, DX_L2_NPU0_PE1_IMG2COL0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_IMG2COL0.PE1_IMG2COL_EN (Category: NPU0) */
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN_LEN        1
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN_OFFSET     31
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN_MASK)
#define READ_DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN_MASK, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL0, bits, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN_MASK, DX_L2_NPU0_PE1_IMG2COL0_PE1_IMG2COL_EN_OFFSET)

/* DX_L2_NPU0.PE1_IMG2COL1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_IMG2COL1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL1)
#define WRITE_DX_L2_NPU0_PE1_IMG2COL1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL1, val)
/* DX_L2_NPU0.PE1_IMG2COL1.PE1_IMG2COL_OUT_FEATURE_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH_LEN        16
#define DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH_OFFSET     0
#define DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL1, DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH_MASK)
#define READ_DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL1, DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH_MASK, DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL1, bits, DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH_MASK, DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OUT_FEATURE_WIDTH_OFFSET)

/* DX_L2_NPU0.PE1_IMG2COL1.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_IMG2COL1_RESERVED0_LEN        8
#define DX_L2_NPU0_PE1_IMG2COL1_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE1_IMG2COL1_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE1_IMG2COL1_RESERVED0_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_IMG2COL1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL1, DX_L2_NPU0_PE1_IMG2COL1_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_IMG2COL1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL1, DX_L2_NPU0_PE1_IMG2COL1_RESERVED0_MASK, DX_L2_NPU0_PE1_IMG2COL1_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_IMG2COL1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL1, bits, DX_L2_NPU0_PE1_IMG2COL1_RESERVED0_MASK, DX_L2_NPU0_PE1_IMG2COL1_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_IMG2COL1.PE1_IMG2COL_OHEIGHT_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET_LEN        8
#define DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET_OFFSET     24
#define DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL1, DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL1, DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET_MASK, DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_IMG2COL1, bits, DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET_MASK, DX_L2_NPU0_PE1_IMG2COL1_PE1_IMG2COL_OHEIGHT_OFFSET_OFFSET)

/* DX_L2_NPU0.PE1_FMT (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_FMT(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_FMT)
#define WRITE_DX_L2_NPU0_PE1_FMT(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_FMT, val)
/* DX_L2_NPU0.PE1_FMT.PE1_FMT_FIRST_LINE_OPT (Category: NPU0) */
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT_LEN        2
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT_OFFSET     0
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT_MASK       0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define READ_MASK_DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_FMT, DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT_MASK)
#define READ_DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_FMT, DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT_MASK, DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_FMT, bits, DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT_MASK, DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_LINE_OPT_OFFSET)

/* DX_L2_NPU0.PE1_FMT.PE1_FMT_FIRST_VALID_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM_LEN        6
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM_OFFSET     2
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM_MASK       0x000000fc /* [7:2] 0b00000000000000000000000011111100 */
#define READ_MASK_DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_FMT, DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM_MASK)
#define READ_DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_FMT, DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM_MASK, DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_FMT, bits, DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM_MASK, DX_L2_NPU0_PE1_FMT_PE1_FMT_FIRST_VALID_NUM_OFFSET)

/* DX_L2_NPU0.PE1_FMT.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_FMT_RESERVED0_LEN        23
#define DX_L2_NPU0_PE1_FMT_RESERVED0_OFFSET     8
#define DX_L2_NPU0_PE1_FMT_RESERVED0_MASK_BITS  0x007fffff /* [22:0] 0b00000000011111111111111111111111 */
#define DX_L2_NPU0_PE1_FMT_RESERVED0_MASK       0x7fffff00 /* [30:8] 0b01111111111111111111111100000000 */
#define READ_MASK_DX_L2_NPU0_PE1_FMT_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_FMT, DX_L2_NPU0_PE1_FMT_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_FMT_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_FMT, DX_L2_NPU0_PE1_FMT_RESERVED0_MASK, DX_L2_NPU0_PE1_FMT_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_FMT_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_FMT, bits, DX_L2_NPU0_PE1_FMT_RESERVED0_MASK, DX_L2_NPU0_PE1_FMT_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_FMT.PE1_FMT_ENABLE (Category: NPU0) */
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE_LEN        1
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE_OFFSET     31
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_FMT, DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE_MASK)
#define READ_DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_FMT, DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE_MASK, DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_FMT, bits, DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE_MASK, DX_L2_NPU0_PE1_FMT_PE1_FMT_ENABLE_OFFSET)

/* DX_L2_NPU0.PE1_FMT_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_FMT_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_FMT_CFG0)
#define WRITE_DX_L2_NPU0_PE1_FMT_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_FMT_CFG0, val)
/* DX_L2_NPU0.PE1_FMT_CFG0.PE1_FMT_READ_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_FMT_CFG0, DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_FMT_CFG0, DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_FMT_CFG0, bits, DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE1_FMT_CFG0_PE1_FMT_READ_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_ADDR_GEN0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_ADDR_GEN0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN0)
#define WRITE_DX_L2_NPU0_PE1_ADDR_GEN0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN0, val)
/* DX_L2_NPU0.PE1_ADDR_GEN0.PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_LEN        16
#define DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET     0
#define DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN0, DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK)
#define READ_DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN0, DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK, DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN0, bits, DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK, DX_L2_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET)

/* DX_L2_NPU0.PE1_ADDR_GEN0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0_LEN        16
#define DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN0, DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN0, DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0_MASK, DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN0, bits, DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0_MASK, DX_L2_NPU0_PE1_ADDR_GEN0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_ADDR_GEN1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_ADDR_GEN1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN1)
#define WRITE_DX_L2_NPU0_PE1_ADDR_GEN1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN1, val)
/* DX_L2_NPU0.PE1_ADDR_GEN1.PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN1, DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN1, DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK, DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN1, bits, DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK, DX_L2_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_ADDR_GEN2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_ADDR_GEN2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN2)
#define WRITE_DX_L2_NPU0_PE1_ADDR_GEN2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN2, val)
/* DX_L2_NPU0.PE1_ADDR_GEN2.PE1_BN_ADDR_GEN_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE_LEN        2
#define DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE_OFFSET     0
#define DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE_MASK       0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define READ_MASK_DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN2, DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE_MASK)
#define READ_DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN2, DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE_MASK, DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN2, bits, DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE_MASK, DX_L2_NPU0_PE1_ADDR_GEN2_PE1_BN_ADDR_GEN_MODE_OFFSET)

/* DX_L2_NPU0.PE1_ADDR_GEN2.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0_LEN        30
#define DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0_OFFSET     2
#define DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0_MASK_BITS  0x3fffffff /* [29:0] 0b00111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0_MASK       0xfffffffc /* [31:2] 0b11111111111111111111111111111100 */
#define READ_MASK_DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN2, DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN2, DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0_MASK, DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN2, bits, DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0_MASK, DX_L2_NPU0_PE1_ADDR_GEN2_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_ADDR_GEN3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_ADDR_GEN3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN3)
#define WRITE_DX_L2_NPU0_PE1_ADDR_GEN3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN3, val)
/* DX_L2_NPU0.PE1_ADDR_GEN3.PE1_BN_ADDR_GEN_MODE1_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET_LEN        16
#define DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET_OFFSET     0
#define DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN3, DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN3, DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET_MASK, DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN3, bits, DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET_MASK, DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_OFFSET_OFFSET)

/* DX_L2_NPU0.PE1_ADDR_GEN3.PE1_BN_ADDR_GEN_MODE1_MAX_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM_LEN        16
#define DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM_OFFSET     16
#define DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN3, DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM_MASK)
#define READ_DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN3, DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM_MASK, DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_ADDR_GEN3, bits, DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM_MASK, DX_L2_NPU0_PE1_ADDR_GEN3_PE1_BN_ADDR_GEN_MODE1_MAX_NUM_OFFSET)

/* DX_L2_NPU0.PE1_CTRL (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CTRL(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CTRL)
#define WRITE_DX_L2_NPU0_PE1_CTRL(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CTRL, val)
/* DX_L2_NPU0.PE1_CTRL.PE1_CONV_TYPE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE_LEN        8
#define DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE_OFFSET     0
#define DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE_MASK)
#define READ_DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE_MASK, DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, bits, DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE_MASK, DX_L2_NPU0_PE1_CTRL_PE1_CONV_TYPE_OFFSET)

/* DX_L2_NPU0.PE1_CTRL.PE1_CONV_EN (Category: NPU0) */
#define DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN_LEN        1
#define DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN_OFFSET     8
#define DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN_MASK)
#define READ_DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN_MASK, DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, bits, DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN_MASK, DX_L2_NPU0_PE1_CTRL_PE1_CONV_EN_OFFSET)

/* DX_L2_NPU0.PE1_CTRL.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CTRL_RESERVED0_LEN        7
#define DX_L2_NPU0_PE1_CTRL_RESERVED0_OFFSET     9
#define DX_L2_NPU0_PE1_CTRL_RESERVED0_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_L2_NPU0_PE1_CTRL_RESERVED0_MASK       0x0000fe00 /* [15:9] 0b00000000000000001111111000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CTRL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CTRL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_RESERVED0_MASK, DX_L2_NPU0_PE1_CTRL_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CTRL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, bits, DX_L2_NPU0_PE1_CTRL_RESERVED0_MASK, DX_L2_NPU0_PE1_CTRL_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_CTRL.PE1_SYSTOLIC_DEPTH (Category: NPU0) */
#define DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_LEN        8
#define DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_OFFSET     16
#define DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_MASK)
#define READ_DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_MASK, DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, bits, DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_MASK, DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_OFFSET)

/* DX_L2_NPU0.PE1_CTRL.PE1_SYSTOLIC_EN (Category: NPU0) */
#define DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_LEN        1
#define DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_OFFSET     24
#define DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_MASK)
#define READ_DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_MASK, DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, bits, DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_MASK, DX_L2_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_OFFSET)

/* DX_L2_NPU0.PE1_CTRL.PE1_SFUIN_VALID_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_LEN        7
#define DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_OFFSET     25
#define DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_MASK       0xfe000000 /* [31:25] 0b11111110000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_MASK)
#define READ_DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_MASK, DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CTRL, bits, DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_MASK, DX_L2_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_OFFSET)

/* DX_L2_NPU0.PE1_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG0)
#define WRITE_DX_L2_NPU0_PE1_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG0, val)
/* DX_L2_NPU0.PE1_CFG0.PE1_LAST_CONV_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_LEN        16
#define DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_OFFSET     0
#define DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG0, DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_MASK)
#define READ_DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG0, DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_MASK, DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG0, bits, DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_MASK, DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_OFFSET)

/* DX_L2_NPU0.PE1_CFG0.PE1_LAST_CONV_MOD (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_LEN        8
#define DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_OFFSET     16
#define DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG0, DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_MASK)
#define READ_DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG0, DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_MASK, DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG0, bits, DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_MASK, DX_L2_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_OFFSET)

/* DX_L2_NPU0.PE1_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG0_RESERVED0_LEN        8
#define DX_L2_NPU0_PE1_CFG0_RESERVED0_OFFSET     24
#define DX_L2_NPU0_PE1_CFG0_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE1_CFG0_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG0, DX_L2_NPU0_PE1_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG0, DX_L2_NPU0_PE1_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG0, bits, DX_L2_NPU0_PE1_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG1)
#define WRITE_DX_L2_NPU0_PE1_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG1, val)
/* DX_L2_NPU0.PE1_CFG1.PE1_OFEATURE_CHANNEL (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_LEN        16
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_OFFSET     0
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG1, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_MASK)
#define READ_DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG1, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_MASK, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG1, bits, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_MASK, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_OFFSET)

/* DX_L2_NPU0.PE1_CFG1.PE1_OFEATURE_NUMBER (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_LEN        15
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_OFFSET     16
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_MASK       0x7fff0000 /* [30:16] 0b01111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG1, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_MASK)
#define READ_DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG1, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_MASK, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG1, bits, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_MASK, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_OFFSET)

/* DX_L2_NPU0.PE1_CFG1.PE1_OFEATURE_DISABLE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE_LEN        1
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE_OFFSET     31
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG1, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG1, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE_MASK, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG1, bits, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE_MASK, DX_L2_NPU0_PE1_CFG1_PE1_OFEATURE_DISABLE_OFFSET)

/* DX_L2_NPU0.PE1_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG2)
#define WRITE_DX_L2_NPU0_PE1_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG2, val)
/* DX_L2_NPU0.PE1_CFG2.PE1_OFEATURE_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_LEN        10
#define DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_OFFSET     0
#define DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_MASK       0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG2, DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_MASK)
#define READ_DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG2, DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_MASK, DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG2, bits, DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_MASK, DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_OFFSET)

/* DX_L2_NPU0.PE1_CFG2.PE1_OFEATURE_HEIGHT (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_LEN        10
#define DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_OFFSET     10
#define DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_MASK       0x000ffc00 /* [19:10] 0b00000000000011111111110000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG2, DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_MASK)
#define READ_DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG2, DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_MASK, DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG2, bits, DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_MASK, DX_L2_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_OFFSET)

/* DX_L2_NPU0.PE1_CFG2.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG2_RESERVED0_LEN        12
#define DX_L2_NPU0_PE1_CFG2_RESERVED0_OFFSET     20
#define DX_L2_NPU0_PE1_CFG2_RESERVED0_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE1_CFG2_RESERVED0_MASK       0xfff00000 /* [31:20] 0b11111111111100000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG2_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG2, DX_L2_NPU0_PE1_CFG2_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CFG2_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG2, DX_L2_NPU0_PE1_CFG2_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG2_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG2_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG2, bits, DX_L2_NPU0_PE1_CFG2_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG2_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG3)
#define WRITE_DX_L2_NPU0_PE1_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG3, val)
/* DX_L2_NPU0.PE1_CFG3.PE1_FEATURE_CHANNEL (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL_LEN        16
#define DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL_OFFSET     0
#define DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG3, DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL_MASK)
#define READ_DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG3, DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL_MASK, DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG3, bits, DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL_MASK, DX_L2_NPU0_PE1_CFG3_PE1_FEATURE_CHANNEL_OFFSET)

/* DX_L2_NPU0.PE1_CFG3.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG3_RESERVED0_LEN        16
#define DX_L2_NPU0_PE1_CFG3_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE1_CFG3_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG3_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG3_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG3, DX_L2_NPU0_PE1_CFG3_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CFG3_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG3, DX_L2_NPU0_PE1_CFG3_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG3_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG3_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG3, bits, DX_L2_NPU0_PE1_CFG3_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG3_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_CFG4 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG4(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG4)
#define WRITE_DX_L2_NPU0_PE1_CFG4(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG4, val)
/* DX_L2_NPU0.PE1_CFG4.PE1_FEATURE_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH_LEN        10
#define DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH_OFFSET     0
#define DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH_MASK       0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG4, DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH_MASK)
#define READ_DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG4, DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH_MASK, DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG4, bits, DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH_MASK, DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_WIDTH_OFFSET)

/* DX_L2_NPU0.PE1_CFG4.PE1_FEATURE_HEIGHT (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_LEN        10
#define DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_OFFSET     10
#define DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_MASK_BITS  0x000003ff /* [9:0] 0b00000000000000000000001111111111 */
#define DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_MASK       0x000ffc00 /* [19:10] 0b00000000000011111111110000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG4, DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_MASK)
#define READ_DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG4, DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_MASK, DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG4, bits, DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_MASK, DX_L2_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_OFFSET)

/* DX_L2_NPU0.PE1_CFG4.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG4_RESERVED0_LEN        12
#define DX_L2_NPU0_PE1_CFG4_RESERVED0_OFFSET     20
#define DX_L2_NPU0_PE1_CFG4_RESERVED0_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE1_CFG4_RESERVED0_MASK       0xfff00000 /* [31:20] 0b11111111111100000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG4_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG4, DX_L2_NPU0_PE1_CFG4_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CFG4_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG4, DX_L2_NPU0_PE1_CFG4_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG4_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG4_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG4, bits, DX_L2_NPU0_PE1_CFG4_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG4_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_CFG5 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG5(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG5)
#define WRITE_DX_L2_NPU0_PE1_CFG5(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG5, val)
/* DX_L2_NPU0.PE1_CFG5.PE1_FILTER_CHANNEL (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL_LEN        16
#define DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL_OFFSET     0
#define DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG5, DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL_MASK)
#define READ_DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG5, DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL_MASK, DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG5, bits, DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL_MASK, DX_L2_NPU0_PE1_CFG5_PE1_FILTER_CHANNEL_OFFSET)

/* DX_L2_NPU0.PE1_CFG5.PE1_FILTER_NUMBER (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_LEN        16
#define DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_OFFSET     16
#define DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG5, DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_MASK)
#define READ_DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG5, DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_MASK, DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG5, bits, DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_MASK, DX_L2_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_OFFSET)

/* DX_L2_NPU0.PE1_CFG6 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG6(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG6)
#define WRITE_DX_L2_NPU0_PE1_CFG6(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG6, val)
/* DX_L2_NPU0.PE1_CFG6.PE1_FILTER_WIDTH (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_LEN        4
#define DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_OFFSET     0
#define DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_MASK       0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG6, DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_MASK)
#define READ_DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG6, DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_MASK, DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG6, bits, DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_MASK, DX_L2_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_OFFSET)

/* DX_L2_NPU0.PE1_CFG6.PE1_FILTER_HEIGHT (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_LEN        4
#define DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_OFFSET     4
#define DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_MASK       0x000000f0 /* [7:4] 0b00000000000000000000000011110000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG6, DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_MASK)
#define READ_DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG6, DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_MASK, DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG6, bits, DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_MASK, DX_L2_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_OFFSET)

/* DX_L2_NPU0.PE1_CFG6.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG6_RESERVED0_LEN        24
#define DX_L2_NPU0_PE1_CFG6_RESERVED0_OFFSET     8
#define DX_L2_NPU0_PE1_CFG6_RESERVED0_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG6_RESERVED0_MASK       0xffffff00 /* [31:8] 0b11111111111111111111111100000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG6_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG6, DX_L2_NPU0_PE1_CFG6_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CFG6_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG6, DX_L2_NPU0_PE1_CFG6_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG6_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG6_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG6, bits, DX_L2_NPU0_PE1_CFG6_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG6_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_CFG7 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG7(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG7)
#define WRITE_DX_L2_NPU0_PE1_CFG7(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG7, val)
/* DX_L2_NPU0.PE1_CFG7.PE1_PAD_TOP_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_LEN        4
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_OFFSET     0
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_MASK       0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_MASK, DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, bits, DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_MASK, DX_L2_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_CFG7.PE1_PAD_BOTTOM_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_LEN        4
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_OFFSET     4
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_MASK       0x000000f0 /* [7:4] 0b00000000000000000000000011110000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_MASK, DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, bits, DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_MASK, DX_L2_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_CFG7.PE1_PAD_LEFT_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_LEN        4
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_OFFSET     8
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_MASK       0x00000f00 /* [11:8] 0b00000000000000000000111100000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_MASK, DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, bits, DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_MASK, DX_L2_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_CFG7.PE1_PAD_RIGHT_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_LEN        4
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_OFFSET     12
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_MASK       0x0000f000 /* [15:12] 0b00000000000000001111000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_MASK, DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, bits, DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_MASK, DX_L2_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_CFG7.PE1_PAD_VALUE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE_LEN        8
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE_OFFSET     16
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE_MASK, DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, bits, DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE_MASK, DX_L2_NPU0_PE1_CFG7_PE1_PAD_VALUE_OFFSET)

/* DX_L2_NPU0.PE1_CFG7.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG7_RESERVED0_LEN        8
#define DX_L2_NPU0_PE1_CFG7_RESERVED0_OFFSET     24
#define DX_L2_NPU0_PE1_CFG7_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE1_CFG7_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG7_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CFG7_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, DX_L2_NPU0_PE1_CFG7_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG7_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG7_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG7, bits, DX_L2_NPU0_PE1_CFG7_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG7_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_CFG8 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG8(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG8)
#define WRITE_DX_L2_NPU0_PE1_CFG8(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG8, val)
/* DX_L2_NPU0.PE1_CFG8.PE1_STRIDE_X (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X_LEN        4
#define DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X_OFFSET     0
#define DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X_MASK       0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG8, DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X_MASK)
#define READ_DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG8, DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X_MASK, DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG8, bits, DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X_MASK, DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_X_OFFSET)

/* DX_L2_NPU0.PE1_CFG8.PE1_STRIDE_Y (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y_LEN        4
#define DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y_OFFSET     4
#define DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y_MASK       0x000000f0 /* [7:4] 0b00000000000000000000000011110000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG8, DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y_MASK)
#define READ_DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG8, DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y_MASK, DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG8, bits, DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y_MASK, DX_L2_NPU0_PE1_CFG8_PE1_STRIDE_Y_OFFSET)

/* DX_L2_NPU0.PE1_CFG8.PE1_DILATION_X (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X_LEN        4
#define DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X_OFFSET     8
#define DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X_MASK       0x00000f00 /* [11:8] 0b00000000000000000000111100000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG8, DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X_MASK)
#define READ_DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG8, DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X_MASK, DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG8, bits, DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X_MASK, DX_L2_NPU0_PE1_CFG8_PE1_DILATION_X_OFFSET)

/* DX_L2_NPU0.PE1_CFG8.PE1_DILATION_Y (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y_LEN        4
#define DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y_OFFSET     12
#define DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y_MASK       0x0000f000 /* [15:12] 0b00000000000000001111000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG8, DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y_MASK)
#define READ_DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG8, DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y_MASK, DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG8, bits, DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y_MASK, DX_L2_NPU0_PE1_CFG8_PE1_DILATION_Y_OFFSET)

/* DX_L2_NPU0.PE1_CFG8.PE1_CHANNEL_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_LEN        16
#define DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_OFFSET     16
#define DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG8, DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG8, DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_MASK, DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG8, bits, DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_MASK, DX_L2_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_OFFSET)

/* DX_L2_NPU0.PE1_CFG9 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG9(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG9)
#define WRITE_DX_L2_NPU0_PE1_CFG9(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG9, val)
/* DX_L2_NPU0.PE1_CFG9.PE1_READ_FEATURE_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG9, DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG9, DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG9, bits, DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG9_PE1_READ_FEATURE_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_CFG10 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG10(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG10)
#define WRITE_DX_L2_NPU0_PE1_CFG10(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG10, val)
/* DX_L2_NPU0.PE1_CFG10.PE1_WRITE_FEATURE_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG10, DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG10, DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG10, bits, DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG10_PE1_WRITE_FEATURE_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_CFG11 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG11(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG11)
#define WRITE_DX_L2_NPU0_PE1_CFG11(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG11, val)
/* DX_L2_NPU0.PE1_CFG11.PE1_READ_WEIGHT_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG11, DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG11, DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG11, bits, DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG11_PE1_READ_WEIGHT_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_CFG12 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG12(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG12)
#define WRITE_DX_L2_NPU0_PE1_CFG12(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG12, val)
/* DX_L2_NPU0.PE1_CFG12.PE1_BN_COEFF_READ_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG12, DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG12, DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG12, bits, DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG12_PE1_BN_COEFF_READ_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_CFG13 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG13(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG13)
#define WRITE_DX_L2_NPU0_PE1_CFG13(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG13, val)
/* DX_L2_NPU0.PE1_CFG13.PE1_BIAS_READ_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG13, DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG13, DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG13, bits, DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG13_PE1_BIAS_READ_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_CFG14 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG14(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG14)
#define WRITE_DX_L2_NPU0_PE1_CFG14(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG14, val)
/* DX_L2_NPU0.PE1_CFG14.PE1_MUL_READ_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG14, DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG14, DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG14, bits, DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG14_PE1_MUL_READ_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_CFG15 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG15(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG15)
#define WRITE_DX_L2_NPU0_PE1_CFG15(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG15, val)
/* DX_L2_NPU0.PE1_CFG15.PE1_ELEM_READ_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG15, DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG15, DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG15, bits, DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR_MASK, DX_L2_NPU0_PE1_CFG15_PE1_ELEM_READ_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_CFG16 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG16(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG16)
#define WRITE_DX_L2_NPU0_PE1_CFG16(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG16, val)
/* DX_L2_NPU0.PE1_CFG16.PE1_ELEM_CHANNEL_OFFSET (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET_LEN        16
#define DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET_OFFSET     0
#define DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG16, DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET_MASK)
#define READ_DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG16, DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET_MASK, DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG16, bits, DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET_MASK, DX_L2_NPU0_PE1_CFG16_PE1_ELEM_CHANNEL_OFFSET_OFFSET)

/* DX_L2_NPU0.PE1_CFG16.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG16_RESERVED0_LEN        16
#define DX_L2_NPU0_PE1_CFG16_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE1_CFG16_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG16_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG16_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG16, DX_L2_NPU0_PE1_CFG16_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CFG16_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG16, DX_L2_NPU0_PE1_CFG16_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG16_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG16_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG16, bits, DX_L2_NPU0_PE1_CFG16_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG16_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_CFG17 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG17(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG17)
#define WRITE_DX_L2_NPU0_PE1_CFG17(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG17, val)
/* DX_L2_NPU0.PE1_CFG17.PE1_WF_ADDR_MODE1_LINE_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE_LEN        16
#define DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE_OFFSET     0
#define DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG17, DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG17, DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE_MASK, DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG17, bits, DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE_MASK, DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_MODE1_LINE_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_CFG17.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG17_RESERVED0_LEN        14
#define DX_L2_NPU0_PE1_CFG17_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE1_CFG17_RESERVED0_MASK_BITS  0x00003fff /* [13:0] 0b00000000000000000011111111111111 */
#define DX_L2_NPU0_PE1_CFG17_RESERVED0_MASK       0x3fff0000 /* [29:16] 0b00111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG17_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG17, DX_L2_NPU0_PE1_CFG17_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CFG17_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG17, DX_L2_NPU0_PE1_CFG17_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG17_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG17_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG17, bits, DX_L2_NPU0_PE1_CFG17_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG17_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_CFG17.PE1_WF_ADDR_GEN_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE_LEN        2
#define DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE_OFFSET     30
#define DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE_MASK       0xc0000000 /* [31:30] 0b11000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG17, DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG17, DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE_MASK, DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG17, bits, DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE_MASK, DX_L2_NPU0_PE1_CFG17_PE1_WF_ADDR_GEN_MODE_OFFSET)

/* DX_L2_NPU0.PE1_CFG18 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG18(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG18)
#define WRITE_DX_L2_NPU0_PE1_CFG18(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG18, val)
/* DX_L2_NPU0.PE1_CFG18.PE1_WF_ADDR_MODE1_LINE_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE_LEN        32
#define DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG18, DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG18, DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE_MASK, DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG18, bits, DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE_MASK, DX_L2_NPU0_PE1_CFG18_PE1_WF_ADDR_MODE1_LINE_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_CFG19 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG19(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG19)
#define WRITE_DX_L2_NPU0_PE1_CFG19(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG19, val)
/* DX_L2_NPU0.PE1_CFG19.PE1_WF_ADDR_LINE_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE_LEN        16
#define DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG19, DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG19, DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE_MASK, DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG19, bits, DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE_MASK, DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_LINE_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_CFG19.PE1_WF_ADDR_SURFACE_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE_LEN        16
#define DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE_OFFSET     16
#define DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG19, DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG19, DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE_MASK, DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG19, bits, DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE_MASK, DX_L2_NPU0_PE1_CFG19_PE1_WF_ADDR_SURFACE_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_CFG20 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG20(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG20)
#define WRITE_DX_L2_NPU0_PE1_CFG20(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG20, val)
/* DX_L2_NPU0.PE1_CFG20.PE1_WF_WRITE_CNT (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT_LEN        16
#define DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT_OFFSET     0
#define DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG20, DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT_MASK)
#define READ_DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG20, DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT_MASK, DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG20, bits, DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT_MASK, DX_L2_NPU0_PE1_CFG20_PE1_WF_WRITE_CNT_OFFSET)

/* DX_L2_NPU0.PE1_CFG20.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG20_RESERVED0_LEN        8
#define DX_L2_NPU0_PE1_CFG20_RESERVED0_OFFSET     16
#define DX_L2_NPU0_PE1_CFG20_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE1_CFG20_RESERVED0_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG20_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG20, DX_L2_NPU0_PE1_CFG20_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CFG20_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG20, DX_L2_NPU0_PE1_CFG20_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG20_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG20_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG20, bits, DX_L2_NPU0_PE1_CFG20_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG20_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_CFG20.PE1_WF_RF2RF_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME_LEN        8
#define DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME_OFFSET     24
#define DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG20, DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME_MASK)
#define READ_DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG20, DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME_MASK, DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG20, bits, DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME_MASK, DX_L2_NPU0_PE1_CFG20_PE1_WF_RF2RF_TIME_OFFSET)

/* DX_L2_NPU0.PE1_CFG21 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG21(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG21)
#define WRITE_DX_L2_NPU0_PE1_CFG21(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG21, val)
/* DX_L2_NPU0.PE1_CFG21.PE1_WF_RF2RF_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE_LEN        16
#define DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE_OFFSET     0
#define DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG21, DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG21, DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE_MASK, DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG21, bits, DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE_MASK, DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_CFG21.PE1_WF_RF2RF_ADDR_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE_LEN        16
#define DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE_OFFSET     16
#define DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG21, DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG21, DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE_MASK, DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG21, bits, DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE_MASK, DX_L2_NPU0_PE1_CFG21_PE1_WF_RF2RF_ADDR_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_CFG22 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG22(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG22)
#define WRITE_DX_L2_NPU0_PE1_CFG22(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG22, val)
/* DX_L2_NPU0.PE1_CFG22.PE1_WF_RF2RF_SRC_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR_LEN        32
#define DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG22, DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG22, DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR_MASK, DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG22, bits, DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR_MASK, DX_L2_NPU0_PE1_CFG22_PE1_WF_RF2RF_SRC_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_CFG23 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG23(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG23)
#define WRITE_DX_L2_NPU0_PE1_CFG23(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG23, val)
/* DX_L2_NPU0.PE1_CFG23.PE1_WF_RF2RF_DST_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR_LEN        32
#define DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG23, DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG23, DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR_MASK, DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG23, bits, DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR_MASK, DX_L2_NPU0_PE1_CFG23_PE1_WF_RF2RF_DST_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_CFG24 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG24(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG24)
#define WRITE_DX_L2_NPU0_PE1_CFG24(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG24, val)
/* DX_L2_NPU0.PE1_CFG24.PE1_SFU_OUT_DATA_VALID_NUM (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM_LEN        3
#define DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM_OFFSET     0
#define DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM_MASK       0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG24, DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM_MASK)
#define READ_DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG24, DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM_MASK, DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG24, bits, DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM_MASK, DX_L2_NPU0_PE1_CFG24_PE1_SFU_OUT_DATA_VALID_NUM_OFFSET)

/* DX_L2_NPU0.PE1_CFG24.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG24_RESERVED1_LEN        5
#define DX_L2_NPU0_PE1_CFG24_RESERVED1_OFFSET     3
#define DX_L2_NPU0_PE1_CFG24_RESERVED1_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_L2_NPU0_PE1_CFG24_RESERVED1_MASK       0x000000f8 /* [7:3] 0b00000000000000000000000011111000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG24_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG24, DX_L2_NPU0_PE1_CFG24_RESERVED1_MASK)
#define READ_DX_L2_NPU0_PE1_CFG24_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG24, DX_L2_NPU0_PE1_CFG24_RESERVED1_MASK, DX_L2_NPU0_PE1_CFG24_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG24_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG24, bits, DX_L2_NPU0_PE1_CFG24_RESERVED1_MASK, DX_L2_NPU0_PE1_CFG24_RESERVED1_OFFSET)

/* DX_L2_NPU0.PE1_CFG24.PE1_ST_ADDR_GEN_SHIFT_LEN (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN_LEN        5
#define DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN_OFFSET     8
#define DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN_MASK       0x00001f00 /* [12:8] 0b00000000000000000001111100000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG24, DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN_MASK)
#define READ_DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG24, DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN_MASK, DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG24, bits, DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN_MASK, DX_L2_NPU0_PE1_CFG24_PE1_ST_ADDR_GEN_SHIFT_LEN_OFFSET)

/* DX_L2_NPU0.PE1_CFG24.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG24_RESERVED0_LEN        19
#define DX_L2_NPU0_PE1_CFG24_RESERVED0_OFFSET     13
#define DX_L2_NPU0_PE1_CFG24_RESERVED0_MASK_BITS  0x0007ffff /* [18:0] 0b00000000000001111111111111111111 */
#define DX_L2_NPU0_PE1_CFG24_RESERVED0_MASK       0xffffe000 /* [31:13] 0b11111111111111111110000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG24_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG24, DX_L2_NPU0_PE1_CFG24_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CFG24_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG24, DX_L2_NPU0_PE1_CFG24_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG24_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG24_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG24, bits, DX_L2_NPU0_PE1_CFG24_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG24_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_CFG25 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_CFG25(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_CFG25)
#define WRITE_DX_L2_NPU0_PE1_CFG25(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_CFG25, val)
/* DX_L2_NPU0.PE1_CFG25.PE1_ST_ADDR_GEN_MUL (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL_LEN        31
#define DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL_OFFSET     0
#define DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL_MASK       0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG25, DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL_MASK)
#define READ_DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG25, DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL_MASK, DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG25, bits, DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL_MASK, DX_L2_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_MUL_OFFSET)

/* DX_L2_NPU0.PE1_CFG25.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_CFG25_RESERVED0_LEN        1
#define DX_L2_NPU0_PE1_CFG25_RESERVED0_OFFSET     31
#define DX_L2_NPU0_PE1_CFG25_RESERVED0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_CFG25_RESERVED0_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_CFG25_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_CFG25, DX_L2_NPU0_PE1_CFG25_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_CFG25_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_CFG25, DX_L2_NPU0_PE1_CFG25_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG25_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_CFG25_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_CFG25, bits, DX_L2_NPU0_PE1_CFG25_RESERVED0_MASK, DX_L2_NPU0_PE1_CFG25_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_VMEM_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_VMEM_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0)
#define WRITE_DX_L2_NPU0_PE1_VMEM_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, val)
/* DX_L2_NPU0.PE1_VMEM_CFG0.PE1_START_NO_IN_BUFF (Category: NPU0) */
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_LEN        6
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_OFFSET     0
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_MASK)
#define READ_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, bits, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_OFFSET)

/* DX_L2_NPU0.PE1_VMEM_CFG0.RESERVED3 (Category: NPU0) */
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3_LEN        2
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3_OFFSET     6
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3_MASK)
#define READ_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, bits, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED3_OFFSET)

/* DX_L2_NPU0.PE1_VMEM_CFG0.PE1_START_NO_WEIGHT_BUFF (Category: NPU0) */
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_LEN        6
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_OFFSET     8
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_MASK       0x00003f00 /* [13:8] 0b00000000000000000011111100000000 */
#define READ_MASK_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_MASK)
#define READ_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, bits, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_OFFSET)

/* DX_L2_NPU0.PE1_VMEM_CFG0.RESERVED2 (Category: NPU0) */
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2_LEN        2
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2_OFFSET     14
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2_MASK       0x0000c000 /* [15:14] 0b00000000000000001100000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2_MASK)
#define READ_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, bits, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED2_OFFSET)

/* DX_L2_NPU0.PE1_VMEM_CFG0.PE1_START_NO_OUT_BUFF (Category: NPU0) */
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_LEN        6
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_OFFSET     16
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_MASK       0x003f0000 /* [21:16] 0b00000000001111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_MASK)
#define READ_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, bits, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_OFFSET)

/* DX_L2_NPU0.PE1_VMEM_CFG0.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1_LEN        2
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1_OFFSET     22
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1_MASK       0x00c00000 /* [23:22] 0b00000000110000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1_MASK)
#define READ_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, bits, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED1_OFFSET)

/* DX_L2_NPU0.PE1_VMEM_CFG0.PE1_START_NO_ELEM_BUFF (Category: NPU0) */
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_LEN        6
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_OFFSET     24
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_MASK       0x3f000000 /* [29:24] 0b00111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_MASK)
#define READ_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, bits, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_OFFSET)

/* DX_L2_NPU0.PE1_VMEM_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0_LEN        2
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0_OFFSET     30
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0_MASK       0xc0000000 /* [31:30] 0b11000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_VMEM_CFG0, bits, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_VMEM_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_SMEM_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_SMEM_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0)
#define WRITE_DX_L2_NPU0_PE1_SMEM_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, val)
/* DX_L2_NPU0.PE1_SMEM_CFG0.PE1_BN_BUFF_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL_LEN        1
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL_OFFSET     0
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL_MASK)
#define READ_DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL_MASK, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, bits, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL_MASK, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_BN_BUFF_SEL_OFFSET)

/* DX_L2_NPU0.PE1_SMEM_CFG0.PE1_B_BUFF_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL_LEN        1
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL_OFFSET     1
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL_MASK       0x00000002 /* [1:1] 0b00000000000000000000000000000010 */
#define READ_MASK_DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL_MASK)
#define READ_DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL_MASK, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, bits, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL_MASK, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_B_BUFF_SEL_OFFSET)

/* DX_L2_NPU0.PE1_SMEM_CFG0.PE1_MUL_BUFF_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL_LEN        1
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL_OFFSET     2
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL_MASK       0x00000004 /* [2:2] 0b00000000000000000000000000000100 */
#define READ_MASK_DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL_MASK)
#define READ_DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL_MASK, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, bits, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL_MASK, DX_L2_NPU0_PE1_SMEM_CFG0_PE1_MUL_BUFF_SEL_OFFSET)

/* DX_L2_NPU0.PE1_SMEM_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0_LEN        29
#define DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0_OFFSET     3
#define DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0_MASK_BITS  0x1fffffff /* [28:0] 0b00011111111111111111111111111111 */
#define DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0_MASK       0xfffffff8 /* [31:3] 0b11111111111111111111111111111000 */
#define READ_MASK_DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_SMEM_CFG0, bits, DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_SMEM_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_CTRL (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_CTRL(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_CTRL)
#define WRITE_DX_L2_NPU0_PE0_DMA_CTRL(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_CTRL, val)
/* DX_L2_NPU0.PE0_DMA_CTRL.PE0_START_OP_EN (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN_LEN        1
#define DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_CTRL, DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_CTRL, DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN_MASK, DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_CTRL, bits, DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN_MASK, DX_L2_NPU0_PE0_DMA_CTRL_PE0_START_OP_EN_OFFSET)

/* DX_L2_NPU0.PE0_DMA_CTRL.PE0_DMA_CTRL_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE_LEN        2
#define DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE_OFFSET     1
#define DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE_MASK       0x00000006 /* [2:1] 0b00000000000000000000000000000110 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_CTRL, DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_CTRL, DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE_MASK, DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_CTRL, bits, DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE_MASK, DX_L2_NPU0_PE0_DMA_CTRL_PE0_DMA_CTRL_MODE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_CTRL.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0_LEN        29
#define DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0_OFFSET     3
#define DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0_MASK_BITS  0x1fffffff /* [28:0] 0b00011111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0_MASK       0xfffffff8 /* [31:3] 0b11111111111111111111111111111000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_CTRL, DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_CTRL, DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_CTRL, bits, DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_CTRL_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_BN_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, val)
/* DX_L2_NPU0.PE0_DMA_BN_CFG0.PE0_RD_BN_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME_LEN        6
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, bits, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_TIME_OFFSET)

/* DX_L2_NPU0.PE0_DMA_BN_CFG0.PE0_RD_BN_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE_LEN        2
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE_OFFSET     6
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, bits, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_MODE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_BN_CFG0.PE0_RD_BN_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL_LEN        1
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL_OFFSET     8
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, bits, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_SEL_OFFSET)

/* DX_L2_NPU0.PE0_DMA_BN_CFG0.PE0_RD_BN_EN (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN_LEN        1
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN_OFFSET     9
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN_MASK       0x00000200 /* [9:9] 0b00000000000000000000001000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, bits, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG0_PE0_RD_BN_EN_OFFSET)

/* DX_L2_NPU0.PE0_DMA_BN_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0_LEN        22
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0_OFFSET     10
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0_MASK_BITS  0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0_MASK       0xfffffc00 /* [31:10] 0b11111111111111111111110000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG0, bits, DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_BN_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG1)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG1, val)
/* DX_L2_NPU0.PE0_DMA_BN_CFG1.PE0_RD_BN_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE_LEN        16
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG1, DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG1, DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG1, bits, DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_BN_CFG1.PE0_RD_BN_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE_LEN        12
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE_OFFSET     16
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG1, DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG1, DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG1, bits, DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG1_PE0_RD_BN_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_BN_CFG1.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0_LEN        4
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0_OFFSET     28
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0_MASK       0xf0000000 /* [31:28] 0b11110000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG1, DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG1, DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG1, bits, DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG1_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_BN_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG2)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG2, val)
/* DX_L2_NPU0.PE0_DMA_BN_CFG2.PE0_RD_BN_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG2, DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG2, DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG2, bits, DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG2_PE0_RD_BN_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_BN_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG3)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG3, val)
/* DX_L2_NPU0.PE0_DMA_BN_CFG3.PE0_WT_BN_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG3, DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG3, DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_BN_CFG3, bits, DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_BN_CFG3_PE0_WT_BN_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_B_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, val)
/* DX_L2_NPU0.PE0_DMA_B_CFG0.PE0_RD_B_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME_LEN        6
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME_MASK, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, bits, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME_MASK, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_TIME_OFFSET)

/* DX_L2_NPU0.PE0_DMA_B_CFG0.PE0_RD_B_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE_LEN        2
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE_OFFSET     6
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE_MASK, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, bits, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE_MASK, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_MODE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_B_CFG0.PE0_RD_B_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL_LEN        1
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL_OFFSET     8
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL_MASK, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, bits, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL_MASK, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_SEL_OFFSET)

/* DX_L2_NPU0.PE0_DMA_B_CFG0.PE0_RD_B_EN (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN_LEN        1
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN_OFFSET     9
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN_MASK       0x00000200 /* [9:9] 0b00000000000000000000001000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN_MASK, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, bits, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN_MASK, DX_L2_NPU0_PE0_DMA_B_CFG0_PE0_RD_B_EN_OFFSET)

/* DX_L2_NPU0.PE0_DMA_B_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0_LEN        22
#define DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0_OFFSET     10
#define DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0_MASK_BITS  0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0_MASK       0xfffffc00 /* [31:10] 0b11111111111111111111110000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG0, bits, DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_B_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_B_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG1)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG1, val)
/* DX_L2_NPU0.PE0_DMA_B_CFG1.PE0_RD_B_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE_LEN        16
#define DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG1, DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG1, DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG1, bits, DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_B_CFG1.PE0_RD_B_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE_LEN        12
#define DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE_OFFSET     16
#define DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG1, DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG1, DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE_MASK, DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG1, bits, DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE_MASK, DX_L2_NPU0_PE0_DMA_B_CFG1_PE0_RD_B_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_B_CFG1.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0_LEN        4
#define DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0_OFFSET     28
#define DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0_MASK       0xf0000000 /* [31:28] 0b11110000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG1, DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG1, DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG1, bits, DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_B_CFG1_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_B_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG2)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG2, val)
/* DX_L2_NPU0.PE0_DMA_B_CFG2.PE0_RD_B_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG2, DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG2, DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG2, bits, DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_B_CFG2_PE0_RD_B_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_B_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG3)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG3, val)
/* DX_L2_NPU0.PE0_DMA_B_CFG3.PE0_WT_B_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG3, DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG3, DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_B_CFG3, bits, DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_B_CFG3_PE0_WT_B_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_MUL_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, val)
/* DX_L2_NPU0.PE0_DMA_MUL_CFG0.PE0_RD_MUL_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME_LEN        6
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, bits, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_TIME_OFFSET)

/* DX_L2_NPU0.PE0_DMA_MUL_CFG0.PE0_RD_MUL_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE_LEN        2
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE_OFFSET     6
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, bits, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_MODE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_MUL_CFG0.PE0_RD_MUL_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL_LEN        1
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL_OFFSET     8
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, bits, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_SEL_OFFSET)

/* DX_L2_NPU0.PE0_DMA_MUL_CFG0.PE0_RD_MUL_EN (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN_LEN        1
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN_OFFSET     9
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN_MASK       0x00000200 /* [9:9] 0b00000000000000000000001000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, bits, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG0_PE0_RD_MUL_EN_OFFSET)

/* DX_L2_NPU0.PE0_DMA_MUL_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0_LEN        22
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0_OFFSET     10
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0_MASK_BITS  0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0_MASK       0xfffffc00 /* [31:10] 0b11111111111111111111110000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG0, bits, DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_MUL_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1, val)
/* DX_L2_NPU0.PE0_DMA_MUL_CFG1.PE0_RD_MUL_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE_LEN        16
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1, DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1, DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1, bits, DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_MUL_CFG1.PE0_RD_MUL_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE_LEN        12
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE_OFFSET     16
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1, DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1, DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1, bits, DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG1_PE0_RD_MUL_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_MUL_CFG1.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0_LEN        4
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0_OFFSET     28
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0_MASK       0xf0000000 /* [31:28] 0b11110000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1, DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1, DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG1, bits, DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG1_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_MUL_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG2)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG2, val)
/* DX_L2_NPU0.PE0_DMA_MUL_CFG2.PE0_RD_MUL_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG2, DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG2, DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG2, bits, DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG2_PE0_RD_MUL_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_MUL_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG3)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG3, val)
/* DX_L2_NPU0.PE0_DMA_MUL_CFG3.PE0_WT_MUL_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG3, DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG3, DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_MUL_CFG3, bits, DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_MUL_CFG3_PE0_WT_MUL_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_ELEM_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, val)
/* DX_L2_NPU0.PE0_DMA_ELEM_CFG0.PE0_RD_ELEM_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE_LEN        16
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, bits, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_ELEM_CFG0.PE0_RD_ELEM_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME_LEN        6
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME_OFFSET     16
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME_MASK       0x003f0000 /* [21:16] 0b00000000001111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, bits, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_TIME_OFFSET)

/* DX_L2_NPU0.PE0_DMA_ELEM_CFG0.PE0_RD_ELEM_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE_LEN        2
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE_OFFSET     22
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE_MASK       0x00c00000 /* [23:22] 0b00000000110000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, bits, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_MODE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_ELEM_CFG0.PE0_RD_ELEM_EN (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN_LEN        1
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN_OFFSET     24
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, bits, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_PE0_RD_ELEM_EN_OFFSET)

/* DX_L2_NPU0.PE0_DMA_ELEM_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_LEN        7
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_OFFSET     25
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_MASK       0xfe000000 /* [31:25] 0b11111110000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG0, bits, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_ELEM_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG1)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG1, val)
/* DX_L2_NPU0.PE0_DMA_ELEM_CFG1.PE0_RD_ELEM_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE_LEN        32
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG1, DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG1, DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG1, bits, DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG1_PE0_RD_ELEM_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_ELEM_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG2)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG2, val)
/* DX_L2_NPU0.PE0_DMA_ELEM_CFG2.PE0_RD_ELEM_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG2, DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG2, DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG2, bits, DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG2_PE0_RD_ELEM_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_ELEM_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG3)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG3, val)
/* DX_L2_NPU0.PE0_DMA_ELEM_CFG3.PE0_WT_ELEM_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG3, DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG3, DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_ELEM_CFG3, bits, DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_ELEM_CFG3_PE0_WT_ELEM_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_W_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, val)
/* DX_L2_NPU0.PE0_DMA_W_CFG0.PE0_RD_W_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME_LEN        12
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME_MASK       0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME_MASK, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, bits, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME_MASK, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_TIME_OFFSET)

/* DX_L2_NPU0.PE0_DMA_W_CFG0.PE0_RD_W_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE_LEN        2
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE_OFFSET     12
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE_MASK       0x00003000 /* [13:12] 0b00000000000000000011000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE_MASK, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, bits, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE_MASK, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_MODE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_W_CFG0.PE0_RD_W_EN (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN_LEN        1
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN_OFFSET     14
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN_MASK       0x00004000 /* [14:14] 0b00000000000000000100000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN_MASK, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, bits, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN_MASK, DX_L2_NPU0_PE0_DMA_W_CFG0_PE0_RD_W_EN_OFFSET)

/* DX_L2_NPU0.PE0_DMA_W_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0_LEN        17
#define DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0_OFFSET     15
#define DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG0, bits, DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_W_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_W_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG1)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG1, val)
/* DX_L2_NPU0.PE0_DMA_W_CFG1.PE0_RD_W_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE_LEN        16
#define DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG1, DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG1, DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG1, bits, DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_W_CFG1.PE0_RD_W_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE_LEN        16
#define DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE_OFFSET     16
#define DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG1, DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG1, DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE_MASK, DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG1, bits, DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE_MASK, DX_L2_NPU0_PE0_DMA_W_CFG1_PE0_RD_W_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_W_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG2)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG2, val)
/* DX_L2_NPU0.PE0_DMA_W_CFG2.PE0_RD_W_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG2, DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG2, DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG2, bits, DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_W_CFG2_PE0_RD_W_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_W_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG3)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG3, val)
/* DX_L2_NPU0.PE0_DMA_W_CFG3.PE0_WT_W_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG3, DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG3, DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_W_CFG3, bits, DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_W_CFG3_PE0_WT_W_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_INF_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, val)
/* DX_L2_NPU0.PE0_DMA_INF_CFG0.PE0_RD_INF_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE_LEN        16
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, bits, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_INF_CFG0.PE0_RD_INF_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME_LEN        12
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME_OFFSET     16
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, bits, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_TIME_OFFSET)

/* DX_L2_NPU0.PE0_DMA_INF_CFG0.PE0_RD_INF_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE_LEN        2
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE_OFFSET     28
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE_MASK       0x30000000 /* [29:28] 0b00110000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, bits, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_MODE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_INF_CFG0.PE0_RD_INF_EN (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN_LEN        1
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN_OFFSET     30
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, bits, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG0_PE0_RD_INF_EN_OFFSET)

/* DX_L2_NPU0.PE0_DMA_INF_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0_LEN        1
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0_OFFSET     31
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG0, bits, DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_INF_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG1)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG1, val)
/* DX_L2_NPU0.PE0_DMA_INF_CFG1.PE0_RD_INF_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE_LEN        32
#define DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG1, DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG1, DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG1, bits, DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG1_PE0_RD_INF_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_INF_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG2)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG2, val)
/* DX_L2_NPU0.PE0_DMA_INF_CFG2.PE0_RD_INF_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG2, DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG2, DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG2, bits, DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG2_PE0_RD_INF_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_INF_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG3)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG3, val)
/* DX_L2_NPU0.PE0_DMA_INF_CFG3.PE0_WT_INF_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE_LEN        32
#define DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG3, DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG3, DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG3, bits, DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG3_PE0_WT_INF_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_INF_CFG4 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG4(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG4)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG4(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG4, val)
/* DX_L2_NPU0.PE0_DMA_INF_CFG4.PE0_WT_INF_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG4, DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG4, DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_INF_CFG4, bits, DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_INF_CFG4_PE0_WT_INF_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_OUTF_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, val)
/* DX_L2_NPU0.PE0_DMA_OUTF_CFG0.PE0_WT_OUTF_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE_LEN        16
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, bits, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_SIZE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_OUTF_CFG0.PE0_WT_OUTF_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME_LEN        12
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME_OFFSET     16
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, bits, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_TIME_OFFSET)

/* DX_L2_NPU0.PE0_DMA_OUTF_CFG0.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_LEN        2
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_OFFSET     28
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_MASK       0x30000000 /* [29:28] 0b00110000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, bits, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_OFFSET)

/* DX_L2_NPU0.PE0_DMA_OUTF_CFG0.PE0_WT_OUTF_EN (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN_LEN        1
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN_OFFSET     30
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, bits, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_PE0_WT_OUTF_EN_OFFSET)

/* DX_L2_NPU0.PE0_DMA_OUTF_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_LEN        1
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_OFFSET     31
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG0, bits, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE0_DMA_OUTF_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG1)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG1, val)
/* DX_L2_NPU0.PE0_DMA_OUTF_CFG1.PE0_WT_OUTF_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE_LEN        32
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG1, DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG1, DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG1, bits, DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG1_PE0_WT_OUTF_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_OUTF_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG2)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG2, val)
/* DX_L2_NPU0.PE0_DMA_OUTF_CFG2.PE0_WT_OUTF_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG2, DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG2, DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG2, bits, DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG2_PE0_WT_OUTF_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE0_DMA_OUTF_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG3)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG3, val)
/* DX_L2_NPU0.PE0_DMA_OUTF_CFG3.PE0_RD_OUTF_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE_LEN        32
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG3, DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG3, DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG3, bits, DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG3_PE0_RD_OUTF_STRIDE_OFFSET)

/* DX_L2_NPU0.PE0_DMA_OUTF_CFG4 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG4(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG4)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG4(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG4, val)
/* DX_L2_NPU0.PE0_DMA_OUTF_CFG4.PE0_RD_OUTF_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG4, DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG4, DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE0_DMA_OUTF_CFG4, bits, DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR_MASK, DX_L2_NPU0_PE0_DMA_OUTF_CFG4_PE0_RD_OUTF_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.RSVD5 (Category: NPU0) */
#define READ_DX_L2_NPU0_RSVD5(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_RSVD5)
#define WRITE_DX_L2_NPU0_RSVD5(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_RSVD5, val)
/* DX_L2_NPU0.RSVD5.RESERVED3 (Category: NPU0) */
#define DX_L2_NPU0_RSVD5_RESERVED3_LEN        8
#define DX_L2_NPU0_RSVD5_RESERVED3_OFFSET     0
#define DX_L2_NPU0_RSVD5_RESERVED3_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD5_RESERVED3_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_NPU0_RSVD5_RESERVED3(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD5, DX_L2_NPU0_RSVD5_RESERVED3_MASK)
#define READ_DX_L2_NPU0_RSVD5_RESERVED3(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD5, DX_L2_NPU0_RSVD5_RESERVED3_MASK, DX_L2_NPU0_RSVD5_RESERVED3_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD5_RESERVED3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD5, bits, DX_L2_NPU0_RSVD5_RESERVED3_MASK, DX_L2_NPU0_RSVD5_RESERVED3_OFFSET)

/* DX_L2_NPU0.RSVD5.RESERVED2 (Category: NPU0) */
#define DX_L2_NPU0_RSVD5_RESERVED2_LEN        8
#define DX_L2_NPU0_RSVD5_RESERVED2_OFFSET     8
#define DX_L2_NPU0_RSVD5_RESERVED2_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD5_RESERVED2_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_L2_NPU0_RSVD5_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD5, DX_L2_NPU0_RSVD5_RESERVED2_MASK)
#define READ_DX_L2_NPU0_RSVD5_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD5, DX_L2_NPU0_RSVD5_RESERVED2_MASK, DX_L2_NPU0_RSVD5_RESERVED2_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD5_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD5, bits, DX_L2_NPU0_RSVD5_RESERVED2_MASK, DX_L2_NPU0_RSVD5_RESERVED2_OFFSET)

/* DX_L2_NPU0.RSVD5.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_RSVD5_RESERVED1_LEN        8
#define DX_L2_NPU0_RSVD5_RESERVED1_OFFSET     16
#define DX_L2_NPU0_RSVD5_RESERVED1_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD5_RESERVED1_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD5_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD5, DX_L2_NPU0_RSVD5_RESERVED1_MASK)
#define READ_DX_L2_NPU0_RSVD5_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD5, DX_L2_NPU0_RSVD5_RESERVED1_MASK, DX_L2_NPU0_RSVD5_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD5_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD5, bits, DX_L2_NPU0_RSVD5_RESERVED1_MASK, DX_L2_NPU0_RSVD5_RESERVED1_OFFSET)

/* DX_L2_NPU0.RSVD5.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_RSVD5_RESERVED0_LEN        8
#define DX_L2_NPU0_RSVD5_RESERVED0_OFFSET     24
#define DX_L2_NPU0_RSVD5_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD5_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD5_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD5, DX_L2_NPU0_RSVD5_RESERVED0_MASK)
#define READ_DX_L2_NPU0_RSVD5_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD5, DX_L2_NPU0_RSVD5_RESERVED0_MASK, DX_L2_NPU0_RSVD5_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD5_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD5, bits, DX_L2_NPU0_RSVD5_RESERVED0_MASK, DX_L2_NPU0_RSVD5_RESERVED0_OFFSET)

/* DX_L2_NPU0.RSVD6 (Category: NPU0) */
#define READ_DX_L2_NPU0_RSVD6(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_RSVD6)
#define WRITE_DX_L2_NPU0_RSVD6(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_RSVD6, val)
/* DX_L2_NPU0.RSVD6.RESERVED3 (Category: NPU0) */
#define DX_L2_NPU0_RSVD6_RESERVED3_LEN        8
#define DX_L2_NPU0_RSVD6_RESERVED3_OFFSET     0
#define DX_L2_NPU0_RSVD6_RESERVED3_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD6_RESERVED3_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_NPU0_RSVD6_RESERVED3(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD6, DX_L2_NPU0_RSVD6_RESERVED3_MASK)
#define READ_DX_L2_NPU0_RSVD6_RESERVED3(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD6, DX_L2_NPU0_RSVD6_RESERVED3_MASK, DX_L2_NPU0_RSVD6_RESERVED3_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD6_RESERVED3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD6, bits, DX_L2_NPU0_RSVD6_RESERVED3_MASK, DX_L2_NPU0_RSVD6_RESERVED3_OFFSET)

/* DX_L2_NPU0.RSVD6.RESERVED2 (Category: NPU0) */
#define DX_L2_NPU0_RSVD6_RESERVED2_LEN        8
#define DX_L2_NPU0_RSVD6_RESERVED2_OFFSET     8
#define DX_L2_NPU0_RSVD6_RESERVED2_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD6_RESERVED2_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_L2_NPU0_RSVD6_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD6, DX_L2_NPU0_RSVD6_RESERVED2_MASK)
#define READ_DX_L2_NPU0_RSVD6_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD6, DX_L2_NPU0_RSVD6_RESERVED2_MASK, DX_L2_NPU0_RSVD6_RESERVED2_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD6_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD6, bits, DX_L2_NPU0_RSVD6_RESERVED2_MASK, DX_L2_NPU0_RSVD6_RESERVED2_OFFSET)

/* DX_L2_NPU0.RSVD6.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_RSVD6_RESERVED1_LEN        8
#define DX_L2_NPU0_RSVD6_RESERVED1_OFFSET     16
#define DX_L2_NPU0_RSVD6_RESERVED1_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD6_RESERVED1_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD6_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD6, DX_L2_NPU0_RSVD6_RESERVED1_MASK)
#define READ_DX_L2_NPU0_RSVD6_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD6, DX_L2_NPU0_RSVD6_RESERVED1_MASK, DX_L2_NPU0_RSVD6_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD6_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD6, bits, DX_L2_NPU0_RSVD6_RESERVED1_MASK, DX_L2_NPU0_RSVD6_RESERVED1_OFFSET)

/* DX_L2_NPU0.RSVD6.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_RSVD6_RESERVED0_LEN        8
#define DX_L2_NPU0_RSVD6_RESERVED0_OFFSET     24
#define DX_L2_NPU0_RSVD6_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD6_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD6_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD6, DX_L2_NPU0_RSVD6_RESERVED0_MASK)
#define READ_DX_L2_NPU0_RSVD6_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD6, DX_L2_NPU0_RSVD6_RESERVED0_MASK, DX_L2_NPU0_RSVD6_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD6_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD6, bits, DX_L2_NPU0_RSVD6_RESERVED0_MASK, DX_L2_NPU0_RSVD6_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_CTRL (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_CTRL(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_CTRL)
#define WRITE_DX_L2_NPU0_PE1_DMA_CTRL(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_CTRL, val)
/* DX_L2_NPU0.PE1_DMA_CTRL.PE1_START_OP_EN (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN_LEN        1
#define DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_CTRL, DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_CTRL, DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN_MASK, DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_CTRL, bits, DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN_MASK, DX_L2_NPU0_PE1_DMA_CTRL_PE1_START_OP_EN_OFFSET)

/* DX_L2_NPU0.PE1_DMA_CTRL.PE1_DMA_CTRL_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE_LEN        2
#define DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE_OFFSET     1
#define DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE_MASK       0x00000006 /* [2:1] 0b00000000000000000000000000000110 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_CTRL, DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_CTRL, DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE_MASK, DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_CTRL, bits, DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE_MASK, DX_L2_NPU0_PE1_DMA_CTRL_PE1_DMA_CTRL_MODE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_CTRL.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0_LEN        29
#define DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0_OFFSET     3
#define DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0_MASK_BITS  0x1fffffff /* [28:0] 0b00011111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0_MASK       0xfffffff8 /* [31:3] 0b11111111111111111111111111111000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_CTRL, DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_CTRL, DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_CTRL, bits, DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_CTRL_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_BN_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, val)
/* DX_L2_NPU0.PE1_DMA_BN_CFG0.PE1_RD_BN_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME_LEN        6
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, bits, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_TIME_OFFSET)

/* DX_L2_NPU0.PE1_DMA_BN_CFG0.PE1_RD_BN_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE_LEN        2
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE_OFFSET     6
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, bits, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_MODE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_BN_CFG0.PE1_RD_BN_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL_LEN        1
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL_OFFSET     8
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, bits, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_SEL_OFFSET)

/* DX_L2_NPU0.PE1_DMA_BN_CFG0.PE1_RD_BN_EN (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN_LEN        1
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN_OFFSET     9
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN_MASK       0x00000200 /* [9:9] 0b00000000000000000000001000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, bits, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG0_PE1_RD_BN_EN_OFFSET)

/* DX_L2_NPU0.PE1_DMA_BN_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0_LEN        22
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0_OFFSET     10
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0_MASK_BITS  0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0_MASK       0xfffffc00 /* [31:10] 0b11111111111111111111110000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG0, bits, DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_BN_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG1)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG1, val)
/* DX_L2_NPU0.PE1_DMA_BN_CFG1.PE1_RD_BN_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE_LEN        16
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG1, DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG1, DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG1, bits, DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_BN_CFG1.PE1_RD_BN_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE_LEN        12
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE_OFFSET     16
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG1, DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG1, DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG1, bits, DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG1_PE1_RD_BN_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_BN_CFG1.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0_LEN        4
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0_OFFSET     28
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0_MASK       0xf0000000 /* [31:28] 0b11110000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG1, DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG1, DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG1, bits, DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG1_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_BN_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG2)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG2, val)
/* DX_L2_NPU0.PE1_DMA_BN_CFG2.PE1_RD_BN_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG2, DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG2, DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG2, bits, DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG2_PE1_RD_BN_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_BN_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG3)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG3, val)
/* DX_L2_NPU0.PE1_DMA_BN_CFG3.PE1_WT_BN_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG3, DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG3, DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_BN_CFG3, bits, DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_BN_CFG3_PE1_WT_BN_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_B_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, val)
/* DX_L2_NPU0.PE1_DMA_B_CFG0.PE1_RD_B_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME_LEN        6
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME_MASK, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, bits, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME_MASK, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_TIME_OFFSET)

/* DX_L2_NPU0.PE1_DMA_B_CFG0.PE1_RD_B_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE_LEN        2
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE_OFFSET     6
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE_MASK, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, bits, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE_MASK, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_MODE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_B_CFG0.PE1_RD_B_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL_LEN        1
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL_OFFSET     8
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL_MASK, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, bits, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL_MASK, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_SEL_OFFSET)

/* DX_L2_NPU0.PE1_DMA_B_CFG0.PE1_RD_B_EN (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN_LEN        1
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN_OFFSET     9
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN_MASK       0x00000200 /* [9:9] 0b00000000000000000000001000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN_MASK, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, bits, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN_MASK, DX_L2_NPU0_PE1_DMA_B_CFG0_PE1_RD_B_EN_OFFSET)

/* DX_L2_NPU0.PE1_DMA_B_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0_LEN        22
#define DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0_OFFSET     10
#define DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0_MASK_BITS  0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0_MASK       0xfffffc00 /* [31:10] 0b11111111111111111111110000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG0, bits, DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_B_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_B_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG1)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG1, val)
/* DX_L2_NPU0.PE1_DMA_B_CFG1.PE1_RD_B_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE_LEN        16
#define DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG1, DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG1, DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG1, bits, DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_B_CFG1.PE1_RD_B_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE_LEN        12
#define DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE_OFFSET     16
#define DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG1, DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG1, DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE_MASK, DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG1, bits, DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE_MASK, DX_L2_NPU0_PE1_DMA_B_CFG1_PE1_RD_B_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_B_CFG1.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0_LEN        4
#define DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0_OFFSET     28
#define DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0_MASK       0xf0000000 /* [31:28] 0b11110000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG1, DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG1, DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG1, bits, DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_B_CFG1_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_B_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG2)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG2, val)
/* DX_L2_NPU0.PE1_DMA_B_CFG2.PE1_RD_B_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG2, DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG2, DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG2, bits, DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_B_CFG2_PE1_RD_B_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_B_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG3)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG3, val)
/* DX_L2_NPU0.PE1_DMA_B_CFG3.PE1_WT_B_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG3, DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG3, DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_B_CFG3, bits, DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_B_CFG3_PE1_WT_B_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_MUL_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, val)
/* DX_L2_NPU0.PE1_DMA_MUL_CFG0.PE1_RD_MUL_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME_LEN        6
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, bits, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_TIME_OFFSET)

/* DX_L2_NPU0.PE1_DMA_MUL_CFG0.PE1_RD_MUL_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE_LEN        2
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE_OFFSET     6
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, bits, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_MODE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_MUL_CFG0.PE1_RD_MUL_SEL (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL_LEN        1
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL_OFFSET     8
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, bits, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_SEL_OFFSET)

/* DX_L2_NPU0.PE1_DMA_MUL_CFG0.PE1_RD_MUL_EN (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN_LEN        1
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN_OFFSET     9
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN_MASK       0x00000200 /* [9:9] 0b00000000000000000000001000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, bits, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG0_PE1_RD_MUL_EN_OFFSET)

/* DX_L2_NPU0.PE1_DMA_MUL_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0_LEN        22
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0_OFFSET     10
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0_MASK_BITS  0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0_MASK       0xfffffc00 /* [31:10] 0b11111111111111111111110000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG0, bits, DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_MUL_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1, val)
/* DX_L2_NPU0.PE1_DMA_MUL_CFG1.PE1_RD_MUL_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE_LEN        16
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1, DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1, DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1, bits, DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_MUL_CFG1.PE1_RD_MUL_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE_LEN        12
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE_OFFSET     16
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1, DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1, DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1, bits, DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG1_PE1_RD_MUL_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_MUL_CFG1.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0_LEN        4
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0_OFFSET     28
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0_MASK       0xf0000000 /* [31:28] 0b11110000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1, DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1, DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG1, bits, DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG1_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_MUL_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG2)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG2, val)
/* DX_L2_NPU0.PE1_DMA_MUL_CFG2.PE1_RD_MUL_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG2, DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG2, DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG2, bits, DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG2_PE1_RD_MUL_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_MUL_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG3)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG3, val)
/* DX_L2_NPU0.PE1_DMA_MUL_CFG3.PE1_WT_MUL_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG3, DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG3, DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_MUL_CFG3, bits, DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_MUL_CFG3_PE1_WT_MUL_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_ELEM_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, val)
/* DX_L2_NPU0.PE1_DMA_ELEM_CFG0.PE1_RD_ELEM_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE_LEN        16
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, bits, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_ELEM_CFG0.PE1_RD_ELEM_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME_LEN        6
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME_OFFSET     16
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME_MASK       0x003f0000 /* [21:16] 0b00000000001111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, bits, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_TIME_OFFSET)

/* DX_L2_NPU0.PE1_DMA_ELEM_CFG0.PE1_RD_ELEM_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE_LEN        2
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE_OFFSET     22
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE_MASK       0x00c00000 /* [23:22] 0b00000000110000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, bits, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_MODE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_ELEM_CFG0.PE1_RD_ELEM_EN (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN_LEN        1
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN_OFFSET     24
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, bits, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_PE1_RD_ELEM_EN_OFFSET)

/* DX_L2_NPU0.PE1_DMA_ELEM_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_LEN        7
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_OFFSET     25
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_MASK       0xfe000000 /* [31:25] 0b11111110000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG0, bits, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_ELEM_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG1)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG1, val)
/* DX_L2_NPU0.PE1_DMA_ELEM_CFG1.PE1_RD_ELEM_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE_LEN        32
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG1, DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG1, DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG1, bits, DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG1_PE1_RD_ELEM_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_ELEM_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG2)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG2, val)
/* DX_L2_NPU0.PE1_DMA_ELEM_CFG2.PE1_RD_ELEM_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG2, DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG2, DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG2, bits, DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG2_PE1_RD_ELEM_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_ELEM_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG3)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG3, val)
/* DX_L2_NPU0.PE1_DMA_ELEM_CFG3.PE1_WT_ELEM_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG3, DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG3, DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_ELEM_CFG3, bits, DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_ELEM_CFG3_PE1_WT_ELEM_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_W_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, val)
/* DX_L2_NPU0.PE1_DMA_W_CFG0.PE1_RD_W_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME_LEN        12
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME_MASK       0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME_MASK, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, bits, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME_MASK, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_TIME_OFFSET)

/* DX_L2_NPU0.PE1_DMA_W_CFG0.PE1_RD_W_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE_LEN        2
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE_OFFSET     12
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE_MASK       0x00003000 /* [13:12] 0b00000000000000000011000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE_MASK, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, bits, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE_MASK, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_MODE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_W_CFG0.PE1_RD_W_EN (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN_LEN        1
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN_OFFSET     14
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN_MASK       0x00004000 /* [14:14] 0b00000000000000000100000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN_MASK, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, bits, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN_MASK, DX_L2_NPU0_PE1_DMA_W_CFG0_PE1_RD_W_EN_OFFSET)

/* DX_L2_NPU0.PE1_DMA_W_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0_LEN        17
#define DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0_OFFSET     15
#define DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG0, bits, DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_W_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_W_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG1)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG1, val)
/* DX_L2_NPU0.PE1_DMA_W_CFG1.PE1_RD_W_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE_LEN        16
#define DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG1, DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG1, DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG1, bits, DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_W_CFG1.PE1_RD_W_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE_LEN        16
#define DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE_OFFSET     16
#define DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG1, DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG1, DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE_MASK, DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG1, bits, DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE_MASK, DX_L2_NPU0_PE1_DMA_W_CFG1_PE1_RD_W_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_W_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG2)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG2, val)
/* DX_L2_NPU0.PE1_DMA_W_CFG2.PE1_RD_W_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG2, DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG2, DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG2, bits, DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_W_CFG2_PE1_RD_W_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_W_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG3)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG3, val)
/* DX_L2_NPU0.PE1_DMA_W_CFG3.PE1_WT_W_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG3, DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG3, DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_W_CFG3, bits, DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_W_CFG3_PE1_WT_W_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_INF_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, val)
/* DX_L2_NPU0.PE1_DMA_INF_CFG0.PE1_RD_INF_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE_LEN        16
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, bits, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_INF_CFG0.PE1_RD_INF_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME_LEN        12
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME_OFFSET     16
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, bits, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_TIME_OFFSET)

/* DX_L2_NPU0.PE1_DMA_INF_CFG0.PE1_RD_INF_MODE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE_LEN        2
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE_OFFSET     28
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE_MASK       0x30000000 /* [29:28] 0b00110000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, bits, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_MODE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_INF_CFG0.PE1_RD_INF_EN (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN_LEN        1
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN_OFFSET     30
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, bits, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG0_PE1_RD_INF_EN_OFFSET)

/* DX_L2_NPU0.PE1_DMA_INF_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0_LEN        1
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0_OFFSET     31
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG0, bits, DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_INF_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG1)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG1, val)
/* DX_L2_NPU0.PE1_DMA_INF_CFG1.PE1_RD_INF_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE_LEN        32
#define DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG1, DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG1, DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG1, bits, DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG1_PE1_RD_INF_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_INF_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG2)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG2, val)
/* DX_L2_NPU0.PE1_DMA_INF_CFG2.PE1_RD_INF_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG2, DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG2, DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG2, bits, DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG2_PE1_RD_INF_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_INF_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG3)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG3, val)
/* DX_L2_NPU0.PE1_DMA_INF_CFG3.PE1_WT_INF_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE_LEN        32
#define DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG3, DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG3, DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG3, bits, DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG3_PE1_WT_INF_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_INF_CFG4 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG4(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG4)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG4(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG4, val)
/* DX_L2_NPU0.PE1_DMA_INF_CFG4.PE1_WT_INF_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG4, DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG4, DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_INF_CFG4, bits, DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_INF_CFG4_PE1_WT_INF_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_OUTF_CFG0 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG0(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG0(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, val)
/* DX_L2_NPU0.PE1_DMA_OUTF_CFG0.PE1_WT_OUTF_SIZE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE_LEN        16
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, bits, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_SIZE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_OUTF_CFG0.PE1_WT_OUTF_TIME (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME_LEN        12
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME_OFFSET     16
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, bits, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_TIME_OFFSET)

/* DX_L2_NPU0.PE1_DMA_OUTF_CFG0.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_LEN        2
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_OFFSET     28
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_MASK       0x30000000 /* [29:28] 0b00110000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, bits, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_OFFSET)

/* DX_L2_NPU0.PE1_DMA_OUTF_CFG0.PE1_WT_OUTF_EN (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN_LEN        1
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN_OFFSET     30
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, bits, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_PE1_WT_OUTF_EN_OFFSET)

/* DX_L2_NPU0.PE1_DMA_OUTF_CFG0.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_LEN        1
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_OFFSET     31
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG0, bits, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_OFFSET)

/* DX_L2_NPU0.PE1_DMA_OUTF_CFG1 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG1(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG1)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG1(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG1, val)
/* DX_L2_NPU0.PE1_DMA_OUTF_CFG1.PE1_WT_OUTF_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE_LEN        32
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG1, DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG1, DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG1, bits, DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG1_PE1_WT_OUTF_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_OUTF_CFG2 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG2(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG2)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG2(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG2, val)
/* DX_L2_NPU0.PE1_DMA_OUTF_CFG2.PE1_WT_OUTF_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG2, DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG2, DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG2, bits, DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG2_PE1_WT_OUTF_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.PE1_DMA_OUTF_CFG3 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG3(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG3)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG3(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG3, val)
/* DX_L2_NPU0.PE1_DMA_OUTF_CFG3.PE1_RD_OUTF_STRIDE (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE_LEN        32
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG3, DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG3, DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG3, bits, DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG3_PE1_RD_OUTF_STRIDE_OFFSET)

/* DX_L2_NPU0.PE1_DMA_OUTF_CFG4 (Category: NPU0) */
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG4(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG4)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG4(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG4, val)
/* DX_L2_NPU0.PE1_DMA_OUTF_CFG4.PE1_RD_OUTF_BASE_ADDR (Category: NPU0) */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR_LEN        32
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR_OFFSET     0
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG4, DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR_MASK)
#define READ_DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG4, DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR_OFFSET)
#define WRITE_DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_PE1_DMA_OUTF_CFG4, bits, DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR_MASK, DX_L2_NPU0_PE1_DMA_OUTF_CFG4_PE1_RD_OUTF_BASE_ADDR_OFFSET)

/* DX_L2_NPU0.RSVD7 (Category: NPU0) */
#define READ_DX_L2_NPU0_RSVD7(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_RSVD7)
#define WRITE_DX_L2_NPU0_RSVD7(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_RSVD7, val)
/* DX_L2_NPU0.RSVD7.RESERVED3 (Category: NPU0) */
#define DX_L2_NPU0_RSVD7_RESERVED3_LEN        8
#define DX_L2_NPU0_RSVD7_RESERVED3_OFFSET     0
#define DX_L2_NPU0_RSVD7_RESERVED3_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD7_RESERVED3_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_NPU0_RSVD7_RESERVED3(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD7, DX_L2_NPU0_RSVD7_RESERVED3_MASK)
#define READ_DX_L2_NPU0_RSVD7_RESERVED3(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD7, DX_L2_NPU0_RSVD7_RESERVED3_MASK, DX_L2_NPU0_RSVD7_RESERVED3_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD7_RESERVED3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD7, bits, DX_L2_NPU0_RSVD7_RESERVED3_MASK, DX_L2_NPU0_RSVD7_RESERVED3_OFFSET)

/* DX_L2_NPU0.RSVD7.RESERVED2 (Category: NPU0) */
#define DX_L2_NPU0_RSVD7_RESERVED2_LEN        8
#define DX_L2_NPU0_RSVD7_RESERVED2_OFFSET     8
#define DX_L2_NPU0_RSVD7_RESERVED2_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD7_RESERVED2_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_L2_NPU0_RSVD7_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD7, DX_L2_NPU0_RSVD7_RESERVED2_MASK)
#define READ_DX_L2_NPU0_RSVD7_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD7, DX_L2_NPU0_RSVD7_RESERVED2_MASK, DX_L2_NPU0_RSVD7_RESERVED2_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD7_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD7, bits, DX_L2_NPU0_RSVD7_RESERVED2_MASK, DX_L2_NPU0_RSVD7_RESERVED2_OFFSET)

/* DX_L2_NPU0.RSVD7.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_RSVD7_RESERVED1_LEN        8
#define DX_L2_NPU0_RSVD7_RESERVED1_OFFSET     16
#define DX_L2_NPU0_RSVD7_RESERVED1_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD7_RESERVED1_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD7_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD7, DX_L2_NPU0_RSVD7_RESERVED1_MASK)
#define READ_DX_L2_NPU0_RSVD7_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD7, DX_L2_NPU0_RSVD7_RESERVED1_MASK, DX_L2_NPU0_RSVD7_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD7_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD7, bits, DX_L2_NPU0_RSVD7_RESERVED1_MASK, DX_L2_NPU0_RSVD7_RESERVED1_OFFSET)

/* DX_L2_NPU0.RSVD7.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_RSVD7_RESERVED0_LEN        8
#define DX_L2_NPU0_RSVD7_RESERVED0_OFFSET     24
#define DX_L2_NPU0_RSVD7_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD7_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD7_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD7, DX_L2_NPU0_RSVD7_RESERVED0_MASK)
#define READ_DX_L2_NPU0_RSVD7_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD7, DX_L2_NPU0_RSVD7_RESERVED0_MASK, DX_L2_NPU0_RSVD7_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD7_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD7, bits, DX_L2_NPU0_RSVD7_RESERVED0_MASK, DX_L2_NPU0_RSVD7_RESERVED0_OFFSET)

/* DX_L2_NPU0.RSVD8 (Category: NPU0) */
#define READ_DX_L2_NPU0_RSVD8(base) dxrt::readl((void*)base, REG_DX_L2_NPU0_RSVD8)
#define WRITE_DX_L2_NPU0_RSVD8(base, val) dxrt::writel((void*)base, REG_DX_L2_NPU0_RSVD8, val)
/* DX_L2_NPU0.RSVD8.RESERVED3 (Category: NPU0) */
#define DX_L2_NPU0_RSVD8_RESERVED3_LEN        8
#define DX_L2_NPU0_RSVD8_RESERVED3_OFFSET     0
#define DX_L2_NPU0_RSVD8_RESERVED3_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD8_RESERVED3_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_L2_NPU0_RSVD8_RESERVED3(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD8, DX_L2_NPU0_RSVD8_RESERVED3_MASK)
#define READ_DX_L2_NPU0_RSVD8_RESERVED3(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD8, DX_L2_NPU0_RSVD8_RESERVED3_MASK, DX_L2_NPU0_RSVD8_RESERVED3_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD8_RESERVED3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD8, bits, DX_L2_NPU0_RSVD8_RESERVED3_MASK, DX_L2_NPU0_RSVD8_RESERVED3_OFFSET)

/* DX_L2_NPU0.RSVD8.RESERVED2 (Category: NPU0) */
#define DX_L2_NPU0_RSVD8_RESERVED2_LEN        8
#define DX_L2_NPU0_RSVD8_RESERVED2_OFFSET     8
#define DX_L2_NPU0_RSVD8_RESERVED2_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD8_RESERVED2_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_L2_NPU0_RSVD8_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD8, DX_L2_NPU0_RSVD8_RESERVED2_MASK)
#define READ_DX_L2_NPU0_RSVD8_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD8, DX_L2_NPU0_RSVD8_RESERVED2_MASK, DX_L2_NPU0_RSVD8_RESERVED2_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD8_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD8, bits, DX_L2_NPU0_RSVD8_RESERVED2_MASK, DX_L2_NPU0_RSVD8_RESERVED2_OFFSET)

/* DX_L2_NPU0.RSVD8.RESERVED1 (Category: NPU0) */
#define DX_L2_NPU0_RSVD8_RESERVED1_LEN        8
#define DX_L2_NPU0_RSVD8_RESERVED1_OFFSET     16
#define DX_L2_NPU0_RSVD8_RESERVED1_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD8_RESERVED1_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD8_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD8, DX_L2_NPU0_RSVD8_RESERVED1_MASK)
#define READ_DX_L2_NPU0_RSVD8_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD8, DX_L2_NPU0_RSVD8_RESERVED1_MASK, DX_L2_NPU0_RSVD8_RESERVED1_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD8_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD8, bits, DX_L2_NPU0_RSVD8_RESERVED1_MASK, DX_L2_NPU0_RSVD8_RESERVED1_OFFSET)

/* DX_L2_NPU0.RSVD8.RESERVED0 (Category: NPU0) */
#define DX_L2_NPU0_RSVD8_RESERVED0_LEN        8
#define DX_L2_NPU0_RSVD8_RESERVED0_OFFSET     24
#define DX_L2_NPU0_RSVD8_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_L2_NPU0_RSVD8_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_L2_NPU0_RSVD8_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_L2_NPU0_RSVD8, DX_L2_NPU0_RSVD8_RESERVED0_MASK)
#define READ_DX_L2_NPU0_RSVD8_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_L2_NPU0_RSVD8, DX_L2_NPU0_RSVD8_RESERVED0_MASK, DX_L2_NPU0_RSVD8_RESERVED0_OFFSET)
#define WRITE_DX_L2_NPU0_RSVD8_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_L2_NPU0_RSVD8, bits, DX_L2_NPU0_RSVD8_RESERVED0_MASK, DX_L2_NPU0_RSVD8_RESERVED0_OFFSET)


int Verify_Dx_l2_npu0_Registers(dxDX_L2_NPU0_t* base);

} /* namespace dxrt */
