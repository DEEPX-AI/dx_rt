# Unit Test Guide

## 1. Architecture

- **Google Test 1.12.0** + **Google Mock** (CMake FetchContent)
- Binary: `bin/dxrt_test`
- `test/unittest/CMakeLists.txt` uses `file(GLOB_RECURSE)` — .cpp 파일 추가 시 CMake 수정 불필요

```
test/unittest/          ← test .cpp files
test/unittest/mocks/    ← mock headers
test/include/           ← shared test headers (dxrt_test.h)
```

### File Naming (팀 공유 규칙 — 변경 금지)

| Suffix | Purpose |
|--------|---------|
| `_test.cpp` | 정상 동작 검증 |
| `_basic_test.cpp` | 기초 동작 |
| `_error_test.cpp` | 에러 경로 (null, invalid param, mock 실패 반환) |
| `_edge_test.cpp` | 경계값, overflow, round-trip, branch 조합 |
| `_*_test.cpp` | 특정 기능 집중 (_formatted_, _transpose_ 등) |

Role split: error path → `_error_test`, 값 경계/branch 조합 → `_edge_test`.
작성 전 기존 파일 검토하여 중복 금지. 불분명하면 `_edge_test`에 추가.

---

## 2. Test 작성 규칙

### AAA 패턴 (필수)

```cpp
TEST(SuiteName, CaseName)
{
    // Arrange
    std::vector<uint8_t> src = {1, 2, 3, 4};
    std::vector<uint8_t> dst(output_size, 0);
    Bytes input{static_cast<uint32_t>(src.size()), src.data()};
    Bytes output{static_cast<uint32_t>(dst.size()), dst.data()};

    // Act
    int rc = NpuFormatHandler::encode(input, output, col, align_unit);

    // Assert
    EXPECT_EQ(0, rc);
    EXPECT_EQ(expected_size, output.size);
    EXPECT_EQ(expected_val, dst[0]);
}
```

### Edge Case 체크리스트

```
□ null 포인터 input/output (data == nullptr)
□ size = 0
□ 음수 파라미터
□ overflow 경계값 (INT_MAX, UINT32_MAX 근처)
□ alignment boundary ±1 (align_unit=4 → col=3,4,5)
□ in-place 연산 (input.data == output.data)
□ input size mismatch (size % col != 0)
□ data type별 element_size (UINT8=1, FLOAT32=4, INT16=2)
□ round-trip (encode→decode→원본 비교)
□ 특수 float (NaN, Inf, denorm, -0.0f)
□ trivial shape (1×1, 1×N, N×1)
```

### Branch Coverage 체크리스트

Line coverage와 별개로, 모든 조건 분기의 true/false 경로를 테스트:

```
□ 복합 조건식(&&, ||) 각 피연산자 true/false 조합
  예: `if (a && b)` → (a=F), (a=T,b=F), (a=T,b=T) 3개 케이스
□ 삼항 연산자 양쪽 경로 발생 입력
  예: `x = (c < d) ? c : d` → c<d인 경우 + c>=d인 경우
□ Mock 실패 반환값 테스트 (ret < 0, nullptr 반환 등)
  예: EXPECT_CALL(*mock, IoctlWrite(_,_,_)).WillOnce(Return(-EIO));
□ switch/case 모든 case + default
□ early return guard의 true/false 양쪽
```

### Branch 미커버 분류 기준

| 유형 | 처리 | 방법 |
|------|------|------|
| Mock 실패 시나리오 미테스트 | **테스트 추가** | `_error_test.cpp`에 Mock 실패 반환 케이스 추가 |
| 복합 조건식 일부 조합 미테스트 | **테스트 추가** | `_edge_test.cpp`에 조건 조합 케이스 추가 |
| 삼항 연산자 한쪽만 테스트 | **테스트 추가** | `_edge_test.cpp`에 양쪽 입력 추가 |
| `catch(bad_alloc)` 방어 코드 | **제외 마킹** | `// LCOV_EXCL_BR_LINE` |
| 수학적 도달 불가 bounds check | **제외 마킹** | `// LCOV_EXCL_BR_START` ~ `// LCOV_EXCL_BR_STOP` |
| `#ifdef` 다른 빌드 구성 분기 | **해당 빌드에서 측정** | 단일 빌드에서 커버 불가, 제외 대상 |
| HW/driver 의존 경로 | **Testability 리포트** | 섹션 6 참조 |

### 아키텍처별 조건부 컴파일 (필수)

NEON/IPP 등 아키텍처 종속 API를 사용하는 테스트는 반드시 전처리 가드로 감싸야 한다.
x86 빌드에서 NEON 코드를 포함하면 **컴파일/링크 에러** 발생.

**가드 매크로 선택 기준:**

| 조건 | 매크로 | 정의 위치 |
|------|--------|----------|
| NFH acceleration 토글 (`_sNfhAcceleration`, `SetEnable(NFH_ACCELERATION,...)`) | `DXRT_NFH_ACCELERATION_AVAILABLE` | `configuration.h` (`USE_IPP \|\| USE_NEON` 시 정의) |
| NEON 전용 함수 (`neon_bidirectional_transpose`, `xnnpack_transpose`) | `USE_NEON` | `lib/CMakeLists.txt` (aarch64 빌드 시 정의) |
| IPP 전용 함수 | `USE_IPP` | `lib/CMakeLists.txt` (x86_64 빌드 시 정의) |
| 아키텍처 직접 분기 | `__aarch64__` / `__x86_64__` | 컴파일러 내장 |

**패턴:**

```cpp
// acceleration 토글을 사용하는 테스트 → DXRT_NFH_ACCELERATION_AVAILABLE
#ifdef DXRT_NFH_ACCELERATION_AVAILABLE
TEST(TransposeBranch, AccelEnabled_ElementSize2_FallsThrough)
{
    config.SetEnable(Configuration::ITEM::NFH_ACCELERATION, true);
    // ...
    config.SetEnable(Configuration::ITEM::NFH_ACCELERATION, false);
}
#endif

// NEON 커널 직접 호출 테스트 → USE_NEON
#ifdef USE_NEON
TEST(XnnpackTranspose, BasicFloat) { ... }
#endif
```

**주의사항:**
- acceleration을 사용하지 않는 순수 로직 테스트 (overflow 검출, 파라미터 검증 등)는 가드 불필요
- 가드 밖에 다른 테스트가 있을 수 있음 — `#ifdef`/`#endif`로 해당 테스트만 감쌀 것
- 참고: [npu_format_handler_transpose_test.cpp](../test/unittest/npu_format_handler_transpose_test.cpp), [npu_format_handler_branch_test.cpp](../test/unittest/npu_format_handler_branch_test.cpp)

### Dead Code 확인 (테스트 작성 전)

```bash
grep -rn "함수명" lib/ --include="*.cpp" --include="*.h" | grep -v "정의 파일"
```
선언/정의만 존재 → 테스트 불필요, 삭제 예정으로 분류.

---

## 3. Build & Run

```bash
# 빌드 (반드시 build.sh만 사용, cmake/ninja 직접 호출 금지)
cd /home/orangepi/dx_rt && ./build.sh

# 전체 테스트
./bin/dxrt_test

# 필터 실행
./bin/dxrt_test --gtest_filter='CdivEdge.*:EncodeEdge.*'
./bin/dxrt_test --gtest_filter='CdivEdge.DivisionByZero'
```

### 실패 디버깅

1. 실패 메시지의 에러 문자열 확인
2. 소스에서 해당 문자열 grep → 조건문 위치 파악
3. 테스트 파라미터를 소스 로직에 수동 대입 추적
4. 판별: 테스트 설정 오류 → 테스트 수정 / 소스 버그 → 소스 수정 또는 리포트

---

## 4. Coverage

### 빌드 (debug 금지 — ASAN 충돌. release 사용)

```bash
cd /home/orangepi/dx_rt
rm -rf build_coverage && mkdir build_coverage && cd build_coverage

cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain.x86_64.cmake \
  -DCMAKE_BUILD_TYPE=release \
  -G Ninja \
  -DUSE_SERVICE=OFF -DUSE_PYTHON=OFF -DUSE_ORT=OFF \
  -DUSE_DXRT_TEST=ON \
  -DENABLE_COVERAGE=ON

ninja dxrt_test
```

### 측정

```bash
# 테스트 실행
./bin/dxrt_test --gtest_filter='<target_filters>'

# Line coverage 수집
lcov --capture --directory . --output-file coverage_raw.info \
  --ignore-errors mismatch,inconsistent

# Branch coverage 수집
lcov --capture --directory . --output-file coverage_raw.info \
  --rc branch_coverage=1 \
  --ignore-errors mismatch,inconsistent

# 타겟 파일 추출
lcov --extract coverage_raw.info '*/lib/target_file.cpp' \
  --output-file coverage_target.info \
  --ignore-errors mismatch,inconsistent,unused

# 요약
lcov --list coverage_target.info --ignore-errors mismatch,inconsistent

# HTML 리포트 (선택)
genhtml coverage_target.info --branch-coverage --output-directory coverage_html
```

### 미커버 분석

```bash
# Line 미커버
gcov -b -c lib/CMakeFiles/dxrt.dir/target_file.cpp.gcno
awk '/^    #####:/ {print NR": "$0}' target_file.cpp.gcov | head -40

# Branch 미커버
grep "branch.*never executed" target_file.cpp.gcov

# 함수별 호출 횟수
grep "^FN\|^FNDA" coverage_target.info
```

### Coverage 기준

| 항목 | 기준 |
|------|------|
| Lines | ≥80% |
| Functions | ≥90% |
| Branches | ≥60% (방어 코드 LCOV_EXCL_BR 제외 후) |

### 정리

```bash
rm -rf /home/orangepi/dx_rt/build_coverage
```

---

## 5. Coverage 개선 파이프라인

```
Step 1: Coverage 측정 (섹션 4)
    ↓
Step 2: 미커버 분석
    - Line: gcov ##### 추출 + FNDA:0 확인
    - Branch: `gcov -b` → "branch never executed" 추출
    ↓
Step 3: 분류
    A. 테스트 추가로 커버 가능:
       - 에러 경로 (Mock 실패 반환으로 진입)
       - 복합 조건식 미테스트 조합
       - 삼항 연산자 한쪽 미실행
       - boundary check, 특정 data type 분기
    B. 제외 마킹 대상:
       - catch(bad_alloc) 방어 코드 → LCOV_EXCL_BR_LINE
       - 수학적 도달 불가 bounds check → LCOV_EXCL_BR_START/STOP
       - #ifdef 다른 빌드 구성 → 해당 빌드에서 별도 측정
    C. Testability 리포트 대상:
       - HW/driver 의존, getenv() 의존
    ↓
Step 4: A는 _error_test.cpp 또는 _edge_test.cpp에 테스트 추가
        B는 소스 코드에 LCOV_EXCL 마킹
        C는 섹션 6에 기록
    ↓
Step 5: ./build.sh → 테스트 실행 → coverage 재측정 → 개선 확인
```

---

## 6. Testability 리포트

### 테스트 불가능 패턴

| 패턴 | 해당 코드 | 이유 |
|------|----------|------|
| HW 의존 | driver 호출, NPU memory 접근 | 실제 장치 없이 실행 불가 |
| `getenv()` 호출 | `bidirectional_transpose` 내부 | 스레드 안전성, 환경 의존 |
| 매직 넘버 | PPU memcpy `128*1024` | bounds check 없는 하드코딩 |

### Branch 제외 마킹 대상

| 패턴 | 마킹 방법 |
|------|----------|
| `catch(bad_alloc)` | `// LCOV_EXCL_BR_LINE` (해당 줄) |
| 도달 불가 bounds check | `// LCOV_EXCL_BR_START` ~ `// LCOV_EXCL_BR_STOP` (블록) |
| `#ifdef USE_IPP/USE_NEON/USE_VNPU` 타 빌드 분기 | 해당 빌드 구성에서 별도 측정 |

### Refactoring 권고

| 이슈 | 위치 | 권고 |
|------|------|------|
| 정수 오버플로우 | `encode`/`decode`의 `(int)row * aligned_col` | cast 전 overflow check 추가 |
| void 리턴으로 에러 숨김 | `bidirectional_transpose` 계열 | int 리턴으로 변경 |
| partial failure → 성공 반환 | `DecodeOutputs`의 `continue` 후 `return 0` | 실패 카운트 추적 |
| output.size 덮어쓰기 | encode/decode에서 `output.size = expected` | 할당 크기 검증 추가 |

---

## 7. Change Log

> AI는 unit test 작업 수행 시마다 행 추가.

| 날짜 | 변경 | 커버리지 영향 |
|------|------|-------------|
| 2025-xx-xx | `npu_format_handler_edge_test.cpp` 생성 (48 tests) | Lines 53.5% (332/620), Fn 85.7% (12/14) |
| 2025-xx-xx | `xnn_kernel_test.cpp` 생성 (aarch64 전용) | x86 측정 불가 |
| 2025-xx-xx | dead code 삭제: `encode_formatted_transposed`, `decode_aligned_transposed` | Lines 46.0% (210/457), Fn 81.8% (9/11) |
| 2025-xx-xx | `npu_format_handler_highlevel_test.cpp` 생성 (31 tests) | **Lines 86.7% (396/457), Fn 100% (11/11)** |