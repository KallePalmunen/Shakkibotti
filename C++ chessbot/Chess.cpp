#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>

extern "C" {
    //white == 0, black == 1
    int bot = 1;
    
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

    std::string piece_to_letter(int n){
        if(abs(n) > 9 && abs(n) < 20){
            return "N";
        }
        if(abs(n) > 19 && abs(n) < 30){
            return "B";
        }
        if(abs(n) > 29 && abs(n) < 40){
            return "R";
        }
        if(abs(n) > 39 && abs(n) < 50){
            return "Q";
        }
        if(abs(n) > 49){
            return "K";
        }
        return "";
    }
    std::string file_to_letter(int x){
        if(x == 7){
            return "a";
        }
        if(x == 6){
            return "b";
        }
        if(x == 5){
            return "c";
        }
        if(x == 4){
            return "d";
        }
        if(x == 3){
            return "e";
        }
        if(x == 2){
            return "f";
        }
        if(x == 1){
            return "g";
        }
        if(x == 0){
            return "h";
        }
        return "";
    }

    int letter_to_file(const char x){
        if(x == 'a'){
            return 7;
        }
        if(x == 'b'){
            return 6;
        }
        if(x == 'c'){
            return 5;
        }
        if(x == 'd'){
            return 4;
        }
        if(x == 'e'){
            return 3;
        }
        if(x == 'f'){
            return 2;
        }
        if(x == 'g'){
            return 1;
        }
        if(x == 'h'){
            return 0;
        }
        return -1;
    }

    std::string convert_to_png(int n, int y0, int x0, int y1, int x1){
        return piece_to_letter(n)  + file_to_letter(x0) + std::to_string(y0+1) + file_to_letter(x1) + std::to_string(y1+1);
    }

    std::vector<int> convert_from_png(std::string move){
        return {int(move[1]-'0')-1, letter_to_file(move[0]), int(move[3]-'0')-1, letter_to_file(move[2])};
    }

    const char* vector_to_string(std::vector<int> vector){
        std::ostringstream oss;
        oss << "[";
        for(int i = 0; i < vector.size(); i++){
            oss << vector[i];
            if (i < vector.size() - 1) {
                oss << ",";
            }
        }
        oss << "]";

        std::string str = oss.str();
        char* charPtr = new char[str.length() + 1];
        std::strcpy(charPtr, str.c_str());

        return charPtr;
    }

    const char* vector_to_string_3d(std::vector<std::vector<std::vector<int>>> vector){
        std::ostringstream oss;
        oss << "[";
        for(int i = 0; i < vector.size(); i++){
            oss << "[";
            for(int j = 0; j < vector[i].size(); j++){
                oss << "[";
                for(int k = 0; k < vector[i][j].size(); k++){
                    oss << vector[i][j][k];
                    if (k < vector[i][j].size() - 1){
                        oss << ",";
                    }
                }
                oss << "]";
                if(j < vector[i].size() - 1){
                    oss << ",";
                }
            }
            oss << "]";
            if(i < vector.size() - 1){
                oss << ",";
            }
        }
        oss << "]";

        std::string str = oss.str();
        char* charPtr = new char[str.length() + 1];
        std::strcpy(charPtr, str.c_str());

        return charPtr;
    }

    std::vector<std::vector<int>> string_to_vector_2d(const char* can_move_positions_str){
        int i = 1;
        int j = -1;
        std::vector<std::vector<int>> result;
        while(i < strlen(can_move_positions_str)){
            result.push_back({});
            j++;
            while(i < strlen(can_move_positions_str)){
                std::string element = "";
                while(true){
                    if(can_move_positions_str[i] == ',' || can_move_positions_str[i] == '['){
                        i++;
                    }else{
                        break;
                    }
                }
                while(true){
                    if(can_move_positions_str[i] == ',' || can_move_positions_str[i] == ']'){
                        i ++;
                        break;
                    }
                    element += can_move_positions_str[i];
                    i++;
                }
                if(element != ""){
                    result[j].push_back(std::stoi(element));
                }
                if(can_move_positions_str[i-1] == ']'){
                    break;
                }
            }
            if(can_move_positions_str[i] == ']'){
                i++;
                break;
            }
        }
        return result;
    }

    std::vector<std::vector<std::vector<int>>> string_to_vector_3d(const char* can_move_positions_str){
        int i = 1;
        int j = -1;
        int k = -1;
        std::vector<std::vector<std::vector<int>>> result;
        while(i < strlen(can_move_positions_str)){
            result.push_back({});
            j++;
            k = -1;

            while(i < strlen(can_move_positions_str)){
                result[j].push_back({});
                k++;
                while(true){
                    if(can_move_positions_str[i] == ','){
                        i++;
                    }else{
                        break;
                    }
                }
                if(k == 0 && can_move_positions_str[i+1] != '['){
                    i += 2;
                    break;
                }
                while(i < strlen(can_move_positions_str)){
                    std::string element = "";
                    while(true){
                        if(can_move_positions_str[i] == ',' || can_move_positions_str[i] == '['){
                            i++;
                        }else{
                            break;
                        }
                    }
                    while(true){
                        if(can_move_positions_str[i] == ',' || can_move_positions_str[i] == ']'){
                            i ++;
                            break;
                        }
                        element += can_move_positions_str[i];
                        i++;
                    }
                    if(element != ""){
                        result[j][k].push_back(std::stoi(element));
                    }
                    if(can_move_positions_str[i-1] == ']'){
                        break;
                    }
                }
                if(can_move_positions_str[i] == ']'){
                    i++;
                    break;
                }
            }
            if(can_move_positions_str[i] == ']'){
                i++;
                break;
            }
        }
        return result;
    }

    std::vector<std::vector<int>> convert_board(const char* board_string){
        int i = 2;
        std::vector<std::vector<int>> result;
        for(int y = 0; y < 8; y++){
            result.push_back({});
            for(int x = 0; x < 8; x++){
                std::string element = "";
                while(true){
                    if(board_string[i] == ',' || board_string[i] == '[' || board_string[i] == ']'){
                        i++;
                    }else{
                        break;
                    }
                }
                while(true){
                    if(board_string[i] == ',' || board_string[i] == ']'){
                        i ++;
                        break;
                    }
                    element += board_string[i];
                    i++;
                }
                result[y].push_back(std::stoi(element));
            }
        }
        return result;
    }

    std::vector<std::vector<std::vector<int>>> convert_positions(const char* positions_string, int moves){
        int i = 0;
        std::vector<std::vector<std::vector<int>>> result;
        for(int move = 0; move <= moves; move++){
            result.push_back({});
            for(int y = 0; y < 8; y++){
                result[move].push_back({});
                for(int x = 0; x < 8; x++){
                    std::string element = "";
                    while(true){
                        if(positions_string[i] == ',' || positions_string[i] == '[' || positions_string[i] == ']'){
                            i++;
                        }else{
                            break;
                        }
                    }
                    while(true){
                        if(positions_string[i] == ',' || positions_string[i] == ']'){
                            i ++;
                            break;
                        }
                        element += positions_string[i];
                        i++;
                    }
                    result[move][y].push_back(std::stoi(element));
                }
            }
        }
        return result;
    }

    const char* locate_pieces(const char* board_string){
        std::vector<std::vector<int>> board = convert_board(board_string);
        std::vector<std::vector<std::vector<int>>> piece_positions;
        for(int n = 1; n < 51; n++){
            bool found = false;
            piece_positions.push_back({});
            for(int y = 0; y < 8; y++) {
                for(int x = 0; x < 8; x++) {
                    if(board[y][x] == n) {
                        piece_positions[n-1].push_back({});
                        piece_positions[n-1][0].push_back(y);
                        piece_positions[n-1][0].push_back(x);
                        found = true;
                        break;
                    }
                }
                if(found){
                    break;
                }
            }
            if(!found){
                piece_positions[n-1].push_back({});
                piece_positions[n-1][0].push_back(-1);
                piece_positions[n-1][0].push_back(-1);
            }
            found = false;
            for(int y = 0; y < 8; y++) {
                for(int x = 0; x < 8; x++) {
                    if(board[y][x] == -n) {
                        piece_positions[n-1].push_back({});
                        piece_positions[n-1][1].push_back(y);
                        piece_positions[n-1][1].push_back(x);
                        found = true;
                        break;
                    }
                }
                if(found){
                    break;
                }
            }
            if(!found){
                piece_positions[n-1].push_back({});
                piece_positions[n-1][1].push_back(-1);
                piece_positions[n-1][1].push_back(-1);
            }
        }
        return vector_to_string_3d(piece_positions);
    }

    void printboard(const char* board_string){
        std::vector<std::vector<int>> board = convert_board(board_string);
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                std::cout << board[i][j] << " ";
            }
            std::cout << "\n";
        };
    }

    //intsign tells the sign of an integer

    int intsign(int a){
        return (a > 0)-(a <= 0);
    }

    bool pawnmove(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board, int& enpassant){
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

    bool knightmove(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board){
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

    bool longmove(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board){
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

    bool bishopmove(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board){
        if(abs(y1-y0) == abs(x1-x0)){
            if(longmove(n, y0, x0, y1, x1, board)){
                return true;
            }
            return false;
        }
        return false;
    }

    bool rookmove(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board){
        if((y1 != y0 && x1 == x0) || (y1 == y0 && x1 != x0)){
            if(longmove(n, y0, x0, y1, x1, board)){
                return true;
            }
            return false;
        }
        return false;
    }

    bool queenmove(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board){
        if(bishopmove(n, y0, x0, y1, x1, board) || rookmove(n, y0, x0, y1, x1, board)){
            return true;
        }
        return false;
    }

    bool kingmove(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board){
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

    bool piecemove(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board, int& enpassant){
        if(abs(n) < 20 && abs(n) >= 10){
            return knightmove(n, y0, x0, y1, x1, board);
        }
        if(abs(n) < 30 && abs(n) >= 20){
            return bishopmove(n, y0, x0, y1, x1, board);
        }
        if(abs(n) < 10){
            return pawnmove(n, y0, x0, y1, x1, board, enpassant);
        }
        if(abs(n) < 40 && abs(n) >= 30){
            return rookmove(n, y0, x0, y1, x1, board);
        }
        if(abs(n) < 50 && abs(n) >= 40){
            return queenmove(n, y0, x0, y1, x1, board);
        }
        if(abs(n) >= 50){
            return kingmove(n, y0, x0, y1, x1, board);
        }
        return false;
    }

    bool botpiecemove(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board, int& enpassant){
        if(n * board[y1][x1] > 0){
            return false;
        }
        if(abs(n) < 10){
            return pawnmove(n, y0, x0, y1, x1, board, enpassant);
        }
        return true;
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

    bool check(int n, std::vector<std::vector<int>>& board, std::vector<std::vector<std::vector<int>>>& piece_positions
    , std::vector<std::vector<int>>& pieces, int& enpassant, int kingy = -1, int kingx = -1){
        if(kingy == -1) {
            bool found = false;
            for(int y = 0; y < 8; y++) {
                for(int x = 0; x < 8; x++) {
                    if(board[y][x] == n) {
                        kingy = y;
                        kingx = x;
                        found = true;
                        break;
                    }
                }
                if(found){
                    break;
                }
            }
            if(!found){
                return true;
            }
        }
        for(int n1 = 0; n1 < 6; n1++){
            for(int n2 = 0; n2 < pieces[n1][(n > 0)]; n2++){
                int piecen = -intsign(n)*(n1*10+n2+(n1 == 0));
                if(piecen != 0){
                    if(piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                        int y0 = piece_positions[abs(piecen)-1][int(piecen<0)][0];
                        int x0 = piece_positions[abs(piecen)-1][int(piecen<0)][1];
                        if(piecemove(piecen, y0, x0, kingy, kingx, board, enpassant)){
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool botcheck(int n, int kingy, int kingx, std::vector<std::vector<int>>& board
    , std::vector<std::vector<std::vector<int>>>& piece_positions, std::vector<int>& pinners, int& enpassant){
        for(int i = 0; i < pinners.size(); i++){
            int piecen = pinners[i];
            if(piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                int y0 = piece_positions[abs(piecen)-1][int(piecen<0)][0];
                int x0 = piece_positions[abs(piecen)-1][int(piecen<0)][1];
                if(piecemove(piecen, y0, x0, kingy, kingx, board, enpassant)){
                    return true;
                }
            }
        }
        return false;
    }

    bool castle(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board,
    std::vector<std::vector<std::vector<int>>>& piece_positions, std::vector<std::vector<int>>& pieces, int& kingmoved
    , int& enpassant, std::vector<std::vector<int>>& rookmoved){
        if(kingmoved%((n>0)*2+(n<0)*3) == 0 || rookmoved[(n>0)][x1>4] == 1){
            return false;
        }
        if(y1 == y0 && (x1 == 1 || x1 == 5) && !check(n, board, piece_positions, pieces, enpassant) &&
        board[y0][(x1 > 4)*7] == intsign(n)*(30 + (x1 > 4))){
            for(int i = 1; i < 3 + (x1 > 4); i++){
                int squarex = x0 + i*intsign(x1-x0);
                if(board[y0][squarex] != 0){
                    return false;
                }
                board[y0][x0] = 0;
                board[y0][squarex] = n;
                piece_positions[49][int(n<0)][1] = squarex;
                if(i < 3 && check(n, board, piece_positions, pieces, enpassant)){
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

    bool pin(int n, int y0, int x0, int y1, int x1, int kingy, int kingx, std::vector<std::vector<int>>& board
    , std::vector<std::vector<std::vector<int>>>& piece_positions, std::vector<std::vector<int>>& pieces, int& enpassant){
        board[y0][x0] = 0;
        int movetosquare = board[y1][x1];
        board[y1][x1] = n;
        piece_positions[abs(n)-1][int(n<0)][0] = y1;
        piece_positions[abs(n)-1][int(n<0)][1] = x1;
        if(abs(movetosquare) > 0){
            piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = -1;
        }
        //checks if you can prevent the mate in next turn 
        //by enpassanting the checking piece
        int enpassanted = -100;
        if(enpassant >= 0 && x1*8+y1 == enpassant && abs(n) < 10){
            enpassanted = board[y1-intsign(y1 - y0)][x1];
            board[y1-intsign(y1 - y0)][x1] = 0;
            piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = -1;
        }
        if(!check(intsign(n)*50, board, piece_positions, pieces, enpassant, kingy, kingx)){
            board[y0][x0] = n;
            board[y1][x1] = movetosquare;
            piece_positions[abs(n)-1][int(n<0)][0] = y0;
            piece_positions[abs(n)-1][int(n<0)][1] = x0;
            if(abs(movetosquare) > 0){
                piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = y1;
            }
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
        if(abs(movetosquare) > 0){
            piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = y1;
        }
        if(enpassanted != -100){
            board[y1-intsign(y1 - y0)][x1] = enpassanted;
            piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = y1-intsign(y1 - y0);
        }
        return true;
    }

    bool botpin(int n, int y0, int x0, int y1, int x1, int kingy, int kingx, std::vector<std::vector<int>>& board
    , std::vector<std::vector<std::vector<int>>>& piece_positions, std::vector<int>& pinners, int& enpassant){
        if(pinners.size() == 0){
            return false;
        }
        board[y0][x0] = 0;
        int movetosquare = board[y1][x1];
        board[y1][x1] = n;
        piece_positions[abs(n)-1][int(n<0)][0] = y1;
        piece_positions[abs(n)-1][int(n<0)][1] = x1;
        if(abs(movetosquare) > 0){
            piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = -1;
        }
        //checks if you can prevent the mate in next turn 
        //by enpassanting the checking piece
        int enpassanted = -100;
        if(enpassant >= 0 && x1*8+y1 == enpassant && abs(n) < 10){
            enpassanted = board[y1-intsign(y1 - y0)][x1];
            board[y1-intsign(y1 - y0)][x1] = 0;
            piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = -1;
        }
        if(!botcheck(intsign(n)*50, kingy, kingx, board, piece_positions, pinners, enpassant)){
            board[y0][x0] = n;
            board[y1][x1] = movetosquare;
            piece_positions[abs(n)-1][int(n<0)][0] = y0;
            piece_positions[abs(n)-1][int(n<0)][1] = x0;
            if(abs(movetosquare) > 0){
                piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = y1;
            }
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
        if(abs(movetosquare) > 0){
            piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = y1;
        }
        if(enpassanted != -100){
            board[y1-intsign(y1 - y0)][x1] = enpassanted;
            piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = y1-intsign(y1 - y0);
        }
        return true;
    }

    bool ispinnable(int n, int y0, int x0, int kingy, int kingx, std::vector<std::vector<int>>& board
    , std::vector<std::vector<std::vector<int>>>& piece_positions, std::vector<int>& pinners, int& enpassant){
        if(pinners.size() == 0){
            return false;
        }
        board[y0][x0] = 0;
        if(!botcheck(intsign(n)*50, kingy, kingx, board, piece_positions, pinners, enpassant)){
            board[y0][x0] = n;
            return false;
        }
        board[y0][x0] = n;
        return true;
    }

    bool canmove(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board
    , std::vector<std::vector<std::vector<int>>>& piece_positions 
    , std::vector<std::vector<int>>& pieces, int& kingmoved, int& enpassant
    , std::vector<std::vector<int>>& rookmoved, int kingy = -1, int kingx = -1){
        if(piecemove(n, y0, x0, y1, x1, board, enpassant) 
        || (abs(n) == 50 && castle(n, y0, x0, y1, x1, board, piece_positions, pieces, kingmoved, enpassant, rookmoved))){
            if(abs(n) == 50){
                kingy = y1;
                kingx = x1;
            }
            if(!pin(n, y0, x0, y1, x1, kingy, kingx, board, piece_positions, pieces, enpassant)){
                return true;
            }
        }
        return false;
    }

    bool botcanmove(int n, int y0, int x0, int y1, int x1, bool pinnable, std::vector<std::vector<int>>& board
    , std::vector<std::vector<std::vector<int>>>& piece_positions
    , std::vector<std::vector<int>>& pieces, std::vector<int>& pinners, int& kingmoved, int& enpassant
    , std::vector<std::vector<int>>& rookmoved, int kingy = -1, int kingx = -1){
        if(abs(n) < 50){
            if(botpiecemove(n, y0, x0, y1, x1, board, enpassant)){
                if(!pinnable){
                    return true;
                }
                return (!botpin(n, y0, x0, y1, x1, kingy, kingx, board, piece_positions, pinners, enpassant));
            }
            return false;
        }
        if(botpiecemove(n, y0, x0, y1, x1, board, enpassant) 
        || (abs(n) == 50 && castle(n, y0, x0, y1, x1, board, piece_positions, pieces, kingmoved, enpassant, rookmoved))){
            return (!pin(n, y0, x0, y1, x1, y1, x1, board, piece_positions, pieces, enpassant));
        }
        return false;
    }

    bool movesomewhere(int n, int y0, int x0, std::vector<std::vector<int>>& board
    , std::vector<std::vector<std::vector<int>>>& piece_positions
    , std::vector<std::vector<int>>& pieces, int& kingmoved, int& enpassant
    , std::vector<std::vector<int>>& rookmoved, int kingy = -1, int kingx = -1){
        for(int y1 = 0; y1 < 8; y1++){
            for(int x1 = 0; x1 < 8; x1++){
                if(canmove(n, y0, x0, y1, x1, board, piece_positions, pieces, kingmoved, enpassant, rookmoved, kingy, kingx)){
                    return true;
                }
            }
        }
        return false;
    }

    bool checkmate(int n, std::vector<std::vector<int>>& board, std::vector<std::vector<std::vector<int>>>& piece_positions
    , std::vector<std::vector<int>>& pieces, int& kingmoved, int& enpassant, std::vector<std::vector<int>>& rookmoved
    , int kingy = -1, int kingx = -1){
        if(!check(n, board, piece_positions, pieces, enpassant, kingy, kingx)){
            return false;
        }
        for(int n1 = 0; n1 < 6; n1++){
            for(int n2 = 0; n2 < pieces[n1][(n < 0)]; n2++){
                int piecen = intsign(n)*(n1*10+n2);
                if(piecen != 0){
                    if(piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                        int y0 = piece_positions[abs(piecen)-1][int(piecen<0)][0];
                        int x0 = piece_positions[abs(piecen)-1][int(piecen<0)][1];
                        if(movesomewhere(piecen, y0, x0, board, piece_positions, pieces, kingmoved, enpassant, rookmoved, kingy, kingx)){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    bool botcheckmate(int n, std::vector<std::vector<int>>& board
    , std::vector<std::vector<std::vector<int>>>& piece_positions, std::vector<std::vector<int>>& pieces
    , std::vector<int>& pinners, int& kingmoved, int& enpassant, std::vector<std::vector<int>>& rookmoved, int kingy = -1, int kingx = -1){
        if(!botcheck(n, kingy, kingx, board, piece_positions, pinners, enpassant)){
            return false;
        }
        for(int n1 = 0; n1 < 6; n1++){
            for(int n2 = 0; n2 < pieces[n1][(n < 0)]; n2++){
                int piecen = intsign(n)*(n1*10+n2);
                if(piecen != 0){
                    if(piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                        int y0 = piece_positions[abs(piecen)-1][int(piecen<0)][0];
                        int x0 = piece_positions[abs(piecen)-1][int(piecen<0)][1];
                        if(movesomewhere(piecen, y0, x0, board, piece_positions, pieces, kingmoved, enpassant, rookmoved, kingy, kingx)){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    void movepieceto(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board, int& castled
    , std::vector<std::vector<std::vector<int>>>& piece_positions, std::vector<std::vector<int>>& pieces, int& kingmoved, int& enpassant
    , std::vector<std::vector<int>>& rookmoved){
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
                castled *= (2*(n > 0) + 3*(n < 0));
                piece_positions[abs(whichrook)-1][int(n<0)][0] = y1;
                piece_positions[abs(whichrook)-1][int(n<0)][1] = x1 + intsign(4-x1);
            }
            kingmoved *= ((n>0)*2 + (n<0)*3);
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
        if(enpassant >= 0 && x1*8+y1 == enpassant && abs(n) < 10){
            piece_positions[abs(board[y1-intsign(y1 - y0)][x1])-1][int(n>0)][0] = -1;
            board[y1-intsign(y1 - y0)][x1] = 0;
        }
        if(abs(n) < 10 && abs(y1-y0) > 1){
            enpassant = x1*8+y0+intsign(y1 - y0);
        }else{
            enpassant = -1;
        }
        board[y0][x0] = 0;
    }

    bool stalemate(int n, std::vector<std::vector<int>>& board, std::vector<std::vector<std::vector<int>>>& piece_positions
    , std::vector<std::vector<int>>& pieces, int& kingmoved, int& enpassant, std::vector<std::vector<int>>& rookmoved){
        if(check(n, board, piece_positions, pieces, enpassant)){
            return false;
        }
        for(int n1 = 1; n1 < 51; n1++){
            int piecen = intsign(n)*n1;
            if(piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                int y0 = piece_positions[abs(piecen)-1][int(piecen<0)][0];
                int x0 = piece_positions[abs(piecen)-1][int(piecen<0)][1];
                if(movesomewhere(piecen, y0, x0, board, piece_positions, pieces, kingmoved, enpassant, rookmoved)){
                    return false;
                }
            }
        }
        return true;
    }

    bool compareposition(int moment, std::vector<std::vector<int>>& board, std::vector<std::vector<std::vector<int>>> positions){
        for(int y = 0; y < 8; y++){
            for(int x = 0; x < 8; x++){
                if(positions[moment][y][x] != board[y][x]){
                    return false;
                }
            }
        }
        return true;
    }

    bool repetition(int this_moment, std::vector<std::vector<int>>& board, std::vector<std::vector<std::vector<int>>> positions){
        int repetitions = 0;
        for(int moment = this_moment%2; moment < this_moment; moment += 2){
            if(compareposition(moment, board, positions)){
                repetitions++;
                if(repetitions >= 2){
                    return true;
                }
            }
        }
        return false;
    }

    int gameend(int turn, int moves, const char* board_string, const char* positions_string
    , const char* piece_positions_str, const char* pieces_str, int kingmoved, int enpassant, const char* rookmoved_str){
        //returns 2 if white won, 1 if black won, 0 if draw, -1 if game continues
        std::vector<std::vector<int>> board = convert_board(board_string);
        std::vector<std::vector<std::vector<int>>> positions = convert_positions(positions_string, moves);
        std::vector<std::vector<std::vector<int>>> piece_positions = string_to_vector_3d(piece_positions_str);
        std::vector<std::vector<int>> pieces = string_to_vector_2d(pieces_str);
        std::vector<std::vector<int>> rookmoved = string_to_vector_2d(rookmoved_str);

        if(checkmate(-50, board, piece_positions, pieces, kingmoved, enpassant, rookmoved)){
            std::cout << "White won" << '\n';
            return 2;
        }
        if(checkmate(50, board, piece_positions, pieces, kingmoved, enpassant, rookmoved)){
            std::cout << "Black won" << '\n';
            return 1;
        }
        if(
        //  (turn == 0 && stalemate(50, board)) || (turn == 1 && stalemate(-50, board)) || 
            repetition(moves, board, positions)){
                std::cout << "Draw" << '\n';
                return 0;
        }
        return -1;
    }

    std::vector<std::vector<std::vector<int>>> update_can_move_positions(int color, int piece, int y0, int x0
    , std::vector<std::vector<std::vector<int>>>& can_move_positions){
        if(piece > 0){
            can_move_positions[piece-1].resize(0);
        }
        if(y0 < 0 || x0 < 0){
            return can_move_positions;
        }
        if(piece > 9 && piece < 20){
            can_move_positions[piece-1].push_back({});
            if(y0 > 0){
                if(x0 < 6){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-1, x0+2});
                }
                if(x0 > 1){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-1, x0-2});
                }
                if(y0 > 1){
                    if(x0 < 7){
                        can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-2, x0+1});
                    }
                    if(x0 > 0){
                        can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-2, x0-1});
                    }
                }
            }
            if(y0 < 7){
                if(x0 < 6){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0+1, x0+2});
                }
                if(x0 > 1){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0+1, x0-2});
                }
                if(y0 < 6){
                    if(x0 < 7){
                        can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0+2, x0+1});
                    }
                    if(x0 > 0){
                        can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0+2, x0-1});
                    }
                }
            }
            return can_move_positions;
        }
        if(abs(piece) < 10){
            can_move_positions[piece-1].push_back({});
            if(color == 0){
                can_move_positions[piece-1][0] = {y0+1, x0, y0+1, x0+1, y0+1, x0-1};
                if(y0 == 1){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0+2, x0});
                }
                return can_move_positions;
            }
            if(color == 1){
                can_move_positions[piece-1][0] = {y0-1, x0, y0-1, x0+1, y0-1, x0-1};
                if(y0 == 6){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-2, x0});
                }
                return can_move_positions;
            }
            return can_move_positions;
        }
        if(piece > 19 && piece < 30){
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0-i >= 0 && y0-i >= 0; i++){
                can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-i, x0-i});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0+i < 8 && y0-i >= 0; i++){
                can_move_positions[piece-1][1].insert(can_move_positions[piece-1][1].end(),{y0-i, x0+i});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0+i < 8 && y0+i < 8; i++){
                can_move_positions[piece-1][2].insert(can_move_positions[piece-1][2].end(),{y0+i, x0+i});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0-i >= 0 && y0+i < 8; i++){
                can_move_positions[piece-1][3].insert(can_move_positions[piece-1][3].end(),{y0+i, x0-i});
            }
            return can_move_positions;
        }
        if(piece > 29 && piece < 40){
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0-i >= 0; i++){
                can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0, x0-i});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0+i < 8; i++){
                can_move_positions[piece-1][1].insert(can_move_positions[piece-1][1].end(),{y0, x0+i});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; y0-i >= 0; i++){
                can_move_positions[piece-1][2].insert(can_move_positions[piece-1][2].end(),{y0-i, x0});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; y0+i < 8; i++){
                can_move_positions[piece-1][3].insert(can_move_positions[piece-1][3].end(),{y0+i, x0});
            }
            return can_move_positions;
        }
        if(piece > 39 && piece < 50){
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0-i >= 0 && y0-i >= 0; i++){
                can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-i, x0-i});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0+i < 8 && y0-i >= 0; i++){
                can_move_positions[piece-1][1].insert(can_move_positions[piece-1][1].end(),{y0-i, x0+i});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0+i < 8 && y0+i < 8; i++){
                can_move_positions[piece-1][2].insert(can_move_positions[piece-1][2].end(),{y0+i, x0+i});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0-i >= 0 && y0+i < 8; i++){
                can_move_positions[piece-1][3].insert(can_move_positions[piece-1][3].end(),{y0+i, x0-i});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0-i >= 0; i++){
                can_move_positions[piece-1][4].insert(can_move_positions[piece-1][4].end(),{y0, x0-i});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; x0+i < 8; i++){
                can_move_positions[piece-1][5].insert(can_move_positions[piece-1][5].end(),{y0, x0+i});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; y0-i >= 0; i++){
                can_move_positions[piece-1][6].insert(can_move_positions[piece-1][6].end(),{y0-i, x0});
            }
            can_move_positions[piece-1].push_back({});
            for(int i = 1; y0+i < 8; i++){
                can_move_positions[piece-1][7].insert(can_move_positions[piece-1][7].end(),{y0+i, x0});
            }
            return can_move_positions;
        }
        if(piece == 50){
            can_move_positions[piece-1].push_back({});
            if(y0 > 0){
                can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-1, x0});
                if(x0 > 0){
                can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-1, x0-1}); 
                }
                if(x0 < 7){
                can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-1, x0+1}); 
                }
            }
            if(x0 > 0){
                can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0, x0-1});
                if(x0 == 3){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0, x0-2});
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0, x0+2});
                }
            }
            if(x0 < 7){
                can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0, x0+1});
            }
            if(y0 < 7){
                can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0+1, x0});
                if(x0 > 0){
                can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0+1, x0-1}); 
                }
                if(x0 < 7){
                can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0+1, x0+1}); 
                }
            }
            return can_move_positions;
        }
        return can_move_positions;
    }

    int movepiece(int y0, int x0, int movetoy, int movetox, int turn, const char* board_string, int& castled
    , const char* piece_positions_str, const char* pieces_str, int kingmoved, int enpassant, const char* rookmoved_str){
        std::vector<std::vector<int>> board = convert_board(board_string);
        std::vector<std::vector<std::vector<int>>> piece_positions = string_to_vector_3d(piece_positions_str);
        std::vector<std::vector<int>> pieces = string_to_vector_2d(pieces_str);
        std::vector<std::vector<int>> rookmoved = string_to_vector_2d(rookmoved_str);
        int piece = board[y0][x0];
        std::cout << piece << ',' << y0 << ',' << x0 << ',' << movetoy << ',' << movetox << '\n'; 
        if((piece > 0 && turn == 0) || (piece < 0 && turn == 1)){
            if(movetoy < 8 && movetox < 8 && movetox >= 0 && movetoy >= 0 && y0>=0 
            && canmove(piece, y0, x0, movetoy, movetox, board, piece_positions, pieces, kingmoved, enpassant, rookmoved)){
                movepieceto(piece, y0, x0, movetoy, movetox, board, castled, piece_positions, pieces, kingmoved, enpassant, rookmoved);
            }else{
                return 0;
            }
        }else{
            std::cout << turn << '\n';
            return 0;
        }
        return 1;
    }

    bool partialrepetition(int current_moment, std::vector<std::vector<int>>& board, std::vector<std::vector<std::vector<int>>> positions){
        for(int moment = current_moment%2; moment < current_moment; moment += 2){
            if(compareposition(moment, board, positions)){
                return true;
            }
        }
        return false;
    }

    const char* set_can_move_positions(const char* piece_positions_str){
        std::vector<std::vector<std::vector<int>>> can_move_positions;
        std::vector<std::vector<std::vector<int>>> piece_positions = string_to_vector_3d(piece_positions_str);
        can_move_positions.resize(0);
        for(int n = 1; n < 50; n++){
            can_move_positions.push_back({});
            update_can_move_positions(int(bot == 0), n, piece_positions[n-1][int(bot ==0)][0]
            , piece_positions[n-1][int(bot == 0)][1], can_move_positions);
        }
        can_move_positions.push_back({});
        return vector_to_string_3d(update_can_move_positions(int(bot == 0), 50, piece_positions[50-1][int(bot ==0)][0]
        , piece_positions[50-1][int(bot == 0)][1], can_move_positions));
    }

    std::vector<int> get_pinners(int piece_sign, int kingy, int kingx, std::vector<std::vector<int>>& board
    , std::vector<std::vector<std::vector<int>>>& piece_positions, std::vector<std::vector<int>>& pieces, int& enpassant){
        std::vector<int> pinners;
        for(int n1 = 0; n1 < 6; n1++){
            for(int n2 = 0; n2 < pieces[n1][piece_sign!=1]; n2++){
                int n = piece_sign*(10*n1+n2+int(n1 == 0));
                int y0 = piece_positions[abs(n)-1][int(n<0)][0];
                int x0 = piece_positions[abs(n)-1][int(n<0)][1];
                if(piecemove(n, y0, x0, kingy, kingx, board, enpassant)){
                    pinners.push_back(n);
                }
            }
        }
        return pinners;
    }

    double evaluate_change(int y, int x, int changesign, std::vector<std::vector<int>>& board, int n = -100){
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

    double evaluate_move(int n, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& board, int& castled){
        return evaluate_change(y1, x1, 1, board, n) + 
            evaluate_change(y0, x0, -1, board, n) + (castled % 2 == 0)*0.1 - (castled % 3 == 0)*0.1;
    }

    double fulleval(std::vector<std::vector<int>>& board, int& castled
    , std::vector<std::vector<std::vector<int>>>& piece_positions){
        double evaluation = 0;
        for(int n = 1; n < 51; n++){
            if(piece_positions[n-1][0][0] != -1){
                int y0 = piece_positions[n-1][0][0];
                int x0 = piece_positions[n-1][0][1];
                evaluation += evaluate_change(y0, x0, 1, board, n);
            }
            if(piece_positions[n-1][1][0] != -1){
                int y0 = piece_positions[n-1][1][0];
                int x0 = piece_positions[n-1][1][1];
                evaluation += evaluate_change(y0, x0, 1, board, -n);
            }
        }
        evaluation -= 0.1*((castled%2 == 0) + (castled%3 == 0));
        return evaluation;
    }

    std::vector<std::vector<int>> reorder(int moves, std::vector<std::vector<int>>& board, std::vector<std::vector<std::vector<int>>> positions
    , int& castled, std::vector<std::vector<std::vector<int>>>& piece_positions, std::vector<std::vector<int>>& pieces, int& kingmoved
    , int& enpassant, std::vector<std::vector<int>>& rookmoved){
        //save current state
        int temp_enpassant = enpassant;
        int temp_kingmoved = kingmoved;
        int temp_castled = castled;
        std::vector<std::vector<int>> temp_rookmoved = rookmoved;
        std::vector<std::vector<int>> temp_pieces = pieces;
        std::vector<std::vector<std::vector<int>>> temp_piece_positions = piece_positions;
        std::vector<std::vector<int>> temp_board = board;
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
                        if(canmove(n, y0, x0, y1, x1, board, piece_positions, pieces, kingmoved, enpassant, rookmoved)){
                            double evaluation_minus = evaluate_change(y1, x1, -1, board)+(n < 9 && x1*8+y1 == enpassant)*intsign(n)*1.0;
                            movepieceto(n, y0, x0, y1, x1, board, castled, piece_positions, pieces, kingmoved, enpassant, rookmoved);
                            if(repetition(moves+1, board, positions)
                            // || stalemate(50, new_board) || stalemate(-50, new_board)
                            ){
                                movescore.push_back(-fulleval(board, castled, piece_positions));
                            }else if(partialrepetition(moves+1, board, positions)){
                                if(bot == 0){
                                    movescore.push_back(std::min(evaluate_move(n, y0, x0, y1, x1, board, castled) 
                                    + evaluation_minus, -fulleval(board, castled, piece_positions)));
                                }else{
                                    movescore.push_back(std::max(evaluate_move(n, y0, x0, y1, x1, board, castled) 
                                    + evaluation_minus, -fulleval(board, castled, piece_positions)));
                                }    
                            }else{
                                movescore.push_back(
                                    evaluate_move(n, y0, x0, y1, x1, board, castled) 
                                    + evaluation_minus);
                            }
                            starting_order.insert(starting_order.end(),
                                {n, y0, x0, y1, x1});

                            //return to saved state
                            enpassant = temp_enpassant;
                            kingmoved = temp_kingmoved;
                            castled = temp_castled;
                            rookmoved = temp_rookmoved;
                            pieces = temp_pieces;
                            piece_positions = temp_piece_positions;
                            board = temp_board;
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

    double last_move(int n0, int y00, int x00, int y10, int x10, double best, std::vector<std::vector<int>>& board
    , std::vector<std::vector<std::vector<int>>>& can_move_positions, int& castled
    , std::vector<std::vector<std::vector<int>>>& piece_positions, std::vector<std::vector<int>>& pieces, int& kingmoved
    , int& enpassant, std::vector<std::vector<int>>& rookmoved){
        int piece_sign = int(bot == 1)-int(bot == 0);
        int kingy = piece_positions[49][int(n0>0)][0];
        int kingx = piece_positions[49][int(n0>0)][1];
        if(kingy == -1){
            return -piece_sign*500000/(ntimes+1.0);
        }
        std::vector<int> pinners = get_pinners(-piece_sign, kingy, kingx, board, piece_positions, pieces, enpassant);
        if(botcheckmate(piece_sign*50, board, piece_positions, pieces, pinners, kingmoved, enpassant, rookmoved, kingy, kingx)){
            return -piece_sign*500000/(ntimes+1.0);
        }
        double previous_movescore = evaluate_move(n0, y00, x00, y10, x10, board, castled);
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
                    bool pinnable = ispinnable(n, y0, x0, kingy, kingx, board, piece_positions, pinners, enpassant);
                    for(int i = 0; i < can_move_positions[abs(n)-1].size(); i++){
                        for(int j = 0; j < can_move_positions[abs(n)-1][i].size(); j+=2){
                            int y1 = can_move_positions[abs(n)-1][i][j];
                            int x1 = can_move_positions[abs(n)-1][i][j+1];
                            if(botcanmove(n, y0, x0, y1, x1, pinnable, board, piece_positions, pieces
                            , pinners, kingmoved, enpassant, rookmoved, kingy, kingx)){
                                double evaluation_minus = evaluate_change(y1, x1, -1, board)+(n < 9 && x1*8+y1 == enpassant)*intsign(n)*1.0;
                                double current_movescore = evaluate_move(n, y0, x0, y1, x1, board, castled)
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

    double nth_move(int n0, int y00, int x00, int y10, int x10, double best, int nmoremoves, std::vector<std::vector<int>> board
    , std::vector<std::vector<std::vector<int>>> can_move_positions, int castled
    , std::vector<std::vector<std::vector<int>>> piece_positions, std::vector<std::vector<int>> pieces, int kingmoved
    , int enpassant, std::vector<std::vector<int>> rookmoved){
        int piece_sign = (intsign(bot==0))*((nmoremoves%2 == 1)-(nmoremoves%2 == 0));
        //white == 0, black == 1
        int color = int(piece_sign!=1);
        int kingy = piece_positions[49][int(n0>0)][0];
        int kingx = piece_positions[49][int(n0>0)][1];
        if(kingy == -1){
            return -piece_sign*500000/(ntimes+1.0);
        }
        if(checkmate(piece_sign*50, board, piece_positions, pieces, kingmoved, enpassant, rookmoved, kingy, kingx)){
            return -piece_sign*500000/(ntimes-nmoremoves+1.0);
        }
        int temp_enpassant = enpassant;
        int temp_kingmoved = kingmoved;
        int temp_castled = castled;
        std::vector<std::vector<int>> temp_rookmoved = rookmoved;
        std::vector<std::vector<int>> temp_pieces = pieces;
        std::vector<std::vector<std::vector<int>>> temp_piece_positions = piece_positions;
        std::vector<std::vector<int>> temp_board = board;
        double movescore[332];
        int movescore_size = 0;
        double best_movescore = -piece_sign*1000000;
        double previous_movescore = evaluate_move(n0, y00, x00, y10, x10, board, castled);
        for(int n00 = 0; n00 < 6; n00++){
            int n1 = int(1*(n00 == 0)+2*(n00 == 1)+3*(n00 == 2)+4*(n00 == 3)
            + 5*(n00 == 5));
            if(n1 >= pieces.size()){
                std::cout << "n1 >= pieces.size(), n1 = " << n1 << '\n';
            }
            for(int n2 = 0; n2 < pieces[n1][(piece_sign!=1)]; n2++){
                int n = piece_sign*(10*n1+n2+int(n1 == 0));
                if(abs(n) > piece_positions.size()){
                    std::cout << "abs(n) > piece_positions.size(), n = " << n << '\n';
                }
                if(piece_positions[abs(n)-1][int(n<0)][0] != -1){
                    int y0 = piece_positions[abs(n)-1][int(n<0)][0];
                    int x0 = piece_positions[abs(n)-1][int(n<0)][1];
                    for(int y1 = 0; y1 < 8; y1++){
                        for(int x1 = 0; x1 < 8; x1++){
                            if(canmove(n, y0, x0, y1, x1, board, piece_positions, pieces, kingmoved, enpassant, rookmoved, kingy, kingx)){
                                if(nmoremoves%2 == 0){
                                    update_can_move_positions(color, abs(n), y1, x1, can_move_positions);
                                }
                                double evaluation_minus = evaluate_change(y1, x1, -1, board)+(n < 9 && x1*8+y1 == enpassant)*intsign(n)*1.0;
                                double current_movescore;
                                movepieceto(n, y0, x0, y1, x1, board, castled
                                , piece_positions, pieces, kingmoved, enpassant, rookmoved);
                                if(nmoremoves%2 == 0 && abs(n) < 10 && ((color == 0 && y1 == 7) 
                                || (color == 1 && y1 == 0))){
                                    update_can_move_positions(color, abs(board[y1][x1]), y1, x1, can_move_positions);
                                }
                                if(abs(n) == 50 && abs(x1-x0) > 1 && nmoremoves%2 == 0){
                                    update_can_move_positions(color, 30
                                    , piece_positions[30-1][color][0], piece_positions[30-1][color][1], can_move_positions);
                                    update_can_move_positions(color, 31
                                    , piece_positions[31-1][color][0], piece_positions[31-1][color][1], can_move_positions);
                                }
                                if(nmoremoves == 1){
                                    current_movescore = last_move(n, y0, x0, y1, x1, best_movescore-evaluation_minus
                                    , board, can_move_positions, castled, piece_positions, pieces, kingmoved, enpassant, rookmoved) 
                                        + evaluation_minus;
                                }else{
                                    current_movescore = nth_move(n, y0, x0, y1, x1, 
                                        best_movescore-evaluation_minus, nmoremoves-1, board, can_move_positions, castled, piece_positions
                                        , pieces, kingmoved, enpassant, rookmoved) + evaluation_minus;
                                }
                                double total_movescore = current_movescore 
                                    + previous_movescore;
                                //restore can_move_positions
                                if(nmoremoves%2 == 0){
                                    update_can_move_positions(color, abs(n), y0, x0, can_move_positions);
                                }
                                if(abs(n) == 50 && abs(x1-x0) > 1 && nmoremoves%2 == 0){
                                    update_can_move_positions(color, 30
                                    , piece_positions[30-1][color][0], piece_positions[30-1][color][1], can_move_positions);
                                    update_can_move_positions(color, 31
                                    , piece_positions[31-1][color][0], piece_positions[31-1][color][1], can_move_positions);
                                }
                                if((total_movescore <= best && (piece_sign!=1))
                                    || (total_movescore >= best && (piece_sign==1))){
                                    return total_movescore;
                                }
                                if((total_movescore < best_movescore && (piece_sign!=1))
                                    || (total_movescore > best_movescore && (piece_sign==1))){
                                    best_movescore = total_movescore;
                                }
                                if(movescore_size >= 332){
                                    std::cout << "movescore_size = " << movescore_size << '\n';
                                }
                                movescore[movescore_size] = total_movescore;
                                movescore_size++;
                                //return to saved state
                                enpassant = temp_enpassant;
                                piece_positions = temp_piece_positions;
                                kingmoved = temp_kingmoved;
                                castled = temp_castled;
                                rookmoved = temp_rookmoved;
                                pieces = temp_pieces;
                                board = temp_board;
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

    void firstmove(int moves, std::vector<std::vector<int>>& board, std::vector<std::vector<std::vector<int>>> positions
    , std::vector<std::vector<std::vector<int>>>& can_move_positions, int& castled
    , std::vector<std::vector<std::vector<int>>>& piece_positions, std::vector<std::vector<int>>& pieces
    , std::vector<std::vector<int>>& bestmove, int& kingmoved, int& enpassant, std::vector<std::vector<int>>& rookmoved, bool all = true){
        //save current state
        int temp_enpassant = enpassant;
        int temp_kingmoved = kingmoved;
        int temp_castled = castled;
        std::vector<std::vector<int>> temp_rookmoved = rookmoved;
        std::vector<std::vector<int>> temp_pieces = pieces;
        std::vector<std::vector<std::vector<int>>> temp_piece_positions = piece_positions;
        std::vector<std::vector<int>> temp_board = board;
        std::vector<double> movescore;
        double best_movescore = -intsign(bot == 0)*1000000;
        std::vector<std::vector<int>> order;
        if(all){
            order = reorder(moves, board, positions, castled, piece_positions, pieces, kingmoved, enpassant, rookmoved);
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
            double evaluation_minus = evaluate_change(y1, x1, -1, board)+(n < 9 && x1*8+y1 == enpassant)*intsign(n)*1.0;
            movepieceto(n, y0, x0, y1, x1, board, castled, piece_positions, pieces, kingmoved, enpassant, rookmoved);
            if(repetition(moves+1, board, positions)
            // || stalemate(50, new_board) || stalemate(-50, new_board)
            ){
                movescore.push_back(-fulleval(board, castled, piece_positions));
            }
            else if(partialrepetition(moves+1, board, positions)){
                if(bot == 0){
                    movescore.push_back(std::min(nth_move(n, y0, x0, y1, x1, best_movescore-evaluation_minus
                    , ntimes, board, can_move_positions, castled, piece_positions, pieces, kingmoved, enpassant, rookmoved) 
                    + evaluation_minus, -fulleval(board, castled, piece_positions)));
                }else{
                    movescore.push_back(std::max(nth_move(n, y0, x0, y1, x1, best_movescore-evaluation_minus
                    , ntimes, board, can_move_positions, castled, piece_positions, pieces, kingmoved, enpassant, rookmoved) 
                    + evaluation_minus, -fulleval(board, castled, piece_positions)));
                } 
            }
            else{
                double current_movescore = 
                    nth_move(n, y0, x0, y1, x1, best_movescore-evaluation_minus
                    , ntimes, board, can_move_positions, castled, piece_positions, pieces, kingmoved, enpassant, rookmoved) 
                    + evaluation_minus;
                movescore.push_back(current_movescore);
                if((current_movescore > best_movescore && bot == 0) 
                    || (current_movescore < best_movescore && bot == 1)){
                    best_movescore = current_movescore;
                }
            }

            //return to saved state
            enpassant = temp_enpassant;
            kingmoved = temp_kingmoved;
            castled = temp_castled;
            rookmoved = temp_rookmoved;
            pieces = temp_pieces;
            piece_positions = temp_piece_positions;
            board = temp_board;
        }
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

    std::vector<std::vector<std::vector<std::vector<int>>>> open_openingbook(const char* openingbook, int size) {
         // Create an input stream with the buffer data
        std::istringstream ss(std::string(openingbook, openingbook + size), std::ios::binary);

        // Read the size of the outer vector
        int outer_size;
        ss.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));

        // Read the contents of the vector
        std::vector<std::vector<std::vector<std::vector<int>>>> myvector(outer_size);
        for (int i = 0; i < outer_size; i++) {
            int outer_inner_size;
            ss.read(reinterpret_cast<char*>(&outer_inner_size), sizeof(outer_inner_size));
            myvector[i].resize(outer_inner_size);
            for (int j = 0; j < outer_inner_size; j++) {
                int inner_inner_size;
                ss.read(reinterpret_cast<char*>(&inner_inner_size), sizeof(inner_inner_size));
                myvector[i][j].resize(inner_inner_size);
                for (int k = 0; k < inner_inner_size; k++) {
                    int inner_most_size;
                    ss.read(reinterpret_cast<char*>(&inner_most_size), sizeof(inner_most_size));
                    myvector[i][j][k].resize(inner_most_size);
                    for (int l = 0; l < inner_most_size; l++) {
                    int value;
                    ss.read(reinterpret_cast<char*>(&value), sizeof(value));
                    myvector[i][j][k][l] = value;
                    }
                }
            }
        }

        return myvector;
    }

    bool compare_to_book(std::vector<std::vector<int>> book_board, std::vector<std::vector<int>>& board){
        for(int j = 0; j < 8; j++){
            for(int k = 0; k < 8; k++){
                if(book_board[j][k] != board[j][k]){
                    return false;
                }
            }
        }
        return true;
    }

    std::vector<int> read_openingbook(int color, const char* openingbook_data, int size, std::vector<std::vector<int>>& board){
        std::vector<std::vector<std::vector<std::vector<int>>>> openingbook = open_openingbook(openingbook_data, size);
        for(int i = 0; i < openingbook.size(); i++){
            if(compare_to_book(openingbook[i][0], board)){
                std::vector<int> bookmove = openingbook[i][1][0];
                std::cout << convert_to_png(bookmove[0], bookmove[1], bookmove[2], bookmove[3], bookmove[4]) << '\n';
                return {1, bookmove[0], bookmove[1], bookmove[2], bookmove[3], bookmove[4]};
            }
        }
        return {0};
    }

    const char* basicbot(const char* openingbook_data, int size, int moves, const char* board_string
    , const char* positions_string, const char* can_move_positions_str, int castled, const char* piece_positions_str
    , const char* pieces_str, int kingmoved, int enpassant, const char* rookmoved_str){
        std::cout << "updated0" << '\n';
        //define vectors
        std::vector<std::vector<int>> board = convert_board(board_string);
        std::vector<std::vector<std::vector<int>>> positions = convert_positions(positions_string, moves);
        std::vector<std::vector<std::vector<int>>> can_move_positions = string_to_vector_3d(can_move_positions_str);
        std::vector<std::vector<std::vector<int>>> piece_positions = string_to_vector_3d(piece_positions_str);
        std::vector<std::vector<int>> pieces = string_to_vector_2d(pieces_str);
        std::vector<std::vector<int>> rookmoved = string_to_vector_2d(rookmoved_str);
        std::vector<std::vector<int>> bestmove;

        if(read_openingbook(bot, openingbook_data, size, board)[0]){
            std::vector<int> book_result = read_openingbook(bot, openingbook_data, size, board);
            std::cout << "book" << '\n';
            return vector_to_string({book_result[1], book_result[2], book_result[3], book_result[4], book_result[5]});
        }
        double score = fulleval(board, castled, piece_positions);
        auto start = std::chrono::high_resolution_clock::now();
        firstmove(moves, board, positions, can_move_positions, castled, piece_positions, pieces, bestmove, kingmoved, enpassant, rookmoved);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast
            <std::chrono::milliseconds>(stop - start);
        while(duration.count()/1000.0 < 0.2){
            if(abs(bestmove[0][5]) > 10000){
                break;
            }
            ntimes += 2;
            firstmove(moves, board, positions, can_move_positions, castled, piece_positions, pieces, bestmove, kingmoved, enpassant, rookmoved);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast
                <std::chrono::milliseconds>(stop - start);
        }
        std::cout << "depth = " << ntimes/2+1;
        if(duration.count()/1000.0 < 0.4 && abs(bestmove[0][5]) <= 10000){
            ntimes += 2;
            firstmove(moves, board, positions, can_move_positions, castled, piece_positions
            , pieces, bestmove, kingmoved, enpassant, rookmoved, false);
            std::cout << '+';
        }
        std::cout << '\n';
        if(ntimes > ntimesmin){
            ntimes = ntimesmin;
        }
        score += bestmove[0][5];
        int n = bestmove[0][0];
        int y0 = bestmove[0][1];
        int x0 = bestmove[0][2];
        int y1 = bestmove[0][3];
        int x1 = bestmove[0][4];
        movepieceto(n, y0, x0, y1, x1, board, castled, piece_positions, pieces, kingmoved, enpassant, rookmoved);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast
            <std::chrono::milliseconds>(stop - start);
        std::cout << duration.count()/1000.0 << '\n';
        std::cout <<  convert_to_png(n, y0, x0, y1, x1) << ", " << score << '\n';
        return vector_to_string({n, y0, x0, y1, x1});
    }
}