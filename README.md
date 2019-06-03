# Tyro

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
* cmake
* libssh2-1-dev
* libwxgtk3.0-dev

After these packages are installed, the project should build with a simple `make` command.

### OS X
Install wxWidgets library (Using homebrew):
* `brew install wxmac`

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




