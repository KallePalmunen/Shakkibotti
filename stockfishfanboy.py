#import torch
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


def calculate():
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

    neural_network1.append([])
    
    for i in range(16):
        neural_network1[2].append(0)
        for j in range(16):
            neural_network1[2][i] += neural_network1[1][j]*multiplier1[1][i][j]
        neural_network1[2][i] += bias1[1][i]
    
    neural_network1.append([])
    
    for i in range(50):
        neural_network1[3].append(0)
        for j in range(16):
            neural_network1[3][i] += neural_network1[2][j]*multiplier1[2][i][j]
        neural_network1[3][i] += bias1[2][i]
    
    #Second network
    ### First layer here

    for i in range(16):
        neural_network2[1].append(0)
        for j in range(114):
            neural_network2[1][i] += neural_network2[0][j]*multiplier2[0][i][j]
        neural_network2[1][i] += bias2[0][i]

    neural_network2.append([])
    
    for i in range(16):
        neural_network1[2].append(0)
        for j in range(16):
            neural_network1[2][i] += neural_network2[1][j]*multiplier2[1][i][j]
        neural_network2[2][i] += bias2[1][i]
    
    neural_network2.append([])
    
    ### Output Layer here


randomize()
calculate()
print(neural_network1)