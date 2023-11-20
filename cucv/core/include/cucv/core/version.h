/**
 * @file Version.h
 *
 * Functions and structures for handling cucv library version.
 */

#pragma once

#include <cucv/VersionDef.h>

//! @addtogroup core_utils
//! @{

inline uint32_t cucvGetVersion()
{
    return CUCV_VERSION;
}

//! @} core_utils
