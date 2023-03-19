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
    for(int i = 0; i < can_move_positions[1][19].size(); i++){
        std::cout << can_move_positions[1][19][i][0] << ',' << can_move_positions[1][19][i][1] << '\n';
    }
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