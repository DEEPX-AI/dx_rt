# Validation Test - AI Assistant Guide

> ⚠️ **프로젝트 전체 규칙 및 Code Convention은 [general.md](general.md)를 참조하세요.**

---

## Sub-Project Overview

**목적**: DXRT Inference Engine의 기능 및 성능 검증을 위한 통합 테스트 프레임워크

**위치**: `dx_rt/test/release/validation_test/`

**주요 기능**:
- JSON 기반 테스트 케이스 정의 및 자동 생성
- 다양한 Inference 옵션 조합 테스트 (sync/async, single/multi input, user/internal buffer)
- Bitmatch 기반 결과 검증 (GT 파일과 비교)
- Subprocess 기반 테스트 실행 (프로세스 격리)
- 테스트 결과 리포트 생성

---

## Architecture

```
validation_test/
├── test_app/
│   ├── cpp/                    # C++ 구현 (메인)
│   │   ├── validation_test.cpp # Entry point
│   │   ├── include/            # Header files
│   │   │   ├── generator.h     # TestCase, IEOption, ExecutionOption 구조체
│   │   │   ├── executor.h      # BaseExecutor 및 파생 클래스
│   │   │   ├── executorManager.h # Thread/IE 관리
│   │   │   ├── test_manager.h  # 테스트 실행 및 리포트
│   │   │   ├── bitmatcher.h    # 결과 검증
│   │   │   └── utils.h         # 유틸리티 함수
│   │   └── src/                # Source files
│   └── python/                 # Python 구현 (미러링)
└── test_config/
    ├── rt/                     # Runtime 테스트 설정 JSON
    ├── do/                     # Dynamic offloading 테스트 설정
    └── tmp/                    # 임시 파일 디렉토리
```

---

## Key Classes

### 1. Generator (`generator.h/cpp`)
- JSON 파일 파싱 및 TestCase 생성
- 옵션 조합 생성 (random 모드 지원)

**주요 구조체**:
```cpp
struct IEOption {
    string model_path;
    string dynamicCpuOffloading;  // "on" / "off"
    string threadType;            // "single-ie" / "multi-ie"
    int threadCount;
    bool ort;
    string bound;                 // "NPU_ALL", "NPU_0", etc.
    string device;                // "all", "0", "0,1"
};

struct ExecutionOption {
    string inferenceFunction;     // "sync", "async", "batch"
    string inputStyle;            // "single", "multi-map", "multi-vec", "auto-split"
    string outputBuffer;          // "user" / "internal"
    string asyncMethod;           // "callback" / "wait"
    int loop;
    int time;
    bool bitmatch;
};

struct TestCase {
    IEOption ieOption;
    std::vector<ExecutionOption> execOptions;
};
```

### 2. Executor (`executor.h/cpp`)
- Template Method 패턴 적용
- `BaseExecutor`: 공통 로직 (validation, bitmatch 호출)
- `SyncExecutor`: 동기 실행
- `AsyncCallbackExecutor`: 비동기 콜백 모드
- `AsyncWaitExecutor`: 비동기 Wait 모드

### 3. ExecutorManager (`executorManager.h/cpp`)
- Thread 관리 및 InferenceEngine 인스턴스 관리
- `single-ie` 모드: 공유 IE로 멀티스레드 실행
- `multi-ie` 모드: 스레드별 독립 IE 생성

### 4. TestManager (`test_manager.h/cpp`)
- 전체 테스트 흐름 관리
- Subprocess 모드로 모델별 테스트 격리
- 결과 집계 및 리포트 생성

### 5. BitMatcher (`bitmatcher.h/cpp`)
- GT (Ground Truth) 파일과 출력 비교
- Byte 단위 비교 및 Float tolerance 비교 지원
- Mask 지원 (v6 모델)

---

## Execution Flow

```
main()
  └── Generator::LoadJson()
  └── Generator::GenerateTestCases()
  └── TestManager::Run()
        └── (각 모델별) runModelInSubprocess()
              └── runSingleTestCase()
                    └── runExecutionOption()
                          └── ExecutorManager::Run()
                                └── BaseExecutor::Execute()
                                      └── doExecute() (virtual)
                                      └── BitMatcher::BitMatch()
        └── printTestSummary()
        └── MakeReport()
```

---

## Files to Track

| 파일 | 역할 | 비고 |
|------|------|------|
| `validation_test.cpp` | Entry point, CLI 파싱 | cxxopts 사용 |
| `generator.cpp` | JSON 파싱, TestCase 생성 | rapidjson 사용 |
| `executor.cpp` | Inference 실행 로직 | Template Method 패턴 |
| `executorManager.cpp` | Thread/IE 관리 | single-ie/multi-ie 분기 |
| `test_manager.cpp` | 테스트 흐름 관리 | Subprocess 모드 |
| `bitmatcher.cpp` | 결과 검증 | GT 비교 |

---

## Sub-Project Specific Notes

### Naming Convention 특이사항
- 일부 파일명이 camelCase (`executorManager.cpp`)와 snake_case (`test_manager.cpp`) 혼용
- 헤더와 소스 파일명 일관성 유지 필요

### 주의사항
- Subprocess 모드에서 각 모델은 별도 프로세스로 실행됨
- `_tempPath` 계산 로직이 플랫폼별로 다름 (Windows/Linux)
- BitMatcher는 thread-safe하게 구현됨 (`_outputsMutex` 사용)

### Dependencies
- `dxrt` 라이브러리 (메인 런타임)
- `rapidjson` (JSON 파싱, 헤더 온리)
- `cxxopts` (CLI 파싱, 헤더 온리)

---

## Known Issues

<!-- Code smell 수정 과정에서 발견된 이슈 기록 -->

| 이슈 | 파일 | 상태 |
|------|------|------|
| | | |

---

## Current Task

- [ ] Code smell 수정

---

## Solution / Progress

### 접근 방법
1. SonarQube 또는 수동 코드 리뷰로 code smell 식별
2. general.md 규칙에 따라 수정
3. 수정 후 validation_test.md 및 general.md 업데이트

### 진행 상황
- [x] 프로젝트 구조 분석 완료
- [x] validation_test.md 작성 완료
- [ ] Code smell 수정 진행 중

---

## References

- [general.md](general.md) - 프로젝트 전체 규칙
- DXRT API 문서
