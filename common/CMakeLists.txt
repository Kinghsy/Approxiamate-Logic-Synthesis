add_executable(test_common
        common_test.cpp)

add_library(common
        truth_table.cpp truth_table.h
        resource.cpp resource.h
        )

add_executable(template_pseudo
        dummy.cpp
        memorize.h
        graph_interface.h graph_common.h
        binary_tree_impl.h
        tree_impl.h
        OrderedQueue.h
        )

target_link_libraries(test_common gtest gtest_main)