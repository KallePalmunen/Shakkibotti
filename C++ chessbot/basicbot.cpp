
int bestmove[6];
std::vector<std::vector<int>> order;
int ntimes = 2;

bool partialrepetition(int current_moment){
    for(int moment = current_moment%2; moment < current_moment; moment += 2){
        if(compareposition(moment)){
            return true;
        }
    }
    return false;
}

double evaluate_change(int y, int x, int changesign, int n = -100){
    if(n == -100){
        n = board[y][x];
    }
    return changesign*intsign(n)*(
        //pawns
        (n != 0 && abs(n) < 9)*(1+(abs(y - 7*(n < 0))*((x > 2 && x < 6)
        *((x == 5)*0.01+(x != 5)*0.05) + 0.001)
        - (moves < 60 && x < 2)*(abs(y - 7*(n < 0)) > 2)*0.03)
        //promoting
        +8*(y == 7*(n > 0)))
        //knights and bishops
        + 3*(abs(n) > 9 && abs(n) < 30) + 
        //rooks
        (5+ (!((y == 0 || y == 7) && (x == 0 || x == 7)))*0.01)
        *(abs(n) > 29 && abs(n) < 40) + 
        //queens
        9*(abs(n) > 39)
        //developed
        + ((abs(n) >= 10 && abs(n) < 30) 
        || (abs(n) >= 40 && abs(n) < 50))*(y != 7*(n < 0))
        *(0.05+0.001*(abs(y - 7*(n < 0)) > 1))
        + (abs(n) >= 10 && abs(n) < 20 && x > 1 && x < 6)*0.01);
}

double evaluate_move(int n, int y0, int x0, int y1, int x1){
    return evaluate_change(y1, x1, 1, n) + 
        evaluate_change(y0, x0, -1, n) + (castled[int(n < 0)] == 1)*0.1;
}

double fulleval(){
    double evaluation = 0;
    for(int n = 1; n < 51; n++){
        if(piece_positions[n-1][0][0] != -1){
            int y0 = piece_positions[n-1][0][0];
            int x0 = piece_positions[n-1][0][1];
            evaluation += evaluate_change(y0, x0, 1, n);
        }
        if(piece_positions[n-1][1][0] != -1){
            int y0 = piece_positions[n-1][1][0];
            int x0 = piece_positions[n-1][1][1];
            evaluation += evaluate_change(y0, x0, 1, -n);
        }
    }
    evaluation -= (castled[0]+castled[1]);
    return evaluation;
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
    int temp_castled[2];
    std::copy(&castled[0], &castled[0]+2, 
        &temp_castled[0]);
    int temp_rookmoved[2][2];
    std::copy(&rookmoved[0][0], &rookmoved[0][0]+4, 
        &temp_rookmoved[0][0]);
    int temp_pieces[6][2];
    std::copy(&pieces[0][0], &pieces[0][0]+12, 
        &temp_pieces[0][0]);
    int temp_piece_positions[50][2][2];
    std::copy(&piece_positions[0][0][0], &piece_positions[0][0][0]+200, 
        &temp_piece_positions[0][0][0]);
    std::vector<double> movescore;
    std::vector<std::vector<int>> starting_order;
    for(int n = 1; n < 51; n++){
        if(piece_positions[piece_positions[abs(n)-1][int(n<0)][0] != -1]){
            int y0 = piece_positions[abs(n)-1][int(n<0)][0];
            int x0 = piece_positions[abs(n)-1][int(n<0)][1];
            for(int y1 = 0; y1 < 8; y1++){
                for(int x1 = 0; x1 < 8; x1++){
                    if(canmove(n, y0, x0, y1, x1)){
                        double evaluation_minus = evaluate_change(y1, x1, -1);
                        movepieceto(n, y0, x0, y1, x1, true);
                        if(repetition(moves) || stalemate(50) || stalemate(-50)){
                            movescore.push_back(-fulleval()
                            -(evaluate_move(n, y0, x0, y1, x1)
                            + evaluation_minus));
                        }else if(partialrepetition(moves)){
                            movescore.push_back(std::min(0.0, (-fulleval()
                            -(evaluate_move(n, y0, x0, y1, x1)
                            + evaluation_minus))));
                        }else{
                            movescore.push_back(
                                evaluate_move(n, y0, x0, y1, x1) 
                                + evaluation_minus);
                        }
                        starting_order.insert(starting_order.end(),
                            {n, y0, x0, y1, x1});

                        //return to saved state
                        moves = temp_moves;
                        enpassant = temp_enpassant;
                        std::copy(&temp_board[0][0], &temp_board[0][0]+64, 
                        &board[0][0]);
                        std::copy(&temp_kingmoved[0], 
                            &temp_kingmoved[0]+2, &kingmoved[0]);
                        std::copy(&temp_castled[0], &temp_castled[0]+2, 
                            &castled[0]);
                        std::copy(&temp_rookmoved[0][0], 
                            &temp_rookmoved[0][0]+4, &rookmoved[0][0]);
                        std::copy(&temp_pieces[0][0], 
                            &temp_pieces[0][0]+12, &pieces[0][0]);
                        std::copy(&temp_piece_positions[0][0][0], &temp_piece_positions[0][0][0]+200, 
                            &piece_positions[0][0][0]);
                        positions.pop_back();
                    }
                }
            }
        }
    }
    for(int i = 0; i < movescore.size(); i++){
        int maxindex = std::distance(&movescore[0],
            std::max_element(&movescore[0], &movescore[0]+movescore.size()));
        order.push_back(starting_order[maxindex]);
        movescore[maxindex] = -1000000;
    }
}

double last_move(int n0, int y00, int x00, int y10, int x10, double best){
    int piece_sign = int(bot == 1)-int(bot == 0);
    int kingy = piece_positions[49][1][0];
    int kingx = piece_positions[49][1][1];
    if(kingy == -1){
        return -piece_sign*500000/(ntimes+1.0);
    }
    if(checkmate(piece_sign*50, kingy, kingx)){
        return -piece_sign*500000/(ntimes+1.0);
    }
    double movescore[332];
    int movescore_size = 0;
    double best_movescore = -piece_sign*1000000;
    double previous_movescore = evaluate_move(n0, y00, x00, y10, x10);
    for(int n0 = 0; n0 < 6; n0++){
        int n1 = int(1*(n0 == 0)+2*(n0 == 1)+3*(n0 == 2)+4*(n0 == 3)
        + 5*(n0 == 5));
        for(int n2 = 0; n2 < pieces[n1][1]; n2++){
            int n = piece_sign*(10*n1+n2+int(n1 == 0));
            if(piece_positions[piece_positions[abs(n)-1][int(n<0)][0] != -1]){
                int y0 = piece_positions[abs(n)-1][int(n<0)][0];
                int x0 = piece_positions[abs(n)-1][int(n<0)][1];
                for(int y1 = 0; y1 < 8; y1++){
                    for(int x1 = 0; x1 < 8; x1++){
                        if(canmove(n, y0, x0, y1, x1, kingy, kingx)){
                            double evaluation_minus = evaluate_change(y1, x1, -1);
                            double current_movescore = evaluate_move(n, y0, x0, y1, x1)
                                + evaluation_minus;
                            double total_movescore = current_movescore 
                                + previous_movescore;
                            if((total_movescore <= best && bot == 0)
                                || (total_movescore >= best && bot == 1)){
                                return total_movescore;
                            }
                            if((total_movescore < best_movescore && bot == 0)
                                || (total_movescore > best_movescore && bot == 1)){
                                best_movescore = total_movescore;
                            }
                            movescore[movescore_size] = total_movescore;
                            movescore_size++;
                        }
                    }
                }
            }
        }
    }
    if(movescore_size == 0){
        return 0.0;
    }else if(bot == 0){
        return *std::min_element(&movescore[0], &movescore[0]+movescore_size);
    }else{
       return *std::max_element(&movescore[0], &movescore[0]+movescore_size); 
    }
}

double nth_move(int n0, int y00, int x00, int y10, int x10, double best, int nmoremoves){
    int piece_sign = (nmoremoves%2 == 1)-(nmoremoves%2 == 0);
    int kingy = piece_positions[49][1][0];
    int kingx = piece_positions[49][1][1];
    if(kingy == -1){
        return -piece_sign*500000/(ntimes+1.0);
    }
    if(checkmate(piece_sign*50, kingy, kingx)){
        return -piece_sign*500000/(ntimes-nmoremoves+1.0);
    }
    int temp_moves = moves;
    int temp_enpassant = enpassant;
    int temp_board[8][8];
    std::copy(&board[0][0], &board[0][0]+64, &temp_board[0][0]);
    int temp_kingmoved[2];
    std::copy(&kingmoved[0], &kingmoved[0]+2, 
        &temp_kingmoved[0]);
    int temp_castled[2];
    std::copy(&castled[0], &castled[0]+2, 
        &temp_castled[0]);
    int temp_rookmoved[2][2];
    std::copy(&rookmoved[0][0], &rookmoved[0][0]+4, 
        &temp_rookmoved[0][0]);
    int temp_pieces[6][2];
    std::copy(&pieces[0][0], &pieces[0][0]+12, 
        &temp_pieces[0][0]);
    int temp_piece_positions[50][2][2];
    std::copy(&piece_positions[0][0][0], &piece_positions[0][0][0]+200, 
        &temp_piece_positions[0][0][0]);
    double movescore[332];
    int movescore_size = 0;
    double best_movescore = -piece_sign*1000000;
    double previous_movescore = evaluate_move(n0, y00, x00, y10, x10);
    for(int n0 = 0; n0 < 6; n0++){
        int n1 = int(1*(n0 == 0)+2*(n0 == 1)+3*(n0 == 2)+4*(n0 == 3)
        + 5*(n0 == 5));
        for(int n2 = 0; n2 < pieces[n1][1]; n2++){
            int n = piece_sign*(10*n1+n2+int(n1 == 0));
            if(piece_positions[piece_positions[abs(n)-1][int(n<0)][0] != -1]){
                int y0 = piece_positions[abs(n)-1][int(n<0)][0];
                int x0 = piece_positions[abs(n)-1][int(n<0)][1];
                for(int y1 = 0; y1 < 8; y1++){
                    for(int x1 = 0; x1 < 8; x1++){
                        if(canmove(n, y0, x0, y1, x1, kingy, kingx)){
                            double evaluation_minus = evaluate_change(y1, x1, -1);
                            double current_movescore;
                            movepieceto(n, y0, x0, y1, x1, false);
                            if(nmoremoves == 1){
                                current_movescore = last_move(n, y0, x0, y1, x1, 
                                    best_movescore-evaluation_minus) + evaluation_minus;
                            }else{
                                current_movescore = nth_move(n, y0, x0, y1, x1, 
                                    best_movescore-evaluation_minus, nmoremoves-1) + evaluation_minus;
                            }
                            double total_movescore = current_movescore 
                                + previous_movescore;
                            if((total_movescore <= best && nmoremoves%2 == 0)
                                || (total_movescore >= best && nmoremoves%2 == 1)){
                                return total_movescore;
                            }
                            if((total_movescore < best_movescore && nmoremoves%2 == 0)
                                || (total_movescore > best_movescore && nmoremoves%2 == 1)){
                                best_movescore = total_movescore;
                            }
                            movescore[movescore_size] = total_movescore;
                            movescore_size++;
                            //return to saved state
                            moves = temp_moves;
                            enpassant = temp_enpassant;
                            std::copy(&temp_board[0][0], &temp_board[0][0]+64, 
                            &board[0][0]);
                            std::copy(&temp_piece_positions[0][0][0], &temp_piece_positions[0][0][0]+200, 
                            &piece_positions[0][0][0]);
                            std::copy(&temp_kingmoved[0], 
                                &temp_kingmoved[0]+2, &kingmoved[0]);
                            std::copy(&temp_castled[0], &temp_castled[0]+2, 
                                &castled[0]);
                            std::copy(&temp_rookmoved[0][0], 
                                &temp_rookmoved[0][0]+4, &rookmoved[0][0]);
                            std::copy(&temp_pieces[0][0], 
                                &temp_pieces[0][0]+12, &pieces[0][0]);
                        }
                    }
                }
            }
        }
    }
    if(movescore_size == 0){
        return 0.0;
    }else if(nmoremoves%2 == 0){
        return *std::min_element(&movescore[0], &movescore[0]+movescore_size);
    }else{
       return *std::max_element(&movescore[0], &movescore[0]+movescore_size); 
    }
}

void whitemove1(){
    //save current state
    order.clear();
    int temp_moves = moves;
    int temp_enpassant = enpassant;
    int temp_board[8][8];
    std::copy(&board[0][0], &board[0][0]+64, &temp_board[0][0]);
    int temp_kingmoved[2];
    std::copy(&kingmoved[0], &kingmoved[0]+2, 
        &temp_kingmoved[0]);
    int temp_castled[2];
    std::copy(&castled[0], &castled[0]+2, 
        &temp_castled[0]);
    int temp_rookmoved[2][2];
    std::copy(&rookmoved[0][0], &rookmoved[0][0]+4, 
        &temp_rookmoved[0][0]);
    int temp_pieces[6][2];
    std::copy(&pieces[0][0], &pieces[0][0]+12, 
        &temp_pieces[0][0]);
    int temp_piece_positions[50][2][2];
    std::copy(&piece_positions[0][0][0], &piece_positions[0][0][0]+200, 
        &temp_piece_positions[0][0][0]);
    std::vector<double> movescore;
    double best_movescore = -1000000;
    reorder();
    for(int i = 0; i < order.size(); i++){
        int n = order[i][0];
        int y0 = order[i][1];
        int x0 = order[i][2];
        int y1 = order[i][3];
        int x1 = order[i][4];
        double evaluation_minus = evaluate_change(y1, x1, -1);
        movepieceto(n, y0, x0, y1, x1, true);
        if(repetition(moves) || stalemate(50) || stalemate(-50)){
            movescore.push_back(-fulleval()
            -(nth_move(n, y0, x0, y1, x1, 
            best_movescore-evaluation_minus, ntimes)
            + evaluation_minus));
        }else if(partialrepetition(moves)){
            movescore.push_back(std::min(0.0, (-fulleval()
            -(nth_move(n, y0, x0, y1, x1, 
            best_movescore-evaluation_minus, ntimes)
            + evaluation_minus))));
        }else{
            double current_movescore = 
                nth_move(n, y0, x0, y1, x1, 
                best_movescore-evaluation_minus, ntimes) + evaluation_minus;
            movescore.push_back(current_movescore);
            if(current_movescore > best_movescore){
                best_movescore = current_movescore;
            }
        }

        //return to saved state
        moves = temp_moves;
        enpassant = temp_enpassant;
        std::copy(&temp_board[0][0], &temp_board[0][0]+64, 
        &board[0][0]);
        std::copy(&temp_kingmoved[0], 
            &temp_kingmoved[0]+2, &kingmoved[0]);
        std::copy(&temp_castled[0], &temp_castled[0]+2, 
            &castled[0]);
        std::copy(&temp_rookmoved[0][0], 
            &temp_rookmoved[0][0]+4, &rookmoved[0][0]);
        std::copy(&temp_pieces[0][0], 
            &temp_pieces[0][0]+12, &pieces[0][0]);
        std::copy(&temp_piece_positions[0][0][0], &temp_piece_positions[0][0][0]+200, 
            &piece_positions[0][0][0]);
        positions.pop_back();
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
    double score = fulleval();
    auto start = std::chrono::high_resolution_clock::now();
    whitemove1();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast
        <std::chrono::milliseconds>(stop - start);
    while(duration.count()/1000.0 < 0.4){
        ntimes += 2;
        whitemove1();
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast
            <std::chrono::milliseconds>(stop - start);
    }
    std::cout << "depth = " << ntimes/2+1 << '\n';
    if(ntimes > 4){
        ntimes = 4;
    }
    score += bestmove[5];
    int n = bestmove[0];
    int y0 = bestmove[1];
    int x0 = bestmove[2];
    int y1 = bestmove[3];
    int x1 = bestmove[4];
    movepieceto(n, y0, x0, y1, x1);
    turn = 1;
    printboard();
    std::cout << duration.count()/1000.0 << '\n';
    std::cout << '(' << n << ',' << y1 << ',' << x1 << ')' << score << '\n';
}