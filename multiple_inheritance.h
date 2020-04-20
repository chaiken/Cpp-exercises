#ifndef MULTIPLE_INHERITANCE_H
#define MULTIPLE_INHERITANCE_H

#include <cassert>
#include <ctime>

#include <iostream>
#include <map>

namespace people_roles {

enum class Month {
  kJan,
  kFeb,
  kMar,
  kApr,
  kMay,
  kJun,
  kJul,
  kAug,
  kSep,
  kOct,
  kNov,
  kDec,
  kUnknown
};

enum class Gender { kFemale, kMale, kOther, kUnknown };
/* Regrettably copied from earlier student_inheritance code.  Refactoring that
 * existing code in order to inherit from Person would be a waste of time. */
enum class StudyYear { kFresh, kSoph, kJunior, kSenior, kUnknown };
enum class WorkStatus { kFulltime, kParttime, kStudent, kRetired, kUnknown };

// Forward declarations
bool YearIsInvalid(const unsigned int year);
bool DayOfMonthIsInvalid(const unsigned int day, const Month month);
Gender LookupGender(const std::string gender);
Month LookupMonth(const unsigned int birth_month);
StudyYear LookupStudyYear(const std::string study_year);
WorkStatus LookupWorkStatus(const std::string work_status);
std::string FormatDate(const unsigned int day, const Month month,
                       const unsigned int year);

const std::map<Month, std::string> MonthDescription = {
    {Month::kJan, "Jan"},        {Month::kFeb, "Feb"}, {Month::kMar, "Mar"},
    {Month::kApr, "Apr"},        {Month::kMay, "May"}, {Month::kJun, "Jun"},
    {Month::kJul, "Jul"},        {Month::kAug, "Aug"}, {Month::kSep, "Sep"},
    {Month::kOct, "Oct"},        {Month::kNov, "Nov"}, {Month::kDec, "Dec"},
    {Month::kUnknown, "Unknown"}};

const std::map<Gender, std::string> GenderDescription{
    {Gender::kFemale, "Female"},
    {Gender::kMale, "Male"},
    {Gender::kOther, "Other"},
    {Gender::kUnknown, "Unknown"}};

// maps apparently cannot be constexpr as they are not literal types.
const std::map<StudyYear, std::string> StudyYearDescription = {
    {StudyYear::kFresh, "Freshman"},  {StudyYear::kSoph, "Sophomore"},
    {StudyYear::kJunior, "Junior"},   {StudyYear::kSenior, "Senior"},
    {StudyYear::kUnknown, "Unknown"},
};

const std::map<WorkStatus, std::string> WorkStatusDescription = {
    {WorkStatus::kFulltime, "Full-time"},
    {WorkStatus::kParttime, "Part-time"},
    {WorkStatus::kStudent, "Student"},
    {WorkStatus::kRetired, "Retired"},
    {WorkStatus::kUnknown, "Unknown"}};

struct person_details {
  person_details(unsigned int by, unsigned int bm, unsigned int bdom,
                 std::string n, std::string a, std::string g)
      : birth_year(by), birth_month(bm), birth_day_of_month(bdom), name(n),
        address(a), gender(g) {}
  unsigned int birth_year;
  unsigned int birth_month;
  unsigned int birth_day_of_month;
  std::string name;
  std::string address;
  std::string gender;
};

// student_details could inherit from person_details, but what would be the
// benefit?
struct student_details {
  student_details(StudyYear a, int b, double c) : y(a), id(b), GPA(c) {}
  StudyYear y;
  const int id;
  double GPA;
};

struct worker_details {
  worker_details(unsigned int bn, unsigned int sy, unsigned int sm,
                 unsigned int dom, std::string ws)
      : badge_number(bn), start_year(sy), start_month(sm),
        start_day_of_month(dom), work_status(ws) {}
  unsigned int badge_number;
  unsigned int start_year;
  unsigned int start_month;
  unsigned int start_day_of_month;
  std::string work_status;
};

class Person {
public:
  Person(const struct person_details pd)
      : birth_year_(pd.birth_year), birth_day_of_month_(pd.birth_day_of_month),
        name_(pd.name), address_(pd.address) {
    std::cerr << "Person ctor" << std::endl;
    if (YearIsInvalid(birth_year_)) {
      std::cerr << "Invalid birth year: " << birth_year_ << std::endl;
      assert_perror(EINVAL);
    }
    gender_ = LookupGender(pd.gender);
    birth_month_ = LookupMonth(pd.birth_month);
    if (DayOfMonthIsInvalid(birth_day_of_month_, birth_month_)) {
      std::cerr << "Invalid birth day: " << birth_day_of_month_ << std::endl;
      assert_perror(EINVAL);
    }
  }

  std::string name() const { return name_; }
  std::string address() const { return address_; }
  std::string gender() const;
  std::string birthday() const {
    return FormatDate(birth_day_of_month_, birth_month_, birth_year_);
  }

protected:
  const unsigned int birth_year_;
  const unsigned int birth_day_of_month_;
  const std::string name_;
  std::string address_;
  Month birth_month_;
  Gender gender_ = Gender::kUnknown;
};

// Largely copied from student_inheritance code.
// "virtual public" prevents multiple Person objects being created by
// StudentWorker's ctor.
class Student : virtual public Person {
public:
  Student(const struct person_details pd, const struct student_details sd)
      : Person(pd), student_id_(sd.id), gpa_(sd.GPA), y_(sd.y) {
    std::cerr << "Student ctor" << std::endl;
    std::map<StudyYear, std::string>::const_iterator idx =
        StudyYearDescription.find(y_);
    // Prevents a Student* from pointing to a GradStudent.
    //    if ((StudyYearDescription.end() == idx) || (StudyYear::kGrad ==
    //    idx->first)) {
    if (StudyYearDescription.end() == idx) {
      std::cerr << "Illegal year for student." << std::endl;
      assert_perror(EINVAL);
    }
  }
  std::string study_year() const;
  int student_id() const { return student_id_; }
  double gpa() const { return gpa_; }

protected:
  int student_id_;
  double gpa_;
  StudyYear y_;
};

std::ostream &operator<<(std::ostream &out, const Student &st);

// "virtual public" prevents multiple Person objects being created by
// StudentWorker's ctor.
class Worker : virtual public Person {
public:
  Worker(const person_details pd, const worker_details wd)
      : Person(pd), badge_number_(wd.badge_number), start_year_(wd.start_year),
        start_day_of_month_(wd.start_day_of_month) {
    std::cerr << "Worker ctor" << std::endl;
    if (YearIsInvalid(start_year_)) {
      std::cerr << "Invalid birth year: " << start_year_ << std::endl;
      assert_perror(EINVAL);
    }
    start_month_ = LookupMonth(wd.start_month);
    work_status_ = LookupWorkStatus(wd.work_status);
    if (DayOfMonthIsInvalid(start_day_of_month_, start_month_)) {
      std::cerr << "Invalid birth day: " << start_day_of_month_ << std::endl;
      assert_perror(EINVAL);
    }
  }
  unsigned int badge_number() const { return badge_number_; }
  std::string work_status() const {
    return WorkStatusDescription.find(work_status_)->second;
  }
  std::string start_date() const {
    return FormatDate(start_day_of_month_, start_month_, start_year_);
  }

protected:
  const unsigned int badge_number_;
  const unsigned int start_year_;
  const unsigned int start_day_of_month_;
  Month start_month_;
  WorkStatus work_status_ = WorkStatus::kUnknown;
};

// Because Student and Worker inherit from Person with "virtual public,"
// StudentWorker must explicitly invoke the Person constructor.   Notably
// StudentWorker, Student and Worker could all invoke different Person
// constructors if there were several choices.
class StudentWorker : public Student, public Worker {
public:
  StudentWorker(const struct person_details pd, const struct student_details sd,
                const struct worker_details wd)
      : Person(pd), Student(pd, sd), Worker(pd, wd) {}
};

// Non-member functions in multiple_inheritance_lib.cc.
// I had previously made extraction operators friends, but if all the data they
// use has a const accessor function, there is no need.
std::ostream &operator<<(std::ostream &out, const Person &person);
std::ostream &operator<<(std::ostream &out, const Student &student);
std::ostream &operator<<(std::ostream &out, const Worker &worker);

} // namespace people_roles

#endif