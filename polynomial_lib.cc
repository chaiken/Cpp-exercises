#include "polynomial.h"
#include "term_vector.h"

#include <cassert>

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
  log("Polynomial assignment operator");
  swap(h_, p.h_);
  return *this;
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

  cout << "Entering operator+() loop" << endl;
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
  return tv;
}

void Polynomial::Release() {
  Term *cursor, *t = h_;
  if (!h_) {
    return;
  }
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
