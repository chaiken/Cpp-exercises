#include "gcd.h"
#include "gtest/gtest.h"

TEST(GCDTest, Correct) { EXPECT_EQ(4u, gcd(100, 8)); }

TEST(GCDTest, Correct2) { EXPECT_EQ(7u, gcd(21, 56)); }

TEST(GCDTest, Correct3) { EXPECT_EQ(2u, gcd(5632, 6)); }

TEST(GCDTest, CorrectWith1) { EXPECT_EQ(1u, gcd(100, 1)); }

TEST(GCDTest, Symmetric) { EXPECT_EQ(gcd(8, 100), gcd(100, 8)); }

TEST(GCDTest, Match) { EXPECT_EQ(gcd(100, 8), gcd2(100, 8)); }

TEST(GCDTest, ZeroCase) { EXPECT_EQ(0u, gcd(0, 100)); }
