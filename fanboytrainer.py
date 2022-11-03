import math
import stockfishfanboy as fan
import Chessbot1
import random

right_values1 = []
cost_values1 = []
multipliergradient1 = [[[]]]
biasgradient1 = [[]]
multipliergradient2 = [[[]]]
biasgradient2 = [[]]
activations1 = [[]]
activations2 = [[]]

lol=True

#multipliers go from j to i

for i in range(16):
    multipliergradient1[0].append([])
    biasgradient1[0].append(0)
    activations1[0].append(0)
    for j in range(64):
        multipliergradient1[0][i].append(0)

multipliergradient1.append([])
biasgradient1.append([])
activations1.append([])
for i in range(16):
    multipliergradient1[1].append([])
    biasgradient1[1].append(0)
    activations1[1].append(0)
    for j in range(16):
        multipliergradient1[1][i].append(0)

multipliergradient1.append([])
biasgradient1.append([])
activations1.append([])
for i in range(50):
    multipliergradient1[2].append([])
    biasgradient1[2].append(0)
    activations1[2].append(0)
    for j in range(16):
        multipliergradient1[2][i].append(0)

#Network 2
for i in range(16):
    multipliergradient2[0].append([])
    biasgradient2[0].append(0)
    activations2[0].append(0)
    for j in range(114):
        multipliergradient2[0][i].append(0)

multipliergradient2.append([])
biasgradient2.append([])
activations2.append([])
for i in range(16):
    multipliergradient2[1].append([])
    biasgradient2[1].append(0)
    activations2[1].append(0)
    for j in range(16):
        multipliergradient2[1][i].append(0)

multipliergradient2.append([])
biasgradient2.append([])
activations2.append([])
for i in range(64):
    multipliergradient2[2].append([])
    biasgradient2[2].append(0)
    activations2[2].append(0)
    for j in range(16):
        multipliergradient2[2][i].append(0)

for i in range(50):
    right_values1.append(0)
    cost_values1.append(0)

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


def backpropagation():

    fan.calculate1()

    totalcost = 0
    for i in range(50):
        cost_values1[i]=1/50*(right_values1[i]-fan.neural_network1[3][i])**2
        totalcost += cost_values1[i]

    for i in range(50):
        for j in range(16):
            multipliergradient1[2][i][j] = -2*(right_values1[i]-fan.neural_network1[3][i])*fan.neural_network1[2][j]\
                *dsigmoid(cost_values1[i])/50
            fan.multiplier1[2][i][j] -= multipliergradient1[2][i][j]
        biasgradient1[2][i] = -2*(right_values1[i]-fan.neural_network1[3][i])*dsigmoid(cost_values1[i])/50
        fan.bias1[2][i] -= biasgradient1[2][i]
    
    for j in range(16):
        activations1[2][j] = 0
        for i in range(50):
            activations1[2][j] += -2*(right_values1[i]-fan.neural_network1[3][i])*fan.multiplier1[2][i][j]\
                *dsigmoid(cost_values1[i])/50

    for i in range(16):
        for j in range(16):
            multipliergradient1[1][i][j] = activations1[2][i]*fan.neural_network1[1][j]\
                *dleak(fan.neural_network1[2][i])
            fan.multiplier1[1][i][j] -= multipliergradient1[1][i][j]
        biasgradient1[1][i] = activations1[2][i]*dleak(fan.neural_network1[2][i])
        fan.bias1[1][i] -= biasgradient1[1][i]

    for j in range(16):
        activations1[1][j] = 0
        for i in range(16):
            activations1[1][j] += activations1[2][i]*fan.multiplier1[1][i][j]\
                *dleak(fan.neural_network1[2][i])
    
    for i in range(16):
        for j in range(64):
            multipliergradient1[0][i][j] = activations1[1][i]*fan.multiplier1[0][i][j]\
                *dleak(fan.neural_network1[1][i])
            fan.multiplier1[0][i][j] -= multipliergradient1[0][i][j]
        biasgradient1[1][i] = activations1[1][i]*dleak(fan.neural_network1[1][i])
        fan.bias1[0][i] -= biasgradient1[0][i]

    print(totalcost)




for i in range(10):
    backpropagation()

print(fan.neural_network1[3])