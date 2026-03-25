# Profiler Visualizer - AI Assistant Guide

> ⚠️ **프로젝트 전체 규칙 및 Code Convention은 [general.md](general.md)를 참조하세요.**

---

## Sub-Project Overview

**목적**: DX-RT profiler.json 데이터를 인터랙티브 타임라인으로 시각화하는 웹 애플리케이션. 파이프라인 병목 분석, 스테이지 전환 지연 통계, 멀티디바이스 비교 등을 제공.

**위치**: `/home/hylee/dxrt_profiler_viewer/` (dx_rt 프로젝트 외부 독립 프로젝트)

**관련 파일**:
- `app.py` — Dash 웹 애플리케이션 (~1,425 lines)
- `parse.py` — profiler.json 파서 및 분석 엔진 (~673 lines)
- `requirements.txt` — Python 의존성 (dash, plotly, pandas)

**이식 시 필요한 파일**: `app.py`, `parse.py`, `requirements.txt` 3개만 복사하면 됨.

---

## 1. 실행 방법

```bash
# 의존성 설치
pip install -r requirements.txt

# 단일 파일 모드
python3 app.py -i profiler.json

# 디렉토리 모드 (여러 json 파일 비교)
python3 app.py -d /path/to/profiler/dir

# 서버 모드 (웹에서 업로드/관리)
python3 app.py -s
python3 app.py -s --upload-dir /data --max-cache 50

# 포트 변경
python3 app.py -s -p 8051
```

CLI 옵션:
| 옵션 | 설명 | 기본값 |
|------|------|--------|
| `-i` / `--input` | 단일 profiler.json 경로 | — |
| `-d` / `--dir` | JSON 파일이 담긴 디렉토리 | — |
| `-s` / `--server` | 서버 모드 (업로드/삭제 UI) | — |
| `-p` / `--port` | 서버 포트 | 8050 |
| `--host` | 서버 호스트 | 0.0.0.0 |
| `--upload-dir` | 업로드 파일 저장 경로 | `./uploads/` |
| `--max-cache` | LRU 캐시에 유지할 최대 파싱 파일 수 | 20 |

`-i`, `-d`, `-s`는 상호 배타적 (mutually exclusive).

---

## 2. profiler.json 입력 형식

DX-RT Profiler가 생성하는 JSON. 구조:

```json
{
  "EventType[Device_X][Job_Y][subgraph][Req_Z]_suffix": [
    {"start": <nanosecond_timestamp>, "end": <nanosecond_timestamp>},
    ...
  ],
  ...
}
```

### 2.1 이벤트 이름 패턴

`parse_event_name()` 함수가 파싱. 주요 패턴:

| 패턴 | 예시 |
|------|------|
| NPU 스테이지 | `PCIe Write[Device_0][Job_10][npu_0][Req_5]_ch0` |
| CPU 스테이지 | `cpu_0[Job_10][cpu_0][Req_5]_t0` |
| Buffer Wait | `Buffer Wait[Device_0][Job_10][npu_0][Req_5]` |
| NPU Task | `NPU Task[Device_0][Job_10][npu_0][Req_5]` |
| CPU Task Queue Wait | `CPU Task Queue Wait[Job_10][cpu_0][Req_5]` |

**주의**: CPU 이벤트에는 `[Device_X]`가 없음. Job → Device 매핑은 NPU 이벤트에서 추론하여 CPU에 역매핑.

### 2.2 제외되는 이벤트

```python
EXCLUDED_EVENTS = {"Framework Response Handling Delay", "Service Process Wait"}
```

---

## 3. 아키텍처

### 3.1 parse.py — 데이터 파서

#### 상수

```python
NPU_SUBGRAPH_INNER_ORDER = [
    "Buffer Wait", "NPU Input Format Handler", "PCIe Write",
    "NPU Core", "PCIe Read", "NPU Output Format Handler",
]
CPU_SUBGRAPH_INNER_ORDER = ["Buffer Wait", "CPU Task Queue Wait"]
TAIL_EVENTS = {"NPU Task"}           # 서브그래프 prefix 없는 이벤트
LANE_SPLIT_EVENTS = {"NPU Task", "CPU Task Queue Wait"}  # 겹침 분할 대상
```

#### 주요 함수

| 함수 | 역할 |
|------|------|
| `parse_event_name(name)` | 이벤트 이름 → `{event_type, device_id, job_id, subgraph, req_id, suffix}` dict |
| `load_profiler(filepath)` | JSON 로드 → 파싱 → DataFrame + 메타데이터 반환 |
| `compute_utilization(df, device_id)` | 디바이스별 NPU 스테이지 Saturation/Avg Util 계산 |
| `compute_shared_utilization(df)` | 전체 디바이스 합산 CPU 스테이지 utilization |
| `compute_transition_latency(flow_data, ...)` | 파이프라인 스테이지 간 전환 지연 통계 |
| `format_duration(ns)` | 나노초 → 사람이 읽을 수 있는 문자열 |

#### `load_profiler()` 반환 구조

```python
{
    "df": pd.DataFrame,          # 모든 이벤트 데이터
    "track_order": {int: [str]}, # 디바이스별 트랙 이름 순서 (파이프라인 순)
    "device_ids": [int],
    "job_ids": [int],
    "job_colors": {int: str},    # 6색 팔레트 순환
    "job_to_device": {int: int}, # Job ID → Device ID 매핑
    "flow_data": {int: [dict]},  # Job별 flow arrow 데이터
    "global_start_ns": int,
    "subgraph_order": [str],     # e.g. ["npu_0", "cpu_0", "cpu_1"]
    "filepath": str,
}
```

#### DataFrame 컬럼

```
event_type, device_id, job_id, subgraph, req_id, suffix,
start_ns, end_ns, duration_ns, start_us, end_us, duration_us,
track, sort_index, color, hover
```

#### Lane Splitting (겹침 해소)

NPU Task와 CPU Task Queue Wait은 동일 디바이스에서 겹칠 수 있음. Greedy interval coloring 알고리즘으로 겹치지 않는 lane으로 분할.

**핵심 로직**:
- `TAIL_EVENTS` (NPU Task): 디바이스 단위로 그룹화, 서브그래프 prefix 없음 → `NPU Task (lane 0)`
- 비-TAIL 이벤트 (CPU Task Queue Wait): **(디바이스, 서브그래프)** 단위로 그룹화, 서브그래프 prefix 있음 → `cpu_0 / CPU Task Queue Wait (lane 0)`, `cpu_1 / CPU Task Queue Wait (lane 0)`

**주의**: 이전에 비-TAIL 이벤트도 디바이스 단위로만 그룹화하여 `cpu_1` 이벤트가 `cpu_0 /` prefix를 갖는 버그가 있었음. 반드시 서브그래프별로 분리해야 함.

Flow data는 lane splitting **이후**에 생성되어야 track 이름이 최종 이름과 일치함.

#### Utilization 계산

- `compute_utilization()`: NPU 서브그래프 스테이지만 (CPU 스테이지는 per-device에서 제외)
- `compute_shared_utilization()`: CPU 스테이지를 전체 디바이스 합산으로 계산
  - 이유: CPU Task는 디바이스별 독립 큐이지만 **물리적 CPU 코어를 공유**하므로 per-device 수치가 낮아 보여도 합산하면 ~100%에 가까움

### 3.2 app.py — Dash 웹 애플리케이션

#### Thread-safe LRU 캐시

```python
class _LRUCache:  # OrderedDict 기반, _cache_lock으로 보호
```

기존 단순 dict `DATA_CACHE` 대신 LRU 캐시 사용. `--max-cache`로 용량 조절.

#### 모드별 동작

| 모드 | 파일 관리 | Upload UI | 파일 목록 |
|------|-----------|-----------|-----------|
| `-i` (단일파일) | CLI에서 지정 | 숨김 | 파일 1개, 바 숨김 |
| `-d` (디렉토리) | 디렉토리 스캔 | 숨김 | 정적 목록 |
| `-s` (서버) | `uploads/` 디렉토리 | Drag & Drop + 클릭 | 동적 (업로드/삭제 시 갱신) |

#### 서버 모드 기능

1. **파일 업로드**:
   - `dcc.Upload` 컴포넌트 (drag & drop, 다중 파일 지원)
   - `POST /api/upload` Flask endpoint (curl 등으로 대용량 업로드)
   - 파일명: `{uuid8}_{sanitized_original_name}.json`
   - JSON 유효성 검증 후 저장

2. **파일 관리**:
   - `uploads/.meta.json`에 원본 파일명, 업로드 시각 저장
   - 파일 목록 테이블 (이름, 크기, 시각) + 🗑️ Delete 버튼
   - 🔄 Refresh 버튼으로 수동 갱신

3. **파일 삭제**:
   - 파일 삭제 + `DATA_CACHE.remove()` + 메타데이터 정리
   - 삭제된 파일을 보고 있던 유저에게 "⚠️ File has been deleted" 표시

4. **멀티유저 안전성**:
   - 유저별 상태(선택 파일, Job, Device)는 브라우저 측 `dcc.Store`/콜백에 격리
   - `DATA_CACHE`는 read-only 공유 (같은 파일을 여러 유저가 볼 때 효율적)
   - `threading.Lock`으로 캐시 + 메타데이터 접근 보호

#### 콜백 구조

| 콜백 | Input | Output | 설명 |
|------|-------|--------|------|
| `on_upload` | upload contents | upload-status, refresh-trigger | 업로드 처리 (서버 모드) |
| `refresh_file_list` | refresh-trigger, refresh btn | file-selector options, file-count, manager-list | 파일 목록 갱신 (서버 모드) |
| `on_delete_file` | delete-file-btn (pattern matching) | refresh-trigger | 파일 삭제 (서버 모드) |
| `on_file_change` | file-selector | file-info, device/job controls | 파일 전환 시 컨트롤 초기화 |
| `on_range_input` | job-range-input | job-selector value | 범위 텍스트 → Job 드롭다운 |
| `on_dropdown_change` | job-selector | job-range-input | 드롭다운 → 범위 텍스트 (압축) |
| `on_bar_click` | timeline clickData | job-selector value | 바 클릭 → Job 토글 |
| `update_view` | device-filter, job-selector | figure, stats, util, transitions, job-info | 메인 뷰 갱신 |

#### UI 섹션 (위에서 아래 순서)

1. **Upload Panel** (서버 모드만): 업로드 + 파일 관리
2. **File Selector Bar**: 파일 드롭다운 + 파일 수 + Refresh
3. **Header**: 타이틀 + 파일 정보 (이벤트 수, Job 수, 디바이스, 파이프라인)
4. **Controls**: Device 체크리스트 + Job 범위 입력 + Job 드롭다운
5. **Timeline**: Plotly 인터랙티브 차트 (scroll=zoom, drag=pan)
6. **Pipeline Statistics**: 스테이지별 Count/Avg/Min/Max/Median (per-device 또는 selected jobs)
7. **Pipeline Bottleneck Analysis**: Saturation/Avg Util 테이블 + Shared CPU 테이블
8. **Stage Transition Latency**: 스테이지 간 전환 지연 통계

---

## 4. 핵심 설계 결정 및 근거

### 4.1 CPU Utilization을 별도 "Shared CPU" 테이블로 분리

- **문제**: CPU Task는 디바이스별 독립 큐이지만 물리적 CPU를 공유. Per-device로 보면 cpu_0 t0이 Device 0에서 48.9%, Device 1에서 49.9%로 낮아 보이나, 합산하면 98.8%로 saturated.
- **해결**: Per-device 테이블에서는 NPU 스테이지만 표시. CPU 스테이지는 "Shared CPU Resources — All Devices" 별도 테이블.

### 4.2 Lane Splitting을 (디바이스, 서브그래프) 단위로

- **문제**: CPU_0 → NPU_0 → CPU_1 파이프라인에서 CPU Task Queue Wait을 디바이스 단위로만 분할하면, `cpu_1` 이벤트에 `cpu_0 /` prefix가 붙음 → Plotly가 같은 y축에 배치 → flow arrow 방향 오류.
- **해결**: 비-TAIL 이벤트는 `(device_id, subgraph)` 단위로 greedy coloring.

### 4.3 Flow Data를 Lane Splitting 이후 생성

- **이유**: Flow arrow의 `track_from`/`track_to`가 최종 트랙 이름(lane 포함)이어야 Plotly에서 올바른 y좌표에 화살표 배치.

### 4.4 `dragmode="pan"` 기본값

- **이유**: 사용자가 zoom과 pan을 동시에 사용. `pan` 모드에서 스크롤=줌, 드래그=팬이 가장 자연스러움.

### 4.5 Job Range Input 양방향 동기화

- 텍스트 입력 `200-219, 500` → 드롭다운 업데이트
- 드롭다운 변경 → 연속 ID를 범위 표기로 압축하여 텍스트 갱신

---

## 5. 알려진 동작 / FAQ

### Q: Job 200 NPU Task와 CPU Task의 flow arrow가 연결이 안 되는 것처럼 보임
**A**: 정상. CPU가 saturated(~99%)인 경우 NPU가 끝나도 CPU에 큐잉되어 20ms+ 후에 실행될 수 있음. 이때 arrow가 시각적으로 매우 먼 곳을 가리키거나, 다른 Job의 CPU 실행 중에 해당 Job이 대기하므로 "엉뚱한 곳"을 가리키는 것처럼 보일 수 있음.

### Q: `_hover()` 함수에서 NaN 크래시
**A**: CPU 이벤트의 `device_id`가 NaN일 수 있음 (Job이 아직 NPU 이벤트와 매핑되지 않은 상태). `pd.notna(row["device_id"])` 가드 추가됨.

### Q: 서버 모드에서 파일을 업로드했는데 목록에 안 나옴
**A**: 업로드 시 `refresh-trigger`가 자동으로 증가하여 파일 목록이 갱신됨. 다른 브라우저 탭에서는 🔄 Refresh 버튼 클릭 필요.

---

## 6. 테스트 데이터

- `/home/hylee/dx_rt/test/release/performance_test/profiler.json` — 1.5MB, 13,000 events, 1000 jobs, 2 devices, 파이프라인: npu_0 → cpu_0 → cpu_1
- 원래 사용하던 `/home/hylee/dx_rt/profiler.json`은 삭제됨

---

## 7. 수정 시 주의사항

1. **Lane splitting 순서**: `load_profiler()`에서 lane splitting → flow data 생성 순서를 절대 바꾸지 말 것.
2. **비-TAIL 이벤트의 서브그래프 prefix**: `LANE_SPLIT_EVENTS`에 새 이벤트를 추가할 때 TAIL인지 아닌지 반드시 확인.
3. **`suppress_callback_exceptions=True`**: 서버 모드의 pattern-matching 콜백 (`{"type": "delete-file-btn", "index": ALL}`) 때문에 필요.
4. **`DATA_CACHE`는 `_LRUCache` 인스턴스**: 기존 dict처럼 `DATA_CACHE[key]`로 접근하지 말고 `.get()`, `.put()`, `.remove()` 사용.
5. **CPU 이벤트에는 Device ID가 없음**: `job_to_device` 매핑으로 추론. 새 이벤트 타입 추가 시 이 로직 확인.
