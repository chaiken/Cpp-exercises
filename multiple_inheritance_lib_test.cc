#include "multiple_inheritance.h"

#include "gtest/gtest.h"

#include <fstream>
#include <type_traits>

using namespace std;

namespace people_roles {
namespace local_testing {

constexpr char TESTDIR[] = "/home/alison/gitsrc/Cpp-Exercises/test_files/";

const struct person_details ad(1948, 5, 28, "Jane", "Alsop", "Leadville CO",
                               "F");
const struct student_details bd(StudyYear::kJunior, 123, 4.2);
const struct worker_details cd(1234, 1999, 12, 10, "F");
const struct person_details og(1948, 11, 28, "Jane", "Alsop", "Leadville CO",
                               "O");
persons_array pa;

TEST(FormatDateTest, BadInput) {
  EXPECT_EQ("malformed date data", FormatDate(99, Month::kJun, 1999));
}

/*
  clang-format off
  Bjarne Stroustrup, _A Tour of C++_, 2nd edition, p. 166:
  'Where do we use smart pointers (such as unique_ptr) rather than
  resource handles with operations designed specifically for the
  resource (such as vector or thread)?  Unsurprisingly, the answer is
  "when we need pointer semantics."'
  -- When we refer to a polymorphic object in classical
     object-oriented code, we need a pointer (or reference) because we
     don't know the exact type of the object referred to (or even its
     size), so a unique_ptr becomes the obvious choice.
  -- A shared polymorphic object typically requires shared_ptrs.
  clang-format on
*/
std::list<std::shared_ptr<Person>> person_list, student_list, worker_list,
    sw_list;
persons_array plists{person_list, student_list, worker_list, sw_list};

TEST(PersonTest, CtorsTest) {
  Person jane(ad);
  EXPECT_EQ(0, jane.first_name().compare("Jane"));
  EXPECT_EQ(0, jane.last_name().compare("Alsop"));
  EXPECT_EQ(0, jane.address().compare("Leadville CO"));
  EXPECT_EQ(0, jane.gender().compare("Female"));
  EXPECT_EQ(0, jane.birthday().compare("May 28, 1948"));
  EXPECT_TRUE(jane.is_type("Person"));
  EXPECT_EQ(people_roles::PersonType::Person, jane.person_type());
  EXPECT_FALSE(jane.is_type("Student"));
  EXPECT_FALSE(jane.is_type("Worker"));
  EXPECT_FALSE(jane.is_type("StudentWorker"));

  Student jane2(ad, bd);
  EXPECT_EQ(0, jane2.first_name().compare("Jane"));
  EXPECT_EQ(0, jane2.last_name().compare("Alsop"));
  EXPECT_EQ(0, jane2.address().compare("Leadville CO"));
  EXPECT_EQ(0, jane2.gender().compare("Female"));
  EXPECT_EQ(0, jane2.birthday().compare("May 28, 1948"));
  EXPECT_EQ(0, jane2.study_year().compare("Junior"));
  EXPECT_EQ(bd.id, jane2.student_id());
  EXPECT_EQ(bd.GPA, jane2.gpa());
  EXPECT_TRUE(jane2.is_type("Student"));
  EXPECT_EQ(people_roles::PersonType::Student, jane2.person_type());
  EXPECT_FALSE(jane2.is_type("Worker"));
  EXPECT_FALSE(jane2.is_type("StudentWorker"));

  Person mitchell(og);
  EXPECT_EQ("Other", mitchell.gender());

  Worker jane3(ad, cd);
  EXPECT_EQ(0, jane3.first_name().compare("Jane"));
  EXPECT_EQ(0, jane3.last_name().compare("Alsop"));
  EXPECT_EQ(0, jane3.address().compare("Leadville CO"));
  EXPECT_EQ(0, jane3.gender().compare("Female"));
  EXPECT_EQ(0, jane3.birthday().compare("May 28, 1948"));
  EXPECT_EQ(1234u, jane3.badge_number());
  EXPECT_EQ(0, jane3.work_status().compare("Full-time"));
  EXPECT_EQ(0, jane3.start_date().compare("Dec 10, 1999"));
  EXPECT_TRUE(jane3.is_type("Worker"));
  EXPECT_EQ(people_roles::PersonType::Worker, jane3.person_type());
  EXPECT_FALSE(jane3.is_type("Student"));
  EXPECT_FALSE(jane3.is_type("StudentWorker"));

  StudentWorker jane4(ad, bd, cd);
  EXPECT_EQ(0, jane4.first_name().compare("Jane"));
  EXPECT_EQ(0, jane4.last_name().compare("Alsop"));
  EXPECT_EQ(0, jane4.address().compare("Leadville CO"));
  EXPECT_EQ(0, jane4.gender().compare("Female"));
  EXPECT_EQ(0, jane4.birthday().compare("May 28, 1948"));
  EXPECT_EQ(0, jane4.study_year().compare("Junior"));
  EXPECT_EQ(bd.id, jane4.student_id());
  EXPECT_EQ(bd.GPA, jane4.gpa());
  EXPECT_EQ(1234u, jane4.badge_number());
  EXPECT_EQ(0, jane4.work_status().compare("Full-time"));
  EXPECT_EQ(0, jane4.start_date().compare("Dec 10, 1999"));
  EXPECT_TRUE(jane4.is_type("Worker"));
  EXPECT_TRUE(jane4.is_type("Student"));
  EXPECT_TRUE(jane4.is_type("StudentWorker"));
  EXPECT_EQ(people_roles::PersonType::StudentWorker, jane4.person_type());
}

class PersonPrintTest : public testing::Test {
  // Need to declare the old pointer first so that we can initialize this
  // variable before the new one.
private:
  streambuf *oldCoutStreamBuf;

public:
  // See https://stackoverflow.com/questions/4810516/c-redirecting-stdout
  // and async_logger_lib_test_improved.cc.
  PersonPrintTest() : oldCoutStreamBuf(cout.rdbuf()) {
    cout.rdbuf(strCout.rdbuf());
  }
  ~PersonPrintTest() {
    cout.rdbuf(oldCoutStreamBuf);
#ifdef DEBUG
    cout << "At test end, output is " << endl << strCout.str() << endl;
#endif
  }
  ostringstream strCout;
};

TEST_F(PersonPrintTest, PersonExtractionOperator) {
  Person jane(ad);
  // Apparently without calling the object's method explicitly, the compiler is
  // trying std::cout.
  // clang-format off
  // error: no match for ‘operator<<’ (operand types are ‘std::ostream’ {aka ‘std::basic_ostream<char>’} and ‘people_roles::Person’)
  // clang-format on
  // cout << jane;
  jane.operator<<(cout);
  EXPECT_NE(string::npos,
            strCout.str().find("Name: Jane Alsop, Address: Leadville "
                               "CO, Gender: Female, Birthday: May "
                               "28, 1948"));
}

TEST_F(PersonPrintTest, StudentExtractionOperator) {
  Student jane(ad, bd);
  jane.operator<<(cout);
  EXPECT_NE(string::npos,
            strCout.str().find(
                "Name: Jane Alsop, Address: Leadville CO, Gender: Female, "
                "Birthday: May "
                "28, 1948, Student id: 123, Study Year: Junior, GPA: 4.2"));
}

TEST_F(PersonPrintTest, WorkerExtractionOperator) {
  Worker jane(ad, cd);
  jane.operator<<(cout);
  EXPECT_NE(string::npos,
            strCout.str().find(
                "Name: Jane Alsop, Address: Leadville CO, Gender: Female, "
                "Birthday: May 28, 1948, Badge number: 1234, Work Status: "
                "Full-time, Start Date: Dec 10, 1999"));
}

TEST_F(PersonPrintTest, StudentWorkerExtractionOperator) {
  StudentWorker jane(ad, bd, cd);
  jane.operator<<(cout);
  EXPECT_NE(string::npos,
            strCout.str().find(
                "Name: Jane Alsop, Address: Leadville CO, Gender: Female, "
                "Birthday: May 28, 1948, Student id: 123, Study Year: Junior, "
                "GPA: 4.2, Badge number: 1234, Work Status: Full-time, Start "
                "Date: Dec 10, 1999"));
}

TEST(ItemParsingTest, GoodItem) {
  const std::string item = "FirstName: Jane, LastName: Alsop, Address: "
                           "Leadville, CO, Gender: Female, BirthYear: "
                           "1994, BirthMonth: Nov, BirthDate: 28";
  std::string response = GetDetail(item, "FirstName");
  EXPECT_EQ(0, response.compare("Jane"));
}

TEST(ItemParsingTest, BadItem) {
  const std::string item = "FirstName: Jane, LastName: Alsop, 28";
  std::string response = GetDetail(item, "birth_month");
  EXPECT_EQ(0, response.compare("Unknown"));
}

TEST(ItemParsingTest, GoodStudentDetails) {
  bool is_person = false, is_student = false, is_worker = false;
  const std::string item =
      "LastName: Alsop, FirstName: Jane, Address: Leadville CO, Gender: "
      "Female, BirthYear: 1948, BirthMonth: 11, BirthDate: 28, ID: 123, "
      "StudyYear: F, GPA: 4.2";
  const struct person_details pd = PopulatePersonDetails(item, &is_person);
  const struct student_details sd = PopulateStudentDetails(item, &is_student);
  const struct worker_details wd = PopulateWorkerDetails(item, &is_worker);
  EXPECT_TRUE(is_student);
  EXPECT_FALSE(is_worker);
  EXPECT_EQ(1948u, pd.birth_year);
  EXPECT_EQ(11u, pd.birth_month);
  EXPECT_EQ(28u, pd.birth_day_of_month);
  EXPECT_EQ(0, pd.first_name.compare("Jane"));
  EXPECT_EQ(0, pd.last_name.compare("Alsop"));
  EXPECT_EQ(0, pd.address.compare("Leadville CO"));
  EXPECT_EQ(0, pd.gender.compare("Female"));
  EXPECT_EQ(StudyYear::kFresh, sd.y);
  EXPECT_EQ(123, sd.id);
  EXPECT_EQ(4.2, sd.GPA);
  EXPECT_EQ(0u, wd.badge_number);
  EXPECT_EQ(0u, wd.start_year);
  EXPECT_EQ(0u, wd.start_month);
  EXPECT_EQ(0u, wd.start_day_of_month);
  EXPECT_EQ("Unknown", wd.work_status);
}

TEST(ItemParsingTest, BadStudentDetails) {
  bool is_person = false, is_student = false, is_worker = false;
  // StudyYear should be 'J'.  "gpa" should be capitalized.
  const std::string item =
      "LastName: Alsop, FirstName: Jane, Address: Leadville CO, Gender: "
      "Female, BirthYear: 1948, BirthMonth: 12, BirthDate: 28, ID: 123, "
      "StudyYear: Junior, gpa: 4.2";
  const struct person_details pd = PopulatePersonDetails(item, &is_person);
  const struct student_details sd = PopulateStudentDetails(item, &is_student);
  EXPECT_FALSE(is_student);
  EXPECT_FALSE(is_worker);
  EXPECT_EQ(1948u, pd.birth_year);
  EXPECT_EQ(12u, pd.birth_month);
  EXPECT_EQ(28u, pd.birth_day_of_month);
  EXPECT_EQ(0, pd.first_name.compare("Jane"));
  EXPECT_EQ(0, pd.last_name.compare("Alsop"));
  EXPECT_EQ(0, pd.address.compare("Leadville CO"));
  EXPECT_EQ(0, pd.gender.compare("Female"));
  EXPECT_EQ(StudyYear::kUnknown, sd.y);
  EXPECT_EQ(123, sd.id);
  EXPECT_EQ(-1.0, sd.GPA);

  const std::string item2 =
      "LastName: Alsop, FirstName: Jane, Address: Leadville CO, Gender: "
      "Female, BirthYear: 1948, BirthMonth: 12, BirthDate: 28, ID: 123, "
      "StudyYear: A, GPA: 4.2";
  const struct person_details pd2 = PopulatePersonDetails(item2, &is_person);
  const struct student_details sd2 = PopulateStudentDetails(item2, &is_student);
  Student astudent(pd2, sd2);
  EXPECT_EQ("Unknown", astudent.study_year());
}

TEST(ItemParsingTest, GoodWorkerDetails) {
  bool is_person = false, is_student = false, is_worker = false;
  const std::string item =
      "FirstName: Jane, LastName: Alsop, Address: Leadville CO, Gender: "
      "Female, BirthDate: 28, BirthYear: 1948, BirthMonth: 12, BadgeNumber: "
      "1234, WorkStatus: Full-time, StartDate: 10, StartMonth: 3, StartYear: "
      "1999";
  const struct person_details pd = PopulatePersonDetails(item, &is_person);
  const struct student_details sd = PopulateStudentDetails(item, &is_student);
  const struct worker_details wd = PopulateWorkerDetails(item, &is_worker);
  EXPECT_FALSE(is_student);
  EXPECT_TRUE(is_worker);
  EXPECT_EQ(1948u, pd.birth_year);
  EXPECT_EQ(12u, pd.birth_month);
  EXPECT_EQ(28u, pd.birth_day_of_month);
  EXPECT_EQ(0, pd.first_name.compare("Jane"));
  EXPECT_EQ(0, pd.last_name.compare("Alsop"));
  EXPECT_EQ(0, pd.address.compare("Leadville CO"));
  EXPECT_EQ(0, pd.gender.compare("Female"));
  EXPECT_EQ(StudyYear::kUnknown, sd.y);
  EXPECT_EQ(-1.0, sd.id);
  EXPECT_EQ(-1.0, sd.GPA);
  EXPECT_EQ(1234u, wd.badge_number);
  EXPECT_EQ(1999u, wd.start_year);
  EXPECT_EQ(3u, wd.start_month);
  EXPECT_EQ(10u, wd.start_day_of_month);
  EXPECT_EQ("Full-time", wd.work_status);
}

TEST(ItemParsingTest, BadWorkerDetails) {
  bool is_person = false, is_student = false, is_worker = false;
  // Missing comma.
  const std::string item =
      "FirstName: Jane, LastName: Alsop, Address: Leadville CO, Gender: "
      "Female,  BirthMonth: 5, BirthDate: 28, BirthYear: 1948, Badge number: "
      "1234 WorkStatus: Full-time, StartDate: 10, StartMonth: 3, StartYear: "
      "1999";
  const struct person_details pd = PopulatePersonDetails(item, &is_person);
  const struct worker_details wd = PopulateWorkerDetails(item, &is_worker);
  EXPECT_FALSE(is_student);
  EXPECT_FALSE(is_worker);
  EXPECT_EQ(1948u, pd.birth_year);
  EXPECT_EQ(5u, pd.birth_month);
  EXPECT_EQ(28u, pd.birth_day_of_month);
  EXPECT_EQ(0, pd.first_name.compare("Jane"));
  EXPECT_EQ(0, pd.last_name.compare("Alsop"));
  EXPECT_EQ(0, pd.address.compare("Leadville CO"));
  EXPECT_EQ(0, pd.gender.compare("Female"));
  EXPECT_EQ(0u, wd.badge_number);
  EXPECT_EQ(1999u, wd.start_year);
  EXPECT_EQ(3u, wd.start_month);
  EXPECT_EQ(10u, wd.start_day_of_month);
  EXPECT_EQ("Full-time", wd.work_status);
}

TEST(ItemParsingTest, GoodStudentWorkerDetails) {
  bool is_person = false, is_student = false, is_worker = false;
  const std::string item =
      "FirstName: Jane, LastName: Alsop, Address: Leadville CO, Gender: "
      "Female, BirthDate: 28, BirthYear: 1948, BirthMonth: 5, BadgeNumber: "
      "1234, WorkStatus: Full-time, StartMonth: 3, StartDate: 10, StartYear: "
      "1999,  ID: 123, StudyYear: So, GPA: 4.2";
  const struct person_details pd = PopulatePersonDetails(item, &is_person);
  const struct student_details sd = PopulateStudentDetails(item, &is_student);
  const struct worker_details wd = PopulateWorkerDetails(item, &is_worker);
  EXPECT_TRUE(is_student);
  EXPECT_TRUE(is_worker);
  EXPECT_EQ(1948u, pd.birth_year);
  EXPECT_EQ(StudyYear::kSoph, sd.y);
  EXPECT_EQ(1234u, wd.badge_number);
}

TEST(ItemParsingTest, BadPersonDetails) {
  bool is_person = false, is_student = false, is_worker = false;
  // Month should be a number.
  const std::string item = "FirstName: Jane, LastName: Alsop, Address: "
                           "Leadville CO, Gender: Female, BirthYear: "
                           "1994, BirthMonth: May, BirthDate: 28";
  const struct person_details pd = PopulatePersonDetails(item, &is_person);
  EXPECT_FALSE(is_person);
  EXPECT_FALSE(is_student);
  EXPECT_FALSE(is_worker);
  EXPECT_EQ(0u, pd.birth_month);
  EXPECT_EQ(1994u, pd.birth_year);
  EXPECT_EQ(28u, pd.birth_day_of_month);
  EXPECT_EQ(0, pd.first_name.compare("Jane"));
  EXPECT_EQ(0, pd.last_name.compare("Alsop"));
  EXPECT_EQ(0, pd.address.compare("Leadville CO"));
  EXPECT_EQ(0, pd.gender.compare("Female"));
}

void ClearLists() {
  for (unsigned int i = 0u; i < number_person_types; i++) {
    pa[i].clear();
  }
}

TEST(ListPopulationTest, GoodPersonTest) {
  const std::string item = "FirstName: Jane, LastName: Alsop, Address: "
                           "Leadville CO, Gender: Female, BirthYear: "
                           "1994, BirthMonth: 5, BirthDate: 28";
  ClearLists();
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Person)].empty());
  ProcessPerson(item, &pa);
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::Person)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Student)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Worker)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::StudentWorker)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Person)].back()->is_type("Person"));
  EXPECT_FALSE(
      pa[GetPersonIndex(PersonType::Person)].back()->is_type("Student"));
  EXPECT_FALSE(
      pa[GetPersonIndex(PersonType::Person)].back()->is_type("Worker"));
}

TEST(ListPopulationTest, BadPersonTest) {
  const std::string item = "FirstName: Jane, LastName: Alsop, Address: "
                           "Leadville CO, Gender: female, BirthYear: "
                           "1994, BirthMonth: May, BirthDate: 28";
  ClearLists();
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Person)].empty());
  ProcessPerson(item, &pa);
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Person)].empty());
}

TEST(ListPopulationTest, GoodStudentTest) {
  const std::string item =
      "LastName: Alsop, FirstName: Jane, Address: Leadville CO, Gender: "
      "Female, BirthYear: 1948, BirthMonth: 5, BirthDate: 28, ID: 123, "
      "StudyYear: Se, GPA: 4.2";
  ClearLists();
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Student)].empty());
  ProcessPerson(item, &pa);
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::Person)].empty());
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::Student)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Worker)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::StudentWorker)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::StudentWorker)].empty());
  EXPECT_TRUE(
      pa[GetPersonIndex(PersonType::Person)].back()->is_type("Student"));
  EXPECT_FALSE(
      pa[GetPersonIndex(PersonType::Person)].back()->is_type("Person"));
  EXPECT_TRUE(
      pa[GetPersonIndex(PersonType::Student)].back()->is_type("Student"));
  EXPECT_FALSE(
      pa[GetPersonIndex(PersonType::Student)].back()->is_type("Worker"));
}

TEST(ListPopulationTest, BadStudentTest) {
  const std::string item =
      "LastName: Alsop, FirstName: Jane, Address: Leadville CO, Gender: "
      "Female, BirthYear: 1948, BirthMonth: 5, BirthDate: 28, ID: 123, "
      "StudyYear: Junior, gpa: 4.2";
  ClearLists();
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Student)].empty());
  ProcessPerson(item, &pa);
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::Person)].empty());
  EXPECT_FALSE(
      pa[GetPersonIndex(PersonType::Person)].back()->is_type("Student"));
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Student)].empty());
}

TEST(ListPopulationTest, GoodWorkerTest) {
  const std::string item =
      "FirstName: Jane, LastName: Alsop, Address: Leadville CO, Gender: "
      "Female, BirthDate: 28, BirthYear: 1948, BirthMonth: 5, BadgeNumber: "
      "1234, WorkStatus: Full-time, StartDate: 10, StartMonth: 3, StartYear: "
      "1999";
  ClearLists();
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Worker)].empty());
  ProcessPerson(item, &pa);
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::Worker)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Person)].back()->is_type("Worker"));
  EXPECT_FALSE(
      pa[GetPersonIndex(PersonType::Person)].back()->is_type("Person"));
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Worker)].back()->is_type("Worker"));
  EXPECT_FALSE(
      pa[GetPersonIndex(PersonType::Worker)].back()->is_type("Student"));
}

TEST(ListPopulationTest, BadWorkerTest) {
  // Missing comma.
  const std::string item =
      "FirstName: Jane, LastName: Alsop, Address: Leadville CO, Gender: "
      "Female,  BirthMonth: 5, BirthDate: 28, BirthYear: 1948, Badge number: "
      "1234 WorkStatus: Full-time, StartDate: 10, StartMonth: 3, StartYear: "
      "1999";
  ClearLists();
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Worker)].empty());
  ProcessPerson(item, &pa);
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::Person)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Worker)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Person)].back()->is_type("Person"));
  EXPECT_FALSE(
      pa[GetPersonIndex(PersonType::Person)].back()->is_type("Worker"));
}

TEST(ListPopulationTest, GoodStudentWorkerTest) {
  const std::string item =
      "FirstName: Jane, LastName: Alsop, Address: Leadville CO, Gender: "
      "Female, BirthDate: 28, BirthYear: 1948, BirthMonth: 5, BadgeNumber: "
      "1234, WorkStatus: Full-time, StartMonth: 3, StartDate: 10, StartYear: "
      "1999,  ID: 123, StudyYear: J, GPA: 4.2";
  ClearLists();
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::StudentWorker)].empty());
  ProcessPerson(item, &pa);
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::StudentWorker)].empty());
  EXPECT_TRUE(
      pa[GetPersonIndex(PersonType::Person)].back()->is_type("StudentWorker"));
  EXPECT_FALSE(
      pa[GetPersonIndex(PersonType::Person)].back()->is_type("Person"));
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::StudentWorker)].back()->is_type(
      "StudentWorker"));
  // True based on how is_type() is defined for StudentWorker.
  EXPECT_TRUE(
      pa[GetPersonIndex(PersonType::StudentWorker)].back()->is_type("Student"));
  EXPECT_TRUE(
      pa[GetPersonIndex(PersonType::StudentWorker)].back()->is_type("Worker"));
}

TEST(ListPopulationTest, BadStudentWorkerTest) {
  const std::string item =
      "FirstName: Jane, LastName: Alsop, Address: Leadville CO, Gender: "
      "Female, BirthDate: 28, BirthYear: 1948, BirthMonth: 5, BadgeNumber: "
      "1234, WorkStatus: Full-time, StartMonth: March, StartDate: 10, "
      "StartYear: 1999,  ID: 123, StudyYear: J, gpa: 4.2";
  ClearLists();
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::StudentWorker)].empty());
  ProcessPerson(item, &pa);
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::Person)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::StudentWorker)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Person)].back()->is_type("Person"));
  EXPECT_FALSE(
      pa[GetPersonIndex(PersonType::Person)].back()->is_type("StudentWorker"));
}

TEST(FileIOTest, BasicTest) {
  ClearLists();
  EXPECT_TRUE(PopulateLists(
      string(TESTDIR) + "multiple_inheritance_test_data.txt", &pa));
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::Person)].empty());
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::Student)].empty());
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::Worker)].empty());
  EXPECT_FALSE(pa[GetPersonIndex(PersonType::StudentWorker)].empty());
  EXPECT_EQ(10u, pa[GetPersonIndex(PersonType::Person)].size());
  EXPECT_EQ(6u, pa[GetPersonIndex(PersonType::Student)].size());
  EXPECT_EQ(5u, pa[GetPersonIndex(PersonType::Worker)].size());
  EXPECT_EQ(3u, pa[GetPersonIndex(PersonType::StudentWorker)].size());
}

TEST(FileIOTest, EmptyFile) {
  ClearLists();
  // "touch" a random file
  string fname = "/tmp/" + to_string(rand());
  ofstream ofs(fname, fstream::out);
  ofs << "";
  ofs.close();
  EXPECT_TRUE(PopulateLists(fname, &pa));
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Person)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Student)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::Worker)].empty());
  EXPECT_TRUE(pa[GetPersonIndex(PersonType::StudentWorker)].empty());
  // Delete file.
  unlink(fname.c_str());
}

TEST(FileIOTest, EmptyFilePath) {
  ClearLists();
  EXPECT_FALSE(PopulateLists("", &pa));
}

TEST(FileIOTest, IsADirectory) {
  ClearLists();
  EXPECT_FALSE(PopulateLists("/etc", &pa));
}

TEST(FileIOTest, PermissionDenied) {
  ClearLists();
  EXPECT_FALSE(PopulateLists("/etc/shadow", &pa));
}

TEST(FileIOTest, NoSuchFile) {
  ClearLists();
  EXPECT_FALSE(PopulateLists("/tmp/superunlikely", &pa));
}

TEST(FileIOTest, GarbageData) {
  ClearLists();
  EXPECT_FALSE(PopulateLists(string(TESTDIR) + "garbage.txt", &pa));
}

// https://gehrcke.de/2011/06/reading-files-in-c-using-ifstream-dealing-correctly-with-badbit-failbit-eofbit-and-perror/
// First line ends with a newline, so getline will succeed.  2nd line with no
// newline will not yet trigger EOF but cannot succeed in creating a string.
TEST(FileIOTest, NoNewline) {
  ClearLists();
  EXPECT_FALSE(PopulateLists(string(TESTDIR) + "unterminated.txt", &pa));
}

TEST_F(PersonPrintTest, ListPrinting) {
  ClearLists();
  PopulateLists(string(TESTDIR) + "multiple_inheritance_test_data.txt", &pa);
  strCout.str().clear();
  // Empty list.
  list<shared_ptr<Person>> pl;
  PrintList(pl);
  EXPECT_TRUE(strCout.str().empty());

  PrintList(pa[GetPersonIndex(PersonType::Person)]);
  EXPECT_NE(
      string::npos,
      strCout.str().find("Name: Bob Johannson, Address: Leadville CO, Gender: "
                         "Male, Birthday: Jan 28, 1948, Badge number: 1234, "
                         "Work Status: Retired, Start Date: Mar 10, 1999\n"));
  EXPECT_NE(
      string::npos,
      strCout.str().find(
          "Name: Torsten Paul, Address: Leadville CO, Gender: Male, Birthday: "
          "Feb 28, 1948, Student id: 123, Study Year: Junior, GPA: 4.2, Badge "
          "number: 1234, Work Status: Full-time, Start Date: Mar 10, 1999\n"));
  EXPECT_NE(string::npos,
            strCout.str().find("Name: Jane Alsop, Address: Leadville CO, "
                               "Gender: Female, Birthday: Mar 28, 1948\n"));
  EXPECT_NE(string::npos,
            strCout.str().find("Name: Bobby Fuller, Address: Leadville CO, "
                               "Gender: Male, Birthday: Apr 28, 1948\n"));
  EXPECT_NE(
      string::npos,
      strCout.str().find(
          "Name: Dirk Hast, Address: Leadville CO, Gender: Male, Birthday: May "
          "28, 1948, Student id: 123, Study Year: Junior, GPA: 4.2\n"));
  EXPECT_NE(string::npos,
            strCout.str().find(
                "Name: Jewlia Behme, Address: Leadville CO, Gender: Female, "
                "Birthday: Jun 28, 1948, Student id: 123, Study Year: Junior, "
                "GPA: 4.2, Badge number: 1234, Work Status: Part-time, Start "
                "Date: Mar 10, 1999\n"));
  EXPECT_NE(string::npos,
            strCout.str().find(
                "Name: Carla Goldberg, Address: Leadville CO, Gender: Female, "
                "Birthday: Jul 28, 1948, Badge number: 1234, Work Status: "
                "Full-time, Start Date: Mar 10, 1999\n"));
  EXPECT_NE(string::npos, strCout.str().find(
                              "Name: Billie Goldberg, Address: Leadville CO, "
                              "Gender: Female, Birthday: Aug 28, 1948, Student "
                              "id: 123, Study Year: Junior, GPA: 4.2\n"));
  EXPECT_NE(string::npos,
            strCout.str().find(
                "Name: Davidlohr Morton, Address: Leadville CO, Gender: Male, "
                "Birthday: Sep 28, 1948, Student id: 123, Study Year: Junior, "
                "GPA: 4.2, Badge number: 1234, Work Status: Student, Start "
                "Date: Mar 10, 1999\n"));
  EXPECT_NE(
      string::npos,
      strCout.str().find(
          "Name: Ben Bueno, Address: Leadville CO, Gender: Male, Birthday: Oct "
          "28, 1948, Student id: 123, Study Year: Junior, GPA: 4.2\n"));
}

TEST(SortingTest, PersonTest) {
  ClearLists();
  PopulateLists(string(TESTDIR) + "multiple_inheritance_test_data.txt", &pa);
  EXPECT_EQ(10u, pa[GetPersonIndex(PersonType::Person)].size());
  EXPECT_EQ("Johannson",
            pa[GetPersonIndex(PersonType::Person)].front().get()->last_name());
  EXPECT_EQ("Bueno",
            pa[GetPersonIndex(PersonType::Person)].back().get()->last_name());
#ifdef DEBUG
  cout << endl << "Unsorted Persons:";
  PrintList(pa[GetPersonIndex(PersonType::Person)]);
#endif
  SortLists(&pa);
  EXPECT_EQ(10u, pa[GetPersonIndex(PersonType::Person)].size());
  EXPECT_EQ("Alsop",
            pa[GetPersonIndex(PersonType::Person)].front().get()->last_name());
  EXPECT_EQ("Paul",
            pa[GetPersonIndex(PersonType::Person)].back().get()->last_name());
#ifdef DEBUG
  cout << endl << "Sorted Persons";
  PrintList(pa[GetPersonIndex(PersonType::Person)]);
#endif
}

TEST(PersonTest, IllegalYear) {
  const struct person_details bad(2038, 14, 40, "Jane", "Alsop", "Leadville CO",
                                  "F");
  EXPECT_THROW(Person bp(bad), RolesException);
}

TEST(PersonTest, IllegalMonth) {
  const struct person_details bad(1948, 14, 40, "Jane", "Alsop", "Leadville CO",
                                  "F");
  EXPECT_THROW(Person bp(bad), RolesException);
}

TEST(PersonTest, IllegalDay) {
  const struct person_details bad(1948, 3, 40, "Jane", "Alsop", "Leadville CO",
                                  "F");
  EXPECT_THROW(Person bp(bad), RolesException);

  const struct worker_details bad2(1234, 1999, 12, 123, "F");
  EXPECT_THROW(Worker bw(ad, bad2), RolesException);
  const struct worker_details bad3(1234, -1, 12, 10, "F");
  EXPECT_THROW(Worker bwb(ad, bad3), RolesException);
}

TEST(PersonTest, IllegalDayinJune) {
  const struct person_details bad(1948, 6, 31, "Jane", "Alsop", "Leadville CO",
                                  "F");
  EXPECT_THROW(Person bp(bad), RolesException);
}

TEST(PersonTest, IllegalType) {
  EXPECT_THROW(GetPersonIndex(static_cast<PersonType>(5u)), RolesException);
}

TEST(PersonTest, IllegalStudentYear) {
  const struct student_details bad(static_cast<StudyYear>(11), 123, 2.4);
  EXPECT_THROW(Student(ad, bad), RolesException);
}

} // namespace local_testing
} // namespace people_roles
