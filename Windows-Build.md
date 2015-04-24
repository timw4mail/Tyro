# Building on Windows

In order to keep a consistent build system, Tyro is build with MinGW and Msys. This software allows for a *nix-style build environment on Windows.

## Build Environment Setup:

1. Download MinGW installer
2. Install MinGW & MSyS
3. Add `{MinGW Path}\bin` and `{MinGW Path}\msys\1.0\bin` to the system Path environment variable
4. Get the latest wxWidgets 3.0 installer for Windows, and install
5. Open the Msys prompt at `{MinGW Path}\msys\1.0\msys.bat`. You'll probably want a shortcut to this, as it is what will be used for compiling everything from here on.

## Build SFTP Dependencies