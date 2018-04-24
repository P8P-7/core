cmake_minimum_required(VERSION 3.5)
project(goliath-core CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

set(CMAKE_CXX_STANDARD  14)

add_subdirectory(third_party)
add_subdirectory(goliath)

enable_testing()

add_subdirectory(tests)