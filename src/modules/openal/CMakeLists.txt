project(goliath-openal-module LANGUAGES CXX)

add_subdirectory(include/goliath)
add_subdirectory(src)

add_library(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${OPENAL_INCLUDE_DIRS}
        ${CMAKE_THREAD_LIBS_INIT}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${OPENAL_LIBRARY}
    )
