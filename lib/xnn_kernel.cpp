// XNNPACK Transpose & Quantize Kernels
// Transpose: ARM NEON, x86-64 SSE2 (Windows x64), scalar fallback
// Quantize:  ARM NEON, x86 SSE4.1, scalar fallback
//
// Based on XNNPACK (https://github.com/google/XNNPACK)
// License: BSD 3-Clause

#include "dxrt/xnn_kernel.h"
#include "dxrt/common.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <cmath>
#include <cstring>
#include <algorithm>

// ============================================================================
// ARM NEON Transpose Kernels
// ============================================================================
#ifdef USE_NEON
#include <arm_neon.h>

// Branch prediction hint for unlikely conditions
#define XNN_UNPREDICTABLE(x) __builtin_expect(!!(x), 0)

// Marker for functions that may read slightly beyond buffer boundaries
// (safe when buffers are properly aligned/padded)
#define XNN_OOB_READS

static inline size_t min(size_t a, size_t b) 
{
  return XNN_UNPREDICTABLE(b < a) ? b : a;
}

static inline size_t doz(size_t a, size_t b) 
{
  return XNN_UNPREDICTABLE(b < a) ? a - b : 0;
}

static inline size_t round_down_po2(size_t n, size_t q) 
{
  return n & -q;
}

// ============================================================================
// uint8_t Transpose: 16x16 NEON vzipq kernel from XNNPACK
// ============================================================================

void xnn_x8_transposec_ukernel__16x16_reuse_mov_zip_neon(
    const uint8_t* input,
    uint8_t* output,
    size_t input_stride,
    size_t output_stride,
    size_t block_width,
    size_t block_height) XNN_OOB_READS
{
  assert(block_width == 1 || output_stride >= block_height * sizeof(uint8_t));
  assert(block_height == 1 || input_stride >= block_width * sizeof(uint8_t));

  const size_t tile_height = 16;
  const size_t tile_width = 16;
  const size_t tile_hbytes = tile_height * sizeof(uint8_t);
  const size_t tile_wbytes = tile_width * sizeof(uint8_t);
  const size_t input_reset = tile_wbytes - round_down_po2(block_height, tile_height) * input_stride;
  const size_t output_reset = tile_width * output_stride - round_down_po2(block_height, 2) * sizeof(uint8_t) - tile_hbytes;

  const uint8_t* i0 = input;
  uint8_t* o = (uint8_t*) ((uintptr_t) output - tile_hbytes);
  const size_t minus_output_stride = -output_stride;

  do 
  {
    const size_t rem = min(block_width - 1, 15);
    const size_t oN_stride = rem * output_stride;
    const size_t oN_offset = oN_stride + tile_hbytes;
    size_t bh = block_height;
    for (; bh >= 16; bh -= 16) 
    {
      const uint8x16_t v4_0 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_1 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_2 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_3 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_4 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_5 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_6 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_7 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_8 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_9 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_10 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_11 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_12 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_13 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_14 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);
      const uint8x16_t v4_15 = vld1q_u8(i0); i0 = (uint8_t*) ((uintptr_t) i0 + input_stride);

      const uint8x16x2_t v3_0 = vzipq_u8(v4_0, v4_8);
      const uint8x16x2_t v3_1 = vzipq_u8(v4_1, v4_9);
      const uint8x16x2_t v3_2 = vzipq_u8(v4_2, v4_10);
      const uint8x16x2_t v3_3 = vzipq_u8(v4_3, v4_11);
      const uint8x16x2_t v3_4 = vzipq_u8(v4_4, v4_12);
      const uint8x16x2_t v3_5 = vzipq_u8(v4_5, v4_13);
      const uint8x16x2_t v3_6 = vzipq_u8(v4_6, v4_14);
      const uint8x16x2_t v3_7 = vzipq_u8(v4_7, v4_15);

      const uint8x16x2_t v2_0 = vzipq_u8(v3_0.val[0], v3_4.val[0]);
      const uint8x16x2_t v2_1 = vzipq_u8(v3_0.val[1], v3_4.val[1]);
      const uint8x16x2_t v2_2 = vzipq_u8(v3_1.val[0], v3_5.val[0]);
      const uint8x16x2_t v2_3 = vzipq_u8(v3_1.val[1], v3_5.val[1]);
      const uint8x16x2_t v2_4 = vzipq_u8(v3_2.val[0], v3_6.val[0]);
      const uint8x16x2_t v2_5 = vzipq_u8(v3_2.val[1], v3_6.val[1]);
      const uint8x16x2_t v2_6 = vzipq_u8(v3_3.val[0], v3_7.val[0]);
      const uint8x16x2_t v2_7 = vzipq_u8(v3_3.val[1], v3_7.val[1]);
      const uint8x16x2_t v1_0 = vzipq_u8(v2_0.val[0], v2_4.val[0]);
      const uint8x16x2_t v1_1 = vzipq_u8(v2_0.val[1], v2_4.val[1]);
      const uint8x16x2_t v1_2 = vzipq_u8(v2_1.val[0], v2_5.val[0]);
      const uint8x16x2_t v1_3 = vzipq_u8(v2_1.val[1], v2_5.val[1]);
      const uint8x16x2_t v1_4 = vzipq_u8(v2_2.val[0], v2_6.val[0]);
      const uint8x16x2_t v1_5 = vzipq_u8(v2_2.val[1], v2_6.val[1]);
      const uint8x16x2_t v1_6 = vzipq_u8(v2_3.val[0], v2_7.val[0]);
      const uint8x16x2_t v1_7 = vzipq_u8(v2_3.val[1], v2_7.val[1]);
      const uint8x16x2_t v0_0 = vzipq_u8(v1_0.val[0], v1_4.val[0]);
      const uint8x16x2_t v0_1 = vzipq_u8(v1_0.val[1], v1_4.val[1]);
      const uint8x16x2_t v0_2 = vzipq_u8(v1_1.val[0], v1_5.val[0]);
      const uint8x16x2_t v0_3 = vzipq_u8(v1_1.val[1], v1_5.val[1]);
      const uint8x16x2_t v0_4 = vzipq_u8(v1_2.val[0], v1_6.val[0]);
      const uint8x16x2_t v0_5 = vzipq_u8(v1_2.val[1], v1_6.val[1]);
      const uint8x16x2_t v0_6 = vzipq_u8(v1_3.val[0], v1_7.val[0]);
      const uint8x16x2_t v0_7 = vzipq_u8(v1_3.val[1], v1_7.val[1]);

      o = (uint8_t*) ((uintptr_t) o + oN_offset);
      vst1q_u8(o, v0_7.val[1]);
      uint8_t *oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width > 15)) 
      { 
        o = oN; 
      }
      vst1q_u8(o, v0_7.val[0]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width >= 15)) { o = oN; }
      vst1q_u8(o, v0_6.val[1]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width > 13)) { o = oN; }
      vst1q_u8(o, v0_6.val[0]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width >= 13)) { o = oN; }
      vst1q_u8(o, v0_5.val[1]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width > 11)) { o = oN; }
      vst1q_u8(o, v0_5.val[0]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width >= 11)) { o = oN; }
      vst1q_u8(o, v0_4.val[1]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width > 9)) { o = oN; }
      vst1q_u8(o, v0_4.val[0]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width >= 9)) { o = oN; }
      vst1q_u8(o, v0_3.val[1]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width > 7)) { o = oN; }
      vst1q_u8(o, v0_3.val[0]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width >= 7)) { o = oN; }
      vst1q_u8(o, v0_2.val[1]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width > 5)) { o = oN; }
      vst1q_u8(o, v0_2.val[0]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width >= 5)) { o = oN; }
      vst1q_u8(o, v0_1.val[1]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width > 3)) { o = oN; }
      vst1q_u8(o, v0_1.val[0]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width >= 3)) { o = oN; }
      vst1q_u8(o, v0_0.val[1]);
      oN = (uint8_t*) ((uintptr_t) o + minus_output_stride);
      if (XNN_UNPREDICTABLE(block_width > 1)) { o = oN; }
      vst1q_u8(o, v0_0.val[0]);
    }
    o = (uint8_t*) ((uintptr_t) o + tile_hbytes);

    if (bh != 0) 
    {
      // Partial tile (bh < 16) is NOT handled by this micro kernel.
      // Callers must pass block_height as a multiple of 16 and handle
      // remaining rows via scalar fallback. See xnnpack_transpose().
      assert(0 && "uint8 micro kernel called with block_height not a multiple of 16");
    }

    i0 = (const uint8_t*) ((uintptr_t) i0 + input_reset);
    o = (uint8_t*) ((uintptr_t) o + output_reset);
    block_width = doz(block_width, tile_width);
  } while (block_width != 0);
}

// ============================================================================
// float/uint32 Transpose: 4x4 NEON vqtbl4q kernel
// ============================================================================

void xnn_x32_transposec_ukernel__4x4_aarch64_neon_tbl128(
    const uint32_t* input,
    uint32_t* output,
    size_t input_stride,
    size_t output_stride,
    size_t block_width,
    size_t block_height) XNN_OOB_READS
{
  assert(input != NULL);
  assert(output != NULL);
  
  // Early exit for empty input
  if (block_width == 0 || block_height == 0) 
  {
    return;
  }

  static const uint8_t pos0[16] = {0, 1, 2, 3, 16, 17, 18, 19, 32, 33, 34, 35, 48, 49, 50, 51};
  static const uint8_t pos1[16] = {4, 5, 6, 7, 20, 21, 22, 23, 36, 37, 38, 39, 52, 53, 54, 55};
  static const uint8_t pos2[16] = {8, 9, 10, 11, 24, 25, 26, 27, 40, 41, 42, 43, 56, 57, 58, 59};
  static const uint8_t pos3[16] = {12, 13, 14, 15, 28, 29, 30, 31, 44, 45, 46, 47, 60, 61, 62, 63};

  const size_t tile_height = 4;
  const size_t tile_width = 4;
  const size_t tile_wbytes = tile_width * sizeof(uint32_t);
  const size_t input_reset = tile_wbytes - round_down_po2(block_height, tile_height) * input_stride;
  const size_t output_reset = tile_height * output_stride - round_down_po2(block_height, 2) * sizeof(uint32_t);
  const size_t tile_stride = tile_height * input_stride;

  const uint8_t* i0 = (const uint8_t*) input;
  // Safe pointer initialization: clamp to valid memory range based on block_height
  const uint8_t* i1 = (block_height > 1) ? (const uint8_t*) ((uintptr_t) i0 + input_stride) : i0;
  const uint8_t* i2 = (block_height > 2) ? (const uint8_t*) ((uintptr_t) i0 + 2 * input_stride) : i0;
  const uint8_t* i3 = (block_height > 3) ? (const uint8_t*) ((uintptr_t) i0 + 3 * input_stride) : i0;

  uint8_t* o0 = (uint8_t*) output;
  uint8_t* o1 = (block_width > 1) ? (uint8_t*) ((uintptr_t) o0 + output_stride) : o0;
  uint8_t* o2 = (block_width > 2) ? (uint8_t*) ((uintptr_t) o0 + 2 * output_stride) : o0;
  uint8_t* o3 = (block_width > 3) ? (uint8_t*) ((uintptr_t) o0 + 3 * output_stride) : o0;

  const uint8x16_t vperm0 = vld1q_u8(pos0);
  const uint8x16_t vperm1 = vld1q_u8(pos1);
  const uint8x16_t vperm2 = vld1q_u8(pos2);
  const uint8x16_t vperm3 = vld1q_u8(pos3);
  
  do 
  {
    if (XNN_UNPREDICTABLE(block_width < 2)) 
    {
      o1 = o0;
    }
    if (XNN_UNPREDICTABLE(block_width <= 2)) 
    {
      o2 = o0;
    }
    if (XNN_UNPREDICTABLE(block_width < 4)) 
    {
      o3 = o0;
    }
    size_t bh = block_height;
    for (; bh >= 4; bh -= 4) 
    {
      uint8x16x4_t v;
      v.val[0] = vld1q_u8(i0); i0 = (const uint8_t*) ((uintptr_t) i0 + tile_stride);
      v.val[1] = vld1q_u8(i1); i1 = (const uint8_t*) ((uintptr_t) i1 + tile_stride);
      v.val[2] = vld1q_u8(i2); i2 = (const uint8_t*) ((uintptr_t) i2 + tile_stride);
      v.val[3] = vld1q_u8(i3); i3 = (const uint8_t*) ((uintptr_t) i3 + tile_stride);

      uint8x16_t vres0 = vqtbl4q_u8(v, vperm0);
      uint8x16_t vres1 = vqtbl4q_u8(v, vperm1);
      uint8x16_t vres2 = vqtbl4q_u8(v, vperm2);
      uint8x16_t vres3 = vqtbl4q_u8(v, vperm3);

      vst1q_u8(o3, vres3); o3 = (uint8_t*) ((uintptr_t) o3 + tile_wbytes);
      vst1q_u8(o2, vres2); o2 = (uint8_t*) ((uintptr_t) o2 + tile_wbytes);
      vst1q_u8(o1, vres1); o1 = (uint8_t*) ((uintptr_t) o1 + tile_wbytes);
      vst1q_u8(o0, vres0); o0 = (uint8_t*) ((uintptr_t) o0 + tile_wbytes);
    }

    if (bh != 0) 
    {
      // CRITICAL FIX: Clamp ALL invalid pointers to i0 to prevent OOB access
      if (XNN_UNPREDICTABLE(bh < 4)) 
      {
        i3 = i0;
      }
      if (XNN_UNPREDICTABLE(bh <= 2)) 
      {
        i2 = i0;
      }
      if (XNN_UNPREDICTABLE(bh < 2)) 
      {
        i1 = i0;
      }
      uint8x16x4_t v;
      v.val[0] = vld1q_u8(i0);
      v.val[1] = vld1q_u8(i1);
      v.val[2] = vld1q_u8(i2);
      v.val[3] = vld1q_u8(i3);

      uint8x16_t vres0 = vqtbl4q_u8(v, vperm0);
      uint8x16_t vres1 = vqtbl4q_u8(v, vperm1);
      uint8x16_t vres2 = vqtbl4q_u8(v, vperm2);
      uint8x16_t vres3 = vqtbl4q_u8(v, vperm3);

      uint8x8_t vres0_low = vget_low_u8(vres0);
      uint8x8_t vres1_low = vget_low_u8(vres1);
      uint8x8_t vres2_low = vget_low_u8(vres2);
      uint8x8_t vres3_low = vget_low_u8(vres3);

      if (bh & 2) {
        vst1_u8(o3, vres3_low); o3 += 8;
        vst1_u8(o2, vres2_low); o2 += 8;
        vst1_u8(o1, vres1_low); o1 += 8;
        vst1_u8(o0, vres0_low); o0 += 8;
        vres0_low = vget_high_u8(vres0);
        vres1_low = vget_high_u8(vres1);
        vres2_low = vget_high_u8(vres2);
        vres3_low = vget_high_u8(vres3);
      }
      if (bh & 1) {
        vst1_lane_u32((uint32_t*) o3, vreinterpret_u32_u8(vres3_low), 0);
        vst1_lane_u32((uint32_t*) o2, vreinterpret_u32_u8(vres2_low), 0);
        vst1_lane_u32((uint32_t*) o1, vreinterpret_u32_u8(vres1_low), 0);
        vst1_lane_u32((uint32_t*) o0, vreinterpret_u32_u8(vres0_low), 0);
      }
    }
    i0 = (const uint8_t*) ((uintptr_t) i0 + input_reset);
    // Safe pointer reset for next column iteration
    i1 = (block_height > 1) ? (const uint8_t*) ((uintptr_t) i0 + input_stride) : i0;
    i2 = (block_height > 2) ? (const uint8_t*) ((uintptr_t) i0 + 2 * input_stride) : i0;
    i3 = (block_height > 3) ? (const uint8_t*) ((uintptr_t) i0 + 3 * input_stride) : i0;
    o0 = (uint8_t*) ((uintptr_t) o0 + output_reset);
    o1 = (uint8_t*) ((uintptr_t) o1 + output_reset);
    o2 = (uint8_t*) ((uintptr_t) o2 + output_reset);
    o3 = (uint8_t*) ((uintptr_t) o3 + output_reset);
    block_width = doz(block_width, tile_width);
  } while (block_width != 0);
}

// ============================================================================
// Macro Kernel: Cache-tiled transpose (macro kernel + micro kernel pattern)
// ============================================================================
//
// The macro kernel divides the matrix into cache-friendly blocks and dispatches
// each block to the SIMD micro kernels above. This prevents L1/L2 cache
// thrashing that occurs when micro kernels process very large matrices in a
// single call (strided reads/writes span huge address ranges).
//
// Block sizes are chosen so that source + destination blocks fit in L1D cache
// (conservative 32 KB assumed, typical for ARM Cortex-A55/A76):
//   uint8:  128 × 128 × 1 = 16 KB per side → 32 KB total (aligned to 16×16 micro tile)
//   float:   64 ×  64 × 4 = 16 KB per side → 32 KB total (aligned to  4×4  micro tile)

static constexpr size_t MACRO_BLOCK_X8  = 128;  // multiple of micro tile 16
static constexpr size_t MACRO_BLOCK_X32 = 64;   // multiple of micro tile 4

// Skip macro tiling when the entire matrix fits in L1D cache already
static constexpr size_t MACRO_TILE_THRESHOLD = 32 * 1024;

template<typename T>
static void xnnpack_transpose_macro(
    const T* input, T* output,
    size_t rows, size_t cols)
{
  const size_t block_size = (sizeof(T) == 1) ? MACRO_BLOCK_X8 : MACRO_BLOCK_X32;
  const size_t input_stride  = cols * sizeof(T);
  const size_t output_stride = rows * sizeof(T);

  // Outer loop over row-blocks: keeps source rows in cache across col-blocks
  for (size_t r0 = 0; r0 < rows; r0 += block_size)
  {
    const size_t bh = min(block_size, rows - r0);

    // Inner loop over col-blocks
    for (size_t c0 = 0; c0 < cols; c0 += block_size)
    {
      const size_t bw = min(block_size, cols - c0);

      // Source sub-block: input[r0..r0+bh, c0..c0+bw]
      const T* src_block = input + r0 * cols + c0;
      // Destination sub-block: output[c0..c0+bw, r0..r0+bh]  (transposed)
      T* dst_block = output + c0 * rows + r0;

      // Dispatch to micro kernel — strides are for the full matrix so the
      // micro kernel can advance row-by-row through the sub-block correctly.
      if (sizeof(T) == 1) 
      {
        LOG_DXRT_DBG << "[xnnpack_transpose_macro] uint8 tile: bw=" << bw
                     << " bh=" << bh << " full_bh=" << round_down_po2(bh, 16) << std::endl;
        // uint8: The 16x16 micro kernel cannot handle block_height < 16.
        // Split into full tiles + scalar remainder.
        const size_t full_bh = round_down_po2(bh, 16);
        if (full_bh > 0) 
        {
          xnn_x8_transposec_ukernel__16x16_reuse_mov_zip_neon(
              (const uint8_t*)src_block, (uint8_t*)dst_block,
              input_stride, output_stride, bw, full_bh);
        }
        for (size_t r = full_bh; r < bh; r++) 
        {
          for (size_t c = 0; c < bw; c++) 
          {
            ((uint8_t*)dst_block)[c * output_stride + r] =
                ((const uint8_t*)src_block)[r * input_stride + c];
          }
        }
      } 
      else if (sizeof(T) == 4)
      {
        const size_t full_bw = bw & ~size_t(3);
        LOG_DXRT_DBG << "[xnnpack_transpose_macro] float tile: bw=" << bw
                     << " bh=" << bh << " full_bw=" << full_bw
                     << " remainder=" << (bw - full_bw) << std::endl;
        // float/uint32: split into full 4-column groups for NEON + scalar
        // remainder to avoid page faults from NEON over-read.
        if (full_bw > 0)
        {
          xnn_x32_transposec_ukernel__4x4_aarch64_neon_tbl128(
              (const uint32_t*)src_block, (uint32_t*)dst_block,
              input_stride, output_stride, full_bw, bh);
        }
        const size_t in_stride_elems = input_stride / sizeof(T);
        const size_t out_stride_elems = output_stride / sizeof(T);
        for (size_t r = 0; r < bh; r++)
        {
          for (size_t c = full_bw; c < bw; c++)
          {
            ((uint32_t*)dst_block)[c * out_stride_elems + r] =
                ((const uint32_t*)src_block)[r * in_stride_elems + c];
          }
        }
      }
      else
      {
        LOG_DXRT_DBG << "[xnnpack_transpose_macro] scalar fallback tile: bw=" << bw
                     << " bh=" << bh << " sizeof(T)=" << sizeof(T) << std::endl;
        // Fallback: scalar transpose for unsupported element sizes
        const size_t in_stride_elems = input_stride / sizeof(T);
        const size_t out_stride_elems = output_stride / sizeof(T);
        for (size_t r = 0; r < bh; r++)
        {
          for (size_t c = 0; c < bw; c++)
          {
            ((T*)dst_block)[c * out_stride_elems + r] =
                ((const T*)src_block)[r * in_stride_elems + c];
          }
        }
      }
    }
  }
}

// ============================================================================
// Public API: xnnpack_transpose<T>
// ============================================================================

template<typename T>
void xnnpack_transpose(
    const T* input,
    T* output,
    size_t rows,
    size_t cols)
{
  assert(input != NULL);
  assert(output != NULL);
  
  // Handle trivial cases efficiently
  if (rows == 0 || cols == 0) 
  {
    return;
  }
  
  // 1x1 matrix: just copy the single element
  if (rows == 1 && cols == 1) 
  {
    *output = *input;
    return;
  }
  
  // 1xN or Nx1 vector: transpose is a simple copy
  // (data layout is identical for row/column vectors)
  if (rows == 1 || cols == 1) 
  {
    const size_t count = rows * cols;
    for (size_t i = 0; i < count; ++i) 
    {
      output[i] = input[i];
    }
    return;
  }
  
  const size_t total_bytes = rows * cols * sizeof(T);

  if (total_bytes > MACRO_TILE_THRESHOLD) 
  {
    // Large matrix: macro kernel tiles into cache-sized blocks,
    // then dispatches each block to the SIMD micro kernel.
    xnnpack_transpose_macro<T>(input, output, rows, cols);
  } 
  else 
  {
    // Small matrix: single micro kernel call (no tiling overhead)
    size_t input_stride = cols * sizeof(T);
    size_t output_stride = rows * sizeof(T);

    if (sizeof(T) == 1) 
    {
      LOG_DXRT_DBG << "[xnnpack_transpose] uint8 path: rows=" << rows
                   << " cols=" << cols << " full_rows=" << round_down_po2(rows, 16) << std::endl;
      // uint8: The 16x16 micro kernel cannot handle block_height < 16
      // (partial tile not implemented). Split into full tiles + scalar remainder.
      const size_t full_rows = round_down_po2(rows, 16);
      if (full_rows > 0) 
      {
        xnn_x8_transposec_ukernel__16x16_reuse_mov_zip_neon(
          (const uint8_t*)input, (uint8_t*)output,
          input_stride, output_stride, cols, full_rows);
      }
      for (size_t r = full_rows; r < rows; r++) 
      {
        for (size_t c = 0; c < cols; c++) 
        {
          ((uint8_t*)output)[c * output_stride + r] = ((const uint8_t*)input)[r * input_stride + c];
        }
      }
    } 
    else if (sizeof(T) == 4)
    {
      const size_t full_cols = cols & ~size_t(3);
      LOG_DXRT_DBG << "[xnnpack_transpose] float path: rows=" << rows
                   << " cols=" << cols << " full_cols=" << full_cols
                   << " remainder=" << (cols - full_cols) << std::endl;
      // float/uint32: The x32 4x4 micro kernel always loads 16 bytes (4 elements)
      // per row via NEON ldr q. When cols%4 != 0, this causes a (4 - cols%4)
      // element over-read on the last row which can cross into an unmapped page
      // if the buffer ends at a page boundary.
      // Split: NEON handles full 4-column groups, scalar handles remainder.
      if (full_cols > 0)
      {
        xnn_x32_transposec_ukernel__4x4_aarch64_neon_tbl128(
          (const uint32_t*)input, (uint32_t*)output,
          input_stride, output_stride, full_cols, rows);
      }
      for (size_t r = 0; r < rows; r++)
      {
        for (size_t c = full_cols; c < cols; c++)
        {
          output[c * rows + r] = input[r * cols + c];
        }
      }
    }
    else
    {
      LOG_DXRT_DBG << "[xnnpack_transpose] scalar fallback: rows=" << rows
                   << " cols=" << cols << " sizeof(T)=" << sizeof(T) << std::endl;
      // Fallback: scalar transpose for unsupported element sizes
      for (size_t r = 0; r < rows; r++)
      {
        for (size_t c = 0; c < cols; c++)
        {
          output[c * rows + r] = input[r * cols + c];
        }
      }
    }
  }
}

// Explicit instantiations
template void xnnpack_transpose<uint8_t>(const uint8_t*, uint8_t*, size_t, size_t);
template void xnnpack_transpose<float>(const float*, float*, size_t, size_t);

#elif defined(USE_SSE2)
// ============================================================================
// x86-64 SSE2 Transpose Kernels (Windows x64 / MSVC)
// ============================================================================
//
// SSE2 is part of the x64 baseline, so these kernels need no compiler flag and
// no runtime CPU check. Only whole tiles are loaded with 16-byte vector loads;
// the remaining rows and columns are copied one element at a time, so no load
// reads past the end of a row or of the buffer.
#include <emmintrin.h>
#include <utility>

namespace {

constexpr size_t SSE2_TILE_X8  = 16;   // 16 x 16 bytes per tile
constexpr size_t SSE2_TILE_X32 = 4;    // 4 x 4 32-bit words per tile

// Cache blocks, same sizes as the NEON macro kernel: the source and destination
// blocks together fit in a 32 KB L1D cache.
constexpr size_t SSE2_BLOCK_X8  = 128;  // multiple of SSE2_TILE_X8
constexpr size_t SSE2_BLOCK_X32 = 64;   // multiple of SSE2_TILE_X32

inline __m128i load_row(const uint8_t* p)
{
    return _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
}

inline void store_row(uint8_t* p, __m128i v)
{
    _mm_storeu_si128(reinterpret_cast<__m128i*>(p), v);
}

// 4 x 4 tile of 32-bit words: interleave neighbouring rows, then join the halves.
void transpose_tile_x32(const uint8_t* in, size_t in_stride, uint8_t* out, size_t out_stride)
{
    const __m128i r0 = load_row(in);                  // a0 a1 a2 a3
    const __m128i r1 = load_row(in + in_stride);      // b0 b1 b2 b3
    const __m128i r2 = load_row(in + 2 * in_stride);  // c0 c1 c2 c3
    const __m128i r3 = load_row(in + 3 * in_stride);  // d0 d1 d2 d3

    const __m128i top_lo    = _mm_unpacklo_epi32(r0, r1);  // a0 b0 a1 b1
    const __m128i top_hi    = _mm_unpackhi_epi32(r0, r1);  // a2 b2 a3 b3
    const __m128i bottom_lo = _mm_unpacklo_epi32(r2, r3);  // c0 d0 c1 d1
    const __m128i bottom_hi = _mm_unpackhi_epi32(r2, r3);  // c2 d2 c3 d3

    store_row(out,                  _mm_unpacklo_epi64(top_lo, bottom_lo));  // a0 b0 c0 d0
    store_row(out + out_stride,     _mm_unpackhi_epi64(top_lo, bottom_lo));  // a1 b1 c1 d1
    store_row(out + 2 * out_stride, _mm_unpacklo_epi64(top_hi, bottom_hi));  // a2 b2 c2 d2
    store_row(out + 3 * out_stride, _mm_unpackhi_epi64(top_hi, bottom_hi));  // a3 b3 c3 d3
}

// 16 x 16 tile of bytes: four rounds of byte interleaving, the same network as
// the NEON vzipq_u8 kernel. Each round pairs vector k with vector k + 8, which
// rotates the 8-bit (row, column) index left by one bit; after four rounds the
// row and column indices have swapped.
void transpose_tile_x8(const uint8_t* in, size_t in_stride, uint8_t* out, size_t out_stride)
{
    __m128i a[16];
    __m128i b[16];
    for (size_t r = 0; r < 16; ++r)
    {
        a[r] = load_row(in + r * in_stride);
    }

    __m128i* src = a;
    __m128i* dst = b;
    for (int round = 0; round < 4; ++round)
    {
        for (size_t k = 0; k < 8; ++k)
        {
            dst[2 * k]     = _mm_unpacklo_epi8(src[k], src[k + 8]);
            dst[2 * k + 1] = _mm_unpackhi_epi8(src[k], src[k + 8]);
        }
        std::swap(src, dst);
    }

    for (size_t c = 0; c < 16; ++c)
    {
        store_row(out + c * out_stride, src[c]);
    }
}

// Tiled transpose of a rows x cols matrix of T. Whole tiles go to the SIMD tile
// kernel, block by block; the right and bottom edges are copied element by element.
template<typename T, size_t TILE, size_t BLOCK>
void sse2_transpose(const T* input, T* output, size_t rows, size_t cols,
                    void (*tile_kernel)(const uint8_t*, size_t, uint8_t*, size_t))
{
    const size_t in_stride = cols * sizeof(T);
    const size_t out_stride = rows * sizeof(T);
    const size_t full_rows = rows / TILE * TILE;
    const size_t full_cols = cols / TILE * TILE;
    const auto* in = reinterpret_cast<const uint8_t*>(input);
    auto* out = reinterpret_cast<uint8_t*>(output);

    for (size_t r0 = 0; r0 < full_rows; r0 += BLOCK)
    {
        const size_t r_end = (std::min)(r0 + BLOCK, full_rows);  // (): <windows.h> min macro
        for (size_t c0 = 0; c0 < full_cols; c0 += BLOCK)
        {
            const size_t c_end = (std::min)(c0 + BLOCK, full_cols);
            for (size_t r = r0; r < r_end; r += TILE)
            {
                for (size_t c = c0; c < c_end; c += TILE)
                {
                    tile_kernel(in + r * in_stride + c * sizeof(T), in_stride,
                                out + c * out_stride + r * sizeof(T), out_stride);
                }
            }
        }
    }

    // Right edge: columns past the last whole tile, for the rows covered by tiles.
    for (size_t r = 0; r < full_rows; ++r)
    {
        for (size_t c = full_cols; c < cols; ++c)
        {
            output[c * rows + r] = input[r * cols + c];
        }
    }
    // Bottom edge: rows past the last whole tile, for every column.
    for (size_t r = full_rows; r < rows; ++r)
    {
        for (size_t c = 0; c < cols; ++c)
        {
            output[c * rows + r] = input[r * cols + c];
        }
    }
}

void sse2_transpose_dispatch(const uint8_t* input, uint8_t* output, size_t rows, size_t cols)
{
    sse2_transpose<uint8_t, SSE2_TILE_X8, SSE2_BLOCK_X8>(input, output, rows, cols, transpose_tile_x8);
}

void sse2_transpose_dispatch(const float* input, float* output, size_t rows, size_t cols)
{
    // Moves 32-bit words bit for bit, so NaN payloads and -0.0f are preserved.
    sse2_transpose<float, SSE2_TILE_X32, SSE2_BLOCK_X32>(input, output, rows, cols, transpose_tile_x32);
}

}  // namespace

template<typename T>
void xnnpack_transpose(const T* input, T* output, size_t rows, size_t cols)
{
    assert(input != NULL);
    assert(output != NULL);
    if (rows == 0 || cols == 0)
    {
        return;
    }
    sse2_transpose_dispatch(input, output, rows, cols);
}

template void xnnpack_transpose<uint8_t>(const uint8_t*, uint8_t*, size_t, size_t);
template void xnnpack_transpose<float>(const float*, float*, size_t, size_t);

#else // !USE_NEON && !USE_SSE2

// Scalar fallback transpose (x86_64 or other non-NEON architectures)
template<typename T>
void xnnpack_transpose(const T* input, T* output, size_t rows, size_t cols)
{
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            output[c * rows + r] = input[r * cols + c];
}

template void xnnpack_transpose<uint8_t>(const uint8_t*, uint8_t*, size_t, size_t);
template void xnnpack_transpose<float>(const float*, float*, size_t, size_t);

#endif // USE_NEON / USE_SSE2

// ============================================================================
// Quantize float32 → int8 kernels
// Formula: output[i] = clamp(round(input[i] * scale + bias), -128, 127)
// ============================================================================

#ifdef USE_NEON
static void neon_quantize_f32_to_s8(
    const float* input, int8_t* output, size_t count,
    float scale, float bias)
{
    const float32x4_t vscale = vdupq_n_f32(scale);
    const float32x4_t vbias = vdupq_n_f32(bias);

    size_t i = 0;

    // Main loop: 16 floats → 16 int8
    for (; i + 16 <= count; i += 16)
    {
        float32x4_t vin0 = vld1q_f32(input + i);
        float32x4_t vin1 = vld1q_f32(input + i + 4);
        float32x4_t vin2 = vld1q_f32(input + i + 8);
        float32x4_t vin3 = vld1q_f32(input + i + 12);

        vin0 = vaddq_f32(vmulq_f32(vin0, vscale), vbias);
        vin1 = vaddq_f32(vmulq_f32(vin1, vscale), vbias);
        vin2 = vaddq_f32(vmulq_f32(vin2, vscale), vbias);
        vin3 = vaddq_f32(vmulq_f32(vin3, vscale), vbias);

        // Round to nearest integer (banker's rounding)
        int32x4_t vacc0 = vcvtnq_s32_f32(vin0);
        int32x4_t vacc1 = vcvtnq_s32_f32(vin1);
        int32x4_t vacc2 = vcvtnq_s32_f32(vin2);
        int32x4_t vacc3 = vcvtnq_s32_f32(vin3);

        // Saturating narrow: s32 → s16
        int16x8_t vacc01 = vcombine_s16(vqmovn_s32(vacc0), vqmovn_s32(vacc1));
        int16x8_t vacc23 = vcombine_s16(vqmovn_s32(vacc2), vqmovn_s32(vacc3));

        // Saturating narrow: s16 → s8 (clamps to [-128, 127])
        int8x16_t vout = vcombine_s8(vqmovn_s16(vacc01), vqmovn_s16(vacc23));
        vst1q_s8(output + i, vout);
    }

    // Secondary loop: 8 floats → 8 int8
    for (; i + 8 <= count; i += 8)
    {
        float32x4_t vin0 = vld1q_f32(input + i);
        float32x4_t vin1 = vld1q_f32(input + i + 4);

        vin0 = vaddq_f32(vmulq_f32(vin0, vscale), vbias);
        vin1 = vaddq_f32(vmulq_f32(vin1, vscale), vbias);

        int32x4_t vacc0 = vcvtnq_s32_f32(vin0);
        int32x4_t vacc1 = vcvtnq_s32_f32(vin1);

        int16x8_t vacc01 = vcombine_s16(vqmovn_s32(vacc0), vqmovn_s32(vacc1));
        int8x8_t vout = vqmovn_s16(vacc01);
        vst1_s8(output + i, vout);
    }

    // Remainder: 1-7 floats (NEON with partial store to keep banker's rounding)
    if (i < count)
    {
        float32x4_t vx_lo = vld1q_f32(input + i);
        const float* x_hi = (const float*)((uintptr_t)(input + i) + ((count - i) & 4) * sizeof(float));
        float32x4_t vx_hi = vld1q_f32(x_hi);

        vx_lo = vaddq_f32(vmulq_f32(vx_lo, vscale), vbias);
        vx_hi = vaddq_f32(vmulq_f32(vx_hi, vscale), vbias);

        const int32x4_t vacc_lo = vcvtnq_s32_f32(vx_lo);
        const int32x4_t vacc_hi = vcvtnq_s32_f32(vx_hi);

        int16x8_t vacc = vcombine_s16(vqmovn_s32(vacc_lo), vqmovn_s32(vacc_hi));
        int8x8_t vy = vqmovn_s16(vacc);

        size_t remaining = count - i;
        if (remaining & 4)
        {
            vst1_lane_u32(reinterpret_cast<uint32_t*>(output + i), vreinterpret_u32_s8(vy), 0);
            i += 4;
            vy = vext_s8(vy, vy, 4);
        }
        if (remaining & 2)
        {
            vst1_lane_u16(reinterpret_cast<uint16_t*>(output + i), vreinterpret_u16_s8(vy), 0);
            i += 2;
            vy = vext_s8(vy, vy, 2);
        }
        if (remaining & 1)
        {
            vst1_lane_s8(reinterpret_cast<int8_t*>(output + i), vy, 0);
        }
    }
}
#endif // USE_NEON

#if defined(__SSE4_1__)
#include <smmintrin.h>
static void sse41_quantize_f32_to_s8(
    const float* input, int8_t* output, size_t count,
    float scale, float bias)
{
    const __m128 vscale = _mm_set1_ps(scale);
    const __m128 vbias = _mm_set1_ps(bias);
    const __m128 vclamp_max = _mm_set1_ps(32767.0f);
    const __m128 vclamp_min = _mm_set1_ps(-32768.0f);

    size_t i = 0;

    // Main loop: 16 floats → 16 int8
    for (; i + 16 <= count; i += 16)
    {
        __m128 vin0 = _mm_loadu_ps(input + i);
        __m128 vin1 = _mm_loadu_ps(input + i + 4);
        __m128 vin2 = _mm_loadu_ps(input + i + 8);
        __m128 vin3 = _mm_loadu_ps(input + i + 12);

        vin0 = _mm_add_ps(_mm_mul_ps(vin0, vscale), vbias);
        vin1 = _mm_add_ps(_mm_mul_ps(vin1, vscale), vbias);
        vin2 = _mm_add_ps(_mm_mul_ps(vin2, vscale), vbias);
        vin3 = _mm_add_ps(_mm_mul_ps(vin3, vscale), vbias);

        // Clamp to safe range before int conversion
        vin0 = _mm_max_ps(_mm_min_ps(vin0, vclamp_max), vclamp_min);
        vin1 = _mm_max_ps(_mm_min_ps(vin1, vclamp_max), vclamp_min);
        vin2 = _mm_max_ps(_mm_min_ps(vin2, vclamp_max), vclamp_min);
        vin3 = _mm_max_ps(_mm_min_ps(vin3, vclamp_max), vclamp_min);

        // Round to nearest integer (banker's rounding)
        __m128i vacc0 = _mm_cvtps_epi32(vin0);
        __m128i vacc1 = _mm_cvtps_epi32(vin1);
        __m128i vacc2 = _mm_cvtps_epi32(vin2);
        __m128i vacc3 = _mm_cvtps_epi32(vin3);

        // Pack s32 → s16 (saturating)
        __m128i vacc01 = _mm_packs_epi32(vacc0, vacc1);
        __m128i vacc23 = _mm_packs_epi32(vacc2, vacc3);

        // Pack s16 → s8 (saturating, clamps to [-128, 127])
        __m128i vout = _mm_packs_epi16(vacc01, vacc23);
        _mm_storeu_si128((__m128i*)(output + i), vout);
    }

    // Secondary loop: 8 floats → 8 int8
    for (; i + 8 <= count; i += 8)
    {
        __m128 vin0 = _mm_loadu_ps(input + i);
        __m128 vin1 = _mm_loadu_ps(input + i + 4);

        vin0 = _mm_add_ps(_mm_mul_ps(vin0, vscale), vbias);
        vin1 = _mm_add_ps(_mm_mul_ps(vin1, vscale), vbias);

        vin0 = _mm_max_ps(_mm_min_ps(vin0, vclamp_max), vclamp_min);
        vin1 = _mm_max_ps(_mm_min_ps(vin1, vclamp_max), vclamp_min);

        __m128i vacc0 = _mm_cvtps_epi32(vin0);
        __m128i vacc1 = _mm_cvtps_epi32(vin1);

        __m128i vacc01 = _mm_packs_epi32(vacc0, vacc1);
        __m128i vout = _mm_packs_epi16(vacc01, vacc01);
        _mm_storel_epi64((__m128i*)(output + i), vout);
    }

    // Remainder: 1-7 floats (SSE4.1 with partial store to keep banker's rounding)
    if (i < count)
    {
        __m128 vx_lo = _mm_loadu_ps(input + i);
        const float* x_hi = (const float*)((uintptr_t)(input + i) + ((count - i) & 4) * sizeof(float));
        __m128 vx_hi = _mm_loadu_ps(x_hi);

        vx_lo = _mm_add_ps(_mm_mul_ps(vx_lo, vscale), vbias);
        vx_hi = _mm_add_ps(_mm_mul_ps(vx_hi, vscale), vbias);

        vx_lo = _mm_max_ps(_mm_min_ps(vx_lo, vclamp_max), vclamp_min);
        vx_hi = _mm_max_ps(_mm_min_ps(vx_hi, vclamp_max), vclamp_min);

        const __m128i vy_lo = _mm_cvtps_epi32(vx_lo);
        const __m128i vy_hi = _mm_cvtps_epi32(vx_hi);

        __m128i vy = _mm_packs_epi32(vy_lo, vy_hi);
        vy = _mm_packs_epi16(vy, vy);

        size_t remaining = count - i;
        if (remaining & 4)
        {
            memcpy(output + i, &vy, 4);
            i += 4;
            vy = _mm_srli_epi64(vy, 32);
        }
        if (remaining & 2)
        {
            uint16_t tmp = (uint16_t)_mm_extract_epi16(vy, 0);
            memcpy(output + i, &tmp, 2);
            i += 2;
            vy = _mm_srli_epi32(vy, 16);
        }
        if (remaining & 1)
        {
            output[i] = (int8_t)_mm_extract_epi8(vy, 0);
        }
    }
}
#endif // __SSE4_1__

#if !defined(USE_NEON) && !defined(__SSE4_1__)
static void scalar_quantize_f32_to_s8(
    const float* input, int8_t* output, size_t count,
    float scale, float bias)
{
    for (size_t i = 0; i < count; i++)
    {
        float val = input[i] * scale + bias;
        int32_t ival = static_cast<int32_t>(std::nearbyintf(val));
        ival = ival < -128 ? -128 : (ival > 127 ? 127 : ival);
        output[i] = static_cast<int8_t>(ival);
    }
}
#endif

// Public API: dispatches to architecture-specific kernel
void xnnpack_quantize_f32_to_s8(
    const float* input,
    int8_t* output,
    size_t count,
    float scale,
    float bias)
{
    if (count == 0) return;
#ifdef USE_NEON
    neon_quantize_f32_to_s8(input, output, count, scale, bias);
#elif defined(__SSE4_1__)
    sse41_quantize_f32_to_s8(input, output, count, scale, bias);
#else
    scalar_quantize_f32_to_s8(input, output, count, scale, bias);
#endif
}
