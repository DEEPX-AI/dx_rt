# DX_RT Project - General Guide for AI Assistant

---

## ⚠️ AI Assistant Instructions (작업 전 필수 확인)

### 코드 리뷰 체크리스트

모든 구현 작업 후 아래 항목을 검토:

- [ ] **완성도**: 구현이 완료되었는지, 누락된 부분은 없는지
- [ ] **개선점**: 코드 품질, 성능, 가독성 개선 여지
- [ ] **Critical Issues**: 보안, 메모리 누수, 크래시 가능성
- [ ] **과도한 구현**: 불필요하게 복잡하거나 over-engineering된 부분
- [ ] **함수/파일 분리**: 너무 긴 함수(100줄+)나 파일은 적절히 분리
- [ ] **설계 검토**: 아키텍처나 설계 자체에 문제가 없는지

> 결정이 필요한 부분은 critical issue가 아니면 **최적의 방안으로 자율 진행**.

### Problem 1-Pager (복잡한 문제의 경우)

문제가 복잡하거나 요구사항이 명확하지 않으면, 코딩 전에 아래 문서를 작성:

| 항목 | 설명 |
|------|------|
| **배경 (Background)** | 변경이 필요한 맥락과 동기 |
| **문제 (Problem)** | 해결하려는 이슈가 무엇인가? |
| **목표 (Goal)** | 성공의 기준 (성공한 상태는?) |
| **비목표 (Non-goals)** | 명확히 범위 밖(scope out)인 것 |
| **제약 (Constraints)** | 반드시 준수해야 할 기술적/비즈니스적 제약 |

> 불분명한 항목이 있으면 **인터뷰를 요청**하여 명확한 답을 얻은 후 진행.

---

## Project Overview

DX_RT (DeepX Runtime)는 DeepX NPU를 위한 런타임 프레임워크입니다.
- NPU 디바이스와의 통신 및 추론 실행을 담당
- C++ 기반의 라이브러리 및 CLI 도구 제공
- Python 바인딩 지원

## Project Structure

```
dx_rt/
├── lib/                    # Core runtime library
│   ├── data/               # Embedded firmware data (ppcpu)
│   ├── device_pool/        # Device management
│   ├── parsers/            # Model parsers (v6, v7, v8)
│   └── include/dxrt/       # Public headers
├── cli/                    # Command-line tools
│   ├── run_model.cpp       # Model execution tool
│   ├── dxbenchmark/        # Benchmarking tool
│   └── dxtop/              # NPU monitoring tool
├── examples/               # Usage examples
├── python_package/         # Python bindings
├── test/                   # Unit tests
├── scripts/                # Build/install scripts
└── guides/                 # AI assistant guides
    ├── general.md          # This file (project-wide rules)
    └── [subproject].md     # Sub-project specific guides
```

## Build System

- **CMake** 기반 빌드 시스템
- **Target architectures**: x86_64, aarch64
- **Cross-compilation** 지원
- **Yocto** 빌드 시스템 지원 (patch 기반)

### ⚠️ 빌드 방법 (필수 준수)

프로젝트 빌드는 **반드시** 아래 명령어로 수행:

```bash
cd /home/hylee/dx_rt && ./build.sh
```

> **절대 `cmake --build`, `make`, `ninja` 등 직접 빌드 명령어를 사용하지 마세요.** `build.sh`가 모든 설정을 포함합니다.

---

# Code Conventions

## Naming Conventions

| 대상 | 규칙 | 예시 |
|------|------|------|
| Local 변수 | snake_case | `int user_count = 0;` |
| 함수 Parameter | lowerCamelCase | `void SetData(int newValue);` |
| Class Public 함수 | UpperCamelCase | `void ProcessTask();` |
| Class Private 함수 | lowerCamelCase | `void internalLogic();` |
| Class Private 변수 | _lowerCamelCase (Underscore prefix) | `int _retryCount;` |

## Bracket Style

- **Allman Style** 적용: `for`, `if`, `while`, `switch`, 함수, class 선언 등 모든 bracket은 새 줄에서 시작
- `{}` 중첩에 따라 **항상 indent 적용**

```cpp
// ✅ 올바른 예시 (Allman Style)
class MyClass
{
public:
    void ProcessTask()
    {
        if (condition)
        {
            for (int i = 0; i < 10; i++)
            {
                doSomething();
            }
        }
        else
        {
            doOther();
        }
    }
};

// ❌ 잘못된 예시 (K&R Style - 사용하지 않음)
class MyClass {
public:
    void ProcessTask() {
        if (condition) {
            for (int i = 0; i < 10; i++) {
                doSomething();
            }
        }
    }
};
```

## General Guidelines

- C++11 이상 표준 사용
- 4 spaces indent (tab 사용 금지)
- 헤더 파일에는 `#pragma once` 사용
- Public API는 `include/dxrt/` 하위에 위치

---

# SonarQube Code Smell 가이드라인

코드 작성 및 수정 시 아래 규칙을 준수하여 SonarQube code smell을 사전에 방지한다.

## C++ 규칙

### 1. Cognitive Complexity ≤ 25
- 함수의 인지 복잡도가 25를 초과하면 안 된다.
- **해결 방법**: 로직을 의미 단위의 private 헬퍼 메서드로 추출한다.
- 복잡도 계산: `if`, `for`, `while`, `catch` 등 분기/반복마다 +1, 중첩 시 depth만큼 추가 가산.

### 2. Nesting Depth ≤ 3
- `for→if→if→try`처럼 4단 이상 중첩하지 않는다.
- **해결 방법**: 내부 로직을 별도 함수로 추출하여 중첩을 줄인다. Early return(guard clause) 활용.

### 3. `const void` 리턴 타입 금지
- `const void`는 의미가 없다. `void`에는 `const`를 한정할 대상이 없기 때문.
- **해결 방법**: 리턴 타입 앞의 `const`를 제거한다.
- 혼동 주의: "리턴 앞 `const` 제거"와 "함수 뒤 `const` 추가"는 서로 다른 위치다.

### 4. 멤버 함수 `const` 한정자
- 멤버 변수를 수정하지 않는 함수는 `const` 멤버 함수로 선언해야 한다.
- **해결 방법**: 함수 시그니처 **뒤에** `const`를 추가한다 (헤더 + 구현 모두).
- 예: `void cleanupTempDirectory() const;`

### 5. Generic catch 금지
- `catch (...)` 대신 구체적 예외 타입을 사용한다.
- **해결 방법**: `catch (const std::exception& e)` 등으로 변경.
- **예외**: 소멸자(`~Destructor()`)에서는 예외 전파를 막기 위해 `catch (...)` 허용.
- **주의**: `dxrt::Exception`은 `std::exception`을 상속하지 않으므로, dxrt 예외를 잡으려면 별도 catch 절 필요.

### 6. Integer Precision Loss 방지
- `std::chrono::count()` 등의 결과를 `int`에 담으면 정밀도 손실 발생.
- **해결 방법**: `int64_t` 또는 `auto`를 사용한다.
- 불필요한 `static_cast<int>(...)` 사용을 피한다.

### 7. 읽기 전용 변수는 `const` 참조
- 반복문 등에서 수정하지 않는 변수는 `const auto&`로 선언한다.
- 예: `for (const auto& tp : tps)` (읽기만 할 때)

### 8. 불필요한 코드 블록 `{ }` 제거
- 의미 없는 중괄호 블록은 제거한다.
- **예외**: `std::lock_guard` 등 RAII 패턴의 스코프 제한 목적 `{ }`는 **제거하면 안 된다**. 이 경우 SonarQube 경고는 false positive이므로 `// NOSONAR` 주석 처리.

## Python 규칙

### 1. Cognitive Complexity ≤ 15
- 함수의 인지 복잡도가 15를 초과하면 안 된다.
- **해결 방법**:
  - 반복되는 `if/elif` 분기 → 딕셔너리 매핑 + 룩업으로 대체.
  - 중첩된 내부 함수(closure) → 인스턴스 메서드로 추출.
  - 깊은 조건 중첩 → guard clause(early return)로 평탄화.

---

# Known Issues & Notes

## Build Issues

- Windows tar로 생성된 `.tgz` 파일은 `--strip-components=2` 필요 (`./` prefix 때문)
- Yocto 빌드 시 patch에 바이너리 파일(`ppcpu.bin` 등) 포함 필요

## Runtime Notes

- `ENABLE_CPU_ACCELERATION=1` 환경변수로 CPU 가속 활성화
- ONNX Runtime 연동 시 `--use-ort` 옵션 사용

### use-ort 옵션과 Task 의존성

**`use-ort`** (`InferenceOption.useORT`)는 CPU Task(ONNX Runtime)를 Inference Engine의 실행 Task 목록에 등록할지 여부를 결정하는 옵션이다.

- `true`: CPU Task를 실행 Task 목록에 등록 → 정상 실행
- `false`: CPU Task를 실행 Task 목록에서 **제외** (NPU-only 실행)

**Task 의존성 추적**: 모델 파싱 시 각 Task의 input이 어떤 Task의 output으로부터 오는지 추적하며, source Task가 Inference Engine의 실행 Task 목록에 등록되어 있는지 확인한다.

**주의 (CPU → NPU 구조)**: CPU Task가 NPU Task보다 앞에 있는 모델 구조에서 `use-ort=false`로 설정하면, NPU Task의 input source인 CPU Task가 실행 목록에 없으므로 **실행이 실패**한다. 이는 유효하지 않은 옵션 조합이다.

---

## Inference Architecture

### 핵심 개념

| 개념 | 설명 |
|------|------|
| **Task** | 모델을 구성하는 개별 실행 단위. NPU Task와 CPU Task로 나뉨 |
| **Inference Job** | 모든 Task를 관리하는 주체. Task 간 의존성(DAG)에 따라 실행 순서 결정 |
| **Request** | Task의 실행 정보를 바탕으로 Inference Job이 생성하는 실제 실행 요청 |

### 모델 구성

하나의 모델은 **NPU Task**와 **CPU Task**로 나뉜다:
- **NPU Task**: NPU가 처리할 수 있는 연산 (DXNN format)
- **CPU Task**: NPU가 처리하지 못하는 연산. ONNX 형태로 추출되어 **ONNX Runtime**으로 실행

Task들은 DAG(Directed Acyclic Graph) 구조로 연결되며, `nexts()`(후속 Task)와 `prevs()`(선행 Task) 관계로 의존성을 표현한다.

### 추론 실행 흐름

```
Inference Engine
    └─ Model Parsing → Task 생성 (NPU/CPU 분리)
        └─ Inference Job 생성
            └─ Head Task부터 Request 생성 → 실행
                └─ Task 완료 시 onRequestComplete() → 후속 Task 스케줄링
                    └─ 모든 Task 완료 시 onAllRequestComplete() → 콜백
```

### 단일 NPU Task 처리 파이프라인

```
Input NFH → PCIe Write → NPU Inference → PCIe Read → Output NFH → [CPU Task]
```

| 단계 | 설명 | 필수 |
|------|------|------|
| **Input NFH** | NCHW → NHWC 변환, NPU 친화적 Data Alignment 적용 | ✅ |
| **PCIe Write** | Host → NPU 메모리 데이터 전송 (DMA) | ✅ |
| **NPU Inference** | NPU 하드웨어 추론 실행 (Firmware가 `inf_time` 반환) | ✅ |
| **PCIe Read** | NPU → Host 메모리 결과 수신 (DMA) | ✅ |
| **Output NFH** | NPU alignment padding 제거, NHWC → NCHW 변환 | ❌ (선택) |
| **CPU Task** | NPU 미지원 연산을 ONNX Runtime으로 CPU 실행 | ❌ (선택) |

### Buffer 관리

각 Task는 자체 **FixedSizeBuffer** 풀을 보유:
- **NPU Task**: `encoded_input`, `output`, `encoded_output` 3개 풀
- **CPU Task**: `output` 1개 풀

Buffer 획득은 `AcquireAllBuffers()`로 일괄 획득하며, 모든 풀에서 사용 가능한 버퍼가 없으면 **condition variable로 블로킹 대기**한다. Inference Job이 모든 Task 완료 후 일괄 release한다.

### NPU Task → CPU Task 전환 메커니즘

1. NPU Task 완료 → `onRequestComplete()` 호출
2. 출력 텐서를 `_tensors` map에 저장 (텐서 이름으로 키)
3. 후속 Task의 모든 입력 텐서가 `_tensors`에 존재하는지 확인 (`checkAndSetTaskReady()`)
4. 조건 충족 시 `processReadyTask()` → CPU Task의 입력 텐서에 NPU 출력 데이터 포인터 매핑
5. `RequestResponse::InferenceRequest()` → CPU 경로 → `CpuHandleWorker` 큐에 enqueue
6. Worker 스레드가 dequeue하여 ONNX Runtime `session->Run()` 실행

### 주요 파일 참조

| 파일 | 역할 |
|------|------|
| `lib/inference_engine.cpp` | 모델 파싱, Task 생성, 추론 실행 진입점 |
| `lib/inference_job.cpp` | Task 실행 순서 관리, 의존성 해소, 콜백 처리 |
| `lib/device_pool/request_response_class.cpp` | Request 생성, NPU/CPU 분기, Buffer 획득 |
| `lib/device_pool/acc_device_task_layer.cpp` | ACC 모드 NPU I/O (PCIe Write/Read) |
| `lib/device_pool/std_device_task_layer.cpp` | STD 모드 NPU I/O |
| `lib/npu_format_handler.cpp` | Input/Output NFH (데이터 포맷 변환) |
| `lib/cpu_handle.cpp` | ONNX Runtime CPU 추론 실행 |
| `lib/cpu_handle_worker.cpp` | CPU Task 워커 스레드 풀 관리 |
| `lib/fixed_size_buffer.cpp` | 고정 크기 버퍼 풀 (블로킹 획득/해제) |