import Chessbot1
import basicbot as bot
import magnusfanboy as fan
from copy import deepcopy, copy
import time
import json

with open("nnuetrainingdata.txt", 'r') as f:
    data = json.loads(f.read())

def reset():
    Chessbot1.board = [[30,10,20,50,40,21,11,31], [1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[-30,-10,-20,-50,-40,-21,-11,-31]]
    Chessbot1.moves = 0
    Chessbot1.positions = [[[]]]
    Chessbot1.positions[0] = deepcopy(Chessbot1.board)
    Chessbot1.turn = 0
    Chessbot1.enpassant = -1
    Chessbot1.pieces = [[8,8],[2,2],[2,2],[2,2],[1,1],[1,1]]
    Chessbot1.kingmoved = [0, 0]
    Chessbot1.rookmoved = [[0, 0], [0, 0]]
    Chessbot1.bot = 0

start = time.time()
for i in range(1000):
    Chessbot1.bot = 0
    Chessbot1.turn = 0
    score = min(max(bot.fulleval() + bot.whitemove()[5],-100),100)
    fan.convertposition()
    data += [[fan.piecepositions, (score+100)/200]]
    Chessbot1.turn = 0
    Chessbot1.randommove()
    Chessbot1.bot = 1
    if Chessbot1.turn == -1:
        reset()
    else:
        Chessbot1.randommove()
        Chessbot1.gameend()
    if Chessbot1.turn == -1:
        reset()
    if((i+1)%10 == 0):
        print(i+1)

end = time.time()
with open("nnuetrainingdata.txt", 'w') as f:
    f.write(str(data))

print(end-start)
print("done")