cmake_minimum_required(VERSION 3.24)
project(asmim)

set(CMAKE_C_STANDARD 11)
#set(CMAKE_CXX_FLAGS  "-O0")

add_executable(asmim
        lab4/plain/plain.h lab4/utils.c lab4/utils.h lab4/plain/plain1.c lab4/plain/plain_std.c lab4/test/test.h lab4/test/teststd.c lab4/test/testv.c lab4/test/test.c lab4/main.c lab4/plain/data_type.h lab4/plain/data_type_double.h lab4/plain/data_type_int.h lab4/plain/plain2.c lab4/plain/plain4.c)

target_link_libraries(asmim papi m)