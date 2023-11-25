import numpy as np
print(np.__version__)

import torch
print(torch.__version__)

import torch.nn as nn
import torch.nn.functional as F

torch.manual_seed(0) #set the same seed for pytorch every time to ensure reproducibility

from tqdm.notebook import trange

import random
import math
import time
from Chessbot1 import piecemove

class TicTacToe:
    def __init__(self): #run when class is initiated
        self.row_count = 8
        self.column_count = 8
        self.action_size = self.row_count * self.column_count
        
    def __repr__(self): #string representation of this class
        return "TicTacToe"
        
    def get_initial_state(self):
        return np.zeros((self.row_count, self.column_count))
    
    def get_next_state(self, state, action, player):
        row = action // self.column_count
        column = action % self.column_count
        state[row, column] = player
        return state
    
    def get_valid_moves(self, state):
        #flattens state into a 1D array, and returns a new array of 8-bit unsigned integers 
        #where each element is 1 if the corresponding element in the original array was 0, and 0 otherwise
        return (state.reshape(-1) == 0).astype(np.uint8)
    
    def check_win(self, state, action):
        if action == None:
            return False
        
        row = action // self.column_count
        column = action % self.column_count
        player = state[row, column]
        
        return (
            np.sum(state[row, :]) == player * self.column_count
            or np.sum(state[:, column]) == player * self.row_count
            or np.sum(np.diag(state)) == player * self.row_count
            or np.sum(np.diag(np.flip(state, axis=0))) == player * self.row_count
        )
    
    def get_value_and_terminated(self, state, action):
        if self.check_win(state, action):
            return 1, True
        if np.sum(self.get_valid_moves(state)) == 0:
            return 0, True
        return 0, False
    
    def get_opponent(self, player):
        return -player
    
    def get_opponent_value(self, value):
        return -value
    
    def change_perspective(self, state, player):
        return state * player
    
    def get_encoded_state(self, state):
        #encoded_state is a matrix where each row represents whether each element in the state array is equal to -1, 0, or 1
        #, and each element is represented as a floating-point number (1.0 for True and 0.0 for False)
        encoded_state = np.stack(
            (state == -1, state == 0, state == 1)
        ).astype(np.float32)
        
        if len(state.shape) == 3:
            #swaps vertical and horizontal axes
            encoded_state = np.swapaxes(encoded_state, 0, 1)
        
        return encoded_state

    def convert_to_action(self, x, y):
        return self.column_count*y+x
    
class Chess:
    def __init__(self): #run when class is initiated
        self.row_count = 8
        self.column_count = 8
        self.action_size = 2*self.row_count * self.column_count
        self.king_moved = [0, 0]
        self.king_moved = [[0,0],[0,0]]
        self.pieces = [[8,8],[2,2],[2,2],[2,2],[1,1],[1,1]]
    
    def __repr__(self): #string representation of this class
        return "Chess"
    
    def get_initial_state(self):
        return np.array([[30,10,20,50,40,21,11,31], [1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[-30,-10,-20,-50,-40,-21,-11,-31]])
    
    def promote(piece, y1):
        if piece < 10 and piece > 0:
            if y1 == 7:
                return True
        if piece > -10 and piece < 0:
            if y1 == 0:
                return True
    
    def get_next_state(self, state, action):
        start_position = action // (self.column_count*self.row_count)
        end_position = (action - start_position) / (self.column_count*self.row_count)
        y0 = start_position // self.column_count
        x0 = start_position % self.column_count
        y1 = end_position // self.column_count
        x1 = end_position % self.column_count
        moved_piece = state[y0, x0]
        if(abs(moved_piece) == 50):
            if abs(x1 - x0) > 1:
                whichrook = int(math.copysign(30 + (x1 > 4), moved_piece))
                state[y1, state[y1].index(whichrook)] = 0
                state[y1, x1 + int(math.copysign(1, 4-x1))] = whichrook
            self.kingmoved[(moved_piece > 0)] = 1
        state[y0,x0] = 0
        if(abs(moved_piece) == 30 or abs(moved_piece) == 31):
            self.rookmoved[(moved_piece >0)][abs(moved_piece)-30] = 1
        if self.promote(moved_piece, y1):
            promoteto = 4
            state[y1,x1] = int(math.copysign(1, moved_piece))*(promoteto*10+self.pieces[promoteto][(moved_piece < 0)])
            self.pieces[promoteto][(moved_piece < 0)] += 1
        else:
            state[y1,x1] = moved_piece
        if enpassant >= 0 and x1*8+y1 == enpassant:
            state[y1-int(math.copysign(1, y1 - y0)),y1] = 0
        if abs(moved_piece) < 10 and abs(y1 - y0) > 1:
            enpassant = x1*8+y0+int(math.copysign(1, y1 - y0))
        else:
            enpassant = -1
        return state
    
    def get_valid_moves(self, state, player):
        #flattens state into a 1D array, and returns a new array of 8-bit unsigned integers 
        #where each element is 1 if the corresponding element in the original array was 0, and 0 otherwise
        valid_moves = []
        for y0 in range(self.row_count):
            for x0 in range(self.column_count):
                valid_moves += [[]]
                if state[y0,x0] != 0 and np.sign(state[y0,x0]) == np.sign(player):
                    for y1 in range(self.row_count):
                        for x1 in range(self.column_count):
                            #use of piecemove is problematic
                            if piecemove(state[y0,x0], y0, x0, y1, x1):
                                valid_moves[y0*self.column_count+x0] += [1]
                            else:
                                valid_moves[y0*self.column_count+x0] += [0]
                else:
                    for i in range(self.row_count*self.column_count):
                        valid_moves[y0*self.column_count+x0] += [0]
        valid_moves = np.array(valid_moves)
        return (valid_moves.reshape(-1)).astype(np.uint8)

    def check_win(self, state, action):
        if action == None:
            return False
        
        start_position = action // (self.column_count*self.row_count)
        y0 = start_position // self.column_count
        x0 = start_position % self.column_count

        player = np.sign(state[y0, x0])

        new_state = self.get_next_state(state, action)

        #check for checkmate        
    
class ResNet(nn.Module):
    def __init__(self, game, num_resBlocks, num_hidden, device):
        super().__init__() #initiates the parent class
        
        self.device = device
        self.startBlock = nn.Sequential(
            nn.Conv2d(3, num_hidden, kernel_size=3, padding=1), #convolutional layer(input channels, output channels, size of layer, match the input size with output size)
            nn.BatchNorm2d(num_hidden), #normalize output
            nn.ReLU()
        )
        
        #create a backbone for neural network by creating multiple Resblocks
        self.backBone = nn.ModuleList(
            [ResBlock(num_hidden) for i in range(num_resBlocks)]
        )
        
        self.policyHead = nn.Sequential(
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
        policy = self.policyHead(x)
        value = self.valueHead(x)
        return policy, value
        
        
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

import matplotlib.pyplot as plt

tictactoe = TicTacToe()

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

state = tictactoe.get_initial_state()
state = tictactoe.get_next_state(state, 2, -1)
state = tictactoe.get_next_state(state, 4, -1)
state = tictactoe.get_next_state(state, 6, 1)
state = tictactoe.get_next_state(state, 8, 1)


encoded_state = tictactoe.get_encoded_state(state)

tensor_state = torch.tensor(encoded_state, device=device).unsqueeze(0) #.unsqueeze ads an extra dimension

model = ResNet(tictactoe, 4, 64, device=device)
model.eval() #playing mode

policy, value = model(tensor_state)
value = value.item()
policy = torch.softmax(policy, axis=1).squeeze(0).detach().cpu().numpy()

print(value)

print(state)
print(tensor_state)

plt.bar(range(tictactoe.action_size), policy)
#plt.show()

class Node:
    def __init__(self, game, args, state, parent=None, action_taken=None, prior=0, visit_count=0):
        self.game = game
        self.args = args
        self.state = state
        self.parent = parent
        self.action_taken = action_taken
        self.prior = prior
        
        self.children = []
        
        self.visit_count = visit_count
        self.value_sum = 0
        
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
        return q_value + self.args['C'] * (math.sqrt(self.visit_count) / (child.visit_count + 1)) * child.prior
    
    def expand(self, policy):
        for action, prob in enumerate(policy):
            if prob > 0:
                child_state = self.state.copy()
                child_state = self.game.get_next_state(child_state, action, 1)
                child_state = self.game.change_perspective(child_state, player=-1)

                child = Node(self.game, self.args, child_state, self, action, prob)
                self.children.append(child)
                
        return child
            
    def backpropagate(self, value):
        self.value_sum += value
        self.visit_count += 1
        
        value = self.game.get_opponent_value(value)
        if self.parent is not None:
            self.parent.backpropagate(value)  


class MCTS:
    def __init__(self, game, args, model):
        self.game = game
        self.args = args
        self.model = model
        
    @torch.no_grad() #don't track gradients for backpropagation
    def search(self, state):
        root = Node(self.game, self.args, state, visit_count=1)
        
        policy, _ = self.model(
            torch.tensor(self.game.get_encoded_state(state), device=self.model.device).unsqueeze(0)
        )
        policy = torch.softmax(policy, axis=1).squeeze(0).cpu().numpy()
        policy = (1 - self.args['dirichlet_epsilon']) * policy + self.args['dirichlet_epsilon'] \
            * np.random.dirichlet([self.args['dirichlet_alpha']] * self.game.action_size)
        
        valid_moves = self.game.get_valid_moves(state)
        policy *= valid_moves
        policy /= np.sum(policy)
        root.expand(policy)
        
        for search in range(self.args['num_searches']):
            node = root
            
            while node.is_fully_expanded():
                node = node.select()
                
            value, is_terminal = self.game.get_value_and_terminated(node.state, node.action_taken)
            value = self.game.get_opponent_value(value)
            
            if not is_terminal:
                policy, value = self.model(
                    torch.tensor(self.game.get_encoded_state(node.state), device=self.model.device).unsqueeze(0)
                )
                policy = torch.softmax(policy, axis=1).squeeze(0).cpu().numpy()
                valid_moves = self.game.get_valid_moves(node.state)
                policy *= valid_moves
                policy /= np.sum(policy)
                
                value = value.item()
                
                node.expand(policy)
                
            node.backpropagate(value)    
            
            
        action_probs = np.zeros(self.game.action_size)
        for child in root.children:
            action_probs[child.action_taken] = child.visit_count
        action_probs /= np.sum(action_probs)
        return action_probs
    
class AlphaZero:
    def __init__(self, model, optimizer, game, args):
        self.model = model
        self.optimizer = optimizer
        self.game = game
        self.args = args
        self.mcts = MCTS(game, args, model)
        
    def selfPlay(self):
        memory = []
        player = 1
        state = self.game.get_initial_state()
        
        while True:
            neutral_state = self.game.change_perspective(state, player)
            action_probs = self.mcts.search(neutral_state)
            
            memory.append((neutral_state, action_probs, player))
            
            temperature_action_probs = action_probs ** (1 / self.args['temperature']) # Divide temperature_action_probs with its sum in case of an error
            temperature_action_probs /= np.sum(temperature_action_probs)
            action = np.random.choice(self.game.action_size, p=temperature_action_probs)
            
            state = self.game.get_next_state(state, action, player)
            
            value, is_terminal = self.game.get_value_and_terminated(state, action)
            
            if is_terminal:
                returnMemory = []
                for hist_neutral_state, hist_action_probs, hist_player in memory:
                    hist_outcome = value if hist_player == player else self.game.get_opponent_value(value)
                    returnMemory.append((
                        self.game.get_encoded_state(hist_neutral_state),
                        hist_action_probs,
                        hist_outcome
                    ))
                return returnMemory
            
            player = self.game.get_opponent(player)
                
    def train(self, memory):
        random.shuffle(memory)
        for batchIdx in range(0, len(memory), self.args['batch_size']):
            sample = memory[batchIdx:min(len(memory) - 1, batchIdx + self.args['batch_size'])] # Change to memory[batchIdx:batchIdx+self.args['batch_size']] in case of an error
            state, policy_targets, value_targets = zip(*sample)
            
            state, policy_targets, value_targets = np.array(state), np.array(policy_targets), np.array(value_targets).reshape(-1, 1)
            
            state = torch.tensor(state, dtype=torch.float32, device=self.model.device)
            policy_targets = torch.tensor(policy_targets, dtype=torch.float32, device=self.model.device)
            value_targets = torch.tensor(value_targets, dtype=torch.float32, device=self.model.device)
            
            out_policy, out_value = self.model(state)
            
            policy_loss = F.cross_entropy(out_policy, policy_targets)
            value_loss = F.mse_loss(out_value, value_targets)
            loss = policy_loss + value_loss
            
            self.optimizer.zero_grad()
            loss.backward()
            self.optimizer.step()
    
    def learn(self):
        for iteration in range(self.args['num_iterations']):
            memory = []
            
            self.model.eval()
            for selfPlay_iteration in trange(self.args['num_selfPlay_iterations']):
                memory += self.selfPlay()
                
            self.model.train()
            for epoch in trange(self.args['num_epochs']):
                self.train(memory)
            
            torch.save(self.model.state_dict(), f"model_{iteration}_{self.game}.pt")
            torch.save(self.optimizer.state_dict(), f"optimizer_{iteration}_{self.game}.pt")

    
game = TicTacToe()

tictactoe = TicTacToe()

model = ResNet(tictactoe, 4, 64, device=device)

optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

def learn(args):
    alphaZero = AlphaZero(model, optimizer, tictactoe, args)
    start_time = time.time()
    alphaZero.learn()
    print(f"learning time: {time.time()-start_time}s")

def play(args):
    #load previously learned values
    model.load_state_dict(torch.load("model_2_TicTacToe.pt", map_location=device))
    model.eval() #playing mode

    print("Game starts")
    tictactoe = TicTacToe()
    state = tictactoe.get_initial_state()
    print(state)

    while True:
        y = -1
        x = -1
        action = 0
        while(y <= 0 or y > tictactoe.row_count or x <= 0 or x > tictactoe.column_count):
            y = int(input("select row: "))
            x = int(input("select column: "))
            action = tictactoe.convert_to_action(x-1, y-1)

        state = tictactoe.get_next_state(state, action, -1)
        print(state)
        if tictactoe.get_value_and_terminated(state, action)[1]:
            if tictactoe.get_value_and_terminated(state, action)[0] == 1:
                print("You win!")
            else:
                print("tie")
            break

        policy = MCTS(tictactoe, args, model).search(state)
        valid_moves = tictactoe.get_valid_moves(state)
        policy *= valid_moves
        policy /= np.sum(policy)
        action = int(np.argmax(policy))
        state = tictactoe.get_next_state(state, action, 1)
        print(state)
        if tictactoe.get_value_and_terminated(state, action)[1]:
            if tictactoe.get_value_and_terminated(state, action)[0] == 1:
                print("You lose")
            else:
                print("tie")
            break