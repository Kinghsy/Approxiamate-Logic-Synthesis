#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Builing of dependent libraries

set(SOURCE_FILES
        src/bool_function.h
        src/kmap.h
        src/node.h
        src/proce.h)


#list(APPEND SOURCE_FILES src/boolean_function.h
                        # )


#add_subdirectory(testcase)

# The default target
#add_executable(approxiamate ${SOURCE_FILES})

add_library(newApprox ${SOURCE_FILES})