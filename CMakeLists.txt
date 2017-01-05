cmake_minimum_required(VERSION 3.6)
project(data_structures)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -lm -Wall -Wextra -Werror -Wfloat-equal -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable")
set(SOURCE_FILES algorithms.h data_structures.h 3_6.c)

add_executable(data_structures ${SOURCE_FILES})
target_link_libraries(data_structures m)