
int board[8][8] = {{30,10,20,50,40,21,11,31},{1,2,3,4,5,6,7,8},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0},{-1,-2,-3,-4,-5,-6,-7,-8},{-30,-10,-20,-50,-40,-21,-11,-31}};
int moves = 0;
std::vector<std::vector<std::vector<int>>> positions;
int turn = 0;
int enpassant = -1;
//[n-1][color][coordinate], white == 0, black == 1, y == 0, x == 1
int piece_positions[50][2][2];
//white == 0, black == 1
int bot = 0;
int castled[2] = {0,0};
bool promotemenu = false;
double evalscore = 0.0;
std::string evaltext = "";
//coordinates where it could be possible for a given piece to move to
std::vector<std::vector<std::vector<std::vector<int>>>> can_move_positions;

//pawns, knights, bishops, rooks, queens and kings (W,B)
int pieces[6][2] = {{8,8},{2,2},{2,2},{2,2},{1,1},{1,1}};
//black, white
int kingmoved[2] = {0,0};
//black left, right - white left, right
int rookmoved[2][2] = {{0,0},{0,0}};

void locate_pieces(){
    for(int n = 1; n < 51; n++){
        int *piece_index = std::find(&board[0][0], &board[0][0]+64, n);
        if(piece_index != &board[0][0]+64){
            int ppos = std::distance(&board[0][0], piece_index);
            int y0 = ppos/8;
            int x0 = ppos-y0*8;
            piece_positions[n-1][0][0] = y0;
            piece_positions[n-1][0][1] = x0;
        }else{
            piece_positions[n-1][0][0] = -1;
            piece_positions[n-1][0][1] = -1;
        }
        piece_index = std::find(&board[0][0], &board[0][0]+64, -n);
        if(piece_index != &board[0][0]+64){
            int ppos = std::distance(&board[0][0], piece_index);
            int y0 = ppos/8;
            int x0 = ppos-y0*8;
            piece_positions[n-1][1][0] = y0;
            piece_positions[n-1][1][1] = x0;
        }else{
            piece_positions[n-1][1][0] = -1;
        }
    }
}

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
    return (a > 0)-(a <= 0);
}

bool pawnmove(int n, int y0, int x0, int y1, int x1){
    int piece_sign = int(n>1)-int(n<1);
    if(n > 0){
        if((x1 == x0 && (y1-y0 == 1 || (y1-y0 == 2 && y0 == 1 
            && board[y1-1][x1] == 0)) && board[y1][x1] == 0) || 
            (y1-y0 == 1 && board[y1][x1] < 0 && 
            (x1 - x0 == 1 || x1 - x0 == -1)) 
            || (x1*8+y1 == enpassant && abs(x1-x0) == 1 && y1 - y0 == 1)){
                return true;
        }
        return false;
    }if(n < 0){
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
    int y = y0, x = x0, xplus = (x1 != x0)*intsign(x1-x0)
    , yplus = (y1 != y0)*intsign(y1-y0);
    if(n > 0){
        if(board[y1][x1] > 0){
            return false;
        }
        for(int i = 1; i < 8; i++){
            y += yplus;
            x += xplus;
            if(y == y1 && x == x1){
                return true;
            }
            if(board[y][x] != 0){
                return false;
            }
        }
        return false;
    }
    if(n < 0){
        if(board[y1][x1] < 0){
            return false;
        }
        for(int i = 1; i < 8; i++){
            y += yplus;
            x += xplus;
            if(y == y1 && x == x1){
                return true;
            }
            if(board[y][x] != 0){
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

bool check(int n, int kingy = -1, int kingx = -1){
    if(kingy == -1){
        int *index = std::find(&board[0][0], &board[0][0]+64, n);
        if(index != &board[0][0]+64){
            int kingpos = std::distance(&board[0][0], index);
            kingy = kingpos/8;
            kingx = kingpos-kingy*8;
        }else{
            return true;
        }
    }
    for(int n1 = 0; n1 < 6; n1++){
        for(int n2 = 0; n2 < pieces[n1][(n > 0)]; n2++){
            int piecen = -intsign(n)*(n1*10+n2+(n1 == 0));
            if(piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                int y0 = piece_positions[abs(piecen)-1][int(piecen<0)][0];
                int x0 = piece_positions[abs(piecen)-1][int(piecen<0)][1];
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
            piece_positions[49][int(n<0)][1] = squarex;
            if(i < 3 && check(n)){
                board[y0][x0] = n;
                board[y0][squarex] = 0;
                piece_positions[49][int(n<0)][1] = x0;
                return false;
            }
            board[y0][x0] = n;
            board[y0][squarex] = 0;
            piece_positions[49][int(n<0)][1] = x0;
        }
        return true;
    }
    return false;
}

bool pin(int n, int y0, int x0, int y1, int x1, int kingy, int kingx){
    board[y0][x0] = 0;
    int movetosquare = board[y1][x1];
    board[y1][x1] = n;
    piece_positions[abs(n)-1][int(n<0)][0] = y1;
    piece_positions[abs(n)-1][int(n<0)][1] = x1;
    piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = -1;
    //checks if you can prevent the mate in next turn 
    //by enpassanting the checking piece
    int enpassanted = -100;
    if(enpassant >= 0 && x1*8+y1 == enpassant){
        enpassanted = board[y1-intsign(y1 - y0)][x1];
        board[y1-intsign(y1 - y0)][x1] = 0;
        piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = -1;
    }
    if(!check(intsign(n)*50, kingy, kingx)){
        board[y0][x0] = n;
        board[y1][x1] = movetosquare;
        piece_positions[abs(n)-1][int(n<0)][0] = y0;
        piece_positions[abs(n)-1][int(n<0)][1] = x0;
        piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = y1;
        if(enpassanted != -100){
            board[y1-intsign(y1 - y0)][x1] = enpassanted;
            piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = y1-intsign(y1 - y0);
        }
        return false;
    }
    board[y0][x0] = n;
    board[y1][x1] = movetosquare;
    piece_positions[abs(n)-1][int(n<0)][0] = y0;
    piece_positions[abs(n)-1][int(n<0)][1] = x0;
    piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = y1;
    if(enpassanted != -100){
        board[y1-intsign(y1 - y0)][x1] = enpassanted;
    }
    return true;
}

bool canmove(int n, int y0, int x0, int y1, int x1, int kingy = -1, int kingx = -1){
    if(piecemove(n, y0, x0, y1, x1) 
    || (abs(n) == 50 && castle(n, y0, x0, y1, x1))){
        if(abs(n) == 50){
            kingy = -1;
            kingx = -1;
        }
        if(!pin(n, y0, x0, y1, x1, kingy, kingx)){
            return true;
        }
    }
    return false;
}

bool movesomewhere(int n, int y0, int x0, int kingy = -1, int kingx = -1){
    for(int y1 = 0; y1 < 8; y1++){
        for(int x1 = 0; x1 < 8; x1++){
            if(canmove(n, y0, x0, y1, x1, kingy, kingx)){
                return true;
            }
        }
    }
    return false;
}

bool checkmate(int n, int kingy = -1, int kingx = -1){
    if(!check(n, kingy, kingx)){
        return false;
    }
    for(int n1 = 0; n1 < 6; n1++){
        for(int n2 = 0; n2 < pieces[n1][(n < 0)]; n2++){
            int piecen = intsign(n)*(n1*10+n2);
            if(piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                int y0 = piece_positions[abs(piecen)-1][int(piecen<0)][0];
                int x0 = piece_positions[abs(piecen)-1][int(piecen<0)][1];
                if(movesomewhere(piecen, y0, x0, kingy, kingx)){
                    return false;
                }
            }
        }
    }
    return true;
}

void movepieceto(int n, int y0, int x0, int y1, int x1, bool addposition = true){
    int promoteto;
    if(board[y1][x1] != 0){
        piece_positions[abs(board[y1][x1])-1][int(n>0)][0] = -1;
    }
    if(abs(n) == 50){
        if(abs(x1-x0) > 1){
            //castle
            int whichrook = intsign(n)*(30 + (x1 > 4));
            int rookx = (x1 > 4)*7;
            board[y1][rookx] = 0;
            board[y1][x1 + intsign(4-x1)] = whichrook;
            castled[(n < 0)] = 1;
            piece_positions[abs(whichrook)-1][int(n<0)][0] = y1;
            piece_positions[abs(whichrook)-1][int(n<0)][1] = x1 + intsign(4-x1);
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
        piece_positions[abs(board[y1][x1])-1][int(board[y1][x1]<0)][0] = y1;
        piece_positions[abs(board[y1][x1])-1][int(board[y1][x1]<0)][1] = x1;
        piece_positions[abs(n)-1][int(n<0)][0] = -1;
    }else{
        board[y1][x1] = n;
        piece_positions[abs(n)-1][int(n<0)][0] = y1;
        piece_positions[abs(n)-1][int(n<0)][1] = x1;
    }
    if(enpassant >= 0 && x1*8+y1 == enpassant){
        piece_positions[abs(board[y1-intsign(y1 - y0)][x1])-1][int(n>0)][0] = -1;
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
        if(piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
            int y0 = piece_positions[abs(piecen)-1][int(piecen<0)][0];
            int x0 = piece_positions[abs(piecen)-1][int(piecen<0)][1];
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

void update_can_move_positions(int color, int piece, int y0, int x0){
    if(piece > 9 && piece < 20){
        can_move_positions[piece-1].resize(0);
        can_move_positions[piece-1].push_back({});
        if(y0 > 0){
            if(x0 < 6){
                can_move_positions[piece-1][0].push_back({y0-1, x0+2});
            }
            if(x0 > 1){
                can_move_positions[piece-1][0].push_back({y0-1, x0-2});
            }
            if(y0 > 1){
                if(x0 < 7){
                    can_move_positions[piece-1][0].push_back({y0-2, x0+1});
                }
                if(x0 > 0){
                    can_move_positions[piece-1][0].push_back({y0-2, x0-1});
                }
            }
        }
        if(y0 < 7){
            if(x0 < 6){
                can_move_positions[piece-1][0].push_back({y0+1, x0+2});
            }
            if(x0 > 1){
                can_move_positions[piece-1][0].push_back({y0+1, x0-2});
            }
            if(y0 < 6){
                if(x0 < 7){
                    can_move_positions[piece-1][0].push_back({y0+2, x0+1});
                }
                if(x0 > 0){
                    can_move_positions[piece-1][0].push_back({y0+2, x0-1});
                }
            }
        }
        return;
    }
    if(abs(piece) < 10){
        can_move_positions[piece-1].resize(0);
        can_move_positions[piece-1].push_back({});
        if(color == 0){
            can_move_positions[piece-1][0] = {{y0+1, x0}, {y0+1, x0+1}, {y0+1, x0-1}};
            if(y0 == 1){
                can_move_positions[piece-1][0].push_back({y0+2, x0});
            }
            return;
        }
        if(color == 1){
            can_move_positions[piece-1][0] = {{y0-1, x0}, {y0-1, x0+1}, {y0-1, x0-1}};
            if(y0 == 6){
                can_move_positions[piece-1][0].push_back({y0-2, x0});
            }
            return;
        }
        return;
    }
    if(piece > 19 && piece < 30){
        can_move_positions[piece-1].resize(0);
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0-i >= 0 && y0-i >= 0; i++){
            can_move_positions[piece-1][0].push_back({y0-i, x0-i});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0+i < 8 && y0-i >= 0; i++){
            can_move_positions[piece-1][1].push_back({y0-i, x0+i});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0+i < 8 && y0+i < 8; i++){
            can_move_positions[piece-1][2].push_back({y0+i, x0+i});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0-i >= 0 && y0+i < 8; i++){
            can_move_positions[piece-1][3].push_back({y0+i, x0-i});
        }
        return;
    }
    if(piece > 29 && piece < 40){
        can_move_positions[piece-1].resize(0);
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0-i >= 0; i++){
            can_move_positions[piece-1][0].push_back({y0, x0-i});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0+i < 8; i++){
            can_move_positions[piece-1][1].push_back({y0, x0+i});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; y0-i >= 0; i++){
            can_move_positions[piece-1][2].push_back({y0-i, x0});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; y0+i < 8; i++){
            can_move_positions[piece-1][3].push_back({y0+i, x0});
        }
        return;
    }
    if(piece > 39 && piece < 50){
        can_move_positions[piece-1].resize(0);
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0-i >= 0 && y0-i >= 0; i++){
            can_move_positions[piece-1][0].push_back({y0-i, x0-i});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0+i < 8 && y0-i >= 0; i++){
            can_move_positions[piece-1][1].push_back({y0-i, x0+i});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0+i < 8 && y0+i < 8; i++){
            can_move_positions[piece-1][2].push_back({y0+i, x0+i});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0-i >= 0 && y0+i < 8; i++){
            can_move_positions[piece-1][3].push_back({y0+i, x0-i});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0-i >= 0; i++){
            can_move_positions[piece-1][4].push_back({y0, x0-i});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; x0+i < 8; i++){
            can_move_positions[piece-1][5].push_back({y0, x0+i});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; y0-i >= 0; i++){
            can_move_positions[piece-1][6].push_back({y0-i, x0});
        }
        can_move_positions[piece-1].push_back({});
        for(int i = 1; y0+i < 8; i++){
            can_move_positions[piece-1][7].push_back({y0+i, x0});
        }
        return;
    }
    if(piece == 50){
        can_move_positions[piece-1].resize(0);
        can_move_positions[piece-1].push_back({});
        if(y0 > 0){
            can_move_positions[piece-1][0].push_back({y0-1, x0});
            if(x0 > 0){
               can_move_positions[piece-1][0].push_back({y0-1, x0-1}); 
            }
            if(x0 < 7){
               can_move_positions[piece-1][0].push_back({y0-1, x0+1}); 
            }
        }
        if(x0 > 0){
            can_move_positions[piece-1][0].push_back({y0, x0-1});
            if(x0 == 3){
                can_move_positions[piece-1][0].push_back({y0, x0-2});
                can_move_positions[piece-1][0].push_back({y0, x0+2});
            }
        }
        if(x0 < 7){
            can_move_positions[piece-1][0].push_back({y0, x0+1});
        }
        if(y0 < 7){
            can_move_positions[piece-1][0].push_back({y0+1, x0});
            if(x0 > 0){
               can_move_positions[piece-1][0].push_back({y0+1, x0-1}); 
            }
            if(x0 < 7){
               can_move_positions[piece-1][0].push_back({y0+1, x0+1}); 
            }
        }
        return;
    }
}

void movepiece(){
    std::string input;
    std::cin >> input;
    std::vector<int> move = convert_from_png(input);
    int y0 = move[0];
    int x0 = move[1];
    int movetoy = move[2];
    int movetox = move[3];
    int piece = board[y0][x0];
    std::cout << piece << ',' << y0 << ',' << x0 << ',' << movetoy << ',' << movetox << '\n'; 
    if((piece > 0 && turn == 0) || (piece < 0 && turn == 1)){
        if(movetoy < 8 && movetox < 8 && movetox >= 0 && movetoy >= 0 && y0>=0 
        && canmove(piece, y0, x0, movetoy, movetox)){
            movepieceto(piece, y0, x0, movetoy, movetox);
        }else{
            std::cout << "Illegal move" << "\n";
            return;
        }
        turn = (turn == 0);
    }else{
        std::cout << "Illegal move" << "\n";
    }
}
