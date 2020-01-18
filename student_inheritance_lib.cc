#include "student_inheritance.h"

namespace student_inheritance {

::std::ostream &operator<<(::std::ostream &out, const Student &st) {
  ::std::cout << "Student printer" << ::std::endl;
  out << "Name: " << st.name_ << ", " << st.student_id_ << ", " << st.year()
      << ", " << st.gpa_ << ::std::endl;
  return out;
}

::std::ostringstream &operator<<(::std::ostringstream &out,
                                 const GradStudent &gs) {
  ::std::cout << "GradStudent printer" << ::std::endl;
  const Student st = gs;
  out << st;
  ::std::string retval = ", " + gs.dept_ + ", Support: " + gs.support() +
                         ", Thesis: " + gs.thesis_;
  // ostringstream does not supply an operator+().
  ::std::operator<<(out, retval);
  return out;
}

} // namespace student_inheritance
