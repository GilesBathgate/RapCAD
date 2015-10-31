#!/bin/bash

postfix=${1%.*}
if [ -n "$2" ]
then
((i=$2-1))
fi

cat $1 | while read line
do
 ((i++))
 if [ $i -lt 100 ]
  then
   if [ $i -lt 10 ]
    then n=00$i
    else n=0$i
   fi
  else n=$i
 fi
 file=$n"_$postfix.rcad"
 echo "function test(){" > $file
 echo "  return $line" >> $file
 echo "}" >> $file
done
