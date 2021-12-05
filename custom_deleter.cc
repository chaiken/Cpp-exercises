#include <cstdlib>
#include <iostream>
#include <memory>

// Following
// https://github.com/JLospinoso/ccc/blob/master/chapter_11/smart_ptrs.cpp
auto adeleter = [](int *x) noexcept {
  std::cout << "deleting int ptr" << std::endl;
  if (x) {
    delete x;
  }
};
using aup = std::unique_ptr<int, decltype(adeleter)>;

auto bdeleter = [](FILE *x) noexcept {
  std::cout << "deleting FILE ptr" << std::endl;
  if (x) {
    return fclose(x);
  }
  return 0;
};
// Following
// https://stackoverflow.com/questions/19053351/how-do-i-use-a-custom-deleter-with-a-stdunique-ptr-member
using fup = std::unique_ptr<FILE, int (*)(FILE *)>;

// A scope at whose termination memory is freed.
void create_aup_in_fn() {
  aup bint(new int(4), adeleter);
  std::cout << "sizeof bint: " << sizeof(bint) << std::endl;
}

int main() {
  create_aup_in_fn();

  // Without the {} scope delimiters, the deleter does not run and the memory
  // leaks.   Why is a main() function is not considered a scope?
  {
    aup anint{new int(4), adeleter};
    std::cout << "sizeof anint: " << sizeof(anint) << std::endl;
  }
  { fup fp{fopen("/etc/hosts", "r"), bdeleter}; }
  exit(EXIT_SUCCESS);
}

/*
Compile with
g++ custom_deleter.cc -o custom_deleter
since ASAN and valgrind are incompatible.

With scope around unique_ptr but without "delete &anint":
[alison@bitscream Cpp-Exercises (master)]$ valgrind --leak-check=full
--show-leak-kinds=all custom_deleter
==1625552== Memcheck, a memory error detector
==1625552== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1625552== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
==1625552== Command: custom_deleter
==1625552==
sizeof anint: 8
deleting
==1625552==
==1625552== HEAP SUMMARY:
==1625552==     in use at exit: 0 bytes in 0 blocks
==1625552==   total heap usage: 3 allocs, 3 frees, 73,732 bytes allocated
==1625552==
==1625552== All heap blocks were freed -- no leaks are possible
==1625552==
==1625552== For lists of detected and suppressed errors, rerun with: -s
==1625552== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)


Without { } scope around unique_ptr and without "delete &anint", a leak:
[alison@bitscream Cpp-Exercises (master)]$ valgrind --leak-check=full
custom_deleter
==1575150== Memcheck, a memory error detector
==1575150== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1575150== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
==1575150== Command: custom_deleter
==1575150==
sizeof anint: 8
==1575150==
==1575150== HEAP SUMMARY:
==1575150==     in use at exit: 4 bytes in 1 blocks
==1575150==   total heap usage: 3 allocs, 2 frees, 73,732 bytes allocated
==1575150==
==1575150== LEAK SUMMARY:
==1575150==    definitely lost: 0 bytes in 0 blocks
==1575150==    indirectly lost: 0 bytes in 0 blocks
==1575150==      possibly lost: 0 bytes in 0 blocks
==1575150==    still reachable: 4 bytes in 1 blocks
==1575150==         suppressed: 0 bytes in 0 blocks
==1575150== Reachable blocks (those to which a pointer was found) are not shown.
==1575150== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==1575150==
==1575150== For lists of detected and suppressed errors, rerun with: -s
==1575150== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
[alison@bitscream Cpp-Exercises (master)]$ valgrind --leak-check=full
--show-leak-kinds=all custom_deleter
==1576075== Memcheck, a memory error detector
==1576075== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1576075== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
==1576075== Command: custom_deleter
==1576075==
sizeof anint: 8
==1576075==
==1576075== HEAP SUMMARY:
==1576075==     in use at exit: 4 bytes in 1 blocks
==1576075==   total heap usage: 3 allocs, 2 frees, 73,732 bytes allocated
==1576075==
==1576075== 4 bytes in 1 blocks are still reachable in loss record 1 of 1
==1576075==    at 0x4838DEF: operator new(unsigned long)
(vg_replace_malloc.c:342)
==1576075==    by 0x1091C7: main (in
/home/alison/gitsrc/Cpp-Exercises/custom_deleter)
==1576075==
==1576075== LEAK SUMMARY:
==1576075==    definitely lost: 0 bytes in 0 blocks
==1576075==    indirectly lost: 0 bytes in 0 blocks
==1576075==      possibly lost: 0 bytes in 0 blocks
==1576075==    still reachable: 4 bytes in 1 blocks
==1576075==         suppressed: 0 bytes in 0 blocks
==1576075==
==1576075== For lists of detected and suppressed errors, rerun with: -s
==1576075== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

Without scope around unique_ptr but with "delete &anint", a double-free:
[alison@bitscream Cpp-Exercises (master)]$ valgrind
--leak-check=full --show-leak-kinds=all custom_deleter
==1578920== Memcheck, a memory error detector
==1578920== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1578920== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
==1578920== Command: custom_deleter
==1578920==
sizeof anint: 8
deleting
==1578920== Invalid free() / delete / delete[] / realloc()
==1578920==    at 0x483A08B: operator delete(void*, unsigned long)
(vg_replace_malloc.c:593)
==1578920==    by 0x109235: main (in
/home/alison/gitsrc/Cpp-Exercises/custom_deleter)
==1578920==  Address 0x1ffeffff48 is on thread 1's stack
==1578920==  in frame #1, created by main (???:)
==1578920==
==1578920==
==1578920== HEAP SUMMARY:
==1578920==     in use at exit: 0 bytes in 0 blocks
==1578920==   total heap usage: 3 allocs, 4 frees, 73,732 bytes allocated
==1578920==
==1578920== All heap blocks were freed -- no leaks are possible
==1578920==
==1578920== For lists of detected and suppressed errors, rerun with: -s
==1578920== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
*/
