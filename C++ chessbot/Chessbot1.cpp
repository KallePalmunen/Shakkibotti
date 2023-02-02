
int board[8][8] = {{30,10,20,50,40,21,11,31}, {1,2,3,4,5,6,7,8},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0},{-1,-2,-3,-4,-5,-6,-7,-8},{-30,-10,-20,-50,-40,-21,-11,-31}};
int moves = 0;
std::vector<std::vector<std::vector<int>>> positions;
int turn = 0;
int enpassant = -1;
int bot = 2;
bool promotemenu = false;
double evalscore = 0.0;
std::string evaltext = "";

//pawns, knights, bishops, rooks, queens and kings (W,B)
int pieces[6][2] = {{8,8},{2,2},{2,2},{2,2},{1,1},{1,1}};
//black, white
int kingmoved[2] = {0,0};
//black left, right - white left, right
int rookmoved[2][2] = {{0,0},{0,0}};

void printboard(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            std::cout << board[i][j] << " ";
        }
        std::cout << "\n";
    };
}

void add_to_positions(){
    std::vector<std::vector<int>> currentposition;
    for(int y = 0; y < 8; y++){
        std::vector<int> currentposition_x;
        currentposition_x.insert(currentposition_x.begin(), 
            board[y], board[y]+8);
        currentposition.push_back(currentposition_x);
    }
    positions.push_back(currentposition);
}

//intsign tells the sign of an integer

int intsign(int a){
    return (a > 0)-(a < 0);
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

//longmove checks if there is anything in the way when moving bishops, rooks and queens. It also checks whether there is a piece in the endsquare

bool longmove(int n, int y0, int x0, int y1, int x1){
    if(n > 0){
        for(int i = 1; i < 8; i++){
            if(y0 + (y1 != y0)*intsign(y1-y0)*i == y1 
                && x0 + (x1 != x0)*intsign(x1-x0)*i == x1 
                && board[y1][x1] <= 0){
                    return true;
            }
            if(board[y0 + (y1 != y0)*intsign(y1-y0)*i]
                [x0 + (x1 != x0)*intsign(x1-x0)*i] != 0){
                    return false;
            }
        }
        return false;
    }
    if(n < 0){
        for(int i = 1; i < 8; i++){
            if(y0 + (y1 != y0)*intsign(y1-y0)*i == y1 
                && x0 + (x1 != x0)*intsign(x1-x0)*i == x1 
                && board[y1][x1] >= 0){
                    return true;
            }
            if(board[y0 + (y1 != y0)*intsign(y1-y0)*i]
                [x0 + (x1 != x0)*intsign(x1-x0)*i] != 0){
                    return false;
            }
        }
        return false;
    }
    return false;
}

bool bishopmove(int n, int y0, int x0, int y1, int x1){
    if(abs(y1-y0) == abs(x1-x0)){
        if(longmove(n, y0, x0, y1, x1)){
            return true;
        }
        return false;
    }
    return false;
}

bool rookmove(int n, int y0, int x0, int y1, int x1){
    if((y1 != y0 && x1 == x0) || (y1 == y0 && x1 != x0)){
        if(longmove(n, y0, x0, y1, x1)){
            return true;
        }
        return false;
    }
    return false;
}

bool queenmove(int n, int y0, int x0, int y1, int x1){
    if(bishopmove(n, y0, x0, y1, x1) || rookmove(n, y0, x0, y1, x1)){
        return true;
    }
    return false;
}

bool kingmove(int n, int y0, int x0, int y1, int x1){
    if(abs(y1-y0) <= 1 && abs(x1-x0) <= 1 && 
    !(y1 == y0 && x1 == x0)){
        if(n > 0 && board[y1][x1] <= 0){
            return true;
        }
        if(n < 0 && board[y1][x1] >= 0){
            return true;
        }
    }
    return false;
}

bool piecemove(int n, int y0, int x0, int y1, int x1){
    if(y1 < 8 && x1 < 8 && x1 >= 0 && y1 >= 0 && y0>=0){
        if(abs(n) < 10){
            if(pawnmove(n, y0, x0, y1, x1)){
                return true;
            }
            return false;
        }
        if(abs(n) < 20 && abs(n) >= 10){
            if(knightmove(n, y0, x0, y1, x1)){
                return true;
            }
            return false;
        }
        if(abs(n) < 30 && abs(n) >= 20){
            if(bishopmove(n, y0, x0, y1, x1)){
                return true;
            }
            return false;
        }
        if(abs(n) < 40 && abs(n) >= 30){
            if(rookmove(n, y0, x0, y1, x1)){
                return true;
            }
            return false;
        }
        if(abs(n) < 50 && abs(n) >= 40){
            if(queenmove(n, y0, x0, y1, x1)){
                return true;
            }
            return false;
        }
        if(abs(n) >= 50){
            if(kingmove(n, y0, x0, y1, x1)){
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}

bool promote(int n, int y1){
    if(n < 10 && n > 0){
        if(y1 == 7){
            return true;
        }
    }
    if(n > -10 && n < 0){
        if(y1 == 0){
            return true;
        }
    }
    return false;
}

bool check(int n){
    int *index;
    index = std::find(&board[0][0], &board[0][0]+64, n);
    int kingy;
    int kingx;

    if(index != &board[0][0]+64){
        int kingpos = std::distance(&board[0][0], index);
        kingy = kingpos/8;
        kingx = kingpos-kingy*8;
    }else{
        return true;
    }

    for(int n1 = 0; n1 < 6; n1++){
        for(int n2 = 0; n2 < pieces[n1][(n > 0)]; n2++){
            int piecen = -intsign(n)*(n1*10+n2+(n1 == 0));
            int *pindex;
            pindex = std::find(&board[0][0], &board[0][0]+64, piecen);
            if(pindex != &board[0][0]+64){
                int ppos = std::distance(&board[0][0], pindex);
                int y0 = ppos/8;
                int x0 = ppos-y0*8;
                if(piecemove(piecen, y0, x0, kingy, kingx)){
                    return true;
                }
            }
        }
    }
    return false;
}

bool castle(int n, int y0, int x0, int y1, int x1){
    if(kingmoved[(n>0)] == 1 || rookmoved[(n>0)][x1>4] == 1){
        return false;
    }
    if(y1 == y0 && (x1 == 1 || x1 == 5) && !check(n) &&
    board[y0][(x1 > 4)*7] == intsign(n)*(30 + (x1 > 4))){
        for(int i = 1; i < 3 + (x1 > 4); i++){
            int squarex = x0 + i*intsign(x1-x0);
            if(board[y0][squarex] != 0){
                return false;
            }
            board[y0][x0] = 0;
            board[y0][squarex] = n;
            if(i < 3 && check(n)){
                board[y0][x0] = n;
                board[y0][squarex] = 0;
                return false;
            }
            board[y0][x0] = n;
            board[y0][squarex] = 0;
        }
        return true;
    }
    return false;
}

bool pin(int n, int y0, int x0, int y1, int x1){
    board[y0][x0] = 0;
    int movetosquare = board[y1][x1];
    board[y1][x1] = n;
    //checks if you can prevent the mate in next turn 
    //by enpassanting the checking piece
    int enpassanted = -100;
    if(enpassant >= 0 && x1*8+y1 == enpassant){
        enpassanted = board[y1-intsign(y1 - y0)][x1];
        board[y1-intsign(y1 - y0)][x1] = 0;
    }
    if(!check(intsign(n)*50)){
        board[y0][x0] = n;
        board[y1][x1] = movetosquare;
        if(enpassanted != -100){
            board[y1-intsign(y1 - y0)][x1] = enpassanted;
        }
        return false;
    }
    board[y0][x0] = n;
    board[y1][x1] = movetosquare;
    if(enpassanted != -100){
        board[y1-intsign(y1 - y0)][x1] = enpassanted;
    }
    return true;
}

bool canmove(int n, int y0, int x0, int y1, int x1){
    if((piecemove(n, y0, x0, y1, x1) 
    || (abs(n) == 50 && castle(n, y0, x0, y1, x1)))
    && !pin(n, y0, x0, y1, x1)){
        return true;
    }
    return false;
}

bool movesomewhere(int n, int y0, int x0){
    for(int y1 = 0; y1 < 8; y1++){
        for(int x1 = 0; x1 < 8; x1++){
            if(canmove(n, y0, x0, y1, x1)){
                return true;
            }
        }
    }
    return false;
}

bool checkmate(int n){
    if(!check(n)){
        return false;
    }
    for(int n1 = 0; n1 < 6; n1++){
        for(int n2 = 0; n2 < pieces[n1][(n < 0)]; n2++){
            int piecen = intsign(n)*(n1*10+n2);
            int *pindex;
            pindex = std::find(&board[0][0], &board[0][0]+64, piecen);
            if(pindex != &board[0][0]+64){
                int ppos = std::distance(&board[0][0], pindex);
                int y0 = ppos/8;
                int x0 = ppos-y0*8;
                if(movesomewhere(piecen, y0, x0)){
                    return false;
                }
            }
        }
    }
    return true;
}

void movepieceto(int n, int y0, int x0, int y1, int x1,
    bool addposition = true){
    int promoteto;
    if(abs(n) == 50){
        if(abs(x1-x0) > 1){
            //castle
            int whichrook = intsign(n)*(30 + (x1 > 4));
            int rookx = (x1 > 4)*7;
            board[y1][rookx] = 0;
            board[y1][x1 + intsign(4-x1)] = whichrook;
        }
        kingmoved[(n>0)] = 1;
    }
    if(abs(n) == 30 || abs(n) == 31){
        rookmoved[(n>0)][abs(n)-30] = 1;
    }
    if(promote(n, y1)){
        promoteto = 4;
        board[y1][x1] = intsign(n)*(promoteto*10+pieces[promoteto][(n < 0)]);
        pieces[promoteto][(n < 0)]++;
    }else{
        board[y1][x1] = n;
    }
    if(enpassant >= 0 && x1*8+y1 == enpassant){
        board[y1-intsign(y1 - y0)][x1] = 0;
    }
    if(abs(n) < 10 && abs(y1-y0) > 1){
        enpassant = x1*8+y0+intsign(y1 - y0);
    }else{
        enpassant = -1;
    }
    board[y0][x0] = 0;
    moves += 1;
    if(addposition){
        add_to_positions();
    }
}

bool stalemate(int n){
    if(check(n)){
        return false;
    }
    for(int n1 = 1; n1 < 51; n1++){
        int piecen = intsign(n)*n1;
        int *pindex;
        pindex = std::find(&board[0][0], &board[0][0]+64, piecen);
        if(pindex != &board[0][0]+64){
            int ppos = std::distance(&board[0][0], pindex);
            int y0 = ppos/8;
            int x0 = ppos-y0*8;
            if(movesomewhere(piecen, y0, x0)){
                return false;
            }
        }
    }
    return true;
}

bool compareposition(int moment){
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            if(positions[moment][y][x] != board[y][x]){
                return false;
            }
        }
    }
    return true;
}

bool repetition(int this_moment){
    int repetitions = 0;
    for(int moment = this_moment%2; moment < this_moment; moment += 2){
        if(compareposition(moment)){
            repetitions++;
            if(repetitions >= 2){
                return true;
            }
        }
    }
    return false;
}

void gameend(){
    if(checkmate(-50)){
        std::cout << "White won" << '\n';
        turn = -1;
    }
    if(checkmate(50)){
        std::cout << "Black won" << '\n';
    }
    if((turn == 0 && stalemate(50)) || (turn == 1 && stalemate(-50))
        || repetition(moves)){
            std::cout << "Draw" << '\n';
            turn = -1;
    }
        
}

void movepiece(){
    int move, movetox, movetoy;
    std::cin >> move;
    std::cin >> movetoy;
    std::cin >> movetox;
    if((move > 0 && turn == 0) || (move < 0 && turn == 1)){
        int *index;
        index = std::find(&board[0][0], &board[0][0]+64,move);
        if(index != &board[0][0]+64){
            int pos = std::distance(&board[0][0], index);
            int y0 = pos/8;
            int x0 = pos-y0*8;
            if(canmove(move, y0, x0, movetoy, movetox)){
                movepieceto(move, y0, x0, movetoy, movetox);
            }else{
                std::cout << "Illegal move" << "\n";
                return;
            }
        }else{
            std::cout << "Illegal move" << "\n";
            return;
        }
        turn = (turn == 0);
    }else{
        std::cout << "Illegal move" << "\n";
    }
}
