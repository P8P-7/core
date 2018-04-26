set(SOURCES
        include/goliath/vision.h
        vision.cpp)

add_library(goliath-vision ${SOURCES})

target_include_directories(goliath-vision
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${OpenCV_INCLUDE_DIR}
)

target_link_libraries(goliath-vision
    PRIVATE
        ${OpenCV_LIBRARY}
)