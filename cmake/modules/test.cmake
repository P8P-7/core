MACRO(add_goliath_test NAME DEPENDENCIES FILE)
    add_executable(goliath-${NAME}-test ${FILE})
    target_include_directories(goliath-${NAME}-test
        PUBLIC
            ${DEPENDENCIES}
            ${Boost_INCLUDE_DIR}
        )
    target_link_libraries(goliath-${NAME}-test
        PUBLIC
            ${DEPENDENCIES}
            ${Boost_LIBRARY}
            ${Boost_LIBS}
        )
ENDMACRO()

MACRO(add_goliath_normal_test NAME DEPENDENCIES)
    add_goliath_test(${NAME} ${DEPENDENCIES} test_${NAME}.cpp)
ENDMACRO()

MACRO(add_goliath_auto_test NAME DEPENDENCIES)
    add_goliath_test(${NAME}-auto ${DEPENDENCIES} autotest_${NAME}.cpp)
    add_test(NAME goliath-${NAME}-auto-test COMMAND $<TARGET_FILE:goliath-${NAME}-auto-test>)
ENDMACRO()
