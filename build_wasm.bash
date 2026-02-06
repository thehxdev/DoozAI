#!/usr/bin/env bash

if ! command -v emcc >/dev/null; then
    echo "[ERROR] filed to find 'emcc'"
    echo "[INFO] install emscripten toolchain from https://emscripten.org/docs/getting_started/downloads.html"
    exit 1
fi

SELF_PATH=$(realpath "$(dirname "$0")")
DOOZ_RAYLIB_WASM="${DOOZ_RAYLIB_WASM:-$SELF_PATH/raylib-wasm}"

emcc -O2 -Wall \
    -I. -I"$DOOZ_RAYLIB_WASM/include" \
    -L. -L"$DOOZ_RAYLIB_WASM/lib" \
    -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB \
    --shell-file "$SELF_PATH/wasm/minshell.html" \
    --preload-file "$SELF_PATH/assets/board.png"@"./assets/board.png" \
    -o "$SELF_PATH/game-wasm.html" \
    "$SELF_PATH/src/build.c" \
    "$DOOZ_RAYLIB_WASM/lib/libraylib.a"
