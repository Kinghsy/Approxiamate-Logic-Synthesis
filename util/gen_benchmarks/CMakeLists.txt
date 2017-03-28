
link_directories(${CUDD_LINK_DIR})

add_executable(gen_benchmarks main.cpp)

target_link_libraries(gen_benchmarks
        policy mffc blif profile
        ${CUDD_LINK_LIB}
        libexabc libabc common
        ${Boost_LIBRARIES})