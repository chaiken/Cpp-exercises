#ifndef MACRO_TESTING_H
#define MACRO_TESTING_H

namespace macro_testing {

// https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.cbclx01/template_non-type_arguments.hhtm
// A non-type template argument provided within a template argument list is an
// expression whose value can be determined at compile time. Such arguments must
// be constant expressions, addresses of functions or objects with external
// linkage, or addresses of static class members. Non-type template arguments
// are normally used to initialize a class or to specify the sizes of class
// members.
// In this example, the template argument size becomes a part of the template
// class name. An object of such a template class is created with both the type
// argument T of the class and the value of the non-type template argument size.
template <typename T> T cube(T x) { return (x * x * x); }

} // namespace macro_testing

#endif
