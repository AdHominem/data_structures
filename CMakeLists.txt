cmake_minimum_required(VERSION 3.6)
project(data_structures)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -lm -fstack-protector-all -Wall -Wextra -Werror -Wfloat-equal -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable")
set(SOURCE_FILES 4_1.c 4_1.h 4_1_functions.h 4_1_helper.h 4_1_helper.c 4_1_functions.c)

add_executable(data_structures ${SOURCE_FILES})
target_link_libraries(data_structures m)