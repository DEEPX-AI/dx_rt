# NFH / CPU Task Acceleration - AI Assistant Guide

> ⚠️ **프로젝트 전체 규칙 및 Code Convention은 [general.md](general.md)를 참조하세요.**

---

## Sub-Project Overview

**목적**: NPU 추론 파이프라인에서 병목인 NFH(NPU Format Handler)와 CPU Task를 SIMD/가속 라이브러리로 최적화

**브랜치**: `accel_cpu_task`

**배경**: NPU 추론 파이프라인에서 호스트 CPU가 담당하는 부분은 크게 데이터 변환(NFH transpose)과 CPU Task(ONNX Runtime 연산)가 있음. 전체 추론 시간 중 NPU 연산 시간이 지배적인 모델에서는 CPU 측을 아무리 최적화해도 효과가 미미하지만, CPU 처리 비중이 큰 모델에서는 이 부분이 throughput 병목이 됨. 이러한 모델들의 성능 개선을 위해 아키텍처별 최적화 라이브러리(IPP, NEON, OpenVINO, XNNPACK)를 활용한 가속을 도입함.

---

## 1. 아키텍처 개요

### 1.1 2-Level Gating (컴파일 타임 + 런타임)

가속 기능은 **2단계 게이팅** 구조로 제어됨:

```
Level 1: CMake 컴파일 타임 (환경 호환성 자동 감지)
  USE_NPU_FORMAT_CONVERSION_ACCELERATION → USE_IPP (x86) / USE_NEON (arm)
  USE_CPU_OP_ACCELERATION                → USE_OPENVINO (x86) / USE_XNNPACK (arm)

Level 2: 런타임 (FORCE 옵션 또는 Configuration API)
  FORCE_*=ON  → Configuration 무시, 항상 활성화
  FORCE_*=OFF → Configuration::GetEnable()으로 제어 (기본값 false)
```

**동작 흐름:**

```
CMake 빌드 시
  ├── USE_NPU_FORMAT_CONVERSION_ACCELERATION=ON?
  │   ├── x86_64: IPP 설치 여부 확인 → #define USE_IPP
  │   ├── aarch64: NEON 필수 탑재 → #define USE_NEON
  │   └── 미지원 아키텍처: 자동 OFF
  │
  ├── USE_CPU_OP_ACCELERATION=ON?
  │   ├── glibc 버전 확인 (x86 ≥ 2.35, arm ≥ 2.31)
  │   ├── x86_64: OpenVINO 설치 여부 → #define USE_OPENVINO
  │   ├── aarch64: NEON 기반 XNNPACK → #define USE_XNNPACK
  │   └── 미지원 아키텍처: 자동 OFF
  │
  └── FORCE_*=ON?
      └── #define FORCE_NFH_ACCELERATION / FORCE_CPU_TASK_ACCELERATION

컴파일 타임 매크로 (configuration.h에서 자동 정의)
  ├── #if USE_IPP || USE_NEON → #define DXRT_NFH_ACCELERATION_AVAILABLE
  └── #if USE_OPENVINO || USE_XNNPACK → #define DXRT_CPU_OP_ACCELERATION_AVAILABLE
  → 이 매크로가 정의되지 않으면 관련 ITEM 열거값, CLI 옵션, atomic 변수가 컴파일에서 제외됨

런타임 실행 시
  ├── #ifdef FORCE_*_ACCELERATION → 무조건 가속 사용
  └── #else → Configuration::IsNfhAccelerationEnabled() / IsCpuOpAccelerationEnabled()
             (SetEnable() 호출 시 atomic write, hot path에서 lock-free read)
```

### 1.2 플랫폼별 기술 스택

| 가속 대상 | x86_64 | aarch64 | Windows |
|-----------|--------|---------|---------|
| **NFH** (Transpose) | Intel IPP (`ippiTranspose`) | ARM NEON (XNNPACK 커널) | ❌ 미지원 |
| **CPU Task** (ORT EP) | OpenVINO EP | XNNPACK EP | ❌ 미지원 |

---

## 2. NFH Acceleration (Transpose 가속)

### 2.1 무엇을 가속하는가

NFH는 NPU와 호스트 간 데이터 전달 시 **행렬 전치(transpose)** 를 수행함. `bidirectional_transpose()` 함수가 핵심이며, 모델의 입출력마다 호출되므로 throughput에 직접 영향을 줌.

### 2.2 지원 조건

| 조건 | 값 |
|------|----|
| `element_size` | **1 byte** (UINT8) 또는 **4 bytes** (FLOAT32) 만 지원 |
| In-place (`src == dst`) | 지원됨 — 임시 버퍼에 복사 후 가속 transpose 실행 |

`element_size`가 1 또는 4가 아니거나, 가속이 비활성화된 경우 naive 구현으로 fallback.

### 2.3 x86_64: Intel IPP

- `ippiTranspose_8u_C1R` (UINT8), `ippiTranspose_32f_C1R` (FLOAT32)
- IPP 내부적으로 AVX2/AVX-512 자동 활용
- IPP 라이브러리 정적 링크 (`.a`)
- IPP 설치 필요: `apt install intel-oneapi-ipp-devel` 또는 `IPPROOT` 환경변수 설정

### 2.4 aarch64: NEON (XNNPACK 커널)

**Macro Kernel + Micro Kernel 아키텍처**를 적용하여 캐시 효율을 극대화:

- **Micro Kernel** (SIMD transpose 실행 단위):
  - `xnn_x8_transposec_ukernel__16x16_reuse_mov_zip_neon` (UINT8, 16×16 타일)
  - `xnn_x32_transposec_ukernel__4x4_aarch64_neon_tbl128` (FLOAT32, 4×4 타일)
- **Macro Kernel** (`xnnpack_transpose_macro`): L1D 캐시 크기(32KB 가정)에 맞춰 행렬을 블록 단위로 분할한 뒤 각 블록을 micro kernel에 디스패치
  - UINT8: 128×128 블록 (16KB × 2 = 32KB, micro tile 16 배수)
  - FLOAT32: 64×64 블록 (16KB × 2 = 32KB, micro tile 4 배수)
  - Threshold: `total_bytes > 32KB` → macro kernel, 이하 → micro kernel 직접 호출
- XNNPACK 프로젝트에서 가져온 micro 커널 코드 (`lib/xnn_kernel.cpp`)
- ARMv8 ASIMD(NEON) 필수 탑재이므로 별도 설치 불필요

**설계 의도**: micro 커널이 대규모 행렬을 한 번에 처리하면 strided read/write가 L1/L2 캐시를 초과하여 cache thrashing이 발생함. macro 커널이 source+destination 블록을 L1D 내에 유지시켜 이를 방지함. 작은 행렬은 tiling 오버헤드 없이 micro 커널을 바로 호출.

### 2.5 코드 흐름

```cpp
// lib/npu_format_handler.cpp — bidirectional_transpose()
bool nfh_accel_enabled = false;
#ifdef FORCE_NFH_ACCELERATION
    nfh_accel_enabled = true;                        // FORCE: Configuration 무시
#else
    nfh_accel_enabled = Configuration::IsNfhAccelerationEnabled();
#endif

if (nfh_accel_enabled && (element_size == 1 || element_size == 4))
{
#ifdef USE_IPP
    ipp_bidirectional_transpose(src, dst, row, col, element_size);  // x86
#elif defined(USE_NEON)
    neon_bidirectional_transpose(src, dst, row, col, element_size); // arm
#endif
}
else
{
    // fallback: naive byte-by-byte transpose
}
```

```cpp
// lib/xnn_kernel.cpp — xnnpack_transpose<T>() (aarch64 내부 흐름)
xnnpack_transpose<T>(input, output, rows, cols)
  ├─ trivial case (0, 1x1, 1xN/Nx1) → 직접 copy/return
  ├─ total_bytes > 32KB → xnnpack_transpose_macro<T>()
  │   └─ 블록 단위 loop → micro kernel 호출 per sub-block
  └─ total_bytes ≤ 32KB → micro kernel 직접 호출 (tiling 오버헤드 없음)
```

---

## 3. CPU Task Acceleration (ONNX Runtime EP 가속)

### 3.1 무엇을 가속하는가

CPU Task는 ONNX Runtime으로 실행되는 CPU 연산 (모델 그래프 중 NPU에서 실행 불가한 부분). 기본 ORT CPU EP 대신 최적화된 Execution Provider를 사용.

### 3.2 성능 특성

- 성능 개선 정도는 **모델 연산 그래프, operation 종류, 호스트 환경에 따라 천차만별**
- 가속 라이브러리는 주로 **arithmetic operation** (Conv, MatMul 등)을 지원하며, **memory operation** (Reshape, Transpose, Concat 등)은 잘 지원하지 않는 경우가 많음
- 그런데 CPU Task에서 실행 시간이 긴 연산은 오히려 memory operation인 경우가 많아, 폭넓은 성능 향상을 위해서는 **SIMD/GPU로 memory operation 가속이 추가로 필요함**
- arm과 x86의 **지원 operation 범위가 다름** (arm이 더 좁음) → 같은 모델이라도 플랫폼에 따라 가속 효과가 있을 수도, 없을 수도 있음

### 3.3 x86_64: OpenVINO EP

- OpenVINO Runtime을 ORT의 Execution Provider로 사용
- `_sessionOptions.AppendExecutionProvider_OpenVINO_V2(options)`
- glibc ≥ 2.35 필요 (Ubuntu 22.04+)

### 3.4 aarch64: XNNPACK EP

- XNNPACK을 ORT의 Execution Provider로 사용
- `_sessionOptions.AppendExecutionProvider("XNNPACK")`
- glibc ≥ 2.31 필요 (Ubuntu 20.04+ / Debian 11+)
- ORT 라이브러리를 XNNPACK EP 포함하여 빌드한 버전 사용

### 3.5 코드 흐름

```cpp
// lib/cpu_handle.cpp — CpuHandle 생성자
bool enable_cpu_acceleration = false;
#ifdef FORCE_CPU_TASK_ACCELERATION
    enable_cpu_acceleration = true;                          // FORCE: Configuration 무시
#else
    enable_cpu_acceleration = Configuration::IsCpuOpAccelerationEnabled();
#endif

if (enable_cpu_acceleration)
{
#ifdef USE_OPENVINO
    options["device_type"] = "CPU";
    _sessionOptions.AppendExecutionProvider_OpenVINO_V2(options);  // x86
#elif defined(USE_XNNPACK)
    _sessionOptions.AppendExecutionProvider("XNNPACK");           // arm
#endif
}
```

---

## 4. VNPU 특이사항

VNPU(Virtual NPU, RK3588 기반)는 다른 환경과 다른 특성을 가짐:

- RK3588 내장 **Mali GPU** 가 사용 가능하므로, GPU를 활용한 가속이 가능
- 일반 환경에서는 범용 GPU 지원이 없어 CPU SIMD(NEON)만 활용
- VNPU 환경에서의 NFH 입력 스킵 로직 등 별도 처리가 존재 (`chore: exclude input nfh skipping logic from VNPU`)

---

## 5. CMake 옵션 레퍼런스

### 5.1 빌드 옵션

| 옵션 | 기본값 | 설명 |
|------|--------|------|
| `USE_NPU_FORMAT_CONVERSION_ACCELERATION` | `OFF` | NFH transpose 가속 코드 빌드 포함 여부. 환경 미지원 시 자동 OFF |
| `USE_CPU_OP_ACCELERATION` | `OFF` | CPU Task EP 가속 코드 빌드 포함 여부. 환경 미지원 시 자동 OFF |
| `FORCE_NPU_FORMAT_CONVERSION_ACCELERATION` | `OFF` | ON이면 런타임에 Configuration 무시, 항상 NFH 가속 사용 |
| `FORCE_CPU_OP_ACCELERATION` | `OFF` | ON이면 런타임에 Configuration 무시, 항상 CPU Task 가속 사용 |

### 5.2 자동 생성되는 Compile Definitions

| Definition | 조건 | 사용처 |
|------------|------|--------|
| `USE_IPP` | x86_64 + IPP 설치됨 | `npu_format_handler.cpp` |
| `USE_NEON` | aarch64 | `npu_format_handler.cpp` |
| `USE_OPENVINO` | x86_64 + OpenVINO 설치됨 | `cpu_handle.cpp` |
| `USE_XNNPACK` | aarch64 + NEON 감지됨 | `cpu_handle.cpp` |
| `FORCE_NFH_ACCELERATION` | `FORCE_NPU_FORMAT_CONVERSION_ACCELERATION=ON` | `npu_format_handler.cpp` |
| `FORCE_CPU_TASK_ACCELERATION` | `FORCE_CPU_OP_ACCELERATION=ON` | `cpu_handle.cpp` |
| `DXRT_NFH_ACCELERATION_AVAILABLE` | `USE_IPP` 또는 `USE_NEON` 정의됨 | `configuration.h` — ITEM 열거값, atomic 변수, accessor 함수의 `#ifdef` 가드 |
| `DXRT_CPU_OP_ACCELERATION_AVAILABLE` | `USE_OPENVINO` 또는 `USE_XNNPACK` 정의됨 | `configuration.h` — ITEM 열거값, atomic 변수, accessor 함수의 `#ifdef` 가드 |

> **Note**: `DXRT_*_AVAILABLE` 매크로는 `configuration.h` 상단에서 자동 정의되며, 가속 관련 코드의 조건부 컴파일에 사용됩니다. OFF 빌드에서는 이 매크로가 정의되지 않아 관련 ITEM 열거값, CLI 옵션, atomic 변수, 단위 테스트가 모두 컴파일에서 제외됩니다.

### 5.3 런타임 제어: Configuration API

환경변수 대신 `Configuration` 싱글톤을 통해 런타임에 가속 on/off를 제어함.

| Configuration ITEM | 기본값 | 설명 |
|--------------------|--------|------|
| `NFH_ACCELERATION` | `false` | NFH transpose 가속 활성화 (`FORCE_NFH_ACCELERATION` 미정의 시 참조) |
| `CPU_OP_ACCELERATION` | `false` | CPU Task EP 가속 활성화 (`FORCE_CPU_TASK_ACCELERATION` 미정의 시 참조) |

**성능 고려사항**: Hot path에서 mutex lock을 피하기 위해 `std::atomic<bool>` 플래그를 사용.
`SetEnable()` 호출 시 atomic write, hot path에서 public static inline accessor로 lock-free read.

**캡슐화**: `_sNfhAcceleration`, `_sCpuOpAcceleration` atomic 변수는 **private**이며, `#ifdef` 가드 내부에만 존재.
외부에서는 반드시 아래 public accessor를 사용해야 함:

| Accessor | 기능 | 기능 미컴파일 시 |
|----------|------|------------------|
| `IsNfhAccelerationEnabled()` | `_sNfhAcceleration.load(relaxed)` | `return false` |
| `SetNfhAccelerationFlag(bool)` | `_sNfhAcceleration.store(v, relaxed)` | no-op |
| `IsCpuOpAccelerationEnabled()` | `_sCpuOpAcceleration.load(relaxed)` | `return false` |
| `SetCpuOpAccelerationFlag(bool)` | `_sCpuOpAcceleration.store(v, relaxed)` | no-op |

```cpp
// C++ 사용 예시
auto& config = Configuration::GetInstance();
config.SetEnable(Configuration::ITEM::NFH_ACCELERATION, true);
config.SetEnable(Configuration::ITEM::CPU_OP_ACCELERATION, true);
```

```python
# Python 사용 예시
config = Configuration()
config.set_enable(Configuration.ITEM.NFH_ACCELERATION, True)
config.set_enable(Configuration.ITEM.CPU_OP_ACCELERATION, True)
```

> **Note**: `ITEM::NFH_ACCELERATION`, `ITEM::CPU_OP_ACCELERATION` 열거값은 해당 기능이 빌드에 포함된 경우에만 존재합니다. OFF 빌드에서는 이 값들이 컴파일에서 제외됩니다.

### 5.4 의존성 자동 감지

| 의존성 | 감지 방법 | 미설치 시 동작 |
|--------|-----------|----------------|
| Intel IPP | `IPPROOT` 환경변수 또는 `/opt/intel/oneapi/ipp/latest` | `USE_NPU_FORMAT_CONVERSION_ACCELERATION` 자동 OFF |
| OpenVINO | `find_package(OpenVINO)` | `USE_CPU_OP_ACCELERATION` 자동 OFF |
| glibc 버전 | `ldd --version` 파싱 | 버전 미달 시 자동 OFF |
| ARM NEON | ARMv8에서 필수 탑재 | 항상 감지됨 |

---

## 6. 주요 파일 맵

| 파일 | 역할 |
|------|------|
| `cmake/dxrt.cfg.cmake` | 모든 가속 관련 CMake 옵션 정의 및 환경 감지 |
| `lib/CMakeLists.txt` | 가속 라이브러리 링크 및 compile definition 설정 |
| `lib/include/dxrt/configuration.h` | `DXRT_*_AVAILABLE` 매크로 정의, ITEM 열거값 `#ifdef` 가드, private atomic 변수, public inline accessor 함수 |
| `lib/configuration.cpp` | Configuration 구현 — SetEnable/LoadConfigFile 내 가속 블록 `#ifdef` 가드 |
| `lib/npu_format_handler.cpp` | NFH transpose 가속 분기 (IPP/NEON/fallback) |
| `lib/npu_format_handler.h` | `ipp_bidirectional_transpose`, `neon_bidirectional_transpose` 선언 |
| `lib/xnn_kernel.cpp` | NEON transpose macro+micro 커널 (XNNPACK 유래). Macro kernel: 캐시 타일링(128×128/64×64). Micro kernel: uint8 16×16 vzipq / float32 4×4 vqtbl4q |
| `lib/include/dxrt/xnn_kernel.h` | NEON transpose 커널 함수 선언 |
| `lib/cpu_handle.cpp` | ORT Execution Provider 선택 (OpenVINO/XNNPACK/기본) |
| `cli/run_model.cpp` | C++ CLI — `--accel-nfh`/`--accel-cpu` 옵션 `#ifdef` 가드 |
| `python_package/src/dx_engine/capi/py_inference_engine.cpp` | pybind 모듈에 `_NFH_ACCEL_AVAILABLE`/`_CPU_ACCEL_AVAILABLE` bool 속성 노출 |
| `python_package/src/dx_engine/configuration.py` | Python ITEM enum 동적 생성 (가속 항목 조건부 포함) |
| `python_package/cli/run_model.py` | Python CLI — `--accel-*` 옵션 조건부 등록 |
| `install.sh` | IPP, OpenVINO 등 의존 패키지 자동 설치 |
| `build.sh` | 빌드 스크립트 (가속 옵션 전달) |

---

## 7. 알려진 제한사항

| 제한사항 | 설명 |
|----------|------|
| **Windows 미지원** | 현재 NFH/CPU Task 가속 모두 Linux 전용 |
| **NFH: element_size 제한** | 1 byte (UINT8) 또는 4 bytes (FLOAT32)만 가속. 2 bytes (FLOAT16/INT16 등) 미지원 시 fallback |
| **CPU Task: operation 지원 범위** | arithmetic op 위주 지원, memory op (Reshape, Transpose, Concat 등) 미지원이 많음 |
| **CPU Task: arm 지원 범위** | x86보다 좁음. 같은 모델도 arm에서는 가속 효과 없을 수 있음 |
| **glibc 요구사항** | x86_64 ≥ 2.35, aarch64 ≥ 2.31. 미달 시 컴파일 타임에 자동 비활성화 |
| **범용 GPU 미지원** | VNPU(RK3588)의 Mali GPU 외에는 GPU 가속 경로 없음 |

---

## 8. 향후 계획

- [ ] **Windows 지원**: IPP/OpenVINO는 Windows에서도 사용 가능하므로, 빌드 시스템 및 런타임 분기 확장
- [ ] **CPU Operation 가속 범위 확장**: memory operation (Transpose, Concat, Reshape 등)에 대한 SIMD/GPU 가속 추가. 현재 arithmetic op 위주의 라이브러리 지원만으로는 폭넓은 성능 향상이 어려움
- [ ] **NFH element_size 확장**: 2 bytes (FLOAT16, INT16) transpose 지원 추가
- [ ] **GPU 활용 확대**: VNPU 외 환경에서도 GPU가 사용 가능한 경우 활용 방안 검토

---

## 9. 브랜치 히스토리 요약

`accel_cpu_task` 브랜치의 주요 커밋 흐름 (시간순):

1. `c2516f3e` — CPU Task 가속 기능 추가 (OpenVINO/XNNPACK EP)
2. `4b966b9f` — install/build 스크립트에 의존 패키지 자동 설치 추가
3. `4bee8c35` — output_tensor 데이터를 encoded_output 버퍼로 변경
4. `20e5111f` — 성능 설정 추가 (CPU governor 등)
5. `57d3a7bc` — Configuration 수정 및 가이드 문서 추가
6. `e831a224` — VNPU 코드 npu_format_handler 처리
7. `e95ec374` — glibc 버전 체크 로직 수정
8. `e4035c68` — C++11 호환성 확보
9. `41e2de8c` — VNPU에서 input NFH 스킵 로직 분리
10. `a7d7827c` — FORCE 영구 가속 설정 옵션 추가
