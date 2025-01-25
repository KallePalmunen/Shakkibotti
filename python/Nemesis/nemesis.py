from SigmaZero.rules import *
import numpy as np
print(np.__version__)
import torch
print(torch.__version__)
import torch.nn as nn
import torch.nn.functional as F
import random
import math
import time

class neuralNetwork(nn.Module):
    def __init__(self, game, numBlocks, num_hidden, device, number_of_inputs):
        super().__init__() #initiates the parent class
        
        self.device = device
        self.startBlock = nn.Sequential(
            nn.Linear(number_of_inputs, num_hidden),
            nn.PReLU()
        )
        
        #create a backbone for neural network by creating multiple blocks
        self.backBone = nn.ModuleList(
            [block(num_hidden) for i in range(numBlocks)]
        )
        
        self.policyHead = nn.Sequential(
            nn.Linear(num_hidden, int(num_hidden/2)),
            nn.PReLU(),
            nn.Linear(int(num_hidden/2), game.action_size) #linear transformation(input size, output size)
        )
        
        self.to(device) #move to the device where you want to run the operations
    
    #iterate through the neural network
    def forward(self, x):
        x = self.startBlock(x)
        for block in self.backBone:
            x = block(x)
        policy = self.policyHead(x)
        return policy

class block(nn.Module):
    def __init__(self, num_hidden):
        super().__init__()
        self.layer = nn.Sequential(
            nn.Linear(num_hidden, num_hidden),
            nn.PReLU()
        )
        
    def forward(self, x):
        x = self.layer(x)
        return x

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

class Nemesis:
    def __init__(self, game, state):
        self.game = game
        self.state = state