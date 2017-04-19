link_directories("../../lib/cudd-2.5.0/obj")
link_directories("../../lib/cudd-2.5.0/cudd")
link_directories("../../lib/cudd-2.5.0/util")
link_directories("../../lib/cudd-2.5.0/st")
link_directories("../../lib/cudd-2.5.0/epd")
link_directories("../../lib/cudd-2.5.0/dddmp")
link_directories("../../lib/cudd-2.5.0/mtr")

add_executable(NewBoolFunctionTest bool_function_test.cpp)
target_link_libraries(NewBoolFunctionTest gtest gtest_main)
target_link_libraries(NewBoolFunctionTest newApprox common)


add_executable(NewKmapTest kmap_test.cpp
        ../../driver/filter_mffc.cpp)
target_link_libraries(NewKmapTest gtest gtest_main)
target_link_libraries(NewKmapTest newApprox
        profile mffc blif common
        ${Boost_LIBRARIES}
        ${CUDD_LINK_LIB})

add_executable(NewAlgorithmDecomposeTest algorithm_decompose_test.cpp
        ../../driver/filter_mffc.cpp)
target_link_libraries(NewAlgorithmDecomposeTest gtest gtest_main)
target_link_libraries(NewAlgorithmDecomposeTest newApprox
        profile mffc blif common
        ${Boost_LIBRARIES}
        ${CUDD_LINK_LIB})



#target_include_directories(PredecompTest BEFORE PRIVATE ../approx_common)