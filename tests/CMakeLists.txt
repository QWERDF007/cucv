project(cucv_tests)
set(CMAKE_FOLDER tests)

enable_testing()

add_library(cucv_test_main
    test_main.cpp
)
target_link_libraries(cucv_test_main
    PUBLIC
    GTest::gtest
    GTest::gmock
    PRIVATE
    CUDA::cudart_static
)

# 添加到CTest
# 在build目录中执行ctest时，所有添加的测试都会执行
# 如果您不想使用CTest，而只是手动执行测试二进制文件，那么就没有必要使用add_test

# 将 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ 转换为平台特定的路径 (windows 反斜杆 "\")，并存储到 ${TESTS_DRIVER_DIR}
file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/" TESTS_DRIVER_DIR)

# UNIX 平台
if(UNIX)
    # 设置 ${TESTS_DRIVER} 为 ${TESTS_DRIVER_DIR}/run_tests.sh
    set(TESTS_DRIVER "${TESTS_DRIVER_DIR}/run_tests.sh")

    # 复制 ${CMAKE_CURRENT_SOURCE_DIR}/run_tests.sh.in 到 ${TESTS_DRIVER}，并在复制过程中替换其中的变量
    # @ONLY 表示只替换 `@VAR@` 出现的变量，其他形式 (例如 `${VAR}`) 不会替换
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run_tests.sh.in ${TESTS_DRIVER} @ONLY)

    # 定义一个名为 cucv_add_test 的宏，该宏接受一个参数 TESTCMD
    macro(cucv_add_test TESTCMD)
        # 获取 ${TESTCMD} 的文件名，存储到 ${TESTNAME}
        get_filename_component(TESTNAME "${TESTCMD}" NAME)

        # 添加一个测试，名为 ${TESTNAME}，测试命令为 ${TESTCMD}
        add_test(NAME "${TESTNAME}" COMMAND "${TESTCMD}")

        # 如果 ${TESTNAME} 是一个目标，将其链接到测试主库
        if(TARGET ${TESTNAME})
            target_link_libraries(${TESTNAME} PRIVATE cucv_test_main)
        endif()

        # 设置超时相关变量 (秒)
        set(TIMEOUT_KILL 800)
        set(TIMEOUT_TERM 620)

        # 设置测试属性，设置超时时间 (秒)
        set_tests_properties(${TESTNAME} PROPERTIES TIMEOUT ${TIMEOUT_TERM})

        # 在 ${TESTS_DRIVER} 追加一行 run ${TESTNAME}
        file(APPEND "${TESTS_DRIVER}" "run ${TESTNAME}\n")

        if(TARGET ${TESTNAME})
            # 安装 tests 组件时，${TESTNAME} 是一个目标，将其安装到 ${CMAKE_INSTALL_BINDIR}
            install(TARGETS ${TESTNAME}
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                COMPONENT tests)
        else()
            # 安装 tests 组件时，${TESTNAME} 是一个程序，将其安装到 ${CMAKE_INSTALL_BINDIR}
            install(PROGRAMS "${TESTCMD}"
                TYPE BIN
                COMPONENT tests)
        endif()
    endmacro()

    # 安装 tests 组件时，${TESTS_DRIVER} 是一个程序，将其安装到 ${CMAKE_INSTALL_BINDIR}
    install(PROGRAMS ${TESTS_DRIVER}
        TYPE BIN
        COMPONENT tests)
else()
    # 设置 ${TESTS_DRIVER} 为 ${TESTS_DRIVER_DIR}/run_tests.bat
    set(TESTS_DRIVER "${TESTS_DRIVER_DIR}/run_tests.bat")

    # 复制 ${CMAKE_CURRENT_SOURCE_DIR}/run_tests.bat.in 到 ${TESTS_DRIVER}，并在复制过程中替换其中的变量
    # @ONLY 表示只替换 `@VAR@` 出现的变量，其他形式 (例如 `${VAR}`) 不会替换
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run_tests.bat.in ${TESTS_DRIVER} @ONLY)

    # 定义一个名为 cucv_add_test 的宏
    macro(cucv_add_test TESTCMD)
        # 获取 ${TESTCMD} 的文件名，存储到 ${TESTNAME}
        get_filename_component(TESTNAME "${TESTCMD}" NAME)

        # 添加一个测试，名为 ${TESTNAME}，测试命令为 ${TESTCMD}
        add_test(NAME "${TESTNAME}" COMMAND "${TESTCMD}")

        # 如果 ${TESTNAME} 是一个目标，将其链接到测试主库
        if(TARGET ${TESTNAME})
            target_link_libraries(${TESTNAME} PRIVATE cucv_test_main)
        endif()

        # 设置超时相关变量 (秒)
        set(TIMEOUT_KILL 800)
        set(TIMEOUT_TERM 620)

        # 设置测试属性，设置超时时间 (秒)
        set_tests_properties(${TESTNAME} PROPERTIES TIMEOUT ${TIMEOUT_TERM})

        # 在 ${TESTS_DRIVER} 追加一行 call :run ${TESTNAME}
        file(APPEND "${TESTS_DRIVER}" "    call :run ${TESTNAME}.exe\n")

        if(TARGET ${TESTNAME})
            # 安装 tests 组件时，${TESTNAME} 是一个目标，将其安装到 ${CMAKE_INSTALL_BINDIR}
            install(TARGETS ${TESTNAME}
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                COMPONENT tests)
        else()
            # 安装 tests 组件时，${TESTNAME} 是一个程序，将其安装到 ${CMAKE_INSTALL_BINDIR}
            install(PROGRAMS "${TESTCMD}"
                TYPE BIN
                COMPONENT tests)
        endif()
    endmacro()
endif()

# Create build tree
add_subdirectory(common)
add_subdirectory(imgproc)