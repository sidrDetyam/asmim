#!/bin/bash

let step=10
let mn=1
let mx=25

echo "Instructions per cache miss : avg cache miss time(clocks)"

for((i=mn; i<mx; ++i))
do
  echo ""

  let cnt=i*step
  #echo "$cnt nops:"
  cat first_part.s > measure.s

  for((j=0; j<cnt; ++j))
  do
      echo "nop" >> measure.s
  done

  cat second_part.s >> measure.s

  gcc -std=c11 main.c measure.s -O0

  #echo "measure $i/25"
  ./a.out $cnt
done

echo "done"
rm a.out