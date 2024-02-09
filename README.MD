# Shakkibotti

This project contains different versions of chess bots. The following versions are included:    

- A random mover bot (python version)    
- A minimax chess bot (python version)  
- A minimax chess bot with alpha-beta pruning (C++/web version)     
- A neural network chess bot (not functional)   
- A neural network MCTS chess bot (not functional, in development)  

## How to play

### C++/web version

visit the following website:    
https://kallepalmunen.github.io/Shakkibotti/

### python version

**1. install requirements**     

    python -m pip install -r requirements.txt   

**2. run pygametester.py**

You can change the version you want to use by changing the value of "botlevel" in pygametester.py

## How to build and run C++ version locally    

### Windows     

**1. Get rid of restrictions**  

Open terminal as administrator    

    Set-ExecutionPolicy Unrestricted -Scope Process

**2. Activate emsdk**   

    cd Path_to_emsdk  
    .\emsdk install latest      
    .\emsdk activate latest     

**3. Go to C++ chessbot folder**    

    cd "Path_to_Shakkibotti\docs\"

**4. Build project**    

    emcc -O3 -gsource-map -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' -s EXPORTED_FUNCTIONS='["_movepiece", "_basicbot", "_gameend", "_malloc", "_free"]' -o chessbot.js "Path_to_Shakkibotti\docs\Chess.cpp"    

or  

    emcc -O3 -gsource-map -c "Path_to_Shakkibotti\docs\Chess.cpp" -o chessbot.o     
    
    emcc -O3 -gsource-map chessbot.o -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' -s EXPORTED_FUNCTIONS='["_movepiece", "_basicbot", "_gameend", "_malloc", "_free"]' -o chessbot.js --source-map-base http://localhost/Shakkibotti/docs/     

**5. Run Shakkibotti in localhost**    

## Please note that    

- functions with the prefix "bot" are optimized functions for the use of bots and may not always give enterely accurate results (but they are good enough for the bots)
