project(goliath-vision-module LANGUAGES CXX)

set(SOURCES
        detectors/detector.cpp
        detectors/line_detector.cpp
        detectors/within_box_detector.cpp
        detectors/follow_line_detector.cpp
        processors/processor.cpp
        processors/color_processor.cpp
        processors/canny_processor.cpp
        processors/roi_processor.cpp
        util/vision_error.cpp
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
