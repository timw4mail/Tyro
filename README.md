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

1. Download the latest wxWidgets source
2. Make a new directory in the source tree, like `wxmac`
3. Run `../configure --disable-shared --disable-webviewwebkit` in the new directory
4. Run `make && make install`

Install libssh2 (Using homebrew):

* `brew install libssh2`

Build the app:

* `make Tyro.app`

### Windows

See the guide for building on Windows: [Windows-Build](./Windows-Build.md)



 
