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

# https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html#C_002b_002b-Dialect-Options
# Weffc++: Warn about violations of the style guidelines from Scott Meyers’ Effective C++ series of books
# Turning Weffc++ on results in errors only from googletest.
#CXXFLAGS= -std=c++11 -pthread -ggdb -Wall -Wextra -Weffc++ -g -O0 -fno-inline -fsanitize=address,undefined -I$(GTEST_HEADERS)
#
# Note -pthread, not -lpthread.   Without this option, Googletest does not compile.
CXXFLAGS= -std=c++17 -pthread -ggdb -Wall -Wextra -Werror -g -O0 -fno-inline -fsanitize=address,undefined -I$(GTEST_HEADERS)
CXXFLAG-NOTEST= -std=c++17 -ggdb -Wall -Wextra -Werror -g -O0 -fno-inline -fsanitize=address,undefined
CXXFLAGS-NOSANITIZE= -std=c++17 -pthread -ggdb -Wall -Wextra -Werror -g -O0 -fno-inline -I$(GTEST_HEADERS)
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include
# https://developers.redhat.com/blog/2020/03/26/static-analysis-in-gcc-10#what_are_the_new_warnings_
#CPPFLAGS += -fanalyzer -fdiagnostics-path-format=separate-events
USDT_FLAGS= -I$(USDT_HEADERS)

LDFLAGS= -ggdb -g -fsanitize=address -L$(GTESTLIBPATH)
LDFLAGS-NOSANITIZE= -ggdb -g -L$(GTESTLIBPATH)
LDFLAGS-NOTEST= -ggdb -g -fsanitize=address
#LDFLAGS= -ggdb -g -L$(GTESTLIBPATH) -lpthread
#THREADFLAGS= -D_REENTRANT -I/usr/include/ntpl -L/usr/lib/nptl -lpthread
THREADFLAGS= -D_REENTRANT -lpthread
#https://gcc.gnu.org/ml/gcc-help/2003-08/msg00128.html
DEADCODESTRIP := -Wl,-static -fvtable-gc -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-s
# gcc and clang won't automatically link .cc files against the standard library.
CXX=/usr/bin/g++
#CXX=/usr/local/bin/g++
#CXX=/usr/bin/clang++
LIBWR=-Llibwr -lwr

# template class have the code in the template_%_impl.h file, not a *lib.cc
#  file, so the template_%_impl.h header must be explicited included in the
# compilation.  It needs the template_%.h file which includes it in order to
# compile.
template_%_lib_test:  template_%_lib_test.o template_%.h $(GTESTHEADERS)
	$(CXX) $(CXXFLAGS)  $(LDFLAGS) $^ $(GTESTLIBS) -o $@

# Compile tests without template class or functions.
%_test:  %.o %_test.o  $(GTESTHEADERS)
	$(CXX) $(CXXFLAGS)  $(LDFLAGS) $^ $(GTESTLIBS) -o $@

# “–coverage” is a synonym for-fprofile-arcs, -ftest-coverage(compiling) and
# -lgcov(linking).
COVERAGE_EXTRA_FLAGS = --coverage
%lib_test-coverage: CXXFLAGS = $(CXXFLAGS-NOSANITIZE) $(COVERAGE_EXTRA_FLAGS)
%lib_test-coverage: LDFLAGS = $(LDFLAGS-NOSANITIZE)
%lib_test-coverage:  %lib_test.cc %lib.cc $(GTESTHEADERS)
	$(CXX) $(CXXFLAGS)  $(LDFLAGS) $^ $(GTESTLIBS) -o $@
	run_lcov.sh $@

template_%_lib_test-coverage: CXXFLAGS = $(CXXFLAGS-NOSANITIZE) $(COVERAGE_EXTRA_FLAGS)
template_%_lib_test-coverage: LDFLAGS = $(LDFLAGS-NOSANITIZE)
template_%_lib_test-coverage:  template_%_lib_test.o template_%.h $(GTESTHEADERS)
	$(CXX) $(CXXFLAGS)  $(LDFLAGS) $^ $(GTESTLIBS) -o $@
	run_lcov.sh $@

# http://www.valgrind.org/docs/manual/quick-start.html#quick-start.prepare
# Compile your program with -g . . . Using -O0 is also a good idea,
# cc1plus: error: ‘-fsanitize=address’ and ‘-fsanitize=kernel-address’ are incompatible with ‘-fsanitize=thread’
# https://www.gnu.org/software/make/manual/make.html#Pattern_002dspecific
# Without the flags redefinition, setting the object files as prerequisites of
# %_lib_test-valgrind would trigger compilation with the rules above, which has
# the wrong flags.   Valgrind does not work with the sanitizers.
# "As with target-specific variable values, multiple pattern values create a
# pattern-specific variable value for each pattern individually."
%_test-valgrind template_%_lib_test-valgrind : CXXFLAGS = $(CXXFLAGS-NOSANITIZE) -O0
%_test-valgrind template_%_lib_test-valgrind :  LDFLAGS = $(LDFLAGS-NOSANITIZE)
%_test-valgrind: %_test.o %.o $(GTESTHEADERS)
	$(CXX) $(CXXFLAGS-NOSANITIZE) $(LDFLAGS-NOSANITIZE) $^ $(GTESTLIBS) -o $@

template_%_lib_test-valgrind: template_%_lib_test.o template_%.h $(GTESTHEADERS)
	$(CXX) $(CXXFLAGS-NOSANITIZE) $(LDFLAGS-NOSANITIZE) $^ $(GTESTLIBS) -o $@

# https://clang.llvm.org/extra/clang-tidy/
# https://stackoverflow.com/questions/47583057/configure-clang-check-for-c-standard-libraries
# Without "-x c++", .h files are considered as C.
# Without header_filter, gtest.h is analyzed.
CLANG_TIDY_OPTIONS=--warnings-as-errors --header_filter=.*
CLANG_TIDY_CLANG_OPTIONS=-std=c++17 -x c++ -I ~/gitsrc/googletest/googletest/include/
CLANG_TIDY_CHECKS=bugprone,core,cplusplus,cppcoreguidelines,deadcode,modernize,performance,readability,security,unix,apiModeling.StdCLibraryFunctions,apiModeling.google.GTest

# Has matching lib.cc file.
%_lib_test-clangtidy: %_lib_test.cc %_lib.cc %.h
	clang-tidy $(CLANG_TIDY_OPTIONS) -checks=$(CLANG_TIDY_CHECKS) $^ -- $(CLANG_TIDY_CLANG_OPTIONS)

# No lib.cc file.
%_lib_test-clangtidy: %_lib_test.cc %.h
	clang-tidy $(CLANG_TIDY_OPTIONS) -checks=$(CLANG_TIDY_CHECKS) $^ -- $(CLANG_TIDY_CLANG_OPTIONS)

# Has matching lib.cc file.
template_%_lib_test-clangtidy: template_%_lib.cc template_%_lib_test.cc template_%.h
	clang-tidy $(CLANG_TIDY_OPTIONS) -checks=$(CLANG_TIDY_CHECKS) $^ -- $(CLANG_TIDY_CLANG_OPTIONS)

# No lib.cc file.
template_%_lib_test-clangtidy: template_%_lib_test.cc template_%.h
	clang-tidy $(CLANG_TIDY_OPTIONS) -checks=$(CLANG_TIDY_CHECKS) $^ -- $(CLANG_TIDY_CLANG_OPTIONS)

#binaries without tests
calc_num_digits: calc_num_digits.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) calc_num_digits.cc -lm -o $@

libcalcfuncs: num_digits.o num_digits.h
	ar rvs libcalcfuncs.a num_digits.o

gcd: gcd.cc gcd_lib.cc gcd_lib.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) gcd.cc gcd_lib.cc -o $@

dyn_string: dyn_string_lib.cc dyn_string.h dyn_string.cc
	$(CXX) $(CFLAGS) $(LDFLAGS) dyn_string_lib.cc dyn_string.cc -o $@

notqsort: notqsort.cc notqsort_lib.cc notqsort.h
	$(CXX) $(CFLAGS) $(LDFLAGS) notqsort_lib.cc notqsort.cc -o $@

#dbl_vector_time: dbl_vector_lib.cc dbl_vector_time.cc dbl_vector.h
#	$(CXX) $(CXXFLAGS) $(LDFLAGS) dbl_vector_lib.cc dbl_vector_time.cc -o $@

slist_main: slist_main.cc slist_lib.cc slist.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) slist_main.cc slist_lib.cc -o $@

# Including matrix_lib.cc and matrix_lib_test.cc rather than the corresponding .o files
# causes both .cc's and .o's to be included in the link list, triggering ODR failures.
# Listing the headers as prerequisites means that the target will be recompiled if the
# header are changed.
matrix_lib_test-coverage matrix_lib_test-valgrind matrix_lib_test-clangtidy matrix_lib_test: dbl_vector.h dbl_vector_lib.cc

matrix_lib_test_debug: matrix_lib.cc matrix_lib_test.cc matrix.h dbl_vector.h dbl_vector_lib.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -DDEBUG $(LDFLAGS) -lm matrix_lib.cc matrix_lib_test.cc dbl_vector_lib.cc  $(GTESTLIBS) -o $@

term_vector_lib.o: term_vector_lib.cc term_vector.h term_lib.cc term.h
	$(CXX) $(CXXFLAGS)  $(LDFLAGS) $^ $(GTESTLIBS) -o $@

polynomial_lib_test-coverage polynomial_lib_test-valgrind polynomial_lib_test-clangtidy polynomial_lib_test: term_lib.cc term_vector_lib.cc term.h  term_vector.h

polynomial_lib_test_debug: polynomial_lib.cc polynomial_lib_test.cc polynomial.h term_lib.cc term_vector_lib.cc term.h term_vector.h $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -DDEBUG $(LDFLAGS) polynomial_lib.cc polynomial_lib_test.cc term_lib.cc term_vector_lib.cc  $(GTESTLIBS) -o $@

complex_vector_lib_test-coverage complex_vector_lib_test-valgrind complex_vector_lib_test-clangtidy complex_vector_lib_test: complex_lib.cc complex_vector.h complex.h

reference_count_string_timer-valgrind reference_count_string_timer: reference_count_string_timer.cc reference_count_string_lib.cc reference_count_string.h $(GTESTLIBS)

# make DEBUG=DEBUG reference_count_string_timer for verbosity
reference_count_string_timer_debug: reference_count_string_timer.cc reference_count_string_lib.cc reference_count_string.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -DDEBUG="DEBUG" reference_count_string_timer.cc reference_count_string_lib.cc reference_count_string.h -o $@

# Does not compile.
#generic_stack_lib_test: generic_stack_lib.cc generic_stack.h generic_stack_lib_test.cc $(GTEST_HEADERS)
#      $(CXX) $(CXXFLAGS) $(LDFLAGS) generic_stack_lib.cc generic_stack_lib_test.cc  $(GTESTLIBS) -o $@

# Since the generic pattern rule for template_%_lib_test include %_lib_test.o
# and template_%.h, listing them here violates the ODR.
template_stack_lib_test-clangtidy: complex_lib.cc complex.h
template_stack_lib_test-coverage template_stack_lib_test-valgrind template_stack_lib_test: complex_lib.cc complex.h $(GTESTLIBS)

# Needs a static rule due to CONSTSIZE
const_template_stack_lib_test: const_template_stack_lib_test.cc complex_lib.cc template_stack.h complex.h $(GTESTLIBS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) const_template_stack_lib_test.cc -DCONSTSIZE=20 $(GTESTLIBS) -o $@
const_template_stack_lib_test-valgrind: const_template_stack_lib_test.cc complex_lib.cc template_stack.h complex.h $(GTESTLIBS)
	$(CXX) $(CXXFLAGS-NOSANITIZE) $(LDFLAGS-NOSANITIZE) const_template_stack_lib_test.cc -DCONSTSIZE=20 $(GTESTLIBS) -o $@
const_template_stack_lib_test-coverage: const_template_stack_lib_test.cc complex_lib.cc template_stack.h complex.h $(GTESTLIBS)
	$(CXX) $(CXXFLAGS-NOSANITIZE) $(COVERAGE_EXTRA_FLAGS) $(LDFLAGS-NOSANITIZE) const_template_stack_lib_test.cc -DCONSTSIZE=20 $(GTESTLIBS) -o $@

macro-vs-template: macro-vs-template.cc macro-vs-template.h complex_lib.cc complex.h
	$(CXX) $(CXXFLAGS-NOTEST) $(LDFLAGS-NOTEST) macro-vs-template.cc complex_lib.cc -o $@

template_cycle_lib_test-coverage template_cycle_lib_test-valgrind template_cycle_lib_test: polynomial_lib.cc polynomial.h term_lib.cc term.h term_vector_lib.cc term_vector.h $(GTESTLIBS)

template_rotate_lib_test-coverage template_rotate_lib_test-valgrind template_rotate_lib_test: complex.h complex_lib.cc $(GTESTLIBS)

template_vector_lib_test-coverage template_vector_lib_test-valgrind template_vector_lib_test: complex.h complex_lib.cc polynomial.h  polynomial_lib.cc term.h term_vector.h term_lib.cc term_vector_lib.cc $(GTESTLIBS)

template_vector_lib_test_debug: template_vector.h template_vector_lib_test.cc complex.h complex_lib.cc polynomial.h polynomial_lib.cc term.h term_vector.h term_lib.cc term_vector_lib.cc  $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -DDEBUG="DEBUG" $(LDFLAGS) template_vector_lib_test.cc complex_lib.cc polynomial_lib.cc term_lib.cc term_vector_lib.cc $(GTESTLIBS) -o $@

template_vector_main: template_vector.h term.h term_vector.h term_lib.cc term_vector_lib.cc template_vector_main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) template_vector_main.cc term_lib.cc term_vector_lib.cc -o $@

# template_%_lib.cc is not a prerequisite of the generic template_%_lib_test
# rule, so adding it here is not an ODR violation.
template_list_lib_test-coverage template_list_lib_test-valgrind template_list_lib_test: template_list_lib.cc complex.h complex_lib.cc $(GTESTLIBS)

# Test name does not match template_%_test pattern.
reverse_list_lib_test: reverse_list.h reverse_list_lib_test.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) reverse_list_lib_test.cc $(GTESTLIBS) -o $@
reverse_list_lib_test-valgrind: reverse_list.h reverse_list_lib_test.cc
	$(CXX) $(CXXFLAGS-NOSANITIZE) $(LDFLAGS-NOSANITIZE) reverse_list_lib_test.cc $(GTESTLIBS) -o $@
reverse_list_lib_test-coverage:
	$(CXX) $(CXXFLAGS-NOSANITIZE) $(COVERAGE_EXTRA_FLAGS) $(LDFLAGS-NOSANITIZE) reverse_list_lib_test.cc $(GTESTLIBS) -o $@

inheritance_casting_main: student_inheritance.h student_inheritance_lib.cc inheritance_casting_main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) student_inheritance_lib.cc inheritance_casting_main.cc -o $@

# Test name does not match template_%_test pattern.
one_index_vector_lib_test: one_index_vector.h one_index_vector_lib_test.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) one_index_vector_lib_test.cc $(GTESTLIBS) -o $@
one_index_vector_lib_test-valgrind: one_index_vector.h  one_index_vector_lib_test.cc
	$(CXX) $(CXXFLAGS-NOSANITIZE) $(LDFLAGS-NOSANITIZE) one_index_vector_lib_test.cc $(GTESTLIBS) -o $@
one_index_vector_lib_test-coverage: one_index_vector.h one_index_vector_lib_test.cc
	$(CXX) $(CXXFLAGS-NOSANITIZE) $(COVERAGE_EXTRA_FLAGS) $(LDFLAGS-NOSANITIZE) one_index_vector_lib_test.cc $(GTESTLIBS) -o $@
	run_lcov.sh $@

override_vs_overload_main: override_vs_overload.h override_vs_overload_main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) override_vs_overload_main.cc -o $@

multiple_inheritance_lib_test-coverage multiple_inheritance_lib_test-valgrind multiple_inheritance_lib_test: student_inheritance.h student_inheritance_lib.cc

# Test name does not match template_%_test pattern.
array_size_deduction_test: array_size_deduction_test.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS)  array_size_deduction_test.cc $(GTESTLIBS) -o $@
array_size_deduction_test-valgrind: array_size_deduction_test.cc
	$(CXX) $(CXXFLAGS-NOSANITIZE) $(LDFLAGS-NOSANITIZE)  array_size_deduction_test.cc $(GTESTLIBS) -o $@
array_size_deduction_test-coverage: array_size_deduction_test.cc
	$(CXX) $(CXXFLAGS-NOSANITIZE) $(COVERAGE_EXTRA_FLAGS) $(LDFLAGS-NOSANITIZE)  array_size_deduction_test.cc $(GTESTLIBS) -o $@

BINARY_LIST = calc_num_digits gcd gcd_lib_test reverse_char_stack_lib_test dyn_string_lib_test dyn_string notqsort notqsort_lib_test dbl_vector_lib_test slist_main slist_lib_test slist2_lib_test matrix_lib_test matrix_lib_test_debug term_lib_test polynomial_lib_test polynomial_lib_test_debug reference_count_string_lib_test rational_lib_test complex_lib_test complex_vector_lib_test reference_count_string_timer reference_count_string_timer_debug smarter_stack_lib_test smarter_queue_lib_test smarter_list_lib_test new_clock_lib_test template_stack_lib_test const_template_stack_lib_test macro-vs-template template_cycle_lib_test template_rotate_lib_test template_vector_lib_test template_vector_lib_test_debug template_vector_main template_list_lib_test template_largest_lib_test template_integrate_lib_test reverse_list_lib_test student_inheritance_lib_test one_index_vector_lib_test override_vs_overload_main multiple_inheritance_lib_test array_size_deduction_test address-of-function-parameter

# Same list as above, but with main binaries and _debug targets removed.
# Removed const_template_stack_lib_test.
NO_DEPS_LIST = gcd_lib_test reverse_char_stack_lib_test dyn_string_lib_test notqsort_lib_test slist_lib_test slist2_lib_test matrix_lib_test reference_count_string_lib_test rational_lib_test smarter_stack_lib_test smarter_queue_lib_test smarter_list_lib_test new_clock_lib_test template_largest_lib_test template_integrate_lib_test reverse_list_lib_test one_index_vector_lib_test multiple_inheritance_lib_test

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
	rm -rf *.o *~ $(BINARY_LIST) *_test *-coverage *-valgrind *.gcda *.gcov *.gcno *.info *_output *css *html a.out

all:
	make clean
	make $(BINARY_LIST)

.SILENT: *.o

# https://github.com/gcovr/gcovr/issues/314
# 'A “stamp mismatch” error is shown when the compilation time stamp *within*
# the gcno and gcda files doesn't match.'
# Therefore compilation must take place in association with the same test binary,
# not in association with another dependent test.
coverage_all:  CXXFLAGS += $(COVERAGE_EXTRA_FLAGS)
coverage_all:
	make clean
	@echo "CXXFLAGS is $(CXXFLAGS)"
	run_lcov_all.sh $(NO_DEPS_LIST)
	run_lcov_all.sh $(COMPLEX_LIB_DEPS_LIST)
	run_lcov_all.sh $(POLYNOMIAL_LIB_DEPS_LIST)
	run_lcov_all.sh $(TERM_LIB_DEPS_LIST)
	run_lcov_all.sh $(MUST_RUN_LAST_LIST)

TEST_LIST = gcd_lib_test reverse_char_stack_lib_test dyn_string_lib_test notqsort_lib_test dbl_vector_lib_test slist_lib_test slist2_lib_test matrix_lib_test term_lib_test polynomial_lib_test reference_count_string_lib_test rational_lib_test complex_lib_test complex_vector_lib_test smarter_stack_lib_test smarter_queue_lib_test smarter_list_lib_test new_clock_lib_test template_stack_lib_test const_template_stack_lib_test template_cycle_lib_test template_rotate_lib_test template_vector_lib_test template_list_lib_test template_largest_lib_test template_integrate_lib_test reverse_list_lib_test student_inheritance_lib_test one_index_vector_lib_test multiple_inheritance_lib_test array_size_deduction_test

test_all: CXXFLAGS += -O2
test_all:
	make clean
	run_all_tests.sh $(TEST_LIST)

valgrind_all: 
	make clean
	valgrind_all_tests.sh $(TEST_LIST)

clang_tidy_all:
	CLANG_TIDY_OUTPUT=clang-tidy-output.txt
