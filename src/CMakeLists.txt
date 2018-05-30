add_subdirectory(controller)
add_subdirectory(modules)

add_executable(${CMAKE_PROJECT_NAME} core.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}
        PUBLIC
            goliath-controller
        )

add_definitions(-DBOOST_LOG_DYN_LINK)

add_configuration(resources/core-config.json)

configure_file(resources/core-text.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/core-text.txt COPYONLY)
