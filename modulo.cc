#include <cstdlib>
#include <iostream>

using namespace std;

class mod_int {
public:
  mod_int();
  mod_int(int i);
  void assign(int i) { v_ = i % modulus_; }

  void Add(int i);
  void Print(void) { cout << v_ << endl; }
  const static int modulus_;

private:
  int v_;
};

const int mod_int::modulus_ = 60;
mod_int::mod_int() { v_ = 0; }
mod_int::mod_int(int i) { v_ = i % modulus_; }

void mod_int::Add(int i) { assign(v_ + i); }

int main(void) {
mod_int : {
  mod_int anint(140);
  anint.Print();
  anint.Add(-3);
  anint.Print();
}

  // We can reuse the name as long as the other snippet goes out of scope at the
  // close of a block.
  mod_int anint(67);
  anint.Print();

  exit(EXIT_SUCCESS);
}
