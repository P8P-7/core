project(goliath-gpio-module LANGUAGES CXX)

set(SOURCES
        gpio.cpp
        )

find_package(Boost
        COMPONENTS
            log
        REQUIRED
        )

add_library(${PROJECT_NAME} ${SOURCES})

find_package(Boost ${BOOST_MIN_VERSION}
        COMPONENTS
        log
        REQUIRED
        )
add_definitions(-DBOOST_LOG_DYN_LINK)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            ${Boost_LIBRARIES}
        )