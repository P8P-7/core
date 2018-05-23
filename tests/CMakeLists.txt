IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_goliath_normal_test(i2c goliath-i2c-module)

    add_goliath_normal_test(vision goliath-vision-module)

    add_goliath_normal_test(openal goliath-openal-module)

    add_goliath_normal_test(motor_controller goliath-motor-controller-module)
ENDIF()

add_definitions(-DBOOST_LOG_DYN_LINK)

