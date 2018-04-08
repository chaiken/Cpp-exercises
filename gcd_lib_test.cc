#include "gtest/gtest.h"
#include "gcd_lib.h"

TEST(GCDTest, Match) {
  EXPECT_EQ(gcd(100,8), gcd2(100,8));
}

TEST(GCDTest, ZeroCase) {
  EXPECT_EQ(0u, gcd2(0,100));
}
