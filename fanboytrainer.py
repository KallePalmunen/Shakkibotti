import math
import stockfishfanboy as fan
import Chessbot1
import random
import json

right_values1 = []
cost_values1 = []
multipliergradient1 = [[[]]]
biasgradient1 = [[]]
multipliergradient2 = [[[]]]
biasgradient2 = [[]]
activations1 = [[]]
activations2 = [[]]

with open("pgndata.txt", 'r') as f:
    data = json.loads(f.read())

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
                *dsigmoid(totalcost)/50
            fan.multiplier1[2][i][j] -= multipliergradient1[2][i][j]
        biasgradient1[2][i] = -2*(right_values1[i]-fan.neural_network1[3][i])*dsigmoid(totalcost)/50
        fan.bias1[2][i] -= biasgradient1[2][i]
    
    for j in range(16):
        activations1[2][j] = 0
        for i in range(50):
            activations1[2][j] += -2*(right_values1[i]-fan.neural_network1[3][i])*fan.multiplier1[2][i][j]\
                *dsigmoid(totalcost)/50

    for i in range(16):
        for j in range(16):
            multipliergradient1[1][i][j] = activations1[2][i]*fan.neural_network1[1][j]\
                *dsigmoid(fan.neural_network1[2][i])
            fan.multiplier1[1][i][j] -= multipliergradient1[1][i][j]
        biasgradient1[1][i] = activations1[2][i]*dsigmoid(fan.neural_network1[2][i])
        fan.bias1[1][i] -= biasgradient1[1][i]

    for j in range(16):
        activations1[1][j] = 0
        for i in range(16):
            activations1[1][j] += activations1[2][i]*fan.multiplier1[1][i][j]\
                *dsigmoid(fan.neural_network1[2][i])
    
    for i in range(16):
        for j in range(64):
            multipliergradient1[0][i][j] = activations1[1][i]*fan.multiplier1[0][i][j]\
                *dsigmoid(fan.neural_network1[1][i])
            fan.multiplier1[0][i][j] -= multipliergradient1[0][i][j]
        biasgradient1[1][i] = activations1[1][i]*dsigmoid(fan.neural_network1[1][i])
        fan.bias1[0][i] -= biasgradient1[0][i]

def setrightvalue(rand):
    v = data[rand][0]-1
    Chessbot1.board = data[rand][1]

    for i in range(50):
        right_values1[i]=False
    right_values1[v] = True

datalen = len(data)

for i in range(10001):
    setrightvalue(int(random.random()*datalen))
    backpropagation()
    if i%1000 == 0:
        print(i)

with open("fanboydata.txt", 'w') as f:
    f.write(f"[{str(fan.bias1)}, {str(fan.multiplier1)}, {str(fan.bias2)}, {str(fan.multiplier2)}]")