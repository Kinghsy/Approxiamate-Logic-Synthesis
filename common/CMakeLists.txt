add_executable(test_common common_test.cpp)

add_library(common
        common.h exception.h
        generator.h stopwatch.h
        resource.cpp resource.h
        bitset_manipulate.h
        urandom.cpp urandom.h
        blif_builder.cpp blif_builder.h
        ttable.cpp ttable.h static_utils.h)

target_link_libraries(test_common gtest gtest_main common)

target_compile_options(common BEFORE
        PRIVATE -march=native
        PRIVATE -Ofast)

set_property(TARGET common
        PROPERTY INTERPROCEDURAL_OPTIMIZATION True)