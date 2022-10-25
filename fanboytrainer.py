from math import cos
from stockfishfanboy import *
import Chessbot1
import random

right_values1 = []
cost_values1 = []
lol=True


for i in range(50):
    right_values1.append(0)
    cost_values1.append(0)

for i in range(50):
    if lol:
        right_values1[i]=True
        lol=False
    else:
        right_values1[i]=False

def backpropagation():
    global neural_network1
    global multiplier1
    global bias1

    calculate1()
    calculate2()

    for i in range(50):
        cost_values1[i]=(right_values1[i]-neural_network1[3][i])**2

backpropagation()