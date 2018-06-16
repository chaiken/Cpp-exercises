#include "gcd_lib.h"
#include "gtest/gtest.h"

TEST(GCDTest, Match) { EXPECT_EQ(gcd(100, 8), gcd2(100, 8)); }

TEST(GCDTest, ZeroCase) { EXPECT_EQ(0u, gcd2(0, 100)); }
