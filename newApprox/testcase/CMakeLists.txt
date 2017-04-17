link_directories("../../lib/cudd-2.5.0/obj")
link_directories("../../lib/cudd-2.5.0/cudd")
link_directories("../../lib/cudd-2.5.0/util")
link_directories("../../lib/cudd-2.5.0/st")
link_directories("../../lib/cudd-2.5.0/epd")
link_directories("../../lib/cudd-2.5.0/dddmp")
link_directories("../../lib/cudd-2.5.0/mtr")


add_executable(NewBoolFunctionTest
        bool_function_test.cpp
        ../../common/ttable.cpp ../../common/ttable.h
        ../../common/bitset_manipulate.h)
target_link_libraries(NewBoolFunctionTest gtest gtest_main)
target_link_libraries(NewBoolFunctionTest newApprox common)


add_executable(NewKmapTest
        kmap_test.cpp
        ../../common/ttable.h ../../common/ttable.cpp
        ../../common/blif_builder.h ../../common/blif_builder.cpp
        ../../common/bitset_manipulate.h)
target_link_libraries(NewKmapTest gtest gtest_main)
target_link_libraries(NewKmapTest newApprox
        mffc blif profile common
        ${Boost_LIBRARIES}
        ${CUDD_LINK_LIB})


#target_include_directories(PredecompTest BEFORE PRIVATE ../approx_common)