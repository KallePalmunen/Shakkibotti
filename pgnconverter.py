import Chessbot1
import math
import time
from copy import copy, deepcopy

games = ['''[Event "Live Chess"]
[Site "Chess.com"]
[Date "2022.11.01"]
[Round "?"]
[White "elovena"]
[Black "leeviloikkanen"]
[Result "0-1"]
[ECO "A00"]
[WhiteElo "819"]
[BlackElo "1367"]
[TimeControl "180"]
[EndTime "12:16:24 PDT"]
[Termination "leeviloikkanen won by resignation"]

1. d3 Nf6 2. c4 g6 3. Nc3 d6 4. Bg5 Bg7 5. Nf3 c5 6. e4 Nc6 7. Be2 O-O 8. O-O h6
9. Bxf6 Bxf6 10. Nd5 Bg7 11. b3 Bxa1 12. Qxa1 e6 13. Nc3 Ne7 14. d4 cxd4 15.
Nxd4 d5 16. f3 b6 17. exd5 exd5 18. f4 dxc4 19. Ndb5 cxb3 20. axb3 Bf5 21. Rd1
Qc8 22. Nd6 Qc5+ 23. Kf1 Rad8 24. Nxf5 Nxf5 25. Re1 Rfe8 26. Na4 Qc2 27. Qb2
Qxb2 28. Nxb2 Rd2 29. Nc4 Rd4 30. Ne5 Rxf4+ 31. Kg1 Rxe5 0-1''',
'''[Event "Live Chess"]
[Site "Chess.com"]
[Date "2022.10.24"]
[Round "?"]
[White "elovena"]
[Black "leeviloikkanen"]
[Result "0-1"]
[ECO "E61"]
[WhiteElo "751"]
[BlackElo "1337"]
[TimeControl "180+2"]
[EndTime "11:09:36 PDT"]
[Termination "leeviloikkanen won by resignation"]

1. d4 Nf6 2. c4 g6 3. Nc3 d6 4. e4 Bg7 5. d5 O-O 6. f3 Re8 7. Bd3 e5 8. Nge2 c6
9. O-O Qc7 10. f4 Bg4 11. h3 Bxe2 12. Qxe2 Nbd7 13. fxe5 Rxe5 14. Bf4 Re7 15. b3
Rae8 16. Bg5 h6 17. Bxf6 Nxf6 18. Rae1 Nh5 19. g4 Ng3 20. Qf2 Nxf1 21. Rxf1 Bxc3
22. h4 c5 23. h5 Bd4 24. Qxd4 cxd4 0-1''']

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
    else:
        x = int(h)-1
        y = letter_to_number(v)
        pn = 0
    
    if starth.isnumeric():
        startx = int(starth)-1
    else:
        startx = -1
    starty = letter_to_number(startv)

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
    
    return [pn, square[0], square[1], x, y]

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
    #s1 tells if the previous symbol was ], s2 if it was 1
    s1 = False
    s2 = False
    for i in range(len(games[g])):
        if games[g][i] == "]":
            s1 = True
        elif s1 == True and games[g][i] == "1":
            s2 = True
            s1 = False
        elif s2 == True and games[g][i] == ".":
            n1 = i
            break
        elif games[g][i] == " " or games[g][i] == "\n":
            pass
        else:
            s1 = False
            s2 = False
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
                    print(v, h, p, startv, starth)
                    Chessbot1.printboard()
                    break
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