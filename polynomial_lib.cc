#include "polynomial.h"

#include <cassert>

using namespace std;
using namespace term;

namespace polynomial {

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
