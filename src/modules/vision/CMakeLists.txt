project(goliath-vision-module LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(include/goliath)

add_library(${PROJECT_NAME} ${INCLUDES} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
            ${OpenCV_INCLUDE_DIR}
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            goliath-foundation-module
        PRIVATE
            ${OpenCV_LIBRARIES}
        )
