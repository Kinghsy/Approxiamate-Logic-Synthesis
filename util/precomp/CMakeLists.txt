add_executable(
        precomp
        main.cpp
        common.cpp do_lut.cpp
)

add_executable(
        precomp_rel
        gen.cpp do_lut.cpp do_lut.h common.cpp
)

set(Source_Files
        common.cpp common.h
        gen.cpp
        lut3.cpp
        lut4.cpp
        lut5.cpp
        main.cpp
        interface.h
        interface.cpp
)

add_library(precomp_lib ${Source_Files})

target_compile_options(precomp BEFORE PRIVATE -O0)
target_compile_options(precomp_rel BEFORE PRIVATE -O0)