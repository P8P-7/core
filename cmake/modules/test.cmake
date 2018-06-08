MACRO(add_goliath_test NAME LIBRARIES INCLUDES FILE)
    add_executable(goliath-${NAME}-test ${FILE})
    target_link_libraries(goliath-${NAME}-test
            PUBLIC
                ${LIBRARIES}
                ${Boost_LIBRARIES}
            )
    target_include_directories(goliath-${NAME}-test
        PUBLIC
            ${INCLUDES}
            ${Boost_INCLUDE_DIR}
        )
ENDMACRO()

MACRO(add_goliath_auto_test NAME LIBRARIES INCLUDES)
    add_goliath_test(${NAME}-auto ${LIBRARIES} INCLUDES autotest_${NAME}.cpp)
    add_test(NAME goliath-${NAME}-auto-test COMMAND $<TARGET_FILE:goliath-${NAME}-auto-test>)
ENDMACRO()
