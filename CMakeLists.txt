cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(goliath-core CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

include(flags)
include(protobuf)

# Library versions
set(OPENCV_MIN_VERSION "3.4.0")
set(BOOST_MIN_VERSION "1.64.0")
set(CPPZMQ_MIN_VERSION "4.2.3")
set(PROTOBUF_MIN_VERSOIN "3.5.1")

find_package(OpenCV ${OPENV_MIN_VERSION} REQUIRED)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
find_package(cppzmq ${CPPZMQ_MIN_VERSION} REQUIRED)
find_package(Protobuf ${PROTOBUF_MIN_VERSION} REQUIRED)

add_subdirectory(third_party)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)