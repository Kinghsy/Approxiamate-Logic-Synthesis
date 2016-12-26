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
link_directories("../lib/cudd-2.5.0/obj")
link_directories("../lib/cudd-2.5.0/cudd")
link_directories("../lib/cudd-2.5.0/util")
link_directories("../lib/cudd-2.5.0/st")
link_directories("../lib/cudd-2.5.0/epd")
link_directories("../lib/cudd-2.5.0/dddmp")
link_directories("../lib/cudd-2.5.0/mtr")

set(SOURCE_FILES
        main.cpp
        searchMFFC.cpp searchMFFC.h
        )

set(SOURCE_LIB
        basics.cpp basics.h
        bnet.cpp bnet.h
        ntr.cpp ntr.h
        cudd_build_v2.cpp cudd_build_v2.h
        )

add_library(mffc_static ${SOURCE_LIB})

add_library(mffc ${SOURCE_LIB}
        interface.cpp interface.h
        err_asses.cpp err_asses.h
        searchMFFC.cpp searchMFFC.h
        )

add_executable(mffc_test ${SOURCE_FILES})

target_link_libraries(mffc_test
        common
        mffc_static dddmp cudd mtr st util epd
        dl readline termcap pthread rt)
#add_subdirectory(testcase)