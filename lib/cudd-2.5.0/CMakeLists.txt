set(CUDD_LINK_DIR
        "${CMAKE_SOURCE_DIR}/lib/-2.5.0/obj"
        "${CMAKE_SOURCE_DIR}/lib/cudd-2.5.0/cudd"
        "${CMAKE_SOURCE_DIR}/lib/cudd-2.5.0/util"
        "${CMAKE_SOURCE_DIR}/lib/cudd-2.5.0/st"
        "${CMAKE_SOURCE_DIR}/lib/cudd-2.5.0/epd"
        "${CMAKE_SOURCE_DIR}/lib/cudd-2.5.0/dddmp"
        "${CMAKE_SOURCE_DIR}/lib/cudd-2.5.0/mtr"
        PARENT_SCOPE)

set(CUDD_LINK_LIB
        dddmp cudd mtr st util epd
        dl readline termcap pthread rt
        PARENT_SCOPE
   )