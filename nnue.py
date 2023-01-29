import math

import random
import Chessbot1
import json
import magnusfanboy as fan

#Neural networks contains values of the nodes, 1 is the piece you want to move and 2 is the square you want to move it to
neural_network1 = [[]]
multiplier1 = [[[]]]
bias1 = [[]]

mode = "train"
nhiddenlayers1 = 4

fan.convertposition()

#create neural network 1
for i in range(270):
    neural_network1[0].append(0)

for j in range(nhiddenlayers1):
    neural_network1.append([])
    for i in range(64):
        neural_network1[j+1].append(0)

neural_network1.append([])

neural_network1[nhiddenlayers1+1].append(0)

def randomize():
    global nhiddenlayers1
    #First is the first hidden layer, then the second and finally the output layer, network 1
    for i in range(64):
        multiplier1[0].append([])
        bias1[0].append(random.random()-0.5)
        for j in range(270):
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
    multiplier1[nhiddenlayers1].append([])
    bias1[nhiddenlayers1].append(random.random()-0.5)
    for j in range(64):
        multiplier1[nhiddenlayers1][0].append(random.random()-0.5)

def sigmoid(a):
    if a < 0:
        return 1 - 1/(1+math.exp(a))
    return 1/(math.exp(-a)+1)

def leakyrelu(a):
    return max(0.01*a, a)

def calculate1():
    global neural_network1

    for i in range(270):
        neural_network1[0][i] = fan.piecepositions[i]

    #First hidden layer for first network, then second and finally the output layer
    for i in range(64):
        neural_network1[1][i] = 0
        for j in range(270):
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
    
    neural_network1[5][0] = 0
    for j in range(64):
        neural_network1[5][0] += neural_network1[4][j]*multiplier1[4][0][j]
    neural_network1[5][0] += bias1[4][0]
    neural_network1[5][0] = sigmoid(neural_network1[5][0])

randomize()

if mode == "play":
    with open("nnuedata.txt", 'r') as f:
        data = json.loads(f.read())
    bias1 = data[0]
    multiplier1 = data[1]