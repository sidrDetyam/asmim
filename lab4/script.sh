#!/bin/bash

plain=plain
result=result
cnt_threads=4
exe=./a.out
freq=1.8

cat /dev/null > $result
printf "Стандартные функции\n" >> $result

cat $plain/data_type_int.h > $plain/data_type.h
gcc -o a.out main.c test/teststd.c plain/plain1.c utils.c plain/plain_std.c -O3 -msse -lpthread

for ((i = 1; i <= cnt_threads; i++))
do
  echo $i
  printf "Потоков $i: " >> $result
  $exe "$i" "$freq" >> $result
  printf "\n" >> $result
done

printf "\nПростые циклы\n" >> $result
for ((k = 1; k <= 4; k = k*2))
do
  echo $k
  printf "\nРаскрутка $k\n" >> $result
  gcc -o a.out main.c test/test.c "plain/plain$k.c" utils.c plain/plain_std.c -O3 -msse -lpthread

  for ((i = 1; i <= cnt_threads; i++))
  do
    printf "Потоков $i: " >> $result
    $exe "$i" "$freq" >> $result
    printf "\n" >> $result
  done
done

printf "\nSIMD-циклы\n" >> $result
for ((k = 1; k <= 4; k = k*2))
do
  echo $k
  printf "\nРаскрутка $k\n" >> $result
  gcc -o a.out main.c test/testv.c "plain/plain$k.c" utils.c plain/plain_std.c -O3 -msse -lpthread

  for ((i = 1; i <= cnt_threads; i++))
  do
    printf "Потоков $i: " >> $result
    $exe "$i" "$freq" >> $result
    printf "\n" >> $result
  done
done

cat $result