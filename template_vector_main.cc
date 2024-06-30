#include "template_vector.hh"

#include <cstdlib>
#include <cwchar>
#include <exception>
#include <stdexcept>
#include <vector>

using namespace template_vect;
using namespace std;

// https://www.geeksforgeeks.org/wide-char-and-library-functions-in-c/

int main(void) {
  vector<wchar_t> vec1{'a', 'b', 'c', 'd', L'ö'};
  TemplateVector<wchar_t> tv1(vec1);
  wchar_t achar = 'a';
  try {
    achar = tv1[11];
  } catch (out_of_range &oe) {
    cerr << "Setting default character value." << endl;
    achar = L'ß';
  }
  // https://stackoverflow.com/questions/2493785/how-i-can-print-the-wchar-t-values-to-console
  wcout << L"achar is now " << achar << endl;

  int index = -1;
  while ((index < 0) || (index > tv1.ub())) {
    try {
      achar = tv1[index];
    } catch (out_of_range &oe) {
      wcerr << "Please input a index between 0 and " << tv1.ub() << ":" << endl;
      wcin >> index;
    }
  }
  wcout << L"achar is now " << tv1[index] << endl;
  exit(EXIT_SUCCESS);
}

/*
clang-format off

Here, konsole encoding is UTF.
[alison@hildesheim Cpp-Exercises (master)]$ ./template_vector_main
::std::vector ctor
Setting default character value.
achar is now �

Here, konsole encoding is ISO-8859-1, although ISO-8859-15 is better for German.
[alison@hildesheim Cpp-Exercises (master)]$ ./template_vector_main
::std::vector ctor
Setting default character value.
achar is now ß
clang-format on
*/
