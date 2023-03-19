#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>
#include "pgnconverter.cpp"
#include "Chessbot1.cpp"
#include "basicbot.cpp"

int main(){
    locate_pieces();
    add_to_positions();
    set_can_move_positions();
    printboard();
    while(true){
        if(turn == bot){
            basicbot();
        }
        gameend();
        if(turn == -1){
            return 0;
        }
        while(turn != bot){
            movepiece();
        }
        if(turn == -1){
            return 0;
        }
        gameend();
    }
    return 0;
}