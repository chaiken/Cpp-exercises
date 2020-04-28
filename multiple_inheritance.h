#ifndef MULTIPLE_INHERITANCE_H
#define MULTIPLE_INHERITANCE_H

#include <cassert>
#include <ctime>

#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <type_traits>

namespace people_roles {

constexpr uint32_t number_person_types = 4u;
enum class PersonType { Person, Student, Worker, StudentWorker };

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
  person_details()
      : birth_year(0u), birth_month(0u), birth_day_of_month(0u),
        first_name("Unknown"), last_name("Unknown"), address("Unknown"),
        gender("Unknown") {}
  person_details(unsigned int by, unsigned int bm, unsigned int bdom,
                 std::string fn, std::string ln, std::string a, std::string g)
      : birth_year(by), birth_month(bm), birth_day_of_month(bdom),
        first_name(fn), last_name(ln), address(a), gender(g) {}
  unsigned int birth_year;
  unsigned int birth_month;
  unsigned int birth_day_of_month;
  std::string first_name;
  std::string last_name;
  std::string address;
  std::string gender;
};

// student_details could inherit from person_details, but what would be the
// benefit?
struct student_details {
  student_details() : y(StudyYear::kUnknown), id(-1), GPA(-1.0) {}
  student_details(StudyYear a, int b, double c) : y(a), id(b), GPA(c) {}
  StudyYear y;
  const int id;
  double GPA;
};

struct worker_details {
  worker_details()
      : badge_number(0u), start_year(0u), start_month(0u),
        start_day_of_month(0u), work_status("Unknown") {}
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
        first_name_(pd.first_name), last_name_(pd.last_name),
        address_(pd.address) {
#ifdef DEBUG
    std::cerr << "Person ctor" << std::endl;
#endif
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

  virtual PersonType person_type() const { return PersonType::Person; }
  std::string first_name() const { return first_name_; }
  std::string last_name() const { return last_name_; }
  std::string address() const { return address_; }
  std::string gender() const;
  std::string birthday() const {
    return FormatDate(birth_day_of_month_, birth_month_, birth_year_);
  }
  bool printer_has_run() { return printer_has_run_; }
  // Classes which are final derivatives should set the bool false.
  void set_printer_has_run(bool printer_has_run) {
    printer_has_run_ = printer_has_run;
  }
  // Without virtual, printing of Person* always calls this version.
  virtual std::ostream &operator<<(std::ostream &out);
  // Cannot check for exact match due to name mangling.
  bool is_type(const std::string &tname) const {
    return (std::string::npos != type_name().find(tname));
  }

protected:
  // Without "virtual", all is_type() tests for derived classes will fail.
  virtual std::string type_name() const {
    std::cout << std::endl << typeid(*this).name() << std::endl;
    return (typeid(*this).name());
  }

  const unsigned int birth_year_;
  const unsigned int birth_day_of_month_;
  const std::string first_name_;
  const std::string last_name_;
  std::string address_;
  Month birth_month_;
  Gender gender_ = Gender::kUnknown;

private:
  bool printer_has_run_ = false;
};

// Largely copied from student_inheritance code.
// "virtual public" prevents multiple Person objects being created by
// StudentWorker's ctor.
class Student : virtual public Person {
public:
  Student(const struct person_details pd, const struct student_details sd)
      : Person(pd), student_id_(sd.id), gpa_(sd.GPA), y_(sd.y) {
#ifdef DEBUG
    std::cerr << "Student ctor" << std::endl;
#endif
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

  PersonType person_type() const { return PersonType::Student; }
  int student_id() const { return student_id_; }
  double gpa() const { return gpa_; }
  std::string study_year() const;

  // Still not an override according to GCC.
  std::ostream &operator<<(std::ostream &out);

protected:
  int student_id_;
  double gpa_;
  StudyYear y_;
};

// "virtual public" prevents multiple Person objects being created by
// StudentWorker's ctor.
class Worker : virtual public Person {
public:
  Worker(const person_details pd, const worker_details wd)
      : Person(pd), badge_number_(wd.badge_number), start_year_(wd.start_year),
        start_day_of_month_(wd.start_day_of_month) {
#ifdef DEBUG
    std::cerr << "Worker ctor" << std::endl;
#endif
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
  PersonType person_type() const { return PersonType::Worker; }
  unsigned int badge_number() const { return badge_number_; }
  std::string work_status() const {
    return WorkStatusDescription.find(work_status_)->second;
  }
  std::string start_date() const {
    return FormatDate(start_day_of_month_, start_month_, start_year_);
  }

  std::ostream &operator<<(std::ostream &out);

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
class StudentWorker final : public Student, public Worker {
public:
  StudentWorker(const struct person_details pd, const struct student_details sd,
                const struct worker_details wd)
      : Person(pd), Student(pd, sd), Worker(pd, wd) {}
  std::ostream &operator<<(std::ostream &out);
  PersonType person_type() const { return PersonType::StudentWorker; }

  bool is_student_worker() const {
    return (is_type("Student") && is_type("Worker"));
  }
};

// Non-member functions defined in multiple_inheritance_lib.cc.  These
// functions are visible in the people_roles namespace only so that
// tests can access them.

// Find data fields that terminate with ": " and include all data up until the
// next comma on a newline in them.
std::string GetDetail(const std::string item, const std::string detail);

// Process Person data so that is suitable for constructing person_details,
// indicating error on failure.
struct person_details PopulatePersonDetails(const std::string item,
                                            bool *is_person);
// Process Person data so that is suitable for constructing student_details,
// indicating error on failure.
struct student_details PopulateStudentDetails(const std::string item,
                                              bool *is_student);
// Process Person data so that is suitable for constructing worker_details,
// indicating error on failure.
struct worker_details PopulateWorkerDetails(const std::string item,
                                            bool *is_worker);

// The kind of array in which lists of Person pointers are stored.
typedef std::array<std::list<std::shared_ptr<Person>>, number_person_types>
    persons_array;

// Print a single list of Persons with their appropriate operator<<().
void PrintList(const std::list<std::shared_ptr<Person>> &pl);
// Print all the lists of Persons.
void PrintListArray(const persons_array pa);

// Return the index in a persons_array where the given PersonType
// appears.  Must have number_person_types values and be consistent
// with the enum class PersonType.
unsigned int GetPersonIndex(PersonType pt);

// Process data item and classify person based on contents, then place on any
// applicable Person-type lists.
void ProcessPerson(const std::string &item, persons_array *pa);

// Given a file containing data about Persons, populate them into the 4 lists in
// the persons_array.
void PopulateLists(const std::string &file_path, persons_array *pa);

// The string comparison operator that SortLists() needs in order to find the
// Person last names inside the shared_ptrs.
bool last_name_comparison(const std::shared_ptr<Person> first,
                          const std::shared_ptr<Person> second);

// Sort the lists of shared_ptrs to Persons inside the persons_array.
void SortLists(persons_array *unsorted);

} // namespace people_roles
#endif
