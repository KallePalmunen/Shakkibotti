#include "Chessbot1.cpp"

int main(){
    string exit;
    copy(&board[0][0], &board[0][0] + 64, &positions[0][0][0]);
    printboard();
    while(true){
        cout << "Quit? y/n" << "\n";
        cin >> exit;
        if(exit == "y"){
            cout << "Draw by annihilation";
            break;
        }
        movepiece();
        printboard();
    }

    return 0;
}