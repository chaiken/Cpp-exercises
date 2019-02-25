#include "new_clock.h"

#include <cassert>
#include <ctime>

#include "gtest/gtest.h"

using namespace std;
using namespace std::chrono;

namespace new_clock {
namespace testing {

class NewClockTest : public ::testing::Test {
public:
  NewClockTest() {
    newc = new NewClock();
    assert(nullptr != newc);
  }
  ~NewClockTest() { delete newc; }

  NewClock *newc;
};

// Both test SetUp() method or constructor need TEST_F.
TEST_F(NewClockTest, OutputTest) {
  ASSERT_EQ(0l, newc->GetSeconds());
  ostringstream output;
  output << (*newc);
  EXPECT_EQ("Wed Dec 31 16:00:00 1969\n", output.str());
  cout << output.str();
}

TEST_F(NewClockTest, OperatorsTest) {
  ASSERT_EQ(0l, newc->GetSeconds());
  newc->operator++(1);
  ostringstream output;
  output << (*newc);
  EXPECT_EQ("Wed Dec 31 16:00:01 1969\n", output.str());
  ASSERT_EQ(1l, newc->GetSeconds());

  // https://stackoverflow.com/questions/20731/how-do-you-clear-a-stringstream-variable
  output.str(std::string());
  newc->operator++(100);
  output << (*newc);
  EXPECT_EQ("Wed Dec 31 16:01:41 1969\n", output.str());
  ASSERT_EQ(101l, newc->GetSeconds());

  output.str(std::string());
  newc->operator--(101);
  output << (*newc);
  EXPECT_EQ("Wed Dec 31 16:00:00 1969\n", output.str());
}

TEST_F(NewClockTest, ParameterConstructor) {
  NewClock nc(1);
  ostringstream output;
  output << nc;
  EXPECT_EQ("Wed Dec 31 16:00:01 1969\n", output.str());
  ASSERT_EQ(1l, nc.GetSeconds());
  nc.operator--(1);
  ASSERT_EQ(0l, nc.GetSeconds());
}

} // namespace testing
} // namespace new_clock
