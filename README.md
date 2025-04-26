# OpenFunscripter

Can be used to create `.funscript` files. (NSFW)  
The project is based on OpenGL, SDL2, ImGui, libmpv, & all these other great [libraries](https://github.com/OpenFunscripter/OpenFunscripter/tree/master/lib).

![OpenFunscripter Screenshot](https://github.com/OpenFunscripter/OpenFunscripter/blob/1b4f096be8c2f6c75ceed7787a300a86a13fb167/OpenFunscripter.jpg)

## How to build

1. Clone the repository
2. `cd "OpenFunscripter"`
3. `./build.sh`

Known linux dependencies to just compile are `build-essential libmpv-dev libglvnd-dev`.  

## This fork

This is a personal fork for linux use, do not expect maitenance, updates or new features

I will not be providing any pre-built binaries

I am also **NOT** a C++ dev, expect non-trivial fixes to have shitty code

### Current fixes compared to 3.2.0

1. Fixed compilation on linux
2. Fixed waveform rendering on linux
3. Fixed mpv spazzing out on linux
