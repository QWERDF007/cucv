# 添加 src 目录下所有 cpp 文件到 ${SOURCES}
file(GLOB SOURCES src/*.cpp)

add_library(cucv_core SHARED
    ${SOURCES}
)

# cucv 包含目录
# $<BUILD_INTERFACE:…> 和 $<INSTALL_INTERFACE:…> 是生成表达式
# 根据是在构建树还是安装树中使用而返回不同的值
target_include_directories(cucv_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 添加一个只包含接口头文件的库 cucv_core_headers
add_library(cucv_core_headers INTERFACE)

# 添加包含目录 include 和 ${CMAKE_CURRENT_BINARY_DIR}/include 到 cucv_core_headers
target_include_directories(cucv_core_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include)

# 设置版本相关变量，生成 VersionDef.h 和 VersionUtils.h，
# 并安装到 ${CMAKE_INSTALL_INCLUDEDIR}/${incpath}
configure_version(cucv_core CUCV cucv ${PROJECT_VERSION})
# 设置 target 版本号和减少可执行文件的大小
setup_dso(cucv_core ${PROJECT_VERSION})

# 这里用 PUBLIC 的原因是让头文件能找到这个宏的定义
target_compile_definitions(cucv_core PUBLIC -DCUCV_EXPORTS=1)

# *************
# 编译完成后安装
# *************

# COMPONENT 表示要将安装的文件分配到一个组件中，这样可以在打包时选择性地安装不同的组件。组件名可以是任意字符串
# LIBRARY 表示要安装库文件和名字链接（如果有）到指定的目录中。
# NAMELINK_COMPONENT 表示要将名字链接分配到另一个组件中，这样可以在打包时选择性地安装开发者或用户所需的文件。
# 某些平台上会动态库有符号链接 lib<name>.so -> lib<name>.so.1
# lib<name>.so.1 才是真正的动态库，"namelink" 允许链接器通过 -l<name> 找到动态库
install(TARGETS cucv_core
    COMPONENT lib
    LIBRARY NAMELINK_COMPONENT dev
)


# 安装 include/cucv/core 目录下的所有文件和子目录到 ${CMAKE_INSTALL_INCLUDEDIR}/cucv 目录下
# 只安装匹配 “.h" 或 ".hpp” 模式的文件，忽略其他类型的文件
# 将这些文件归属于 dev 组件，这样可以在运行 cmake_install.cmake 脚本时通过 COMPONENT 变量来选择性地安装或不安装它们
install(DIRECTORY include/cucv/core
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cucv
    COMPONENT dev
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")