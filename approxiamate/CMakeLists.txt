#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Builing of dependent libraries

set(SOURCE_FILES src/boolean_function.cpp
        src/conts.h
        src/object.h
        src/search.h
        src/tree.h
        src/search_node.cpp)

#list(APPEND SOURCE_FILES src/boolean_function.h
                        # )


add_subdirectory(testcase)

# The default target
add_executable(approxiamate ${SOURCE_FILES})