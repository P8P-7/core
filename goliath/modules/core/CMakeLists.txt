project(core-module CXX)

set(CORE_MODULE_INCLUDES
    include/goliath/movement_service.h
    )

add_library(${PROJECT_NAME} SHARED
    ${CORE_MODULE_INCLUDES}
    )

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINKER_LANGUAGE CXX
    )

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)