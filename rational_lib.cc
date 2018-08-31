#include "rational.h"

using namespace std;

namespace rational {

namespace {
int max(int i, int j) {
  return (i>=j ? i : j);
}

int min(int i, int j) {
  return (i>=j ? j : i);
}

}

int GCD(int i, int j) {
  if ((0==i) || (0==j)) {
    return 0;
  }
  if (i==j) {
    return i;
  }
  int divided = max(i,j), divisor = min(i,j);
  int ratio = divided/divisor;
  int remainder = divided - (ratio*divisor);
  if (0 == remainder) {
    return divisor;
  } else {
    return GCD(divided, remainder);
  }

}

}
