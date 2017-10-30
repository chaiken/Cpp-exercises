#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

void strrev(char *s1, const char *s2) {
  int i = strlen(s2)-1, j=0;
  while (i-- > 0) {
    *(s1+j) = s2[i];
    j++;
  }
  s1[i] = '\0';
}

int main(void) {
  char input[] = "hello, world!\n";
  char *output = (char *)malloc(strlen(input));
  strrev(output, input);
  cout << "output is '" << output << "'" << endl;
  free(output);
  exit(EXIT_SUCCESS);
}
