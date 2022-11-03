#import torch
import math

import random
import Chessbot1

#Neural networks contains values of the nodes, 1 is the piece you want to move and 2 is the square you want to move it to
neural_network1 = [[]]
multiplier1 = [[[]]]
bias1 = [[]]

neural_network2 = [[]]
multiplier2 = [[[]]]
bias2 = [[]]

def randomize():
    #First is the first hidden layer, then the second and finally the output layer, network 1
    for i in range(16):
        multiplier1[0].append([])
        bias1[0].append(random.random()-0.5)
        for j in range(64):
            multiplier1[0][i].append(random.random()-0.5)

    multiplier1.append([])
    bias1.append([])
    for i in range(16):
        multiplier1[1].append([])
        bias1[1].append(random.random()-0.5)
        for j in range(16):
            multiplier1[1][i].append(random.random()-0.5)

    multiplier1.append([])
    bias1.append([])
    for i in range(50):
        multiplier1[2].append([])
        bias1[2].append(random.random()-0.5)
        for j in range(16):
            multiplier1[2][i].append(random.random()-0.5)

    #Network 2
    for i in range(16):
        multiplier2[0].append([])
        bias2[0].append(random.random()-0.5)
        for j in range(114):
            multiplier2[0][i].append(random.random()-0.5)

    multiplier2.append([])
    bias2.append([])
    for i in range(16):
        multiplier2[1].append([])
        bias2[1].append(random.random()-0.5)
        for j in range(16):
            multiplier2[1][i].append(random.random()-0.5)

    multiplier2.append([])
    bias2.append([])
    for i in range(64):
        multiplier2[2].append([])
        bias2[2].append(random.random()-0.5)
        for j in range(16):
            multiplier2[2][i].append(random.random()-0.5)

def sigmoid(a):
    if a < 0:
        return 1 - 1/(1+math.exp(a))
    return 1/(math.exp(-a)+1)

def calculate1():
    global neural_network1
    neural_network1.clear()
    neural_network1 = [[]]
    for i in range(8):
        for j in range(8):
            neural_network1[0].append(Chessbot1.board[i][j])

    neural_network1.append([])

    #First hidden layer for first network, then second and finally the output layer
    for i in range(16):
        neural_network1[1].append(0)
        for j in range(64):
            neural_network1[1][i] += neural_network1[0][j]*multiplier1[0][i][j]
        neural_network1[1][i] += bias1[0][i]
        neural_network1[1][i] = max(0, neural_network1[1][i])

    neural_network1.append([])
    
    for i in range(16):
        neural_network1[2].append(0)
        for j in range(16):
            neural_network1[2][i] += neural_network1[1][j]*multiplier1[1][i][j]
        neural_network1[2][i] += bias1[1][i]
        neural_network1[2][i] = max(0, neural_network1[2][i])
    
    neural_network1.append([])
    
    for i in range(50):
        neural_network1[3].append(0)
        for j in range(16):
            neural_network1[3][i] += neural_network1[2][j]*multiplier1[2][i][j]
        neural_network1[3][i] += bias1[2][i]
        neural_network1[3][i] = sigmoid(neural_network1[3][i])
    
#Second network


def calculate2():
    n = neural_network1[3].index(max(neural_network1[3]))+1
    for i in range(8):
        if n in Chessbot1.board[i]:
            x0 = i
            y0 = Chessbot1.board[i].index(n)
            break
    else:
        x0 = -1
        y0 = -1
    for i in range(8):
        for j in range(8):
            neural_network2[0].append(Chessbot1.board[i][j])
    fanboymove = neural_network1[3].index(max(neural_network1[3]))
    for i in range(50):
        neural_network2[0].append((i == fanboymove))

    neural_network2.append([])

    for i in range(16):
        neural_network2[1].append(0)
        for j in range(114):
            neural_network2[1][i] += neural_network2[0][j]*multiplier2[0][i][j]
        neural_network2[1][i] += bias2[0][i]
        neural_network2[1][i] = max(0, neural_network2[1][i])

    neural_network2.append([])
    
    for i in range(16):
        neural_network2[2].append(0)
        for j in range(16):
            neural_network2[2][i] += neural_network2[1][j]*multiplier2[1][i][j]
        neural_network2[2][i] += bias2[1][i]
        neural_network2[2][i] = max(0, neural_network2[2][i])
    
    neural_network2.append([])
    
    for i in range(64):
        neural_network2[3].append(0)
        for j in range(16):
            neural_network2[3][i] += neural_network2[2][j]*multiplier2[2][i][j]
        neural_network2[3][i] += bias2[2][i]
        neural_network2[3][i] = max(0, neural_network2[3][i])
        x1 = int(i/8)
        y1 = i-x1*8
        if (not Chessbot1.piecemove(n, x0, y0, x1, y1)) or Chessbot1.pin(n, x0, y0, x1, y1):
            neural_network2[3][i] = -1000000

randomize()

def move():
    global neural_network1, neural_network2
    calculate1()    

    while True:
        calculate2()
        move = neural_network1[3].index(max(neural_network1[3]))+1
        if max(neural_network2[3]) <= -1000000:
            neural_network1[3][move-1] = -1000000
            neural_network2.clear()
            neural_network2 = [[]]
        else:
            break

    move = neural_network1[3].index(max(neural_network1[3]))+1
    movetox = int(neural_network2[3].index(max(neural_network2[3]))/8)
    movetoy = neural_network2[3].index(max(neural_network2[3]))-movetox*8
    for i in range(8):
        if move in Chessbot1.board[i]:
            if Chessbot1.piecemove(move, i, Chessbot1.board[i].index(move), movetox, movetoy) and not \
                Chessbot1.pin(move, i, Chessbot1.board[i].index(move), movetox, movetoy):
                Chessbot1.movepieceto(move, i, Chessbot1.board[i].index(move), movetox, movetoy)
                Chessbot1.turn = (Chessbot1.bot == 0)
            break
    neural_network1.clear()
    neural_network1 = [[]]
    neural_network2.clear()
    neural_network2 = [[]]