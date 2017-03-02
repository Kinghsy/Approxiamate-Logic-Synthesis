link_directories(${CUDD_LINK_DIR})

add_executable(blif_info main.cpp)

target_link_libraries(blif_info
        policy mffc blif profile common
        ${Boost_LIBRARIES}
        ${CUDD_LINK_LIB})