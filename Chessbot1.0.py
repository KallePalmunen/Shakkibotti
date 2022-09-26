
from itertools import filterfalse


board = [[0,10,0,0,0,0,11,0], [1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[0,-10,0,0,0,0,-11,0]]

turn = 0

def printboard():
    for i in range(8):
        print(board[i])

def piecemove(n, x0, y0, x1, y1):
    if x1 < 8 and y1 < 8 and y1 >= 0 and x1 >= 0:
        if abs(n) < 10:
            if pawnmove(n, x0, y0, x1, y1):
                return True
            return False
        if abs(n) < 20 and abs(n) >= 10:
            if knightmove(n, x0, y0, x1, y1):
                return True
            return False
        return False
    return False

def pawnmove(n, x0, y0, x1, y1):
    if n > 0:
        if ((y1 == y0 and (x1-x0 == 1 or (x1-x0 == 2 and x0 == 1 and board[x1-1][y1] == 0)) and board[x1][y1] == 0) or 
            (x1-x0 == 1 and board[x1][y1] < 0 and (y1 - y0 == 1 or y1 - y0 == -1))):
            return True
        return False
    elif n < 0:
        if ((y1 == y0 and (x0-x1 == 1 or (x0-x1 == 2 and x0 == 6 and board[x1+1][y1] == 0)) and board[x1][y1] == 0) or
            (x0-x1 == 1 and board[x1][y1] > 0 and (y1 - y0 == 1 or y1 - y0 == -1))):
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

def movepawn():
    global turn
    move = int(input())
    movetox = int(input())
    movetoy = int(input())
    if (move > 0 and turn == 0) or (move < 0 and turn == 1):
        for i in range(8):
            if move in board[i]:
                if piecemove(move, i, board[i].index(move), movetox, movetoy):
                    piecefile = board[i].index(move)
                    board[i][piecefile] = 0
                    board[movetox][movetoy] = move
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

printboard()
for i in range(100):
    movepawn()

