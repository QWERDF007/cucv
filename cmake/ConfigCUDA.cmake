# 将 ${CMAKE_CUDA_COMPILER_VERSION} 中的所有 `.` 替换成 `;`，然后存储到 ${CUDA_VERSION_LIST}
string(REPLACE "." ";" CUDA_VERSION_LIST ${CMAKE_CUDA_COMPILER_VERSION})

# 获取 ${CUDA_VERSION_LIST} 索引为 0 的值，存储到 ${CUDA_VERSION_MAJOR}
list(GET CUDA_VERSION_LIST 0 CUDA_VERSION_MAJOR)
list(GET CUDA_VERSION_LIST 1 CUDA_VERSION_MINOR)
list(GET CUDA_VERSION_LIST 2 CUDA_VERSION_PATCH)

find_package(CUDAToolkit ${CUDA_VERSION_MAJOR}.${CUDA_VERSION_MINOR} REQUIRED)

# 设置 CUDA 的语言标准和 C++ 的语言标准一样
set(CMAKE_CUDA_STANDARD ${CMAKE_CXX_STANDARD})

# 添加 CUDA 编译器选项，压缩所有的内核，以生成更小的可执行文件
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xfatbin=--compress-all")

# 确保编译 CUDA 代码时支持正确的 GPU 架构
if(NOT USE_CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES "$ENV{CUDAARCHS}")

    # 将构建成功的架构 添加到 ${CMAKE_CUDA_ARCHITECTURES}
    list(APPEND CMAKE_CUDA_ARCHITECTURES
        70-real # Volta  - gv100/Tesla
        75-real # Turing - tu10x/GeForce
        80-real # Ampere - ga100/Tesla
        86-real # Ampere - ga10x/GeForce
    )

    # CUDA 编译器版本大于 11.8 则添加下列架构到 ${CMAKE_CUDA_ARCHITECTURES}
    if(CMAKE_CUDA_COMPILER_VERSION VERSION_GREATER_EQUAL "11.8")
        list(APPEND CMAKE_CUDA_ARCHITECTURES
            89-real # Ada    - ad102/GeForce
            90-real # Hopper - gh100/Tesla
        )
    endif()

    # 将 70-virtual 添加到 ${CMAKE_CUDA_ARCHITECTURES}
    # compute_70：支持 fast fp16 和向前兼容
    list(APPEND CMAKE_CUDA_ARCHITECTURES 70-virtual)

    # 将 ${CMAKE_CUDA_ARCHITECTURES} 写入缓存，覆盖 CMake 的默认值
    # 否则会使用 nvcc 支持的老架构，并不是我们想要的
    set(CMAKE_CUDA_ARCHITECTURES "${CMAKE_CUDA_ARCHITECTURES}" CACHE STRING "CUDA architectures to build for" FORCE)
endif()
