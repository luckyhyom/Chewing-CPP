# VCRM 학습/구현 계획

이 문서는 `ground/spec.md`의 내용을 바탕으로, C++ 샘플 프로젝트에서 어떤 기능을 어떤 순서로 학습하고 구현할지 정리한 문서입니다.

목표는 전체 VCRM 시스템을 한 번에 구현하는 것이 아니라, 실무에 필요한 핵심 기능을 작은 코드와 테스트로 검증하는 것입니다.

## 2. 프로젝트 라이브러리 공식 문서 샘플 확인 및 사용

단순히 라이브러리가 설치되는지 확인하는 것은 충분하지 않습니다.

각 라이브러리가 프로젝트에서 담당할 기능을 정하고, 공식 문서의 예제를 참고해서 실제 기능 코드와 테스트에 녹여야 합니다.

| 라이브러리 | 공식 샘플/문서 | 프로젝트에서 써볼 기능 |
| --- | --- | --- |
| `nlohmann/json` | 공식 API 문서에 예제 있음 | Policy JSON 파싱 |
| `valijson` | GitHub 예제 있음 | Policy Schema 검증 |
| `protobuf` | 공식 C++ 튜토리얼 있음 | DataRecord 저장 포맷 |
| `libcurl` | 공식 HTTP POST 예제 있음 | Policy 다운로드 / 데이터 업로드 |
| `OpenSSL` | EVP 공식 문서 있음 | SHA256, AES 암호화 |
| `Paho MQTT C++` | Eclipse 공식 문서 있음 | Policy 알림 수신 / Streaming 전송 |
| `libpcap` | libpcap API/man page 문서 있음 | Raw Ethernet 패킷 수집 |
| `spdlog` | 공식 README 예제 있음 | 로그 출력 |
| Unix Domain Socket | Linux man page 있음 | DPM-VDC IPC |

공식 문서:

- nlohmann/json: <https://nlohmann.github.io/json/api/json/>
- protobuf C++ tutorial: <https://protobuf.dev/getting-started/cpptutorial/>
- libcurl examples: <https://curl.se/libcurl/c/example.html>
- OpenSSL EVP: <https://docs.openssl.org/3.0/man3/EVP_EncryptInit/>
- Paho MQTT C++: <https://eclipse.dev/paho/clients/cpp>
- spdlog: <https://github.com/gabime/spdlog>
- Unix domain socket: <https://man7.org/linux/man-pages/man7/unix.7.html>

## 3. 두 프로세스가 Policy를 어떻게 공유할 것인가

권장 구조는 다음과 같습니다.

```text
DPM = Policy의 소유자
VDC = Policy의 적용자
```

DPM은 클라우드에서 Policy를 받고 검증합니다.

VDC는 DPM이 검증한 Policy를 받아 실제 데이터 수집 파이프라인에 적용합니다.

### 전체 흐름

```text
Cloud
  -> MQTT로 Policy 변경 알림
  -> DPM이 HTTP로 Policy 다운로드
  -> DPM이 SHA256/서명 검증
  -> DPM이 JSON Schema 검증
  -> DPM이 Policy 파일을 안전하게 저장
  -> DPM이 VDC에 "새 Policy 적용" IPC 메시지 전송
  -> VDC가 Policy를 읽고 런타임 파이프라인에 적용
  -> VDC가 DPM에 Applied/Failed 응답
```

중요한 점은 Policy 객체를 두 프로세스가 메모리로 직접 공유하지 않는 것입니다.

두 프로세스는 서로 다른 주소 공간을 갖기 때문에, 한쪽의 C++ 객체 포인터를 다른 프로세스가 직접 사용할 수 없습니다.

따라서 좋은 방식은 다음입니다.

```text
DPM:
  validated_policy.tmp 작성
  fsync
  rename -> persistent_policy.json
  VDC에 ApplyPolicy 메시지 전송

VDC:
  ApplyPolicy 메시지 수신
  checksum 확인
  파일 읽기
  기존 Policy와 교체
  ACK 응답
```

### 왜 파일 + IPC 알림 구조가 좋은가

Policy는 크기가 커질 수 있고, 나중에는 Schema 검증, checksum, version 관리가 붙습니다.

이런 데이터를 IPC 메시지 하나에 전부 싣는 것보다, 검증된 파일을 저장하고 IPC로는 “어떤 파일을 적용하라”는 명령만 보내는 편이 안전합니다.

IPC 메시지는 작고 명확하게 유지합니다.

Policy 본문은 파일로 관리합니다.

### ApplyPolicy 메시지 예시

```json
{
  "messageType": "ApplyPolicy",
  "policyType": 0,
  "policyVersion": 2,
  "path": "/vcrm/policy/persistent.json",
  "sha256": "abc...",
  "applyImmediately": true
}
```

핵심 필드는 다음입니다.

| 필드 | 의미 |
| --- | --- |
| `messageType` | 어떤 IPC 명령인지 |
| `policyType` | Persistent / On-demand / Streaming / Vehicle Logger |
| `policyVersion` | Policy 버전 |
| `path` | DPM이 저장한 Policy 파일 경로 |
| `sha256` | VDC가 다시 확인할 checksum |
| `applyImmediately` | 즉시 적용 여부 |

VDC는 `path`에 있는 파일을 읽은 뒤, 직접 SHA256을 계산해서 메시지의 `sha256`과 비교해야 합니다.

그래야 “DPM이 검증한 바로 그 파일”을 VDC가 적용하는지 확인할 수 있습니다.

## 4. 두 프로세스가 서로 통신하게 해보기

같은 장비 안의 DPM/VDC라면 Unix Domain Socket이 가장 적절합니다.

네트워크 TCP 소켓보다 가볍고, 파일 권한으로 접근 제어도 할 수 있습니다.

### 추천 실습 구조

```text
vdc_process
  - /tmp/vcrm_vdc.sock 생성
  - DPM 메시지 대기
  - ApplyPolicy 수신 시 Policy 적용
  - ACK 응답

dpm_process
  - Policy 파일 생성/검증
  - /tmp/vcrm_vdc.sock 접속
  - ApplyPolicy 메시지 전송
  - ACK 확인
```

### 메시지 프레이밍

IPC 메시지 포맷은 처음에는 JSON으로 충분합니다.

다만 단순히 JSON 문자열만 보내면 안 됩니다.

소켓의 `read()`는 한 번에 메시지 전체를 보장하지 않습니다.

그래서 메시지 앞에 길이를 붙이는 방식이 좋습니다.

```text
[4바이트 길이][JSON payload]
```

예:

```text
00000072{"messageType":"ApplyPolicy","policyType":0,...}
```

실제 구현에서는 앞 4바이트를 binary little-endian 정수로 두면 됩니다.

이 구조를 쓰면 receiver는 먼저 4바이트를 읽어서 payload 길이를 알고, 그 길이만큼 다시 읽으면 됩니다.

### 실습 순서

```text
1. VDC IPC 서버 만들기
2. DPM IPC 클라이언트 만들기
3. ApplyPolicy 메시지 보내기
4. VDC가 ACK 보내기
5. 잘못된 checksum이면 reject
6. 같은 type의 Policy가 오면 hot swap
7. DeletePolicy 또는 IdlePolicy도 보내보기
```

### IPC 응답 예시

성공:

```json
{
  "messageType": "ApplyPolicyResult",
  "policyType": 0,
  "policyVersion": 2,
  "status": "Applied"
}
```

실패:

```json
{
  "messageType": "ApplyPolicyResult",
  "policyType": 0,
  "policyVersion": 2,
  "status": "Rejected",
  "reason": "checksum mismatch"
}
```

## 구현 목표 정리

다음 단계의 구현 목표는 아래와 같습니다.

```text
DPM 프로세스:
  Policy를 생성/검증/저장하는 역할

VDC 프로세스:
  Policy를 적용하고 데이터 수집 파이프라인을 바꾸는 역할

통신 방식:
  Unix Domain Socket

메시지 형식:
  length-prefixed JSON

Policy 공유 방식:
  파일 저장 + IPC 알림 + checksum 검증
```

## 현재 샘플 프로젝트와 연결되는 부분

| 현재 샘플 기능 | 다음 확장 |
| --- | --- |
| `sample_policies.hpp`에서 C++로 Policy 생성 | `nlohmann/json`으로 Policy 파일 파싱 |
| `sha256Hex()` | Policy checksum 검증 |
| `VcrmPipeline::applyPolicy()` | IPC로 받은 Policy 적용 |
| `TriggerRuleEvaluator` | 새 Policy hot swap |
| `StorageManager::clearFiles()` | Privacy Mode / Policy reset 시 파일 삭제 |
| `TransmissionManager` | HTTP 업로드 실습 |
| `BlockingQueue` | IPC 메시지 수신 후 pipeline에 안전하게 전달 |

## 우선순위

1. `nlohmann/json`으로 Policy JSON 파일 읽기
2. Policy checksum 검증
3. Unix Domain Socket으로 DPM -> VDC 메시지 보내기
4. VDC에서 `ApplyPolicy`를 받아 `VcrmPipeline::applyPolicy()` 호출
5. ACK/Rejected 응답 구현
6. Policy hot swap 테스트 확장
7. protobuf 저장 포맷으로 교체
8. MQTT / libpcap / spdlog 확장
