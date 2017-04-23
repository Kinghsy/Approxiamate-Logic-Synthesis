#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Builing of dependent libraries

set(NEW_SOURCE_FILES
        src/const.h
        src/min_set.h

        src/bool_function.h
        src/algorithm_decompose.h
        src/kmap.h
        src/decomp_small.h

        src/algorithm_decompose.cpp
        src/kmap.cpp
        src/bool_function.cpp
        src/decomp_small.cpp
    )


# list(APPEND SOURCE_FILES src/boolean_function.h #)

add_subdirectory(testcase)

# The default target
# add_executable(approxiamate ${SOURCE_FILES})

add_library(newApprox ${NEW_SOURCE_FILES})

target_compile_options(newApprox BEFORE
        PRIVATE -O0
        PRIVATE -march=native)