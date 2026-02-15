# DoozAI
An AI agent for a modified version of 12 Men Morris (ancient Dooz) board game with Mini-max and
Alpha-Beta pruning. The game is written in C and [Raylib](https://www.raylib.com) library.

## Build
Just configure and build with `cmake`. CMake will download a copy of Raylib's source code so be
sure that you can access Github. Project is compiled and tested with MSVC version 19 and GCC version 15.2.0.
```bash
mkdir -p ./build/
cmake --fresh -S . -B ./build/ -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

## Run
```bash
# *nix
./build/dooz

# Windows
.\build\dooz.exe
```
