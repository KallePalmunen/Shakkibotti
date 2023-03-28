
std::vector<std::vector<int>> bestmove;
int ntimesmin = 4;
//ntimes == (amount of half moves that basicbot searches forward) - 2
int ntimes = ntimesmin;
//amount of moves calculated one full move deeper
int plusamount = 2;
double pawn_position_eval[8][8] = {{80.0,80.0,80.0,80.0,80.0,80.0,80.0,80.0},{0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5}
,{0.1,0.1,0.2,0.3,0.3,0.2,0.1,0.1},{0.05,0.05,0.1,0.25,0.25,0.1,0.05,0.05},{0.0,0.0,0.0,0.2,0.2,0.0,0.0,0.0}
,{0.05,-0.05,-0.1,0.0,0.0,-0.1,-0.05,0.05},{0.05,0.1,0.1,-0.2,-0.2,0.1,0.1,0.05},{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}};
double knight_position_eval[8][8] = {{-0.5,-0.4,-0.3,-0.3,-0.3,-0.3,-0.4,-0.5},{-0.4,-0.2,0.0,0.0,0.0,0.0,-0.2,-0.4}
,{-0.3,0.0,0.1,0.15,0.15,0.1,0.0,-0.3},{-0.3,0.05,0.15,0.2,0.2,0.15,0.05,-0.3},{-0.3,0.05,0.15,0.2,0.2,0.15,0.05,-0.3}
,{-0.3,0.0,0.1,0.15,0.15,0.1,0.0,-0.3},{-0.4,-0.2,0.0,0.0,0.0,0.0,-0.2,-0.4},{-0.5,-0.4,-0.3,-0.3,-0.3,-0.3,-0.4,-0.5}};
double bishop_position_eval[8][8] = {{-0.2,-0.1,-0.1,-0.1,-0.1,-0.1,-0.1,-0.2},{-0.1,0.0,0.0,0.0,0.0,0.0,0.0,-0.1}
,{-0.1,0.0,0.05,0.1,0.1,0.05,0.0,-0.1},{-0.1,0.05,0.05,0.1,0.1,0.05,0.05,-0.1},{-0.1,0.0,0.1,0.1,0.1,0.1,0.0,-0.1}
,{-0.1,0.1,0.01,0.1,0.1,0.1,0.1,-0.1},{-0.1,0.05,0.0,0.0,0.0,0.0,0.05,-0.1},{-0.2,-0.1,-0.1,-0.1,-0.1,-0.1,-0.1,-0.2}};
double rook_position_eval[8][8] = {{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},{0.05,0.1,0.1,0.1,0.1,0.1,0.1,0.05}
,{-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05},{-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05},{-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05}
,{-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05},{-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05},{0.0,0.0,0.0,0.5,0.5,0.0,0.0,0.0}};
double queen_position_eval[8][8] = {{-0.2,-0.1,-0.1,-0.05,-0.05,-0.1,-0.1,-0.2},{-0.1,0.0,0.0,0.0,0.0,0.0,0.0,-0.1}
,{-0.1,0.0,0.05,0.05,0.05,0.05,0.0,-0.1},{-0.05,0.0,0.05,0.05,0.05,0.05,0.0,-0.05},{0.0,0.0,0.05,0.05,0.05,0.05,0.0,-0.05}
,{-0.1,0.05,0.05,0.05,0.05,0.05,0.0,-0.1},{-0.1,0.0,0.05,0.0,0.0,0.0,0.0,-0.1},{-0.2,-0.1,-0.1,-0.05,-0.05,-0.1,-0.1,-0.2}};
double king_position_eval[8][8] = {{-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3},{-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3}
,{-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3},{-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3},{-0.2,-0.3,-0.3,-0.4,-0.4,-0.3,-0.3,-0.2}
,{-0.1,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.1},{0.2,0.2,0.0,0.0,0.0,0.0,0.2,0.2},{0.2,0.3,0.1,0.0,0.0,0.1,0.3,0.2}};

bool partialrepetition(int current_moment){
    for(int moment = current_moment%2; moment < current_moment; moment += 2){
        if(compareposition(moment)){
            return true;
        }
    }
    return false;
}

void set_can_move_positions(){
    can_move_positions.resize(0);
    for(int n = 1; n < 51; n++){
        can_move_positions.push_back({});
        update_can_move_positions(int(bot == 0), n, piece_positions[n-1][int(bot ==0)][0]
        , piece_positions[n-1][int(bot == 0)][1]);
    }
}

void get_pinners(int piece_sign, int kingy, int kingx){
    pinners.clear();
    for(int n1 = 0; n1 < 6; n1++){
        for(int n2 = 0; n2 < pieces[n1][piece_sign!=1]; n2++){
            int n = piece_sign*(10*n1+n2+int(n1 == 0));
            int y0 = piece_positions[abs(n)-1][int(n<0)][0];
            int x0 = piece_positions[abs(n)-1][int(n<0)][1];
            if(piecemove(n, y0, x0, kingy, kingx)){
                pinners.push_back(n);
            }
        }
    }
}

double evaluate_change(int y, int x, int changesign, int n = -100){
    if(n == -100){
        n = board[y][x];
    }
    if(n == 0){
        return 0.0;
    }
    //pawns
    if(n != 0 && abs(n) < 9){
        return changesign*intsign(n)*(1+0.1*pawn_position_eval[(n<0)*y+(n>0)*(7-y)][(n<0)*x+(n>0)*(7-x)]);
    }
    //knights and bishops
    if(abs(n) > 9 && abs(n) < 20){
        return changesign*intsign(n)*(3+0.1*knight_position_eval[(n<0)*y+(n>0)*(7-y)][(n<0)*x+(n>0)*(7-x)]);
    }
    //bishops
    if(abs(n) > 19 && abs(n) < 30){
        return changesign*intsign(n)*(3+0.1*bishop_position_eval[(n<0)*y+(n>0)*(7-y)][(n<0)*x+(n>0)*(7-x)]);
    }
    //rooks
    if(abs(n) > 29 && abs(n) < 40){
        return changesign*intsign(n)*(5+0.1*rook_position_eval[(n<0)*y+(n>0)*(7-y)][(n<0)*x+(n>0)*(7-x)]);
    }
    //queens
    if(abs(n) > 39 && abs(n) < 50){
        return changesign*intsign(n)*(9+0.1*queen_position_eval[(n<0)*y+(n>0)*(7-y)][(n<0)*x+(n>0)*(7-x)]);
    }
    return 0.0;
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

std::vector<std::vector<int>> reorder(){
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
    for(int n1 = 1; n1 < 51; n1++){
        int n = n1; 
        if(bot == 1){n=-n1;}
        if(piece_positions[abs(n)-1][int(n<0)][0] != -1){
            int y0 = piece_positions[abs(n)-1][int(n<0)][0];
            int x0 = piece_positions[abs(n)-1][int(n<0)][1];
            for(int y1 = 0; y1 < 8; y1++){
                for(int x1 = 0; x1 < 8; x1++){
                    if(canmove(n, y0, x0, y1, x1)){
                        double evaluation_minus = evaluate_change(y1, x1, -1)+(n < 9 && x1*8+y1 == enpassant)*intsign(n)*1.0;
                        movepieceto(n, y0, x0, y1, x1, true);
                        if(repetition(moves) || stalemate(50) || stalemate(-50)){
                            movescore.push_back(-fulleval());
                        }else if(partialrepetition(moves)){
                            if(bot == 0){
                                movescore.push_back(std::min(evaluate_move(n, y0, x0, y1, x1) 
                                + evaluation_minus, -fulleval()));
                            }else{
                                movescore.push_back(std::max(evaluate_move(n, y0, x0, y1, x1) 
                                + evaluation_minus, -fulleval()));
                            }    
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
    std::vector<std::vector<int>> return_vector;
    int index;
    for(int i = 0; i < movescore.size(); i++){
        if(bot==0){
            int maxindex = std::distance(&movescore[0],
                std::max_element(&movescore[0], &movescore[0]+movescore.size()));
            return_vector.push_back(starting_order[maxindex]);
            index = maxindex;
        }else{
            int minindex = std::distance(&movescore[0],
                std::min_element(&movescore[0], &movescore[0]+movescore.size()));
            return_vector.push_back(starting_order[minindex]);
            index = minindex;
        }
        movescore[index] = -1000000*intsign(bot==0);
    }
    return return_vector;
}

double last_move(int n0, int y00, int x00, int y10, int x10, double best){
    int piece_sign = int(bot == 1)-int(bot == 0);
    int kingy = piece_positions[49][int(n0>0)][0];
    int kingx = piece_positions[49][int(n0>0)][1];
    if(kingy == -1){
        return -piece_sign*500000/(ntimes+1.0);
    }
    get_pinners(-piece_sign, kingy, kingx);
    if(botcheckmate(piece_sign*50, kingy, kingx)){
        return -piece_sign*500000/(ntimes+1.0);
    }
    double previous_movescore = evaluate_move(n0, y00, x00, y10, x10);
    double movescore[304];
    int movescore_size = 0;
    double best_movescore = -piece_sign*1000000;
    for(int n00 = 0; n00 < 6; n00++){
        //Goes through the piece types in peculiar order
        int n1 = int(1*(n00 == 0)+2*(n00 == 1)+3*(n00 == 2)+4*(n00 == 3)
        + 5*(n00 == 5));
        for(int n2 = 0; n2 < pieces[n1][piece_sign!=1]; n2++){
            int n = piece_sign*(10*n1+n2+int(n1 == 0));
            if(piece_positions[abs(n)-1][int(n<0)][0] != -1){
                int y0 = piece_positions[abs(n)-1][int(n<0)][0];
                int x0 = piece_positions[abs(n)-1][int(n<0)][1];
                bool pinnable = ispinnable(n, y0, x0, kingy, kingx);
                for(int i = 0; i < can_move_positions[abs(n)-1].size(); i++){
                    for(int j = 0; j < can_move_positions[abs(n)-1][i].size(); j+=2){
                        int y1 = can_move_positions[abs(n)-1][i][j];
                        int x1 = can_move_positions[abs(n)-1][i][j+1];
                        if(botcanmove(n, y0, x0, y1, x1, pinnable, kingy, kingx)){
                            double evaluation_minus = evaluate_change(y1, x1, -1)+(n < 9 && x1*8+y1 == enpassant)*intsign(n)*1.0;
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
                            if(abs(n) > 19 && abs(n) < 50 && board[y1][x1] != 0){
                                break;
                            }
                        }else if(abs(n) > 19 && abs(n) < 50 && board[y1][x1] != 0){
                            break;
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
    int piece_sign = (intsign(bot==0))*((nmoremoves%2 == 1)-(nmoremoves%2 == 0));
    //white == 0, black == 1
    int color = int(piece_sign!=1);
    int kingy = piece_positions[49][int(n0>0)][0];
    int kingx = piece_positions[49][int(n0>0)][1];
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
    for(int n00 = 0; n00 < 6; n00++){
        int n1 = int(1*(n00 == 0)+2*(n00 == 1)+3*(n00 == 2)+4*(n00 == 3)
        + 5*(n00 == 5));
        for(int n2 = 0; n2 < pieces[n1][piece_sign!=1]; n2++){
            int n = piece_sign*(10*n1+n2+int(n1 == 0));
            if(piece_positions[abs(n)-1][int(n<0)][0] != -1){
                int y0 = piece_positions[abs(n)-1][int(n<0)][0];
                int x0 = piece_positions[abs(n)-1][int(n<0)][1];
                for(int y1 = 0; y1 < 8; y1++){
                    for(int x1 = 0; x1 < 8; x1++){
                        if(canmove(n, y0, x0, y1, x1, kingy, kingx)){
                            if(nmoremoves%2 == 0){
                                update_can_move_positions(color, abs(n), y1, x1);
                            }
                            double evaluation_minus = evaluate_change(y1, x1, -1)+(n < 9 && x1*8+y1 == enpassant)*intsign(n)*1.0;
                            double current_movescore;
                            movepieceto(n, y0, x0, y1, x1, false);
                            if(nmoremoves%2 == 0 && abs(n) < 10 && ((color == 0 && y1 == 7) 
                            || (color == 1 && y1 == 0))){
                                update_can_move_positions(color, abs(board[y1][x1]), y1, x1);
                            }
                            if(abs(n) == 50 && abs(x1-x0) > 1 && nmoremoves%2 == 0){
                                update_can_move_positions(color, 30
                                , piece_positions[30-1][color][0], piece_positions[30-1][color][1]);
                                update_can_move_positions(color, 31
                                , piece_positions[31-1][color][0], piece_positions[31-1][color][1]);
                            }
                            if(nmoremoves == 1){
                                current_movescore = last_move(n, y0, x0, y1, x1, 
                                    best_movescore-evaluation_minus) + evaluation_minus;
                            }else{
                                current_movescore = nth_move(n, y0, x0, y1, x1, 
                                    best_movescore-evaluation_minus, nmoremoves-1) + evaluation_minus;
                            }
                            double total_movescore = current_movescore 
                                + previous_movescore;
                            //restore can_move_positions
                            if(nmoremoves%2 == 0){
                                update_can_move_positions(color, abs(n), y0, x0);
                            }
                            if(abs(n) == 50 && abs(x1-x0) > 1 && nmoremoves%2 == 0){
                                update_can_move_positions(color, 30
                                , piece_positions[30-1][color][0], piece_positions[30-1][color][1]);
                                update_can_move_positions(color, 31
                                , piece_positions[31-1][color][0], piece_positions[31-1][color][1]);
                            }
                            if((total_movescore <= best && (piece_sign!=1))
                                || (total_movescore >= best && (piece_sign==1))){
                                return total_movescore;
                            }
                            if((total_movescore < best_movescore && (piece_sign!=1))
                                || (total_movescore > best_movescore && (piece_sign==1))){
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
    }else if((piece_sign!=1)){
        return *std::min_element(&movescore[0], &movescore[0]+movescore_size);
    }else{
        return *std::max_element(&movescore[0], &movescore[0]+movescore_size); 
    }
}

void firstmove(bool all = true){
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
    double best_movescore = -intsign(bot == 0)*1000000;
    std::vector<std::vector<int>> order;
    if(all){
        order = reorder();
    }else{
        for(int i = 0; i < plusamount; i++){
            order.push_back({bestmove[i][0],bestmove[i][1],bestmove[i][2],bestmove[i][3],bestmove[i][4]});
        }
    }
    for(int i = 0; i < order.size(); i++){
        int n = order[i][0];
        int y0 = order[i][1];
        int x0 = order[i][2];
        int y1 = order[i][3];
        int x1 = order[i][4];
        double evaluation_minus = evaluate_change(y1, x1, -1)+(n < 9 && x1*8+y1 == enpassant)*intsign(n)*1.0;
        movepieceto(n, y0, x0, y1, x1, true);
        if(repetition(moves) || stalemate(50) || stalemate(-50)){
            movescore.push_back(-fulleval());
        }else if(partialrepetition(moves)){
            if(bot == 0){
                movescore.push_back(std::min(nth_move(n, y0, x0, y1, x1, 
                best_movescore-evaluation_minus, ntimes) + evaluation_minus, -fulleval()));
            }else{
                movescore.push_back(std::max(nth_move(n, y0, x0, y1, x1, 
                best_movescore-evaluation_minus, ntimes) + evaluation_minus, -fulleval()));
            } 
        }else{
            double current_movescore = 
                nth_move(n, y0, x0, y1, x1, 
                best_movescore-evaluation_minus, ntimes) + evaluation_minus;
            movescore.push_back(current_movescore);
            if((current_movescore > best_movescore && bot == 0) 
                || (current_movescore < best_movescore && bot == 1)){
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
    turn = int(bot == 0);
    int bestindex;
    bestmove.resize(0);
    if(bot == 0){
        for(int i = 0; i < plusamount; i++){
            bestindex = std::distance(std::begin(movescore),
            std::max_element(std::begin(movescore), std::end(movescore)));
            
            bestmove.push_back({});
            for(int j = 0; j < 5; j++){
                bestmove[i].push_back(order[bestindex][j]);
            }
            bestmove[i].push_back(movescore[bestindex]);
            movescore[bestindex] = -1000000;
        }
    }else{
        for(int i = 0; i < plusamount; i++){
            bestindex = std::distance(std::begin(movescore),
            std::min_element(std::begin(movescore), std::end(movescore)));
            bestmove.push_back({});
            for(int j = 0; j < 5; j++){
                bestmove[i].push_back(order[bestindex][j]);
            }
            bestmove[i].push_back(movescore[bestindex]);
            movescore[bestindex] = 1000000;
        }
    }
}

std::vector<std::vector<std::vector<std::vector<int>>>> open_openingbook(const std::string& filename) {
    // Open the file
    std::ifstream infile(filename, std::ios::binary);
    // Read the size of the outer vector
    int outer_size;
    infile.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));

    // Read the contents of the vector
    std::vector<std::vector<std::vector<std::vector<int>>>> myvector(outer_size);
    for (int i = 0; i < outer_size; i++) {
        int outer_inner_size;
        infile.read(reinterpret_cast<char*>(&outer_inner_size), sizeof(outer_inner_size));
        myvector[i].resize(outer_inner_size);
        for (int j = 0; j < outer_inner_size; j++) {
            int inner_inner_size;
            infile.read(reinterpret_cast<char*>(&inner_inner_size), sizeof(inner_inner_size));
            myvector[i][j].resize(inner_inner_size);
            for (int k = 0; k < inner_inner_size; k++) {
                int inner_most_size;
                infile.read(reinterpret_cast<char*>(&inner_most_size), sizeof(inner_most_size));
                myvector[i][j][k].resize(inner_most_size);
                for (int l = 0; l < inner_most_size; l++) {
                int value;
                infile.read(reinterpret_cast<char*>(&value), sizeof(value));
                myvector[i][j][k][l] = value;
                }
            }
        }
    }

    // Close the file and return the vector
    infile.close();
    return myvector;
}

bool compare_to_book(std::vector<std::vector<int>> book_board){
    for(int j = 0; j < 8; j++){
        for(int k = 0; k < 8; k++){
            if(book_board[j][k] != board[j][k]){
                return false;
            }
        }
    }
    return true;
}

bool read_openingbook(int color){
    std::vector<std::vector<std::vector<std::vector<int>>>> openingbook;
    if(color == 0){
        openingbook = open_openingbook("openingbook.bin");
    }else{
        openingbook = open_openingbook("bopeningbook.bin");
    }
    for(int i = 0; i < openingbook.size(); i++){
        if(compare_to_book(openingbook[i][0])){
            std::vector<int> bookmove = openingbook[i][1][0];
            movepieceto(bookmove[0], bookmove[1], bookmove[2], bookmove[3], bookmove[4]);
            std::cout <<  convert_to_png(bookmove[0], bookmove[1], bookmove[2], bookmove[3], bookmove[4]) << '\n';
            return true;
        }
    }
    return false;
}

int basicbot(){
    if(read_openingbook(bot)){
        turn = int(bot == 0);
        std::cout << "book" << '\n';
        printboard();
        return 0;
    }
    double score = fulleval();
    auto start = std::chrono::high_resolution_clock::now();
    firstmove();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast
        <std::chrono::milliseconds>(stop - start);
    while(duration.count()/1000.0 < 0.2){
        if(abs(bestmove[0][5]) > 10000){
            break;
        }
        ntimes += 2;
        firstmove();
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast
            <std::chrono::milliseconds>(stop - start);
    }
    std::cout << "depth = " << ntimes/2+1;
    if(duration.count()/1000.0 < 0.4 && abs(bestmove[0][5]) <= 10000){
        ntimes += 2;
        firstmove(false);
        std::cout << '+';
    }
    std:: cout << '\n';
    if(ntimes > ntimesmin){
        ntimes = ntimesmin;
    }
    score += bestmove[0][5];
    int n = bestmove[0][0];
    int y0 = bestmove[0][1];
    int x0 = bestmove[0][2];
    int y1 = bestmove[0][3];
    int x1 = bestmove[0][4];
    movepieceto(n, y0, x0, y1, x1);
    turn = int(bot == 0);
    printboard();
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast
        <std::chrono::milliseconds>(stop - start);
    std::cout << duration.count()/1000.0 << '\n';
    std::cout <<  convert_to_png(n, y0, x0, y1, x1) << ", " << score << '\n';
    //std::cout << timer << '\n';
    return 0;
}