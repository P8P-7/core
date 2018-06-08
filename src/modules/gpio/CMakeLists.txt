project(goliath-gpio-module LANGUAGES CXX)

add_subdirectory(include/goliath)
add_subdirectory(src)

add_library(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

add_definitions(-DBOOST_LOG_DYN_LINK)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${Boost_INCLUDE_DIR}
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            ${Boost_LIBRARIES}
        )
