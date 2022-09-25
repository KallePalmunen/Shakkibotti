
board = [[0,0,0,0,0,0,0,0], [1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[0,0,0,0,0,0,0,0]];

turn = 0;

def printboard():
    for i in range(8):
        print(board[i]);

def pawnmove(a, b, c, d):

    if b > 0:
        if c < 8 and (c-a == 1 or (c-a == 2 and a == 1)) and board[c][d] == 0:
            return True;
        return False;
    elif b < 0:
        if c >= 0 and (a-c == 1 or (a-c == 2 and a == 6)) and board[c][d] == 0:
            return True;
        return False;
    return False;

def movepawn():
    global turn;
    move = int(input());
    moveto = int(input());
    if (move > 0 and turn == 0) or (move < 0 and turn == 1):
        for i in range(8):
            if move in board[i]:
                if pawnmove(i, move, moveto, board[i].index(move)):
                    piecefile = board[i].index(move);
                    board[i][piecefile] = 0;
                    board[moveto][piecefile] = move;
                    break
                else:
                    print('Illegal move');
                    return;
        else:
            print('Illegal move');
            return;
        if turn == 0:
            turn = 1;
        else:
            turn = 0;
    else:
        print('Illegal move');
        return;
    printboard();

printboard();
for i in range(100):
    movepawn();

