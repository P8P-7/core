cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(goliath-core CXX)

# Library versions
set(OPENCV_MIN_VERSION "3.4.0")
set(BOOST_MIN_VERSION "1.66.0")
set(CPPZMQ_MIN_VERSION "4.2.3")
set(PROTOBUF_MIN_VERSOIN "3.5.1")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

set(CMAKE_CXX_STANDARD  14)

MACRO(goliath_message MESSAGE)
    message("-- GOLIATH: " ${MESSAGE})
ENDMACRO(goliath_message)

add_subdirectory(third_party)
add_subdirectory(goliath)

enable_testing()

add_subdirectory(tests)