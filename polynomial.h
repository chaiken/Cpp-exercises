#include <array>
#include <iostream>

namespace polynomial {

// Numerically sort array1, and reorder array2 the same way.
template <long unsigned int N>
void SyncSortTwoArrays(::std::array<int, N> *arr1,
                       ::std::array<double, N> *arr2, int index);

struct term {
  term(int e, double c, term *n = 0) : exponent(e), coefficient(c), next(n) {}
  void print() { std::cout << coefficient << "x^" << exponent << " "; }
  int exponent;
  double coefficient;
  term *next;
};

class Polynomial {
public:
  // Polynomial() : h_(0), degree_(0) {}
  Polynomial() : h_(0) {}
  template <long unsigned int N>
  Polynomial(::std::array<double, N> coef, ::std::array<int, N> expon);
  // Copy constructor.
  Polynomial(const Polynomial &p);
  // Summing constructor.
  Polynomial(const Polynomial &a, const Polynomial &b);
  ~Polynomial() {
    if (0 != h_)
      Release();
  }
  const term &Head() const { return *h_; }
  void Print() const;
  void Reverse();

private:
  term *h_;
  //  int degree_;
  void Prepend(term *t);
  void Release();
  void RestOf(term *rest);
};

} // namespace polynomial

#include "polynomial_impl.h"
