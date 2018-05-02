# Core

## Build steps

### Raspbian (based on Debian 9 "Stretch")
1. Install packages
   ```bash
   sudo apt-get install libtool pkg-config build-essential autoconf automake git cmake
   ```

2. Build and install [libzmq](https://github.com/zeromq/libzmq).
    ```bash
    curl -L https://github.com/zeromq/libzmq/releases/download/v4.2.5/zeromq-4.2.5.tar.gz -o zeromq-4.2.5.tar.gz
    tar -xvzf zeromq-4.2.5.tar.gz
    cd zeromq-4.2.5
    sed -i -e 's/include(ClangFormat)/\#include(ClangFormat)/g' CMakeLists.txt
    mkdir build && cd build
    cmake -D ZMQ_BUILD_TESTS=OFF ..
    sudo make -j4 install
    ```

3. Build and install [cppzmq](https://github.com/zeromq/cppzmq).
   ```bash
   git clone https://github.com/zeromq/cppzmq.git
   cd cppzmq
   mkdir build && cd build
   cmake -D CPPZMQ_BUILD_TESTS=OFF ..
   sudo make -j4 install
   ```

4. Install [protobuf](https://github.com/google/protobuf).
    ```bash
    sudo apt-get install libprotobuf-dev protobuf-compiler
    ```

5. Build and install [OpenCV](https://github.com/opencv/opencv).
    ```bash
    sudo apt-get install libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev libatlas-base-dev gfortran
    curl -L https://github.com/opencv/opencv/archive/3.4.1.zip -o opencv-3.4.1.zip
    unzip opencv-3.4.1.zip
    curl -L https://github.com/opencv/opencv_contrib/archive/3.4.1.zip -o opencv_contrib-3.4.1.zip
    unzip opencv_contrib-3.4.1.zip
    cd opencv-3.4.1
    mkdir build && cd build
    cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-3.4.1/modules \
    -D BUILD_EXAMPLES=OFF ..
    sudo make -j4 install
    sudo ldconfig
    ```

6. Install [Boost](https://github.com/boostorg/boost).
    ```bash
    sudo apt-get install libboost-dev
    ```

7. Build and install [Goliath core](https://github.com/P8P-7/core).
    ```bash
    git clone --recurse-submodules -j8 https://github.com/P8P-7/core.git
    cd core
    mkdir build && cd build
    cmake -D CMAKE_BUILD_TYPE=Release ..
    sudo make
    ```
