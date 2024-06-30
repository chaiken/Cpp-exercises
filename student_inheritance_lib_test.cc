#include "student_inheritance.hh"

#include "gtest/gtest.h"

using namespace std;

namespace student_inheritance {
namespace local_testing {

const struct student_details marvin_details(Year::kJunior, 123, 4.2, "Marvin");
const struct student_details angela_details(Year::kGrad, 124, 2.4, "Angela");
const struct grad_student_extra angela_extra(
    Support::kRA, "Physics",
    "Anistropic Superconductivity in Graphite Intercalation Compounds");

struct StudentInheritanceTest : public ::testing::Test {
public:
  StudentInheritanceTest()
      : marvin(Student(marvin_details)),
        angela(GradStudent(angela_details, angela_extra)) {}
  const Student marvin;
  const GradStudent angela;
};

// Why do the move ctor and assignment operators fail to move() objects created
// via the test fixture, but succeed with objects created on the stack inside
// the test?
// clang-format off
// error: use of deleted function ‘student_inheritance::Student::Student(const student_inheritance::Student&)’
// Student a(marvin);
// error: use of deleted function ‘student_inheritance::Student& student_inheritance::Student::operator=(const student_inheritance::Student&)’
// a = marvin;
// error: use of deleted function ‘student_inheritance::GradStudent::GradStudent(const student_inheritance::GradStudent&)’
// GradStudent b(move(angela));
// clang-format on

TEST_F(StudentInheritanceTest, MoveCtors) {
  Student a(marvin_details);
  Student b(move(a));
  EXPECT_EQ(b.gpa(), marvin.gpa());
  EXPECT_EQ(b.year(), marvin.year());
  EXPECT_EQ(b.name(), marvin.name());

  const grad_student_extra another(
      Support::kTA, "Math", "Convex Eigenfunctions of a Reflected Closure");
  GradStudent c(angela_details, another);
  GradStudent d{move(c)};
  EXPECT_EQ(d.gpa(), angela.gpa());
  EXPECT_EQ(d.id(), angela.id());
  EXPECT_EQ(d.year(), angela.year());
  EXPECT_EQ(d.name(), angela.name());
  EXPECT_EQ(d.dept(), another.dept);
  EXPECT_EQ(d.support(), another.support);
  EXPECT_NE(d.support(), Support::kRA);
  EXPECT_EQ(d.thesis(), another.thesis);
  EXPECT_EQ(-1, c.gpa());
}

TEST_F(StudentInheritanceTest, MoveAssignment) {
  const grad_student_extra another(
      Support::kTA, "Math", "Convex Eigenfunctions of a Reflected Closure");
  GradStudent a(angela_details, another);
  const student_details sd(Year::kGrad, 123, 4.2, "Marvin");
  // clang-format off
  // The statement
  // GradStudent b = move(a);
  // instead calls the move ctor.
  // clang-format on
  GradStudent b(sd, angela_extra);
  b = move(a);
  EXPECT_EQ(b.gpa(), angela.gpa());
  EXPECT_EQ(b.id(), angela.id());
  EXPECT_EQ(b.year(), angela.year());
  EXPECT_EQ(b.name(), angela.name());
  EXPECT_EQ(b.dept(), another.dept);
  EXPECT_EQ(b.support(), another.support);
  EXPECT_NE(b.support(), Support::kRA);
  EXPECT_EQ(b.thesis(), another.thesis);
  EXPECT_EQ(-1, a.gpa());
}

TEST_F(StudentInheritanceTest, StudentPrinting) {
  ostringstream oss;
  oss << marvin;
  EXPECT_EQ(oss.str(), "Name: Marvin, 123, Junior, 4.2\n");
}

TEST_F(StudentInheritanceTest, GradStudentPrinting) {
  ostringstream oss;
  student_inheritance::operator<<(oss, angela);
  EXPECT_EQ(oss.str(), "Name: Angela, 124, Grad, 2.4\n, Physics, Support: "
                       "Research assistant, Thesis: Anistropic "
                       "Superconductivity in Graphite Intercalation Compounds");
}

TEST_F(StudentInheritanceTest, StudentPrintFunction) {
  ostringstream oss;
  marvin.print(oss);
  EXPECT_EQ(oss.str(), "Name: Marvin, 123, Junior, 4.2\n");
}

TEST_F(StudentInheritanceTest, GradStudentPrintFunction) {
  ostringstream oss;
  angela.print(oss);
  EXPECT_EQ(oss.str(),
            "Student::gpa_ is not protected inside this member-function "
            "wrapper: 2.4\nName: Angela, 124, Grad, 2.4\n, Physics, Support: "
            "Research assistant, Thesis: Anistropic Superconductivity in "
            "Graphite Intercalation Compounds");
}

TEST_F(StudentInheritanceTest, CrossMembership) {
  // Apparently the objects declared in SetUp() are by default const, and trying
  // to modify them results in compilation errors.
  Student Fred(marvin_details);
  GradStudent Sue(angela_details, angela_extra);
  Sue.setGPA(Fred.gpa() + 1.0);
  ostringstream oss;
  student_inheritance::operator<<(oss, Sue);
  EXPECT_EQ(oss.str(), "Name: Angela, 124, Grad, 5.2\n, Physics, Support: "
                       "Research assistant, Thesis: Anistropic "
                       "Superconductivity in Graphite Intercalation Compounds");
}

/* Cannot have this behavior as it means a Student* cannot be a GradStudent.
TEST_F(StudentInheritanceTest, IllegalStudentYear) {
const struct student_details bad_details(Year::kGrad, 123, 4.2, "Marvin");
//EXPECT_DEATH(Student badmarvin(bad_details), "Invalid argument");
EXPECT_EXIT(Student badmarvin(bad_details), ::testing::KilledBySignal(SIGABRT),
"Invalid argument");
} */

TEST_F(StudentInheritanceTest, IllegalGradYear) {
  const struct student_details bad_details(Year::kFresh, 124, 2.4, "Angela");
  // Error below is not trapped.
  // EXPECT_DEATH(GradStudent badangela(bad_details, angela_extra), "Invalid
  // argument"); Note that testing::KilledBySignal() chooses THIS testing
  // namespace, which is poorly named.  Use either ::testing::KilledBySignal()
  // or rename this namespace.
  EXPECT_THROW(GradStudent badangela(bad_details, angela_extra),
               StudentException);
}

TEST_F(StudentInheritanceTest, IllegalStudentYear) {
  const struct student_details bad_details(static_cast<Year>(14), 124, 2.4,
                                           "Angela");
  EXPECT_THROW(Student badangela(bad_details), StudentException);
}

TEST_F(StudentInheritanceTest, IllegalSupport) {
  const struct grad_student_extra bad_extra(
      static_cast<Support>(-1), "Physics",
      "Anistropic Superconductivity in Graphite Intercalation Compounds");
  EXPECT_THROW(GradStudent badangela(angela_details, bad_extra),
               StudentException);
}

} // namespace local_testing
} // namespace student_inheritance
