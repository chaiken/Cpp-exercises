#!/bin/bash
#
# Generate HTML-formatted gcov coverage analysis for the smarter_stack_lib_test
# program.
# Largely follows
# https://qiaomuf.wordpress.com/2011/05/26/use-gcov-and-lcov-to-know-your-test-coverage/

set -e
set -u
set -o pipefail

readonly LCOV=/usr/local/bin/lcov
echo "Using $(which lcov)"

if (( $# != 1)); then
  echo "Usage: run_lcov.sh <test name>, where 'test name' is that of the test"
  echo "binary, not that of the source file."
  exit 1
fi

if [ ! -e /usr/bin/lcov ]; then
  echo "Please install the lcov package."
  exit 1
fi

# Note: .gcno files are created by compilation with --coverage switch.
#       .gcda files are created by executing the binary directly or with
# 	 ""$LCOV" -c -o foo.info".

readonly test_name="$1"
echo "test_name is ${test_name}"
readonly target="$(grep "${test_name}:" Makefile)"
echo "Creating coverage data for ${test_name}."

# Actually run the test.  Not actually necessary, but test failure will as a
# side benefit terminate the script.
./"$test_name" > /tmp/"$test_name".out

# "man lcov" Capture coverage data; creates info file
# Use the --directory option to capture counts for a user  space  program.
# Running the binary directly creates .gcda files, but only running this command
# creates .info files.  Running the command with existing .gcda files does not
# work.
# See https://github.com/linux-test-project/lcov/issues/192 for discussion of
# problems with googletest and lcov.
"$LCOV" --rc geninfo_unexecuted_blocks=1 --ignore mismatch \
     --ignore-errors inconsistent -branch-coverage --base-directory . \
     --directory . --capture -o "$test_name".info

# Applications which depend on libraries that have their own tests:
#
# Depend on complex_lib.cc: complex_vector_lib_test, template_stack_lib_test,
# template_rotate_lib_test, template_vector_lib_test, template_list_lib_test
#
# Depend on polynomial_lib.cc: template_cycle_lib_test, template_vector_lib_test

# The blog posting above mentioned the same tracefile as input and output,
# which did not work for me.
# There does not appear to be any way to remove the test binary itself
# from the coverage analysis, which makes the resulting coverage percentages
# too high.
"$LCOV" --remove "$test_name".info \
     "/usr/*" \
     "/home/alison/gitsrc/googletest/*" \
     -o "$test_name"_processed.info

genhtml -o . -t "${test_name} coverage" \
    --num-spaces 2 "$test_name"_processed.info

# Removes all gcda files -- not what is needed.
# "$LCOV" --base-directory . --directory . --zerocounters
#
# Remove existing .gcda files that may interfere with creation of new ones
# when multiple tests include the same library.  If bar_lib depends on foo_lib
# therefore, run coverage analysis on bar_lib_test first, remove foo_lib.gcda,
# and then run foo_lib_test coverage analysis.
remove_conflicting_artifacts() {
  if (( 1 != $# )); then
    >&2 echo "remove_conflicting_arguments: Provide the name of a single test"
    >&2 echo "as an argument."
    exit 5
  fi
  local tname="$1"
  # Note tname and _test below, not "tname" or "$tname".
  local libname="${tname%%_test}"
  if [[ -z "$libname" ]]; then
    >&2 echo "Erroneous test name ${tname}?"
    exit 6
  fi
  if [[ -f "$libname".gcno ]]; then
    >&2 echo "*** Removing ${libname}.gcno ***"
    /bin/rm "$libname".gcno
  fi
  if [[ -f "$libname".gcda ]]; then
    >&2 echo "*** Removing ${libname}.gcda ***"
    /bin/rm "$libname".gcda
  fi
}

# complex_vector_lib, template_stack_lib, template_rotate_lib,
# template_vector_lib, template_list_lib all depend on complex_lib
if [[ ("$test_name" != "complex_lib_test") ]]; then
  remove_conflicting_artifacts complex_lib_test
fi
#
if [[ ("$test_name" != "term_lib_test") ]]; then
  remove_conflicting_artifacts term_lib_test
fi
if [[ ("$test_name" != "term_vector_lib_test") ]]; then
  remove_conflicting_artifacts term_vector_lib_test
fi
# matrix_lib depends on dbl_vector_lib
if [[ ("$test_name" != "dbl_vector_lib_test") ]]; then
  remove_conflicting_artifacts dbl_vector_lib_test
fi
# template_cycle_lib and template_vector_lib depend on polynomial_lib
if [[ ("$test_name" != "polynomial_lib_test") ]];
then
  remove_conflicting_artifacts polynomial_lib_test
fi
# template_cycle_lib, template_vector_lib and polynomial_lib depend on term_lib.
if [[ ("$test_name" != "term_lib_test") ]];
then
  remove_conflicting_artifacts term_lib_test
fi
# multiple_inheritance_lib depends on student_inheritance_lib.
if [[ ("$test_name" != "student_inheritance_lib_test") ]]; then
  remove_conflicting_artifacts student_inheritance_lib_test
fi
