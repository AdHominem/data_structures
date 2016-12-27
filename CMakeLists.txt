cmake_minimum_required(VERSION 3.6)
project(data_structures)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter")
set(SOURCE_FILES data_structures.h algorithms.h 3_6.c)

add_executable(data_structures ${SOURCE_FILES})
target_link_libraries(data_structures m)