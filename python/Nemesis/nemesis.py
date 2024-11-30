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

class ResNet(nn.Module):
    def __init__(self, game, num_resBlocks, num_hidden, device, number_of_input_channels):
        super().__init__() #initiates the parent class
        
        self.device = device
        self.startBlock = nn.Sequential(
            nn.Conv2d(number_of_input_channels, num_hidden, kernel_size=3, padding=1), #convolutional layer(input channels, output channels, size of layer, match the input size with output size)
            nn.BatchNorm2d(num_hidden), #normalize output
            nn.ReLU()
        )
        
        #create a backbone for neural network by creating multiple Resblocks
        self.backBone = nn.ModuleList(
            [ResBlock(num_hidden) for i in range(num_resBlocks)]
        )
        
        self.policyHead_startsquare = nn.Sequential(
            nn.Conv2d(num_hidden, 32, kernel_size=3, padding=1),
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.Flatten(), #takes each element from a tensor to an array
            nn.Linear(32 * game.row_count * game.column_count, game.action_size) #linear transformation(input size, output size)
        )

        self.policyHead_endsquare = nn.Sequential(
            nn.Conv2d(num_hidden, 32, kernel_size=3, padding=1),
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.Flatten(), #takes each element from a tensor to an array
            nn.Linear(32 * game.row_count * game.column_count, game.action_size) #linear transformation(input size, output size)
        )
        
        self.valueHead = nn.Sequential(
            nn.Conv2d(num_hidden, 3, kernel_size=3, padding=1),
            nn.BatchNorm2d(3),
            nn.ReLU(),
            nn.Flatten(),
            nn.Linear(3 * game.row_count * game.column_count, 1), #tranform to single value
            nn.Tanh() #activation function
        )
        
        self.to(device) #move to the device where you want to run the operations
    
    #iterate through the neural network
    def forward(self, x):
        x = self.startBlock(x)
        for resBlock in self.backBone:
            x = resBlock(x)
        startsquare_policy = self.policyHead_startsquare(x)
        endsquare_policy = self.policyHead_endsquare(x)
        value = self.valueHead(x)
        return startsquare_policy, endsquare_policy, value
        
        
class ResBlock(nn.Module):
    def __init__(self, num_hidden):
        super().__init__()
        self.conv1 = nn.Conv2d(num_hidden, num_hidden, kernel_size=3, padding=1)
        self.bn1 = nn.BatchNorm2d(num_hidden)
        self.conv2 = nn.Conv2d(num_hidden, num_hidden, kernel_size=3, padding=1)
        self.bn2 = nn.BatchNorm2d(num_hidden)
        
    def forward(self, x):
        residual = x
        x = F.relu(self.bn1(self.conv1(x)))
        x = self.bn2(self.conv2(x))
        x += residual
        x = F.relu(x)
        return x

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

class Node:
    def __init__(self, game, args, state, parent=None, startsquare_action_taken=None, endsquare_action_taken=None, probability=0, visit_count=0, depth = 0):
        self.game = game
        self.args = args
        self.state = state
        self.parent = parent
        self.startsquare_action_taken = startsquare_action_taken
        self.endsquare_action_taken = endsquare_action_taken
        self.probability = probability
        
        self.children = []
        
        self.visit_count = visit_count
        self.value_sum = 0
        self.depth = depth
        
    def is_fully_expanded(self):
        return len(self.children) > 0
    
    def select(self):
        best_child = None
        best_ucb = -np.inf
        
        for child in self.children:
            ucb = self.get_ucb(child)
            if ucb > best_ucb:
                best_child = child
                best_ucb = ucb
                
        return best_child
    
    def get_ucb(self, child):
        if child.visit_count == 0:
            q_value = 0
        else:
            q_value = 1 - ((child.value_sum / child.visit_count) + 1) / 2
        return q_value + self.args['C'] * (math.sqrt(self.visit_count) / (child.visit_count + 1)) * child.probability
    
    def expand(self, state, startsquare_policy, endsquare_policy):
        for startsquare_action, startsquare_probability in enumerate(startsquare_policy):
            if startsquare_probability > 0:
                y0 = startsquare_action // self.game.column_count
                x0 = startsquare_action % self.game.column_count
                valid_endsquares = self.game.bot_get_valid_endsquares(state, y0, x0)

                endsquare_policy_for_startsquare = np.copy(endsquare_policy) #create a copy to not affect the original
                endsquare_policy_for_startsquare *= valid_endsquares
                if np.sum(endsquare_policy_for_startsquare) > 0:
                    endsquare_policy_for_startsquare /= np.sum(endsquare_policy_for_startsquare)
                    for endquare_action, endsquare_probability in enumerate(endsquare_policy_for_startsquare):
                        if endsquare_probability > 0:
                            child_state = self.state.copy()
                            child_state = self.game.get_next_state(child_state, startsquare_action, endquare_action, 1)
                            child_state = self.game.change_perspective(child_state, player=-1)
                            
                            probability = startsquare_probability*endsquare_probability

                            child = Node(self.game, self.args, child_state, self, startsquare_action, endquare_action, probability, depth = self.depth + 1)
                            self.children.append(child)
        return child

            
    def backpropagate(self, value):
        self.value_sum += value
        self.visit_count += 1
        
        value = self.game.get_opponent_value(value)
        if self.parent is not None:
            self.parent.backpropagate(value)