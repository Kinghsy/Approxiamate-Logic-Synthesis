add_executable(test_common
        common_test.cpp
        ttable.cpp
        urandom.cpp
        resource.cpp
        blif_builder.cpp)

add_library(common
        common.h exception.h
        generator.h stopwatch.h
        resource.cpp resource.h
        bitset_manipulate.h
        urandom.cpp urandom.h
        blif_builder.cpp blif_builder.h
        ttable.cpp ttable.h)

target_link_libraries(test_common gtest gtest_main)
