# 添加一个静态库 cucv_test_common
add_library(cucv_test_common STATIC
    printers.cpp
)

target_include_directories(cucv_test_common
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(cucv_test_common
    PUBLIC
    cucv_imgproc_headers
)
