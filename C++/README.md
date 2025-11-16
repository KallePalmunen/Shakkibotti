# Setup

On windows (replace with your own path)
```
Set-ExecutionPolicy Unrestricted -Scope Process
cd C:\Users\mikae\emsdk
.\emsdk install latest
.\emsdk activate latest
cd "C:\Users\mikae\Documents\GitHub\Shakkibotti\docs\"
```

# Build

On windows (replace with your own paths)
```
emcc -O3 -gsource-map -s WASM=1 -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" -s EXPORTED_FUNCTIONS="['_movepiece', '_basicbot', '_gameend', '_malloc', '_free']" -o chessbot.js "C:\Users\mikae\Documents\GitHub\Shakkibotti\C++\Chess.cpp"
```