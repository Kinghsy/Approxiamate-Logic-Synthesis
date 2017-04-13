#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Builing of dependent libraries

set(NEW_SOURCE_FILES
        src/boolean_function.h
        src/algorithm_decompose.h
        src/decomposition_info.h
        src/kmap.h
        src/ttable.h
        src/const.h

        src/algorithm_decompose.cpp
        src/kmap.cpp
        src/boolean_function.cpp
        src/ttable.cpp
    )


#list(APPEND SOURCE_FILES src/boolean_function.h
                        # )


#add_subdirectory(testcase)

# The default target
#add_executable(approxiamate ${SOURCE_FILES})

add_library(newApprox ${NEW_SOURCE_FILES})