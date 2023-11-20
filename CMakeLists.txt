cmake_minimum_required(VERSION 3.20)

project(cucv
    LANGUAGES C CXX
    VERSION 0.0.1
    DESCRIPTION "cuda image process"
)

# Make sure the cuda host compiler agrees with what we're using,
# unless user overwrites it (at their own risk).
if(NOT CMAKE_CUDA_HOST_COMPILER)
    set(CMAKE_CUDA_HOST_COMPILER "${CMAKE_CXX_COMPILER}")
endif()

enable_language(CUDA)

# # 如果没有设置安装前缀
# if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
# set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/bin")
# endif()

# 设置安装位置
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/cucv_install")

# Options to configure the build tree =======
# 是否编译测试用例
option(BUILD_TESTS "Enable testsuite" ON)
option(BUILD_DOCS "Build Doc" ON)

# 是否开启运行时检查器
option(ENABLE_SANITIZER "Enabled sanitized build" OFF)

# Configure build tree ======================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ConfigVersion)
include(ConfigBuildTree)
include(ConfigCompiler)
include(ConfigCUDA)
include(ConfigOpenCV)

# Define the build tree ====================
add_subdirectory(3rdparty EXCLUDE_FROM_ALL)
add_subdirectory(cucv)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_DOCS)
    add_subdirectory(docs)
endif()

# if(BUILD_SAMPLES)
#     add_subdirectory(samples)
# endif()


# 打印构建配置
message(STATUS "")
include(PrintConfig)
