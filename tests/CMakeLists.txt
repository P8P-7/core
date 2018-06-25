add_goliath_test(i2c goliath-i2c-module "" test_i2c.cpp)

add_goliath_test(vision goliath-vision-module "" test_vision.cpp)

add_goliath_test(openal goliath-openal-module ${OPENAL_INCLUDE_DIR} test_openal.cpp)

add_goliath_test(motor-controller goliath-motor-controller-module "" test_motor_controller.cpp)

add_goliath_test(led-control goliath-led-strip-controller-module "" test_led_control.cpp)

add_goliath_test(webcam goliath-vision-module "" test_webcam.cpp)

add_goliath_test(config goliath-proto "" test_config.cpp)

target_compile_definitions(goliath-config-test PRIVATE GOLIATH_TEST_CONFIG_OUTPUT="${CMAKE_SOURCE_DIR}/src/resources/core-config.json")

add_goliath_test(wing-controller "goliath-gpio-module;goliath-servo-module" "" test_wing_controller.cpp)

add_definitions(-DBOOST_LOG_DYN_LINK)