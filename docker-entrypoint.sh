#!/bin/sh

cmake -D CMAKE_BUILD_TYPE=Debug -D BOOST_ROOT="/usr/include/boost" .
make

# Only test the vision module
ctest -R vision