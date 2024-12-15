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


# 소스 파일 추가
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/**/*.cpp)

# 특정 파일 추가
set(SOURCES ${SOURCES} src/main.cpp)

add_library(MyProjectLib ${SOURCES})

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES})


# tests 디렉토리 추가
enable_testing()
add_subdirectory(tests)
