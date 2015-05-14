# Building On OSX for maximum compatibility

## Install Older Platform SDKs
Use [this script](http://devernay.free.fr/hacks/xcodelegacy/) to install older SDKs.

## Additional configure flags for compiling wxWidgets
### Building on 10.8 or newer
* --with-macosx-version-min=10.5 
* --with-macosx-sdk=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.5.sdk

### Building on 10.5 to 10.7
* --with-macosx-version-min=10.5
* --with-macosx-sdk=/Developer/SDKs/MacOSX10.5.sdk


## Recommended flags for development/debugging (Assuming OS X > 10.7)
* --enable-debug_gdb
* --enable-profile
* --disable-compat28
* --disable-shared
* --without-liblzma
* --without-webviewwebkit
* --with-macosx-version-min=10.5
* --with-macosx-sdk=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.5.sdk

