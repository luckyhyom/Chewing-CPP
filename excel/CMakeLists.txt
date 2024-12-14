# 최소 CMake 버전 설정
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름과 사용 언어
project(TableProgram LANGUAGES CXX)

# 컴파일 옵션
set(CMAKE_CXX_STANDARD 17)          # C++17 표준 사용
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++17 필수
set(CMAKE_CXX_FLAGS "-Wall -g")     # 컴파일 플래그

# 소스 파일 목록
set(SOURCES
    main.cpp
    Cell.cpp
    Table.cpp
    TextTable.cpp
    CSVTable.cpp
    HTMLTable.cpp
)

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES})

# clean 작업 자동 포함
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM true)
