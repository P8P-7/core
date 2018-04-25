include_directories(${Protobuf_INCLUDE_DIRS})

file(GLOB PROTOBUF_FILELIST proto/*.proto)

IF (PROTOBUF_FILELIST)
    protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTOBUF_FILELIST})
ENDIF ()

add_library(goliath-proto ${PROTO_HEADER} ${PROTO_SRC})

target_include_directories(goliath-proto
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${Protobuf_INCLUDE_DIRS}
)

target_link_libraries(goliath-proto ${Protobuf_LIBRARIES})