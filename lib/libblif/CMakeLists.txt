set(SOURCE_LIB
        bnet.cpp bnet.h
        ntr.cpp ntr.h
        cudd_build_v2.cpp cudd_build_v2.h
        )

add_library(blif ${SOURCE_LIB})

target_compile_options(blif BEFORE
        PRIVATE -O3
)