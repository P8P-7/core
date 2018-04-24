MACRO(set_test TEST_EXEC TEST_SRC TARGETS)
    add_executable(${TEST_EXEC} ${TEST_SRC})
    add_test(${TEST_EXEC} ${TEST_EXEC})
    target_link_libraries(${TEST_EXEC} ${TARGETS})
    link_boost(${TEST_EXEC})
ENDMACRO(set_test)

set(TEST_TEST_SRC
    test_test.cpp
    )

set(TEST_TEST_TARGETS
    caterpillar-movement-module
    )

set_test(test_test ${TEST_TEST_SRC} ${TEST_TEST_TARGETS})