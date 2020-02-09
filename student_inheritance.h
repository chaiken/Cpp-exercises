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
  // Needed by Exercise 2 of Chapter 8.
  void print();
  double gpa() const { return gpa_; }

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
  // Setting the value of a variable that is a member of the parent class has no
  // effect on the value inside that class except in the case, tested below,
  // where the parent class' member Student::gpa_ is also explicitly a member of
  // the derived class.
  void setGPA(double newgpa) { gpa_ = newgpa; }
  double gpa() const { return gpa_; }

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
  // Needed by Exercise 2 of Chapter 8.
  void print();

protected:
  // clang-format off
  // Without the lines
  // double Student::gpa_;
  // or equivalently simply
  // double gpa_;
  //   CrossMembership test passes.   With it uncommented:
  // RUN      ] StudentInheritanceTest.CrossMembership
  // GradStudent printer
  // Possibly uninitialized GradStudent::GPA is 5.2
  // Student printer
  // GPA is 2.4
  // student_inheritance_lib_test.cc:50: Failure
  // Expected equality of these values:
  //  oss.str()
  //    Which is: "Name: Angela, 124, Grad, 2.4\n, Physics, Support: Research assistant, Thesis: Anistropic Superconductivity in Graphite Intercalation Compounds"
  // "Name: Angela, 124, Grad, 5.2\n, Physics, Support: " "Research assistant, Thesis: Anistropic " "Superconductivity in Graphite Intercalation Compounds"
  // Which is: "Name: Angela, 124, Grad, 5.2\n, Physics, Support: Research assistant, Thesis: Anistropic Superconductivity in Graphite Intercalation Compounds"
  // clang-format on
  Support support_;
  ::std::string dept_;
  ::std::string thesis_;
};

::std::ostringstream &operator<<(::std::ostringstream &out,
                                 const GradStudent &gs);
} // namespace student_inheritance

#endif
