#!/bin/bash

set -e
set -u


OUTPUT="$(mktemp)"
echo "Redirecting stdout to ${OUTPUT}"

TEST_EXTRA_FLAGS="-O2"
GTEST_DIR="$HOME"/gitsrc/googletest/googletest
GTEST_HEADERS="$GTEST_DIR"/include

export CXXFLAGS="-std=c++11 -pthread -ggdb -Wall -Wextra -g ${TEST_EXTRA_FLAGS} -fno-inline -fsanitize=address,undefined -I${GTEST_HEADERS}"

for FILE in $@
do
 # https://www.gnu.org/software/automake/manual/html_node/Tricks-For-Silencing-Make.html
  make "$FILE" > /dev/null || make
  ./"$FILE" >> "$OUTPUT"
done
