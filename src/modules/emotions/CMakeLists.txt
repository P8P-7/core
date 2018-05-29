project(goliath-emotions-module LANGUAGES CXX)

set(SOURCES
        emotion_publisher.cpp
        emotion_handle.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            goliath-proto
            goliath-zmq-messaging-module
        )
