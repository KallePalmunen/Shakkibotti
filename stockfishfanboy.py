#import torch
import random

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

randomize()
print(multiplier1)
print(bias1)
print(multiplier2)
print(bias2)