########################################################
# A variant of PROTOBUF_GENERATE_CPP that keeps the directory hierarchy.
# ROOT_DIR must be absolute, and proto paths must be relative to ROOT_DIR.
FUNCTION(relative_protobuf_generate_cpp SRCS HDRS ROOT_DIR)
    IF(NOT ARGN)
        message(SEND_ERROR "Error: RELATIVE_PROTOBUF_GENERATE_CPP() called without any proto files")
        return()
    ENDIF()

    set(${SRCS})
    set(${HDRS})
    FOREACH(FIL ${ARGN})
        set(ABS_FIL ${ROOT_DIR}/${FIL})
        get_filename_component(FIL_WE ${FIL} NAME_WE)
        get_filename_component(FIL_DIR ${ABS_FIL} PATH)
        file(RELATIVE_PATH REL_DIR ${ROOT_DIR} ${FIL_DIR})

        list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${REL_DIR}/${FIL_WE}.pb.cc")
        list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${REL_DIR}/${FIL_WE}.pb.h")

        add_custom_command(
                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${REL_DIR}/${FIL_WE}.pb.cc"
                "${CMAKE_CURRENT_BINARY_DIR}/${REL_DIR}/${FIL_WE}.pb.h"
                COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
                ARGS --cpp_out ${CMAKE_CURRENT_BINARY_DIR} -I ${ROOT_DIR} ${ABS_FIL} -I ${PROTOBUF_INCLUDE_DIRS}
                DEPENDS ${ABS_FIL} proto
                COMMENT "Running C++ protocol buffer compiler on ${FIL}"
                VERBATIM)
    ENDFOREACH()

    set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
    set(${HDRS} ${${HDRS}} PARENT_SCOPE)
ENDFUNCTION(relative_protobuf_generate_cpp)