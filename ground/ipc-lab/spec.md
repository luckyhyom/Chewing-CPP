# ipc-lab 설계 (spec)

작성일: 2026-06-03

이 문서는 **무엇을, 왜** 만드는지를 정의한다. 구현 방법은 [plan.md](plan.md), 진행 상태는 [tasks.md](tasks.md)에 있다.

## 1. 목적

두 개의 독립된 C++ 프로세스가 **파일 + IPC 알림** 패턴으로 통신하는 과정을, 아주 단순한 것부터 점진적으로 쌓아 올리며 학습한다.

`ground/LEARNING_PLAN.md`의 DPM ↔ VDC 통신 설계를, **샘플 프로젝트(`ground/vcrm_x86_sample`)와 완전히 독립된** 작은 실험장에서 직접 구현해 검증하는 것이 목표다.

전체 VCRM을 구현하는 것이 아니라, "두 프로세스가 안전하게 Policy를 주고받는다"는 한 가지 본질에 집중한다.

## 2. 등장 인물

```text
DPM (A 프로세스) = Policy의 소유자
  - 신호를 받고
  - Policy 정보를 얻어
  - JSON 파일을 만들고
  - VDC에게 "이 파일을 적용하라"고 IPC로 알린다

VDC (B 프로세스) = Policy의 적용자
  - IPC 알림을 받고
  - 파일을 읽어 무결성을 검증하고
  - Policy를 적용한 뒤
  - DPM에게 결과(ACK/Reject)를 회신한다
```

두 프로세스는 모두 C++로 작성하며, **역할만 다른 별도 실행 파일**이다(`vcrm_dpm`, `vcrm_vdc`).

## 3. 핵심 설계 원칙

### 3.1 본문은 파일, 신호는 IPC

Policy 본문(JSON)은 커질 수 있고 checksum·version 관리가 붙는다. 이런 데이터를 IPC 메시지 하나에 전부 싣지 않는다.

```text
무거운 데이터(Policy JSON) → 파일로
가벼운 신호("이 파일 적용해") → IPC로
```

DPM은 검증된 파일을 디스크에 저장하고, IPC로는 "어떤 파일을, 어떤 checksum으로 적용하라"는 작은 명령만 보낸다.

### 3.2 메모리를 공유하지 않는다

두 프로세스는 서로 다른 주소 공간을 가진다. 한쪽의 C++ 객체 포인터를 다른 쪽이 직접 쓸 수 없다.

`common/`을 공유하는 것은 **컴파일 타임에 같은 코드(프레이밍 규칙, 메시지 구조체 모양)를 양쪽에 박는 것**일 뿐, 런타임 메모리 공유가 아니다. 둘은 오직 socket의 바이트 스트림으로만 만난다. 따라서 직렬화(length-prefixed JSON)는 반드시 필요하다.

### 3.3 Mock-first (안에서 바깥으로)

IPC 척추를 먼저 세우고, 외부 의존성(MQTT 신호, HTTP 다운로드)은 가짜로 시작해 나중에 진짜로 교체한다.

```text
"MQTT 신호 수신"  → 처음엔 수동 입력/타이머 → 나중에 Paho MQTT
"HTTP 다운로드"   → 처음엔 로컬 파일 생성   → 나중에 libcurl
```

이렇게 하면 학습의 대부분(통신 본질)을 외부 인프라 설치 없이 끝낼 수 있다.

## 4. 통신 설계

### 4.1 전송 계층

같은 장비 안의 두 프로세스이므로 **Unix Domain Socket**을 사용한다. TCP보다 가볍고 파일 권한으로 접근 제어가 된다.

- 소켓 경로: `/tmp/vcrm_vdc.sock`
- VDC가 서버(소켓 생성·대기), DPM이 클라이언트(접속·전송)

### 4.2 메시지 프레이밍

`read()`는 한 번에 메시지 전체를 보장하지 않는다(짧은 read 가능). 따라서 길이 prefix를 붙인다.

```text
[4바이트 길이 (binary, little-endian)][JSON payload]
```

수신측은 먼저 4바이트를 읽어 payload 길이를 알고, 그 길이만큼 정확히 다시 읽는다.

### 4.3 메시지 포맷

**DPM → VDC : ApplyPolicy**

```json
{
  "messageType": "ApplyPolicy",
  "policyType": 0,
  "policyVersion": 2,
  "path": "/tmp/vcrm/policy/persistent.json",
  "sha256": "abc...",
  "applyImmediately": true
}
```

VDC는 `path`의 파일을 읽고 직접 SHA256을 계산해 메시지의 `sha256`과 비교한다. 그래야 "DPM이 검증한 바로 그 파일"을 적용하는지 보장된다.

**VDC → DPM : ApplyPolicyResult**

성공:

```json
{ "messageType": "ApplyPolicyResult", "policyType": 0, "policyVersion": 2, "status": "Applied" }
```

실패:

```json
{ "messageType": "ApplyPolicyResult", "policyType": 0, "policyVersion": 2, "status": "Rejected", "reason": "checksum mismatch" }
```

## 5. 디렉토리 구조

```text
ipc-lab/
├── CMakeLists.txt
├── README.md              # 각 마일스톤 실행 레시피 기록
├── spec.md                # 이 문서 (WHAT/WHY)
├── plan.md                # 마일스톤별 구현 방법 (HOW)
├── tasks.md               # 체크리스트 (STATUS)
├── common/                # 두 프로세스가 공유하는 작은 라이브러리
│   ├── frame.hpp/cpp      # length-prefixed 송수신
│   └── message.hpp        # ApplyPolicy / ApplyPolicyResult 구조체
├── dpm/
│   └── main.cpp           # A: 신호 → policy 생성 → IPC 알림
├── vdc/
│   └── main.cpp           # B: IPC 대기 → 파일 읽기 → 적용 → 회신
└── tests/
```

## 6. 마일스톤 사다리

각 마일스톤은 **독립적으로 실행·커밋 가능한 작은 프로그램**이다. 항상 `cmake --build && 실행`이 동작하는 상태를 유지한다.

| #  | 마일스톤 | 새로 배우는 것 | 외부 의존 |
|----|----------|----------------|:---------:|
| M0 | Walking skeleton: VDC 대기 → DPM이 `"hello"` 전송 → VDC 출력 | Unix Domain Socket 연결 | 없음 |
| M1 | 길이 프레이밍 `[4B len][payload]` | read 경계 문제 해결 | 없음 |
| M2 | JSON 메시지: 하드코딩 ApplyPolicy 전송·파싱 | nlohmann/json | 없음 |
| M3 | 파일+IPC 분리: DPM이 파일 저장 후 `{path, sha256}`만 전송 | 핵심 패턴 | 없음 |
| M4 | checksum 검증: VDC가 파일 SHA256 계산·비교 | 무결성 | OpenSSL |
| M5 | ACK/Reject 응답: VDC 회신, DPM 수신 | 양방향 통신 | 없음 |
| M6 | 적용 + hot swap: VDC가 현재 policy 상태 교체, Idle/Delete | 런타임 상태 교체 | 없음 |
| M7 | 진짜 HTTP: DPM이 로컬 HTTP에서 libcurl로 다운로드 | HTTP 다운로드 | libcurl + 로컬 서버 |
| M8 | 진짜 MQTT: DPM이 토픽 구독 → 신호 시 다운로드→알림 | MQTT 구독 | Paho + mosquitto |
| M9 | (선택) protobuf 저장 / spdlog / libpcap | 포맷·로깅 확장 | 추가 라이브러리 |

**M8까지 완성되면** 최초 목표 흐름이 완성된다:

```text
MQTT 신호(M8) → HTTP 다운로드(M7) → JSON 파일 생성(M3)
            → IPC 알림(M5) → VDC가 읽어 적용(M6)
```

> **전체 VCRM 메커니즘까지 확장하는 방향(M10~M20)** 은 별도 문서 [spec-extended.md](spec-extended.md)에 정리되어 있다. 지금 구현 범위는 M0~M9이며, 필요해지면 확장판을 꺼내 plan/tasks로 펼친다.

## 7. 범위 밖 (Out of Scope)

- 실제 차량 데이터 수집 파이프라인(Receiver~Transmission 6레이어) — [spec-extended.md](spec-extended.md)의 M10+에서 다룬다
- 전자서명(private/public key) 검증 — checksum(SHA256)까지만 다룬다
- 멀티 Policy Type 동시 운용의 완전한 구현 — hot swap 개념 검증까지만
- 프로덕션 수준의 에러 복구·재연결 — 학습에 필요한 만큼만

## 8. 마일스톤 공통 완료 기준

각 마일스톤은 다음을 만족하면 "완료"로 본다.

1. 빌드 성공 (`cmake --build`)
2. 두 프로세스를 실행해 의도한 동작을 눈으로 확인
3. 의도적으로 한 번 깨뜨려 보기 (연결 실패 / 짧은 read / checksum 불일치 / 깨진 JSON 등 해당 단계의 실패 케이스)
4. `README.md`에 실행 레시피 기록
5. `tasks.md` 체크박스 갱신 후 커밋
