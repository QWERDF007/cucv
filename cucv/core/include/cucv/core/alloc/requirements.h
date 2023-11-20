#pragma once

#include <stdint.h>

#define MAX_MEM_REQUIREMENTS_LOG2_BLOCK_SIZE (32)

#define MAX_MEM_REQUIREMENTS_BLOCK_SIZE (((int64_t)1) << MAX_MEM_REQUIREMENTS_LOG2_BLOCK_SIZE)

namespace cucv
{

/** 存储内存资源需求 */
typedef struct MemRequirementsRec
{
    /** 给定大小所需的总块数，以字节为单位.
     * 索引是 log2(size)
     * 每个块都对齐到 size
     * */
    int64_t num_blocks[MAX_MEM_REQUIREMENTS_LOG2_BLOCK_SIZE];
} MemRequirements_t;

/** 存储资源需求 */
typedef struct RequirementsRec
{
    MemRequirements_t cudaMem;       /*< 设备内存 memory */
    MemRequirements_t hostMem;       /*< 主机内存 */
    MemRequirements_t hostPinnedMem; /*< 主机锁页内存 */
} Requirements_t;

} // namespace cucv
