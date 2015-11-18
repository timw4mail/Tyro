#Tyro

A Cross-platform Code Editor

[![Build Status](https://jenkins.timshomepage.net/buildStatus/icon?job=tyro)](https://jenkins.timshomepage.net/job/tyro/) (Linux Build Only)

## Planned Features

* Syntax Highlighting
* Line Numbers
* Custom Syntax themes
* SFTP editing

## Building

### Linux (Ubuntu/Debian)
Required packages:

* build-essential
* libssh2-1-dev
* libwxgtk3.0-dev

After these packages are installed, the project should build with a simple `make` command.

### OS X
Building wxWidgets:

If you want maximum compatibility with older versions of OS X,
view [the guide](./Mac-compatibility-build.md). Otherwise, these simpler
steps should work fine for a local build.

1. Download the latest wxWidgets source (>= 3.0.2)
2. Run `export CXX="clang++ -std=c++11 -stdlib=libc++"` to compile with clang for better C++11 support
3. Make a new directory in the source tree, like `wxmac`
4. Run `../configure --disable-shared --disable-webviewwebkit --disable-compat28` in the new directory
5. Run `make && make install`

Install libssh2 (Using homebrew):

* `brew install libssh2`

Build the app:

* `make Tyro.app`

### Windows

See the guide for building on Windows: [Windows-Build](./Windows-Build.md)

## Make commands
Please note that make commands are chainable. For a typical programming loop, `make clean dev run` is very useful.

* all - Make a release binary in the build folder
* clean - Remove intermediate build files
* dev - Make a development binary in the build folder
* run - Run the current binary in the build folder
* release - Make a release binary, with the appropriate resources compiled in. Makes an app bundle on OS X, and adds the icon to the program on Windows.




