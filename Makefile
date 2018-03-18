# http://www.valgrind.org/docs/manual/quick-start.html#quick-start.prepare
# Compile your program with -g . . . Using -O0 is also a good idea, 
CFLAGS= -ggdb -Wall -Wextra -g -O0 -fno-inline -fsanitize=undefined -I/home/alison/gitsrc/googletest
LDFLAGS= -ggdb -g -fsanitize=undefined
#THREADFLAGS= -D_REENTRANT -I/usr/include/ntpl -L/usr/lib/nptl -lpthread
THREADFLAGS= -D_REENTRANT -lpthread
#https://gcc.gnu.org/ml/gcc-help/2003-08/msg00128.html
DEADCODESTRIP := -Wl,-static -fvtable-gc -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-s
# gcc and clang won't automatically link .cc files against the standard library.
CC=/usr/bin/g++
#CC=/usr/bin/clang
LIBWR=-Llibwr -lwr

#googletest stuff
# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
CFLAGSTEST = -ggdb -Wall -Wextra -g -O0 -fno-inline -fsanitize=undefined -pthread
GTEST_DIR =/home/alison/gitsrc/googletest/googletest
USER_DIR = .

num_digits: num_digits.cc num_digits.h
	$(CC) ${CFLAGS} ${LDFLAGS} calc_num_digits.cc -lm

libcalcfuncs: num_digits.o
	ar rvs libcalcfuncs.a num_digits.o
