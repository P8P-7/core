# Core

[![Build Status](https://travis-ci.org/P8P-7/core.svg?branch=master)](https://travis-ci.org/P8P-7/core)

## Overview
_Goliath II_'s heart. Contains all actions, commands and handles. If a command is found, it will be executed within the core.

## Build steps

### Raspbian (based on Debian 9 "Stretch")
```bash
git clone --recurse-submodules -j8 https://github.com/P8P-7/core.git
cd core
sudo ./preinstall.sh -j4
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
sudo make
```
