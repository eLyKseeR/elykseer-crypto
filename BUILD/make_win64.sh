#!/bin/sh
#
# need to install the MinGW64 version of cmake:
#
# pacman -S mingw-w64-x86_64-cmake

if [[ $(uname -s) != MINGW64* ]]; then
	echo "this compilation is only valid on a MinGW64 installation"
	exit 1
fi

# cleanup
rm -rf CMakeCache.txt CMakeFiles src/CMakeFiles test/CMakeFiles

set -e

export CC=gcc
export CXX=g++

# prepare source code
./mk_cpp.sh

# where the compiled dependencies have been installed
export PKG_CONFIG_PATH=$(pwd)/../ext/win64/lib/pkgconfig:$PKG_CONFIG_PATH

# compile in Debug | Release mode
#MODE=Debug
MODE=Release
cmake --fresh -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=${MODE} .
cmake --build .
make package

# show output
ls -l src/libelykseer-crypto*.a

