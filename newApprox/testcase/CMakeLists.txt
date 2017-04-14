link_directories("../../lib/cudd-2.5.0/obj")
link_directories("../../lib/cudd-2.5.0/cudd")
link_directories("../../lib/cudd-2.5.0/util")
link_directories("../../lib/cudd-2.5.0/st")
link_directories("../../lib/cudd-2.5.0/epd")
link_directories("../../lib/cudd-2.5.0/dddmp")
link_directories("../../lib/cudd-2.5.0/mtr")


add_executable(NewKmapTest
        kmap_test.cpp
        ../src/kmap.h ../src/kmap.cpp
        ../src/bool_function.h ../src/bool_function.cpp
        ../../common/ttable.h ../../common/ttable.cpp)
target_link_libraries(NewKmapTest gtest gtest_main)

add_executable(NewBoolFunctionTest
        bool_function_test.cpp
        ../src/bool_function.h ../src/bool_function.cpp
        ../../common/ttable.h ../../common/ttable.cpp)
target_link_libraries(NewBoolFunctionTest gtest gtest_main)

#target_include_directories(PredecompTest BEFORE PRIVATE ../approx_common)