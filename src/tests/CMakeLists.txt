find_package(GTest QUIET)

# 判断是否找到 GTest
if (GTEST_FOUND)
    message(STATUS "GTest found.")
    enable_testing()
    include_directories(${GTEST_INCLUDE_DIRS})
    link_libraries(${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
    add_executable(string_util_test string_util_test.cpp ../global/sha1.cpp ../global/base64.cpp)
    # 将Google Test库链接到测试目标
    target_link_libraries(string_util_test PRIVATE gtest gtest_main)
    # 添加测试
    add_test(NAME string_util_test COMMAND string_util_test)
else()
    message(WARNING "GTest not found. Tests will not be built.")
endif()