#include "reverse_char_stack_lib.h"

using namespace std;

int main(void) {
  const char input[] = "Not unreasonably long.";
  const char other[] =
      "Ridiculously, absurdly, unquestionably, indubitably too long";
  char copy1[64], copy2[64];

  charstack::CharStack tester;
  tester.PushMultiple(6u, input);
  cout << "Pushed: ";
  tester.Print();
  tester.PopMultiple(6u, copy1);
  cout << "Popped: " << copy1 << endl;
  tester.PushMultiple(strlen(input), input);
  tester.PopMultiple(strlen(input), copy2);
  cout << endl;

  cout << "Reverse() as a method:" << endl;
  charstack::CharStack new_stack;
  new_stack.reset();
  unsigned i = 0u;
  while (i < strlen(input)) {
    new_stack.Push(input[i++]);
  }
  new_stack.Push('\0');
  cout << "Input: ";
  new_stack.Print();
  new_stack.Reverse();
  cout << "Output: ";
  new_stack.Print();
  cout << endl;

  cout << "reverse() as a non-member function:" << endl;
  char output[strlen(input)];
  cout << "input: " << input << endl;
  charstack::reverse(input, output);
  cout << "output: " << output << endl;

  exit(EXIT_SUCCESS);
}
