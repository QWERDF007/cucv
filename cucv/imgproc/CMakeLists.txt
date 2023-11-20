add_subdirectory(src/legacy)


# 添加 src 目录下所有 cpp 文件到 ${SOURCES}
file(GLOB SOURCES src/*.cpp)

# 添加一个动态库 cucv_imgproc
add_library(cucv_imgproc SHARED
    ${SOURCES}
)

# cucv_imgproc 链接依赖库
target_link_libraries(cucv_imgproc
    PRIVATE
        cucv_legacy
        cucv_core
)


# cucv_imgproc 包含目录
# $<BUILD_INTERFACE:…> 和 $<INSTALL_INTERFACE:…> 是生成表达式
# 根据是在构建树还是安装树中使用而返回不同的值
target_include_directories(cucv_imgproc
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 添加一个只包含接口头文件的库 cucv_headers
add_library(cucv_imgproc_headers INTERFACE)

# 添加包含目录 ${CMAKE_CURRENT_SOURCE_DIR}/include 和 ${CMAKE_CURRENT_BINARY_DIR}/include 到 cucv_headers
target_include_directories(cucv_imgproc_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include)

# configure_version(cucv_imgproc CUCV cucv ${PROJECT_VERSION})
setup_dso(cucv_imgproc ${PROJECT_VERSION})

# # 添加一个宏 CVCUDA_EXPORTING，并传递给代码
# target_compile_definitions(cvcuda PRIVATE -DCVCUDA_EXPORTING=1)


# *************
# 编译完成后安装
# *************

# 将 cucv 导出为 cucv
# COMPONENT 表示要将安装的文件分配到一个组件中，这样可以在打包时选择性地安装不同的组件。组件名可以是任意字符串
# LIBRARY 表示要安装库文件和名字链接（如果有）到指定的目录中。
# NAMELINK_COMPONENT 表示要将名字链接分配到另一个组件中，这样可以在打包时选择性地安装开发者或用户所需的文件。
# 某些平台上会动态库有符号链接 lib<name>.so -> lib<name>.so.1
# lib<name>.so.1 才是真正的动态库，"namelink" 允许链接器通过 -l<name> 找到动态库
install(TARGETS cucv_imgproc
    # EXPORT cucv
    COMPONENT lib
    LIBRARY NAMELINK_COMPONENT dev
)

# 安装 include/cucv/imgproc 目录下的所有文件和子目录到 ${CMAKE_INSTALL_INCLUDEDIR}/cucv 目录下
# 只安装匹配 “.h" 或 ".hpp” 模式的文件，忽略其他类型的文件
# 将这些文件归属于 dev 组件，这样可以在运行 cmake_install.cmake 脚本时通过 COMPONENT 变量来选择性地安装或不安装它们
install(DIRECTORY include/cucv/imgproc
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cucv
    COMPONENT dev
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

# # 在安装 dev 组件时可以通过 find_package 方法进行引用
# # 1. 声明版本信息
# # 包含一些辅助函数，用于创建配置文件
# include(CMakePackageConfigHelpers)

# # 创建版本声明文件 cucv-config-version.cmake，声明兼容性为 SameMajorVersion
# # 只有当包的主版本号与请求的主版本号相同时，才认为是兼容的。
# write_basic_package_version_file(cucv-config-version.cmake
#     COMPATIBILITY SameMajorVersion)

# # 2. 在安装 dev 组件时，将编译安装后的目标导出到 ${CMAKE_INSTALL_LIBDIR}/cmake/cucv/cucv-config.cmake
# install(EXPORT cucv
#     DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cucv"
#     FILE cucv-config.cmake
#     COMPONENT dev
# )

# # 下面两段是处理安装目录是 link 时的 prefix 错误 https://gitlab.kitware.com/cmake/cmake/-/issues/23563
# # 安装 dev 组件时调用给定的命令将 CUCV_CONFIG_PATH 设置为
# # ${DESTDIR}/{CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/cucv/cucv-config.cmake
# install(CODE "set(CUCV_CONFIG_PATH \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/cucv/cucv-config.cmake\")"
#     COMPONENT dev)

# # 在安装 dev 组件时，将 ${CUCV_CONFIG_PATH} 所指文件内容读进变量 contents 中
# # contents 中的内容替换掉
# # 将替换后的内容重新写入文件 ${CUCV_CONFIG_PATH}
# install(CODE [=[
#     file(READ "${CUCV_CONFIG_PATH}" contents)
#     string(REPLACE "get_filename_component(_IMPORT_PREFIX \"\${CMAKE_CURRENT_LIST_FILE}\" PATH)" 
#         [[get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH) get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" REALPATH)]] 
#         contents "${contents}")
#     file(WRITE "${CUCV_CONFIG_PATH}" "${contents}")
#     ]=]
#     COMPONENT dev)

# # 在安装 dev 组件时将 ${CMAKE_CURRENT_BINARY_DIR}/cucv-config-version.cmake
# # 安装到 ${CMAKE_INSTALL_LIBDIR}/cmake/cucv
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cucv-config-version.cmake
#     DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cucv"
#     COMPONENT dev)