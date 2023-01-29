import math
import nnue
import Chessbot1
import random
import json
import time
import magnusfanboy as fan

right_value = 0
multipliergradient1 = [[[]]]
biasgradient1 = [[]]
activations1 = [[]]
currentpiece = 0
#stochastic gradient descent sample size
samplesize = 10
learningrate = 0.1

with open("nnuetrainingdata.txt", 'r') as f:
    data = json.loads(f.read())

#multipliers go from j to i

for i in range(64):
    multipliergradient1[0].append([])
    biasgradient1[0].append(0)
    for j in range(270):
        multipliergradient1[0][i].append(0)
        activations1[0].append(0)

for ii in range(nnue.nhiddenlayers1-1):
    multipliergradient1.append([])
    biasgradient1.append([])
    activations1.append([])
    for i in range(64):
        multipliergradient1[ii+1].append([])
        biasgradient1[ii+1].append(0)
        for j in range(64):
            multipliergradient1[ii+1][i].append(0)
            activations1[ii+1].append(0)

multipliergradient1.append([])
biasgradient1.append([])
activations1.append([])

multipliergradient1[nnue.nhiddenlayers1].append([])
biasgradient1[nnue.nhiddenlayers1].append(0)
for j in range(64):
    multipliergradient1[nnue.nhiddenlayers1][0].append(0)
    activations1[nnue.nhiddenlayers1].append(0)

#derivative of leaky relu
def dleak(a):
    if a <= 0:
        return 0.01
    return 1

#derivative of sigmoid
def dsigmoid(a):
    return math.exp(a)/((math.exp(a)+1)**2)


def backpropagation1():

    nnue.calculate1()

    totalcost = (right_value-nnue.neural_network1[5][0])**2

    for j in range(64):
        multipliergradient1[4][0][j] += -2*(right_value-nnue.neural_network1[5][0])*nnue.neural_network1[4][j]\
            *dsigmoid(totalcost)
    biasgradient1[4][0] += -2*(right_value-nnue.neural_network1[5][0])*dsigmoid(totalcost)
    
    for j in range(64):
        activations1[4][j] = 0
        activations1[4][j] += -2*(right_value-nnue.neural_network1[5][0])*nnue.multiplier1[4][0][j]\
            *dsigmoid(totalcost)

    for i in range(64):
        for j in range(64):
            multipliergradient1[3][i][j] += activations1[4][i]*nnue.neural_network1[3][j]\
                *dsigmoid(nnue.neural_network1[4][i])
        biasgradient1[3][i] += activations1[4][i]*dsigmoid(nnue.neural_network1[4][i])

    for j in range(64):
        activations1[3][j] = 0
        for i in range(64):
            activations1[3][j] += activations1[4][i]*nnue.multiplier1[3][i][j]\
                *dsigmoid(nnue.neural_network1[4][i])

    for i in range(64):
        for j in range(64):
            multipliergradient1[2][i][j] += activations1[3][i]*nnue.neural_network1[2][j]\
                *dsigmoid(nnue.neural_network1[3][i])
        biasgradient1[2][i] += activations1[3][i]*dsigmoid(nnue.neural_network1[3][i])

    for j in range(64):
        activations1[2][j] = 0
        for i in range(64):
            activations1[2][j] += activations1[3][i]*nnue.multiplier1[2][i][j]\
                *dsigmoid(nnue.neural_network1[3][i])
    
    for i in range(64):
        for j in range(64):
            multipliergradient1[1][i][j] += activations1[2][i]*nnue.neural_network1[1][j]\
                *dsigmoid(nnue.neural_network1[2][i])
        biasgradient1[1][i] += activations1[2][i]*dsigmoid(nnue.neural_network1[2][i])

    for j in range(64):
        activations1[1][j] = 0
        for i in range(64):
            activations1[1][j] += activations1[2][i]*nnue.multiplier1[1][i][j]\
                *dsigmoid(nnue.neural_network1[2][i])
    
    for i in range(64):
        for j in range(270):
            multipliergradient1[0][i][j] += activations1[1][i]*nnue.multiplier1[0][i][j]\
                *dsigmoid(nnue.neural_network1[1][i])
        biasgradient1[0][i] += activations1[1][i]*dsigmoid(nnue.neural_network1[1][i])
    return totalcost

def updateneuralnetwork1():
    global samplesize, learningrate

    for j in range(64):
        nnue.multiplier1[4][0][j] -= learningrate*multipliergradient1[4][0][j]/samplesize
        multipliergradient1[4][0][j] = 0
    nnue.bias1[4][0] -= learningrate*biasgradient1[4][0]/samplesize
    biasgradient1[4][0] = 0

    for i in range(64):
        for j in range(64):
            nnue.multiplier1[3][i][j] -= learningrate*multipliergradient1[3][i][j]/samplesize
            multipliergradient1[3][i][j] = 0
        nnue.bias1[3][i] -= learningrate*biasgradient1[2][i]/samplesize
        biasgradient1[3][i] = 0

    for i in range(64):
        for j in range(64):
            nnue.multiplier1[2][i][j] -= learningrate*multipliergradient1[2][i][j]/samplesize
            multipliergradient1[2][i][j] = 0
        nnue.bias1[2][i] -= learningrate*biasgradient1[2][i]/samplesize
        biasgradient1[2][i] = 0

    for i in range(64):
        for j in range(64):
            nnue.multiplier1[1][i][j] -= learningrate*multipliergradient1[1][i][j]/samplesize
            multipliergradient1[1][i][j] = 0
        nnue.bias1[1][i] -= learningrate*biasgradient1[1][i]/samplesize
        biasgradient1[1][i] = 0
    
    for i in range(64):
        for j in range(270):
            nnue.multiplier1[0][i][j] -= learningrate*multipliergradient1[0][i][j]/samplesize
            multipliergradient1[0][i][j] = 0
        nnue.bias1[0][i] -= learningrate*biasgradient1[0][i]/samplesize
        biasgradient1[0][i] = 0

def setrightvalue1(rand):
    global right_value
    right_value = data[rand][1]
    fan.piecepositions = data[rand][0]

datalen = len(data)

times = 10000

start = time.time()
cost = 0
for i in range(times):
    setrightvalue1(int(random.random()*datalen))
    cost += backpropagation1()
    if (i+1)%samplesize == 0:
        updateneuralnetwork1()
    if (i+1)%1000 == 0:
        print(i+1)
        print(cost)
        cost = 0

print("done")
end = time.time()
print(end-start)

with open("nnuedata.txt", 'w') as f:
    f.write(f"[{str(nnue.bias1)}, {str(nnue.multiplier1)}]")