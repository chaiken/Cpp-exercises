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
  Polynomial() : h_(0) { log("Polynomial default constructor"); }
  template <long unsigned int N>
  Polynomial(::std::array<double, N> coef, ::std::array<int, N> expon);
  // List constructor
  Polynomial(const term::Term &termlist);
  // TermVector constructor.
  Polynomial(const termvector::TermVector &tv);
  // Move constructor.
  Polynomial(Polynomial &&p) : Polynomial() {
#ifdef DEBUG
    ::std::cout << "term polynomial move constructor" << ::std::endl;
#endif
    *this = ::std::move(p);
  }
  ~Polynomial() {
    if (0 != h_)
      Release();
  }
  const term::Term *head() const { return h_; }
  // If the head has a next pointer but a zero coefficient, it should be deleted
  // and replaced.
  bool empty() const {
    return ((nullptr == h_) || ((h_->empty()) && (nullptr == h_->next)));
  }
  void RemoveEmptyTerms();
  void Reverse();
  Polynomial &operator=(Polynomial &&p);
  Polynomial &operator=(const Polynomial &p);
  friend ::std::ostream &operator<<(::std::ostream &out, const Polynomial &pn);
  friend Polynomial operator+(const Polynomial &a, const Polynomial &b);
  friend Polynomial operator-(const Polynomial &a, const Polynomial &b);
  friend Polynomial operator*(const int factor, const Polynomial &a);
  friend Polynomial operator*(const Polynomial &a, const int factor);
  friend bool operator!=(const Polynomial &a, const Polynomial &b);
  friend bool operator==(const Polynomial &a, const Polynomial &b);

private:
  term::Term *h_;
  //  int degree_;
  void Prepend(term::Term *t);
  void Release();
  // idea for function from
  // https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/
  // unused attribute from
  // https://stackoverflow.com/questions/31909631/c11-style-unused-attribute-in-gcc/31909713#31909713
  void log(const char *msg [[gnu::unused]]) {
#ifdef DEBUG
    //    ::std::cout << "[" << *this << "] " << msg << "\n";
    ::std::cout << msg << "\n";
#endif
  }
};

Polynomial operator+(const Polynomial &a, const Polynomial &b);
Polynomial operator-(const Polynomial &a, const Polynomial &b);
Polynomial operator*(const Polynomial &a, const int factor);
Polynomial operator*(const int factor, const Polynomial &a);
::std::ostream &operator<<(::std::ostream &out, const Polynomial &pn);
bool operator!=(const Polynomial &a, const Polynomial &b);
bool operator==(const Polynomial &a, const Polynomial &b);
} // namespace polynomial

#include "polynomial_impl.h"

#endif
