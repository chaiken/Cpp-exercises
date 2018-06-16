#include <iostream>

using namespace std;

void *memcpy(void *s1, const void *s2, unsigned n) {
  char *from = s2, *to = s1;
}

int main(void) {
  char *two = "hello world\n";
  char *one = (char *)malloc(4096);
  memcpy(one, two);
  cout << one;
}
