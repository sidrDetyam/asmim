cmake_minimum_required(VERSION 3.24)
project(asmim)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS  "-O0")

add_executable(asmim
        lab3/lab3.cpp)

target_link_libraries(asmim papi m)