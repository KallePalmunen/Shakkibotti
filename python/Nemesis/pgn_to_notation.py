import sys, pathlib
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent.parent))

import rules_old
import math
import time
from copy import copy, deepcopy
import json
import Magnusfanboy.magnusfanboy as fan

#Should be approximately ready with small errors/problems that need to be fixed

def letter_to_number(s):
    if s == "a":
        return 7
    if s == "b":
        return 6
    if s == "c":
        return 5
    if s == "d":
        return 4
    if s == "e":
        return 3
    if s == "f":
        return 2
    if s == "g":
        return 1
    if s == "h":
        return 0
    else:
        return -1

def piece_to_number(v, h, startv, starth, p="p"):
    #p=piece, v means the y coordinate and h means the x coordinate, like in d6, the d and the 6 
    #(its actually the other way around but we have it this way here :D)
    if p == 'O':
        pn = 50
        if rules_old.turn == 1:
            x = 7
        else:
            x = 0
        if h == '-':
            #long castle
            y = 5
        else:
            #short castle
            y = 1
        promoteto = ""
    elif h == '=':
        x = (rules_old.turn == 0)*7
        y = letter_to_number(v)
        pn = 1
        if p == 'Q':
            promoteto = 4
        if p == 'R':
            promoteto = 3
        if p == "B":
            promoteto = 2
        if p == 'N':
            promoteto = 1
    else:
        x = int(h)-1
        y = letter_to_number(v)
        pn = 0
        promoteto = ""
    
    if starth.isnumeric():
        startx = int(starth)-1
    else:
        startx = -1
    starty = letter_to_number(startv)

    if h != '=':
        if p == "p" or p == "":
            pn = 1
        if p == "N":
            pn = 10
        if p == "B":
            pn = 20
        if p == "R":
            pn = 30
        if p == "Q":
            pn = 40
        if p == "K":
            pn = 50

    if rules_old.turn == 1:
        pn *= -1

    for n1 in range(rules_old.pieces[int(abs(pn)/10)][rules_old.turn]):
        n = pn + int(math.copysign(n1, (rules_old.turn == 0)-0.5))
        #found is True if piece has been found
        found = False
        for i in range(8):
            if n in rules_old.board[i]:
                square = [i, rules_old.board[i].index(n)]
                found = True
                break
        if found and (startx == square[0] or startx == -1) and (starty == square[1] or starty == -1):
            if rules_old.piecemove(n, square[0], square[1], x, y) and not rules_old.pin(n, square[0], square[1], x, y):
                pn = n
                break
    else:
        #if no legal move
        print(n, square[0], square[1], x, y)
        return [0, 0, 0, 0, 0]
    
    return [pn, square[0], square[1], x, y, promoteto]

def translator(v, h, startv, starth, p="p"):
    list = []
    list += piece_to_number(v, h, startv, starth, p)
    return list

def get_converted_games(games, max_game_index=0):
    game_index = 0
    converted = [[]]

    if max_game_index == 0:
        max_game_index = len(games)

    for row in range(min(len(games), max_game_index)):

        #reset the backend
        rules_old.board = [[30,10,20,50,40,21,11,31], [1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
            [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[-30,-10,-20,-50,-40,-21,-11,-31]]
        rules_old.moves = 0
        rules_old.positions = [[[]]]
        rules_old.positions[0] = deepcopy(rules_old.board)
        rules_old.turn = 0
        rules_old.enpassant = -1
        rules_old.pieces = [[8,8],[2,2],[2,2],[2,2],[1,1],[1,1]]
        rules_old.kingmoved = [0, 0]
        rules_old.rookmoved = [[0, 0], [0, 0]]

        #n1 is where we find 1.
        n1 = 0
        #s1 tells if the previous symbol was 1, s2 if it was .
        s1 = False
        s2 = False
        for i in range(len(games[row])):
            if games[row][i] == "1":
                s1 = True
            elif s1 == True and games[row][i] == ".":
                s2 = True
                s1 = False
            elif s2 == True and games[row][i] == " ":
                n1 = i
                break
            elif games[row][i] == "\n":
                pass
            else:
                s1 = False
                s2 = False
        else:
            n1 = len(games[row])
        p = ""
        v = ""
        h = ""
        startv = ""
        starth = ""
        for i in range(n1+1, len(games[row])):
            if games[row][i] == ' ' or games[row][i] == '\n':
                if v != "" and h != "":
                    translated = translator(v,h,startv,starth,p)
                    if rules_old.piecemove(*translated) and not rules_old.pin(*translated):
                        fan.convertposition()
                        converted[game_index] += [translated]
                        rules_old.movepieceto(*translated)
                        rules_old.turn = (rules_old.turn == 0)
                    else:
                        print(games[row], games[row+1], games[row-2])
                        print(v, h, p, startv, starth)
                        rules_old.printboard()
                        with open("./python/Nemesis/pgndata.txt", 'w') as f:
                            f.write(str(converted))
                        raise Exception("lol")

                p = ""
                v = ""
                h = ""
                startv = ""
                starth = ""
            elif games[row][i] == ".":
                p = ""
                v = ""
                h = ""
                startv = ""
                starth = ""
            elif games[row][i] == "x" or games[row][i] == '+':
                pass
            elif games[row][i] == '=':
                h = '='
            elif games[row][i] == "O":
                if p == "":
                    if games[row][i+3] == '-':
                        #long castle
                        p = 'O'
                        v = '-'
                        h = '-'
                    else:
                        #short castle
                        p = 'O'
                        v = '-'
                        h = 'O'
            elif games[row][i].isnumeric():
                if h != "":
                    starth = h
                h = games[row][i]
            elif games[row][i].isupper():
                p = games[row][i]
            elif games[row][i].isalpha():
                if v != "":
                    startv = v
                v = games[row][i]

        if n1 != len(games[row]):
            converted += [[]]
            game_index += 1
    
    return converted

def convert_games():
    with open("./python/Magnusfanboy/carlsen, magnus.pgn", 'r') as f:
        games = f.readlines()

    print(games[0])

    converted = get_converted_games(games, max_game_index=100)

    with open("./python/Nemesis/pgndata.txt", "w") as f:
        for group in converted:
            f.write(str(group) + "\n")

if __name__ == "__main__":
    convert_games()