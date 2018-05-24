add_subdirectory(controller)
add_subdirectory(modules)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
        goliath-controller
    )

add_configuration(resources/core-config.json)

add_definitions(-DBOOST_LOG_DYN_LINK)
