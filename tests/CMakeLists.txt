# tests/CMakeLists.txt

# GoogleTest 찾기
find_package(GTest REQUIRED)

# core 테스트 소스 파일
file(GLOB_RECURSE CORE_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp
)

# table 테스트 소스 파일
file(GLOB_RECURSE TABLE_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/table/*.cpp
)

message(STATUS "TABLE_TEST_SOURCES found: ${TABLE_TEST_SOURCES}")

# 모든 테스트 소스를 하나로 합침
set(TEST_SOURCES
    ${CORE_TEST_SOURCES}
    ${TABLE_TEST_SOURCES}
)

# 테스트 실행 파일 생성
add_executable(UnitTests ${TEST_SOURCES})

# GoogleTest와 링크
target_link_libraries(UnitTests
    PRIVATE
        GTest::GTest
        GTest::Main
        # 만약 테스트 대상 라이브러리가 따로 있다면 여기에 추가
        # e.g. MyLib
        # e.g. PRIVATE MyLib
        MyProjectLib
)

# 빌드 출력 디렉토리 설정 (선택 사항)
set_target_properties(UnitTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 테스트 등록
enable_testing()
add_test(NAME UnitTests COMMAND UnitTests)
