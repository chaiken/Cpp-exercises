#include "student_inheritance.h"

#include <typeinfo>

using namespace std;

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
  map<Year, string>::const_iterator idx = YearDescription.find(y_);
  // Prevents a Student* from pointing to a GradStudent.
  //    if ((YearDescription.end() == idx) || (Year::kGrad == idx->first)) {
  if (YearDescription.end() == idx) {
    const std::invalid_argument ia{"Illegal year for student."};
    throw StudentException(ia);
  }
}

//  'The state of the default constructor is often the “natural” state of a
//  moved-from object . . .'
// per Nikolai Josuttis in _C++ Move Semantics_, p. 57.
// There is no default ctor, but it's clear what one would do.
Student::Student(Student &&st) {
  *this = move(st);
  st.name_.clear();
  st.student_id_ = -1;
  st.gpa_ = -1.0;
}

Student &Student::operator=(Student &&st) {
  name_ = st.name_;
  student_id_ = st.student_id_;
  gpa_ = st.gpa_;
  y_ = st.y_;
  st.name_.clear();
  st.student_id_ = -1;
  st.gpa_ = -1.0;
  return *this;
}

GradStudent::GradStudent(struct student_details sd,
                         struct grad_student_extra gse)
    : Student::Student(sd), support_(gse.support), dept_(gse.dept),
      thesis_(gse.thesis) {
  map<Year, string>::const_iterator idx = YearDescription.find(y_);
  if ((YearDescription.end() == idx) || (Year::kGrad != y_)) {
    const std::invalid_argument ia{"Illegal year for graduate student."};
    throw StudentException(ia);
  }
  map<Support, string>::const_iterator idx2 = SupportDescription.find(support_);
  if (SupportDescription.end() == idx2) {
    const std::invalid_argument ia{"Illegal support."};
    throw StudentException(ia);
  }
}

// Without the initialization, the call to the Student constructor will not
// match the prototypes.  Why does the function need to call move() given that
// it's receiving an R-value reference already?
GradStudent::GradStudent(GradStudent &&gs) : Student(move(gs)) {
  // Default move() of the GradStudent overwrites the values initialized by the
  // move of the  Student.
  //  *this = move(gs);
  gs.name_.clear();
  gs.student_id_ = -1;
  gs.gpa_ = -1.0;
  support_ = gs.support_;
  dept_ = gs.dept_;
  gs.dept_.clear();
  thesis_ = gs.thesis_;
  gs.thesis_.clear();
}

GradStudent &GradStudent::operator=(GradStudent &&gs) {
  name_ = gs.name_;
  student_id_ = gs.student_id_;
  gpa_ = gs.gpa_;
  y_ = gs.y_;
  support_ = gs.support_;
  dept_ = gs.dept_;
  thesis_ = gs.thesis_;
  gs.name_.clear();
  gs.student_id_ = -1;
  gs.gpa_ = -1.0;
  gs.dept_.clear();
  gs.thesis_.clear();
  return *this;
}

ostream &operator<<(ostream &out, const Student &st) {
  cout << "Student printer" << endl;
  if (typeid(Student) != typeid(st)) {
#ifdef DEBUG
    // Detects if the Student reference contains a StudentWorker derived type.
    cout << typeid(st).name() << endl;
#endif
  }
  cout << "GPA is " << st.gpa() << endl;
  out << "Name: " << st.name_ << ", " << st.student_id_ << ", "
      << st.year_description() << ", " << st.gpa_ << endl;
  return out;
}

// This function is a friend of GradStudent, but not of Student, so
// it cannot access Student::gpa_.
ostringstream &operator<<(ostringstream &out, const GradStudent &gs) {
  cout << "GradStudent printer" << endl;
  if (typeid(GradStudent) != typeid(gs)) {
#ifdef DEBUG
    cout << typeid(gs).name() << endl;
#endif
  }
  // The GPA of GradStudent is uninitialized except for the tested case that
  // Student::gpa_ is a member of GradStudent.
  //  cout << "Possibly uninitialized GradStudent::GPA is " << gs.gpa()
  //              << endl;
  // clang-format off
  // error: use of deleted function ‘student_inheritance::Student::Student(const student_inheritance::Student&)’
  // clang-format on
  // because the following line requires a copy.
  // const Student st = gs;
  struct student_details sd(gs.year(), gs.id(), gs.gpa(), gs.name());
  Student st(sd);
  out << st;
  string retval;
  // Check for malformed input.   These assertions appear to offer no
  // protection in the event that variables are uninitialized due a cast.  Not
  // sure why since the value is larger than max_size().
  //  assert((gs.dept_.size() > 0u) && (gs.dept_.size() < retval.max_size()));
  //  assert((gs.thesis_.size() > 0u) && (gs.thesis_.size() <
  //  retval.max_size()));
  retval = ", " + gs.dept_ + ", Support: " + gs.support() +
           ", Thesis: " + gs.thesis_;
  // ostringstream does not supply an operator+().
  operator<<(out, retval);
  return out;
}

void Student::print(std::ostream &oss) const { oss << *this; }

void GradStudent::print(std::ostringstream &oss) const {
  oss << "Student::gpa_ is not protected inside this member-function "
         "wrapper: "
      << Student::gpa_ << endl;
  // Calls the Student operator<<(), not the GradStudent one.
  operator<<(oss, *this);
}

} // namespace student_inheritance
