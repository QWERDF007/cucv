#include "gtest/gtest.h"
#include <cucv/core/version.h>


TEST(VersionTests, version_numeric)
{
    EXPECT_EQ(CUCV_VERSION, cucvGetVersion());
}
