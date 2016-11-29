#
# Builing of dependent libraries

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES)

list(APPEND SOURCE_FILES src/boolean_function.h
                         src/boolean_function.cpp
                         src/conts.h
                         src/object.h
                         src/search.h
                         src/tree.h
                         src/search_node.cpp)

add_subdirectory(testcase)

# The default target
add_executable(approxiamate ${SOURCE_FILES})