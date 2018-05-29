project(goliath-i2c-module)

add_subdirectory(include/goliath)
add_subdirectory(src)

add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            goliath-foundation-module
            ${CMAKE_THREAD_LIBS_INIT}
            ${Boost_LIBRARIES}
        )
