add_goliath_auto_test(vision goliath-vision-module "")
target_compile_definitions(goliath-vision-auto-test
        PRIVATE
            GOLIATH_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/autotest_vision_images/"
        )

add_goliath_auto_test(controller goliath-controller "")

add_goliath_auto_test(transitions goliath-transitions-module "")
