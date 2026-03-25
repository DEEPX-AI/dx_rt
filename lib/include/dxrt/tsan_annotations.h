/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#ifndef DXRT_TSAN_ANNOTATIONS_H_
#define DXRT_TSAN_ANNOTATIONS_H_

// TSAN annotation header (only available with sanitizer builds)
// When compiled with -fsanitize=thread, provides explicit synchronization
// annotations. When not using TSAN, these macros become no-ops.
#ifdef __SANITIZER_THREAD_SAFETY_ANALYSIS_H_GUARD__
#include <sanitizer/thread_safety_analysis.h>
#else
// Fallback: define empty macros if header not available (non-TSAN builds)
#define ANNOTATE_HAPPENS_BEFORE(x) (void)(x)
#define ANNOTATE_HAPPENS_AFTER(x) (void)(x)
#define ANNOTATE_BENIGN_RACE(x, desc) (void)(x)
#endif

// Additional macros for stronger synchronization signaling
extern "C" {
    // These are TSAN runtime functions that help it understand synchronization
    void __tsan_release(void*);
    void __tsan_acquire(void*);
}

#endif  // DXRT_TSAN_ANNOTATIONS_H_
