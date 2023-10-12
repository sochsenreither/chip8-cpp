# Chip8-cpp

This repository contains a wip implementation of a Chip8 interpreter written in C++.

## Dependencies
- [SDL2](https://github.com/libsdl-org/SDL) - Graphics, audio, input

## Building with CMake
```
$ git clone https://github.com/sochsenreither/chip8-cpp.git
$ cd chip8-cpp
$ git submodule update --init 
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Usage 

```
chip8 roms/INVADERS
```
If no rom path is provided the emulator is started with TETRIS as rom.