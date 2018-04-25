MACRO(add_simple_library TARGET PACKAGE)
    IF (NOT ${PACKAGE}_FOUND)
        find_package(${PACKAGE} REQUIRED)
        include_directories(${${PACKAGE}_INCLUDE_DIR})
    ENDIF ()

    target_link_libraries(${TARGET} ${${PACKAGE}_LIBRARY} ${${PACKAGE}_LIBS})
ENDMACRO()

MACRO(multithread TARGET)
    find_package(Threads REQUIRED)
    target_link_libraries(${TARGET} ${CMAKE_THREAD_LIBS_INIT})
ENDMACRO(multithread)

MACRO(link_opencv TARGET)
    add_simple_library(${TARGET} OpenCV)
ENDMACRO(link_opencv)

MACRO(link_cppzmq TARGET)
    find_package(cppzmq REQUIRED)

    IF (cppzmq_FOUND)
        include_directories(${ZeroMQ_INCLUDE_DIR} ${cppzmq_INCLUDE_DIR})
        target_link_libraries(${TARGET} ${cppzmq_LIBRARY})
    ENDIF ()
ENDMACRO(link_cppzmq)

MACRO(link_boost TARGET)
    add_simple_library(${TARGET} Boost)
ENDMACRO(link_boost)

find_package(Protobuf REQUIRED)

file(GLOB PROTOBUF_FILELIST ${CMAKE_BINARY_DIR}/third_party/proto/*.proto)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_BINARY_DIR}/third_party/proto)

IF (PROTOBUF_FILELIST)
    protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTOBUF_FILELIST})

    add_library(proto ${PROTO_HEADER} ${PROTO_SRC})
    set_target_properties(proto PROPERTIES LINKER_LANGUAGE CXX)

    file(GLOB PROTOBUF_SRC ${CMAKE_BINARY_DIR}/third_party/*.pb.cc)
    file(GLOB PROTOBUF_HDR ${CMAKE_BINARY_DIR}/third_party/*.pb.h)

    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/goliath/proto/goliath)

    FOREACH (PROTOBUF_SRC_FILE ${PROTOBUF_SRC})
        file(COPY ${PROTOBUF_SRC_FILE} DESTINATION ${CMAKE_SOURCE_DIR}/goliath/proto/goliath)
    ENDFOREACH ()

    FOREACH (PROTOBUF_HDR_FILE ${PROTOBUF_HDR})
        file(COPY ${PROTOBUF_HDR_FILE} DESTINATION ${CMAKE_SOURCE_DIR}/goliath/proto/goliath)
    ENDFOREACH ()

    file(GLOB PROTOBUF_LIB_SRC ${CMAKE_SOURCE_DIR}/goliath/proto/goliath/*.pb.cc)
    file(GLOB PROTOBUF_LIB_HDR ${CMAKE_SOURCE_DIR}/goliath/proto/goliath/*.pb.h)
ENDIF ()

MACRO(link_protobuf TARGET)
    find_package(Protobuf REQUIRED)

    # First generate proto c++ code, this will also move code to source dir
    target_link_libraries(${TARGET} proto)

    # Create a new library from the copied over source with goliath prefix
    add_library(proto_lib ${PROTOBUF_LIB_SRC} ${PROTOBUF_LIB_HDR})
    set_target_properties(proto_lib PROPERTIES LINKER_LANGUAGE CXX)
    target_link_libraries(${TARGET} proto_lib ${Protobuf_LIBRARY})
    include_directories(${CMAKE_SOURCE_DIR}/goliath/proto)
ENDMACRO()