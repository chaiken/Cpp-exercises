#include <cassert>
#include <cstdlib>
#include <cstring>

#include "ch_stac1.h"

using namespace std;

void reverse(const char s1[], char s2[]) {
  CharStack ch_stack;
  ch_stack.reset();

  int i = 0, j = 0;
  while (s1[i] != '\0') {
    ch_stack.Push(s1[i]);
    i++;
  }
  while (i--) {
    s2[j++] = ch_stack.Pop();
  }
  // Don't forget terminal NULL for second string.
  s2[j] = '\0';
  // Check that second string's termination is correct.
  assert(strlen(s1) == strlen(s2));
}


int main(void) {
  const char input[] = "Not unreasonably long.";
  const char other[] =
      "Ridiculously, absurdly, unquestionably, indubitably too long";
  char copy1[64], copy2[64];

  CharStack tester;
  tester.PushMultiple(6u, input);
  tester.PopMultiple(6u, copy1);
  cout << "Popped: " << copy1 << endl;
  tester.PushMultiple(strlen(input), input);
  tester.PopMultiple(strlen(input), copy2);

  // Reverse() as a method.
  cout << "Input: " << input << endl;
  CharStack new_stack;
  new_stack.reset();
  unsigned i = 0u;
  while (i < strlen(input)) {
    new_stack.Push(input[i++]);
  }
  new_stack.Print();
  cout << "Output: " << input << endl;
  new_stack.Reverse();
  new_stack.Print();

  // reverse() as a non-member function.
  char output[strlen(input)];
  cout << "input: " << input << endl;
  reverse(input, output);
  cout << "output: " << output << endl;

  char output2[strlen(other)];
  cout << "input: " << other << endl;
  reverse(other, output2);
  cout << "output: " << output2 << endl;

  cout << endl;

  exit(EXIT_SUCCESS);
}
