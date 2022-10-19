import Chessbot1
import random
import math
from copy import copy, deepcopy
import time
import json


movescore = [[]]
for i in range(3):
    movescore.append([])

def movepieceto(n, x0, y0, x1, y1):
    if(abs(n) == 50):
        if abs(y1 - y0) > 1:
            whichrook = int(math.copysign(30 + (y1 > 4), n))
            Chessbot1.board[x1][Chessbot1.board[x1].index(whichrook)] = 0
            Chessbot1.board[x1][y1 + int(math.copysign(1, 4-y1))] = whichrook
        Chessbot1.kingmoved[(n > 0)] = 1
    Chessbot1.board[x0][y0] = 0
    if(abs(n) == 30 or abs(n) == 31):
        Chessbot1.rookmoved[(n >0)][abs(n)-30] = 1
    if Chessbot1.promote(n, x1):
        promoteto = 4
        Chessbot1.board[x1][y1] = int(math.copysign(1, n))*(promoteto*10+Chessbot1.pieces[promoteto][(n < 0)])
        Chessbot1.pieces[promoteto][(n < 0)] += 1
    else:
        Chessbot1.board[x1][y1] = n
    if Chessbot1.enpassant >= 0 and y1*8+x1 == Chessbot1.enpassant:
        Chessbot1.board[x1-int(math.copysign(1, x1 - x0))][y1] = 0
    if abs(n) < 10 and abs(x1 - x0) > 1:
        Chessbot1.enpassant = y1*8+x0+int(math.copysign(1, x1 - x0))
    else:
        Chessbot1.enpassant = -1
    Chessbot1.moves += 1
    Chessbot1.positions += [deepcopy(Chessbot1.board)]
def evaluate():
    if Chessbot1.checkmate(50):
        return -500000
    if Chessbot1.checkmate(-50):
        return 500000
    evaluation = (Chessbot1.board[0][1] == 50 or Chessbot1.board[0][5] == 50 or Chessbot1.board[7][1] == -50 or Chessbot1.board[7][5] == -50)*0.05
    for x in range(8):
        for y in range(8):
            n = Chessbot1.board[x][y]
            n1 = Chessbot1.board[min(7, x + 1)][min(7, y+1)]
            n2 = Chessbot1.board[min(7, x + 1)][max(0, y-1)]
            evaluation += math.copysign((n != 0 and abs(n) < 9)*(1+(abs(x - 7*(n < 0))*((y > 2 and y < 6)*((y == 5)*0.01+(y != 5)*0.05) + 0.001)\
                - (Chessbot1.moves < 60 and y < 2)*(abs(x - 7*(n < 0)) > 2)*0.03))\
                + (3-((n > 0) and ((n1 < 0 and n1 >-9) or (n2 < 0 and n2 >-9)))*1.5)\
                *(abs(n) > 9 and abs(n) < 30) + \
                (5-((n > 0) and ((n1 < 0 and n1 >-9) or (n2 < 0 and n2 >-9)))*2.5)\
                *(abs(n) > 29 and abs(n) < 40) + \
                (9-((n > 0) and ((n1 < 0 and n1 >-9) or (n2 < 0 and n2 >-9)))*4.5)\
                *(abs(n) > 39), n)\
                + ((abs(n) >= 10 and abs(n) < 30) or (abs(n) >= 40 and abs(n) < 50))*(x != 7*(n < 0))*(0.05+0.001*(abs(x - 7*(n < 0)) > 1))\
                + (abs(n) >= 10 and abs(n) < 20 and y > 1 and y < 6)*0.01
    return evaluation


def blackmove():
    moves = Chessbot1.moves
    enpassant = Chessbot1.enpassant
    board = str(Chessbot1.board)
    kingmoved = str(Chessbot1.kingmoved)
    rookmoved = str(Chessbot1.rookmoved)
    pieces = str(Chessbot1.pieces)
    positions = str(Chessbot1.positions)
    movescore1 = [1000000]
    for n in range(1, 9):
        for i in range(1, 8):
            if -n in Chessbot1.board[i]:
                y0 = Chessbot1.board[i].index(-n)
                for x1 in range(i - 2, i):
                    for y1 in range(y0 - 1, y0+2):
                        if (Chessbot1.piecemove(-n, i, y0, x1, y1) 
                            and not Chessbot1.pin(-n, i, y0, x1, y1)):
                            movepieceto(-n, i, y0, x1, y1)
                            movescore1 += wmove(),
                            Chessbot1.turn = Chessbot1.bot
                                
                            Chessbot1.moves = moves
                            Chessbot1.enpassant = enpassant
                            Chessbot1.board = json.loads(board)
                            Chessbot1.kingmoved = json.loads(kingmoved)
                            Chessbot1.rookmoved = json.loads(rookmoved)
                            Chessbot1.pieces = json.loads(pieces)
                            Chessbot1.positions = json.loads(positions)
                break
    for n2 in range(Chessbot1.pieces[1][1]):
        n = 10+n2
        for i in range(8):
            if -n in Chessbot1.board[i]:
                y0 = Chessbot1.board[i].index(-n)
                for x1 in range(i-2, i+3):
                    for y1 in range(y0-2, y0+3):
                        if (Chessbot1.piecemove(-n, i, y0, x1, y1) 
                            and not Chessbot1.pin(-n, i, y0, x1, y1)):
                            movepieceto(-n, i, y0, x1, y1)
                            movescore1 += wmove(),
                            Chessbot1.turn = Chessbot1.bot
                                
                            Chessbot1.moves = moves
                            Chessbot1.enpassant = enpassant
                            Chessbot1.board = json.loads(board)
                            Chessbot1.kingmoved = json.loads(kingmoved)
                            Chessbot1.rookmoved = json.loads(rookmoved)
                            Chessbot1.pieces = json.loads(pieces)
                            Chessbot1.positions = json.loads(positions)
    for n1 in range(2, 6):
        for n2 in range(Chessbot1.pieces[n1][1]):
            n = n1*10+n2
            for i in range(8):
                if -n in Chessbot1.board[i]:
                    for x1 in range(8):
                        for y1 in range(8):
                            if (Chessbot1.piecemove(-n, i, Chessbot1.board[i].index(-n), x1, y1) 
                                and not Chessbot1.pin(-n, i, Chessbot1.board[i].index(-n), x1, y1)):
                                movepieceto(-n, i, Chessbot1.board[i].index(-n), x1, y1)
                                movescore1 += wmove(),
                                Chessbot1.turn = Chessbot1.bot
                                
                                Chessbot1.moves = moves
                                Chessbot1.enpassant = enpassant
                                Chessbot1.board = json.loads(board)
                                Chessbot1.kingmoved = json.loads(kingmoved)
                                Chessbot1.rookmoved = json.loads(rookmoved)
                                Chessbot1.pieces = json.loads(pieces)
                                Chessbot1.positions = json.loads(positions)
                    break
                                
    Chessbot1.moves = moves
    Chessbot1.enpassant = enpassant
    Chessbot1.board = json.loads(board)
    Chessbot1.kingmoved = json.loads(kingmoved)
    Chessbot1.rookmoved = json.loads(rookmoved)
    Chessbot1.pieces = json.loads(pieces)
    Chessbot1.positions = json.loads(positions)
    Chessbot1.turn = (Chessbot1.bot == 0)
    return min(movescore1)

def bmove():
    moves = Chessbot1.moves
    enpassant = Chessbot1.enpassant
    board = str(Chessbot1.board)
    kingmoved = str(Chessbot1.kingmoved)
    rookmoved = str(Chessbot1.rookmoved)
    pieces = str(Chessbot1.pieces)
    positions = str(Chessbot1.positions)
    movescore3 = [1000000]
    for n in range(8):
        for i in range(8):
            if -n in Chessbot1.board[7-i]:
                x0 = 7-i
                y0 = Chessbot1.board[x0].index(-n)
                for x1 in range(x0 - 2, x0):
                    for y1 in range(y0 - 1, y0+2):
                        if (Chessbot1.piecemove(-n, x0, y0, x1, y1) 
                            and not Chessbot1.pin(-n, x0, y0, x1, y1)):
                            movepieceto(-n, x0, y0, x1, y1)
                            movescore3 += evaluate(),
                            Chessbot1.turn = Chessbot1.bot
                                
                            Chessbot1.moves = moves
                            Chessbot1.enpassant = enpassant
                            Chessbot1.board = json.loads(board)
                            Chessbot1.kingmoved = json.loads(kingmoved)
                            Chessbot1.rookmoved = json.loads(rookmoved)
                            Chessbot1.pieces = json.loads(pieces)
                            Chessbot1.positions = json.loads(positions)
                break
    for n2 in range(Chessbot1.pieces[1][1]):
        n = 10+n2
        for i in range(8):
            if -n in Chessbot1.board[7-i]:
                x0 = 7-i
                y0 = Chessbot1.board[x0].index(-n)
                for x1 in range(x0-2, x0+3):
                    for y1 in range(y0-2, y0+3):
                        if (Chessbot1.piecemove(-n, x0, y0, x1, y1) 
                            and not Chessbot1.pin(-n, x0, y0, x1, y1)):
                            movepieceto(-n, x0, y0, x1, y1)
                            movescore3 += evaluate(),
                            Chessbot1.turn = Chessbot1.bot
                                
                            Chessbot1.moves = moves
                            Chessbot1.enpassant = enpassant
                            Chessbot1.board = json.loads(board)
                            Chessbot1.kingmoved = json.loads(kingmoved)
                            Chessbot1.rookmoved = json.loads(rookmoved)
                            Chessbot1.pieces = json.loads(pieces)
                            Chessbot1.positions = json.loads(positions)
    for n1 in range(2, 6):
        for n2 in range(Chessbot1.pieces[n1][1]):
            n = n1*10+n2
            for i in range(8):
                if -n in Chessbot1.board[7-i]:
                    x0 = 7-i
                    y0 = Chessbot1.board[x0].index(-n)
                    for x1 in range(8):
                        for y1 in range(8):
                            if (Chessbot1.piecemove(-n, x0, y0, x1, y1) 
                                and not Chessbot1.pin(-n, x0, y0, x1, y1)):
                                movepieceto(-n, x0, y0, x1, y1)
                                movescore3 += evaluate(),
                                Chessbot1.turn = Chessbot1.bot
                                
                                Chessbot1.moves = moves
                                Chessbot1.enpassant = enpassant
                                Chessbot1.board = json.loads(board)
                                Chessbot1.kingmoved = json.loads(kingmoved)
                                Chessbot1.rookmoved = json.loads(rookmoved)
                                Chessbot1.pieces = json.loads(pieces)
                                Chessbot1.positions = json.loads(positions)
                    break
                                
    Chessbot1.moves = moves
    Chessbot1.enpassant = enpassant
    Chessbot1.board = json.loads(board)
    Chessbot1.kingmoved = json.loads(kingmoved)
    Chessbot1.rookmoved = json.loads(rookmoved)
    Chessbot1.pieces = json.loads(pieces)
    Chessbot1.positions = json.loads(positions)
    Chessbot1.turn = (Chessbot1.bot == 0)
    return min(movescore3)

def wmove():
    moves = Chessbot1.moves
    enpassant = Chessbot1.enpassant
    board = str(Chessbot1.board)
    kingmoved = str(Chessbot1.kingmoved)
    rookmoved = str(Chessbot1.rookmoved)
    pieces = str(Chessbot1.pieces)
    positions = str(Chessbot1.positions)
    movescore2 = [-1000000]
    for n in range(1, 9):
        for i in range(8):
            if n in Chessbot1.board[i]:
                for x1 in range(i, 8):
                    if x1 - i <= 2:
                        for y1 in range(8):
                            if (Chessbot1.piecemove(n, i, Chessbot1.board[i].index(n), x1, y1) 
                                and not Chessbot1.pin(n, i, Chessbot1.board[i].index(n), x1, y1)):
                                movepieceto(n, i, Chessbot1.board[i].index(n), x1, y1)
                                movescore2 += bmove(),
                                Chessbot1.turn = (Chessbot1.bot == 0)
                                
                                Chessbot1.moves = moves
                                Chessbot1.enpassant = enpassant
                                Chessbot1.board = json.loads(board)
                                Chessbot1.kingmoved = json.loads(kingmoved)
                                Chessbot1.rookmoved = json.loads(rookmoved)
                                Chessbot1.pieces = json.loads(pieces)
                                Chessbot1.positions = json.loads(positions)
                break
    for n2 in range(Chessbot1.pieces[1][0]):
        n = 10+n2
        for i in range(8):
            if n in Chessbot1.board[i]:
                y0 = Chessbot1.board[i].index(n)
                for x1 in range(x1-2, x1+3):
                    for y1 in range(y0-2, y0+3):
                        if (Chessbot1.piecemove(n, i, y0, x1, y1) 
                            and not Chessbot1.pin(n, i, y0, x1, y1)):
                            movepieceto(n, i, y0, x1, y1)
                            movescore2 += bmove(),
                            Chessbot1.turn = (Chessbot1.bot == 0)
                                
                            Chessbot1.moves = moves
                            Chessbot1.enpassant = enpassant
                            Chessbot1.board = json.loads(board)
                            Chessbot1.kingmoved = json.loads(kingmoved)
                            Chessbot1.rookmoved = json.loads(rookmoved)
                            Chessbot1.pieces = json.loads(pieces)
                            Chessbot1.positions = json.loads(positions)
                break
    for n1 in range(2, 6):
        for n2 in range(Chessbot1.pieces[n1][0]):
            n = n1*10+n2
            for i in range(8):
                if n in Chessbot1.board[i]:
                    for x1 in range(8):
                        for y1 in range(8):
                            if (Chessbot1.piecemove(n, i, Chessbot1.board[i].index(n), x1, y1) 
                                and not Chessbot1.pin(n, i, Chessbot1.board[i].index(n), x1, y1)):
                                movepieceto(n, i, Chessbot1.board[i].index(n), x1, y1)
                                movescore2 += bmove(),
                                Chessbot1.turn = (Chessbot1.bot == 0)
                                
                                Chessbot1.moves = moves
                                Chessbot1.enpassant = enpassant
                                Chessbot1.board = json.loads(board)
                                Chessbot1.kingmoved = json.loads(kingmoved)
                                Chessbot1.rookmoved = json.loads(rookmoved)
                                Chessbot1.pieces = json.loads(pieces)
                                Chessbot1.positions = json.loads(positions)
                    break
                                
    Chessbot1.moves = moves
    Chessbot1.enpassant = enpassant
    Chessbot1.board = json.loads(board)
    Chessbot1.kingmoved = json.loads(kingmoved)
    Chessbot1.rookmoved = json.loads(rookmoved)
    Chessbot1.pieces = json.loads(pieces)
    Chessbot1.positions = json.loads(positions)
    Chessbot1.turn = Chessbot1.bot
    return max(movescore2)

def whitemove():
    moves = Chessbot1.moves
    enpassant = Chessbot1.enpassant
    board = str(Chessbot1.board)
    kingmoved = str(Chessbot1.kingmoved)
    rookmoved = str(Chessbot1.rookmoved)
    pieces = str(Chessbot1.pieces)
    positions = str(Chessbot1.positions)
    movescore[0].clear()
    for n in range(1, 51):
        for i in range(8):
            if n in Chessbot1.board[i]:
                for x1 in range(8):
                    for y1 in range(8):
                        if (Chessbot1.piecemove(n, i, Chessbot1.board[i].index(n), x1, y1) 
                            and not Chessbot1.pin(n, i, Chessbot1.board[i].index(n), x1, y1)):
                            movepieceto(n, i, Chessbot1.board[i].index(n), x1, y1)
                            Chessbot1.turn = (Chessbot1.bot == 0)
                            movescore[0] += [blackmove()]
                                
                            Chessbot1.moves = moves
                            Chessbot1.enpassant = enpassant
                            Chessbot1.board = json.loads(board)
                            Chessbot1.kingmoved = json.loads(kingmoved)
                            Chessbot1.rookmoved = json.loads(rookmoved)
                            Chessbot1.pieces = json.loads(pieces)
                            Chessbot1.positions = json.loads(positions)
                        else:
                            movescore[0] += [-1000000]
                break
        else:
            for xy in range(64):
                movescore[0] += [-1000000]
    Chessbot1.turn = Chessbot1.bot

def basicbot():
    start = time.time()
    whitemove()
    bestmove = movescore[0].index(max(movescore[0]))
    move = int(bestmove/64)+1
    movetox = int((bestmove-math.floor(bestmove/64)*64)/8)
    movetoy = int(bestmove-movetox*8-(move-1)*64)
    for i in range(8):
        if move in Chessbot1.board[i]:
            Chessbot1.movepieceto(move, i, Chessbot1.board[i].index(move), movetox, movetoy)
            Chessbot1.turn = (Chessbot1.bot == 0)
    end = time.time()
    print(end-start)
    print(max(movescore[0]))
