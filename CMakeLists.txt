cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

IF(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type not set - defaulting to Debug")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build from: Debug Release" FORCE)
ENDIF()

project(goliath-core
        VERSION 1.0.0
        DESCRIPTION "Core of the Goliath II robot"
        LANGUAGES
            CXX
        )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

option(BUILD_DOCS "Build the doxygen docs" OFF)
option(BUILD_TESTS "Build test programs" ON)
option(BUILD_AUTO_TESTS "Build auto test programs" OFF)

include(flags)
include(protobuf)
include(test)

# Library versions
set(OPENCV_MIN_VERSION "3.4.0")
set(CPPZMQ_MIN_VERSION "4.2.3")
set(PROTOBUF_MIN_VERSOIN "3.5.1")
set(OPENAL_MIN_VERSION "1.18.0")
set(DOXYGEN_MIN_VERSION "1.8.0")
set(BOOST_MIN_VERSION "1.62.0")

find_package(OpenCV ${OPENV_MIN_VERSION} REQUIRED)
find_package(cppzmq ${CPPZMQ_MIN_VERSION} REQUIRED)
find_package(Protobuf ${PROTOBUF_MIN_VERSION} REQUIRED)
find_package(OpenAL ${OPENAL_MIN_VERSION} REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost ${BOOST_MIN_VERSION}
        COMPONENTS
            log
        REQUIRED
        )

add_subdirectory(third_party)
add_subdirectory(src)

IF(BUILD_TESTS OR BUILD_AUTO_TESTS)
    enable_testing()
ENDIF()

IF(BUILD_TESTS)
    add_subdirectory(tests)
ENDIF()

IF(BUILD_AUTO_TESTS)
    add_subdirectory(autotests)
ENDIF()

add_subdirectory(doc)
