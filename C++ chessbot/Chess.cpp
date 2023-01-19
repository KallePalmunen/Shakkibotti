#include "Chessbot1.cpp"
#include "basicbot.cpp"

int main(){
    std::string exit;
    std::copy(&board[0][0], &board[0][0] + 64, &positions[0][0][0]);
    basicbot();
    printboard();
    while(true){
        movepiece();
        printboard();
        gameend();
    }

    return 0;
}