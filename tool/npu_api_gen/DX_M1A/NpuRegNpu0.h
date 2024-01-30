/* Automatically generated from DX_M1A.xlsx
 * by Deepx Neural Network Generator v1.0
 */
#pragma once

#include <stdint.h>
#include "dxrt/npuif/npuif.h"

namespace dxrt{
extern bool logNpuRegAccess;

/* Address */
#define REG_DX_M1A_NPU0_OFFSET 0x200
#define REG_DX_M1A_NPU0_DATA_CFG0    0x00000000 /* 0 */
#define REG_DX_M1A_NPU0_CTRL    0x00000004 /* 4 */
#define REG_DX_M1A_NPU0_MISC    0x00000008 /* 8 */
#define REG_DX_M1A_NPU0_PSEUDO    0x0000000c /* 12 */
#define REG_DX_M1A_NPU0_TRP0    0x00000010 /* 16 */
#define REG_DX_M1A_NPU0_TRP1    0x00000014 /* 20 */
#define REG_DX_M1A_NPU0_TRP2    0x00000018 /* 24 */
#define REG_DX_M1A_NPU0_TRP3    0x0000001c /* 28 */
#define REG_DX_M1A_NPU0_TRP4    0x00000020 /* 32 */
#define REG_DX_M1A_NPU0_ARG0    0x00000024 /* 36 */
#define REG_DX_M1A_NPU0_POOL0    0x00000028 /* 40 */
#define REG_DX_M1A_NPU0_POOL1    0x0000002c /* 44 */
#define REG_DX_M1A_NPU0_POOL2    0x00000030 /* 48 */
#define REG_DX_M1A_NPU0_SFU_CTRL0    0x00000034 /* 52 */
#define REG_DX_M1A_NPU0_SFU_COEFF0    0x00000038 /* 56 */
#define REG_DX_M1A_NPU0_SFU_COEFF1    0x0000003c /* 60 */
#define REG_DX_M1A_NPU0_SFU_COEFF2    0x00000040 /* 64 */
#define REG_DX_M1A_NPU0_SFU_COEFF3    0x00000044 /* 68 */
#define REG_DX_M1A_NPU0_SFU_COEFF4    0x00000048 /* 72 */
#define REG_DX_M1A_NPU0_SFU_COEFF5    0x0000004c /* 76 */
#define REG_DX_M1A_NPU0_SFU_COEFF6    0x00000050 /* 80 */
#define REG_DX_M1A_NPU0_SFU_COEFF7    0x00000054 /* 84 */
#define REG_DX_M1A_NPU0_SFU_COEFF8    0x00000058 /* 88 */
#define REG_DX_M1A_NPU0_SFU_COEFF9    0x0000005c /* 92 */
#define REG_DX_M1A_NPU0_SFU_PAF0    0x00000060 /* 96 */
#define REG_DX_M1A_NPU0_SFU_PAF1    0x00000064 /* 100 */
#define REG_DX_M1A_NPU0_SFU_PAF2    0x00000068 /* 104 */
#define REG_DX_M1A_NPU0_SFU_PAF3    0x0000006c /* 108 */
#define REG_DX_M1A_NPU0_SFU_PAF4    0x00000070 /* 112 */
#define REG_DX_M1A_NPU0_SFU_PAF5    0x00000074 /* 116 */
#define REG_DX_M1A_NPU0_SFU_PAF6    0x00000078 /* 120 */
#define REG_DX_M1A_NPU0_SFU_PAF7    0x0000007c /* 124 */
#define REG_DX_M1A_NPU0_SFU_PAF8    0x00000080 /* 128 */
#define REG_DX_M1A_NPU0_SFU_PAF9    0x00000084 /* 132 */
#define REG_DX_M1A_NPU0_SFU_PAF10    0x00000088 /* 136 */
#define REG_DX_M1A_NPU0_SFU_PAF11    0x0000008c /* 140 */
#define REG_DX_M1A_NPU0_SFU_PAF12    0x00000090 /* 144 */
#define REG_DX_M1A_NPU0_SFU_PAF13    0x00000094 /* 148 */
#define REG_DX_M1A_NPU0_SFU_PAF14    0x00000098 /* 152 */
#define REG_DX_M1A_NPU0_SFU_PAF15    0x0000009c /* 156 */
#define REG_DX_M1A_NPU0_SFU_PAF16    0x000000a0 /* 160 */
#define REG_DX_M1A_NPU0_SFU_PAF17    0x000000a4 /* 164 */
#define REG_DX_M1A_NPU0_SFU_PAF18    0x000000a8 /* 168 */
#define REG_DX_M1A_NPU0_SFU_PAF19    0x000000ac /* 172 */
#define REG_DX_M1A_NPU0_SFU_PAF20    0x000000b0 /* 176 */
#define REG_DX_M1A_NPU0_SFU_PAF21    0x000000b4 /* 180 */
#define REG_DX_M1A_NPU0_SFU_PAF22    0x000000b8 /* 184 */
#define REG_DX_M1A_NPU0_SFU_PAF23    0x000000bc /* 188 */
#define REG_DX_M1A_NPU0_SFU_PAF24    0x000000c0 /* 192 */
#define REG_DX_M1A_NPU0_SFU_PAF25    0x000000c4 /* 196 */
#define REG_DX_M1A_NPU0_SFU_PAF26    0x000000c8 /* 200 */
#define REG_DX_M1A_NPU0_SFU_PAF27    0x000000cc /* 204 */
#define REG_DX_M1A_NPU0_SFU_PAF28    0x000000d0 /* 208 */
#define REG_DX_M1A_NPU0_SFU_PAF29    0x000000d4 /* 212 */
#define REG_DX_M1A_NPU0_SFU_PAF30    0x000000d8 /* 216 */
#define REG_DX_M1A_NPU0_SFU_PAF31    0x000000dc /* 220 */
#define REG_DX_M1A_NPU0_SFU_PAF32    0x000000e0 /* 224 */
#define REG_DX_M1A_NPU0_SFU_PAF33    0x000000e4 /* 228 */
#define REG_DX_M1A_NPU0_SFU_PAF34    0x000000e8 /* 232 */
#define REG_DX_M1A_NPU0_SFU_PAF35    0x000000ec /* 236 */
#define REG_DX_M1A_NPU0_SFU_PAF36    0x000000f0 /* 240 */
#define REG_DX_M1A_NPU0_SFU_PAF37    0x000000f4 /* 244 */
#define REG_DX_M1A_NPU0_SFU_PAF38    0x000000f8 /* 248 */
#define REG_DX_M1A_NPU0_SFU_PAF39    0x000000fc /* 252 */
#define REG_DX_M1A_NPU0_SFU_PAF40    0x00000100 /* 256 */
#define REG_DX_M1A_NPU0_SFU_PAF41    0x00000104 /* 260 */
#define REG_DX_M1A_NPU0_SFU_PAF42    0x00000108 /* 264 */
#define REG_DX_M1A_NPU0_SFU_PAF43    0x0000010c /* 268 */
#define REG_DX_M1A_NPU0_SFU_PAF44    0x00000110 /* 272 */
#define REG_DX_M1A_NPU0_SFU_PAF45    0x00000114 /* 276 */
#define REG_DX_M1A_NPU0_SFU_PAF46    0x00000118 /* 280 */
#define REG_DX_M1A_NPU0_SFU_SCALE_CFG0    0x0000011c /* 284 */
#define REG_DX_M1A_NPU0_SFU_SCALE_CFG1    0x00000120 /* 288 */
#define REG_DX_M1A_NPU0_SFU_SCALE_CFG2    0x00000124 /* 292 */
#define REG_DX_M1A_NPU0_SFU_SCALE_CFG3    0x00000128 /* 296 */
#define REG_DX_M1A_NPU0_SFU_ELEM_BRD0    0x0000012c /* 300 */
#define REG_DX_M1A_NPU0_SFU_PAF_DISABLE    0x00000130 /* 304 */
#define REG_DX_M1A_NPU0_PRE0_I2C0    0x00000134 /* 308 */
#define REG_DX_M1A_NPU0_PRE0_I2C1    0x00000138 /* 312 */
#define REG_DX_M1A_NPU0_PRE0_I2C2    0x0000013c /* 316 */
#define REG_DX_M1A_NPU0_PRE0_I2C3    0x00000140 /* 320 */
#define REG_DX_M1A_NPU0_PRE0_FMT0    0x00000144 /* 324 */
#define REG_DX_M1A_NPU0_PRE0_FMT1    0x00000148 /* 328 */
#define REG_DX_M1A_NPU0_PE0_ADDR_GEN0    0x0000014c /* 332 */
#define REG_DX_M1A_NPU0_PE0_ADDR_GEN1    0x00000150 /* 336 */
#define REG_DX_M1A_NPU0_PE0_ADDR_GEN2    0x00000154 /* 340 */
#define REG_DX_M1A_NPU0_PE0_ADDR_GEN3    0x00000158 /* 344 */
#define REG_DX_M1A_NPU0_PE0_ADDR_GEN4    0x0000015c /* 348 */
#define REG_DX_M1A_NPU0_PE0_ADDR_GEN5    0x00000160 /* 352 */
#define REG_DX_M1A_NPU0_PE0_ADDR_GEN6    0x00000164 /* 356 */
#define REG_DX_M1A_NPU0_PE0_ADDR_GEN7    0x00000168 /* 360 */
#define REG_DX_M1A_NPU0_PE0_ADDR_GEN8    0x0000016c /* 364 */
#define REG_DX_M1A_NPU0_PE0_CTRL    0x00000170 /* 368 */
#define REG_DX_M1A_NPU0_PE0_CFG0    0x00000174 /* 372 */
#define REG_DX_M1A_NPU0_PE0_CFG1    0x00000178 /* 376 */
#define REG_DX_M1A_NPU0_PE0_CFG2    0x0000017c /* 380 */
#define REG_DX_M1A_NPU0_PE0_CFG3    0x00000180 /* 384 */
#define REG_DX_M1A_NPU0_PE0_CFG4    0x00000184 /* 388 */
#define REG_DX_M1A_NPU0_PE0_CFG5    0x00000188 /* 392 */
#define REG_DX_M1A_NPU0_PE0_CFG6    0x0000018c /* 396 */
#define REG_DX_M1A_NPU0_PE0_CFG7    0x00000190 /* 400 */
#define REG_DX_M1A_NPU0_PE0_CFG8    0x00000194 /* 404 */
#define REG_DX_M1A_NPU0_PE0_CFG9    0x00000198 /* 408 */
#define REG_DX_M1A_NPU0_PE0_CFG10    0x0000019c /* 412 */
#define REG_DX_M1A_NPU0_PE0_CFG11    0x000001a0 /* 416 */
#define REG_DX_M1A_NPU0_PE0_CFG12    0x000001a4 /* 420 */
#define REG_DX_M1A_NPU0_PE0_CFG13    0x000001a8 /* 424 */
#define REG_DX_M1A_NPU0_PE0_CFG14    0x000001ac /* 428 */
#define REG_DX_M1A_NPU0_PE0_CFG15    0x000001b0 /* 432 */
#define REG_DX_M1A_NPU0_PE0_CFG16    0x000001b4 /* 436 */
#define REG_DX_M1A_NPU0_PE0_CFG17    0x000001b8 /* 440 */
#define REG_DX_M1A_NPU0_PE0_CFG18    0x000001bc /* 444 */
#define REG_DX_M1A_NPU0_PE0_CFG19    0x000001c0 /* 448 */
#define REG_DX_M1A_NPU0_PE0_CFG20    0x000001c4 /* 452 */
#define REG_DX_M1A_NPU0_PE0_CFG21    0x000001c8 /* 456 */
#define REG_DX_M1A_NPU0_PE0_CFG22    0x000001cc /* 460 */
#define REG_DX_M1A_NPU0_PE0_CFG23    0x000001d0 /* 464 */
#define REG_DX_M1A_NPU0_PE0_CFG24    0x000001d4 /* 468 */
#define REG_DX_M1A_NPU0_PE0_CFG25    0x000001d8 /* 472 */
#define REG_DX_M1A_NPU0_PE0_CFG26    0x000001dc /* 476 */
#define REG_DX_M1A_NPU0_PE0_VMEM_CFG0    0x000001e0 /* 480 */
#define REG_DX_M1A_NPU0_REDUCE0_CFG0    0x000001e4 /* 484 */
#define REG_DX_M1A_NPU0_REDUCE0_CFG1    0x000001e8 /* 488 */
#define REG_DX_M1A_NPU0_REDUCE0_CFG2    0x000001ec /* 492 */
#define REG_DX_M1A_NPU0_REDUCE0_CFG3    0x000001f0 /* 496 */
#define REG_DX_M1A_NPU0_PE0_DMA_CTRL    0x000001f4 /* 500 */
#define REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0    0x000001f8 /* 504 */
#define REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG1    0x000001fc /* 508 */
#define REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG2    0x00000200 /* 512 */
#define REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG3    0x00000204 /* 516 */
#define REG_DX_M1A_NPU0_PE0_DMA_W_CFG0    0x00000208 /* 520 */
#define REG_DX_M1A_NPU0_PE0_DMA_W_CFG1    0x0000020c /* 524 */
#define REG_DX_M1A_NPU0_PE0_DMA_W_CFG2    0x00000210 /* 528 */
#define REG_DX_M1A_NPU0_PE0_DMA_W_CFG3    0x00000214 /* 532 */
#define REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0    0x00000218 /* 536 */
#define REG_DX_M1A_NPU0_PE0_DMA_INF_CFG1    0x0000021c /* 540 */
#define REG_DX_M1A_NPU0_PE0_DMA_INF_CFG2    0x00000220 /* 544 */
#define REG_DX_M1A_NPU0_PE0_DMA_INF_CFG3    0x00000224 /* 548 */
#define REG_DX_M1A_NPU0_PE0_DMA_INF_CFG4    0x00000228 /* 552 */
#define REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0    0x0000022c /* 556 */
#define REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG1    0x00000230 /* 560 */
#define REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG2    0x00000234 /* 564 */
#define REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG3    0x00000238 /* 568 */
#define REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG4    0x0000023c /* 572 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0    0x00000240 /* 576 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1    0x00000244 /* 580 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG2    0x00000248 /* 584 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG3    0x0000024c /* 588 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG4    0x00000250 /* 592 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG5    0x00000254 /* 596 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6    0x00000258 /* 600 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7    0x0000025c /* 604 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG8    0x00000260 /* 608 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG9    0x00000264 /* 612 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG10    0x00000268 /* 616 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG11    0x0000026c /* 620 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12    0x00000270 /* 624 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13    0x00000274 /* 628 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG14    0x00000278 /* 632 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG15    0x0000027c /* 636 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG16    0x00000280 /* 640 */
#define REG_DX_M1A_NPU0_PE0_DMA_RF_CFG17    0x00000284 /* 644 */
#define REG_DX_M1A_NPU0_PRE1_I2C0    0x00000288 /* 648 */
#define REG_DX_M1A_NPU0_PRE1_I2C1    0x0000028c /* 652 */
#define REG_DX_M1A_NPU0_PRE1_I2C2    0x00000290 /* 656 */
#define REG_DX_M1A_NPU0_PRE1_I2C3    0x00000294 /* 660 */
#define REG_DX_M1A_NPU0_PRE1_FMT0    0x00000298 /* 664 */
#define REG_DX_M1A_NPU0_PRE1_FMT1    0x0000029c /* 668 */
#define REG_DX_M1A_NPU0_PE1_ADDR_GEN0    0x000002a0 /* 672 */
#define REG_DX_M1A_NPU0_PE1_ADDR_GEN1    0x000002a4 /* 676 */
#define REG_DX_M1A_NPU0_PE1_ADDR_GEN2    0x000002a8 /* 680 */
#define REG_DX_M1A_NPU0_PE1_ADDR_GEN3    0x000002ac /* 684 */
#define REG_DX_M1A_NPU0_PE1_ADDR_GEN4    0x000002b0 /* 688 */
#define REG_DX_M1A_NPU0_PE1_ADDR_GEN5    0x000002b4 /* 692 */
#define REG_DX_M1A_NPU0_PE1_ADDR_GEN6    0x000002b8 /* 696 */
#define REG_DX_M1A_NPU0_PE1_ADDR_GEN7    0x000002bc /* 700 */
#define REG_DX_M1A_NPU0_PE1_ADDR_GEN8    0x000002c0 /* 704 */
#define REG_DX_M1A_NPU0_PE1_CTRL    0x000002c4 /* 708 */
#define REG_DX_M1A_NPU0_PE1_CFG0    0x000002c8 /* 712 */
#define REG_DX_M1A_NPU0_PE1_CFG1    0x000002cc /* 716 */
#define REG_DX_M1A_NPU0_PE1_CFG2    0x000002d0 /* 720 */
#define REG_DX_M1A_NPU0_PE1_CFG3    0x000002d4 /* 724 */
#define REG_DX_M1A_NPU0_PE1_CFG4    0x000002d8 /* 728 */
#define REG_DX_M1A_NPU0_PE1_CFG5    0x000002dc /* 732 */
#define REG_DX_M1A_NPU0_PE1_CFG6    0x000002e0 /* 736 */
#define REG_DX_M1A_NPU0_PE1_CFG7    0x000002e4 /* 740 */
#define REG_DX_M1A_NPU0_PE1_CFG8    0x000002e8 /* 744 */
#define REG_DX_M1A_NPU0_PE1_CFG9    0x000002ec /* 748 */
#define REG_DX_M1A_NPU0_PE1_CFG10    0x000002f0 /* 752 */
#define REG_DX_M1A_NPU0_PE1_CFG11    0x000002f4 /* 756 */
#define REG_DX_M1A_NPU0_PE1_CFG12    0x000002f8 /* 760 */
#define REG_DX_M1A_NPU0_PE1_CFG13    0x000002fc /* 764 */
#define REG_DX_M1A_NPU0_PE1_CFG14    0x00000300 /* 768 */
#define REG_DX_M1A_NPU0_PE1_CFG15    0x00000304 /* 772 */
#define REG_DX_M1A_NPU0_PE1_CFG16    0x00000308 /* 776 */
#define REG_DX_M1A_NPU0_PE1_CFG17    0x0000030c /* 780 */
#define REG_DX_M1A_NPU0_PE1_CFG18    0x00000310 /* 784 */
#define REG_DX_M1A_NPU0_PE1_CFG19    0x00000314 /* 788 */
#define REG_DX_M1A_NPU0_PE1_CFG20    0x00000318 /* 792 */
#define REG_DX_M1A_NPU0_PE1_CFG21    0x0000031c /* 796 */
#define REG_DX_M1A_NPU0_PE1_CFG22    0x00000320 /* 800 */
#define REG_DX_M1A_NPU0_PE1_CFG23    0x00000324 /* 804 */
#define REG_DX_M1A_NPU0_PE1_CFG24    0x00000328 /* 808 */
#define REG_DX_M1A_NPU0_PE1_CFG25    0x0000032c /* 812 */
#define REG_DX_M1A_NPU0_PE1_CFG26    0x00000330 /* 816 */
#define REG_DX_M1A_NPU0_PE1_VMEM_CFG0    0x00000334 /* 820 */
#define REG_DX_M1A_NPU0_REDUCE1_CFG0    0x00000338 /* 824 */
#define REG_DX_M1A_NPU0_REDUCE1_CFG1    0x0000033c /* 828 */
#define REG_DX_M1A_NPU0_REDUCE1_CFG2    0x00000340 /* 832 */
#define REG_DX_M1A_NPU0_REDUCE1_CFG3    0x00000344 /* 836 */
#define REG_DX_M1A_NPU0_PE1_DMA_CTRL    0x00000348 /* 840 */
#define REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0    0x0000034c /* 844 */
#define REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG1    0x00000350 /* 848 */
#define REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG2    0x00000354 /* 852 */
#define REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG3    0x00000358 /* 856 */
#define REG_DX_M1A_NPU0_PE1_DMA_W_CFG0    0x0000035c /* 860 */
#define REG_DX_M1A_NPU0_PE1_DMA_W_CFG1    0x00000360 /* 864 */
#define REG_DX_M1A_NPU0_PE1_DMA_W_CFG2    0x00000364 /* 868 */
#define REG_DX_M1A_NPU0_PE1_DMA_W_CFG3    0x00000368 /* 872 */
#define REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0    0x0000036c /* 876 */
#define REG_DX_M1A_NPU0_PE1_DMA_INF_CFG1    0x00000370 /* 880 */
#define REG_DX_M1A_NPU0_PE1_DMA_INF_CFG2    0x00000374 /* 884 */
#define REG_DX_M1A_NPU0_PE1_DMA_INF_CFG3    0x00000378 /* 888 */
#define REG_DX_M1A_NPU0_PE1_DMA_INF_CFG4    0x0000037c /* 892 */
#define REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0    0x00000380 /* 896 */
#define REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG1    0x00000384 /* 900 */
#define REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG2    0x00000388 /* 904 */
#define REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG3    0x0000038c /* 908 */
#define REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG4    0x00000390 /* 912 */
#define REG_DX_M1A_NPU0_PPU_CFG_0    0x00000394 /* 916 */
#define REG_DX_M1A_NPU0_PPU_CFG_1    0x00000398 /* 920 */
#define REG_DX_M1A_NPU0_PPU_CFG_2    0x0000039c /* 924 */
#define REG_DX_M1A_NPU0_PPU_CFG_3    0x000003a0 /* 928 */
#define REG_DX_M1A_NPU0_PPU_CFG_4    0x000003a4 /* 932 */
#define REG_DX_M1A_NPU0_PPU_CFG_6    0x000003a8 /* 936 */
#define REG_DX_M1A_NPU0_PPU_CFG_7    0x000003ac /* 940 */
#define REG_DX_M1A_NPU0_PPU_CFG_9    0x000003b0 /* 944 */
#define REG_DX_M1A_NPU0_CHECKSUM    0x000003b4 /* 948 */

/* Register Definition */
typedef volatile union _DX_M1A_NPU0_DATA_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.DATA_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FEATURE_BIT_WIDTH:8;  /* DX_M1A_NPU0.DATA_CFG0.FEATURE_BIT_WIDTH [7:0] (RW, 0x0) OUTPUT FEATURE DATA BIT WIDTH */
        volatile uint32_t W_FEATURE_BIT_WIDTH:8;  /* DX_M1A_NPU0.DATA_CFG0.W_FEATURE_BIT_WIDTH [15:8] (RW, 0x0) INPUT FEATURE DATA BIT WIDTH */
        volatile uint32_t RESERVED0:16;  /* DX_M1A_NPU0.DATA_CFG0.RESERVED0 [31:16] (R, 0x0)  */
    };
} DX_M1A_NPU0_DATA_CFG0_t;
typedef volatile union _DX_M1A_NPU0_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:6;  /* DX_M1A_NPU0.CTRL.RESERVED0 [5:0] (R, 0x0)  */
        volatile uint32_t RESIZE:2;  /* DX_M1A_NPU0.CTRL.RESIZE [7:6] (RW, 0x0) 1: DEQUANTIZATION OPERATION,  2: NEARLIST NEIGHBOR OPERATION,  3: BILINEAR OPERATION */
        volatile uint32_t WRITE_POOL_ONLY:1;  /* DX_M1A_NPU0.CTRL.WRITE_POOL_ONLY [8:8] (RW, 0x0) POOLING이 있을 경우 ON THE FLY로 수행하게 될 때, SRAM에 저장할 DATA를 선택 0: POOLING 전/후 모두 저장 1: POOLING 후만 저장 */
        volatile uint32_t WEIGHT_BROADCAST_MODE_EN:2;  /* DX_M1A_NPU0.CTRL.WEIGHT_BROADCAST_MODE_EN [10:9] (RW, 0x0) THREAD간 WEIGHT를 공유해서 사용할 때 ENABLE 0: 공유 X 1: THREAD #0의 DATA를 모든 THREAD가 공유 2: THREAD #0,1 공유 , THREAD #2,3 공유 */
        volatile uint32_t FEATURE_BROADCAST_MODE_EN:2;  /* DX_M1A_NPU0.CTRL.FEATURE_BROADCAST_MODE_EN [12:11] (RW, 0x0) THREAD간 FEATURE를 공유해서 사용할 때 ENABLE 0: 공유 X 1: THREAD #0의 DATA를 모든 THREAD가 공유 2: THREAD #0,1 공유 , THREAD #2,3 공유 */
        volatile uint32_t ACTIVE_CNT:8;  /* DX_M1A_NPU0.CTRL.ACTIVE_CNT [20:13] (RW, 0x0) MAC ↔ SFU간의 THROUGHPUT으로 인한 속도 조절 PARAMETER */
        volatile uint32_t ACTIVE_PERIOD:8;  /* DX_M1A_NPU0.CTRL.ACTIVE_PERIOD [28:21] (RW, 0x0) MAC ↔ SFU간의 THROUGHPUT으로 인한 속도 조절 PARAMETER */
        volatile uint32_t FEATURE_DATA_TYPE:2;  /* DX_M1A_NPU0.CTRL.FEATURE_DATA_TYPE [30:29] (RW, 0x0) 0: UNSIGNED, 1: SIGNED, 2: FLOAT24 */
        volatile uint32_t INPUT_DONE:1;  /* DX_M1A_NPU0.CTRL.INPUT_DONE [31:31] (RW, 0x0) 0: NO INPUT DONE IRQ, 1: GENERATE INPUT DONE IRQ @ TILE */
    };
} DX_M1A_NPU0_CTRL_t;
typedef volatile union _DX_M1A_NPU0_MISC_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.MISC*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t TILE_NUM:16;  /* DX_M1A_NPU0.MISC.TILE_NUM [15:0] (RW, 0x0) TILE NO.(0~) NO USE, BUT FOR DEBUG */
        volatile uint32_t LAYER_NUM:16;  /* DX_M1A_NPU0.MISC.LAYER_NUM [31:16] (RW, 0x0) LAYER NO.(0~) NO USE, BUT FOR DEBUG */
    };
} DX_M1A_NPU0_MISC_t;
typedef volatile union _DX_M1A_NPU0_PSEUDO_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PSEUDO*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PSEUDO_CNT:32;  /* DX_M1A_NPU0.PSEUDO.PSEUDO_CNT [31:0] (RW, 0x0) PSEUDONPU CONVOLUTION COUNT */
    };
} DX_M1A_NPU0_PSEUDO_t;
typedef volatile union _DX_M1A_NPU0_TRP0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.TRP0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t TRP_SURFACE_STRIDE:16;  /* DX_M1A_NPU0.TRP0.TRP_SURFACE_STRIDE [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:14;  /* DX_M1A_NPU0.TRP0.RESERVED0 [29:16] (R, 0x0)  */
        volatile uint32_t TRP_32BIT_MODE:1;  /* DX_M1A_NPU0.TRP0.TRP_32BIT_MODE [30:30] (RW, 0x0) 0: 8 BIT MODE, 1: 32 BIT MODE */
        volatile uint32_t TRP_EN:1;  /* DX_M1A_NPU0.TRP0.TRP_EN [31:31] (RW, 0x0) 0: TRANSPOSE OFF, 1: TRANSPOSE ON */
    };
} DX_M1A_NPU0_TRP0_t;
typedef volatile union _DX_M1A_NPU0_TRP1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.TRP1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t TRP_TOTAL_WSIZE:16;  /* DX_M1A_NPU0.TRP1.TRP_TOTAL_WSIZE [15:0] (RW, 0x0)  */
        volatile uint32_t TRP_IN_SIZE:16;  /* DX_M1A_NPU0.TRP1.TRP_IN_SIZE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_TRP1_t;
typedef volatile union _DX_M1A_NPU0_TRP2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.TRP2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t TRP_ADDR_STRIDE:16;  /* DX_M1A_NPU0.TRP2.TRP_ADDR_STRIDE [15:0] (RW, 0x0)  */
        volatile uint32_t TRP_ITER:9;  /* DX_M1A_NPU0.TRP2.TRP_ITER [24:16] (RW, 0x0)  */
        volatile uint32_t TRP_LAST_WSIZE:7;  /* DX_M1A_NPU0.TRP2.TRP_LAST_WSIZE [31:25] (RW, 0x0)  */
    };
} DX_M1A_NPU0_TRP2_t;
typedef volatile union _DX_M1A_NPU0_TRP3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.TRP3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t TRP_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.TRP3.TRP_READ_BASE_ADDR [31:0] (RW, 0x0) DATA READ BASE ADDRESS(SRAM) */
    };
} DX_M1A_NPU0_TRP3_t;
typedef volatile union _DX_M1A_NPU0_TRP4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.TRP4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t TRP_WRITE_BASE_ADDR:32;  /* DX_M1A_NPU0.TRP4.TRP_WRITE_BASE_ADDR [31:0] (RW, 0x0) DATA WRITE BASE ADDRESS(SRAM) */
    };
} DX_M1A_NPU0_TRP4_t;
typedef volatile union _DX_M1A_NPU0_ARG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.ARG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:13;  /* DX_M1A_NPU0.ARG0.RESERVED0 [12:0] (R, 0x0)  */
        volatile uint32_t ARG_MAX_MODE:2;  /* DX_M1A_NPU0.ARG0.ARG_MAX_MODE [14:13] (RW, 0x0) 0: OFF, 1: (16 DATA) IN @ IC,  2: (2048 DATA) IN @ SEG PHASE 1,  3: (128 DUMMY + 8 INDEX + 8 DATA) IN @ SEG PHASE 2 */
        volatile uint32_t ARG_CLASS_NUM:16;  /* DX_M1A_NPU0.ARG0.ARG_CLASS_NUM [30:15] (RW, 0x0) # CLASS */
        volatile uint32_t ARG_MAX_OUT_FORMAT:1;  /* DX_M1A_NPU0.ARG0.ARG_MAX_OUT_FORMAT [31:31] (RW, 0x0) @ ARG_MAX_MODE == 2 0: FINAL OUTPUT (CHANNEL < 64 일때, PHASE 2 없이 바로 최종 출력) 1: INDEX + VALUE(8) */
    };
} DX_M1A_NPU0_ARG0_t;
typedef volatile union _DX_M1A_NPU0_POOL0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.POOL0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:30;  /* DX_M1A_NPU0.POOL0.RESERVED0 [29:0] (R, 0x0)  */
        volatile uint32_t POOL_MODE:2;  /* DX_M1A_NPU0.POOL0.POOL_MODE [31:30] (RW, 0x0) 0: BYPSS, 1: AVG POOL,  2: MAX POOL */
    };
} DX_M1A_NPU0_POOL0_t;
typedef volatile union _DX_M1A_NPU0_POOL1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.POOL1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t POOL_WINSIZE:32;  /* DX_M1A_NPU0.POOL1.POOL_WINSIZE [31:0] (RW, 0x0) SIZE(NUMBER OF ELEMENT) OF KERNEL WHEN MAX_POOLING */
    };
} DX_M1A_NPU0_POOL1_t;
typedef volatile union _DX_M1A_NPU0_POOL2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.POOL2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t AVG_POOL_MULT_COEFF:32;  /* DX_M1A_NPU0.POOL2.AVG_POOL_MULT_COEFF [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_POOL2_t;
typedef volatile union _DX_M1A_NPU0_SFU_CTRL0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_CTRL0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SFU_HALF_RUN_MODE:1;  /* DX_M1A_NPU0.SFU_CTRL0.SFU_HALF_RUN_MODE [0:0] (RW, 0x0) 0: SFU THROUGHPUT 64,  1: SFU THROUGHPUT 32 */
        volatile uint32_t SKIP_ADD_SEL:1;  /* DX_M1A_NPU0.SFU_CTRL0.SKIP_ADD_SEL [1:1] (RW, 0x0) 0: COEF FROM SFR,  1: COEF FROM RF MEM */
        volatile uint32_t SKIP_MUL_SEL:1;  /* DX_M1A_NPU0.SFU_CTRL0.SKIP_MUL_SEL [2:2] (RW, 0x0) 0: COEF FROM SFR,  1: COEF FROM RF MEM */
        volatile uint32_t FLOAT2INT_MODE:1;  /* DX_M1A_NPU0.SFU_CTRL0.FLOAT2INT_MODE [3:3] (RW, 0x0) 0: DISABLE,  1: ENABLE */
        volatile uint32_t SKIP_IN_TYPE:1;  /* DX_M1A_NPU0.SFU_CTRL0.SKIP_IN_TYPE [4:4] (RW, 0x0) 0: SIGNED 8BIT,  1: UNSIGNED 8BIT */
        volatile uint32_t PRE_ACT_POW_EN:1;  /* DX_M1A_NPU0.SFU_CTRL0.PRE_ACT_POW_EN [5:5] (RW, 0x0)  */
        volatile uint32_t PRE_ACT_MODE:3;  /* DX_M1A_NPU0.SFU_CTRL0.PRE_ACT_MODE [8:6] (RW, 0x0) 0: BYPASS,  2: ADD_FLOAT_BIAS,  4: ADD_FLOAT_ONLY,  5: MULT_FLOAT_ONLY, 6: TWO LANE MODE,  1,3,7: BYPASS */
        volatile uint32_t PRE_MULT_COEFF_SEL:1;  /* DX_M1A_NPU0.SFU_CTRL0.PRE_MULT_COEFF_SEL [9:9] (RW, 0x0) 0: COEF FROM SFR,  1: COEF FROM RF MEM */
        volatile uint32_t ACT_MODE:3;  /* DX_M1A_NPU0.SFU_CTRL0.ACT_MODE [12:10] (RW, 0x0) 0: BYPASS,  1: RELU,  2: LEAKY-RELU,  3: RELU6,  4: PAF,  5: PRELU */
        volatile uint32_t ACT_INOUT_CTRL:1;  /* DX_M1A_NPU0.SFU_CTRL0.ACT_INOUT_CTRL [13:13] (RW, 0x0) 0: PRE_ACT → ACT,  1: POST_ACT → ACT */
        volatile uint32_t POST_ACT_MODE:2;  /* DX_M1A_NPU0.SFU_CTRL0.POST_ACT_MODE [15:14] (RW, 0x0) 0: BYPASS,  1: ELEM_ADD,  2: ELEM_MUL */
        volatile uint32_t PRE_ADD_COEFF_SEL:1;  /* DX_M1A_NPU0.SFU_CTRL0.PRE_ADD_COEFF_SEL [16:16] (RW, 0x0) 0: COEF FROM SFR,  1: COEF FROM RF MEM */
        volatile uint32_t SE_EN:1;  /* DX_M1A_NPU0.SFU_CTRL0.SE_EN [17:17] (RW, 0x0) SQUEEZE & EXCITATION LAYER에서 1X1 결과 사용 OPTION(EXCITATION LAYER) 0: SE RF DISENABLE(READ),  1: SE RF ENABLE(READ) */
        volatile uint32_t SE_WRITE:1;  /* DX_M1A_NPU0.SFU_CTRL0.SE_WRITE [18:18] (RW, 0x0) SQUEEZE & EXCITATION LAYER에서 1X1 결과 출력 OPTION(SQUEEZE LAYER) 0: SRAM MEMORY WRITE,  1: RF MEMORY WRITE */
        volatile uint32_t SFU_OUT_MERGE_NUM:3;  /* DX_M1A_NPU0.SFU_CTRL0.SFU_OUT_MERGE_NUM [21:19] (RW, 0x0) MERGE 되는 SFU 출력 데이터 수(WORD 단위) */
        volatile uint32_t SFU_OUT_MERGE_EN:1;  /* DX_M1A_NPU0.SFU_CTRL0.SFU_OUT_MERGE_EN [22:22] (RW, 0x0) SFU 출력 DATA MERGE(24BIT X16 -> 8BIT X 16) */
        volatile uint32_t RESERVED0:9;  /* DX_M1A_NPU0.SFU_CTRL0.RESERVED0 [31:23] (R, 0x0)  */
    };
} DX_M1A_NPU0_SFU_CTRL0_t;
typedef volatile union _DX_M1A_NPU0_SFU_COEFF0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_COEFF0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CLIP_MAX:32;  /* DX_M1A_NPU0.SFU_COEFF0.CLIP_MAX [31:0] (RW, 0x0) CLIPPING UPPER LIMIT */
    };
} DX_M1A_NPU0_SFU_COEFF0_t;
typedef volatile union _DX_M1A_NPU0_SFU_COEFF1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_COEFF1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CLIP_MIN:32;  /* DX_M1A_NPU0.SFU_COEFF1.CLIP_MIN [31:0] (RW, 0x0) CLIPPING LOWER LIMIT */
    };
} DX_M1A_NPU0_SFU_COEFF1_t;
typedef volatile union _DX_M1A_NPU0_SFU_COEFF2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_COEFF2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SKIP_ADD_COEFF:32;  /* DX_M1A_NPU0.SFU_COEFF2.SKIP_ADD_COEFF [31:0] (RW, 0x0) VALUE WHEN SKIP_ADD_SEL = 0 (FLOAT24, LSB DUMMY) */
    };
} DX_M1A_NPU0_SFU_COEFF2_t;
typedef volatile union _DX_M1A_NPU0_SFU_COEFF3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_COEFF3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PRE_ACT_MULT_COEFF:32;  /* DX_M1A_NPU0.SFU_COEFF3.PRE_ACT_MULT_COEFF [31:0] (RW, 0x0) VALUE WHEN PRE_MULT_COEFF_SEL = 0 (FLOAT24, LSB DUMMY) */
    };
} DX_M1A_NPU0_SFU_COEFF3_t;
typedef volatile union _DX_M1A_NPU0_SFU_COEFF4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_COEFF4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PRE_ACT_ADD_COEFF:32;  /* DX_M1A_NPU0.SFU_COEFF4.PRE_ACT_ADD_COEFF [31:0] (RW, 0x0) VALUE WHEN PRE_ADD_COEFF_SEL = 0 (FLOAT24, LSB DUMMY) */
    };
} DX_M1A_NPU0_SFU_COEFF4_t;
typedef volatile union _DX_M1A_NPU0_SFU_COEFF5_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_COEFF5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t LMUL_ADD_COEFF:32;  /* DX_M1A_NPU0.SFU_COEFF5.LMUL_ADD_COEFF [31:0] (RW, 0x0) POST_ACT LAST ADD COEF */
    };
} DX_M1A_NPU0_SFU_COEFF5_t;
typedef volatile union _DX_M1A_NPU0_SFU_COEFF6_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_COEFF6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t ACT_OFFSET_VAL:32;  /* DX_M1A_NPU0.SFU_COEFF6.ACT_OFFSET_VAL [31:0] (RW, 0x0) OFFSET VALUE WHEN RELU OR RELU6 (FLOAT24, LSB DUMMY) */
    };
} DX_M1A_NPU0_SFU_COEFF6_t;
typedef volatile union _DX_M1A_NPU0_SFU_COEFF7_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_COEFF7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SKIP_SCALE:32;  /* DX_M1A_NPU0.SFU_COEFF7.SKIP_SCALE [31:0] (RW, 0x0) VALUE WHEN SKIP_MUL_SEL = 0 (FLOAT24, LSB DUMMY) */
    };
} DX_M1A_NPU0_SFU_COEFF7_t;
typedef volatile union _DX_M1A_NPU0_SFU_COEFF8_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_COEFF8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t LEAKY_RELU_SLOPE:32;  /* DX_M1A_NPU0.SFU_COEFF8.LEAKY_RELU_SLOPE [31:0] (RW, 0x0) NEGATIVE SLOPE (FLOAT24, LSB DUMMY) */
    };
} DX_M1A_NPU0_SFU_COEFF8_t;
typedef volatile union _DX_M1A_NPU0_SFU_COEFF9_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_COEFF9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RELU6_CLIP_MAX:32;  /* DX_M1A_NPU0.SFU_COEFF9.RELU6_CLIP_MAX [31:0] (RW, 0x0) RELU6 CLIPPING VALUE */
    };
} DX_M1A_NPU0_SFU_COEFF9_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY0:32;  /* DX_M1A_NPU0.SFU_PAF0.PAF_BOUNDARY0 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION0 AND REGION1 */
    };
} DX_M1A_NPU0_SFU_PAF0_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY1:32;  /* DX_M1A_NPU0.SFU_PAF1.PAF_BOUNDARY1 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION1 AND REGION2 */
    };
} DX_M1A_NPU0_SFU_PAF1_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY2:32;  /* DX_M1A_NPU0.SFU_PAF2.PAF_BOUNDARY2 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION2 AND REGION3 */
    };
} DX_M1A_NPU0_SFU_PAF2_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY3:32;  /* DX_M1A_NPU0.SFU_PAF3.PAF_BOUNDARY3 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION3 AND REGION4 */
    };
} DX_M1A_NPU0_SFU_PAF3_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY4:32;  /* DX_M1A_NPU0.SFU_PAF4.PAF_BOUNDARY4 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION4 AND REGION5 */
    };
} DX_M1A_NPU0_SFU_PAF4_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF5_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY5:32;  /* DX_M1A_NPU0.SFU_PAF5.PAF_BOUNDARY5 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION5 AND REGION6 */
    };
} DX_M1A_NPU0_SFU_PAF5_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF6_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY6:32;  /* DX_M1A_NPU0.SFU_PAF6.PAF_BOUNDARY6 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION6 AND REGION7 */
    };
} DX_M1A_NPU0_SFU_PAF6_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF7_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY7:32;  /* DX_M1A_NPU0.SFU_PAF7.PAF_BOUNDARY7 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION7 AND REGION8 */
    };
} DX_M1A_NPU0_SFU_PAF7_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF8_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY8:32;  /* DX_M1A_NPU0.SFU_PAF8.PAF_BOUNDARY8 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION8 AND REGION9 */
    };
} DX_M1A_NPU0_SFU_PAF8_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF9_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY9:32;  /* DX_M1A_NPU0.SFU_PAF9.PAF_BOUNDARY9 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION9 AND REGION10 */
    };
} DX_M1A_NPU0_SFU_PAF9_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF10_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF10*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY10:32;  /* DX_M1A_NPU0.SFU_PAF10.PAF_BOUNDARY10 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION10 AND REGION11 */
    };
} DX_M1A_NPU0_SFU_PAF10_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF11_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF11*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY11:32;  /* DX_M1A_NPU0.SFU_PAF11.PAF_BOUNDARY11 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION11 AND REGION12 */
    };
} DX_M1A_NPU0_SFU_PAF11_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF12_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF12*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY12:32;  /* DX_M1A_NPU0.SFU_PAF12.PAF_BOUNDARY12 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION12 AND REGION13 */
    };
} DX_M1A_NPU0_SFU_PAF12_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF13_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF13*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY13:32;  /* DX_M1A_NPU0.SFU_PAF13.PAF_BOUNDARY13 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION13 AND REGION14 */
    };
} DX_M1A_NPU0_SFU_PAF13_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF14_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF14*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_BOUNDARY14:32;  /* DX_M1A_NPU0.SFU_PAF14.PAF_BOUNDARY14 [31:0] (RW, 0x0) PAF PARAMETER, BORDER OF REGION14 AND REGION15 */
    };
} DX_M1A_NPU0_SFU_PAF14_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF15_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF15*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF0:32;  /* DX_M1A_NPU0.SFU_PAF15.PAF_MULT_COEFF0 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION0 */
    };
} DX_M1A_NPU0_SFU_PAF15_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF16_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF16*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF1:32;  /* DX_M1A_NPU0.SFU_PAF16.PAF_MULT_COEFF1 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION1 */
    };
} DX_M1A_NPU0_SFU_PAF16_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF17_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF17*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF2:32;  /* DX_M1A_NPU0.SFU_PAF17.PAF_MULT_COEFF2 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION2 */
    };
} DX_M1A_NPU0_SFU_PAF17_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF18_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF18*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF3:32;  /* DX_M1A_NPU0.SFU_PAF18.PAF_MULT_COEFF3 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION3 */
    };
} DX_M1A_NPU0_SFU_PAF18_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF19_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF19*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF4:32;  /* DX_M1A_NPU0.SFU_PAF19.PAF_MULT_COEFF4 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION4 */
    };
} DX_M1A_NPU0_SFU_PAF19_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF20_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF20*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF5:32;  /* DX_M1A_NPU0.SFU_PAF20.PAF_MULT_COEFF5 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION5 */
    };
} DX_M1A_NPU0_SFU_PAF20_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF21_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF21*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF6:32;  /* DX_M1A_NPU0.SFU_PAF21.PAF_MULT_COEFF6 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION6 */
    };
} DX_M1A_NPU0_SFU_PAF21_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF22_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF22*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF7:32;  /* DX_M1A_NPU0.SFU_PAF22.PAF_MULT_COEFF7 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION7 */
    };
} DX_M1A_NPU0_SFU_PAF22_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF23_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF23*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF8:32;  /* DX_M1A_NPU0.SFU_PAF23.PAF_MULT_COEFF8 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION8 */
    };
} DX_M1A_NPU0_SFU_PAF23_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF24_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF24*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF9:32;  /* DX_M1A_NPU0.SFU_PAF24.PAF_MULT_COEFF9 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION9 */
    };
} DX_M1A_NPU0_SFU_PAF24_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF25_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF25*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF10:32;  /* DX_M1A_NPU0.SFU_PAF25.PAF_MULT_COEFF10 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION10 */
    };
} DX_M1A_NPU0_SFU_PAF25_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF26_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF26*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF11:32;  /* DX_M1A_NPU0.SFU_PAF26.PAF_MULT_COEFF11 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION11 */
    };
} DX_M1A_NPU0_SFU_PAF26_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF27_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF27*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF12:32;  /* DX_M1A_NPU0.SFU_PAF27.PAF_MULT_COEFF12 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION12 */
    };
} DX_M1A_NPU0_SFU_PAF27_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF28_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF28*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF13:32;  /* DX_M1A_NPU0.SFU_PAF28.PAF_MULT_COEFF13 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION13 */
    };
} DX_M1A_NPU0_SFU_PAF28_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF29_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF29*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF14:32;  /* DX_M1A_NPU0.SFU_PAF29.PAF_MULT_COEFF14 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION14 */
    };
} DX_M1A_NPU0_SFU_PAF29_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF30_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF30*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_MULT_COEFF15:32;  /* DX_M1A_NPU0.SFU_PAF30.PAF_MULT_COEFF15 [31:0] (RW, 0x0) PAF PARAMETER, WEIGHT OF REGION15 */
    };
} DX_M1A_NPU0_SFU_PAF30_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF31_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF31*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF0:32;  /* DX_M1A_NPU0.SFU_PAF31.PAF_ADD_COEFF0 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION0 */
    };
} DX_M1A_NPU0_SFU_PAF31_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF32_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF32*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF1:32;  /* DX_M1A_NPU0.SFU_PAF32.PAF_ADD_COEFF1 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION1 */
    };
} DX_M1A_NPU0_SFU_PAF32_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF33_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF33*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF2:32;  /* DX_M1A_NPU0.SFU_PAF33.PAF_ADD_COEFF2 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION2 */
    };
} DX_M1A_NPU0_SFU_PAF33_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF34_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF34*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF3:32;  /* DX_M1A_NPU0.SFU_PAF34.PAF_ADD_COEFF3 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION3 */
    };
} DX_M1A_NPU0_SFU_PAF34_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF35_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF35*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF4:32;  /* DX_M1A_NPU0.SFU_PAF35.PAF_ADD_COEFF4 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION4 */
    };
} DX_M1A_NPU0_SFU_PAF35_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF36_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF36*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF5:32;  /* DX_M1A_NPU0.SFU_PAF36.PAF_ADD_COEFF5 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION5 */
    };
} DX_M1A_NPU0_SFU_PAF36_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF37_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF37*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF6:32;  /* DX_M1A_NPU0.SFU_PAF37.PAF_ADD_COEFF6 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION6 */
    };
} DX_M1A_NPU0_SFU_PAF37_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF38_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF38*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF7:32;  /* DX_M1A_NPU0.SFU_PAF38.PAF_ADD_COEFF7 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION7 */
    };
} DX_M1A_NPU0_SFU_PAF38_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF39_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF39*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF8:32;  /* DX_M1A_NPU0.SFU_PAF39.PAF_ADD_COEFF8 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION8 */
    };
} DX_M1A_NPU0_SFU_PAF39_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF40_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF40*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF9:32;  /* DX_M1A_NPU0.SFU_PAF40.PAF_ADD_COEFF9 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION9 */
    };
} DX_M1A_NPU0_SFU_PAF40_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF41_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF41*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF10:32;  /* DX_M1A_NPU0.SFU_PAF41.PAF_ADD_COEFF10 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION10 */
    };
} DX_M1A_NPU0_SFU_PAF41_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF42_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF42*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF11:32;  /* DX_M1A_NPU0.SFU_PAF42.PAF_ADD_COEFF11 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION11 */
    };
} DX_M1A_NPU0_SFU_PAF42_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF43_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF43*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF12:32;  /* DX_M1A_NPU0.SFU_PAF43.PAF_ADD_COEFF12 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION12 */
    };
} DX_M1A_NPU0_SFU_PAF43_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF44_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF44*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF13:32;  /* DX_M1A_NPU0.SFU_PAF44.PAF_ADD_COEFF13 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION13 */
    };
} DX_M1A_NPU0_SFU_PAF44_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF45_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF45*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF14:32;  /* DX_M1A_NPU0.SFU_PAF45.PAF_ADD_COEFF14 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION14 */
    };
} DX_M1A_NPU0_SFU_PAF45_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF46_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF46*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_ADD_COEFF15:32;  /* DX_M1A_NPU0.SFU_PAF46.PAF_ADD_COEFF15 [31:0] (RW, 0x0) PAF PARAMETER, BIAS OF REGION15 */
    };
} DX_M1A_NPU0_SFU_PAF46_t;
typedef volatile union _DX_M1A_NPU0_SFU_SCALE_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_SCALE_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SCALE_OUTLIER_EN:1;  /* DX_M1A_NPU0.SFU_SCALE_CFG0.SCALE_OUTLIER_EN [0:0] (RW, 0x0) SCALE OUTLIER ENABLE */
        volatile uint32_t RESERVED0:31;  /* DX_M1A_NPU0.SFU_SCALE_CFG0.RESERVED0 [31:1] (R, 0x0)  */
    };
} DX_M1A_NPU0_SFU_SCALE_CFG0_t;
typedef volatile union _DX_M1A_NPU0_SFU_SCALE_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_SCALE_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SCALE_OUTLIER_BIG_BOUND:24;  /* DX_M1A_NPU0.SFU_SCALE_CFG1.SCALE_OUTLIER_BIG_BOUND [23:0] (RW, 0x0) BIGGER BOUND VALUE(FP24) */
        volatile uint32_t RESERVED0:8;  /* DX_M1A_NPU0.SFU_SCALE_CFG1.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_M1A_NPU0_SFU_SCALE_CFG1_t;
typedef volatile union _DX_M1A_NPU0_SFU_SCALE_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_SCALE_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SCALE_OUTLIER_SMALL_BOUND:24;  /* DX_M1A_NPU0.SFU_SCALE_CFG2.SCALE_OUTLIER_SMALL_BOUND [23:0] (RW, 0x0) SMALLER BOUND VALUE(FP24) */
        volatile uint32_t RESERVED0:8;  /* DX_M1A_NPU0.SFU_SCALE_CFG2.RESERVED0 [31:24] (R, 0x0)  */
    };
} DX_M1A_NPU0_SFU_SCALE_CFG2_t;
typedef volatile union _DX_M1A_NPU0_SFU_SCALE_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_SCALE_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t SCALE_OUTLIER_BIG_OUT_SCALE_EXP:8;  /* DX_M1A_NPU0.SFU_SCALE_CFG3.SCALE_OUTLIER_BIG_OUT_SCALE_EXP [7:0] (RW, 0x0) OUT SCALE EXPONENT VALUE */
        volatile uint32_t SCALE_OUTLIER_BIG_IN_SCALE_EXP:8;  /* DX_M1A_NPU0.SFU_SCALE_CFG3.SCALE_OUTLIER_BIG_IN_SCALE_EXP [15:8] (RW, 0x0) IN SCALE EXPONENT VALUE */
        volatile uint32_t SCALE_OUTLIER_SMALL_OUT_SCALE_EXP:8;  /* DX_M1A_NPU0.SFU_SCALE_CFG3.SCALE_OUTLIER_SMALL_OUT_SCALE_EXP [23:16] (RW, 0x0) OUT SCALE EXPONENT VALUE */
        volatile uint32_t SCALE_OUTLIER_SMALL_IN_SCALE_EXP:8;  /* DX_M1A_NPU0.SFU_SCALE_CFG3.SCALE_OUTLIER_SMALL_IN_SCALE_EXP [31:24] (RW, 0x0) IN SCALE EXPONENT VALUE */
    };
} DX_M1A_NPU0_SFU_SCALE_CFG3_t;
typedef volatile union _DX_M1A_NPU0_SFU_ELEM_BRD0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_ELEM_BRD0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t ELEM_BROADCAST0_CHANNEL_NUM:16;  /* DX_M1A_NPU0.SFU_ELEM_BRD0.ELEM_BROADCAST0_CHANNEL_NUM [15:0] (RW, 0x0) BROADCAST CHANNEL 개수 */
        volatile uint32_t ELEM_BROADCAST0_CHANNEL_SIZE:8;  /* DX_M1A_NPU0.SFU_ELEM_BRD0.ELEM_BROADCAST0_CHANNEL_SIZE [23:16] (RW, 0x0) BROADCAST CHANNEL WORD 개수 */
        volatile uint32_t ELEM_BROADCAST0_EN:1;  /* DX_M1A_NPU0.SFU_ELEM_BRD0.ELEM_BROADCAST0_EN [24:24] (RW, 0x0) BROADCAST ENABLE(1 CHANNEL → N CHANNEL) */
        volatile uint32_t RESERVED0:7;  /* DX_M1A_NPU0.SFU_ELEM_BRD0.RESERVED0 [31:25] (R, 0x0)  */
    };
} DX_M1A_NPU0_SFU_ELEM_BRD0_t;
typedef volatile union _DX_M1A_NPU0_SFU_PAF_DISABLE_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.SFU_PAF_DISABLE*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PAF_LINE_DISABLE:32;  /* DX_M1A_NPU0.SFU_PAF_DISABLE.PAF_LINE_DISABLE [31:0] (RW, 0x0) SFU PAF + SCALE_OUTILIER LINE DISABLE */
    };
} DX_M1A_NPU0_SFU_PAF_DISABLE_t;
typedef volatile union _DX_M1A_NPU0_PRE0_I2C0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE0_I2C0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:5;  /* DX_M1A_NPU0.PRE0_I2C0.RESERVED0 [4:0] (R, 0x0)  */
        volatile uint32_t I2C0_LINE_OFFSET:9;  /* DX_M1A_NPU0.PRE0_I2C0.I2C0_LINE_OFFSET [13:5] (RW, 0x0) 1 LINE DATA에 대한 ADDRESS 개수 */
        volatile uint32_t I2C0_LAST_VALID_BYTE:8;  /* DX_M1A_NPU0.PRE0_I2C0.I2C0_LAST_VALID_BYTE [21:14] (RW, 0x0) SRAM LAST WORD에 들어있는 IMAGE RAW DATA SIZE(BYTE) */
        volatile uint32_t I2C0_RQST_NUM:9;  /* DX_M1A_NPU0.PRE0_I2C0.I2C0_RQST_NUM [30:22] (RW, 0x0) IMAGE RAW DATA가 SRAM WORD에 FULL로 들어가있는 ADDRESS 개수 */
        volatile uint32_t I2C0_EN:1;  /* DX_M1A_NPU0.PRE0_I2C0.I2C0_EN [31:31] (RW, 0x0) 0: IMG2COL OFF, 1: IMG2COL ON */
    };
} DX_M1A_NPU0_PRE0_I2C0_t;
typedef volatile union _DX_M1A_NPU0_PRE0_I2C1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE0_I2C1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t I2C0_WRITE_FEATURE_BASE_ADDR_0:16;  /* DX_M1A_NPU0.PRE0_I2C1.I2C0_WRITE_FEATURE_BASE_ADDR_0 [15:0] (RW, 0x0) IMG2COL BASE ADDRESS */
        volatile uint32_t RESERVED0:16;  /* DX_M1A_NPU0.PRE0_I2C1.RESERVED0 [31:16] (R, 0x0)  */
    };
} DX_M1A_NPU0_PRE0_I2C1_t;
typedef volatile union _DX_M1A_NPU0_PRE0_I2C2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE0_I2C2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t I2C0_WRITE_FEATURE_BASE_ADDR_2:16;  /* DX_M1A_NPU0.PRE0_I2C2.I2C0_WRITE_FEATURE_BASE_ADDR_2 [15:0] (RW, 0x0) IMG2COL BASE ADDRESS의 129TH CHANNEL BASE ADDRESS */
        volatile uint32_t I2C0_WRITE_FEATURE_BASE_ADDR_1:16;  /* DX_M1A_NPU0.PRE0_I2C2.I2C0_WRITE_FEATURE_BASE_ADDR_1 [31:16] (RW, 0x0) IMG2COL BASE ADDRESS의 65TH CHANNEL BASE ADDRESS */
    };
} DX_M1A_NPU0_PRE0_I2C2_t;
typedef volatile union _DX_M1A_NPU0_PRE0_I2C3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE0_I2C3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t I2C0_HEIGHT_MAX:16;  /* DX_M1A_NPU0.PRE0_I2C3.I2C0_HEIGHT_MAX [15:0] (RW, 0x0) IMG2COL 이후 FEATURE HEIGHT */
        volatile uint32_t I2C0_WIDTH_MAX:16;  /* DX_M1A_NPU0.PRE0_I2C3.I2C0_WIDTH_MAX [31:16] (RW, 0x0) IMG2COL 이후 FEATURE WIDTH */
    };
} DX_M1A_NPU0_PRE0_I2C3_t;
typedef volatile union _DX_M1A_NPU0_PRE0_FMT0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE0_FMT0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:22;  /* DX_M1A_NPU0.PRE0_FMT0.RESERVED0 [21:0] (R, 0x0)  */
        volatile uint32_t FMT0_FIRST_LINE_OPT:2;  /* DX_M1A_NPU0.PRE0_FMT0.FMT0_FIRST_LINE_OPT [23:22] (RW, 0x0) RAW DATA의 첫번째 DATA TYPE(0: R, 1: G, 2: B) */
        volatile uint32_t FMT0_FIRST_VALID_NUM:6;  /* DX_M1A_NPU0.PRE0_FMT0.FMT0_FIRST_VALID_NUM [29:24] (RW, 0x0) RAW DATA에서 유용한 첫번째 R의 위치 */
        volatile uint32_t FMT0_CH_MODE:1;  /* DX_M1A_NPU0.PRE0_FMT0.FMT0_CH_MODE [30:30] (RW, 0x0) 0: 3CH, 1: 1CH */
        volatile uint32_t FMT0_ENABLE:1;  /* DX_M1A_NPU0.PRE0_FMT0.FMT0_ENABLE [31:31] (RW, 0x0) RTL NO USE */
    };
} DX_M1A_NPU0_PRE0_FMT0_t;
typedef volatile union _DX_M1A_NPU0_PRE0_FMT1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE0_FMT1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FMT0_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PRE0_FMT1.FMT0_READ_BASE_ADDR [31:0] (RW, 0x0) DATA READ BASE ADDRESS(SRAM) */
    };
} DX_M1A_NPU0_PRE0_FMT1_t;
typedef volatile union _DX_M1A_NPU0_PE0_ADDR_GEN0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_ADDR_GEN0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_DEFAULT_F_ADDR_GEN_MODE:1;  /* DX_M1A_NPU0.PE0_ADDR_GEN0.PE0_DEFAULT_F_ADDR_GEN_MODE [0:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:4;  /* DX_M1A_NPU0.PE0_ADDR_GEN0.RESERVED0 [4:1] (R, 0x0)  */
        volatile uint32_t PE0_RF_ADDR_GEN_MAX_NUM:27;  /* DX_M1A_NPU0.PE0_ADDR_GEN0.PE0_RF_ADDR_GEN_MAX_NUM [31:5] (RW, 0x0) SFU ↔ PE 처리 속도차이(32 VS 64)에 의해 주기(우측수식)로 RF ENABLE 발생 */
    };
} DX_M1A_NPU0_PE0_ADDR_GEN0_t;
typedef volatile union _DX_M1A_NPU0_PE0_ADDR_GEN1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_ADDR_GEN1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_ADDR_GEN1.PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR [31:0] (RW, 0x0) SFU ONLY USE에서만 사용하는 FEATURE MEMORY OFFSET(EX. FORMATTER) */
    };
} DX_M1A_NPU0_PE0_ADDR_GEN1_t;
typedef volatile union _DX_M1A_NPU0_PE0_ADDR_GEN2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_ADDR_GEN2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_DEFAULT_F_SIZE:16;  /* DX_M1A_NPU0.PE0_ADDR_GEN2.PE0_DEFAULT_F_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM:16;  /* DX_M1A_NPU0.PE0_ADDR_GEN2.PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_ADDR_GEN2_t;
typedef volatile union _DX_M1A_NPU0_PE0_ADDR_GEN3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_ADDR_GEN3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_DEFAULT_F_SURFACE_SIZE:16;  /* DX_M1A_NPU0.PE0_ADDR_GEN3.PE0_DEFAULT_F_SURFACE_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t PE0_DEFAULT_F_STRIDE:16;  /* DX_M1A_NPU0.PE0_ADDR_GEN3.PE0_DEFAULT_F_STRIDE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_ADDR_GEN3_t;
typedef volatile union _DX_M1A_NPU0_PE0_ADDR_GEN4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_ADDR_GEN4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_ELEM_ADDR_GEN_MODE:1;  /* DX_M1A_NPU0.PE0_ADDR_GEN4.PE0_ELEM_ADDR_GEN_MODE [0:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:15;  /* DX_M1A_NPU0.PE0_ADDR_GEN4.RESERVED0 [15:1] (R, 0x0)  */
        volatile uint32_t PE0_DEFAULT_F_SURFACE_STRIDE:16;  /* DX_M1A_NPU0.PE0_ADDR_GEN4.PE0_DEFAULT_F_SURFACE_STRIDE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_ADDR_GEN4_t;
typedef volatile union _DX_M1A_NPU0_PE0_ADDR_GEN5_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_ADDR_GEN5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_ELEM_STRIDE:16;  /* DX_M1A_NPU0.PE0_ADDR_GEN5.PE0_ELEM_STRIDE [15:0] (RW, 0x0)  */
        volatile uint32_t PE0_ELEM_SIZE:16;  /* DX_M1A_NPU0.PE0_ADDR_GEN5.PE0_ELEM_SIZE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_ADDR_GEN5_t;
typedef volatile union _DX_M1A_NPU0_PE0_ADDR_GEN6_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_ADDR_GEN6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_ELEM_SURFACE_STRIDE:16;  /* DX_M1A_NPU0.PE0_ADDR_GEN6.PE0_ELEM_SURFACE_STRIDE [15:0] (RW, 0x0)  */
        volatile uint32_t PE0_ELEM_SURFACE_SIZE:16;  /* DX_M1A_NPU0.PE0_ADDR_GEN6.PE0_ELEM_SURFACE_SIZE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_ADDR_GEN6_t;
typedef volatile union _DX_M1A_NPU0_PE0_ADDR_GEN7_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_ADDR_GEN7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_MUL_ADDR_GEN_MODE_OFFSET:16;  /* DX_M1A_NPU0.PE0_ADDR_GEN7.PE0_MUL_ADDR_GEN_MODE_OFFSET [15:0] (RW, 0x0) MODE == 1,2 에서의 ADDRESS OFFSET, RTL ONLY USE 8 BIT */
        volatile uint32_t PE0_MUL_ADDR_GEN_MODE:2;  /* DX_M1A_NPU0.PE0_ADDR_GEN7.PE0_MUL_ADDR_GEN_MODE [17:16] (RW, 0x0) SFU MUL COEFFICIENT의 READ OPTION 0: ADDRESS를 차례대로 읽음 1: 특정 구간을 반복해서 읽음 2: 특정 구간을 반복해서 읽음(DUMMY 존재) */
        volatile uint32_t PE0_MUL_ADDR_GEN_MODE2_MAX_NUM:14;  /* DX_M1A_NPU0.PE0_ADDR_GEN7.PE0_MUL_ADDR_GEN_MODE2_MAX_NUM [31:18] (RW, 0x0) MODE == 1,2 에서의 반복 주기, RTL ONLY USE 9 BIT */
    };
} DX_M1A_NPU0_PE0_ADDR_GEN7_t;
typedef volatile union _DX_M1A_NPU0_PE0_ADDR_GEN8_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_ADDR_GEN8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_ADD_ADDR_GEN_MODE_OFFSET:16;  /* DX_M1A_NPU0.PE0_ADDR_GEN8.PE0_ADD_ADDR_GEN_MODE_OFFSET [15:0] (RW, 0x0) MODE == 1,2 에서의 ADDRESS OFFSET, RTL ONLY USE 8 BIT */
        volatile uint32_t PE0_ADD_ADDR_GEN_MODE:2;  /* DX_M1A_NPU0.PE0_ADDR_GEN8.PE0_ADD_ADDR_GEN_MODE [17:16] (RW, 0x0) SFU MUL COEFFICIENT의 READ OPTION 0: ADDRESS를 차례대로 읽음 1: 특정 구간을 반복해서 읽음 2: 특정 구간을 반복해서 읽음(DUMMY 존재) */
        volatile uint32_t PE0_ADD_ADDR_GEN_MODE2_MAX_NUM:14;  /* DX_M1A_NPU0.PE0_ADDR_GEN8.PE0_ADD_ADDR_GEN_MODE2_MAX_NUM [31:18] (RW, 0x0) MODE == 1,2 에서의 반복 주기, RTL ONLY USE 9 BIT */
    };
} DX_M1A_NPU0_PE0_ADDR_GEN8_t;
typedef volatile union _DX_M1A_NPU0_PE0_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_CONV_TYPE:8;  /* DX_M1A_NPU0.PE0_CTRL.PE0_CONV_TYPE [7:0] (RW, 0x0) 0: STANDARD 1: DEPTHWISE 4: SFU ONLY(EX. DE-QUANTIZATION) 5: FORMATTER 6: BILINEAR 7: IMG2COL 8: TRANSPOSE */
        volatile uint32_t PE0_CONV_EN:1;  /* DX_M1A_NPU0.PE0_CTRL.PE0_CONV_EN [8:8] (RW, 0x0) 0: ETC 연산 1: CONV / GEMM / MATMUL / CONVTRANSPOSE */
        volatile uint32_t RESERVED0:7;  /* DX_M1A_NPU0.PE0_CTRL.RESERVED0 [15:9] (R, 0x0)  */
        volatile uint32_t PE0_SYSTOLIC_DEPTH:8;  /* DX_M1A_NPU0.PE0_CTRL.PE0_SYSTOLIC_DEPTH [23:16] (RW, 0x0) SYSTOLIC ARRAY 中 사용하는 ROW 개수, RTL ONLY USE 6 BIT */
        volatile uint32_t PE0_SYSTOLIC_EN:1;  /* DX_M1A_NPU0.PE0_CTRL.PE0_SYSTOLIC_EN [24:24] (RW, 0x0) 0: ARRAY NO USE, 1: ARRAY USE */
        volatile uint32_t PE0_SFUIN_VALID_NUM:7;  /* DX_M1A_NPU0.PE0_CTRL.PE0_SFUIN_VALID_NUM [31:25] (RW, 0x0) SFU 입력 64EA 中 유효한 입력 DATA */
    };
} DX_M1A_NPU0_PE0_CTRL_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_LAST_CONV_NUM:26;  /* DX_M1A_NPU0.PE0_CFG0.PE0_LAST_CONV_NUM [25:0] (RW, 0x0) PE ARRAY ITERATION NO. */
        volatile uint32_t PE0_LAST_CONV_MOD:6;  /* DX_M1A_NPU0.PE0_CFG0.PE0_LAST_CONV_MOD [31:26] (RW, 0x0) PE ARRAY 마지막 ITERATION에서 사용하는 PE ROW 개수 */
    };
} DX_M1A_NPU0_PE0_CFG0_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_OFEATURE_CHANNEL:16;  /* DX_M1A_NPU0.PE0_CFG1.PE0_OFEATURE_CHANNEL [15:0] (RW, 0x0) OUPTUT FEATURE CHANNEL, RTL ONLY USE 15BIT(MAX 25088) */
        volatile uint32_t PE0_OFEATURE_NUMBER:16;  /* DX_M1A_NPU0.PE0_CFG1.PE0_OFEATURE_NUMBER [31:16] (RW, 0x0) OUTPUT FEATURE NUMBER, RTL NO USE, ONLY DEBUG */
    };
} DX_M1A_NPU0_PE0_CFG1_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_OFEATURE_WIDTH:16;  /* DX_M1A_NPU0.PE0_CFG2.PE0_OFEATURE_WIDTH [15:0] (RW, 0x0) OUPTUT FEATURE WIDTH, RTL ONLY USE 13BIT(MAX 7680) */
        volatile uint32_t PE0_OFEATURE_HEIGHT:16;  /* DX_M1A_NPU0.PE0_CFG2.PE0_OFEATURE_HEIGHT [31:16] (RW, 0x0) OUPTUT FEATURE HEIGHT, RTL ONLY USE 13BIT(MAX 4320) */
    };
} DX_M1A_NPU0_PE0_CFG2_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_OFEATURE_SIZE:32;  /* DX_M1A_NPU0.PE0_CFG3.PE0_OFEATURE_SIZE [31:0] (RW, 0x0) RTL ONLY USE 26BIT(MAX 7680*4320) */
    };
} DX_M1A_NPU0_PE0_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_FEATURE_HEIGHT:16;  /* DX_M1A_NPU0.PE0_CFG4.PE0_FEATURE_HEIGHT [15:0] (RW, 0x0) INPUT FEATURE HEIGHT, RTL ONLY USE 13BIT(MAX 4320) */
        volatile uint32_t PE0_FEATURE_CHANNEL:16;  /* DX_M1A_NPU0.PE0_CFG4.PE0_FEATURE_CHANNEL [31:16] (RW, 0x0) INPUT FEATURE CHANNEL, RTL ONLY USE 15BIT(MAX 25088) */
    };
} DX_M1A_NPU0_PE0_CFG4_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG5_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_FILTER_NUMBER:16;  /* DX_M1A_NPU0.PE0_CFG5.PE0_FILTER_NUMBER [15:0] (RW, 0x0) FILTER NUMBER, RTL ONLY USE 15BIT(MAX 25088), NO USE */
        volatile uint32_t PE0_FEATURE_WIDTH:16;  /* DX_M1A_NPU0.PE0_CFG5.PE0_FEATURE_WIDTH [31:16] (RW, 0x0) INPUT FEATURE WIDTH, RTL ONLY USE 13BIT(MAX 7680) */
    };
} DX_M1A_NPU0_PE0_CFG5_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG6_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_PAD_MODE:1;  /* DX_M1A_NPU0.PE0_CFG6.PE0_PAD_MODE [0:0] (RW, 0x0) 0: CONSTANT PAD, 1: REPLICATION PAD */
        volatile uint32_t RESERVED0:5;  /* DX_M1A_NPU0.PE0_CFG6.RESERVED0 [5:1] (R, 0x0)  */
        volatile uint32_t PE0_FILTER_WIDTH:5;  /* DX_M1A_NPU0.PE0_CFG6.PE0_FILTER_WIDTH [10:6] (RW, 0x0) FILTER WIDTH, ONLY POOLING BLOCK USE 5BIT, OTHER BLOCK USE 4BIT */
        volatile uint32_t PE0_FILTER_HEIGHT:5;  /* DX_M1A_NPU0.PE0_CFG6.PE0_FILTER_HEIGHT [15:11] (RW, 0x0) FILTER HEIGHT, ONLY POOLING BLOCK USE 5BIT, OTHER BLOCK USE 4BIT */
        volatile uint32_t PE0_FILTER_CHANNEL:16;  /* DX_M1A_NPU0.PE0_CFG6.PE0_FILTER_CHANNEL [31:16] (RW, 0x0) FILTER CHANNEL, RTL ONLY USE 15BIT(MAX 25088) */
    };
} DX_M1A_NPU0_PE0_CFG6_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG7_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_PAD_TOP_SIZE:6;  /* DX_M1A_NPU0.PE0_CFG7.PE0_PAD_TOP_SIZE [5:0] (RW, 0x0) PAD TOP */
        volatile uint32_t PE0_PAD_BOTTOM_SIZE:6;  /* DX_M1A_NPU0.PE0_CFG7.PE0_PAD_BOTTOM_SIZE [11:6] (RW, 0x0) PAD BOTTOM */
        volatile uint32_t PE0_PAD_LEFT_SIZE:6;  /* DX_M1A_NPU0.PE0_CFG7.PE0_PAD_LEFT_SIZE [17:12] (RW, 0x0) PAD LEFT */
        volatile uint32_t PE0_PAD_RIGHT_SIZE:6;  /* DX_M1A_NPU0.PE0_CFG7.PE0_PAD_RIGHT_SIZE [23:18] (RW, 0x0) PAD RIGHT */
        volatile uint32_t PE0_PAD_VALUE:8;  /* DX_M1A_NPU0.PE0_CFG7.PE0_PAD_VALUE [31:24] (RW, 0x0) CONSTATNT PAD VALUE */
    };
} DX_M1A_NPU0_PE0_CFG7_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG8_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_STRIDE_X:5;  /* DX_M1A_NPU0.PE0_CFG8.PE0_STRIDE_X [4:0] (RW, 0x0) ONLY POOLING BLOCK USE 5BIT, OTHER BLOCK USE 4BIT */
        volatile uint32_t PE0_STRIDE_Y:5;  /* DX_M1A_NPU0.PE0_CFG8.PE0_STRIDE_Y [9:5] (RW, 0x0) ONLY POOLING BLOCK USE 5BIT, OTHER BLOCK USE 4BIT */
        volatile uint32_t RESERVED0:6;  /* DX_M1A_NPU0.PE0_CFG8.RESERVED0 [15:10] (R, 0x0)  */
        volatile uint32_t PE0_CHANNEL_OFFSET:16;  /* DX_M1A_NPU0.PE0_CFG8.PE0_CHANNEL_OFFSET [31:16] (RW, 0x0) IN FEATURE READ STRIDE(PE에서 READ 할 때 사용하는 STRIDE) 이전 TILE에서 STRIDE를 주어 SRAM에 저장하거나, TILE 경계부의 중복된 DATA로 인한  DATA 연속성이 어긋났을때 사용(HALO EFFECT) */
    };
} DX_M1A_NPU0_PE0_CFG8_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG9_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_ELEM_CHANNEL_OFFSET:16;  /* DX_M1A_NPU0.PE0_CFG9.PE0_ELEM_CHANNEL_OFFSET [15:0] (RW, 0x0) SKIP DATA READ STRIDE(PE에서 READ 할 때 사용하는 STRIDE) 이전 TILE에서 STRIDE를 주어 SRAM에 저장하거나, TILE 경계부의 중복된 DATA로 인한  DATA 연속성이 어긋났을때 사용(HALO) */
        volatile uint32_t PE0_DILATION_X:6;  /* DX_M1A_NPU0.PE0_CFG9.PE0_DILATION_X [21:16] (RW, 0x0) DILATION X */
        volatile uint32_t PE0_DILATION_Y:6;  /* DX_M1A_NPU0.PE0_CFG9.PE0_DILATION_Y [27:22] (RW, 0x0) DILATION Y */
        volatile uint32_t RESERVED0:4;  /* DX_M1A_NPU0.PE0_CFG9.RESERVED0 [31:28] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE0_CFG9_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG10_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG10*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_READ_FEATURE_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_CFG10.PE0_READ_FEATURE_BASE_ADDR [31:0] (RW, 0x0) INPUT FEATURE MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_CFG10_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG11_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG11*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WRITE_FEATURE_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_CFG11.PE0_WRITE_FEATURE_BASE_ADDR [31:0] (RW, 0x0) OUTPUT FEATURE MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_CFG11_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG12_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG12*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_READ_WEIGHT_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_CFG12.PE0_READ_WEIGHT_BASE_ADDR [31:0] (RW, 0x0) WEIGHT MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_CFG12_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG13_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG13*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_ELEM_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_CFG13.PE0_ELEM_READ_BASE_ADDR [31:0] (RW, 0x0) SKIP DATA MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_CFG13_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG14_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG14*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_MUL_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_CFG14.PE0_MUL_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0) SFU MUL COEFFICIENT MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_CFG14_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG15_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG15*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_ADD_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_CFG15.PE0_ADD_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0) SFU ADD COEFFICIENT MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_CFG15_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG16_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG16*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_CFG16.PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0) SFU SKIP MUL COEFFICIENT MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_CFG16_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG17_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG17*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_CFG17.PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0) SFU SKIP ADD COEFFICIENT MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_CFG17_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG18_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG18*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_SE_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_CFG18.PE0_SE_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0) SFU SE(SQUEEZE 결과) COEFFICIENT MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_CFG18_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG19_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG19*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_CFG19.PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0) SFU GENERAL(PRELU) COEFFICIENT MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_CFG19_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG20_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG20*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_ADDR_MODE1_LINE_SIZE:16;  /* DX_M1A_NPU0.PE0_CFG20.PE0_WF_ADDR_MODE1_LINE_SIZE [15:0] (RW, 0x0) WR_ADDR_GEN_MODE == 1에서 LINE DATA ADDRESS 개수 */
        volatile uint32_t RESERVED0:13;  /* DX_M1A_NPU0.PE0_CFG20.RESERVED0 [28:16] (R, 0x0)  */
        volatile uint32_t PE0_WF_ADDR_GEN_MODE:3;  /* DX_M1A_NPU0.PE0_CFG20.PE0_WF_ADDR_GEN_MODE [31:29] (RW, 0x0) 0: 일반적인 FEATURE WRITE 1: FEATURE REUSE를 위해 다른 THREAD 영역을 띄워놓는 용도 2: OUTPUT DATA CHANNEL 방향으로 저장하기 위한 용도 3: TRANSPOSE 7: BYPASS */
    };
} DX_M1A_NPU0_PE0_CFG20_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG21_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG21*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_ADDR_MODE1_LINE_STRIDE:32;  /* DX_M1A_NPU0.PE0_CFG21.PE0_WF_ADDR_MODE1_LINE_STRIDE [31:0] (RW, 0x0) WR_ADDR_GEN_MODE == 1에서 LITE (DATA+DUMMY) ADDRESS 개수, RTL ONLY USE 16BIT */
    };
} DX_M1A_NPU0_PE0_CFG21_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG22_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG22*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_ADDR_SURFACE_SIZE:32;  /* DX_M1A_NPU0.PE0_CFG22.PE0_WF_ADDR_SURFACE_SIZE [31:0] (RW, 0x0) OUTPUT FEATURE의 64 CHANNEL 단위위면적의 ADDRESS 개수(TILING 후 기준), RTL ONLY USE 16BIT */
    };
} DX_M1A_NPU0_PE0_CFG22_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG23_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG23*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_WRITE_CNT:16;  /* DX_M1A_NPU0.PE0_CFG23.PE0_WF_WRITE_CNT [15:0] (RW, 0x0) OPERATION FINISH를 띄우기 위한 ADDRESS TOTAL 개수 */
        volatile uint32_t PE0_WF_ADDR_SURFACE_STRIDE:16;  /* DX_M1A_NPU0.PE0_CFG23.PE0_WF_ADDR_SURFACE_STRIDE [31:16] (RW, 0x0) OUTPUT FEATURE의 저장공간 확보 */
    };
} DX_M1A_NPU0_PE0_CFG23_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG24_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG24*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_WF_SRC1_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_CFG24.PE0_WF_SRC1_BASE_ADDR [31:0] (RW, 0x0) ON THE FLY 연산 時, SFU에서 나오는 2번째 OPERATION 결과 WRITE BASE ADDRESS */
    };
} DX_M1A_NPU0_PE0_CFG24_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG25_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG25*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_SFU_OUT_DATA_VALID_NUM:3;  /* DX_M1A_NPU0.PE0_CFG25.PE0_SFU_OUT_DATA_VALID_NUM [2:0] (RW, 0x0) ONLY HWC USE, RTL NO USE */
        volatile uint32_t RESERVED1:5;  /* DX_M1A_NPU0.PE0_CFG25.RESERVED1 [7:3] (R, 0x0)  */
        volatile uint32_t PE0_ST_ADDR_GEN_SHIFT_LEN:5;  /* DX_M1A_NPU0.PE0_CFG25.PE0_ST_ADDR_GEN_SHIFT_LEN [12:8] (RW, 0x0) ST_ADDR_GEN_MUL 이후 RIGHT SHIFT NUMBER, RTL NO USE */
        volatile uint32_t RESERVED0:19;  /* DX_M1A_NPU0.PE0_CFG25.RESERVED0 [31:13] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE0_CFG25_t;
typedef volatile union _DX_M1A_NPU0_PE0_CFG26_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_CFG26*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_ST_ADDR_GEN_MUL:31;  /* DX_M1A_NPU0.PE0_CFG26.PE0_ST_ADDR_GEN_MUL [30:0] (RW, 0x0) 1 / OUT FEATURE WIDTH 을 계산하기 위해 230 /  MUL COEFFICIENT 을 변환한 숫자 이후에 ST_ADDR_GEN_SHFT_LEN으로 RIGHT SHIFT하여 계산 */
        volatile uint32_t RESERVED0:1;  /* DX_M1A_NPU0.PE0_CFG26.RESERVED0 [31:31] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE0_CFG26_t;
typedef volatile union _DX_M1A_NPU0_PE0_VMEM_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_VMEM_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE0_START_NO_IN_BUFF:6;  /* DX_M1A_NPU0.PE0_VMEM_CFG0.PE0_START_NO_IN_BUFF [5:0] (RW, 0x0) IN FEATURE DATA SRAM NO. */
        volatile uint32_t RESERVED3:2;  /* DX_M1A_NPU0.PE0_VMEM_CFG0.RESERVED3 [7:6] (R, 0x0)  */
        volatile uint32_t PE0_START_NO_WEIGHT_BUFF:6;  /* DX_M1A_NPU0.PE0_VMEM_CFG0.PE0_START_NO_WEIGHT_BUFF [13:8] (RW, 0x0) WEIGHT DATA SRAM NO. */
        volatile uint32_t RESERVED2:2;  /* DX_M1A_NPU0.PE0_VMEM_CFG0.RESERVED2 [15:14] (R, 0x0)  */
        volatile uint32_t PE0_START_NO_OUT_BUFF:6;  /* DX_M1A_NPU0.PE0_VMEM_CFG0.PE0_START_NO_OUT_BUFF [21:16] (RW, 0x0) OUT FEATURE DATA SRAM NO. */
        volatile uint32_t RESERVED1:2;  /* DX_M1A_NPU0.PE0_VMEM_CFG0.RESERVED1 [23:22] (R, 0x0)  */
        volatile uint32_t PE0_START_NO_ELEM_BUFF:6;  /* DX_M1A_NPU0.PE0_VMEM_CFG0.PE0_START_NO_ELEM_BUFF [29:24] (RW, 0x0) SKIP DATA SRAM NO. */
        volatile uint32_t RESERVED0:2;  /* DX_M1A_NPU0.PE0_VMEM_CFG0.RESERVED0 [31:30] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE0_VMEM_CFG0_t;
typedef volatile union _DX_M1A_NPU0_REDUCE0_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.REDUCE0_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t REDUCE0_CHANNEL_SIZE:8;  /* DX_M1A_NPU0.REDUCE0_CFG0.REDUCE0_CHANNEL_SIZE [7:0] (RW, 0x0) CEIL(CHANNEL / 16) */
        volatile uint32_t REDUCE0_CHANNEL_NUM:16;  /* DX_M1A_NPU0.REDUCE0_CFG0.REDUCE0_CHANNEL_NUM [23:8] (RW, 0x0) INPUT CHANNEL NUMBER */
        volatile uint32_t REDUCE0_MODE:2;  /* DX_M1A_NPU0.REDUCE0_CFG0.REDUCE0_MODE [25:24] (RW, 0x0) 0: SUM 1: MAX 2: RESERVED */
        volatile uint32_t REDUCE0_EN:1;  /* DX_M1A_NPU0.REDUCE0_CFG0.REDUCE0_EN [26:26] (RW, 0x0) REDUCE ENABLE */
        volatile uint32_t RESERVED0:5;  /* DX_M1A_NPU0.REDUCE0_CFG0.RESERVED0 [31:27] (R, 0x0)  */
    };
} DX_M1A_NPU0_REDUCE0_CFG0_t;
typedef volatile union _DX_M1A_NPU0_REDUCE0_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.REDUCE0_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t REDUCE0_WIN_NUM:16;  /* DX_M1A_NPU0.REDUCE0_CFG1.REDUCE0_WIN_NUM [15:0] (RW, 0x0) FEATURE WIDTH * FEATURE HEIGHT */
        volatile uint32_t REDUCE0_CNT_MAX_NUM:16;  /* DX_M1A_NPU0.REDUCE0_CFG1.REDUCE0_CNT_MAX_NUM [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_REDUCE0_CFG1_t;
typedef volatile union _DX_M1A_NPU0_REDUCE0_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.REDUCE0_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t REDUCE0_RD_BASE_ADDR:32;  /* DX_M1A_NPU0.REDUCE0_CFG2.REDUCE0_RD_BASE_ADDR [31:0] (RW, 0x0) READ BASE ADDRESS */
    };
} DX_M1A_NPU0_REDUCE0_CFG2_t;
typedef volatile union _DX_M1A_NPU0_REDUCE0_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.REDUCE0_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t REDUCE0_WR_BASE_ADDR:32;  /* DX_M1A_NPU0.REDUCE0_CFG3.REDUCE0_WR_BASE_ADDR [31:0] (RW, 0x0) WRITE BASE ADDRESS */
    };
} DX_M1A_NPU0_REDUCE0_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_START_OP_EN:1;  /* DX_M1A_NPU0.PE0_DMA_CTRL.VMEM0_START_OP_EN [0:0] (RW, 0x0) 해당 TILE의 OPERATION 존재 여부 0: OPERATION 없음 1: OPERATION 존재 */
        volatile uint32_t VMEM0_DMA_CTRL_MODE:2;  /* DX_M1A_NPU0.PE0_DMA_CTRL.VMEM0_DMA_CTRL_MODE [2:1] (RW, 0x0) OPERATION FINISH 생성 시점 CONTROL OPTION 0: READ DATA 이후 1: WRITE DATA 이후 2: OPERATION 이후 */
        volatile uint32_t VMEM0_WEIGHT_BIT_WIDTH:8;  /* DX_M1A_NPU0.PE0_DMA_CTRL.VMEM0_WEIGHT_BIT_WIDTH [10:3] (RW, 0x0) UNPACKER VARIABLE BIT OPTION */
        volatile uint32_t VMEM0_WEIGHT_DUMMY_MODE:2;  /* DX_M1A_NPU0.PE0_DMA_CTRL.VMEM0_WEIGHT_DUMMY_MODE [12:11] (RW, 0x0) 0: NO OPERATION(64), 1: 16, 2: 32, 3: 48 */
        volatile uint32_t RESERVED0:19;  /* DX_M1A_NPU0.PE0_DMA_CTRL.RESERVED0 [31:13] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_CTRL_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_ELEM_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_RD_ELEM_SIZE:16;  /* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.VMEM0_RD_ELEM_SIZE [15:0] (RW, 0x0) DMA로 읽는 단위 TENSOR의 SIZE */
        volatile uint32_t VMEM0_RD_ELEM_TIME:6;  /* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.VMEM0_RD_ELEM_TIME [21:16] (RW, 0x0) DMA로 읽는 단위 TENSOR 의 개수 */
        volatile uint32_t VMEM0_RD_ELEM_MODE:2;  /* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.VMEM0_RD_ELEM_MODE [23:22] (RW, 0x0) SKIP DATA READ OPTION 0: DMA → MEM 1: MEM → DMA → MEM (COPY) 2: DMA → MEM X2 */
        volatile uint32_t VMEM0_RD_ELEM_EN:1;  /* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.VMEM0_RD_ELEM_EN [24:24] (RW, 0x0)  */
        volatile uint32_t VMEM0_RD_ELEM_SRC:1;  /* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.VMEM0_RD_ELEM_SRC [25:25] (RW, 0x0) 0: USE RAW IMG(IN_FEATURE_BASE), 1: USE TENSOR DATA(OUT_FEATURE_BASE) */
        volatile uint32_t RESERVED0:6;  /* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.RESERVED0 [31:26] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_ELEM_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_RD_ELEM_STRIDE:32;  /* DX_M1A_NPU0.PE0_DMA_ELEM_CFG1.VMEM0_RD_ELEM_STRIDE [31:0] (RW, 0x0) DMA로 읽는 단위 TENSOR간 BASE ADDRESS 차이 */
    };
} DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_ELEM_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_RD_ELEM_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_ELEM_CFG2.VMEM0_RD_ELEM_BASE_ADDR [31:0] (RW, 0x0) SKIP DATA READ MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_ELEM_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_WT_ELEM_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_ELEM_CFG3.VMEM0_WT_ELEM_BASE_ADDR [31:0] (RW, 0x0) SKIP DATA WRITE MEMORY OFFSET */
    };
} DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_W_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_W_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_RD_W_TIME:12;  /* DX_M1A_NPU0.PE0_DMA_W_CFG0.VMEM0_RD_W_TIME [11:0] (RW, 0x0)  */
        volatile uint32_t VMEM0_RD_W_MODE:2;  /* DX_M1A_NPU0.PE0_DMA_W_CFG0.VMEM0_RD_W_MODE [13:12] (RW, 0x0)  */
        volatile uint32_t VMEM0_RD_W_EN:1;  /* DX_M1A_NPU0.PE0_DMA_W_CFG0.VMEM0_RD_W_EN [14:14] (RW, 0x0)  */
        volatile uint32_t RESERVED0:17;  /* DX_M1A_NPU0.PE0_DMA_W_CFG0.RESERVED0 [31:15] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_W_CFG0_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_W_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_W_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_RD_W_STRIDE:16;  /* DX_M1A_NPU0.PE0_DMA_W_CFG1.VMEM0_RD_W_STRIDE [15:0] (RW, 0x0)  */
        volatile uint32_t VMEM0_RD_W_SIZE:16;  /* DX_M1A_NPU0.PE0_DMA_W_CFG1.VMEM0_RD_W_SIZE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_W_CFG1_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_W_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_W_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_RD_W_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_W_CFG2.VMEM0_RD_W_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_W_CFG2_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_W_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_W_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_WT_W_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_W_CFG3.VMEM0_WT_W_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_W_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_INF_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_INF_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_RD_INF_SIZE:16;  /* DX_M1A_NPU0.PE0_DMA_INF_CFG0.VMEM0_RD_INF_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t VMEM0_RD_INF_TIME:12;  /* DX_M1A_NPU0.PE0_DMA_INF_CFG0.VMEM0_RD_INF_TIME [27:16] (RW, 0x0)  */
        volatile uint32_t VMEM0_RD_INF_MODE:2;  /* DX_M1A_NPU0.PE0_DMA_INF_CFG0.VMEM0_RD_INF_MODE [29:28] (RW, 0x0)  */
        volatile uint32_t VMEM0_RD_INF_EN:1;  /* DX_M1A_NPU0.PE0_DMA_INF_CFG0.VMEM0_RD_INF_EN [30:30] (RW, 0x0)  */
        volatile uint32_t VMEM0_RD_INF_SRC:1;  /* DX_M1A_NPU0.PE0_DMA_INF_CFG0.VMEM0_RD_INF_SRC [31:31] (RW, 0x0) 0: USE RAW IMG(IN_FEATURE_BASE), 1: USE TENSOR DATA(OUT_FEATURE_BASE) */
    };
} DX_M1A_NPU0_PE0_DMA_INF_CFG0_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_INF_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_INF_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_RD_INF_STRIDE:32;  /* DX_M1A_NPU0.PE0_DMA_INF_CFG1.VMEM0_RD_INF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_INF_CFG1_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_INF_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_INF_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_RD_INF_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_INF_CFG2.VMEM0_RD_INF_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_INF_CFG2_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_INF_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_INF_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_WT_INF_STRIDE:32;  /* DX_M1A_NPU0.PE0_DMA_INF_CFG3.VMEM0_WT_INF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_INF_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_INF_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_INF_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_WT_INF_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_INF_CFG4.VMEM0_WT_INF_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_INF_CFG4_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_OUTF_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_WT_OUTF_SIZE:16;  /* DX_M1A_NPU0.PE0_DMA_OUTF_CFG0.VMEM0_WT_OUTF_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t VMEM0_WT_OUTF_TIME:12;  /* DX_M1A_NPU0.PE0_DMA_OUTF_CFG0.VMEM0_WT_OUTF_TIME [27:16] (RW, 0x0)  */
        volatile uint32_t RESERVED1:2;  /* DX_M1A_NPU0.PE0_DMA_OUTF_CFG0.RESERVED1 [29:28] (R, 0x0)  */
        volatile uint32_t VMEM0_WT_OUTF_EN:1;  /* DX_M1A_NPU0.PE0_DMA_OUTF_CFG0.VMEM0_WT_OUTF_EN [30:30] (RW, 0x0)  */
        volatile uint32_t RESERVED0:1;  /* DX_M1A_NPU0.PE0_DMA_OUTF_CFG0.RESERVED0 [31:31] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_OUTF_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_WT_OUTF_STRIDE:32;  /* DX_M1A_NPU0.PE0_DMA_OUTF_CFG1.VMEM0_WT_OUTF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_OUTF_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_WT_OUTF_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_OUTF_CFG2.VMEM0_WT_OUTF_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_OUTF_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_RD_OUTF_STRIDE:32;  /* DX_M1A_NPU0.PE0_DMA_OUTF_CFG3.VMEM0_RD_OUTF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_OUTF_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM0_RD_OUTF_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_OUTF_CFG4.VMEM0_RD_OUTF_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_ADD_SIZE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_ADD_SIZE [11:0] (RW, 0x0)  */
        volatile uint32_t RF_RD_ADD_MODE:2;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_ADD_MODE [13:12] (RW, 0x0)  */
        volatile uint32_t RF_RD_ADD_EN:1;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_ADD_EN [14:14] (RW, 0x0)  */
        volatile uint32_t RF_RD_MUL_SIZE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_MUL_SIZE [26:15] (RW, 0x0)  */
        volatile uint32_t RF_RD_MUL_MODE:2;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_MUL_MODE [28:27] (RW, 0x0)  */
        volatile uint32_t RF_RD_MUL_EN:1;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_MUL_EN [29:29] (RW, 0x0)  */
        volatile uint32_t RESERVED0:2;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RESERVED0 [31:30] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG0_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_ADD_STRIDE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG1.RF_RD_ADD_STRIDE [11:0] (RW, 0x0)  */
        volatile uint32_t RF_RD_MUL_STRIDE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG1.RF_RD_MUL_STRIDE [23:12] (RW, 0x0)  */
        volatile uint32_t RF_RD_ADD_TIME:4;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG1.RF_RD_ADD_TIME [27:24] (RW, 0x0)  */
        volatile uint32_t RF_RD_MUL_TIME:4;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG1.RF_RD_MUL_TIME [31:28] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG1_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_MUL_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG2.RF_RD_MUL_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG2_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_WT_MUL_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG3.RF_WT_MUL_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_ADD_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG4.RF_RD_ADD_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG4_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG5_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_WT_ADD_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG5.RF_WT_ADD_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG5_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG6_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_SKIP_ADD_SIZE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_ADD_SIZE [11:0] (RW, 0x0)  */
        volatile uint32_t RF_RD_SKIP_ADD_MODE:2;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_ADD_MODE [13:12] (RW, 0x0)  */
        volatile uint32_t RF_RD_SKIP_ADD_EN:1;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_ADD_EN [14:14] (RW, 0x0)  */
        volatile uint32_t RF_RD_SKIP_MUL_SIZE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_MUL_SIZE [26:15] (RW, 0x0)  */
        volatile uint32_t RF_RD_SKIP_MUL_MODE:2;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_MUL_MODE [28:27] (RW, 0x0)  */
        volatile uint32_t RF_RD_SKIP_MUL_EN:1;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_MUL_EN [29:29] (RW, 0x0)  */
        volatile uint32_t RESERVED0:2;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RESERVED0 [31:30] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG6_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG7_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_SKIP_ADD_STRIDE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG7.RF_RD_SKIP_ADD_STRIDE [11:0] (RW, 0x0)  */
        volatile uint32_t RF_RD_SKIP_MUL_STRIDE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG7.RF_RD_SKIP_MUL_STRIDE [23:12] (RW, 0x0)  */
        volatile uint32_t RF_RD_SKIP_ADD_TIME:4;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG7.RF_RD_SKIP_ADD_TIME [27:24] (RW, 0x0)  */
        volatile uint32_t RF_RD_SKIP_MUL_TIME:4;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG7.RF_RD_SKIP_MUL_TIME [31:28] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG7_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG8_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_SKIP_MUL_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG8.RF_RD_SKIP_MUL_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG8_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG9_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_WT_SKIP_MUL_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG9.RF_WT_SKIP_MUL_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG9_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG10_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG10*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_SKIP_ADD_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG10.RF_RD_SKIP_ADD_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG10_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG11_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG11*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_WT_SKIP_ADD_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG11.RF_WT_SKIP_ADD_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG11_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG12_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG12*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_GENERAL_SIZE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_GENERAL_SIZE [11:0] (RW, 0x0)  */
        volatile uint32_t RF_RD_GENERAL_MODE:2;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_GENERAL_MODE [13:12] (RW, 0x0)  */
        volatile uint32_t RF_RD_GENERAL_EN:1;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_GENERAL_EN [14:14] (RW, 0x0)  */
        volatile uint32_t RF_RD_SE_SIZE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_SE_SIZE [26:15] (RW, 0x0)  */
        volatile uint32_t RF_RD_SE_MODE:2;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_SE_MODE [28:27] (RW, 0x0)  */
        volatile uint32_t RF_RD_SE_EN:1;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_SE_EN [29:29] (RW, 0x0)  */
        volatile uint32_t RESERVED0:2;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RESERVED0 [31:30] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG12_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG13_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG13*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_GENERAL_STRIDE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG13.RF_RD_GENERAL_STRIDE [11:0] (RW, 0x0)  */
        volatile uint32_t RF_RD_SE_STRIDE:12;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG13.RF_RD_SE_STRIDE [23:12] (RW, 0x0)  */
        volatile uint32_t RF_RD_GENERAL_TIME:4;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG13.RF_RD_GENERAL_TIME [27:24] (RW, 0x0)  */
        volatile uint32_t RF_RD_SE_TIME:4;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG13.RF_RD_SE_TIME [31:28] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG13_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG14_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG14*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_SE_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG14.RF_RD_SE_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG14_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG15_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG15*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_WT_SE_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG15.RF_WT_SE_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG15_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG16_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG16*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_RD_GENERAL_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG16.RF_RD_GENERAL_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG16_t;
typedef volatile union _DX_M1A_NPU0_PE0_DMA_RF_CFG17_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE0_DMA_RF_CFG17*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RF_WT_GENERAL_BASE_ADDR:32;  /* DX_M1A_NPU0.PE0_DMA_RF_CFG17.RF_WT_GENERAL_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE0_DMA_RF_CFG17_t;
typedef volatile union _DX_M1A_NPU0_PRE1_I2C0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE1_I2C0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:5;  /* DX_M1A_NPU0.PRE1_I2C0.RESERVED0 [4:0] (R, 0x0)  */
        volatile uint32_t I2C1_LINE_OFFSET:9;  /* DX_M1A_NPU0.PRE1_I2C0.I2C1_LINE_OFFSET [13:5] (RW, 0x0)  */
        volatile uint32_t I2C1_LAST_VALID_BYTE:8;  /* DX_M1A_NPU0.PRE1_I2C0.I2C1_LAST_VALID_BYTE [21:14] (RW, 0x0)  */
        volatile uint32_t I2C1_RQST_NUM:9;  /* DX_M1A_NPU0.PRE1_I2C0.I2C1_RQST_NUM [30:22] (RW, 0x0)  */
        volatile uint32_t I2C1_EN:1;  /* DX_M1A_NPU0.PRE1_I2C0.I2C1_EN [31:31] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PRE1_I2C0_t;
typedef volatile union _DX_M1A_NPU0_PRE1_I2C1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE1_I2C1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t I2C1_WRITE_FEATURE_BASE_ADDR_0:16;  /* DX_M1A_NPU0.PRE1_I2C1.I2C1_WRITE_FEATURE_BASE_ADDR_0 [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:16;  /* DX_M1A_NPU0.PRE1_I2C1.RESERVED0 [31:16] (R, 0x0)  */
    };
} DX_M1A_NPU0_PRE1_I2C1_t;
typedef volatile union _DX_M1A_NPU0_PRE1_I2C2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE1_I2C2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t I2C1_WRITE_FEATURE_BASE_ADDR_2:16;  /* DX_M1A_NPU0.PRE1_I2C2.I2C1_WRITE_FEATURE_BASE_ADDR_2 [15:0] (RW, 0x0)  */
        volatile uint32_t I2C1_WRITE_FEATURE_BASE_ADDR_1:16;  /* DX_M1A_NPU0.PRE1_I2C2.I2C1_WRITE_FEATURE_BASE_ADDR_1 [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PRE1_I2C2_t;
typedef volatile union _DX_M1A_NPU0_PRE1_I2C3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE1_I2C3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t I2C1_HEIGHT_MAX:16;  /* DX_M1A_NPU0.PRE1_I2C3.I2C1_HEIGHT_MAX [15:0] (RW, 0x0)  */
        volatile uint32_t I2C1_WIDTH_MAX:16;  /* DX_M1A_NPU0.PRE1_I2C3.I2C1_WIDTH_MAX [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PRE1_I2C3_t;
typedef volatile union _DX_M1A_NPU0_PRE1_FMT0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE1_FMT0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:22;  /* DX_M1A_NPU0.PRE1_FMT0.RESERVED0 [21:0] (R, 0x0)  */
        volatile uint32_t FMT1_FIRST_LINE_OPT:2;  /* DX_M1A_NPU0.PRE1_FMT0.FMT1_FIRST_LINE_OPT [23:22] (RW, 0x0)  */
        volatile uint32_t FMT1_FIRST_VALID_NUM:6;  /* DX_M1A_NPU0.PRE1_FMT0.FMT1_FIRST_VALID_NUM [29:24] (RW, 0x0)  */
        volatile uint32_t FMT1_CH_MODE:1;  /* DX_M1A_NPU0.PRE1_FMT0.FMT1_CH_MODE [30:30] (RW, 0x0) 0: 3CH, 1: 1CH */
        volatile uint32_t FMT1_ENABLE:1;  /* DX_M1A_NPU0.PRE1_FMT0.FMT1_ENABLE [31:31] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PRE1_FMT0_t;
typedef volatile union _DX_M1A_NPU0_PRE1_FMT1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PRE1_FMT1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t FMT1_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PRE1_FMT1.FMT1_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PRE1_FMT1_t;
typedef volatile union _DX_M1A_NPU0_PE1_ADDR_GEN0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_ADDR_GEN0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_DEFAULT_F_ADDR_GEN_MODE:1;  /* DX_M1A_NPU0.PE1_ADDR_GEN0.PE1_DEFAULT_F_ADDR_GEN_MODE [0:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:4;  /* DX_M1A_NPU0.PE1_ADDR_GEN0.RESERVED0 [4:1] (R, 0x0)  */
        volatile uint32_t PE1_RF_ADDR_GEN_MAX_NUM:27;  /* DX_M1A_NPU0.PE1_ADDR_GEN0.PE1_RF_ADDR_GEN_MAX_NUM [31:5] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_ADDR_GEN0_t;
typedef volatile union _DX_M1A_NPU0_PE1_ADDR_GEN1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_ADDR_GEN1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_ADDR_GEN1.PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_ADDR_GEN1_t;
typedef volatile union _DX_M1A_NPU0_PE1_ADDR_GEN2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_ADDR_GEN2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_DEFAULT_F_SIZE:16;  /* DX_M1A_NPU0.PE1_ADDR_GEN2.PE1_DEFAULT_F_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM:16;  /* DX_M1A_NPU0.PE1_ADDR_GEN2.PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_ADDR_GEN2_t;
typedef volatile union _DX_M1A_NPU0_PE1_ADDR_GEN3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_ADDR_GEN3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_DEFAULT_F_SURFACE_SIZE:16;  /* DX_M1A_NPU0.PE1_ADDR_GEN3.PE1_DEFAULT_F_SURFACE_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t PE1_DEFAULT_F_STRIDE:16;  /* DX_M1A_NPU0.PE1_ADDR_GEN3.PE1_DEFAULT_F_STRIDE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_ADDR_GEN3_t;
typedef volatile union _DX_M1A_NPU0_PE1_ADDR_GEN4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_ADDR_GEN4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_ELEM_ADDR_GEN_MODE:1;  /* DX_M1A_NPU0.PE1_ADDR_GEN4.PE1_ELEM_ADDR_GEN_MODE [0:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:15;  /* DX_M1A_NPU0.PE1_ADDR_GEN4.RESERVED0 [15:1] (R, 0x0)  */
        volatile uint32_t PE1_DEFAULT_F_SURFACE_STRIDE:16;  /* DX_M1A_NPU0.PE1_ADDR_GEN4.PE1_DEFAULT_F_SURFACE_STRIDE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_ADDR_GEN4_t;
typedef volatile union _DX_M1A_NPU0_PE1_ADDR_GEN5_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_ADDR_GEN5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_ELEM_STRIDE:16;  /* DX_M1A_NPU0.PE1_ADDR_GEN5.PE1_ELEM_STRIDE [15:0] (RW, 0x0)  */
        volatile uint32_t PE1_ELEM_SIZE:16;  /* DX_M1A_NPU0.PE1_ADDR_GEN5.PE1_ELEM_SIZE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_ADDR_GEN5_t;
typedef volatile union _DX_M1A_NPU0_PE1_ADDR_GEN6_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_ADDR_GEN6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_ELEM_SURFACE_STRIDE:16;  /* DX_M1A_NPU0.PE1_ADDR_GEN6.PE1_ELEM_SURFACE_STRIDE [15:0] (RW, 0x0)  */
        volatile uint32_t PE1_ELEM_SURFACE_SIZE:16;  /* DX_M1A_NPU0.PE1_ADDR_GEN6.PE1_ELEM_SURFACE_SIZE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_ADDR_GEN6_t;
typedef volatile union _DX_M1A_NPU0_PE1_ADDR_GEN7_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_ADDR_GEN7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_MUL_ADDR_GEN_MODE_OFFSET:16;  /* DX_M1A_NPU0.PE1_ADDR_GEN7.PE1_MUL_ADDR_GEN_MODE_OFFSET [15:0] (RW, 0x0) RTL ONLY USE 8 BIT */
        volatile uint32_t PE1_MUL_ADDR_GEN_MODE:2;  /* DX_M1A_NPU0.PE1_ADDR_GEN7.PE1_MUL_ADDR_GEN_MODE [17:16] (RW, 0x0)  */
        volatile uint32_t PE1_MUL_ADDR_GEN_MODE2_MAX_NUM:14;  /* DX_M1A_NPU0.PE1_ADDR_GEN7.PE1_MUL_ADDR_GEN_MODE2_MAX_NUM [31:18] (RW, 0x0) RTL ONLY USE 9 BIT */
    };
} DX_M1A_NPU0_PE1_ADDR_GEN7_t;
typedef volatile union _DX_M1A_NPU0_PE1_ADDR_GEN8_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_ADDR_GEN8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_ADD_ADDR_GEN_MODE_OFFSET:16;  /* DX_M1A_NPU0.PE1_ADDR_GEN8.PE1_ADD_ADDR_GEN_MODE_OFFSET [15:0] (RW, 0x0) RTL ONLY USE 8 BIT */
        volatile uint32_t PE1_ADD_ADDR_GEN_MODE:2;  /* DX_M1A_NPU0.PE1_ADDR_GEN8.PE1_ADD_ADDR_GEN_MODE [17:16] (RW, 0x0)  */
        volatile uint32_t PE1_ADD_ADDR_GEN_MODE2_MAX_NUM:14;  /* DX_M1A_NPU0.PE1_ADDR_GEN8.PE1_ADD_ADDR_GEN_MODE2_MAX_NUM [31:18] (RW, 0x0) RTL ONLY USE 9 BIT */
    };
} DX_M1A_NPU0_PE1_ADDR_GEN8_t;
typedef volatile union _DX_M1A_NPU0_PE1_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_CONV_TYPE:8;  /* DX_M1A_NPU0.PE1_CTRL.PE1_CONV_TYPE [7:0] (RW, 0x0)  */
        volatile uint32_t PE1_CONV_EN:1;  /* DX_M1A_NPU0.PE1_CTRL.PE1_CONV_EN [8:8] (RW, 0x0)  */
        volatile uint32_t RESERVED0:7;  /* DX_M1A_NPU0.PE1_CTRL.RESERVED0 [15:9] (R, 0x0)  */
        volatile uint32_t PE1_SYSTOLIC_DEPTH:8;  /* DX_M1A_NPU0.PE1_CTRL.PE1_SYSTOLIC_DEPTH [23:16] (RW, 0x0) RTL ONLY USE 6 BIT */
        volatile uint32_t PE1_SYSTOLIC_EN:1;  /* DX_M1A_NPU0.PE1_CTRL.PE1_SYSTOLIC_EN [24:24] (RW, 0x0)  */
        volatile uint32_t PE1_SFUIN_VALID_NUM:7;  /* DX_M1A_NPU0.PE1_CTRL.PE1_SFUIN_VALID_NUM [31:25] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CTRL_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_LAST_CONV_NUM:26;  /* DX_M1A_NPU0.PE1_CFG0.PE1_LAST_CONV_NUM [25:0] (RW, 0x0)  */
        volatile uint32_t PE1_LAST_CONV_MOD:6;  /* DX_M1A_NPU0.PE1_CFG0.PE1_LAST_CONV_MOD [31:26] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG0_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_OFEATURE_CHANNEL:16;  /* DX_M1A_NPU0.PE1_CFG1.PE1_OFEATURE_CHANNEL [15:0] (RW, 0x0) RTL ONLY USE 15BIT(MAX 25088) */
        volatile uint32_t PE1_OFEATURE_NUMBER:16;  /* DX_M1A_NPU0.PE1_CFG1.PE1_OFEATURE_NUMBER [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG1_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_OFEATURE_WIDTH:16;  /* DX_M1A_NPU0.PE1_CFG2.PE1_OFEATURE_WIDTH [15:0] (RW, 0x0) RTL ONLY USE 13BIT(MAX 7680) */
        volatile uint32_t PE1_OFEATURE_HEIGHT:16;  /* DX_M1A_NPU0.PE1_CFG2.PE1_OFEATURE_HEIGHT [31:16] (RW, 0x0) RTL ONLY USE 13BIT(MAX 4320) */
    };
} DX_M1A_NPU0_PE1_CFG2_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_OFEATURE_SIZE:32;  /* DX_M1A_NPU0.PE1_CFG3.PE1_OFEATURE_SIZE [31:0] (RW, 0x0) RTL ONLY USE 26BIT(MAX 7680*4320) */
    };
} DX_M1A_NPU0_PE1_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_FEATURE_HEIGHT:16;  /* DX_M1A_NPU0.PE1_CFG4.PE1_FEATURE_HEIGHT [15:0] (RW, 0x0) RTL ONLY USE 13BIT(MAX 4320) */
        volatile uint32_t PE1_FEATURE_CHANNEL:16;  /* DX_M1A_NPU0.PE1_CFG4.PE1_FEATURE_CHANNEL [31:16] (RW, 0x0) RTL ONLY USE 15BIT(MAX 25088) */
    };
} DX_M1A_NPU0_PE1_CFG4_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG5_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG5*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_FILTER_NUMBER:16;  /* DX_M1A_NPU0.PE1_CFG5.PE1_FILTER_NUMBER [15:0] (RW, 0x0) RTL ONLY USE 15BIT(MAX 25088), NO USE */
        volatile uint32_t PE1_FEATURE_WIDTH:16;  /* DX_M1A_NPU0.PE1_CFG5.PE1_FEATURE_WIDTH [31:16] (RW, 0x0) RTL ONLY USE 13BIT(MAX 7680) */
    };
} DX_M1A_NPU0_PE1_CFG5_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG6_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_PAD_MODE:1;  /* DX_M1A_NPU0.PE1_CFG6.PE1_PAD_MODE [0:0] (RW, 0x0) 0: CONSTANT PAD, 1: REPLICATION PAD */
        volatile uint32_t RESERVED0:5;  /* DX_M1A_NPU0.PE1_CFG6.RESERVED0 [5:1] (R, 0x0)  */
        volatile uint32_t PE1_FILTER_WIDTH:5;  /* DX_M1A_NPU0.PE1_CFG6.PE1_FILTER_WIDTH [10:6] (RW, 0x0) ONLY POOLING BLOCK USE 5BIT, OTHER BLOCK USE 4BIT */
        volatile uint32_t PE1_FILTER_HEIGHT:5;  /* DX_M1A_NPU0.PE1_CFG6.PE1_FILTER_HEIGHT [15:11] (RW, 0x0) ONLY POOLING BLOCK USE 5BIT, OTHER BLOCK USE 4BIT */
        volatile uint32_t PE1_FILTER_CHANNEL:16;  /* DX_M1A_NPU0.PE1_CFG6.PE1_FILTER_CHANNEL [31:16] (RW, 0x0) RTL ONLY USE 15BIT(MAX 25088) */
    };
} DX_M1A_NPU0_PE1_CFG6_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG7_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_PAD_TOP_SIZE:6;  /* DX_M1A_NPU0.PE1_CFG7.PE1_PAD_TOP_SIZE [5:0] (RW, 0x0)  */
        volatile uint32_t PE1_PAD_BOTTOM_SIZE:6;  /* DX_M1A_NPU0.PE1_CFG7.PE1_PAD_BOTTOM_SIZE [11:6] (RW, 0x0)  */
        volatile uint32_t PE1_PAD_LEFT_SIZE:6;  /* DX_M1A_NPU0.PE1_CFG7.PE1_PAD_LEFT_SIZE [17:12] (RW, 0x0)  */
        volatile uint32_t PE1_PAD_RIGHT_SIZE:6;  /* DX_M1A_NPU0.PE1_CFG7.PE1_PAD_RIGHT_SIZE [23:18] (RW, 0x0)  */
        volatile uint32_t PE1_PAD_VALUE:8;  /* DX_M1A_NPU0.PE1_CFG7.PE1_PAD_VALUE [31:24] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG7_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG8_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG8*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_STRIDE_X:5;  /* DX_M1A_NPU0.PE1_CFG8.PE1_STRIDE_X [4:0] (RW, 0x0) ONLY POOLING BLOCK USE 5BIT, OTHER BLOCK USE 4BIT */
        volatile uint32_t PE1_STRIDE_Y:5;  /* DX_M1A_NPU0.PE1_CFG8.PE1_STRIDE_Y [9:5] (RW, 0x0) ONLY POOLING BLOCK USE 5BIT, OTHER BLOCK USE 4BIT */
        volatile uint32_t RESERVED0:6;  /* DX_M1A_NPU0.PE1_CFG8.RESERVED0 [15:10] (R, 0x0)  */
        volatile uint32_t PE1_CHANNEL_OFFSET:16;  /* DX_M1A_NPU0.PE1_CFG8.PE1_CHANNEL_OFFSET [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG8_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG9_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_ELEM_CHANNEL_OFFSET:16;  /* DX_M1A_NPU0.PE1_CFG9.PE1_ELEM_CHANNEL_OFFSET [15:0] (RW, 0x0)  */
        volatile uint32_t PE1_DILATION_X:6;  /* DX_M1A_NPU0.PE1_CFG9.PE1_DILATION_X [21:16] (RW, 0x0)  */
        volatile uint32_t PE1_DILATION_Y:6;  /* DX_M1A_NPU0.PE1_CFG9.PE1_DILATION_Y [27:22] (RW, 0x0)  */
        volatile uint32_t RESERVED0:4;  /* DX_M1A_NPU0.PE1_CFG9.RESERVED0 [31:28] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG9_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG10_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG10*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_READ_FEATURE_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_CFG10.PE1_READ_FEATURE_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG10_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG11_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG11*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WRITE_FEATURE_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_CFG11.PE1_WRITE_FEATURE_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG11_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG12_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG12*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_READ_WEIGHT_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_CFG12.PE1_READ_WEIGHT_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG12_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG13_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG13*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_ELEM_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_CFG13.PE1_ELEM_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG13_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG14_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG14*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_MUL_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_CFG14.PE1_MUL_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG14_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG15_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG15*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_ADD_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_CFG15.PE1_ADD_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG15_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG16_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG16*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_CFG16.PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG16_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG17_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG17*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_CFG17.PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG17_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG18_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG18*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_SE_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_CFG18.PE1_SE_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG18_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG19_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG19*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_CFG19.PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG19_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG20_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG20*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_ADDR_MODE1_LINE_SIZE:16;  /* DX_M1A_NPU0.PE1_CFG20.PE1_WF_ADDR_MODE1_LINE_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:13;  /* DX_M1A_NPU0.PE1_CFG20.RESERVED0 [28:16] (R, 0x0)  */
        volatile uint32_t PE1_WF_ADDR_GEN_MODE:3;  /* DX_M1A_NPU0.PE1_CFG20.PE1_WF_ADDR_GEN_MODE [31:29] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG20_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG21_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG21*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_ADDR_MODE1_LINE_STRIDE:32;  /* DX_M1A_NPU0.PE1_CFG21.PE1_WF_ADDR_MODE1_LINE_STRIDE [31:0] (RW, 0x0) RTL ONLY USE 16BIT */
    };
} DX_M1A_NPU0_PE1_CFG21_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG22_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG22*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_ADDR_SURFACE_SIZE:32;  /* DX_M1A_NPU0.PE1_CFG22.PE1_WF_ADDR_SURFACE_SIZE [31:0] (RW, 0x0) RTL ONLY USE 16BIT */
    };
} DX_M1A_NPU0_PE1_CFG22_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG23_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG23*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_WRITE_CNT:16;  /* DX_M1A_NPU0.PE1_CFG23.PE1_WF_WRITE_CNT [15:0] (RW, 0x0)  */
        volatile uint32_t PE1_WF_ADDR_SURFACE_STRIDE:16;  /* DX_M1A_NPU0.PE1_CFG23.PE1_WF_ADDR_SURFACE_STRIDE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG23_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG24_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG24*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_WF_SRC1_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_CFG24.PE1_WF_SRC1_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG24_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG25_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG25*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_SFU_OUT_DATA_VALID_NUM:3;  /* DX_M1A_NPU0.PE1_CFG25.PE1_SFU_OUT_DATA_VALID_NUM [2:0] (RW, 0x0) ONLY HWC USE, RTL NO USE */
        volatile uint32_t RESERVED1:5;  /* DX_M1A_NPU0.PE1_CFG25.RESERVED1 [7:3] (R, 0x0)  */
        volatile uint32_t PE1_ST_ADDR_GEN_SHIFT_LEN:5;  /* DX_M1A_NPU0.PE1_CFG25.PE1_ST_ADDR_GEN_SHIFT_LEN [12:8] (RW, 0x0) RTL NO USE */
        volatile uint32_t RESERVED0:19;  /* DX_M1A_NPU0.PE1_CFG25.RESERVED0 [31:13] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG25_t;
typedef volatile union _DX_M1A_NPU0_PE1_CFG26_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_CFG26*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_ST_ADDR_GEN_MUL:31;  /* DX_M1A_NPU0.PE1_CFG26.PE1_ST_ADDR_GEN_MUL [30:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:1;  /* DX_M1A_NPU0.PE1_CFG26.RESERVED0 [31:31] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE1_CFG26_t;
typedef volatile union _DX_M1A_NPU0_PE1_VMEM_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_VMEM_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PE1_START_NO_IN_BUFF:6;  /* DX_M1A_NPU0.PE1_VMEM_CFG0.PE1_START_NO_IN_BUFF [5:0] (RW, 0x0)  */
        volatile uint32_t RESERVED3:2;  /* DX_M1A_NPU0.PE1_VMEM_CFG0.RESERVED3 [7:6] (R, 0x0)  */
        volatile uint32_t PE1_START_NO_WEIGHT_BUFF:6;  /* DX_M1A_NPU0.PE1_VMEM_CFG0.PE1_START_NO_WEIGHT_BUFF [13:8] (RW, 0x0)  */
        volatile uint32_t RESERVED2:2;  /* DX_M1A_NPU0.PE1_VMEM_CFG0.RESERVED2 [15:14] (R, 0x0)  */
        volatile uint32_t PE1_START_NO_OUT_BUFF:6;  /* DX_M1A_NPU0.PE1_VMEM_CFG0.PE1_START_NO_OUT_BUFF [21:16] (RW, 0x0)  */
        volatile uint32_t RESERVED1:2;  /* DX_M1A_NPU0.PE1_VMEM_CFG0.RESERVED1 [23:22] (R, 0x0)  */
        volatile uint32_t PE1_START_NO_ELEM_BUFF:6;  /* DX_M1A_NPU0.PE1_VMEM_CFG0.PE1_START_NO_ELEM_BUFF [29:24] (RW, 0x0)  */
        volatile uint32_t RESERVED0:2;  /* DX_M1A_NPU0.PE1_VMEM_CFG0.RESERVED0 [31:30] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE1_VMEM_CFG0_t;
typedef volatile union _DX_M1A_NPU0_REDUCE1_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.REDUCE1_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t REDUCE1_CHANNEL_SIZE:8;  /* DX_M1A_NPU0.REDUCE1_CFG0.REDUCE1_CHANNEL_SIZE [7:0] (RW, 0x0) CEIL(CHANNEL / 16) */
        volatile uint32_t REDUCE1_CHANNEL_NUM:16;  /* DX_M1A_NPU0.REDUCE1_CFG0.REDUCE1_CHANNEL_NUM [23:8] (RW, 0x0) INPUT CHANNEL NUMBER */
        volatile uint32_t REDUCE1_MODE:2;  /* DX_M1A_NPU0.REDUCE1_CFG0.REDUCE1_MODE [25:24] (RW, 0x0) 0: SUM 1: MAX 2: RESERVED */
        volatile uint32_t REDUCE1_EN:1;  /* DX_M1A_NPU0.REDUCE1_CFG0.REDUCE1_EN [26:26] (RW, 0x0) REDUCE ENABLE */
        volatile uint32_t RESERVED0:5;  /* DX_M1A_NPU0.REDUCE1_CFG0.RESERVED0 [31:27] (R, 0x0)  */
    };
} DX_M1A_NPU0_REDUCE1_CFG0_t;
typedef volatile union _DX_M1A_NPU0_REDUCE1_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.REDUCE1_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t REDUCE1_WIN_NUM:16;  /* DX_M1A_NPU0.REDUCE1_CFG1.REDUCE1_WIN_NUM [15:0] (RW, 0x0) FEATURE WIDTH * FEATURE HEIGHT */
        volatile uint32_t REDUCE1_CNT_MAX_NUM:16;  /* DX_M1A_NPU0.REDUCE1_CFG1.REDUCE1_CNT_MAX_NUM [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_REDUCE1_CFG1_t;
typedef volatile union _DX_M1A_NPU0_REDUCE1_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.REDUCE1_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t REDUCE1_RD_BASE_ADDR:32;  /* DX_M1A_NPU0.REDUCE1_CFG2.REDUCE1_RD_BASE_ADDR [31:0] (RW, 0x0) READ BASE ADDRESS */
    };
} DX_M1A_NPU0_REDUCE1_CFG2_t;
typedef volatile union _DX_M1A_NPU0_REDUCE1_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.REDUCE1_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t REDUCE1_WR_BASE_ADDR:32;  /* DX_M1A_NPU0.REDUCE1_CFG3.REDUCE1_WR_BASE_ADDR [31:0] (RW, 0x0) WRITE BASE ADDRESS */
    };
} DX_M1A_NPU0_REDUCE1_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_CTRL_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_CTRL*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_START_OP_EN:1;  /* DX_M1A_NPU0.PE1_DMA_CTRL.VMEM1_START_OP_EN [0:0] (RW, 0x0)  */
        volatile uint32_t VMEM1_DMA_CTRL_MODE:2;  /* DX_M1A_NPU0.PE1_DMA_CTRL.VMEM1_DMA_CTRL_MODE [2:1] (RW, 0x0)  */
        volatile uint32_t VMEM1_WEIGHT_BIT_WIDTH:8;  /* DX_M1A_NPU0.PE1_DMA_CTRL.VMEM1_WEIGHT_BIT_WIDTH [10:3] (RW, 0x0)  */
        volatile uint32_t VMEM1_WEIGHT_DUMMY_MODE:2;  /* DX_M1A_NPU0.PE1_DMA_CTRL.VMEM1_WEIGHT_DUMMY_MODE [12:11] (RW, 0x0) 0: NO OPERATION(64), 1: 16, 2: 32, 3: 48 */
        volatile uint32_t RESERVED0:19;  /* DX_M1A_NPU0.PE1_DMA_CTRL.RESERVED0 [31:13] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_CTRL_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_ELEM_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_RD_ELEM_SIZE:16;  /* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.VMEM1_RD_ELEM_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t VMEM1_RD_ELEM_TIME:6;  /* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.VMEM1_RD_ELEM_TIME [21:16] (RW, 0x0)  */
        volatile uint32_t VMEM1_RD_ELEM_MODE:2;  /* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.VMEM1_RD_ELEM_MODE [23:22] (RW, 0x0)  */
        volatile uint32_t VMEM1_RD_ELEM_EN:1;  /* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.VMEM1_RD_ELEM_EN [24:24] (RW, 0x0)  */
        volatile uint32_t VMEM1_RD_ELEM_SRC:1;  /* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.VMEM1_RD_ELEM_SRC [25:25] (RW, 0x0) 0: USE RAW IMG(IN_FEATURE_BASE), 1: USE TENSOR DATA(OUT_FEATURE_BASE) */
        volatile uint32_t RESERVED0:6;  /* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.RESERVED0 [31:26] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_ELEM_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_RD_ELEM_STRIDE:32;  /* DX_M1A_NPU0.PE1_DMA_ELEM_CFG1.VMEM1_RD_ELEM_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_ELEM_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_RD_ELEM_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_DMA_ELEM_CFG2.VMEM1_RD_ELEM_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_ELEM_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_WT_ELEM_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_DMA_ELEM_CFG3.VMEM1_WT_ELEM_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_W_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_W_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_RD_W_TIME:12;  /* DX_M1A_NPU0.PE1_DMA_W_CFG0.VMEM1_RD_W_TIME [11:0] (RW, 0x0)  */
        volatile uint32_t VMEM1_RD_W_MODE:2;  /* DX_M1A_NPU0.PE1_DMA_W_CFG0.VMEM1_RD_W_MODE [13:12] (RW, 0x0)  */
        volatile uint32_t VMEM1_RD_W_EN:1;  /* DX_M1A_NPU0.PE1_DMA_W_CFG0.VMEM1_RD_W_EN [14:14] (RW, 0x0)  */
        volatile uint32_t RESERVED0:17;  /* DX_M1A_NPU0.PE1_DMA_W_CFG0.RESERVED0 [31:15] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_W_CFG0_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_W_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_W_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_RD_W_STRIDE:16;  /* DX_M1A_NPU0.PE1_DMA_W_CFG1.VMEM1_RD_W_STRIDE [15:0] (RW, 0x0)  */
        volatile uint32_t VMEM1_RD_W_SIZE:16;  /* DX_M1A_NPU0.PE1_DMA_W_CFG1.VMEM1_RD_W_SIZE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_W_CFG1_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_W_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_W_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_RD_W_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_DMA_W_CFG2.VMEM1_RD_W_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_W_CFG2_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_W_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_W_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_WT_W_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_DMA_W_CFG3.VMEM1_WT_W_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_W_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_INF_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_INF_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_RD_INF_SIZE:16;  /* DX_M1A_NPU0.PE1_DMA_INF_CFG0.VMEM1_RD_INF_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t VMEM1_RD_INF_TIME:12;  /* DX_M1A_NPU0.PE1_DMA_INF_CFG0.VMEM1_RD_INF_TIME [27:16] (RW, 0x0)  */
        volatile uint32_t VMEM1_RD_INF_MODE:2;  /* DX_M1A_NPU0.PE1_DMA_INF_CFG0.VMEM1_RD_INF_MODE [29:28] (RW, 0x0)  */
        volatile uint32_t VMEM1_RD_INF_EN:1;  /* DX_M1A_NPU0.PE1_DMA_INF_CFG0.VMEM1_RD_INF_EN [30:30] (RW, 0x0)  */
        volatile uint32_t VMEM1_RD_INF_SRC:1;  /* DX_M1A_NPU0.PE1_DMA_INF_CFG0.VMEM1_RD_INF_SRC [31:31] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_INF_CFG0_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_INF_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_INF_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_RD_INF_STRIDE:32;  /* DX_M1A_NPU0.PE1_DMA_INF_CFG1.VMEM1_RD_INF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_INF_CFG1_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_INF_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_INF_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_RD_INF_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_DMA_INF_CFG2.VMEM1_RD_INF_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_INF_CFG2_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_INF_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_INF_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_WT_INF_STRIDE:32;  /* DX_M1A_NPU0.PE1_DMA_INF_CFG3.VMEM1_WT_INF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_INF_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_INF_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_INF_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_WT_INF_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_DMA_INF_CFG4.VMEM1_WT_INF_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_INF_CFG4_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_OUTF_CFG0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_WT_OUTF_SIZE:16;  /* DX_M1A_NPU0.PE1_DMA_OUTF_CFG0.VMEM1_WT_OUTF_SIZE [15:0] (RW, 0x0)  */
        volatile uint32_t VMEM1_WT_OUTF_TIME:12;  /* DX_M1A_NPU0.PE1_DMA_OUTF_CFG0.VMEM1_WT_OUTF_TIME [27:16] (RW, 0x0)  */
        volatile uint32_t RESERVED1:2;  /* DX_M1A_NPU0.PE1_DMA_OUTF_CFG0.RESERVED1 [29:28] (R, 0x0)  */
        volatile uint32_t VMEM1_WT_OUTF_EN:1;  /* DX_M1A_NPU0.PE1_DMA_OUTF_CFG0.VMEM1_WT_OUTF_EN [30:30] (RW, 0x0)  */
        volatile uint32_t RESERVED0:1;  /* DX_M1A_NPU0.PE1_DMA_OUTF_CFG0.RESERVED0 [31:31] (R, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_OUTF_CFG1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_WT_OUTF_STRIDE:32;  /* DX_M1A_NPU0.PE1_DMA_OUTF_CFG1.VMEM1_WT_OUTF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_OUTF_CFG2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_WT_OUTF_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_DMA_OUTF_CFG2.VMEM1_WT_OUTF_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_OUTF_CFG3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_RD_OUTF_STRIDE:32;  /* DX_M1A_NPU0.PE1_DMA_OUTF_CFG3.VMEM1_RD_OUTF_STRIDE [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_t;
typedef volatile union _DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PE1_DMA_OUTF_CFG4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t VMEM1_RD_OUTF_BASE_ADDR:32;  /* DX_M1A_NPU0.PE1_DMA_OUTF_CFG4.VMEM1_RD_OUTF_BASE_ADDR [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_t;
typedef volatile union _DX_M1A_NPU0_PPU_CFG_0_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PPU_CFG_0*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PPU_GRID_HEIGHT_OFFSET:8;  /* DX_M1A_NPU0.PPU_CFG_0.PPU_GRID_HEIGHT_OFFSET [7:0] (RW, 0x0) GRID HEIGHT OFFSET FOR HEIGHT TILING */
        volatile uint32_t PPU_GRID_HEIGHT:8;  /* DX_M1A_NPU0.PPU_CFG_0.PPU_GRID_HEIGHT [15:8] (RW, 0x0) GRID_HEIGHT FOR GRID_GENERATION */
        volatile uint32_t PPU_GRID_WIDTH:8;  /* DX_M1A_NPU0.PPU_CFG_0.PPU_GRID_WIDTH [23:16] (RW, 0x0) GRID_WIDTH FOR GRID_GENERATION */
        volatile uint32_t PPU_VMEM_WRITE_EN:1;  /* DX_M1A_NPU0.PPU_CFG_0.PPU_VMEM_WRITE_EN [24:24] (RW, 0x0) PPU FILTER DATA TO VMEM FOR DEBUG */
        volatile uint32_t PPU_1ST_TILE_EN:1;  /* DX_M1A_NPU0.PPU_CFG_0.PPU_1ST_TILE_EN [25:25] (RW, 0x0) 1ST TILE OF PPU BLOCKS */
        volatile uint32_t PPU_EN:1;  /* DX_M1A_NPU0.PPU_CFG_0.PPU_EN [26:26] (RW, 0x0) BLOCK ENABLE WHEN OPERATION LAYER */
        volatile uint32_t RESERVED0:5;  /* DX_M1A_NPU0.PPU_CFG_0.RESERVED0 [31:27] (R, 0x0)  */
    };
} DX_M1A_NPU0_PPU_CFG_0_t;
typedef volatile union _DX_M1A_NPU0_PPU_CFG_1_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PPU_CFG_1*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PPU_OUT_FEATURE_CHANNEL:15;  /* DX_M1A_NPU0.PPU_CFG_1.PPU_OUT_FEATURE_CHANNEL [14:0] (RW, 0x0)  */
        volatile uint32_t RESERVED0:1;  /* DX_M1A_NPU0.PPU_CFG_1.RESERVED0 [15:15] (R, 0x0)  */
        volatile uint32_t PPU_ARG_CLASS_NUM:16;  /* DX_M1A_NPU0.PPU_CFG_1.PPU_ARG_CLASS_NUM [31:16] (RW, 0x0) # OF CLASS */
    };
} DX_M1A_NPU0_PPU_CFG_1_t;
typedef volatile union _DX_M1A_NPU0_PPU_CFG_2_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PPU_CFG_2*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PPU_FILTER_IN_SIZE:16;  /* DX_M1A_NPU0.PPU_CFG_2.PPU_FILTER_IN_SIZE [15:0] (RW, 0x0) # OF BOX TO PROCESS   EG) 20X20 -> 400 */
        volatile uint32_t PPU_ARG_IN_SIZE:16;  /* DX_M1A_NPU0.PPU_CFG_2.PPU_ARG_IN_SIZE [31:16] (RW, 0x0)  */
    };
} DX_M1A_NPU0_PPU_CFG_2_t;
typedef volatile union _DX_M1A_NPU0_PPU_CFG_3_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PPU_CFG_3*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PPU_FILTER_THR:32;  /* DX_M1A_NPU0.PPU_CFG_3.PPU_FILTER_THR [31:0] (RW, 0x0) THRESHOLD FOR FILTERING */
    };
} DX_M1A_NPU0_PPU_CFG_3_t;
typedef volatile union _DX_M1A_NPU0_PPU_CFG_4_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PPU_CFG_4*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PPU_ANCHOR_NUM:8;  /* DX_M1A_NPU0.PPU_CFG_4.PPU_ANCHOR_NUM [7:0] (RW, 0x0) ANCHOR SET NUMBER IN CONV LAYER */
        volatile uint32_t PPU_CONV_NUM:8;  /* DX_M1A_NPU0.PPU_CFG_4.PPU_CONV_NUM [15:8] (RW, 0x0) CONV LAYER ORDER NUMBER */
        volatile uint32_t PPU_WRITE_ADDR_RST:1;  /* DX_M1A_NPU0.PPU_CFG_4.PPU_WRITE_ADDR_RST [16:16] (RW, 0x0) ADDRESS RESET(DEBUG 用) */
        volatile uint32_t PPU_DATA_SIZE:4;  /* DX_M1A_NPU0.PPU_CFG_4.PPU_DATA_SIZE [20:17] (RW, 0x0) FILTER OUTPUT DATA MEMORY WORD 개수 */
        volatile uint32_t PPU_LABEL_ON:1;  /* DX_M1A_NPU0.PPU_CFG_4.PPU_LABEL_ON [21:21] (RW, 0x0)  */
        volatile uint32_t RESERVED0:9;  /* DX_M1A_NPU0.PPU_CFG_4.RESERVED0 [30:22] (R, 0x0)  */
        volatile uint32_t PPU_FILTER_MODE:1;  /* DX_M1A_NPU0.PPU_CFG_4.PPU_FILTER_MODE [31:31] (RW, 0x0) 0: CONFIDENCE*SCORE FILTER,  1: ONLY SCORE FILTER */
    };
} DX_M1A_NPU0_PPU_CFG_4_t;
typedef volatile union _DX_M1A_NPU0_PPU_CFG_6_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PPU_CFG_6*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PPU_READ_BASE_ADDR:32;  /* DX_M1A_NPU0.PPU_CFG_6.PPU_READ_BASE_ADDR [31:0] (RW, 0x0) BOX&CLASS DATA STORED SRAM START ADDRESS (VMEM) */
    };
} DX_M1A_NPU0_PPU_CFG_6_t;
typedef volatile union _DX_M1A_NPU0_PPU_CFG_7_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PPU_CFG_7*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t PPU_WRITE_BASE_ADDR:32;  /* DX_M1A_NPU0.PPU_CFG_7.PPU_WRITE_BASE_ADDR [31:0] (RW, 0x0) BOX&CLASS DATA STORED SRAM START ADDRESS (VMEM) */
    };
} DX_M1A_NPU0_PPU_CFG_7_t;
typedef volatile union _DX_M1A_NPU0_PPU_CFG_9_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.PPU_CFG_9*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t RESERVED0:32;  /* DX_M1A_NPU0.PPU_CFG_9.RESERVED0 [31:0] (R, 0x0)  */
    };
} DX_M1A_NPU0_PPU_CFG_9_t;
typedef volatile union _DX_M1A_NPU0_CHECKSUM_t
{
    volatile uint32_t U;  /* Register-Access : DX_M1A_NPU0.CHECKSUM*/
    struct {     /* Register-Bitfields-Access */
        volatile uint32_t CHECKSUM_TILE:32;  /* DX_M1A_NPU0.CHECKSUM.CHECKSUM_TILE [31:0] (RW, 0x0)  */
    };
} DX_M1A_NPU0_CHECKSUM_t;

/* DX_M1A_NPU0 Module Definition */
typedef volatile struct _dxDX_M1A_NPU0_t
{
    DX_M1A_NPU0_DATA_CFG0_t DATA_CFG0;  /* NPU0 0x0 */
    DX_M1A_NPU0_CTRL_t CTRL;  /* NPU0 0x4 */
    DX_M1A_NPU0_MISC_t MISC;  /* NPU0 0x8 */
    DX_M1A_NPU0_PSEUDO_t PSEUDO;  /* NPU0 0xc */
    DX_M1A_NPU0_TRP0_t TRP0;  /* NPU0 0x10 */
    DX_M1A_NPU0_TRP1_t TRP1;  /* NPU0 0x14 */
    DX_M1A_NPU0_TRP2_t TRP2;  /* NPU0 0x18 */
    DX_M1A_NPU0_TRP3_t TRP3;  /* NPU0 0x1c */
    DX_M1A_NPU0_TRP4_t TRP4;  /* NPU0 0x20 */
    DX_M1A_NPU0_ARG0_t ARG0;  /* NPU0 0x24 */
    DX_M1A_NPU0_POOL0_t POOL0;  /* NPU0 0x28 */
    DX_M1A_NPU0_POOL1_t POOL1;  /* NPU0 0x2c */
    DX_M1A_NPU0_POOL2_t POOL2;  /* NPU0 0x30 */
    DX_M1A_NPU0_SFU_CTRL0_t SFU_CTRL0;  /* NPU0 0x34 */
    DX_M1A_NPU0_SFU_COEFF0_t SFU_COEFF0;  /* NPU0 0x38 */
    DX_M1A_NPU0_SFU_COEFF1_t SFU_COEFF1;  /* NPU0 0x3c */
    DX_M1A_NPU0_SFU_COEFF2_t SFU_COEFF2;  /* NPU0 0x40 */
    DX_M1A_NPU0_SFU_COEFF3_t SFU_COEFF3;  /* NPU0 0x44 */
    DX_M1A_NPU0_SFU_COEFF4_t SFU_COEFF4;  /* NPU0 0x48 */
    DX_M1A_NPU0_SFU_COEFF5_t SFU_COEFF5;  /* NPU0 0x4c */
    DX_M1A_NPU0_SFU_COEFF6_t SFU_COEFF6;  /* NPU0 0x50 */
    DX_M1A_NPU0_SFU_COEFF7_t SFU_COEFF7;  /* NPU0 0x54 */
    DX_M1A_NPU0_SFU_COEFF8_t SFU_COEFF8;  /* NPU0 0x58 */
    DX_M1A_NPU0_SFU_COEFF9_t SFU_COEFF9;  /* NPU0 0x5c */
    DX_M1A_NPU0_SFU_PAF0_t SFU_PAF0;  /* NPU0 0x60 */
    DX_M1A_NPU0_SFU_PAF1_t SFU_PAF1;  /* NPU0 0x64 */
    DX_M1A_NPU0_SFU_PAF2_t SFU_PAF2;  /* NPU0 0x68 */
    DX_M1A_NPU0_SFU_PAF3_t SFU_PAF3;  /* NPU0 0x6c */
    DX_M1A_NPU0_SFU_PAF4_t SFU_PAF4;  /* NPU0 0x70 */
    DX_M1A_NPU0_SFU_PAF5_t SFU_PAF5;  /* NPU0 0x74 */
    DX_M1A_NPU0_SFU_PAF6_t SFU_PAF6;  /* NPU0 0x78 */
    DX_M1A_NPU0_SFU_PAF7_t SFU_PAF7;  /* NPU0 0x7c */
    DX_M1A_NPU0_SFU_PAF8_t SFU_PAF8;  /* NPU0 0x80 */
    DX_M1A_NPU0_SFU_PAF9_t SFU_PAF9;  /* NPU0 0x84 */
    DX_M1A_NPU0_SFU_PAF10_t SFU_PAF10;  /* NPU0 0x88 */
    DX_M1A_NPU0_SFU_PAF11_t SFU_PAF11;  /* NPU0 0x8c */
    DX_M1A_NPU0_SFU_PAF12_t SFU_PAF12;  /* NPU0 0x90 */
    DX_M1A_NPU0_SFU_PAF13_t SFU_PAF13;  /* NPU0 0x94 */
    DX_M1A_NPU0_SFU_PAF14_t SFU_PAF14;  /* NPU0 0x98 */
    DX_M1A_NPU0_SFU_PAF15_t SFU_PAF15;  /* NPU0 0x9c */
    DX_M1A_NPU0_SFU_PAF16_t SFU_PAF16;  /* NPU0 0xa0 */
    DX_M1A_NPU0_SFU_PAF17_t SFU_PAF17;  /* NPU0 0xa4 */
    DX_M1A_NPU0_SFU_PAF18_t SFU_PAF18;  /* NPU0 0xa8 */
    DX_M1A_NPU0_SFU_PAF19_t SFU_PAF19;  /* NPU0 0xac */
    DX_M1A_NPU0_SFU_PAF20_t SFU_PAF20;  /* NPU0 0xb0 */
    DX_M1A_NPU0_SFU_PAF21_t SFU_PAF21;  /* NPU0 0xb4 */
    DX_M1A_NPU0_SFU_PAF22_t SFU_PAF22;  /* NPU0 0xb8 */
    DX_M1A_NPU0_SFU_PAF23_t SFU_PAF23;  /* NPU0 0xbc */
    DX_M1A_NPU0_SFU_PAF24_t SFU_PAF24;  /* NPU0 0xc0 */
    DX_M1A_NPU0_SFU_PAF25_t SFU_PAF25;  /* NPU0 0xc4 */
    DX_M1A_NPU0_SFU_PAF26_t SFU_PAF26;  /* NPU0 0xc8 */
    DX_M1A_NPU0_SFU_PAF27_t SFU_PAF27;  /* NPU0 0xcc */
    DX_M1A_NPU0_SFU_PAF28_t SFU_PAF28;  /* NPU0 0xd0 */
    DX_M1A_NPU0_SFU_PAF29_t SFU_PAF29;  /* NPU0 0xd4 */
    DX_M1A_NPU0_SFU_PAF30_t SFU_PAF30;  /* NPU0 0xd8 */
    DX_M1A_NPU0_SFU_PAF31_t SFU_PAF31;  /* NPU0 0xdc */
    DX_M1A_NPU0_SFU_PAF32_t SFU_PAF32;  /* NPU0 0xe0 */
    DX_M1A_NPU0_SFU_PAF33_t SFU_PAF33;  /* NPU0 0xe4 */
    DX_M1A_NPU0_SFU_PAF34_t SFU_PAF34;  /* NPU0 0xe8 */
    DX_M1A_NPU0_SFU_PAF35_t SFU_PAF35;  /* NPU0 0xec */
    DX_M1A_NPU0_SFU_PAF36_t SFU_PAF36;  /* NPU0 0xf0 */
    DX_M1A_NPU0_SFU_PAF37_t SFU_PAF37;  /* NPU0 0xf4 */
    DX_M1A_NPU0_SFU_PAF38_t SFU_PAF38;  /* NPU0 0xf8 */
    DX_M1A_NPU0_SFU_PAF39_t SFU_PAF39;  /* NPU0 0xfc */
    DX_M1A_NPU0_SFU_PAF40_t SFU_PAF40;  /* NPU0 0x100 */
    DX_M1A_NPU0_SFU_PAF41_t SFU_PAF41;  /* NPU0 0x104 */
    DX_M1A_NPU0_SFU_PAF42_t SFU_PAF42;  /* NPU0 0x108 */
    DX_M1A_NPU0_SFU_PAF43_t SFU_PAF43;  /* NPU0 0x10c */
    DX_M1A_NPU0_SFU_PAF44_t SFU_PAF44;  /* NPU0 0x110 */
    DX_M1A_NPU0_SFU_PAF45_t SFU_PAF45;  /* NPU0 0x114 */
    DX_M1A_NPU0_SFU_PAF46_t SFU_PAF46;  /* NPU0 0x118 */
    DX_M1A_NPU0_SFU_SCALE_CFG0_t SFU_SCALE_CFG0;  /* NPU0 0x11c */
    DX_M1A_NPU0_SFU_SCALE_CFG1_t SFU_SCALE_CFG1;  /* NPU0 0x120 */
    DX_M1A_NPU0_SFU_SCALE_CFG2_t SFU_SCALE_CFG2;  /* NPU0 0x124 */
    DX_M1A_NPU0_SFU_SCALE_CFG3_t SFU_SCALE_CFG3;  /* NPU0 0x128 */
    DX_M1A_NPU0_SFU_ELEM_BRD0_t SFU_ELEM_BRD0;  /* NPU0 0x12c */
    DX_M1A_NPU0_SFU_PAF_DISABLE_t SFU_PAF_DISABLE;  /* NPU0 0x130 */
    DX_M1A_NPU0_PRE0_I2C0_t PRE0_I2C0;  /* NPU0 0x134 */
    DX_M1A_NPU0_PRE0_I2C1_t PRE0_I2C1;  /* NPU0 0x138 */
    DX_M1A_NPU0_PRE0_I2C2_t PRE0_I2C2;  /* NPU0 0x13c */
    DX_M1A_NPU0_PRE0_I2C3_t PRE0_I2C3;  /* NPU0 0x140 */
    DX_M1A_NPU0_PRE0_FMT0_t PRE0_FMT0;  /* NPU0 0x144 */
    DX_M1A_NPU0_PRE0_FMT1_t PRE0_FMT1;  /* NPU0 0x148 */
    DX_M1A_NPU0_PE0_ADDR_GEN0_t PE0_ADDR_GEN0;  /* NPU0 0x14c */
    DX_M1A_NPU0_PE0_ADDR_GEN1_t PE0_ADDR_GEN1;  /* NPU0 0x150 */
    DX_M1A_NPU0_PE0_ADDR_GEN2_t PE0_ADDR_GEN2;  /* NPU0 0x154 */
    DX_M1A_NPU0_PE0_ADDR_GEN3_t PE0_ADDR_GEN3;  /* NPU0 0x158 */
    DX_M1A_NPU0_PE0_ADDR_GEN4_t PE0_ADDR_GEN4;  /* NPU0 0x15c */
    DX_M1A_NPU0_PE0_ADDR_GEN5_t PE0_ADDR_GEN5;  /* NPU0 0x160 */
    DX_M1A_NPU0_PE0_ADDR_GEN6_t PE0_ADDR_GEN6;  /* NPU0 0x164 */
    DX_M1A_NPU0_PE0_ADDR_GEN7_t PE0_ADDR_GEN7;  /* NPU0 0x168 */
    DX_M1A_NPU0_PE0_ADDR_GEN8_t PE0_ADDR_GEN8;  /* NPU0 0x16c */
    DX_M1A_NPU0_PE0_CTRL_t PE0_CTRL;  /* NPU0 0x170 */
    DX_M1A_NPU0_PE0_CFG0_t PE0_CFG0;  /* NPU0 0x174 */
    DX_M1A_NPU0_PE0_CFG1_t PE0_CFG1;  /* NPU0 0x178 */
    DX_M1A_NPU0_PE0_CFG2_t PE0_CFG2;  /* NPU0 0x17c */
    DX_M1A_NPU0_PE0_CFG3_t PE0_CFG3;  /* NPU0 0x180 */
    DX_M1A_NPU0_PE0_CFG4_t PE0_CFG4;  /* NPU0 0x184 */
    DX_M1A_NPU0_PE0_CFG5_t PE0_CFG5;  /* NPU0 0x188 */
    DX_M1A_NPU0_PE0_CFG6_t PE0_CFG6;  /* NPU0 0x18c */
    DX_M1A_NPU0_PE0_CFG7_t PE0_CFG7;  /* NPU0 0x190 */
    DX_M1A_NPU0_PE0_CFG8_t PE0_CFG8;  /* NPU0 0x194 */
    DX_M1A_NPU0_PE0_CFG9_t PE0_CFG9;  /* NPU0 0x198 */
    DX_M1A_NPU0_PE0_CFG10_t PE0_CFG10;  /* NPU0 0x19c */
    DX_M1A_NPU0_PE0_CFG11_t PE0_CFG11;  /* NPU0 0x1a0 */
    DX_M1A_NPU0_PE0_CFG12_t PE0_CFG12;  /* NPU0 0x1a4 */
    DX_M1A_NPU0_PE0_CFG13_t PE0_CFG13;  /* NPU0 0x1a8 */
    DX_M1A_NPU0_PE0_CFG14_t PE0_CFG14;  /* NPU0 0x1ac */
    DX_M1A_NPU0_PE0_CFG15_t PE0_CFG15;  /* NPU0 0x1b0 */
    DX_M1A_NPU0_PE0_CFG16_t PE0_CFG16;  /* NPU0 0x1b4 */
    DX_M1A_NPU0_PE0_CFG17_t PE0_CFG17;  /* NPU0 0x1b8 */
    DX_M1A_NPU0_PE0_CFG18_t PE0_CFG18;  /* NPU0 0x1bc */
    DX_M1A_NPU0_PE0_CFG19_t PE0_CFG19;  /* NPU0 0x1c0 */
    DX_M1A_NPU0_PE0_CFG20_t PE0_CFG20;  /* NPU0 0x1c4 */
    DX_M1A_NPU0_PE0_CFG21_t PE0_CFG21;  /* NPU0 0x1c8 */
    DX_M1A_NPU0_PE0_CFG22_t PE0_CFG22;  /* NPU0 0x1cc */
    DX_M1A_NPU0_PE0_CFG23_t PE0_CFG23;  /* NPU0 0x1d0 */
    DX_M1A_NPU0_PE0_CFG24_t PE0_CFG24;  /* NPU0 0x1d4 */
    DX_M1A_NPU0_PE0_CFG25_t PE0_CFG25;  /* NPU0 0x1d8 */
    DX_M1A_NPU0_PE0_CFG26_t PE0_CFG26;  /* NPU0 0x1dc */
    DX_M1A_NPU0_PE0_VMEM_CFG0_t PE0_VMEM_CFG0;  /* NPU0 0x1e0 */
    DX_M1A_NPU0_REDUCE0_CFG0_t REDUCE0_CFG0;  /* NPU0 0x1e4 */
    DX_M1A_NPU0_REDUCE0_CFG1_t REDUCE0_CFG1;  /* NPU0 0x1e8 */
    DX_M1A_NPU0_REDUCE0_CFG2_t REDUCE0_CFG2;  /* NPU0 0x1ec */
    DX_M1A_NPU0_REDUCE0_CFG3_t REDUCE0_CFG3;  /* NPU0 0x1f0 */
    DX_M1A_NPU0_PE0_DMA_CTRL_t PE0_DMA_CTRL;  /* NPU0 0x1f4 */
    DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_t PE0_DMA_ELEM_CFG0;  /* NPU0 0x1f8 */
    DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_t PE0_DMA_ELEM_CFG1;  /* NPU0 0x1fc */
    DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_t PE0_DMA_ELEM_CFG2;  /* NPU0 0x200 */
    DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_t PE0_DMA_ELEM_CFG3;  /* NPU0 0x204 */
    DX_M1A_NPU0_PE0_DMA_W_CFG0_t PE0_DMA_W_CFG0;  /* NPU0 0x208 */
    DX_M1A_NPU0_PE0_DMA_W_CFG1_t PE0_DMA_W_CFG1;  /* NPU0 0x20c */
    DX_M1A_NPU0_PE0_DMA_W_CFG2_t PE0_DMA_W_CFG2;  /* NPU0 0x210 */
    DX_M1A_NPU0_PE0_DMA_W_CFG3_t PE0_DMA_W_CFG3;  /* NPU0 0x214 */
    DX_M1A_NPU0_PE0_DMA_INF_CFG0_t PE0_DMA_INF_CFG0;  /* NPU0 0x218 */
    DX_M1A_NPU0_PE0_DMA_INF_CFG1_t PE0_DMA_INF_CFG1;  /* NPU0 0x21c */
    DX_M1A_NPU0_PE0_DMA_INF_CFG2_t PE0_DMA_INF_CFG2;  /* NPU0 0x220 */
    DX_M1A_NPU0_PE0_DMA_INF_CFG3_t PE0_DMA_INF_CFG3;  /* NPU0 0x224 */
    DX_M1A_NPU0_PE0_DMA_INF_CFG4_t PE0_DMA_INF_CFG4;  /* NPU0 0x228 */
    DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_t PE0_DMA_OUTF_CFG0;  /* NPU0 0x22c */
    DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_t PE0_DMA_OUTF_CFG1;  /* NPU0 0x230 */
    DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_t PE0_DMA_OUTF_CFG2;  /* NPU0 0x234 */
    DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_t PE0_DMA_OUTF_CFG3;  /* NPU0 0x238 */
    DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_t PE0_DMA_OUTF_CFG4;  /* NPU0 0x23c */
    DX_M1A_NPU0_PE0_DMA_RF_CFG0_t PE0_DMA_RF_CFG0;  /* NPU0 0x240 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG1_t PE0_DMA_RF_CFG1;  /* NPU0 0x244 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG2_t PE0_DMA_RF_CFG2;  /* NPU0 0x248 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG3_t PE0_DMA_RF_CFG3;  /* NPU0 0x24c */
    DX_M1A_NPU0_PE0_DMA_RF_CFG4_t PE0_DMA_RF_CFG4;  /* NPU0 0x250 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG5_t PE0_DMA_RF_CFG5;  /* NPU0 0x254 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG6_t PE0_DMA_RF_CFG6;  /* NPU0 0x258 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG7_t PE0_DMA_RF_CFG7;  /* NPU0 0x25c */
    DX_M1A_NPU0_PE0_DMA_RF_CFG8_t PE0_DMA_RF_CFG8;  /* NPU0 0x260 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG9_t PE0_DMA_RF_CFG9;  /* NPU0 0x264 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG10_t PE0_DMA_RF_CFG10;  /* NPU0 0x268 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG11_t PE0_DMA_RF_CFG11;  /* NPU0 0x26c */
    DX_M1A_NPU0_PE0_DMA_RF_CFG12_t PE0_DMA_RF_CFG12;  /* NPU0 0x270 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG13_t PE0_DMA_RF_CFG13;  /* NPU0 0x274 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG14_t PE0_DMA_RF_CFG14;  /* NPU0 0x278 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG15_t PE0_DMA_RF_CFG15;  /* NPU0 0x27c */
    DX_M1A_NPU0_PE0_DMA_RF_CFG16_t PE0_DMA_RF_CFG16;  /* NPU0 0x280 */
    DX_M1A_NPU0_PE0_DMA_RF_CFG17_t PE0_DMA_RF_CFG17;  /* NPU0 0x284 */
    DX_M1A_NPU0_PRE1_I2C0_t PRE1_I2C0;  /* NPU0 0x288 */
    DX_M1A_NPU0_PRE1_I2C1_t PRE1_I2C1;  /* NPU0 0x28c */
    DX_M1A_NPU0_PRE1_I2C2_t PRE1_I2C2;  /* NPU0 0x290 */
    DX_M1A_NPU0_PRE1_I2C3_t PRE1_I2C3;  /* NPU0 0x294 */
    DX_M1A_NPU0_PRE1_FMT0_t PRE1_FMT0;  /* NPU0 0x298 */
    DX_M1A_NPU0_PRE1_FMT1_t PRE1_FMT1;  /* NPU0 0x29c */
    DX_M1A_NPU0_PE1_ADDR_GEN0_t PE1_ADDR_GEN0;  /* NPU0 0x2a0 */
    DX_M1A_NPU0_PE1_ADDR_GEN1_t PE1_ADDR_GEN1;  /* NPU0 0x2a4 */
    DX_M1A_NPU0_PE1_ADDR_GEN2_t PE1_ADDR_GEN2;  /* NPU0 0x2a8 */
    DX_M1A_NPU0_PE1_ADDR_GEN3_t PE1_ADDR_GEN3;  /* NPU0 0x2ac */
    DX_M1A_NPU0_PE1_ADDR_GEN4_t PE1_ADDR_GEN4;  /* NPU0 0x2b0 */
    DX_M1A_NPU0_PE1_ADDR_GEN5_t PE1_ADDR_GEN5;  /* NPU0 0x2b4 */
    DX_M1A_NPU0_PE1_ADDR_GEN6_t PE1_ADDR_GEN6;  /* NPU0 0x2b8 */
    DX_M1A_NPU0_PE1_ADDR_GEN7_t PE1_ADDR_GEN7;  /* NPU0 0x2bc */
    DX_M1A_NPU0_PE1_ADDR_GEN8_t PE1_ADDR_GEN8;  /* NPU0 0x2c0 */
    DX_M1A_NPU0_PE1_CTRL_t PE1_CTRL;  /* NPU0 0x2c4 */
    DX_M1A_NPU0_PE1_CFG0_t PE1_CFG0;  /* NPU0 0x2c8 */
    DX_M1A_NPU0_PE1_CFG1_t PE1_CFG1;  /* NPU0 0x2cc */
    DX_M1A_NPU0_PE1_CFG2_t PE1_CFG2;  /* NPU0 0x2d0 */
    DX_M1A_NPU0_PE1_CFG3_t PE1_CFG3;  /* NPU0 0x2d4 */
    DX_M1A_NPU0_PE1_CFG4_t PE1_CFG4;  /* NPU0 0x2d8 */
    DX_M1A_NPU0_PE1_CFG5_t PE1_CFG5;  /* NPU0 0x2dc */
    DX_M1A_NPU0_PE1_CFG6_t PE1_CFG6;  /* NPU0 0x2e0 */
    DX_M1A_NPU0_PE1_CFG7_t PE1_CFG7;  /* NPU0 0x2e4 */
    DX_M1A_NPU0_PE1_CFG8_t PE1_CFG8;  /* NPU0 0x2e8 */
    DX_M1A_NPU0_PE1_CFG9_t PE1_CFG9;  /* NPU0 0x2ec */
    DX_M1A_NPU0_PE1_CFG10_t PE1_CFG10;  /* NPU0 0x2f0 */
    DX_M1A_NPU0_PE1_CFG11_t PE1_CFG11;  /* NPU0 0x2f4 */
    DX_M1A_NPU0_PE1_CFG12_t PE1_CFG12;  /* NPU0 0x2f8 */
    DX_M1A_NPU0_PE1_CFG13_t PE1_CFG13;  /* NPU0 0x2fc */
    DX_M1A_NPU0_PE1_CFG14_t PE1_CFG14;  /* NPU0 0x300 */
    DX_M1A_NPU0_PE1_CFG15_t PE1_CFG15;  /* NPU0 0x304 */
    DX_M1A_NPU0_PE1_CFG16_t PE1_CFG16;  /* NPU0 0x308 */
    DX_M1A_NPU0_PE1_CFG17_t PE1_CFG17;  /* NPU0 0x30c */
    DX_M1A_NPU0_PE1_CFG18_t PE1_CFG18;  /* NPU0 0x310 */
    DX_M1A_NPU0_PE1_CFG19_t PE1_CFG19;  /* NPU0 0x314 */
    DX_M1A_NPU0_PE1_CFG20_t PE1_CFG20;  /* NPU0 0x318 */
    DX_M1A_NPU0_PE1_CFG21_t PE1_CFG21;  /* NPU0 0x31c */
    DX_M1A_NPU0_PE1_CFG22_t PE1_CFG22;  /* NPU0 0x320 */
    DX_M1A_NPU0_PE1_CFG23_t PE1_CFG23;  /* NPU0 0x324 */
    DX_M1A_NPU0_PE1_CFG24_t PE1_CFG24;  /* NPU0 0x328 */
    DX_M1A_NPU0_PE1_CFG25_t PE1_CFG25;  /* NPU0 0x32c */
    DX_M1A_NPU0_PE1_CFG26_t PE1_CFG26;  /* NPU0 0x330 */
    DX_M1A_NPU0_PE1_VMEM_CFG0_t PE1_VMEM_CFG0;  /* NPU0 0x334 */
    DX_M1A_NPU0_REDUCE1_CFG0_t REDUCE1_CFG0;  /* NPU0 0x338 */
    DX_M1A_NPU0_REDUCE1_CFG1_t REDUCE1_CFG1;  /* NPU0 0x33c */
    DX_M1A_NPU0_REDUCE1_CFG2_t REDUCE1_CFG2;  /* NPU0 0x340 */
    DX_M1A_NPU0_REDUCE1_CFG3_t REDUCE1_CFG3;  /* NPU0 0x344 */
    DX_M1A_NPU0_PE1_DMA_CTRL_t PE1_DMA_CTRL;  /* NPU0 0x348 */
    DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_t PE1_DMA_ELEM_CFG0;  /* NPU0 0x34c */
    DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_t PE1_DMA_ELEM_CFG1;  /* NPU0 0x350 */
    DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_t PE1_DMA_ELEM_CFG2;  /* NPU0 0x354 */
    DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_t PE1_DMA_ELEM_CFG3;  /* NPU0 0x358 */
    DX_M1A_NPU0_PE1_DMA_W_CFG0_t PE1_DMA_W_CFG0;  /* NPU0 0x35c */
    DX_M1A_NPU0_PE1_DMA_W_CFG1_t PE1_DMA_W_CFG1;  /* NPU0 0x360 */
    DX_M1A_NPU0_PE1_DMA_W_CFG2_t PE1_DMA_W_CFG2;  /* NPU0 0x364 */
    DX_M1A_NPU0_PE1_DMA_W_CFG3_t PE1_DMA_W_CFG3;  /* NPU0 0x368 */
    DX_M1A_NPU0_PE1_DMA_INF_CFG0_t PE1_DMA_INF_CFG0;  /* NPU0 0x36c */
    DX_M1A_NPU0_PE1_DMA_INF_CFG1_t PE1_DMA_INF_CFG1;  /* NPU0 0x370 */
    DX_M1A_NPU0_PE1_DMA_INF_CFG2_t PE1_DMA_INF_CFG2;  /* NPU0 0x374 */
    DX_M1A_NPU0_PE1_DMA_INF_CFG3_t PE1_DMA_INF_CFG3;  /* NPU0 0x378 */
    DX_M1A_NPU0_PE1_DMA_INF_CFG4_t PE1_DMA_INF_CFG4;  /* NPU0 0x37c */
    DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_t PE1_DMA_OUTF_CFG0;  /* NPU0 0x380 */
    DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_t PE1_DMA_OUTF_CFG1;  /* NPU0 0x384 */
    DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_t PE1_DMA_OUTF_CFG2;  /* NPU0 0x388 */
    DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_t PE1_DMA_OUTF_CFG3;  /* NPU0 0x38c */
    DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_t PE1_DMA_OUTF_CFG4;  /* NPU0 0x390 */
    DX_M1A_NPU0_PPU_CFG_0_t PPU_CFG_0;  /* NPU0 0x394 */
    DX_M1A_NPU0_PPU_CFG_1_t PPU_CFG_1;  /* NPU0 0x398 */
    DX_M1A_NPU0_PPU_CFG_2_t PPU_CFG_2;  /* NPU0 0x39c */
    DX_M1A_NPU0_PPU_CFG_3_t PPU_CFG_3;  /* NPU0 0x3a0 */
    DX_M1A_NPU0_PPU_CFG_4_t PPU_CFG_4;  /* NPU0 0x3a4 */
    DX_M1A_NPU0_PPU_CFG_6_t PPU_CFG_6;  /* NPU0 0x3a8 */
    DX_M1A_NPU0_PPU_CFG_7_t PPU_CFG_7;  /* NPU0 0x3ac */
    DX_M1A_NPU0_PPU_CFG_9_t PPU_CFG_9;  /* NPU0 0x3b0 */
    DX_M1A_NPU0_CHECKSUM_t CHECKSUM;  /* NPU0 0x3b4 */
} dxDX_M1A_NPU0_t;

/* Register access API */
/* DX_M1A_NPU0.DATA_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_DATA_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_DATA_CFG0)
#define WRITE_DX_M1A_NPU0_DATA_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_DATA_CFG0, val)
/* DX_M1A_NPU0.DATA_CFG0.FEATURE_BIT_WIDTH (Category: NPU0) */
#define DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_LEN        8
#define DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_OFFSET     0
#define DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_DATA_CFG0, DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_MASK)
#define READ_DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_DATA_CFG0, DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_MASK, DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_OFFSET)
#define WRITE_DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_DATA_CFG0, bits, DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_MASK, DX_M1A_NPU0_DATA_CFG0_FEATURE_BIT_WIDTH_OFFSET)

/* DX_M1A_NPU0.DATA_CFG0.W_FEATURE_BIT_WIDTH (Category: NPU0) */
#define DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH_LEN        8
#define DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH_OFFSET     8
#define DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_DATA_CFG0, DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH_MASK)
#define READ_DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_DATA_CFG0, DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH_MASK, DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH_OFFSET)
#define WRITE_DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_DATA_CFG0, bits, DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH_MASK, DX_M1A_NPU0_DATA_CFG0_W_FEATURE_BIT_WIDTH_OFFSET)

/* DX_M1A_NPU0.DATA_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_DATA_CFG0_RESERVED0_LEN        16
#define DX_M1A_NPU0_DATA_CFG0_RESERVED0_OFFSET     16
#define DX_M1A_NPU0_DATA_CFG0_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_DATA_CFG0_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_DATA_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_DATA_CFG0, DX_M1A_NPU0_DATA_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_DATA_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_DATA_CFG0, DX_M1A_NPU0_DATA_CFG0_RESERVED0_MASK, DX_M1A_NPU0_DATA_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_DATA_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_DATA_CFG0, bits, DX_M1A_NPU0_DATA_CFG0_RESERVED0_MASK, DX_M1A_NPU0_DATA_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.CTRL (Category: NPU0) */
#define READ_DX_M1A_NPU0_CTRL(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_CTRL)
#define WRITE_DX_M1A_NPU0_CTRL(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_CTRL, val)
/* DX_M1A_NPU0.CTRL.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_CTRL_RESERVED0_LEN        6
#define DX_M1A_NPU0_CTRL_RESERVED0_OFFSET     0
#define DX_M1A_NPU0_CTRL_RESERVED0_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_CTRL_RESERVED0_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_M1A_NPU0_CTRL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_CTRL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_RESERVED0_MASK, DX_M1A_NPU0_CTRL_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_CTRL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_CTRL, bits, DX_M1A_NPU0_CTRL_RESERVED0_MASK, DX_M1A_NPU0_CTRL_RESERVED0_OFFSET)

/* DX_M1A_NPU0.CTRL.RESIZE (Category: NPU0) */
#define DX_M1A_NPU0_CTRL_RESIZE_LEN        2
#define DX_M1A_NPU0_CTRL_RESIZE_OFFSET     6
#define DX_M1A_NPU0_CTRL_RESIZE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_CTRL_RESIZE_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_M1A_NPU0_CTRL_RESIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_RESIZE_MASK)
#define READ_DX_M1A_NPU0_CTRL_RESIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_RESIZE_MASK, DX_M1A_NPU0_CTRL_RESIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_CTRL_RESIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_CTRL, bits, DX_M1A_NPU0_CTRL_RESIZE_MASK, DX_M1A_NPU0_CTRL_RESIZE_OFFSET)

/* DX_M1A_NPU0.CTRL.WRITE_POOL_ONLY (Category: NPU0) */
#define DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY_LEN        1
#define DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY_OFFSET     8
#define DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY_MASK)
#define READ_DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY_MASK, DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY_OFFSET)
#define WRITE_DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_CTRL, bits, DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY_MASK, DX_M1A_NPU0_CTRL_WRITE_POOL_ONLY_OFFSET)

/* DX_M1A_NPU0.CTRL.WEIGHT_BROADCAST_MODE_EN (Category: NPU0) */
#define DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN_LEN        2
#define DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN_OFFSET     9
#define DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN_MASK       0x00000600 /* [10:9] 0b00000000000000000000011000000000 */
#define READ_MASK_DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN_MASK)
#define READ_DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN_MASK, DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_CTRL, bits, DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN_MASK, DX_M1A_NPU0_CTRL_WEIGHT_BROADCAST_MODE_EN_OFFSET)

/* DX_M1A_NPU0.CTRL.FEATURE_BROADCAST_MODE_EN (Category: NPU0) */
#define DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN_LEN        2
#define DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN_OFFSET     11
#define DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN_MASK       0x00001800 /* [12:11] 0b00000000000000000001100000000000 */
#define READ_MASK_DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN_MASK)
#define READ_DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN_MASK, DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_CTRL, bits, DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN_MASK, DX_M1A_NPU0_CTRL_FEATURE_BROADCAST_MODE_EN_OFFSET)

/* DX_M1A_NPU0.CTRL.ACTIVE_CNT (Category: NPU0) */
#define DX_M1A_NPU0_CTRL_ACTIVE_CNT_LEN        8
#define DX_M1A_NPU0_CTRL_ACTIVE_CNT_OFFSET     13
#define DX_M1A_NPU0_CTRL_ACTIVE_CNT_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_CTRL_ACTIVE_CNT_MASK       0x001fe000 /* [20:13] 0b00000000000111111110000000000000 */
#define READ_MASK_DX_M1A_NPU0_CTRL_ACTIVE_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_ACTIVE_CNT_MASK)
#define READ_DX_M1A_NPU0_CTRL_ACTIVE_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_ACTIVE_CNT_MASK, DX_M1A_NPU0_CTRL_ACTIVE_CNT_OFFSET)
#define WRITE_DX_M1A_NPU0_CTRL_ACTIVE_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_CTRL, bits, DX_M1A_NPU0_CTRL_ACTIVE_CNT_MASK, DX_M1A_NPU0_CTRL_ACTIVE_CNT_OFFSET)

/* DX_M1A_NPU0.CTRL.ACTIVE_PERIOD (Category: NPU0) */
#define DX_M1A_NPU0_CTRL_ACTIVE_PERIOD_LEN        8
#define DX_M1A_NPU0_CTRL_ACTIVE_PERIOD_OFFSET     21
#define DX_M1A_NPU0_CTRL_ACTIVE_PERIOD_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_CTRL_ACTIVE_PERIOD_MASK       0x1fe00000 /* [28:21] 0b00011111111000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_CTRL_ACTIVE_PERIOD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_ACTIVE_PERIOD_MASK)
#define READ_DX_M1A_NPU0_CTRL_ACTIVE_PERIOD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_ACTIVE_PERIOD_MASK, DX_M1A_NPU0_CTRL_ACTIVE_PERIOD_OFFSET)
#define WRITE_DX_M1A_NPU0_CTRL_ACTIVE_PERIOD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_CTRL, bits, DX_M1A_NPU0_CTRL_ACTIVE_PERIOD_MASK, DX_M1A_NPU0_CTRL_ACTIVE_PERIOD_OFFSET)

/* DX_M1A_NPU0.CTRL.FEATURE_DATA_TYPE (Category: NPU0) */
#define DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE_LEN        2
#define DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE_OFFSET     29
#define DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE_MASK       0x60000000 /* [30:29] 0b01100000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE_MASK)
#define READ_DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE_MASK, DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE_OFFSET)
#define WRITE_DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_CTRL, bits, DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE_MASK, DX_M1A_NPU0_CTRL_FEATURE_DATA_TYPE_OFFSET)

/* DX_M1A_NPU0.CTRL.INPUT_DONE (Category: NPU0) */
#define DX_M1A_NPU0_CTRL_INPUT_DONE_LEN        1
#define DX_M1A_NPU0_CTRL_INPUT_DONE_OFFSET     31
#define DX_M1A_NPU0_CTRL_INPUT_DONE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_CTRL_INPUT_DONE_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_CTRL_INPUT_DONE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_INPUT_DONE_MASK)
#define READ_DX_M1A_NPU0_CTRL_INPUT_DONE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_CTRL, DX_M1A_NPU0_CTRL_INPUT_DONE_MASK, DX_M1A_NPU0_CTRL_INPUT_DONE_OFFSET)
#define WRITE_DX_M1A_NPU0_CTRL_INPUT_DONE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_CTRL, bits, DX_M1A_NPU0_CTRL_INPUT_DONE_MASK, DX_M1A_NPU0_CTRL_INPUT_DONE_OFFSET)

/* DX_M1A_NPU0.MISC (Category: NPU0) */
#define READ_DX_M1A_NPU0_MISC(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_MISC)
#define WRITE_DX_M1A_NPU0_MISC(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_MISC, val)
/* DX_M1A_NPU0.MISC.TILE_NUM (Category: NPU0) */
#define DX_M1A_NPU0_MISC_TILE_NUM_LEN        16
#define DX_M1A_NPU0_MISC_TILE_NUM_OFFSET     0
#define DX_M1A_NPU0_MISC_TILE_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_MISC_TILE_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_MISC_TILE_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_MISC, DX_M1A_NPU0_MISC_TILE_NUM_MASK)
#define READ_DX_M1A_NPU0_MISC_TILE_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_MISC, DX_M1A_NPU0_MISC_TILE_NUM_MASK, DX_M1A_NPU0_MISC_TILE_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_MISC_TILE_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_MISC, bits, DX_M1A_NPU0_MISC_TILE_NUM_MASK, DX_M1A_NPU0_MISC_TILE_NUM_OFFSET)

/* DX_M1A_NPU0.MISC.LAYER_NUM (Category: NPU0) */
#define DX_M1A_NPU0_MISC_LAYER_NUM_LEN        16
#define DX_M1A_NPU0_MISC_LAYER_NUM_OFFSET     16
#define DX_M1A_NPU0_MISC_LAYER_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_MISC_LAYER_NUM_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_MISC_LAYER_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_MISC, DX_M1A_NPU0_MISC_LAYER_NUM_MASK)
#define READ_DX_M1A_NPU0_MISC_LAYER_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_MISC, DX_M1A_NPU0_MISC_LAYER_NUM_MASK, DX_M1A_NPU0_MISC_LAYER_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_MISC_LAYER_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_MISC, bits, DX_M1A_NPU0_MISC_LAYER_NUM_MASK, DX_M1A_NPU0_MISC_LAYER_NUM_OFFSET)

/* DX_M1A_NPU0.PSEUDO (Category: NPU0) */
#define READ_DX_M1A_NPU0_PSEUDO(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PSEUDO)
#define WRITE_DX_M1A_NPU0_PSEUDO(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PSEUDO, val)
/* DX_M1A_NPU0.PSEUDO.PSEUDO_CNT (Category: NPU0) */
#define DX_M1A_NPU0_PSEUDO_PSEUDO_CNT_LEN        32
#define DX_M1A_NPU0_PSEUDO_PSEUDO_CNT_OFFSET     0
#define DX_M1A_NPU0_PSEUDO_PSEUDO_CNT_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PSEUDO_PSEUDO_CNT_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PSEUDO_PSEUDO_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PSEUDO, DX_M1A_NPU0_PSEUDO_PSEUDO_CNT_MASK)
#define READ_DX_M1A_NPU0_PSEUDO_PSEUDO_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PSEUDO, DX_M1A_NPU0_PSEUDO_PSEUDO_CNT_MASK, DX_M1A_NPU0_PSEUDO_PSEUDO_CNT_OFFSET)
#define WRITE_DX_M1A_NPU0_PSEUDO_PSEUDO_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PSEUDO, bits, DX_M1A_NPU0_PSEUDO_PSEUDO_CNT_MASK, DX_M1A_NPU0_PSEUDO_PSEUDO_CNT_OFFSET)

/* DX_M1A_NPU0.TRP0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_TRP0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_TRP0)
#define WRITE_DX_M1A_NPU0_TRP0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_TRP0, val)
/* DX_M1A_NPU0.TRP0.TRP_SURFACE_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE_LEN        16
#define DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE_OFFSET     0
#define DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_TRP0, DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE_MASK)
#define READ_DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_TRP0, DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE_MASK, DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_TRP0, bits, DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE_MASK, DX_M1A_NPU0_TRP0_TRP_SURFACE_STRIDE_OFFSET)

/* DX_M1A_NPU0.TRP0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_TRP0_RESERVED0_LEN        14
#define DX_M1A_NPU0_TRP0_RESERVED0_OFFSET     16
#define DX_M1A_NPU0_TRP0_RESERVED0_MASK_BITS  0x00003fff /* [13:0] 0b00000000000000000011111111111111 */
#define DX_M1A_NPU0_TRP0_RESERVED0_MASK       0x3fff0000 /* [29:16] 0b00111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_TRP0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_TRP0, DX_M1A_NPU0_TRP0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_TRP0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_TRP0, DX_M1A_NPU0_TRP0_RESERVED0_MASK, DX_M1A_NPU0_TRP0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_TRP0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_TRP0, bits, DX_M1A_NPU0_TRP0_RESERVED0_MASK, DX_M1A_NPU0_TRP0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.TRP0.TRP_32BIT_MODE (Category: NPU0) */
#define DX_M1A_NPU0_TRP0_TRP_32BIT_MODE_LEN        1
#define DX_M1A_NPU0_TRP0_TRP_32BIT_MODE_OFFSET     30
#define DX_M1A_NPU0_TRP0_TRP_32BIT_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_TRP0_TRP_32BIT_MODE_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_TRP0_TRP_32BIT_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_TRP0, DX_M1A_NPU0_TRP0_TRP_32BIT_MODE_MASK)
#define READ_DX_M1A_NPU0_TRP0_TRP_32BIT_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_TRP0, DX_M1A_NPU0_TRP0_TRP_32BIT_MODE_MASK, DX_M1A_NPU0_TRP0_TRP_32BIT_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_TRP0_TRP_32BIT_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_TRP0, bits, DX_M1A_NPU0_TRP0_TRP_32BIT_MODE_MASK, DX_M1A_NPU0_TRP0_TRP_32BIT_MODE_OFFSET)

/* DX_M1A_NPU0.TRP0.TRP_EN (Category: NPU0) */
#define DX_M1A_NPU0_TRP0_TRP_EN_LEN        1
#define DX_M1A_NPU0_TRP0_TRP_EN_OFFSET     31
#define DX_M1A_NPU0_TRP0_TRP_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_TRP0_TRP_EN_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_TRP0_TRP_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_TRP0, DX_M1A_NPU0_TRP0_TRP_EN_MASK)
#define READ_DX_M1A_NPU0_TRP0_TRP_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_TRP0, DX_M1A_NPU0_TRP0_TRP_EN_MASK, DX_M1A_NPU0_TRP0_TRP_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_TRP0_TRP_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_TRP0, bits, DX_M1A_NPU0_TRP0_TRP_EN_MASK, DX_M1A_NPU0_TRP0_TRP_EN_OFFSET)

/* DX_M1A_NPU0.TRP1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_TRP1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_TRP1)
#define WRITE_DX_M1A_NPU0_TRP1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_TRP1, val)
/* DX_M1A_NPU0.TRP1.TRP_TOTAL_WSIZE (Category: NPU0) */
#define DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE_LEN        16
#define DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE_OFFSET     0
#define DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_TRP1, DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE_MASK)
#define READ_DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_TRP1, DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE_MASK, DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_TRP1, bits, DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE_MASK, DX_M1A_NPU0_TRP1_TRP_TOTAL_WSIZE_OFFSET)

/* DX_M1A_NPU0.TRP1.TRP_IN_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_TRP1_TRP_IN_SIZE_LEN        16
#define DX_M1A_NPU0_TRP1_TRP_IN_SIZE_OFFSET     16
#define DX_M1A_NPU0_TRP1_TRP_IN_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_TRP1_TRP_IN_SIZE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_TRP1_TRP_IN_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_TRP1, DX_M1A_NPU0_TRP1_TRP_IN_SIZE_MASK)
#define READ_DX_M1A_NPU0_TRP1_TRP_IN_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_TRP1, DX_M1A_NPU0_TRP1_TRP_IN_SIZE_MASK, DX_M1A_NPU0_TRP1_TRP_IN_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_TRP1_TRP_IN_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_TRP1, bits, DX_M1A_NPU0_TRP1_TRP_IN_SIZE_MASK, DX_M1A_NPU0_TRP1_TRP_IN_SIZE_OFFSET)

/* DX_M1A_NPU0.TRP2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_TRP2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_TRP2)
#define WRITE_DX_M1A_NPU0_TRP2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_TRP2, val)
/* DX_M1A_NPU0.TRP2.TRP_ADDR_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE_LEN        16
#define DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE_OFFSET     0
#define DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_TRP2, DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE_MASK)
#define READ_DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_TRP2, DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE_MASK, DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_TRP2, bits, DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE_MASK, DX_M1A_NPU0_TRP2_TRP_ADDR_STRIDE_OFFSET)

/* DX_M1A_NPU0.TRP2.TRP_ITER (Category: NPU0) */
#define DX_M1A_NPU0_TRP2_TRP_ITER_LEN        9
#define DX_M1A_NPU0_TRP2_TRP_ITER_OFFSET     16
#define DX_M1A_NPU0_TRP2_TRP_ITER_MASK_BITS  0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define DX_M1A_NPU0_TRP2_TRP_ITER_MASK       0x01ff0000 /* [24:16] 0b00000001111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_TRP2_TRP_ITER(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_TRP2, DX_M1A_NPU0_TRP2_TRP_ITER_MASK)
#define READ_DX_M1A_NPU0_TRP2_TRP_ITER(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_TRP2, DX_M1A_NPU0_TRP2_TRP_ITER_MASK, DX_M1A_NPU0_TRP2_TRP_ITER_OFFSET)
#define WRITE_DX_M1A_NPU0_TRP2_TRP_ITER(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_TRP2, bits, DX_M1A_NPU0_TRP2_TRP_ITER_MASK, DX_M1A_NPU0_TRP2_TRP_ITER_OFFSET)

/* DX_M1A_NPU0.TRP2.TRP_LAST_WSIZE (Category: NPU0) */
#define DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE_LEN        7
#define DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE_OFFSET     25
#define DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE_MASK       0xfe000000 /* [31:25] 0b11111110000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_TRP2, DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE_MASK)
#define READ_DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_TRP2, DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE_MASK, DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_TRP2, bits, DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE_MASK, DX_M1A_NPU0_TRP2_TRP_LAST_WSIZE_OFFSET)

/* DX_M1A_NPU0.TRP3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_TRP3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_TRP3)
#define WRITE_DX_M1A_NPU0_TRP3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_TRP3, val)
/* DX_M1A_NPU0.TRP3.TRP_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_TRP3, DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_TRP3, DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR_MASK, DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_TRP3, bits, DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR_MASK, DX_M1A_NPU0_TRP3_TRP_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.TRP4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_TRP4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_TRP4)
#define WRITE_DX_M1A_NPU0_TRP4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_TRP4, val)
/* DX_M1A_NPU0.TRP4.TRP_WRITE_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_TRP4, DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_TRP4, DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR_MASK, DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_TRP4, bits, DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR_MASK, DX_M1A_NPU0_TRP4_TRP_WRITE_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.ARG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_ARG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_ARG0)
#define WRITE_DX_M1A_NPU0_ARG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_ARG0, val)
/* DX_M1A_NPU0.ARG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_ARG0_RESERVED0_LEN        13
#define DX_M1A_NPU0_ARG0_RESERVED0_OFFSET     0
#define DX_M1A_NPU0_ARG0_RESERVED0_MASK_BITS  0x00001fff /* [12:0] 0b00000000000000000001111111111111 */
#define DX_M1A_NPU0_ARG0_RESERVED0_MASK       0x00001fff /* [12:0] 0b00000000000000000001111111111111 */
#define READ_MASK_DX_M1A_NPU0_ARG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_ARG0, DX_M1A_NPU0_ARG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_ARG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_ARG0, DX_M1A_NPU0_ARG0_RESERVED0_MASK, DX_M1A_NPU0_ARG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_ARG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_ARG0, bits, DX_M1A_NPU0_ARG0_RESERVED0_MASK, DX_M1A_NPU0_ARG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.ARG0.ARG_MAX_MODE (Category: NPU0) */
#define DX_M1A_NPU0_ARG0_ARG_MAX_MODE_LEN        2
#define DX_M1A_NPU0_ARG0_ARG_MAX_MODE_OFFSET     13
#define DX_M1A_NPU0_ARG0_ARG_MAX_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_ARG0_ARG_MAX_MODE_MASK       0x00006000 /* [14:13] 0b00000000000000000110000000000000 */
#define READ_MASK_DX_M1A_NPU0_ARG0_ARG_MAX_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_ARG0, DX_M1A_NPU0_ARG0_ARG_MAX_MODE_MASK)
#define READ_DX_M1A_NPU0_ARG0_ARG_MAX_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_ARG0, DX_M1A_NPU0_ARG0_ARG_MAX_MODE_MASK, DX_M1A_NPU0_ARG0_ARG_MAX_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_ARG0_ARG_MAX_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_ARG0, bits, DX_M1A_NPU0_ARG0_ARG_MAX_MODE_MASK, DX_M1A_NPU0_ARG0_ARG_MAX_MODE_OFFSET)

/* DX_M1A_NPU0.ARG0.ARG_CLASS_NUM (Category: NPU0) */
#define DX_M1A_NPU0_ARG0_ARG_CLASS_NUM_LEN        16
#define DX_M1A_NPU0_ARG0_ARG_CLASS_NUM_OFFSET     15
#define DX_M1A_NPU0_ARG0_ARG_CLASS_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_ARG0_ARG_CLASS_NUM_MASK       0x7fff8000 /* [30:15] 0b01111111111111111000000000000000 */
#define READ_MASK_DX_M1A_NPU0_ARG0_ARG_CLASS_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_ARG0, DX_M1A_NPU0_ARG0_ARG_CLASS_NUM_MASK)
#define READ_DX_M1A_NPU0_ARG0_ARG_CLASS_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_ARG0, DX_M1A_NPU0_ARG0_ARG_CLASS_NUM_MASK, DX_M1A_NPU0_ARG0_ARG_CLASS_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_ARG0_ARG_CLASS_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_ARG0, bits, DX_M1A_NPU0_ARG0_ARG_CLASS_NUM_MASK, DX_M1A_NPU0_ARG0_ARG_CLASS_NUM_OFFSET)

/* DX_M1A_NPU0.ARG0.ARG_MAX_OUT_FORMAT (Category: NPU0) */
#define DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT_LEN        1
#define DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT_OFFSET     31
#define DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_ARG0, DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT_MASK)
#define READ_DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_ARG0, DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT_MASK, DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT_OFFSET)
#define WRITE_DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_ARG0, bits, DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT_MASK, DX_M1A_NPU0_ARG0_ARG_MAX_OUT_FORMAT_OFFSET)

/* DX_M1A_NPU0.POOL0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_POOL0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_POOL0)
#define WRITE_DX_M1A_NPU0_POOL0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_POOL0, val)
/* DX_M1A_NPU0.POOL0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_POOL0_RESERVED0_LEN        30
#define DX_M1A_NPU0_POOL0_RESERVED0_OFFSET     0
#define DX_M1A_NPU0_POOL0_RESERVED0_MASK_BITS  0x3fffffff /* [29:0] 0b00111111111111111111111111111111 */
#define DX_M1A_NPU0_POOL0_RESERVED0_MASK       0x3fffffff /* [29:0] 0b00111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_POOL0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_POOL0, DX_M1A_NPU0_POOL0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_POOL0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_POOL0, DX_M1A_NPU0_POOL0_RESERVED0_MASK, DX_M1A_NPU0_POOL0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_POOL0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_POOL0, bits, DX_M1A_NPU0_POOL0_RESERVED0_MASK, DX_M1A_NPU0_POOL0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.POOL0.POOL_MODE (Category: NPU0) */
#define DX_M1A_NPU0_POOL0_POOL_MODE_LEN        2
#define DX_M1A_NPU0_POOL0_POOL_MODE_OFFSET     30
#define DX_M1A_NPU0_POOL0_POOL_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_POOL0_POOL_MODE_MASK       0xc0000000 /* [31:30] 0b11000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_POOL0_POOL_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_POOL0, DX_M1A_NPU0_POOL0_POOL_MODE_MASK)
#define READ_DX_M1A_NPU0_POOL0_POOL_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_POOL0, DX_M1A_NPU0_POOL0_POOL_MODE_MASK, DX_M1A_NPU0_POOL0_POOL_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_POOL0_POOL_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_POOL0, bits, DX_M1A_NPU0_POOL0_POOL_MODE_MASK, DX_M1A_NPU0_POOL0_POOL_MODE_OFFSET)

/* DX_M1A_NPU0.POOL1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_POOL1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_POOL1)
#define WRITE_DX_M1A_NPU0_POOL1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_POOL1, val)
/* DX_M1A_NPU0.POOL1.POOL_WINSIZE (Category: NPU0) */
#define DX_M1A_NPU0_POOL1_POOL_WINSIZE_LEN        32
#define DX_M1A_NPU0_POOL1_POOL_WINSIZE_OFFSET     0
#define DX_M1A_NPU0_POOL1_POOL_WINSIZE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_POOL1_POOL_WINSIZE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_POOL1_POOL_WINSIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_POOL1, DX_M1A_NPU0_POOL1_POOL_WINSIZE_MASK)
#define READ_DX_M1A_NPU0_POOL1_POOL_WINSIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_POOL1, DX_M1A_NPU0_POOL1_POOL_WINSIZE_MASK, DX_M1A_NPU0_POOL1_POOL_WINSIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_POOL1_POOL_WINSIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_POOL1, bits, DX_M1A_NPU0_POOL1_POOL_WINSIZE_MASK, DX_M1A_NPU0_POOL1_POOL_WINSIZE_OFFSET)

/* DX_M1A_NPU0.POOL2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_POOL2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_POOL2)
#define WRITE_DX_M1A_NPU0_POOL2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_POOL2, val)
/* DX_M1A_NPU0.POOL2.AVG_POOL_MULT_COEFF (Category: NPU0) */
#define DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF_LEN        32
#define DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF_OFFSET     0
#define DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_POOL2, DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF_MASK)
#define READ_DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_POOL2, DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF_MASK, DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF_OFFSET)
#define WRITE_DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_POOL2, bits, DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF_MASK, DX_M1A_NPU0_POOL2_AVG_POOL_MULT_COEFF_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_CTRL0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, val)
/* DX_M1A_NPU0.SFU_CTRL0.SFU_HALF_RUN_MODE (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE_OFFSET     0
#define DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE_MASK, DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE_MASK, DX_M1A_NPU0_SFU_CTRL0_SFU_HALF_RUN_MODE_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.SKIP_ADD_SEL (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL_OFFSET     1
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL_MASK       0x00000002 /* [1:1] 0b00000000000000000000000000000010 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL_MASK, DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL_MASK, DX_M1A_NPU0_SFU_CTRL0_SKIP_ADD_SEL_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.SKIP_MUL_SEL (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL_OFFSET     2
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL_MASK       0x00000004 /* [2:2] 0b00000000000000000000000000000100 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL_MASK, DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL_MASK, DX_M1A_NPU0_SFU_CTRL0_SKIP_MUL_SEL_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.FLOAT2INT_MODE (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE_OFFSET     3
#define DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE_MASK       0x00000008 /* [3:3] 0b00000000000000000000000000001000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE_MASK, DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE_MASK, DX_M1A_NPU0_SFU_CTRL0_FLOAT2INT_MODE_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.SKIP_IN_TYPE (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE_OFFSET     4
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE_MASK       0x00000010 /* [4:4] 0b00000000000000000000000000010000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE_MASK, DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE_MASK, DX_M1A_NPU0_SFU_CTRL0_SKIP_IN_TYPE_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.PRE_ACT_POW_EN (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN_OFFSET     5
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN_MASK       0x00000020 /* [5:5] 0b00000000000000000000000000100000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN_MASK, DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN_MASK, DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_POW_EN_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.PRE_ACT_MODE (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE_LEN        3
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE_OFFSET     6
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE_MASK       0x000001c0 /* [8:6] 0b00000000000000000000000111000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE_MASK, DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE_MASK, DX_M1A_NPU0_SFU_CTRL0_PRE_ACT_MODE_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.PRE_MULT_COEFF_SEL (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL_OFFSET     9
#define DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL_MASK       0x00000200 /* [9:9] 0b00000000000000000000001000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL_MASK, DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL_MASK, DX_M1A_NPU0_SFU_CTRL0_PRE_MULT_COEFF_SEL_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.ACT_MODE (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_ACT_MODE_LEN        3
#define DX_M1A_NPU0_SFU_CTRL0_ACT_MODE_OFFSET     10
#define DX_M1A_NPU0_SFU_CTRL0_ACT_MODE_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_NPU0_SFU_CTRL0_ACT_MODE_MASK       0x00001c00 /* [12:10] 0b00000000000000000001110000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_ACT_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_ACT_MODE_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_ACT_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_ACT_MODE_MASK, DX_M1A_NPU0_SFU_CTRL0_ACT_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_ACT_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_ACT_MODE_MASK, DX_M1A_NPU0_SFU_CTRL0_ACT_MODE_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.ACT_INOUT_CTRL (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL_OFFSET     13
#define DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL_MASK       0x00002000 /* [13:13] 0b00000000000000000010000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL_MASK, DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL_MASK, DX_M1A_NPU0_SFU_CTRL0_ACT_INOUT_CTRL_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.POST_ACT_MODE (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE_LEN        2
#define DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE_OFFSET     14
#define DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE_MASK       0x0000c000 /* [15:14] 0b00000000000000001100000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE_MASK, DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE_MASK, DX_M1A_NPU0_SFU_CTRL0_POST_ACT_MODE_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.PRE_ADD_COEFF_SEL (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL_OFFSET     16
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL_MASK       0x00010000 /* [16:16] 0b00000000000000010000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL_MASK, DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL_MASK, DX_M1A_NPU0_SFU_CTRL0_PRE_ADD_COEFF_SEL_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.SE_EN (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_SE_EN_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_SE_EN_OFFSET     17
#define DX_M1A_NPU0_SFU_CTRL0_SE_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_SE_EN_MASK       0x00020000 /* [17:17] 0b00000000000000100000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_SE_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SE_EN_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_SE_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SE_EN_MASK, DX_M1A_NPU0_SFU_CTRL0_SE_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_SE_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_SE_EN_MASK, DX_M1A_NPU0_SFU_CTRL0_SE_EN_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.SE_WRITE (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_SE_WRITE_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_SE_WRITE_OFFSET     18
#define DX_M1A_NPU0_SFU_CTRL0_SE_WRITE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_SE_WRITE_MASK       0x00040000 /* [18:18] 0b00000000000001000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_SE_WRITE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SE_WRITE_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_SE_WRITE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SE_WRITE_MASK, DX_M1A_NPU0_SFU_CTRL0_SE_WRITE_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_SE_WRITE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_SE_WRITE_MASK, DX_M1A_NPU0_SFU_CTRL0_SE_WRITE_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.SFU_OUT_MERGE_NUM (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM_LEN        3
#define DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM_OFFSET     19
#define DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM_MASK       0x00380000 /* [21:19] 0b00000000001110000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM_MASK, DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM_MASK, DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_NUM_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.SFU_OUT_MERGE_EN (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN_LEN        1
#define DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN_OFFSET     22
#define DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN_MASK       0x00400000 /* [22:22] 0b00000000010000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN_MASK, DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN_MASK, DX_M1A_NPU0_SFU_CTRL0_SFU_OUT_MERGE_EN_OFFSET)

/* DX_M1A_NPU0.SFU_CTRL0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_CTRL0_RESERVED0_LEN        9
#define DX_M1A_NPU0_SFU_CTRL0_RESERVED0_OFFSET     23
#define DX_M1A_NPU0_SFU_CTRL0_RESERVED0_MASK_BITS  0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define DX_M1A_NPU0_SFU_CTRL0_RESERVED0_MASK       0xff800000 /* [31:23] 0b11111111100000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_CTRL0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_SFU_CTRL0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, DX_M1A_NPU0_SFU_CTRL0_RESERVED0_MASK, DX_M1A_NPU0_SFU_CTRL0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_CTRL0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_CTRL0, bits, DX_M1A_NPU0_SFU_CTRL0_RESERVED0_MASK, DX_M1A_NPU0_SFU_CTRL0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.SFU_COEFF0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_COEFF0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_COEFF0)
#define WRITE_DX_M1A_NPU0_SFU_COEFF0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_COEFF0, val)
/* DX_M1A_NPU0.SFU_COEFF0.CLIP_MAX (Category: NPU0) */
#define DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX_LEN        32
#define DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX_OFFSET     0
#define DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_COEFF0, DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX_MASK)
#define READ_DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF0, DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX_MASK, DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF0, bits, DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX_MASK, DX_M1A_NPU0_SFU_COEFF0_CLIP_MAX_OFFSET)

/* DX_M1A_NPU0.SFU_COEFF1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_COEFF1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_COEFF1)
#define WRITE_DX_M1A_NPU0_SFU_COEFF1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_COEFF1, val)
/* DX_M1A_NPU0.SFU_COEFF1.CLIP_MIN (Category: NPU0) */
#define DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN_LEN        32
#define DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN_OFFSET     0
#define DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_COEFF1, DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN_MASK)
#define READ_DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF1, DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN_MASK, DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF1, bits, DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN_MASK, DX_M1A_NPU0_SFU_COEFF1_CLIP_MIN_OFFSET)

/* DX_M1A_NPU0.SFU_COEFF2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_COEFF2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_COEFF2)
#define WRITE_DX_M1A_NPU0_SFU_COEFF2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_COEFF2, val)
/* DX_M1A_NPU0.SFU_COEFF2.SKIP_ADD_COEFF (Category: NPU0) */
#define DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF_LEN        32
#define DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF_OFFSET     0
#define DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_COEFF2, DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF_MASK)
#define READ_DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF2, DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF_MASK, DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF2, bits, DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF_MASK, DX_M1A_NPU0_SFU_COEFF2_SKIP_ADD_COEFF_OFFSET)

/* DX_M1A_NPU0.SFU_COEFF3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_COEFF3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_COEFF3)
#define WRITE_DX_M1A_NPU0_SFU_COEFF3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_COEFF3, val)
/* DX_M1A_NPU0.SFU_COEFF3.PRE_ACT_MULT_COEFF (Category: NPU0) */
#define DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF_LEN        32
#define DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF_OFFSET     0
#define DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_COEFF3, DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF_MASK)
#define READ_DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF3, DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF_MASK, DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF3, bits, DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF_MASK, DX_M1A_NPU0_SFU_COEFF3_PRE_ACT_MULT_COEFF_OFFSET)

/* DX_M1A_NPU0.SFU_COEFF4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_COEFF4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_COEFF4)
#define WRITE_DX_M1A_NPU0_SFU_COEFF4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_COEFF4, val)
/* DX_M1A_NPU0.SFU_COEFF4.PRE_ACT_ADD_COEFF (Category: NPU0) */
#define DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF_LEN        32
#define DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF_OFFSET     0
#define DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_COEFF4, DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF_MASK)
#define READ_DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF4, DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF_MASK, DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF4, bits, DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF_MASK, DX_M1A_NPU0_SFU_COEFF4_PRE_ACT_ADD_COEFF_OFFSET)

/* DX_M1A_NPU0.SFU_COEFF5 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_COEFF5(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_COEFF5)
#define WRITE_DX_M1A_NPU0_SFU_COEFF5(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_COEFF5, val)
/* DX_M1A_NPU0.SFU_COEFF5.LMUL_ADD_COEFF (Category: NPU0) */
#define DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF_LEN        32
#define DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF_OFFSET     0
#define DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_COEFF5, DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF_MASK)
#define READ_DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF5, DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF_MASK, DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF5, bits, DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF_MASK, DX_M1A_NPU0_SFU_COEFF5_LMUL_ADD_COEFF_OFFSET)

/* DX_M1A_NPU0.SFU_COEFF6 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_COEFF6(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_COEFF6)
#define WRITE_DX_M1A_NPU0_SFU_COEFF6(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_COEFF6, val)
/* DX_M1A_NPU0.SFU_COEFF6.ACT_OFFSET_VAL (Category: NPU0) */
#define DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL_LEN        32
#define DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL_OFFSET     0
#define DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_COEFF6, DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL_MASK)
#define READ_DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF6, DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL_MASK, DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF6, bits, DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL_MASK, DX_M1A_NPU0_SFU_COEFF6_ACT_OFFSET_VAL_OFFSET)

/* DX_M1A_NPU0.SFU_COEFF7 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_COEFF7(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_COEFF7)
#define WRITE_DX_M1A_NPU0_SFU_COEFF7(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_COEFF7, val)
/* DX_M1A_NPU0.SFU_COEFF7.SKIP_SCALE (Category: NPU0) */
#define DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE_LEN        32
#define DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE_OFFSET     0
#define DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_COEFF7, DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE_MASK)
#define READ_DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF7, DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE_MASK, DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF7, bits, DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE_MASK, DX_M1A_NPU0_SFU_COEFF7_SKIP_SCALE_OFFSET)

/* DX_M1A_NPU0.SFU_COEFF8 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_COEFF8(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_COEFF8)
#define WRITE_DX_M1A_NPU0_SFU_COEFF8(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_COEFF8, val)
/* DX_M1A_NPU0.SFU_COEFF8.LEAKY_RELU_SLOPE (Category: NPU0) */
#define DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE_LEN        32
#define DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE_OFFSET     0
#define DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_COEFF8, DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE_MASK)
#define READ_DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF8, DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE_MASK, DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF8, bits, DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE_MASK, DX_M1A_NPU0_SFU_COEFF8_LEAKY_RELU_SLOPE_OFFSET)

/* DX_M1A_NPU0.SFU_COEFF9 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_COEFF9(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_COEFF9)
#define WRITE_DX_M1A_NPU0_SFU_COEFF9(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_COEFF9, val)
/* DX_M1A_NPU0.SFU_COEFF9.RELU6_CLIP_MAX (Category: NPU0) */
#define DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX_LEN        32
#define DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX_OFFSET     0
#define DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_COEFF9, DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX_MASK)
#define READ_DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF9, DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX_MASK, DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_COEFF9, bits, DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX_MASK, DX_M1A_NPU0_SFU_COEFF9_RELU6_CLIP_MAX_OFFSET)

/* DX_M1A_NPU0.SFU_PAF0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF0)
#define WRITE_DX_M1A_NPU0_SFU_PAF0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF0, val)
/* DX_M1A_NPU0.SFU_PAF0.PAF_BOUNDARY0 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0_LEN        32
#define DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF0, DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF0, DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0_MASK, DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF0, bits, DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0_MASK, DX_M1A_NPU0_SFU_PAF0_PAF_BOUNDARY0_OFFSET)

/* DX_M1A_NPU0.SFU_PAF1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF1)
#define WRITE_DX_M1A_NPU0_SFU_PAF1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF1, val)
/* DX_M1A_NPU0.SFU_PAF1.PAF_BOUNDARY1 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1_LEN        32
#define DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF1, DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF1, DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1_MASK, DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF1, bits, DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1_MASK, DX_M1A_NPU0_SFU_PAF1_PAF_BOUNDARY1_OFFSET)

/* DX_M1A_NPU0.SFU_PAF2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF2)
#define WRITE_DX_M1A_NPU0_SFU_PAF2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF2, val)
/* DX_M1A_NPU0.SFU_PAF2.PAF_BOUNDARY2 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2_LEN        32
#define DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF2, DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF2, DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2_MASK, DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF2, bits, DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2_MASK, DX_M1A_NPU0_SFU_PAF2_PAF_BOUNDARY2_OFFSET)

/* DX_M1A_NPU0.SFU_PAF3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF3)
#define WRITE_DX_M1A_NPU0_SFU_PAF3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF3, val)
/* DX_M1A_NPU0.SFU_PAF3.PAF_BOUNDARY3 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3_LEN        32
#define DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF3, DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF3, DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3_MASK, DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF3, bits, DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3_MASK, DX_M1A_NPU0_SFU_PAF3_PAF_BOUNDARY3_OFFSET)

/* DX_M1A_NPU0.SFU_PAF4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF4)
#define WRITE_DX_M1A_NPU0_SFU_PAF4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF4, val)
/* DX_M1A_NPU0.SFU_PAF4.PAF_BOUNDARY4 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4_LEN        32
#define DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF4, DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF4, DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4_MASK, DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF4, bits, DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4_MASK, DX_M1A_NPU0_SFU_PAF4_PAF_BOUNDARY4_OFFSET)

/* DX_M1A_NPU0.SFU_PAF5 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF5(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF5)
#define WRITE_DX_M1A_NPU0_SFU_PAF5(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF5, val)
/* DX_M1A_NPU0.SFU_PAF5.PAF_BOUNDARY5 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5_LEN        32
#define DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF5, DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF5, DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5_MASK, DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF5, bits, DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5_MASK, DX_M1A_NPU0_SFU_PAF5_PAF_BOUNDARY5_OFFSET)

/* DX_M1A_NPU0.SFU_PAF6 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF6(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF6)
#define WRITE_DX_M1A_NPU0_SFU_PAF6(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF6, val)
/* DX_M1A_NPU0.SFU_PAF6.PAF_BOUNDARY6 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6_LEN        32
#define DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF6, DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF6, DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6_MASK, DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF6, bits, DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6_MASK, DX_M1A_NPU0_SFU_PAF6_PAF_BOUNDARY6_OFFSET)

/* DX_M1A_NPU0.SFU_PAF7 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF7(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF7)
#define WRITE_DX_M1A_NPU0_SFU_PAF7(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF7, val)
/* DX_M1A_NPU0.SFU_PAF7.PAF_BOUNDARY7 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7_LEN        32
#define DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF7, DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF7, DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7_MASK, DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF7, bits, DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7_MASK, DX_M1A_NPU0_SFU_PAF7_PAF_BOUNDARY7_OFFSET)

/* DX_M1A_NPU0.SFU_PAF8 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF8(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF8)
#define WRITE_DX_M1A_NPU0_SFU_PAF8(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF8, val)
/* DX_M1A_NPU0.SFU_PAF8.PAF_BOUNDARY8 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8_LEN        32
#define DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF8, DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF8, DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8_MASK, DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF8, bits, DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8_MASK, DX_M1A_NPU0_SFU_PAF8_PAF_BOUNDARY8_OFFSET)

/* DX_M1A_NPU0.SFU_PAF9 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF9(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF9)
#define WRITE_DX_M1A_NPU0_SFU_PAF9(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF9, val)
/* DX_M1A_NPU0.SFU_PAF9.PAF_BOUNDARY9 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9_LEN        32
#define DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF9, DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF9, DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9_MASK, DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF9, bits, DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9_MASK, DX_M1A_NPU0_SFU_PAF9_PAF_BOUNDARY9_OFFSET)

/* DX_M1A_NPU0.SFU_PAF10 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF10(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF10)
#define WRITE_DX_M1A_NPU0_SFU_PAF10(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF10, val)
/* DX_M1A_NPU0.SFU_PAF10.PAF_BOUNDARY10 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10_LEN        32
#define DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF10, DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF10, DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10_MASK, DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF10, bits, DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10_MASK, DX_M1A_NPU0_SFU_PAF10_PAF_BOUNDARY10_OFFSET)

/* DX_M1A_NPU0.SFU_PAF11 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF11(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF11)
#define WRITE_DX_M1A_NPU0_SFU_PAF11(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF11, val)
/* DX_M1A_NPU0.SFU_PAF11.PAF_BOUNDARY11 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11_LEN        32
#define DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF11, DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF11, DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11_MASK, DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF11, bits, DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11_MASK, DX_M1A_NPU0_SFU_PAF11_PAF_BOUNDARY11_OFFSET)

/* DX_M1A_NPU0.SFU_PAF12 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF12(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF12)
#define WRITE_DX_M1A_NPU0_SFU_PAF12(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF12, val)
/* DX_M1A_NPU0.SFU_PAF12.PAF_BOUNDARY12 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12_LEN        32
#define DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF12, DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF12, DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12_MASK, DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF12, bits, DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12_MASK, DX_M1A_NPU0_SFU_PAF12_PAF_BOUNDARY12_OFFSET)

/* DX_M1A_NPU0.SFU_PAF13 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF13(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF13)
#define WRITE_DX_M1A_NPU0_SFU_PAF13(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF13, val)
/* DX_M1A_NPU0.SFU_PAF13.PAF_BOUNDARY13 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13_LEN        32
#define DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF13, DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF13, DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13_MASK, DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF13, bits, DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13_MASK, DX_M1A_NPU0_SFU_PAF13_PAF_BOUNDARY13_OFFSET)

/* DX_M1A_NPU0.SFU_PAF14 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF14(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF14)
#define WRITE_DX_M1A_NPU0_SFU_PAF14(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF14, val)
/* DX_M1A_NPU0.SFU_PAF14.PAF_BOUNDARY14 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14_LEN        32
#define DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF14, DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF14, DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14_MASK, DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF14, bits, DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14_MASK, DX_M1A_NPU0_SFU_PAF14_PAF_BOUNDARY14_OFFSET)

/* DX_M1A_NPU0.SFU_PAF15 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF15(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF15)
#define WRITE_DX_M1A_NPU0_SFU_PAF15(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF15, val)
/* DX_M1A_NPU0.SFU_PAF15.PAF_MULT_COEFF0 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0_LEN        32
#define DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF15, DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF15, DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0_MASK, DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF15, bits, DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0_MASK, DX_M1A_NPU0_SFU_PAF15_PAF_MULT_COEFF0_OFFSET)

/* DX_M1A_NPU0.SFU_PAF16 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF16(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF16)
#define WRITE_DX_M1A_NPU0_SFU_PAF16(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF16, val)
/* DX_M1A_NPU0.SFU_PAF16.PAF_MULT_COEFF1 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1_LEN        32
#define DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF16, DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF16, DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1_MASK, DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF16, bits, DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1_MASK, DX_M1A_NPU0_SFU_PAF16_PAF_MULT_COEFF1_OFFSET)

/* DX_M1A_NPU0.SFU_PAF17 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF17(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF17)
#define WRITE_DX_M1A_NPU0_SFU_PAF17(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF17, val)
/* DX_M1A_NPU0.SFU_PAF17.PAF_MULT_COEFF2 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2_LEN        32
#define DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF17, DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF17, DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2_MASK, DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF17, bits, DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2_MASK, DX_M1A_NPU0_SFU_PAF17_PAF_MULT_COEFF2_OFFSET)

/* DX_M1A_NPU0.SFU_PAF18 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF18(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF18)
#define WRITE_DX_M1A_NPU0_SFU_PAF18(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF18, val)
/* DX_M1A_NPU0.SFU_PAF18.PAF_MULT_COEFF3 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3_LEN        32
#define DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF18, DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF18, DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3_MASK, DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF18, bits, DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3_MASK, DX_M1A_NPU0_SFU_PAF18_PAF_MULT_COEFF3_OFFSET)

/* DX_M1A_NPU0.SFU_PAF19 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF19(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF19)
#define WRITE_DX_M1A_NPU0_SFU_PAF19(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF19, val)
/* DX_M1A_NPU0.SFU_PAF19.PAF_MULT_COEFF4 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4_LEN        32
#define DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF19, DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF19, DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4_MASK, DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF19, bits, DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4_MASK, DX_M1A_NPU0_SFU_PAF19_PAF_MULT_COEFF4_OFFSET)

/* DX_M1A_NPU0.SFU_PAF20 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF20(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF20)
#define WRITE_DX_M1A_NPU0_SFU_PAF20(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF20, val)
/* DX_M1A_NPU0.SFU_PAF20.PAF_MULT_COEFF5 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5_LEN        32
#define DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF20, DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF20, DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5_MASK, DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF20, bits, DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5_MASK, DX_M1A_NPU0_SFU_PAF20_PAF_MULT_COEFF5_OFFSET)

/* DX_M1A_NPU0.SFU_PAF21 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF21(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF21)
#define WRITE_DX_M1A_NPU0_SFU_PAF21(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF21, val)
/* DX_M1A_NPU0.SFU_PAF21.PAF_MULT_COEFF6 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6_LEN        32
#define DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF21, DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF21, DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6_MASK, DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF21, bits, DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6_MASK, DX_M1A_NPU0_SFU_PAF21_PAF_MULT_COEFF6_OFFSET)

/* DX_M1A_NPU0.SFU_PAF22 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF22(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF22)
#define WRITE_DX_M1A_NPU0_SFU_PAF22(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF22, val)
/* DX_M1A_NPU0.SFU_PAF22.PAF_MULT_COEFF7 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7_LEN        32
#define DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF22, DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF22, DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7_MASK, DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF22, bits, DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7_MASK, DX_M1A_NPU0_SFU_PAF22_PAF_MULT_COEFF7_OFFSET)

/* DX_M1A_NPU0.SFU_PAF23 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF23(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF23)
#define WRITE_DX_M1A_NPU0_SFU_PAF23(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF23, val)
/* DX_M1A_NPU0.SFU_PAF23.PAF_MULT_COEFF8 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8_LEN        32
#define DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF23, DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF23, DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8_MASK, DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF23, bits, DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8_MASK, DX_M1A_NPU0_SFU_PAF23_PAF_MULT_COEFF8_OFFSET)

/* DX_M1A_NPU0.SFU_PAF24 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF24(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF24)
#define WRITE_DX_M1A_NPU0_SFU_PAF24(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF24, val)
/* DX_M1A_NPU0.SFU_PAF24.PAF_MULT_COEFF9 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9_LEN        32
#define DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF24, DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF24, DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9_MASK, DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF24, bits, DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9_MASK, DX_M1A_NPU0_SFU_PAF24_PAF_MULT_COEFF9_OFFSET)

/* DX_M1A_NPU0.SFU_PAF25 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF25(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF25)
#define WRITE_DX_M1A_NPU0_SFU_PAF25(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF25, val)
/* DX_M1A_NPU0.SFU_PAF25.PAF_MULT_COEFF10 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10_LEN        32
#define DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF25, DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF25, DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10_MASK, DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF25, bits, DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10_MASK, DX_M1A_NPU0_SFU_PAF25_PAF_MULT_COEFF10_OFFSET)

/* DX_M1A_NPU0.SFU_PAF26 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF26(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF26)
#define WRITE_DX_M1A_NPU0_SFU_PAF26(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF26, val)
/* DX_M1A_NPU0.SFU_PAF26.PAF_MULT_COEFF11 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11_LEN        32
#define DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF26, DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF26, DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11_MASK, DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF26, bits, DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11_MASK, DX_M1A_NPU0_SFU_PAF26_PAF_MULT_COEFF11_OFFSET)

/* DX_M1A_NPU0.SFU_PAF27 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF27(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF27)
#define WRITE_DX_M1A_NPU0_SFU_PAF27(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF27, val)
/* DX_M1A_NPU0.SFU_PAF27.PAF_MULT_COEFF12 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12_LEN        32
#define DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF27, DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF27, DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12_MASK, DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF27, bits, DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12_MASK, DX_M1A_NPU0_SFU_PAF27_PAF_MULT_COEFF12_OFFSET)

/* DX_M1A_NPU0.SFU_PAF28 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF28(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF28)
#define WRITE_DX_M1A_NPU0_SFU_PAF28(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF28, val)
/* DX_M1A_NPU0.SFU_PAF28.PAF_MULT_COEFF13 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13_LEN        32
#define DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF28, DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF28, DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13_MASK, DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF28, bits, DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13_MASK, DX_M1A_NPU0_SFU_PAF28_PAF_MULT_COEFF13_OFFSET)

/* DX_M1A_NPU0.SFU_PAF29 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF29(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF29)
#define WRITE_DX_M1A_NPU0_SFU_PAF29(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF29, val)
/* DX_M1A_NPU0.SFU_PAF29.PAF_MULT_COEFF14 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14_LEN        32
#define DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF29, DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF29, DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14_MASK, DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF29, bits, DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14_MASK, DX_M1A_NPU0_SFU_PAF29_PAF_MULT_COEFF14_OFFSET)

/* DX_M1A_NPU0.SFU_PAF30 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF30(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF30)
#define WRITE_DX_M1A_NPU0_SFU_PAF30(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF30, val)
/* DX_M1A_NPU0.SFU_PAF30.PAF_MULT_COEFF15 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15_LEN        32
#define DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF30, DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF30, DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15_MASK, DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF30, bits, DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15_MASK, DX_M1A_NPU0_SFU_PAF30_PAF_MULT_COEFF15_OFFSET)

/* DX_M1A_NPU0.SFU_PAF31 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF31(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF31)
#define WRITE_DX_M1A_NPU0_SFU_PAF31(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF31, val)
/* DX_M1A_NPU0.SFU_PAF31.PAF_ADD_COEFF0 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0_LEN        32
#define DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF31, DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF31, DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0_MASK, DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF31, bits, DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0_MASK, DX_M1A_NPU0_SFU_PAF31_PAF_ADD_COEFF0_OFFSET)

/* DX_M1A_NPU0.SFU_PAF32 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF32(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF32)
#define WRITE_DX_M1A_NPU0_SFU_PAF32(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF32, val)
/* DX_M1A_NPU0.SFU_PAF32.PAF_ADD_COEFF1 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1_LEN        32
#define DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF32, DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF32, DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1_MASK, DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF32, bits, DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1_MASK, DX_M1A_NPU0_SFU_PAF32_PAF_ADD_COEFF1_OFFSET)

/* DX_M1A_NPU0.SFU_PAF33 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF33(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF33)
#define WRITE_DX_M1A_NPU0_SFU_PAF33(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF33, val)
/* DX_M1A_NPU0.SFU_PAF33.PAF_ADD_COEFF2 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2_LEN        32
#define DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF33, DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF33, DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2_MASK, DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF33, bits, DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2_MASK, DX_M1A_NPU0_SFU_PAF33_PAF_ADD_COEFF2_OFFSET)

/* DX_M1A_NPU0.SFU_PAF34 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF34(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF34)
#define WRITE_DX_M1A_NPU0_SFU_PAF34(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF34, val)
/* DX_M1A_NPU0.SFU_PAF34.PAF_ADD_COEFF3 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3_LEN        32
#define DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF34, DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF34, DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3_MASK, DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF34, bits, DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3_MASK, DX_M1A_NPU0_SFU_PAF34_PAF_ADD_COEFF3_OFFSET)

/* DX_M1A_NPU0.SFU_PAF35 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF35(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF35)
#define WRITE_DX_M1A_NPU0_SFU_PAF35(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF35, val)
/* DX_M1A_NPU0.SFU_PAF35.PAF_ADD_COEFF4 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4_LEN        32
#define DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF35, DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF35, DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4_MASK, DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF35, bits, DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4_MASK, DX_M1A_NPU0_SFU_PAF35_PAF_ADD_COEFF4_OFFSET)

/* DX_M1A_NPU0.SFU_PAF36 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF36(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF36)
#define WRITE_DX_M1A_NPU0_SFU_PAF36(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF36, val)
/* DX_M1A_NPU0.SFU_PAF36.PAF_ADD_COEFF5 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5_LEN        32
#define DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF36, DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF36, DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5_MASK, DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF36, bits, DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5_MASK, DX_M1A_NPU0_SFU_PAF36_PAF_ADD_COEFF5_OFFSET)

/* DX_M1A_NPU0.SFU_PAF37 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF37(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF37)
#define WRITE_DX_M1A_NPU0_SFU_PAF37(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF37, val)
/* DX_M1A_NPU0.SFU_PAF37.PAF_ADD_COEFF6 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6_LEN        32
#define DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF37, DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF37, DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6_MASK, DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF37, bits, DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6_MASK, DX_M1A_NPU0_SFU_PAF37_PAF_ADD_COEFF6_OFFSET)

/* DX_M1A_NPU0.SFU_PAF38 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF38(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF38)
#define WRITE_DX_M1A_NPU0_SFU_PAF38(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF38, val)
/* DX_M1A_NPU0.SFU_PAF38.PAF_ADD_COEFF7 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7_LEN        32
#define DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF38, DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF38, DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7_MASK, DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF38, bits, DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7_MASK, DX_M1A_NPU0_SFU_PAF38_PAF_ADD_COEFF7_OFFSET)

/* DX_M1A_NPU0.SFU_PAF39 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF39(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF39)
#define WRITE_DX_M1A_NPU0_SFU_PAF39(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF39, val)
/* DX_M1A_NPU0.SFU_PAF39.PAF_ADD_COEFF8 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8_LEN        32
#define DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF39, DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF39, DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8_MASK, DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF39, bits, DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8_MASK, DX_M1A_NPU0_SFU_PAF39_PAF_ADD_COEFF8_OFFSET)

/* DX_M1A_NPU0.SFU_PAF40 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF40(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF40)
#define WRITE_DX_M1A_NPU0_SFU_PAF40(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF40, val)
/* DX_M1A_NPU0.SFU_PAF40.PAF_ADD_COEFF9 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9_LEN        32
#define DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF40, DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF40, DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9_MASK, DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF40, bits, DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9_MASK, DX_M1A_NPU0_SFU_PAF40_PAF_ADD_COEFF9_OFFSET)

/* DX_M1A_NPU0.SFU_PAF41 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF41(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF41)
#define WRITE_DX_M1A_NPU0_SFU_PAF41(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF41, val)
/* DX_M1A_NPU0.SFU_PAF41.PAF_ADD_COEFF10 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10_LEN        32
#define DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF41, DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF41, DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10_MASK, DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF41, bits, DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10_MASK, DX_M1A_NPU0_SFU_PAF41_PAF_ADD_COEFF10_OFFSET)

/* DX_M1A_NPU0.SFU_PAF42 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF42(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF42)
#define WRITE_DX_M1A_NPU0_SFU_PAF42(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF42, val)
/* DX_M1A_NPU0.SFU_PAF42.PAF_ADD_COEFF11 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11_LEN        32
#define DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF42, DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF42, DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11_MASK, DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF42, bits, DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11_MASK, DX_M1A_NPU0_SFU_PAF42_PAF_ADD_COEFF11_OFFSET)

/* DX_M1A_NPU0.SFU_PAF43 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF43(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF43)
#define WRITE_DX_M1A_NPU0_SFU_PAF43(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF43, val)
/* DX_M1A_NPU0.SFU_PAF43.PAF_ADD_COEFF12 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12_LEN        32
#define DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF43, DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF43, DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12_MASK, DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF43, bits, DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12_MASK, DX_M1A_NPU0_SFU_PAF43_PAF_ADD_COEFF12_OFFSET)

/* DX_M1A_NPU0.SFU_PAF44 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF44(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF44)
#define WRITE_DX_M1A_NPU0_SFU_PAF44(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF44, val)
/* DX_M1A_NPU0.SFU_PAF44.PAF_ADD_COEFF13 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13_LEN        32
#define DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF44, DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF44, DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13_MASK, DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF44, bits, DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13_MASK, DX_M1A_NPU0_SFU_PAF44_PAF_ADD_COEFF13_OFFSET)

/* DX_M1A_NPU0.SFU_PAF45 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF45(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF45)
#define WRITE_DX_M1A_NPU0_SFU_PAF45(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF45, val)
/* DX_M1A_NPU0.SFU_PAF45.PAF_ADD_COEFF14 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14_LEN        32
#define DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF45, DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF45, DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14_MASK, DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF45, bits, DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14_MASK, DX_M1A_NPU0_SFU_PAF45_PAF_ADD_COEFF14_OFFSET)

/* DX_M1A_NPU0.SFU_PAF46 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF46(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF46)
#define WRITE_DX_M1A_NPU0_SFU_PAF46(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF46, val)
/* DX_M1A_NPU0.SFU_PAF46.PAF_ADD_COEFF15 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15_LEN        32
#define DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF46, DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF46, DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15_MASK, DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF46, bits, DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15_MASK, DX_M1A_NPU0_SFU_PAF46_PAF_ADD_COEFF15_OFFSET)

/* DX_M1A_NPU0.SFU_SCALE_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG0)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG0, val)
/* DX_M1A_NPU0.SFU_SCALE_CFG0.SCALE_OUTLIER_EN (Category: NPU0) */
#define DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN_LEN        1
#define DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN_OFFSET     0
#define DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG0, DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN_MASK)
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG0, DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN_MASK, DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG0, bits, DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN_MASK, DX_M1A_NPU0_SFU_SCALE_CFG0_SCALE_OUTLIER_EN_OFFSET)

/* DX_M1A_NPU0.SFU_SCALE_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0_LEN        31
#define DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0_OFFSET     1
#define DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0_MASK       0xfffffffe /* [31:1] 0b11111111111111111111111111111110 */
#define READ_MASK_DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG0, DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG0, DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0_MASK, DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG0, bits, DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0_MASK, DX_M1A_NPU0_SFU_SCALE_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.SFU_SCALE_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG1)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG1, val)
/* DX_M1A_NPU0.SFU_SCALE_CFG1.SCALE_OUTLIER_BIG_BOUND (Category: NPU0) */
#define DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND_LEN        24
#define DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND_OFFSET     0
#define DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG1, DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND_MASK)
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG1, DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND_MASK, DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG1, bits, DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND_MASK, DX_M1A_NPU0_SFU_SCALE_CFG1_SCALE_OUTLIER_BIG_BOUND_OFFSET)

/* DX_M1A_NPU0.SFU_SCALE_CFG1.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0_LEN        8
#define DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0_OFFSET     24
#define DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG1, DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG1, DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0_MASK, DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG1, bits, DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0_MASK, DX_M1A_NPU0_SFU_SCALE_CFG1_RESERVED0_OFFSET)

/* DX_M1A_NPU0.SFU_SCALE_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG2)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG2, val)
/* DX_M1A_NPU0.SFU_SCALE_CFG2.SCALE_OUTLIER_SMALL_BOUND (Category: NPU0) */
#define DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND_LEN        24
#define DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND_OFFSET     0
#define DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND_MASK_BITS  0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND_MASK       0x00ffffff /* [23:0] 0b00000000111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG2, DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND_MASK)
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG2, DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND_MASK, DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG2, bits, DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND_MASK, DX_M1A_NPU0_SFU_SCALE_CFG2_SCALE_OUTLIER_SMALL_BOUND_OFFSET)

/* DX_M1A_NPU0.SFU_SCALE_CFG2.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0_LEN        8
#define DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0_OFFSET     24
#define DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG2, DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG2, DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0_MASK, DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG2, bits, DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0_MASK, DX_M1A_NPU0_SFU_SCALE_CFG2_RESERVED0_OFFSET)

/* DX_M1A_NPU0.SFU_SCALE_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, val)
/* DX_M1A_NPU0.SFU_SCALE_CFG3.SCALE_OUTLIER_BIG_OUT_SCALE_EXP (Category: NPU0) */
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP_LEN        8
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP_OFFSET     0
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP_MASK)
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP_MASK, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, bits, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP_MASK, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_OUT_SCALE_EXP_OFFSET)

/* DX_M1A_NPU0.SFU_SCALE_CFG3.SCALE_OUTLIER_BIG_IN_SCALE_EXP (Category: NPU0) */
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP_LEN        8
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP_OFFSET     8
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP_MASK)
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP_MASK, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, bits, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP_MASK, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_BIG_IN_SCALE_EXP_OFFSET)

/* DX_M1A_NPU0.SFU_SCALE_CFG3.SCALE_OUTLIER_SMALL_OUT_SCALE_EXP (Category: NPU0) */
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP_LEN        8
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP_OFFSET     16
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP_MASK)
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP_MASK, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, bits, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP_MASK, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_OUT_SCALE_EXP_OFFSET)

/* DX_M1A_NPU0.SFU_SCALE_CFG3.SCALE_OUTLIER_SMALL_IN_SCALE_EXP (Category: NPU0) */
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP_LEN        8
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP_OFFSET     24
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP_MASK)
#define READ_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP_MASK, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_SCALE_CFG3, bits, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP_MASK, DX_M1A_NPU0_SFU_SCALE_CFG3_SCALE_OUTLIER_SMALL_IN_SCALE_EXP_OFFSET)

/* DX_M1A_NPU0.SFU_ELEM_BRD0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_ELEM_BRD0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0)
#define WRITE_DX_M1A_NPU0_SFU_ELEM_BRD0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, val)
/* DX_M1A_NPU0.SFU_ELEM_BRD0.ELEM_BROADCAST0_CHANNEL_NUM (Category: NPU0) */
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM_LEN        16
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM_OFFSET     0
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM_MASK)
#define READ_DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM_MASK, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, bits, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM_MASK, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_NUM_OFFSET)

/* DX_M1A_NPU0.SFU_ELEM_BRD0.ELEM_BROADCAST0_CHANNEL_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE_LEN        8
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE_OFFSET     16
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE_MASK)
#define READ_DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE_MASK, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, bits, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE_MASK, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_CHANNEL_SIZE_OFFSET)

/* DX_M1A_NPU0.SFU_ELEM_BRD0.ELEM_BROADCAST0_EN (Category: NPU0) */
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN_LEN        1
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN_OFFSET     24
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN_MASK)
#define READ_DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN_MASK, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, bits, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN_MASK, DX_M1A_NPU0_SFU_ELEM_BRD0_ELEM_BROADCAST0_EN_OFFSET)

/* DX_M1A_NPU0.SFU_ELEM_BRD0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0_LEN        7
#define DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0_OFFSET     25
#define DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0_MASK       0xfe000000 /* [31:25] 0b11111110000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0_MASK, DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_ELEM_BRD0, bits, DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0_MASK, DX_M1A_NPU0_SFU_ELEM_BRD0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.SFU_PAF_DISABLE (Category: NPU0) */
#define READ_DX_M1A_NPU0_SFU_PAF_DISABLE(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_SFU_PAF_DISABLE)
#define WRITE_DX_M1A_NPU0_SFU_PAF_DISABLE(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_SFU_PAF_DISABLE, val)
/* DX_M1A_NPU0.SFU_PAF_DISABLE.PAF_LINE_DISABLE (Category: NPU0) */
#define DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE_LEN        32
#define DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE_OFFSET     0
#define DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_SFU_PAF_DISABLE, DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE_MASK)
#define READ_DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_SFU_PAF_DISABLE, DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE_MASK, DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE_OFFSET)
#define WRITE_DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_SFU_PAF_DISABLE, bits, DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE_MASK, DX_M1A_NPU0_SFU_PAF_DISABLE_PAF_LINE_DISABLE_OFFSET)

/* DX_M1A_NPU0.PRE0_I2C0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE0_I2C0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0)
#define WRITE_DX_M1A_NPU0_PRE0_I2C0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, val)
/* DX_M1A_NPU0.PRE0_I2C0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_I2C0_RESERVED0_LEN        5
#define DX_M1A_NPU0_PRE0_I2C0_RESERVED0_OFFSET     0
#define DX_M1A_NPU0_PRE0_I2C0_RESERVED0_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PRE0_I2C0_RESERVED0_MASK       0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define READ_MASK_DX_M1A_NPU0_PRE0_I2C0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, DX_M1A_NPU0_PRE0_I2C0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PRE0_I2C0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, DX_M1A_NPU0_PRE0_I2C0_RESERVED0_MASK, DX_M1A_NPU0_PRE0_I2C0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_I2C0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, bits, DX_M1A_NPU0_PRE0_I2C0_RESERVED0_MASK, DX_M1A_NPU0_PRE0_I2C0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PRE0_I2C0.I2C0_LINE_OFFSET (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET_LEN        9
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET_OFFSET     5
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET_MASK_BITS  0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET_MASK       0x00003fe0 /* [13:5] 0b00000000000000000011111111100000 */
#define READ_MASK_DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET_MASK)
#define READ_DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET_MASK, DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, bits, DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET_MASK, DX_M1A_NPU0_PRE0_I2C0_I2C0_LINE_OFFSET_OFFSET)

/* DX_M1A_NPU0.PRE0_I2C0.I2C0_LAST_VALID_BYTE (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE_LEN        8
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE_OFFSET     14
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE_MASK       0x003fc000 /* [21:14] 0b00000000001111111100000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE_MASK)
#define READ_DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE_MASK, DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, bits, DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE_MASK, DX_M1A_NPU0_PRE0_I2C0_I2C0_LAST_VALID_BYTE_OFFSET)

/* DX_M1A_NPU0.PRE0_I2C0.I2C0_RQST_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM_LEN        9
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM_OFFSET     22
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM_MASK_BITS  0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM_MASK       0x7fc00000 /* [30:22] 0b01111111110000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM_MASK)
#define READ_DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM_MASK, DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, bits, DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM_MASK, DX_M1A_NPU0_PRE0_I2C0_I2C0_RQST_NUM_OFFSET)

/* DX_M1A_NPU0.PRE0_I2C0.I2C0_EN (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_EN_LEN        1
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_EN_OFFSET     31
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PRE0_I2C0_I2C0_EN_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE0_I2C0_I2C0_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, DX_M1A_NPU0_PRE0_I2C0_I2C0_EN_MASK)
#define READ_DX_M1A_NPU0_PRE0_I2C0_I2C0_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, DX_M1A_NPU0_PRE0_I2C0_I2C0_EN_MASK, DX_M1A_NPU0_PRE0_I2C0_I2C0_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_I2C0_I2C0_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C0, bits, DX_M1A_NPU0_PRE0_I2C0_I2C0_EN_MASK, DX_M1A_NPU0_PRE0_I2C0_I2C0_EN_OFFSET)

/* DX_M1A_NPU0.PRE0_I2C1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE0_I2C1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE0_I2C1)
#define WRITE_DX_M1A_NPU0_PRE0_I2C1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE0_I2C1, val)
/* DX_M1A_NPU0.PRE0_I2C1.I2C0_WRITE_FEATURE_BASE_ADDR_0 (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0_LEN        16
#define DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0_OFFSET     0
#define DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_I2C1, DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0_MASK)
#define READ_DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C1, DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0_MASK, DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C1, bits, DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0_MASK, DX_M1A_NPU0_PRE0_I2C1_I2C0_WRITE_FEATURE_BASE_ADDR_0_OFFSET)

/* DX_M1A_NPU0.PRE0_I2C1.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_I2C1_RESERVED0_LEN        16
#define DX_M1A_NPU0_PRE0_I2C1_RESERVED0_OFFSET     16
#define DX_M1A_NPU0_PRE0_I2C1_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE0_I2C1_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE0_I2C1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_I2C1, DX_M1A_NPU0_PRE0_I2C1_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PRE0_I2C1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C1, DX_M1A_NPU0_PRE0_I2C1_RESERVED0_MASK, DX_M1A_NPU0_PRE0_I2C1_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_I2C1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C1, bits, DX_M1A_NPU0_PRE0_I2C1_RESERVED0_MASK, DX_M1A_NPU0_PRE0_I2C1_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PRE0_I2C2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE0_I2C2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE0_I2C2)
#define WRITE_DX_M1A_NPU0_PRE0_I2C2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE0_I2C2, val)
/* DX_M1A_NPU0.PRE0_I2C2.I2C0_WRITE_FEATURE_BASE_ADDR_2 (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2_LEN        16
#define DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2_OFFSET     0
#define DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_I2C2, DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2_MASK)
#define READ_DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C2, DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2_MASK, DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C2, bits, DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2_MASK, DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_2_OFFSET)

/* DX_M1A_NPU0.PRE0_I2C2.I2C0_WRITE_FEATURE_BASE_ADDR_1 (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1_LEN        16
#define DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1_OFFSET     16
#define DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_I2C2, DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1_MASK)
#define READ_DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C2, DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1_MASK, DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C2, bits, DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1_MASK, DX_M1A_NPU0_PRE0_I2C2_I2C0_WRITE_FEATURE_BASE_ADDR_1_OFFSET)

/* DX_M1A_NPU0.PRE0_I2C3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE0_I2C3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE0_I2C3)
#define WRITE_DX_M1A_NPU0_PRE0_I2C3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE0_I2C3, val)
/* DX_M1A_NPU0.PRE0_I2C3.I2C0_HEIGHT_MAX (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX_LEN        16
#define DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX_OFFSET     0
#define DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_I2C3, DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX_MASK)
#define READ_DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C3, DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX_MASK, DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C3, bits, DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX_MASK, DX_M1A_NPU0_PRE0_I2C3_I2C0_HEIGHT_MAX_OFFSET)

/* DX_M1A_NPU0.PRE0_I2C3.I2C0_WIDTH_MAX (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX_LEN        16
#define DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX_OFFSET     16
#define DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_I2C3, DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX_MASK)
#define READ_DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C3, DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX_MASK, DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_I2C3, bits, DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX_MASK, DX_M1A_NPU0_PRE0_I2C3_I2C0_WIDTH_MAX_OFFSET)

/* DX_M1A_NPU0.PRE0_FMT0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE0_FMT0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0)
#define WRITE_DX_M1A_NPU0_PRE0_FMT0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, val)
/* DX_M1A_NPU0.PRE0_FMT0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_FMT0_RESERVED0_LEN        22
#define DX_M1A_NPU0_PRE0_FMT0_RESERVED0_OFFSET     0
#define DX_M1A_NPU0_PRE0_FMT0_RESERVED0_MASK_BITS  0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define DX_M1A_NPU0_PRE0_FMT0_RESERVED0_MASK       0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PRE0_FMT0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, DX_M1A_NPU0_PRE0_FMT0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PRE0_FMT0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, DX_M1A_NPU0_PRE0_FMT0_RESERVED0_MASK, DX_M1A_NPU0_PRE0_FMT0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_FMT0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, bits, DX_M1A_NPU0_PRE0_FMT0_RESERVED0_MASK, DX_M1A_NPU0_PRE0_FMT0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PRE0_FMT0.FMT0_FIRST_LINE_OPT (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT_LEN        2
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT_OFFSET     22
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT_MASK       0x00c00000 /* [23:22] 0b00000000110000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT_MASK)
#define READ_DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT_MASK, DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, bits, DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT_MASK, DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_LINE_OPT_OFFSET)

/* DX_M1A_NPU0.PRE0_FMT0.FMT0_FIRST_VALID_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM_LEN        6
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM_OFFSET     24
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM_MASK       0x3f000000 /* [29:24] 0b00111111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM_MASK)
#define READ_DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM_MASK, DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, bits, DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM_MASK, DX_M1A_NPU0_PRE0_FMT0_FMT0_FIRST_VALID_NUM_OFFSET)

/* DX_M1A_NPU0.PRE0_FMT0.FMT0_CH_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE_LEN        1
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE_OFFSET     30
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE_MASK)
#define READ_DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE_MASK, DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, bits, DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE_MASK, DX_M1A_NPU0_PRE0_FMT0_FMT0_CH_MODE_OFFSET)

/* DX_M1A_NPU0.PRE0_FMT0.FMT0_ENABLE (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE_LEN        1
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE_OFFSET     31
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE_MASK)
#define READ_DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE_MASK, DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT0, bits, DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE_MASK, DX_M1A_NPU0_PRE0_FMT0_FMT0_ENABLE_OFFSET)

/* DX_M1A_NPU0.PRE0_FMT1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE0_FMT1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE0_FMT1)
#define WRITE_DX_M1A_NPU0_PRE0_FMT1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE0_FMT1, val)
/* DX_M1A_NPU0.PRE0_FMT1.FMT0_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE0_FMT1, DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT1, DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE0_FMT1, bits, DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PRE0_FMT1_FMT0_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN0)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN0, val)
/* DX_M1A_NPU0.PE0_ADDR_GEN0.PE0_DEFAULT_F_ADDR_GEN_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE_LEN        1
#define DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE_OFFSET     0
#define DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN0, DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN0, DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN0, bits, DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_DEFAULT_F_ADDR_GEN_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0_LEN        4
#define DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0_OFFSET     1
#define DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0_MASK       0x0000001e /* [4:1] 0b00000000000000000000000000011110 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN0, DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN0, DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0_MASK, DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN0, bits, DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0_MASK, DX_M1A_NPU0_PE0_ADDR_GEN0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN0.PE0_RF_ADDR_GEN_MAX_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM_LEN        27
#define DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM_OFFSET     5
#define DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM_MASK_BITS  0x07ffffff /* [26:0] 0b00000111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM_MASK       0xffffffe0 /* [31:5] 0b11111111111111111111111111100000 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN0, DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN0, DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM_MASK, DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN0, bits, DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM_MASK, DX_M1A_NPU0_PE0_ADDR_GEN0_PE0_RF_ADDR_GEN_MAX_NUM_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN1)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN1, val)
/* DX_M1A_NPU0.PE0_ADDR_GEN1.PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN1, DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN1, DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN1, bits, DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_ADDR_GEN1_PE0_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN2)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN2, val)
/* DX_M1A_NPU0.PE0_ADDR_GEN2.PE0_DEFAULT_F_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE_LEN        16
#define DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN2, DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN2, DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN2, bits, DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN2.PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_LEN        16
#define DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET     16
#define DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN2, DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN2, DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK, DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN2, bits, DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK, DX_M1A_NPU0_PE0_ADDR_GEN2_PE0_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN3)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN3, val)
/* DX_M1A_NPU0.PE0_ADDR_GEN3.PE0_DEFAULT_F_SURFACE_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE_LEN        16
#define DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN3, DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN3, DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN3, bits, DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_SURFACE_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN3.PE0_DEFAULT_F_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE_LEN        16
#define DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE_OFFSET     16
#define DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN3, DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN3, DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN3, bits, DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN3_PE0_DEFAULT_F_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN4)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN4, val)
/* DX_M1A_NPU0.PE0_ADDR_GEN4.PE0_ELEM_ADDR_GEN_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE_LEN        1
#define DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE_OFFSET     0
#define DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN4, DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN4, DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN4, bits, DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_ELEM_ADDR_GEN_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN4.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0_LEN        15
#define DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0_OFFSET     1
#define DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0_MASK       0x0000fffe /* [15:1] 0b00000000000000001111111111111110 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN4, DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN4, DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0_MASK, DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN4, bits, DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0_MASK, DX_M1A_NPU0_PE0_ADDR_GEN4_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN4.PE0_DEFAULT_F_SURFACE_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE_LEN        16
#define DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE_OFFSET     16
#define DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN4, DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN4, DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN4, bits, DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN4_PE0_DEFAULT_F_SURFACE_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN5 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN5(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN5)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN5(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN5, val)
/* DX_M1A_NPU0.PE0_ADDR_GEN5.PE0_ELEM_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE_LEN        16
#define DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN5, DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN5, DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN5, bits, DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN5.PE0_ELEM_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE_LEN        16
#define DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE_OFFSET     16
#define DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN5, DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN5, DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN5, bits, DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN5_PE0_ELEM_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN6 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN6(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN6)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN6(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN6, val)
/* DX_M1A_NPU0.PE0_ADDR_GEN6.PE0_ELEM_SURFACE_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE_LEN        16
#define DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN6, DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN6, DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN6, bits, DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN6.PE0_ELEM_SURFACE_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE_LEN        16
#define DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE_OFFSET     16
#define DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN6, DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN6, DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN6, bits, DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN6_PE0_ELEM_SURFACE_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN7 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN7(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN7)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN7(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN7, val)
/* DX_M1A_NPU0.PE0_ADDR_GEN7.PE0_MUL_ADDR_GEN_MODE_OFFSET (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET_LEN        16
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET_OFFSET     0
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN7, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN7, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET_MASK, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN7, bits, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET_MASK, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN7.PE0_MUL_ADDR_GEN_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_LEN        2
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET     16
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_MASK       0x00030000 /* [17:16] 0b00000000000000110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN7, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN7, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN7, bits, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN7.PE0_MUL_ADDR_GEN_MODE2_MAX_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM_LEN        14
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM_OFFSET     18
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM_MASK_BITS  0x00003fff /* [13:0] 0b00000000000000000011111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM_MASK       0xfffc0000 /* [31:18] 0b11111111111111000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN7, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN7, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM_MASK, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN7, bits, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM_MASK, DX_M1A_NPU0_PE0_ADDR_GEN7_PE0_MUL_ADDR_GEN_MODE2_MAX_NUM_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN8 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN8(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN8)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN8(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN8, val)
/* DX_M1A_NPU0.PE0_ADDR_GEN8.PE0_ADD_ADDR_GEN_MODE_OFFSET (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET_LEN        16
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET_OFFSET     0
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN8, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN8, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET_MASK, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN8, bits, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET_MASK, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN8.PE0_ADD_ADDR_GEN_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_LEN        2
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET     16
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_MASK       0x00030000 /* [17:16] 0b00000000000000110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN8, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN8, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN8, bits, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_ADDR_GEN8.PE0_ADD_ADDR_GEN_MODE2_MAX_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM_LEN        14
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM_OFFSET     18
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM_MASK_BITS  0x00003fff /* [13:0] 0b00000000000000000011111111111111 */
#define DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM_MASK       0xfffc0000 /* [31:18] 0b11111111111111000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN8, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM_MASK)
#define READ_DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN8, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM_MASK, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_ADDR_GEN8, bits, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM_MASK, DX_M1A_NPU0_PE0_ADDR_GEN8_PE0_ADD_ADDR_GEN_MODE2_MAX_NUM_OFFSET)

/* DX_M1A_NPU0.PE0_CTRL (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CTRL(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CTRL)
#define WRITE_DX_M1A_NPU0_PE0_CTRL(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, val)
/* DX_M1A_NPU0.PE0_CTRL.PE0_CONV_TYPE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE_LEN        8
#define DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE_OFFSET     0
#define DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE_MASK)
#define READ_DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE_MASK, DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, bits, DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE_MASK, DX_M1A_NPU0_PE0_CTRL_PE0_CONV_TYPE_OFFSET)

/* DX_M1A_NPU0.PE0_CTRL.PE0_CONV_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN_LEN        1
#define DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN_OFFSET     8
#define DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN_MASK, DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, bits, DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN_MASK, DX_M1A_NPU0_PE0_CTRL_PE0_CONV_EN_OFFSET)

/* DX_M1A_NPU0.PE0_CTRL.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CTRL_RESERVED0_LEN        7
#define DX_M1A_NPU0_PE0_CTRL_RESERVED0_OFFSET     9
#define DX_M1A_NPU0_PE0_CTRL_RESERVED0_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_M1A_NPU0_PE0_CTRL_RESERVED0_MASK       0x0000fe00 /* [15:9] 0b00000000000000001111111000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CTRL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_CTRL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_RESERVED0_MASK, DX_M1A_NPU0_PE0_CTRL_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CTRL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, bits, DX_M1A_NPU0_PE0_CTRL_RESERVED0_MASK, DX_M1A_NPU0_PE0_CTRL_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_CTRL.PE0_SYSTOLIC_DEPTH (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_LEN        8
#define DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_OFFSET     16
#define DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_MASK)
#define READ_DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_MASK, DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, bits, DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_MASK, DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_DEPTH_OFFSET)

/* DX_M1A_NPU0.PE0_CTRL.PE0_SYSTOLIC_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_LEN        1
#define DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_OFFSET     24
#define DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_MASK, DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, bits, DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_MASK, DX_M1A_NPU0_PE0_CTRL_PE0_SYSTOLIC_EN_OFFSET)

/* DX_M1A_NPU0.PE0_CTRL.PE0_SFUIN_VALID_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_LEN        7
#define DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_OFFSET     25
#define DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_MASK       0xfe000000 /* [31:25] 0b11111110000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_MASK)
#define READ_DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_MASK, DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CTRL, bits, DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_MASK, DX_M1A_NPU0_PE0_CTRL_PE0_SFUIN_VALID_NUM_OFFSET)

/* DX_M1A_NPU0.PE0_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG0)
#define WRITE_DX_M1A_NPU0_PE0_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG0, val)
/* DX_M1A_NPU0.PE0_CFG0.PE0_LAST_CONV_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_LEN        26
#define DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_MASK_BITS  0x03ffffff /* [25:0] 0b00000011111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_MASK       0x03ffffff /* [25:0] 0b00000011111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG0, DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG0, DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_MASK, DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG0, bits, DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_MASK, DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_NUM_OFFSET)

/* DX_M1A_NPU0.PE0_CFG0.PE0_LAST_CONV_MOD (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_LEN        6
#define DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_OFFSET     26
#define DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_MASK       0xfc000000 /* [31:26] 0b11111100000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG0, DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG0, DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_MASK, DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG0, bits, DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_MASK, DX_M1A_NPU0_PE0_CFG0_PE0_LAST_CONV_MOD_OFFSET)

/* DX_M1A_NPU0.PE0_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG1)
#define WRITE_DX_M1A_NPU0_PE0_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG1, val)
/* DX_M1A_NPU0.PE0_CFG1.PE0_OFEATURE_CHANNEL (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_LEN        16
#define DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG1, DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG1, DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_MASK, DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG1, bits, DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_MASK, DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_CHANNEL_OFFSET)

/* DX_M1A_NPU0.PE0_CFG1.PE0_OFEATURE_NUMBER (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_LEN        16
#define DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_OFFSET     16
#define DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG1, DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG1, DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_MASK, DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG1, bits, DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_MASK, DX_M1A_NPU0_PE0_CFG1_PE0_OFEATURE_NUMBER_OFFSET)

/* DX_M1A_NPU0.PE0_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG2)
#define WRITE_DX_M1A_NPU0_PE0_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG2, val)
/* DX_M1A_NPU0.PE0_CFG2.PE0_OFEATURE_WIDTH (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_LEN        16
#define DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG2, DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG2, DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_MASK, DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG2, bits, DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_MASK, DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_WIDTH_OFFSET)

/* DX_M1A_NPU0.PE0_CFG2.PE0_OFEATURE_HEIGHT (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_LEN        16
#define DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_OFFSET     16
#define DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG2, DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG2, DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_MASK, DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG2, bits, DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_MASK, DX_M1A_NPU0_PE0_CFG2_PE0_OFEATURE_HEIGHT_OFFSET)

/* DX_M1A_NPU0.PE0_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG3)
#define WRITE_DX_M1A_NPU0_PE0_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG3, val)
/* DX_M1A_NPU0.PE0_CFG3.PE0_OFEATURE_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE_LEN        32
#define DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG3, DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG3, DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE_MASK, DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG3, bits, DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE_MASK, DX_M1A_NPU0_PE0_CFG3_PE0_OFEATURE_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG4)
#define WRITE_DX_M1A_NPU0_PE0_CFG4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG4, val)
/* DX_M1A_NPU0.PE0_CFG4.PE0_FEATURE_HEIGHT (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_LEN        16
#define DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG4, DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG4, DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_MASK, DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG4, bits, DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_MASK, DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_HEIGHT_OFFSET)

/* DX_M1A_NPU0.PE0_CFG4.PE0_FEATURE_CHANNEL (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL_LEN        16
#define DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL_OFFSET     16
#define DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG4, DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG4, DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL_MASK, DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG4, bits, DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL_MASK, DX_M1A_NPU0_PE0_CFG4_PE0_FEATURE_CHANNEL_OFFSET)

/* DX_M1A_NPU0.PE0_CFG5 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG5(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG5)
#define WRITE_DX_M1A_NPU0_PE0_CFG5(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG5, val)
/* DX_M1A_NPU0.PE0_CFG5.PE0_FILTER_NUMBER (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_LEN        16
#define DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG5, DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG5, DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_MASK, DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG5, bits, DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_MASK, DX_M1A_NPU0_PE0_CFG5_PE0_FILTER_NUMBER_OFFSET)

/* DX_M1A_NPU0.PE0_CFG5.PE0_FEATURE_WIDTH (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH_LEN        16
#define DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH_OFFSET     16
#define DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG5, DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG5, DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH_MASK, DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG5, bits, DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH_MASK, DX_M1A_NPU0_PE0_CFG5_PE0_FEATURE_WIDTH_OFFSET)

/* DX_M1A_NPU0.PE0_CFG6 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG6(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG6)
#define WRITE_DX_M1A_NPU0_PE0_CFG6(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, val)
/* DX_M1A_NPU0.PE0_CFG6.PE0_PAD_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE_LEN        1
#define DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE_MASK, DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, bits, DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE_MASK, DX_M1A_NPU0_PE0_CFG6_PE0_PAD_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG6.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG6_RESERVED0_LEN        5
#define DX_M1A_NPU0_PE0_CFG6_RESERVED0_OFFSET     1
#define DX_M1A_NPU0_PE0_CFG6_RESERVED0_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE0_CFG6_RESERVED0_MASK       0x0000003e /* [5:1] 0b00000000000000000000000000111110 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG6_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, DX_M1A_NPU0_PE0_CFG6_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG6_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, DX_M1A_NPU0_PE0_CFG6_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG6_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG6_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, bits, DX_M1A_NPU0_PE0_CFG6_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG6_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_CFG6.PE0_FILTER_WIDTH (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_LEN        5
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_OFFSET     6
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_MASK       0x000007c0 /* [10:6] 0b00000000000000000000011111000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_MASK, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, bits, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_MASK, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_WIDTH_OFFSET)

/* DX_M1A_NPU0.PE0_CFG6.PE0_FILTER_HEIGHT (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_LEN        5
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_OFFSET     11
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_MASK       0x0000f800 /* [15:11] 0b00000000000000001111100000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_MASK, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, bits, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_MASK, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_HEIGHT_OFFSET)

/* DX_M1A_NPU0.PE0_CFG6.PE0_FILTER_CHANNEL (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL_LEN        16
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL_OFFSET     16
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL_MASK, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG6, bits, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL_MASK, DX_M1A_NPU0_PE0_CFG6_PE0_FILTER_CHANNEL_OFFSET)

/* DX_M1A_NPU0.PE0_CFG7 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG7(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG7)
#define WRITE_DX_M1A_NPU0_PE0_CFG7(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, val)
/* DX_M1A_NPU0.PE0_CFG7.PE0_PAD_TOP_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_LEN        6
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_MASK, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, bits, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_MASK, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_TOP_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG7.PE0_PAD_BOTTOM_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_LEN        6
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_OFFSET     6
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_MASK       0x00000fc0 /* [11:6] 0b00000000000000000000111111000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_MASK, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, bits, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_MASK, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_BOTTOM_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG7.PE0_PAD_LEFT_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_LEN        6
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_OFFSET     12
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_MASK       0x0003f000 /* [17:12] 0b00000000000000111111000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_MASK, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, bits, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_MASK, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_LEFT_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG7.PE0_PAD_RIGHT_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_LEN        6
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_OFFSET     18
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_MASK       0x00fc0000 /* [23:18] 0b00000000111111000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_MASK, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, bits, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_MASK, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_RIGHT_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG7.PE0_PAD_VALUE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE_LEN        8
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE_OFFSET     24
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE_MASK, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG7, bits, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE_MASK, DX_M1A_NPU0_PE0_CFG7_PE0_PAD_VALUE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG8 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG8(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG8)
#define WRITE_DX_M1A_NPU0_PE0_CFG8(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, val)
/* DX_M1A_NPU0.PE0_CFG8.PE0_STRIDE_X (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X_LEN        5
#define DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X_MASK       0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X_MASK, DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, bits, DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X_MASK, DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_X_OFFSET)

/* DX_M1A_NPU0.PE0_CFG8.PE0_STRIDE_Y (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y_LEN        5
#define DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y_OFFSET     5
#define DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y_MASK       0x000003e0 /* [9:5] 0b00000000000000000000001111100000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y_MASK, DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, bits, DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y_MASK, DX_M1A_NPU0_PE0_CFG8_PE0_STRIDE_Y_OFFSET)

/* DX_M1A_NPU0.PE0_CFG8.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG8_RESERVED0_LEN        6
#define DX_M1A_NPU0_PE0_CFG8_RESERVED0_OFFSET     10
#define DX_M1A_NPU0_PE0_CFG8_RESERVED0_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_CFG8_RESERVED0_MASK       0x0000fc00 /* [15:10] 0b00000000000000001111110000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG8_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, DX_M1A_NPU0_PE0_CFG8_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG8_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, DX_M1A_NPU0_PE0_CFG8_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG8_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG8_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, bits, DX_M1A_NPU0_PE0_CFG8_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG8_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_CFG8.PE0_CHANNEL_OFFSET (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_LEN        16
#define DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_OFFSET     16
#define DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_MASK, DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG8, bits, DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_MASK, DX_M1A_NPU0_PE0_CFG8_PE0_CHANNEL_OFFSET_OFFSET)

/* DX_M1A_NPU0.PE0_CFG9 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG9(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG9)
#define WRITE_DX_M1A_NPU0_PE0_CFG9(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, val)
/* DX_M1A_NPU0.PE0_CFG9.PE0_ELEM_CHANNEL_OFFSET (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET_LEN        16
#define DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET_MASK, DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, bits, DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET_MASK, DX_M1A_NPU0_PE0_CFG9_PE0_ELEM_CHANNEL_OFFSET_OFFSET)

/* DX_M1A_NPU0.PE0_CFG9.PE0_DILATION_X (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X_LEN        6
#define DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X_OFFSET     16
#define DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X_MASK       0x003f0000 /* [21:16] 0b00000000001111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X_MASK, DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, bits, DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X_MASK, DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_X_OFFSET)

/* DX_M1A_NPU0.PE0_CFG9.PE0_DILATION_Y (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y_LEN        6
#define DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y_OFFSET     22
#define DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y_MASK       0x0fc00000 /* [27:22] 0b00001111110000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y_MASK, DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, bits, DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y_MASK, DX_M1A_NPU0_PE0_CFG9_PE0_DILATION_Y_OFFSET)

/* DX_M1A_NPU0.PE0_CFG9.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG9_RESERVED0_LEN        4
#define DX_M1A_NPU0_PE0_CFG9_RESERVED0_OFFSET     28
#define DX_M1A_NPU0_PE0_CFG9_RESERVED0_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_NPU0_PE0_CFG9_RESERVED0_MASK       0xf0000000 /* [31:28] 0b11110000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG9_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, DX_M1A_NPU0_PE0_CFG9_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG9_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, DX_M1A_NPU0_PE0_CFG9_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG9_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG9_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG9, bits, DX_M1A_NPU0_PE0_CFG9_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG9_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_CFG10 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG10(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG10)
#define WRITE_DX_M1A_NPU0_PE0_CFG10(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG10, val)
/* DX_M1A_NPU0.PE0_CFG10.PE0_READ_FEATURE_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG10, DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG10, DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG10, bits, DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG10_PE0_READ_FEATURE_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_CFG11 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG11(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG11)
#define WRITE_DX_M1A_NPU0_PE0_CFG11(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG11, val)
/* DX_M1A_NPU0.PE0_CFG11.PE0_WRITE_FEATURE_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG11, DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG11, DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG11, bits, DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG11_PE0_WRITE_FEATURE_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_CFG12 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG12(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG12)
#define WRITE_DX_M1A_NPU0_PE0_CFG12(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG12, val)
/* DX_M1A_NPU0.PE0_CFG12.PE0_READ_WEIGHT_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG12, DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG12, DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG12, bits, DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG12_PE0_READ_WEIGHT_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_CFG13 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG13(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG13)
#define WRITE_DX_M1A_NPU0_PE0_CFG13(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG13, val)
/* DX_M1A_NPU0.PE0_CFG13.PE0_ELEM_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG13, DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG13, DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG13, bits, DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG13_PE0_ELEM_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_CFG14 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG14(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG14)
#define WRITE_DX_M1A_NPU0_PE0_CFG14(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG14, val)
/* DX_M1A_NPU0.PE0_CFG14.PE0_MUL_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG14, DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG14, DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG14, bits, DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG14_PE0_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_CFG15 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG15(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG15)
#define WRITE_DX_M1A_NPU0_PE0_CFG15(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG15, val)
/* DX_M1A_NPU0.PE0_CFG15.PE0_ADD_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG15, DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG15, DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG15, bits, DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG15_PE0_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_CFG16 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG16(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG16)
#define WRITE_DX_M1A_NPU0_PE0_CFG16(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG16, val)
/* DX_M1A_NPU0.PE0_CFG16.PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG16, DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG16, DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG16, bits, DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG16_PE0_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_CFG17 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG17(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG17)
#define WRITE_DX_M1A_NPU0_PE0_CFG17(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG17, val)
/* DX_M1A_NPU0.PE0_CFG17.PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG17, DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG17, DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG17, bits, DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG17_PE0_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_CFG18 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG18(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG18)
#define WRITE_DX_M1A_NPU0_PE0_CFG18(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG18, val)
/* DX_M1A_NPU0.PE0_CFG18.PE0_SE_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG18, DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG18, DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG18, bits, DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG18_PE0_SE_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_CFG19 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG19(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG19)
#define WRITE_DX_M1A_NPU0_PE0_CFG19(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG19, val)
/* DX_M1A_NPU0.PE0_CFG19.PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG19, DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG19, DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG19, bits, DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG19_PE0_GENERAL_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_CFG20 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG20(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG20)
#define WRITE_DX_M1A_NPU0_PE0_CFG20(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG20, val)
/* DX_M1A_NPU0.PE0_CFG20.PE0_WF_ADDR_MODE1_LINE_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE_LEN        16
#define DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG20, DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG20, DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE_MASK, DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG20, bits, DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE_MASK, DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_MODE1_LINE_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG20.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG20_RESERVED0_LEN        13
#define DX_M1A_NPU0_PE0_CFG20_RESERVED0_OFFSET     16
#define DX_M1A_NPU0_PE0_CFG20_RESERVED0_MASK_BITS  0x00001fff /* [12:0] 0b00000000000000000001111111111111 */
#define DX_M1A_NPU0_PE0_CFG20_RESERVED0_MASK       0x1fff0000 /* [28:16] 0b00011111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG20_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG20, DX_M1A_NPU0_PE0_CFG20_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG20_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG20, DX_M1A_NPU0_PE0_CFG20_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG20_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG20_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG20, bits, DX_M1A_NPU0_PE0_CFG20_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG20_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_CFG20.PE0_WF_ADDR_GEN_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE_LEN        3
#define DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE_OFFSET     29
#define DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE_MASK       0xe0000000 /* [31:29] 0b11100000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG20, DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG20, DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG20, bits, DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE0_CFG20_PE0_WF_ADDR_GEN_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG21 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG21(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG21)
#define WRITE_DX_M1A_NPU0_PE0_CFG21(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG21, val)
/* DX_M1A_NPU0.PE0_CFG21.PE0_WF_ADDR_MODE1_LINE_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE_LEN        32
#define DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG21, DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG21, DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE_MASK, DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG21, bits, DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE_MASK, DX_M1A_NPU0_PE0_CFG21_PE0_WF_ADDR_MODE1_LINE_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG22 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG22(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG22)
#define WRITE_DX_M1A_NPU0_PE0_CFG22(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG22, val)
/* DX_M1A_NPU0.PE0_CFG22.PE0_WF_ADDR_SURFACE_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE_LEN        32
#define DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG22, DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG22, DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG22, bits, DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE0_CFG22_PE0_WF_ADDR_SURFACE_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG23 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG23(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG23)
#define WRITE_DX_M1A_NPU0_PE0_CFG23(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG23, val)
/* DX_M1A_NPU0.PE0_CFG23.PE0_WF_WRITE_CNT (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT_LEN        16
#define DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG23, DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG23, DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT_MASK, DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG23, bits, DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT_MASK, DX_M1A_NPU0_PE0_CFG23_PE0_WF_WRITE_CNT_OFFSET)

/* DX_M1A_NPU0.PE0_CFG23.PE0_WF_ADDR_SURFACE_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE_LEN        16
#define DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE_OFFSET     16
#define DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG23, DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG23, DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG23, bits, DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE0_CFG23_PE0_WF_ADDR_SURFACE_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_CFG24 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG24(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG24)
#define WRITE_DX_M1A_NPU0_PE0_CFG24(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG24, val)
/* DX_M1A_NPU0.PE0_CFG24.PE0_WF_SRC1_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG24, DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG24, DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG24, bits, DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_CFG24_PE0_WF_SRC1_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_CFG25 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG25(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG25)
#define WRITE_DX_M1A_NPU0_PE0_CFG25(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, val)
/* DX_M1A_NPU0.PE0_CFG25.PE0_SFU_OUT_DATA_VALID_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM_LEN        3
#define DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM_MASK       0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM_MASK, DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, bits, DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM_MASK, DX_M1A_NPU0_PE0_CFG25_PE0_SFU_OUT_DATA_VALID_NUM_OFFSET)

/* DX_M1A_NPU0.PE0_CFG25.RESERVED1 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG25_RESERVED1_LEN        5
#define DX_M1A_NPU0_PE0_CFG25_RESERVED1_OFFSET     3
#define DX_M1A_NPU0_PE0_CFG25_RESERVED1_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE0_CFG25_RESERVED1_MASK       0x000000f8 /* [7:3] 0b00000000000000000000000011111000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG25_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, DX_M1A_NPU0_PE0_CFG25_RESERVED1_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG25_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, DX_M1A_NPU0_PE0_CFG25_RESERVED1_MASK, DX_M1A_NPU0_PE0_CFG25_RESERVED1_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG25_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, bits, DX_M1A_NPU0_PE0_CFG25_RESERVED1_MASK, DX_M1A_NPU0_PE0_CFG25_RESERVED1_OFFSET)

/* DX_M1A_NPU0.PE0_CFG25.PE0_ST_ADDR_GEN_SHIFT_LEN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN_LEN        5
#define DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN_OFFSET     8
#define DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN_MASK       0x00001f00 /* [12:8] 0b00000000000000000001111100000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN_MASK, DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, bits, DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN_MASK, DX_M1A_NPU0_PE0_CFG25_PE0_ST_ADDR_GEN_SHIFT_LEN_OFFSET)

/* DX_M1A_NPU0.PE0_CFG25.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG25_RESERVED0_LEN        19
#define DX_M1A_NPU0_PE0_CFG25_RESERVED0_OFFSET     13
#define DX_M1A_NPU0_PE0_CFG25_RESERVED0_MASK_BITS  0x0007ffff /* [18:0] 0b00000000000001111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG25_RESERVED0_MASK       0xffffe000 /* [31:13] 0b11111111111111111110000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG25_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, DX_M1A_NPU0_PE0_CFG25_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG25_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, DX_M1A_NPU0_PE0_CFG25_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG25_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG25_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG25, bits, DX_M1A_NPU0_PE0_CFG25_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG25_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_CFG26 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_CFG26(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_CFG26)
#define WRITE_DX_M1A_NPU0_PE0_CFG26(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_CFG26, val)
/* DX_M1A_NPU0.PE0_CFG26.PE0_ST_ADDR_GEN_MUL (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL_LEN        31
#define DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL_OFFSET     0
#define DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL_MASK       0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG26, DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG26, DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL_MASK, DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG26, bits, DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL_MASK, DX_M1A_NPU0_PE0_CFG26_PE0_ST_ADDR_GEN_MUL_OFFSET)

/* DX_M1A_NPU0.PE0_CFG26.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_CFG26_RESERVED0_LEN        1
#define DX_M1A_NPU0_PE0_CFG26_RESERVED0_OFFSET     31
#define DX_M1A_NPU0_PE0_CFG26_RESERVED0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_CFG26_RESERVED0_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_CFG26_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_CFG26, DX_M1A_NPU0_PE0_CFG26_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_CFG26_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_CFG26, DX_M1A_NPU0_PE0_CFG26_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG26_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_CFG26_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_CFG26, bits, DX_M1A_NPU0_PE0_CFG26_RESERVED0_MASK, DX_M1A_NPU0_PE0_CFG26_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_VMEM_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_VMEM_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0)
#define WRITE_DX_M1A_NPU0_PE0_VMEM_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, val)
/* DX_M1A_NPU0.PE0_VMEM_CFG0.PE0_START_NO_IN_BUFF (Category: NPU0) */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_LEN        6
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_OFFSET     0
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_MASK)
#define READ_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, bits, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_IN_BUFF_OFFSET)

/* DX_M1A_NPU0.PE0_VMEM_CFG0.RESERVED3 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3_LEN        2
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3_OFFSET     6
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3_MASK)
#define READ_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, bits, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED3_OFFSET)

/* DX_M1A_NPU0.PE0_VMEM_CFG0.PE0_START_NO_WEIGHT_BUFF (Category: NPU0) */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_LEN        6
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_OFFSET     8
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_MASK       0x00003f00 /* [13:8] 0b00000000000000000011111100000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_MASK)
#define READ_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, bits, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_WEIGHT_BUFF_OFFSET)

/* DX_M1A_NPU0.PE0_VMEM_CFG0.RESERVED2 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2_LEN        2
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2_OFFSET     14
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2_MASK       0x0000c000 /* [15:14] 0b00000000000000001100000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2_MASK)
#define READ_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, bits, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED2_OFFSET)

/* DX_M1A_NPU0.PE0_VMEM_CFG0.PE0_START_NO_OUT_BUFF (Category: NPU0) */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_LEN        6
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_OFFSET     16
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_MASK       0x003f0000 /* [21:16] 0b00000000001111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_MASK)
#define READ_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, bits, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_OUT_BUFF_OFFSET)

/* DX_M1A_NPU0.PE0_VMEM_CFG0.RESERVED1 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1_LEN        2
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1_OFFSET     22
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1_MASK       0x00c00000 /* [23:22] 0b00000000110000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1_MASK)
#define READ_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, bits, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED1_OFFSET)

/* DX_M1A_NPU0.PE0_VMEM_CFG0.PE0_START_NO_ELEM_BUFF (Category: NPU0) */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_LEN        6
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_OFFSET     24
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_MASK       0x3f000000 /* [29:24] 0b00111111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_MASK)
#define READ_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, bits, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_PE0_START_NO_ELEM_BUFF_OFFSET)

/* DX_M1A_NPU0.PE0_VMEM_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0_LEN        2
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0_OFFSET     30
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0_MASK       0xc0000000 /* [31:30] 0b11000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_VMEM_CFG0, bits, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE0_VMEM_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.REDUCE0_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_REDUCE0_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, val)
/* DX_M1A_NPU0.REDUCE0_CFG0.REDUCE0_CHANNEL_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE_LEN        8
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE_OFFSET     0
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE_MASK)
#define READ_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE_MASK, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, bits, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE_MASK, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_SIZE_OFFSET)

/* DX_M1A_NPU0.REDUCE0_CFG0.REDUCE0_CHANNEL_NUM (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM_LEN        16
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM_OFFSET     8
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM_MASK       0x00ffff00 /* [23:8] 0b00000000111111111111111100000000 */
#define READ_MASK_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM_MASK)
#define READ_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM_MASK, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, bits, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM_MASK, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_CHANNEL_NUM_OFFSET)

/* DX_M1A_NPU0.REDUCE0_CFG0.REDUCE0_MODE (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE_LEN        2
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE_OFFSET     24
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE_MASK       0x03000000 /* [25:24] 0b00000011000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE_MASK)
#define READ_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE_MASK, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, bits, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE_MASK, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_MODE_OFFSET)

/* DX_M1A_NPU0.REDUCE0_CFG0.REDUCE0_EN (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN_LEN        1
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN_OFFSET     26
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN_MASK       0x04000000 /* [26:26] 0b00000100000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN_MASK)
#define READ_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN_MASK, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, bits, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN_MASK, DX_M1A_NPU0_REDUCE0_CFG0_REDUCE0_EN_OFFSET)

/* DX_M1A_NPU0.REDUCE0_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0_LEN        5
#define DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0_OFFSET     27
#define DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0_MASK       0xf8000000 /* [31:27] 0b11111000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0_MASK, DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG0, bits, DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0_MASK, DX_M1A_NPU0_REDUCE0_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.REDUCE0_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_REDUCE0_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG1)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG1, val)
/* DX_M1A_NPU0.REDUCE0_CFG1.REDUCE0_WIN_NUM (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM_LEN        16
#define DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM_OFFSET     0
#define DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG1, DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM_MASK)
#define READ_DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG1, DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM_MASK, DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG1, bits, DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM_MASK, DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_WIN_NUM_OFFSET)

/* DX_M1A_NPU0.REDUCE0_CFG1.REDUCE0_CNT_MAX_NUM (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM_LEN        16
#define DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM_OFFSET     16
#define DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG1, DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM_MASK)
#define READ_DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG1, DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM_MASK, DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG1, bits, DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM_MASK, DX_M1A_NPU0_REDUCE0_CFG1_REDUCE0_CNT_MAX_NUM_OFFSET)

/* DX_M1A_NPU0.REDUCE0_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_REDUCE0_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG2)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG2, val)
/* DX_M1A_NPU0.REDUCE0_CFG2.REDUCE0_RD_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG2, DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG2, DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR_MASK, DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG2, bits, DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR_MASK, DX_M1A_NPU0_REDUCE0_CFG2_REDUCE0_RD_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.REDUCE0_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_REDUCE0_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG3)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG3, val)
/* DX_M1A_NPU0.REDUCE0_CFG3.REDUCE0_WR_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG3, DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG3, DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR_MASK, DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE0_CFG3, bits, DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR_MASK, DX_M1A_NPU0_REDUCE0_CFG3_REDUCE0_WR_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_CTRL (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_CTRL(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL)
#define WRITE_DX_M1A_NPU0_PE0_DMA_CTRL(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, val)
/* DX_M1A_NPU0.PE0_DMA_CTRL.VMEM0_START_OP_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN_LEN        1
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN_MASK, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, bits, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN_MASK, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_START_OP_EN_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_CTRL.VMEM0_DMA_CTRL_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE_LEN        2
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE_OFFSET     1
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE_MASK       0x00000006 /* [2:1] 0b00000000000000000000000000000110 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE_MASK, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, bits, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE_MASK, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_DMA_CTRL_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_CTRL.VMEM0_WEIGHT_BIT_WIDTH (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH_LEN        8
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH_OFFSET     3
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH_MASK       0x000007f8 /* [10:3] 0b00000000000000000000011111111000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH_MASK, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, bits, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH_MASK, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_BIT_WIDTH_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_CTRL.VMEM0_WEIGHT_DUMMY_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE_LEN        2
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE_OFFSET     11
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE_MASK       0x00001800 /* [12:11] 0b00000000000000000001100000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE_MASK, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, bits, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE_MASK, DX_M1A_NPU0_PE0_DMA_CTRL_VMEM0_WEIGHT_DUMMY_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_CTRL.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0_LEN        19
#define DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0_OFFSET     13
#define DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0_MASK_BITS  0x0007ffff /* [18:0] 0b00000000000001111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0_MASK       0xffffe000 /* [31:13] 0b11111111111111111110000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_CTRL, bits, DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_CTRL_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, val)
/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.VMEM0_RD_ELEM_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE_LEN        16
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.VMEM0_RD_ELEM_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME_LEN        6
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME_OFFSET     16
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME_MASK       0x003f0000 /* [21:16] 0b00000000001111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_TIME_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.VMEM0_RD_ELEM_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE_LEN        2
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE_OFFSET     22
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE_MASK       0x00c00000 /* [23:22] 0b00000000110000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.VMEM0_RD_ELEM_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN_LEN        1
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN_OFFSET     24
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_EN_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.VMEM0_RD_ELEM_SRC (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC_LEN        1
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC_OFFSET     25
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC_MASK       0x02000000 /* [25:25] 0b00000010000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_VMEM0_RD_ELEM_SRC_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_LEN        6
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_OFFSET     26
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_MASK       0xfc000000 /* [31:26] 0b11111100000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG1)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG1, val)
/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG1.VMEM0_RD_ELEM_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE_LEN        32
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG1, DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG1, DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG1, bits, DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG1_VMEM0_RD_ELEM_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG2)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG2, val)
/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG2.VMEM0_RD_ELEM_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG2, DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG2, DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG2, bits, DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG2_VMEM0_RD_ELEM_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG3)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG3, val)
/* DX_M1A_NPU0.PE0_DMA_ELEM_CFG3.VMEM0_WT_ELEM_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG3, DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG3, DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_ELEM_CFG3, bits, DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_ELEM_CFG3_VMEM0_WT_ELEM_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_W_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, val)
/* DX_M1A_NPU0.PE0_DMA_W_CFG0.VMEM0_RD_W_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME_LEN        12
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME_MASK       0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, bits, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_TIME_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_W_CFG0.VMEM0_RD_W_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE_LEN        2
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE_OFFSET     12
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE_MASK       0x00003000 /* [13:12] 0b00000000000000000011000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, bits, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_W_CFG0.VMEM0_RD_W_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN_LEN        1
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN_OFFSET     14
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN_MASK       0x00004000 /* [14:14] 0b00000000000000000100000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, bits, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG0_VMEM0_RD_W_EN_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_W_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0_LEN        17
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0_OFFSET     15
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG0, bits, DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_W_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG1)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG1, val)
/* DX_M1A_NPU0.PE0_DMA_W_CFG1.VMEM0_RD_W_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE_LEN        16
#define DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG1, DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG1, DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG1, bits, DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_W_CFG1.VMEM0_RD_W_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE_LEN        16
#define DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE_OFFSET     16
#define DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG1, DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG1, DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG1, bits, DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG1_VMEM0_RD_W_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_W_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG2)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG2, val)
/* DX_M1A_NPU0.PE0_DMA_W_CFG2.VMEM0_RD_W_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG2, DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG2, DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG2, bits, DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG2_VMEM0_RD_W_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_W_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG3)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG3, val)
/* DX_M1A_NPU0.PE0_DMA_W_CFG3.VMEM0_WT_W_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG3, DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG3, DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_W_CFG3, bits, DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_W_CFG3_VMEM0_WT_W_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_INF_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, val)
/* DX_M1A_NPU0.PE0_DMA_INF_CFG0.VMEM0_RD_INF_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE_LEN        16
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_INF_CFG0.VMEM0_RD_INF_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME_LEN        12
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME_OFFSET     16
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_TIME_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_INF_CFG0.VMEM0_RD_INF_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE_LEN        2
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE_OFFSET     28
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE_MASK       0x30000000 /* [29:28] 0b00110000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_INF_CFG0.VMEM0_RD_INF_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN_LEN        1
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN_OFFSET     30
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_EN_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_INF_CFG0.VMEM0_RD_INF_SRC (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC_LEN        1
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC_OFFSET     31
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG0_VMEM0_RD_INF_SRC_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_INF_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG1)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG1, val)
/* DX_M1A_NPU0.PE0_DMA_INF_CFG1.VMEM0_RD_INF_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE_LEN        32
#define DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG1, DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG1, DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG1, bits, DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG1_VMEM0_RD_INF_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_INF_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG2)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG2, val)
/* DX_M1A_NPU0.PE0_DMA_INF_CFG2.VMEM0_RD_INF_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG2, DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG2, DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG2, bits, DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG2_VMEM0_RD_INF_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_INF_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG3)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG3, val)
/* DX_M1A_NPU0.PE0_DMA_INF_CFG3.VMEM0_WT_INF_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE_LEN        32
#define DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG3, DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG3, DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG3, bits, DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG3_VMEM0_WT_INF_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_INF_CFG4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG4)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG4, val)
/* DX_M1A_NPU0.PE0_DMA_INF_CFG4.VMEM0_WT_INF_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG4, DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG4, DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_INF_CFG4, bits, DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_INF_CFG4_VMEM0_WT_INF_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, val)
/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG0.VMEM0_WT_OUTF_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE_LEN        16
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG0.VMEM0_WT_OUTF_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME_LEN        12
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME_OFFSET     16
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_TIME_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG0.RESERVED1 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_LEN        2
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_OFFSET     28
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_MASK       0x30000000 /* [29:28] 0b00110000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED1_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG0.VMEM0_WT_OUTF_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN_LEN        1
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN_OFFSET     30
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_VMEM0_WT_OUTF_EN_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_LEN        1
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_OFFSET     31
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG1)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG1, val)
/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG1.VMEM0_WT_OUTF_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE_LEN        32
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG1, DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG1, DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG1, bits, DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG1_VMEM0_WT_OUTF_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG2)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG2, val)
/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG2.VMEM0_WT_OUTF_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG2, DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG2, DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG2, bits, DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG2_VMEM0_WT_OUTF_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG3)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG3, val)
/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG3.VMEM0_RD_OUTF_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE_LEN        32
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG3, DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG3, DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG3, bits, DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG3_VMEM0_RD_OUTF_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG4)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG4, val)
/* DX_M1A_NPU0.PE0_DMA_OUTF_CFG4.VMEM0_RD_OUTF_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG4, DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG4, DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_OUTF_CFG4, bits, DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_OUTF_CFG4_VMEM0_RD_OUTF_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_ADD_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE_MASK       0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_ADD_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE_LEN        2
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE_OFFSET     12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE_MASK       0x00003000 /* [13:12] 0b00000000000000000011000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_ADD_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN_LEN        1
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN_OFFSET     14
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN_MASK       0x00004000 /* [14:14] 0b00000000000000000100000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_ADD_EN_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_MUL_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE_OFFSET     15
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE_MASK       0x07ff8000 /* [26:15] 0b00000111111111111000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_MUL_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE_LEN        2
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE_OFFSET     27
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE_MASK       0x18000000 /* [28:27] 0b00011000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RF_RD_MUL_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN_LEN        1
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN_OFFSET     29
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN_MASK       0x20000000 /* [29:29] 0b00100000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RF_RD_MUL_EN_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0_LEN        2
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0_OFFSET     30
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0_MASK       0xc0000000 /* [31:30] 0b11000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG0, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG1.RF_RD_ADD_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE_MASK       0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG1.RF_RD_MUL_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE_OFFSET     12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE_MASK       0x00fff000 /* [23:12] 0b00000000111111111111000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG1.RF_RD_ADD_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME_LEN        4
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME_OFFSET     24
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME_MASK       0x0f000000 /* [27:24] 0b00001111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_ADD_TIME_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG1.RF_RD_MUL_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME_LEN        4
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME_OFFSET     28
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME_MASK       0xf0000000 /* [31:28] 0b11110000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG1, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG1_RF_RD_MUL_TIME_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG2)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG2, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG2.RF_RD_MUL_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG2, DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG2, DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG2, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG2_RF_RD_MUL_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG3)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG3, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG3.RF_WT_MUL_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG3, DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG3, DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG3, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG3_RF_WT_MUL_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG4)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG4, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG4.RF_RD_ADD_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG4, DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG4, DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG4, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG4_RF_RD_ADD_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG5 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG5(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG5)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG5(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG5, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG5.RF_WT_ADD_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG5, DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG5, DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG5, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG5_RF_WT_ADD_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG6 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG6(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG6(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_ADD_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE_MASK       0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_ADD_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE_LEN        2
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE_OFFSET     12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE_MASK       0x00003000 /* [13:12] 0b00000000000000000011000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_ADD_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN_LEN        1
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN_OFFSET     14
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN_MASK       0x00004000 /* [14:14] 0b00000000000000000100000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_ADD_EN_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_MUL_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE_OFFSET     15
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE_MASK       0x07ff8000 /* [26:15] 0b00000111111111111000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_MUL_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE_LEN        2
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE_OFFSET     27
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE_MASK       0x18000000 /* [28:27] 0b00011000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RF_RD_SKIP_MUL_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN_LEN        1
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN_OFFSET     29
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN_MASK       0x20000000 /* [29:29] 0b00100000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RF_RD_SKIP_MUL_EN_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG6.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0_LEN        2
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0_OFFSET     30
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0_MASK       0xc0000000 /* [31:30] 0b11000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG6, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG6_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG7 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG7(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG7(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG7.RF_RD_SKIP_ADD_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE_MASK       0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG7.RF_RD_SKIP_MUL_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE_OFFSET     12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE_MASK       0x00fff000 /* [23:12] 0b00000000111111111111000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG7.RF_RD_SKIP_ADD_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME_LEN        4
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME_OFFSET     24
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME_MASK       0x0f000000 /* [27:24] 0b00001111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_ADD_TIME_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG7.RF_RD_SKIP_MUL_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME_LEN        4
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME_OFFSET     28
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME_MASK       0xf0000000 /* [31:28] 0b11110000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG7, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG7_RF_RD_SKIP_MUL_TIME_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG8 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG8(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG8)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG8(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG8, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG8.RF_RD_SKIP_MUL_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG8, DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG8, DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG8, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG8_RF_RD_SKIP_MUL_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG9 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG9(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG9)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG9(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG9, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG9.RF_WT_SKIP_MUL_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG9, DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG9, DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG9, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG9_RF_WT_SKIP_MUL_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG10 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG10(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG10)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG10(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG10, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG10.RF_RD_SKIP_ADD_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG10, DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG10, DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG10, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG10_RF_RD_SKIP_ADD_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG11 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG11(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG11)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG11(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG11, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG11.RF_WT_SKIP_ADD_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG11, DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG11, DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG11, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG11_RF_WT_SKIP_ADD_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG12 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG12(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG12(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_GENERAL_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE_MASK       0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_GENERAL_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE_LEN        2
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE_OFFSET     12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE_MASK       0x00003000 /* [13:12] 0b00000000000000000011000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_GENERAL_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN_LEN        1
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN_OFFSET     14
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN_MASK       0x00004000 /* [14:14] 0b00000000000000000100000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_GENERAL_EN_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_SE_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE_OFFSET     15
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE_MASK       0x07ff8000 /* [26:15] 0b00000111111111111000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_SIZE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_SE_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE_LEN        2
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE_OFFSET     27
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE_MASK       0x18000000 /* [28:27] 0b00011000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_MODE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RF_RD_SE_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN_LEN        1
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN_OFFSET     29
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN_MASK       0x20000000 /* [29:29] 0b00100000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RF_RD_SE_EN_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG12.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0_LEN        2
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0_OFFSET     30
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0_MASK       0xc0000000 /* [31:30] 0b11000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG12, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG12_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG13 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG13(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG13(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG13.RF_RD_GENERAL_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE_MASK       0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG13.RF_RD_SE_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE_LEN        12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE_OFFSET     12
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE_MASK       0x00fff000 /* [23:12] 0b00000000111111111111000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG13.RF_RD_GENERAL_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME_LEN        4
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME_OFFSET     24
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME_MASK       0x0f000000 /* [27:24] 0b00001111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_GENERAL_TIME_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG13.RF_RD_SE_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME_LEN        4
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME_OFFSET     28
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME_MASK       0xf0000000 /* [31:28] 0b11110000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG13, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG13_RF_RD_SE_TIME_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG14 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG14(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG14)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG14(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG14, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG14.RF_RD_SE_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG14, DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG14, DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG14, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG14_RF_RD_SE_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG15 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG15(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG15)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG15(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG15, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG15.RF_WT_SE_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG15, DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG15, DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG15, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG15_RF_WT_SE_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG16 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG16(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG16)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG16(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG16, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG16.RF_RD_GENERAL_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG16, DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG16, DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG16, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG16_RF_RD_GENERAL_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE0_DMA_RF_CFG17 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG17(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG17)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG17(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG17, val)
/* DX_M1A_NPU0.PE0_DMA_RF_CFG17.RF_WT_GENERAL_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG17, DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG17, DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE0_DMA_RF_CFG17, bits, DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR_MASK, DX_M1A_NPU0_PE0_DMA_RF_CFG17_RF_WT_GENERAL_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PRE1_I2C0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE1_I2C0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0)
#define WRITE_DX_M1A_NPU0_PRE1_I2C0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, val)
/* DX_M1A_NPU0.PRE1_I2C0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_I2C0_RESERVED0_LEN        5
#define DX_M1A_NPU0_PRE1_I2C0_RESERVED0_OFFSET     0
#define DX_M1A_NPU0_PRE1_I2C0_RESERVED0_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PRE1_I2C0_RESERVED0_MASK       0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define READ_MASK_DX_M1A_NPU0_PRE1_I2C0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, DX_M1A_NPU0_PRE1_I2C0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PRE1_I2C0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, DX_M1A_NPU0_PRE1_I2C0_RESERVED0_MASK, DX_M1A_NPU0_PRE1_I2C0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_I2C0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, bits, DX_M1A_NPU0_PRE1_I2C0_RESERVED0_MASK, DX_M1A_NPU0_PRE1_I2C0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PRE1_I2C0.I2C1_LINE_OFFSET (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET_LEN        9
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET_OFFSET     5
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET_MASK_BITS  0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET_MASK       0x00003fe0 /* [13:5] 0b00000000000000000011111111100000 */
#define READ_MASK_DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET_MASK)
#define READ_DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET_MASK, DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, bits, DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET_MASK, DX_M1A_NPU0_PRE1_I2C0_I2C1_LINE_OFFSET_OFFSET)

/* DX_M1A_NPU0.PRE1_I2C0.I2C1_LAST_VALID_BYTE (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE_LEN        8
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE_OFFSET     14
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE_MASK       0x003fc000 /* [21:14] 0b00000000001111111100000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE_MASK)
#define READ_DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE_MASK, DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, bits, DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE_MASK, DX_M1A_NPU0_PRE1_I2C0_I2C1_LAST_VALID_BYTE_OFFSET)

/* DX_M1A_NPU0.PRE1_I2C0.I2C1_RQST_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM_LEN        9
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM_OFFSET     22
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM_MASK_BITS  0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM_MASK       0x7fc00000 /* [30:22] 0b01111111110000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM_MASK)
#define READ_DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM_MASK, DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, bits, DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM_MASK, DX_M1A_NPU0_PRE1_I2C0_I2C1_RQST_NUM_OFFSET)

/* DX_M1A_NPU0.PRE1_I2C0.I2C1_EN (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_EN_LEN        1
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_EN_OFFSET     31
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PRE1_I2C0_I2C1_EN_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE1_I2C0_I2C1_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, DX_M1A_NPU0_PRE1_I2C0_I2C1_EN_MASK)
#define READ_DX_M1A_NPU0_PRE1_I2C0_I2C1_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, DX_M1A_NPU0_PRE1_I2C0_I2C1_EN_MASK, DX_M1A_NPU0_PRE1_I2C0_I2C1_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_I2C0_I2C1_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C0, bits, DX_M1A_NPU0_PRE1_I2C0_I2C1_EN_MASK, DX_M1A_NPU0_PRE1_I2C0_I2C1_EN_OFFSET)

/* DX_M1A_NPU0.PRE1_I2C1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE1_I2C1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE1_I2C1)
#define WRITE_DX_M1A_NPU0_PRE1_I2C1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE1_I2C1, val)
/* DX_M1A_NPU0.PRE1_I2C1.I2C1_WRITE_FEATURE_BASE_ADDR_0 (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0_LEN        16
#define DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0_OFFSET     0
#define DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_I2C1, DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0_MASK)
#define READ_DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C1, DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0_MASK, DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C1, bits, DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0_MASK, DX_M1A_NPU0_PRE1_I2C1_I2C1_WRITE_FEATURE_BASE_ADDR_0_OFFSET)

/* DX_M1A_NPU0.PRE1_I2C1.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_I2C1_RESERVED0_LEN        16
#define DX_M1A_NPU0_PRE1_I2C1_RESERVED0_OFFSET     16
#define DX_M1A_NPU0_PRE1_I2C1_RESERVED0_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE1_I2C1_RESERVED0_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE1_I2C1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_I2C1, DX_M1A_NPU0_PRE1_I2C1_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PRE1_I2C1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C1, DX_M1A_NPU0_PRE1_I2C1_RESERVED0_MASK, DX_M1A_NPU0_PRE1_I2C1_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_I2C1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C1, bits, DX_M1A_NPU0_PRE1_I2C1_RESERVED0_MASK, DX_M1A_NPU0_PRE1_I2C1_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PRE1_I2C2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE1_I2C2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE1_I2C2)
#define WRITE_DX_M1A_NPU0_PRE1_I2C2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE1_I2C2, val)
/* DX_M1A_NPU0.PRE1_I2C2.I2C1_WRITE_FEATURE_BASE_ADDR_2 (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2_LEN        16
#define DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2_OFFSET     0
#define DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_I2C2, DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2_MASK)
#define READ_DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C2, DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2_MASK, DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C2, bits, DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2_MASK, DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_2_OFFSET)

/* DX_M1A_NPU0.PRE1_I2C2.I2C1_WRITE_FEATURE_BASE_ADDR_1 (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1_LEN        16
#define DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1_OFFSET     16
#define DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_I2C2, DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1_MASK)
#define READ_DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C2, DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1_MASK, DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C2, bits, DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1_MASK, DX_M1A_NPU0_PRE1_I2C2_I2C1_WRITE_FEATURE_BASE_ADDR_1_OFFSET)

/* DX_M1A_NPU0.PRE1_I2C3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE1_I2C3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE1_I2C3)
#define WRITE_DX_M1A_NPU0_PRE1_I2C3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE1_I2C3, val)
/* DX_M1A_NPU0.PRE1_I2C3.I2C1_HEIGHT_MAX (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX_LEN        16
#define DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX_OFFSET     0
#define DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_I2C3, DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX_MASK)
#define READ_DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C3, DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX_MASK, DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C3, bits, DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX_MASK, DX_M1A_NPU0_PRE1_I2C3_I2C1_HEIGHT_MAX_OFFSET)

/* DX_M1A_NPU0.PRE1_I2C3.I2C1_WIDTH_MAX (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX_LEN        16
#define DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX_OFFSET     16
#define DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_I2C3, DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX_MASK)
#define READ_DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C3, DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX_MASK, DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_I2C3, bits, DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX_MASK, DX_M1A_NPU0_PRE1_I2C3_I2C1_WIDTH_MAX_OFFSET)

/* DX_M1A_NPU0.PRE1_FMT0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE1_FMT0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0)
#define WRITE_DX_M1A_NPU0_PRE1_FMT0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, val)
/* DX_M1A_NPU0.PRE1_FMT0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_FMT0_RESERVED0_LEN        22
#define DX_M1A_NPU0_PRE1_FMT0_RESERVED0_OFFSET     0
#define DX_M1A_NPU0_PRE1_FMT0_RESERVED0_MASK_BITS  0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define DX_M1A_NPU0_PRE1_FMT0_RESERVED0_MASK       0x003fffff /* [21:0] 0b00000000001111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PRE1_FMT0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, DX_M1A_NPU0_PRE1_FMT0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PRE1_FMT0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, DX_M1A_NPU0_PRE1_FMT0_RESERVED0_MASK, DX_M1A_NPU0_PRE1_FMT0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_FMT0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, bits, DX_M1A_NPU0_PRE1_FMT0_RESERVED0_MASK, DX_M1A_NPU0_PRE1_FMT0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PRE1_FMT0.FMT1_FIRST_LINE_OPT (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT_LEN        2
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT_OFFSET     22
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT_MASK       0x00c00000 /* [23:22] 0b00000000110000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT_MASK)
#define READ_DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT_MASK, DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, bits, DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT_MASK, DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_LINE_OPT_OFFSET)

/* DX_M1A_NPU0.PRE1_FMT0.FMT1_FIRST_VALID_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM_LEN        6
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM_OFFSET     24
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM_MASK       0x3f000000 /* [29:24] 0b00111111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM_MASK)
#define READ_DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM_MASK, DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, bits, DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM_MASK, DX_M1A_NPU0_PRE1_FMT0_FMT1_FIRST_VALID_NUM_OFFSET)

/* DX_M1A_NPU0.PRE1_FMT0.FMT1_CH_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE_LEN        1
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE_OFFSET     30
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE_MASK)
#define READ_DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE_MASK, DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, bits, DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE_MASK, DX_M1A_NPU0_PRE1_FMT0_FMT1_CH_MODE_OFFSET)

/* DX_M1A_NPU0.PRE1_FMT0.FMT1_ENABLE (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE_LEN        1
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE_OFFSET     31
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE_MASK)
#define READ_DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE_MASK, DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT0, bits, DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE_MASK, DX_M1A_NPU0_PRE1_FMT0_FMT1_ENABLE_OFFSET)

/* DX_M1A_NPU0.PRE1_FMT1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PRE1_FMT1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PRE1_FMT1)
#define WRITE_DX_M1A_NPU0_PRE1_FMT1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PRE1_FMT1, val)
/* DX_M1A_NPU0.PRE1_FMT1.FMT1_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PRE1_FMT1, DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT1, DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PRE1_FMT1, bits, DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PRE1_FMT1_FMT1_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN0)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN0, val)
/* DX_M1A_NPU0.PE1_ADDR_GEN0.PE1_DEFAULT_F_ADDR_GEN_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE_LEN        1
#define DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE_OFFSET     0
#define DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN0, DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN0, DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN0, bits, DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_DEFAULT_F_ADDR_GEN_MODE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0_LEN        4
#define DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0_OFFSET     1
#define DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0_MASK       0x0000001e /* [4:1] 0b00000000000000000000000000011110 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN0, DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN0, DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0_MASK, DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN0, bits, DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0_MASK, DX_M1A_NPU0_PE1_ADDR_GEN0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN0.PE1_RF_ADDR_GEN_MAX_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM_LEN        27
#define DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM_OFFSET     5
#define DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM_MASK_BITS  0x07ffffff /* [26:0] 0b00000111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM_MASK       0xffffffe0 /* [31:5] 0b11111111111111111111111111100000 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN0, DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN0, DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM_MASK, DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN0, bits, DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM_MASK, DX_M1A_NPU0_PE1_ADDR_GEN0_PE1_RF_ADDR_GEN_MAX_NUM_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN1)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN1, val)
/* DX_M1A_NPU0.PE1_ADDR_GEN1.PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN1, DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN1, DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN1, bits, DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_ADDR_GEN1_PE1_DEFAULT_F_ADDR_GEN_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN2)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN2, val)
/* DX_M1A_NPU0.PE1_ADDR_GEN2.PE1_DEFAULT_F_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE_LEN        16
#define DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN2, DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN2, DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN2, bits, DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN2.PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_LEN        16
#define DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET     16
#define DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN2, DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN2, DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK, DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN2, bits, DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_MASK, DX_M1A_NPU0_PE1_ADDR_GEN2_PE1_DEFAULT_F_ADDR_GEN_CNT_MAX_NUM_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN3)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN3, val)
/* DX_M1A_NPU0.PE1_ADDR_GEN3.PE1_DEFAULT_F_SURFACE_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE_LEN        16
#define DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN3, DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN3, DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN3, bits, DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_SURFACE_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN3.PE1_DEFAULT_F_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE_LEN        16
#define DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE_OFFSET     16
#define DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN3, DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN3, DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN3, bits, DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN3_PE1_DEFAULT_F_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN4)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN4, val)
/* DX_M1A_NPU0.PE1_ADDR_GEN4.PE1_ELEM_ADDR_GEN_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE_LEN        1
#define DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE_OFFSET     0
#define DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN4, DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN4, DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN4, bits, DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_ELEM_ADDR_GEN_MODE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN4.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0_LEN        15
#define DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0_OFFSET     1
#define DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0_MASK       0x0000fffe /* [15:1] 0b00000000000000001111111111111110 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN4, DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN4, DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0_MASK, DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN4, bits, DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0_MASK, DX_M1A_NPU0_PE1_ADDR_GEN4_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN4.PE1_DEFAULT_F_SURFACE_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE_LEN        16
#define DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE_OFFSET     16
#define DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN4, DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN4, DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN4, bits, DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN4_PE1_DEFAULT_F_SURFACE_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN5 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN5(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN5)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN5(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN5, val)
/* DX_M1A_NPU0.PE1_ADDR_GEN5.PE1_ELEM_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE_LEN        16
#define DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN5, DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN5, DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN5, bits, DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN5.PE1_ELEM_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE_LEN        16
#define DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE_OFFSET     16
#define DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN5, DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN5, DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN5, bits, DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN5_PE1_ELEM_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN6 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN6(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN6)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN6(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN6, val)
/* DX_M1A_NPU0.PE1_ADDR_GEN6.PE1_ELEM_SURFACE_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE_LEN        16
#define DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN6, DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN6, DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN6, bits, DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN6.PE1_ELEM_SURFACE_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE_LEN        16
#define DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE_OFFSET     16
#define DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN6, DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN6, DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN6, bits, DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN6_PE1_ELEM_SURFACE_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN7 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN7(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN7)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN7(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN7, val)
/* DX_M1A_NPU0.PE1_ADDR_GEN7.PE1_MUL_ADDR_GEN_MODE_OFFSET (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET_LEN        16
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET_OFFSET     0
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN7, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN7, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET_MASK, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN7, bits, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET_MASK, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN7.PE1_MUL_ADDR_GEN_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_LEN        2
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET     16
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_MASK       0x00030000 /* [17:16] 0b00000000000000110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN7, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN7, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN7, bits, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN7.PE1_MUL_ADDR_GEN_MODE2_MAX_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM_LEN        14
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM_OFFSET     18
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM_MASK_BITS  0x00003fff /* [13:0] 0b00000000000000000011111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM_MASK       0xfffc0000 /* [31:18] 0b11111111111111000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN7, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN7, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM_MASK, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN7, bits, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM_MASK, DX_M1A_NPU0_PE1_ADDR_GEN7_PE1_MUL_ADDR_GEN_MODE2_MAX_NUM_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN8 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN8(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN8)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN8(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN8, val)
/* DX_M1A_NPU0.PE1_ADDR_GEN8.PE1_ADD_ADDR_GEN_MODE_OFFSET (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET_LEN        16
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET_OFFSET     0
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN8, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN8, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET_MASK, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN8, bits, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET_MASK, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN8.PE1_ADD_ADDR_GEN_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_LEN        2
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET     16
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_MASK       0x00030000 /* [17:16] 0b00000000000000110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN8, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN8, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN8, bits, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE_OFFSET)

/* DX_M1A_NPU0.PE1_ADDR_GEN8.PE1_ADD_ADDR_GEN_MODE2_MAX_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM_LEN        14
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM_OFFSET     18
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM_MASK_BITS  0x00003fff /* [13:0] 0b00000000000000000011111111111111 */
#define DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM_MASK       0xfffc0000 /* [31:18] 0b11111111111111000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN8, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM_MASK)
#define READ_DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN8, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM_MASK, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_ADDR_GEN8, bits, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM_MASK, DX_M1A_NPU0_PE1_ADDR_GEN8_PE1_ADD_ADDR_GEN_MODE2_MAX_NUM_OFFSET)

/* DX_M1A_NPU0.PE1_CTRL (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CTRL(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CTRL)
#define WRITE_DX_M1A_NPU0_PE1_CTRL(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, val)
/* DX_M1A_NPU0.PE1_CTRL.PE1_CONV_TYPE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE_LEN        8
#define DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE_OFFSET     0
#define DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE_MASK)
#define READ_DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE_MASK, DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, bits, DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE_MASK, DX_M1A_NPU0_PE1_CTRL_PE1_CONV_TYPE_OFFSET)

/* DX_M1A_NPU0.PE1_CTRL.PE1_CONV_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN_LEN        1
#define DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN_OFFSET     8
#define DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN_MASK       0x00000100 /* [8:8] 0b00000000000000000000000100000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN_MASK)
#define READ_DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN_MASK, DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, bits, DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN_MASK, DX_M1A_NPU0_PE1_CTRL_PE1_CONV_EN_OFFSET)

/* DX_M1A_NPU0.PE1_CTRL.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CTRL_RESERVED0_LEN        7
#define DX_M1A_NPU0_PE1_CTRL_RESERVED0_OFFSET     9
#define DX_M1A_NPU0_PE1_CTRL_RESERVED0_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_M1A_NPU0_PE1_CTRL_RESERVED0_MASK       0x0000fe00 /* [15:9] 0b00000000000000001111111000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CTRL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_CTRL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_RESERVED0_MASK, DX_M1A_NPU0_PE1_CTRL_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CTRL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, bits, DX_M1A_NPU0_PE1_CTRL_RESERVED0_MASK, DX_M1A_NPU0_PE1_CTRL_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_CTRL.PE1_SYSTOLIC_DEPTH (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_LEN        8
#define DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_OFFSET     16
#define DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_MASK)
#define READ_DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_MASK, DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, bits, DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_MASK, DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_DEPTH_OFFSET)

/* DX_M1A_NPU0.PE1_CTRL.PE1_SYSTOLIC_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_LEN        1
#define DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_OFFSET     24
#define DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_MASK)
#define READ_DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_MASK, DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, bits, DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_MASK, DX_M1A_NPU0_PE1_CTRL_PE1_SYSTOLIC_EN_OFFSET)

/* DX_M1A_NPU0.PE1_CTRL.PE1_SFUIN_VALID_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_LEN        7
#define DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_OFFSET     25
#define DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_MASK_BITS  0x0000007f /* [6:0] 0b00000000000000000000000001111111 */
#define DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_MASK       0xfe000000 /* [31:25] 0b11111110000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_MASK)
#define READ_DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_MASK, DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CTRL, bits, DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_MASK, DX_M1A_NPU0_PE1_CTRL_PE1_SFUIN_VALID_NUM_OFFSET)

/* DX_M1A_NPU0.PE1_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG0)
#define WRITE_DX_M1A_NPU0_PE1_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG0, val)
/* DX_M1A_NPU0.PE1_CFG0.PE1_LAST_CONV_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_LEN        26
#define DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_MASK_BITS  0x03ffffff /* [25:0] 0b00000011111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_MASK       0x03ffffff /* [25:0] 0b00000011111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG0, DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG0, DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_MASK, DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG0, bits, DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_MASK, DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_NUM_OFFSET)

/* DX_M1A_NPU0.PE1_CFG0.PE1_LAST_CONV_MOD (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_LEN        6
#define DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_OFFSET     26
#define DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_MASK       0xfc000000 /* [31:26] 0b11111100000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG0, DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG0, DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_MASK, DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG0, bits, DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_MASK, DX_M1A_NPU0_PE1_CFG0_PE1_LAST_CONV_MOD_OFFSET)

/* DX_M1A_NPU0.PE1_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG1)
#define WRITE_DX_M1A_NPU0_PE1_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG1, val)
/* DX_M1A_NPU0.PE1_CFG1.PE1_OFEATURE_CHANNEL (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_LEN        16
#define DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG1, DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG1, DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_MASK, DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG1, bits, DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_MASK, DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_CHANNEL_OFFSET)

/* DX_M1A_NPU0.PE1_CFG1.PE1_OFEATURE_NUMBER (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_LEN        16
#define DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_OFFSET     16
#define DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG1, DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG1, DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_MASK, DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG1, bits, DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_MASK, DX_M1A_NPU0_PE1_CFG1_PE1_OFEATURE_NUMBER_OFFSET)

/* DX_M1A_NPU0.PE1_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG2)
#define WRITE_DX_M1A_NPU0_PE1_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG2, val)
/* DX_M1A_NPU0.PE1_CFG2.PE1_OFEATURE_WIDTH (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_LEN        16
#define DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG2, DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG2, DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_MASK, DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG2, bits, DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_MASK, DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_WIDTH_OFFSET)

/* DX_M1A_NPU0.PE1_CFG2.PE1_OFEATURE_HEIGHT (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_LEN        16
#define DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_OFFSET     16
#define DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG2, DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG2, DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_MASK, DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG2, bits, DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_MASK, DX_M1A_NPU0_PE1_CFG2_PE1_OFEATURE_HEIGHT_OFFSET)

/* DX_M1A_NPU0.PE1_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG3)
#define WRITE_DX_M1A_NPU0_PE1_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG3, val)
/* DX_M1A_NPU0.PE1_CFG3.PE1_OFEATURE_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE_LEN        32
#define DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG3, DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG3, DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE_MASK, DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG3, bits, DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE_MASK, DX_M1A_NPU0_PE1_CFG3_PE1_OFEATURE_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG4)
#define WRITE_DX_M1A_NPU0_PE1_CFG4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG4, val)
/* DX_M1A_NPU0.PE1_CFG4.PE1_FEATURE_HEIGHT (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_LEN        16
#define DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG4, DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG4, DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_MASK, DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG4, bits, DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_MASK, DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_HEIGHT_OFFSET)

/* DX_M1A_NPU0.PE1_CFG4.PE1_FEATURE_CHANNEL (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL_LEN        16
#define DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL_OFFSET     16
#define DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG4, DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG4, DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL_MASK, DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG4, bits, DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL_MASK, DX_M1A_NPU0_PE1_CFG4_PE1_FEATURE_CHANNEL_OFFSET)

/* DX_M1A_NPU0.PE1_CFG5 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG5(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG5)
#define WRITE_DX_M1A_NPU0_PE1_CFG5(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG5, val)
/* DX_M1A_NPU0.PE1_CFG5.PE1_FILTER_NUMBER (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_LEN        16
#define DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG5, DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG5, DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_MASK, DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG5, bits, DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_MASK, DX_M1A_NPU0_PE1_CFG5_PE1_FILTER_NUMBER_OFFSET)

/* DX_M1A_NPU0.PE1_CFG5.PE1_FEATURE_WIDTH (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH_LEN        16
#define DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH_OFFSET     16
#define DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG5, DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG5, DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH_MASK, DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG5, bits, DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH_MASK, DX_M1A_NPU0_PE1_CFG5_PE1_FEATURE_WIDTH_OFFSET)

/* DX_M1A_NPU0.PE1_CFG6 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG6(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG6)
#define WRITE_DX_M1A_NPU0_PE1_CFG6(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, val)
/* DX_M1A_NPU0.PE1_CFG6.PE1_PAD_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE_LEN        1
#define DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE_MASK, DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, bits, DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE_MASK, DX_M1A_NPU0_PE1_CFG6_PE1_PAD_MODE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG6.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG6_RESERVED0_LEN        5
#define DX_M1A_NPU0_PE1_CFG6_RESERVED0_OFFSET     1
#define DX_M1A_NPU0_PE1_CFG6_RESERVED0_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE1_CFG6_RESERVED0_MASK       0x0000003e /* [5:1] 0b00000000000000000000000000111110 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG6_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, DX_M1A_NPU0_PE1_CFG6_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG6_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, DX_M1A_NPU0_PE1_CFG6_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG6_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG6_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, bits, DX_M1A_NPU0_PE1_CFG6_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG6_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_CFG6.PE1_FILTER_WIDTH (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_LEN        5
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_OFFSET     6
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_MASK       0x000007c0 /* [10:6] 0b00000000000000000000011111000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_MASK, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, bits, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_MASK, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_WIDTH_OFFSET)

/* DX_M1A_NPU0.PE1_CFG6.PE1_FILTER_HEIGHT (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_LEN        5
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_OFFSET     11
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_MASK       0x0000f800 /* [15:11] 0b00000000000000001111100000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_MASK, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, bits, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_MASK, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_HEIGHT_OFFSET)

/* DX_M1A_NPU0.PE1_CFG6.PE1_FILTER_CHANNEL (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL_LEN        16
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL_OFFSET     16
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL_MASK, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG6, bits, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL_MASK, DX_M1A_NPU0_PE1_CFG6_PE1_FILTER_CHANNEL_OFFSET)

/* DX_M1A_NPU0.PE1_CFG7 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG7(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG7)
#define WRITE_DX_M1A_NPU0_PE1_CFG7(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, val)
/* DX_M1A_NPU0.PE1_CFG7.PE1_PAD_TOP_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_LEN        6
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_MASK, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, bits, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_MASK, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_TOP_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG7.PE1_PAD_BOTTOM_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_LEN        6
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_OFFSET     6
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_MASK       0x00000fc0 /* [11:6] 0b00000000000000000000111111000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_MASK, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, bits, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_MASK, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_BOTTOM_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG7.PE1_PAD_LEFT_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_LEN        6
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_OFFSET     12
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_MASK       0x0003f000 /* [17:12] 0b00000000000000111111000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_MASK, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, bits, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_MASK, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_LEFT_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG7.PE1_PAD_RIGHT_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_LEN        6
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_OFFSET     18
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_MASK       0x00fc0000 /* [23:18] 0b00000000111111000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_MASK, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, bits, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_MASK, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_RIGHT_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG7.PE1_PAD_VALUE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE_LEN        8
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE_OFFSET     24
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE_MASK       0xff000000 /* [31:24] 0b11111111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE_MASK, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG7, bits, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE_MASK, DX_M1A_NPU0_PE1_CFG7_PE1_PAD_VALUE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG8 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG8(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG8)
#define WRITE_DX_M1A_NPU0_PE1_CFG8(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, val)
/* DX_M1A_NPU0.PE1_CFG8.PE1_STRIDE_X (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X_LEN        5
#define DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X_MASK       0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X_MASK, DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, bits, DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X_MASK, DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_X_OFFSET)

/* DX_M1A_NPU0.PE1_CFG8.PE1_STRIDE_Y (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y_LEN        5
#define DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y_OFFSET     5
#define DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y_MASK       0x000003e0 /* [9:5] 0b00000000000000000000001111100000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y_MASK, DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, bits, DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y_MASK, DX_M1A_NPU0_PE1_CFG8_PE1_STRIDE_Y_OFFSET)

/* DX_M1A_NPU0.PE1_CFG8.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG8_RESERVED0_LEN        6
#define DX_M1A_NPU0_PE1_CFG8_RESERVED0_OFFSET     10
#define DX_M1A_NPU0_PE1_CFG8_RESERVED0_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_CFG8_RESERVED0_MASK       0x0000fc00 /* [15:10] 0b00000000000000001111110000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG8_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, DX_M1A_NPU0_PE1_CFG8_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG8_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, DX_M1A_NPU0_PE1_CFG8_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG8_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG8_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, bits, DX_M1A_NPU0_PE1_CFG8_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG8_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_CFG8.PE1_CHANNEL_OFFSET (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_LEN        16
#define DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_OFFSET     16
#define DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_MASK, DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG8, bits, DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_MASK, DX_M1A_NPU0_PE1_CFG8_PE1_CHANNEL_OFFSET_OFFSET)

/* DX_M1A_NPU0.PE1_CFG9 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG9(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG9)
#define WRITE_DX_M1A_NPU0_PE1_CFG9(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, val)
/* DX_M1A_NPU0.PE1_CFG9.PE1_ELEM_CHANNEL_OFFSET (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET_LEN        16
#define DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET_MASK, DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, bits, DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET_MASK, DX_M1A_NPU0_PE1_CFG9_PE1_ELEM_CHANNEL_OFFSET_OFFSET)

/* DX_M1A_NPU0.PE1_CFG9.PE1_DILATION_X (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X_LEN        6
#define DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X_OFFSET     16
#define DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X_MASK       0x003f0000 /* [21:16] 0b00000000001111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X_MASK, DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, bits, DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X_MASK, DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_X_OFFSET)

/* DX_M1A_NPU0.PE1_CFG9.PE1_DILATION_Y (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y_LEN        6
#define DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y_OFFSET     22
#define DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y_MASK       0x0fc00000 /* [27:22] 0b00001111110000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y_MASK, DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, bits, DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y_MASK, DX_M1A_NPU0_PE1_CFG9_PE1_DILATION_Y_OFFSET)

/* DX_M1A_NPU0.PE1_CFG9.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG9_RESERVED0_LEN        4
#define DX_M1A_NPU0_PE1_CFG9_RESERVED0_OFFSET     28
#define DX_M1A_NPU0_PE1_CFG9_RESERVED0_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_NPU0_PE1_CFG9_RESERVED0_MASK       0xf0000000 /* [31:28] 0b11110000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG9_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, DX_M1A_NPU0_PE1_CFG9_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG9_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, DX_M1A_NPU0_PE1_CFG9_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG9_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG9_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG9, bits, DX_M1A_NPU0_PE1_CFG9_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG9_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_CFG10 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG10(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG10)
#define WRITE_DX_M1A_NPU0_PE1_CFG10(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG10, val)
/* DX_M1A_NPU0.PE1_CFG10.PE1_READ_FEATURE_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG10, DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG10, DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG10, bits, DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG10_PE1_READ_FEATURE_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_CFG11 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG11(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG11)
#define WRITE_DX_M1A_NPU0_PE1_CFG11(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG11, val)
/* DX_M1A_NPU0.PE1_CFG11.PE1_WRITE_FEATURE_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG11, DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG11, DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG11, bits, DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG11_PE1_WRITE_FEATURE_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_CFG12 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG12(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG12)
#define WRITE_DX_M1A_NPU0_PE1_CFG12(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG12, val)
/* DX_M1A_NPU0.PE1_CFG12.PE1_READ_WEIGHT_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG12, DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG12, DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG12, bits, DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG12_PE1_READ_WEIGHT_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_CFG13 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG13(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG13)
#define WRITE_DX_M1A_NPU0_PE1_CFG13(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG13, val)
/* DX_M1A_NPU0.PE1_CFG13.PE1_ELEM_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG13, DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG13, DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG13, bits, DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG13_PE1_ELEM_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_CFG14 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG14(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG14)
#define WRITE_DX_M1A_NPU0_PE1_CFG14(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG14, val)
/* DX_M1A_NPU0.PE1_CFG14.PE1_MUL_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG14, DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG14, DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG14, bits, DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG14_PE1_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_CFG15 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG15(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG15)
#define WRITE_DX_M1A_NPU0_PE1_CFG15(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG15, val)
/* DX_M1A_NPU0.PE1_CFG15.PE1_ADD_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG15, DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG15, DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG15, bits, DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG15_PE1_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_CFG16 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG16(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG16)
#define WRITE_DX_M1A_NPU0_PE1_CFG16(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG16, val)
/* DX_M1A_NPU0.PE1_CFG16.PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG16, DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG16, DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG16, bits, DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG16_PE1_SKIP_MUL_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_CFG17 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG17(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG17)
#define WRITE_DX_M1A_NPU0_PE1_CFG17(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG17, val)
/* DX_M1A_NPU0.PE1_CFG17.PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG17, DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG17, DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG17, bits, DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG17_PE1_SKIP_ADD_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_CFG18 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG18(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG18)
#define WRITE_DX_M1A_NPU0_PE1_CFG18(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG18, val)
/* DX_M1A_NPU0.PE1_CFG18.PE1_SE_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG18, DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG18, DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG18, bits, DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG18_PE1_SE_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_CFG19 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG19(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG19)
#define WRITE_DX_M1A_NPU0_PE1_CFG19(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG19, val)
/* DX_M1A_NPU0.PE1_CFG19.PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG19, DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG19, DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG19, bits, DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG19_PE1_GENERAL_ADDR_GEN_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_CFG20 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG20(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG20)
#define WRITE_DX_M1A_NPU0_PE1_CFG20(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG20, val)
/* DX_M1A_NPU0.PE1_CFG20.PE1_WF_ADDR_MODE1_LINE_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE_LEN        16
#define DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG20, DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG20, DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE_MASK, DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG20, bits, DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE_MASK, DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_MODE1_LINE_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG20.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG20_RESERVED0_LEN        13
#define DX_M1A_NPU0_PE1_CFG20_RESERVED0_OFFSET     16
#define DX_M1A_NPU0_PE1_CFG20_RESERVED0_MASK_BITS  0x00001fff /* [12:0] 0b00000000000000000001111111111111 */
#define DX_M1A_NPU0_PE1_CFG20_RESERVED0_MASK       0x1fff0000 /* [28:16] 0b00011111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG20_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG20, DX_M1A_NPU0_PE1_CFG20_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG20_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG20, DX_M1A_NPU0_PE1_CFG20_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG20_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG20_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG20, bits, DX_M1A_NPU0_PE1_CFG20_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG20_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_CFG20.PE1_WF_ADDR_GEN_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE_LEN        3
#define DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE_OFFSET     29
#define DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE_MASK       0xe0000000 /* [31:29] 0b11100000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG20, DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG20, DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG20, bits, DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE_MASK, DX_M1A_NPU0_PE1_CFG20_PE1_WF_ADDR_GEN_MODE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG21 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG21(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG21)
#define WRITE_DX_M1A_NPU0_PE1_CFG21(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG21, val)
/* DX_M1A_NPU0.PE1_CFG21.PE1_WF_ADDR_MODE1_LINE_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE_LEN        32
#define DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG21, DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG21, DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE_MASK, DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG21, bits, DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE_MASK, DX_M1A_NPU0_PE1_CFG21_PE1_WF_ADDR_MODE1_LINE_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG22 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG22(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG22)
#define WRITE_DX_M1A_NPU0_PE1_CFG22(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG22, val)
/* DX_M1A_NPU0.PE1_CFG22.PE1_WF_ADDR_SURFACE_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE_LEN        32
#define DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG22, DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG22, DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG22, bits, DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE_MASK, DX_M1A_NPU0_PE1_CFG22_PE1_WF_ADDR_SURFACE_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG23 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG23(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG23)
#define WRITE_DX_M1A_NPU0_PE1_CFG23(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG23, val)
/* DX_M1A_NPU0.PE1_CFG23.PE1_WF_WRITE_CNT (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT_LEN        16
#define DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG23, DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG23, DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT_MASK, DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG23, bits, DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT_MASK, DX_M1A_NPU0_PE1_CFG23_PE1_WF_WRITE_CNT_OFFSET)

/* DX_M1A_NPU0.PE1_CFG23.PE1_WF_ADDR_SURFACE_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE_LEN        16
#define DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE_OFFSET     16
#define DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG23, DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG23, DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG23, bits, DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE_MASK, DX_M1A_NPU0_PE1_CFG23_PE1_WF_ADDR_SURFACE_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_CFG24 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG24(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG24)
#define WRITE_DX_M1A_NPU0_PE1_CFG24(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG24, val)
/* DX_M1A_NPU0.PE1_CFG24.PE1_WF_SRC1_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG24, DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG24, DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG24, bits, DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_CFG24_PE1_WF_SRC1_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_CFG25 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG25(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG25)
#define WRITE_DX_M1A_NPU0_PE1_CFG25(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, val)
/* DX_M1A_NPU0.PE1_CFG25.PE1_SFU_OUT_DATA_VALID_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM_LEN        3
#define DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM_MASK_BITS  0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM_MASK       0x00000007 /* [2:0] 0b00000000000000000000000000000111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM_MASK, DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, bits, DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM_MASK, DX_M1A_NPU0_PE1_CFG25_PE1_SFU_OUT_DATA_VALID_NUM_OFFSET)

/* DX_M1A_NPU0.PE1_CFG25.RESERVED1 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG25_RESERVED1_LEN        5
#define DX_M1A_NPU0_PE1_CFG25_RESERVED1_OFFSET     3
#define DX_M1A_NPU0_PE1_CFG25_RESERVED1_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE1_CFG25_RESERVED1_MASK       0x000000f8 /* [7:3] 0b00000000000000000000000011111000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG25_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, DX_M1A_NPU0_PE1_CFG25_RESERVED1_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG25_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, DX_M1A_NPU0_PE1_CFG25_RESERVED1_MASK, DX_M1A_NPU0_PE1_CFG25_RESERVED1_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG25_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, bits, DX_M1A_NPU0_PE1_CFG25_RESERVED1_MASK, DX_M1A_NPU0_PE1_CFG25_RESERVED1_OFFSET)

/* DX_M1A_NPU0.PE1_CFG25.PE1_ST_ADDR_GEN_SHIFT_LEN (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN_LEN        5
#define DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN_OFFSET     8
#define DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN_MASK       0x00001f00 /* [12:8] 0b00000000000000000001111100000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN_MASK, DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, bits, DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN_MASK, DX_M1A_NPU0_PE1_CFG25_PE1_ST_ADDR_GEN_SHIFT_LEN_OFFSET)

/* DX_M1A_NPU0.PE1_CFG25.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG25_RESERVED0_LEN        19
#define DX_M1A_NPU0_PE1_CFG25_RESERVED0_OFFSET     13
#define DX_M1A_NPU0_PE1_CFG25_RESERVED0_MASK_BITS  0x0007ffff /* [18:0] 0b00000000000001111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG25_RESERVED0_MASK       0xffffe000 /* [31:13] 0b11111111111111111110000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG25_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, DX_M1A_NPU0_PE1_CFG25_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG25_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, DX_M1A_NPU0_PE1_CFG25_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG25_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG25_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG25, bits, DX_M1A_NPU0_PE1_CFG25_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG25_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_CFG26 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_CFG26(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_CFG26)
#define WRITE_DX_M1A_NPU0_PE1_CFG26(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_CFG26, val)
/* DX_M1A_NPU0.PE1_CFG26.PE1_ST_ADDR_GEN_MUL (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL_LEN        31
#define DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL_OFFSET     0
#define DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL_MASK_BITS  0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL_MASK       0x7fffffff /* [30:0] 0b01111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG26, DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG26, DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL_MASK, DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG26, bits, DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL_MASK, DX_M1A_NPU0_PE1_CFG26_PE1_ST_ADDR_GEN_MUL_OFFSET)

/* DX_M1A_NPU0.PE1_CFG26.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_CFG26_RESERVED0_LEN        1
#define DX_M1A_NPU0_PE1_CFG26_RESERVED0_OFFSET     31
#define DX_M1A_NPU0_PE1_CFG26_RESERVED0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_CFG26_RESERVED0_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_CFG26_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_CFG26, DX_M1A_NPU0_PE1_CFG26_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_CFG26_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_CFG26, DX_M1A_NPU0_PE1_CFG26_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG26_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_CFG26_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_CFG26, bits, DX_M1A_NPU0_PE1_CFG26_RESERVED0_MASK, DX_M1A_NPU0_PE1_CFG26_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_VMEM_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_VMEM_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0)
#define WRITE_DX_M1A_NPU0_PE1_VMEM_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, val)
/* DX_M1A_NPU0.PE1_VMEM_CFG0.PE1_START_NO_IN_BUFF (Category: NPU0) */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_LEN        6
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_OFFSET     0
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_MASK       0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_MASK)
#define READ_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, bits, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_IN_BUFF_OFFSET)

/* DX_M1A_NPU0.PE1_VMEM_CFG0.RESERVED3 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3_LEN        2
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3_OFFSET     6
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3_MASK       0x000000c0 /* [7:6] 0b00000000000000000000000011000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3_MASK)
#define READ_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, bits, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED3_OFFSET)

/* DX_M1A_NPU0.PE1_VMEM_CFG0.PE1_START_NO_WEIGHT_BUFF (Category: NPU0) */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_LEN        6
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_OFFSET     8
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_MASK       0x00003f00 /* [13:8] 0b00000000000000000011111100000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_MASK)
#define READ_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, bits, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_WEIGHT_BUFF_OFFSET)

/* DX_M1A_NPU0.PE1_VMEM_CFG0.RESERVED2 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2_LEN        2
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2_OFFSET     14
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2_MASK       0x0000c000 /* [15:14] 0b00000000000000001100000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2_MASK)
#define READ_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, bits, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED2_OFFSET)

/* DX_M1A_NPU0.PE1_VMEM_CFG0.PE1_START_NO_OUT_BUFF (Category: NPU0) */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_LEN        6
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_OFFSET     16
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_MASK       0x003f0000 /* [21:16] 0b00000000001111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_MASK)
#define READ_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, bits, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_OUT_BUFF_OFFSET)

/* DX_M1A_NPU0.PE1_VMEM_CFG0.RESERVED1 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1_LEN        2
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1_OFFSET     22
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1_MASK       0x00c00000 /* [23:22] 0b00000000110000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1_MASK)
#define READ_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, bits, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED1_OFFSET)

/* DX_M1A_NPU0.PE1_VMEM_CFG0.PE1_START_NO_ELEM_BUFF (Category: NPU0) */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_LEN        6
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_OFFSET     24
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_MASK       0x3f000000 /* [29:24] 0b00111111000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_MASK)
#define READ_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, bits, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_PE1_START_NO_ELEM_BUFF_OFFSET)

/* DX_M1A_NPU0.PE1_VMEM_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0_LEN        2
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0_OFFSET     30
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0_MASK       0xc0000000 /* [31:30] 0b11000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_VMEM_CFG0, bits, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE1_VMEM_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.REDUCE1_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_REDUCE1_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, val)
/* DX_M1A_NPU0.REDUCE1_CFG0.REDUCE1_CHANNEL_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE_LEN        8
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE_OFFSET     0
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE_MASK)
#define READ_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE_MASK, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, bits, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE_MASK, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_SIZE_OFFSET)

/* DX_M1A_NPU0.REDUCE1_CFG0.REDUCE1_CHANNEL_NUM (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM_LEN        16
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM_OFFSET     8
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM_MASK       0x00ffff00 /* [23:8] 0b00000000111111111111111100000000 */
#define READ_MASK_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM_MASK)
#define READ_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM_MASK, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, bits, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM_MASK, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_CHANNEL_NUM_OFFSET)

/* DX_M1A_NPU0.REDUCE1_CFG0.REDUCE1_MODE (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE_LEN        2
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE_OFFSET     24
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE_MASK       0x03000000 /* [25:24] 0b00000011000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE_MASK)
#define READ_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE_MASK, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, bits, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE_MASK, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_MODE_OFFSET)

/* DX_M1A_NPU0.REDUCE1_CFG0.REDUCE1_EN (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN_LEN        1
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN_OFFSET     26
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN_MASK       0x04000000 /* [26:26] 0b00000100000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN_MASK)
#define READ_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN_MASK, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, bits, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN_MASK, DX_M1A_NPU0_REDUCE1_CFG0_REDUCE1_EN_OFFSET)

/* DX_M1A_NPU0.REDUCE1_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0_LEN        5
#define DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0_OFFSET     27
#define DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0_MASK       0xf8000000 /* [31:27] 0b11111000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0_MASK, DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG0, bits, DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0_MASK, DX_M1A_NPU0_REDUCE1_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.REDUCE1_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_REDUCE1_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG1)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG1, val)
/* DX_M1A_NPU0.REDUCE1_CFG1.REDUCE1_WIN_NUM (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM_LEN        16
#define DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM_OFFSET     0
#define DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG1, DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM_MASK)
#define READ_DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG1, DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM_MASK, DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG1, bits, DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM_MASK, DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_WIN_NUM_OFFSET)

/* DX_M1A_NPU0.REDUCE1_CFG1.REDUCE1_CNT_MAX_NUM (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM_LEN        16
#define DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM_OFFSET     16
#define DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG1, DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM_MASK)
#define READ_DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG1, DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM_MASK, DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG1, bits, DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM_MASK, DX_M1A_NPU0_REDUCE1_CFG1_REDUCE1_CNT_MAX_NUM_OFFSET)

/* DX_M1A_NPU0.REDUCE1_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_REDUCE1_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG2)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG2, val)
/* DX_M1A_NPU0.REDUCE1_CFG2.REDUCE1_RD_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG2, DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG2, DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR_MASK, DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG2, bits, DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR_MASK, DX_M1A_NPU0_REDUCE1_CFG2_REDUCE1_RD_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.REDUCE1_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_REDUCE1_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG3)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG3, val)
/* DX_M1A_NPU0.REDUCE1_CFG3.REDUCE1_WR_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG3, DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG3, DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR_MASK, DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_REDUCE1_CFG3, bits, DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR_MASK, DX_M1A_NPU0_REDUCE1_CFG3_REDUCE1_WR_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_CTRL (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_CTRL(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL)
#define WRITE_DX_M1A_NPU0_PE1_DMA_CTRL(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, val)
/* DX_M1A_NPU0.PE1_DMA_CTRL.VMEM1_START_OP_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN_LEN        1
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN_MASK       0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN_MASK, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, bits, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN_MASK, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_START_OP_EN_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_CTRL.VMEM1_DMA_CTRL_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE_LEN        2
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE_OFFSET     1
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE_MASK       0x00000006 /* [2:1] 0b00000000000000000000000000000110 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE_MASK, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, bits, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE_MASK, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_DMA_CTRL_MODE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_CTRL.VMEM1_WEIGHT_BIT_WIDTH (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH_LEN        8
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH_OFFSET     3
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH_MASK       0x000007f8 /* [10:3] 0b00000000000000000000011111111000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH_MASK, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, bits, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH_MASK, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_BIT_WIDTH_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_CTRL.VMEM1_WEIGHT_DUMMY_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE_LEN        2
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE_OFFSET     11
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE_MASK       0x00001800 /* [12:11] 0b00000000000000000001100000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE_MASK, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, bits, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE_MASK, DX_M1A_NPU0_PE1_DMA_CTRL_VMEM1_WEIGHT_DUMMY_MODE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_CTRL.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0_LEN        19
#define DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0_OFFSET     13
#define DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0_MASK_BITS  0x0007ffff /* [18:0] 0b00000000000001111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0_MASK       0xffffe000 /* [31:13] 0b11111111111111111110000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0_MASK, DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_CTRL, bits, DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0_MASK, DX_M1A_NPU0_PE1_DMA_CTRL_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, val)
/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.VMEM1_RD_ELEM_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE_LEN        16
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.VMEM1_RD_ELEM_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME_LEN        6
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME_OFFSET     16
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME_MASK       0x003f0000 /* [21:16] 0b00000000001111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_TIME_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.VMEM1_RD_ELEM_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE_LEN        2
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE_OFFSET     22
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE_MASK       0x00c00000 /* [23:22] 0b00000000110000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_MODE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.VMEM1_RD_ELEM_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN_LEN        1
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN_OFFSET     24
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_EN_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.VMEM1_RD_ELEM_SRC (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC_LEN        1
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC_OFFSET     25
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC_MASK       0x02000000 /* [25:25] 0b00000010000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_VMEM1_RD_ELEM_SRC_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_LEN        6
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_OFFSET     26
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_MASK_BITS  0x0000003f /* [5:0] 0b00000000000000000000000000111111 */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_MASK       0xfc000000 /* [31:26] 0b11111100000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG0, bits, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG1)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG1, val)
/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG1.VMEM1_RD_ELEM_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE_LEN        32
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG1, DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG1, DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG1, bits, DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG1_VMEM1_RD_ELEM_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG2)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG2, val)
/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG2.VMEM1_RD_ELEM_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG2, DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG2, DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG2, bits, DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG2_VMEM1_RD_ELEM_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG3)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG3, val)
/* DX_M1A_NPU0.PE1_DMA_ELEM_CFG3.VMEM1_WT_ELEM_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG3, DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG3, DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_ELEM_CFG3, bits, DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_ELEM_CFG3_VMEM1_WT_ELEM_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_W_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, val)
/* DX_M1A_NPU0.PE1_DMA_W_CFG0.VMEM1_RD_W_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME_LEN        12
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME_MASK       0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, bits, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_TIME_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_W_CFG0.VMEM1_RD_W_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE_LEN        2
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE_OFFSET     12
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE_MASK       0x00003000 /* [13:12] 0b00000000000000000011000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, bits, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_MODE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_W_CFG0.VMEM1_RD_W_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN_LEN        1
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN_OFFSET     14
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN_MASK       0x00004000 /* [14:14] 0b00000000000000000100000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, bits, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG0_VMEM1_RD_W_EN_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_W_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0_LEN        17
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0_OFFSET     15
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0_MASK_BITS  0x0001ffff /* [16:0] 0b00000000000000011111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0_MASK       0xffff8000 /* [31:15] 0b11111111111111111000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG0, bits, DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_W_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG1)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG1, val)
/* DX_M1A_NPU0.PE1_DMA_W_CFG1.VMEM1_RD_W_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE_LEN        16
#define DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG1, DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG1, DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG1, bits, DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_W_CFG1.VMEM1_RD_W_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE_LEN        16
#define DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE_OFFSET     16
#define DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG1, DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG1, DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG1, bits, DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG1_VMEM1_RD_W_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_W_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG2)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG2, val)
/* DX_M1A_NPU0.PE1_DMA_W_CFG2.VMEM1_RD_W_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG2, DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG2, DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG2, bits, DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG2_VMEM1_RD_W_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_W_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG3)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG3, val)
/* DX_M1A_NPU0.PE1_DMA_W_CFG3.VMEM1_WT_W_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG3, DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG3, DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_W_CFG3, bits, DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_W_CFG3_VMEM1_WT_W_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_INF_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, val)
/* DX_M1A_NPU0.PE1_DMA_INF_CFG0.VMEM1_RD_INF_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE_LEN        16
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, bits, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_INF_CFG0.VMEM1_RD_INF_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME_LEN        12
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME_OFFSET     16
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, bits, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_TIME_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_INF_CFG0.VMEM1_RD_INF_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE_LEN        2
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE_OFFSET     28
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE_MASK       0x30000000 /* [29:28] 0b00110000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, bits, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_MODE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_INF_CFG0.VMEM1_RD_INF_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN_LEN        1
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN_OFFSET     30
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, bits, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_EN_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_INF_CFG0.VMEM1_RD_INF_SRC (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC_LEN        1
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC_OFFSET     31
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG0, bits, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG0_VMEM1_RD_INF_SRC_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_INF_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG1)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG1, val)
/* DX_M1A_NPU0.PE1_DMA_INF_CFG1.VMEM1_RD_INF_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE_LEN        32
#define DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG1, DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG1, DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG1, bits, DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG1_VMEM1_RD_INF_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_INF_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG2)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG2, val)
/* DX_M1A_NPU0.PE1_DMA_INF_CFG2.VMEM1_RD_INF_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG2, DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG2, DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG2, bits, DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG2_VMEM1_RD_INF_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_INF_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG3)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG3, val)
/* DX_M1A_NPU0.PE1_DMA_INF_CFG3.VMEM1_WT_INF_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE_LEN        32
#define DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG3, DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG3, DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG3, bits, DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG3_VMEM1_WT_INF_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_INF_CFG4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG4)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG4, val)
/* DX_M1A_NPU0.PE1_DMA_INF_CFG4.VMEM1_WT_INF_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG4, DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG4, DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_INF_CFG4, bits, DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_INF_CFG4_VMEM1_WT_INF_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, val)
/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG0.VMEM1_WT_OUTF_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE_LEN        16
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, bits, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_SIZE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG0.VMEM1_WT_OUTF_TIME (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME_LEN        12
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME_OFFSET     16
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME_MASK_BITS  0x00000fff /* [11:0] 0b00000000000000000000111111111111 */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME_MASK       0x0fff0000 /* [27:16] 0b00001111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, bits, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_TIME_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG0.RESERVED1 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_LEN        2
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_OFFSET     28
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_MASK_BITS  0x00000003 /* [1:0] 0b00000000000000000000000000000011 */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_MASK       0x30000000 /* [29:28] 0b00110000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, bits, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED1_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG0.VMEM1_WT_OUTF_EN (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN_LEN        1
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN_OFFSET     30
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN_MASK       0x40000000 /* [30:30] 0b01000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, bits, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_VMEM1_WT_OUTF_EN_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_LEN        1
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_OFFSET     31
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG0, bits, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG1)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG1, val)
/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG1.VMEM1_WT_OUTF_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE_LEN        32
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG1, DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG1, DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG1, bits, DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG1_VMEM1_WT_OUTF_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG2)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG2, val)
/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG2.VMEM1_WT_OUTF_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG2, DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG2, DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG2, bits, DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG2_VMEM1_WT_OUTF_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG3)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG3, val)
/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG3.VMEM1_RD_OUTF_STRIDE (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE_LEN        32
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG3, DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG3, DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG3, bits, DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG3_VMEM1_RD_OUTF_STRIDE_OFFSET)

/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG4)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG4, val)
/* DX_M1A_NPU0.PE1_DMA_OUTF_CFG4.VMEM1_RD_OUTF_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG4, DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG4, DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PE1_DMA_OUTF_CFG4, bits, DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR_MASK, DX_M1A_NPU0_PE1_DMA_OUTF_CFG4_VMEM1_RD_OUTF_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_0 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PPU_CFG_0(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0)
#define WRITE_DX_M1A_NPU0_PPU_CFG_0(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, val)
/* DX_M1A_NPU0.PPU_CFG_0.PPU_GRID_HEIGHT_OFFSET (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET_LEN        8
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET_OFFSET     0
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, bits, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_0.PPU_GRID_HEIGHT (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_LEN        8
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET     8
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, bits, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_HEIGHT_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_0.PPU_GRID_WIDTH (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH_LEN        8
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH_OFFSET     16
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH_MASK       0x00ff0000 /* [23:16] 0b00000000111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, bits, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_GRID_WIDTH_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_0.PPU_VMEM_WRITE_EN (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN_LEN        1
#define DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN_OFFSET     24
#define DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN_MASK       0x01000000 /* [24:24] 0b00000001000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, bits, DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_VMEM_WRITE_EN_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_0.PPU_1ST_TILE_EN (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN_LEN        1
#define DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN_OFFSET     25
#define DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN_MASK       0x02000000 /* [25:25] 0b00000010000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, bits, DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_1ST_TILE_EN_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_0.PPU_EN (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_EN_LEN        1
#define DX_M1A_NPU0_PPU_CFG_0_PPU_EN_OFFSET     26
#define DX_M1A_NPU0_PPU_CFG_0_PPU_EN_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PPU_CFG_0_PPU_EN_MASK       0x04000000 /* [26:26] 0b00000100000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_0_PPU_EN(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_EN_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_0_PPU_EN(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_PPU_EN_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_EN_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_0_PPU_EN(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, bits, DX_M1A_NPU0_PPU_CFG_0_PPU_EN_MASK, DX_M1A_NPU0_PPU_CFG_0_PPU_EN_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_0.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_0_RESERVED0_LEN        5
#define DX_M1A_NPU0_PPU_CFG_0_RESERVED0_OFFSET     27
#define DX_M1A_NPU0_PPU_CFG_0_RESERVED0_MASK_BITS  0x0000001f /* [4:0] 0b00000000000000000000000000011111 */
#define DX_M1A_NPU0_PPU_CFG_0_RESERVED0_MASK       0xf8000000 /* [31:27] 0b11111000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_0_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_0_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, DX_M1A_NPU0_PPU_CFG_0_RESERVED0_MASK, DX_M1A_NPU0_PPU_CFG_0_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_0_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_0, bits, DX_M1A_NPU0_PPU_CFG_0_RESERVED0_MASK, DX_M1A_NPU0_PPU_CFG_0_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_1 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PPU_CFG_1(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PPU_CFG_1)
#define WRITE_DX_M1A_NPU0_PPU_CFG_1(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PPU_CFG_1, val)
/* DX_M1A_NPU0.PPU_CFG_1.PPU_OUT_FEATURE_CHANNEL (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL_LEN        15
#define DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL_OFFSET     0
#define DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL_MASK_BITS  0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL_MASK       0x00007fff /* [14:0] 0b00000000000000000111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_1, DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_1, DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL_MASK, DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_1, bits, DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL_MASK, DX_M1A_NPU0_PPU_CFG_1_PPU_OUT_FEATURE_CHANNEL_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_1.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_1_RESERVED0_LEN        1
#define DX_M1A_NPU0_PPU_CFG_1_RESERVED0_OFFSET     15
#define DX_M1A_NPU0_PPU_CFG_1_RESERVED0_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PPU_CFG_1_RESERVED0_MASK       0x00008000 /* [15:15] 0b00000000000000001000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_1_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_1, DX_M1A_NPU0_PPU_CFG_1_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_1_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_1, DX_M1A_NPU0_PPU_CFG_1_RESERVED0_MASK, DX_M1A_NPU0_PPU_CFG_1_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_1_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_1, bits, DX_M1A_NPU0_PPU_CFG_1_RESERVED0_MASK, DX_M1A_NPU0_PPU_CFG_1_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_1.PPU_ARG_CLASS_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM_LEN        16
#define DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM_OFFSET     16
#define DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_1, DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_1, DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM_MASK, DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_1, bits, DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM_MASK, DX_M1A_NPU0_PPU_CFG_1_PPU_ARG_CLASS_NUM_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_2 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PPU_CFG_2(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PPU_CFG_2)
#define WRITE_DX_M1A_NPU0_PPU_CFG_2(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PPU_CFG_2, val)
/* DX_M1A_NPU0.PPU_CFG_2.PPU_FILTER_IN_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE_LEN        16
#define DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE_OFFSET     0
#define DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE_MASK       0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_2, DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_2, DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE_MASK, DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_2, bits, DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE_MASK, DX_M1A_NPU0_PPU_CFG_2_PPU_FILTER_IN_SIZE_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_2.PPU_ARG_IN_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE_LEN        16
#define DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE_OFFSET     16
#define DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE_MASK_BITS  0x0000ffff /* [15:0] 0b00000000000000001111111111111111 */
#define DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE_MASK       0xffff0000 /* [31:16] 0b11111111111111110000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_2, DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_2, DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE_MASK, DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_2, bits, DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE_MASK, DX_M1A_NPU0_PPU_CFG_2_PPU_ARG_IN_SIZE_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_3 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PPU_CFG_3(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PPU_CFG_3)
#define WRITE_DX_M1A_NPU0_PPU_CFG_3(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PPU_CFG_3, val)
/* DX_M1A_NPU0.PPU_CFG_3.PPU_FILTER_THR (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR_LEN        32
#define DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR_OFFSET     0
#define DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_3, DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_3, DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR_MASK, DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_3, bits, DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR_MASK, DX_M1A_NPU0_PPU_CFG_3_PPU_FILTER_THR_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_4 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PPU_CFG_4(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4)
#define WRITE_DX_M1A_NPU0_PPU_CFG_4(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, val)
/* DX_M1A_NPU0.PPU_CFG_4.PPU_ANCHOR_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM_LEN        8
#define DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM_OFFSET     0
#define DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM_MASK       0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, bits, DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_ANCHOR_NUM_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_4.PPU_CONV_NUM (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM_LEN        8
#define DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM_OFFSET     8
#define DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM_MASK_BITS  0x000000ff /* [7:0] 0b00000000000000000000000011111111 */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM_MASK       0x0000ff00 /* [15:8] 0b00000000000000001111111100000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, bits, DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_CONV_NUM_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_4.PPU_WRITE_ADDR_RST (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST_LEN        1
#define DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST_OFFSET     16
#define DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST_MASK       0x00010000 /* [16:16] 0b00000000000000010000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, bits, DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_WRITE_ADDR_RST_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_4.PPU_DATA_SIZE (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE_LEN        4
#define DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE_OFFSET     17
#define DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE_MASK_BITS  0x0000000f /* [3:0] 0b00000000000000000000000000001111 */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE_MASK       0x001e0000 /* [20:17] 0b00000000000111100000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, bits, DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_DATA_SIZE_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_4.PPU_LABEL_ON (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON_LEN        1
#define DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON_OFFSET     21
#define DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON_MASK       0x00200000 /* [21:21] 0b00000000001000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, bits, DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_LABEL_ON_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_4.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_4_RESERVED0_LEN        9
#define DX_M1A_NPU0_PPU_CFG_4_RESERVED0_OFFSET     22
#define DX_M1A_NPU0_PPU_CFG_4_RESERVED0_MASK_BITS  0x000001ff /* [8:0] 0b00000000000000000000000111111111 */
#define DX_M1A_NPU0_PPU_CFG_4_RESERVED0_MASK       0x7fc00000 /* [30:22] 0b01111111110000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_4_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_4_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_RESERVED0_MASK, DX_M1A_NPU0_PPU_CFG_4_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_4_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, bits, DX_M1A_NPU0_PPU_CFG_4_RESERVED0_MASK, DX_M1A_NPU0_PPU_CFG_4_RESERVED0_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_4.PPU_FILTER_MODE (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE_LEN        1
#define DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE_OFFSET     31
#define DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE_MASK_BITS  0x00000001 /* [0:0] 0b00000000000000000000000000000001 */
#define DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE_MASK       0x80000000 /* [31:31] 0b10000000000000000000000000000000 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_4, bits, DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE_MASK, DX_M1A_NPU0_PPU_CFG_4_PPU_FILTER_MODE_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_6 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PPU_CFG_6(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PPU_CFG_6)
#define WRITE_DX_M1A_NPU0_PPU_CFG_6(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PPU_CFG_6, val)
/* DX_M1A_NPU0.PPU_CFG_6.PPU_READ_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_6, DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_6, DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_6, bits, DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR_MASK, DX_M1A_NPU0_PPU_CFG_6_PPU_READ_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_7 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PPU_CFG_7(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PPU_CFG_7)
#define WRITE_DX_M1A_NPU0_PPU_CFG_7(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PPU_CFG_7, val)
/* DX_M1A_NPU0.PPU_CFG_7.PPU_WRITE_BASE_ADDR (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR_LEN        32
#define DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR_OFFSET     0
#define DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_7, DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_7, DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR_MASK, DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_7, bits, DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR_MASK, DX_M1A_NPU0_PPU_CFG_7_PPU_WRITE_BASE_ADDR_OFFSET)

/* DX_M1A_NPU0.PPU_CFG_9 (Category: NPU0) */
#define READ_DX_M1A_NPU0_PPU_CFG_9(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_PPU_CFG_9)
#define WRITE_DX_M1A_NPU0_PPU_CFG_9(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_PPU_CFG_9, val)
/* DX_M1A_NPU0.PPU_CFG_9.RESERVED0 (Category: NPU0) */
#define DX_M1A_NPU0_PPU_CFG_9_RESERVED0_LEN        32
#define DX_M1A_NPU0_PPU_CFG_9_RESERVED0_OFFSET     0
#define DX_M1A_NPU0_PPU_CFG_9_RESERVED0_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_PPU_CFG_9_RESERVED0_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_PPU_CFG_9_RESERVED0(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_PPU_CFG_9, DX_M1A_NPU0_PPU_CFG_9_RESERVED0_MASK)
#define READ_DX_M1A_NPU0_PPU_CFG_9_RESERVED0(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_9, DX_M1A_NPU0_PPU_CFG_9_RESERVED0_MASK, DX_M1A_NPU0_PPU_CFG_9_RESERVED0_OFFSET)
#define WRITE_DX_M1A_NPU0_PPU_CFG_9_RESERVED0(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_PPU_CFG_9, bits, DX_M1A_NPU0_PPU_CFG_9_RESERVED0_MASK, DX_M1A_NPU0_PPU_CFG_9_RESERVED0_OFFSET)

/* DX_M1A_NPU0.CHECKSUM (Category: NPU0) */
#define READ_DX_M1A_NPU0_CHECKSUM(base) dxrt::readl((void*)base, REG_DX_M1A_NPU0_CHECKSUM)
#define WRITE_DX_M1A_NPU0_CHECKSUM(base, val) dxrt::writel((void*)base, REG_DX_M1A_NPU0_CHECKSUM, val)
/* DX_M1A_NPU0.CHECKSUM.CHECKSUM_TILE (Category: NPU0) */
#define DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE_LEN        32
#define DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE_OFFSET     0
#define DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE_MASK_BITS  0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE_MASK       0xffffffff /* [31:0] 0b11111111111111111111111111111111 */
#define READ_MASK_DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE(base) \
    dxrt::readl_mask((void*)base, REG_DX_M1A_NPU0_CHECKSUM, DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE_MASK)
#define READ_DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE(base) \
    dxrt::readb((void*)base, REG_DX_M1A_NPU0_CHECKSUM, DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE_MASK, DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE_OFFSET)
#define WRITE_DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE(base, bits) \
    dxrt::writeb((void*)base, REG_DX_M1A_NPU0_CHECKSUM, bits, DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE_MASK, DX_M1A_NPU0_CHECKSUM_CHECKSUM_TILE_OFFSET)


int Verify_Dx_m1a_npu0_Registers(dxDX_M1A_NPU0_t* base);

} /* namespace dxrt */
