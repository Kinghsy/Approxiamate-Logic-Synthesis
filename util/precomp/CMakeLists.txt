add_executable(
        precomp
        main.cpp common.h
        lut3.cpp lut3.h
        lut4.cpp lut4.h
        lut5.cpp lut5.h
        lut6.cpp lut6.h
)

add_executable(
        precomp_rel
        gen.cpp lut3.cpp lut4.cpp lut5.cpp lut6.cpp
)

set(Source_Files
        common.h
        gen.cpp
        lut3.cpp
        lut4.cpp
        lut5.cpp
        main.cpp
        interface.h
        interface.cpp
)

add_library(precomp_lib ${Source_Files})

target_compile_options(precomp BEFORE PRIVATE -Og)
target_compile_options(precomp_rel BEFORE PRIVATE -O0)