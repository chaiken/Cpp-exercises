#include "student_inheritance.h"

#include <typeinfo>

namespace student_inheritance {

bool operator==(const std::string &support_description, const Support support) {
  if (support_description == SupportDescription.find(support)->second) {
    return true;
  }
  return false;
}

bool operator!=(const std::string &support_description, const Support support) {
  return (!operator==(support_description, support));
}

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
    : Student::Student(sd), support_(gse.support), dept_(gse.dept),
      thesis_(gse.thesis) {
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
}

// Without the initialization, the call to the Student constructor will not
// match the prototypes.  Why does the function need to call move() given that
// it's receiving an R-value reference already?
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
  if (typeid(Student) != typeid(st)) {
#ifdef DEBUG
    // Detects if the Student reference contains a StudentWorker derived type.
    ::std::cout << typeid(st).name() << ::std::endl;
#endif
  }
  ::std::cout << "GPA is " << st.gpa() << ::std::endl;
  out << "Name: " << st.name_ << ", " << st.student_id_ << ", " << st.year()
      << ", " << st.gpa_ << ::std::endl;
  return out;
}

// This function is a friend of GradStudent, but not of Student, so
// it cannot access Student::gpa_.
::std::ostringstream &operator<<(::std::ostringstream &out,
                                 const GradStudent &gs) {
  ::std::cout << "GradStudent printer" << ::std::endl;
  if (typeid(GradStudent) != typeid(gs)) {
#ifdef DEBUG
    ::std::cout << typeid(gs).name() << ::std::endl;
#endif
  }
  // The GPA of GradStudent is uninitialized except for the tested case that
  // Student::gpa_ is a member of GradStudent.
  //  ::std::cout << "Possibly uninitialized GradStudent::GPA is " << gs.gpa()
  //              << ::std::endl;
  // clang-format off
  // error: use of deleted function ‘student_inheritance::Student::Student(const student_inheritance::Student&)’
  // clang-format on
  // because the following line requires a copy.
  // const Student st = gs;
  const Student &st(::std::move(gs));
  out << st;
  ::std::string retval;
  // Check for malformed input.   These assertions appear to offer no protection
  // in the event that variables are uninitialized due a cast.  Not sure why
  // since the value is larger than max_size().
  assert((gs.dept_.size() > 0u) && (gs.dept_.size() < retval.max_size()));
  assert((gs.thesis_.size() > 0u) && (gs.thesis_.size() < retval.max_size()));
  retval = ", " + gs.dept_ + ", Support: " + gs.support() +
           ", Thesis: " + gs.thesis_;
  // ostringstream does not supply an operator+().
  ::std::operator<<(out, retval);
  return out;
}

void Student::print(std::ostream &oss) const { oss << *this; }

void GradStudent::print(std::ostringstream &oss) const {
  oss << "Student::gpa_ is not protected inside this member-function wrapper: "
      << Student::gpa_ << ::std::endl;
  // Calls the Student operator<<(), not the GradStudent one.
  operator<<(oss, *this);
}

} // namespace student_inheritance
