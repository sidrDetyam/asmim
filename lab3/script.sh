#!/bin/bash

first_part=first_part
second_part=second_part
dummy_branches=dummy_branches
bruh=bruh.cpp
result=./results/result

rm -r ./results
mkdir ./results
cat /dev/null > $dummy_branches

for ((i = 0; i <= 10; i++))
do
  echo $i
  cat $first_part $dummy_branches $second_part > $bruh
  g++ $bruh -O0 -lpapi

  cat /dev/null > $result"-$i"
  for ((k = 3; k <= 100; k++))
  do
    ./a.out $k >> $result"-$i"
    ./a.out $k >> $result"-$i"
    ./a.out $k >> $result"-$i"
  done

  printf "\n\t\tif(i < -$i){unused = -$i;}\n" >> $dummy_branches
done
