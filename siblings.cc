// Also, needs a complete definition of sister to compile, and vice-versa.
// Simple forward declaration won't work, as structs are composite types of a
// priori unknown size.
//   struct sister s;
// won't work.  Pointer to the struct is okay, as sizeof(struct *) is
// independent of sizeof(struct).

#include <cstdlib>

struct brother {
  char name[20];
  int age;
  // struct sister sib;
  struct sister *sib;
} a;

struct sister {
  char name[20];
  int age;
  // struct brother sib;
  struct brother *sib;
} b;
// Why won't compile: multiple declarations of a.
// } a;

int main(void) {
  exit(EXIT_SUCCESS);
}
