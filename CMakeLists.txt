cmake_minimum_required(VERSION 3.6)
project(data_structures)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter")

set(SOURCE_FILES main.c 2_5.h data_structures.h 2_3.h)
add_executable(data_structures ${SOURCE_FILES})