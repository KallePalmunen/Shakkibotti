import rules_old
import random
import math
from copy import copy, deepcopy
import time
import json


movescore = [[]]
for i in range(3):
    movescore.append([])

def getpinners(kingx, kingy):
    pinners = []
    board = str(rules_old.board)
    for n1 in range(5):
        for n2 in range(rules_old.pieces[n1][1]):
            n = n1*10+n2
            for x in range(8):
                if -n in rules_old.board[x]:
                    rules_old.board[x][rules_old.board[x].index(-n)] = 0
                    break
    for n1 in range(6):
        for n2 in range(rules_old.pieces[n1][0]):
            n = n1*10+n2
            for x in range(8):
                if n in rules_old.board[x]:
                    if rules_old.piecemove(n, x, rules_old.board[x].index(n), kingx, kingy):
                        pinners += n,
                    break
    rules_old.board = json.loads(board)
    return pinners


def movepieceto(n, x0, y0, x1, y1):
    if(abs(n) == 50):
        if abs(y1 - y0) > 1:
            whichrook = int(math.copysign(30 + (y1 > 4), n))
            rules_old.board[x1][rules_old.board[x1].index(whichrook)] = 0
            rules_old.board[x1][y1 + int(math.copysign(1, 4-y1))] = whichrook
        rules_old.kingmoved[(n > 0)] = 1
    rules_old.board[x0][y0] = 0
    if(abs(n) == 30 or abs(n) == 31):
        rules_old.rookmoved[(n >0)][abs(n)-30] = 1
    if rules_old.promote(n, x1):
        promoteto = 4
        rules_old.board[x1][y1] = int(math.copysign(1, n))*(promoteto*10+rules_old.pieces[promoteto][(n < 0)])
        rules_old.pieces[promoteto][(n < 0)] += 1
    else:
        rules_old.board[x1][y1] = n
    if rules_old.enpassant >= 0 and y1*8+x1 == rules_old.enpassant:
        rules_old.board[x1-int(math.copysign(1, x1 - x0))][y1] = 0
    if abs(n) < 10 and abs(x1 - x0) > 1:
        rules_old.enpassant = y1*8+x0+int(math.copysign(1, x1 - x0))
    else:
        rules_old.enpassant = -1
    rules_old.moves += 1

def check(kingx, kingy, pinners):
    for j in range(len(pinners)):
        i = pinners[j]
        for ii in range(8):
            if i in rules_old.board[ii]:
                if rules_old.piecemove(i, ii, rules_old.board[ii].index(i), kingx, kingy):
                    return True
                break
    return False

def partialpin(n,x0,y0,x1,y1,kingx,kingy, pinners):
    rules_old.board[x0][y0] = 0
    movetosquare = rules_old.board[x1][y1]
    rules_old.board[x1][y1] = n
    if not check(kingx,kingy,pinners):
        rules_old.board[x0][y0] = n
        rules_old.board[x1][y1] = movetosquare
        return False
    rules_old.board[x0][y0] = n
    rules_old.board[x1][y1] = movetosquare
    return True

def pinnable(n,x0,y0,kingx,kingy,pinners):
    if pinners == []:
        return False
    rules_old.board[x0][y0] = 0
    if not check(kingx,kingy,pinners):
        rules_old.board[x0][y0] = n
        return False
    rules_old.board[x0][y0] = n
    return True

def movepieceto2(n, x0, y0, x1, y1):
    rules_old.board[x0][y0] = 0
    if rules_old.promote(n, x1):
        rules_old.board[x1][y1] = int(math.copysign(1, n))*(40+rules_old.pieces[4][(n < 0)])
    else:
        rules_old.board[x1][y1] = n

def partialrepetition(m):
    for i in range(m):
        if rules_old.compareposition(i) and i%2 == m%2:
            return True
    return False

def fulleval():
    evaluation = 0
    for n in range(1, 51):
        for x in range(8):
            if n in rules_old.board[x]:
                evaluation += evaluate2(n, x, rules_old.board[x].index(n))
                break
    for n in range(1, 51):
        for x in range(8):
            if -n in rules_old.board[x]:
                evaluation += evaluate2(-n, x, rules_old.board[x].index(-n))
                break
    return evaluation


def evaluate(n, x0, y0, x1, y1):
    return evaluate2(n, x1, y1)+evaluate1(n,x0,y0)

def evaluate2(n, x1, y1):
    n1 = rules_old.board[min(7, x1 + 1)][min(7, y1+1)]*(x1 != 7 and\
        y1 != 7)
    n2 = rules_old.board[min(7, x1 + 1)][max(0, y1-1)]*(x1 != 7 and\
        y1 != 0)
    evaluation = math.copysign((n != 0 and abs(n) < 9)*(1+(abs(x1 - 7*(n < 0))*((y1 > 2 and y1 < 6)*((y1 == 5)*0.01+(y1 != 5)*0.05) + 0.001)\
                - (rules_old.moves < 60 and y1 < 2)*(abs(x1 - 7*(n < 0)) > 2)*0.03))\
                + 3*(abs(n) > 9 and abs(n) < 30) + \
                (5 + (not ((x1 == 0 or x1 == 7) and (y1 == 0 or y1 == 7)))*0.01)\
                *(abs(n) > 29 and abs(n) < 40) + \
                9*(abs(n) > 39)+ ((abs(n) >= 10 and abs(n) < 30) or (abs(n) >= 40 and abs(n) < 50))*(x1 != 7*(n < 0))*(0.05+0.001*(abs(x1 - 7*(n < 0)) > 1))\
                + (abs(n) >= 10 and abs(n) < 20 and y1 > 1 and y1 < 6)*0.01\
                + ((abs(n) == 50) and (x1 == 0 or x1 == 7) and (y1 == 1 or y1 == 5))*0.05, n)\
                *(1-((n > 0) and ((n1 < 0 and n1 >-9) or (n2 < 0 and n2 >-9)))*0.5)
    return evaluation

def evaluate1(n, x0, y0):
    n1 = rules_old.board[min(7, x0 + 1)][min(7, y0+1)]*(x0 != 7 and\
        y0 != 7)
    n2 = rules_old.board[min(7, x0 + 1)][max(0, y0-1)]*(x0 != 7 and\
        y0 != 0)
    evaluation = -math.copysign((n != 0 and abs(n) < 9)*(1+(abs(x0 - 7*(n < 0))*((y0 > 2 and y0 < 6)*((y0 == 5)*0.01+(y0 != 5)*0.05) + 0.001)\
                - (rules_old.moves < 60 and y0 < 2)*(abs(x0 - 7*(n < 0)) > 2)*0.03))\
                + 3*(abs(n) > 9 and abs(n) < 30) + \
                (5+ (not ((x0 == 0 or x0 == 7) and (y0 == 0 or y0 == 7)))*0.01)\
                *(abs(n) > 29 and abs(n) < 40) + \
                9*(abs(n) > 39)+ ((abs(n) >= 10 and abs(n) < 30) or (abs(n) >= 40 and abs(n) < 50))*(x0 != 7*(n < 0))*(0.05+0.001*(abs(x0 - 7*(n < 0)) > 1))\
                + (abs(n) >= 10 and abs(n) < 20 and y0 > 1 and y0 < 6)*0.01\
                + ((abs(n) == 50) and (x0 == 0 or x0 == 7) and (y0 == 1 or y0 == 5))*0.05, n)\
                *(1-((n > 0) and ((n1 < 0 and n1 >-9) or (n2 < 0 and n2 >-9)))*0.5)
    return evaluation

def evaluate0(x0, y0):
    n = rules_old.board[x0][y0]
    #n1 and n2 are squares where a threatening pawn could be
    n1 = rules_old.board[min(7, x0 + 1)][min(7, y0+1)]*(x0 != 7 and\
        y0 != 7)
    n2 = rules_old.board[min(7, x0 + 1)][max(0, y0-1)]*(x0 != 7 and\
        y0 != 0)
    return -math.copysign((n != 0 and abs(n) < 9)*(1+(abs(x0 - 7*(n < 0))*((y0 > 2 and y0 < 6)*((y0 == 5)*0.01+(y0 != 5)*0.05) + 0.001)\
                - (rules_old.moves < 60 and y0 < 2)*(abs(x0 - 7*(n < 0)) > 2)*0.03))\
                + 3*(abs(n) > 9 and abs(n) < 30) + \
                (5+ (not ((x0 == 0 or x0 == 7) and (y0 == 0 or y0 == 7)))*0.01)\
                *(abs(n) > 29 and abs(n) < 40) + \
                9*(abs(n) > 39)+ ((abs(n) >= 10 and abs(n) < 30) or (abs(n) >= 40 and abs(n) < 50))*(x0 != 7*(n < 0))*(0.05+0.001*(abs(x0 - 7*(n < 0)) > 1))\
                + (abs(n) >= 10 and abs(n) < 20 and y0 > 1 and y0 < 6)*0.01\
                + ((abs(n) == 50) and (x0 == 0 or x0 == 7) and (y0 == 1 or y0 == 5))*0.05, n)\
                *(1-((n > 0) and ((n1 < 0 and n1 >-9) or (n2 < 0 and n2 >-9)))*0.5)

def reorder():
    moves = rules_old.moves
    enpassant = rules_old.enpassant
    board = str(rules_old.board)
    kingmoved = str(rules_old.kingmoved)
    rookmoved = str(rules_old.rookmoved)
    pieces = str(rules_old.pieces)
    positions = str(rules_old.positions)
    movescore0 = []
    preorder = []
    for n in range(1, 51):
        for i in range(8):
            if n in rules_old.board[i]:
                y0 = rules_old.board[i].index(n)
                for x1 in range(8):
                    for y1 in range(8):
                        if (rules_old.piecemove(n, i, y0, x1, y1) 
                            and not rules_old.pin(n, i, y0, x1, y1)):
                            evaluation1 = evaluate0(x1, y1)
                            movepieceto(n, i, y0, x1, y1)
                            rules_old.positions += deepcopy(rules_old.board)
                            rules_old.turn = (rules_old.bot == 0)
                            if(rules_old.repetition(rules_old.moves) or rules_old.stalemate(50) or rules_old.stalemate(-50)):
                                movescore0 += [(-fulleval()-(evaluate(n, i, y0, x1, y1) + evaluation1))]
                            elif partialrepetition(rules_old.moves):
                                movescore0 += [min(0, (-fulleval()-(evaluate(n, i, y0, x1, y1) + evaluation1)))]
                            else:
                                movescore0 += [(evaluate(n, i, y0, x1, y1) + evaluation1)]
                            preorder += [n, i, y0, x1, y1],
                                
                            rules_old.moves = moves
                            rules_old.enpassant = enpassant
                            rules_old.board = json.loads(board)
                            rules_old.kingmoved = json.loads(kingmoved)
                            rules_old.rookmoved = json.loads(rookmoved)
                            rules_old.pieces = json.loads(pieces)
                            rules_old.positions = json.loads(positions)
                break
    rules_old.turn = rules_old.bot
    order = [[0,0,0,0,0]]
    for i in range(len(movescore0)):
        order += preorder[movescore0.index(max(movescore0))],
        movescore0[movescore0.index(max(movescore0))] = -1000000
    return order
    

def blackmove(n01, x01, y01, x11, y11, best):
    if rules_old.checkmate(-50):
        return 500000
    moves = rules_old.moves
    enpassant = rules_old.enpassant
    board = str(rules_old.board)
    kingmoved = str(rules_old.kingmoved)
    rookmoved = str(rules_old.rookmoved)
    pieces = str(rules_old.pieces)
    movescore1 = [1000000]
    evaluation0 = evaluate(n01, x01, y01, x11, y11)
    for n2 in range(rules_old.pieces[1][1]):
        n = 10+n2
        for i in range(8):
            if -n in rules_old.board[i]:
                y0 = rules_old.board[i].index(-n)
                for x1 in range(i-2, i+3):
                    for y1 in range(y0-2, y0+3):
                        if (rules_old.piecemove(-n, i, y0, x1, y1) 
                            and not rules_old.pin(-n, i, y0, x1, y1)):
                            evaluation1 = evaluate0(x1, y1)
                            movepieceto(-n, i, y0, x1, y1)
                            holder = wmove(-n, i, y0, x1, y1, min(movescore1)-evaluation1) + evaluation1
                            movescore1 += holder,
                            if holder + evaluation0 <= best:
                                return holder + evaluation0
                            rules_old.turn = rules_old.bot
                                
                            rules_old.moves = moves
                            rules_old.enpassant = enpassant
                            rules_old.board = json.loads(board)
                            rules_old.kingmoved = json.loads(kingmoved)
                            rules_old.rookmoved = json.loads(rookmoved)
                            rules_old.pieces = json.loads(pieces)
    for n1 in range(2, 6):
        for n2 in range(rules_old.pieces[n1][1]):
            n = n1*10+n2
            for i in range(8):
                if -n in rules_old.board[i]:
                    y0 = rules_old.board[i].index(-n)
                    for x1 in range(8):
                        for y1 in range(8):
                            if (rules_old.piecemove(-n, i, y0, x1, y1) 
                                and not rules_old.pin(-n, i, y0, x1, y1)):
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto(-n, i, y0, x1, y1)
                                holder = wmove(-n, i, y0, x1, y1, min(movescore1)-evaluation1) + evaluation1
                                movescore1 += holder,
                                if holder + evaluation0 <= best:
                                    return holder + evaluation0
                                rules_old.turn = rules_old.bot
                                
                                rules_old.moves = moves
                                rules_old.enpassant = enpassant
                                rules_old.board = json.loads(board)
                                rules_old.kingmoved = json.loads(kingmoved)
                                rules_old.rookmoved = json.loads(rookmoved)
                                rules_old.pieces = json.loads(pieces)
                    break
    for n in range(1, 9):
        for i in range(1, 8):
            if -n in rules_old.board[i]:
                y0 = rules_old.board[i].index(-n)
                for x1 in range(i - 2, i):
                    for y1 in range(y0 - 1, y0+2):
                        if (rules_old.piecemove(-n, i, y0, x1, y1) 
                            and not rules_old.pin(-n, i, y0, x1, y1)):
                            evaluation1 = evaluate0(x1, y1)
                            movepieceto(-n, i, y0, x1, y1)
                            holder = wmove(-n, i, y0, x1, y1, min(movescore1)-evaluation1) + evaluation1
                            movescore1 += holder,
                            if holder + evaluation0 <= best:
                                return holder + evaluation0
                            rules_old.turn = rules_old.bot
                                
                            rules_old.moves = moves
                            rules_old.enpassant = enpassant
                            rules_old.board = json.loads(board)
                            rules_old.kingmoved = json.loads(kingmoved)
                            rules_old.rookmoved = json.loads(rookmoved)
                            rules_old.pieces = json.loads(pieces)
                break
                                
    rules_old.moves = moves
    rules_old.enpassant = enpassant
    rules_old.board = json.loads(board)
    rules_old.kingmoved = json.loads(kingmoved)
    rules_old.rookmoved = json.loads(rookmoved)
    rules_old.pieces = json.loads(pieces)
    rules_old.turn = (rules_old.bot == 0)
    return min(movescore1)+evaluation0

def bmove(n01, x01, y01, x11, y11, best):
    if rules_old.checkmate(-50):
        return 250000
    if rules_old.stalemate(-50):
        return 0
    movescore3 = [1000000]
    evaluation0 = evaluate(n01, x01, y01, x11, y11)
    for i in range(8):
        if -50 in rules_old.board[i]:
            kingx = i
            kingy = rules_old.board[i].index(-50)
            break
    pinners = getpinners(kingx, kingy)
    for n2 in range(rules_old.pieces[1][1]):
        n = 10+n2
        for i in range(8):
            if -n in rules_old.board[7-i]:
                x0 = 7-i
                y0 = rules_old.board[x0].index(-n)
                evaluation2 = evaluate1(-n, x0, y0)
                if not pinnable(-n, x0, y0, kingx, kingy, pinners):
                    for x1 in range(x0-2, x0+3):
                        for y1 in range(y0-2, y0+3):
                            if (rules_old.piecemove(-n, x0, y0, x1, y1)):
                                wastheren = rules_old.board[x1][y1]
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto2(-n, x0, y0, x1, y1)
                                holder = evaluate2(-n, x1, y1) + evaluation1 + evaluation2
                                movescore3 += holder,
                                if holder + evaluation0 <= best:
                                    return holder + evaluation0
                                rules_old.turn = rules_old.bot
                                    
                                rules_old.board[x1][y1] = wastheren
                                rules_old.board[x0][y0] = -n
                else:
                    for x1 in range(x0-2, x0+3):
                        for y1 in range(y0-2, y0+3):
                            if (rules_old.piecemove(-n, x0, y0, x1, y1) 
                                and not partialpin(-n, x0, y0, x1, y1, kingx, kingy, pinners)):
                                wastheren = rules_old.board[x1][y1]
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto2(-n, x0, y0, x1, y1)
                                holder = evaluate2(-n, x1, y1) + evaluation1 + evaluation2
                                movescore3 += holder,
                                if holder + evaluation0 <= best:
                                    return holder + evaluation0
                                rules_old.turn = rules_old.bot
                                    
                                rules_old.board[x1][y1] = wastheren
                                rules_old.board[x0][y0] = -n
                break
    for n1 in range(2, 5):
        for n2 in range(rules_old.pieces[n1][1]):
            n = n1*10+n2
            for i in range(8):
                if -n in rules_old.board[7-i]:
                    x0 = 7-i
                    y0 = rules_old.board[x0].index(-n)
                    evaluation2 = evaluate1(-n, x0, y0)
                    if not pinnable(-n, x0, y0, kingx, kingy, pinners):
                        for x1 in range(8):
                            for y1 in range(8):
                                if (rules_old.piecemove(-n, x0, y0, x1, y1)):
                                    wastheren = rules_old.board[x1][y1]
                                    evaluation1 = evaluate0(x1, y1)
                                    movepieceto2(-n, x0, y0, x1, y1)
                                    if rules_old.checkmate(50):
                                        return -250000
                                    holder = evaluate2(-n, x1, y1) + evaluation1 + evaluation2
                                    movescore3 += holder,
                                    if holder + evaluation0 <= best:
                                        return holder + evaluation0
                                    rules_old.turn = rules_old.bot
                                    
                                    rules_old.board[x1][y1] = wastheren
                                    rules_old.board[x0][y0] = -n
                    else:
                        for x1 in range(8):
                            for y1 in range(8):
                                if (rules_old.piecemove(-n, x0, y0, x1, y1) 
                                    and not partialpin(-n, x0, y0, x1, y1, kingx, kingy, pinners)):
                                    wastheren = rules_old.board[x1][y1]
                                    evaluation1 = evaluate0(x1, y1)
                                    movepieceto2(-n, x0, y0, x1, y1)
                                    if rules_old.checkmate(50):
                                        return -250000
                                    holder = evaluate2(-n, x1, y1) + evaluation1 + evaluation2
                                    movescore3 += holder,
                                    if holder + evaluation0 <= best:
                                        return holder + evaluation0
                                    rules_old.turn = rules_old.bot
                                    
                                    rules_old.board[x1][y1] = wastheren
                                    rules_old.board[x0][y0] = -n
                    break
    for n in range(1,9):
        for i in range(8):
            if -n in rules_old.board[7-i]:
                x0 = 7-i
                y0 = rules_old.board[x0].index(-n)
                evaluation2 = evaluate1(-n, x0, y0)
                if not pinnable(-n, x0, y0, kingx, kingy, pinners):
                    for x1 in range(x0 - 2, x0):
                        for y1 in range(y0 - 1, y0+2):
                            if (rules_old.piecemove(-n, x0, y0, x1, y1)):
                                wastheren = rules_old.board[x1][y1]
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto2(-n, x0, y0, x1, y1)
                                holder = evaluate2(-n, x1, y1) + evaluation1 + evaluation2
                                movescore3 += holder,
                                if holder + evaluation0 <= best:
                                    return holder + evaluation0
                                rules_old.turn = rules_old.bot
                                
                                rules_old.board[x1][y1] = wastheren
                                rules_old.board[x0][y0] = -n
                else: 
                    for x1 in range(x0 - 2, x0):
                        for y1 in range(y0 - 1, y0+2):
                            if (rules_old.piecemove(-n, x0, y0, x1, y1) 
                                and not partialpin(-n, x0, y0, x1, y1, kingx, kingy, pinners)):
                                wastheren = rules_old.board[x1][y1]
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto2(-n, x0, y0, x1, y1)
                                holder = evaluate2(-n, x1, y1) + evaluation1 + evaluation2
                                movescore3 += holder,
                                if holder + evaluation0 <= best:
                                    return holder + evaluation0
                                rules_old.turn = rules_old.bot
                                    
                                rules_old.board[x1][y1] = wastheren
                                rules_old.board[x0][y0] = -n
                            
                break
    n = 50
    for i in range(8):
        if -n in rules_old.board[7-i]:
            x0 = 7-i
            y0 = rules_old.board[x0].index(-n)
            evaluation2 = evaluate1(-n, x0, y0)
            for x1 in range(x0-1, x0+2):
                for y1 in range(y0-1,y0+2):
                    if (rules_old.piecemove(-n, x0, y0, x1, y1) 
                        and not rules_old.pin(-n, x0, y0, x1, y1)):
                        wastheren = rules_old.board[x1][y1]
                        evaluation1 = evaluate0(x1, y1)
                        movepieceto2(-n, x0, y0, x1, y1)
                        if rules_old.checkmate(50):
                            return -250000
                        holder = evaluate2(-n, x1, y1) + evaluation1 + evaluation2
                        movescore3 += holder,
                        if holder + evaluation0 <= best:
                            return holder + evaluation0
                        rules_old.turn = rules_old.bot
                        
                        rules_old.board[x1][y1] = wastheren
                        rules_old.board[x0][y0] = -n
            break
    rules_old.turn = (rules_old.bot == 0)
    return min(movescore3)+evaluation0

def wmove(n01, x01, y01, x11, y11, best):
    if rules_old.checkmate(50):
        return -500000
    moves = rules_old.moves
    enpassant = rules_old.enpassant
    board = str(rules_old.board)
    kingmoved = str(rules_old.kingmoved)
    rookmoved = str(rules_old.rookmoved)
    pieces = str(rules_old.pieces)
    movescore2 = [-1000000]
    evaluation0 = evaluate(n01, x01, y01, x11, y11)
    for n2 in range(rules_old.pieces[1][0]):
        n = 10+n2
        for i in range(8):
            if n in rules_old.board[i]:
                y0 = rules_old.board[i].index(n)
                for x1 in range(i-2, i+3):
                    for y1 in range(y0-2, y0+3):
                        if (rules_old.piecemove(n, i, y0, x1, y1) 
                            and not rules_old.pin(n, i, y0, x1, y1)):
                            evaluation1 = evaluate0(x1, y1)
                            movepieceto(n, i, y0, x1, y1)
                            holder = bmove(n, i, y0, x1, y1, max(movescore2)-evaluation1) + evaluation1
                            movescore2 += holder,
                            if holder + evaluation0 >= best:
                                return holder + evaluation0
                            rules_old.turn = (rules_old.bot == 0)
                                
                            rules_old.moves = moves
                            rules_old.enpassant = enpassant
                            rules_old.board = json.loads(board)
                            rules_old.kingmoved = json.loads(kingmoved)
                            rules_old.rookmoved = json.loads(rookmoved)
                            rules_old.pieces = json.loads(pieces)
                break
    for n1 in range(2, 6):
        for n2 in range(rules_old.pieces[n1][0]):
            n = n1*10+n2
            for i in range(8):
                if n in rules_old.board[i]:
                    y0 = rules_old.board[i].index(n)
                    for x1 in range(8):
                        for y1 in range(8):
                            if (rules_old.piecemove(n, i, y0, x1, y1) 
                                and not rules_old.pin(n, i, y0, x1, y1)):
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto(n, i, y0, x1, y1)
                                holder = bmove(n, i, y0, x1, y1, max(movescore2)-evaluation1) + evaluation1
                                movescore2 += holder,
                                if holder + evaluation0 >= best:
                                    return holder + evaluation0
                                rules_old.turn = (rules_old.bot == 0)
                                
                                rules_old.moves = moves
                                rules_old.enpassant = enpassant
                                rules_old.board = json.loads(board)
                                rules_old.kingmoved = json.loads(kingmoved)
                                rules_old.rookmoved = json.loads(rookmoved)
                                rules_old.pieces = json.loads(pieces)
                    break
    for n in range(1, 9):
        for i in range(8):
            if n in rules_old.board[i]:
                y0 = rules_old.board[i].index(n)
                for x1 in range(i, 8):
                    if x1 - i <= 2:
                        for y1 in range(8):
                            if (rules_old.piecemove(n, i, y0, x1, y1) 
                                and not rules_old.pin(n, i, y0, x1, y1)):
                                evaluation1 = evaluate0(x1, y1)
                                movepieceto(n, i, y0, x1, y1)
                                holder = bmove(n, i, y0, x1, y1, max(movescore2)-evaluation1) + evaluation1
                                movescore2 += holder,
                                if holder + evaluation0 >= best:
                                    return holder + evaluation0
                                rules_old.turn = (rules_old.bot == 0)
                                
                                rules_old.moves = moves
                                rules_old.enpassant = enpassant
                                rules_old.board = json.loads(board)
                                rules_old.kingmoved = json.loads(kingmoved)
                                rules_old.rookmoved = json.loads(rookmoved)
                                rules_old.pieces = json.loads(pieces)
                break
                                
    rules_old.moves = moves
    rules_old.enpassant = enpassant
    rules_old.board = json.loads(board)
    rules_old.kingmoved = json.loads(kingmoved)
    rules_old.rookmoved = json.loads(rookmoved)
    rules_old.pieces = json.loads(pieces)
    rules_old.turn = rules_old.bot
    return max(movescore2)+evaluation0

def whitemove():
    #save current state
    moves = rules_old.moves
    enpassant = rules_old.enpassant
    board = str(rules_old.board)
    kingmoved = str(rules_old.kingmoved)
    rookmoved = str(rules_old.rookmoved)
    pieces = str(rules_old.pieces)
    positions = str(rules_old.positions)
    movescore0 = [-1000000]
    order = reorder()
    for i in range(len(order)-1):
        n = order[i+1][0]
        x0 = order[i+1][1]
        y0 = order[i+1][2]
        x1 = order[i+1][3]
        y1 = order[i+1][4]
        evaluation1 = evaluate0(x1, y1)
        movepieceto(n, x0, y0, x1, y1)
        rules_old.positions += deepcopy(rules_old.board)
        rules_old.turn = (rules_old.bot == 0)
        if(rules_old.repetition(rules_old.moves) or rules_old.stalemate(50) or rules_old.stalemate(-50)):
            movescore0 += [(-fulleval()-(blackmove(n, x0, y0, x1, y1, max(movescore0)-evaluation1) + evaluation1))]
        elif partialrepetition(rules_old.moves):
            movescore0 += [min(0, (-fulleval()-(blackmove(n, x0, y0, x1, y1, max(movescore0)-evaluation1) + evaluation1)))]
        else:
            movescore0 += [(blackmove(n, x0, y0, x1, y1, max(movescore0)-evaluation1) + evaluation1)]
            
        rules_old.moves = moves
        rules_old.enpassant = enpassant
        rules_old.board = json.loads(board)
        rules_old.kingmoved = json.loads(kingmoved)
        rules_old.rookmoved = json.loads(rookmoved)
        rules_old.pieces = json.loads(pieces)
        rules_old.positions = json.loads(positions)
    rules_old.turn = rules_old.bot
    index = movescore0.index(max(movescore0))
    return [order[index][0], order[index][1], order[index][2], order[index][3], order[index][4], movescore0[index]]

def basicbot():
    start = time.time()
    score = fulleval()
    bestmove = whitemove()
    score += bestmove[5]
    move = bestmove[0]
    x0 = bestmove[1]
    y0 = bestmove[2]
    movetox = bestmove[3]
    movetoy = bestmove[4]
    rules_old.movepieceto(move, x0, y0, movetox, movetoy)
    rules_old.turn = (rules_old.bot == 0)
    end = time.time()
    print(end-start)
    if(score >= 200000):
        rules_old.evaltext = "M"
        print(score)
        rules_old.evalscore = 1000
    elif(score <= -200000):
        rules_old.evaltext = "-M"
        rules_old.evalscore = -1000
    else:
        rules_old.evaltext = str(round(score,2))
        rules_old.evalscore = score
