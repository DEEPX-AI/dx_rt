// XNNPACK NEON Transpose Kernels
// Supports uint8_t (16x16 vzipq) and float (4x4 vqtbl4q)
//
// Based on XNNPACK (https://github.com/google/XNNPACK)
// License: BSD 3-Clause

#include "dxrt/xnn_kernel.h"
#include <arm_neon.h>
#include <assert.h>
#include <stddef.h>

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
        // The uint8 16x16 micro kernel cannot handle block_height < 16
        // (partial tile not implemented). Split into full tiles + scalar remainder.
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
      else 
      {
        xnn_x32_transposec_ukernel__4x4_aarch64_neon_tbl128(
            (const uint32_t*)src_block, (uint32_t*)dst_block,
            input_stride, output_stride, bw, bh);
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
      // The uint8 16x16 micro kernel cannot handle block_height < 16
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
    else 
    {
      xnn_x32_transposec_ukernel__4x4_aarch64_neon_tbl128(
        (const uint32_t*)input, (uint32_t*)output,
        input_stride, output_stride, cols, rows);
    }
  }
}

// Explicit instantiations
template void xnnpack_transpose<uint8_t>(const uint8_t*, uint8_t*, size_t, size_t);
template void xnnpack_transpose<float>(const float*, float*, size_t, size_t);
