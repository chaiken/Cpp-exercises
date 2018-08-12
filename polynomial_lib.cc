#include "polynomial.h"

#include <cassert>

using namespace std;

namespace polynomial {

namespace {

// Add two Polynomial terms.  Since the terms are passed in and returned as
// pointers, the function need not be a class member.
term *AddTerms(term **termAp, term **termBp) {
  term *t;
  // If the one of the terms passed in is NULL, return a copy of the one that is
  // not.
  if ((0 == *termBp) || ((*termAp)->exponent > (*termBp)->exponent)) {
    t = new term((*termAp)->exponent, (*termAp)->coefficient);
    assert(t != 0);
    (*termAp) = (*termAp)->next;
  } else if ((0 == *termAp) || ((*termAp)->exponent < (*termBp)->exponent)) {
    t = new term((*termBp)->exponent, (*termBp)->coefficient);
    assert(t != 0);
    (*termBp) = (*termBp)->next;
  } else {
    // Exponents are equal.
    t = new term((*termAp)->exponent,
                 (*termAp)->coefficient + (*termBp)->coefficient);
    assert(t != 0);
    (*termAp) = (*termAp)->next;
    (*termBp) = (*termBp)->next;
  }
  return t;
}
}

// size is number of elements in coef[] and expon[].
// degree_ is never used for anything, so ditch it.
Polynomial::Polynomial(int size, double coef[], int expon[]) {
  // Head of list.
  term *temp = new term(expon[0], coef[0]);
  assert(temp != 0);
  h_ = 0;
  Prepend(temp);

  // Other nodes.
  for (int i = 1; i < size; i++) {
    assert(expon[i - 1] < expon[i]);
    term *temp = new term(expon[i], coef[i]);
    assert(temp != 0);
    Prepend(temp);
  }
  // degree_ may have arbitary relationship to size if polynomial is sparse.
  // degree_ = h_->exponent;
}

Polynomial::Polynomial(const Polynomial &p) {
  // degree_ = p.degree_;
  h_ = new term(p.h_->exponent, p.h_->coefficient);
  assert(h_ != 0);

  term *cursor = p.h_->next;
  term *t;
  while (cursor != 0) {
    t = new term(cursor->exponent, cursor->coefficient);
    assert(t != 0);
    Prepend(t);
    cursor = cursor->next;
  }
  Reverse();
}

// When adding two Polynomials, if one has terms of lower degree than the other,
// just Prepend() all of them.
void Polynomial::RestOf(term *t) {
  while (t != 0) {
    term *copy = new term(t->exponent, t->coefficient);
    assert(copy != 0);
    Prepend(copy);
    t = t->next;
  }
}

// Instead of the stupid Plus() function.
// FIX ME: going to bomb if one of the arguments is NULL.
Polynomial::Polynomial(const Polynomial &a, const Polynomial &b) {
  term *cursorA = a.h_;
  term *cursorB = b.h_;

  h_ = AddTerms(&cursorA, &cursorB);

  while ((cursorA != 0) && (cursorB != 0)) {
    term *t = AddTerms(&cursorA, &cursorB);
    Prepend(t);
  }

  if ((0 == cursorA) && (0 != cursorB)) {
    RestOf(cursorB);
  }
  if ((0 == cursorB) && (0 != cursorA)) {
    RestOf(cursorA);
  }
  Reverse();
}

void Polynomial::Release() {
  term *cursor, *t = h_;
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

inline void Polynomial::Prepend(term *t) {
  t->next = h_;
  h_ = t;
}

void Polynomial::Print() const {
  term *cursor = h_;
  if (!h_) {
    return;
  }
  cout << endl;
  do {
    cursor->print();
    if (cursor->next != 0) {
      cout << " + ";
    }
  } while ((cursor = cursor->next) != 0);
  cout << endl;
}

// Far simpler than what appears in the text on p. 176.
void Polynomial::Reverse() {
  term *predecessor = 0, *element, *successor = h_;
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
}
