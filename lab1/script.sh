#!/bin/bash

echo "compiling..."
gcc -std=c11 main.c measure.s -O0

echo "" > results

for i in {1..10}
do
  echo "measure $i/10"
  ./a.out >> results
done
echo "done"
cat results
rm a.out
