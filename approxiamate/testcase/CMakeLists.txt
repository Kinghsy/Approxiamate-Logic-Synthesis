add_executable(GraphTest graph_test.cpp)
target_link_libraries(GraphTest gtest gtest_main)

add_executable(BooleanFunctionTest boolean_function_test.cpp ../src/boolean_function.cpp ../src/boolean_function.h)
target_link_libraries(BooleanFunctionTest gtest gtest_main)