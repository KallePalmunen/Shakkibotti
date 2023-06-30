
struct node{
    double visited;
    double wins;
    std::vector<std::vector<int>> moves;
    std::vector<node *> child;
};

double UCT(double wins, double visited, double parent_visited){
    return wins/visited + sqrt(2.0)*sqrt(log(parent_visited)/visited);
}

std::vector<std::vector<int>> find_moves(int piece_sign){
    std::vector<std::vector<int>> found_moves;
    for(int n1 = 0; n1 < 6; n1++){
        for(int n2 = 0; n2 < pieces[n1][piece_sign!=1]; n2++){
            int n = piece_sign*(10*n1+n2+int(n1 == 0));
            if(piece_positions[abs(n)-1][int(n<0)][0] != -1){
                int y0 = piece_positions[abs(n)-1][int(n<0)][0];
                int x0 = piece_positions[abs(n)-1][int(n<0)][1];
                for(int y1 = 0; y1 < 8; y1++){
                    for(int x1 = 0; x1 < 8; x1++){
                        if(canmove(n, y0, x0, y1, x1)){
                            found_moves.push_back({n, y0, x0, y1, x1});
                        }
                    }
                }
            }
        }
    }
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(found_moves), std::end(found_moves), rng);
    return found_moves;
}

node *newNode(int piece_sign){
    node *temp = new node;
    temp->visited = 0.0;
    temp->wins = 0.0;
    return temp;
}

node *monte_carlo_nth_move(int piece_sign, node *root, int depth, int maxdepth){
    if((root->moves).size() == 0){
        (root->moves) = find_moves(piece_sign);
    }
    if((root->moves).size() == 0){
        return root;
    }
    if((root->visited) < (root->moves).size()){
        int i = (root->visited);
        (root->visited) += 1.0;
        (root->child).push_back(newNode(piece_sign));
        int n = root->moves[i][0];
        int y0 = root->moves[i][1];
        int x0 = root->moves[i][2];
        int y1 = root->moves[i][3];
        int x1 = root->moves[i][4];
        movepieceto(n, y0, x0, y1, x1, true);
        if(repetition(moves) || stalemate(50) || stalemate(-50)){
            (root->child[i]->visited) += 1.0;
        }else if(checkmate(-piece_sign*50)){
            double start_wins = (root->child[i]->wins);
            (root->child[i]->wins) += (int(bot == 0)-int(bot == 1))*piece_sign*1.0;
            (root->child[i]->visited) += 1.0;
            (root->wins) += (root->child[i]->wins) - start_wins;
        }else if(depth == maxdepth){
            double result = std::min((int(bot == 0)-int(bot == 1))*fulleval()/10.0, 1.0);
            (root->child[i]->wins) += result;
            (root->child[i]->visited) += 1.0;
            (root->wins) += result;
        }else{
            double start_wins = (root->child[i]->wins);
            root->child[i] = monte_carlo_nth_move(-piece_sign, root->child[i], depth+1, maxdepth);
            (root->wins) += (root->child[i]->wins) - start_wins;
        }
    }else{
        double max_UCT = -100;
        int bestindex;
        for(int i = 0; i < (root->moves).size(); i++){
            if(UCT((root->child[i]->wins), (root->child[i]->visited), (root->visited)) > max_UCT){
                max_UCT = UCT((root->child[i]->wins), (root->child[i]->visited), (root->visited));
                bestindex = i;
            }
        }
        int i = bestindex;
        (root->visited) += 1.0;
        (root->child).push_back(newNode(piece_sign));
        int n = root->moves[i][0];
        int y0 = root->moves[i][1];
        int x0 = root->moves[i][2];
        int y1 = root->moves[i][3];
        int x1 = root->moves[i][4];
        movepieceto(n, y0, x0, y1, x1, true);
        if(repetition(moves) || stalemate(50) || stalemate(-50)){
            (root->child[i]->visited) += 1.0;
        }else if(checkmate(-piece_sign*50)){
            double start_wins = (root->child[i]->wins);
            (root->child[i]->wins) += (int(bot == 0)-int(bot == 1))*piece_sign*1.0;
            (root->child[i]->visited) += 1.0;
            (root->wins) += (root->child[i]->wins) - start_wins;
        }else if(depth == maxdepth){
            double result = std::min((int(bot == 0)-int(bot == 1))*fulleval()/10.0, 10.0);
            (root->child[i]->wins) += result;
            (root->child[i]->visited) += 1.0;
            (root->wins) += result;
        }else{
            double start_wins = (root->child[i]->wins);
            root->child[i] = monte_carlo_nth_move(-piece_sign, root->child[i], depth+1, maxdepth);
            (root->wins) += (root->child[i]->wins) - start_wins;
        }
    }
    return root;
}

double monte_carlo_firstmove(int maxdepth, double maxtime){
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
    std::vector<std::vector<std::vector<int>>> temp_positions = positions;
    auto start = std::chrono::high_resolution_clock::now();
    int depth = 0;
    int piece_sign = int(bot == 0)-int(bot == 1);
    node *root = newNode(piece_sign);
    (root->moves) = find_moves(piece_sign);
    for(int i = 0; i < (root->moves).size(); i++){
        (root->visited) += 1.0;
        (root->child).push_back(newNode(piece_sign));
        int n = root->moves[i][0];
        int y0 = root->moves[i][1];
        int x0 = root->moves[i][2];
        int y1 = root->moves[i][3];
        int x1 = root->moves[i][4];
        movepieceto(n, y0, x0, y1, x1, true);
        if(repetition(moves) || stalemate(50) || stalemate(-50)){
            (root->child[i]->visited) += 1.0;
        }else if(checkmate(-piece_sign*50)){
            (root->child[i]->wins) += 1.0;
            (root->child[i]->visited) += 1.0;
        }else{
            root->child[i] = monte_carlo_nth_move(-piece_sign, root->child[i], depth+1, maxdepth);
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
        positions = temp_positions;
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    while(duration.count()/1000.0 < maxtime){
        double max_UCT = -100;
        int bestindex;
        for(int i = 0; i < (root->moves).size(); i++){
            if(UCT((root->child[i]->wins), (root->child[i]->visited), (root->visited)) > max_UCT){
                max_UCT = UCT((root->child[i]->wins), (root->child[i]->visited), (root->visited));
                bestindex = i;
            }
        }
        int i = bestindex;
        (root->visited) += 1.0;
        (root->child).push_back(newNode(piece_sign));
        int n = root->moves[i][0];
        int y0 = root->moves[i][1];
        int x0 = root->moves[i][2];
        int y1 = root->moves[i][3];
        int x1 = root->moves[i][4];
        movepieceto(n, y0, x0, y1, x1, true);
        if(repetition(moves) || stalemate(50) || stalemate(-50)){
            (root->child[i]->visited) += 1.0;
        }else if(checkmate(-piece_sign*50)){
            (root->child[i]->wins) += 1;
        }else{
            root->child[i] = monte_carlo_nth_move(-piece_sign, root->child[i], depth+1, maxdepth);
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
        positions = temp_positions;
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    }
    turn = int(bot == 0);
    bestmove.resize(0);
    int bestindex;
    double best_winrate = -100;
    for(int i = 0; i < (root->moves).size(); i++){
        if((root->child[i]->wins)/(root->child[i]->visited) > best_winrate){
            best_winrate = (root->child[i]->wins)/(root->child[i]->visited);
            bestindex = i;
        }
    }
    bestmove.push_back({});
    for(int j = 0; j < 5; j++){
        bestmove[0].push_back(root->moves[bestindex][j]);
    }
    std::cout << "visited: " << root->visited << '\n';
    return best_winrate;
}

int monte_carlo_move(){
    if(read_openingbook(bot)){
        turn = int(bot == 0);
        std::cout << "book" << '\n';
        return 0;
    }
    int maxdepth = 20;
    double maxtime = 1.0;
    double score = monte_carlo_firstmove(maxdepth, maxtime);
    if(ntimes > ntimesmin){
        ntimes = ntimesmin;
    }
    int n = bestmove[0][0];
    int y0 = bestmove[0][1];
    int x0 = bestmove[0][2];
    int y1 = bestmove[0][3];
    int x1 = bestmove[0][4];
    movepieceto(n, y0, x0, y1, x1);
    turn = int(bot == 0);
    std::cout <<  convert_to_png(n, y0, x0, y1, x1) << ", " << score << '\n';
    return 0;
}