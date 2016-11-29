add_executable(test_common
        common_test.cpp
        data.cpp
        data.h
        dataset.h
        dataset_impl.hpp
        generator.h
        exception.h
        BinaryTree.h
        BinaryTree.hpp
        )

add_library(common
        boolean_tree.cpp
        data.cpp
        LUT2.cpp
        truth_table.cpp
        truth_table.h
        )

target_link_libraries(test_common gtest gtest_main)