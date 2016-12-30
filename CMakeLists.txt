cmake_minimum_required(VERSION 3.6)
project(data_structures)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra -Werror -Wfloat-equal -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable")
set(SOURCE_FILES algorithms.h data_structures.h 3_5.c)

add_executable(data_structures ${SOURCE_FILES})
