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
  Polynomial() : h_(0), degree_(0) {}
  Polynomial(const Polynomial &p);
  Polynomial(int size, double coef[], int expon[]);
  ~Polynomial() { Release(); }
  const term &Head() { return *h_; }
  void Print() const;
  void Reverse();
  void Plus(Polynomial a, Polynomial b);

private:
  term *h_;
  int degree_;

  void Prepend(term *t);
  void AddTerm(term *&a, term *&b);
  void Release();
  void RestOf(term *rest);
};
}
