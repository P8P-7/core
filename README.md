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

## Cross compile for the Raspberry PI

1. Copy the entire `/usr` and `/lib` directories of your Raspberry PI into a newly created folder:
```bash
mkdir raspbian-rootfs
rsync -rl --delete-after pi@$RASPI:/{lib,usr,opt} raspbian-rootfs/
```
(where `$RASPI` is the IP address of the Raspberry)

2. Install the Raspberry PI toolchain.
```bash
./install-toolchain.sh
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

5. Upload `build/bin` to your Raspberry PI.
