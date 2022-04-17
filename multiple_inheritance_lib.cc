#include "multiple_inheritance.h"

#include <cassert>
#include <sys/errno.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace people_roles {
namespace {
// The following three functions process a string into a number in an
// exception-avoiding manner, and return a nonsense value on failure.
// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
unsigned int convert_unsigned_detail(const string detail) {
  string::const_iterator it = detail.begin();
  while (it != detail.end() && isdigit(*it)) {
    ++it;
  }
  if (!detail.empty() && (detail.end() == it)) {
    return stoul(detail);
  }
  return 0u;
}

int convert_int_detail(const string detail) {
  string::const_iterator it = detail.begin();
  while (it != detail.end() && (isdigit(*it) || ('.' == *it))) {
    ++it;
  }
  if (!detail.empty() && (detail.end() == it)) {
    return stoi(detail);
  }
  return -1.0;
}

double convert_double_detail(const string detail) {
  string::const_iterator it = detail.begin();
  while (it != detail.end() && (isdigit(*it) || ('.' == *it))) {
    ++it;
  }
  if (!detail.empty() && (detail.end() == it)) {
    return stod(detail);
  }
  return -1.0;
}

bool file_operations_okay(const ifstream &ifile, const string &path,
                          const int err) {
  // http://www.cplusplus.com/reference/string/string/getline/
  // "Notice that some eofbit cases will also set failbit.  Operations that
  // attempt to read at the End-of-File fail, and thus both the eofbit and the
  // failbit end up set."
  if (ifile.eof()) {
    if (!err) {
      return true;
    } else {
      return false;
    }
  }
  if (ifile.fail()) {
    cerr << "Logical error on I/O operation for " << path << "." << endl;
    return false;
  }
  if (ifile.bad()) {
    cerr << "Read/writing error on I/O operation for " << path << "." << endl;
    return false;
  }
  return true;
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
    out_of_range re("Bad PersonType");
    throw RolesException(re);
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
    cout << "Invalid birth year " << year << endl;
    return true;
  }
  return false;
}

Gender LookupGender(const string gender) {
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
    out_of_range re("Invalid month");
    throw RolesException(re);
  }
}

StudyYear LookupStudyYear(const string study_year) {
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

WorkStatus LookupWorkStatus(const string work_status) {
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
// Return a failure status from this function is overkill given that ctors
// already check the input parameters.
string FormatDate(const unsigned int day, const Month month,
                  const unsigned int year) {
  map<Month, string>::const_iterator it = MonthDescription.find(month);
  if ((MonthDescription.cend() == it) || (day > 31u) || (0u == day)) {
    return "malformed date data";
  }
  string date = it->second + " " + to_string(day) + ", " + to_string(year);
  return date;
}

void PrintList(const list<shared_ptr<Person>> &pl) {
  // Without this check, typeid() throws an exception on an empty list.
  if (pl.empty()) {
    return;
  }
  // tn cannot be empty after this cal, as on errors typeid() will throw an
  // exception.
  // clang-format off
  // clang-tidy complains
  // warning: expression with side effects will be evaluated despite being used as an operand to 'typeid' [clang-diagnostic-potentially-evaluated-expression]
  // The warning seems daft: reading the first element from a const list has "side effects"?
  // clang-format off
  // NOLINTNEXTLINE
  string tn = typeid(*(pl.front().get())).name();
  cout << endl << "List of " << tn << endl;
  for (auto x : pl) {
    x.get()->operator<<(cout);
    cout << endl;
  }
}

string GetDetail(const string item, const string detail) {
  const size_t cursor = item.find(detail);
  if (string::npos == cursor) {
    return "Unknown";
  }
  // Go past ": ".
  size_t begin = cursor + detail.length() + 2;
  // If end is npos, the field is the last on the line.
  size_t end = item.find(",", begin);
  // If end is npos, the field is the last on the line.
  if (string::npos == end) {
    return item.substr(begin, string::npos);
  }
  // Do not include the comma.
  return item.substr(begin, (end - begin));
}

struct person_details PopulatePersonDetails(const string item,
                                            bool *is_person) {
  unsigned int birth_year =
      convert_unsigned_detail((GetDetail(item, "BirthYear")));
  unsigned int birth_month =
      convert_unsigned_detail(GetDetail(item, "BirthMonth"));
  unsigned int birth_day_of_month =
      convert_unsigned_detail(GetDetail(item, "BirthDate"));
  string first_name = GetDetail(item, "FirstName");
  string last_name = GetDetail(item, "LastName");
  string address = GetDetail(item, "Address");
  string gender = GetDetail(item, "Gender");
  *is_person = ((0u != birth_year) && (0u != birth_month) &&
                (0u != birth_day_of_month) && ("Unknown" != first_name) &&
                ("Unknown" != last_name) && ("Unknown" != address) &&
                ("Unknown" != gender));
  return person_details(birth_year, birth_month, birth_day_of_month, first_name,
                        last_name, address, gender);
}

struct student_details PopulateStudentDetails(const string item,
                                              bool *is_student) {
  StudyYear y = LookupStudyYear(GetDetail(item, "StudyYear"));
  int id = convert_int_detail(GetDetail(item, "ID"));
  double GPA = convert_double_detail(GetDetail(item, "GPA"));
  *is_student = ((y != StudyYear::kUnknown) && (-1 != id) && (-1.0 != GPA));
  return student_details(y, id, GPA);
}

struct worker_details PopulateWorkerDetails(const string item,
                                            bool *is_worker) {
  unsigned int badge_number =
      convert_unsigned_detail(GetDetail(item, "BadgeNumber"));
  unsigned int start_year =
      convert_unsigned_detail(GetDetail(item, "StartYear"));
  unsigned int start_month =
      convert_unsigned_detail(GetDetail(item, "StartMonth"));
  unsigned int start_day_of_month =
      convert_unsigned_detail(GetDetail(item, "StartDate"));
  string work_status = GetDetail(item, "WorkStatus");
  *is_worker =
      ((0u != badge_number) && (0u != start_year) && (0u != start_month) &&
       (0u != start_day_of_month) && ("Unknown" != work_status));
  return worker_details(badge_number, start_year, start_month,
                        start_day_of_month, work_status);
}

// Pointers to objects from the derived classes can be pushed onto the Person*
// lists.
void ProcessPerson(const string &item, persons_array *pa) {
  bool is_person = false, is_student = false, is_worker = false;

  const struct person_details pd = PopulatePersonDetails(item, &is_person);
  const struct student_details sd = PopulateStudentDetails(item, &is_student);
  const struct worker_details wd = PopulateWorkerDetails(item, &is_worker);
  if (item.empty() || (!is_person)) {
    cerr << "Illegal person data: " << item << endl;
    return;
  }
  if (is_student) {
    if (is_worker) {
      shared_ptr<Person> astudentworker(new StudentWorker(pd, sd, wd));
      (*pa)[GetPersonIndex(PersonType::Person)].push_back(astudentworker);
      (*pa)[GetPersonIndex(PersonType::Student)].push_back(astudentworker);
      (*pa)[GetPersonIndex(PersonType::Worker)].push_back(astudentworker);
      (*pa)[GetPersonIndex(PersonType::StudentWorker)].push_back(
          astudentworker);
    } else {
      shared_ptr<Person> astudent(new Student(pd, sd));
      (*pa)[GetPersonIndex(PersonType::Person)].push_back(astudent);
      (*pa)[GetPersonIndex(PersonType::Student)].push_back(astudent);
    }
  } else {
    if (is_worker) {
      shared_ptr<Person> aworker(new Worker(pd, wd));
      (*pa)[GetPersonIndex(PersonType::Person)].push_back(aworker);
      (*pa)[GetPersonIndex(PersonType::Worker)].push_back(aworker);
    } else {
      shared_ptr<Person> aperson(new Person(pd));
      (*pa)[GetPersonIndex(PersonType::Person)].push_back(aperson);
    }
  }
}

bool PopulateLists(const string &file_path, persons_array *pa) {
  if (file_path.empty()) {
    cerr << "Please provide an input filename.";
    return false;
  }
  // Calls open() implicitly.
  ifstream in_file(file_path);
  if (in_file.is_open()) {
    // Works because operator!() is defined for ios.   Checks failbit and
    // badbit; see
    // https://gehrcke.de/2011/06/reading-files-in-c-using-ifstream-dealing-correctly-with-badbit-failbit-eofbit-and-perror/
    errno = 0;
    string item;
    // "getline() actually returns the stream object which is evaluated in a
    // bool expression in the loop header."
    // http://www.cplusplus.com/reference/string/string/getline/
    // The call below is the string class version of getline(), not the
    // getline() function which is a member of the istream class, which has
    // signature
    // clang-format off
    // istream's version: istream& getline (char* s, streamsize n, char delim );
    // string's version: istream& getline (istream&  is, string& str, char delim);
    // http://www.cplusplus.com/reference/ios/ios/eof/
    // "Operations that attempt to read at the End-of-File fail, and thus both
    // "the eofbit and the failbit end up set."
    // clang-format on
    while ((!in_file.eof()) && (getline(in_file, item))) {
      if (!item.empty()) {
        if (string::npos == item.find("LastName: ")) {
          cerr << "Warning: unparsable record: " << item << endl;
          return false;
        } else {
          ProcessPerson(item, pa);
        }
      }
    }
    if (!file_operations_okay(in_file, file_path, errno)) {
      return false;
    }
  } else {
    if (!file_operations_okay(in_file, file_path, errno)) {
      return false;
    }
  }
  in_file.close();
  return true;
}

bool last_name_comparison(const shared_ptr<Person> first,
                          const shared_ptr<Person> second) {
  return (first.get()->last_name() < second.get()->last_name());
}

void SortLists(persons_array *unsorted) {
  for (uint32_t i = 0u; i < number_person_types; i++) {
    (*unsorted)[i].sort(last_name_comparison);
  }
}

//  Person member functions
string Person::gender() const {
  map<Gender, string>::const_iterator it = GenderDescription.find(gender_);
  /* Because there is a default case, the following code is unreachable:
  if (GenderDescription.cend() == it) {
    return GenderDescription.find(Gender::kUnknown)->second;
  }
  */
  return it->second;
}

ostream &Person::operator<<(ostream &out) {
  set_printer_has_run(true);
  out << "Name: " << first_name_ << " " << last_name_ << ", "
      << "Address: " << address_ << ", "
      << "Gender: " << gender() << ", Birthday: " << birthday();
  return out;
}

// Student member functions
string Student::study_year() const {
  map<StudyYear, string>::const_iterator it = StudyYearDescription.find(y_);
  // Prevents a Student* from pointing to a GradStudent.
  //    if ((StudyYearDescription.end() == idx) || (StudyYear::kGrad ==
  //    idx->first)) {
  /* Because there is a default case, the following code is unreachable:
  if (StudyYearDescription.cend() == it) {
    return StudyYearDescription.find(StudyYear::kUnknown)->second;
  }
  */
  return it->second;
}

ostream &Student::operator<<(ostream &out) {
  if (!printer_has_run()) {
    Person::operator<<(out);
  }
#ifdef DEBUG
  else {
    cerr << "Student printer: Person printer has already been called." << endl;
  }
#endif
  out << ", Student id: " << student_id_ << ", Study Year: " << study_year()
      << ", GPA: " << gpa_;
  // The cast succeeds when the Person is a StudentWorker, but fails for a plain
  // Student, which is just what is needed here. This type detection method
  // comes from Stroustrup's _A Tour of C++_, 2nd Edition, p. 61.
  Worker *w = dynamic_cast<Worker *>(this);
  if (nullptr == w) {
#ifdef DEBUG
    cerr << "***Worker cast failed.***" << endl;
#endif
    set_printer_has_run(false);
  }
  return out;
}

// Worker member functions.
ostream &Worker::operator<<(ostream &out) {
  if (!printer_has_run()) {
#ifdef DEBUG
    cerr << "Worker printer: calling Person printer." << endl;
#endif
    Person::operator<<(out);
  }
#ifdef DEBUG
  else {
    cerr << "Student printer: Person printer has already been called." << endl;
  }
#endif

  out << ", Badge number: " << badge_number_
      << ", Work Status: " << work_status() << ", Start Date: " << start_date();
  // The cast succeeds when the Person is a StudentWorker, but fails for a plain
  // Worker, which is just what is needed here. This type detection method comes
  // from Stroustrup's _A Tour of C++_, 2nd Edition, p. 61.
  Student *st = dynamic_cast<Student *>(this);
  if (nullptr == st) {
#ifdef DEBUG
    cerr << "****Student cast failed.***" << endl;
#endif
    set_printer_has_run(false);
  }
  return out;
}

// StudentWorker member functions.
ostream &StudentWorker::operator<<(ostream &out) {
  Student::operator<<(out);
  Worker::operator<<(out);
  set_printer_has_run(false);
  return out;
}

} // namespace people_roles
