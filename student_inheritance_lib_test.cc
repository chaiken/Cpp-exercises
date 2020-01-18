#include "student_inheritance.h"

#include "gtest/gtest.h"

namespace student_inheritance {
namespace testing {

TEST(StudentInheritanceTest, StudentPrinting) {
  // The order is unnatural, but efficient struct packing requires it.
  struct student_details marvin_details(Year::kJunior, 123, 4.2, "Marvin");
  Student marvin(marvin_details);
  ::std::ostringstream oss;
  oss << marvin;
  EXPECT_EQ(oss.str(), "Name: Marvin, 123, Junior, 4.2\n");
}

TEST(StudentInheritanceTest, GradStudentPrinting) {
  struct student_details angela_details(Year::kGrad, 124, 2.4, "Angela");
  struct grad_student_extra angela_extra(
      Support::kRA, "Physics",
      "Anistropic Superconductivity in Graphite Intercalation Compounds");
  GradStudent angela(angela_details, angela_extra);

  ::std::ostringstream oss;
  student_inheritance::operator<<(oss, angela);
  EXPECT_EQ(oss.str(), "Name: Angela, 124, Grad, 2.4\n, Physics, Support: "
                       "Research assistant, Thesis: Anistropic "
                       "Superconductivity in Graphite Intercalation Compounds");
}

} // namespace testing
} // namespace student_inheritance
