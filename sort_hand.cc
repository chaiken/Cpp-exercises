// Sort a hand of cards by deck-card number that is acquired by dealing from a
// deck.  Make the hands quasi-random by setting a seed derived on the mS field
// of time since the Epoch.
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <sys/timeb.h>

using namespace std;

namespace {
constexpr int kCardsPerHand = 5;
constexpr int kPlayers = 5;
}

enum suit { clubs, diamonds, hearts, spades };

class Pip {
public:
  void assign(int n) { p_ = n % 13 + 1; }
  int getpip() const { return p_; }
  void PrintPipName() const;

private:
  int p_;
};

void Pip::PrintPipName() const {
  switch (getpip()) {
  case 1:
    cout << "A";
    return;
  case 11:
    cout << "J";
    return;
  case 12:
    cout << "Q";
    return;
  case 13:
    cout << "K";
    return;
  }
  if (getpip() <= 10) {
    cout << getpip();
    return;
  } else {
    cerr << "Unknown pip " << getpip();
    exit(EINVAL);
  }
}

class Card {
public:
  suit s;
  Pip p;
  void assign(int n) {
    card_ = n;
    s = static_cast<suit>(n / 13);
    p.assign(n);
  }
  void PrintSuitName();
  void copy_card(Card *newcard) { newcard->assign(card_); };
  void pr_card() {
    p.PrintPipName();
    PrintSuitName();
  };

private:
  int card_;
};

void Card::PrintSuitName() {
  switch (s) {
  case 0:
    cout << "C" << endl;
    break;
  case 1:
    cout << "D" << endl;
    break;
  case 2:
    cout << "H" << endl;
    break;
  case 3:
    cout << "S" << endl;
    break;
  default:
    cerr << "Illegal suit: " << s << endl;
    exit(EINVAL);
  }
}

class Deck {
public:
  void InitDeck();
  void Shuffle();
  void Deal(int, int, Card *);
  void PrintDeck();

private:
  Card d_[52];
};

void Deck::InitDeck() {
  for (int i = 0; i < 52; ++i) {
    d_[i].assign(i);
  }
}

void Deck::Shuffle() {
  for (int i = 0; i < 52; ++i) {
    int k = i + (rand() % (52 - i));
    Card temp = d_[i];
    d_[i] = d_[k];
    d_[k] = temp;
  }
}

void Deck::Deal(int n, int pos, Card *hand) {
  for (int i = pos; i < pos + n; ++i) {
    hand[i - pos] = d_[i];
  }
}

void Deck::PrintDeck() {
  for (int i = 0; i < 52; i++) {
    d_[i].pr_card();
  }
}

int CompareCards(const void *a, const void *b) {
  const Card *CardA = reinterpret_cast<const Card *>(a);
  const Card *CardB = reinterpret_cast<const Card *>(b);
  if (CardA->p.getpip() == CardB->p.getpip()) {
    return 0;
  }
  // Inverted for descending sort, per Exercise 3, p. 143.
  return (CardA->p.getpip() > CardB->p.getpip() ? -1 : 1);
}

void DescendingSortHand(Card *handp) {
  qsort(handp, kCardsPerHand, sizeof(Card), CompareCards);
}

void PrintHand(Card hand[]) {
  for (int i = 0; i < kCardsPerHand; i++) {
    hand[i].pr_card();
  }
  cout << endl;
}

bool IsFlush(Card hand[]) {
  suit first_suit = hand[0].s;
  int i = 1;
  while (i < kCardsPerHand) {
    if ((hand[i].s) != first_suit) {
      return false;
    }
    i++;
  }
  return true;
}

void FlushTest(Card hand[]) {
  if (IsFlush(hand)) {
    cout << "Hand is a flush." << endl;
  } else {
    cout << "Hand is NOT a flush." << endl;
  }
}

bool IsStraight(Card hand[]) {
  DescendingSortHand(hand);
  int i;
  for (i = 0; i < (kCardsPerHand - 1); i++) {
    if (1 != (hand[i].p.getpip() - hand[i + 1].p.getpip())) {
      return false;
    }
  }
  return true;
}

void StraightTest(Card hand[]) {
  if (IsStraight(hand)) {
    cout << "Hand is a straight." << endl;
  } else {
    cout << "Hand is NOT a straight." << endl;
  }
}

int main(void) {
  Card hand[kPlayers][kCardsPerHand];
  Deck deck;
  int i;

  // Get a (more or less) random number to seed the shuffle.
  // Use the milliseconds field of time since the Epoch.
  struct timeb *tp = static_cast<struct timeb *>(malloc(sizeof(struct timeb)));
  if (!tp) {
    cerr << "Out of memory";
    exit(ENOMEM);
  }
  ftime(tp);
  // Set the seed.
  srand(tp->millitm);
  free(tp);

  deck.InitDeck();
  deck.Shuffle();
  int start_hand = (rand() % 52);
  for (i = 0; i < kPlayers; i++) {
    deck.Deal(kCardsPerHand, start_hand, hand[i]);
    cout << "Hand " << i << " : " << endl;
    PrintHand(hand[i]);
    DescendingSortHand(&hand[i][0]);
    PrintHand(hand[i]);
    start_hand += kCardsPerHand;
    FlushTest(hand[i]);
    StraightTest(hand[i]);
  }

  cout << "Flush and straight test:" << endl;
  Card test[kCardsPerHand];
  for (i = 0; i < kCardsPerHand; i++) {
    test[i].assign(i);
  }
  PrintHand(test);
  FlushTest(test);
  StraightTest(test);

  cout << "Flush, not straight test:" << endl;
  for (i = 0; i < kCardsPerHand; i++) {
    test[i].assign(i / 2);
  }
  PrintHand(test);
  FlushTest(test);
  StraightTest(test);
}
