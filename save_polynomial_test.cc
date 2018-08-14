
TEST(PolynomialTest, SyncSortTest) {
  int expon[] = {1, 2, 3};
  double coeffs[] = {1.0, 2.0, 3.0};
  SyncSortTwoArrays(3, &coeffs, &expon);
  ASSERT_EQ(expon[0], 1);
  ASSERT_EQ(expon[2], 3);
  ASSERT_EQ(coeffs[0], 1);
  ASSERT_EQ(coeffs[2], 3);
  int expon1[] = {3, 2, 1};
  double coeffs1[] = {3.0, 2.0, 1.0};
  SyncSortTwoArrays(3, &coeffs1, &expon1);
  ASSERT_EQ(expon1[0], 1);
  ASSERT_EQ(expon1[2], 3);
  ASSERT_EQ(coeffs1[0], 1);
  ASSERT_EQ(coeffs1[2], 3);
}
