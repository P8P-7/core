project(vision-module CXX)

set(SOURCES
        webcam.cpp
        color_detector.cpp
        line_detector.cpp
        include/goliath/webcam_handle.h webcam_handle.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${OpenCV_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        core-module
    PRIVATE
        ${OpenCV_LIBRARY}
)