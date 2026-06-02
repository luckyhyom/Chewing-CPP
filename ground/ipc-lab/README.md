1. C++ 실무를 위한 핵심 정보 학습
2. 프로젝트에서 사용되는 라이브러리의 공식 문서 샘플이 있는지 확인하고 사용해보기
3. 서로 다른 2개의 프로세스가, 네트워크를 통해 전송된 데이터 관리에 대한 policy를 어떻게 공유할 것인가?
   1. 처음에 파일을 만들 때 파일명을 정확히 하지 않고, 파일 쓰기가 모두 완료되면 파일명을 정상적으로 수정한다. (깨진 파일을 읽는 이슈를 방지하기 위해)
      1. policy.tmp -> policy_YYYY_MM_DD_hh_mm_ss.json
4. 2개의 프로세스가 서로 통신하게 해보기
   1. .sock 파일 (Unix Domain Socket Inter-Process Comunication)
   2. 소켓 통신용 스레드 추가 or 비동기/non-blocking 방식 (epoll / select / poll)



---


IPC Message
```
{
  "messageType": "ApplyPolicy",
  "path": "/vcrm/policy/persistent.json",
  "sha256": "abc..."
}
```