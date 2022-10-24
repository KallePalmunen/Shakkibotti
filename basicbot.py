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

def check(kingx, kingy):
    for n1 in range(2,5):
        for n2 in range(Chessbot1.pieces[n1][0]):
            i = n1*10+n2
            for ii in range(8):
                if i in Chessbot1.board[ii]:
                    if Chessbot1.piecemove(i, ii, Chessbot1.board[ii].index(i), kingx, kingy):
                        return True
                    break
    return False

def partialpin(n,x0,y0,x1,y1,kingx,kingy):
    Chessbot1.board[x0][y0] = 0
    movetosquare = Chessbot1.board[x1][y1]
    Chessbot1.board[x1][y1] = n
    if not check(kingx,kingy):
        Chessbot1.board[x0][y0] = n
        Chessbot1.board[x1][y1] = movetosquare
        return False
    Chessbot1.board[x0][y0] = n
    Chessbot1.board[x1][y1] = movetosquare
    return True

def pinnable(n,x0,y0,kingx,kingy):
    Chessbot1.board[x0][y0] = 0
    if not check(kingx,kingy):
        Chessbot1.board[x0][y0] = n
        return False
    Chessbot1.board[x0][y0] = n
    return True

def movepieceto2(n, x0, y0, x1, y1):
    Chessbot1.board[x0][y0] = 0
    if Chessbot1.promote(n, x1):
        Chessbot1.board[x1][y1] = int(math.copysign(1, n))*(40+Chessbot1.pieces[4][(n < 0)])
    else:
        Chessbot1.board[x1][y1] = n

def partialrepetition(m):
    for i in range(m):
        if Chessbot1.compareposition(i) and i%2 == m%2:
            return True
    return False

def fulleval():
    evaluation = 0
    for n in range(1, 51):
        for x in range(8):
            if n in Chessbot1.board[x]:
                evaluation += evaluate2(n, x, Chessbot1.board[x].index(n))
                break
    for n in range(1, 51):
        for x in range(8):
            if -n in Chessbot1.board[x]:
                evaluation += evaluate2(-n, x, Chessbot1.board[x].index(-n))
                break
    return evaluation


def evaluate(n, x0, y0, x1, y1):
    return evaluate2(n, x1, y1)+evaluate1(n,x0,y0)

def evaluate2(n, x1, y1):
    n1 = Chessbot1.board[min(7, x1 + 1)][min(7, y1+1)]
    n2 = Chessbot1.board[min(7, x1 + 1)][max(0, y1-1)]
    evaluation = math.copysign((n != 0 and abs(n) < 9)*(1+(abs(x1 - 7*(n < 0))*((y1 > 2 and y1 < 6)*((y1 == 5)*0.01+(y1 != 5)*0.05) + 0.001)\
                - (Chessbot1.moves < 60 and y1 < 2)*(abs(x1 - 7*(n < 0)) > 2)*0.03))\
                + 3*(abs(n) > 9 and abs(n) < 30) + \
                (5 + (not ((x1 == 0 or x1 == 7) and (y1 == 0 or y1 == 7)))*0.01)\
                *(abs(n) > 29 and abs(n) < 40) + \
                9*(abs(n) > 39)+ ((abs(n) >= 10 and abs(n) < 30) or (abs(n) >= 40 and abs(n) < 50))*(x1 != 7*(n < 0))*(0.05+0.001*(abs(x1 - 7*(n < 0)) > 1))\
                + (abs(n) >= 10 and abs(n) < 20 and y1 > 1 and y1 < 6)*0.01\
                + ((abs(n) == 50) and (x1 == 0 or x1 == 7) and (y1 == 1 or y1 == 5))*0.05, n)\
                *(1-((n > 0) and ((n1 < 0 and n1 >-9) or (n2 < 0 and n2 >-9)))*0.5)
    return evaluation

def evaluate1(n, x0, y0):
    n1 = Chessbot1.board[min(7, x0 + 1)][min(7, y0+1)]
    n2 = Chessbot1.board[min(7, x0 + 1)][max(0, y0-1)]
    evaluation = -math.copysign((n != 0 and abs(n) < 9)*(1+(abs(x0 - 7*(n < 0))*((y0 > 2 and y0 < 6)*((y0 == 5)*0.01+(y0 != 5)*0.05) + 0.001)\
                - (Chessbot1.moves < 60 and y0 < 2)*(abs(x0 - 7*(n < 0)) > 2)*0.03))\
                + 3*(abs(n) > 9 and abs(n) < 30) + \
                (5+ (not ((x0 == 0 or x0 == 7) and (y0 == 0 or y0 == 7)))*0.01)\
                *(abs(n) > 29 and abs(n) < 40) + \
                9*(abs(n) > 39)+ ((abs(n) >= 10 and abs(n) < 30) or (abs(n) >= 40 and abs(n) < 50))*(x0 != 7*(n < 0))*(0.05+0.001*(abs(x0 - 7*(n < 0)) > 1))\
                + (abs(n) >= 10 and abs(n) < 20 and y0 > 1 and y0 < 6)*0.01\
                + ((abs(n) == 50) and (x0 == 0 or x0 == 7) and (y0 == 1 or y0 == 5))*0.05, n)\
                *(1-((n > 0) and ((n1 < 0 and n1 >-9) or (n2 < 0 and n2 >-9)))*0.5)
    return evaluation

def evaluate0(x0, y0):
    n = Chessbot1.board[x0][y0]
    n1 = Chessbot1.board[min(7, x0 + 1)][min(7, y0+1)]
    n2 = Chessbot1.board[min(7, x0 + 1)][max(0, y0-1)]
    return -math.copysign((n != 0 and abs(n) < 9)*(1+(abs(x0 - 7*(n < 0))*((y0 > 2 and y0 < 6)*((y0 == 5)*0.01+(y0 != 5)*0.05) + 0.001)\
                - (Chessbot1.moves < 60 and y0 < 2)*(abs(x0 - 7*(n < 0)) > 2)*0.03))\
                + 3*(abs(n) > 9 and abs(n) < 30) + \
                (5+ (not ((x0 == 0 or x0 == 7) and (y0 == 0 or y0 == 7)))*0.01)\
                *(abs(n) > 29 and abs(n) < 40) + \
                9*(abs(n) > 39)+ ((abs(n) >= 10 and abs(n) < 30) or (abs(n) >= 40 and abs(n) < 50))*(x0 != 7*(n < 0))*(0.05+0.001*(abs(x0 - 7*(n < 0)) > 1))\
                + (abs(n) >= 10 and abs(n) < 20 and y0 > 1 and y0 < 6)*0.01\
                + ((abs(n) == 50) and (x0 == 0 or x0 == 7) and (y0 == 1 or y0 == 5))*0.05, n)\
                *(1-((n > 0) and ((n1 < 0 and n1 >-9) or (n2 < 0 and n2 >-9)))*0.5)


def blackmove(n01, x01, y01, x11, y11):
    if Chessbot1.checkmate(-50):
        return 500000
    moves = Chessbot1.moves
    enpassant = Chessbot1.enpassant
    board = str(Chessbot1.board)
    kingmoved = str(Chessbot1.kingmoved)
    rookmoved = str(Chessbot1.rookmoved)
    pieces = str(Chessbot1.pieces)
    movescore1 = [1000000]
    evaluation0 = evaluate(n01, x01, y01, x11, y11)
    for n in range(1, 9):
        for i in range(1, 8):
            if -n in Chessbot1.board[i]:
                y0 = Chessbot1.board[i].index(-n)
                for x1 in range(i - 2, i):
                    for y1 in range(y0 - 1, y0+2):
                        if (Chessbot1.piecemove(-n, i, y0, x1, y1) 
                            and not Chessbot1.pin(-n, i, y0, x1, y1)):
                            evaluation1 = evaluate0(x1, y1)
                            movepieceto(-n, i, y0, x1, y1)
                            movescore1 += wmove(-n, i, y0, x1, y1) + evaluation1,
                            Chessbot1.turn = Chessbot1.bot
                                
                            Chessbot1.moves = moves
                            Chessbot1.enpassant = enpassant
                            Chessbot1.board = json.loads(board)
                            Chessbot1.kingmoved = json.loads(kingmoved)
                            Chessbot1.rookmoved = json.loads(rookmoved)
                            Chessbot1.pieces = json.loads(pieces)
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
                            evaluation1 = evaluate0(x1, y1)
                            movepieceto(-n, i, y0, x1, y1)
                            movescore1 += wmove(-n, i, y0, x1, y1)+evaluation1,
                            Chessbot1.turn = Chessbot1.bot
                                
                            Chessbot1.moves = moves
                            Chessbot1.enpassant = enpassant
                            Chessbot1.board = json.loads(board)
                            Chessbot1.kingmoved = json.loads(kingmoved)
                            Chessbot1.rookmoved = json.loads(rookmoved)
                            Chessbot1.pieces = json.loads(pieces)
    for n1 in range(2, 6):
        for n2 in range(Chessbot1.pieces[n1][1]):
            n = n1*10+n2
            for i in range(8):
                if -n in Chessbot1.board[i]:
                    for x1 in range(8):
                        for y1 in range(8):
                            if (Chessbot1.piecemove(-n, i, Chessbot1.board[i].index(-n), x1, y1) 
                                and not Chessbot1.pin(-n, i, Chessbot1.board[i].index(-n), x1, y1)):
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto(-n, i, Chessbot1.board[i].index(-n), x1, y1)
                                movescore1 += wmove(-n, i, y0, x1, y1)+evaluation1,
                                Chessbot1.turn = Chessbot1.bot
                                
                                Chessbot1.moves = moves
                                Chessbot1.enpassant = enpassant
                                Chessbot1.board = json.loads(board)
                                Chessbot1.kingmoved = json.loads(kingmoved)
                                Chessbot1.rookmoved = json.loads(rookmoved)
                                Chessbot1.pieces = json.loads(pieces)
                    break
                                
    Chessbot1.moves = moves
    Chessbot1.enpassant = enpassant
    Chessbot1.board = json.loads(board)
    Chessbot1.kingmoved = json.loads(kingmoved)
    Chessbot1.rookmoved = json.loads(rookmoved)
    Chessbot1.pieces = json.loads(pieces)
    Chessbot1.turn = (Chessbot1.bot == 0)
    return min(movescore1)+evaluation0

def bmove(n01, x01, y01, x11, y11):
    if Chessbot1.checkmate(-50):
        return 500000
    movescore3 = [1000000]
    evaluation0 = evaluate(n01, x01, y01, x11, y11)
    for i in range(8):
        if -50 in Chessbot1.board[i]:
            kingx = i
            kingy = Chessbot1.board[i].index(-50)
            break
    for n in range(8):
        for i in range(8):
            if -n in Chessbot1.board[7-i]:
                x0 = 7-i
                y0 = Chessbot1.board[x0].index(-n)
                evaluation2 = evaluate1(-n, x0, y0)
                if not pinnable(-n, x0, y0, kingx, kingy):
                    for x1 in range(x0 - 2, x0):
                        for y1 in range(y0 - 1, y0+2):
                            if (Chessbot1.piecemove(-n, x0, y0, x1, y1)):
                                wastheren = Chessbot1.board[x1][y1]
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto2(-n, x0, y0, x1, y1)
                                movescore3 += evaluate2(-n, x1, y1) + evaluation1 + evaluation2,
                                Chessbot1.turn = Chessbot1.bot
                                
                                Chessbot1.board[x1][y1] = wastheren
                                Chessbot1.board[x0][y0] = -n
                else: 
                    for x1 in range(x0 - 2, x0):
                        for y1 in range(y0 - 1, y0+2):
                            if (Chessbot1.piecemove(-n, x0, y0, x1, y1) 
                                and not partialpin(-n, x0, y0, x1, y1, kingx, kingy)):
                                wastheren = Chessbot1.board[x1][y1]
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto2(-n, x0, y0, x1, y1)
                                movescore3 += evaluate2(-n, x1, y1) + evaluation1 + evaluation2,
                                Chessbot1.turn = Chessbot1.bot
                                    
                                Chessbot1.board[x1][y1] = wastheren
                                Chessbot1.board[x0][y0] = -n
                            
                break
    for n2 in range(Chessbot1.pieces[1][1]):
        n = 10+n2
        for i in range(8):
            if -n in Chessbot1.board[7-i]:
                x0 = 7-i
                y0 = Chessbot1.board[x0].index(-n)
                evaluation2 = evaluate1(-n, x0, y0)
                if not pinnable(-n, x0, y0, kingx, kingy):
                    for x1 in range(x0-2, x0+3):
                        for y1 in range(y0-2, y0+3):
                            if (Chessbot1.piecemove(-n, x0, y0, x1, y1)):
                                wastheren = Chessbot1.board[x1][y1]
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto2(-n, x0, y0, x1, y1)
                                movescore3 += evaluate2(-n, x1, y1) + evaluation1 + evaluation2,
                                Chessbot1.turn = Chessbot1.bot
                                    
                                Chessbot1.board[x1][y1] = wastheren
                                Chessbot1.board[x0][y0] = -n
                else:
                    for x1 in range(x0-2, x0+3):
                        for y1 in range(y0-2, y0+3):
                            if (Chessbot1.piecemove(-n, x0, y0, x1, y1) 
                                and not partialpin(-n, x0, y0, x1, y1, kingx, kingy)):
                                wastheren = Chessbot1.board[x1][y1]
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto2(-n, x0, y0, x1, y1)
                                movescore3 += evaluate2(-n, x1, y1) + evaluation1 + evaluation2,
                                Chessbot1.turn = Chessbot1.bot
                                    
                                Chessbot1.board[x1][y1] = wastheren
                                Chessbot1.board[x0][y0] = -n
                break
    for n1 in range(2, 5):
        for n2 in range(Chessbot1.pieces[n1][1]):
            n = n1*10+n2
            for i in range(8):
                if -n in Chessbot1.board[7-i]:
                    x0 = 7-i
                    y0 = Chessbot1.board[x0].index(-n)
                    evaluation2 = evaluate1(-n, x0, y0)
                    if not pinnable(-n, x0, y0, kingx, kingy):
                        for x1 in range(8):
                            for y1 in range(8):
                                if (Chessbot1.piecemove(-n, x0, y0, x1, y1)):
                                    wastheren = Chessbot1.board[x1][y1]
                                    evaluation1 = evaluate0(x1, y1)
                                    movepieceto2(-n, x0, y0, x1, y1)
                                    if Chessbot1.checkmate(50):
                                        return -500000
                                    movescore3 += evaluate2(-n, x1, y1) + evaluation1 + evaluation2,
                                    Chessbot1.turn = Chessbot1.bot
                                    
                                    Chessbot1.board[x1][y1] = wastheren
                                    Chessbot1.board[x0][y0] = -n
                    else:
                        for x1 in range(8):
                            for y1 in range(8):
                                if (Chessbot1.piecemove(-n, x0, y0, x1, y1) 
                                    and not partialpin(-n, x0, y0, x1, y1, kingx, kingy)):
                                    wastheren = Chessbot1.board[x1][y1]
                                    evaluation1 = evaluate0(x1, y1)
                                    movepieceto2(-n, x0, y0, x1, y1)
                                    if Chessbot1.checkmate(50):
                                        return -500000
                                    movescore3 += evaluate2(-n, x1, y1) + evaluation1 + evaluation2,
                                    Chessbot1.turn = Chessbot1.bot
                                    
                                    Chessbot1.board[x1][y1] = wastheren
                                    Chessbot1.board[x0][y0] = -n
                    break
    n = 50
    for i in range(8):
        if -n in Chessbot1.board[7-i]:
            x0 = 7-i
            y0 = Chessbot1.board[x0].index(-n)
            evaluation2 = evaluate1(-n, x0, y0)
            for x1 in range(x0-1, x0+2):
                for y1 in range(y0-1,y0+2):
                    if (Chessbot1.piecemove(-n, x0, y0, x1, y1) 
                        and not Chessbot1.pin(-n, x0, y0, x1, y1)):
                        wastheren = Chessbot1.board[x1][y1]
                        evaluation1 = evaluate0(x1, y1)
                        movepieceto2(-n, x0, y0, x1, y1)
                        if Chessbot1.checkmate(50):
                            return -500000
                        movescore3 += evaluate2(-n, x1, y1) + evaluation1 + evaluation2,
                        Chessbot1.turn = Chessbot1.bot
                        
                        Chessbot1.board[x1][y1] = wastheren
                        Chessbot1.board[x0][y0] = -n
            break
    Chessbot1.turn = (Chessbot1.bot == 0)
    return min(movescore3)+evaluation0

def wmove(n01, x01, y01, x11, y11):
    if Chessbot1.checkmate(50):
        return -500000
    moves = Chessbot1.moves
    enpassant = Chessbot1.enpassant
    board = str(Chessbot1.board)
    kingmoved = str(Chessbot1.kingmoved)
    rookmoved = str(Chessbot1.rookmoved)
    pieces = str(Chessbot1.pieces)
    movescore2 = [-1000000]
    evaluation0 = evaluate(n01, x01, y01, x11, y11)
    for n in range(1, 9):
        for i in range(8):
            if n in Chessbot1.board[i]:
                y0 = Chessbot1.board[i].index(n)
                for x1 in range(i, 8):
                    if x1 - i <= 2:
                        for y1 in range(8):
                            if (Chessbot1.piecemove(n, i, y0, x1, y1) 
                                and not Chessbot1.pin(n, i, y0, x1, y1)):
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto(n, i, y0, x1, y1)
                                movescore2 += bmove(n, i, y0, x1, y1) + evaluation1,
                                Chessbot1.turn = (Chessbot1.bot == 0)
                                
                                Chessbot1.moves = moves
                                Chessbot1.enpassant = enpassant
                                Chessbot1.board = json.loads(board)
                                Chessbot1.kingmoved = json.loads(kingmoved)
                                Chessbot1.rookmoved = json.loads(rookmoved)
                                Chessbot1.pieces = json.loads(pieces)
                break
    for n2 in range(Chessbot1.pieces[1][0]):
        n = 10+n2
        for i in range(8):
            if n in Chessbot1.board[i]:
                y0 = Chessbot1.board[i].index(n)
                for x1 in range(i-2, i+3):
                    for y1 in range(y0-2, y0+3):
                        if (Chessbot1.piecemove(n, i, y0, x1, y1) 
                            and not Chessbot1.pin(n, i, y0, x1, y1)):
                            evaluation1 = evaluate0(x1, y1)
                            movepieceto(n, i, y0, x1, y1)
                            movescore2 += bmove(n, i, y0, x1, y1) + evaluation1,
                            Chessbot1.turn = (Chessbot1.bot == 0)
                                
                            Chessbot1.moves = moves
                            Chessbot1.enpassant = enpassant
                            Chessbot1.board = json.loads(board)
                            Chessbot1.kingmoved = json.loads(kingmoved)
                            Chessbot1.rookmoved = json.loads(rookmoved)
                            Chessbot1.pieces = json.loads(pieces)
                break
    for n1 in range(2, 6):
        for n2 in range(Chessbot1.pieces[n1][0]):
            n = n1*10+n2
            for i in range(8):
                if n in Chessbot1.board[i]:
                    y0 = Chessbot1.board[i].index(n)
                    for x1 in range(8):
                        for y1 in range(8):
                            if (Chessbot1.piecemove(n, i, y0, x1, y1) 
                                and not Chessbot1.pin(n, i, y0, x1, y1)):
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto(n, i, y0, x1, y1)
                                movescore2 += bmove(n, i, y0, x1, y1) + evaluation1,
                                Chessbot1.turn = (Chessbot1.bot == 0)
                                
                                Chessbot1.moves = moves
                                Chessbot1.enpassant = enpassant
                                Chessbot1.board = json.loads(board)
                                Chessbot1.kingmoved = json.loads(kingmoved)
                                Chessbot1.rookmoved = json.loads(rookmoved)
                                Chessbot1.pieces = json.loads(pieces)
                    break
                                
    Chessbot1.moves = moves
    Chessbot1.enpassant = enpassant
    Chessbot1.board = json.loads(board)
    Chessbot1.kingmoved = json.loads(kingmoved)
    Chessbot1.rookmoved = json.loads(rookmoved)
    Chessbot1.pieces = json.loads(pieces)
    Chessbot1.turn = Chessbot1.bot
    return max(movescore2)+evaluation0

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
                y0 = Chessbot1.board[i].index(n)
                for x1 in range(8):
                    for y1 in range(8):
                        if (Chessbot1.piecemove(n, i, y0, x1, y1) 
                            and not Chessbot1.pin(n, i, y0, x1, y1)):
                            evaluation1 = evaluate0(x1, y1)
                            movepieceto(n, i, y0, x1, y1)
                            Chessbot1.positions += deepcopy(Chessbot1.board)
                            Chessbot1.turn = (Chessbot1.bot == 0)
                            if(Chessbot1.repetition(Chessbot1.moves) or Chessbot1.stalemate(50) or Chessbot1.stalemate(-50)):
                                movescore[0] += [(-fulleval()-(blackmove(n, i, y0, x1, y1) + evaluation1))]
                            elif partialrepetition(Chessbot1.moves):
                                movescore[0] += [min(0, (-fulleval()-(blackmove(n, i, y0, x1, y1) + evaluation1)))]
                            else:
                                movescore[0] += [(blackmove(n, i, y0, x1, y1) + evaluation1)]
                                
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
            break
    end = time.time()
    print(end-start)
    if(max(movescore[0]) >= 500000):
        Chessbot1.evaltext = "M"
        Chessbot1.evalscore = 1000
    elif(max(movescore[0]) <= -500000):
        Chessbot1.evaltext = "-M"
        Chessbot1.evalscore = -1000
    else:
        Chessbot1.evaltext = str(round(fulleval() + max(movescore[0]),2))
        Chessbot1.evalscore = fulleval() + max(movescore[0])
