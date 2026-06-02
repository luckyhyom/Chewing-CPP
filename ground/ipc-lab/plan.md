# ipc-lab 구현 계획 (plan)

이 문서는 [spec.md](spec.md)의 마일스톤을 **어떻게** 구현할지 정의한다(HOW). 진행 상태(체크박스)는 [tasks.md](tasks.md)에만 둔다.

각 마일스톤은: **목표 → 만질 파일 → 핵심 구현 → 검증법(깨뜨려 보기 포함) → 완료조건** 순으로 적는다.

---

## M0 — Walking skeleton

**목표:** 두 프로세스가 소켓 하나로 단방향 한 마디를 주고받는다.

**만질 파일:** `CMakeLists.txt`, `vdc/main.cpp`, `dpm/main.cpp`

**핵심 구현:**
- VDC: `socket(AF_UNIX, SOCK_STREAM, 0)` → `bind("/tmp/vcrm_vdc.sock")` → `listen` → `accept` → `read` → 출력. 시작 시 기존 소켓 파일 `unlink` 먼저.
- DPM: `socket` → `connect` → `write("hello")`.
- CMake: `vcrm_vdc`, `vcrm_dpm` 두 실행 파일 타깃.

**검증:** 터미널 2개. 먼저 VDC 실행(대기), 그다음 DPM 실행 → VDC가 `hello` 출력.
**깨뜨려 보기:** VDC 없이 DPM만 실행 → `connect` 실패(ECONNREFUSED/ENOENT) 처리 확인.

**완료조건:** 위 동작 확인 + README에 실행 순서 기록.

---

## M1 — 길이 프레이밍

**목표:** 임의 길이 메시지를 정확히 한 덩어리로 송수신한다.

**만질 파일:** `common/frame.hpp`, `common/frame.cpp`, 양쪽 `main.cpp`

**핵심 구현:**
- `writeFrame(fd, bytes)`: 4바이트 길이(LE) 먼저 쓰고 payload 쓰기. `write`가 부분 전송될 수 있으므로 루프.
- `readFrame(fd) -> bytes`: 4바이트를 **정확히** 읽는 `readExact()` 헬퍼로 길이 획득 → 그 길이만큼 `readExact()`.
- `readExact(fd, buf, n)`: n바이트를 다 읽을 때까지 루프(짧은 read 대응), EOF/에러 구분.

**검증:** 길이가 다른 메시지 여러 개(짧은 것, 수 KB)를 연달아 보내 모두 온전히 수신.
**깨뜨려 보기:** 일부러 길이 헤더보다 짧은 payload를 보내 `readExact`가 블록/에러 처리하는지 관찰.

**완료조건:** 경계가 깨지지 않고 N개 메시지 순차 수신.

---

## M2 — JSON 메시지

**목표:** 구조화된 ApplyPolicy 메시지를 직렬화/역직렬화한다.

**만질 파일:** `common/message.hpp`, 양쪽 `main.cpp`, `CMakeLists.txt`(nlohmann/json 의존)

**핵심 구현:**
- nlohmann/json 도입(single-header 또는 `find_package`).
- `message.hpp`에 `ApplyPolicy` 구조체 + `to_json/from_json`.
- DPM: 하드코딩한 ApplyPolicy를 `j.dump()` → `writeFrame`.
- VDC: `readFrame` → `json::parse` → 필드 출력.

**검증:** VDC가 `messageType/policyType/path/sha256`를 정확히 출력.
**깨뜨려 보기:** 깨진 JSON("{bad") 전송 → VDC가 `parse_error`를 잡아 죽지 않고 로그.

**완료조건:** 정상 메시지 파싱 + 깨진 메시지 graceful 처리.

---

## M3 — 파일 + IPC 분리 (핵심 패턴)

**목표:** Policy 본문은 파일로, IPC로는 경로만 보낸다.

**만질 파일:** `dpm/main.cpp`, `vdc/main.cpp`, (공용 경로 헬퍼)

**핵심 구현:**
- DPM: policy JSON을 `…/policy.tmp`에 쓰고 → `fsync` → `rename(tmp, persistent.json)` (원자적 교체). 그 후 `{path}` 담은 ApplyPolicy 전송.
- VDC: 메시지의 `path`로 파일을 열어 내용 읽고 파싱.

**검증:** DPM이 만든 파일을 VDC가 읽어 policy 내용 출력. `cat`으로 파일 실제 존재 확인.
**깨뜨려 보기:** 존재하지 않는 path 전송 → VDC가 파일 오픈 실패 처리.

**완료조건:** 파일 경유로 policy 전달 성공 + rename 원자성 이해 기록.

---

## M4 — checksum 검증

**목표:** VDC가 받은 파일이 DPM이 검증한 그 파일인지 확인한다.

**만질 파일:** `common/`(sha256 헬퍼), 양쪽 `main.cpp`, `CMakeLists.txt`(OpenSSL)

**핵심 구현:**
- OpenSSL EVP로 `sha256Hex(bytes) -> string`.
- DPM: 파일 작성 후 SHA256 계산해 메시지 `sha256`에 담음.
- VDC: 파일 읽은 뒤 직접 SHA256 계산 → 메시지 값과 비교.

**검증:** 일치 시 통과 로그.
**깨뜨려 보기:** VDC가 파일 읽기 전, 파일을 외부에서 한 글자 수정 → checksum 불일치 감지.

**완료조건:** 일치/불일치 두 경로 모두 확인.

---

## M5 — ACK / Reject 응답 (양방향)

**목표:** VDC가 처리 결과를 DPM에 회신한다.

**만질 파일:** `common/message.hpp`(ApplyPolicyResult), 양쪽 `main.cpp`

**핵심 구현:**
- `ApplyPolicyResult` 구조체(status: Applied/Rejected, reason).
- VDC: 검증 결과에 따라 결과 메시지를 `writeFrame`으로 회신.
- DPM: 응답을 `readFrame`으로 받아 출력. (같은 연결에서 요청→응답)

**검증:** checksum 일치 → Applied 수신 / 불일치 → Rejected+reason 수신.
**깨뜨려 보기:** VDC가 응답 전에 종료 → DPM의 응답 read가 EOF 처리.

**완료조건:** 양방향 요청-응답 1왕복 완성.

---

## M6 — 적용 + hot swap

**목표:** VDC가 "현재 policy" 상태를 들고, 새 메시지로 교체한다.

**만질 파일:** `vdc/main.cpp`(현재 policy 상태 보관), `dpm/main.cpp`(여러 번 전송)

**핵심 구현:**
- VDC를 상시 루프로: `accept`를 반복하거나 한 연결에서 여러 프레임 수신.
- VDC가 `currentPolicy`(map<policyType, Policy>) 보유 → ApplyPolicy 적용 시 교체.
- `IdlePolicy`(빈 policy)와 `DeletePolicy` 메시지 처리: 해당 type 비활성/제거.

**검증:** policy v2 전송 후 v3 전송 → VDC 상태가 교체됨을 로그로 확인. Idle 전송 시 비활성.
**깨뜨려 보기:** 적용 도중(로그 사이) 두 번째 메시지가 와도 순서대로 처리되는지 확인.

**완료조건:** 같은 type 교체 + Idle/Delete 동작.

---

## M7 — 진짜 HTTP 다운로드

**목표:** DPM이 하드코딩 대신 HTTP로 policy를 받아온다.

**만질 파일:** `dpm/`(http 다운로드), `CMakeLists.txt`(libcurl)

**핵심 구현:**
- 로컬 HTTP 서버 준비: `python3 -m http.server`로 policy json 파일 서빙(가장 간단). 또는 libcurl `file://`로 먼저 연습.
- DPM: libcurl로 GET → 응답 바디를 파일로 저장 → 이후 M3~M5 흐름(저장·checksum·전송) 재사용.

**검증:** 서버의 policy를 DPM이 받아 저장 → VDC가 적용까지 end-to-end.
**깨뜨려 보기:** 서버 끄고 다운로드 → 실패 처리·재시도 관찰.

**완료조건:** HTTP 경유 다운로드 → 적용 완성.

---

## M8 — 진짜 MQTT 트리거

**목표:** DPM이 MQTT 신호를 받아 전체 흐름을 시작한다.

**만질 파일:** `dpm/`(mqtt 구독), `CMakeLists.txt`(Paho MQTT C++)

**핵심 구현:**
- mosquitto 브로커 로컬 실행.
- DPM: 토픽 구독 → 메시지 수신 콜백에서 "M7 다운로드 → M3 저장 → M5 알림" 트리거.
- 신호 payload에 policy URL/version 등을 담아도 좋음.

**검증:** `mosquitto_pub`로 신호 발행 → DPM이 다운로드·VDC 적용까지 자동 진행.
**깨뜨려 보기:** 브로커 끊김 시 DPM 재연결 동작 관찰.

**완료조건:** MQTT→HTTP→파일→IPC→적용 전체 자동 흐름.

---

## M9 — (선택) 확장

- **protobuf:** 저장 record를 JSON 대신 protobuf로. `varint 길이 + 메시지` 프레이밍.
- **spdlog:** 양쪽 프로세스 로깅 표준화.
- **libpcap:** Raw 패킷 수집 맛보기(VCRM Receiver 확장 실험).

각 항목은 독립적으로 붙일 수 있으며, 필요해질 때 tasks.md에 세부 항목을 추가한다.

---

## 권장 진행 방식

- 한 번에 **한 마일스톤**만. 끝나면 커밋하고 tasks.md 갱신.
- 막히면 "지금 어떤 바이트가 소켓에 오가는가"를 먼저 확인(작은 출력/`xxd`).
- 각 단계의 "깨뜨려 보기"를 건너뛰지 말 것 — 실패 경로가 진짜 학습 지점이다.
