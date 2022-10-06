
import math
import random
from copy import copy, deepcopy

#Guten tag

board = [[30,10,20,50,40,21,11,31], [1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[-30,-10,-20,-50,-40,-21,-11,-31]]

moves = 0
positions = [[[]]]
positions[0] = deepcopy(board)

turn = 0
enpassant = -1
bot = 2

#pawns, knights, bishops, rooks, queens and kings (W,B)
pieces = [[8,8],[2,2],[2,2],[2,2],[1,1],[1,1]]
#black, white
kingmoved = [0, 0]
#black left, right - white left, right
rookmoved = [[0, 0], [0, 0]]

def printboard():
    print()
    for i in range(8):
        print("{0:5}{1:5}{2:5}{3:5}{4:5}{5:5}{6:5}{7:5}"\
            .format(board[i][0], board[i][1], board[i][2], board[i][3], board[i][4], board[i][5], board[i][6], board[i][7]))

def piecemove(n, x0, y0, x1, y1):
    #checks if a certain piece can move (also there is specific functions for each piece like pawnmove, knightmove etc.)
    #n=unique piece, x0, y0 = starting coordinates, x1, y1 = ending coordinates
    if x1 < 8 and y1 < 8 and y1 >= 0 and x1 >= 0:
        if abs(n) < 10:
            if pawnmove(n, x0, y0, x1, y1):
                return True
            return False
        if abs(n) < 20 and abs(n) >= 10:
            if knightmove(n, x0, y0, x1, y1):
                return True
            return False
        if abs(n) < 30 and abs(n) >= 20:
            if bishopmove(n, x0, y0, x1, y1):
                return True
            return False
        if abs(n) < 40 and abs(n) >= 30:
            if rookmove(n, x0, y0, x1, y1):
                return True
            return False
        if abs(n) < 50 and abs(n) >= 40:
            if queenmove(n, x0, y0, x1, y1):
                return True
            return False
        if abs(n) >= 50:
            if kingmove(n, x0, y0, x1, y1):
                return True
            return False
        return False
    return False

def pawnmove(n, x0, y0, x1, y1):
    if n > 0:
        if ((y1 == y0 and (x1-x0 == 1 or (x1-x0 == 2 and x0 == 1 and board[x1-1][y1] == 0)) and board[x1][y1] == 0) or 
            (x1-x0 == 1 and board[x1][y1] < 0 and (y1 - y0 == 1 or y1 - y0 == -1)) 
            or (y1*8+x1 == enpassant and abs(y1-y0) == 1 and x1 - x0 == 1)):
            return True
        return False
    elif n < 0:
        if ((y1 == y0 and (x0-x1 == 1 or (x0-x1 == 2 and x0 == 6 and board[x1+1][y1] == 0)) and board[x1][y1] == 0) or
            (x0-x1 == 1 and board[x1][y1] > 0 and (y1 - y0 == 1 or y1 - y0 == -1)) 
            or (y1*8+x1 == enpassant and abs(y1-y0) == 1 and x0 - x1 == 1)):
            return True
        return False
    return False

def knightmove(n, x0, y0, x1, y1):
    if ((x1 - x0 == 2 or x1 - x0 == -2) and (y1 - y0 == 1 or y1 - y0 == -1)) or ((x1- x0 == 1 or x1 - x0 == -1) and
        (y1 - y0 == 2 or y1 - y0 == -2)):
        if n > 0 and (board[x1][y1] == 0 or board[x1][y1] < 0):
            return True
        if n < 0 and (board[x1][y1] == 0 or board[x1][y1] > 0):
            return True
        return False
    return False


#longmove checks if there is anything in the way when moving bishops, rooks and queens. It also checks whether there is a piece in the endsquare

def longmove(n, x0, y0, x1, y1):
    if(n > 0):
        for i in range(1, 8):
            if(x0 + math.copysign((x1-x0 != 0),x1-x0)*i == x1 and y0 + 
               math.copysign((y1-y0 != 0),y1-y0)*i == y1 and board[x1][y1] <= 0):
                return True
            if(board[x0 + int(math.copysign((x1-x0 != 0),x1-x0))*i][y0 + int(math.copysign((y1-y0 != 0),y1-y0))*i] != 0):
                return False
        return False
    if(n < 0):
        for i in range(1, 8):
            if(x0 + math.copysign((x1-x0 != 0),x1-x0)*i == x1 and y0 + 
               math.copysign((y1-y0!=0),y1-y0)*i == y1 and board[x1][y1] >= 0):
                return True
            if(board[x0 + int(math.copysign((x1-x0 != 0),x1-x0))*i][y0 + int(math.copysign((y1-y0 != 0),y1-y0))*i] != 0):
                return False
        return False
    return False

def bishopmove(n, x0, y0, x1, y1):
    if(abs(x1-x0) == abs(y1-y0)):
        if longmove(n, x0, y0, x1, y1):
            return True
        return False
    return False

def rookmove(n, x0, y0, x1, y1):
    if (x1-x0 != 0 and y1-y0 == 0) or (x1-x0 == 0 and y1-y0 != 0):
        if longmove(n, x0, y0, x1, y1):
            return True
        return False
    return False

def queenmove(n, x0, y0, x1, y1):
    if bishopmove(n, x0, y0, x1, y1) or rookmove(n, x0, y0, x1, y1):
        return True
    return False

def kingmove(n, x0, y0, x1, y1):
    if castle(n, x0, y0, x1, y1):
        return True
    if abs(x1-x0) <= 1 and abs(y1-y0) <= 1 and not (x1 == x0 and y1 == y0):
        if n > 0 and board[x1][y1] <= 0:
            return True
        if n < 0 and board[x1][y1] >= 0:
            return True
        return False
    return False

def promote(n, x1):
    if n < 10 and n > 0:
        if x1 == 7:
            return True
    if n > -10 and n < 0:
        if x1 == 0:
            return True

def checkmate(n):
    if not check(n):
        return False
    if n > 0:
        for i in range(1, 51):
            for ii in range(8):
                if i in board[ii]:
                    piecex0 = ii
                    piecey0 = board[ii].index(i)
                    for j in range(8):
                        for jj in range(8):
                            if piecemove(i, piecex0, piecey0, j, jj):
                                board[piecex0][piecey0] = 0
                                movetosquare = board[j][jj]
                                board[j][jj] = i
                                if not check(n):
                                    board[piecex0][piecey0] = i
                                    board[j][jj] = movetosquare
                                    return False
                                board[piecex0][piecey0] = i
                                board[j][jj] = movetosquare
                    break
        return True
    if n < 0:
        for i in range(1, 51):
            for ii in range(8):
                if -i in board[ii]:
                    piecex0 = ii
                    piecey0 = board[ii].index(-i)
                    for j in range(8):
                        for jj in range(8):
                            if piecemove(-i, piecex0, piecey0, j, jj):
                                board[piecex0][piecey0] = 0
                                movetosquare = board[j][jj]
                                board[j][jj] = -i
                                if not check(n):
                                    board[piecex0][piecey0] = -i
                                    board[j][jj] = movetosquare
                                    return False
                                board[piecex0][piecey0] = -i
                                board[j][jj] = movetosquare
                    break
        return True


def check(n):
    if n < 0:
        for i in range(8):
            if n in board[i]:
                kingx = i
                kingy = board[i].index(n)
                break
        else:
            return True
        for i in range(1, 50):
            for ii in range(8):
                if i in board[ii]:
                    if piecemove(i, ii, board[ii].index(i), kingx, kingy):
                        return True
                    break
        return False
    if n > 0:
        for i in range(8):
            if n in board[i]:
                kingx = i
                kingy = board[i].index(n)
                break
        else:
            return True
        for i in range(1, 50):
            for ii in range(8):
                if -i in board[ii]:
                    if piecemove(-i, ii, board[ii].index(-i), kingx, kingy):
                        return True
                    break
        return False
    return False

def pin(n, x0, y0, x1, y1):
    board[x0][y0] = 0
    movetosquare = board[x1][y1]
    board[x1][y1] = n
    if not check(int(math.copysign(50,n))):
        board[x0][y0] = n
        board[x1][y1] = movetosquare
        return False
    board[x0][y0] = n
    board[x1][y1] = movetosquare
    return True

def castle(n, x0, y0, x1, y1):
    if(kingmoved[(n > 0)] == 1) or rookmoved[(n > 0)][(y1 > 4)]:
        return False
    if(x1 == x0 and (y1 == 1 or y1 == 5) and not check(n)) and board[x0][(y1 > 4)*7] == int(math.copysign(30 + (y1 > 4), n)):
        for i in range(1, 3 + (y1 > 4)):
            if board[x0][y0 + i*int(math.copysign(1, y1-y0))] != 0:
                return False
            board[x0][y0] = 0
            board[x0][y0 + i*int(math.copysign(1, y1-y0))] = n
            if i < 3 and check(n):
                board[x0][y0] = n
                board[x0][y0 + i*int(math.copysign(1, y1-y0))] = 0
                return False
            board[x0][y0] = n
            board[x0][y0 + i*int(math.copysign(1, y1-y0))] = 0
        return True
    return False

def movesomewhere(n, x0, y0):
    for i in range(8):
        for ii in range(8):
            if piecemove(n, x0, y0, i, ii) and not pin(n, x0, y0, i, ii):
                return True
    return False

def stalemate(n):
    if check(n):
        return False
    for i in range(1, 51):
        for ii in range(8):
            if int(math.copysign(i, n)) in board[ii]:
                if movesomewhere(int(math.copysign(i, n)), ii, board[ii].index(int(math.copysign(i, n)))):
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
        if compareposition(i):
            repetitions += 1
            if(repetitions >= 2):
                return True
    return False

def movepieceto(n, x0, y0, x1, y1):
    global turn
    global moves
    global enpassant
    if(abs(n) == 50):
        if abs(y1 - y0) > 1:
            whichrook = int(math.copysign(30 + (y1 > 4), n))
            board[x1][board[x1].index(whichrook)] = 0
            board[x1][y1 + int(math.copysign(1, 4-y1))] = whichrook
        kingmoved[(n > 0)] = 1
    board[x0][y0] = 0
    if(abs(n) == 30 or abs(n) == 31):
        rookmoved[(n >0)][abs(n)-30] = 1
    if promote(n, x1):
        print('Type 1 for knight, 2 for bishope, 3 for rook, 4 for queen')
        try:
            promoteto = int(input())
        except ValueError:
            print('Invalid input, promoting to queen')
            promoteto = 4
        board[x1][y1] = int(math.copysign(1, n))*(promoteto*10+pieces[promoteto][(n < 0)])
        pieces[promoteto][(n < 0)] += 1
    else:
        board[x1][y1] = n
    if enpassant >= 0 and y1*8+x1 == enpassant:
        board[x1-int(math.copysign(1, x1 - x0))][y1] = 0
    if abs(n) < 10 and abs(x1 - x0) > 1:
        enpassant = y1*8+x0+int(math.copysign(1, x1 - x0))
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
    printboard()

def botmove():
    global turn
    while turn == bot:
        move = int(random.random()*51*math.copysign(1, 0.5-bot))
        movetox = int(random.random()*8)
        movetoy = int(random.random()*8)
        for i in range(8):
            if move in board[i]:
                if (piecemove(move, i, board[i].index(move), movetox, movetoy) 
                    and not pin(move, i, board[i].index(move), movetox, movetoy)):
                    movepieceto(move, i, board[i].index(move), movetox, movetoy)
                    turn = (bot == 0)
                    printboard()
                    return
                else:
                    break

printboard()

print('Type 1 to play as white, 0 to play as black, or 2 for player vs player')
try:
    bot = int(input())
except ValueError:
    bot = 2

if not(bot == 0 or bot == 1):
    bot = 2

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

while turn >= 0:
    if turn == bot:
        botmove()
    gameend()
    if turn != bot:
        movepiece()
    gameend()

input()
