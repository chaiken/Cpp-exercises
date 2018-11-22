#include "generic_stack.h"

#include "gtest/gtest.h"

using namespace std;

namespace generic_stack {
namespace testing {

class GenericStackTest : public ::testing::Test {
public:
  GenericStackTest() {
    double data[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    double reversed[] = {6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};
    GenericStack stack1(data, 7);
    GenericStack stack2(reversed, 7);
  }

  GenericStack *stack1, *stack2;
};

TEST_F(GenericStackTest, CtorWorks) {
  SetUp();
  cout << *stack1;
  cout << *stack2;
  EXPECT_EQ(0.0, stack1->DoublePop());
  EXPECT_EQ(6.0, stack2->DoublePop());
}

} // namespace testing
} // namespace generic_stack
