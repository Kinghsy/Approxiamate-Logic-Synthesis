cmake_minimum_required(VERSION 3.6)
project(VE490)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native -std=c++11")

include_directories(lib/GTest/googletest/include)

include_directories("lib/cudd-3.0.0/include")
link_directories("lib/cudd-3.0.0/cplusplus/.libs")
link_directories("lib/cudd-3.0.0/cudd/.libs")


include_directories(common)

add_subdirectory(lib/GTest)

add_subdirectory(common)

add_subdirectory(approxiamate)