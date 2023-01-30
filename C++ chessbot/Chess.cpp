#include <iostream>
#include <algorithm>
#include <vector>
#include "Chessbot1.cpp"
#include "basicbot.cpp"

int main(){
    std::string exit;
    basicbot();
    printboard();
    while(true){
        movepiece();
        printboard();
        gameend();
    }

    return 0;
}