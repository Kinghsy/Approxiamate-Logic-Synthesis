add_executable(test_common
        common_test.cpp
        data.cpp
        data.h
        dataset.h
        dataset_impl.hpp
        generator.h
        exception.h)

add_library(common data.cpp)

target_link_libraries(test_common gtest gtest_main)