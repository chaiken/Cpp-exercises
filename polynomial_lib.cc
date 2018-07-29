#include "polynomial.h"

#include <cassert>

using namespace std;

namespace polynomial {

// size is number of elements in coef[] and expon[].
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
  degree_ = h_->exponent;
}

void Polynomial::Release() {
  term *cursor, *t = h_;
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
  cout << endl;
  do {
    cursor->print();
    if (cursor->next != 0) {
      cout << " + ";
    }
  } while ((cursor = cursor->next) != 0);
  cout << endl;
}

void Polynomial::Reverse() {
  term *predecessor = 0, *element, *successor = h_;
  while (successor != 0) {
    element = successor;
    successor = successor->next;
    element->next = predecessor;
    predecessor = element;
  }
  h_ = element;
}
}
