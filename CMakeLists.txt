cmake_minimum_required(VERSION 3.31)
project(turing_machine C)

set(CMAKE_C_STANDARD 23)

add_executable(turing_machine src/main.c
        src/turing.c)
