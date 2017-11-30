#!/bin/bash
for filename in `find . -name '*.cpp'`;
do
  gcov -n -o . $filename > /dev/null;
done
