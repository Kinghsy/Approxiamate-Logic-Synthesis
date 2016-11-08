cmake_minimum_required(VERSION 3.6)
project(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp boolean_function.cpp boolean_function.h conts.h object.h search.h tree.h test_boolean_function.cpp)
set(testing_boolean_function boolean_function.cpp boolean_function.h test_boolean_function.cpp)

add_executable(src ${SOURCE_FILES})
add_executable(testing_boolean_function ${testing_boolean_function})