# Profiler - AI Assistant Guide

> ⚠️ **프로젝트 전체 규칙 및 Code Convention은 [general.md](general.md)를 참조하세요.**

---

## Sub-Project Overview

**목적**: DXRT SDK의 성능 측정 및 분석을 위한 프로파일링 도구. 다양한 이벤트의 시작/종료 시간을 기록하고 통계를 제공.

**위치**: `dx_rt/lib/`

**관련 파일**:
- `lib/profiler.cpp` - Profiler 클래스 구현
- `lib/include/dxrt/profiler.h` - Profiler 헤더 및 API 정의

---

## 1. Profiler 동작 방식

### 1.1 싱글톤 패턴

```cpp
Profiler& Profiler::GetInstance()
{
    if (_staticInstance == nullptr) _staticInstance = new Profiler();
    return *_staticInstance;
}
```
- 전역에서 단일 인스턴스로 접근
- `ObjectsPool`을 통해 생명주기 관리

### 1.2 시간 측정 구조

```cpp
struct TimePoint {
    ProfilerClock::time_point start;  // 시작 시간
    ProfilerClock::time_point end;    // 종료 시간
};
```
- `std::chrono::steady_clock` 기반 (ProfilerClock)
- 각 이벤트별로 `numSamples` (기본 10개) 만큼의 TimePoint 배열 유지
- 순환 버퍼 방식으로 최근 N개의 샘플만 보관

### 1.3 데이터 저장 구조

```cpp
std::map<std::string, std::vector<TimePoint>> timePoints;  // 이벤트명 → 시간 데이터
std::map<std::string, int> idx;  // 이벤트명 → 현재 인덱스
```

### 1.4 컴파일 조건

- `USE_PROFILER` 매크로가 정의되어야 프로파일링 활성화
- 비활성화 시 `PROFILER_DEFAULT_SAMPLES = 0`으로 설정되어 오버헤드 최소화

---

## 2. 현재 구현의 문제점

### 2.1 싱글톤으로 인한 멀티 디바이스 데이터 혼합 ✅ 해결됨

**해결:**
- 모든 NPU 관련 이벤트명에 `[Device_X]` 포함
- `Show()` 그룹화 로직을 `EventType[Device_X]` 패턴으로 변경
- `GetPerformanceDataByDevice(int deviceId)` API 추가

```cpp
// 변경 후: 디바이스별 분리
profiler.Start("NPU Task[Device_0][Job_1][model][Req_123]");  // Device 0 명확
profiler.Start("NPU Task[Device_1][Job_2][model][Req_456]");  // Device 1 명확
// → Show()에서 "NPU Task[Device_0]", "NPU Task[Device_1]"로 분리 그룹화
```

### 2.2 메모리 사용량 무한 증가 가능성

```cpp
static const uint64_t MEMORY_PER_EVENT = 350;      // 이벤트당 ~350 bytes
static const uint64_t THRESHOLD_BASE = 100*1024*1024;  // 100MB 경고 기준
```
- 장시간 실행 시 이벤트 수 무한 증가
- `Flush()` 명시적 호출 필요

### 2.3 그룹화 로직의 정보 손실 ✅ 해결됨

**해결:** `Show()`에서 `EventType[Device_X]` 패턴으로 그룹화하도록 변경

```cpp
// Show()에서 regex로 EventType[Device_X] 패턴 추출
std::regex devicePattern("^([^\\[]+\\[Device_\\d+\\])");
// → "NPU Core[Device_0]", "PCIe Write[Device_1]" 등으로 그룹화
// Device가 없는 이벤트는 첫 bracket 앞으로 그룹화
```

### 2.4 스레드 안전성 오버헤드

```cpp
std::unique_lock<std::mutex> lk(_lock);  // 모든 연산에 mutex 사용
```
- 고빈도 프로파일링 시 lock contention 발생 가능

### 2.5 NPU Firmware 시간과 Host 시간 불일치 ✅ 해결됨 (방안 B 적용)

**해결:** PCIe Write 완료 시점을 NPU 시작 시간으로 사용

```cpp
// acc_device_task_layer.cpp - InputHandler에서 PCIe Write 완료 시점 저장
_writeCompleteTimestamps[requestId] = steady_clock::now().time_since_epoch().count();

// OutputHandler에서 NPU Core 시간 = [PCIe Write 완료, PCIe Write 완료 + inf_time]
npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(write_complete_ns));
npu_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(write_complete_ns + inf_time_ns));
```

**결과:** 모든 타임스탬프가 Host `steady_clock` 도메인 내에서 일관됨.
Timeline에서 PCIe Write → NPU Core → PCIe Read 순서가 올바르게 표시됨.

---

## 3. 사용 방법

### 3.1 기본 사용법

```cpp
#include "dxrt/profiler.h"

// 1. 인스턴스 획득
auto& profiler = dxrt::Profiler::GetInstance();

// 2. 이벤트 측정
profiler.Start("my_event");
// ... 측정할 코드 ...
profiler.End("my_event");

// 3. 결과 조회
uint64_t lastDuration = profiler.Get("my_event");      // 최근 값 (us)
double avgDuration = profiler.GetAverage("my_event");  // 평균값 (us)

// 4. 결과 출력/저장
profiler.Show();              // 콘솔 출력
profiler.Save("output.json"); // JSON 파일 저장
```

### 3.2 TimePoint 직접 추가

```cpp
auto tp = std::make_shared<TimePoint>();
tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(start_ns));
tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(end_ns));
profiler.AddTimePoint("event_name", tp);
```

### 3.3 설정 변경

```cpp
// Configuration을 통한 설정
Configuration::SetAttribute(Configuration::ATTRIBUTE::PROFILER_SAVE_DATA, true);
Configuration::SetAttribute(Configuration::ATTRIBUTE::PROFILER_SHOW_DATA, true);
```

---

## 4. 현재 Tracking 중인 성능 지표

### 4.1 NPU Task 전체 (request_response_class.cpp)

| 이벤트 | 설명 |
|--------|------|
| `Buffer Wait[Device_X][Job_Y][task][Req_Z]` | NPU 경로: 버퍼 할당 대기 시간 (AcquireAllBuffers) |
| `NPU Task[Device_X][Job_Y][task][Req_Z]` | NPU 요청의 전체 처리 시간 (input preprocess → PCIe → NPU 실행 → output postprocess) |

### 4.2 CPU Task (request_response_class.cpp, cpu_handle_worker.cpp)

| 이벤트 | 설명 |
|--------|------|
| `Buffer Wait[Job_Y][task][Req_Z]` | CPU 경로: 버퍼 할당 대기 시간 (AcquireAllBuffers) |
| `CPU Task Queue Wait[Job_Y][task][Req_Z]` | CPU Worker 큐 대기 시간 (enqueue → dequeue) |

### 4.3 CPU 추론 실행 (cpu_handle.cpp)

| 이벤트 | 설명 |
|--------|------|
| `CPU[Job_X][model][Req_Y]_tZ` | ONNX Runtime CPU 추론 시간 (Z = processed_id) |
| `PPCPU[Job_X][model][Req_Y]_tZ` | Post-Processing CPU 시간 |

### 4.4 입출력 포맷 변환 (npu_format_handler.cpp)

| 이벤트 | 설명 |
|--------|------|
| `NPU Input Format Handler[Job_X][model][Req_Y](T)` | 입력 인코딩 시간 (T = thread id) |
| `NPU Output Format Handler[Job_X][model][Req_Y](T)` | 출력 디코딩 시간 |

### 4.5 디바이스 I/O - ACC 모드 (acc_device_task_layer.cpp)

| 이벤트 | 설명 |
|--------|------|
| `PCIe Write[Device_X][Job_Y][model][Req_Z](CH)` | PCIe DMA 입력 전송 시간 (CH = DMA 채널) |
| `PCIe Read[Device_X][Job_Y][model][Req_Z](CH)` | PCIe DMA 출력 수신 시간 |
| `NPU Core[Device_X][Job_Y][model][Req_Z]_CH` | NPU 코어 실제 실행 시간 (PCIe Write 완료 기준 + FW inf_time) |
| `Framework Response Handling Delay[Device_X][Job_Y][model][Req_Z]_CH` | 응답 큐잉 지연 시간 |
| `Service Process Wait[Device_X][Job_Y][model][Req_Z]_CH` | 서비스 프로세스 대기 시간 |

### 4.5 디바이스 I/O - STD 모드 (std_device_task_layer.cpp)

| 이벤트 | 설명 |
|--------|------|
| `ThreadImpl Wait[device X]` | 디바이스 응답 대기 시간 |
| `STD Memcpy[device X pickY]` | 입력 버퍼 복사 시간 |
| `STD Write[device X pickY]` | STD 모드 디바이스 쓰기 시간 |

### 4.6 벤치마크 (dxbenchmark.cpp)

| 이벤트 | 설명 |
|--------|------|
| `dxbenchmark_[filename]` | 벤치마크 전체 추론 시간 |
| `benchmark` | inference_engine의 RunBenchmark 시간 |

---

## 5. 멀티 디바이스 문제 해결 방향

### 5.1 현재 문제

```
Device 0: NPU Core 실행 시간 2ms
Device 1: NPU Core 실행 시간 5ms
→ 현재: "NPU Core" avg = 3.5ms (구분 불가)
```

### 5.2 해결 방안 A: 이벤트명에 디바이스 ID 포함 (단기)

**변경 전:**
```cpp
profiler.AddTimePoint("NPU Core[Job_X][model][Req_Y]_CH", npu_tp);
```

**변경 후:**
```cpp
profiler.AddTimePoint("NPU Core[Device_" + std::to_string(core()->id()) + "][Job_X][model][Req_Y]_CH", npu_tp);
```

**Show() 그룹화 수정:**
```cpp
// 현재: "NPU Core" 로 그룹화
// 변경: "NPU Core[Device_0]", "NPU Core[Device_1]" 로 분리

// 첫 번째 bracket까지만 추출하도록 수정
size_t firstBracketEnd = fullName.find(']');
if (firstBracketEnd != string::npos) {
    baseName = fullName.substr(0, firstBracketEnd + 1);
}
```

### 5.3 해결 방안 B: 계층적 데이터 구조 (중기)

```cpp
// 새로운 데이터 구조 제안
struct ProfilerKey {
    std::string event_type;   // "NPU Core", "PCIe Write" 등
    int device_id;            // 0, 1, 2, ...
    int job_id;
    std::string model_name;
    uint32_t request_id;
};

std::map<ProfilerKey, std::vector<TimePoint>> hierarchicalTimePoints;
```

**장점:**
- 다양한 필터링 쿼리 지원 (디바이스별, 모델별, Job별)
- 정보 손실 없음

### 5.4 해결 방안 C: 디바이스별 Profiler 분리 (장기)

```cpp
class ProfilerRegistry {
public:
    static Profiler& GetProfiler(int device_id);
    static std::map<int, Profiler*> GetAllProfilers();
    static void ShowAll();  // 모든 디바이스 결과 통합 출력
};
```

**장점:**
- Lock contention 감소 (디바이스별 독립 mutex)
- 확장성 우수

### 5.5 GetPerformanceData() 수정 제안

```cpp
// 현재 구현
std::map<string, std::vector<int64_t>> GetPerformanceData();

// 개선안: 디바이스 ID 포함
std::map<int, std::map<string, std::vector<int64_t>>> GetPerformanceDataByDevice();
// 또는
std::map<string, std::vector<int64_t>> GetPerformanceData(int device_id = -1);  // -1: 전체
```

### 5.6 구현 우선순위 권장

1. **즉시 적용 가능**: 이벤트명에 Device ID 추가 (방안 A)
2. **Show() 그룹화 로직 수정**: Device 단위 그룹화 지원
3. **GetPerformanceData() 확장**: 디바이스별 필터링 API 추가
4. **장기**: 계층적 구조로 리팩토링 (방안 B/C)

---

## 6. NPU Firmware 시간 오프셋 문제 해결 방안

### 6.1 현재 구현의 한계

현재 코드는 Firmware `inf_time`을 Host 시간 기준으로 역산하여 timeline 배치:
```cpp
// response.wait_end_time: Host가 응답 받은 시간 (steady_clock)
// response.inf_time: FW가 측정한 NPU 실행 시간 (FW 내부 클럭, microseconds)
uint64_t npu_start_ns = response.wait_end_time - inf_time_ns;
```

**문제:** FW 클럭과 Host 클럭이 다르면 `npu_start_ns`가 실제와 다름.

### 6.2 DXRT만으로 해결 가능 여부

**결론: 부분적으로 가능하나, 완벽한 해결은 Firmware 수정 필요**

| 방식 | DXRT만 가능? | 정확도 | 구현 복잡도 |
|------|-------------|--------|------------|
| A. 상대 duration만 사용 | ✅ Yes | 낮음 | 낮음 |
| B. 요청/응답 시점 기준 추정 | ✅ Yes | 중간 | 중간 |
| C. 주기적 시간 동기화 | ⚠️ FW 지원 필요 | 높음 | 높음 |
| D. FW 절대 타임스탬프 | ❌ FW 수정 필요 | 매우 높음 | 높음 |

### 6.3 해결 방안 A: 상대 Duration 기반 배치 (DXRT만 수정)

**원리:** 절대 시간 대신 "응답 수신 직전 N us 동안 NPU 실행"으로 간주

**현재 코드 (이미 일부 적용됨):**
```cpp
// Service가 꺼진 경우: 응답 수신 시점 기준 역산
npu_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns));
npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns - inf_time_ns));
```

**개선안:** 모든 케이스에서 일관되게 적용
```cpp
// NPU Core 시간 = 응답 수신 직전 inf_time 구간으로 배치
// → PCIe Read 시작 전에 NPU 실행이 끝났다고 가정
auto npu_tp = std::make_shared<TimePoint>();
npu_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns));
npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns - inf_time_ns));
```

**장점:** 간단, 추가 인프라 불필요
**단점:** NPU 실제 실행 시점이 아닌 추정치

### 6.4 해결 방안 B: PCIe Write 완료 시점 기준 배치 (DXRT만 수정)

**원리:** "PCIe Write 완료 후 NPU 시작"이라는 물리적 사실 활용

```cpp
// 1. PCIe Write 완료 시점 기록 (이미 profiler에 있음)
profiler.End("PCIe Write[...]");
uint64_t pcie_write_end_ns = ProfilerClock::now().time_since_epoch().count();

// 2. NPU Core 시간 배치
auto npu_tp = std::make_shared<TimePoint>();
npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(pcie_write_end_ns));
npu_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(pcie_write_end_ns + inf_time_ns));
```

**장점:** 물리적으로 더 정확한 timeline
**단점:** PCIe Write 완료 → NPU 시작 사이 지연 무시

### 6.5 해결 방안 C: Host-NPU 시간 오프셋 계산 (Firmware 지원 필요)

**원리:** 주기적으로 Host-NPU 클럭 오프셋 측정

**필요한 Firmware 지원:**
```cpp
// 새로운 Driver 명령 추가
typedef enum {
    ...
    DXRT_CMD_GET_NPU_TIMESTAMP,  // FW 현재 타임스탬프 요청
    ...
} dxrt_cmd_t;

// FW 응답 구조체 확장
typedef struct _dxrt_response_t {
    ...
    uint64_t  npu_timestamp = 0;  // FW 내부 클럭의 현재 값 (nanoseconds)
    ...
} dxrt_response_t;
```

**DXRT 구현:**
```cpp
class NpuTimeSynchronizer {
public:
    // 주기적으로 호출하여 오프셋 업데이트
    void Synchronize(int device_id) {
        auto host_before = ProfilerClock::now();
        uint64_t npu_time = core->GetNpuTimestamp();  // FW에 요청
        auto host_after = ProfilerClock::now();
        
        // RTT/2 보정
        auto host_mid = host_before + (host_after - host_before) / 2;
        uint64_t host_mid_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            host_mid.time_since_epoch()).count();
        
        _offset[device_id] = static_cast<int64_t>(host_mid_ns) - static_cast<int64_t>(npu_time);
    }
    
    // NPU 시간 → Host 시간 변환
    uint64_t ToHostTime(int device_id, uint64_t npu_time_ns) {
        return npu_time_ns + _offset[device_id];
    }
    
private:
    std::map<int, int64_t> _offset;  // device_id → offset (host - npu)
};
```

**Timeline 배치에 적용:**
```cpp
// FW에서 npu_start_timestamp, npu_end_timestamp 제공 시
uint64_t host_npu_start = synchronizer.ToHostTime(device_id, response.npu_start_timestamp);
uint64_t host_npu_end = synchronizer.ToHostTime(device_id, response.npu_end_timestamp);
```

### 6.6 권장 구현 순서

1. **즉시 (DXRT만):** 방안 A 또는 B 적용 - `response_recv_ns` 기준 일관된 역산
2. **중기:** Firmware에 `DXRT_CMD_GET_NPU_TIMESTAMP` 명령 추가 요청
3. **장기:** `NpuTimeSynchronizer` 구현 및 주기적 동기화

### 6.7 구현 예제 (방안 B - DXRT만 수정)

```cpp
// acc_device_task_layer.cpp 수정

// InferenceRequest에서 PCIe Write 완료 시점 저장
#ifdef USE_PROFILER
    profiler.End("PCIe Write[...]");
    {
        std::lock_guard<std::mutex> lock(_writeTimestampLock);
        _writeCompleteTimestamps[requestId] = std::chrono::duration_cast<std::chrono::nanoseconds>(
            ProfilerClock::now().time_since_epoch()).count();
    }
#endif

// OutputHandler에서 활용
#ifdef USE_PROFILER
    uint64_t write_complete_ns = 0;
    {
        std::lock_guard<std::mutex> lock(_writeTimestampLock);
        auto it = _writeCompleteTimestamps.find(reqId);
        if (it != _writeCompleteTimestamps.end()) {
            write_complete_ns = it->second;
            _writeCompleteTimestamps.erase(it);
        }
    }
    
    if (write_complete_ns > 0) {
        // PCIe Write 완료 후 NPU 시작으로 가정
        uint64_t inf_time_ns = static_cast<uint64_t>(response.inf_time) * 1000;
        auto npu_tp = std::make_shared<TimePoint>();
        npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(write_complete_ns));
        npu_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(write_complete_ns + inf_time_ns));
        profiler.AddTimePoint("NPU Core[...]", npu_tp);
    }
#endif
```

---

## Files to Track

| 파일 | 역할 | 상태 |
|------|------|------|
| `lib/profiler.cpp` | Profiler 핵심 구현 | ✅ Show() 그룹화 + GetPerformanceDataByDevice() 구현 |
| `lib/include/dxrt/profiler.h` | API 정의 | ✅ GetPerformanceDataByDevice() 선언 추가 |
| `lib/device_pool/acc_device_task_layer.cpp` | ACC 모드 프로파일링 | ✅ Device ID, NPU 시간 오프셋 수정 완료 |
| `lib/device_pool/request_response_class.cpp` | NPU/CPU Task 프로파일링 | ✅ Device ID + Buffer Wait 이벤트 추가 |
| `lib/include/dxrt/device_task_layer.h` | PCIe Write 타임스탬프 저장 | ✅ _writeCompleteTimestamps 추가 |
| `lib/cpu_handle_worker.cpp` | CPU Worker 큐 대기 프로파일링 | ✅ CPU Task Queue Wait 이벤트 추가 |
| `lib/device_pool/std_device_task_layer.cpp` | STD 모드 프로파일링 | - |
| `lib/npu_format_handler.cpp` | I/O 포맷 변환 프로파일링 | - |
| `lib/cpu_handle.cpp` | CPU 추론 프로파일링 | - |

---

## Sub-Project Specific Notes

### 빌드 설정

```cmake
# USE_PROFILER 매크로 활성화 필요
add_definitions(-DUSE_PROFILER)
```

### 성능 영향

- 프로파일링 활성화 시 약간의 오버헤드 발생
- Production 환경에서는 `USE_PROFILER` 비활성화 권장
- `PROFILER_FORCE_SHOW_DURATIONS = 1`로 상세 출력 강제

### 메모리 관리

- 장시간 실행 시 `Flush()` 주기적 호출 권장
- 100MB 단위로 메모리 사용량 경고 로그 출력

---

## References

- [profiler.h](../lib/include/dxrt/profiler.h) - API 정의
- [profiler.cpp](../lib/profiler.cpp) - 구현 코드
- [04_Model_Inference.md](../docs/source/docs/04_Model_Inference.md) - 사용 가이드
