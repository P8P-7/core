project(goliath-vision-module LANGUAGES CXX)

set(SOURCES
        processors/processor.cpp
        processors/color_processor.cpp
        processors/line_processor.cpp
        detectors/detector.cpp
        detectors/line_detector.cpp
        webcam_handle.cpp
        webcam.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
            ${OpenCV_INCLUDE_DIR}
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            goliath-core-module
        PRIVATE
            ${OpenCV_LIBRARIES}
        )