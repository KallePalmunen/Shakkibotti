#include <iostream>
#include <algorithm>

using namespace std;

int board[8][8] = {{30,10,20,50,40,21,11,31}, {1,2,3,4,5,6,7,8},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0},{-1,-2,-3,-4,-5,-6,-7,-8},{-30,-10,-20,-50,-40,-21,-11,-31}};
int moves = 0;
int positions[10000][8][8];
int turn = 0;
int enpassant = -1;
int bot = 2;
bool promotemenu = false;
double evalscore = 0.0;
string evaltext = "";

//pawns, knights, bishops, rooks, queens and kings (W,B)
int pieces[6][2] = {{8,8},{2,2},{2,2},{2,2},{1,1},{1,1}};
//black, white
int kingmoved[2] = {0,0};
//black left, right - white left, right
int rookmoved[2][2] = {{0,0},{0,0}};

void printboard(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            cout << board[i][j] << " ";
        }
        cout << "\n";
    };
}

bool pawnmove(int n, int y0, int x0, int y1, int x1){
    if(n > 0){
        if((x1 == x0 && (y1-y0 == 1 || (y1-y0 == 2 && y0 == 1 
            && board[y1-1][x1] == 0)) && board[y1][x1] == 0) || 
            (y1-y0 == 1 && board[y1][x1] < 0 && 
            (x1 - x0 == 1 || x1 - x0 == -1)) 
            || (x1*8+y1 == enpassant && abs(x1-x0) == 1 && y1 - y0 == 1)){
                return true;
        }
        return false;
    }else if(n < 0){
        if ((x1 == x0 && (y0-y1 == 1 || (y0-y1 == 2 && y0 == 6 
            && board[y1+1][x1] == 0)) && board[y1][x1] == 0) ||
            (y0-y1 == 1 && board[y1][x1] > 0 && 
            (x1 - x0 == 1 || x1 - x0 == -1))
            || (x1*8+y1 == enpassant && abs(x1-x0) == 1 && y0 - y1 == 1)){
                return true;
        }
        return false;
    }
    return false;
}

bool knightmove(int n, int y0, int x0, int y1, int x1){
    if(((y1 - y0 == 2 || y1 - y0 == -2) && (x1 - x0 == 1 || x1 - x0 == -1))
        || ((y1- y0 == 1 || y1 - y0 == -1) &&
        (x1 - x0 == 2 || x1 - x0 == -2))){
            if(n > 0 && (board[y1][x1] == 0 || board[y1][x1] < 0)){
                return true;
            }
            if(n < 0 && (board[y1][x1] == 0 || board[y1][x1] > 0)){
                return true;
            }
            return false;
        }
    return false;
}

bool piecemove(int n, int y0, int x0, int y1, int x1){
    if(y1 < 8 && x1 < 8 && x1 >= 0 && y1 >= 0 && y0>=0){
        if(abs(n) < 10){
            if(pawnmove(n, y0, x0, y1, x1)){
                return true;
            }
        }
        if(abs(n) < 20 && abs(n) >= 10){
            if(knightmove(n, y0, x0, y1, x1)){
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}

void movepieceto(int n, int y0, int x0, int y1, int x1){
    board[y0][x0] = 0;
    board[y1][x1] = n;
}

void movepiece(){
    int move, movetox, movetoy;
    cin >> move;
    cin >> movetoy;
    cin >> movetox;
    if((move > 0 && turn == 0) || (move < 0 && turn == 1)){
        int *index;
        index = find(&board[0][0], &board[0][0]+64,move);
        if(index != &board[0][0]+64){
            int pos = distance(&board[0][0], index);
            int y0 = pos/8;
            int x0 = pos-y0*8;
            if(piecemove(move, y0, x0, movetoy, movetox)){
                movepieceto(move, y0, x0, movetoy, movetox);
            }else{
                cout << "Illegal move" << "\n";
                return;
            }
        }else{
            cout << "Illegal move" << "\n";
            return;
        }
        turn = (turn == 0);
    }else{
        cout << "Illegal move" << "\n";
    }
}
