#Tyro

A Cross-platform Code Editor

## Planned Features

* Syntax Highlighting
* Line Numbers
* Custom Syntax themes
* SFTP editing

## Building

### Linux (Ubuntu/Debian)
Required packages:

* build-essential
* libwxgtk3.0-dev
* libssh2-1-dev

Optional:

* libwxgtk3.0-0-dbg
* libssh2-1-dbg


After these packages are installed, the project should build with a simple `make` command.

### OS X
Building wxWidgets:

If you want maximum compatibility with older versions of OS X, 
view [the guide](./Mac-compatibility-build.md). Otherwise, these simpler 
steps should work fine for a local build.

1. Download the latest wxWidgets source
2. Make a new directory in the source tree, like `wxmac`
3. Run `../configure --disable-shared --disable-webviewwebkit --disable-compat28` in the new directory
4. Run `make && make install`

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



 
