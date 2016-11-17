#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Builing of dependent libraries

set(SOURCE_FILES
        src/main.cpp
        src/boolean_function.cpp
        src/boolean_function.h
        src/conts.h src/object.h
        src/search.h src/tree.h
        src/test_boolean_function.cpp)

add_subdirectory(testcase)

# The default target
add_executable(approxiamate ${SOURCE_FILES})