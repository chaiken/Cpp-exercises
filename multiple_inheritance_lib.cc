#include "multiple_inheritance.h"

#include <sys/errno.h>

#include <fstream>
#include <iostream>
#include <string>

namespace people_roles {
namespace {
// The following three functions process a string into a number in an
// exception-avoiding manner, and return a nonsense value on failure.
// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
unsigned int convert_unsigned_detail(const std::string detail) {
  std::string::const_iterator it = detail.begin();
  while (it != detail.end() && std::isdigit(*it)) {
    ++it;
  }
  if (!detail.empty() && (detail.end() == it)) {
    return stoul(detail);
  }
  return 0u;
}

int convert_int_detail(const std::string detail) {
  std::string::const_iterator it = detail.begin();
  while (it != detail.end() && (std::isdigit(*it) || ('.' == *it))) {
    ++it;
  }
  if (!detail.empty() && (detail.end() == it)) {
    return stoi(detail);
  }
  return -1.0;
}

double convert_double_detail(const std::string detail) {
  std::string::const_iterator it = detail.begin();
  while (it != detail.end() && (std::isdigit(*it) || ('.' == *it))) {
    ++it;
  }
  if (!detail.empty() && (detail.end() == it)) {
    return stod(detail);
  }
  return -1.0;
}
} // namespace

// non-member functions

unsigned int GetPersonIndex(PersonType pt) {
  switch (pt) {
  case PersonType::Person:
    return 0u;
  case PersonType::Student:
    return 1u;
  case PersonType::Worker:
    return 2u;
  case PersonType::StudentWorker:
    return 3u;
  default:
    std::cerr << "Bad PersonType" << std::endl;
    assert_perror(EINVAL);
  }
}

bool DayOfMonthIsInvalid(const unsigned int day, const Month month) {
  if ((day > 31) || ((Month::kFeb == month) && (day > 28)) ||
      (((Month::kSep == month) || (Month::kApr == month) ||
        (Month::kJun == month)) &&
       (day > 30))) {
    return true;
  }
  return false;
}

bool YearIsInvalid(const unsigned int year) {
  const time_t currtime = time(nullptr);
  const struct tm *now = localtime(&currtime);
  if (static_cast<unsigned int>(now->tm_year + 1900) < year) {
    std::cout << "Invalid birth year " << year << std::endl;
    return true;
  }
  return false;
}

Gender LookupGender(const std::string gender) {
  if ((!gender.compare("f")) || (!gender.compare("F"))) {
    return Gender::kFemale;
  }
  if ((!gender.compare("m")) || (!gender.compare("M"))) {
    return Gender::kMale;
  }
  if ((!gender.compare("o")) || (!gender.compare("O"))) {
    return Gender::kOther;
  }
  return Gender::kUnknown;
}

Month LookupMonth(const unsigned int birth_month) {
  switch (birth_month) {
  case 1:
    return Month::kJan;
  case 2:
    return Month::kFeb;
  case 3:
    return Month::kMar;
  case 4:
    return Month::kApr;
  case 5:
    return Month::kMay;
  case 6:
    return Month::kJun;
  case 7:
    return Month::kJul;
  case 8:
    return Month::kAug;
  case 9:
    return Month::kSep;
  case 10:
    return Month::kOct;
  case 11:
    return Month::kNov;
  case 12:
    return Month::kDec;
  default:
    std::cerr << "Invalid month: " << birth_month << std::endl;
    assert_perror(EINVAL);
  }
}

StudyYear LookupStudyYear(const std::string study_year) {
  if (!study_year.compare("F") || !study_year.compare("f")) {
    return StudyYear::kFresh;
  }
  if (!study_year.compare("So") || !study_year.compare("so")) {
    return StudyYear::kSoph;
  }
  if (!study_year.compare("J") || !study_year.compare("j")) {
    return StudyYear::kJunior;
  }
  if (!study_year.compare("Se") || !study_year.compare("se")) {
    return StudyYear::kSenior;
  }
  return StudyYear::kUnknown;
}

WorkStatus LookupWorkStatus(const std::string work_status) {
  if ((!work_status.compare("f")) || (!work_status.compare("F"))) {
    return WorkStatus::kFulltime;
  }
  if ((!work_status.compare("p")) || (!work_status.compare("P"))) {
    return WorkStatus::kParttime;
  }
  if ((!work_status.compare("s")) || (!work_status.compare("S"))) {
    return WorkStatus::kStudent;
  }
  if ((!work_status.compare("r")) || (!work_status.compare("R"))) {
    return WorkStatus::kRetired;
  }
  return WorkStatus::kUnknown;
}

// Pretty-print three appropriate numbers into a date string.
std::string FormatDate(const unsigned int day, const Month month,
                       const unsigned int year) {
  std::map<Month, std::string>::const_iterator it =
      MonthDescription.find(month);
  if ((MonthDescription.cend() == it) || (day > 31u) || (0u == day)) {
    std::cerr << "Illegal month." << std::endl;
    assert_perror(EINVAL);
  }
  std::string date =
      it->second + " " + std::to_string(day) + ", " + std::to_string(year);
  return date;
}

std::string GetDetail(const std::string item, const std::string detail) {
  const size_t cursor = item.find(detail);
  if (std::string::npos == cursor) {
    return "Unknown";
  }
  // Go past ": ".
  size_t begin = cursor + detail.length() + 2;
  // If end is npos, the field is the last on the line.
  size_t end = item.find(",", begin);
  // If end is npos, the field is the last on the line.
  if (std::string::npos == end) {
    return item.substr(begin, std::string::npos);
  }
  // Do not include the comma.
  return item.substr(begin, (end - begin));
}

struct person_details PopulatePersonDetails(const std::string item,
                                            bool *is_person) {
  unsigned int birth_year =
      convert_unsigned_detail((GetDetail(item, "BirthYear")));
  unsigned int birth_month =
      convert_unsigned_detail(GetDetail(item, "BirthMonth"));
  unsigned int birth_day_of_month =
      convert_unsigned_detail(GetDetail(item, "BirthDate"));
  std::string first_name = GetDetail(item, "FirstName");
  std::string last_name = GetDetail(item, "LastName");
  std::string address = GetDetail(item, "Address");
  std::string gender = GetDetail(item, "Gender");
  *is_person = ((0u != birth_year) && (0u != birth_month) &&
                (0u != birth_day_of_month) && ("Unknown" != first_name) &&
                ("Unknown" != last_name) && ("Unknown" != address) &&
                ("Unknown" != gender));
  return person_details(birth_year, birth_month, birth_day_of_month, first_name,
                        last_name, address, gender);
}

struct student_details PopulateStudentDetails(const std::string item,
                                              bool *is_student) {
  StudyYear y = LookupStudyYear(GetDetail(item, "StudyYear"));
  int id = convert_int_detail(GetDetail(item, "ID"));
  double GPA = convert_double_detail(GetDetail(item, "GPA"));
  *is_student = ((y != StudyYear::kUnknown) && (-1 != id) && (-1.0 != GPA));
  return student_details(y, id, GPA);
}

struct worker_details PopulateWorkerDetails(const std::string item,
                                            bool *is_worker) {
  unsigned int badge_number =
      convert_unsigned_detail(GetDetail(item, "BadgeNumber"));
  unsigned int start_year =
      convert_unsigned_detail(GetDetail(item, "StartYear"));
  unsigned int start_month =
      convert_unsigned_detail(GetDetail(item, "StartMonth"));
  unsigned int start_day_of_month =
      convert_unsigned_detail(GetDetail(item, "StartDate"));
  std::string work_status = GetDetail(item, "WorkStatus");
  *is_worker =
      ((0u != badge_number) && (0u != start_year) && (0u != start_month) &&
       (0u != start_day_of_month) && ("Unknown" != work_status));
  return worker_details(badge_number, start_year, start_month,
                        start_day_of_month, work_status);
}

// Pointers to objects from the derived classes can be pushed onto the Person*
// lists.
void ProcessPerson(const std::string &item, persons_array *pa) {
  bool is_person = false, is_student = false, is_worker = false;

  const struct person_details pd = PopulatePersonDetails(item, &is_person);
  const struct student_details sd = PopulateStudentDetails(item, &is_student);
  const struct worker_details wd = PopulateWorkerDetails(item, &is_worker);
  if (!is_person) {
    std::cerr << "Illegal person data: " << item << std::endl;
    return;
  }
  if (is_student) {
    if (is_worker) {
      std::shared_ptr<Person> astudentworker(new StudentWorker(pd, sd, wd));
      (*pa)[GetPersonIndex(PersonType::Person)].push_back(astudentworker);
      (*pa)[GetPersonIndex(PersonType::Student)].push_back(astudentworker);
      (*pa)[GetPersonIndex(PersonType::Worker)].push_back(astudentworker);
      (*pa)[GetPersonIndex(PersonType::StudentWorker)].push_back(
          astudentworker);
    } else {
      std::shared_ptr<Person> astudent(new Student(pd, sd));
      (*pa)[GetPersonIndex(PersonType::Person)].push_back(astudent);
      (*pa)[GetPersonIndex(PersonType::Student)].push_back(astudent);
      std::cout << "Pushing a Student" << std::endl;
    }
  } else {
    if (is_worker) {
      std::shared_ptr<Person> aworker(new Worker(pd, wd));
      (*pa)[GetPersonIndex(PersonType::Person)].push_back(aworker);
      (*pa)[GetPersonIndex(PersonType::Worker)].push_back(aworker);
    } else {
      std::shared_ptr<Person> aperson(new Person(pd));
      (*pa)[GetPersonIndex(PersonType::Person)].push_back(aperson);
      (*pa)[GetPersonIndex(PersonType::Person)].push_back(aperson);
    }
  }
}

void PopulateLists(const std::string &file_path, persons_array *pa) {
  assert(!file_path.empty());
  // Calls open() implicitly.
  std::ifstream in_file(file_path, std::ifstream::in);
  std::string item;
  if (in_file.is_open()) {
    while (in_file.good()) {
      std::getline(in_file, item);
      if (!item.empty()) {
        if (std::string::npos == item.find("LastName: ")) {
          std::cerr << "Warning: unparsable record: " << item << std::endl;
        } else {
          ProcessPerson(item, pa);
        }
      }
    }
  } else {
    std::cerr << "Open of file " << file_path << "failed." << std::endl;
    assert_perror(EPERM);
  }
  in_file.close();
}

//  Person member functions
std::string Person::gender() const {
  std::map<Gender, std::string>::const_iterator it =
      GenderDescription.find(gender_);
  if (GenderDescription.cend() == it) {
    std::cerr << "Illegal gender." << std::endl;
    assert_perror(EINVAL);
  }
  return it->second;
}

std::ostream &Person::operator<<(std::ostream &out) {
  printer_has_run_ = true;
  out << "Name: " << first_name_ << " " << last_name_ << ", "
      << "Address: " << address_ << ", "
      << "Gender: " << gender() << ", Birthday: " << birthday();
  return out;
}

// Student member functions
std::string Student::study_year() const {
  std::map<StudyYear, std::string>::const_iterator it =
      StudyYearDescription.find(y_);
  // Prevents a Student* from pointing to a GradStudent.
  //    if ((StudyYearDescription.end() == idx) || (StudyYear::kGrad ==
  //    idx->first)) {
  if (StudyYearDescription.cend() == it) {
    std::cerr << "Illegal year for student." << std::endl;
    assert_perror(EINVAL);
  }
  return it->second;
}

std::ostream &Student::operator<<(std::ostream &out) {
  if (!person_printer_has_run()) {
    std::cerr << "Student printer: calling Person printer." << std::endl;
    Person::operator<<(out);
  } else {
    std::cerr << "Student printer: Person printer has already been called."
              << std::endl;
  }
  out << ", Student id: " << student_id_ << ", Study Year: " << study_year()
      << ", GPA: " << gpa_;
  return out;
}

// Worker member functions.
std::ostream &Worker::operator<<(std::ostream &out) {
  if (!person_printer_has_run()) {
    std::cerr << "Worker printer: calling Person printer." << std::endl;
    Person::operator<<(out);
  } else {
    std::cerr << "Worker printer: Person printer has already been called."
              << std::endl;
  }
  out << ", Badge number: " << badge_number_
      << ", Work Status: " << work_status() << ", Start Date: " << start_date();
  return out;
}

// StudentWorker member functions.
std::ostream &StudentWorker::operator<<(std::ostream &out) {
  Student::operator<<(out);
  Worker::operator<<(out);
  return out;
}

} // namespace people_roles
