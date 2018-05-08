include_directories(${Boost_INCLUDE_DIR})

MACRO(add_goliath_test NAME DEPENDENCIES)
    add_executable(test_${NAME} test_${NAME}.cpp)
    add_test(test_${NAME} test_${NAME})
    target_include_directories(test_${NAME}
            PUBLIC
                ${DEPENDENCIES}
            )
    target_link_libraries(test_${NAME}
            PUBLIC
                ${DEPENDENCIES}
                ${Boost_LIBRARY}
                ${Boost_LIBS}
            )
ENDMACRO()

add_goliath_test(i2c goliath-i2c-module)

add_goliath_test(vision goliath-vision-module)
target_compile_definitions(test_vision PRIVATE GOLIATH_TEST_IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test_vision_images/")