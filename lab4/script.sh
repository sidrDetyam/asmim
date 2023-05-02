
gcc -o a.out main.c test/test.c plain/plain1.c utils.c plain/plain_std.c -O3 -msse -lpthread
./a.out
