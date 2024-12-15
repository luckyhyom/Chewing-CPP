cmake_minimum_required(VERSION 3.10)
project(TableProgram LANGUAGES CXX)

# 컴파일 옵션
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -g")

# 빌드 경로
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# include 디렉토리 추가
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/core)
include_directories(${CMAKE_SOURCE_DIR}/include/table)

# ------------------------------------------------------------------------------
# 라이브러리 소스 수집 (main.cpp는 제외)
# ------------------------------------------------------------------------------
file(GLOB_RECURSE LIB_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/**/*.cpp
)

# main.cpp 제거
list(REMOVE_ITEM LIB_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

# 라이브러리 생성
add_library(MyProjectLib ${LIB_SOURCES})

# ------------------------------------------------------------------------------
# 실행 파일 (TableProgram) 생성
# ------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE MyProjectLib)

# ------------------------------------------------------------------------------
# 테스트 설정
# ------------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
