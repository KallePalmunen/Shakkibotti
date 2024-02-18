import math
import random
from copy import copy, deepcopy
import numpy as np

def piecemove(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant):
    #checks if a certain piece can move (also there is specific functions for each piece like pawnmove, knightmove etc.)
    #n=unique piece, x0, y0 = starting coordinates, x1, y1 = ending coordinates
    if y1 < 8 and x1 < 8 and x1 >= 0 and y1 >= 0 and y0 >= 0:
        if abs(piece) < 10:
            return pawnmove(state, piece, y0, x0, y1, x1, enpassant)
        if abs(piece) < 20 and abs(piece) >= 10:
            return knightmove(state, piece, y0, x0, y1, x1)
        if abs(piece) < 30 and abs(piece) >= 20:
            return bishopmove(state, piece, y0, x0, y1, x1)
        if abs(piece) < 40 and abs(piece) >= 30:
            return rookmove(state, piece, y0, x0, y1, x1)
        if abs(piece) < 50 and abs(piece) >= 40:
            return queenmove(state, piece, y0, x0, y1, x1)
        if abs(piece) >= 50:
            return kingmove(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant)
        return False
    return False

def botpiecemove(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant):
    if state[y1,x1] > 0:
        return False
    if abs(piece) < 10:
        return pawnmove(state, piece, y0, x0, y1, x1, enpassant)
    if abs(piece) < 20 and abs(piece) >= 10:
        return True
    if abs(piece) < 50 and abs(piece) >= 20:
        return longmove(state, piece, y0, x0, y1, x1)
    if abs(piece) >= 50:
        return kingmove(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant)
    return False

def canmove(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant, kingy = -1, kingx = -1):
    if piecemove(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant) and not pin(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, enpassant, pieces, kingy, kingx):
        return True
    return False

def pawnmove(state, piece, y0, x0, y1, x1, enpassant):
    if piece > 0:
        if ((x1 == x0 and (y1-y0 == 1 or (y1-y0 == 2 and y0 == 1 and state[y1-1,x1] == 0)) and state[y1,x1] == 0) or 
            (y1-y0 == 1 and state[y1,x1] < 0 and (x1 - x0 == 1 or x1 - x0 == -1)) 
            or (x1*8+y1 == enpassant and abs(x1-x0) == 1 and y1 - y0 == 1)):
            return True
        return False
    elif piece < 0:
        if ((x1 == x0 and (y0-y1 == 1 or (y0-y1 == 2 and y0 == 6 and state[y1+1,x1] == 0)) and state[y1,x1] == 0) or
            (y0-y1 == 1 and state[y1,x1] > 0 and (x1 - x0 == 1 or x1 - x0 == -1)) 
            or (x1*8+y1 == enpassant and abs(x1-x0) == 1 and y0 - y1 == 1)):
            return True
        return False
    return False

def knightmove(state, piece, y0, x0, y1, x1):
    if ((y1 - y0 == 2 or y1 - y0 == -2) and (x1 - x0 == 1 or x1 - x0 == -1)) or ((y1- y0 == 1 or y1 - y0 == -1) and
        (x1 - x0 == 2 or x1 - x0 == -2)):
        if piece > 0 and (state[y1,x1] == 0 or state[y1,x1] < 0):
            return True
        if piece < 0 and (state[y1,x1] == 0 or state[y1,x1] > 0):
            return True
        return False
    return False


#longmove checks if there is anything in the way when moving bishops, rooks and queens. It also checks whether there is a piece in the endsquare
#math.copysign((x1-x0 != 0),x1-x0): if x1 != x0: it gives the sign of (x1-x0)

def longmove(state, piece, y0, x0, y1, x1):
    if(piece > 0):
        for i in range(1, 8):
            if(y0 + math.copysign((y1-y0 != 0),y1-y0)*i == y1 and x0 + 
               math.copysign((x1-x0 != 0),x1-x0)*i == x1 and state[y1,x1] <= 0):
                return True
            if(state[y0 + int(math.copysign((y1-y0 != 0),y1-y0))*i, x0 + int(math.copysign((x1-x0 != 0),x1-x0))*i] != 0):
                return False
        return False
    if(piece < 0):
        for i in range(1, 8):
            if(y0 + math.copysign((y1-y0 != 0),y1-y0)*i == y1 and x0 + 
               math.copysign((x1-x0!=0),x1-x0)*i == x1 and state[y1,x1] >= 0):
                return True
            if(state[y0 + int(math.copysign((y1-y0 != 0),y1-y0))*i, x0 + int(math.copysign((x1-x0 != 0),x1-x0))*i] != 0):
                return False
        return False
    return False

def bishopmove(state, piece, y0, x0, y1, x1):
    if(abs(y1-y0) == abs(x1-x0)):
        return longmove(state, piece, y0, x0, y1, x1)
    return False

def rookmove(state, piece, y0, x0, y1, x1):
    if (y1-y0 != 0 and x1-x0 == 0) or (y1-y0 == 0 and x1-x0 != 0):
        return longmove(state, piece, y0, x0, y1, x1)
    return False

def queenmove(state, piece, y0, x0, y1, x1):
    if bishopmove(state, piece, y0, x0, y1, x1) or rookmove(state, piece, y0, x0, y1, x1):
        return True
    return False

def kingmove(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant):
    if castle(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant):
        return True
    if abs(y1-y0) <= 1 and abs(x1-x0) <= 1 and not (y1 == y0 and x1 == x0):
        if piece > 0 and state[y1,x1] <= 0:
            return True
        if piece < 0 and state[y1,x1] >= 0:
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

def checkmate(state, piece, kingmoved, rookmoved, pieces, enpassant):
    for i in range(8):
        if piece in state[i]:
            kingy = i
            kingx = np.where(state[i] == piece)[0][0]
            break
    else:
        return True
    if not check(state, piece, kingmoved, rookmoved, pieces, enpassant, kingy, kingx):
        return False
    if piece > 0:
        for y0 in range(8):
            for x0 in range(8):
                tested_piece = state[y0,x0]
                if tested_piece > 0:
                    for y1 in range(8):
                        for x1 in range(8):
                            if canmove(state, tested_piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant):
                                return False
        return True
    if piece < 0:
        for y0 in range(8):
            for x0 in range(8):
                tested_piece = state[y0,x0]
                if tested_piece < 0:
                    for y1 in range(8):
                        for x1 in range(8):
                            if piecemove(state, tested_piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant)\
                            and not pin(state, tested_piece, y0, x0, y1, x1, kingmoved, rookmoved, enpassant, pieces):
                                return False
        return True


def check(state, piece, kingmoved, rookmoved, pieces, enpassant, kingy = -1, kingx = -1):
    if kingy == -1:
        for i in range(8):
            if piece in state[i]:
                kingy = i
                kingx = np.where(state[i] == piece)[0][0]
                break
        else:
            return True
    if piece > 0:
        for y0 in range(8):
            for x0 in range(8):
                tested_piece = state[y0,x0]
                if tested_piece < 0:
                    if piecemove(state, tested_piece, y0, x0, kingy, kingx, kingmoved, rookmoved, pieces, enpassant):
                        return True
                    break
        return False
    if piece < 0:
        for y0 in range(8):
            for x0 in range(8):
                tested_piece = state[y0,x0]
                if tested_piece > 0:
                    if piecemove(state, tested_piece, y0, x0, kingy, kingx, kingmoved, rookmoved, pieces, enpassant):
                        return True
                    break
        return False
    return False

def pin(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, enpassant, pieces, kingy = -1, kingx = -1):
    state[y0,x0] = 0
    movetosquare = state[y1,x1]
    state[y1,x1] = piece
    #checks if you can prevent the mate in next turn by enpassanting the checking piece
    enpassanted = ""
    if enpassant >= 0 and x1*8+y1 == enpassant:
        enpassanted = state[y1-int(math.copysign(1, y1 - y0)), x1]
        state[y1-int(math.copysign(1, y1 - y0)), y1] = 0
    if not check(state, np.sign(piece)*50, kingmoved, rookmoved, pieces, enpassant, kingy, kingx):
        state[y0,x0] = piece
        state[y1,x1] = movetosquare
        if enpassanted != "":
            state[y1-int(math.copysign(1, y1 - y0)), x1] = enpassanted
        return False
    state[y0,x0] = piece
    state[y1,x1] = movetosquare
    if enpassanted != "":
        state[y1-int(math.copysign(1, y1 - y0)), x1] = enpassanted
    return True

def castle(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant):
    if(kingmoved[(piece > 0)] == 1) or rookmoved[int(piece > 0)][int(x1 > 4)]:
        return False
    if(y1 == y0 and (x1 == 1 or x1 == 5) and not check(state, piece, kingmoved, rookmoved, pieces, enpassant)) and state[y0, (x1 > 4)*7] == int(math.copysign(30 + (x1 > 4), piece)):
        for i in range(1, 3 + (x1 > 4)):
            if state[y0, x0 + i*int(math.copysign(1, x1-x0))] != 0:
                return False
            state[y0,x0] = 0
            state[y0,x0 + i*int(math.copysign(1, x1-x0))] = piece
            if i < 3 and check(state, piece, kingmoved, rookmoved, pieces, enpassant):
                state[y0,x0] = piece
                state[y0, x0 + i*int(math.copysign(1, x1-x0))] = 0
                return False
            state[y0,x0] = piece
            state[y0, x0 + i*int(math.copysign(1, x1-x0))] = 0
        return True
    return False

def movesomewhere(state, piece, y0, x0, kingmoved, rookmoved, pieces, enpassant):
    for i in range(8):
        for ii in range(8):
            if piecemove(state, piece, y0, x0, i, ii, kingmoved, rookmoved, pieces, enpassant) and not pin(piece, y0, x0, i, ii):
                return True
    return False

def stalemate(state, piece, kingmoved, rookmoved, pieces, enpassant):
    if check(state, piece, kingmoved, rookmoved, pieces, enpassant):
        return False
    for i in range(1, 51):
        for ii in range(8):
            if int(math.copysign(i, piece)) in state[ii]:
                if movesomewhere(int(math.copysign(i, piece)), ii, np.where(state[ii] == int(math.copysign(i, piece)))[0][0]):
                    return False
                break
    return True