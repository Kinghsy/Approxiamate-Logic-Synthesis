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
        main.cpp)

add_executable(main ${SOURCE_FILES})

target_link_libraries(main
        mffc approx blif_replace common
        ${Boost_LIBRARIES}
        dddmp cudd mtr st util epd
        dl readline termcap pthread rt
        readline termcap pthread)
#add_subdirectory(testcase)