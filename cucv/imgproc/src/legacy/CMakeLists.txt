# 添加静态库 cucv_cuda
add_library(cucv_legacy
    STATIC
        copy.cu
)

# cucv_cuda 链接依赖库
target_link_libraries(cucv_legacy
    PUBLIC
        cucv_imgproc_headers
        CUDA::cudart_static
)