#ifndef STUDENT_INHERITANCE_H
#define STUDENT_INHERITANCE_H

#include <cassert>
#include <cstring>

#include <iostream>
#include <map>
#include <sstream>

namespace student_inheritance {

enum class Year { kFresh, kSoph, kJunior, kSenior, kGrad };
enum class Support { kTA, kRA, kFellowship, kOther };

namespace {
// maps apparently cannot be constexpr as they are not literal types.
const ::std::map<Year, ::std::string> YearDescription = {
    {Year::kFresh, "Freshman"}, {Year::kSoph, "Sophomore"},
    {Year::kJunior, "Junior"},  {Year::kSenior, "Senior"},
    {Year::kGrad, "Grad"},
};
const ::std::map<Support, ::std::string> SupportDescription{
    {Support::kTA, "Teaching assistant"},
    {Support::kRA, "Research assistant"},
    {Support::kFellowship, "Fellowship"},
    {Support::kOther, "Other"},
};
} // namespace

struct student_details {
  student_details(Year a, int b, double c, ::std::string d)
      : y(a), id(b), GPA(c), name(d) {}
  Year y;
  const int id;
  double GPA;
  const ::std::string name;
};

struct grad_student_extra {
  grad_student_extra(Support a, ::std::string b, ::std::string c)
      : support(a), Dept(b.c_str()), Thesis(c.c_str()) {}
  Support support;
  const ::std::string Dept;
  const ::std::string Thesis;
};

class Student {
public:
  Student(struct student_details sd);
  Student(Student &&st);
  Student &operator=(Student &&st);
  ::std::string year() const { return YearDescription.find(y_)->second; }
  friend ::std::ostream &operator<<(::std::ostream &out, const Student &st);

protected:
  ::std::string name_;
  int student_id_;
  double gpa_;
  Year y_;
};
// The order of these arguments can't be controlled?
::std::ostream &operator<<(::std::ostream &out, const Student &st);

class GradStudent : public Student {
public:
  GradStudent(struct student_details sd, struct grad_student_extra gse);
  GradStudent(GradStudent &&gs);
  GradStudent &operator=(GradStudent &&gs);
  ::std::string support() const {
    return SupportDescription.find(support_)->second;
  }

  // Friend functions cannot be virtual or override.  Fails here since
  // GradStudent is a type of Student, so the anticipated function
  // overload isn't provided by the compiler.
  //
  // undefined reference to `student_inheritance::operator<<(std::ostream&,
  // student_inheritance::Student const&)' /usr/bin/ld:
  // /home/alison/gitsrc/Cpp-Exercises/student_inheritance_lib_test.cc:24:
  // undefined reference to `student_inheritance::operator<<(std::ostream&,
  // student_inheritance::GradStudent const&)' collect2: error: ld returned 1
  // exit status
  // friend ::std::ostringstream &operator<<(::std::ostringstream &out, const
  // GradStudent &gs);
  friend ::std::ostringstream &operator<<(::std::ostringstream &out,
                                          const GradStudent &gs);

protected:
  Support support_;
  ::std::string dept_;
  ::std::string thesis_;
};

::std::ostringstream &operator<<(::std::ostringstream &out,
                                 const GradStudent &gs);
} // namespace student_inheritance

#endif
