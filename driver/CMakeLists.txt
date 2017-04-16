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

set(SOURCE_FILES
        filter_mffc.cpp precomp_sim.cpp)

add_executable(main main.cpp ${SOURCE_FILES})
add_executable(playground playground.cpp)
add_executable(verify_simulator verify_simulator.cpp)
add_executable(preDecomp_test pre_decomp.cpp)
add_executable(abc_playground abc_playground.cpp)
add_executable(mffcDriver mffc.cpp)

set(ALL_LIBS
        policy mffc blif profile libexabc libabc
        approx blif_replace preDecomp common
        ${Boost_LIBRARIES}
        ${CUDD_LINK_LIB})

target_link_libraries(main ${ALL_LIBS})
target_link_libraries(playground ${ALL_LIBS})
target_link_libraries(verify_simulator ${ALL_LIBS})
target_link_libraries(preDecomp_test ${ALL_LIBS})
target_link_libraries(abc_playground ${ALL_LIBS})
target_link_libraries(mffcDriver ${ALL_LIBS})

target_compile_options(mffcDriver BEFORE PRIVATE -O0)

#add_subdirectory(testcase)