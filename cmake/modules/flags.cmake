# WARNING_LEVEL: 1 - 4

MACRO(compiler_flags WARNING_LEVEL)
    IF (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        IF (CMAKE_COMPILER_IS_GNUCXX)
            IF (${WARNING_LEVEL} GREATER 2)
                add_compile_options("-D_FORTIFY_SOURCE=2"
                        "-D_GLIBCXX_ASSERTIONS"
                        "-fasynchronous-unwind-tables"
                        "-fexceptions"
                        "-fpic"
                        "-shared"
                        "-Og"
                        "-pipe"
                        "-Werror=format-security"
                        "-Werror=implicit-function-declaration"
                        )
            ENDIF ()
            add_compile_options("-Wall")
        ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            add_compile_options("-Wall")
        ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            IF (${WARNING_LEVEL} GREATER 2)
                add_compile_options("/Wall")
            ENDIF ()
            add_compile_options("/W${WARNING_LEVEL}")
        ENDIF ()
    ELSEIF (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        IF (CMAKE_COMPILER_IS_GNUCXX)
            add_compile_options("-O2")
        ENDIF ()
    ENDIF ()
ENDMACRO(compiler_flags)