include_directories(${Boost_INCLUDE_DIR})

add_executable(test_test test_test.cpp)
add_test(NAME test_test COMMAND $<TARGET_FILE:test_test>)
target_link_libraries(test_test
        ${Boost_LIBRARY}
        ${Boost_LIBS}
    )

add_executable(test_i2c test_i2c.cpp)
add_test(NAME test_i2c COMMAND $<TARGET_FILE:test_i2c>)
target_include_directories(test_i2c
        PUBLIC
        goliath-i2c-module
        )
target_link_libraries(test_i2c
        PUBLIC
        goliath-i2c-module
        ${Boost_LIBRARY}
        ${Boost_LIBS}
        )