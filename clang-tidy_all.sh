#!/bin/bash

set -e
set -u

readonly OUTPUT=clang-tidy-output.txt

readonly CLANGTIDY=/usr/bin/clang-tidy
if [ ! -e "$CLANGTIDY" ]; then
   echo "Please install clang-tidy package."
   exit 1
fi

if ((0 == $#)); then
   echo "Please provide a list of binaries to check."
   exit 1
fi

/bin/rm -f "$OUTPUT"

for FILE in $@; do
      echo "Analyzing ${FILE} . . . "
      echo "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" >>  "$OUTPUT"
      echo "" >> "$OUTPUT"
      echo "make ${FILE}-clangtidy" >> "$OUTPUT"
      make "$FILE"-clangtidy >> "$OUTPUT" 2>&1
done
