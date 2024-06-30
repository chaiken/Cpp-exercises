#include <iostream>
#include <vector>

namespace {
constexpr int kMaxLen = 64;
}

class IntStack {
public:
  void reset() { s_.clear(); }
  int top_of() const { return s_.back(); }
  bool empty() const { return (s_.size() == 0); }
  bool full() const { return (s_.size() == kMaxLen); }
  void Push(int c);
  void PushMultiple(int m, const int arr[]);
  template <long unsigned int N> void PushArray(::std::array<int, N> array);
  int Pop();
  void PopMultiple(int m, int arr[]);
  int ReversePop();
  void Reverse();
  void Print() const;

private:
  ::std::vector<int> s_;
};

void IntStack::Push(int c) {
  if (full()) {
    ::std::cerr << "Stack overflow." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  s_.push_back(c);
}

int IntStack::Pop() {
  if (empty()) {
    ::std::cerr << "Stack underflow." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  int retval = s_.back();
  s_.pop_back();
  return retval;
}

void IntStack::PushMultiple(int m, const int arr[]) {
  int i = 0;
  while (i < m) {
    Push(arr[i++]);
  }
}

void IntStack::PopMultiple(int m, int arr[]) {
  int i = 0;
  while (i < m) {
    arr[i++] = Pop();
  }
}

template <long unsigned int N>
void IntStack::PushArray(::std::array<int, N> arrinst) {
  int i = 0;
  while (i < N) {
    Push(arrinst[i]);
    i++;
  }
}

int IntStack::ReversePop() {
  if (empty()) {
    ::std::cerr << "Stack underflow." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  // 'pop' the front of the vector.
  int retval = s_.front();
  s_.erase(s_.begin());
  return retval;
}

void IntStack::Reverse() {
  IntStack temp;
  temp.reset();
  while (!empty()) {
    temp.Push(Pop());
  }
  while (!temp.empty()) {
    Push(temp.ReversePop());
  }
}

// Prints from first-pushed end, not the end that would Pop().
void IntStack::Print() const {
  for (::std::vector<int>::const_iterator it = s_.cbegin(); it != s_.cend();
       it++) {
    ::std::cout << *it << " ";
  }
  ::std::cout << ::std::endl;
}
