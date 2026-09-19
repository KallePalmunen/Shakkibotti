#!/bin/bash

set -e

# Read EMSDK_PATH from build.config
source build.config

if [ -z "$EMSDK_PATH" ]; then
    echo "Error: EMSDK_PATH is not set in build.config"
    exit 1
fi

# Remember the current directory
ORIGINAL_DIR="$(pwd)"

# Go to emsdk
cd "$EMSDK_PATH"

# Install and activate latest Emscripten
./emsdk install latest
./emsdk activate latest

source "$EMSDK_PATH/emsdk_env.sh"

echo "Emscripten setup complete."

cd "$ORIGINAL_DIR/../docs"

emcc -O3 -gsource-map -s WASM=1 -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap', 'HEAPU8']" -s EXPORTED_FUNCTIONS="['_movepiece', '_basicbot', '_gameend', '_malloc', '_free']" -o chessbot.js "$ORIGINAL_DIR/Chess.cpp"
