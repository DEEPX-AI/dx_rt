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

#endif // XNN_KERNEL_H
