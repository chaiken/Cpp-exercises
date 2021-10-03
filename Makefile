# See ~/gitsrc/googletest/googletest/make/Makefile
# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = $(HOME)/gitsrc/googletest
# Wrong: do not include $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_HEADERS = $(GTEST_DIR)/googletest/include
GTESTLIBPATH=$(GTEST_DIR)/build/lib
# See ~/gitsrc/googletest/googletest/README.md.
# export GTEST_DIR=/home/alison/gitsrc/googletest/
# g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
# cd make; make all
# 'make' in the README.md above doesn't create libgtest_main.a.  'make all' does.
GTESTLIBS= $(GTESTLIBPATH)/libgtest.a $(GTESTLIBPATH)/libgtest_main.a
# Where to find user code.
USER_DIR = .

USDT_DIR=$(HOME)/gitsrc/bcc
USDT_LIB_PATH=$(USDT_DIR)/build/src/cc
USDT_HEADERS=$(USDT_DIR)/tests/python/include
USDT_LIBS=$(USDT_LIB_PATH)/libbcc.a $(USDT_LIB_PATH)/libbcc_bpf.a

# http://www.valgrind.org/docs/manual/quick-start.html#quick-start.prepare
# Compile your program with -g . . . Using -O0 is also a good idea, 
# cc1plus: error: ‘-fsanitize=address’ and ‘-fsanitize=kernel-address’ are incompatible with ‘-fsanitize=thread’
# https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html#C_002b_002b-Dialect-Options
# Weffc++: Warn about violations of the style guidelines from Scott Meyers’ Effective C++ series of books
# Turning Weffc++ on results in errors only from googletest.
#CXXFLAGS= -std=c++11 -pthread -ggdb -Wall -Wextra -Weffc++ -g -O0 -fno-inline -fsanitize=address,undefined -I$(GTEST_HEADERS)
CXXFLAGS= -std=c++11 -pthread -ggdb -Wall -Wextra -g -O0 -fno-inline -fsanitize=address,undefined -I$(GTEST_HEADERS)
CXXFLAG-NOTEST= -std=c++11 -ggdb -Wall -Wextra -g -O0 -fno-inline -fsanitize=address,undefined
CXXFLAGS-NOSANITIZE= -std=c++11 -ggdb -Wall -Wextra -g -O0 -fno-inline -I$(GTEST_HEADERS)
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include
USDT_FLAGS= -I$(USDT_HEADERS)

LDFLAGS= -ggdb -g -fsanitize=address -L$(GTESTLIBPATH) -lpthread
LDFLAGS-NOSANITIZE= -ggdb -g -L$(GTESTLIBPATH) -lpthread
LDFLAGS-NOTEST= -ggdb -g -fsanitize=address -lpthread
#LDFLAGS= -ggdb -g -L$(GTESTLIBPATH) -lpthread
#THREADFLAGS= -D_REENTRANT -I/usr/include/ntpl -L/usr/lib/nptl -lpthread
THREADFLAGS= -D_REENTRANT -lpthread
#https://gcc.gnu.org/ml/gcc-help/2003-08/msg00128.html
DEADCODESTRIP := -Wl,-static -fvtable-gc -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-s
# gcc and clang won't automatically link .cc files against the standard library.
CC=/usr/bin/g++
#CC=/usr/bin/clang
LIBWR=-Llibwr -lwr

template_%_lib_test:  template_%_lib_test.o template_%.h $(GTESTHEADERS)
	$(CC) $(CXXFLAGS)  $(LDFLAGS) $^ $(GTESTLIBS) -o $@

%_test:  %.o %_test.o  $(GTESTHEADERS)
	$(CC) $(CXXFLAGS)  $(LDFLAGS) $^ $(GTESTLIBS) -o $@

calc_num_digits: calc_num_digits.cc
	$(CC) $(CXXFLAGS) $(LDFLAGS) calc_num_digits.cc -lm -o $@

libcalcfuncs: num_digits.o num_digits.h
	ar rvs libcalcfuncs.a num_digits.o

gcd: gcd.cc gcd_lib.cc gcd_lib.h
	$(CC) $(CXXFLAGS) $(LDFLAGS) gcd.cc gcd_lib.cc -o $@

dyn_string: dyn_string_lib.cc dyn_string.h dyn_string.cc
	$(CC) $(CFLAGS) $(LDFLAGS) dyn_string_lib.cc dyn_string.cc -o $@

notqsort: notqsort.cc notqsort_lib.cc notqsort.h
	$(CC) $(CFLAGS) $(LDFLAGS) notqsort_lib.cc notqsort.cc -o $@

dbl_vector_time: dbl_vector_lib.cc dbl_vector_time.cc dbl_vector.h
	$(CC) $(CXXFLAGS) $(LDFLAGS) dbl_vector_lib.cc dbl_vector_time.cc -o $@

slist_main: slist_main.cc slist_lib.cc slist.h
	$(CC) $(CXXFLAGS) $(LDFLAGS) slist_main.cc slist_lib.cc -o $@

# Including matrix_lib.cc and matrix_lib_test.cc rather than the corresponding .o files
# causes both .cc's and .o's to be included in the link list, triggering ODR failures.
# Listing the headers as prerequisites means that the target will be recompiled if the
# header are changed.
matrix_lib_test: matrix_lib.o matrix_lib_test.o matrix.h dbl_vector.h dbl_vector_lib.cc

matrix_lib_test_debug: matrix_lib.cc matrix_lib_test.cc matrix.h dbl_vector.h dbl_vector_lib.cc $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) -DDEBUG $(LDFLAGS) -lm matrix_lib.cc matrix_lib_test.cc dbl_vector_lib.cc  $(GTESTLIBS) -o $@

polynomial_lib_test: polynomial_lib.o polynomial_lib_test.o polynomial.h polynomial_impl.h term_lib.cc term_vector_lib.cc term.h term_impl.h term_vector.h

polynomial_lib_test_debug: polynomial_lib.cc polynomial_lib_test.cc polynomial.h polynomial_impl.h term_lib.cc term_vector_lib.cc term.h term_impl.h term_vector.h $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) -DDEBUG $(LDFLAGS) polynomial_lib.cc polynomial_lib_test.cc term_lib.cc term_vector_lib.cc  $(GTESTLIBS) -o $@

complex_vector_lib_test: complex_vector_lib.o complex_vector_lib_test.o complex_lib.o complex_vector.h complex.h

reference_count_string_timer: reference_count_string_timer.cc reference_count_string_lib.cc reference_count_string.h $(GTESTLIBS)

# make DEBUG=DEBUG reference_count_string_timer for verbosity
reference_count_string_timer_debug: reference_count_string_timer.cc reference_count_string_lib.cc reference_count_string.h
	$(CC) $(CXXFLAGS) $(LDFLAGS) -DDEBUG="DEBUG" reference_count_string_timer.cc reference_count_string_lib.cc reference_count_string.h -o $@

# Does not compile.
#generic_stack_lib_test: generic_stack_lib.cc generic_stack.h generic_stack_lib_test.cc $(GTEST_HEADERS)
#      $(CC) $(CXXFLAGS) $(LDFLAGS) generic_stack_lib.cc generic_stack_lib_test.cc  $(GTESTLIBS) -o $@

# Since the generic pattern rule for template_%_lib_test include %_lib_test.o
# and template_%.h, listing them here violates the ODR.
template_stack_lib_test: complex_lib.cc complex.h $(GTESTLIBS)

# Needs a static rule due to CONSTSIZE
const_template_stack_lib_test: const_template_stack_lib_test.cc complex_lib.cc template_stack.h complex.h $(GTESTLIBS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) const_template_stack_lib_test.cc -DCONSTSIZE=20 $(GTESTLIBS) -o $@

macro-vs-template: macro-vs-template.cc macro-vs-template.h complex_lib.cc complex.h
	$(CC) $(CXXFLAGS-NOTEST) $(LDFLAGS-NOTEST) macro-vs-template.cc complex_lib.cc -o $@

template_cycle_lib_test: polynomial_lib.cc polynomial.h polynomial_impl.h term_lib.cc term.h term_impl.h term_vector_lib.cc term_vector.h $(GTESTLIBS)

template_rotate_lib_test: complex.h complex_lib.cc $(GTESTLIBS)

template_vector_lib_test: complex.h complex_lib.cc polynomial.h polynomial_impl.h polynomial_lib.cc term.h term_impl.h term_vector.h term_lib.cc term_vector_lib.cc $(GTESTLIBS)

template_vector_lib_test_debug: template_vector.h template_vector_impl.h template_vector_lib_test.cc complex.h complex_lib.cc polynomial.h polynomial_impl.h polynomial_lib.cc term.h term_impl.h term_vector.h term_lib.cc term_vector_lib.cc  $(GTEST_HEADERS)
	$(CC) $(CXXFLAGS) -DDEBUG="DEBUG" $(LDFLAGS) template_vector_lib_test.cc complex_lib.cc polynomial_lib.cc term_lib.cc term_vector_lib.cc $(GTESTLIBS) -o $@

template_vector_main: template_vector.h template_vector_impl.h term.h term_impl.h term_vector.h term_lib.cc term_vector_lib.cc template_vector_main.cc
	$(CC) $(CXXFLAGS) $(LDFLAGS) template_vector_main.cc term_lib.cc term_vector_lib.cc -o $@

# template_%_lib.cc is not a prerequisite of the generic template_%_lib_test
# rule, so adding it here is not an ODR violation.
template_list_lib_test: template_list_lib.cc complex.h complex_lib.cc $(GTESTLIBS)

# Test name does not match template_%_lib_test pattern.
reverse_list_lib_test: reverse_list.h reverse_list_impl.h reverse_list_lib_test.cc
	$(CC) $(CXXFLAGS) $(LDFLAGS) reverse_list_lib_test.cc $(GTESTLIBS) -o $@

inheritance_casting_main: student_inheritance.h student_inheritance_lib.cc inheritance_casting_main.cc
	$(CC) $(CXXFLAGS) $(LDFLAGS) student_inheritance_lib.cc inheritance_casting_main.cc -o $@

# Test name does not match template_%_lib_test pattern.
one_index_vector_lib_test: one_index_vector.h one_index_vector_impl.h one_index_vector_lib_test.cc
	$(CC) $(CXXFLAGS) $(LDFLAGS) one_index_vector_lib_test.cc $(GTESTLIBS) -o $@

override_vs_overload_main: override_vs_overload.h override_vs_overload_main.cc
	$(CC) $(CXXFLAGS) $(LDFLAGS) override_vs_overload_main.cc -o $@

multiple_inheritance_lib_test: student_inheritance.h student_inheritance_lib.cc

# Test name does not match template_%_lib_test pattern.
array_size_deduction_test: array_size_deduction_test.cc array_size_deduction_impl.h
	$(CC) $(CXXFLAGS) $(LDFLAGS)  array_size_deduction_test.cc $(GTESTLIBS) -o $@

BINARY_LIST = calc_num_digits gcd gcd_lib_test reverse_char_stack_lib_test dyn_string_lib_test dyn_string notqsort notqsort_lib_test dbl_vector_lib_test dbl_vector_time slist_main slist_lib_test slist_lib2_test matrix_lib_test matrix_lib_test_debug term_lib_test polynomial_lib_test polynomial_lib_test_debug reference_count_string_lib_test rational_lib_test complex_lib_test complex_vector_lib_test reference_count_string_timer reference_count_string_timer_debug smarter_stack_lib_test smarter_queue_lib_test smarter_list_lib_test new_clock_lib_test template_stack_lib_test const_template_stack_lib_test macro-vs-template template_cycle_lib_test template_rotate_lib_test template_vector_lib_test template_vector_lib_test_debug template_vector_main template_list_lib_test template_largest_lib_test template_integrate_lib_test reverse_list_lib_test student_inheritance_lib_test one_index_vector_lib_test override_vs_overload_main multiple_inheritance_lib_test

# Same list as above, but with main binaries and _debug targets removed.
# Removed const_template_stack_lib_test.
NO_DEPS_LIST = gcd_lib_test reverse_char_stack_lib_test dyn_string_lib_test notqsort_lib_test slist_lib_test slist_lib2_test matrix_lib_test reference_count_string_lib_test rational_lib_test smarter_stack_lib_test smarter_queue_lib_test smarter_list_lib_test new_clock_lib_test template_largest_lib_test template_integrate_lib_test reverse_list_lib_test one_index_vector_lib_test multiple_inheritance_lib_test

# complex_lib_test must run after all the tests listed below.
COMPLEX_LIB_DEPS_LIST = template_stack_lib_test template_rotate_lib_test template_vector_lib_test template_list_lib_test complex_vector_lib_test

# polynomial_lib_test must run after template_cycle_lib_test and template_vector_lib_test.
POLYNOMIAL_LIB_DEPS_LIST = template_cycle_lib_test template_vector_lib_test

# term_lib_test must run after polynomial_lib_test and tests that depend on it.
# term_vector_lib does not have its own tests.
TERM_LIB_DEPS_LIST = polynomial_lib_test

# dbl_vector_lib_test must run after matrix_lib_test.
# student_inheritance_lib_test must run after multiple_inheritance_lib_test.
MUST_RUN_LAST_LIST = dbl_vector_lib_test student_inheritance_lib_test template_vector_lib_test polynomial_lib_test term_lib_test complex_lib_test

clean:
	rm -rf *.o *~ $(BINARY_LIST) *.gcda *.gcov *.gcno *.info *_output *css *html a.out

all:
	make clean
	make $(BINARY_LIST)

.SILENT: *.o

# “–coverage” is a synonym for-fprofile-arcs, -ftest-coverage(compiling) and
# -lgcov(linking).
COVERAGE_EXTRA_FLAGS = --coverage -Werror

$(NO_DEPS_LIST) $(COMPLEX_LIBS_DEPS_LIST) $(POLYNOMIAL_LIBS_DEPS_LIST) $(TERM_LIB_DEPS_LIST) $(MUST_RUN_LAST_LIST): CXXFLAGS += $(COVERAGE_EXTRA_FLAGS)

# https://github.com/gcovr/gcovr/issues/314
# 'A “stamp mismatch” error is shown when the compilation time stamp *within*
# the gcno and gcda files doesn't match.'
# Therefore compilation must take place in association with the same test binary,
# not in association with another dependent test.
coverage_all:
	make clean
	@echo "CXXFLAGS is $(CXXFLAGS)"
	run_lcov_all.sh $(NO_DEPS_LIST)
	run_lcov_all.sh $(COMPLEX_LIB_DEPS_LIST)
	run_lcov_all.sh $(POLYNOMIAL_LIB_DEPS_LIST)
	run_lcov_all.sh $(TERM_LIB_DEPS_LIST)
	run_lcov_all.sh $(MUST_RUN_LAST_LIST)

TEST_LIST = gcd_lib_test reverse_char_stack_lib_test dyn_string_lib_test notqsort_lib_test dbl_vector_lib_test slist_lib_test slist_lib2_test matrix_lib_test matrix_lib_test_debug term_lib_test polynomial_lib_test polynomial_lib_test_debug reference_count_string_lib_test rational_lib_test complex_lib_test complex_vector_lib_test smarter_stack_lib_test smarter_queue_lib_test smarter_list_lib_test new_clock_lib_test template_stack_lib_test const_template_stack_lib_test template_cycle_lib_test template_rotate_lib_test template_vector_lib_test template_vector_lib_test_debug template_list_lib_test template_largest_lib_test template_integrate_lib_test reverse_list_lib_test student_inheritance_lib_test one_index_vector_lib_test multiple_inheritance_lib_test

TEST_EXTRA_FLAGS = -Werror -O2
# None of these work work:
# $(TEST_LIST): CXXFLAGS = -std=c++11 -pthread -ggdb -Wall -Wextra -g $(TEST_EXTRA_FLAGS) -fno-inline -fsanitize=address,undefined -I$(GTEST_HEADERS)
#TEST_LIST: CXXFLAGS = -std=c++11 -pthread -ggdb -Wall -Wextra -g $(TEST_EXTRA_FLAGS) -fno-inline -fsanitize=address,undefined -I$(GTEST_HEADERS)
TESTFLAGS = -std=c++11 -pthread -ggdb -Wall -Wextra -g $(TEST_EXTRA_FLAGS) -fno-inline -fsanitize=address,undefined -I$(GTEST_HEADERS)

# Doesn't work.
#%.o: %.cc
#	override CXXFLAGS = $(TESTFLAGS)
#	$(CC) $(CXXFLAGS) $(LDFLAGS) $< -o $@

test_all:
	make clean
	@echo "CXXFLAGS is $(CXXFLAGS)"
	run_all_tests.sh $(TEST_LIST)
