/* {cvu} magazine p. 10, "Homework challenge 2", May 2021: "I need a
   program in C++ that will count the number of occurrences of the
   letter 'a' in ASCII file of text.  The count should include both
   upper and lower case instances of the letter."

   Here, figure out how Silas Brown's tortured, verbose solution
   works.
*/

#include <iostream>
#include <stdint.h>
#include <stdio.h>

using namespace std;

template <int b> static inline bool checkByte(uint64_t i) {
  // 0x41 is 'A' while 0x61 is 'a'.  Less obscure version:
  //  enum { shift = 8 * b, mask = 0xDFLL << shift, eq = 0x41LL << shift };
  enum { shift = 8 * b, mask = 0xFFLL << shift, eq = 0x41LL << shift };
  return (i & mask) == eq;
}

int main() {
  cout << "Enter the name of the ascii file: ";
  string filename;
  cin >> filename;
  /* The  mode string can also include the letter 'b' either as a last character
   * or as a character between the characters in any of the  two-character
   * strings described above.  This is strictly for compatibility with C89 and
   * has no effect; the 'b' is ignored on all POSIX conforming  systems,
   * including Linux.  (Other systems may treat text files and binary files
   * differently, and adding the 'b' may be a good idea if you do  I/O to a
   * binary file and expect that your program may be ported to non-UNIX
   * environments.) */
  FILE *f = fopen(filename.c_str(), "rb");
  // not in original code
  if (!f) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  fseek(f, 0, SEEK_END);
  // n is the number of bytes in the file.
  long c = 0, n = ftell(f);
  fseek(f, 0, SEEK_SET);
  // d is the number of 8-byte sequences in the file.   The '7' is there to make
  // certain that the division rounds up so that fread doesn't try to write past
  // the end of b.
  uint64_t d, *b = new uint64_t[d = (n + 7) >> 3];
  fread(b, 1, n, f);
  fclose(f);
  do {
    // 101 1111 1111 0x5F masks off all but the lowest 9 bits plus the
    // 11th bit.  0x41 is 100 0000 0001 and 0x61 is 110 0000 0001, so
    // the only function of the mask is to remove the bit in which they differ.
    uint64_t i = b[--d] // --d since the highest array value is d-1.
                        // Simply changing 0xFF in checkByte() to 0xDF would be
                        // less obscure, but so would boost::to_lower.
                 & 0x5F5F5F5F5F5F5F5FLL;
    c += checkByte<0>(i) + checkByte<1>(i) + checkByte<2>(i) + checkByte<3>(i) +
         checkByte<4>(i) + checkByte<5>(i) + checkByte<6>(i) + checkByte<7>(i);
  } while (d);
  cout << c << endl;
  // The following was not in original code.
  delete[] b;
  exit(EXIT_SUCCESS);
}
