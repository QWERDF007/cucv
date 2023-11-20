# ${CMAKE_CURRENT_LIST_FILE} 当前正在处理的列表文件的完整路径
# 获取 ${CMAKE_CURRENT_LIST_FILE} 文件的路径，赋给 ${config_version_script_path}
get_filename_component(config_version_script_path ${CMAKE_CURRENT_LIST_FILE} PATH)

# 包含用于从Git获取版本字符串的模块
include(GetGitRevisionDescription)

# 获取Git的引用和提交哈希，并赋值给 ${GIT_REFSPEC} 和 ${REPO_COMMIT}
get_git_head_revision(GIT_REFSPEC REPO_COMMIT)

# 设置 ${PROJECT_VERSION} 为 ${PROJECT_VERSION}${PROJECT_VERSION_SUFFIX}
set(PROJECT_VERSION "${PROJECT_VERSION}${PROJECT_VERSION_SUFFIX}")

# 定义一个函数 configure_version，接收四个参数
# 设置版本相关变量，生成 VersionDef.h 和 VersionUtils.h，
# 并安装到 ${CMAKE_INSTALL_INCLUDEDIR}/${incpath}
# target：目标库的名称
# LIBPREFIX：库的前缀
# incpath：头文件所在的目录
# VERSION_FULL：是完整的版本号。
function(configure_version target LIBPREFIX incpath VERSION_FULL)
    # 将 ${target} 转换为大写并赋值给 ${TARGET}
    string(TOUPPER "${target}" TARGET)

    # 使用正则表达式从 ${VERSION_FULL} 提取版本后缀（如果有的话），例如"-rc1"，并赋值给version_suffix变量
    string(REGEX MATCH "-(.*)$" version_suffix "${VERSION_FULL}")

    # 将正则表达式匹配到的第一个子串（即版本后缀）赋值给VERSION_SUFFIX变量
    set(VERSION_SUFFIX ${CMAKE_MATCH_1})

    # 使用正则表达式从 ${VERSION_FULL} 中提取所有数字，并将它们存储在一个列表 ${version_list} 中
    string(REGEX MATCHALL "[0-9]+" version_list "${VERSION_FULL}")

    # 从 ${version_list} 中获取第一元素（即主版本号）给 ${VERSION_MAJOR}
    # 主版本号 次版本号 修订号 微调号
    list(GET version_list 0 VERSION_MAJOR)
    list(GET version_list 1 VERSION_MINOR)
    list(GET version_list 2 VERSION_PATCH)

    # 获取 ${version_list} 的长度，赋给 ${num_version_components}
    list(LENGTH version_list num_version_components)

    # 判断 ${version_list} 是否有三个元素
    if(num_version_components EQUAL 3)
        # 设置 ${VERSION_TWEAK} 为0，表示没有微调号
        set(VERSION_TWEAK 0)
    elseif(num_version_components EQUAL 4)
        # 有四个元素则赋给 ${VERSION_TWEAK}
        list(GET version_list 3 VERSION_TWEAK)
    else()
        message(FATAL_ERROR "Version must have either 3 or 4 components")
    endif()

    # 计算 ${VERSION_MAJOR}*100 + ${VERSION_MINOR}，并赋给 VERSION_API_CODE
    math(EXPR VERSION_API_CODE "${VERSION_MAJOR}*100 + ${VERSION_MINOR}")

    # ${VERSION_FULL} 中的 "-"替换为 "_"，并赋值给 ${tmp}
    string(REPLACE "-" "_" tmp ${VERSION_FULL})

    # 设置 ${VERSION_BUILD} 为 ${tmp}-
    set(VERSION_BUILD "${tmp}-${CUCV_BUILD_SUFFIX}")

    # 通过 VersionDef.h.in 生成 VersionDef.h，并替换形如 @VAR@ 的变量，转义双引号和单引号
    configure_file(${config_version_script_path}/VersionDef.h.in include/${incpath}/VersionDef.h @ONLY ESCAPE_QUOTES)
    configure_file(${config_version_script_path}/VersionUtils.h.in include/${incpath}/detail/VersionUtils.h @ONLY ESCAPE_QUOTES)
    # configure_file(${config_version_script_path}/VersionDef.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/${incpath}/VersionDef.h @ONLY ESCAPE_QUOTES)
    # configure_file(${config_version_script_path}/VersionUtils.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/${incpath}/detail/VersionUtils.h @ONLY ESCAPE_QUOTES)

    # 将 ${VERSION_FULL} 赋值给 ${LIBPREFIX}_VERSION_FULL，并将它存储在缓存中，以便其他CMake脚本可以访问。
    # INTERNAL选项表示这个变量不会出现在CMake GUI中
    set(${LIBPREFIX}_VERSION_FULL ${VERSION_FULL} CACHE INTERNAL "${TARGET} full version")
    set(${LIBPREFIX}_VERSION_MAJOR ${VERSION_MAJOR} CACHE INTERNAL "${TARGET} major version")
    set(${LIBPREFIX}_VERSION_MINOR ${VERSION_MINOR} CACHE INTERNAL "${TARGET} minor version")
    set(${LIBPREFIX}_VERSION_PATCH ${VERSION_PATCH} CACHE INTERNAL "${TARGET} patch version")
    set(${LIBPREFIX}_VERSION_TWEAK ${VERSION_TWEAK} CACHE INTERNAL "${TARGET} tweak version")
    set(${LIBPREFIX}_VERSION_SUFFIX ${VERSION_SUFFIX} CACHE INTERNAL "${TARGET} version suffix")
    set(${LIBPREFIX}_VERSION_API ${VERSION_MAJOR}.${VERSION_MINOR} CACHE INTERNAL "${TARGET} API version")
    set(${LIBPREFIX}_VERSION_API_CODE ${VERSION_API_CODE} CACHE INTERNAL "${TARGET} API code")
    set(${LIBPREFIX}_VERSION_BUILD ${VERSION_BUILD} CACHE INTERNAL "${TARGET} build version")

    # ${CMAKE_CURRENT_BINARY_DIR} 是 target 的编译目录
    # 添加包含目录 ${CMAKE_CURRENT_BINARY_DIR}/include
    # PUBLIC：表示后面的包含目录既对目标本身有效，也对链接到目标的其他目标有效
    # $<BUILD_INTERFACE:...> 表示只在构建阶段有效的包含目录
    target_include_directories(${target}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    )

    # 安装 dev 组件时，复制 ${CMAKE_CURRENT_BINARY_DIR}/include/${incpath}/VersionDef.h
    # 到 ${CMAKE_INSTALL_INCLUDEDIR}/${incpath} 目录下
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/${incpath}/VersionDef.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${incpath}
        COMPONENT dev)
    
    # ${CMAKE_CURRENT_BINARY_DIR} 是 target 的编译目录
    # 安装 dev 组件时，复制 ${CMAKE_CURRENT_BINARY_DIR}/include/${incpath}/detail/VersionUtils.h
    # 到 ${CMAKE_INSTALL_INCLUDEDIR}/${incpath}/detail 目录下
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/${incpath}/detail/VersionUtils.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${incpath}/detail
        COMPONENT dev)
endfunction()
