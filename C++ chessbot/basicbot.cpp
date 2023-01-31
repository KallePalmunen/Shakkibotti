
int bestmove[6];
std::vector<std::vector<int>> order;

double evaluate_change(int y, int x, int changesign, int n = -100){
    if(n == -100){
        n = board[y][x];
    }
    //n1 and n2 are squares where a threatening pawn could be
    int n1 = board[std::min(7, y + 1)][std::min(7, x+1)]
        *(y != 7 && x != 7);
    int n2 = board[std::min(7, y + 1)][std::max(0, x-1)]
    *(y != 7 && x != 0);
    return changesign*intsign(n)*((n != 0 && abs(n) < 9)
        *(1+(abs(y - 7*(n < 0))*((x > 2 && x < 6)*((x == 5)
        *0.01+(x != 5)*0.05) + 0.001)
        - (moves < 60 && x < 2)*(abs(y - 7*(n < 0)) > 2)*0.03))
        + 3*(abs(n) > 9 && abs(n) < 30) + 
        (5+ (!((y == 0 || y == 7) && (x == 0 || x == 7)))*0.01)
        *(abs(n) > 29 && abs(n) < 40) + 
        9*(abs(n) > 39)+ ((abs(n) >= 10 && abs(n) < 30) 
        || (abs(n) >= 40 && abs(n) < 50))*(y != 7*(n < 0))
        *(0.05+0.001*(abs(y - 7*(n < 0)) > 1))
        + (abs(n) >= 10 && abs(n) < 20 && x > 1 && x < 6)*0.01
        + ((abs(n) == 50) && (y == 0 || y == 7) 
        && (x == 1 || x == 5))*0.05)
        *(1-((n > 0) && ((n1 < 0 && n1 >-9) || (n2 < 0 && n2 >-9)))*0.5);
}

double evaluate_move(int n, int y0, int x0, int y1, int x1){
    return evaluate_change(y1, x1, 1, n) + 
        evaluate_change(y0, x0, -1, n);
}

void reorder(){
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
    std::vector<double> movescore;
    std::vector<std::vector<int>> starting_order;
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
                        int evaluation_minus = evaluate_change(y1, x1, -1);
                        movepieceto(n, y0, x0, y1, x1, true);
                        turn = 1;
                        //repetition check here
                        //partial repetition check here
                        //else
                        movescore.push_back(
                            evaluate_move(n, y0, x0, y1, x1) 
                            + evaluation_minus);
                        starting_order.insert(starting_order.end(),
                            {n, y0, x0, y1, x1});

                        //return to saved state
                        moves = temp_moves;
                        enpassant = temp_enpassant;
                        std::copy(&temp_board[0][0], &temp_board[0][0]+64, 
                        &board[0][0]);
                        std::copy(&temp_kingmoved[0], 
                            &temp_kingmoved[0]+2, &kingmoved[0]);
                        std::copy(&temp_rookmoved[0][0], 
                            &temp_rookmoved[0][0]+4, &rookmoved[0][0]);
                        std::copy(&temp_pieces[0][0], 
                            &temp_pieces[0][0]+12, &pieces[0][0]);
                        positions.pop_back();
                        turn = 0;
                    }
                }
            }
        }
    }
    turn = 0;
    for(int i = 0; i < movescore.size(); i++){
        int maxindex = std::distance(&movescore[0],
            std::max_element(&movescore[0], &movescore[0]+movescore.size()));
        order.push_back(starting_order[maxindex]);
        movescore[maxindex] = -1000000;
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
    std::vector<double> movescore = {};
    reorder();
    for(int i = 0; i < order.size(); i++){
        int n = order[i][0];
        int y0 = order[i][1];
        int x0 = order[i][2];
        int y1 = order[i][3];
        int x1 = order[i][4];
        int evaluation_minus = evaluate_change(y1, x1, -1);
        movepieceto(n, y0, x0, y1, x1, true);
        turn = 1;
        //repetition check here
        //partial repetition check here
        //else
        movescore.push_back(
            evaluate_move(n, y0, x0, y1, x1) 
            + evaluation_minus);

        //return to saved state
        moves = temp_moves;
        enpassant = temp_enpassant;
        std::copy(&temp_board[0][0], &temp_board[0][0]+64, 
        &board[0][0]);
        std::copy(&temp_kingmoved[0], 
            &temp_kingmoved[0]+2, &kingmoved[0]);
        std::copy(&temp_rookmoved[0][0], 
            &temp_rookmoved[0][0]+4, &rookmoved[0][0]);
        std::copy(&temp_pieces[0][0], 
            &temp_pieces[0][0]+12, &pieces[0][0]);
        positions.pop_back();
        turn = 0;
    }
    turn = 0;
    int maxindex = std::distance(std::begin(movescore),
        std::max_element(std::begin(movescore), std::end(movescore)));
    for(int i = 0; i < 5; i++){
        bestmove[i] = order[maxindex][i];
    }
    bestmove[5] = movescore[maxindex];
}

void basicbot(){
    auto start = std::chrono::high_resolution_clock::now();
    whitemove1();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast
        <std::chrono::seconds>(stop - start);
    std::cout << duration.count() << '\n';
    int score = 0;
    score += bestmove[5];
    int n = bestmove[0];
    int y0 = bestmove[1];
    int x0 = bestmove[2];
    int y1 = bestmove[3];
    int x1 = bestmove[4];
    movepieceto(n, y0, x0, y1, x1);
    turn = 1;
}