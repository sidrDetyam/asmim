#!/bin/bash
PBS -l select=1:ncpus=1

cd $PBS_O_WORKDIR

echo "compiling..."
gcc -std=c11 main.c -O0
echo "" > results
./a.out >> results

