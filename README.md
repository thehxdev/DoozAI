# DoozAI
An AI agent for a modified version of 12 Men Morris (ancient Dooz) board game with Mini-max and
Alpha-Beta pruning. The game is written in C and [Raylib](https://www.raylib.com) library.

## Build
Download and install Raylib to `raylib` directory in the project's root directory.
Then configure and build with `cmake`.
```bash
mkdir -p ./build/
cmake --fresh -S . -B ./build/
cmake --build build
```

## Run
```bash
./build/dooz
```
