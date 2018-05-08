#!/bin/sh

cmake -D CMAKE_BUILD_TYPE=Debug -D BOOST_ROOT="/usr/include/boost" .
make

# Omit the I2C test
ctest -E i2c