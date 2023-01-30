#include <iostream>

int bestmove[6];
std::vector<int> order;

double evaluate0(int y0, int x0){
    int n = board[y0][x0];
    return 0.0;
    //n1 and n2 are squares where a threatening pawn could be
}

void reorder(){
    std::vector<double> movescore = {};
    std::vector<int> starting_order = {};
    for(int n = 1; n < 51; n++){
        int *pindex;
        pindex = std::find(&board[0][0], &board[0][0]+64, n);
        if(pindex != &board[0][0]+64){
            int nposition = std::distance(&board[0][0], pindex);
            int y0 = nposition/8;
            int x0 = nposition-y0*8;
            for(int y1 = 0; y1 < 8; y1++){
                for(int x1 = 0; x1 < 8; x1++){
                    if(canmove(n, y0, x0, y1, x1)){
                        
                    }
                }
            }
        }
    }
}

void whitemove1(){
    //save current state
    int temp_moves = moves;
    int temp_enpassant = enpassant;
    int temp_board[8][8];
    std::copy(&board[0][0], &board[0][0]+64, &temp_board[0][0]);
    int temp_kingmoved[2];
    std::copy(&kingmoved[0], &kingmoved[0]+2, 
        &temp_kingmoved[0]);
    int temp_rookmoved[2][2];
    std::copy(&rookmoved[0][0], &rookmoved[0][0]+4, 
        &temp_rookmoved[0][0]);
    int temp_pieces[6][2];
    std::copy(&pieces[0][0], &pieces[0][0]+12, 
        &temp_pieces[0][0]);
    std::vector<std::vector<std::vector<int>>> temp_positions
        = positions;
    std::vector<double> movescore = {-1000000};
}

void basicbot(){
    whitemove1();
}