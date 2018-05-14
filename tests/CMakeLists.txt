IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    MACRO(add_goliath_test NAME DEPENDENCIES)
        add_executable(test_${NAME} test_${NAME}.cpp)
        add_test(NAME test_${NAME} COMMAND $<TARGET_FILE:test_${NAME}>)
        target_include_directories(test_${NAME}
                PUBLIC
                    ${DEPENDENCIES}
                    ${Boost_INCLUDE_DIR}
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
ENDIF()