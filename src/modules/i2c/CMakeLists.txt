project(goliath-i2c-module CXX)

set(SOURCES
        handles/i2c_slave_handle.cpp
        handles/i2c_bus_handle.cpp
        i2c_slave.cpp
        i2c_slave.cpp
        i2c_error.cpp
        )

add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        goliath-core-module
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES}
        )
