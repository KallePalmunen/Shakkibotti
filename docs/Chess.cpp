#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>

extern "C" {
    class Chess{
    public:
        //evaluation reference tables from white's perspective
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
        //other variables
        int kingmoved; //%2 == 0 if white king has moved, %3 == 0 if black king has moved
        int enpassant; //x*8+y, -1 if no chance to enpassant
        int castled; //%2 == 0 if white has castled, %3 == 0 if black has castled
        std::array<std::array<int, 8>, 8> board;
        std::array<std::array<std::array<int, 2>,2>, 50> piece_positions;

        //constructor
        Chess(int kingmoved_input, int enpassant_input, int castled_input, std::array<std::array<int, 8>, 8> board_input
        , std::array<std::array<std::array<int, 2>,2>, 50> piece_positions_input){
            kingmoved = kingmoved_input;
            enpassant = enpassant_input;
            castled = castled_input;
            board = board_input;
            piece_positions = piece_positions_input;
        }

        //Copy values from another Chess object
        void Copy_game(Chess game_to_copy){
            kingmoved = game_to_copy.kingmoved;
            enpassant = game_to_copy.enpassant;
            castled = game_to_copy.castled;
            board = game_to_copy.board;
            piece_positions = game_to_copy.piece_positions;
        }
    };

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

    std::array<std::array<std::array<int, 2>,2>, 50> convert_piece_positions(const char* can_move_positions_str){
        int i = 1;
        int j = -1;
        int k = -1;
        std::array<std::array<std::array<int, 2>,2>, 50> result;
        while(i < strlen(can_move_positions_str)){
            j++;
            k = -1;
            while(i < strlen(can_move_positions_str)){
                std::vector<int> elements;
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
                        elements.push_back(std::stoi(element));
                    }
                    if(can_move_positions_str[i-1] == ']'){
                        result[j][k][0] = elements[0];
                        result[j][k][1] = elements[1];
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

    std::array<std::array<int, 8>, 8> convert_board(const char* board_string){
        int i = 2;
        std::array<std::array<int, 8>, 8> result;
        for(int y = 0; y < 8; y++){
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
                result[y][x] = std::stoi(element);
            }
        }
        return result;
    }

    std::vector<std::array<std::array<int, 8>, 8>> convert_positions(const char* positions_string, int moves){
        int i = 0;
        std::vector<std::array<std::array<int, 8>, 8>> result;
        for(int move = 0; move <= moves; move++){
            result.push_back({});
            for(int y = 0; y < 8; y++){
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
                    result[move][y][x] = std::stoi(element);
                }
            }
        }
        return result;
    }

    void printboard(const char* board_string){
        std::array<std::array<int, 8>, 8> board = convert_board(board_string);
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

    bool pawnmove(int piece, int y0, int x0, int y1, int x1, Chess& game){
        int piece_sign = int(piece>1)-int(piece<1);
        if(piece > 0){
            if((x1 == x0 && (y1-y0 == 1 || (y1-y0 == 2 && y0 == 1 
                && game.board[y1-1][x1] == 0)) && game.board[y1][x1] == 0) || 
                (y1-y0 == 1 && game.board[y1][x1] < 0 && 
                (x1 - x0 == 1 || x1 - x0 == -1)) 
                || (x1*8+y1 == game.enpassant && abs(x1-x0) == 1 && y1 - y0 == 1)){
                    return true;
            }
            return false;
        }if(piece < 0){
            if ((x1 == x0 && (y0-y1 == 1 || (y0-y1 == 2 && y0 == 6 
                && game.board[y1+1][x1] == 0)) && game.board[y1][x1] == 0) ||
                (y0-y1 == 1 && game.board[y1][x1] > 0 && 
                (x1 - x0 == 1 || x1 - x0 == -1))
                || (x1*8+y1 == game.enpassant && abs(x1-x0) == 1 && y0 - y1 == 1)){
                    return true;
            }
            return false;
        }
        return false;
    }

    bool knightmove(int piece, int y0, int x0, int y1, int x1, Chess& game){
        if(((y1 - y0 == 2 || y1 - y0 == -2) && (x1 - x0 == 1 || x1 - x0 == -1))
            || ((y1- y0 == 1 || y1 - y0 == -1) &&
            (x1 - x0 == 2 || x1 - x0 == -2))){
                if(piece > 0 && (game.board[y1][x1] == 0 || game.board[y1][x1] < 0)){
                    return true;
                }
                if(piece < 0 && (game.board[y1][x1] == 0 || game.board[y1][x1] > 0)){
                    return true;
                }
                return false;
            }
        return false;
    }

    //longmove checks if there is anything in the way when moving bishops, rooks and queens. It also checks whether there is a piece in the endsquare

    bool longmove(int piece, int y0, int x0, int y1, int x1, Chess& game){
        int y = y0, x = x0, xplus = (x1 != x0)*intsign(x1-x0)
        , yplus = (y1 != y0)*intsign(y1-y0);
        if(piece > 0){
            if(game.board[y1][x1] > 0){
                return false;
            }
            for(int i = 1; i < 8; i++){
                y += yplus;
                x += xplus;
                if(y == y1 && x == x1){
                    return true;
                }
                if(game.board[y][x] != 0){
                    return false;
                }
            }
            return false;
        }
        if(piece < 0){
            if(game.board[y1][x1] < 0){
                return false;
            }
            for(int i = 1; i < 8; i++){
                y += yplus;
                x += xplus;
                if(y == y1 && x == x1){
                    return true;
                }
                if(game.board[y][x] != 0){
                    return false;
                }
            }
            return false;
        }
        return false;
    }

    bool bishopmove(int piece, int y0, int x0, int y1, int x1, Chess& game){
        if(abs(y1-y0) == abs(x1-x0)){
            if(longmove(piece, y0, x0, y1, x1, game)){
                return true;
            }
            return false;
        }
        return false;
    }

    bool rookmove(int piece, int y0, int x0, int y1, int x1, Chess& game){
        if((y1 != y0 && x1 == x0) || (y1 == y0 && x1 != x0)){
            if(longmove(piece, y0, x0, y1, x1, game)){
                return true;
            }
            return false;
        }
        return false;
    }

    bool queenmove(int piece, int y0, int x0, int y1, int x1, Chess& game){
        if(bishopmove(piece, y0, x0, y1, x1, game) || rookmove(piece, y0, x0, y1, x1, game)){
            return true;
        }
        return false;
    }

    bool kingmove(int piece, int y0, int x0, int y1, int x1, Chess& game){
        if(abs(y1-y0) <= 1 && abs(x1-x0) <= 1 && 
        !(y1 == y0 && x1 == x0)){
            if(piece > 0 && game.board[y1][x1] <= 0){
                return true;
            }
            if(piece < 0 && game.board[y1][x1] >= 0){
                return true;
            }
        }
        return false;
    }

    bool piecemove(int piece, int y0, int x0, int y1, int x1, Chess& game){
        if(abs(piece) < 20 && abs(piece) >= 10){
            return knightmove(piece, y0, x0, y1, x1, game);
        }
        if(abs(piece) < 30 && abs(piece) >= 20){
            return bishopmove(piece, y0, x0, y1, x1, game);
        }
        if(abs(piece) < 10){
            return pawnmove(piece, y0, x0, y1, x1, game);
        }
        if(abs(piece) < 40 && abs(piece) >= 30){
            return rookmove(piece, y0, x0, y1, x1, game);
        }
        if(abs(piece) < 50 && abs(piece) >= 40){
            return queenmove(piece, y0, x0, y1, x1, game);
        }
        if(abs(piece) >= 50){
            return kingmove(piece, y0, x0, y1, x1, game);
        }
        return false;
    }

    bool promote(int piece, int y1){
        if(piece < 10 && piece > 0){
            if(y1 == 7){
                return true;
            }
        }
        if(piece > -10 && piece < 0){
            if(y1 == 0){
                return true;
            }
        }
        return false;
    }

    bool check(int piece, std::vector<std::vector<int>>& pieces, Chess& game, int kingy = -1, int kingx = -1){
        //find the position of the king if not provided
        if(kingy == -1) {
            bool found = false;
            for(int y = 0; y < 8; y++) {
                for(int x = 0; x < 8; x++) {
                    if(game.board[y][x] == piece) {
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
        for(int piece_type = 0; piece_type < 6; piece_type++){
            for(int n2 = 0; n2 < pieces[piece_type][(piece > 0)]; n2++){
                int piecen = -intsign(piece)*(piece_type*10+n2+(piece_type == 0));
                if(piecen != 0){
                    if(game.piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                        int y0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][0];
                        int x0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][1];
                        if(piecemove(piecen, y0, x0, kingy, kingx, game)){
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool botcheck(int piece, int kingy, int kingx, std::vector<int>& pinners, Chess& game){
        for(int i = 0; i < pinners.size(); i++){
            int piecen = pinners[i];
            if(game.piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                int y0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][0];
                int x0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][1];
                if(piecemove(piecen, y0, x0, kingy, kingx, game)){
                    return true;
                }
            }
        }
        return false;
    }

    bool castle(int piece, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& pieces, Chess& game
    , std::vector<std::vector<int>> rookmoved){
        if(game.kingmoved%((piece>0)*2+(piece<0)*3) == 0 || rookmoved[(piece>0)][x1>4] == 1){
            return false;
        }
        if(y1 == y0 && (x1 == 1 || x1 == 5) && !check(piece, pieces, game) &&
        game.board[y0][(x1 > 4)*7] == intsign(piece)*(30 + (x1 > 4))){
            for(int i = 1; i < 3 + (x1 > 4); i++){
                int squarex = x0 + i*intsign(x1-x0);
                if(game.board[y0][squarex] != 0){
                    return false;
                }
                game.board[y0][x0] = 0;
                game.board[y0][squarex] = piece;
                game.piece_positions[49][int(piece<0)][1] = squarex;
                if(i < 3 && check(piece, pieces, game)){
                    game.board[y0][x0] = piece;
                    game.board[y0][squarex] = 0;
                    game.piece_positions[49][int(piece<0)][1] = x0;
                    return false;
                }
                game.board[y0][x0] = piece;
                game.board[y0][squarex] = 0;
                game.piece_positions[49][int(piece<0)][1] = x0;
            }
            return true;
        }
        return false;
    }

    bool pin(int piece, int y0, int x0, int y1, int x1, int kingy, int kingx, std::vector<std::vector<int>>& pieces, Chess& game){
        game.board[y0][x0] = 0;
        int movetosquare = game.board[y1][x1];
        game.board[y1][x1] = piece;
        game.piece_positions[abs(piece)-1][int(piece<0)][0] = y1;
        game.piece_positions[abs(piece)-1][int(piece<0)][1] = x1;
        if(abs(movetosquare) > 0){
            game.piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = -1;
        }
        //checks if you can prevent the mate in next turn 
        //by enpassanting the checking piece
        int enpassanted = -100;
        if(game.enpassant >= 0 && x1*8+y1 == game.enpassant && abs(piece) < 10){
            int enpassant_square = game.board[y1-intsign(y1 - y0)][x1];
            if(abs(enpassant_square) > 0){
                enpassanted = enpassant_square;
                game.board[y1-intsign(y1 - y0)][x1] = 0;
                game.piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = -1;
            }
        }
        if(!check(intsign(piece)*50, pieces, game, kingy, kingx)){
            game.board[y0][x0] = piece;
            game.board[y1][x1] = movetosquare;
            game.piece_positions[abs(piece)-1][int(piece<0)][0] = y0;
            game.piece_positions[abs(piece)-1][int(piece<0)][1] = x0;
            if(abs(movetosquare) > 0){
                game.piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = y1;
            }
            if(enpassanted != -100){
                game.board[y1-intsign(y1 - y0)][x1] = enpassanted;
                game.piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = y1-intsign(y1 - y0);
            }
            return false;
        }
        game.board[y0][x0] = piece;
        game.board[y1][x1] = movetosquare;
        game.piece_positions[abs(piece)-1][int(piece<0)][0] = y0;
        game.piece_positions[abs(piece)-1][int(piece<0)][1] = x0;
        if(abs(movetosquare) > 0){
            game.piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = y1;
        }
        if(enpassanted != -100){
            game.board[y1-intsign(y1 - y0)][x1] = enpassanted;
            game.piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = y1-intsign(y1 - y0);
        }
        return true;
    }

    bool botpin(int piece, int y0, int x0, int y1, int x1, int kingy, int kingx, std::vector<int>& pinners, Chess& game){
        if(pinners.size() == 0){
            return false;
        }
        game.board[y0][x0] = 0;
        int movetosquare = game.board[y1][x1];
        game.board[y1][x1] = piece;
        game.piece_positions[abs(piece)-1][int(piece<0)][0] = y1;
        game.piece_positions[abs(piece)-1][int(piece<0)][1] = x1;
        if(abs(movetosquare) > 0){
            game.piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = -1;
        }
        //checks if you can prevent the mate in next turn 
        //by enpassanting the checking piece
        int enpassanted = -100;
        if(game.enpassant >= 0 && x1*8+y1 == game.enpassant && abs(piece) < 10){
            enpassanted = game.board[y1-intsign(y1 - y0)][x1];
            game.board[y1-intsign(y1 - y0)][x1] = 0;
            game.piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = -1;
        }
        if(!botcheck(intsign(piece)*50, kingy, kingx, pinners, game)){
            game.board[y0][x0] = piece;
            game.board[y1][x1] = movetosquare;
            game.piece_positions[abs(piece)-1][int(piece<0)][0] = y0;
            game.piece_positions[abs(piece)-1][int(piece<0)][1] = x0;
            if(abs(movetosquare) > 0){
                game.piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = y1;
            }
            if(enpassanted != -100){
                game.board[y1-intsign(y1 - y0)][x1] = enpassanted;
                game.piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = y1-intsign(y1 - y0);
            }
            return false;
        }
        game.board[y0][x0] = piece;
        game.board[y1][x1] = movetosquare;
        game.piece_positions[abs(piece)-1][int(piece<0)][0] = y0;
        game.piece_positions[abs(piece)-1][int(piece<0)][1] = x0;
        if(abs(movetosquare) > 0){
            game.piece_positions[abs(movetosquare)-1][int(movetosquare<0)][0] = y1;
        }
        if(enpassanted != -100){
            game.board[y1-intsign(y1 - y0)][x1] = enpassanted;
            game.piece_positions[abs(enpassanted)-1][int(enpassanted<0)][0] = y1-intsign(y1 - y0);
        }
        return true;
    }

    bool ispinnable(int piece, int y0, int x0, int kingy, int kingx, std::vector<int>& pinners, Chess& game){
        if(pinners.size() == 0){
            return false;
        }
        game.board[y0][x0] = 0;
        if(!botcheck(intsign(piece)*50, kingy, kingx, pinners, game)){
            game.board[y0][x0] = piece;
            return false;
        }
        game.board[y0][x0] = piece;
        return true;
    }

    bool canmove(int piece, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& pieces, Chess& game
    , std::vector<std::vector<int>>& rookmoved, int kingy = -1, int kingx = -1){
        if(piecemove(piece, y0, x0, y1, x1, game) 
        || (abs(piece) == 50 && castle(piece, y0, x0, y1, x1, pieces, game, rookmoved))){
            if(abs(piece) == 50){
                kingy = y1;
                kingx = x1;
            }
            if(!pin(piece, y0, x0, y1, x1, kingy, kingx, pieces, game)){
                return true;
            }
        }
        return false;
    }

    bool botcanmove(int piece, int y0, int x0, int y1, int x1, bool pinnable
    , std::vector<std::vector<int>>& pieces, std::vector<int>& pinners, Chess& game, int kingy = -1, int kingx = -1){
        if(piece * game.board[y1][x1] > 0){
            return false;
        }
        if(abs(piece) < 50){
            if(abs(piece) < 10){
                if(pawnmove(piece, y0, x0, y1, x1, game)){
                    if(!pinnable){
                        return true;
                    }
                    return (!botpin(piece, y0, x0, y1, x1, kingy, kingx, pinners, game));
                }
                return false;
            }
            if(!pinnable){
                return true;
            }
            return (!botpin(piece, y0, x0, y1, x1, kingy, kingx, pinners, game));
        }
        if(abs(piece) == 50){
            return (!pin(piece, y0, x0, y1, x1, y1, x1, pieces, game));
        }
        return false;
    }

    bool movesomewhere(int piece, int y0, int x0, std::vector<std::vector<int>>& pieces, Chess& game
    , std::vector<std::vector<int>>& rookmoved, int kingy = -1, int kingx = -1){
        for(int y1 = 0; y1 < 8; y1++){
            for(int x1 = 0; x1 < 8; x1++){
                if(canmove(piece, y0, x0, y1, x1, pieces, game, rookmoved, kingy, kingx)){
                    return true;
                }
            }
        }
        return false;
    }

    bool checkmate(int piece, std::vector<std::vector<int>>& pieces, Chess& game, std::vector<std::vector<int>>& rookmoved
    , int kingy = -1, int kingx = -1){
        if(!check(piece, pieces, game, kingy, kingx)){
            return false;
        }
        for(int piece_type = 0; piece_type < 6; piece_type++){
            for(int n2 = 0; n2 < pieces[piece_type][(piece < 0)]; n2++){
                int piecen = intsign(piece)*(piece_type*10+n2);
                if(piecen != 0){
                    if(game.piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                        int y0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][0];
                        int x0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][1];
                        if(movesomewhere(piecen, y0, x0, pieces, game, rookmoved, kingy, kingx)){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    bool botcheckmate(int piece, std::vector<std::vector<int>>& pieces
    , std::vector<int>& pinners, Chess& game, std::vector<std::vector<int>>& rookmoved, int kingy = -1, int kingx = -1){
        if(!botcheck(piece, kingy, kingx, pinners, game)){
            return false;
        }
        for(int piece_type = 0; piece_type < 6; piece_type++){
            for(int n2 = 0; n2 < pieces[piece_type][(piece < 0)]; n2++){
                int piecen = intsign(piece)*(piece_type*10+n2);
                if(piecen != 0){
                    if(game.piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                        int y0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][0];
                        int x0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][1];
                        if(movesomewhere(piecen, y0, x0, pieces, game, rookmoved, kingy, kingx)){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    void movepieceto(int piece, int y0, int x0, int y1, int x1, std::vector<std::vector<int>>& pieces, Chess& game
    , std::vector<std::vector<int>>& rookmoved){
        int promoteto;
        if(game.board[y1][x1] != 0){
            game.piece_positions[abs(game.board[y1][x1])-1][int(piece>0)][0] = -1;
        }
        if(abs(piece) == 50){
            if(abs(x1-x0) > 1){
                //castle
                int whichrook = intsign(piece)*(30 + (x1 > 4));
                int rookx = (x1 > 4)*7;
                game.board[y1][rookx] = 0;
                game.board[y1][x1 + intsign(4-x1)] = whichrook;
                game.castled *= (2*(piece > 0) + 3*(piece < 0));
                game.piece_positions[abs(whichrook)-1][int(piece<0)][0] = y1;
                game.piece_positions[abs(whichrook)-1][int(piece<0)][1] = x1 + intsign(4-x1);
            }
            game.kingmoved *= ((piece>0)*2 + (piece<0)*3);
        }
        if(abs(piece) == 30 || abs(piece) == 31){
            rookmoved[(piece>0)][abs(piece)-30] = 1;
        }
        if(promote(piece, y1)){
            promoteto = 4;
            game.board[y1][x1] = intsign(piece)*(promoteto*10+pieces[promoteto][(piece < 0)]);
            pieces[promoteto][(piece < 0)]++;
            game.piece_positions[abs(game.board[y1][x1])-1][int(game.board[y1][x1]<0)][0] = y1;
            game.piece_positions[abs(game.board[y1][x1])-1][int(game.board[y1][x1]<0)][1] = x1;
            game.piece_positions[abs(piece)-1][int(piece<0)][0] = -1;
        }else{
            game.board[y1][x1] = piece;
            game.piece_positions[abs(piece)-1][int(piece<0)][0] = y1;
            game.piece_positions[abs(piece)-1][int(piece<0)][1] = x1;
        }
        if(game.enpassant >= 0 && x1*8+y1 == game.enpassant && abs(piece) < 10){
            game.piece_positions[abs(game.board[y1-intsign(y1 - y0)][x1])-1][int(piece>0)][0] = -1;
            game.board[y1-intsign(y1 - y0)][x1] = 0;
        }
        if(abs(piece) < 10 && abs(y1-y0) > 1){
            game.enpassant = x1*8+y0+intsign(y1 - y0);
        }else{
            game.enpassant = -1;
        }
        game.board[y0][x0] = 0;
    }

    bool stalemate(int piece, std::vector<std::vector<int>>& pieces, Chess& game, std::vector<std::vector<int>>& rookmoved){
        if(check(piece, pieces, game)){
            return false;
        }
        for(int n1 = 1; n1 < 51; n1++){
            int piecen = intsign(piece)*n1;
            if(game.piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                int y0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][0];
                int x0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][1];
                if(movesomewhere(piecen, y0, x0, pieces, game, rookmoved)){
                    return false;
                }
            }
        }
        return true;
    }

    bool compareposition(int moment, Chess& game, std::vector<std::array<std::array<int, 8>, 8>> positions){
        for(int y = 0; y < 8; y++){
            for(int x = 0; x < 8; x++){
                if(positions[moment][y][x] != game.board[y][x]){
                    return false;
                }
            }
        }
        return true;
    }

    bool repetition(int this_moment, Chess& game, std::vector<std::array<std::array<int, 8>, 8>> positions){
        int repetitions = 0;
        for(int moment = 0; moment < this_moment; moment ++){
            if(compareposition(moment, game, positions)){
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
        std::array<std::array<int, 8>, 8> board = convert_board(board_string);
        std::vector<std::array<std::array<int, 8>, 8>> positions = convert_positions(positions_string, moves);
        std::array<std::array<std::array<int, 2>,2>, 50> piece_positions = convert_piece_positions(piece_positions_str);
        std::vector<std::vector<int>> pieces = string_to_vector_2d(pieces_str);
        std::vector<std::vector<int>> rookmoved = string_to_vector_2d(rookmoved_str);

        int castled = 0; //set a value for castled, this value does not matter here

        Chess game(kingmoved, enpassant, castled, board, piece_positions);

        if(checkmate(-50, pieces, game, rookmoved)){
            std::cout << "White won" << '\n';
            return 2;
        }
        if(checkmate(50, pieces, game, rookmoved)){
            std::cout << "Black won" << '\n';
            return 1;
        }
        if((turn == 0 && stalemate(50, pieces, game, rookmoved)) 
        || (turn == 1 && stalemate(-50, pieces, game, rookmoved)) || 
            repetition(moves, game, positions)){
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
                can_move_positions[piece-1][0] = {y0+1, x0};
                if(x0 < 7){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0+1, x0+1});
                }
                if(x0 > 0){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0+1, x0-1});
                }
                if(y0 == 1){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0+2, x0});
                }
                return can_move_positions;
            }
            if(color == 1){
                can_move_positions[piece-1][0] = {y0-1, x0};
                if(x0 < 7){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-1, x0+1});
                }
                if(x0 > 0){
                    can_move_positions[piece-1][0].insert(can_move_positions[piece-1][0].end(),{y0-1, x0-1});
                }
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

    int movepiece(int y0, int x0, int movetoy, int movetox, int turn, const char* board_string, int castled
    , const char* piece_positions_str, const char* pieces_str, int kingmoved, int enpassant, const char* rookmoved_str){
        std::array<std::array<int, 8>, 8> board = convert_board(board_string);
        std::array<std::array<std::array<int, 2>,2>, 50> piece_positions = convert_piece_positions(piece_positions_str);
        std::vector<std::vector<int>> pieces = string_to_vector_2d(pieces_str);
        std::vector<std::vector<int>> rookmoved = string_to_vector_2d(rookmoved_str);
        int piece = board[y0][x0];
        std::cout << piece << ',' << y0 << ',' << x0 << ',' << movetoy << ',' << movetox << '\n'; 

        Chess game(kingmoved, enpassant, castled, board, piece_positions);

        if((piece > 0 && turn == 0) || (piece < 0 && turn == 1)){
            if(movetoy < 8 && movetox < 8 && movetox >= 0 && movetoy >= 0 && y0>=0 
            && canmove(piece, y0, x0, movetoy, movetox, pieces, game, rookmoved)){
                movepieceto(piece, y0, x0, movetoy, movetox, pieces, game, rookmoved);
            }else{
                return 0;
            }
        }else{
            std::cout << turn << '\n';
            return 0;
        }
        return 1;
    }

    bool partialrepetition(int current_moment, Chess& game, std::vector<std::array<std::array<int, 8>, 8>> positions){
        for(int moment = 0; moment < current_moment; moment++){
            if(compareposition(moment, game, positions)){
                return true;
            }
        }
        return false;
    }

    std::vector<std::vector<std::vector<int>>> set_can_move_positions(Chess& game, int bot){
        std::vector<std::vector<std::vector<int>>> can_move_positions;
        for(int piece = 1; piece < 50; piece++){
            can_move_positions.push_back({});
            update_can_move_positions(int(bot == 0), piece, game.piece_positions[piece-1][int(bot ==0)][0]
            , game.piece_positions[piece-1][int(bot == 0)][1], can_move_positions);
        }
        can_move_positions.push_back({});
        return update_can_move_positions(int(bot == 0), 50, game.piece_positions[50-1][int(bot ==0)][0]
        , game.piece_positions[50-1][int(bot == 0)][1], can_move_positions);
    }

    std::vector<int> get_pinners(int piece_sign, int kingy, int kingx, std::vector<std::vector<int>>& pieces, Chess& game){
        std::vector<int> pinners;
        for(int piece_type = 0; piece_type < 6; piece_type++){
            for(int n2 = 0; n2 < pieces[piece_type][piece_sign!=1]; n2++){
                int n = piece_sign*(10*piece_type+n2+int(piece_type == 0));
                int y0 = game.piece_positions[abs(n)-1][int(n<0)][0];
                int x0 = game.piece_positions[abs(n)-1][int(n<0)][1];
                if(piecemove(n, y0, x0, kingy, kingx, game)){
                    pinners.push_back(n);
                }
            }
        }
        return pinners;
    }

    double evaluate_change(int y, int x, int changesign, int& pieceAt_yx, Chess& game, int piece = -100){
        if(piece == -100){
            piece = pieceAt_yx;
        }
        if(piece == 0){
            return 0.0;
        }
        //pawns
        if(piece != 0 && abs(piece) < 9){
            return changesign*intsign(piece)*(1+0.1*game.pawn_position_eval[(piece<0)*y+(piece>0)*(7-y)][(piece<0)*x+(piece>0)*(7-x)]);
        }
        //knights and bishops
        if(abs(piece) > 9 && abs(piece) < 20){
            return changesign*intsign(piece)*(3+0.1*game.knight_position_eval[(piece<0)*y+(piece>0)*(7-y)][(piece<0)*x+(piece>0)*(7-x)]);
        }
        //bishops
        if(abs(piece) > 19 && abs(piece) < 30){
            return changesign*intsign(piece)*(3+0.1*game.bishop_position_eval[(piece<0)*y+(piece>0)*(7-y)][(piece<0)*x+(piece>0)*(7-x)]);
        }
        //rooks
        if(abs(piece) > 29 && abs(piece) < 40){
            return changesign*intsign(piece)*(5+0.1*game.rook_position_eval[(piece<0)*y+(piece>0)*(7-y)][(piece<0)*x+(piece>0)*(7-x)]);
        }
        //queens
        if(abs(piece) > 39 && abs(piece) < 50){
            return changesign*intsign(piece)*(9+0.1*game.queen_position_eval[(piece<0)*y+(piece>0)*(7-y)][(piece<0)*x+(piece>0)*(7-x)]);
        }
        return 0.0;
    }

    double evaluate_move(int piece, int y0, int x0, int y1, int x1, Chess& game){
        return evaluate_change(y1, x1, 1, game.board[y1][x1], game, piece) + 
            evaluate_change(y0, x0, -1, game.board[y0][x0], game, piece) + (game.castled % 2 == 0)*0.1 - (game.castled % 3 == 0)*0.1;
    }

    double fulleval(Chess& game){
        double evaluation = 0;
        for(int piece = 1; piece < 51; piece++){
            if(game.piece_positions[piece-1][0][0] != -1){
                int y0 = game.piece_positions[piece-1][0][0];
                int x0 = game.piece_positions[piece-1][0][1];
                evaluation += evaluate_change(y0, x0, 1, game.board[y0][x0], game, piece);
            }
            if(game.piece_positions[piece-1][1][0] != -1){
                int y0 = game.piece_positions[piece-1][1][0];
                int x0 = game.piece_positions[piece-1][1][1];
                evaluation += evaluate_change(y0, x0, 1, game.board[y0][x0], game, -piece);
            }
        }
        evaluation -= 0.1*((game.castled%2 == 0) + (game.castled%3 == 0));
        return evaluation;
    }

    std::vector<std::vector<int>> reorder(int moves, std::vector<std::array<std::array<int, 8>, 8>> positions
    , std::vector<std::vector<int>>& pieces, Chess& game, std::vector<std::vector<int>>& rookmoved, int bot){
        //save current state
        Chess game_previous_state(game.kingmoved, game.enpassant, game.castled, game.board, game.piece_positions);
        std::vector<std::vector<int>> temp_rookmoved = rookmoved;
        std::vector<std::vector<int>> temp_pieces = pieces;
        std::vector<double> movescore;
        std::vector<std::vector<int>> starting_order;
        std::vector<std::array<std::array<int, 8>, 8>> temp_positions = positions;
        for(int n1 = 1; n1 < 51; n1++){
            int piece = n1; 
            if(bot == 1){piece=-n1;}
            if(game.piece_positions[abs(piece)-1][int(piece<0)][0] != -1){
                int y0 = game.piece_positions[abs(piece)-1][int(piece<0)][0];
                int x0 = game.piece_positions[abs(piece)-1][int(piece<0)][1];
                for(int y1 = 0; y1 < 8; y1++){
                    for(int x1 = 0; x1 < 8; x1++){
                        if(canmove(piece, y0, x0, y1, x1, pieces, game, rookmoved)){
                            double evaluation_minus = evaluate_change(y1, x1, -1, game.board[y1][x1], game)+(piece < 9 && x1*8+y1 == game.enpassant)*intsign(piece)*1.0;
                            movepieceto(piece, y0, x0, y1, x1, pieces, game, rookmoved);
                            positions.push_back(game.board);
                            if(repetition(moves+1, game, positions)
                            || stalemate(50, pieces, game, rookmoved) 
                            || stalemate(-50, pieces, game, rookmoved)
                            ){
                                movescore.push_back(-fulleval(game));
                            }else if(partialrepetition(moves+1, game, positions)){
                                if(bot == 0){
                                    movescore.push_back(std::min(evaluate_move(piece, y0, x0, y1, x1, game) 
                                    + evaluation_minus, -fulleval(game)));
                                }else{
                                    movescore.push_back(std::max(evaluate_move(piece, y0, x0, y1, x1, game) 
                                    + evaluation_minus, -fulleval(game)));
                                }    
                            }else{
                                movescore.push_back(
                                    evaluate_move(piece, y0, x0, y1, x1, game) 
                                    + evaluation_minus);
                            }
                            starting_order.insert(starting_order.end(),
                                {piece, y0, x0, y1, x1});

                            //return to saved state
                            game.Copy_game(game_previous_state);
                            rookmoved = temp_rookmoved;
                            pieces = temp_pieces;
                            positions = temp_positions;
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

    double last_move(int n0, int y00, int x00, int y10, int x10, double best
    , std::vector<std::vector<std::vector<int>>>& can_move_positions, std::vector<std::vector<int>>& pieces, Chess& game
    , std::vector<std::vector<int>>& rookmoved, int bot, int ntimes){
        int piece_sign = int(bot == 1)-int(bot == 0);
        int kingy = game.piece_positions[49][int(n0>0)][0];
        int kingx = game.piece_positions[49][int(n0>0)][1];
        if(kingy == -1){
            return -piece_sign*500000/(ntimes+1.0);
        }
        std::vector<int> pinners = get_pinners(-piece_sign, kingy, kingx, pieces, game);
        if(botcheckmate(piece_sign*50, pieces, pinners, game, rookmoved, kingy, kingx)){
            return -piece_sign*500000/(ntimes+1.0);
        }
        double previous_movescore = evaluate_move(n0, y00, x00, y10, x10, game);
        double movescore[304];
        int movescore_size = 0;
        double best_movescore = -piece_sign*1000000;
        for(int n00 = 0; n00 < 6; n00++){
            //Goes through the piece types in peculiar order
            int piece_type = int(1*(n00 == 0)+2*(n00 == 1)+3*(n00 == 2)+4*(n00 == 3)
            + 5*(n00 == 5));
            for(int n2 = 0; n2 < pieces[piece_type][piece_sign!=1]; n2++){
                int piece = piece_sign*(10*piece_type+n2+int(piece_type == 0));
                if(game.piece_positions[abs(piece)-1][int(piece<0)][0] != -1){
                    int y0 = game.piece_positions[abs(piece)-1][int(piece<0)][0];
                    int x0 = game.piece_positions[abs(piece)-1][int(piece<0)][1];
                    bool pinnable = ispinnable(piece, y0, x0, kingy, kingx, pinners, game);
                    for(int i = 0; i < can_move_positions[abs(piece)-1].size(); i++){
                        for(int j = 0; j+1 < can_move_positions[abs(piece)-1][i].size(); j+=2){
                            int y1 = can_move_positions[abs(piece)-1][i][j];
                            int x1 = can_move_positions[abs(piece)-1][i][j+1];
                            if(botcanmove(piece, y0, x0, y1, x1, pinnable, pieces
                            , pinners, game, kingy, kingx)){
                                //change to eval
                                double total_movescore = evaluate_move(piece, y0, x0, y1, x1, game)
                                    + evaluate_change(y1, x1, -1, game.board[y1][x1], game)+(piece < 9 && x1*8+y1 == game.enpassant)*intsign(piece)*1.0
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
                                if(abs(piece) > 19 && abs(piece) < 50 && game.board[y1][x1] != 0){
                                    break;
                                }
                            }else if(abs(piece) > 19 && abs(piece) < 50 && game.board[y1][x1] != 0){
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

    double nth_move(int n0, int y00, int x00, int y10, int x10, double best, int nmoremoves
    , std::vector<std::vector<std::vector<int>>> can_move_positions, std::vector<std::vector<int>> pieces, Chess& game
    , std::vector<std::vector<int>> rookmoved, int bot, int ntimes){
        int piece_sign = (intsign(bot==0))*((nmoremoves%2 == 1)-(nmoremoves%2 == 0));
        //white == 0, black == 1
        int color = int(piece_sign!=1);
        int kingy = game.piece_positions[49][int(n0>0)][0];
        int kingx = game.piece_positions[49][int(n0>0)][1];
        if(kingy == -1){
            return -piece_sign*500000/(ntimes+1.0);
        }
        if(checkmate(piece_sign*50, pieces, game, rookmoved, kingy, kingx)){
            return -piece_sign*500000/(ntimes-nmoremoves+1.0);
        }
        Chess game_previous_state(game.kingmoved, game.enpassant, game.castled, game.board, game.piece_positions);
        std::vector<std::vector<int>> temp_rookmoved = rookmoved;
        std::vector<std::vector<int>> temp_pieces = pieces;
        double movescore[332];
        int movescore_size = 0;
        double best_movescore = -piece_sign*1000000;
        double previous_movescore = evaluate_move(n0, y00, x00, y10, x10, game);
        for(int n00 = 0; n00 < 6; n00++){
            int piece_type = int(1*(n00 == 0)+2*(n00 == 1)+3*(n00 == 2)+4*(n00 == 3)
            + 5*(n00 == 5));
            for(int n2 = 0; n2 < pieces[piece_type][(piece_sign!=1)]; n2++){
                int piece = piece_sign*(10*piece_type+n2+int(piece_type == 0));
                if(game.piece_positions[abs(piece)-1][int(piece<0)][0] != -1){
                    int y0 = game.piece_positions[abs(piece)-1][int(piece<0)][0];
                    int x0 = game.piece_positions[abs(piece)-1][int(piece<0)][1];
                    for(int y1 = 0; y1 < 8; y1++){
                        for(int x1 = 0; x1 < 8; x1++){
                            if(canmove(piece, y0, x0, y1, x1, pieces, game, rookmoved, kingy, kingx)){
                                if(nmoremoves%2 == 0){
                                    update_can_move_positions(color, abs(piece), y1, x1, can_move_positions);
                                }
                                double evaluation_minus = evaluate_change(y1, x1, -1, game.board[y1][x1], game)+(piece < 9 && x1*8+y1 == game.enpassant)*intsign(piece)*1.0;
                                double current_movescore;
                                movepieceto(piece, y0, x0, y1, x1, pieces, game, rookmoved);
                                if(nmoremoves%2 == 0 && abs(piece) < 10 && ((color == 0 && y1 == 7) 
                                || (color == 1 && y1 == 0))){
                                    update_can_move_positions(color, abs(game.board[y1][x1]), y1, x1, can_move_positions);
                                }
                                if(abs(piece) == 50 && abs(x1-x0) > 1 && nmoremoves%2 == 0){
                                    update_can_move_positions(color, 30, game.piece_positions[30-1][color][0]
                                    , game.piece_positions[30-1][color][1], can_move_positions);
                                    update_can_move_positions(color, 31, game.piece_positions[31-1][color][0]
                                    , game.piece_positions[31-1][color][1], can_move_positions);
                                }
                                if(nmoremoves == 1){
                                    current_movescore = last_move(piece, y0, x0, y1, x1, best_movescore-evaluation_minus
                                    , can_move_positions, pieces, game, rookmoved, bot, ntimes) 
                                        + evaluation_minus;
                                }else{
                                    current_movescore = nth_move(piece, y0, x0, y1, x1, 
                                        best_movescore-evaluation_minus, nmoremoves-1, can_move_positions
                                        , pieces, game, rookmoved, bot, ntimes) + evaluation_minus;
                                }
                                double total_movescore = current_movescore 
                                    + previous_movescore;
                                //restore can_move_positions
                                if(nmoremoves%2 == 0){
                                    update_can_move_positions(color, abs(piece), y0, x0, can_move_positions);
                                }
                                if(nmoremoves%2 == 0 && abs(piece) < 10 && ((color == 0 && y1 == 7) 
                                || (color == 1 && y1 == 0))){
                                    update_can_move_positions(color, abs(game.board[y1][x1]), -1, -1, can_move_positions);
                                }
                                //return to saved state
                                game.Copy_game(game_previous_state);
                                rookmoved = temp_rookmoved;
                                pieces = temp_pieces;
                                if(abs(piece) == 50 && abs(x1-x0) > 1 && nmoremoves%2 == 0){
                                    update_can_move_positions(color, 30, game.piece_positions[30-1][color][0]
                                    , game.piece_positions[30-1][color][1], can_move_positions);
                                    update_can_move_positions(color, 31, game.piece_positions[31-1][color][0]
                                    , game.piece_positions[31-1][color][1], can_move_positions);
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

    void firstmove(int moves, std::vector<std::array<std::array<int, 8>, 8>> positions
    , std::vector<std::vector<std::vector<int>>> can_move_positions, std::vector<std::vector<int>> pieces
    , std::vector<std::vector<int>>& bestmove, Chess& game, std::vector<std::vector<int>> rookmoved, int bot
    , int ntimes, int plusamount, bool all = true){
        //save current state
        Chess game_previous_state(game.kingmoved, game.enpassant, game.castled, game.board, game.piece_positions);
        std::vector<std::vector<int>> temp_rookmoved = rookmoved;
        std::vector<std::vector<int>> temp_pieces = pieces;
        std::vector<std::array<std::array<int, 8>, 8>> temp_positions = positions;
        std::vector<double> movescore;
        double best_movescore = -intsign(bot == 0)*1000000;
        std::vector<std::vector<int>> order;
        int opponent_piece_sign = int(bot == 1)-int(bot == 0);
        if(all){
            order = reorder(moves, positions, pieces, game, rookmoved, bot);
        }else{
            for(int i = 0; i < plusamount; i++){
                order.push_back({bestmove[i][0],bestmove[i][1],bestmove[i][2],bestmove[i][3],bestmove[i][4]});
            }
        }
        if(order.size() == 1){
            bestmove.resize(0);
            bestmove.push_back({});
            for(int i = 0; i < 5; i++){
                bestmove[0].push_back(order[0][i]);
            }
            return;
        }
        for(int i = 0; i < order.size(); i++){
            int piece = order[i][0];
            int y0 = order[i][1];
            int x0 = order[i][2];
            int y1 = order[i][3];
            int x1 = order[i][4];
            double evaluation_minus = evaluate_change(y1, x1, -1, game.board[y1][x1], game)+(piece < 9 && x1*8+y1 == game.enpassant)*intsign(piece)*1.0;
            movepieceto(piece, y0, x0, y1, x1, pieces, game, rookmoved);
            positions.push_back(game.board);
            if(repetition(moves+1, game, positions) 
            || stalemate(opponent_piece_sign*50, pieces, game, rookmoved)){
                std::cout << "repetition" << '\n';
                movescore.push_back(-fulleval(game));
            }
            else if(partialrepetition(moves+1, game, positions)){
                std::cout << "partialrepetition" << '\n';
                if(bot == 0){
                    movescore.push_back(std::min(nth_move(piece, y0, x0, y1, x1, best_movescore-evaluation_minus
                    , ntimes, can_move_positions, pieces, game, rookmoved, bot, ntimes) 
                    + evaluation_minus, -fulleval(game)));
                }else{
                    movescore.push_back(std::max(nth_move(piece, y0, x0, y1, x1, best_movescore-evaluation_minus
                    , ntimes, can_move_positions, pieces, game, rookmoved, bot, ntimes) 
                    + evaluation_minus, -fulleval(game)));
                } 
            }
            else{
                double current_movescore = 
                    nth_move(piece, y0, x0, y1, x1, best_movescore-evaluation_minus
                    , ntimes, can_move_positions, pieces, game, rookmoved, bot, ntimes) 
                    + evaluation_minus;
                movescore.push_back(current_movescore);
                if((current_movescore > best_movescore && bot == 0) 
                    || (current_movescore < best_movescore && bot == 1)){
                    best_movescore = current_movescore;
                }
            }

            //return to saved state
            game.Copy_game(game_previous_state);
            rookmoved = temp_rookmoved;
            pieces = temp_pieces;
            positions = temp_positions;
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

    bool compare_to_book(std::vector<std::vector<int>> book_board, Chess& game){
        for(int j = 0; j < 8; j++){
            for(int k = 0; k < 8; k++){
                if(book_board[j][k] != game.board[j][k]){
                    return false;
                }
            }
        }
        return true;
    }

    std::vector<int> read_openingbook(int color, const char* openingbook_data, int size, Chess& game){
        std::vector<std::vector<std::vector<std::vector<int>>>> openingbook = open_openingbook(openingbook_data, size);
        for(int i = 0; i < openingbook.size(); i++){
            if(compare_to_book(openingbook[i][0], game)){
                std::vector<int> bookmove = openingbook[i][1][0];
                std::cout << convert_to_png(bookmove[0], bookmove[1], bookmove[2], bookmove[3], bookmove[4]) << '\n';
                return {1, bookmove[0], bookmove[1], bookmove[2], bookmove[3], bookmove[4]};
            }
        }
        return {0};
    }

    const char* basicbot(const char* openingbook_data, int size, int moves, const char* board_string, const char* positions_string
    , int castled, const char* piece_positions_str
    , const char* pieces_str, int kingmoved, int enpassant, const char* rookmoved_str, int bot){
        std::cout << "updated0" << '\n';
        //define vectors
        std::array<std::array<int, 8>, 8> board = convert_board(board_string);
        std::vector<std::array<std::array<int, 8>, 8>> positions = convert_positions(positions_string, moves);
        std::array<std::array<std::array<int, 2>,2>, 50> piece_positions = convert_piece_positions(piece_positions_str);
        std::vector<std::vector<int>> pieces = string_to_vector_2d(pieces_str);
        std::vector<std::vector<int>> rookmoved = string_to_vector_2d(rookmoved_str);

        //Create Chess object
        Chess game(kingmoved, enpassant, castled, board, piece_positions);
        
        int ntimesmin = 4;
        //ntimes == (amount of half moves that basicbot searches forward) - 2
        int ntimes = ntimesmin;
        //amount of moves calculated one full move deeper
        int plusamount = 2;
        
        std::vector<std::vector<int>> bestmove;
        std::vector<std::vector<std::vector<int>>> can_move_positions = set_can_move_positions(game, bot);

        if(read_openingbook(bot, openingbook_data, size, game)[0]){
            std::vector<int> book_result = read_openingbook(bot, openingbook_data, size, game);
            std::cout << "book" << '\n';
            return vector_to_string({book_result[1], book_result[2], book_result[3], book_result[4], book_result[5]});
        }
        double score = fulleval(game);
        auto start = std::chrono::high_resolution_clock::now();
        firstmove(moves, positions, can_move_positions, pieces, bestmove, game, rookmoved, bot
        , ntimes, plusamount);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast
            <std::chrono::milliseconds>(stop - start);
        while(duration.count()/1000.0 < 0.2){
            if(abs(bestmove[0][5]) > 10000 || bestmove.size() <= 1){
                break;
            }
            ntimes += 2;
            firstmove(moves, positions, can_move_positions, pieces, bestmove, game, rookmoved, bot, ntimes, plusamount);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast
                <std::chrono::milliseconds>(stop - start);
        }
        std::cout << "depth = " << ntimes/2+1;
        if(duration.count()/1000.0 < 0.4 && abs(bestmove[0][5]) <= 10000 && bestmove.size() > 1){
            ntimes += 2;
            firstmove(moves, positions, can_move_positions, pieces, bestmove, game, rookmoved, bot, ntimes, plusamount, false);
            std::cout << '+';
        }
        std::cout << '\n';
        if(ntimes > ntimesmin){
            ntimes = ntimesmin;
        }
        score += bestmove[0][5];
        int piece = bestmove[0][0];
        int y0 = bestmove[0][1];
        int x0 = bestmove[0][2];
        int y1 = bestmove[0][3];
        int x1 = bestmove[0][4];
        movepieceto(piece, y0, x0, y1, x1, pieces, game, rookmoved);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast
            <std::chrono::milliseconds>(stop - start);
        std::cout << duration.count()/1000.0 << '\n';
        std::cout <<  convert_to_png(piece, y0, x0, y1, x1) << ", " << score << '\n';
        return vector_to_string({piece, y0, x0, y1, x1});
    }
}