#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Chessbot1.cpp"
#include "basicbot.cpp"

int main(){
    std::string exit;
    locate_pieces();
    add_to_positions();
    printboard();
    while(true){
        basicbot();
        gameend();
        if(turn == -1){
            return 0;
        }
        while(turn == 1){
            movepiece();
        }
        if(turn == -1){
            return 0;
        }
        gameend();
    }
    return 0;
}