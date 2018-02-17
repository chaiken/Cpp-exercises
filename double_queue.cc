#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

namespace {
constexpr int kMaxLen = 65;
constexpr int kCenter = (kMaxLen / 2);
}

class DoubleQueue {
public:
  void PushTop(const char c), PushBottom(const char c);
  char PopTop(void), PopBottom(void);
  const char top_of(void) const { return s_[top_]; }
  const char bottom_of(void) const { return s_[bottom_]; };
  bool empty(void) const { return (top_ < bottom_); };
  bool full(void) const { return ((top_ >= (kMaxLen - 1)) || (bottom_ <= 0)); };
  // Because top_ starts below bottom_, it will increment before assignment
  // and include (kCenter+1) (if kMaxLen is even).  bottom_ will decrement
  // before assignment and thereby start at kCenter.
  void reset() {
    top_ = bottom_ = kMaxLen / 2;
    top_--;
  }
  void OutStack(bool forward) const;
  bool Recenter(void);

private:
  int top_, bottom_;
  char s_[kMaxLen];
};

// Sum of strlens is < kMaxLen.
void SwapShort(char s1[], char s2[]) {
  DoubleQueue queue;
  int len1 = strlen(s1), len2 = strlen(s2);
  if ((len1 + len2) > kMaxLen) {
    cerr << "Strings are too long to swap in a single queue." << endl;
    exit(1);
  }
  if ((len1 * len2) == 0) {
    cerr << "Zero-length string." << endl;
  }
  int i;
  queue.reset();

  for (i = 0; i < len1; i++) {
    queue.PushTop(s1[i]);
  }
  for (i = 0; i < len2; i++) {
    queue.PushBottom(s2[i]);
  }
  for (i = 0; i < len1; i++) {
    s2[(len1 - 1) - i] = queue.PopTop();
  }
  for (i = 0; i < len2; i++) {
    s1[(len2 - 1) - i] = queue.PopBottom();
  }
  s2[len1] = '\0';
  s1[len2] = '\0';
}

bool DoubleQueue::Recenter() {
  int i;
  if (empty()) {
    return true;
  }
  // Top runs kCenter to (kMaxLen-1); bottom runs 0 to (kCenter-1).
  // Top is empty when (top_ == kCenter-1).
  // Bottom is empty when (bottom_ == kCenter).
  int shift = ((kCenter - bottom_) - ((top_ - kCenter) + 1)) / 2;
  // Out of space.  Last element is (kMaxLen-1).
  if (((shift + top_) > (kMaxLen - 1)) || ((shift + bottom_) < 0)) {
    cerr << "Cannot recenter: shift is " << shift << "." << endl;
    return false;
  }
  // Already centered, so we've failed.
  if ((shift < 2) && (shift > -2)) {
    cerr << "Already centered." << endl;
    return false;
  }
  // Shift up from the top, and down from the bottom, to avoid overwriting.
  if (shift > 0) {
    cout << "Shifting up by " << shift << endl;
    for (i = top_; i >= bottom_; i--) {
      s_[i + shift] = s_[i];
    }
  } else { // Already returned for |shift| < 2.
    cout << "Shifting down by " << shift << endl;
    for (i = bottom_; i <= top_; i++) {
      s_[i + shift] = s_[i];
    }
  }
  top_ += shift;
  bottom_ += shift;
  return true;
}

void DoubleQueue::PushTop(const char c) {
  if (full()) {
    if (!Recenter()) {
      cerr << "Queue is full." << endl;
      exit(EXIT_FAILURE);
    }
  }
  s_[++top_] = c;
}

void DoubleQueue::PushBottom(const char c) {
  if (full()) {
    if (!Recenter()) {
      cerr << "Queue is full." << endl;
      exit(EXIT_FAILURE);
    }
  }
  s_[--bottom_] = c;
}

char DoubleQueue::PopTop(void) {
  if (empty()) {
    cerr << "Queue is empty." << endl;
    exit(EXIT_FAILURE);
  }
  return (s_[top_--]);
}

char DoubleQueue::PopBottom(void) {
  if (empty()) {
    cerr << "Queue is empty." << endl;
    exit(EXIT_FAILURE);
  }
  return (s_[bottom_++]);
}

void DoubleQueue::OutStack(bool forward) const {
  int i = bottom_;
  if (forward) {
    while (i <= top_) {
      cout << s_[i++];
    }
  } else {
    while (i >= bottom_) {
      cout << s_[i--];
    }
  }

  cout << '\0' << endl;
}

void BothWays(const char input[]) {
  DoubleQueue new_queue;

  int i = 0;
  new_queue.reset();
  while (input[i] != '\0') {
    new_queue.PushTop(input[i]);
    i++;
  }
  new_queue.OutStack(true);

  i = 0;
  new_queue.reset();
  while (input[i] != '\0') {
    new_queue.PushBottom(input[i]);
    i++;
  }
  new_queue.OutStack(true);
}

int main(void) {
  char input[] = "Hello world\n";
  BothWays(input);

  char input2[] = "Apt. 41 1555 W. Middlefield Road Mountain View CA 94043\n";
  BothWays(input2);

  char input3[] = "We the People of the United States, in order to form "
                  "a more perfect Union, establish Justice, insure "
                  "domestic Tranquility, provide for the common "
                  "defence\n";
  // BothWays(input3);

  // Can't declare array size and initialize in one statement.
  char input4[32], input5[32];
  strcpy(input4, "This is NOT a test.\n");
  strcpy(input5, "This is still NOT a test.\n");

  cout << "start:" << endl;
  cout << input4 << input5 << endl;
  SwapShort(input4, input5);
  cout << "finish:" << endl;
  cout << input4 << input5 << endl;

  cout << "start:" << endl;
  cout << input4 << input2 << endl;
  strcpy(input4, input2);
  SwapShort(input4, input5);
  cout << "finish:" << endl;
  cout << input4 << input2 << endl;

  exit(EXIT_SUCCESS);
}
