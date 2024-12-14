#!/bin/bash

# 디버깅할 프로그램과 관련된 파일 감시
FILES="main.cpp TextTable.cpp Table.cpp"
PROGRAM="TableProgram"
BUILD_DIR="build"

# LLDB 디버깅 세션 실행 함수
start_lldb() {
    lldb ./${BUILD_DIR}/${PROGRAM}
}

# 파일 변경 감지 및 재컴파일
while true; do
    if ! command -v fswatch &> /dev/null; then
        echo "Error: fswatch is not installed. Please install it and try again."
        exit 1
    fi

    # 파일 변경 감지
    fswatch -1 $FILES
    clear

    # 프로젝트 재컴파일
    echo "Recompiling..."
    if ! cmake --build $BUILD_DIR; then
        echo "Build failed. Fix the errors and try again."
    else
        echo "Rebuild successful. Restarting LLDB..."
        pkill lldb 2>/dev/null # 기존 LLDB 프로세스 종료
        sleep 1
        start_lldb             # LLDB 다시 시작
    fi
done
