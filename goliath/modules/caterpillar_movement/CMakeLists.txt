project(caterpillar-movement-module CXX)

set(CATERPILLAR_MOVEMENT_SOURCE
    src/caterpillar_movement.cpp
    )

add_library(${PROJECT_NAME} SHARED ${CATERPILLAR_MOVEMENT_SOURCE})

target_link_libraries(${PROJECT_NAME} core-module)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )