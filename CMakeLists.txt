cmake_minimum_required(VERSION 3.24)
project(asmim)

set(CMAKE_C_STANDARD 11)
#set(CMAKE_CXX_FLAGS  "-O0")

add_executable(asmim
        lab4/non_vectorized.c lab4/plain.h lab4/utils.c lab4/utils.h lab4/plain1.c lab4/plain_common.c lab4/test.h lab4/teststd.c lab4/testv.c lab4/test.c)

target_link_libraries(asmim papi m)