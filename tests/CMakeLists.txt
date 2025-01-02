# tests/CMakeLists.txt

# GoogleTest 찾기
find_package(GTest REQUIRED)

# 빌드 출력 디렉토리 설정 (선택 사항)
# 테스트 실행 파일들이 build/bin 에 생성되도록 설정
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ------------------------------------------------------------------------------
# Core 테스트
# ------------------------------------------------------------------------------
file(GLOB_RECURSE CORE_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp
)

add_executable(CoreTests ${CORE_TEST_SOURCES})
target_link_libraries(CoreTests
    PRIVATE
        GTest::GTest
        GTest::Main
        MyProjectLib   # 실제 프로젝트 라이브러리 이름
)
set_target_properties(CoreTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ctest에 "CoreTests"라는 이름으로 등록
add_test(NAME CoreTests COMMAND CoreTests)


# ------------------------------------------------------------------------------
# Table 테스트
# ------------------------------------------------------------------------------
file(GLOB_RECURSE TABLE_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/table/*.cpp
)

add_executable(TableTests ${TABLE_TEST_SOURCES})
target_link_libraries(TableTests
    PRIVATE
        GTest::GTest
        GTest::Main
        MyProjectLib
)
set_target_properties(TableTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ctest에 "TableTests"라는 이름으로 등록
add_test(NAME TableTests COMMAND TableTests)

# ------------------------------------------------------------------------------
# utils 테스트
# ------------------------------------------------------------------------------
file(GLOB_RECURSE UTILS_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp
)

add_executable(UtilTests ${UTILS_TEST_SOURCES})
target_link_libraries(UtilTests
    PRIVATE
        GTest::GTest
        GTest::Main
        MyProjectLib
)
set_target_properties(UtilTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ctest에 "UtilTests"라는 이름으로 등록
add_test(NAME UtilTests COMMAND UtilTests)

# ------------------------------------------------------------------------------
# 최종적으로 ctest를 통해 테스트 실행 가능
# ------------------------------------------------------------------------------
enable_testing()
