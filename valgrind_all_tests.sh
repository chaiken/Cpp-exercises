#!/bin/bash

set -e
set -u

VALGRIND=/usr/bin/valgrind

if [ ! -e "$VALGRIND" ]; then
   echo "Please install valgrind package."
   exit 1
fi

if ((0 == $#)); then
   echo "Please provide a list of binaries to check."
   exit 1
fi

OUTPUT="$(mktemp)"
echo "Redirecting stdout to ${OUTPUT}"

TEST_EXTRA_FLAGS="-O0"
GTEST_DIR="$HOME"/gitsrc/googletest/googletest
GTEST_HEADERS="$GTEST_DIR"/include
GTESTLIBPATH="$GTEST_DIR"/build/lib

export CXXFLAGS="-std=c++11 -pthread -ggdb -Wall -Wextra -g ${TEST_EXTRA_FLAGS} -fno-inline -I${GTEST_HEADERS}"
export LDFLAGS="-ggdb -g -L$GTESTLIBPATH"

for FILE in $@
do
 # https://www.gnu.org/software/automake/manual/html_node/Tricks-For-Silencing-Make.html
    make "$FILE"-valgrind > /dev/null || make
   # valgrind prints all output to stderr
  "$VALGRIND" ./"$FILE"-valgrind >> "$OUTPUT" 2>&1
done
