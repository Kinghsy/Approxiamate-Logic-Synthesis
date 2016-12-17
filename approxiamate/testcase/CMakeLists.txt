link_directories("../../lib/cudd-2.5.0/obj")
link_directories("../../lib/cudd-2.5.0/cudd")
link_directories("../../lib/cudd-2.5.0/util")
link_directories("../../lib/cudd-2.5.0/st")
link_directories("../../lib/cudd-2.5.0/epd")
link_directories("../../lib/cudd-2.5.0/dddmp")
link_directories("../../lib/cudd-2.5.0/mtr")


add_executable(GraphTest graph_test.cpp)
target_link_libraries(GraphTest gtest gtest_main)

add_executable(BooleanFunctionTest
        boolean_function_test.cpp
        ../src/boolean_function.cpp ../src/boolean_function.h)
target_link_libraries(BooleanFunctionTest
        gtest gtest_main)


add_executable(SearchTreeTest search_tree_test.cpp
        ../src/search.h
        ../src/boolean_function.h
        ../src/conts.h
        ../../common/truth_table.h
        ../src/boolean_function.cpp
        ../src/search_node.cpp
        ../src/search_space.cpp
        ../src/search_tree.cpp
        ../../common/truth_table.cpp)
target_link_libraries(SearchTreeTest
        gtest gtest_main)




add_executable(SearchMethodCoreTest search_method_core_test.cpp)
target_link_libraries(SearchMethodCoreTest
        gtest gtest_main)
target_link_libraries(SearchMethodCoreTest
        mffc approx blif_replace common
        ${Boost_LIBRARIES}
        dddmp cudd mtr st util epd
        dl readline termcap pthread rt
        readline termcap pthread)