#!/bin/bash
RC="$(dirname $0)/astylerc"
files=$(git status -s | grep "^[^ D] .*\(h\|cpp\)$" | cut -d' ' -f3)
#Apply astyle rules to files that would be commited. 
if [ -n "$files" ]; then
  astyle --options=$RC $files
else
  echo "You need to add some files to the index first!"
fi
