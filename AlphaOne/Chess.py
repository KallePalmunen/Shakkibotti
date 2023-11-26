import math
import random
from copy import copy, deepcopy
import numpy as np

def piecemove(piece, y0, x0, y1, x1):
    #checks if a certain piece can move (also there is specific functions for each piece like pawnmove, knightmove etc.)
    #n=unique piece, x0, y0 = starting coordinates, x1, y1 = ending coordinates
    if y1 < 8 and x1 < 8 and x1 >= 0 and y1 >= 0 and y0 >= 0:
        if abs(piece) < 10:
            if pawnmove(piece, y0, x0, y1, x1):
                return True
            return False
        if abs(piece) < 20 and abs(piece) >= 10:
            if knightmove(piece, y0, x0, y1, x1):
                return True
            return False
        if abs(piece) < 30 and abs(piece) >= 20:
            if bishopmove(piece, y0, x0, y1, x1):
                return True
            return False
        if abs(piece) < 40 and abs(piece) >= 30:
            if rookmove(piece, y0, x0, y1, x1):
                return True
            return False
        if abs(piece) < 50 and abs(piece) >= 40:
            if queenmove(piece, y0, x0, y1, x1):
                return True
            return False
        if abs(piece) >= 50:
            if kingmove(piece, y0, x0, y1, x1):
                return True
            return False
        return False
    return False

def pawnmove(piece, y0, x0, y1, x1):
    if piece > 0:
        if ((x1 == x0 and (y1-y0 == 1 or (y1-y0 == 2 and y0 == 1 and board[y1-1][x1] == 0)) and board[y1][x1] == 0) or 
            (y1-y0 == 1 and board[y1][x1] < 0 and (x1 - x0 == 1 or x1 - x0 == -1)) 
            or (x1*8+y1 == enpassant and abs(x1-x0) == 1 and y1 - y0 == 1)):
            return True
        return False
    elif piece < 0:
        if ((x1 == x0 and (y0-y1 == 1 or (y0-y1 == 2 and y0 == 6 and board[y1+1][x1] == 0)) and board[y1][x1] == 0) or
            (y0-y1 == 1 and board[y1][x1] > 0 and (x1 - x0 == 1 or x1 - x0 == -1)) 
            or (x1*8+y1 == enpassant and abs(x1-x0) == 1 and y0 - y1 == 1)):
            return True
        return False
    return False

def knightmove(piece, y0, x0, y1, x1):
    if ((y1 - y0 == 2 or y1 - y0 == -2) and (x1 - x0 == 1 or x1 - x0 == -1)) or ((y1- y0 == 1 or y1 - y0 == -1) and
        (x1 - x0 == 2 or x1 - x0 == -2)):
        if piece > 0 and (board[y1][x1] == 0 or board[y1][x1] < 0):
            return True
        if piece < 0 and (board[y1][x1] == 0 or board[y1][x1] > 0):
            return True
        return False
    return False


#longmove checks if there is anything in the way when moving bishops, rooks and queens. It also checks whether there is a piece in the endsquare
#math.copysign((x1-x0 != 0),x1-x0): if x1 != x0: it gives the sign of (x1-x0)

def longmove(piece, y0, x0, y1, x1):
    if(piece > 0):
        for i in range(1, 8):
            if(y0 + math.copysign((y1-y0 != 0),y1-y0)*i == y1 and x0 + 
               math.copysign((x1-x0 != 0),x1-x0)*i == x1 and board[y1][x1] <= 0):
                return True
            if(board[y0 + int(math.copysign((y1-y0 != 0),y1-y0))*i][x0 + int(math.copysign((x1-x0 != 0),x1-x0))*i] != 0):
                return False
        return False
    if(piece < 0):
        for i in range(1, 8):
            if(y0 + math.copysign((y1-y0 != 0),y1-y0)*i == y1 and x0 + 
               math.copysign((x1-x0!=0),x1-x0)*i == x1 and board[y1][x1] >= 0):
                return True
            if(board[y0 + int(math.copysign((y1-y0 != 0),y1-y0))*i][x0 + int(math.copysign((x1-x0 != 0),x1-x0))*i] != 0):
                return False
        return False
    return False

def bishopmove(piece, y0, x0, y1, x1):
    if(abs(y1-y0) == abs(x1-x0)):
        if longmove(piece, y0, x0, y1, x1):
            return True
        return False
    return False

def rookmove(piece, y0, x0, y1, x1):
    if (y1-y0 != 0 and x1-x0 == 0) or (y1-y0 == 0 and x1-x0 != 0):
        if longmove(piece, y0, x0, y1, x1):
            return True
        return False
    return False

def queenmove(piece, y0, x0, y1, x1):
    if bishopmove(piece, y0, x0, y1, x1) or rookmove(piece, y0, x0, y1, x1):
        return True
    return False

def kingmove(piece, y0, x0, y1, x1):
    if castle(piece, y0, x0, y1, x1):
        return True
    if abs(y1-y0) <= 1 and abs(x1-x0) <= 1 and not (y1 == y0 and x1 == x0):
        if piece > 0 and board[y1][x1] <= 0:
            return True
        if piece < 0 and board[y1][x1] >= 0:
            return True
        return False
    return False

def promote(piece, y1):
    if piece < 10 and piece > 0:
        if y1 == 7:
            return True
    if piece > -10 and piece < 0:
        if y1 == 0:
            return True

def checkmate(piece):
    if not check(piece):
        return False
    if piece > 0:
        for n1 in range(6):
            for n2 in range(pieces[n1][0]):
                i = n1*10+n2
                for ii in range(8):
                    if i in board[ii]:
                        piecey0 = ii
                        piecex0 = board[ii].index(i)
                        for j in range(8):
                            for jj in range(8):
                                if piecemove(i, piecey0, piecex0, j, jj):
                                    board[piecey0][piecex0] = 0
                                    movetosquare = board[j][jj]
                                    board[j][jj] = i
                                    enpassanted = ""
                                    if enpassant >= 0 and jj*8+j == enpassant:
                                        enpassanted = board[j-int(math.copysign(1, j - piecey0))][jj]
                                        board[j-int(math.copysign(1, j - piecey0))][jj] = 0
                                    if not check(piece):
                                        board[piecey0][piecex0] = i
                                        board[j][jj] = movetosquare
                                        if enpassanted != "":
                                            board[j-int(math.copysign(1, j - piecey0))][jj] = enpassanted
                                        return False
                                    board[piecey0][piecex0] = i
                                    board[j][jj] = movetosquare
                                    if enpassanted != "":
                                        board[j-int(math.copysign(1, j - piecey0))][jj] = enpassanted
                        break
        return True
    if piece < 0:
        for n1 in range(6):
            for n2 in range(pieces[n1][1]):
                i = n1*10+n2
                for ii in range(8):
                    if -i in board[ii]:
                        piecey0 = ii
                        piecex0 = board[ii].index(-i)
                        for j in range(8):
                            for jj in range(8):
                                if piecemove(-i, piecey0, piecex0, j, jj):
                                    board[piecey0][piecex0] = 0
                                    movetosquare = board[j][jj]
                                    board[j][jj] = -i
                                    if not check(piece):
                                        board[piecey0][piecex0] = -i
                                        board[j][jj] = movetosquare
                                        return False
                                    board[piecey0][piecex0] = -i
                                    board[j][jj] = movetosquare
                        break
        return True


def check(piece):
    if piece < 0:
        for i in range(8):
            if piece in board[i]:
                kingy = i
                kingx = board[i].index(piece)
                break
        else:
            return True
        for n1 in range(6):
            for n2 in range(pieces[n1][0]):
                i = n1*10+n2
                for ii in range(8):
                    if i in board[ii]:
                        if piecemove(i, ii, board[ii].index(i), kingy, kingx):
                            return True
                        break
        return False
    if piece > 0:
        for i in range(8):
            if piece in board[i]:
                kingy = i
                kingx = board[i].index(n)
                break
        else:
            return True
        for n1 in range(6):
            for n2 in range(pieces[n1][1]):
                i = n1*10+n2
                for ii in range(8):
                    if -i in board[ii]:
                        if piecemove(-i, ii, board[ii].index(-i), kingy, kingx):
                            return True
                        break
        return False
    return False

def pin(piece, y0, x0, y1, x1, promoter = ""):
    board[y0][x0] = 0
    movetosquare = board[y1][x1]
    board[y1][x1] = piece
    #checks if you can prevent the mate in next turn by enpassanting the checking piece
    enpassanted = ""
    if enpassant >= 0 and x1*8+y1 == enpassant:
        enpassanted = board[y1-int(math.copysign(1, y1 - y0))][x1]
        board[y1-int(math.copysign(1, y1 - y0))][y1] = 0
    if not check(int(math.copysign(50,piece))):
        board[y0][x0] = piece
        board[y1][x1] = movetosquare
        if enpassanted != "":
            board[y1-int(math.copysign(1, y1 - y0))][x1] = enpassanted
        return False
    board[y0][x0] = piece
    board[y1][x1] = movetosquare
    if enpassanted != "":
        board[y1-int(math.copysign(1, y1 - y0))][x1] = enpassanted
    return True

def castle(piece, y0, x0, y1, x1):
    if(kingmoved[(piece > 0)] == 1) or rookmoved[(piece > 0)][(x1 > 4)]:
        return False
    if(y1 == y0 and (x1 == 1 or x1 == 5) and not check(piece)) and board[y0][(x1 > 4)*7] == int(math.copysign(30 + (x1 > 4), piece)):
        for i in range(1, 3 + (x1 > 4)):
            if board[y0][x0 + i*int(math.copysign(1, x1-x0))] != 0:
                return False
            board[y0][x0] = 0
            board[y0][x0 + i*int(math.copysign(1, x1-x0))] = piece
            if i < 3 and check(piece):
                board[y0][x0] = piece
                board[y0][x0 + i*int(math.copysign(1, x1-x0))] = 0
                return False
            board[y0][x0] = piece
            board[y0][x0 + i*int(math.copysign(1, x1-x0))] = 0
        return True
    return False

def movesomewhere(piece, y0, x0):
    for i in range(8):
        for ii in range(8):
            if piecemove(piece, y0, x0, i, ii) and not pin(piece, y0, x0, i, ii):
                return True
    return False

def stalemate(piece):
    if check(piece):
        return False
    for i in range(1, 51):
        for ii in range(8):
            if int(math.copysign(i, piece)) in board[ii]:
                if movesomewhere(int(math.copysign(i, piece)), ii, board[ii].index(int(math.copysign(i, piece)))):
                    return False
                break
    return True

def compareposition(m):
    for j in range(8):
        for jj in range(8):
            if positions[m][j][jj] != board[j][jj]:
                return False
    return True

def repetition(m):
    repetitions = 0
    for i in range(m):
        if compareposition(i) and i%2 == m%2:
            repetitions += 1
            if(repetitions >= 2):
                return True
    return False

def movepieceto(piece, y0, x0, y1, x1, promoter = ""):
    global turn
    global moves
    global enpassant
    global promotemenu
    if(abs(piece) == 50):
        if abs(x1 - x0) > 1:
            whichrook = int(math.copysign(30 + (x1 > 4), piece))
            board[y1][board[y1].index(whichrook)] = 0
            board[y1][x1 + int(math.copysign(1, 4-x1))] = whichrook
        kingmoved[(piece > 0)] = 1
    board[y0][x0] = 0
    if(abs(piece) == 30 or abs(piece) == 31):
        rookmoved[(piece >0)][abs(piece)-30] = 1
    if promote(piece, y1):
        if promoter != "":
            promoteto = promoter
        elif bot == turn:
            promoteto = 4
        else:
            promotemenu = True
            return
        board[y1][x1] = int(math.copysign(1, piece))*(promoteto*10+pieces[promoteto][(piece < 0)])
        pieces[promoteto][(piece < 0)] += 1
    else:
        board[y1][x1] = piece
    if enpassant >= 0 and x1*8+y1 == enpassant:
        board[y1-int(math.copysign(1, y1 - y0))][x1] = 0
    if abs(piece) < 10 and abs(y1 - y0) > 1:
        enpassant = x1*8+y0+int(math.copysign(1, y1 - y0))
    else:
        enpassant = -1
    moves += 1
    positions.append(deepcopy(board))

def movepiece():
    global turn
    global moves
    global enpassant
    try:
        move = int(input())
        movetox = int(input())
        movetoy = int(input())
    except ValueError:
        return
    if (move > 0 and turn == 0) or (move < 0 and turn == 1):
        for i in range(8):
            if move in board[i]:
                if (piecemove(move, i, board[i].index(move), movetox, movetoy) 
                    and not pin(move, i, board[i].index(move), movetox, movetoy)):
                    movepieceto(move, i, board[i].index(move), movetox, movetoy)
                    break
                else:
                    print('Illegal move')
                    return
        else:
            print('Illegal move')
            return
        if turn == 0:
            turn = 1
        else:
            turn = 0
    else:
        print('Illegal move')
        return

def randommove():
    global turn
    while turn == bot:
        move = int(random.random()*51*math.copysign(1, 0.5-bot))
        movetoy = int(random.random()*8)
        movetox = int(random.random()*8)
        for i in range(8):
            if move in board[i]:
                if (piecemove(move, i, board[i].index(move), movetoy, movetox) 
                    and not pin(move, i, board[i].index(move), movetoy, movetox)):
                    movepieceto(move, i, board[i].index(move), movetoy, movetox)
                    turn = (bot == 0)
                    
                    return
                else:
                    break


def gameend():
    global turn
    if checkmate(-50):
        print('White won')
        turn = -1
    if checkmate(50):
        print('Black won')
        turn = -1
    if (turn == 0 and stalemate(50)) or (turn == 1 and stalemate(-50)) or repetition(moves):
        print('Draw')
        turn = -1