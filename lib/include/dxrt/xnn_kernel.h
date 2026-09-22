#ifndef XNN_KERNEL_H
#define XNN_KERNEL_H

#include <stddef.h>
#include <stdint.h>

template<typename T>
void xnnpack_transpose(
    const T* input,
    T* output,
    size_t rows,
    size_t cols);

// Explicit instantiations
extern template void xnnpack_transpose<uint8_t>(const uint8_t*, uint8_t*, size_t, size_t);
extern template void xnnpack_transpose<float>(const float*, float*, size_t, size_t);

// Quantize float32 to int8:
//   output[i] = clamp(round(input[i] * scale + bias), -128, 127)
// Supports in-place: output can alias (int8_t*)input since sizeof(float) > sizeof(int8_t)
void xnnpack_quantize_f32_to_s8(
    const float* input,
    int8_t* output,
    size_t count,
    float scale,
    float bias);

#endif // XNN_KERNEL_H
