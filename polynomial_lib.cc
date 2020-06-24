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
  h_ = new Term(tv.at(0).exponent, tv.at(0).coefficient);
  for (int i = 1; i < tv.size(); i++) {
    Term *t = new Term(tv.at(i).exponent, tv.at(i).coefficient);
    Prepend(t);
  }
  Reverse();
}

// Termlist constructor.
Polynomial::Polynomial(const Term &tlist) {
  log("Polynomial termlist constructor");
  // An earlier version included the next pointer as an argument to the Term
  // constructor.  That resulted in h_ have a non-NULL next pointer, which after
  // reversal meant that the original trailing Terms were now at the head of the
  // list.
  h_ = new Term(tlist.exponent, tlist.coefficient);
  assert(h_ != 0);

  Term *cursor = tlist.next;
  while (cursor != 0) {
    Term *t = new Term(move(*cursor));
    Prepend(t);
    cursor = cursor->next;
  }
  Reverse();
}

Polynomial &Polynomial::operator=(Polynomial &&p) {
  log("Polynomial move assignment operator");
  swap(h_, p.h_);
  return *this;
}

// Tasks: delete existing terms that are part of a polynomial as we link in new
// terms that replace them.  Failure to delete the existing terms will result in
// a memory leak.
// It's striking how much more complex the copy assignment is than the move
// assignment.
Polynomial &Polynomial::operator=(const Polynomial &p) {
  log("Polynomial copy assignment operator");
  if (h_) {
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
  while (cursor != nullptr) {
    // Doesn't leak memory since the dtor only uses head() to find the rest.
    Term *newterm = new Term(cursor->exponent, cursor->coefficient);
    Prepend(newterm);
    cursor = cursor->next;
  }
  Reverse();
  return *this;
}

// Remove any terms whose coefficient is zero.
// The resulting polynomial will be shorter or even empty.
void Polynomial::RemoveEmptyTerms() {
  if (empty()) {
    return;
  }
  std::unique_ptr<Term> cursor(h_);
  Term *previous = cursor.get();
  // Go past all empty terms at head().
  while (h_ && h_->empty()) {
    cursor.reset(previous->next);
    h_ = cursor.get();
    previous = h_;
  }
  // If all terms are empty, then set the polynomial empty.
  if (!h_ || !h_->next) {
    h_ = nullptr;
    return;
  }

  while (nullptr != cursor) {
    if (cursor->empty()) {
      // empty() means coefficient==0.0, not that exponent==0.0 or
      // next==nullptr.
      previous->next = cursor.get()->next;
      // Destroys the object currently managed by the unique_ptr (if any) and
      // takes ownership of parameter.
      cursor.reset(previous->next);
      previous = cursor.get();
    } else {
      // Simply advance forward.
      previous = cursor.release();
      cursor.reset(previous->next);
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
  Term *cursorA = a.h_;
  Term *cursorB = b.h_;
  int i = 0;
  vector<double> coeffs;
  vector<int> expon;

  while ((0 != cursorA) && (0 != cursorB)) {
    if (cursorA->exponent > cursorB->exponent) {
      Term *A_next = cursorA->next;
      coeffs.push_back(cursorA->coefficient);
      expon.push_back(cursorA->coefficient);
      cursorA = A_next;
    } else if (cursorB->exponent > cursorA->exponent) {
      Term *B_next = cursorB->next;
      coeffs.push_back(cursorB->coefficient);
      expon.push_back(cursorB->exponent);
      cursorB = B_next;
    } else {
      Term *A_next = cursorA->next;
      Term *B_next = cursorB->next;
      coeffs.push_back(cursorA->coefficient + cursorB->coefficient);
      expon.push_back(cursorA->exponent);
      cursorA = A_next;
      cursorB = B_next;
    }
    i++;
  }
  while (0 != cursorB) {
    Term *B_next = cursorB->next;
    coeffs.push_back(cursorB->coefficient);
    expon.push_back(cursorB->exponent);
    cursorB = B_next;
  }
  while (0 != cursorA) {
    Term *A_next = cursorA->next;
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

  Term *cursor = a.h_;
  if (0 == factor) {
    return Term(0, 0.0);
  }
  while (nullptr != cursor) {
    coeffs.push_back(factor * cursor->coefficient);
    exps.push_back(cursor->exponent);
    cursor = cursor->next;
  }
  TermVector tv(coeffs, exps);
  return tv;
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
  // https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/
  t = 0;
}

bool operator==(const Polynomial &a, const Polynomial &b) {
  // No proper XOR in C++.
  if (a.empty() && b.empty()) {
    return true;
  }
  if (a.empty() || b.empty()) {
    return false;
  }
#ifdef DEBUG
  cout << "Polynomial equality operator" << endl;
#endif
  // Need to work around the fact that head() returns const.
  if (*a.head() == *b.head()) {
    Term *ap = a.head()->next;
    Term *bp = b.head()->next;
    if ((!ap) && (!bp)) {
      return true;
    }
    while (*ap == *bp) {
      ap = ap->next;
      bp = bp->next;
      // Exhausted both lists without finding a mismatch.
      if ((ap == nullptr) && (bp == nullptr)) {
        return true;
      }
      if ((ap == nullptr) || (bp == nullptr)) {
        return false;
      }
    }
  }
  return false;
}

bool operator!=(const Polynomial &a, const Polynomial &b) {
#ifdef DEBUG
  cout << "Polynomial inequality operator." << endl;
#endif
  return !(a == b);
}

inline void Polynomial::Prepend(Term *t) {
  t->next = h_;
  h_ = t;
}

// Far simpler than what appears in the text on p. 176.
void Polynomial::Reverse() {
  Term *predecessor = 0, *element, *successor = h_;
  if (!h_) {
    return;
  }
  while (successor != 0) {
    element = successor;
    successor = successor->next;
    element->next = predecessor;
    predecessor = element;
  }
  h_ = element;
}

ostream &operator<<(ostream &out, const Polynomial &pn) {
  if (!pn.h_) {
    return out;
  }
  Term *cursor = pn.h_;
  do {
    out << *cursor;
    if (cursor->next != 0) {
      out << "+ ";
    }
  } while ((cursor = cursor->next) != 0);
  return out;
}

} // namespace polynomial
