add_executable(test_common
        common_test.cpp)

add_library(common
        truth_table.cpp truth_table.h
        )

add_executable(template_pseudo
        graph_interface.h graph_common.h)

target_link_libraries(test_common gtest gtest_main)