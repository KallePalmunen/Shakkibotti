#import torch
import math

import random
import Chessbot1
import json

#Neural networks contains values of the nodes, 1 is the piece you want to move and 2 is the square you want to move it to
neural_network1 = [[]]
multiplier1 = [[[]]]
bias1 = [[]]

neural_network2 = [[]]
multiplier2 = [[[]]]
bias2 = [[]]

mode = "play"
nhiddenlayers1 = 4
nhiddenlayers2 = 4
piecepositions = []

for i in range(300):
    piecepositions += [0]

#create neural network 1
for i in range(300):
    neural_network1[0].append(0)

for j in range(nhiddenlayers1):
    neural_network1.append([])
    for i in range(64):
        neural_network1[j+1].append(0)

neural_network1.append([])

for i in range(50):
    neural_network1[nhiddenlayers1+1].append(0)

#create neural network 2

for i in range(350):
    neural_network2[0].append(0)

for j in range(nhiddenlayers2):
    neural_network2.append([])
    for i in range(64):
        neural_network2[j+1].append(0)

neural_network2.append([])

for i in range(64):
    neural_network2[nhiddenlayers2+1].append(0)

def randomize():
    global nhiddenlayers1, nhiddenlayers2
    #First is the first hidden layer, then the second and finally the output layer, network 1
    for i in range(64):
        multiplier1[0].append([])
        bias1[0].append(random.random()-0.5)
        for j in range(300):
            multiplier1[0][i].append(random.random()-0.5)

    for ii in range(nhiddenlayers1-1):    
        multiplier1.append([])
        bias1.append([])
        for i in range(64):
            multiplier1[ii+1].append([])
            bias1[ii+1].append(random.random()-0.5)
            for j in range(64):
                multiplier1[ii+1][i].append(random.random()-0.5)

    multiplier1.append([])
    bias1.append([])
    for i in range(50):
        multiplier1[nhiddenlayers1].append([])
        bias1[nhiddenlayers1].append(random.random()-0.5)
        for j in range(64):
            multiplier1[nhiddenlayers1][i].append(random.random()-0.5)

    #Network 2
    for i in range(64):
        multiplier2[0].append([])
        bias2[0].append(random.random()-0.5)
        for j in range(350):
            multiplier2[0][i].append(random.random()-0.5)

    for ii in range(nhiddenlayers2-1):
        multiplier2.append([])
        bias2.append([])
        for i in range(64):
            multiplier2[ii+1].append([])
            bias2[ii+1].append(random.random()-0.5)
            for j in range(64):
                multiplier2[ii+1][i].append(random.random()-0.5)

    multiplier2.append([])
    bias2.append([])
    for i in range(64):
        multiplier2[nhiddenlayers2].append([])
        bias2[nhiddenlayers2].append(random.random()-0.5)
        for j in range(64):
            multiplier2[nhiddenlayers2][i].append(random.random()-0.5)

def sigmoid(a):
    if a < 0:
        return 1 - 1/(1+math.exp(a))
    return 1/(math.exp(-a)+1)

def leakyrelu(a):
    return max(0.01*a, a)

def calculate1():
    global neural_network1

    for i in range(300):
        neural_network1[0][i] = piecepositions[i]

    #First hidden layer for first network, then second and finally the output layer
    for i in range(64):
        neural_network1[1][i] = 0
        for j in range(300):
            neural_network1[1][i] += neural_network1[0][j]*multiplier1[0][i][j]
        neural_network1[1][i] += bias1[0][i]
        neural_network1[1][i] = sigmoid(neural_network1[1][i])
    
    for i in range(64):
        neural_network1[2][i] = 0
        for j in range(64):
            neural_network1[2][i] += neural_network1[1][j]*multiplier1[1][i][j]
        neural_network1[2][i] += bias1[1][i]
        neural_network1[2][i] = sigmoid(neural_network1[2][i])
    
    for i in range(64):
        neural_network1[3][i] = 0
        for j in range(64):
            neural_network1[3][i] += neural_network1[2][j]*multiplier1[2][i][j]
        neural_network1[3][i] += bias1[2][i]
        neural_network1[3][i] = sigmoid(neural_network1[3][i])
    
    for i in range(64):
        neural_network1[4][i] = 0
        for j in range(64):
            neural_network1[4][i] += neural_network1[3][j]*multiplier1[3][i][j]
        neural_network1[4][i] += bias1[3][i]
        neural_network1[4][i] = sigmoid(neural_network1[4][i])
    
    for i in range(50):
        neural_network1[5][i] = 0
        for j in range(64):
            neural_network1[5][i] += neural_network1[4][j]*multiplier1[4][i][j]
        neural_network1[5][i] += bias1[4][i]
        neural_network1[5][i] = sigmoid(neural_network1[5][i])
    
#Second network


def calculate2(n):
    global neural_network2

    for i in range(300):
        neural_network2[0][i] = piecepositions[i]
    fanboymove = n-1
    for i in range(50):
        neural_network2[0][300+i] = (i == fanboymove)

    for i in range(64):
        neural_network2[1][i] = 0
        for j in range(350):
            neural_network2[1][i] += neural_network2[0][j]*multiplier2[0][i][j]
        neural_network2[1][i] += bias2[0][i]
        neural_network2[1][i] = sigmoid(neural_network2[1][i])
    
    for i in range(64):
        neural_network2[2][i] = 0
        for j in range(64):
            neural_network2[2][i] += neural_network2[1][j]*multiplier2[1][i][j]
        neural_network2[2][i] += bias2[1][i]
        neural_network2[2][i] = sigmoid(neural_network2[2][i])
    
    for i in range(64):
        neural_network2[3][i] = 0
        for j in range(64):
            neural_network2[3][i] += neural_network2[2][j]*multiplier2[2][i][j]
        neural_network2[3][i] += bias2[2][i]
        neural_network2[3][i] = sigmoid(neural_network2[3][i])
        
    for i in range(64):
        neural_network2[4][i] = 0
        for j in range(64):
            neural_network2[4][i] += neural_network2[3][j]*multiplier2[3][i][j]
        neural_network2[4][i] += bias2[3][i]
        neural_network2[4][i] = sigmoid(neural_network2[4][i])
    
    for i in range(64):
        neural_network2[5][i] = 0
        for j in range(64):
            neural_network2[5][i] += neural_network2[4][j]*multiplier2[4][i][j]
        neural_network2[5][i] += bias2[4][i]
        neural_network2[5][i] = sigmoid(neural_network2[5][i])

def convertposition():
    global piecepositions
    for n in range(50):
        for x in range(8):
            if n+1 in Chessbot1.board[x]:
                y = Chessbot1.board[x].index(n+1)
                piecepositions[n*3] = x/7
                piecepositions[n*3+1] = y/7
                piecepositions[n*3+2] = 1
                break
        else:
            #if not found
            piecepositions[n*3] = -1
            piecepositions[n*3+1] = -1
            piecepositions[n*3+2] = 0
    for n in range(50):
        for x in range(8):
            if -n-1 in Chessbot1.board[x]:
                y = Chessbot1.board[x].index(-n-1)
                piecepositions[n*3+150] = x/7
                piecepositions[n*3+151] = y/7
                piecepositions[n*3+152] = 1
                break
        else:
            #if not found
            piecepositions[n*3+150] = -1
            piecepositions[n*3+151] = -1
            piecepositions[n*3+152] = 0
    return piecepositions

randomize()

if mode == "play":
    with open("fanboydata.txt", 'r') as f:
        data = json.loads(f.read())
    bias1 = data[0]
    multiplier1 = data[1]
    bias2 = data[2]
    multiplier2 = data[3]

def move():
    global neural_network1, neural_network2
    calculate1()

    while True:
        calculate2(neural_network1[nhiddenlayers1+1].index(max(neural_network1[nhiddenlayers1+1]))+1)
        for i in range(64):
            n = neural_network1[nhiddenlayers1+1].index(max(neural_network1[nhiddenlayers1+1]))+1
            for j in range(8):
                if n in Chessbot1.board[j]:
                    x0 = j
                    y0 = Chessbot1.board[j].index(n)
                    break
            else:
                x0 = -1
                y0 = -1
            x1 = int(i/8)
            y1 = i-x1*8
            if (not Chessbot1.piecemove(n, x0, y0, x1, y1)) or Chessbot1.pin(n, x0, y0, x1, y1):
                neural_network2[nhiddenlayers2+1][i] = -1000000
        move = neural_network1[nhiddenlayers1+1].index(max(neural_network1[nhiddenlayers1+1]))+1
        if max(neural_network2[nhiddenlayers2+1]) <= -1000000:
            neural_network1[nhiddenlayers1+1][move-1] = -1000000
        else:
            break

    move = neural_network1[nhiddenlayers1+1].index(max(neural_network1[nhiddenlayers1+1]))+1
    movetox = int(neural_network2[nhiddenlayers2+1].index(max(neural_network2[nhiddenlayers2+1]))/8)
    movetoy = neural_network2[nhiddenlayers2+1].index(max(neural_network2[nhiddenlayers2+1]))-movetox*8
    for i in range(8):
        if move in Chessbot1.board[i]:
            if Chessbot1.piecemove(move, i, Chessbot1.board[i].index(move), movetox, movetoy) and not \
                Chessbot1.pin(move, i, Chessbot1.board[i].index(move), movetox, movetoy):
                Chessbot1.movepieceto(move, i, Chessbot1.board[i].index(move), movetox, movetoy)
                Chessbot1.turn = (Chessbot1.bot == 0)
            break