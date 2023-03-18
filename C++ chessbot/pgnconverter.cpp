
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