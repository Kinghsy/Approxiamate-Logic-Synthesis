add_library(preDecomp pre_decomp.cpp)

target_compile_options(preDecomp BEFORE
        PRIVATE -Ofast
        PRIVATE -march=native
        )