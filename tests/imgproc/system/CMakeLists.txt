# 添加可执行文件 cucv_tests
add_executable(cucv_test_system
    test_copy.cpp
    test_version.cpp
)

# cucv_tests 链接依赖库
target_link_libraries(cucv_test_system
    PUBLIC
        cucv_core
        cucv_imgproc
        cucv_test_common
)

cucv_add_test(cucv_test_system)