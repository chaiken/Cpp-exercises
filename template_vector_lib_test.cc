#include "template_vector.h"

#include "complex.h"
#include "polynomial.h"

#include <chrono>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"

using namespace complex;
using namespace polynomial;
using namespace std;
using namespace chrono;

namespace template_vect {
namespace testing {

TEST(TemplateVectorDeathTest, ZeroLengthVector) {
  EXPECT_DEATH(TemplateVector<int>(0), "Invalid argument");
}

TEST(TemplateVectorTest, DefaultCtor) {
  // These both hit Most Vexing Parse.
  // TemplateVector<int> tv();
  // TemplateVector<int> tv[2]();
  TemplateVector<int> tv(10);
  ASSERT_EQ(9, tv.ub());
}

TEST(TemplateVectorTest, ArrayCtor) {
  Complex carr[]{Complex(0.0, 0.0), Complex(1.0, 1.0), Complex(2.0, 2.0),
                 Complex(3.0, 3.0), Complex(4.0, 4.0)};
  TemplateVector<Complex> tv(carr, 5);
  int i = 0;
  for (TemplateVector<Complex>::iterator tvit = tv.begin(); tvit != tv.end();
       tvit++, i++) {
    EXPECT_EQ(carr[i], *tvit);
  }
}

TEST(TemplateVectorTest, VectorCtor) {
  vector<double> dv{{1.0, 2.0, 3.0, 4.0, 5.0}};
  TemplateVector<double> tv(dv);
  int i = 0;
  for (double x : dv) {
    EXPECT_EQ(x, tv[i]);
    i++;
  }
}

TEST(TemplateVectorTest, MoveCtor) {
  Complex carr[]{Complex(0.0, 0.0), Complex(1.0, 1.0), Complex(2.0, 2.0),
                 Complex(3.0, 3.0), Complex(4.0, 4.0)};
  TemplateVector<Complex> tv(carr, 5);
  TemplateVector<Complex> tv2(move(tv));
  int i = 0;
  for (TemplateVector<Complex>::iterator tvit = tv2.begin(); tvit != tv2.end();
       tvit++, i++) {
    EXPECT_EQ(carr[i], *tvit);
  }
}

TEST(TemplateVectorTest, MoveAssignment) {
  vector<double> dv{{1.0, 2.0, 3.0, 4.0, 5.0}};
  TemplateVector<double> tv1(dv), tv2(5);
  int i = 0;
  for (double x : dv) {
    ASSERT_NE(x, tv2[i]);
    i++;
  }
  tv2 = move(tv1);
  i = 0;
  for (double x : dv) {
    EXPECT_EQ(x, tv2[i]);
    i++;
  }
}

TEST(TemplateVectorTest, AssignmentSizeMismatch) {
  vector<double> vec1{{-3.0, -2.0, -1.0}}, vec2{{1.0, 2.0, 3.0, 4.0, 5.0}};
  TemplateVector<double> tv1(vec1), tv2(vec2);
  ASSERT_NE(tv1.ub(), tv2.ub());
  EXPECT_EQ(4, tv2.ub());
  tv2 = move(tv1);
  EXPECT_EQ(2, tv2.ub());
}

TEST(TemplateVectorTest, PrinterTest) {
  Complex carr[]{Complex(0.0, 0.0), Complex(1.0, 1.0), Complex(2.0, 2.0),
                 Complex(3.0, 3.0), Complex(4.0, 4.0)};
  TemplateVector<Complex> tv(carr, 5);
  ostringstream out, out2;
  out << tv;
  out2 << "0 + 0i, 1 + 1i, 2 + 2i, 3 + 3i, 4 + 4i, ";
  EXPECT_EQ(out2.str(), out.str());
}

TEST(TemplateVectorTest, EqualityTest) {
  vector<double> dv{{1.0, 2.0, 3.0, 4.0, 5.0}};
  const TemplateVector<double> tv1(dv);
  ASSERT_TRUE(tv1 == TemplateVector<double>(dv));
  ASSERT_FALSE(tv1 == TemplateVector<double>(5));
  vector<int> iv{{1, 2, 3, 4, 5}};
  ASSERT_FALSE(tv1 == TemplateVector<int>(iv));
  ASSERT_FALSE(tv1 == TemplateVector<double>(vector<double>{{2.0, 3.0, 4.0}}));
}

TEST(TemplateVectorDeathTest, AssignmentFailsSize) {
  vector<char> charvec{{'a', 'b', 'c', 'd', 'e'}};
  vector<int> intvec{{83, 84, 85}};
  TemplateVector<char> cvec(charvec);
  TemplateVector<int> ivec(intvec);
  EXPECT_DEATH(tvassign(cvec, ivec),
               "Cannot assign a TemplateVector to one of another size.");
}

TEST(TemplateVectorTest, AssignmentWorks) {
  vector<char> charvec{{'a', 'b', 'c', 'd', 'e'}};
  vector<int> intvec{{83, 84, 85, 86, 87}};
  TemplateVector<char> cvec(charvec);
  TemplateVector<int> ivec(intvec);
  cout << "Before: " << endl;
  cout << "cvec type name: " << typeid(cvec).name() << endl;
  cout << "ivec type name: " << typeid(ivec).name() << endl;
  cout << "cvec " << cvec << endl;
  cout << "ivec " << ivec << endl;
  cout << endl;
  tvassign(cvec, ivec);
  cout << "After: " << endl;
  cout << "cvec type name: " << typeid(cvec).name() << endl;
  cout << "cvec " << cvec << endl;
  cout << endl;
  int i = 0;
  for (TemplateVector<char>::iterator tvit = cvec.begin(); tvit != cvec.end();
       tvit++, i++) {
    EXPECT_EQ(static_cast<char>(intvec[i]), *tvit);
  }
}

TEST(TemplateVectorTest, SwapConversionTest) {
  vector<char> charvec{{'a', 'b', 'c', 'd', 'e'}};
  vector<int> intvec{{83, 84, 85, 86, 87}};
  TemplateVector<char> cvec(charvec);
  TemplateVector<int> ivec(intvec);
  cout << "Before: " << endl;
  cout << "cvec type name: " << typeid(cvec).name() << endl;
  cout << "ivec type name: " << typeid(ivec).name() << endl;
  cout << "cvec " << cvec << endl;
  cout << "ivec " << ivec << endl;
  cout << endl;
  tvswap(cvec, ivec);
  cout << "After: " << endl;
  cout << "cvec type name: " << typeid(cvec).name() << endl;
  cout << "cvec " << cvec << endl;
  cout << "ivec " << ivec << endl;
  cout << endl;
  {
    int i = 0;
    for (TemplateVector<char>::iterator tvit = cvec.begin(); tvit != cvec.end();
         tvit++, i++) {
      EXPECT_EQ(static_cast<char>(intvec[i]), *tvit);
    }
  }
  {
    int i = 0;
    for (TemplateVector<int>::iterator tvit = ivec.begin(); tvit != ivec.end();
         tvit++, i++) {
      EXPECT_EQ(static_cast<int>(charvec[i]), *tvit);
    }
  }
}

TEST(TemplateVectorTest, SortTest) {
  Complex carr[]{Complex(1.0, 1.0), Complex(3.0, 3.0), Complex(2.0, 2.0),
                 Complex(0.0, 0.0), Complex(4.0, 4.0), Complex(1.0, 1.0),
                 Complex(3.0, 3.0), Complex(2.0, 2.0), Complex(0.0, 0.0),
                 Complex(4.0, 4.0), Complex(1.0, 1.0), Complex(3.0, 3.0),
                 Complex(2.0, 2.0), Complex(0.0, 0.0), Complex(4.0, 4.0),
                 Complex(1.0, 1.0), Complex(3.0, 3.0), Complex(2.0, 2.0),
                 Complex(0.0, 0.0), Complex(4.0, 4.0), Complex(1.0, 1.0),
                 Complex(3.0, 3.0), Complex(2.0, 2.0), Complex(0.0, 0.0),
                 Complex(4.0, 4.0), Complex(1.0, 1.0), Complex(3.0, 3.0),
                 Complex(2.0, 2.0), Complex(0.0, 0.0), Complex(4.0, 4.0)};
  TemplateVector<Complex> tv(carr, 25), tv2(carr, 25);
  cout << "Start: " << tv << endl;

  system_clock::time_point start = system_clock::now();
  tvsort(tv);
  system_clock::time_point stop = system_clock::now();
  cout << endl << "Result: " << tv << endl;

  Complex carr2[]{Complex(0.0, 0.0), Complex(0.0, 0.0), Complex(0.0, 0.0),
                  Complex(0.0, 0.0), Complex(0.0, 0.0), Complex(1.0, 1.0),
                  Complex(1.0, 1.0), Complex(1.0, 1.0), Complex(1.0, 1.0),
                  Complex(1.0, 1.0), Complex(2.0, 2.0), Complex(2.0, 2.0),
                  Complex(2.0, 2.0), Complex(2.0, 2.0), Complex(2.0, 2.0),
                  Complex(3.0, 3.0), Complex(3.0, 3.0), Complex(3.0, 3.0),
                  Complex(3.0, 3.0), Complex(3.0, 3.0), Complex(4.0, 4.0),
                  Complex(4.0, 4.0), Complex(4.0, 4.0), Complex(4.0, 4.0),
                  Complex(4.0, 4.0)};
  TemplateVector<Complex> tv3(carr2, 25);
  cout << endl << "Test: " << tv3 << endl;
  EXPECT_TRUE(tv == tv3);
  cout << "Bisecting sort duration: "
       << duration_cast<microseconds>(stop - start).count() * 1e-3 << " ms."
       << endl;

  start = system_clock::now();
  sort(tv2.begin(), tv2.end());
  stop = system_clock::now();
  EXPECT_TRUE(tv2 == tv3);
  cout << "::std::sort duration: "
       << duration_cast<microseconds>(stop - start).count() * 1e-3 << " ms."
       << endl;

  ::std::array<int, 3> expon = {{1, 2, 3}};
  ::std::array<double, 3> coeffs = {{1.0, 2.0, 3.0}};
  ::std::array<int, 3> expon1 = {{-1, -2, -3}};
  ::std::array<double, 3> coeffs1 = {{1.0, 2.0, 3.0}};
  ::std::array<int, 3> expon2 = {{-1, -2, -3}};
  ::std::array<double, 3> coeffs2 = {{-1.0, -2.0, -3.0}};
  ::std::array<int, 3> expon3 = {{1, 2, 3}};
  ::std::array<double, 3> coeffs3 = {{-1.0, -2.0, -3.0}};
  ::std::array<int, 3> expon4 = {{-2, -4, -6}};
  ::std::array<double, 3> coeffs4 = {{-1.0, -2.0, -3.0}};
  Polynomial parr[]{Polynomial(coeffs, expon), Polynomial(coeffs1, expon1),
                    Polynomial(coeffs2, expon2), Polynomial(coeffs3, expon3),
                    Polynomial(coeffs4, expon4)};
  TemplateVector<Polynomial> tv4(parr, 5);
  TemplateVector<Polynomial> tv5(parr, 5);
  EXPECT_TRUE(tv4 == tv5);
  // template_vector_lib_test.cc:234:42: required from here
  // /usr/include/c++/9/bits/predefined_ops.h:43:23: error: no match for
  // ‘operator<’ (operand types are ‘polynomial::Polynomial’ and
  // ‘polynomial::Polynomial’)
  //  sort(move(tv4).begin(), move(tv4).end());
  //  EXPECT_FALSE(tv4 == tv5);
}

} // namespace testing
} // namespace template_vect