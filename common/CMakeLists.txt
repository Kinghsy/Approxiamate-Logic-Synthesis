add_executable(test_common
        common_test.cpp)

add_library(common
        resource.cpp resource.h bitset_manipulate.h
        urandom.cpp urandom.h stopwatch.h)

add_executable(template_pseudo
        dummy.cpp
        memorize.h
        binary_tree_impl.h
        )

target_link_libraries(test_common gtest gtest_main)