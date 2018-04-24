find_package(Threads REQUIRED)

MACRO(multithread TARGET)
    target_link_libraries(${TARGET} ${CMAKE_THREAD_LIBS_INIT})
ENDMACRO(multithread)

find_package(OpenCV 3 REQUIRED)

MACRO(link_opencv TARGET)
    IF(OpenCV_FOUND)
        target_link_libraries(${TARGET} ${OpenCV_LIBS})
    ENDIF()
ENDMACRO(link_opencv)

find_package(cppzmq REQUIRED)

MACRO(link_cppzmq TARGET)
    include_directories(${ZeroMQ_INCLUDE_DIR} ${cppzmq_INCLUDE_DIR})
    target_link_libraries(${TARGET} ${ZeroMQ_LIBRARY} ${cppzmq_LIBRARY})
ENDMACRO(link_cppzmq)

find_package(Boost REQUIRED)

MACRO(link_boost TARGET)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${TARGET} ${Boost_LIBRARIES})
ENDMACRO(link_boost)

find_package(Protobuf REQUIRED)

file(GLOB PROTOBUF_FILELIST proto/*.proto)

include(FindProtobuf)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_BINARY_DIR}/third_party/proto)

IF(PROTOBUF_FILELIST)
    protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTOBUF_FILELIST})

    add_library(proto ${PROTO_HEADER} ${PROTO_SRC})
    set_target_properties(proto PROPERTIES LINKER_LANGUAGE CXX)

    file(GLOB PROTOBUF_SRC ${CMAKE_BINARY_DIR}/third_party/*.pb.cc)
    file(GLOB PROTOBUF_HDR ${CMAKE_BINARY_DIR}/third_party/*.pb.h)

    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/goliath/proto/goliath)

    FOREACH(PROTOBUF_SRC_FILE ${PROTOBUF_SRC})
        file(COPY ${PROTOBUF_SRC_FILE} DESTINATION ${CMAKE_SOURCE_DIR}/goliath/proto/goliath)
    ENDFOREACH()

    FOREACH(PROTOBUF_HDR_FILE ${PROTOBUF_HDR})
        file(COPY ${PROTOBUF_HDR_FILE} DESTINATION ${CMAKE_SOURCE_DIR}/goliath/proto/goliath)
    ENDFOREACH()

    file(GLOB PROTOBUF_LIB_SRC ${CMAKE_SOURCE_DIR}/goliath/proto/goliath/*.pb.cc)
    file(GLOB PROTOBUF_LIB_HDR ${CMAKE_SOURCE_DIR}/goliath/proto/goliath/*.pb.h)
ENDIF()

MACRO(link_protobuf TARGET)
    # First generate proto c++ code, this will also move code to source dir
    target_link_libraries(${TARGET} proto)

    # Create a new library from the copied over source with goliath prefix
    add_library(proto_lib ${PROTOBUF_LIB_SRC} ${PROTOBUF_LIB_HDR})
    set_target_properties(proto_lib PROPERTIES LINKER_LANGUAGE CXX)
    target_link_libraries(${TARGET} proto_lib)
    include_directories(${CMAKE_SOURCE_DIR}/goliath/proto)
ENDMACRO()