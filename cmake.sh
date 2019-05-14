#!/bin/bash
mkdir -p build

unset MACOSX_DEPLOYMENT_TARGET
unset CMAKE_OSX_SYSROOT

export MACOSX_DEPLOYMENT_TARGET="10.9"
export CMAKE_OSX_SYSROOT="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk"

cd build
cmake -stdlib=libc++  ..
make "$@"
cd ..
