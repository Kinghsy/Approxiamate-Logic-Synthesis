set(CUDD_LINK_DIR
        "../lib/-2.5.0/obj"
        "../lib/cudd-2.5.0/cudd"
        "../lib/cudd-2.5.0/util"
        "../lib/cudd-2.5.0/st"
        "../lib/cudd-2.5.0/epd"
        "../lib/cudd-2.5.0/dddmp"
        "../lib/cudd-2.5.0/mtr"
        PARENT_SCOPE)

set(CUDD_LINK_LIB
        dddmp cudd mtr st util epd
        dl readline termcap pthread rt
        PARENT_SCOPE
   )