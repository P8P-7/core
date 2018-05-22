project(goliath-servo-module LANGUAGES CXX)

set(SOURCES
        servo_handle.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            goliath-proto
            goliath-dynamixel
            goliath-gpio-module
            goliath-foundation-module
        PRIVATE
            ${Protobuf_LIBRARY}
            ${CMAKE_THREAD_LIBS_INIT}
            ${Boost_LIBRARIES}
        )

add_executable(servo_test
        test.cpp)

target_link_libraries(servo_test
        PUBLIC
            ${PROJECT_NAME}
        )