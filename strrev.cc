#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void strrev(char *s1, const char *s2) {
  int i = strlen(s2) - 1, j = 0;
  while (i-- > 0) {
    *(s1 + j) = s2[i];
    j++;
  }
  s1[i] = '\0';
}

void strrev2(char *&s1, const char *s2) {
  int i = strlen(s2) - 1, j = 0;
  while (i-- > 0) {
    *(s1 + j) = s2[i];
    j++;
  }
  s1[i] = '\0';
}

void strrev3(string output2, string input2) {
  ostringstream out(output2);
  // First -- goes past the end, second past "\n".
  for (string::iterator it = --(input2.end()); it != input2.begin(); --it) {
    out.put(*(it - 1));
  }
  cout << "out3: " << out.str() << endl;
  return;
}

int main(void) {
  char input[] = "hello, world!\n";

  char *output = (char *)malloc(strlen(input));
  strrev(output, input);
  cout << "output is '" << output << "'" << endl;
  free(output);

  char *out2 = new char[strlen(input) + 1];
  strrev2(out2, input);
  cout << "out2 '" << out2 << "'" << endl;
  delete out2;

  string input2(input);
  string output2(output);
  strrev3(output2, input2);

  exit(EXIT_SUCCESS);
}
