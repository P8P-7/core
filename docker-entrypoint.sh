#!/bin/sh

cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_DOCS=ON -DBUILD_TESTS=OFF -DBUILD_AUTO_TESTS=ON -DBOOST_ROOT="/usr/include/boost" .
make

# Only test the vision module
ctest
