# Define our host system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_LIBRARY_ARCHITECTURE arm-linux-gnueabihf)

# Define the cross compiler locations
set(CMAKE_C_COMPILER ${CMAKE_SOURCE_DIR}/gcc-linaro-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/gcc-linaro-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

# Specify path for CMake to search when invoke find_package(), find_library(), find_path() functions
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SOURCE_DIR}/raspbian-rootfs)

# Use our definitions for compiler tools
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries, headers and packages in the target directories only
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Specifically set linker flags for multiarch libraries.
set(LINK_FLAGS "-Wl,-rpath-link=${CMAKE_FIND_ROOT_PATH}/lib/${CMAKE_LIBRARY_ARCHITECTURE} -Wl,-rpath-link=${CMAKE_FIND_ROOT_PATH}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE} -Wl,-rpath-link=${CMAKE_FIND_ROOT_PATH}/usr/local/lib -Wl,-rpath-link=${CMAKE_FIND_ROOT_PATH}/opt/vc/lib")

set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} ${LINK_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${LINK_FLAGS}"  CACHE STRING "" FORCE)

# Hint OpenAL path, otherwise the wrong path is targeted
set(OPENAL_INCLUDE_DIR ${CMAKE_FIND_ROOT_PATH}/usr/include/ CACHE STRING "" FORCE)

# Compile with C++ 14 support
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)