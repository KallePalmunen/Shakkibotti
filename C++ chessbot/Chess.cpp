#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Chessbot1.cpp"
#include "basicbot.cpp"

int main(){
    std::string exit;
    printboard();
    while(true){
        basicbot();
        printboard();
        gameend();
        movepiece();
        printboard();
        gameend();
    }

    return 0;
}