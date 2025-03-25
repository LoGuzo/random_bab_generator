cmake_minimum_required(VERSION 3.10)

project(RANDOM_BOB VERSION 1.0)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/../src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../inc)

add_executable(Random_Bob ${SRC_DIR}/main.c)

target_include_directories(Random_Bob PRIVATE ${INCLUDE_DIR})
