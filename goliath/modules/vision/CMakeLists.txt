project(vision-module CXX)

set(VISION_MODULE_SRC
    src/webcam.cpp
    src/color_detector.cpp
    src/line_detector.cpp
    )

add_library(${PROJECT_NAME} ${VISION_MODULE_SRC})

link_opencv(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
