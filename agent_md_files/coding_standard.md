# DX_RT Coding Standard

이 문서는 DX_RT 코드 작성 시 기본으로 준수해야 할 네이밍/문체/흐름 규칙을 정의한다.

---

## 1. Naming Convention

기본 원칙:
- C++: Camel Case 중심
- Python: Snake Case 중심
- 약어는 의미가 명확하도록 일관성 있게 표기

### 1.1 Camel Case

단어를 공백 없이 연결하고, 단어 경계에서 대문자를 사용한다.

#### Lower Camel Case

첫 단어는 소문자로 시작하고, 이후 단어의 첫 글자는 대문자로 표기한다.

적용 대상:
- C++ class private member function
- C++ function parameter
- C++ class private member variable (prefix `_`와 함께 사용)

예시:
```cpp
// private member variable
TaskInfo _taskInfo;
RequestData _requestData;

// private member function
TaskInfo getTaskInfo() const;
RequestData getRequestData() const;

// function parameter
void SetTaskInfo(const TaskInfo& taskInfo, const RequestData& requestData);
```

#### Upper Camel Case (Pascal Case)

모든 단어의 첫 글자를 대문자로 표기한다.

적용 대상:
- C++ class/interface name
- C++ class/interface public member function

예시:
```cpp
class TaskInfo
{
public:
    RequestData GetRequestData() const;
};
```

### 1.2 Snake Case

단어를 `_`로 구분한다.

#### Lower Snake Case

모든 글자를 소문자로 작성한다.

적용 대상:
- Python 변수명
- Python 함수명
- C++ local 변수명

예시:
```python
def get_task_info(request_data):
    task_info = parse_task_info(request_data)
    return task_info
```

```cpp
void TaskRunner::RunTask()
{
    int request_count = 0;
}
```

#### Upper Snake Case

모든 글자를 대문자로 작성한다.

적용 대상:
- C++ 상수
- Python 상수

예시:
```cpp
constexpr int TASK_INFO = 1;
constexpr int REQUEST_DATA = 2;
```

```python
TASK_INFO = "task_info"
REQUEST_DATA = "request_data"
```

### 1.3 약어 표기 규칙

원칙:
- 약어 단독/끝 단어: 대문자 유지 가능
- 약어 뒤에 후속 단어가 이어지면 단어 경계에 맞춰 첫 글자만 대문자 사용

예시:
- `useORT`
- `IsPPU()`
- `GetNpuInferenceTime()`
- `GetNpuInferenceTimeCnt()`

주의:
- 동일 의미의 약어는 파일/모듈 전반에서 동일한 형태를 유지한다.
- `Npu`와 `NPU`를 혼용하지 않는다(프로젝트 합의에 맞춰 일관 적용).

---

## 2. Indentation & Bracket Style

기본 스타일:
- Allman Style 사용
- 중괄호 `{`는 항상 다음 줄에 배치
- indent는 스페이스 4칸 사용 (tab 금지)

예시:
```cpp
while (condition)
{
    Process();
}

do
{
    Process();
} while (condition);

for (;;)
{
    Process();
}
```

---

## 3. If / Else Rules

원칙:
- 모든 `if`, `else if`, `else`는 라인 수와 무관하게 중괄호를 사용한다.
- 모든 `if`, `else if` 구문에는 반드시 `else`를 추가한다.
- 설계상 `else`가 필요 없는 경우에는 사유를 주석으로 명시한다.

예시 1: `else`가 필요 없는 경우
```cpp
if (condition)
{
    Process();
} // @no_else: guard clause로 조건 불만족 시 즉시 반환되어 else가 불필요
```

예시 2: 일반적인 if-else
```cpp
if (condition)
{
    ProcessA();
}
else
{
    ProcessB();
}
```

예시 3: if-else if-else
```cpp
if (condition_a)
{
    ProcessA();
}
else if (condition_b)
{
    ProcessB();
}
else
{
    ProcessDefault();
}
```

---

## 4. Early Return Pattern

원칙:
- `else` 사용을 최소화한다.
- main logic 내부의 깊은 `if` 중첩을 줄인다.
- guard clause(early return)로 흐름을 평탄화한다.

권장 예시:
```cpp
Status Processor::Run(const RequestData& requestData)
{
    if (!IsValid(requestData))
    {
        return Status::InvalidArgument;
    }

    if (!IsReady())
    {
        return Status::NotReady;
    }

    return Execute(requestData);
}
```

비권장 예시:
```cpp
Status Processor::Run(const RequestData& requestData)
{
    if (IsValid(requestData))
    {
        if (IsReady())
        {
            return Execute(requestData);
        }
        else
        {
            return Status::NotReady;
        }
    }
    else
    {
        return Status::InvalidArgument;
    }
}
```

---

## 5. Quick Checklist

코드 작성/리뷰 시 아래 항목을 확인한다.

- [ ] C++ Camel Case / Python Snake Case를 준수했는가?
- [ ] C++ private member variable에 `_` prefix를 사용했는가?
- [ ] Allman Style과 4-space indent를 지켰는가?
- [ ] 모든 `if`/`else if`/`else`에 중괄호를 적용했는가?
- [ ] `else`가 없으면 `@no_else` 코멘트로 사유를 남겼는가?
- [ ] guard clause로 early return을 우선 적용했는가?
