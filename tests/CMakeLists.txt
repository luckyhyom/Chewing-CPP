# 테스트 파일 추가
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_SOURCE_DIR}/tests/*.cpp)

# GoogleTest 또는 다른 프레임워크 링크
add_executable(UnitTests ${TEST_SOURCES})
target_link_libraries(UnitTests gtest gtest_main)
add_test(NAME UnitTests COMMAND UnitTests)
