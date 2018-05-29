project(goliath-motor-controller-module LANGUAGES CXX)

set(SOURCES
        motor_controller.cpp
        motor_handle.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES})

add_definitions(-DBOOST_LOG_DYN_LINK)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            goliath-foundation-module
            goliath-i2c-module
            ${Boost_LIBRARIES}
        )
