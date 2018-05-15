include_directories(${Boost_INCLUDE_DIR})

find_package(Boost ${BOOST_MIN_VERSION}
        COMPONENTS
        log
        REQUIRED
        )
add_definitions(-DBOOST_LOG_DYN_LINK)

add_executable(test_test test_test.cpp)
add_test(test_test test_test)
target_link_libraries(test_test
        ${Boost_LIBRARY}
        ${Boost_LIBS}
    )

add_executable(test_i2c test_i2c.cpp)
add_test(test_i2c test_i2c)
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

add_executable(test_motor_controller test_motor_controller.cpp)
add_test(test_motor_controller test_motor_controller)
target_include_directories(test_motor_controller
        PUBLIC
        goliath-motor-controller-module
        )
target_link_libraries(test_motor_controller
        PUBLIC
        goliath-motor-controller-module
        ${Boost_LIBRARY}
        ${Boost_LIBS}
        )