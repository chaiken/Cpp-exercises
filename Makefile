# See ~/gitsrc/googletest/googletest/make/Makefile
# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = /home/alison/gitsrc/googletest/googletest
# Wrong: do not include $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_HEADERS = $(GTEST_DIR)/include
GTESTLIBPATH=$(GTEST_DIR)/make
GTESTLIBS= $(GTESTLIBPATH)/libgtest.a $(GTESTLIBPATH)/gtest_main.a
# Where to find user code.
USER_DIR = .

# http://www.valgrind.org/docs/manual/quick-start.html#quick-start.prepare
# Compile your program with -g . . . Using -O0 is also a good idea, 
CXXFLAGS= -ggdb -Wall -Wextra -g -O0 -fno-inline -fsanitize=address -I$(GTEST_HEADERS)
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

LDFLAGS= -ggdb -g -fsanitize=undefined -L$(GTESTLIBPATH) -lpthread
#THREADFLAGS= -D_REENTRANT -I/usr/include/ntpl -L/usr/lib/nptl -lpthread
THREADFLAGS= -D_REENTRANT -lpthread
#https://gcc.gnu.org/ml/gcc-help/2003-08/msg00128.html
DEADCODESTRIP := -Wl,-static -fvtable-gc -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-s
# gcc and clang won't automatically link .cc files against the standard library.
CC=/usr/bin/g++
#CC=/usr/bin/clang
LIBWR=-Llibwr -lwr

num_digits: num_digits.cc num_digits.h
	$(CC) $(CFLAGS) $(LDFLAGS) calc_num_digits.cc -lm

libcalcfuncs: num_digits.o
	ar rvs libcalcfuncs.a num_digits.o

gcd: gcd.cc gcd_lib.cc gcd_lib.h
	$(CC) $(CFLAGS) $(LDFLAGS) gcd.cc gcd_lib.cc -o $@

gcd_lib_test: gcd_lib_test.cc gcd_lib.cc gcd_lib.h $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(GTESTLIBS) gcd_lib_test.cc gcd_lib.cc -o $@

reverse_char_stack: reverse_char_stack.cc reverse_char_stack_lib.cc \
    reverse_char_stack_lib.h
	$(CC) $(CFLAGS) $(LDFLAGS) reverse_char_stack.cc \
           reverse_char_stack_lib.cc -o $@

reverse_char_stack_lib_test: reverse_char_stack_lib_test.cc \
    reverse_char_stack_lib.cc reverse_char_stack_lib.h $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(GTESTLIBS) reverse_char_stack_lib_test.cc \
           reverse_char_stack_lib.cc -o $@

dyn_string_lib_test: dyn_string_lib.cc dyn_string.h dyn_string_lib_test.cc $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(GTESTLIBS) dyn_string_lib.cc \
           dyn_string_lib_test.cc -o $@

dyn_string: dyn_string_lib.cc dyn_string.h
	$(CC) $(CFLAGS) $(LDFLAGS) dyn_string_lib.cc dyn_string.cc -o $@

notqsort: notqsort.cc notqsort_lib.cc notqsort_lib.h
	$(CC) $(CFLAGS) $(LDFLAGS) notqsort_lib.cc notqsort.cc -o $@

notqsort_lib_test: notqsort_lib.cc notqsort_lib_test.cc notqsort_lib.h  $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(GTESTLIBS) notqsort_lib_test.cc \
           notqsort_lib.cc -o $@

dbl_vector_lib_test: dbl_vector_lib.cc dbl_vector_lib_test.cc dbl_vector.h  $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(GTESTLIBS) dbl_vector_lib.cc dbl_vector_lib_test.cc -o $@

dbl_vector_lib_test2: dbl_vector_lib.cc dbl_vector_lib_test2.cc dbl_vector.h  $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(GTESTLIBS) dbl_vector_lib.cc dbl_vector_lib_test2.cc -o $@

dbl_vector_time: dbl_vector_lib.cc dbl_vector_time.cc dbl_vector.h
	$(CC) $(CXXFLAGS) $(LDFLAGS) dbl_vector_lib.cc dbl_vector_time.cc -o $@

slist_main: slist_main.cc slist_lib.cc slist.h
	$(CC) $(CXXFLAGS) $(LDFLAGS) slist_main.cc slist_lib.cc -o $@

slist_lib_test: slist_lib.cc slist_lib_test.cc slist.h  $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(GTESTLIBS) slist_lib.cc slist_lib_test.cc -o $@

slist_lib2_test: slist_lib2.cc slist_lib2_test.cc slist2.h  $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(GTESTLIBS) slist_lib2.cc slist_lib2_test.cc -o $@

matrix_lib_test: matrix_lib.cc matrix_lib_test.cc matrix.h $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) -lm $(GTESTLIBS) matrix_lib.cc matrix_lib_test.cc -o $@

polynomial_lib_test: polynomial_lib.cc polynomial_lib_test.cc polynomial.h $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(GTESTLIBS) polynomial_lib.cc polynomial_lib_test.cc -o $@

test: reverse_char_stack_lib_test
	./reverse_char_stack_lib_test
