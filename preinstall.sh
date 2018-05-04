#!/bin/bash

cmake_version_latest=3.11.1
cmake_version_latest_major_minor=3.11
cmake_version_latest_patch=1

libzmq_version_latest=4.2.5

opencv_version_latest=3.4.1

protobuf_version_latest=3.5.2

boost_root="/usr/local"
boost_version_latest=1.67.0
boost_lib_version_latest="1_67"

make_flags="-j4"

if [ ! -z "$1" ]; then
  make_flags="$@"
fi

install_cmake_from_source() {
  echo "Compiling CMake $cmake_version_latest_major_minor.$cmake_version_latest_patch from source"
  curl -O -L https://cmake.org/files/v$cmake_version_latest_major_minor/cmake-$cmake_version_latest_major_minor.$cmake_version_latest_patch.tar.gz
  tar -xvzf cmake-$cmake_version_latest_major_minor.$cmake_version_latest_patch.tar.gz
  cd cmake-$cmake_version_latest_major_minor.$cmake_version_latest_patch
  ./bootstrap
  make $make_flags install
  cd ..
  rm -rf cmake-$cmake_version_latest_major_minor.$cmake_version_latest_patch
  rm cmake-$cmake_version_latest_major_minor.$cmake_version_latest_patch.tar.gz
  ldconfig
  echo "Installed CMake $cmake_version_latest_major_minor.$cmake_version_latest_patch"
}

install_libzmq_from_source() {
  echo "Compiling ZeroMQ $libzmq_version_latest from source"
  curl -O -L https://github.com/zeromq/libzmq/releases/download/v$libzmq_version_latest/zeromq-$libzmq_version_latest.tar.gz
  tar -xvzf zeromq-$libzmq_version_latest.tar.gz
  cd zeromq-$libzmq_version_latest
  # zeromq-4.2.5 doesn't include files required to include ClangFormat
  sed -i -e 's/include(ClangFormat)/\#include(ClangFormat)/g' CMakeLists.txt
  mkdir build && cd build
  cmake -D ZMQ_BUILD_TESTS=OFF ..
  make $make_flags install
  cd ../..
  rm -rf zeromq-$libzmq_version_latest
  rm zeromq-$libzmq_version_latest.tar.gz
  ldconfig
  echo "Installed ZeroMQ $(PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig:/usr/lib/pkgconfig pkg-config --modversion libzmq)"
}

install_cppzmq_from_source() {
  echo "Compiling cppzmq from master"
  git clone https://github.com/zeromq/cppzmq.git
  cd cppzmq
  mkdir build && cd build
  cmake -D CPPZMQ_BUILD_TESTS=OFF ..
  make $make_flags install
  cd ../..
  rm -rf cppzmq
  ldconfig
  echo "Installed cppzmq"
}

install_opencv_from_source() {
  echo "Compiling OpenCV $opencv_version_latest from source"
  curl -L https://github.com/opencv/opencv/archive/$opencv_version_latest.tar.gz -o opencv-$opencv_version_latest.tar.gz
  tar -xvzf opencv-$opencv_version_latest.tar.gz
  curl -L https://github.com/opencv/opencv_contrib/archive/$opencv_version_latest.tar.gz -o opencv_contrib-$opencv_version_latest.tar.gz
  tar -xvzf opencv_contrib-$opencv_version_latest.tar.gz
  cd opencv-$opencv_version_latest
  mkdir build && cd build
  cmake -D CMAKE_BUILD_TYPE=RELEASE \
  -D CMAKE_INSTALL_PREFIX=/usr/local \
  -D OPENCV_EXTRA_MODULES_PATH=$PWD/../../opencv_contrib-$opencv_version_latest/modules \
  -D BUILD_EXAMPLES=OFF ..
  make $make_flags install
  cd ../..
  rm -rf opencv-$opencv_version_latest
  rm -rf opencv_contrib-$opencv_version_latest
  rm opencv-$opencv_version_latest.tar.gz
  rm opencv_contrib-$opencv_version_latest.tar.gz
  ldconfig
  echo "Installed OpenCV $(PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig:/usr/lib/pkgconfig pkg-config --modversion opencv)"
}

install_protobuf_from_source() {
  echo "Compiling Protobuf $protobuf_version_latest from source"
  curl -L https://github.com/google/protobuf/archive/v$protobuf_version_latest.tar.gz -o protobuf-$protobuf_version_latest.tar.gz
  tar -xvzf protobuf-$protobuf_version_latest.tar.gz
  cd protobuf-$protobuf_version_latest
  ./autogen.sh
  ./configure
  make $make_flags install
  cd ..
  rm -rf protobuf-$protobuf_version_latest
  rm protobuf-$protobuf_version_latest.tar.gz
  ldconfig
  echo "Installed Protobuf $(PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig:/usr/lib/pkgconfig pkg-config --modversion protobuf)"
}

install_boost_from_source() {
  echo "Compiling Boost $boost_version_latest from source"
  curl -O -L https://dl.bintray.com/boostorg/release/$boost_version_latest/source/boost_${boost_version_latest//./\_}.tar.gz
  tar -xvzf boost_${boost_version_latest//./\_}.tar.gz
  cd boost_${boost_version_latest//./\_}
  ./bootstrap.sh --prefix=$boost_root --with-libraries=coroutine,date_time,filesystem,iostreams,log,program_options,regex,serialization,system,test,thread
  ./b2 $make_flags install 
  cd ..
  rm -rf boost_${boost_version_latest//./\_}
  rm boost_${boost_version_latest//./\_}.tar.gz
  ldconfig
  echo "Installed Boost $boost_version_latest from source"
}

pkg_config_path="$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig:/usr/lib/pkgconfig"

check_if_library_exists() {
  PKG_CONFIG_PATH=$pkg_config_path pkg-config --exists $1
  if [ $? -eq 0 ]; then
    version_found=$(PKG_CONFIG_PATH=$pkg_config_path pkg-config --modversion $1)
    PKG_CONFIG_PATH=$pkg_config_path pkg-config --atleast-version=$2 $1
    if [ $? -eq 0 ]; then
      # Found suitable version
      echo "Found $1 $version_found"
      return 1
    fi
    echo "Found $1 $version_found but require $2"
  else
    echo "Could not find $1 using a PKG_CONFIG_PATH of '$pkg_config_path'"
  fi
  return 0
}

get_distribution() {
  lsb_dist=""
  # Every system that we officially support has /etc/os-release
  if [ -r /etc/os-release ]; then
    lsb_dist="$(. /etc/os-release && echo "$ID")"
  fi
  # Returning an empty string here should be alright
  echo "$lsb_dist"
}

check_if_library_exists "libzmq" "$libzmq_version_latest"
libzmq_exists=$?

check_if_library_exists "opencv" "$opencv_version_latest"
opencv_exists=$?

check_if_library_exists "protobuf" "$protobuf_version_latest"
protobuf_exists=$?

# Verify root/sudo access
if [ "$(id -u)" -ne "0" ]; then
  echo "Sorry, I need root/sudo access to continue"
  exit 1
fi

# Perform some very rudimentary platform detection
lsb_dist=$( get_distribution )
lsb_dist="$(echo "$lsb_dist" | tr '[:upper:]' '[:lower:]')"
dist_version="$(sed 's/\/.*//' /etc/debian_version | sed 's/\..*//')"

if [ "$lsb_dist" != "raspbian" ] || [ "$dist_version" != 9 ]; then
  echo "Sorry, this script only supports Raspbian Stretch as distro"
  exit 1
fi

# https://stackoverflow.com/a/37939589/1480019
version() { 
  echo "$@" | awk -F. '{ printf("%d%03d%03d%03d\n", $1,$2,$3,$4); }'; 
}

# Check CMake
cmake_exe=$(which cmake 2> /dev/null)
cmake_version=""
if [ -x "$cmake_exe" ]; then
  cmake_version=`${cmake_exe} --version | head -1 | awk '{print $3}'`
fi

if [ $(version $cmake_version) -lt $(version $cmake_version_latest) ]; then
  if [ "$cmake_version" != "" ]; then
    echo "Found CMake $cmake_version but require $cmake_version_latest"
  else 
    echo "Could not find CMake"
  fi

  echo "Installing CMake dependencies via apt-get"
  apt-get install -y automake build-essential curl git libssl-dev libtool
  install_cmake_from_source
else
  echo "Found CMake $cmake_version"
fi

# Check ZeroMQ
if [ $libzmq_exists -eq 0 ]; then
  install_libzmq_from_source
  install_cppzmq_from_source
fi

# Check OpenCV
if [ $opencv_exists -eq 0 ]; then
  echo "Installing OpenCV dependencies via apt-get"
  apt-get install -y libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev \
  libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
  libxvidcore-dev libx264-dev libatlas-base-dev gfortran
  install_opencv_from_source
fi

# Check Protobuf
if [ $protobuf_exists -eq 0 ]; then
  install_protobuf_from_source
fi

# Check Boost
boost_version=`cat "${boost_root}/include/boost/version.hpp" 2>/dev/null | grep BOOST_LIB_VERSION | tail -1 | tr -s ' ' | cut -d\  -f3 | sed 's/[^0-9\._]//gI'`

if [ "$boost_version" != "$boost_lib_version_latest" ]; then
  if [ "$boost_version" != "" ]; then
    echo "Found Boost $boost_version but require $boost_lib_version_latest"
  else 
    echo "Could not find Boost"
  fi

  echo "Enable the source packages in source.list"
  sed -i -e 's/\#deb-src/deb-src/g' /etc/apt/sources.list
  echo "Update the packages index"
  apt-get update
  echo "Installing Boost dependencies via apt-get build-dep"
  apt-get build-dep -y boost1.62
  install_boost_from_source
else
  echo "Found Boost $boost_version"
fi