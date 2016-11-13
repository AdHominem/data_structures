cmake_minimum_required(VERSION 3.6)
project(data_structures)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter")
set(SOURCE_FILES 2_3.c input.h data_structures.h)

add_executable(data_structures ${SOURCE_FILES})