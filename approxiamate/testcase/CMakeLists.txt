add_executable(GraphTest graph_test.cpp)
target_link_libraries(GraphTest gtest gtest_main)


add_executable(BooleanFunctionTest boolean_function_test.cpp ../src/boolean_function.cpp ../src/boolean_function.h)
target_link_libraries(BooleanFunctionTest gtest gtest_main)


add_executable(SearchTreeTest search_tree_test.cpp
        ../src/search.h
        ../src/boolean_function.h
        ../src/conts.h
        ../src/boolean_function.cpp
        ../src/search_node.cpp
        ../src/search_space.cpp
        ../src/search_tree.cpp)
target_link_libraries(SearchTreeTest
        gtest gtest_main)