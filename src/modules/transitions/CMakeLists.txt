project(goliath-transitions-module LANGUAGES CXX)

add_subdirectory(include/goliath)
add_subdirectory(src)

add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
        )

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES}
    )

