#!/bin/sh
mkdir -p build

unset MACOSX_DEPLOYMENT_TARGET
unset CMAKE_OSX_SYSROOT

export MACOSX_DEPLOYMENT_TARGET="10.7"
export CMAKE_OSX_SYSROOT="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk"

cd build
cmake ..
make "$@"
cd ..