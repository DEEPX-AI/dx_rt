# Bitmatch - AI Assistant Guide

> ⚠️ **프로젝트 전체 규칙 및 Code Convention은 [general.md](general.md)를 참조하세요.**

---

## Sub-Project Overview

**목적**: NPU 추론 결과를 Ground Truth(GT)와 bit 단위로 비교하여, 추론 파이프라인의 **어느 단계에서** 오류가 발생했는지 특정하는 테스트 도구.

**위치**: `dx_rt/python_package/bitmatch/` (CLI 엔트리포인트), `dx_rt/python_package/src/dx_engine/bitmatch/` (핵심 로직)

**관련 파일**:

| 파일 | 역할 |
|------|------|
| `python_package/bitmatch/bitmatch.py` | CLI 엔트리포인트 (argparse → `run_bitmatch_test` 호출) |
| `python_package/src/dx_engine/bitmatch/core.py` | 메인 실행 루프 (모델 탐색, TestConfig 구성, 모델별 테스트 실행) |
| `python_package/src/dx_engine/bitmatch/module/tester.py` | 핵심 테스트 로직 (추론 실행, GT 로딩, bitmatch 비교, 결과 집계) |
| `python_package/src/dx_engine/bitmatch/module/debug_analyzer.py` | `--debug 2` 모드에서 중간 단계별 dump 파일 분석 |
| `python_package/src/dx_engine/bitmatch/module/config.py` | `TestConfig` dataclass |
| `python_package/src/dx_engine/bitmatch/module/utils.py` | 유틸리티 (float 비교, 경로 처리, RTOL/ATOL 상수 등) |
| `python_package/src/dx_engine/bitmatch/module/statistics.py` | `TestStatistics` 결과 집계 |

---

## 배경 지식

### DXNN 모델

- ONNX → DeepX Compiler로 컴파일 → DeepX NPU 실행용 모델 (.dxnn)
- **NPU Task** (1개 이상): NPU가 처리하는 연산. DXNN format.
- **CPU Task** (0개 이상): NPU가 처리할 수 없는 연산. ONNX 형태로 추출되어 OnnxRuntime으로 실행.
- Task 구조 예시: `npu_0 → cpu_0`, `npu_0 → cpu_0 → npu_1`, `cpu_0 → npu_0` 등

### NFH (NPU Format Handling)

DeepX NPU는 **NHWC format**을 사용하고, 연산 dimension의 element 개수가 16/64 등의 배수일 때 효율이 극대화된다.

| 단계 | 설명 |
|------|------|
| **Input NFH** | raw input → NCHW→NHWC 변환 + alignment padding 추가. 모델에 따라 변환/패딩이 불필요할 수도 있음 |
| **Output NFH** | NPU 결과(NHWC+padding) → padding 제거 + NHWC→NCHW 변환 |

NFH는 모델에 따라 발생하지 않을 수도 있다 (이미 format/align이 맞는 경우).

### GT (Ground Truth)

- **생성 주체**: Simulator (x86에서만 동작)
- **알려진 이슈**: ARM에서는 simulator가 돌지 않아 GT가 약간 다를 수 있음

---

## GT 파일 구조

### 파일 네이밍 규칙

```
{device}_{task_id}_{stage}_{test_case_idx}.bin
```

### 예시: YoloV8S (npu_0 → cpu_0 구조, 5개 테스트 케이스)

| 파일 | 의미 | 파이프라인 위치 |
|------|------|----------------|
| `input_N.bin` | 모델 최초 input | 시작 |
| `npu_0_encoder_input_N.bin` | NPU task의 raw input (Input NFH 전) — Encoder에 들어갈 입력 | Input NFH 전 |
| `npu_0_input_N.bin` | Input NFH 완료된 NPU input (padding+format 변환됨) — NPU에 실제로 들어가는 데이터 | Input NFH 후 |
| `npu_0_output_N.bin` | NPU 추론 결과 (NHWC+padding 포함) | NPU 추론 후 |
| `npu_0_decoder_output_N.bin` | Output NFH 완료 (padding 제거+NCHW) | Output NFH 후 |
| `cpu_0_input_N.bin` | CPU task의 input | CPU 추론 전 |
| `cpu_0_output_N.bin` | CPU task의 output | CPU 추론 후 |
| `output_N.bin` | 모델 최종 output | 끝 |

`N`은 테스트 케이스 인덱스 (0부터 시작).

### 파이프라인 흐름 (npu_0 → cpu_0 모델)

```
input
  → npu_0_encoder_input → [Input NFH] → npu_0_input
    → [NPU 추론] → npu_0_output
      → [Output NFH] → npu_0_decoder_output
        → cpu_0_input → [CPU 추론] → cpu_0_output
          → output
```

### 주의사항

- `input`과 `npu_0_encoder_input`은 모델에 따라 같을 수도, 다를 수도 있음 (첫 task가 npu_0인 경우 동일)
- `npu_0_decoder_output`과 `cpu_0_input`은 npu→cpu 구조에서만 같음. cpu→npu 구조에서는 다름
- `npu_0_encoder_input`과 `npu_0_input`이 동일한 경우도 있음 (NFH가 불필요한, 즉 align이 이미 맞는 모델). GT에 파일이 존재하되 내용이 같을 수 있음
- 마지막 task의 output이 `output_N.bin`과 논리적으로 동일할 것으로 예상

---

## 비교 메커니즘

### 일반 모드 (RELEASE, debug 옵션 없음)

**callback/return 기반 메모리 비교**. dump 파일 없음.

- **async** (`기본`): `ie.run_async()` → callback에서 output 수신 → worker 스레드에서 GT와 비교
- **sync** (`--sync`): `ie.run()` → 반환된 output을 GT와 비교
- **batch** (`--batch`): `ie.run(batch)` → batch output을 GT와 비교

→ **최종 output(`"LAST"`)만 비교**. 중간 단계는 비교하지 않음.

### 디버그 모드 (`--debug 2`)

**dump 파일 기반 단계별 비교**.

1. `DXRT_DEBUG_DATA=1` 환경변수 설정
2. `ie.run()` (sync)으로 실행 → RT가 CWD에 각 단계별 bin 파일 dump
3. dump된 RT 파일을 test case별 디렉토리로 이동
4. `DebugAnalyzer`가 GT 파일과 RT 파일을 네이밍 패턴으로 매칭하여 **모든 중간 단계** 비교
5. 최종 output도 `bitmatch_logic()`으로 메모리에서 별도 비교

→ **모든 단계 비교** (npu_input, encoder_input, npu_output, decoder_output, cpu_input, cpu_output, model output)

### `--debug 1`

`DXRT_DEBUG_DATA=1`만 설정하여 RT bin 파일을 생성하고 `rt/` 디렉토리로 이동. 분석은 수행하지 않음.

---

## 비교 로직 (Tolerance)

`bitmatch_logic()` → `_compare_outputs()` 순서:

```
1단계: np.array_equal()  →  bit-exact 비교  →  "PASS"
2단계: (실패 시) --use-ort이고 데이터가 4바이트 배수일 때만
       → uint8 데이터를 float32로 reinterpret
       → np.isclose(rtol=1e-4, atol=1e-2)  →  "PASS(CLOSE)"
3단계: 위 두 단계 모두 실패  →  "FAIL"
```

| 조건 | 비교 방식 |
|------|-----------|
| `--use-ort` OFF | bit-exact만. FAIL 아니면 PASS |
| `--use-ort` ON | bit-exact 먼저 시도 → 실패 시 float tolerance 비교 |

**상수** (`utils.py`): `RTOL = 1e-4`, `ATOL = 1e-2`

**근거**: CPU Task(OnnxRuntime)는 floating point 연산이므로 플랫폼에 따라 미세한 오차 발생 가능. NPU-only 실행은 bit-exact여야 함.

---

## Input/GT 로딩 로직

compile type과 use_ort 옵션에 따라 로딩하는 파일 패턴이 달라짐:

### Input 파일 패턴

| compile_type | use_ort | model version | input 패턴 |
|---|---|---|---|
| RELEASE | ON | - | `input_*.bin` |
| RELEASE | OFF | v7/v8 | `npu_0_encoder_input_*.bin` |
| RELEASE | OFF | v6 | `npu_0_input_*.bin` |
| DEBUG | - | v6/v7/v8 | `npu_0_input_*.bin` |

### GT Output 파일 패턴

| compile_type | use_ort | model version | GT 패턴 |
|---|---|---|---|
| RELEASE | ON | - | `output_*.bin` |
| RELEASE | OFF | v7/v8 | `npu_*_decoder_output_*.bin` |
| RELEASE | OFF | v6 | `npu_*_output_*.bin` |
| DEBUG | - | - | `npu_*_output_*.bin` (encoder/decoder 제외) |

### Bitmatch Mask

- RELEASE + use_ort: mask 없음
- RELEASE + non-use_ort (v7/v8): mask 없음
- RELEASE + non-use_ort (v6): `ie.get_bitmatch_mask(0)` 사용
- DEBUG: `ie.get_bitmatch_mask(0)` 사용

---

## 실행 모드 분기

```
process_model()
├── compile_type == "DEBUG"
│   ├── debug_mode → validate_device() + DebugAnalyzer (NPU만, skip_cpu_tasks=True)
│   └── 일반     → validate_device()만
├── compile_type == "RELEASE" + debug_mode
│   └── ie.run() 순차실행 + DebugAnalyzer (CPU 포함, skip_cpu_tasks=False)
└── compile_type == "RELEASE" (일반)
    ├── performance_mode → async (비교 없이 속도만 측정)
    ├── batch_mode       → batch 실행 + bitmatch
    ├── sync_mode        → sync 실행 + bitmatch
    └── 기본(async)      → async 실행 + callback에서 bitmatch
```

---

## CLI 옵션 레퍼런스

| 옵션 | 기능 |
|---|---|
| `--model_path, -m` | 단일 .dxnn 파일 또는 모델 디렉토리 지정 |
| `--gt_dir` | GT 디렉토리명 (기본: `gt`) |
| `--rt_dir` | RT 출력 디렉토리명 (기본: `rt`) |
| `--loops, -l` | 추론 반복 횟수 |
| `--dir, -d` | Regression 디렉토리 (하위 모델 폴더 일괄 테스트) |
| `--sync, -s` | sync 모드 (`ie.run()` 사용) |
| `--batch, -b` | batch 모드 |
| `--batch_iteration, -bi` | batch 반복 횟수 |
| `--test_iteration, -ti` | 전체 테스트 반복 횟수 (모든 모델을 N번 반복) |
| `--verbose, -v` | 상세 출력 |
| `--no-logging` | 파일 로깅(`BITMATCH_RESULTS.log`) 비활성화 |
| `--debug 0` | RT 바이너리 생성 안 함 (기본) |
| `--debug 1` | `DXRT_DEBUG_DATA=1` → RT 바이너리만 생성 |
| `--debug 2` | RT 바이너리 생성 + `DebugAnalyzer`로 모든 중간 단계 분석 |
| `--model_filter` | 테스트할 모델 이름 필터 파일 경로 |
| `--compile_type` | `RELEASE` 또는 `DEBUG` (미지정 시 모델에서 파싱) |
| `--performance, -p` | 성능 모드 (GT 비교 없이 속도 측정만) |
| `--multi_processing, -mp` | 멀티프로세싱 모드 |
| `--use-ort` | ONNX Runtime 사용 (CPU Task 포함 전체 파이프라인 실행) |
| `--npu, -n` | NPU bounding (0=all, 1=NPU0, 2=NPU1, ...) |
| `--devices` | NPU 디바이스 지정 (`all`, `0`, `0,1,2`, `count:N`) |
| `--input-order` | 입력 순서 (`random` 또는 `sequential`) |
| `--save_debug_report` | debug 분석 결과를 JSON 파일로 저장 |

---

## 핵심 요약

| 질문 | 답변 |
|------|------|
| bitmatch란? | 추론의 각 단계별 결과를 GT와 비교하여 **어디서부터 틀렸는지** 특정하는 테스트 도구 |
| 일반 모드 비교 방식? | callback/return으로 **메모리에서 최종 output만** 비교 |
| 디버그 모드 비교 방식? | `DXRT_DEBUG_DATA=1`로 dump 파일 생성 → **모든 중간 단계** 파일 비교 |
| float tolerance? | `--use-ort`일 때만 적용 (rtol=1e-4, atol=1e-2). 아니면 bit-exact |
| GT 생성? | Simulator (x86 전용). ARM에서는 GT가 약간 다를 수 있음 |

---

## NPU Core Tracking (Failure Diagnosis)

### 목적

bitmatch FAIL 시, 해당 추론이 **어느 NPU core (0/1/2)**에서 처리되었는지 보고하여, 특정 core의 하드웨어 불량 여부를 진단할 수 있게 한다.

### 구현 개요

| 계층 | 파일 | 변경 내용 |
|------|------|-----------|
| C++ Engine | `lib/include/dxrt/inference_engine.h` | `GetJobDevice(int jobId)` public method, `_jobDeviceMap` + mutex 추가 |
| C++ Job | `lib/inference_job.cpp` | `onAllRequestComplete()`에서 head NPU request의 `_processedDevId`를 `_jobDeviceMap[_jobId]`에 저장 (callback 호출 **전**) |
| C++ Engine | `lib/inference_engine.cpp` | `GetJobDevice()` 구현: mutex lock → map 조회 → entry 삭제 → return (없으면 -1) |
| pybind11 | `py_inference_engine.cpp` | `.def("get_job_device", ...)` 바인딩 추가 |
| Python | `tester.py` | `_record_failed_device()` 헬퍼, `job_device_map`, `failed_job_devices` dict 추가 |
| Python | `core.py` | `failed_job_devices`를 `failed_models` dict에 전달 |

### API

```python
# C++ side (pybind11 바인딩)
npu_core_id = ie.get_job_device(job_id)  # Returns 0, 1, 2 or -1 (unknown)
# One-shot: entry is removed after retrieval (메모리 누수 방지)
```

### 데이터 흐름

1. `_run_async_mode()`: `ie.run_async()` 호출 → `job_id` 반환 → `self.job_device_map[loop] = job_id`
2. C++ `onAllRequestComplete()`: head NPU request의 `_processedDevId` → `_jobDeviceMap[_jobId]`에 저장
3. callback → `_worker_loop()` / `callback_handler_sync_bitmatch()`에서 bitmatch 수행
4. FAIL 시: `_record_failed_device(loop_id)` 호출
   - `job_id = self.job_device_map[loop_id]`
   - `npu_core = self.ie.get_job_device(job_id)` (one-shot, C++ map entry 삭제됨)
   - `self.failed_job_devices[loop_id] = npu_core`
   - 로그 출력: `FAIL at loop {loop_id} processed by NPU core {npu_core}`
5. `log_all_results()`: NPU Core Distribution 요약 출력 (e.g., `NPU_0: 3 fail(s), NPU_2: 1 fail(s)`)

### 주의사항

- `get_job_device()`는 **one-shot** — 호출 후 entry가 삭제되므로, FAIL인 job만 조회해야 함
- PASS인 job은 조회하지 않으므로 map에 entry가 남지만, `process_model()` 시작 시 `self.job_device_map = {}` 리셋되어 Python 측 메모리는 해제됨
- C++ 측 `_jobDeviceMap`의 PASS entries는 InferenceEngine 소멸 시 자동 해제됨
- sync 모드 (`ie.run()`)에서는 `job_device_map`이 비어있으므로 device 추적이 동작하지 않음 (async 전용)

---

## References

- GT 예시 경로: `/home/hylee/test_models/YoloV8S-YOLOV8S-1/gt/`
- general.md의 Inference Architecture 섹션 참조 (Task, NFH, Buffer 관리 등)
