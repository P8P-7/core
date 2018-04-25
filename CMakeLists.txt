cmake_minimum_required(VERSION 3.5)
project(goliath-core CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(cppzmq REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED)

add_subdirectory(third_party)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)