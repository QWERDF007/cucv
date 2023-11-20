# 设置 Doxygen 输入/输出
find_package(Doxygen)

# 获取 cucv_core 的 SOURCE_DIR 属性，赋给 ${CORE_SRCPATH}
get_target_property(CORE_SRCPATH cucv_core SOURCE_DIR)
# 获取 cucv_imgproc 的 SOURCE_DIR 属性，赋给 ${IMGPROC_SRCPATH}
get_target_property(IMGPROC_SRCPATH cucv_imgproc SOURCE_DIR)

# 设置 DOXYGEN 的相关变量
set(DOXYGEN_INPUT_DIR "${CORE_SRCPATH}/include ${IMGPROC_SRCPATH}/include")
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# 复制 ${DOXYFILE_IN} 到 ${DOXYFILE_OUT}，并替换掉 @@ 所包含的变量
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# 仅在 Doxyfile 更改时重新生成 Doxygen
# 创建目录 ${DOXYGEN_OUTPUT_DIR}
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
# 添加一个自定义命令
# 指定该命令的输出文件为 ${DOXYGEN_INDEX_FILE}
# 指定该命令的执行程序为 ${DOXYGEN_EXECUTABLE}，并传入 ${DOXYFILE_OUT} 作为参数
# 指定该命令的主要依赖项为 ${DOXYFILE_OUT} 和 ${DOXYFILE_IN} 文件，如果这些文件发生变化，那么该命令将重新执行
# 指定该命令的注释为 "Generating doxygen xml"，在构建时显示在控制台上
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                   MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                   COMMENT "Generating doxygen xml")

add_custom_target(cucv_doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

install(DIRECTORY ${DOXYGEN_OUTPUT_DIR} DESTINATION docs)