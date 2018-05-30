add_goliath_normal_test(i2c goliath-i2c-module)

add_goliath_normal_test(vision goliath-vision-module)

add_goliath_normal_test(openal goliath-openal-module)

add_goliath_test(motor-controller goliath-motor-controller-module test_motor_controller.cpp)

add_goliath_test(led-control goliath-led-strip-controller-module test_led_control.cpp)

add_goliath_test(config goliath-proto test_config.cpp)
target_compile_definitions(goliath-config-test PRIVATE GOLIATH_TEST_CONFIG_OUTPUT="${CMAKE_SOURCE_DIR}/src/resources/core-config.json")

add_definitions(-DBOOST_LOG_DYN_LINK)

