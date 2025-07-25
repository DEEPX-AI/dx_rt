# Multi-Input Model 추론 예제

이 디렉토리는 DXRT에서 multi-input 모델을 사용한 추론 예제들을 포함합니다.

## 파일 구조

```
examples/
├── multi_input_model_inference/
│   ├── multi_input_model_inference.cpp      # C++ 예제 코드
│   └── CMakeLists.txt                       # 빌드 설정
├── python/
│   └── multi_input_model_inference.py       # Python 예제 코드
└── README_multi_input_examples.md           # 이 파일
```

## 예제 내용

### C++ 예제 (`multi_input_model_inference.cpp`)

다음과 같은 9가지 예제들을 포함합니다:

#### 단일 추론 - No Output Buffer (자동 할당)
1. **Dictionary Format (No Buffer)**: 입력 텐서를 이름으로 매핑, 출력 자동 할당
2. **Vector Format (No Buffer)**: 입력 텐서를 순서대로 벡터 제공, 출력 자동 할당  
3. **Auto-Split (No Buffer)**: 연결된 단일 버퍼 자동 분할, 출력 자동 할당

#### 단일 추론 - With Output Buffer (사용자 제공)
4. **Dictionary Format (With Buffer)**: 입력 텐서를 이름으로 매핑, 사용자 출력 버퍼
5. **Vector Format (With Buffer)**: 입력 텐서를 순서대로 벡터 제공, 사용자 출력 버퍼
6. **Auto-Split (With Buffer)**: 연결된 단일 버퍼 자동 분할, 사용자 출력 버퍼

#### 배치 추론
7. **Batch Explicit**: 배치 샘플들을 명시적 구조로 처리

#### 비동기 추론
8. **Async Callback**: 콜백 함수 기반 비동기 추론
9. **Simple Async**: 간단한 비동기 추론

### Python 예제 (`multi_input_model_inference.py`)

다음과 같은 10가지 예제들을 포함합니다:

#### 단일 추론 - No Output Buffer (자동 할당)
1. **Dictionary Format (No Buffer)**: 딕셔너리 형태로 입력 제공, 출력 자동 할당
2. **Vector Format (No Buffer)**: 리스트 형태로 입력 제공, 출력 자동 할당
3. **Auto-Split (No Buffer)**: 연결된 단일 버퍼를 자동 분할, 출력 자동 할당

#### 단일 추론 - With Output Buffer (사용자 제공)
4. **Dictionary Format (With Buffer)**: 딕셔너리 형태로 입력 제공, 사용자 출력 버퍼
5. **Vector Format (With Buffer)**: 리스트 형태로 입력 제공, 사용자 출력 버퍼
6. **Auto-Split (With Buffer)**: 연결된 단일 버퍼를 자동 분할, 사용자 출력 버퍼

#### 배치 추론
7. **Explicit 배치 추론**: 명시적 배치 형태로 추론 (List[List[np.ndarray]])
8. **Flattened 배치 추론**: 플래튼된 형태로 배치 추론 (List[np.ndarray])

#### 비동기 추론
9. **비동기 추론 (콜백)**: 콜백을 사용한 비동기 추론
10. **간단한 비동기 추론**: run_async 방식의 비동기 추론

### 강화된 검증 기능

모든 예제에서 다음과 같은 강화된 출력 검증을 수행합니다:

#### 검증 항목
- **출력 존재성**: None, 빈 리스트 검사
- **데이터 타입**: numpy.ndarray/TensorPtr 타입 검증
- **텐서 크기**: 빈 텐서 (size=0) 검사
- **형태 유효성**: 유효하지 않은 shape 검사
- **수치 유효성**: NaN, Inf 값 검사 (Python)
- **포인터 유효성**: null 포인터 검사 (C++)
- **배치 구조**: 배치 출력의 올바른 중첩 구조 검증

#### 출력 검증 로직
```cpp
// C++ 검증 예시
bool validateOutputs(const dxrt::TensorPtrs& outputs, size_t expectedCount, const std::string& testName) {
    // 1. 출력 존재 검사
    if (outputs.empty()) return false;
    
    // 2. 개수 검증
    if (outputs.size() != expectedCount) return false;
    
    // 3. 각 텐서 검증
    for (const auto& output : outputs) {
        if (!output || output->size_in_bytes() == 0 || 
            output->shape().empty() || !output->data()) {
            return false;
        }
    }
    return true;
}
```

```python
# Python 검증 예시
def validate_outputs(outputs, expected_count, test_name):
    # 1. 기본 검사
    if outputs is None or not isinstance(outputs, list):
        return False
    
    # 2. 배치/단일 구분
    is_batch = isinstance(outputs[0], list) if outputs else False
    
    # 3. 각 텐서 검증 (NaN/Inf 포함)
    for output in outputs:
        if not isinstance(output, np.ndarray) or output.size == 0:
            return False
        if np.any(np.isnan(output)) or np.any(np.isinf(output)):
            return False
    
    return True
```

### C++ 예제 실행

```bash
./multi_input_model_inference <model_path>
```

예시:
```bash
./multi_input_model_inference /path/to/your/multi_input_model.dxnn
```

### Python 예제 실행

#### 필수 요구사항

- Python 3.8+
- numpy
- dx_engine (DXRT Python 패키지)

```bash
cd examples/python
python multi_input_model_inference.py <model_path>
```

예시:
```bash
python multi_input_model_inference.py /path/to/your/multi_input_model.dxnn
```

## 모델 요구사항

이 예제들을 실행하려면 다음 조건을 만족하는 모델이 필요합니다:

1. **Multi-Input 모델**: 여러 개의 입력 텐서를 가진 모델
2. **컴파일된 모델**: DX-COM으로 컴파일된 `.dxnn` 파일

## 예제 출력

### 모델 정보 출력 예시

```
============================================================
                    MODEL INFORMATION
============================================================
Multi-input model: Yes
Input tensor count: 2
Total input size: 614400 bytes
Total output size: 4000 bytes

Input tensor details:
  input1: 150528 bytes -> Task: npu_task_0
  input2: 463872 bytes -> Task: npu_task_0

Output tensor details:
  output1: 4000 bytes
============================================================
```

### 테스트 실행 출력 예시

```
============================================================
                    RUNNING TESTS
============================================================

1. Dictionary Format Single Inference (No Output Buffer)
   - Input: Dictionary mapping tensor names to data
   - API: ie.RunMultiInput(input_dict) - auto-allocated output

[RESULT] Dictionary Format (No Buffer): All outputs valid (1 tensors)
         Inference time: 15.34 ms

------------------------------------------------------------

...

============================================================
                    TEST SUMMARY  
============================================================
* PASS | Dictionary Format (No Buffer)
* PASS | Vector Format (No Buffer)
* PASS | Auto-Split (No Buffer)
* PASS | Dictionary Format (With Buffer)
* PASS | Vector Format (With Buffer)
* PASS | Auto-Split (With Buffer)
* PASS | Batch Explicit
* PASS | Async Callback
* PASS | Simple Async
------------------------------------------------------------
Total: 9 | Passed: 9 | Failed: 0
 *** All tests passed successfully! ***
```

## 성능 팁

1. **출력 버퍼 사전 할당**: With Buffer 테스트들처럼 출력 버퍼를 미리 할당하여 성능 향상
2. **배치 처리**: 여러 샘플을 배치로 처리하여 처리량 향상
3. **비동기 처리**: CPU 작업과 NPU 작업을 병렬로 수행하여 전체 성능 향상
4. **메모리 재사용**: 입력/출력 버퍼를 재사용하여 메모리 할당 오버헤드 감소
5. **검증 최적화**: 프로덕션 환경에서는 검증 로직을 간소화하여 성능 향상

## 문제 해결

### 일반적인 오류들

1. **"This model is not a multi-input model"**: 단일 입력 모델에서 multi-input API 사용 시 발생
2. **"Input tensor names mismatch"**: 제공된 입력 텐서 이름이 모델 요구사항과 다를 때 발생
3. **"Buffer size mismatch"**: 입력/출력 버퍼 크기가 모델 요구사항과 다를 때 발생
4. **"[ERROR] Output validation failed"**: 출력 검증에서 문제 발견 시 발생 (NaN, 빈 텐서 등)

### 디버깅 팁

1. **모델 정보 확인**: `ie.IsMultiInputModel()`, `ie.GetInputTensorNames()` 등으로 모델 정보 확인
2. **버퍼 크기 확인**: `ie.GetInputTensorSizes()`, `ie.GetOutputTensorSizes()` 로 필요한 버퍼 크기 확인
3. **검증 로그 확인**: 각 테스트의 검증 결과 메시지를 통해 문제점 파악
4. **출력 버퍼 타입**: 사용자 제공 버퍼와 자동 할당 버퍼의 차이점 이해

## 테스트 시나리오별 특징

### No Output Buffer vs With Output Buffer
- **No Buffer**: 메모리 관리 간편, 추론 엔진이 자동 할당
- **With Buffer**: 성능 최적화 가능, 메모리 재사용 가능

### 동기 vs 비동기
- **동기**: 간단한 구현, 순차 처리
- **비동기**: 높은 처리량, 복잡한 콜백 관리 필요

### 단일 vs 배치
- **단일**: 지연시간 최적화
- **배치**: 처리량 최적화, 더 많은 메모리 사용

## 추가 참고 자료
- DXRT API 문서
- Multi-Input 모델 추론 가이드 (multi_input_model_inference_guide.md)