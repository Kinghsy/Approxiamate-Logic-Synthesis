add_library(preDecomp pre_decomp.cpp)

target_compile_options(preDecomp BEFORE
        PRIVATE -O3
        PRIVATE -march=native
        )

set_property(TARGET preDecomp
        PROPERTY INTERPROCEDURAL_OPTIMIZATION True)