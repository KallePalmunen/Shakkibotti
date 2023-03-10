import math
import magnusfanboy as fan
import Chessbot1
import random
import json
import time

right_values1 = []
cost_values1 = []
right_values2 = []
cost_values2 = []
multipliergradient1 = [[[]]]
biasgradient1 = [[]]
multipliergradient2 = [[[]]]
biasgradient2 = [[]]
activations1 = [[]]
activations2 = [[]]
currentpiece = 0
batchsize = 10
learningrate = 0.1

with open("pgndata.txt", 'r') as f:
    data = json.loads(f.read())

lol=True

#multipliers go from j to i

for i in range(64):
    multipliergradient1[0].append([])
    biasgradient1[0].append(0)
    for j in range(270):
        multipliergradient1[0][i].append(0)
        activations1[0].append(0)

for ii in range(fan.nhiddenlayers1-1):
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
for i in range(50):
    multipliergradient1[fan.nhiddenlayers1].append([])
    biasgradient1[fan.nhiddenlayers1].append(0)
    for j in range(64):
        multipliergradient1[fan.nhiddenlayers1][i].append(0)
        activations1[fan.nhiddenlayers1].append(0)

#Network 2
for i in range(64):
    multipliergradient2[0].append([])
    biasgradient2[0].append(0)
    for j in range(320):
        multipliergradient2[0][i].append(0)
        activations2[0].append(0)

for ii in range(fan.nhiddenlayers2-1):
    multipliergradient2.append([])
    biasgradient2.append([])
    activations2.append([])
    for i in range(64):
        multipliergradient2[ii+1].append([])
        biasgradient2[ii+1].append(0)
        for j in range(64):
            multipliergradient2[ii+1][i].append(0)
            activations2[ii+1].append(0)

multipliergradient2.append([])
biasgradient2.append([])
activations2.append([])
for i in range(64):
    multipliergradient2[fan.nhiddenlayers2].append([])
    biasgradient2[fan.nhiddenlayers2].append(0)
    for j in range(64):
        multipliergradient2[fan.nhiddenlayers2][i].append(0)
        activations2[fan.nhiddenlayers2].append(0)

for i in range(50):
    right_values1.append(0)
    cost_values1.append(0)

for i in range(64):
    right_values2 += [0]
    cost_values2 += [0]

for i in range(50):
    if lol:
        right_values1[i]=True
        lol=False
    else:
        right_values1[i]=False

#derivative of leaky relu
def dleak(a):
    if a <= 0:
        return 0.01
    return 1

#derivative of sigmoid
def dsigmoid(a):
    return math.exp(a)/((math.exp(a)+1)**2)


def backpropagation1():

    fan.calculate1()

    totalcost = 0
    for i in range(50):
        cost_values1[i]=1/50*(right_values1[i]-fan.neural_network1[5][i])**2
        totalcost += cost_values1[i]

    for i in range(50):
        for j in range(64):
            multipliergradient1[4][i][j] += -2*(right_values1[i]-fan.neural_network1[5][i])*fan.neural_network1[4][j]\
                *dsigmoid(totalcost)/50
        biasgradient1[4][i] += -2*(right_values1[i]-fan.neural_network1[5][i])*dsigmoid(totalcost)/50
    
    for j in range(64):
        activations1[4][j] = 0
        for i in range(50):
            activations1[4][j] += -2*(right_values1[i]-fan.neural_network1[5][i])*fan.multiplier1[4][i][j]\
                *dsigmoid(totalcost)/50

    for i in range(64):
        for j in range(64):
            multipliergradient1[3][i][j] += activations1[4][i]*fan.neural_network1[3][j]\
                *dsigmoid(fan.neural_network1[4][i])
        biasgradient1[3][i] += activations1[4][i]*dsigmoid(fan.neural_network1[4][i])

    for j in range(64):
        activations1[3][j] = 0
        for i in range(64):
            activations1[3][j] += activations1[4][i]*fan.multiplier1[3][i][j]\
                *dsigmoid(fan.neural_network1[4][i])

    for i in range(64):
        for j in range(64):
            multipliergradient1[2][i][j] += activations1[3][i]*fan.neural_network1[2][j]\
                *dsigmoid(fan.neural_network1[3][i])
        biasgradient1[2][i] += activations1[3][i]*dsigmoid(fan.neural_network1[3][i])

    for j in range(64):
        activations1[2][j] = 0
        for i in range(64):
            activations1[2][j] += activations1[3][i]*fan.multiplier1[2][i][j]\
                *dsigmoid(fan.neural_network1[3][i])
    
    for i in range(64):
        for j in range(64):
            multipliergradient1[1][i][j] += activations1[2][i]*fan.neural_network1[1][j]\
                *dsigmoid(fan.neural_network1[2][i])
        biasgradient1[1][i] += activations1[2][i]*dsigmoid(fan.neural_network1[2][i])

    for j in range(64):
        activations1[1][j] = 0
        for i in range(64):
            activations1[1][j] += activations1[2][i]*fan.multiplier1[1][i][j]\
                *dsigmoid(fan.neural_network1[2][i])
    
    for i in range(64):
        for j in range(270):
            multipliergradient1[0][i][j] += activations1[1][i]*fan.multiplier1[0][i][j]\
                *dsigmoid(fan.neural_network1[1][i])
        biasgradient1[0][i] += activations1[1][i]*dsigmoid(fan.neural_network1[1][i])
    return totalcost

def updateneuralnetwork1():
    global batchsize, learningrate

    for i in range(50):
        for j in range(64):
            fan.multiplier1[4][i][j] -= learningrate*multipliergradient1[4][i][j]/batchsize
            multipliergradient1[4][i][j] = 0
        fan.bias1[4][i] -= learningrate*biasgradient1[4][i]/batchsize
        biasgradient1[4][i] = 0

    for i in range(64):
        for j in range(64):
            fan.multiplier1[3][i][j] -= learningrate*multipliergradient1[3][i][j]/batchsize
            multipliergradient1[3][i][j] = 0
        fan.bias1[3][i] -= learningrate*biasgradient1[2][i]/batchsize
        biasgradient1[3][i] = 0

    for i in range(64):
        for j in range(64):
            fan.multiplier1[2][i][j] -= learningrate*multipliergradient1[2][i][j]/batchsize
            multipliergradient1[2][i][j] = 0
        fan.bias1[2][i] -= learningrate*biasgradient1[2][i]/batchsize
        biasgradient1[2][i] = 0

    for i in range(64):
        for j in range(64):
            fan.multiplier1[1][i][j] -= learningrate*multipliergradient1[1][i][j]/batchsize
            multipliergradient1[1][i][j] = 0
        fan.bias1[1][i] -= learningrate*biasgradient1[1][i]/batchsize
        biasgradient1[1][i] = 0
    
    for i in range(64):
        for j in range(270):
            fan.multiplier1[0][i][j] -= learningrate*multipliergradient1[0][i][j]/batchsize
            multipliergradient1[0][i][j] = 0
        fan.bias1[0][i] -= learningrate*biasgradient1[0][i]/batchsize
        biasgradient1[0][i] = 0


def backpropagation2():
    global currentpiece

    fan.calculate2(currentpiece)

    totalcost = 0
    for i in range(64):
        cost_values2[i]=(1/64)*(right_values2[i]-fan.neural_network2[5][i])**2
        totalcost += cost_values2[i]

    for i in range(64):
        for j in range(64):
            multipliergradient2[4][i][j] += -2*(right_values2[i]-fan.neural_network2[5][i])*fan.neural_network2[4][j]\
                *dsigmoid(totalcost)/64
        biasgradient2[4][i] += -2*(right_values2[i]-fan.neural_network2[5][i])*dsigmoid(totalcost)/64
    
    for j in range(64):
        activations2[4][j] = 0
        for i in range(64):
            activations2[4][j] += -2*(right_values2[i]-fan.neural_network2[5][i])*fan.multiplier2[4][i][j]\
                *dsigmoid(totalcost)/64
                
    for i in range(64):
        for j in range(64):
            multipliergradient2[3][i][j] += activations2[4][i]*fan.neural_network2[3][j]\
                *dsigmoid(fan.neural_network2[4][i])
        biasgradient2[3][i] += activations2[4][i]*dsigmoid(fan.neural_network2[3][i])

    for j in range(64):
        activations2[3][j] = 0
        for i in range(64):
            activations2[3][j] += activations2[4][i]*fan.multiplier2[3][i][j]\
                *dsigmoid(fan.neural_network2[4][i])
    
    for i in range(64):
        for j in range(64):
            multipliergradient2[2][i][j] += activations2[3][i]*fan.neural_network2[2][j]\
                *dsigmoid(fan.neural_network2[3][i])
        biasgradient2[2][i] += activations2[3][i]*dsigmoid(fan.neural_network2[2][i])

    for j in range(64):
        activations2[2][j] = 0
        for i in range(64):
            activations2[2][j] += activations2[3][i]*fan.multiplier2[2][i][j]\
                *dsigmoid(fan.neural_network2[3][i])

    for i in range(64):
        for j in range(64):
            multipliergradient2[1][i][j] += activations2[2][i]*fan.neural_network2[1][j]\
                *dsigmoid(fan.neural_network2[2][i])
        biasgradient2[1][i] += activations2[2][i]*dsigmoid(fan.neural_network2[2][i])

    for j in range(64):
        activations2[1][j] = 0
        for i in range(64):
            activations2[1][j] += activations2[2][i]*fan.multiplier2[1][i][j]\
                *dsigmoid(fan.neural_network2[2][i])
    
    for i in range(64):
        for j in range(320):
            multipliergradient2[0][i][j] += activations2[1][i]*fan.multiplier2[0][i][j]\
                *dsigmoid(fan.neural_network2[1][i])
        biasgradient2[0][i] += activations2[1][i]*dsigmoid(fan.neural_network2[1][i])
    return totalcost
    
def updateneuralnetwork2():
    global batchsize, learningrate

    for i in range(64):
        for j in range(64):
            fan.multiplier2[4][i][j] -= learningrate*multipliergradient2[4][i][j]/batchsize
            multipliergradient2[4][i][j] = 0
        fan.bias2[4][i] -= learningrate*biasgradient2[4][i]/batchsize
        biasgradient2[4][i] = 0

    for i in range(64):
        for j in range(64):
            fan.multiplier2[3][i][j] -= learningrate*multipliergradient2[3][i][j]/batchsize
            multipliergradient2[3][i][j] = 0
        fan.bias2[3][i] -= learningrate*biasgradient2[3][i]/batchsize
        biasgradient2[3][i] = 0

    for i in range(64):
        for j in range(64):
            fan.multiplier2[2][i][j] -= learningrate*multipliergradient2[2][i][j]/batchsize
            multipliergradient2[2][i][j] = 0
        fan.bias2[2][i] -= learningrate*biasgradient2[2][i]/batchsize
        biasgradient2[2][i] = 0

    for i in range(64):
        for j in range(64):
            fan.multiplier2[1][i][j] -= learningrate*multipliergradient2[1][i][j]/batchsize
            multipliergradient2[1][i][j] = 0
        fan.bias2[1][i] -= learningrate*biasgradient2[1][i]/batchsize
        biasgradient2[1][i] = 0
    
    for i in range(64):
        for j in range(320):
            fan.multiplier2[0][i][j] -= learningrate*multipliergradient2[0][i][j]/batchsize
            multipliergradient2[0][i][j] = 0
        fan.bias2[0][i] -= learningrate*biasgradient2[0][i]/batchsize
        biasgradient2[0][i] = 0

def setrightvalue1(rand):
    v = data[rand][0]-1
    fan.piecepositions = data[rand][1]
    for i in range(50):
        right_values1[i]=0
    right_values1[v] = 1

def setrightvalue2(rand):
    global currentpiece

    x1 = data[rand][2]
    y1 = data[rand][3]
    square = 8*x1+y1
    fan.piecepositions = data[rand][1]
    currentpiece = data[rand][0]

    for i in range(64):
        right_values2[i]=0
    right_values2[square] = 1

datalen = len(data)

times = 100000

start = time.time()
cost = 0
for i in range(times):
    setrightvalue1(int(random.random()*datalen))
    cost += backpropagation1()
    if (i+1)%batchsize == 0:
        updateneuralnetwork1()
    if (i+1)%1000 == 0:
        print(i+1)
        print(cost)
        cost = 0

cost = 0
for i in range(times):
    setrightvalue2(int(random.random()*datalen))
    cost += backpropagation2()
    if (i+1)%batchsize == 0:
        updateneuralnetwork2()
    if (i+1)%1000 == 0:
        print(i+1)
        print(cost)
        cost = 0

print("done")
end = time.time()
print(end-start)

with open("fanboydata.txt", 'w') as f:
    f.write(f"[{str(fan.bias1)}, {str(fan.multiplier1)}, {str(fan.bias2)}, {str(fan.multiplier2)}]")