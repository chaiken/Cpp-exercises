#include <array>
#include <iostream>

using namespace std;

class A {
public:
  A() : xx_(0) { cout << "A() called. "; }
  A(int n) : xx_(n) { cout << "A(int) " << n << " called. "; }
  // Conversion constructor.
  A(double y) : xx_(y + 0.5) { cout << "A(double) " << y << " called. "; }
  void assign(int n) { xx_ = n; }
  void assign(double y) { xx_ = y + 0.5; }
  void print(void) const { cout << xx_ << endl; }
  ~A() { cout << "~A() with A::xx_ " << xx_ << " called.\n"; }

private:
  int xx_;
};

int main(void) {
  double y = 17.3;
  int x = 14;

  cout << "Instantiating objects: " << endl;
  A z(11), zz(11.5), zzz(0);
  cout << "\nObject allocation layout:";
  cout << "\n double y is at " << &y << " sizeof(double) is " << sizeof(double);
  cout << "\n int x is at " << &x << " sizeof(int) is " << sizeof(int);
  cout << "\n A:z is at " << &z;
  cout << "\n A:zz is at " << &zz;
  cout << "\n A:zzz is at " << &zzz;
  cout << "\n-------------------------\n" << endl;
  zzz = A(x);
  // Destructor for original zzz is called before constructor of new one.
  zzz = A(y);
  // Pointers to member functions are not taken into account, as they would be
  // with a struct.
  cout << "sizeof(A) " << sizeof(A) << endl;

  const array<A, 5> d = {0, 1, 2, 3, 4};
  int i = 0;
  cout << "\n5-element array, backwards:" << endl;
  for (auto rit = d.crbegin(); rit < d.crend(); ++rit) {
    (*rit).print();
  }
  cout << "exit main\n" << endl;
}
