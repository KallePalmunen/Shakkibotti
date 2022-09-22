
board = [[0,0,0,0,0,0,0,0], [1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[0,0,0,0,0,0,0,0]];

turn = 0;

def printboard():
    for i in range(8):
        print(board[i]);

def movepawn():
    move = int(input());
    global turn;
    if move > 0 and turn == 0:
        for i in range(8):
            if move in board[i]:
                moveto = board[i].index(move);
                board[i][moveto] = 0;
                board[i+1][moveto] = move;
                break
        else:
            print('Illegal move');
            return;
        turn = 1;
    elif move < 0 and turn == 1:
        for i in range(8):
            if move in board[i]:
                moveto = board[i].index(move);
                board[i][moveto] = 0;
                board[i-1][moveto] = move;
                break
        else:
            print('Illegal move');
            return;
        turn = 0;
    else:
        print('Illegal move');
        return;
    printboard();

printboard();
for i in range(100):
    movepawn();

