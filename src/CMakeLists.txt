add_subdirectory(io)
add_subdirectory(commands)

add_executable(publisher publisher.cpp)
target_link_libraries(publisher
    PUBLIC
        goliath-io
)

add_executable(subscriber subscriber.cpp)
target_link_libraries(subscriber
    PUBLIC
        goliath-io
)