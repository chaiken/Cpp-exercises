#include "multiple_inheritance.h"

#include "gtest/gtest.h"

using namespace std;

namespace people_roles {
namespace local_testing {

const struct person_details ad(1948, 5, 28, "Jane", "Leadville CO", "F");
const struct student_details bd(StudyYear::kJunior, 123, 4.2);
const struct worker_details cd(1234, 1999, 3, 10, "F");

TEST(PersonTest, CtorsTest) {
  Person jane(ad);
  EXPECT_EQ(0, jane.name().compare("Jane"));
  EXPECT_EQ(0, jane.address().compare("Leadville CO"));
  EXPECT_EQ(0, jane.gender().compare("Female"));
  EXPECT_EQ(0, jane.birthday().compare("May 28, 1948"));

  cout << endl;
  Student jane2(ad, bd);
  EXPECT_EQ(0, jane2.name().compare("Jane"));
  EXPECT_EQ(0, jane2.address().compare("Leadville CO"));
  EXPECT_EQ(0, jane2.gender().compare("Female"));
  EXPECT_EQ(0, jane2.birthday().compare("May 28, 1948"));
  EXPECT_EQ(0, jane2.study_year().compare("Junior"));
  EXPECT_EQ(bd.id, jane2.student_id());
  EXPECT_EQ(bd.GPA, jane2.gpa());

  cout << endl;
  Worker jane3(ad, cd);
  EXPECT_EQ(0, jane3.name().compare("Jane"));
  EXPECT_EQ(0, jane3.address().compare("Leadville CO"));
  EXPECT_EQ(0, jane3.gender().compare("Female"));
  EXPECT_EQ(0, jane3.birthday().compare("May 28, 1948"));
  EXPECT_EQ(1234u, jane3.badge_number());
  EXPECT_EQ(0, jane3.work_status().compare("Full-time"));
  EXPECT_EQ(0, jane3.start_date().compare("Mar 10, 1999"));

  cout << endl;
  StudentWorker jane4(ad, bd, cd);
  EXPECT_EQ(0, jane4.name().compare("Jane"));
  EXPECT_EQ(0, jane4.address().compare("Leadville CO"));
  EXPECT_EQ(0, jane4.gender().compare("Female"));
  EXPECT_EQ(0, jane4.birthday().compare("May 28, 1948"));
  EXPECT_EQ(0, jane4.study_year().compare("Junior"));
  EXPECT_EQ(bd.id, jane4.student_id());
  EXPECT_EQ(bd.GPA, jane4.gpa());
  EXPECT_EQ(1234u, jane4.badge_number());
  EXPECT_EQ(0, jane4.work_status().compare("Full-time"));
  EXPECT_EQ(0, jane4.start_date().compare("Mar 10, 1999"));
}

TEST(PersonTest, ExtractionOperators) {
  Person jane1(ad);
  ostringstream out;
  out << jane1;
  EXPECT_EQ("Name: Jane, Address: Leadville CO, Gender: Female, Birthday: May "
            "28, 1948\n",
            out.str());
  Student jane2(ad, bd);
  out.str().clear();
  out << jane2;
  EXPECT_EQ("Name: Jane, Address: Leadville CO, Gender: Female, Birthday: May "
            "28, 1948\n, Student id: 123, Study Year: Junior, GPA: 4.2\n",
            out.str());
}

TEST(PersonDeathTest, IllegalYear) {
  const struct person_details bad(2038, 14, 40, "Jane", "Leadville CO", "F");
  EXPECT_EXIT(Person bp(bad), testing::KilledBySignal(SIGABRT),
              "Invalid birth year");
}

TEST(PersonDeathTest, IllegalMonth) {
  const struct person_details bad(1948, 14, 40, "Jane", "Leadville CO", "F");
  EXPECT_EXIT(Person bp(bad), testing::KilledBySignal(SIGABRT),
              "Invalid month:");
}

TEST(PersonDeathTest, IllegalDay) {
  const struct person_details bad(1948, 3, 40, "Jane", "Leadville CO", "F");
  EXPECT_EXIT(Person bp(bad), testing::KilledBySignal(SIGABRT),
              "Invalid birth day:");
}

} // namespace local_testing
} // namespace people_roles
