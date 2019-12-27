[![Downloads](https://img.shields.io/github/downloads/de4me/devilutionX/total.svg)](https://github.com/de4me/devilutionX/releases)
[![github stars](https://img.shields.io/github/stars/de4me/devilutionX.svg)](https://github.com/de4me/devilutionX/stargazers)

Nightly builds | Platform
---:| ---
[![Build status](https://ci.appveyor.com/api/projects/status/7yvnxh5nc0sfpeam?svg=true)](https://ci.appveyor.com/project/de4me/devilutionx-macos-mojave) | macOS Mojave 64bit
[![Build status](https://ci.appveyor.com/api/projects/status/s2ajkkpob9eisk3s?svg=true)](https://ci.appveyor.com/project/de4me/devilutionx-ubuntu-18) | Ubuntu 18.04 64bit
[![Build status](https://ci.appveyor.com/api/projects/status/alluattmnjm0mmn7?svg=true)](https://ci.appveyor.com/project/de4me/devilutionx-windows-7-64bit) | Windows 7 64bit

# How To Play:
 - Copy diabdat.mpq from your CD, or GoG install folder, to the DevilutionX install folder ; Make sure it is all lowercase.
 - [Download DevilutionX](https://github.com/de4me/devilutionX/releases), or build from source
 - Install [SDL2](https://www.libsdl.org/download-2.0.php) (including [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/) and [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/))
 - Run `./devilutionx`

Please keep in mind that this is still being worked on and is missing parts of UI and some minor bugs, see [milestone 1](https://github.com/de4me/devilutionX/milestone/1) for a full list of known issues.

# Building from Source
<details><summary>Linux</summary>

### Installing dependencies on Debian and Ubuntu
```
sudo apt-get install cmake g++ libsdl2-mixer-dev libsdl2-ttf-dev libsodium-dev
```
### Installing dependencies on Fedora
```
sudo dnf install cmake glibc-devel SDL2-devel SDL2_ttf-devel SDL2_mixer-devel libsodium-devel libasan libubsan
```
### Compiling
```
cd build
cmake ..
make -j$(nproc)
```
</details>

<details><summary>macOS</summary>

Make sure you have [Homebrew](https://brew.sh/) installed, then run:

```
brew bundle
cd build
cmake ..
cmake --build . -j $(sysctl -n hw.physicalcpu)
```
</details>
<details><summary>FreeBSD</summary>

### Installing dependencies
```
pkg install cmake sdl2_mixer sdl2_ttf libsodium
```
### Compiling
```
cd build
cmake ..
cmake --build . -j $(sysctl -n hw.ncpu)
```
</details>

<details><summary>OpenBSD</summary>

### Installing dependencies
```
pkg_add cmake sdl2-mixer sdl2-ttf libsodium gmake
```
### Compiling
```
cd build
cmake -DCMAKE_MAKE_PROGRAM=gmake ..
cmake --build . -j $(sysctl -n hw.ncpuonline)
```
</details>

<details><summary>Windows via MinGW</summary>

### Installing dependencies on WSL, Debian and Ubuntu

Download and place the 32bit MinGW Development Libraries of [SDL2](https://www.libsdl.org/download-2.0.php), [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/), [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/) and [Libsodium](https://github.com/jedisct1/libsodium/releases) in `/usr/i686-w64-mingw32`.

```
sudo apt-get install cmake gcc-mingw-w64-i686 g++-mingw-w64-i686
```
### Compiling
```
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../CMake/mingwcc.cmake ..
make -j$(nproc)
```
</details>
<details><summary>Windows via Visual Studio</summary>

### Installing dependencies
Make sure to install the `C++ CMake tools for Windows` component for Visual Studio.

* **Using vcpkg (recommended)**
1. Install vcpkg following the instructions from https://github.com/microsoft/vcpkg#quick-start.

   Don't forget to perform _user-wide integration_ step for additional convenience.
2. Install required dependencies by executing the following command (via cmd or powershell):

   For the 64-bit version of the dependencies please run this command:

   ```
   vcpkg install sdl2:x64-windows sdl2-mixer:x64-windows sdl2-ttf:x64-windows libsodium:x64-windows
   ```

   For the 32-bit version of the dependencies please run this command:

   ```
   vcpkg install sdl2:x86-windows sdl2-mixer:x86-windows sdl2-ttf:x86-windows libsodium:x86-windows
   ```

* **Manually**
1. Download and place the MSVC Development Libraries of [SDL2](https://www.libsdl.org/download-2.0.php), [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/), [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/) and [Libsodium](https://github.com/jedisct1/libsodium/releases) in `%USERPROFILE%\AppData\Local\Microsoft\WindowsApps\`.
2. If dependencies are not found or you wish to place them in other location - configure required path variables in _"Manage Configurations..."_ dialog inside Visual Studio or in _cmake-gui_.

### Compiling

* **Through Open->CMake in Visual Studio**
1. Go to `File -> Open -> CMake`, select `CMakeLists.txt` from the project root.
2. Select `Build devilution.exe` from the `Build` menu.

* **Through cmake-gui**

1. Input the path to devilutionx source directory at `Where is the source code:` field.
2. Input the path where the binaries would be placed at `Where to build the binaries:` field. If you want to place them inside source directory it's preferable to do so inside directory called `build` to avoid the binaries being added to the source tree.
3. It's recommended to input `Win32` in `Optional Platform for Generator`, otherwise it will default to x64 build.
4. In case you're using `vcpkg` select `Specify toolchain file for cross-compiling` and select the file `scripts/buildsystems/vcpkg.cmake` from `vcpkg` directory otherwise just go with `Use default native compilers`.
5. In case you need to select any paths to dependencies manually do this right in cmake-gui window.
6. Press `Generate` and open produced `.sln` file using Visual Studio.
7. Use build/debug etc. commands inside Visual Studio Solution like with any normal Visual Studio project.
</details>

<details><summary>Nintendo Switch</summary>
Run:

```
Packaging/switch/build.sh
```

This will install the [Switch devkit](https://switchbrew.org/wiki/Setting_up_Development_Environment) and build a DevilutionX Switch package. If you already have the devkit installed, or are on a non-Debian system, pass the the devkit path to the script like this:

```
DEVKITPRO=<path to devkit> Packaging/switch/build.sh
```

The nro-file will be generated in the build folder. Test with an emulator (RyuJinx) or real hardware.
</details>

<details><summary>Haiku</summary>

### Installing dependencies on 32 bit Haiku
```
pkgman install cmake_x86 devel:libsdl2_x86 devel:libsdl2_mixer_x86 devel:libsdl2_ttf_x86 devel:libsodium_x86
```
### Installing dependencies on 64 bit Haiku
```
pkgman install cmake devel:libsdl2 devel:libsdl2_mixer devel:libsdl2_ttf devel:libsodium
```
### Compiling on 32 bit Haiku
```
cd build
setarch x86 #Switch to secondary compiler toolchain (GCC8+)
cmake ..
cmake --build . -j $(nproc)
```
### Compiling on 64 bit Haiku
No setarch required, as there is no secondary toolchain on x86_64, and the primary is GCC8+
```
cd build
cmake ..
cmake --build . -j $(nproc)
```
</details>

<details><summary>OpenDingux / RetroFW</summary>

DevilutionX uses buildroot to build packages for OpenDingux and RetroFW.

The build script does the following:

1. Downloads and configures the buildroot if necessary.
2. Builds the executable (using CMake).
3. Packages the executable and all related resources into an `.ipk` package.

The buildroot uses ~4 GiB of disk space and can take almost an hour to build.

For OpenDingux builds `mksquashfs` needs to be installed.

### RetroFW (RS97, RG300, LDK)

The RetroFW build uses the buildroot at `$HOME/buildroot-2018.02.9-retrofw`.

~~~ bash
Packaging/OpenDingux/build-retrofw.sh
~~~

### OpenDingux (RG350, GCW0)

This OpenDingux build uses the buildroot at `$HOME/buildroot-rg350-devilutionx`.

~~~ bash
Packaging/OpenDingux/build-rg350.sh
~~~

</details>

<details><summary><b>CMake build options</b></summary>

### General
- `-DBINARY_RELEASE=ON` changed build type to release and optimize for distribution.
- `-DNONET=ON` disable network support, this also removes the need for the ASIO and Sodium.
- `-DUSE_SDL1=ON` build for SDL v1 instead of v2, not all features are supported under SDL v1, notably upscaling.
- `-DSPAWN=ON` build the shareware version, using spawn.mpq from the original shareware; which can still be [downloaded](http://ftp.blizzard.com/pub/demos/diablosw.exe) for free.
- `-DCMAKE_TOOLCHAIN_FILE=../CMake/32bit.cmake` generate 32bit builds on 64bit platforms (remember to use the `linux32` command if on Linux).
- `-DCROSS_PREFIX=/path/to/prefix` set the path to the `i686-w64-mingw32` directory.

### Debug builds
- `-DDEBUG=OFF` disable debug mode of the Diablo engine.
- `-DASAN=OFF` disable address sanitizer.
- `-DUBSAN=OFF` disable undefined behavior sanitizer.

</details>

# Multiplayer
 - TCP/IP only requires the host to expose port 6112
 - UDP/IP requires that all players expose port 6112

All games are encrypted and password protected.

# Controller support

DevilutionX supports gamepad controls.

Default controller mappings (A/B/X/Y as in Nintendo layout, so the rightmost button is attack):

- Left analog / DPad: move hero
- Right analog: simulate mouse
- A: attack nearby enemies, talk to towns people and merchants, pickup/place items in the inventory, OK while in main menu
- B: Select spell, back while in menus
- X: pickup items, open chests and doors that are nearby, use item in the inventory
- Y: cast spell, delete character while in main menu
- R1: use mana potion from belt
- L1: use health item from belt
- R2: inventory
- L2: character sheet
- Left analog click: toggle automap
- Right analog click: left mouse click
- Start: game Menu, skip movie
- Select + L2: quest log
- Select + R2: spell book
- Select + Right analog click: right mouse click
- Select + A/B/X/Y: hot spell

For now, they can be re-mapped by changing `SourceX/controls` or by setting the `SDL_GAMECONTROLLERCONFIG` environment
variable (see
[SDL_GameControllerDB](https://github.com/gabomdq/SDL_GameControllerDB)).

# Contributing
[Guidelines](docs/CONTRIBUTING.md)

# F.A.Q.
> Wow, does this mean I can download and play Diablo for free now?

No, you'll need access to the data from the original game. If you don't have an original CD then you can [buy Diablo from GoG.com](https://www.gog.com/game/diablo). Alternatively you can also use `spawn.mpq` from the [http://ftp.blizzard.com/pub/demos/diablosw.exe](shareware) version and compile the with the SPAWN flag defined.
> What game changes does DevilutionX provide

DevilutionX's main focuse is to making the game work on multiple platforms and making the engine mod frindly. As such there are not change to the gameplay, but we will be making some enhancments to the engine it self. To give some examples there is now upscaling, unlocked fps and controller support and multiplayer via TCP.
> Is 1080p supported?

Currently the game simply scales the original 640x480 to beset fit, but we are working on widescreen support.
> What about Hellfire?

Hellfire is being worked on and is mostly done though not fully playable at the moment.
> Does it work with Battle.net?

Battle.net is a service provided by Blizzard, as we are not associated with them we have not worked on intergrating with there service.
</details>`

# Credits
- The original Devilution project [Devilution](https://github.com/diasurgical/devilution#credits)
- [Everyone](https://github.com/diasurgical/devilutionX/graphs/contributors) who worked on Devilution/DevilutionX
- And a thanks to all who support the project, report bugs and help spread the word <3

# Changelog
[From the beginning until release](docs/CHANGELOG.md)

# Legal
DevilutionX is released to the Public Domain. The documentation and function provided by Devilution may only be utilized with assets provided by ownership of Diablo.

Diablo(R) - Copyright (C) 1996 Blizzard Entertainment, Inc. All rights reserved. Diablo and Blizzard Entertainment are trademarks or registered trademarks of Blizzard Entertainment, Inc. in the U.S. and/or other countries.

Devilution and any of its' maintainers are in no way associated with or endorsed by Blizzard Entertainment(R).
