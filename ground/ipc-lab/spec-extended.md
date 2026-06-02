# ipc-lab 확장판 — 전체 VCRM 메커니즘 로드맵 (M10+)

작성일: 2026-06-03 · 상태: **참고용 로드맵 (아직 미착수)**

이 문서는 [spec.md](spec.md)의 M0~M9(통신 척추 + 정책 획득)를 **데이터 수집 파이프라인까지 확장**해, `ground/docs.md`·`ground/solution.md`가 기술한 VCRM 전체 메커니즘을 작은 코드로 실험하기 위한 방향을 정리한다.

지금 당장 구현하는 것이 아니라, M9에 가까워졌을 때 꺼내 `plan.md`/`tasks.md`로 펼치기 위한 **미래 계획**이다.

## 1. 전제: M0~M9의 종착점

`spec.md`의 M0~M9를 완료하면 다음이 동작한다.

```text
MQTT 신호(M8) → HTTP 다운로드(M7) → JSON 파일 생성(M3)
            → IPC 알림(M5) → VDC가 읽어 적용(M6)
```

이것은 **DPM↔VDC 통신 척추 + 정책 획득**까지다. 아직 "받은 정책으로 실제 차량 데이터를 수집"하지는 않는다. 그 부분이 M10+의 영역이다.

## 2. 연결 원칙 — 기존 계획을 변경하지 않는다

M0~M9를 **다시 고치지 않는다.** 이미 박혀 있는 두 개의 **연결 지점(seam)** 에 뒤에서 "추가(append)"만 한다.

```text
1) M6의 정책 적용 후크
   VDC가 IPC로 ApplyPolicy 수신 (M5/M6)
     -> applyPolicy(PolicySnapshot)            ← 여기에 파이프라인을 꽂는다
        -> [Receiver -> PreProcessor -> DataCache -> Capture -> Storage -> Transmission]

2) M1의 length-prefix 프레이밍 / M0~M5의 메시지·epoch 개념
   레이어 사이 BlockingQueue, 파일 record, hot swap epoch에 그대로 재사용
```

기존 vcrm 샘플(`ground/vcrm_x86_sample`)의 `VcrmPipeline::applyPolicy(Policy)`, `Command::ApplyPolicy`, `BlockingQueue<Command>`가 정확히 이 seam과 맞물린다. ipc-lab의 VDC와 샘플의 파이프라인은 이 지점에서 합류한다.

```text
[M0~M9] 통신 척추 + 정책 획득
   └─ applyPolicy seam ─┐
[M10~M20] 수집 파이프라인 ┘  =  docs.md 전체 VCRM 메커니즘
```

## 3. mock-first 유지

확장 단계에서도 원칙은 같다. 실제 차량 HW 대신 **모의 Receiver**(타이머로 가짜 데이터 주입, M12)로 전 구간을 외부 인프라 없이 검증한다. 실제 CAN/이더넷 수신은 최후에 선택적으로 붙인다.

## 4. 확장 마일스톤 (M10~M20)

| #   | 마일스톤 | 실험 내용 | 참고 문서 |
|-----|----------|-----------|-----------|
| M10 | 파이프라인 seam + PolicyEpoch | `applyPolicy`를 상태교체 → **불변 `PolicySnapshot(epoch)` 생성**으로 격상. 모든 데이터·제어 메시지에 `epoch` 부착 | solution §3, docs §30 |
| M11 | BlockingQueue 레이어 골격 | 6레이어를 **각자 스레드 + 입력 큐**로. `PipelineMessage{Data, ApplyPolicy, Privacy, Shutdown}` 정의 | docs §30–31 |
| M12 | 모의 Receiver | 타이머로 가짜 CAN Signal / VehicleStatus `DataMessage` 주입 (mock-first 유지) | docs §5.1, §21 |
| M13 | **DataCache 최소 주기 조율** | `DataKey` 병합, `effective_period = min(period)`, ring buffer, subscriber 메타 (**난이도 2위**) | solution §2, docs §5.3, §18 |
| M14 | **TriggerRuleEvaluator** | `ICondition` 계층 + `(AND)OR` DNF + 3치 논리 + STOPPED/STARTED 전이. Signal/VehicleStatus부터 (**난이도 1위**) | solution §1, docs §12–13 |
| M15 | Capture → Storage | 수집 ON 구간을 파일로. length-prefix record, 파일 회전·개수·순환 삭제 | docs §5.5, §9, §19 |
| M16 | 암호화 + 위치정보 분리 | AES-128+ 파일 암호화, 위치정보 별도 파일 + `secureTransmissionInterval` | docs §10, §22 |
| M17 | Transmission | Persistent/On-demand **HTTP POST**(압축·분할·재시도), Streaming **MQTT** | docs §11, §23 |
| M18 | **Policy hot swap (full)** | epoch 기반 교체, DataPlan diff, TriggerPolicy ID 상태 이관, Prepare/Commit 2단계 (**난이도 3위**) | solution §3, docs §8, §25 |
| M19 | Privacy Mode | 즉시 전체 STOP + 미전송 데이터 삭제, 해제 후 startCondition 재평가 | docs §16 |
| M20 | 나머지 Trigger 타입 | Timing / Schedule / Geofence / Internal Event + 조합 제약 검증 | docs §12 |

## 5. 난이도 배치 의도

`solution.md`가 꼽은 **구현 난이도 Top 3**가 각각 **M14 / M13 / M18**에 배치된다.

| 난이도 | 컴포넌트 | 배치 |
|:---:|---|:---:|
| 1위 | TriggerRuleEvaluator | M14 |
| 2위 | DataCache 주기 조율 | M13 |
| 3위 | Policy hot swap | M18 |

세 난이도를 연달아 두지 않고 사이사이 평이한 단계(M12 모의 Receiver, M15 Storage 회전)를 끼워 "어려운 것 → 쉬운 것 → 어려운 것" 리듬을 만든다. 매번 동작하는 결과물로 동기를 유지하면서 어려운 컴포넌트를 하나씩 정복한다. `solution.md`의 권장 구현 순서와도 일치한다.

## 6. 종착점

M20까지 가면 `docs.md`가 정의한 전체 메커니즘을 작은 코드로 모두 거치게 된다. 이 시점에서 ipc-lab은 "두 프로세스 통신 실험"을 넘어, **정책 획득부터 데이터 수집·저장·전송까지의 VCRM 축소판**이 된다.

## 7. 이 확장에서도 범위 밖

M10+까지 가더라도 다음은 다루지 않는다(학습 비용 대비 효용이 낮음).

- 전자서명(private/public key) 검증 — 무결성은 checksum(SHA256)까지만
- 실제 차량 HW 드라이버(LLCE / MCU Shared Memory / GMAC 등) — 모의 Receiver로 대체
- 프로덕션 수준의 에러 복구·재연결·고가용성 — 학습에 필요한 만큼만
- 클라우드 서버 측 데이터 재조합 — CCU(차량) 측만 구현

## 8. 펼치는 시점

- 이 문서는 **방향 메모**다. M9에 가까워지면 그때 각 마일스톤을 `plan.md`(HOW)와 `tasks.md`(체크리스트)로 펼친다.
- 지금 미리 펼치지 않는 이유: 추측성 항목이 너무 많아져 유지 비용만 늘기 때문. 실제로 손에 닿을 때 구체화하는 편이 정확하다.
