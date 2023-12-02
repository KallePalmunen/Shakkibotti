import math
import random
from copy import copy, deepcopy
import numpy as np

def piecemove(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant):
    #checks if a certain piece can move (also there is specific functions for each piece like pawnmove, knightmove etc.)
    #n=unique piece, x0, y0 = starting coordinates, x1, y1 = ending coordinates
    if y1 < 8 and x1 < 8 and x1 >= 0 and y1 >= 0 and y0 >= 0:
        if abs(piece) < 10:
            if pawnmove(state, piece, y0, x0, y1, x1, enpassant):
                return True
            return False
        if abs(piece) < 20 and abs(piece) >= 10:
            if knightmove(state, piece, y0, x0, y1, x1):
                return True
            return False
        if abs(piece) < 30 and abs(piece) >= 20:
            if bishopmove(state, piece, y0, x0, y1, x1):
                return True
            return False
        if abs(piece) < 40 and abs(piece) >= 30:
            if rookmove(state, piece, y0, x0, y1, x1):
                return True
            return False
        if abs(piece) < 50 and abs(piece) >= 40:
            if queenmove(state, piece, y0, x0, y1, x1):
                return True
            return False
        if abs(piece) >= 50:
            if kingmove(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant):
                return True
            return False
        return False
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
        if longmove(state, piece, y0, x0, y1, x1):
            return True
        return False
    return False

def rookmove(state, piece, y0, x0, y1, x1):
    if (y1-y0 != 0 and x1-x0 == 0) or (y1-y0 == 0 and x1-x0 != 0):
        if longmove(state, piece, y0, x0, y1, x1):
            return True
        return False
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
    if not check(state, piece, kingmoved, rookmoved, pieces, enpassant):
        return False
    if piece > 0:
        for n1 in range(6):
            for n2 in range(pieces[n1][0]):
                i = n1*10+n2
                for ii in range(8):
                    if i in state[ii]:
                        piecey0 = ii
                        piecex0 = np.where(state[ii] == i)[0]
                        for j in range(8):
                            for jj in range(8):
                                if piecemove(state, i, piecey0, piecex0, j, jj, kingmoved, rookmoved, pieces, enpassant):
                                    state[piecey0,piecex0] = 0
                                    movetosquare = state[j,jj]
                                    state[j,jj] = i
                                    enpassanted = ""
                                    if enpassant >= 0 and jj*8+j == enpassant:
                                        enpassanted = state[j-int(math.copysign(1, j - piecey0)), jj]
                                        state[j-int(math.copysign(1, j - piecey0)), jj] = 0
                                    if not check(state, piece, kingmoved, rookmoved, pieces, enpassant):
                                        state[piecey0,piecex0] = i
                                        state[j,jj] = movetosquare
                                        if enpassanted != "":
                                            state[j-int(math.copysign(1, j - piecey0)),jj] = enpassanted
                                        return False
                                    state[piecey0,piecex0] = i
                                    state[j,jj] = movetosquare
                                    if enpassanted != "":
                                        state[j-int(math.copysign(1, j - piecey0)), jj] = enpassanted
                        break
        return True
    if piece < 0:
        for n1 in range(6):
            for n2 in range(pieces[n1][1]):
                i = n1*10+n2
                for ii in range(8):
                    if -i in state[ii]:
                        piecey0 = ii
                        piecex0 = np.where(state[ii] == -i)[0]
                        for j in range(8):
                            for jj in range(8):
                                if piecemove(state, -i, piecey0, piecex0, j, jj, kingmoved, rookmoved, pieces, enpassant):
                                    state[piecey0,piecex0] = 0
                                    movetosquare = state[j,jj]
                                    state[j,jj] = -i
                                    if not check(state, piece, kingmoved, rookmoved, pieces, enpassant):
                                        state[piecey0,piecex0] = -i
                                        state[j,jj] = movetosquare
                                        return False
                                    state[piecey0,piecex0] = -i
                                    state[j,jj] = movetosquare
                        break
        return True


def check(state, piece, kingmoved, rookmoved, pieces, enpassant):
    if piece < 0:
        for i in range(8):
            if piece in state[i]:
                kingy = i
                kingx = np.where(state[i] == piece)[0]
                break
        else:
            return True
        for n1 in range(6):
            for n2 in range(pieces[n1][0]):
                i = n1*10+n2
                for ii in range(8):
                    if i in state[ii]:
                        if piecemove(state, i, ii, np.where(state[ii] == i)[0], kingy, kingx, kingmoved, rookmoved, pieces, enpassant):
                            return True
                        break
        return False
    if piece > 0:
        for i in range(8):
            if piece in state[i]:
                kingy = i
                kingx = np.where(state[i] == piece)[0]
                break
        else:
            return True
        for n1 in range(6):
            for n2 in range(state[n1][1]):
                i = n1*10+n2
                for ii in range(8):
                    if -i in state[ii]:
                        if piecemove(state, -i, ii, np.where(state[ii] == -i)[0], kingy, kingx, kingmoved, rookmoved, pieces, enpassant):
                            return True
                        break
        return False
    return False

def pin(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, enpassant, pieces):
    state[y0,x0] = 0
    movetosquare = state[y1,x1]
    state[y1,x1] = piece
    #checks if you can prevent the mate in next turn by enpassanting the checking piece
    enpassanted = ""
    if enpassant >= 0 and x1*8+y1 == enpassant:
        enpassanted = state[y1-int(math.copysign(1, y1 - y0)), x1]
        state[y1-int(math.copysign(1, y1 - y0)), y1] = 0
    if not check(state, int(math.copysign(50,piece)), kingmoved, rookmoved, pieces, enpassant):
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
    if(kingmoved[(piece > 0)] == 1) or rookmoved[(piece > 0)][(x1 > 4)]:
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
                if movesomewhere(int(math.copysign(i, piece)), ii, np.where(state[ii] == int(math.copysign(i, piece)))[0]):
                    return False
                break
    return True