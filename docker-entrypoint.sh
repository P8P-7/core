#!/bin/sh

cmake -D CMAKE_BUILD_TYPE=Release -D BOOST_ROOT="/usr/include/boost" .
make

# Only test the vision module
# TODO Enable / fix vision test
# ctest -R vision