cmake_minimum_required(VERSION 3.10)
project(zeroknowledge)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Viktor.cpp Viktor.h Peggy.cpp Peggy.h)
add_executable(zeroknowledge ${SOURCE_FILES})
target_link_libraries(zeroknowledge ntl)