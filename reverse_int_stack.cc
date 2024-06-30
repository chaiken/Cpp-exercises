#include <array>
#include <cstdlib>
#include <cstring>

#include "int_stack.hh"

using namespace std;

void print_int_array(const int arr[], const int arrlen) {
  char c;
  int i = 0;
  while (i < arrlen) {
    c = static_cast<char>(arr[i]) + '0';
    cout << c << " ";
    i++;
  }
  cout << endl;
}

void reverse(IntStack *stacka) {
  IntStack stackb;
  stackb.reset();
  while (!stacka->empty()) {
    stackb.Push(stacka->Pop());
  }
  while (!stackb.empty()) {
    stacka->Push(stackb.ReversePop());
  }
}

int main(void) {
  const int input[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  const array<int, 15> other = {
      3, 1, 4, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3,
  };
  int copy[kMaxLen];

  // Push and pop specified ranges of array elements.
  IntStack tester;
  tester.reset();
  tester.PushMultiple(6, input);
  cout << "Pushed: ";
  tester.Print();
  tester.PopMultiple(6, copy);
  cout << "Popped: ";
  print_int_array(copy, 6);

  // Reverse() as a method.
  tester.reset();
  cout << "Input: ";
  int i = 0;
  while (i < 10) {
    tester.Push(input[i++]);
  }
  tester.Print();
  cout << "Reversed, unpopped stack: ";
  tester.Reverse();
  tester.Print();

  // reverse() as a non-member function.
  tester.PushArray(other);
  cout << "Printing pushed array: ";
  tester.Print();
  reverse(&tester);
  cout << "Reversal of stack by non-member function: ";
  tester.Print();
}
