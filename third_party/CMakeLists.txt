MACRO(message_version PACKAGE)
    IF(${PACKAGE}_FOUND)
        goliath_message("Found ${PACKAGE} ${${PACKAGE}_VERSION}")
    ENDIF()
ENDMACRO(message_version)

MACRO(add_simple_library TARGET PACKAGE VERSION)
    IF(NOT ${PACKAGE}_FOUND)
        find_package(${PACKAGE} ${VERSION} REQUIRED)
        include_directories(${${PACKAGE}_INCLUDE_DIR})
    ENDIF()

    message_version(${PACKAGE})
    target_link_libraries(${TARGET} ${${PACKAGE}_LIBRARY} ${${PACKAGE}_LIBS})
ENDMACRO()

MACRO(multithread TARGET)
    find_package(Threads REQUIRED)
    target_link_libraries(${TARGET} ${CMAKE_THREAD_LIBS_INIT})
ENDMACRO(multithread)

MACRO(link_opencv TARGET)
    add_simple_library(${TARGET} OpenCV ${OPENCV_MIN_VERSION})
ENDMACRO(link_opencv)

MACRO(link_cppzmq TARGET)
    find_package(cppzmq ${CPPZMQ_MIN_VERSION} REQUIRED)

    IF(cppzmq_FOUND)
        message_version(cppzmq)

        include_directories(${ZeroMQ_INCLUDE_DIR} ${cppzmq_INCLUDE_DIR})
        target_link_libraries(${TARGET} ${cppzmq_LIBRARY})
    ENDIF()
ENDMACRO(link_cppzmq)

MACRO(link_boost TARGET)
    add_simple_library(${TARGET} Boost ${BOOST_MIN_VERSION})
ENDMACRO(link_boost)

find_package(Protobuf ${PROTOBUF_MIN_VERSION} REQUIRED)
message_version(Protobuf)
include_directories(${Protobuf_INCLUDE_DIRS})

file(GLOB PROTOBUF_FILELIST proto/*.proto)

set(PROTOBUF_FILENAMES "")

FOREACH(PROTOBUF_FILE ${PROTOBUF_FILELIST})
    get_filename_component(PROTOBUF_FILENAME ${PROTOBUF_FILE} NAME)
    list(APPEND PROTOBUF_FILENAMES ${PROTOBUF_FILENAME})
ENDFOREACH()

execute_process(
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND protoc -I=third_party/proto --cpp_out=goliath/proto/goliath ${PROTOBUF_FILENAMES}
    )

MACRO(link_protobuf TARGET)
    file(GLOB PROTOUT_SRC ${CMAKE_SOURCE_DIR}/goliath/proto/goliath/*.pb.cc)
    file(GLOB PROTOUT_HDR ${CMAKE_SOURCE_DIR}/goliath/proto/goliath/*.pb.h)

    include_directories(${Protobuf_INCLUDE_DIRS})
    include_directories(${CMAKE_SOURCE_DIR}/goliath/proto)
    add_library(proto ${PROTOUT_SRC} ${PROTOUT_HDR})
ENDMACRO()