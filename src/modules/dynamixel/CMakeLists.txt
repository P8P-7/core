project(goliath-dynamixel-module LANGUAGES CXX)

set(SOURCES
        test.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            goliath-gpio-module
            goliath-dynamixel
        )

add_executable(dynamixel-test
        test.cpp
        )

target_link_libraries(dynamixel-test
        PUBLIC
            goliath-gpio-module
            goliath-dynamixel
            ${CMAKE_THREAD_LIBS_INIT}
        )
