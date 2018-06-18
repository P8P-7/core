## Protocol Buffers
file(GLOB_RECURSE PROTOBUF_FILELIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/proto
        "${CMAKE_CURRENT_SOURCE_DIR}/proto/**/*.proto"
        "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto"
        )

IF(PROTOBUF_FILELIST)
    relative_protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
            ${CMAKE_CURRENT_SOURCE_DIR}/proto ${PROTOBUF_FILELIST})
ENDIF()

add_library(goliath-proto ${PROTO_SRCS} ${PROTO_HDRS})

target_include_directories(goliath-proto
        PUBLIC
            ${CMAKE_CURRENT_BINARY_DIR}
            ${Protobuf_INCLUDE_DIRS}
        )

target_link_libraries(goliath-proto ${PROTOBUF_LIBRARIES})

## libdynamixel
add_subdirectory(lib-dynamixel)
