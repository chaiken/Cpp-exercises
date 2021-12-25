#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "term.h"
#include "term_vector.h"

// FRIEND_TEST declaration below does not work.
// #include "gtest/gtest_prod.h"

#include <array>
#include <iostream>

namespace polynomial {

class Polynomial {
public:
  // With term::term_deleter instead of term::Term::term_deleter, the compiler
  // complains that term_deleter is in an anonymous namespace.   That's not
  // allowed since the anonymous namespace must be unique to each file.
  // https://stackoverflow.com/questions/154469/unnamed-anonymous-namespaces-vs-static-functions
  Polynomial() : h_(std::unique_ptr<term::Term>()) {
    log("Polynomial default constructor");
  }
  /*  template <long unsigned int N>
      Polynomial(std::array<double, N> coef, std::array<int, N> expon); */
  template <long unsigned int N>
  Polynomial(std::array<double, N> coef, std::array<int, N> expon);
  // List constructor
  Polynomial(const term::Term &termlist);
  // TermVector constructor.
  Polynomial(const termvector::TermVector &tv);
  // Move constructor.  Could be default except for DEBUG output.
  Polynomial(Polynomial &&p) noexcept : Polynomial() {
#ifdef DEBUG
    ::std::cout << "term polynomial move constructor" << ::std::endl;
#endif
    *this = std::move(p);
    p.h_.reset();
  }
  Polynomial(const Polynomial &p) = delete;
  ~Polynomial() {
    if (h_) {
#ifdef DEBUG
      std::cout << "Deleting polynomial head" << std::endl;
#endif
      h_.reset();
    }
  }
  const term::Term *head() const { return h_.get(); }
  // A Term is empty if the coefficient is zero.  An empty Polynomial must have
  // an empty head as well as a null next pointer.
  bool empty() const { return (!h_ || ((h_->empty()) && (!h_->next))); }
  void RemoveEmptyTerms();
  void Reverse();
  Polynomial &operator=(Polynomial &&p) = default;
  Polynomial &operator=(const Polynomial &p) = delete;
  friend ::std::ostream &operator<<(::std::ostream &out, const Polynomial &pn);
  friend Polynomial operator+(const Polynomial &a, const Polynomial &b);
  friend Polynomial operator-(const Polynomial &a, const Polynomial &b);
  friend Polynomial operator*(const int factor, const Polynomial &a);
  friend Polynomial operator*(const Polynomial &a, const int factor);
  friend bool operator!=(const Polynomial &a, const Polynomial &b);
  friend bool operator==(const Polynomial &a, const Polynomial &b);

private:
  void Prepend(std::unique_ptr<term::Term> t);
  std::unique_ptr<term::Term> h_;
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
  // Does not work.
  // FRIEND_TEST(PolynomialSimpleTest, PrependTest);
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
