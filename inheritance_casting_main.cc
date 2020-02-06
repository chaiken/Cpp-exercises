#include "student_inheritance.h"

#include <cstdlib>

#include <fstream>

using namespace student_inheritance;
using namespace std;

const struct student_details marvin_details(Year::kJunior, 123, 4.2, "Marvin");
const struct student_details angela_details(Year::kGrad, 124, 2.4, "Angela");
const struct grad_student_extra angela_extra(
    Support::kRA, "Physics",
    "Anistropic Superconductivity in Graphite Intercalation Compounds");

int main() {
  Student marvin(marvin_details), *ps;
  GradStudent angela(angela_details, angela_extra), *pgs;

  cout << "With operator<<()." << endl;

  cout << "Without casting:" << endl;
  ps = &marvin;
  // base printer
  // Name: Marvin, 123, Junior, 4.2
  cout << *ps << endl;

  // Without an ostringstream, the GradStudent method is never called, as cout
  // is an ostream.
  ostringstream oss;
  pgs = &angela;
  oss << *pgs;
  // Grad student printer
  // base printer
  // Name: Angela, 124, Grad, 2.4
  // , Physics, Support: Research assistant, Thesis: Anistropic
  // Superconductivity in Graphite Intercalation Compounds
  cout << oss.str() << endl << endl;
  oss.str().clear();

  cout << "Print GradStudent from Student pointer:" << endl;
  ps = &angela;
  oss << *ps;
  // base printer
  // Name: Angela, 124, Grad, 2.4
  //, Physics, Support: Research assistant, Thesis: Anistropic Superconductivity
  //in Graphite Intercalation CompoundsName: Angela, 124, Grad, 2.4
  // The following matches GradStudent's function signature.
  cout << oss.str() << endl << endl;
  // max_size: 3fffffffffffffff
  // cout << "max_size: " << hex << oss.str().max_size() << endl;
  oss.str().clear();

  // Casting a Student to a GradStudent results in an attempt by operator+() of
  // std::string to allocate memory of the size of the uninitialized gs.dept_,
  // which is a random 64-bit number.
  //
  // Grad student printer
  // Cast a Student to a GradStudent:
  // Breakpoint 2, student_inheritance::operator<< (out=..., gs=...) at
  // student_inheritance_lib.cc:77 77        ::std::cout << "Grad student
  // printer" << ::std::endl; (gdb) n Grad student printer 82        const
  // Student &st(::std::move(gs)); (gdb) n 90        ::std::string
  // retval(out.str()); (gdb) n 92                  ", Thesis: " + gs.thesis_;
  // (gdb) n
  // 91        retval += ", " + gs.dept_ + ", Support: " + gs.support() +
  // (gdb) n
  // =================================================================
  // ==454880==ERROR: AddressSanitizer: requested allocation size 0x7fffffffdb83
  // (0x7fffffffeb88 after adjustments for alignment, red zones etc.) exceeds
  // maximum supported size of 0x10000000000 (thread T0)
  //    #0 0x7ffff768819f in operator new(unsigned long)
  //    (/usr/lib/x86_64-linux-gnu/libasan.so.5+0x10919f) #1 0x7ffff74de20b in
  //    std::__cxx11::basic_string<char, std::char_traits<char>,
  //    std::allocator<char> >::reserve(unsigned long)
  //    (/usr/lib/x86_64-linux-gnu/libstdc++.so.6+0x13920b) #2 0x5555555a65df
  //    (/home/alison/gitsrc/Cpp-Exercises/inheritance_casting_main+0x525df)
  /*  cout << "Cast a Student to a GradStudent:" << endl;
  ps = &marvin;
  oss << *(reinterpret_cast<GradStudent *>(ps));
  cout << oss.str() << endl << endl;
  oss.str().clear(); */

  cout << "Cast a GradStudent to a Student:" << endl;
  oss << *(dynamic_cast<Student *>(pgs));
  // Matches GradStudent's function signature.
  // base printer
  // Name: Angela, 124, Grad, 2.4
  // , Physics, Support: Research assistant, Thesis: Anistropic
  // Superconductivity in Graphite Intercalation CompoundsName: Angela, 124,
  // Grad, 2.4 Name: Angela, 124, Grad, 2.4
  cout << oss.str() << endl << endl;
  oss.str().clear();

  cout << "Try to force Student printing of GradStudent by matching Student's "
          "function signature:"
       << endl;
  // No public ostream default ctor.
  filebuf fb;
  fb.open("/tmp/foo", ios::out);
  ostream os(&fb);
  // Result is Student printout:
  // Name: Angela, 124, Grad, 2.4
  operator<<(os, *pgs);
  fb.close();

  cout << endl
       << "Try to force GradStudent printing of Student by matching "
          "GradStudent's function signature:"
       << endl;
  operator<<(oss, *ps);
  cout << oss.str() << endl;

  cout << endl << endl << "With print()." << endl;
  cout << "Without casting:" << endl;
  ps = &marvin;
  // base printer
  // Name: Marvin, 123, Junior, 4.2
  ps->print();

  pgs = &angela;
  pgs->print();

  cout << endl << endl << "Print GradStudent from Student pointer:" << endl;
  ps = &angela;
  ps->print();

  /*
  clang-format off
  Same failure with uninitialized fields as above.
  cout << endl << "Cast a Student to a GradStudent:" << endl;
  ps = &marvin;
  reinterpret_cast<GradStudent *>(ps)->print();
  clang-format on
  */

  cout << endl << endl << "Cast a GradStudent to a Student:" << endl;
  pgs = &angela;
  dynamic_cast<Student *>(pgs)->print();

  cout << endl
       << "Try to force Student printing of GradStudent by direct invocation:"
       << endl;
  pgs = &angela;
  pgs->Student::print();

  /*
  clang-format off
  inheritance_casting_main.cc:138:20: error: ‘student_inheritance::GradStudent’ is not a base of ‘student_inheritance::Student’
  cout << "Try to force GradStudent printing of Student by direct invocation:" << endl;
  ps = &marvin;
  ps->GradStudent::print();
  clang-format on
 */
}
