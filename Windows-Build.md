# Building on Windows

In order to keep a consistent build system, Tyro is build with MinGW and Msys. This software allows for a *nix-style build environment on Windows.

## Build Environment Setup:

1. Download MinGW installer
2. Install MinGW & MSyS (at least the following packages)

	* mingw-developer-toolkit
	* mingw32-base
	* mingw32-gcc-g++
	* msys-base

3. Add `{MinGW Path}\bin` and `{MinGW Path}\msys\1.0\bin` to the system Path environment variable
4. Get the latest wxWidgets 3.0 installer for Windows, and install
5. Open the Msys prompt at `{MinGW Path}\msys\1.0\msys.bat`. You'll probably want a shortcut to this, as it is what will be used for compiling everything from here on.
6. Install Git

## Build SFTP Dependencies

1. Run `git clone https://github.com/timw4mail/Tyro-depends.git` in the folder of your choice.
2. Run `sh ./buildssh2.sh` to build the dependencies.

## Build Tyro

In the source folder, run `make dev run` to build the develop version and run it immediately