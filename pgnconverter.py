import Chessbot1
import math
import time
from copy import copy, deepcopy
import json

with open("carlsen, magnus.pgn", 'r') as f:
    games = f.readlines()

print(games[0])

converted = []

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
        if Chessbot1.turn == 1:
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
        x = (Chessbot1.turn == 0)*7
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

    if Chessbot1.turn == 1:
        pn *= -1

    for n1 in range(Chessbot1.pieces[int(abs(pn)/10)][Chessbot1.turn]):
        n = pn + int(math.copysign(n1, (Chessbot1.turn == 0)-0.5))
        #found is True if piece has been found
        found = False
        for i in range(8):
            if n in Chessbot1.board[i]:
                square = [i, Chessbot1.board[i].index(n)]
                found = True
                break
        if found and (startx == square[0] or startx == -1) and (starty == square[1] or starty == -1):
            if Chessbot1.piecemove(n, square[0], square[1], x, y) and not Chessbot1.pin(n, square[0], square[1], x, y):
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

#g i the game we're converting
for g in range(len(games)):

    #reset the backend
    Chessbot1.board = [[30,10,20,50,40,21,11,31], [1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[-30,-10,-20,-50,-40,-21,-11,-31]]
    Chessbot1.moves = 0
    Chessbot1.positions = [[[]]]
    Chessbot1.positions[0] = deepcopy(Chessbot1.board)
    Chessbot1.turn = 0
    Chessbot1.enpassant = -1
    Chessbot1.pieces = [[8,8],[2,2],[2,2],[2,2],[1,1],[1,1]]
    Chessbot1.kingmoved = [0, 0]
    Chessbot1.rookmoved = [[0, 0], [0, 0]]

    #n1 is where we find 1.
    n1 = 0
    #s1 tells if the previous symbol was 1, s2 if it was .
    s1 = False
    s2 = False
    for i in range(len(games[g])):
        if games[g][i] == "1":
            s1 = True
        elif s1 == True and games[g][i] == ".":
            s2 = True
            s1 = False
        elif s2 == True and games[g][i] == " ":
            n1 = i
            break
        elif games[g][i] == "\n":
            pass
        else:
            s1 = False
            s2 = False
    else:
        n1 = len(games[g])
    p = ""
    v = ""
    h = ""
    startv = ""
    starth = ""
    for i in range(n1+1, len(games[g])):
        if games[g][i] == ' ' or games[g][i] == '\n':
            if v != "" and h != "":
                translated = translator(v,h,startv,starth,p)
                if Chessbot1.piecemove(*translated) and not Chessbot1.pin(*translated):
                    if Chessbot1.turn == 0:
                        converted += [[translated[0], Chessbot1.board, translated[3], translated[4]]]
                    Chessbot1.movepieceto(*translated)
                    Chessbot1.turn = (Chessbot1.turn == 0)
                else:
                    print(games[g], games[g+1], games[g-2])
                    print(v, h, p, startv, starth)
                    Chessbot1.printboard()
                    with open("pgndata.txt", 'w') as f:
                        f.write(str(converted))
                    raise Exception("lol")
            p = ""
            v = ""
            h = ""
            startv = ""
            starth = ""
        elif games[g][i] == ".":
            p = ""
            v = ""
            h = ""
            startv = ""
            starth = ""
        elif games[g][i] == "x" or games[g][i] == '+':
            pass
        elif games[g][i] == '=':
            h = '='
        elif games[g][i] == "O":
            if p == "":
                if games[g][i+3] == '-':
                    #long castle
                    p = 'O'
                    v = '-'
                    h = '-'
                else:
                    #short castle
                    p = 'O'
                    v = '-'
                    h = 'O'
        elif games[g][i].isnumeric():
            if h != "":
                starth = h
            h = games[g][i]
        elif games[g][i].isupper():
            p = games[g][i]
        elif games[g][i].isalpha():
            if v != "":
                startv = v
            v = games[g][i]

with open("pgndata.txt", 'w') as f:
    f.write(str(converted))