#include "multiple_inheritance.h"

#include <sys/errno.h>

#include <iostream>

namespace people_roles {

// non-member functions
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

// Non-member printer functions
std::ostream &operator<<(std::ostream &out, const Person &person) {
  std::cout << "Person printer" << std::endl;
  out << "Name: " << person.name() << ", "
      << "Address: " << person.address() << ", "
      << "Gender: " << person.gender() << ", Birthday: " << person.birthday()
      << std::endl;
  return out;
}

std::ostream &operator<<(std::ostream &out, const Student &st) {
  std::cout << "Student printer" << std::endl;
  if (typeid(Student) != typeid(st)) {
    std::cerr << "typeid is " << typeid(st).name() << std::endl;
  }

  out << ", Student id: " << st.student_id()
      << ", Study Year: " << st.study_year() << ", GPA: " << st.gpa()
      << std::endl;
  return out;
}

std::ostream &operator<<(std::ostream &out, const Worker &worker) {
  std::cout << "Worker printer" << std::endl;
  if (typeid(Worker) != typeid(worker)) {
    std::cerr << "typeid is " << typeid(worker).name() << std::endl;
  }

  operator<<(out, worker);
  out << ", Badge number: " << worker.badge_number()
      << ", Work Status: " << worker.work_status()
      << ", Start Date: " << worker.start_date() << std::endl;
  return out;
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

} // namespace people_roles
