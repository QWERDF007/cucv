# 创建动态库，所有它依赖的项目开启【地址无关代码】
# 指可在主存储器中任意位置正确地运行，而不受其绝对地址影响的一种机器码
# PIC广泛使用于共享库，使得同一个库中的代码能够被加载到不同进程的地址空间中
# https://en.wikipedia.org/wiki/Position-independent_code

set(CMAKE_POSITION_INDEPENDENT_CODE on)

if(LTO_ENABLED)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION on)
endif()

add_subdirectory(core)
add_subdirectory(imgproc)