# Yocto Build Integration - AI Assistant Guide

> ⚠️ **프로젝트 전체 규칙 및 Code Convention은 [general.md](general.md)를 참조하세요.**

---

## Sub-Project Overview

**목적**: DX_RT를 Yocto 빌드 시스템에서 빌드할 수 있도록 patch 기반 통합 지원

**위치**: Yocto layer 내 `dx_rt` recipe

**관련 파일**:
- `/home/orangepi/cpu_accel.patch` - dx_rt_main에 CPU 가속 기능 추가하는 patch
- `lib/data/ppcpu_data.cpp` - ppcpu_bin 심볼이 정의된 C 배열 파일

---

## Current Task

- [x] ppcpu_bin reference error 원인 분석 및 해결
- [x] NEON 크로스 컴파일 미활성화 문제 해결
- [x] cpu_accel.patch 업데이트
- [ ] **Yocto 환경에서 빌드 테스트** (x86 호스트 → ARM64 타겟)

---

## 해결된 문제들

### Problem 1: ppcpu_bin undefined reference

**증상**:
```
undefined reference to `ppcpu_bin`
undefined reference to `ppcpu_bin_len`
```

**원인**: `lib/data/CMakeLists.txt`가 `ppcpu.bin` 존재 여부만 확인하고, 이미 생성된 `ppcpu_data.cpp`는 확인하지 않음

**해결**: `lib/data/CMakeLists.txt` 수정 - 기존 `ppcpu_data.cpp` 우선 사용
```cmake
# 수정 후: ppcpu_data.cpp가 이미 존재하면 그대로 사용
if(EXISTS ${PPCPU_GEN_CPP})
    message(STATUS "Using existing ppcpu_data.cpp")
    set(PPCPU_DATA_CPP ${PPCPU_GEN_CPP})
elseif(EXISTS ${PPCPU_BIN})
    # ppcpu.bin에서 생성...
endif()
```

---

### Problem 2: NEON 크로스 컴파일 미활성화

**증상**: 
- Yocto 빌드(x86 호스트)에서 USE_NEON이 비활성화됨
- `neon_bidirectional_transpose` 함수가 일반 버전으로 컴파일됨

**원인**: `cmake/dxrt.cfg.cmake`에서 `/proc/cpuinfo`를 읽어 NEON 지원 확인
```cmake
# 기존 코드 (문제)
file(READ "/proc/cpuinfo" PROC_CPUINFO)
if(PROC_CPUINFO MATCHES "Features.*neon|Features.*asimd")
    set(ARM_NEON_FOUND TRUE)
endif()
```
→ 크로스 컴파일 시 호스트(x86) CPU 정보를 읽어서 NEON 미감지

**해결**: `/proc/cpuinfo` 체크 제거 - ARMv8(aarch64)은 NEON/ASIMD가 필수
```cmake
# 수정 후: ARMv8은 무조건 NEON 지원
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
    # NEON/ASIMD is MANDATORY on ARMv8 - no runtime check needed
    set(ARM_NEON_FOUND TRUE)
    message(STATUS "ARM NEON/ASIMD enabled (mandatory on ARMv8/aarch64)")
endif()
```

---

## 테스트 가이드

### ⚠️ 중요: 실제 크로스 컴파일 환경에서 테스트 필요

현재 Orange Pi (ARM)에서의 테스트는 **네이티브 빌드**이므로, 실제 크로스 컴파일 환경을 검증하지 못함.

**반드시 x86 호스트에서 Yocto 빌드 테스트 수행!**

### Yocto 빌드 테스트 절차

```bash
# 1. Yocto 환경에서 패치 적용 확인
bitbake dx-rt -c configure

# 2. 빌드 로그에서 NEON 활성화 확인
# 다음 메시지가 있어야 함:
# "ARM NEON/ASIMD enabled (mandatory on ARMv8/aarch64)"

# 3. 전체 빌드
bitbake dx-rt

# 4. 빌드 후 심볼 확인
nm -D libdxrt.so | grep neon_bidirectional_transpose
# 결과가 있어야 함 (NEON 버전 사용됨)

nm -D libdxrt.so | grep ppcpu_bin
# 결과가 있어야 함 (ppcpu 심볼 정의됨)
```

### 시뮬레이션 테스트 (패치 적용 확인용)

```bash
# GitHub main clone 후 패치 적용 테스트
git clone https://github.com/aspect-aeon/dx_rt_main.git yocto_sim
cd yocto_sim
patch -p1 < /home/orangepi/cpu_accel.patch

# 패치 적용 확인
grep -A5 'aarch64|arm64' cmake/dxrt.cfg.cmake
# /proc/cpuinfo 체크가 없어야 함
```

---

## Environment

| 항목 | 값 |
|------|---|
| Host OS | x86_64 Linux (Yocto 빌드 서버) |
| Target | aarch64 (ARM64) |
| Build Type | Patch-based integration |
| Base Project | dx_rt_main (GitHub) |
| Patch File | `/home/orangepi/cpu_accel.patch` |

---

## cpu_accel.patch 수정 워크플로우

### 패치 수정 → 테스트 → 적용 사이클

```
┌─────────────────────────────────────────────────────────────┐
│  1. dx_rt 소스 수정                                          │
│     (/home/orangepi/dx_rt 에서 수정)                         │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│  2. cpu_accel.patch 업데이트                                 │
│     - 수정된 파일의 diff 생성                                 │
│     - 패치 파일의 해당 섹션 교체                              │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│  3. 패치 적용 테스트 (시뮬레이션)                             │
│     git clone dx_rt_main → patch -p1 < cpu_accel.patch      │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│  4. 빌드 테스트                                              │
│     cmake .. && make                                         │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│  5. Yocto 빌드 테스트 (실제 크로스 컴파일)                    │
│     bitbake dx-rt                                            │
└─────────────────────────────────────────────────────────────┘
```

### diff 생성 명령어 예시

```bash
# 특정 파일의 diff 생성 (dx_rt_main과 비교)
diff -u /home/orangepi/dx_rt_main/cmake/dxrt.cfg.cmake \
        /home/orangepi/dx_rt/cmake/dxrt.cfg.cmake

# 결과를 cpu_accel.patch의 해당 섹션에 교체
```

---

## Files to Track

| 파일 | 역할 | 상태 |
|------|------|------|
| `lib/data/ppcpu_data.cpp` | ppcpu_bin/ppcpu_bin_len 정의 | ✅ patch에 포함 |
| `lib/data/ppcpu.cpp` | ppcpu 관련 함수 | ✅ patch에 포함 |
| `lib/data/ppcpu.h` | 헤더 파일 | ✅ patch에 포함 |
| `lib/data/CMakeLists.txt` | 빌드 로직 (ppcpu_data.cpp 우선 사용) | ✅ patch에 포함 |
| `cmake/dxrt.cfg.cmake` | NEON 감지 로직 (크로스 컴파일 호환) | ✅ patch에 포함 |
| `lib/npu_format_handler.cpp` | neon_bidirectional_transpose 함수 | ✅ patch에 포함 |

---

## Sub-Project Specific Notes

### 주의사항

1. **크로스 컴파일 환경**
   - `/proc/cpuinfo`는 호스트 CPU 정보를 반환
   - `CMAKE_SYSTEM_PROCESSOR`가 타겟 아키텍처를 나타냄
   - 런타임 체크 대신 컴파일 타임 체크 사용

2. **ARMv8 NEON 지원**
   - ARMv8(aarch64)에서 NEON/ASIMD는 **필수 기능**
   - 런타임 감지 불필요, 무조건 활성화

3. **패치 적용 순서**
   - 새 파일 추가는 `diff --git a/... b/...` 형식
   - 기존 파일 수정은 `--- a/... +++ b/...` 형식
   - 바이너리 파일은 패치에 포함 어려움 (cpp 파일로 대체)

### Dependencies

- CMake 3.14+
- Cross-compiler toolchain (aarch64-linux-gnu-gcc)
- Yocto SDK (poky)

---

## References

- [general.md](general.md) - 프로젝트 전체 규칙
- `dx_rt/lib/data/` - ppcpu 관련 소스 파일
- `dx_rt/cmake/dxrt.cfg.cmake` - SIMD 감지 로직
- `/home/orangepi/cpu_accel.patch` - 현재 patch 파일
- `/home/orangepi/dx_rt_main` - GitHub main (비교용)