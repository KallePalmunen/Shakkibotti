import sys, pathlib
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent.parent))

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
from Nemesis.move_selection import *

class neuralNetwork(nn.Module):
    def __init__(self, game, numBlocks, num_hidden, device, number_of_inputs, output_size):
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
            nn.Linear(int(num_hidden/2), output_size) #linear transformation(input size, output size)
        )
        
        self.to(device) #move to the device where you want to run the operations
    
    #iterate through the neural network
    def forward(self, x):
        x = self.startBlock(x)
        for block in self.backBone:
            x = block(x)
        policy = self.policyHead(x)
        if policy.dim() == 1:
            policy = policy.unsqueeze(0)
        policy = F.normalize(policy, p=2, dim=1)
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
    def __init__(self, game, number_of_move_vector_components):
        self.game = game
        self.state = game.get_initial_state()
        self.number_of_input_moves = 1
        self.model = neuralNetwork(self.game, 2, 16, device=device, number_of_inputs=number_of_move_vector_components, output_size=2)
        self.previous_move = [0,0]

    def make_move(self, state, botcolor, prev_move):
        self.state = state
        color = int(botcolor == 0) - int(botcolor == 1)

        input_vector = torch.tensor(prev_move, dtype=torch.float32, device=self.model.device)
        out_policy = self.model(input_vector)
        move_vector = out_policy.squeeze(0).detach().cpu().numpy()

        self.previous_move = move_vector

        ### TODO: select move based on move vector
        kingmoved = [0,0]
        rookmoved = [[0,0],[0,0]]
        pieces = []
        enpassant = -1

        move = select_move(move_vector, self.state, color, kingmoved, rookmoved, pieces, enpassant)

        self.state = self.game.get_next_state(self.state, move)

        return self.state
    
def test():
    from SigmaZero.rules import Chess
    import rules_old
    game = Chess()

    botObject = Nemesis(game, number_of_move_vector_components=2)
    state = np.array(rules_old.board)
    state = botObject.make_move(state, 1, [1,0])

    rules_old.turn = (rules_old.turn == 0)
    rules_old.board = state.tolist()
    print(rules_old.board)

if __name__=="__main__":
    test()