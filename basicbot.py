import Chessbot1
import random
import math
from copy import copy, deepcopy

movescore = []

turn = Chessbot1.turn
moves = Chessbot1.moves
enpassant = Chessbot1.enpassant
board = deepcopy(Chessbot1.board)
kingmoved = deepcopy(Chessbot1.kingmoved)
rookmoved = deepcopy(Chessbot1.rookmoved)
pieces = deepcopy(Chessbot1.pieces)
positions = deepcopy(Chessbot1.positions)

def update():
    global turn, moves, enpassant, board, kingmoved, rookmoved, pieces, positions
    turn = Chessbot1.turn
    moves = Chessbot1.moves
    enpassant = Chessbot1.enpassant
    board = deepcopy(Chessbot1.board)
    kingmoved = deepcopy(Chessbot1.kingmoved)
    rookmoved = deepcopy(Chessbot1.rookmoved)
    pieces = deepcopy(Chessbot1.pieces)
    positions = deepcopy(Chessbot1.positions)
    movescore.clear()

def restore():
    global turn, moves, enpassant, board, kingmoved, rookmoved, pieces, positions
    Chessbot1.turn = turn
    Chessbot1.moves = moves
    Chessbot1.enpassant = enpassant
    Chessbot1.board = deepcopy(board)
    Chessbot1.kingmoved = deepcopy(kingmoved)
    Chessbot1.rookmoved = deepcopy(rookmoved)
    Chessbot1.pieces = deepcopy(pieces)
    Chessbot1.positions = deepcopy(positions)

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
        print('Type 1 for knight, 2 for bishope, 3 for rook, 4 for queen')
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
    Chessbot1.positions.append(deepcopy(Chessbot1.board))

def evaluate():
    evaluation = 0
    if Chessbot1.checkmate(50):
        return -500000
    elif Chessbot1.checkmate(-50):
        return 500000
    else:
        for x in range(8):
            for y in range(8):
                n = Chessbot1.board[x][y]
                evaluation += math.copysign(math.ceil(abs(n)), n)\
                    + (abs(n) >= 10 and abs(n) < 50)*(x != 7*(n < 0))*0.05\
                    + (abs(n) < 9 and abs(n) > 0 and y > 2 and y < 6)*abs(x - 7*(n < 0))*0.01\
                    + (abs(n) == 50 and (y == 1 or y == 5) and x == 7*(n < 0))*0.05
    return evaluation

def basicbot():
    update()
    global turn
    for n in range(1, 51):
        for i in range(8):
            if n in Chessbot1.board[i]:
                for x1 in range(8):
                    for y1 in range(8):
                        if (Chessbot1.piecemove(n, i, Chessbot1.board[i].index(n), x1, y1) 
                            and not Chessbot1.pin(n, i, Chessbot1.board[i].index(n), x1, y1)):
                            movepieceto(n, i, Chessbot1.board[i].index(n), x1, y1)
                            movescore.append(evaluate())
                            Chessbot1.turn = (Chessbot1.bot == 0)
                        else:
                            movescore.append(-1000000)
                        restore()
                break
        else:
            for x1 in range(8):
                for y1 in range(8):
                    movescore.append(-1000000)
    bestmove = movescore.index(max(movescore))
    move = int(bestmove/64)+1
    movetox = int((bestmove-math.floor(bestmove/64)*64)/8)
    movetoy = int(bestmove-movetox*8-(move-1)*64)
    for i in range(8):
        if move in Chessbot1.board[i]:
            Chessbot1.movepieceto(move, i, Chessbot1.board[i].index(move), movetox, movetoy)
            Chessbot1.turn = (Chessbot1.bot == 0)
