#include "student_inheritance.h"

namespace student_inheritance {

Student::Student(struct student_details sd)
    : name_(sd.name), student_id_(sd.id), gpa_(sd.GPA), y_(sd.y) {
  ::std::map<Year, ::std::string>::const_iterator idx =
      YearDescription.find(y_);
  // Prevents a Student* from pointing to a GradStudent.
  //    if ((YearDescription.end() == idx) || (Year::kGrad == idx->first)) {
  if (YearDescription.end() == idx) {
    ::std::cerr << "Illegal year for student." << ::std::endl;
    assert_perror(EINVAL);
  }
}
Student::Student(Student &&st) { *this = ::std::move(st); }

Student &Student::operator=(Student &&st) {
  name_ = st.name_;
  student_id_ = st.student_id_;
  gpa_ = st.gpa_;
  y_ = st.y_;
  return *this;
}

GradStudent::GradStudent(struct student_details sd,
                         struct grad_student_extra gse)
    : Student::Student(sd), support_(gse.support), dept_(gse.Dept),
      thesis_(gse.Thesis) {
  {
    ::std::map<Year, ::std::string>::const_iterator idx =
        YearDescription.find(y_);
    if ((YearDescription.end() == idx) || (Year::kGrad != y_)) {
      ::std::cerr << "Illegal year for graduate student." << ::std::endl;
      assert_perror(EINVAL);
    }
  }
  {
    ::std::map<Support, ::std::string>::const_iterator idx =
        SupportDescription.find(support_);
    if (SupportDescription.end() == idx) {
      ::std::cerr << "Illegal support." << ::std::endl;
      assert_perror(EINVAL);
    }
  }
  ::std::cout << "grad student constructor" << ::std::endl;
}

// Without the initialization, the call to the Student constructor will not
// match the prototypes.
GradStudent::GradStudent(GradStudent &&gs) : Student(::std::move(gs)) {
  *this = ::std::move(gs);
}

GradStudent &GradStudent::operator=(GradStudent &&gs) {
  name_ = gs.name_;
  student_id_ = gs.student_id_;
  gpa_ = gs.gpa_;
  y_ = gs.y_;
  support_ = gs.support_;
  dept_ = gs.dept_;
  thesis_ = gs.thesis_;
  return *this;
}

::std::ostream &operator<<(::std::ostream &out, const Student &st) {
  ::std::cout << "Student printer" << ::std::endl;
  out << "Name: " << st.name_ << ", " << st.student_id_ << ", " << st.year()
      << ", " << st.gpa_ << ::std::endl;
  return out;
}

::std::ostringstream &operator<<(::std::ostringstream &out,
                                 const GradStudent &gs) {
  ::std::cout << "GradStudent printer" << ::std::endl;
  // clang-format off
  // error: use of deleted function ‘student_inheritance::Student::Student(const student_inheritance::Student&)’
  // clang-format on
  // because the following line requires a copy.
  // const Student st = gs;
  const Student &st(::std::move(gs));
  out << st;
  ::std::string retval = ", " + gs.dept_ + ", Support: " + gs.support() +
                         ", Thesis: " + gs.thesis_;
  // ostringstream does not supply an operator+().
  ::std::operator<<(out, retval);
  return out;
}

void Student::print() { ::std::cout << *this; }

void GradStudent::print() {
  ::std::ostringstream oss;
  oss << *this;
  ::std::cout << oss.str();
}

} // namespace student_inheritance
