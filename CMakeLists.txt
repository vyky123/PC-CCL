cmake_minimum_required(VERSION 3.18)
project(sem_prace_2 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

add_executable(sem_prace_2 main.c ccl.h pgm.c)
