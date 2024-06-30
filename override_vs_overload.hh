#include <iostream>

namespace override_overload {
class B {
public:
  B(int j = 0) : i(j) {}
  // print() instead of operator<<() is lame, but making one of these functions
  // a (static) friend instead of a member would change the problem
  // significantly.
  virtual void print() const { std::cout << " i = " << i << std::endl; }
  void print(std::string s) const { std::cout << s << i << std::endl; }

private:
  int i;
};

class D : public B {
public:
  D(int j = 0) : B(5), i(j) {}
  // override keyword works since the function is virtual in B.
  void print() const override { std::cout << " i = " << i << std::endl; }
  // Not effectively an overload, since only changes to the parameter list of
  // the function matter to the compiler.  Nothing in main() checks the return
  // value anyway.
  int print(std::string s) const {
    std::cout << s << i << std::endl;
    return i;
  }

private:
  int i;
};

} // namespace override_overload
