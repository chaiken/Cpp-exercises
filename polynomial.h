#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "term.h"
#include "term_vector.h"

#include <array>
#include <iostream>

namespace polynomial {

class Polynomial {
public:
  // Polynomial() : h_(0), degree_(0) {}
  Polynomial() : h_(0) { log("default constructor"); }
  template <long unsigned int N>
  Polynomial(::std::array<double, N> coef, ::std::array<int, N> expon);
  // List constructor
  Polynomial(const term::Term &termlist);
  // TermVector constructor.
  Polynomial(const termvector::TermVector &tv);
  // Move constructor.
  Polynomial(Polynomial &&p) : Polynomial() {
    ::std::cout << "term polynomial move constructor" << ::std::endl;
    *this = ::std::move(p);
  }
  ~Polynomial() {
    if (0 != h_)
      Release();
  }
  const term::Term *head() const { return h_; }
  void Reverse();
  Polynomial &operator=(Polynomial &&p);
  friend ::std::ostream &operator<<(::std::ostream &out, const Polynomial &pn);
  friend Polynomial operator+(const Polynomial &a, const Polynomial &b);

private:
  term::Term *h_;
  //  int degree_;
  void Prepend(term::Term *t);
  void Release();
  // from
  // https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/
  void log(const char *msg) {
    //    ::std::cout << "[" << *this << "] " << msg << "\n";
    ::std::cout << msg << "\n";
  }
};

::std::ostream &operator<<(::std::ostream &out, const Polynomial &pn);
} // namespace polynomial

#include "polynomial_impl.h"

#endif
