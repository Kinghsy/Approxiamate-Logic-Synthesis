add_executable(
        precomp
        main.cpp common.h
        lut3.cpp lut3.h
        lut4.cpp lut4.h
        lut5.cpp lut5.h
)

add_executable(
        precomp_rel
        gen.cpp lut3.cpp lut4.cpp lut5.cpp
)

target_compile_options(precomp BEFORE PRIVATE -Og)
target_compile_options(precomp_rel BEFORE PRIVATE -O2)