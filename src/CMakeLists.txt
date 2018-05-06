add_subdirectory(controller)
add_subdirectory(modules)

add_executable(publisher publisher.cpp)
target_link_libraries(publisher
        PUBLIC
            io-module
        )