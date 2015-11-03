#!/bin/bash
mkdir -p build

unset MACOSX_DEPLOYMENT_TARGET
unset CMAKE_OSX_SYSROOT

export MACOSX_DEPLOYMENT_TARGET="10.7"
export CMAKE_OSX_SYSROOT="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk"

if [[ "$OSTYPE" == "linux-gnu" ]]; then
	cd build
	cmake -DHUNTER_ENABLED=OFF  ..
	make "$@"
	cd ..
elif [[ "$OSTYPE" == "darwin"* ]]; then
	# On OSX, disable hunter, because it doesn't work
	# for building wxWidgets, as it thinks it is on linux
	cd build
	cmake -DHUNTER_ENABLED=OFF  ..
	make "$@"
	cd ..
fi

