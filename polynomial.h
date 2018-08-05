#include <iostream>

namespace polynomial {

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
  Polynomial(int size, double coef[], int expon[]);
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
  void AddTerm(term *&a, term *&b);
  void Prepend(term *t);
  void Release();
  void RestOf(term *rest);
};
}
