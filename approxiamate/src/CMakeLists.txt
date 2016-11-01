cmake_minimum_required(VERSION 3.6)
project(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp boolean_function.cpp boolean_function.h conts.h object.h search.h tree.h)
add_executable(src ${SOURCE_FILES})