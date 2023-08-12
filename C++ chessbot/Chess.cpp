#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <cmath>
#include "pgnconverter.cpp"
#include "Chessbot1.cpp"
#include "basicbot.cpp"
#include "montecarlo.cpp"

int main(){
    //0 == basicbot, 1 == monte_carlo
    int bot_version = 1;
    locate_pieces();
    add_to_positions();
    set_can_move_positions();
    printboard();
    while(true){
        if(turn == bot){
            if(bot_version == 0){
                basicbot();
            }
            if(bot_version == 1){
                monte_carlo_move();
            }
        }
        set_can_move_positions();
        gameend();
        if(turn == -1){
            return 0;
        }
        while(turn != bot){
            movepiece();
        }
        set_can_move_positions();
        gameend();
        if(turn == -1){
            return 0;
        }
    }
    return 0;
}