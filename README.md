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

## Cross compile for the Raspberry Pi

1. Copy the entire `/lib`, `/usr` and `/opt` directories of your Raspberry Pi into a newly created folder:
```bash
mkdir raspbian-rootfs
rsync -vrl --delete-after pi@$RASPI:/{lib,usr,opt} raspbian-rootfs/
```
(where `$RASPI` is the IP address of the Raspberry Pi)

2. Install the Raspberry Pi toolchain.
```bash
# For Raspberry PI 3B
sudo ./install-toolchain.sh --arch=armv7

# For Raspberry PI 3B+
sudo ./install-toolchain.sh --arch=armv8
```

3. Turn all the absolute symlinks into relative ones:
```bash
wget https://raw.githubusercontent.com/riscv/riscv-poky/master/scripts/sysroot-relativelinks.py
chmod +x sysroot-relativelinks.py
./sysroot-relativelinks.py raspbian-rootfs/
```

4. Let's cross compile!
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="../toolchains/toolchain-rpi.cmake" ..
make
```

5. Upload `build/bin` to your Raspberry Pi.
