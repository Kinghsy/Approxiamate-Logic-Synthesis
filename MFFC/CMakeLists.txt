#include_directories("../lib/cudd-2.5.0/cudd")
#include_directories("../lib/cudd-2.5.0/util")
#include_directories("../lib/cudd-2.5.0/cplusplus")
#include_directories("../lib/cudd-2.5.0/st")
#include_directories("../lib/cudd-2.5.0/mtr")
#include_directories("../lib/cudd-2.5.0/epd")
#include_directories("../lib/cudd-2.5.0/dddmp")
#include_directories("../lib/cudd-2.5.0/nanotrav")
#include_directories("../lib/cudd-2.5.0")
#
link_directories(${CUDD_LINK_DIR})

set(SOURCE_DRIVER
        main.cpp
        )

set(SOURCE_LIB
        interface.cpp interface.h
        err_asses.cpp err_asses.h
        net_algorithms.cpp
        pattern_gen.cpp pattern_gen.h
        )

add_library(mffc ${SOURCE_LIB}        )

add_executable(mffc_test ${SOURCE_DRIVER})

target_link_libraries(mffc_test
        mffc blif common profile
        ${CUDD_LINK_LIB})

add_subdirectory(testcases)