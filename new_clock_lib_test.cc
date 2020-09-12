#include "new_clock.h"

#include <cassert>
#include <ctime>

#include "gtest/gtest.h"

using namespace std;
using namespace std::chrono;

namespace new_clock {
namespace local_testing {

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
  ASSERT_EQ(0l, newc->GetSeconds(CLOCK_REALTIME));
  ostringstream output;
  output << (*newc);
  EXPECT_EQ("Wed Dec 31 16:00:00 1969\n", output.str());
  cout << output.str();
}

TEST_F(NewClockTest, OperatorsTest) {
  ASSERT_EQ(0l, newc->GetSeconds(CLOCK_REALTIME));
  newc->operator++(1);
  ostringstream output;
  output << (*newc);
  EXPECT_EQ("Wed Dec 31 16:00:01 1969\n", output.str());
  ASSERT_EQ(1l, newc->GetSeconds(CLOCK_REALTIME));

  // https://stackoverflow.com/questions/20731/how-do-you-clear-a-stringstream-variable
  output.str(std::string());
  newc->operator++(100);
  output << (*newc);
  EXPECT_EQ("Wed Dec 31 16:01:41 1969\n", output.str());
  ASSERT_EQ(101l, newc->GetSeconds(CLOCK_REALTIME));

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
  ASSERT_EQ(1l, nc.GetSeconds(CLOCK_REALTIME));
  nc.operator--(1);
  ASSERT_EQ(0l, nc.GetSeconds(CLOCK_REALTIME));
}

//  using  NewClockDeathTest = NewClockTest;

TEST(NewClockDeathTest, IllegalClock) {
  NewClock nc(1);
  EXPECT_EXIT(nc.GetSeconds(CLOCK_TAI + 1), testing::KilledBySignal(SIGABRT),
              "Unexpected error: Invalid argument.");
}

} // namespace local_testing
} // namespace new_clock
