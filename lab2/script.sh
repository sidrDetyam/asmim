#!/bin/bash

let step=25
let mn=1
let mx=15

echo "Instructions per cache miss : avg cache miss time(clocks)"

for((i=mn; i<mx; ++i))
do
  let cnt=i*step

  cat first_part.s > measure.s
  for((j=0; j<cnt; ++j))
  do
      echo "nop" >> measure.s
  done
  cat second_part.s >> measure.s
  gcc -std=c11 main.c measure.s -O0

  ./a.out $cnt
  echo ""
done

printf "\ndone\n"
rm a.out
