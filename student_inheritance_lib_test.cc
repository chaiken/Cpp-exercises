#include "student_inheritance.h"

#include "gtest/gtest.h"

namespace student_inheritance {
namespace testing {

const struct student_details marvin_details(Year::kJunior, 123, 4.2, "Marvin");
const struct student_details angela_details(Year::kGrad, 124, 2.4, "Angela");
const struct grad_student_extra angela_extra(
    Support::kRA, "Physics",
    "Anistropic Superconductivity in Graphite Intercalation Compounds");

class StudentInheritanceTest : public ::testing::Test {
public:
  StudentInheritanceTest() {
    marvin = new Student(marvin_details);
    angela = new GradStudent(angela_details, angela_extra);
  }
  ~StudentInheritanceTest() {
    delete marvin;
    delete angela;
  }
  const Student *marvin;
  const GradStudent *angela;
};

TEST_F(StudentInheritanceTest, StudentPrinting) {
  ::std::ostringstream oss;
  oss << *marvin;
  EXPECT_EQ(oss.str(), "Name: Marvin, 123, Junior, 4.2\n");
}

TEST_F(StudentInheritanceTest, GradStudentPrinting) {
  ::std::ostringstream oss;
  student_inheritance::operator<<(oss, *angela);
  EXPECT_EQ(oss.str(), "Name: Angela, 124, Grad, 2.4\n, Physics, Support: "
                       "Research assistant, Thesis: Anistropic "
                       "Superconductivity in Graphite Intercalation Compounds");
}

TEST_F(StudentInheritanceTest, CrossMembership) {
  // Apparently the objects declared in SetUp() are by default const, and trying
  // to modify them results in compilation errors.
  Student Fred(marvin_details);
  GradStudent Sue(angela_details, angela_extra);
  Sue.setGPA(Fred.gpa() + 1.0);
  ::std::ostringstream oss;
  student_inheritance::operator<<(oss, Sue);
  EXPECT_EQ(oss.str(), "Name: Angela, 124, Grad, 5.2\n, Physics, Support: "
                       "Research assistant, Thesis: Anistropic "
                       "Superconductivity in Graphite Intercalation Compounds");
}

/*  Apparently gtest doesn't trap these errors?

using StudentInheritanceDeathTest = StudentInheritanceTest;

TEST_F(StudentInheritanceDeathTest, IllegalGradYear) {
const struct student_details bad_details(Year::kFresh, 124, 2.4, "Angela");
EXPECT_DEATH(GradStudent badangela(bad_details, angela_extra),
             "Invalid argument");
}

TEST_F(StudentInheritanceDeathTest, IllegalStudentYear) {
const struct student_details bad_details(Year::kGrad, 123, 4.2, "Marvin");
EXPECT_DEATH(Student badmarvin(bad_details), "Invalid argument");
}
TEST_F(StudentInheritanceDeathTest, IllegalSupport) {
const struct grad_student_extra bad_extra(
    static_cast<Support>(-1), "Physics",
    "Anistropic Superconductivity in Graphite Intercalation Compounds");
EXPECT_DEATH(GradStudent badangela(angela_details, bad_extra),
             "Invalid argument");
             }*/

} // namespace testing
} // namespace student_inheritance
