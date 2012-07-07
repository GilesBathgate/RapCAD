#!/bin/bash

#Apply astyle rules to files that would be commited. 
astyle --options=astylerc $(git status -s | grep "^. .*\.h\|.cpp$" | cut -d' ' -f3)
