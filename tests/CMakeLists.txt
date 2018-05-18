IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    MACRO(add_goliath_test NAME DEPENDENCIES)
        add_executable(goliath-${NAME}-test test_${NAME}.cpp)
        add_test(NAME goliath-${NAME}-test COMMAND $<TARGET_FILE:goliath-${NAME}-test>)
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

    add_goliath_test(i2c goliath-i2c-module)

    add_goliath_test(vision goliath-vision-module)
    target_compile_definitions(goliath-vision-test PRIVATE GOLIATH_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test_vision_images/")

    add_goliath_test(openal goliath-openal-module)

ENDIF()

find_package(Boost ${BOOST_MIN_VERSION}
        COMPONENTS
        log
        REQUIRED
        )
add_definitions(-DBOOST_LOG_DYN_LINK)

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