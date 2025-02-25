#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>

extern "C" {
    std::array<std::array<std::array<std::array<std::vector<std::vector<int>>, 8>, 8>, 6>, 2> update_canMoveTo(int color, int pieceType, int y, int x, std::array<std::array<std::array<std::array<std::vector<std::vector<int>>, 8>, 8>, 6>, 2>& canMoveTo){
        canMoveTo[color][pieceType][y][x].resize(0);
        if(pieceType == 1){
            canMoveTo[color][pieceType][y][x].push_back({});
            if(y > 0){
                if(x < 6){
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-1, x+2});
                }
                if(x > 1){
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-1, x-2});
                }
                if(y > 1){
                    if(x < 7){
                        canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-2, x+1});
                    }
                    if(x > 0){
                        canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-2, x-1});
                    }
                }
            }
            if(y < 7){
                if(x < 6){
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y+1, x+2});
                }
                if(x > 1){
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y+1, x-2});
                }
                if(y < 6){
                    if(x < 7){
                        canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y+2, x+1});
                    }
                    if(x > 0){
                        canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y+2, x-1});
                    }
                }
            }
            return canMoveTo;
        }
        if(pieceType == 0){
            canMoveTo[color][pieceType][y][x].push_back({});
            if(color == 0){
                canMoveTo[color][pieceType][y][x][0] = {y+1, x};
                if(x < 7){
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y+1, x+1});
                }
                if(x > 0){
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y+1, x-1});
                }
                if(y == 1){
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y+2, x});
                }
                return canMoveTo;
            }
            if(color == 1){
                canMoveTo[color][pieceType][y][x][0] = {y-1, x};
                if(x < 7){
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-1, x+1});
                }
                if(x > 0){
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-1, x-1});
                }
                if(y == 6){
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-2, x});
                }
                return canMoveTo;
            }
            return canMoveTo;
        }
        if(pieceType == 2){
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x-i >= 0 && y-i >= 0; i++){
                canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-i, x-i});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x+i < 8 && y-i >= 0; i++){
                canMoveTo[color][pieceType][y][x][1].insert(canMoveTo[color][pieceType][y][x][1].end(),{y-i, x+i});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x+i < 8 && y+i < 8; i++){
                canMoveTo[color][pieceType][y][x][2].insert(canMoveTo[color][pieceType][y][x][2].end(),{y+i, x+i});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x-i >= 0 && y+i < 8; i++){
                canMoveTo[color][pieceType][y][x][3].insert(canMoveTo[color][pieceType][y][x][3].end(),{y+i, x-i});
            }
            return canMoveTo;
        }
        if(pieceType == 3){
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x-i >= 0; i++){
                canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y, x-i});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x+i < 8; i++){
                canMoveTo[color][pieceType][y][x][1].insert(canMoveTo[color][pieceType][y][x][1].end(),{y, x+i});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; y-i >= 0; i++){
                canMoveTo[color][pieceType][y][x][2].insert(canMoveTo[color][pieceType][y][x][2].end(),{y-i, x});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; y+i < 8; i++){
                canMoveTo[color][pieceType][y][x][3].insert(canMoveTo[color][pieceType][y][x][3].end(),{y+i, x});
            }
            return canMoveTo;
        }
        if(pieceType == 4){
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x-i >= 0 && y-i >= 0; i++){
                canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-i, x-i});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x+i < 8 && y-i >= 0; i++){
                canMoveTo[color][pieceType][y][x][1].insert(canMoveTo[color][pieceType][y][x][1].end(),{y-i, x+i});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x+i < 8 && y+i < 8; i++){
                canMoveTo[color][pieceType][y][x][2].insert(canMoveTo[color][pieceType][y][x][2].end(),{y+i, x+i});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x-i >= 0 && y+i < 8; i++){
                canMoveTo[color][pieceType][y][x][3].insert(canMoveTo[color][pieceType][y][x][3].end(),{y+i, x-i});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x-i >= 0; i++){
                canMoveTo[color][pieceType][y][x][4].insert(canMoveTo[color][pieceType][y][x][4].end(),{y, x-i});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; x+i < 8; i++){
                canMoveTo[color][pieceType][y][x][5].insert(canMoveTo[color][pieceType][y][x][5].end(),{y, x+i});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; y-i >= 0; i++){
                canMoveTo[color][pieceType][y][x][6].insert(canMoveTo[color][pieceType][y][x][6].end(),{y-i, x});
            }
            canMoveTo[color][pieceType][y][x].push_back({});
            for(int i = 1; y+i < 8; i++){
                canMoveTo[color][pieceType][y][x][7].insert(canMoveTo[color][pieceType][y][x][7].end(),{y+i, x});
            }
            return canMoveTo;
        }
        if(pieceType == 5){
            canMoveTo[color][pieceType][y][x].push_back({});
            if(y > 0){
                canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-1, x});
                if(x > 0){
                canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-1, x-1}); 
                }
                if(x < 7){
                canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y-1, x+1}); 
                }
            }
            if(x > 0){
                canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y, x-1});
                if(x == 3){
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y, x-2});
                    canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y, x+2});
                }
            }
            if(x < 7){
                canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y, x+1});
            }
            if(y < 7){
                canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y+1, x});
                if(x > 0){
                canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y+1, x-1}); 
                }
                if(x < 7){
                canMoveTo[color][pieceType][y][x][0].insert(canMoveTo[color][pieceType][y][x][0].end(),{y+1, x+1}); 
                }
            }
            return canMoveTo;
        }
        return canMoveTo;
    }
        
    std::array<std::array<std::array<std::array<std::vector<std::vector<int>>, 8>, 8>, 6>, 2> set_canMoveTo(){
        std::array<std::array<std::array<std::array<std::vector<std::vector<int>>, 8>, 8>, 6>, 2> canMoveTo;
        for(int color = 0; color <= 1; color++){
            for(int pieceType = 0; pieceType <= 5; pieceType++){
                for(int y = 0; y < 8; y++){
                    for(int x = 0; x < 8; x++){
                        update_canMoveTo(color, pieceType, x, y, canMoveTo);
                    }
                }
            }
        }
        return canMoveTo;
    }

    class Chess{
    public:
        //Possible squares pieces can move to
        std::array<std::array<std::array<std::array<std::vector<std::vector<int>>, 8>, 8>, 6>, 2> canMoveTo = set_canMoveTo();
        //evaluation reference tables from white's perspective
        float pawn_position_eval[8][8] = {{80.0f,80.0f,80.0f,80.0f,80.0f,80.0f,80.0f,80.0f},{0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f}
        ,{0.1f,0.1f,0.2f,0.3f,0.3f,0.2f,0.1f,0.1f},{0.05f,0.05f,0.1f,0.25f,0.25f,0.1f,0.05f,0.05f},{0.0f,0.0f,0.0f,0.2f,0.2f,0.0f,0.0f,0.0f}
        ,{0.05f,-0.05f,-0.1f,0.0f,0.0f,-0.1f,-0.05f,0.05f},{0.05f,0.1f,0.1f,-0.2f,-0.2f,0.1f,0.1f,0.05f},{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}};
        float knight_position_eval[8][8] = {{-0.5f,-0.4f,-0.3f,-0.3f,-0.3f,-0.3f,-0.4f,-0.5f},{-0.4f,-0.2f,0.0f,0.0f,0.0f,0.0f,-0.2f,-0.4f}
        ,{-0.3f,0.0f,0.1f,0.15f,0.15f,0.1f,0.0f,-0.3f},{-0.3f,0.05f,0.15f,0.2f,0.2f,0.15f,0.05f,-0.3f},{-0.3f,0.05f,0.15f,0.2f,0.2f,0.15f,0.05f,-0.3f}
        ,{-0.3f,0.0f,0.1f,0.15f,0.15f,0.1f,0.0f,-0.3f},{-0.4f,-0.2f,0.0f,0.0f,0.0f,0.0f,-0.2f,-0.4f},{-0.5f,-0.4f,-0.3f,-0.3f,-0.3f,-0.3f,-0.4f,-0.5f}};
        float bishop_position_eval[8][8] = {{-0.2f,-0.1f,-0.1f,-0.1f,-0.1f,-0.1f,-0.1f,-0.2f},{-0.1f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.1f}
        ,{-0.1f,0.0f,0.05f,0.1f,0.1f,0.05f,0.0f,-0.1f},{-0.1f,0.05f,0.05f,0.1f,0.1f,0.05f,0.05f,-0.1f},{-0.1f,0.0f,0.1f,0.1f,0.1f,0.1f,0.0f,-0.1f}
        ,{-0.1f,0.1f,0.01f,0.1f,0.1f,0.1f,0.1f,-0.1f},{-0.1f,0.05f,0.0f,0.0f,0.0f,0.0f,0.05f,-0.1f},{-0.2f,-0.1f,-0.1f,-0.1f,-0.1f,-0.1f,-0.1f,-0.2f}};
        float rook_position_eval[8][8] = {{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},{0.05f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.05f}
        ,{-0.05f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.05f},{-0.05f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.05f},{-0.05f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.05f}
        ,{-0.05f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.05f},{-0.05f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.05f},{0.0f,0.0f,0.0f,0.5f,0.5f,0.0f,0.0f,0.0f}};
        float queen_position_eval[8][8] = {{-0.2f,-0.1f,-0.1f,-0.05f,-0.05f,-0.1f,-0.1f,-0.2f},{-0.1f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.1f}
        ,{-0.1f,0.0f,0.05f,0.05f,0.05f,0.05f,0.0f,-0.1f},{-0.05f,0.0f,0.05f,0.05f,0.05f,0.05f,0.0f,-0.05f},{0.0f,0.0f,0.05f,0.05f,0.05f,0.05f,0.0f,-0.05f}
        ,{-0.1f,0.05f,0.05f,0.05f,0.05f,0.05f,0.0f,-0.1f},{-0.1f,0.0f,0.05f,0.0f,0.0f,0.0f,0.0f,-0.1f},{-0.2f,-0.1f,-0.1f,-0.05f,-0.05f,-0.1f,-0.1f,-0.2f}};
        float king_position_eval[8][8] = {{-0.3f,-0.4f,-0.4f,-0.5f,-0.5f,-0.4f,-0.4f,-0.3f},{-0.3f,-0.4f,-0.4f,-0.5f,-0.5f,-0.4f,-0.4f,-0.3f}
        ,{-0.3f,-0.4f,-0.4f,-0.5f,-0.5f,-0.4f,-0.4f,-0.3f},{-0.3f,-0.4f,-0.4f,-0.5f,-0.5f,-0.4f,-0.4f,-0.3f},{-0.2f,-0.3f,-0.3f,-0.4f,-0.4f,-0.3f,-0.3f,-0.2f}
        ,{-0.1f,-0.2f,-0.2f,-0.2f,-0.2f,-0.2f,-0.2f,-0.1f},{0.2f,0.2f,0.0f,0.0f,0.0f,0.0f,0.2f,0.2f},{0.2f,0.3f,0.1f,0.0f,0.0f,0.1f,0.3f,0.2f}};
        //other variables
        int kingmoved; //%2 == 0 if white king has moved, %3 == 0 if black king has moved
        int enpassant; //x*8+y, -1 if no chance to enpassant
        int castled; //%2 == 0 if white has castled, %3 == 0 if black has castled
        std::array<std::array<int, 8>, 8> board;
        std::array<std::array<std::array<int, 2>,2>, 50> piece_positions;
        std::array<std::array<int, 2>,2> rookmoved; //black left, right - white left, right
        std::array<std::array<int, 2>,6> pieces; //number of pawns, knights, bishops, rooks, queens and kings (W,B)

        //constructor
        Chess(int kingmoved_input, int enpassant_input, int castled_input, std::array<std::array<int, 8>, 8> board_input
        , std::array<std::array<std::array<int, 2>,2>, 50> piece_positions_input, std::array<std::array<int, 2>,2> rookmoved_input
        , std::array<std::array<int, 2>,6> pieces_input){
            kingmoved = kingmoved_input;
            enpassant = enpassant_input;
            castled = castled_input;
            board = board_input;
            piece_positions = piece_positions_input;
            rookmoved = rookmoved_input;
            pieces = pieces_input;
        }

        //Copy values from another Chess object
        void Copy_game(Chess game_to_copy){
            kingmoved = game_to_copy.kingmoved;
            enpassant = game_to_copy.enpassant;
            castled = game_to_copy.castled;
            board = game_to_copy.board;
            piece_positions = game_to_copy.piece_positions;
            rookmoved = game_to_copy.rookmoved;
            pieces = game_to_copy.pieces;
        }
    };

    //Converts a piece to the corresponding PGN letter
    std::string piece_to_letter(int piece){
        if(abs(piece) > 9 && abs(piece) < 20){
            return "N";
        }
        if(abs(piece) > 19 && abs(piece) < 30){
            return "B";
        }
        if(abs(piece) > 29 && abs(piece) < 40){
            return "R";
        }
        if(abs(piece) > 39 && abs(piece) < 50){
            return "Q";
        }
        if(abs(piece) > 49){
            return "K";
        }
        return "";
    }

    //Converts a file to the corresponding PGN letter
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

    //Converts a PGN letter to the corresponding file number
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

    //Converts move to pseudo pgn format (doesn't include information about captures etc)
    std::string convert_to_pgn(int piece, int y0, int x0, int y1, int x1){
        return piece_to_letter(piece)  + file_to_letter(x0) + std::to_string(y0+1) + file_to_letter(x1) + std::to_string(y1+1);
    }

    //Gets the variable values for a move from pseudo pgn format (doesn't include information about captures etc)
    std::vector<int> convert_from_pgn(std::string move){
        return {int(move[1]-'0')-1, letter_to_file(move[0]), int(move[3]-'0')-1, letter_to_file(move[2])};
    }

    //Converts a vector to a string (for example {0,1,2,3} -> "{0,1,2,3}")
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

    //Converts a 2d vector to a string (for example {{0,1},{2,3}} -> "{{0,1},{2,3}}")
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

    //Converts the variable rookmoved from vector to array
    std::array<std::array<int, 2>,2> rookmoved_to_array(std::vector<std::vector<int>> rookmoved_vector){
        std::array<std::array<int, 2>,2> rookmoved_array;
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                rookmoved_array[i][j] = rookmoved_vector[i][j];
            }
        }
        return rookmoved_array;
    }

    //Converts the variable pieces from vector to array
    std::array<std::array<int, 2>,6> pieces_to_array(std::vector<std::vector<int>> pieces_vector){
        std::array<std::array<int, 2>,6> pieces_array;
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 2; j++){
                pieces_array[i][j] = pieces_vector[i][j];
            }
        }
        return pieces_array;
    }

    //Converts the variable piece_positions from string to array
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

    //Converts the variable board from string to array
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

    //Converts the variable positions from string to array
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

    //prints out the elements in board
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

    bool check(int piece, Chess& game, int kingy = -1, int kingx = -1){
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
            for(int piece_number = 0; piece_number < game.pieces[piece_type][(piece > 0)]; piece_number++){
                int piecen = -intsign(piece)*(piece_type*10+piece_number+(piece_type == 0));
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

    bool castle(int piece, int y0, int x0, int y1, int x1, Chess& game){
        if(game.kingmoved%((piece>0)*2+(piece<0)*3) == 0 || game.rookmoved[(piece>0)][x1>4] == 1){
            return false;
        }
        if(y1 == y0 && (x1 == 1 || x1 == 5) && !check(piece, game) &&
        game.board[y0][(x1 > 4)*7] == intsign(piece)*(30 + (x1 > 4))){
            for(int i = 1; i < 3 + (x1 > 4); i++){
                int squarex = x0 + i*intsign(x1-x0);
                if(game.board[y0][squarex] != 0){
                    return false;
                }
                game.board[y0][x0] = 0;
                game.board[y0][squarex] = piece;
                game.piece_positions[49][int(piece<0)][1] = squarex;
                if(i < 3 && check(piece, game)){
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

    bool pin(int piece, int y0, int x0, int y1, int x1, int kingy, int kingx, Chess& game){
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
        if(!check(intsign(piece)*50, game, kingy, kingx)){
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

    bool canmove(int piece, int y0, int x0, int y1, int x1, Chess& game
    , int kingy = -1, int kingx = -1){
        if(piecemove(piece, y0, x0, y1, x1, game) 
        || (abs(piece) == 50 && castle(piece, y0, x0, y1, x1, game))){
            if(abs(piece) == 50){
                kingy = y1;
                kingx = x1;
            }
            if(!pin(piece, y0, x0, y1, x1, kingy, kingx, game)){
                return true;
            }
        }
        return false;
    }

    bool botcanmove(int piece, int y0, int x0, int y1, int x1, bool pinnable
    , std::vector<int>& pinners, Chess& game, int kingy = -1, int kingx = -1){
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
            return (!pin(piece, y0, x0, y1, x1, y1, x1, game));
        }
        return false;
    }

    bool movesomewhere(int piece, int y0, int x0, Chess& game, int kingy = -1, int kingx = -1){
        for(int y1 = 0; y1 < 8; y1++){
            for(int x1 = 0; x1 < 8; x1++){
                if(canmove(piece, y0, x0, y1, x1, game, kingy, kingx)){
                    return true;
                }
            }
        }
        return false;
    }

    bool checkmate(int piece, Chess& game, int kingy = -1, int kingx = -1){
        if(!check(piece, game, kingy, kingx)){
            return false;
        }
        for(int piece_type = 0; piece_type < 6; piece_type++){
            for(int piece_number = 0; piece_number < game.pieces[piece_type][(piece < 0)]; piece_number++){
                int piecen = intsign(piece)*(piece_type*10+piece_number+(piece_type == 0));
                if(piecen != 0){
                    if(game.piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                        int y0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][0];
                        int x0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][1];
                        if(movesomewhere(piecen, y0, x0, game, kingy, kingx)){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    bool botcheckmate(int piece, std::vector<int>& pinners, Chess& game, int kingy = -1, int kingx = -1){
        if(!botcheck(piece, kingy, kingx, pinners, game)){
            return false;
        }
        for(int piece_type = 0; piece_type < 6; piece_type++){
            for(int piece_number = 0; piece_number < game.pieces[piece_type][(piece < 0)]; piece_number++){
                int piecen = intsign(piece)*(piece_type*10+piece_number+(piece_type == 0));
                if(piecen != 0){
                    if(game.piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                        int y0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][0];
                        int x0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][1];
                        if(movesomewhere(piecen, y0, x0, game, kingy, kingx)){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    void movepieceto(int piece, int y0, int x0, int y1, int x1, Chess& game){
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
            game.rookmoved[(piece>0)][abs(piece)-30] = 1;
        }
        if(promote(piece, y1)){
            promoteto = 4;
            game.board[y1][x1] = intsign(piece)*(promoteto*10+game.pieces[promoteto][(piece < 0)]);
            game.pieces[promoteto][(piece < 0)]++;
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

    bool stalemate(int piece, Chess& game){
        if(check(piece, game)){
            return false;
        }
        for(int n1 = 1; n1 < 51; n1++){
            int piecen = intsign(piece)*n1;
            if(game.piece_positions[abs(piecen)-1][int(piecen<0)][0] != -1){
                int y0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][0];
                int x0 = game.piece_positions[abs(piecen)-1][int(piecen<0)][1];
                if(movesomewhere(piecen, y0, x0, game)){
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
        for(int moment = 0; moment < this_moment; moment++){
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
        std::array<std::array<int, 2>,6> pieces = pieces_to_array(string_to_vector_2d(pieces_str));
        std::array<std::array<int, 2>,2> rookmoved = rookmoved_to_array(string_to_vector_2d(rookmoved_str));

        int castled = 0; //set a value for castled, this value does not matter here

        Chess game(kingmoved, enpassant, castled, board, piece_positions, rookmoved, pieces);

        if(checkmate(-50, game)){
            std::cout << "White won" << '\n';
            return 2;
        }
        if(checkmate(50, game)){
            std::cout << "Black won" << '\n';
            return 1;
        }
        if((turn == 0 && stalemate(50, game)) 
        || (turn == 1 && stalemate(-50, game)) || 
            repetition(moves, game, positions)){
                std::cout << "Draw" << '\n';
                return 0;
        }
        return -1;
    }

    

    int movepiece(int y0, int x0, int movetoy, int movetox, int turn, const char* board_string, int castled
    , const char* piece_positions_str, const char* pieces_str, int kingmoved, int enpassant, const char* rookmoved_str){
        std::array<std::array<int, 8>, 8> board = convert_board(board_string);
        std::array<std::array<std::array<int, 2>,2>, 50> piece_positions = convert_piece_positions(piece_positions_str);
        std::array<std::array<int, 2>,6> pieces = pieces_to_array(string_to_vector_2d(pieces_str));
        std::array<std::array<int, 2>,2> rookmoved = rookmoved_to_array(string_to_vector_2d(rookmoved_str));
        int piece = board[y0][x0];
        std::cout << piece << ',' << y0 << ',' << x0 << ',' << movetoy << ',' << movetox << '\n'; 

        Chess game(kingmoved, enpassant, castled, board, piece_positions, rookmoved, pieces);

        if((piece > 0 && turn == 0) || (piece < 0 && turn == 1)){
            if(movetoy < 8 && movetox < 8 && movetox >= 0 && movetoy >= 0 && y0>=0 
            && canmove(piece, y0, x0, movetoy, movetox, game)){
                movepieceto(piece, y0, x0, movetoy, movetox, game);
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

    std::vector<int> get_pinners(int piece_sign, int kingy, int kingx, Chess& game){
        std::vector<int> pinners;
        for(int piece_type = 0; piece_type < 6; piece_type++){
            for(int piece_number = 0; piece_number < game.pieces[piece_type][piece_sign!=1]; piece_number++){
                int n = piece_sign*(10*piece_type+piece_number+int(piece_type == 0));
                int y0 = game.piece_positions[abs(n)-1][int(n<0)][0];
                int x0 = game.piece_positions[abs(n)-1][int(n<0)][1];
                if(piecemove(n, y0, x0, kingy, kingx, game)){
                    pinners.push_back(n);
                }
            }
        }
        return pinners;
    }

    float evaluate_change(int y, int x, int changesign, int& pieceAt_yx, Chess& game, int piece = -100){
        if(piece == -100){
            piece = pieceAt_yx;
        }
        if(piece == 0){
            return 0.0f;
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
        return 0.0f;
    }

    float evaluate_move(int piece, int y0, int x0, int y1, int x1, Chess& game){
        return evaluate_change(y1, x1, 1, game.board[y1][x1], game, piece) + 
            evaluate_change(y0, x0, -1, game.board[y0][x0], game, piece) + (game.castled % 2 == 0)*0.1f - (game.castled % 3 == 0)*0.1f;
    }

    float fulleval(Chess& game){
        float evaluation = 0.0f;
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
        evaluation -= 0.1f*((game.castled%2 == 0) + (game.castled%3 == 0));
        return evaluation;
    }

    std::vector<std::vector<int>> reorder(int moves, std::vector<std::array<std::array<int, 8>, 8>> positions
    , Chess& game, int bot){
        //save current state
        Chess game_previous_state(game.kingmoved, game.enpassant, game.castled, game.board, game.piece_positions
        , game.rookmoved, game.pieces);
        std::vector<std::array<std::array<int, 8>, 8>> temp_positions = positions;

        //create vectors
        std::vector<float> moveScore;
        std::vector<std::vector<int>> starting_order;

        //loop through pieces
        for(int n1 = 1; n1 < 51; n1++){
            int piece = n1; 
            if(bot == 1){piece=-n1;}
            if(game.piece_positions[abs(piece)-1][int(piece<0)][0] != -1){
                int y0 = game.piece_positions[abs(piece)-1][int(piece<0)][0];
                int x0 = game.piece_positions[abs(piece)-1][int(piece<0)][1];
                for(int y1 = 0; y1 < 8; y1++){
                    for(int x1 = 0; x1 < 8; x1++){
                        if(canmove(piece, y0, x0, y1, x1, game)){
                            float evaluation_minus = evaluate_change(y1, x1, -1, game.board[y1][x1], game)+(piece < 9 && x1*8+y1 == game.enpassant)*intsign(piece)*1.0f;
                            movepieceto(piece, y0, x0, y1, x1, game);
                            positions.push_back(game.board);
                            if(repetition(moves+1, game, positions) || stalemate(50, game) || stalemate(-50, game)){
                                moveScore.push_back(-fulleval(game));
                            }else if(partialrepetition(moves+1, game, positions)){
                                if(bot == 0){
                                    moveScore.push_back(std::min(evaluate_move(piece, y0, x0, y1, x1, game) 
                                    + evaluation_minus, -fulleval(game)));
                                }else{
                                    moveScore.push_back(std::max(evaluate_move(piece, y0, x0, y1, x1, game) 
                                    + evaluation_minus, -fulleval(game)));
                                }    
                            }else{
                                moveScore.push_back(
                                    evaluate_move(piece, y0, x0, y1, x1, game) 
                                    + evaluation_minus);
                            }
                            starting_order.insert(starting_order.end(),
                                {piece, y0, x0, y1, x1});

                            //return to saved state
                            game.Copy_game(game_previous_state);
                            positions = temp_positions;
                        }
                    }
                }
            }
        }
        std::vector<std::vector<int>> return_vector;
        int index;
        for(int i = 0; i < moveScore.size(); i++){
            if(bot==0){
                int maxindex = std::distance(&moveScore[0],
                    std::max_element(&moveScore[0], &moveScore[0]+moveScore.size()));
                return_vector.push_back(starting_order[maxindex]);
                index = maxindex;
            }else{
                int minindex = std::distance(&moveScore[0],
                    std::min_element(&moveScore[0], &moveScore[0]+moveScore.size()));
                return_vector.push_back(starting_order[minindex]);
                index = minindex;
            }
            moveScore[index] = -1000000*intsign(bot==0);
        }
        return return_vector;
    }

    float last_move(int previous_piece, int previous_y0, int previous_x0, int previous_y1, int previous_x1, float best, Chess& game, int bot, int ntimes){
        int piece_sign = int(bot == 1)-int(bot == 0);
        int color = int(bot == 1);
        int kingy = game.piece_positions[49][int(previous_piece>0)][0];
        int kingx = game.piece_positions[49][int(previous_piece>0)][1];
        if(kingy == -1){
            return -piece_sign*500000/(ntimes+1.0);
        }
        std::vector<int> pinners = get_pinners(-piece_sign, kingy, kingx, game);
        if(botcheckmate(piece_sign*50, pinners, game, kingy, kingx)){
            return -piece_sign*500000/(ntimes+1.0);
        }
        //get evaluation change for previous move
        float previous_moveScore = evaluate_move(previous_piece, previous_y0, previous_x0, previous_y1, previous_x1, game);
        float best_moveScore = -piece_sign*1000000.0f;
        
        for(int i = 0; i < 6; i++){
            //Goes through the piece types in peculiar order
            int piece_type = int(1*(i == 0)+2*(i == 1)+3*(i == 2)+4*(i == 3)
            + 5*(i == 5));
            for(int piece_number = 0; piece_number < game.pieces[piece_type][piece_sign!=1]; piece_number++){
                int piece = piece_sign*(10*piece_type+piece_number+int(piece_type == 0));
                if(game.piece_positions[abs(piece)-1][int(piece<0)][0] != -1){
                    int y0 = game.piece_positions[abs(piece)-1][int(piece<0)][0];
                    int x0 = game.piece_positions[abs(piece)-1][int(piece<0)][1];
                    bool pinnable = ispinnable(piece, y0, x0, kingy, kingx, pinners, game);
                    for(int i = 0; i < game.canMoveTo[color][piece_type][y0][x0].size(); i++){
                        for(int j = 0; j+1 < game.canMoveTo[color][piece_type][y0][x0][i].size(); j+=2){
                            int y1 = game.canMoveTo[color][piece_type][y0][x0][i][j];
                            int x1 = game.canMoveTo[color][piece_type][y0][x0][i][j+1];
                            if(botcanmove(piece, y0, x0, y1, x1, pinnable, pinners, game, kingy, kingx)){
                                //change to evaluation
                                float total_moveScore = evaluate_move(piece, y0, x0, y1, x1, game)
                                    + evaluate_change(y1, x1, -1, game.board[y1][x1], game)+(piece < 9 && x1*8+y1 == game.enpassant)*intsign(piece)*1.0f
                                    + previous_moveScore;
                                if((total_moveScore <= best && bot == 0)
                                    || (total_moveScore >= best && bot == 1)){
                                    return total_moveScore;
                                }
                                if((total_moveScore < best_moveScore && bot == 0)
                                    || (total_moveScore > best_moveScore && bot == 1)){
                                    best_moveScore = total_moveScore;
                                }
                                //if the piece is a bishop, a rook or a queen, break if a piece is in the way
                                if(abs(piece) > 19 && abs(piece) < 50 && game.board[y1][x1] != 0){
                                    break;
                                }
                            //if the piece is a bishop, a rook or a queen, break if a piece is in the way
                            }else if(abs(piece) > 19 && abs(piece) < 50 && game.board[y1][x1] != 0){
                                break;
                            }
                        }
                    }
                }
            }
        }
        if(best_moveScore == -piece_sign*1000000){ //no legal moves
            return 0.0;
        }else{
            return best_moveScore; 
        }
    }

    float nth_move(int previous_piece, int previous_y0, int previous_x0, int previous_y1, int previous_x1, float best
    , int nmoremoves, Chess& game, int bot, int ntimes){
        int piece_sign = (intsign(bot==0))*((nmoremoves%2 == 1)-(nmoremoves%2 == 0));
        //white == 0, black == 1
        int color = int(piece_sign!=1);
        int kingy = game.piece_positions[49][int(previous_piece>0)][0];
        int kingx = game.piece_positions[49][int(previous_piece>0)][1];
        if(kingy == -1){
            return -piece_sign*500000/(ntimes+1.0);
        }
        if(checkmate(piece_sign*50, game, kingy, kingx)){
            return -piece_sign*500000/(ntimes-nmoremoves+1.0);
        }
        //save current state
        Chess game_previous_state(game.kingmoved, game.enpassant, game.castled, game.board, game.piece_positions
        , game.rookmoved, game.pieces);

        float best_moveScore = -piece_sign*1000000.0f;
        //get evaluation change for previous move
        float previous_moveScore = evaluate_move(previous_piece, previous_y0, previous_x0, previous_y1, previous_x1, game);

        for(int i = 0; i < 6; i++){
            int piece_type = int(1*(i == 0)+2*(i == 1)+3*(i == 2)+4*(i == 3)
            + 5*(i == 5));
            for(int piece_number = 0; piece_number < game.pieces[piece_type][(piece_sign!=1)]; piece_number++){
                int piece = piece_sign*(10*piece_type+piece_number+int(piece_type == 0));
                if(game.piece_positions[abs(piece)-1][int(piece<0)][0] != -1){
                    int y0 = game.piece_positions[abs(piece)-1][int(piece<0)][0];
                    int x0 = game.piece_positions[abs(piece)-1][int(piece<0)][1];
                    for(int y1 = 0; y1 < 8; y1++){
                        for(int x1 = 0; x1 < 8; x1++){
                            if(canmove(piece, y0, x0, y1, x1, game, kingy, kingx)){
                                float evaluation_minus = evaluate_change(y1, x1, -1, game.board[y1][x1], game)+(piece < 9 && x1*8+y1 == game.enpassant)*intsign(piece)*1.0;
                                float current_moveScore;
                                movepieceto(piece, y0, x0, y1, x1, game);
                                if(nmoremoves == 1){
                                    current_moveScore = last_move(piece, y0, x0, y1, x1, best_moveScore-evaluation_minus
                                    , game, bot, ntimes) + evaluation_minus;
                                }else{
                                    current_moveScore = nth_move(piece, y0, x0, y1, x1, best_moveScore-evaluation_minus
                                    , nmoremoves-1, game, bot, ntimes) + evaluation_minus;
                                }
                                float total_moveScore = current_moveScore + previous_moveScore;
                                //return to saved state
                                game.Copy_game(game_previous_state);
                                //prune if worse than previously found branch
                                if((total_moveScore <= best && (piece_sign!=1))
                                    || (total_moveScore >= best && (piece_sign==1))){
                                    return total_moveScore;
                                }
                                //check for new best moveScore
                                if((total_moveScore < best_moveScore && (piece_sign!=1))
                                    || (total_moveScore > best_moveScore && (piece_sign==1))){
                                    best_moveScore = total_moveScore;
                                }
                            }
                        }
                    }
                }
            }
        }
        if(best_moveScore == -piece_sign*1000000){ //no legal moves
            return 0.0;
        }else{
            return best_moveScore; 
        }
    }

    void firstmove(int moves, std::vector<std::array<std::array<int, 8>, 8>> positions
    , std::vector<std::vector<int>>& bestmove, Chess& game, int bot, int ntimes, int plusamount, bool all = true){
        //save current state
        Chess game_previous_state(game.kingmoved, game.enpassant, game.castled, game.board, game.piece_positions
        , game.rookmoved, game.pieces);
        std::vector<std::array<std::array<int, 8>, 8>> temp_positions = positions;

        std::vector<float> moveScore;
        float best_moveScore = -intsign(bot == 0)*1000000.0f;
        std::vector<std::vector<int>> order;
        int opponent_piece_sign = int(bot == 1)-int(bot == 0);
        if(all){
            order = reorder(moves, positions, game, bot);
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
            float evaluation_minus = evaluate_change(y1, x1, -1, game.board[y1][x1], game)+(piece < 9 && x1*8+y1 == game.enpassant)*intsign(piece)*1.0f;
            movepieceto(piece, y0, x0, y1, x1, game);
            positions.push_back(game.board);
            if(repetition(moves+1, game, positions) || stalemate(opponent_piece_sign*50, game)){
                std::cout << "repetition" << '\n';
                moveScore.push_back(-fulleval(game));
            }
            else if(partialrepetition(moves+1, game, positions)){
                std::cout << "partialrepetition" << '\n';
                if(bot == 0){
                    moveScore.push_back(std::min(nth_move(piece, y0, x0, y1, x1, best_moveScore-evaluation_minus
                    , ntimes, game, bot, ntimes) + evaluation_minus, -fulleval(game)));
                }else{
                    moveScore.push_back(std::max(nth_move(piece, y0, x0, y1, x1, best_moveScore-evaluation_minus
                    , ntimes, game, bot, ntimes) + evaluation_minus, -fulleval(game)));
                } 
            }
            else{
                float current_moveScore = nth_move(piece, y0, x0, y1, x1, best_moveScore-evaluation_minus
                    , ntimes, game, bot, ntimes) + evaluation_minus;
                moveScore.push_back(current_moveScore);
                if((current_moveScore > best_moveScore && bot == 0) || (current_moveScore < best_moveScore && bot == 1)){
                    best_moveScore = current_moveScore;
                }
            }

            //return to saved state
            game.Copy_game(game_previous_state);
            positions = temp_positions;
        }
        int bestindex;
        bestmove.resize(0);
        if(bot == 0){
            for(int i = 0; i < plusamount; i++){
                bestindex = std::distance(std::begin(moveScore),
                std::max_element(std::begin(moveScore), std::end(moveScore)));
                
                bestmove.push_back({});
                for(int j = 0; j < 5; j++){
                    bestmove[i].push_back(order[bestindex][j]);
                }
                bestmove[i].push_back(moveScore[bestindex]);
                moveScore[bestindex] = -1000000;
            }
        }else{
            for(int i = 0; i < plusamount; i++){
                bestindex = std::distance(std::begin(moveScore),
                std::min_element(std::begin(moveScore), std::end(moveScore)));
                bestmove.push_back({});
                for(int j = 0; j < 5; j++){
                    bestmove[i].push_back(order[bestindex][j]);
                }
                bestmove[i].push_back(moveScore[bestindex]);
                moveScore[bestindex] = 1000000;
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
                std::cout << convert_to_pgn(bookmove[0], bookmove[1], bookmove[2], bookmove[3], bookmove[4]) << '\n';
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
        std::array<std::array<int, 2>,6> pieces = pieces_to_array(string_to_vector_2d(pieces_str));
        std::array<std::array<int, 2>,2> rookmoved = rookmoved_to_array(string_to_vector_2d(rookmoved_str));

        //Create Chess object
        Chess game(kingmoved, enpassant, castled, board, piece_positions, rookmoved, pieces);
        
        int ntimesmin = 4;
        //ntimes == (amount of half moves that basicbot searches forward) - 2
        int ntimes = ntimesmin;
        //amount of moves calculated one full move deeper
        int plusamount = 2;
        
        std::vector<std::vector<int>> bestmove;

        if(read_openingbook(bot, openingbook_data, size, game)[0]){
            std::vector<int> book_result = read_openingbook(bot, openingbook_data, size, game);
            std::cout << "book" << '\n';
            return vector_to_string({book_result[1], book_result[2], book_result[3], book_result[4], book_result[5]});
        }
        float score = fulleval(game);
        auto start = std::chrono::high_resolution_clock::now();
        firstmove(moves, positions, bestmove, game, bot, ntimes, plusamount);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast
            <std::chrono::milliseconds>(stop - start);
        while(duration.count()/1000.0 < 0.2){
            if(abs(bestmove[0][5]) > 10000 || bestmove.size() <= 1){
                break;
            }
            ntimes += 2;
            firstmove(moves, positions, bestmove, game, bot, ntimes, plusamount);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast
                <std::chrono::milliseconds>(stop - start);
        }
        std::cout << "depth = " << ntimes/2+1;
        if(duration.count()/1000.0 < 0.4 && abs(bestmove[0][5]) <= 10000 && bestmove.size() > 1){
            ntimes += 2;
            firstmove(moves, positions, bestmove, game, bot, ntimes, plusamount, false);
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
        movepieceto(piece, y0, x0, y1, x1, game);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast
            <std::chrono::milliseconds>(stop - start);
        std::cout << duration.count()/1000.0 << '\n';
        std::cout <<  convert_to_pgn(piece, y0, x0, y1, x1) << ", " << score << '\n';
        return vector_to_string({piece, y0, x0, y1, x1});
    }
}