add_library(profile
        profile.h profile.cpp
        sim_profile.h sim_profile.cpp)

target_compile_options(profile BEFORE
        PRIVATE -march=native
        PRIVATE -O0)

