cmake_minimum_required(VERSION 3.10)
project(Random_Bob VERSION 1.0)
add_executable(Ran_Bob ../inc/header.h ../inc/shuffle.h ../src/shuffle.c
                       ../src/main.c)
