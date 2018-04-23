# WARNING_LEVEL: 1 - 4

MACRO(compiler_flags TARGET WARNING_LEVEL)
    IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        IF("${CMAKE_CXX_COMPILER}" STREQUAL "GNU")
            IF(${WARNING_LEVEL} > 2)
                target_compile_options(${TARGET}
                    "-D_FORTIFY_SOURCE=2"
                    "-D_GLIBCXX_ASSERTIONS"
                    "-fasynchronous-unwind-tables"
                    "-fexceptions"
                    "-fpic -shared"
                    "-fstack-clash-protection"
                    "-fstack-protector-strong"
                    "-g"
                    "-O2"
                    "-pipe"
                    "-Werror=format-security"
                    "-Werror=implicit-function-declaration"
                    )
            ENDIF()
            target_compile_options(${TARGET} "-Wall")
        ELSEIF("${CMAKE_CXX_COMPILER}" STREQUAL "Clang")
            target_compile_options(${TARGET} "-Wall")
        ELSEIF("${CMAKE_CXX_COMPILER}" STREQUAL "MSVC")
            IF(${WARNING_LEVEL} > 2)
                target_compile_options(${TARGET} "/Wall")
            ENDIF()
            target_compile_options(${TARGET} "/W${WARNING_LEVEL}")
        ENDIF()
    ELSEIF(${CMAKE_BUILD_TYPE} STREQUAL "Relase")
        IF("$(CMAKE_CXX_COMPILER}" STREQUAL "GNU")
            target_compile_options(${TARGET} "-O1")
        ENDIF()
    ENDIF()
ENDMACRO(compiler_flags)