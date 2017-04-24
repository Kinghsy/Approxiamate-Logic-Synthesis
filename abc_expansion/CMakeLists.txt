set(SOURCE
        ex_abc.cpp ex_abc.h
        )

add_library(libexabc ${SOURCE})
target_link_libraries(libexabc libabc)
target_include_directories(libexabc BEFORE
       PRIVATE "../lib/abc/"
)
target_compile_definitions(libexabc
        PRIVATE "LIN64"
        PRIVATE "SIZEOF_VOID_P=8"
        PRIVATE "SIZEOF_LONG=8"
        PRIVATE "SIZEOF_INT=4"
        PRIVATE "ABC_USE_CUDD=1"
        PRIVATE "ABC_USE_READLINE"
        PRIVATE "ABC_USE_PTHREADS"
        )

target_compile_options(libexabc BEFORE PRIVATE -O3)

set(DRIVER driver.cpp)

add_executable(exabc ${DRIVER})
target_link_libraries(exabc libexabc common)
