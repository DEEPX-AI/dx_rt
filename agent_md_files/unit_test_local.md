

# Unit Test 개요

## 📌 목적
- `test/unittest/` 파일들을 반복적으로 수정하면서 **coverage 지표를 추적**
- lcov + genhtml을 활용한 **실시간 HTML 리포트 생성 및 분석**
- Code → Test → Measure → Optimize 순환 반복으로 **목표 달성**

## 🎯 대상 Coverage 지표
| 메트릭 | 목표 | 방법 |
|--------|------|------|
| **Lines** | ≥80% | 모든 함수의 주요 경로 테스트 |
| **Functions** | ≥90% | Public API + 중요 Private 함수 테스트 |
| **Branches** | ≥60% | 에러 처리, 조건부 분기 전부 테스트 |

## 🔄 워크플로우
```
1. test/unittest/*.cpp 수정
   ↓
2. build_x86_64에서 빌드 (./build.sh 로 빌드)
   ↓
3. dxrt_test 실행
   ↓
4. lcov + genhtml으로 HTML 리포트 생성
   ↓
5. 현황 분석 → 부족한 테스트 식별
   ↓
6. 1번으로 돌아가기 (반복)
```

## 📁 주요 경로
- **소스 코드**: `lib/` (85개 파일, 26,362줄)
- **테스트 파일**: `test/unittest/` (수정 대상)
- **빌드 디렉토리**: `build_coverage/` (gcov 활성화)

## 🚀 빠른 시작 (반복 사이클)

**반드시 `./build.sh` 사용** (cmake/ninja 직접 호출 금지)

```bash
# Step 1: 빌드
cd /home/sjkim/DevPR/coverage-07/dx_rt_fork
./build.sh

# Step 2: 전체 테스트 (모델 파일 필수, 모두 순차 실행)
# 실행전 clean
lcov --directory . --zerocounters

# 순차 실행
./bin/dxrt_test -m ~/Assets/unittest/v6/YOLOv7_512-YOLOV7-4/YOLOv7_512.dxnn
./bin/dxrt_test -m ~/Assets/unittest/v7/YOLOv7_512-YOLOV7-4/YOLOv7_512.dxnn
./bin/dxrt_test -m ~/Assets/unittest/v8/YOLOv7_512-YOLOV7-4/YOLOv7_512.dxnn

# Step 3: Coverage 리포트 생성
lcov --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1 --gcov-tool /usr/bin/gcov-12
genhtml coverage.info --output-directory coverage_html --rc lcov_branch_coverage=1

# Step 4: HTML 리포트 확인
python3 -m http.server 8000
# 브라우저에서 http://localhost:8000/coverage_html/ 접속
```

**이후 반복**: test/unittest 파일 수정 → Step 1 ~ 4 반복


# 테스트 케이스 작성

## class 코드 제외
개별 class 테스트 케이스 작성이 어려운 class에 대해서는 어려운 이유를 설명
가능한 class에서만 테스트 케이스 작성


# Coverage 확인
## Build & Run

> **주의**: 테스트 실행 시 반드시 `-m <model>.dxnn` 옵션으로 dxnn 모델 파일을 지정해야 함.

```bash
# 빌드 (반드시 build.sh만 사용, cmake/ninja 직접 호출 금지)
cd /home/sjkim/DevPR/coverage-07/dx_rt_fork && ./build.sh


# 전체 테스트 (모델 파일 필수)
 반드시 dxrt_test는 순차적으로 실행 해야함

# v6 model
./bin/dxrt_test -m ~/Assets/unittest/v6/YOLOv7_512-YOLOV7-4/YOLOv7_512.dxnn


# v7 model
./bin/dxrt_test -m ~/Assets/unittest/v7/YOLOv7_512-YOLOV7-4/YOLOv7_512.dxnn

# v8 model
./bin/dxrt_test -m ~/Assets/unittest/v8/YOLOv7_512-YOLOV7-4/YOLOv7_512.dxnn
```

## Coverage 확인 방법

### 제외 코드

```
    **/examples/**,**/python_package/**,**/test/**,**/*test*/**,**/3rdparty/**,**/build*/**,**/*.pb.h,**/*.pb.cc, \
    **/cli/**, \
    **/lib/cli.cpp, **/lib/include/dxrt/cli.h, \
    **/lib/cli_internal.cpp, **/lib/include/dxrt/cli_internal.h, \
    **/lib/inference_job.cpp, **/lib/include/dxrt/inference_job.h, \
    **/lib/device_pool/std_device_task_layer.cpp, \
    **/lib/device_pool/request_response_class.cpp, **/lib/include/dxrt/request_response_class.h, \
    **/lib/dxrt_service/**, \
    **/lib/data/ppcpu_data_gen.py, \
    **/lib/wrapper/ipc_wrapper/**, \
    **/lib/multiprocess_memory.cpp, **/lib/include/dxrt/multiprocess_memory.h, \
    **/lib/include/dxrt/ipc_wrapper/**, \
    **/lib/exception_hander.cpp, **/lib/include/dxrt/exception_hander.h, \
    **/lib/driver_adapter/**, **/lib/include/dxrt/driver_adapter/**, \
    **/lib/testdata.cpp, \
    **/lib/resource/**, \
```

### lcov
lcov --capture --directory . --output-file new_coverage.info --gcov-tool /usr/bin/gcov-12

### genhtml
genhtml new_coverage.info

### http.server
python3 -m http.server 8000