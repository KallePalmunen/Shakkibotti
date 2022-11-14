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

void movepiece(){
    int move, movetox, movetoy;
    cin >> move;
    cin >> movetoy;
    cin >> movetox;
    if((move > 0 && turn == 0) || (move < 0 && turn == 1)
        || (0 == 0)){
        int *index;
        index = find(&board[0][0], &board[0][0]+64,move);
        if(index != &board[0][0]+64){
            int pos = distance(&board[0][0], index);
            int y0 = pos/8;
            int x0 = pos-y0*8;
            if(movetoy >= 0 && movetoy < 8 && movetox >= 0 && movetox < 8){
                board[y0][x0] = 0;
                board[movetoy][movetox] = move;
            }
        }
    }
}

int main(){
    copy(&board[0][0], &board[0][0] + 64, &positions[0][0][0]);
    printboard();
    movepiece();
    printboard();
    
    return 0;
}