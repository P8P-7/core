project(goliath-motor-controller-module LANGUAGES CXX)

set(SOURCES
        include/goliath/motor_controller.h
        motor_controller.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES})

add_definitions(-DBOOST_LOG_DYN_LINK)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            goliath-i2c-module
            ${Boost_LIBRARIES}
        )
