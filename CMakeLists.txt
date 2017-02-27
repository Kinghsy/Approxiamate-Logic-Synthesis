cmake_minimum_required(VERSION 3.6)
project(VE490)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -std=c++11")

find_package(Boost COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

include_directories(lib/GTest/googletest/include)
include_directories(common)

add_subdirectory(lib/GTest)
add_subdirectory(lib/libblif)
add_subdirectory(lib/cudd-2.5.0)

message(STATUS "CUDD library info")
message(STATUS "CUDD link libs : ${CUDD_LINK_DIR}" )
message(STATUS "CUDD libs : ${CUDD_LINK_LIB}" )

add_subdirectory(common)
add_subdirectory(approxiamate)
add_subdirectory(MFFC)
add_subdirectory(blif_replace)
add_subdirectory(circuit_profile)

add_subdirectory(policy)

add_subdirectory(driver)

add_subdirectory(util/subDecCal)


