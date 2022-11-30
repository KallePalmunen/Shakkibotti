import Chessbot1
import math
import time

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
Qxb2 28. Nxb2 Rd2 29. Nc4 Rd4 30. Ne5 Rxf4+ 31. Kg1 Rxe5 0-1''']

converted = []

def letter_to_number(s):
    if s == "a":
        return 0
    if s == "b":
        return 1
    if s == "c":
        return 2
    if s == "d":
        return 3
    if s == "e":
        return 4
    if s == "f":
        return 5
    if s == "g":
        return 6
    if s == "h":
        return 7

def piece_to_number(v, h, p="p"):
    #p=piece, v means the y coordinate and h means the x coordinate, like in d6, the d and the 6 
    #(its actually the other way around but we have it this way here :D)

    x = int(h)-1
    y = letter_to_number(v)
    pn = 0

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
        for i in range(8):
            if n in Chessbot1.board[i]:
                square = [i, Chessbot1.board[i].index(n)]
                break
        if Chessbot1.piecemove(n, square[0], square[1], x, y) and not Chessbot1.pin(n, square[0], square[1], x, y):
            pn = n
            break
    else:
        #if no legal move
        return [0, 0, 0, 0, 0]

    return [pn, square[0], square[1], x, y]

def translator(v, h, p="p"):
    list = []
    list += piece_to_number(v, h, p)
    return list

#g i the game we're converting
for g in range(1):
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
    for i in range(n1+1, len(games[g])):
        if games[g][i] == ' ' or games[g][i] == '\n':
            if v != "" and h != "":
                if Chessbot1.piecemove(*translator(v,h,p)) and not Chessbot1.pin(*translator(v,h,p)):
                    if Chessbot1.turn == 0:
                        converted += [translator(v,h,p)]
                    Chessbot1.movepieceto(*translator(v,h,p))
                    Chessbot1.turn = (Chessbot1.turn == 0)
                else:
                    print(v, h, p)
                    break
            p = ""
            v = ""
            h = ""
        elif games[g][i] == ".":
            p = ""
            v = ""
            h = ""
        elif games[g][i] == "x":
            pass
        elif games[g][i].isnumeric():
            h = games[g][i]
        elif games[g][i].isupper():
            p = games[g][i]
        elif games[g][i].isalpha():
            v = games[g][i]

print(converted)