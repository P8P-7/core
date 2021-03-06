project(goliath-controller LANGUAGES CXX)

add_subdirectory(include/goliath)
add_subdirectory(src)

add_library(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

add_definitions(-DBOOST_LOG_DYN_LINK)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            goliath-foundation-module
            goliath-zmq-messaging-module
            goliath-gpio-module
            goliath-servo-module
            goliath-vision-module
            goliath-emotions-module
            goliath-motor-controller-module
            goliath-led-strip-controller-module
            goliath-i2c-module
            goliath-led-strip-controller-module
            goliath-transitions-module
            ${Boost_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
        )

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${Boost_INCLUDE_DIR}
        )
