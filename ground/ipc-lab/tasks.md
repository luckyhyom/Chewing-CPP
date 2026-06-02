# ipc-lab 작업 체크리스트 (tasks)

진행 **상태**의 유일한 출처(single source of truth). 설계는 [spec.md](spec.md), 구현 방법은 [plan.md](plan.md).

상태 표기: `[ ]` 미착수 · `[~]` 진행중 · `[x]` 완료

---

## M0 — Walking skeleton
- [ ] CMakeLists.txt: `vcrm_vdc`, `vcrm_dpm` 두 타깃 빌드
- [ ] VDC: Unix Domain Socket bind/listen/accept/read → 출력 (시작 시 기존 .sock unlink)
- [ ] DPM: connect → `"hello"` write
- [ ] 검증: VDC 먼저 실행 → DPM 실행 → `hello` 출력 확인
- [ ] 깨뜨리기: VDC 없이 DPM 실행 → connect 실패 처리
- [ ] README에 실행 순서 기록 + 커밋

## M1 — 길이 프레이밍
- [ ] `common/frame.{hpp,cpp}`: `readExact`, `writeFrame`, `readFrame`
- [ ] 양쪽 main을 frame API로 전환
- [ ] 검증: 길이 다른 메시지 여러 개 순차 송수신 온전성
- [ ] 깨뜨리기: 헤더보다 짧은 payload로 readExact 동작 관찰
- [ ] 커밋

## M2 — JSON 메시지
- [ ] CMake에 nlohmann/json 의존 추가
- [ ] `common/message.hpp`: `ApplyPolicy` + to_json/from_json
- [ ] DPM: 하드코딩 ApplyPolicy dump → 전송
- [ ] VDC: parse → 필드 출력
- [ ] 깨뜨리기: 깨진 JSON 전송 → parse_error graceful 처리
- [ ] 커밋

## M3 — 파일 + IPC 분리
- [ ] DPM: policy를 tmp 작성 → fsync → rename(원자적)
- [ ] DPM: `{path}` 담아 ApplyPolicy 전송
- [ ] VDC: path로 파일 읽어 파싱
- [ ] 검증: 파일 경유 policy 전달 + cat으로 파일 확인
- [ ] 깨뜨리기: 없는 path 전송 → 오픈 실패 처리
- [ ] 커밋

## M4 — checksum 검증
- [ ] CMake에 OpenSSL 의존 추가
- [ ] `sha256Hex()` 헬퍼(EVP)
- [ ] DPM: 파일 SHA256 계산해 메시지에 포함
- [ ] VDC: 파일 직접 해시 → 비교
- [ ] 깨뜨리기: 파일 한 글자 수정 → 불일치 감지
- [ ] 커밋

## M5 — ACK / Reject 응답
- [ ] `ApplyPolicyResult` 구조체(status/reason)
- [ ] VDC: 결과를 회신
- [ ] DPM: 응답 수신·출력
- [ ] 검증: 일치→Applied / 불일치→Rejected+reason
- [ ] 깨뜨리기: VDC 응답 전 종료 → DPM EOF 처리
- [ ] 커밋

## M6 — 적용 + hot swap
- [ ] VDC 상시 루프 + `currentPolicy` 상태 보관
- [ ] 같은 type 새 policy로 교체
- [ ] IdlePolicy / DeletePolicy 메시지 처리
- [ ] 검증: v2→v3 교체, Idle 비활성 로그 확인
- [ ] 커밋

## M7 — 진짜 HTTP 다운로드
- [ ] CMake에 libcurl 의존 추가
- [ ] 로컬 HTTP 서버로 policy 서빙(`python3 -m http.server`)
- [ ] DPM: libcurl GET → 파일 저장 → 이후 흐름 재사용
- [ ] 검증: HTTP 다운로드 → 적용 end-to-end
- [ ] 깨뜨리기: 서버 끄고 다운로드 → 실패/재시도 관찰
- [ ] 커밋

## M8 — 진짜 MQTT 트리거
- [ ] CMake에 Paho MQTT C++ 의존 추가
- [ ] mosquitto 로컬 브로커 실행
- [ ] DPM: 토픽 구독 → 수신 시 다운로드→저장→알림 트리거
- [ ] 검증: `mosquitto_pub` 발행 → 전체 자동 흐름
- [ ] 깨뜨리기: 브로커 끊김 → 재연결 관찰
- [ ] 커밋

## M9 — (선택) 확장
- [ ] protobuf 저장 포맷
- [ ] spdlog 로깅
- [ ] libpcap raw 패킷 맛보기
