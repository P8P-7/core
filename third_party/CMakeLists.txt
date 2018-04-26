MACRO(message_version PACKAGE)
    IF(${PACKAGE}_FOUND)
        goliath_message("Found ${PACKAGE} ${${PACKAGE}_VERSION}")
    ENDIF()
ENDMACRO(message_version)

# Macro to add a find and add a specific library that doesn't need special treatment
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

IF(NOT ${Protobuf_FOUND})
    find_package(Protobuf ${PROTOBUF_MIN_VERSION} REQUIRED)
ENDIF()
message_version(Protobuf)
include_directories(${Protobuf_INCLUDE_DIRS})

file(GLOB PROTO_FILES proto/*.proto)

IF(PROTO_FILES)
    protobuf_generate_cpp(PROTO_SRC PROTO_HDR ${PROTO_FILES})
    goliath_message("Generated Protbuf files")
ENDIF()

add_library(goliath-proto ${PROTO_SRC} ${PROTO_HDR})

target_include_directories(goliath-proto
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Protobuf_INCLUDE_DIRS}
    )

target_link_libraries(goliath-proto ${Protobuf_LIBRARIES})

MACRO(link_protobuf TARGET)
    target_link_libraries(${TARGET} goliath-proto)
ENDMACRO()