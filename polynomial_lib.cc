#include "polynomial.h"
#include "term_vector.h"

#include <cassert>

#include <memory>

using namespace std;
using namespace term;
using namespace termvector;

namespace polynomial {

Polynomial::Polynomial(const TermVector &tv) {
  log("Polynomial TermVector constructor");
  h_ = std::unique_ptr<Term>(new Term(tv.at(0).exponent, tv.at(0).coefficient));
  for (int i = 1; i < tv.size(); i++) {
    // Separately creating a temporary unique_ptr and then Prepend()-ing
    // requires deleted unique_ptr copy ctor.
    Prepend(std::unique_ptr<Term>(
        new Term(tv.at(i).exponent, tv.at(i).coefficient)));
  }
  Reverse();
}

// Termlist constructor.
Polynomial::Polynomial(const Term &tlist) {
  log("Polynomial termlist constructor");
  h_ = std::unique_ptr<Term>(new Term(tlist.exponent, tlist.coefficient));
  Term *cursor = tlist.next.get();
  while (cursor) {
    // If next is not explicitly set to NULL, it contains garbage, resulting in
    // an infinite loop.
    Term *next = nullptr;
    if (cursor->next) {
      next = cursor->next.get();
    }
    Prepend(
        std::unique_ptr<Term>(new Term(cursor->exponent, cursor->coefficient)));
    cursor = next;
  }
  Reverse();
}

/* Polynomial &Polynomial::operator=(Polynomial &&p) {
  log("Polynomial move assignment operator");
  h_ = std::make_unique<Term>(p.h_.release());
  return *this;
} */

// Tasks: delete existing terms that are part of a polynomial as we link in new
// terms that replace them.  Failure to delete the existing terms will result in
// a memory leak.
// It's striking how much more complex the copy assignment is than the move
// assignment.
/* Polynomial &Polynomial::operator=(const Polynomial &p) {
  log("Polynomial copy assignment operator");
  if ((h_) && (!h_->empty())) {
    // Delete everything but head.
    Term *tp = h_->next;
    while (tp != nullptr) {
      Term *cursor = tp->next;
      delete tp;
      tp = cursor;
    }
    // Modify the contents of head() while preserving the pointer itself.
    h_->coefficient = p.h_->coefficient;
    h_->exponent = p.h_->exponent;
    // Needed for a single Term; if there are multiple terms, Prepend() will add
    // them correctly.
    h_->next = nullptr;
  } else {
    h_ = new Term(p.h_->exponent, p.h_->coefficient);
  }
  Term *cursor = p.h_->next;
  while (cursor != nullptr) {    // Doesn't leak memory since the dtor only uses
head() to find the rest. Term *newterm = new Term(cursor->exponent,
cursor->coefficient); Prepend(newterm); cursor = cursor->next;
  }
  Reverse();
  return *this;
} */

// Remove any terms whose coefficient is zero.
// The resulting polynomial will be shorter or even empty.
void Polynomial::RemoveEmptyTerms() {
  if (empty()) {
    return;
  }
  // In order to walk two pointers through a list of unique_ptr's, one must
  // point to a raw pointer in order to avoid copying.
  Term *previous = h_.get();
  // Remove all terms at head() with 0 coefficients.
  while (h_ && h_->empty()) {
    h_.reset(previous->next.release());
    previous = h_.get();
  }
  //  The whole polynomial consisted of empty terms.
  if (!h_) {
    return;
  }

  // Empty terms have been removed at the list head only.
  // previous->next and cursor are the same.
  Term *cursor = h_->next.get();
  while (cursor) {
    if (cursor->empty()) {
      previous->next.swap(cursor->next);
      cursor->next.reset();
      cursor = previous->next.get();
      previous = cursor;
    } else {
      // Simply advance forward.
      previous = cursor;
      cursor = previous->next.get();
    }
  }
}

// A binary operator for an object must itself return by value a different kind
// of object that is constructible into the object that the operator returns.
// https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/
// "the binary addition operator '+' takes two rvalues as arguments and returns
// an rvalue" That's why a binary operator must not allocate memory, as there's
// no way to free it via the returned rvalue. Implicitly calls TermVector
// constructor.
Polynomial operator+(const Polynomial &a, const Polynomial &b) {
  Term *cursorA = a.h_.get();
  Term *cursorB = b.h_.get();
  int i = 0;
  vector<double> coeffs;
  vector<int> expon;

  while ((cursorA) && (cursorB)) {
    if (cursorA->exponent > cursorB->exponent) {
      Term *A_next = cursorA->next.get();
      coeffs.push_back(cursorA->coefficient);
      expon.push_back(cursorA->coefficient);
      cursorA = A_next;
    } else if (cursorB->exponent > cursorA->exponent) {
      Term *B_next = cursorB->next.get();
      coeffs.push_back(cursorB->coefficient);
      expon.push_back(cursorB->exponent);
      cursorB = B_next;
    } else {
      Term *A_next = cursorA->next.get();
      Term *B_next = cursorB->next.get();
      coeffs.push_back(cursorA->coefficient + cursorB->coefficient);
      expon.push_back(cursorA->exponent);
      cursorA = A_next;
      cursorB = B_next;
    }
    i++;
  }
  while (0 != cursorB) {
    Term *B_next = cursorB->next.get();
    coeffs.push_back(cursorB->coefficient);
    expon.push_back(cursorB->exponent);
    cursorB = B_next;
  }
  while (0 != cursorA) {
    Term *A_next = cursorA->next.get();
    coeffs.push_back(cursorA->coefficient);
    expon.push_back(cursorA->coefficient);
    cursorA = A_next;
  }
  // Implicitly call TermVector constructor.
  TermVector tv(coeffs, expon);
  Polynomial temp(tv);
  temp.RemoveEmptyTerms();
  return (temp);
}

Polynomial operator*(const int factor, const Polynomial &a) {
  std::vector<int> exps;
  std::vector<double> coeffs;

  // Must the existing Polynomial be destroyed?
  if (0 == factor) {
    // Implicitly call TermVector ctor.
    return {Term(0, 0.0)};
  }
  Term *cursor = a.h_.get();
  while (cursor) {
    coeffs.push_back(factor * cursor->coefficient);
    exps.push_back(cursor->exponent);
    cursor = cursor->next.get();
  }
  return TermVector(coeffs, exps);
}

// Order of operands matters.  Without this, (-1 * b) compiles but (b * -1) does
// not.
Polynomial operator*(const Polynomial &a, const int factor) {
  return (factor * a);
}

Polynomial operator-(const Polynomial &a, const Polynomial &b) {
  Polynomial temp(a + (-1 * b));
  temp.RemoveEmptyTerms();
  return (temp);
}

/* No longer needed with std::unique_ptr implementation.
void Polynomial::Release() {
  Term *cursor, *t = h_;
  // A test for (nullptr==h_) appears unreachable.
  while (t->next != 0) {
    cursor = t->next;
    delete t;
    t = cursor;
  }
  delete t;
  // Following
  //
https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/
  t = 0;
} */

bool operator==(const Polynomial &a, const Polynomial &b) {
  // No proper XOR in C++.
  if (a.empty() && b.empty()) {
    return true;
  }
  if (a.empty() || b.empty()) {
    return false;
  }
  if (*(a.h_.get()) == *(b.h_.get())) {
    Term *ap = a.h_->next.get();
    Term *bp = b.h_->next.get();
    if ((!ap) && (!bp)) {
      return true;
    }
    while (*ap == *bp) {
      ap = ap->next.get();
      bp = bp->next.get();
      // Exhausted both lists without finding a mismatch.
      if ((!ap) && (!bp)) {
        return true;
      }
    }
  }
  return false;
}

bool operator!=(const Polynomial &a, const Polynomial &b) { return !(a == b); }

void Polynomial::Prepend(std::unique_ptr<Term> t) {
  t->next = std::move(h_);
  h_ = std::move(t);
}

// Far simpler than what appears in the text on p. 176.
void Polynomial::Reverse() {
  if (!h_ || !h_->next) {
    return;
  }
  std::unique_ptr<Term> successor(h_->next.release());
  while (successor) {
    std::unique_ptr<Term> successor_next;
    if (successor->next) {
      successor_next = std::move(successor->next);
    }
    Prepend(std::move(successor));
    successor = std::move(successor_next);
  }
}

ostream &operator<<(ostream &out, const Polynomial &pn) {
  if (!pn.h_) {
    return out;
  }
  Term *cursor = pn.h_.get();
  while (cursor) {
    out << *cursor;
    if (cursor->next.get()) {
      out << "+ ";
    }
    cursor = cursor->next.get();
  }
  return out;
}

} // namespace polynomial
